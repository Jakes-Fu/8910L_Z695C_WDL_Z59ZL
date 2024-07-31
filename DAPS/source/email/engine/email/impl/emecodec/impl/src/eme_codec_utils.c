/*=====================================================================================
FILE NAME   : eme_codec_utils.c
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File is support some way to implement encoding and decoding a whole email,
      the way will be transfer by function from eme_codec_mailmessage.c.
      These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
        20070307    wxs        01                create
        20070429    wxs        02                modify
        
                 
=====================================================================================*/

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#include "eme_file_api.h"

#include "eme_codec_def.h"
#include "eme_encode_base64.h"
#include "eme_codec_decode_bodystruct.h"
#include "eme_codec_mailmessage.h"
#include "eme_utils_api.h"
#include "eme_codec_utils_api.h"
#include "we_mime.h"
#include "we_csc.h"

/************************************************************************/
/* eme_codec_utils                                                     */
/************************************************************************/
#define EME_UTF8_MAX_ONE_CHAR_LEN   6
#define EME_UNKNOW_MIME             "application/octet-stream"


WE_UINT EME_ToSeconds(TimeType *pDate);
WE_INT32 EME_ToJulianDate(WE_UINT32 dwSecs, TimeType *pDate);
/*===================================================================
* 
*                    Functions implementation
* 
* ==================================================================*/
WE_INT32 EME_SetHeader(stEMSGHandle *pstInHandle)
{
   stEMSGHandle        *pstEmailHandle = pstInHandle;
   WE_INT32            iResult = EME_FAILED;
   TimeType            stJulianDateType;
   EmailOpt            stHeaderOpt = {0};
   void                *pvMemMgr = NULL;

   EME_UTILS_MEMSET(&stJulianDateType,0x00,sizeof(TimeType));

   EME_UTILS_LOG_INFO(("EME_SetHeader : START!!!!"));
   
   if (NULL == pstEmailHandle || NULL == pstEmailHandle->pvEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_SetHeader(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      /* From */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_FROM, 0, &stHeaderOpt);
      if (stHeaderOpt.pVal != NULL)
      {
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_FROM);
         EME_UTILS_BREAKIF(iResult,"EME_SetAddrField() set From failed!");
      }
      
      /* To */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_TO, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal)
      { 
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_TO);
         EME_UTILS_BREAKIF(iResult,"EME_SetAddrField() failed!");
      }

      /* Cc */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_CC, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal )
      {
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_CC);
         EME_UTILS_BREAKIF(iResult,"EME_SetAddrField(): set CC failed!");
      }
     
      /* Bcc */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_BCC, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal )
      {
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_BCC);
         EME_UTILS_BREAKIF(iResult,"EME_SetAddrField(): set Bcc failed!");
      }
      
      /* Subject */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_SUBJECT, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal)
      {
         iResult = EME_MailMsg_SetSubject(pstEmailHandle,pstEmailHandle->pvEmailMsg, (WE_CHAR*)stHeaderOpt.pVal);
         EME_UTILS_BREAKIF(iResult, "EME_SetHeader EME_MailMsg_SetSubject,failed!!!!");
      }
      
      /* Date */
  
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_DATE, 0, &stHeaderOpt);
      EME_UTILS_LOG_INFO(("EME_SetHeader:stHeaderOpt.pVal = %d ",  stHeaderOpt.pVal));
      if (NULL != stHeaderOpt.pVal)
      {
	     WE_UINT32 uiSecond = 0;
	     
         iResult = EME_ToJulianDate((WE_UINT32)stHeaderOpt.pVal, &stJulianDateType);
         EME_UTILS_LOG_INFO(("EME_SetHeader:stJulianDateType.DayIndex = %d ",  stJulianDateType.DayIndex));
	  EME_UTILS_LOG_INFO(("EME_SetHeader: stJulianDateType.nDay = %d ",  stJulianDateType.nDay));
	  EME_UTILS_LOG_INFO(("EME_SetHeader: stJulianDateType.nHour= %d ",  stJulianDateType.nHour));
	  EME_UTILS_LOG_INFO(("EME_SetHeader: stJulianDateType.nMin= %d ",  stJulianDateType.nMin));
	  EME_UTILS_LOG_INFO(("EME_SetHeader: stJulianDateType.nMonth = %d ",  stJulianDateType.nMonth));
	  EME_UTILS_LOG_INFO(("EME_SetHeader: stJulianDateType.nSec = %d ",  stJulianDateType.nSec));
         EME_UTILS_LOG_INFO(("EME_SetHeader:stJulianDateType.nYear = %d ",  stJulianDateType.nYear));
         EME_UTILS_BREAKIF(iResult, "EME_SetHeader EME_ToJulianDate,failed!!!!");
         
	 uiSecond = (WE_UINT32)stHeaderOpt.pVal;
         EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
         EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_TIME_ZONE, 0, &stHeaderOpt);
	  EME_UTILS_LOG_INFO(("EME_SetHeader:EMAIL_OPT_TIME_ZONE  = %d ",  stHeaderOpt.pVal));
/*	  
	stJulianDateType.DayIndex = 2;
       stJulianDateType.nDay = 14;
       stJulianDateType.nMonth = 5;
	stJulianDateType.nYear = 2008;
	stJulianDateType.nHour = 10;
	stJulianDateType.nMin = 20;
	stJulianDateType.nSec = 30;
	stHeaderOpt.pVal = (void*)8;*/
		
	  iResult = EME_MailMsg_SetDateTimeEx(pstEmailHandle, pstEmailHandle->pvEmailMsg,  uiSecond, (WE_INT16)((WE_UINT32)stHeaderOpt.pVal));
         //iResult = EME_MailMsg_SetDateTime(pstEmailHandle,
         //   pstEmailHandle->pvEmailMsg, 
        //    (WE_UINT16)(stJulianDateType.DayIndex + 1), 
        //    (WE_UINT16)stJulianDateType.nDay, 
        //    (WE_UINT16)stJulianDateType.nMonth, 
        //    (WE_UINT16)stJulianDateType.nYear,
        //    (WE_UINT16)stJulianDateType.nHour, 
        //    (WE_UINT16)stJulianDateType.nMin, 
       //     (WE_UINT16)stJulianDateType.nSec,
       //     (WE_INT16)(WE_INT32)stHeaderOpt.pVal);
         EME_UTILS_BREAKIF(iResult, "EME_SetHeader EME_MailMsg_SetDateTime,failed!!!!");
      }


      /* Reply-To */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_REPLY_TO, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal )
      {
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_REPLY_TO);
         EME_UTILS_BREAKIF(iResult,"EME_SetHeader reply to failed!");
      }

      /* Priority */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_PRIORITY, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal)
      {
         iResult = EME_SetPriority(pstEmailHandle,pstEmailHandle->pvEmailMsg, (WE_UINT32)stHeaderOpt.pVal);
         EME_UTILS_BREAKIF(iResult,"EME_SetHeaderPriority() failed!");
      }

     /* Disposition-Notification-To */
      EME_UTILS_MEMSET(&stHeaderOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstEmailHandle->pstHeaderOpts, EMAIL_OPT_DISP_NOTIFICATION_TO, 0, &stHeaderOpt);
      if (NULL != stHeaderOpt.pVal)
      { 
         iResult = EME_SetAddrField(pstEmailHandle, stHeaderOpt.pVal, EMAIL_OPT_DISP_NOTIFICATION_TO);
         EME_UTILS_BREAKIF(iResult,"EME_SetAddrField() failed!");
      }

   } while(0);

   
   if (EME_SUCCESS != iResult)
   {
   }

   return iResult;
}


WE_INT32 EME_SetOneBodyPart(stEMSGHandle *pstInHandle, WE_UINT32 uiIndex)
{
   stEMSGHandle *pstEmailHandle = pstInHandle;
   EmailOpt     stPartOpt = {0};
   WE_INT32     iResult = EME_SUCCESS;
   WE_VOID      *pvEmailMsg = NULL;
   WE_VOID      *pMsgPart = NULL;
   WE_VOID      *pvMemMgr = NULL;
   WE_UINT32    uiPartType = 0;
   WE_UINT32    uiSrcType = 0;
   WE_CHAR      *pcDataBuf = NULL;
   WE_CHAR      *pcAttchName = NULL;
   WE_CHAR      *pcCharSet = NULL;
   WE_CHAR      *pcTransEnc = NULL;
   WE_UINT32    uiDataSize = 0;
   EmailOpt     *pstOpts = NULL;
   St_MsgPartList *pstOnePart = NULL;

   WE_CHAR      *pcContentType = NULL;

   
   if (NULL == pstEmailHandle || NULL == pstEmailHandle->pvEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_SetOneBodyPart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {  
      pstOnePart = pstEmailHandle->pstPartList;
      while (uiIndex != 0 && pstOnePart != NULL)
      {
         pstOnePart = pstOnePart->pstNext;
         uiIndex--;         
      }
      if (pstOnePart == NULL)
      {
         return EME_FAILED;
      }

      pstOpts = pstOnePart->pstPartOpts;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_PART_TYPE, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get part type failed");
      uiPartType = (WE_UINT32)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_DATA_BUFF, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get data buf failed");
      pcDataBuf = (WE_CHAR*)stPartOpt.pVal;
      EME_UpdateOpt(pstOpts, EMAILPART_OPT_DATA_BUFF, NULL, NULL);

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_SRC_TYPE, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get src type failed");
      uiSrcType = (WE_UINT32)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPARAM_OPT_FILE_NAME, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get file name failed");
      pcAttchName = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_DATA_SIZE, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get data size failed");
      uiDataSize = (WE_UINT32)stPartOpt.pVal;
      
      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPARAM_OPT_CHARSET, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get file name failed");
      pcCharSet = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_TRANS_ENCODING, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "get file name failed");
      pcTransEnc = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_MIME_TYPE, uiIndex, &stPartOpt);
      pcContentType = (WE_CHAR*)stPartOpt.pVal;

      pvEmailMsg = pstEmailHandle->pvEmailMsg;

      switch((WE_UINT32)uiPartType)
      {
      case EMAILPART_PARTTYPE_TEXTPLAIN:
         iResult = EME_MailMsg_SetTextPlain(pstEmailHandle, pvEmailMsg, pcDataBuf, pcCharSet, pcTransEnc);
         break;

      case EMAILPART_PARTTYPE_TEXTHTML:
         iResult = EME_MailMsg_SetTextHtml(pstEmailHandle,pvEmailMsg, pcDataBuf, pcCharSet, pcTransEnc);
         break;
         
      case EMAILPART_PARTTYPE_ATTACHMENT:
         {
            WE_CHAR *pcMimeType    = NULL;
            WE_CHAR *pcSuffix      = NULL;

            iResult = EME_MailMsgPart_Create(pstEmailHandle, &pMsgPart);
            EME_UTILS_BREAKNULL(pMsgPart,
                                iResult,
                                EME_NOMEMORY,
                                "EME_SetOneBodyPart,EME_MailMsgPart_Create failed!!!");

            iResult = EME_MailMsgPart_SetName(pstEmailHandle,pMsgPart, pcAttchName);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPart,EME_MailMsgPart_SetName failed!!!!");

            iResult  = EME_MailMsgPart_SetFileName(pstEmailHandle,pMsgPart, pcAttchName);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPart,EME_MailMsgPart_SetFileName failed!!!!");
            
            pcSuffix = EME_UTILS_GETEXT(pcAttchName);
            if (pcSuffix != NULL)
            {
               /*pcMimeType malloc have not use our memory manager*/
               pcMimeType = (WE_CHAR *)WeMime_Ext2Mime(pcSuffix);
            }
            else if(pcContentType != NULL)
            {
               pcMimeType = EME_UTILS_STRDUP(NULL, pcContentType);
            }
            else
            {
               pcMimeType = EME_UTILS_STRDUP(NULL, EME_UNKNOW_MIME);
            }
            iResult  = EME_MailMsgPart_SetContType(pstEmailHandle,pMsgPart, pcMimeType);
            /*Free the Mime Type not by our memory manager*/
            EME_UTILS_FREEIF(NULL, pcMimeType);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPart,EME_MailMsgPart_SetContType failed!!!!");

            iResult  = EME_MailMsgPart_SetBodyData(pstEmailHandle,
                                                   pMsgPart, 
                                                   pcDataBuf,
                                                   uiDataSize);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPart,EME_MailMsgPart_SetBodyData failed!!!!");
            
            iResult = EME_MailMsg_AddMsgPart(pstEmailHandle,pvEmailMsg, pMsgPart);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPart,EME_MailMsg_AddMsgPart failed!!!!");
         }
         break;
         
      default:
         iResult =  EME_FAILED;
      }

   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcDataBuf);

   /*exceptional handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pMsgPart);
   }
   
   return iResult;
}

WE_INT32 EME_DecodeMsgPart(stEMSGHandle *pstInHandle, 
                           WE_CHAR* pMsgPartBuf,
                           WE_CHAR* charset, 
                           WE_CHAR* transerEncoding)
{  
   WE_INT32      iResult         = EME_SUCCESS; /*must be init EME_SUCCESS*/
   stEMSGHandle  *pstEmailHandle = pstInHandle;
   void          *pvMemMgr       = NULL;
   void          *pEecoded       = NULL;
   St_MsgPartList *pMsgPart  = NULL;
   St_MsgPartList *pstLast  = NULL;
   EmailOpt      *pstOneOpt = NULL;
   E_EncodeType   eEncodeType;
   E_Charset eCharset   = E_CHARSET_UTF8;
   WE_UINT32 uiOutSize  = 0;
   
   if (NULL == pMsgPartBuf || NULL == pstEmailHandle)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsgPart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;
   
   do 
   { 
      pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
      EME_UTILS_BREAKNULL(pMsgPart,
                          iResult,
                          EME_NOMEMORY,
                          "EME_CreateMsgPartList,failed!!");
      EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));

      pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
      EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
                          iResult,
                          EME_NOMEMORY,
                          "EME_CreateMsgPartList,failed!!");
      EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);

      eEncodeType = EME_MailMsg_ParseTransEncoding(transerEncoding);

      pstOneOpt = pMsgPart->pstPartOpts;
      
      if (NULL == charset)
      {
         /* Data size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (WE_VOID*)EME_Base64DecodeSize(EME_UTILS_STRLEN(pMsgPartBuf));
         pstOneOpt++;

         /* Data buf */
         pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;
         pstOneOpt->pVal = (WE_VOID*)EME_MailMsg_DecodeBody(pstEmailHandle,pMsgPartBuf, EME_UTILS_STRLEN(pMsgPartBuf), eEncodeType, NULL);
         pstOneOpt++;

      }
      else
      {
         WE_UINT32    uiStrLen = 0;
         /* Data buf */
         pEecoded = EME_MailMsg_DecodeBody(pstEmailHandle,pMsgPartBuf, EME_UTILS_STRLEN(pMsgPartBuf), eEncodeType, &uiStrLen);
         pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;

         eCharset = EME_MailMsg_ParseChaset(pEecoded, charset);
         EME_XXToUtf8(pstInHandle->pICSC,
                      pvMemMgr,
                      pEecoded,
                      uiStrLen,
                      eCharset,
                      (WE_CHAR**)&pstOneOpt->pVal,
                      &uiOutSize);           
         //pstOneOpt->pVal = EME_MailMsg_DecodeCharset(pstEmailHandle,pEecoded, charset);
         EME_UTILS_FREEIF(pvMemMgr,pEecoded);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                             iResult,
                             EME_FAILED,
                             "EME_DecodeMsgPart EME_MailMsg_DecodeCharset, get  charset option failed!!!!!!");
         
         pstOneOpt++;
         
         /* Data size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (WE_VOID*)uiStrLen;
         pstOneOpt++;
      }
      
      pstEmailHandle->uiPartsNum++;

         
      /* add msg part */
      if (pstEmailHandle->pstPartList == NULL)
      {
         pstEmailHandle->pstPartList = pMsgPart;
      }
      else
      {
         pstLast = pstEmailHandle->pstPartList;
         while (pstLast->pstNext != NULL)
         {
            pstLast = pstLast->pstNext;
         }

         pstLast->pstNext = pMsgPart;
      }
   } while(0);
   
   /*error handling*/
   if (EME_SUCCESS != iResult && NULL != pMsgPart)
   {
      EME_FreeAllOptsValue(pvMemMgr, pMsgPart->pstPartOpts);
      EME_UTILS_FREEIF(pvMemMgr, pMsgPart->pstPartOpts);
      EME_UTILS_FREEIF(pvMemMgr, pMsgPart);
   }
   
   return iResult;
}

WE_INT32 EME_GetHdFieldValue(stEMSGHandle *pstInHandle, WE_CHAR* pcIn,WE_CHAR** ppcOut)
{
   WE_CHAR* pcTmp     = NULL;
   WE_CHAR* pcTmpIn   = NULL;
   WE_CHAR* pcTmpOut  = NULL;
   WE_UINT32 uiNum    = 0;
   WE_INT32 iResult   = EME_SUCCESS; /*must be init success*/
   void *pvMemMgr     = NULL;
   
   if(NULL == pstInHandle || NULL == pcIn || NULL == ppcOut)
   {
      EME_UTILS_LOG_ERROR(("EME_GetHdFieldValue,bad input"));
      return EME_FAILED;
   }
   pvMemMgr = pstInHandle->pvMemMgr;
   do 
   {
      pcTmp = pcIn;
      /*judge have more address fields in pcIn, boundary is LFCR */
      while (*pcTmp != FLAG_END)
      {
         if (FLAG_D == *(pcTmp-1) && FLAG_A == *pcTmp)
         {
            /*totalize the number*/
            uiNum++;         
         }
         pcTmp++;
      }
      pcTmp = NULL;
      
      /*only one address*/
      if (uiNum == 0)
      {
         *ppcOut = EME_UTILS_STRDUP(pvMemMgr,pcIn);
         EME_UTILS_BREAKNULL(*ppcOut,
            iResult,
            EME_NOMEMORY,
            "EME_GetHdFieldValue malloc failed!!!");
      }

      /*have more address fields*/
      else
      {  
         pcTmpIn = pcIn;
         *ppcOut = (WE_CHAR*)EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcIn) + 1 - 2*uiNum);
         EME_UTILS_BREAKNULL(*ppcOut,
            iResult,
            EME_NOMEMORY,
            "EME_GetHdFieldValue malloc failed!!!");
         
         EME_UTILS_MEMSET(*ppcOut,0,EME_UTILS_STRLEN(pcIn) + 1 - 2*uiNum);
         
         pcTmpOut = *ppcOut;
         
         while (*pcTmpIn != FLAG_END)
         {
            if (FLAG_D == *(pcTmpIn) && FLAG_A == *(pcTmpIn + 1))
            {
               pcTmpIn = pcTmpIn + 2;
               continue;  
            }
            *pcTmpOut = *pcTmpIn;
            pcTmpIn++;
            pcTmpOut++;
         }
      }
       pcTmpOut = NULL;
       pcTmpIn  = NULL;

   } while(0);
   
   return iResult;
}

WE_INT32 EME_DecodeMsg_JustHd(stEMSGHandle *pstInHandle, WE_CHAR* pcEmailBuff,WE_BOOL bOnlyHasHd)
{
   stEMSGHandle      *pstEmailHandle        = pstInHandle;
   St_EmailMsg       *pstEmailMsg           = NULL;
   void              *pvMemMgr              = NULL;
   WE_INT16          timeZone               = 0;
   WE_INT32          iResult                = EME_SUCCESS; /*must be init SUCCESS;*/
   TimeType          stJulianDateType;

   EmailOpt          *pstOneOpt             = NULL;

   EME_UTILS_MEMSET(&stJulianDateType, 0, sizeof(TimeType));

   if ( NULL == pstEmailHandle || pstEmailHandle->pstHeaderOpts != NULL)
   {  
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustHd(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pstEmailMsg =(St_EmailMsg *)pstEmailHandle->pvEmailMsg;
   if ( NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustHd,email Message pointer is NULL!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      iResult = EME_MailMsg_ParseDataBuf(pstEmailHandle,(void *)pstEmailMsg, pcEmailBuff, bOnlyHasHd);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,EME_MailMsg_ParseDataBuf failed!!!!");

      pstEmailHandle->pstHeaderOpts = EME_UTILS_MALLOC(pvMemMgr, EME_MAX_HEAD_OPTS * sizeof(EmailOpt));
      EME_UTILS_BREAKNULL(pstEmailHandle->pstHeaderOpts,
                          iResult,
                          EME_NOMEMORY,
                          "no memory");
      EME_UTILS_MEMSET(pstEmailHandle->pstHeaderOpts, 0, EME_MAX_HEAD_OPTS * sizeof(EmailOpt));
      pstOneOpt = pstEmailHandle->pstHeaderOpts;

      /*add date to header opts*/
      if(NULL != pstEmailMsg->pcDate)
      {
         iResult = EME_MailMsg_GetDateTime((void *)pstEmailMsg, 
            &stJulianDateType,
            &timeZone);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,EME_MailMsg_GetDateTime  failed!!!!");
         
         /*Add Date to header opts*/
         pstOneOpt->iId = EMAIL_OPT_DATE;
         pstOneOpt->pVal = (WE_VOID*)EME_ToSeconds(&stJulianDateType);
         pstOneOpt++;
         
         /*Add Time Zone to header opts*/
         pstOneOpt->iId = EMAIL_OPT_TIME_ZONE;
         pstOneOpt->pVal = (void*)timeZone;
         pstOneOpt++;
      }

      /*Add From addr to header opts*/
      if (NULL != pstEmailMsg->pcFrom)
      {
         pstOneOpt->iId = EMAIL_OPT_FROM;
         pstOneOpt->pVal = (void*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsg->pcFrom);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal, iResult, EME_NOMEMORY, "no memory");

         pstOneOpt++;
      }
      
      /*Add To to header opts*/
      if (NULL!= pstEmailMsg->pcTo)
      {
         pstOneOpt->iId = EMAIL_OPT_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcTo,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse To addr failed!!!!");
            
         pstOneOpt++;
      }
     
      /*Add Cc to header opts*/
      if (NULL != pstEmailMsg->pcCc)
      {
         pstOneOpt->iId = EMAIL_OPT_CC;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcCc,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse Cc addr failed!!!!");
            
         pstOneOpt++;        
      }
      
      /*Add Bcc to header opts*/
      if (NULL!= pstEmailMsg->pcBcc)
      {
         pstOneOpt->iId = EMAIL_OPT_BCC;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcBcc,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse Bcc addr failed!!!!");
            
         pstOneOpt++;  
      }
     
      /*Add Subject to header opts*/
      if (NULL != pstEmailMsg->pcSubject)
      {
         pstOneOpt->iId = EMAIL_OPT_SUBJECT;
         pstOneOpt->pVal = (void *)EME_UTILS_STRDUP(pvMemMgr, pstEmailMsg->pcSubject);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal, iResult, EME_NOMEMORY, "no memory");
         
         pstOneOpt++;
      }
      
      /*Add Reply-To to header opts*/
      if (NULL != pstEmailMsg->pcReplyTo)
      {
         pstOneOpt->iId = EMAIL_OPT_REPLY_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle, pstEmailMsg->pcReplyTo, (WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse ReplyTo addr failed!!!!");
         
         pstOneOpt++;
      }
     
      /*Add X-Priority to header opts*/
      if (NULL != pstEmailMsg->pcPriority)
      {
         pstOneOpt->iId = EMAIL_OPT_PRIORITY;
         pstOneOpt->pVal = (WE_VOID*)EME_UTILS_ATOI((const WE_UCHAR*)pstEmailMsg->pcPriority);
         pstOneOpt++;
      }

      /*Add Disposition-Notification-To to header opts*/
      if (NULL != pstEmailMsg->pcNotifyTo)
      {
         pstOneOpt->iId = EMAIL_OPT_DISP_NOTIFICATION_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle, pstEmailMsg->pcNotifyTo, (WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse NotifyTo addr failed!!!!");
         
         pstOneOpt++;
      }
      
      pstOneOpt->iId = EMAILOPT_END;

   } while(0);
  
   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
   }
   
   return iResult;
}


WE_INT32 EME_DecodeBodyStruct(stEMSGHandle *pstInHandle, WE_CHAR* pBodyStructBuf)
{
   WE_INT32             iResult           = EME_SUCCESS;
   stEMSGHandle         *pstEmailHandle   = pstInHandle;
   void                 *pEmailBodyStruct = NULL;
   void                 *pvMemMgr         = NULL;
   St_BodyStructPart    *pBodyStructPart  = NULL;
   WE_UINT32            partNum           = 0;
   WE_UINT32            uiIndex           = 0;
   St_MsgPartList       *pMsgPart         = NULL;
   St_MsgPartList       *pstLast          = NULL;
   EmailOpt             *pstOneOpt        = NULL;
   
   if (NULL == pstEmailHandle || NULL == pBodyStructBuf)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeBodyStruct(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      iResult = EME_BodyStruct_Create(pstEmailHandle,&pEmailBodyStruct);
      EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Create(),failed!!!!!!!");
      EME_UTILS_BREAKNULL(pEmailBodyStruct,
                          iResult,
                          EME_NOMEMORY,
                          "EME_BodyStruct_Create(),malloc memory failed!!!!!");
      
      iResult = EME_BodyStruct_Parse(pstEmailHandle,pEmailBodyStruct, pBodyStructBuf);
      EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Parse(),failed!!!!");
      
      partNum = EME_BodyStruct_GetBdNum(pEmailBodyStruct);
      if (0 == partNum)
      {
         iResult = EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_DecodeBodyStruct,the body part number is NULL!!!!");
      }
      
      for(uiIndex = 0; uiIndex < partNum; uiIndex++)
      {
         pBodyStructPart = EME_BodyStruct_GetBdPart(pEmailBodyStruct, uiIndex);
         EME_UTILS_BREAKNULL(pBodyStructPart,
                             iResult,
                             EME_FAILED,
                             "EME_BodyStruct_GetBdPart,failed!!");
         
         pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
         EME_UTILS_BREAKNULL(pMsgPart,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));

         pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
         EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);

         pstOneOpt = pMsgPart->pstPartOpts;

         /* file name */
//          if (NULL != pBodyStructPart->pcAttachName)
//          {
//             pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
//             pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr, pBodyStructPart->pcAttachName);
//             EME_UTILS_BREAKNULL(pstOneOpt->pVal,
//                                 iResult,
//                                 EME_NOMEMORY,
//                                 "meomory strdup allocate faild!!!!!");
//             pstOneOpt++;
//          }
         
         /* part type */
         pstOneOpt->iId = EMAILPART_OPT_PART_TYPE;
         switch(pBodyStructPart->eBodyStructType)
         {
         case E_BODYS_TEXTPLAIN:
            pstOneOpt->pVal = (void*)EMAILPART_PARTTYPE_TEXTPLAIN;
            break;
            
         case E_BODYS_TEXTHTML:
            pstOneOpt->pVal = (void*)EMAILPART_PARTTYPE_TEXTHTML;
            break;
            
         case E_BODYS_RELATEDATTACH:
         case E_BODYS_ATTACHMENT:
            pstOneOpt->pVal = (void*)EMAILPART_PARTTYPE_ATTACHMENT;
            break;
            
         default:
            break;
         }
         pstOneOpt++;
     
         /* src type */
         pstOneOpt->iId = EMAILPART_OPT_SRC_TYPE;
         pstOneOpt->pVal = (void*)EMAILPART_SRCTYPE_BUFF;
         pstOneOpt++;

         /* date size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (void*)pBodyStructPart->uiBSPartSize;
         pstOneOpt++;

         /* content id */
         if (NULL != pBodyStructPart->pcContentId)
         {
            pstOneOpt->iId = EMAILPART_OPT_CONTENT_ID;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,pBodyStructPart->pcContentId);
            EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                                iResult,
                                EME_NOMEMORY,
                                "pMsgPart->msgPartOpts[3].pVal EME_UTILS_STRDUP failed!!!");
            pstOneOpt++;
         }

         /* section id */
         pstOneOpt->iId = EMAILPART_OPT_SECTION_ID;
         pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,pBodyStructPart->acSection);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                             iResult,
                             EME_NOMEMORY,
                             "pMsgPart->msgPartOpts[4].pVal EME_UTILS_STRDUP failed!!!");
         pstOneOpt++;


         /* char set */
         if (NULL != pBodyStructPart->pcCharset )
         {
            pstOneOpt->iId = EMAILPARAM_OPT_CHARSET;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,pBodyStructPart->pcCharset);
            EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                                iResult,
                                EME_NOMEMORY,
                                "pMsgPart->msgPartOpts[6].pVal EME_UTILS_STRDUP failed!!!");
            pstOneOpt++;
         }
         
         /* transfer encoding */
         if (NULL != pBodyStructPart->pcTransEncoding )
         {
            pstOneOpt->iId = EMAILPART_OPT_TRANS_ENCODING;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,pBodyStructPart->pcTransEncoding);
            EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                                iResult,
                                EME_NOMEMORY,
                                "pMsgPart->msgPartOpts[7].pVal EME_UTILS_STRDUP failed!!!");
            pstOneOpt++;
         }

         /* file name */
         if (NULL != pBodyStructPart->pcAttachName)
         {
            pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,pBodyStructPart->pcAttachName);
            EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                                iResult,
                                EME_NOMEMORY,
                                "pMsgPart->msgPartOpts[7].pVal EME_UTILS_STRDUP failed!!!");
            pstOneOpt++;
         }
         else if (pBodyStructPart->eBodyStructType != E_BODYS_TEXTPLAIN && pBodyStructPart->eBodyStructType != E_BODYS_TEXTHTML)
         {
            pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr,EME_DEFAULT_ATTACH_NAME);
            EME_UTILS_BREAKNULL(pstOneOpt->pVal,
                                iResult,
                                EME_NOMEMORY,
                                "pMsgPart->msgPartOpts[7].pVal EME_UTILS_STRDUP failed!!!");
            pstOneOpt++;
         }

         pstOneOpt->iId = EMAILOPT_END;

         pstEmailHandle->uiPartsNum++;
         
         if (pstEmailHandle->pstPartList == NULL)
         {
            pstEmailHandle->pstPartList = pMsgPart;
         }
         else
         {
            pstLast = pstEmailHandle->pstPartList;
            while (pstLast->pstNext != NULL)
            {
               pstLast = pstLast->pstNext;
            }

            pstLast->pstNext = pMsgPart;
         }

      }
      
   } while(0);
   
   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      if (NULL != pMsgPart)
      {
         EME_FreeAllOptsValue(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr,pMsgPart);
      }
      
   }
   
   EME_BodyStruct_Destroy(pstEmailHandle,&pEmailBodyStruct);
   
   return iResult;
}


WE_INT32 EME_DecodeEmailMsg(stEMSGHandle *pstInHandle, WE_CHAR* pcWholeMail)
{
   WE_INT32       iResult              = EME_SUCCESS; /*must be init to EME_SUCCESS*/
   stEMSGHandle  *pstEmailHandle       = pstInHandle;
   WE_CHAR       *pcTextPlain          = NULL;
   WE_CHAR       *pcTextHtml           = NULL;
   void          *pvMemMgr             = NULL;
   WE_UINT32      i                    = 0;
   WE_UINT32      iMsgPartNum          = 0;
   St_EmailMsg   *pstEmailMsg          = NULL;
   St_MsgPartList *pMsgPart            = NULL;
   St_MsgPartList *pstLast             = NULL;
   EmailOpt *pstOneOpt                 = NULL;
   E_EncodeType eEncoding              = E_ENCODETYPE_BASE64;
   WE_UINT32   uiSize                  = 0;


   if (NULL == pstEmailHandle || NULL == pcWholeMail || NULL == pstEmailHandle->pvEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeEmailMsg(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;
   pstEmailMsg = (St_EmailMsg *)pstEmailHandle->pvEmailMsg;

   do 
   { 
      iResult = EME_DecodeMsg_JustHd(pstEmailHandle, pcWholeMail,FALSE);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeEmailMsg,EME_DecodeMsg_JustHd failed!!!!");

      EME_MailMsg_GetPlainText(pstEmailHandle,pstEmailMsg, &pcTextPlain);
      if (NULL != pcTextPlain)
      {
         pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
         EME_UTILS_BREAKNULL(pMsgPart,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));

         pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
         EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);

         pstOneOpt = pMsgPart->pstPartOpts;
         
         /* part type */
         pstOneOpt->iId = EMAILPART_OPT_PART_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_PARTTYPE_TEXTPLAIN;
         pstOneOpt++;

         /* data size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRLEN(pcTextPlain);
         pstOneOpt++;

         /* data buf */
         pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;
         pstOneOpt->pVal = (WE_VOID*)pcTextPlain;
         pstOneOpt++;

         /* char set */
         if (pstEmailMsg->pcPlainCharset != NULL)
         {
            pstOneOpt->iId = EMAILPARAM_OPT_CHARSET;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsg->pcPlainCharset);
            pstOneOpt++;
         }

         /* transfer encoding */
         if (pstEmailMsg->pcPlainTransEnc != NULL)
         {
            pstOneOpt->iId = EMAILPART_OPT_TRANS_ENCODING;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr, pstEmailMsg->pcPlainTransEnc);
            pstOneOpt++;
         }

         /* src type */
         pstOneOpt->iId = EMAILPART_OPT_SRC_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_SRCTYPE_BUFF;
         pstOneOpt++;

         pstOneOpt->iId = EMAILOPT_END;
         
         pstEmailHandle->uiPartsNum++;

         /* add msg part */
         if (pstEmailHandle->pstPartList == NULL)
         {
            pstEmailHandle->pstPartList = pMsgPart;
         }
         else
         {
            pstLast = pstEmailHandle->pstPartList;
            while (pstLast->pstNext != NULL)
            {
               pstLast = pstLast->pstNext;
            }

            pstLast->pstNext = pMsgPart;
         }
      }

      EME_MailMsg_GetHtmlText(pstEmailHandle,pstEmailMsg, &pcTextHtml);
      if (NULL != pcTextHtml)
      {
         pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
         EME_UTILS_BREAKNULL(pMsgPart,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));

         pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
         EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);

         pstOneOpt = pMsgPart->pstPartOpts;
         
         /* part type */
         pstOneOpt->iId = EMAILPART_OPT_PART_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_PARTTYPE_TEXTHTML;
         pstOneOpt++;

         /* data size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRLEN(pcTextHtml);
         pstOneOpt++;

         /* data buf */
         pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;
         pstOneOpt->pVal = (WE_VOID*)pcTextHtml;
         pstOneOpt++;

         /* char set */
         if (pstEmailMsg->pcHtmlCharset != NULL)
         {
            pstOneOpt->iId = EMAILPARAM_OPT_CHARSET;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsg->pcHtmlCharset);
            pstOneOpt++;
         }

         /* transfer encoding */
         if (pstEmailMsg->pcHtmlTransEnc != NULL)
         {
            pstOneOpt->iId = EMAILPART_OPT_TRANS_ENCODING;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsg->pcHtmlTransEnc);
            pstOneOpt++;
         }

         /* src type */
         pstOneOpt->iId = EMAILPART_OPT_SRC_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_SRCTYPE_BUFF;
         pstOneOpt++;

         pstOneOpt->iId = EMAILOPT_END;
         
         pstEmailHandle->uiPartsNum++;
         
         /* add msg part */
         if (pstEmailHandle->pstPartList == NULL)
         {
            pstEmailHandle->pstPartList = pMsgPart;
         }
         else
         {
            pstLast = pstEmailHandle->pstPartList;
            while (pstLast->pstNext != NULL)
            {
               pstLast = pstLast->pstNext;
            }

            pstLast->pstNext = pMsgPart;
         }
      }

      iMsgPartNum = pstEmailMsg->uiEMsgPartNum;
      for (i = 0; i < iMsgPartNum ; i++)
      {
         St_EmailMsgPart *pstEmailMsgPart = NULL;

         pstEmailMsgPart = EME_MailMsg_GetOnePartByIndex(pstEmailHandle->pvEmailMsg, i);
         EME_UTILS_BREAKNULL(pstEmailMsgPart,
                             iResult,
                             EME_FAILED,
                             "EME_DecodeEmailMsg, EME_MailMsg_GetOnePartByIndex failed!!!");

         pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
         EME_UTILS_BREAKNULL(pMsgPart,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));

         pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
         EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
                             iResult,
                             EME_NOMEMORY,
                             "EME_CreateMsgPartList,failed!!");
         EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);

         pstOneOpt = pMsgPart->pstPartOpts;
         
         /* part type */
         pstOneOpt->iId = EMAILPART_OPT_PART_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_PARTTYPE_ATTACHMENT;
         pstOneOpt++;

         /* data buf */
         eEncoding = EME_MailMsg_ParseTransEncoding(pstEmailMsgPart->pcTransEnc);
         pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;
         pstOneOpt->pVal = EME_MailMsg_DecodeBody(pstEmailHandle, 
                                                  pstEmailMsgPart->pcOnePartData,
                                                  pstEmailMsgPart->uiOnePartSize,
                                                  eEncoding,
                                                  &uiSize);
         pstOneOpt++;

         /* data size */
         pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
         pstOneOpt->pVal = (WE_VOID*)uiSize;
         pstOneOpt++;

         /* content type */
//          if (pstEmailMsgPart->pcContentType != NULL)
//          {
//             pstOneOpt->iId = EMAILPART_OPT_MIME_TYPE;
//             pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcContentType);
//             pstOneOpt++;
//          }

         /* content id */
         if (pstEmailMsgPart->pcContentId != NULL)
         {
            pstOneOpt->iId = EMAILPART_OPT_CONTENT_ID;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcContentId);
            pstOneOpt++;
         }

         /* content loc */
//          if (pstEmailMsgPart->pcContentLoc != NULL)
//          {
//             pstOneOpt->iId = EMAILPART_OPT_CONTENT_LOC;
//             pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcContentLoc);
//             pstOneOpt++;
//          }

         /* transfer encoding */
         if (pstEmailMsgPart->pcTransEnc != NULL)
         {
            pstOneOpt->iId = EMAILPART_OPT_TRANS_ENCODING;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcTransEnc);
            pstOneOpt++;
         }

         /* file name */
         if (pstEmailMsgPart->pcName != NULL)
         {
            pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcName);
            pstOneOpt++;
         }
         else
         {
            pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
            pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr, EME_DEFAULT_ATTACH_NAME);
            pstOneOpt++;
         }
         
         /* src type */
         pstOneOpt->iId = EMAILPART_OPT_SRC_TYPE;
         pstOneOpt->pVal = (WE_VOID*)EMAILPART_SRCTYPE_BUFF;
         pstOneOpt++;

         pstOneOpt->iId = EMAILOPT_END;
         
         pstEmailHandle->uiPartsNum++;
         
         /* add msg part */
         if (pstEmailHandle->pstPartList == NULL)
         {
            pstEmailHandle->pstPartList = pMsgPart;
         }
         else
         {
            pstLast = pstEmailHandle->pstPartList;
            while (pstLast->pstNext != NULL)
            {
               pstLast = pstLast->pstNext;
            }

            pstLast->pstNext = pMsgPart;
         }
      }

   } while(0);
   
   if (iResult != EME_SUCCESS)
   {
      if (pMsgPart != NULL)
      {
         EME_FreeAllOptsValue(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart);
      }
   }


   return iResult;
}

WE_INT32 EME_SetAddrField(stEMSGHandle *pstInHandle, WE_CHAR* pcAddrList, WE_INT32 iEmailOptID)
{
	stEMSGHandle  *pstEmailHandle = pstInHandle;
	void          *pvMemMgr       = NULL;
	WE_CHAR       *pcField         = NULL;  
	WE_CHAR       *pNextAddr      = NULL;  
	
	WE_INT32 iResult = EME_SUCCESS; /*must be init success*/
	
	if (NULL == pcAddrList || NULL == pstEmailHandle || iEmailOptID < EMAILOPT_END )
	{
		EME_UTILS_LOG_ERROR(("EME_SetAddrField(),input parameters is invalid!!!!"));
		return EME_FAILED;
	}
	pvMemMgr = pstEmailHandle->pvMemMgr;
	do 
	{
		pcField = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcAddrList) + 1);
		EME_UTILS_BREAKNULL(pcField,
			iResult,
			EME_NOMEMORY,
			"EME_SetAddrField malloc failed!!!");
		EME_UTILS_MEMSET(pcField,0x00,EME_UTILS_STRLEN(pcAddrList) + 1);
		EME_UTILS_MEMCPY(pcField, pcAddrList, EME_UTILS_STRLEN(pcAddrList)+1);
		
		pNextAddr = pcField;
		
		while(*pNextAddr)
		{
			char* pcName = NULL;
			char* pcAddr = NULL;
			
			if(*pNextAddr == ' ' || *pNextAddr == '\t' || *pNextAddr == ';' || *pNextAddr == ',')
			{
				pNextAddr++;
				continue;
			}
			
			pcAddr = pNextAddr;
			while(*pNextAddr && *pNextAddr != ',' && *pNextAddr != ';')
			{
				//process email
				if(*pNextAddr == '\"')
				{
					*pNextAddr = 0;
					pcName = ++pNextAddr;
					while(*pNextAddr)
					{
						if(*pNextAddr == '\"')
						{
							*pNextAddr = 0;
							break;
						}
						pNextAddr++;
					}
				}
				
				//process email
				if(*pNextAddr == '<')
				{
					*pNextAddr = 0;
					if(NULL == pcName)
					{
						pcName = pcAddr;
						if(*(pcName + EME_UTILS_STRLEN(pcName)) == ' ')
						{
							*(pcName + EME_UTILS_STRLEN(pcName)) = 0;
						}
					}

					pcAddr = ++pNextAddr;
					while(*pNextAddr)
					{
						if(*pNextAddr == '>')
						{
							*pNextAddr = 0;
							break;
						}
						pNextAddr++;
					}
				}
				
				pNextAddr++;
			}
			//select one address to append
			if(NULL != pcAddr)
			{
				char * pcAt= EME_UTILS_STRCHR(pcAddr, '@');
				char *pcPoint = NULL;
				WE_CHAR orgchr =  *pNextAddr;
				*pNextAddr = 0;				

				if(NULL == pcAt)
				{
					iResult = EME_INVALID_EMAIL_ADDR;
					break;
				}
				
				pcPoint = EME_UTILS_STRCHR(pcAt, '.');
				if(NULL == pcPoint)
				{
					iResult = EME_INVALID_EMAIL_ADDR;
					break;
				}
				
				switch(iEmailOptID)
				{   
				case EMAIL_OPT_FROM:
					iResult = EME_MailMsg_SetFrom(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
				case EMAIL_OPT_TO:
					iResult = EME_MailMsg_AppendTo(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
				case EMAIL_OPT_CC:
					iResult = EME_MailMsg_AppendCc(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
				case EMAIL_OPT_BCC:
					iResult = EME_MailMsg_AppendBcc(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
				case EMAIL_OPT_REPLY_TO:
					EME_MailMsg_SetReplyTo(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
				case EMAIL_OPT_DISP_NOTIFICATION_TO:
					iResult = EME_MailMsg_AppendNotifyTo(pstEmailHandle,pstEmailHandle->pvEmailMsg, pcAddr, pcName);
					break;
					
				default:
					break;
				}
				*pNextAddr = orgchr;
			}
			else
			{
				iResult = EME_INVALID_EMAIL_ADDR;
				break;
			}
		}
		
   } while(0);
   
   /*error handling */
   EME_UTILS_FREEIF(pvMemMgr,pcField);
   
   EME_UTILS_LOG_INFO(("EME_SetAddrField : end return = %d", iResult));
   return iResult;
}


// WE_INT32 EME_GetMsgOpts(EmailOpt *pstOpts, WE_INT32 iOptId, EmailOpt *pstOptOut)
// {
//    EmailOpt   *pstOneOpt = NULL;
//    if (NULL == pstOpts || NULL == pstOptOut)
//    {
//       EME_UTILS_LOG_ERROR(("EME_GetMsgOpts(),input parameters is invalid!!!!"));
//       return EME_FAILED;
//    }
// 
//    pstOneOpt = pstOpts;
//    while (EMAILOPT_END != pstOneOpt->iId)
//    {
//       if (pstOneOpt->iId == iOptId)
//       {
//          pstOptOut->iId = pstOneOpt->iId;
//          pstOptOut->pVal = pstOneOpt->pVal;
//          return EME_SUCCESS;
//       }
// 
//       pstOneOpt ++;
//    }
//    
//    /*exceptional handling*/
//    pstOptOut->iId = iOptId;
//    pstOptOut->pVal = NULL;
// 
//    return EME_FAILED;
// }


WE_INT32 EME_FreeAllOptsValue(WE_VOID *pvMemHdr ,EmailOpt *pstFreeOpts)
{
   EmailOpt      *pstOneOpt = NULL;
   WE_INT32       iRet = EME_SUCCESS;

   if (pstFreeOpts == NULL)
   {
      return EME_SUCCESS;
   }

   pstOneOpt = pstFreeOpts;
   while (pstOneOpt->iId != EMAILOPT_END)
   {
      if ((pstOneOpt->iId > EMAILOPT_STR_START) && (pstOneOpt->iId < EMAILOPT_STR_END))
      {
        // DBGPRINTF("%x", pstOneOpt->iId);
         EME_UTILS_FREEIF(pvMemHdr, pstOneOpt->pVal);        
      }
      pstOneOpt++;
   }

   return iRet;
}

EmailOpt* EME_CopyOpts(WE_VOID* pvMemHdr, EmailOpt *pstOrigOpts)
{
   WE_INT32    iOptNum = 0;
   EmailOpt      *pstNewOpts = NULL;
   EmailOpt      *pstOneOpt = NULL;
   EmailOpt      *pstOptCopy = NULL;
   WE_INT32    iRet = EME_SUCCESS;

   if (pstOrigOpts == NULL)
   {
      return NULL;
   }

   do 
   {
      /* calculate opts numbers */
      pstOneOpt = pstOrigOpts;
      while (pstOneOpt->iId != EMAILOPT_END)
      {
         iOptNum++;
         pstOneOpt++;
      }
      

      pstNewOpts = (EmailOpt*)EME_UTILS_MALLOC(pvMemHdr, (iOptNum + 1) * sizeof(EmailOpt));
      EME_UTILS_BREAKNULL(pstNewOpts, iRet, EME_NOMEMORY, "EME_CopyOpts, no memory");
      EME_UTILS_MEMSET(pstNewOpts, 0, (iOptNum + 1) * sizeof(EmailOpt));

      /* copy opts */
      pstOneOpt = pstOrigOpts;
      pstOptCopy = pstNewOpts;
      for (; iOptNum > 0; iOptNum--, pstOneOpt++, pstOptCopy++)
      {
         pstOptCopy->iId = pstOneOpt->iId;

         /* str opts */
         if (pstOneOpt->iId > EMAILOPT_STR_START && pstOneOpt->iId < EMAILOPT_STR_END)
         {
            if (pstOneOpt->pVal == NULL)
            {
               iRet = EME_FAILED;
               EME_UTILS_BREAK(iRet, "EME_CopyOpts, invalid opt val");
            }
            pstOptCopy->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemHdr, (WE_CHAR*)pstOneOpt->pVal);
            EME_UTILS_BREAKNULL(pstOptCopy->pVal, iRet, EME_NOMEMORY, "EME_CopyOpts, no memory");
         }
         /* int opts */
         else
         {
            pstOptCopy->pVal = pstOneOpt->pVal;
         }
      }
      pstOptCopy->iId = EMAILOPT_END;

      EME_UTILS_BREAKIF(iRet, "EME_CopyOpts, copy opt failed");

      

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_FreeAllOptsValue(pvMemHdr, pstNewOpts);
      EME_UTILS_FREEIF(pvMemHdr, pstNewOpts);
   }

   return pstNewOpts;
}


WE_INT32 EME_GetOptsByIndex(EmailOpt *pstAllOpts, 
                            WE_INT32 iOptId, 
                            WE_INT32 iIndex, 
                            EmailOpt *pstOpt)
{      
   EmailOpt       *pstCurrentOpt = NULL;
   WE_INT32       iNum = 0;
   
   if (NULL == pstAllOpts || NULL == pstOpt || iIndex < 0)
   {
      EME_UTILS_LOG_ERROR(("EME_GetOptsByIndex(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pstCurrentOpt = pstAllOpts;
   while (pstCurrentOpt->iId != EMAILOPT_END)
   {
      /*If the opt is the one to be got*/
      if (pstCurrentOpt->iId == iOptId)
      {
         /*If is the right index*/
         if (iNum == iIndex)
         {
            /*Set opt-id*/
            pstOpt->iId = pstCurrentOpt->iId;
            /*Get the opt-value type*/
            pstOpt->pVal = pstCurrentOpt->pVal;
            return EME_SUCCESS;
         }
         
         iNum++;
      }
      pstCurrentOpt++;
   }
   
   return EME_FAILED;
}

WE_INT32 EME_UpdateOpt(EmailOpt *pstAllOpts, WE_INT32 iId, WE_VOID* pvVal, WE_VOID** ppvOrigVal)
{
   EmailOpt    *pstOneOpt = NULL;

   if (pstAllOpts == NULL)
   {
      return EME_FAILED;
   }

   pstOneOpt = pstAllOpts;
   while (pstOneOpt->iId != EMAILOPT_END)
   {
      if (pstOneOpt->iId == iId)
      {
         if (ppvOrigVal != NULL)
         {
            *ppvOrigVal = pstOneOpt->pVal;
         }
         pstOneOpt->pVal = pvVal;
         return EME_SUCCESS;
      }

      pstOneOpt++;
   }

   return EME_FAILED;
}

WE_INT32 EME_FreeMsgPartList(WE_VOID* pvMemHdr, St_MsgPartList *pstPartList)
{
   St_MsgPartList *pstFree = NULL;
   St_MsgPartList *pstTmp = NULL;

   pstFree = pstPartList;
   
   while (pstFree != NULL)
   {
      EME_FreeAllOptsValue(pvMemHdr, pstFree->pstPartOpts);
      EME_UTILS_FREEIF(pvMemHdr, pstFree->pstPartOpts);
      
      pstTmp = pstFree;
      pstFree = pstFree->pstNext;
      
      EME_UTILS_FREEIF(pvMemHdr, pstTmp);
   }
   
   return EME_SUCCESS;
}


WE_INT32 EME_ToJulianDate(WE_UINT32 dwSecs, TimeType *pDate)
{
    TimeType stJulianType;
//   EME_UTILS_MEMSET(&stJulianType,0x00,sizeof(JulianType));
//   
//   if ( NULL == pDate)
//   {
//      EME_UTILS_LOG_ERROR(("EME_ToJulianDate(),input parameters is invalid!!!!"));
//      return EME_FAILED;
//   }
//
//   GETJULIANDATE(dwSecs, &stJulianType);
	 if (NULL == pDate)
	 {
		 return EME_BADPARM;
	 }

	EME_UTILS_GETJULIANDATE(dwSecs, &stJulianType);
   pDate->nDay     = stJulianType.nDay;
   pDate->nHour    = stJulianType.nHour;
   pDate->nMin     = stJulianType.nMin;
   pDate->nMonth   = stJulianType.nMonth;
   pDate->nSec     = stJulianType.nSec;
   pDate->nYear    = stJulianType.nYear;
   pDate->DayIndex = stJulianType.DayIndex;

   return EME_SUCCESS;
}


WE_UINT EME_ToSeconds(TimeType *pDate)
{
	return  EME_UTILS_JULIANTOSECONDS(pDate);
}


WE_INT32 EME_DecodeMsg_JustPart(stEMSGHandle *pstInHandle,EmailOpt *pstOpt,WE_CHAR *pcPartBuf)
{
   WE_INT32     iResult         = EME_FAILED;
   stEMSGHandle *pstEmailHandle = pstInHandle;
   EmailOpt     optCharset;
   EmailOpt     optEncoding;

   EME_UTILS_MEMSET(&optCharset,0x00,sizeof(EmailOpt));
   EME_UTILS_MEMSET(&optEncoding,0x00,sizeof(EmailOpt));

   if ( NULL == pstEmailHandle || NULL == pstOpt || NULL == pcPartBuf)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustPart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   do 
   {
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPARAM_OPT_CHARSET, 0, &optCharset);
      EME_UTILS_BREAKIF(iResult, "get charset failed");

      iResult = EME_GetOptsByIndex(pstOpt, EMAILPART_OPT_TRANS_ENCODING, 0, &optEncoding);
      EME_UTILS_BREAKIF(iResult, "get transfer encoding failed");

      iResult = EME_DecodeMsgPart(pstEmailHandle, pcPartBuf, (WE_CHAR *)optCharset.pVal, (WE_CHAR *)optEncoding.pVal);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustPart,EME_DecodeMsgPart failed!!!!");
   
   } while(0);
  
   return iResult;
}

WE_CHAR* EME_GetCharSet(void *pvMemMgr, E_Charset eCharset)
{
   WE_CHAR *pcRet = NULL;

   switch(eCharset)
   {
   case E_CHARSET_US_ASCII:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_US_ASCII);
      break;
   case E_CHARSET_ISO_8859_1:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_ISO_8859_1);
      break;
   case E_CHARSET_UTF8:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_UTF8);
      break;
   case E_CHARSET_UTF16:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_UTF16);
      break;
   case E_CHARSET_GB2312:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_GB2312);
      break;
   case E_CHARSET_BIG5:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_BIG5);
      break;
   case E_CHARSET_HZ_GB_2312:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_HZ_GB_2312);
      break;
   case E_CHARSET_GB18030:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_GB18030);
      break;
   case E_CHARSET_GBK:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_GBK);
      break;
   default:
      /* ??? */
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_CHARSET_UTF8);
      break;
   }
   return pcRet;
}

WE_CHAR* EME_GetTransEncoding(void *pvMemMgr, E_EncodeType eEncondingType )
{
   WE_CHAR *pcRet = NULL;
   switch(eEncondingType)
   {
   case E_ENCODETYPE_7BIT :
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_TRANS_ENC_7BIT);
      break;
   case E_ENCODETYPE_8BIT:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_TRANS_ENC_8BIT);
      break;
   case E_ENCODETYPE_BASE64:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_TRANS_ENC_BASE64);
      break;
   case E_ENCODETYPE_QUOTEDPRINTABLE:
      pcRet = EME_UTILS_STRDUP(pvMemMgr,EME_TRANS_ENC_QUOTED_PRINTABLE);
      break;
   case E_ENCODETYPE_UUENCODE:
      pcRet = EME_UTILS_STRDUP(pvMemMgr, EME_TRANS_ENC_UUENCODE);
      break;
   case E_ENCODETYPE_BINHEX:
      pcRet = EME_UTILS_STRDUP(pvMemMgr, EME_TRANS_ENC_BINHEX);
      break;
   default:
      /* ??? */
      pcRet = NULL;
      break;
   }
   return pcRet;
}


WE_INT32 EME_Utf8ToXX(void* pICSC, 
                      void *pvMemMgr, 
                      WE_CHAR *pcInBuf, 
                      WE_UINT32 uiInSize,
                      E_Charset eCharset, 
                      WE_VOID** ppvOutBuf,
                      WE_UINT32 *puiOutSize)
{
   WE_INT32                iRet = EME_SUCCESS;
   Fn_We_Charset_Convert   *pfn = NULL;
   WE_CHAR                 *pcOut = NULL;
   WE_INT32                iCharSet = WE_CSC_UNKNOWN;
   WE_UINT32               uiOutSize = 0x7fffffff;

   if (pICSC == NULL || pcInBuf == NULL || ppvOutBuf == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      switch(eCharset)
      {
      case E_CHARSET_GB18030:
         iCharSet = WE_CSC_GB18030;
         break;
      case E_CHARSET_GB2312:
         iCharSet = WE_CSC_GB2312;
         break;
      case E_CHARSET_BIG5:
         iCharSet = WE_CSC_BIG5;
         break;
      case E_CHARSET_ISO_8859_1:
         iCharSet = WE_CSC_ISO_8859_1;
         break;
      case E_CHARSET_UTF16:
         iCharSet = WE_CSC_UTF_16;
         break;      
      case E_CHARSET_US_ASCII:
         iCharSet = WE_CSC_ASCII;
         break;
      case E_CHARSET_GBK:
         iCharSet = WE_CSC_GBK;
         break;
      case E_CHARSET_UTF8:
         *ppvOutBuf = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr, pcInBuf);
         if (*ppvOutBuf == NULL)
         {
            return EME_FAILED;
         }
         if (puiOutSize != NULL)
         {
            *puiOutSize = EME_UTILS_STRLEN(*ppvOutBuf);
         }
         return EME_SUCCESS;

      case E_CHARSET_HZ_GB_2312:
      default:
         return EME_FAILED;
      }

      pfn = WE_FINDFUNCTIONOFCSC(pICSC, WE_CSC_UTF_8, iCharSet);
      EME_UTILS_BREAKNULL(pfn, iRet, EME_FAILED, "found function failed");
   
      if (TRUE != pfn(pICSC, pcInBuf, (WE_LONG*)&uiInSize, NULL, (WE_LONG*)&uiOutSize))
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "csc get size failed");
      }

      pcOut = EME_UTILS_MALLOC(pvMemMgr, uiOutSize + 1);
      EME_UTILS_BREAKNULL(pcOut, iRet, EME_NOMEMORY, "no memory");
      EME_UTILS_MEMSET(pcOut, 0, uiOutSize + 1);

      if (TRUE != pfn(pICSC, pcInBuf, (WE_LONG*)&uiInSize, pcOut, (WE_LONG*)&uiOutSize))
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "csc parse failed");
      }

      *ppvOutBuf = (WE_VOID*)pcOut;
      if (puiOutSize != NULL)
      {
         *puiOutSize = uiOutSize;
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pcOut);
   }

   return iRet;
}



WE_INT32 EME_XXToUtf8(void* pICSC, 
                      void *pvMemMgr, 
                      WE_VOID *pvInBuf, 
                      WE_UINT32 uiInSize,
                      E_Charset eCharset, 
                      WE_CHAR** ppcOutBuf,
                      WE_UINT32 *puiOutSize)
{
   WE_INT32                iRet = EME_SUCCESS;
   Fn_We_Charset_Convert   *pfn = NULL;
   WE_CHAR                 *pcOut = NULL;
   WE_INT32                iCharSet = WE_CSC_UNKNOWN;
   WE_UINT32               uiSizeBack = uiInSize;
   WE_UINT32               uiOutSize = 0x7fffffff;

   if (pICSC == NULL || pvInBuf == NULL || ppcOutBuf == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      switch(eCharset)
      {
      case E_CHARSET_GB18030:
         iCharSet = WE_CSC_GB18030;
         break;
      case E_CHARSET_GB2312:
         iCharSet = WE_CSC_GB2312;
         break;
      case E_CHARSET_BIG5:
         iCharSet = WE_CSC_BIG5;
         break;
      case E_CHARSET_ISO_8859_1:
         iCharSet = WE_CSC_ISO_8859_1;
         break;
      case E_CHARSET_UTF16:
         iCharSet = WE_CSC_UTF_16;
         break;
      case E_CHARSET_US_ASCII:
         iCharSet = WE_CSC_ASCII;
         break;
      case E_CHARSET_GBK:
         iCharSet = WE_CSC_GBK;
         break;
      case E_CHARSET_KOI8R:
         iCharSet = WE_CSC_KOI8_R;
         break;
      case E_CHARSET_WIN1251:
         iCharSet = WE_CSC_WINDOWS_1251;
         break;
      case E_CHARSET_UTF8:
         *ppcOutBuf = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)pvInBuf);
         if (*ppcOutBuf == NULL)
         {
            return EME_FAILED;
         }
         if (puiOutSize != NULL)
         {
            *puiOutSize = EME_UTILS_STRLEN(*ppcOutBuf);
         }
         return EME_SUCCESS;

      case E_CHARSET_HZ_GB_2312:
      default:
         return EME_FAILED;
      }
    
      EME_UTILS_LOG_INFO(("EME_XXToUtf8: charset is %d", iCharSet));

      pfn = WE_FINDFUNCTIONOFCSC(pICSC, iCharSet, WE_CSC_UTF_8);
      EME_UTILS_BREAKNULL(pfn, iRet, EME_FAILED, "found function failed");
   
//      EME_UTILS_LOG_INFO(("EME_XXToUtf8: input char =%s", pvInBuf));
      EME_UTILS_LOG_INFO(("EME_XXToUtf8: input char size =%d", uiInSize));
      if (TRUE != pfn(pICSC, (WE_CHAR*)pvInBuf, (WE_LONG*)&uiInSize, NULL, (WE_LONG*)&uiOutSize))
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "csc get size failed");
      }

      pcOut = EME_UTILS_MALLOC(pvMemMgr, uiOutSize + 1);
      EME_UTILS_BREAKNULL(pcOut, iRet, EME_NOMEMORY, "no memory");
      EME_UTILS_MEMSET(pcOut, 0, uiOutSize + 1);

      uiInSize = uiSizeBack;
      if (TRUE != pfn(pICSC, (WE_CHAR*)pvInBuf, (WE_LONG*)&uiInSize, pcOut, (WE_LONG*)&uiOutSize))
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAKIF(iRet, "csc parse failed");
      }

      *ppcOutBuf = (WE_VOID*)pcOut;
      if (puiOutSize != NULL)
      {
         *puiOutSize = uiOutSize;
      }

   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pcOut);
   }

   return iRet;
}


WE_INT32 EME_ResetOneSegment(WE_CHAR *pcSrc, WE_INT32 iTransEnc, WE_UINT32 *puiNewSize)
{
   WE_INT32 iResult = EME_SUCCESS;
   WE_CHAR  *pcTmp = NULL;
   WE_CHAR  *pcLastTmp = NULL;

   EME_UTILS_LOG_INFO(("EME_ResetOneSegment---: Start"));
   if(NULL == pcSrc || NULL == puiNewSize)
   {
      EME_UTILS_LOG_ERROR(("EME_ResetOneSegment--: Bad parameter and return"));
      return EME_FAILED;
   }

   if(iTransEnc == E_ENCODETYPE_BASE64)
   {
      WE_INT32 i = 0;
      
      pcTmp = pcSrc;
      while(*pcTmp != '\0')
      {
         if(*pcTmp == '\r' || *pcTmp == '\n')
         {
            pcTmp++;
            continue;
         }
         pcTmp++;
         i++;
         i = i % 4;
         if(i == 0)
         {
            pcLastTmp = pcTmp;
         }
      }

      if(NULL != pcLastTmp)
      {
         *pcLastTmp = '\0';
         *puiNewSize = pcLastTmp - pcSrc;
      }
      else
      {
         return EME_FAILED;
      }
   }
   else if(iTransEnc == E_ENCODETYPE_QUOTEDPRINTABLE)
   {
      pcLastTmp = pcSrc + EME_UTILS_STRLEN(pcSrc);
      pcTmp = pcSrc + EME_UTILS_STRLEN(pcSrc) - 1;
      if(*pcTmp == '=')
      {
         *pcTmp = '\0';
         pcLastTmp = pcTmp;
      }
      else if(*(--pcTmp) == '=')
      {
         *pcTmp = '\0';
         pcLastTmp = pcTmp;
      }

      *puiNewSize = pcLastTmp - pcSrc;
   }
   else if(iTransEnc == E_ENCODETYPE_UUENCODE)
   {
      WE_INT32 iStrlen = EME_UTILS_STRLEN(pcSrc);

      pcTmp = EME_UTILS_STRSTR(pcSrc, "\r\n \r\nend");
      if(NULL != pcTmp)
      {
         *puiNewSize = EME_UTILS_STRLEN(pcSrc);
         *(pcTmp + 8) = '\0';
      }
      else if(NULL != (pcTmp = EME_UTILS_STRSTR(pcSrc, "\r\n`\r\nend")))
      {
          *puiNewSize = EME_UTILS_STRLEN(pcSrc);
          *(pcTmp + 8) = '\0';
      }
      else
      {
         pcTmp = pcSrc;
         pcTmp = EME_UTILS_STRSTR(pcTmp, "\r\n");
         while(NULL != pcTmp)
         {
            pcTmp += 2;
            pcLastTmp = pcTmp;
            pcTmp = EME_UTILS_STRSTR(pcTmp, "\r\n");
         }

         if(NULL == pcLastTmp)
         {
            return EME_FAILED;
         }

         if((pcLastTmp - pcSrc + 6) > iStrlen)
         {
            do 
            {
               pcLastTmp -= 3;
               while(pcLastTmp != pcSrc && *pcLastTmp != 0x0d)
               {
                  pcLastTmp--;
               }
               
               if(pcLastTmp == pcSrc)
               {
                  return EME_FAILED;
               }

               pcLastTmp += 2;

            } while(pcLastTmp - pcSrc + 6 > iStrlen);
         }

         EME_UTILS_MEMSET(pcLastTmp, 0, EME_UTILS_STRLEN(pcLastTmp));
         EME_UTILS_MEMCPY(pcLastTmp, " \r\nend", 6);
         
         //pcLastTmp += 8;
         *puiNewSize = pcLastTmp - pcSrc;
      }
   }
   else if(iTransEnc == E_ENCODETYPE_7BIT || iTransEnc == E_ENCODETYPE_8BIT)
   {
      iResult = EME_SUCCESS;
      *puiNewSize = EME_UTILS_STRLEN(pcSrc);
   }
   else
   {
      iResult = EME_FAILED;
   }

   EME_UTILS_LOG_INFO(("EME_ResetOneSegment---: End, Result = %d", iResult));
   return iResult;
}


WE_INT32 EME_DecodeMsg_JustPartEx(stEMSGHandle *pstInHandle,EmailOpt *pstOpt, WE_CHAR *pcTmpFile, WE_CHAR *pcDir)
{
   WE_INT32     iResult         = EME_FAILED;
   stEMSGHandle *pstEmailHandle = pstInHandle;
   St_EmailMsg  *pstMailMsg = NULL;
   void         *pvMemMgr = NULL;
   EmailOpt     optGet;

   EME_UTILS_MEMSET(&optGet,0x00,sizeof(EmailOpt));
   EME_UTILS_LOG_INFO(("EME_DecodeMsg_JustPartEx---: Start"));
   if ( NULL == pstEmailHandle || NULL == pstOpt || NULL == pcTmpFile || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustPartEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pstMailMsg = (St_EmailMsg*)pstEmailHandle->pvEmailMsg;
   if(NULL == pstMailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustPartEx--: mail message is null"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      pstMailMsg->bIsDecoding = TRUE;
      pstMailMsg->unPartInfo.astDecodingPartInfo.pvEMsgHdl = (void*)pstInHandle;
      
      pstMailMsg->pcMailFileName = EME_UTILS_STRDUP(pstEmailHandle->pvMemMgr, pcTmpFile);
      EME_UTILS_BREAKNULL(pstMailMsg->pcMailFileName, iResult, EME_NOMEMORY, "no memory");

      pstMailMsg->unPartInfo.astDecodingPartInfo.pcDir = EME_UTILS_STRDUP(pvMemMgr, pcDir);
      EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.pcDir, iResult, EME_NOMEMORY, "no memory for dir");

      /*charset*/
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPARAM_OPT_CHARSET, 0, &optGet);
      if(iResult == EME_SUCCESS && NULL != optGet.pVal)
      {
         pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet = EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)optGet.pVal);
         EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet,
            iResult,
            EME_NOMEMORY,
            "no memory for charset");
         pstMailMsg->unPartInfo.astDecodingPartInfo.iCharset = EME_MailMsg_ParseChaset(NULL, optGet.pVal);
      }

      EME_UTILS_MEMSET(&optGet,0x00,sizeof(EmailOpt));
      /*transaction encoding*/
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPART_OPT_TRANS_ENCODING, 0, &optGet);
      if(iResult == EME_SUCCESS && NULL != optGet.pVal)
      {
         pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc = EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)optGet.pVal);
         EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc,
            iResult,
            EME_NOMEMORY,
            "no memory for tran enc");
         pstMailMsg->unPartInfo.astDecodingPartInfo.iTransEnc = EME_MailMsg_ParseTransEncoding(optGet.pVal);
      }

      EME_UTILS_MEMSET(&optGet,0x00,sizeof(EmailOpt));
      /*content id*/
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPART_OPT_CONTENT_ID, 0, &optGet);
      if(iResult == EME_SUCCESS && NULL != optGet.pVal)
      {
         pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId = EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)optGet.pVal);
         EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId,
            iResult,
            EME_NOMEMORY,
            "no memory for tran enc");
      }

      EME_UTILS_MEMSET(&optGet,0x00,sizeof(EmailOpt));
      /*mime type*/
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPART_OPT_MIME_TYPE, 0, &optGet);
      if(iResult == EME_SUCCESS && NULL != optGet.pVal)
      {
         pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType = EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)optGet.pVal);
         EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType,
            iResult,
            EME_NOMEMORY,
            "no memory for tran enc");
      }

      EME_UTILS_MEMSET(&optGet,0x00,sizeof(EmailOpt));
      /*file name*/
      iResult = EME_GetOptsByIndex(pstOpt, EMAILPARAM_OPT_FILE_NAME, 0, &optGet);
      if(iResult == EME_SUCCESS && NULL != optGet.pVal)
      {
         pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName = EME_UTILS_STRDUP(pvMemMgr, (WE_CHAR*)optGet.pVal);
         EME_UTILS_BREAKNULL(pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName,
            iResult,
            EME_NOMEMORY,
            "no memory for tran enc");
      }

      iResult =EME_MailMsg_ParseJustPartEx(pstInHandle);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustPart,EME_DecodeMsgPart failed!!!!");
   } while(0);

   if(EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->pcMailFileName);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.pcDir);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcName);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc);
      EME_UTILS_FREEIF(pvMemMgr, pstMailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet);
   }
  
   EME_UTILS_LOG_INFO(("EME_DecodeMsg_JustPartEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_DecodeMsg_JustHdEx(stEMSGHandle *pstInHandle, WE_CHAR* pcEmailBuff, WE_CHAR *pcDir, WE_BOOL bOnlyHasHd)
{
   stEMSGHandle      *pstEmailHandle        = pstInHandle;
   St_EmailMsg       *pstEmailMsg           = NULL;
   void              *pvMemMgr              = NULL;
   WE_INT16          timeZone               = 0;
   WE_INT32          iResult                = EME_SUCCESS; /*must be init SUCCESS;*/
   TimeType             stJulianDateType;
   EmailOpt          *pstOneOpt             = NULL;

   EME_UTILS_LOG_INFO(("EME_DecodeMsg_JustHdEx---: Start"));
   EME_UTILS_MEMSET(&stJulianDateType, 0, sizeof(TimeType));

   if ( NULL == pstEmailHandle || pstEmailHandle->pstHeaderOpts != NULL || NULL == pcDir || NULL == pcEmailBuff)
   {  
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustHdEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pstEmailMsg =(St_EmailMsg *)pstEmailHandle->pvEmailMsg;
   if ( NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeMsg_JustHd,email Message pointer is NULL!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      pstEmailMsg->bIsDecoding = TRUE;
      /*start parse email file*/
      iResult = EME_MailMsg_ParseDataBufEx(pstEmailHandle,(void *)pstEmailMsg, pcEmailBuff, pcDir, bOnlyHasHd);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,EME_MailMsg_ParseDataBuf failed!!!!");

      pstEmailHandle->pstHeaderOpts = EME_UTILS_MALLOC(pvMemMgr, EME_MAX_HEAD_OPTS * sizeof(EmailOpt));
      EME_UTILS_BREAKNULL(pstEmailHandle->pstHeaderOpts,
                          iResult,
                          EME_NOMEMORY,
                          "no memory");
      EME_UTILS_MEMSET(pstEmailHandle->pstHeaderOpts, 0, EME_MAX_HEAD_OPTS * sizeof(EmailOpt));
      pstOneOpt = pstEmailHandle->pstHeaderOpts;
	  
      /*add date to header opts*/
      if(NULL != pstEmailMsg->pcDate)
      {
        EME_UTILS_LOG_INFO(("EME_DecodeMsg_JustHd: pstEmailMsg->pcDate = %s\n", pstEmailMsg->pcDate));
		
         iResult = EME_MailMsg_GetDateTime((void *)pstEmailMsg, 
            &stJulianDateType,
            &timeZone);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,EME_MailMsg_GetDateTime  failed!!!!");
         /*Add Date to header opts*/
         pstOneOpt->iId = EMAIL_OPT_DATE;
         pstOneOpt->pVal = (void*)EME_ToSeconds(&stJulianDateType);
         pstOneOpt++;
         
         /*Add Time Zone to header opts*/
         pstOneOpt->iId = EMAIL_OPT_TIME_ZONE;
         pstOneOpt->pVal = (void*)timeZone;
         pstOneOpt++;
      }

      /*Add From addr to header opts*/
      if (NULL != pstEmailMsg->pcFrom)
      {
         pstOneOpt->iId = EMAIL_OPT_FROM;
         pstOneOpt->pVal = (void*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsg->pcFrom);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal, iResult, EME_NOMEMORY, "no memory");

         pstOneOpt++;
      }
      
      /*Add To to header opts*/
      if (NULL!= pstEmailMsg->pcTo)
      {
         pstOneOpt->iId = EMAIL_OPT_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcTo,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse To addr failed!!!!");
            
         pstOneOpt++;
      }
     
      /*Add Cc to header opts*/
      if (NULL != pstEmailMsg->pcCc)
      {
         pstOneOpt->iId = EMAIL_OPT_CC;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcCc,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse Cc addr failed!!!!");
            
         pstOneOpt++;        
      }
      
      /*Add Bcc to header opts*/
      if (NULL!= pstEmailMsg->pcBcc)
      {
         pstOneOpt->iId = EMAIL_OPT_BCC;
         iResult = EME_GetHdFieldValue(pstEmailHandle,pstEmailMsg->pcBcc,(WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHd,parse Bcc addr failed!!!!");
            
         pstOneOpt++;  
      }
     
      /*Add Subject to header opts*/
      if (NULL != pstEmailMsg->pcSubject)
      {
         pstOneOpt->iId = EMAIL_OPT_SUBJECT;
         pstOneOpt->pVal = (void *)EME_UTILS_STRDUP(pvMemMgr, pstEmailMsg->pcSubject);
         EME_UTILS_BREAKNULL(pstOneOpt->pVal, iResult, EME_NOMEMORY, "no memory");
         
         pstOneOpt++;
      }
      
      
      /*Add Reply-To to header opts*/
      if (NULL != pstEmailMsg->pcReplyTo)
      {
         pstOneOpt->iId = EMAIL_OPT_REPLY_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle, pstEmailMsg->pcReplyTo, (WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHdEx---: parse ReplyTo addr failed!!!!");
         
         pstOneOpt++;
      }
     
      /*Add X-Priority to header opts*/
      if (NULL != pstEmailMsg->pcPriority)
      {
         pstOneOpt->iId = EMAIL_OPT_PRIORITY;
         pstOneOpt->pVal = (WE_VOID*)EME_UTILS_ATOI((const WE_UCHAR*)pstEmailMsg->pcPriority);
         pstOneOpt++;
      }

      /*Add Disposition-Notification-To to header opts*/
      if (NULL != pstEmailMsg->pcNotifyTo)
      {
         pstOneOpt->iId = EMAIL_OPT_DISP_NOTIFICATION_TO;
         iResult = EME_GetHdFieldValue(pstEmailHandle, pstEmailMsg->pcNotifyTo, (WE_CHAR**)&pstOneOpt->pVal);
         EME_UTILS_BREAKIF(iResult, "EME_DecodeMsg_JustHdEx---: parse NotifyTo addr failed!!!!");
         
         pstOneOpt++;
      }
      
      pstOneOpt->iId = EMAILOPT_END;

   } while(0);

   if(iResult != EME_SUCCESS)
   {
      EME_FreeAllOptsValue(pvMemMgr, pstEmailHandle->pstHeaderOpts);
      EME_UTILS_FREEIF(pvMemMgr, pstEmailHandle->pstHeaderOpts);
   }

   EME_UTILS_LOG_INFO(("EME_DecodeMsg_JustHdEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_DecodeEmailMsgEx(stEMSGHandle *pstInHandle, WE_CHAR* pcWholeMail, WE_CHAR *pcDir)
{
   WE_INT32       iResult              = EME_SUCCESS; /*must be init to EME_SUCCESS*/
   stEMSGHandle   *pstEmailHandle      = pstInHandle;
   void           *pvMemMgr            = NULL;
   St_EmailMsg    *pstEmailMsg         = NULL;

   EME_UTILS_LOG_INFO(("EME_DecodeEmailMsgEx---: Start"));
   if (NULL == pstEmailHandle || NULL == pcWholeMail || NULL == pstEmailHandle->pvEmailMsg || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeEmailMsgEx---: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;
   pstEmailMsg = (St_EmailMsg *)pstEmailHandle->pvEmailMsg;

   do 
   { 
      iResult = EME_DecodeMsg_JustHdEx(pstEmailHandle, pcWholeMail, pcDir, FALSE);
      EME_UTILS_BREAKIF(iResult, "EME_DecodeEmailMsgEx---: EME_DecodeMsg_JustHd failed!!!!");
   } while(0);
   
   EME_UTILS_LOG_INFO(("EME_DecodeEmailMsgEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_SetOneBodyPartEx(stEMSGHandle *pstInHandle, WE_UINT32 uiIndex)
{
   stEMSGHandle *pstEmailHandle = pstInHandle;
   EmailOpt     stPartOpt = {0};
   WE_INT32     iResult = EME_SUCCESS;
   WE_VOID      *pvEmailMsg = NULL;
   WE_VOID      *pMsgPart = NULL;
   WE_VOID      *pvMemMgr = NULL;
   WE_UINT32    uiPartType = 0;
   WE_UINT32    uiSrcType = 0;
   WE_CHAR      *pcDataBuf = NULL;
   WE_CHAR      *pcAttchName = NULL;
   WE_CHAR      *pcCharSet = NULL;
   WE_CHAR      *pcTransEnc = NULL;
   WE_UINT32    uiDataSize = 0;
   EmailOpt     *pstOpts = NULL;
   St_MsgPartList *pstOnePart = NULL;

   WE_CHAR      *pcContentType = NULL;

   EME_UTILS_LOG_INFO(("EME_SetOneBodyPartEx---: Start"));
   if (NULL == pstEmailHandle || NULL == pstEmailHandle->pvEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_SetOneBodyPartEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {  
      pstOnePart = pstEmailHandle->pstPartList;
      while (uiIndex != 0 && pstOnePart != NULL)
      {
         pstOnePart = pstOnePart->pstNext;
         uiIndex--;         
      }
      if (pstOnePart == NULL)
      {
         return EME_FAILED;
      }

      pstOpts = pstOnePart->pstPartOpts;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_PART_TYPE, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get part type failed");
      uiPartType = (WE_UINT32)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_DATA_BUFF, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get data buf failed");
      pcDataBuf = (WE_CHAR*)stPartOpt.pVal;
      EME_UpdateOpt(pstOpts, EMAILPART_OPT_DATA_BUFF, NULL, NULL);

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_SRC_TYPE, uiIndex, &stPartOpt);
      EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get src type failed");
      uiSrcType = (WE_UINT32)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPARAM_OPT_FILE_NAME, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get file name failed");
      pcAttchName = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_DATA_SIZE, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get data size failed");
      uiDataSize = (WE_UINT32)stPartOpt.pVal;
      
      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPARAM_OPT_CHARSET, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get file name failed");
      pcCharSet = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_TRANS_ENCODING, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get file name failed");
      pcTransEnc = (WE_CHAR*)stPartOpt.pVal;

      EME_UTILS_MEMSET(&stPartOpt, 0, sizeof(EmailOpt));
      EME_GetOptsByIndex(pstOpts, EMAILPART_OPT_MIME_TYPE, uiIndex, &stPartOpt);
      //EME_UTILS_BREAKNULL(stPartOpt.pVal, iResult, EME_FAILED, "EME_SetOneBodyPartEx---: get file name failed");
      pcContentType = (WE_CHAR*)stPartOpt.pVal;

      pvEmailMsg = pstEmailHandle->pvEmailMsg;

      switch((WE_UINT32)uiPartType)
      {
      case EMAILPART_PARTTYPE_TEXTPLAIN:
         iResult = EME_MailMsg_SetTextPlain(pstEmailHandle, pvEmailMsg, pcDataBuf, pcCharSet, pcTransEnc);
         break;

      case EMAILPART_PARTTYPE_TEXTHTML:
         iResult = EME_MailMsg_SetTextHtml(pstEmailHandle,pvEmailMsg, pcDataBuf, pcCharSet, pcTransEnc);
         break;
         
      case EMAILPART_PARTTYPE_ATTACHMENT:
         {
            WE_CHAR *pcMimeType    = NULL;
            WE_CHAR *pcSuffix      = NULL;

            iResult = EME_MailMsgPart_Create(pstEmailHandle, &pMsgPart);
            EME_UTILS_BREAKNULL(pMsgPart,
                                iResult,
                                EME_NOMEMORY,
                                "EME_SetOneBodyPartEx---: EME_MailMsgPart_Create failed!!!");

            iResult = EME_MailMsgPart_SetName(pstEmailHandle,pMsgPart, pcAttchName);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPartEx---: EME_MailMsgPart_SetName failed!!!!");

            iResult  = EME_MailMsgPart_SetFileName(pstEmailHandle,pMsgPart, pcAttchName);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPartEx---: EME_MailMsgPart_SetFileName failed!!!!");
            
            pcSuffix = EME_UTILS_GETEXT(pcAttchName);
            if (pcSuffix != NULL)
            {
               /*pcMimeType malloc have not use our memory manager*/
               pcMimeType = (WE_CHAR *)WeMime_Ext2Mime(pcSuffix);
            }
            else if(pcContentType != NULL)
            {
               pcMimeType = EME_UTILS_STRDUP(NULL, pcContentType);
            }
            else
            {
               pcMimeType = EME_UTILS_STRDUP(NULL, EME_UNKNOW_MIME);
            }
            iResult  = EME_MailMsgPart_SetContType(pstEmailHandle,pMsgPart, pcMimeType);
            /*Free the Mime Type not by our memory manager*/
            EME_UTILS_FREEIF(NULL, pcMimeType);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPartEx---: EME_MailMsgPart_SetContType failed!!!!");

            iResult  = EME_MailMsgPart_SetBodyDataEx(pstEmailHandle,
                                                   pMsgPart, 
                                                   pcDataBuf,
                                                   uiDataSize);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPartEx---: EME_MailMsgPart_SetBodyData failed!!!!");
            
            iResult = EME_MailMsg_AddMsgPart(pstEmailHandle,pvEmailMsg, pMsgPart);
            EME_UTILS_BREAKIF(iResult, "EME_SetOneBodyPartEx---: EME_MailMsg_AddMsgPart failed!!!!");
         }
         break;
         
      default:
         iResult =  EME_FAILED;
      }

   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcDataBuf);

   /*exceptional handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pMsgPart);
   }
   EME_UTILS_LOG_INFO(("EME_SetOneBodyPartEx---: End, Result = %d", iResult));
   return iResult;
}


WE_INT32 EME_DecodeBodyStructEx(stEMSGHandle *pstInHandle, WE_CHAR* pBodyStructBuf, WE_CHAR *pcPath)
{
   return EME_MailMsg_DecodeBodyStruct(pstInHandle, pBodyStructBuf, pcPath);
}