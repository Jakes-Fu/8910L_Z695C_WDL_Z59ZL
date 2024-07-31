/*=====================================================================================
FILE NAME   : eme_codec_mailmessage.c
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File is support a material way to implement encoding and decoding a whole email.
      These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
        20070307    wxs        01                create
        20070309    wxs        02                modify
        20070428    wxs        03                improved
                 
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
#include "eme_file_api.h"

#include "eme_codec_mailmessage.h"
#include "eme_encode_base64.h"
#include "eme_codec_decode_quoted.h"
#include "eme_codec_decode_bodystruct.h"
#include "eme_codec_utils_api.h"
#include "eme_codec_def.h"

#include "eme_codec_api.h"
#include "eme_codec_uu.h"
#include "oem_asyncproc_api.h"
#include "eme_codec_file.h"

#include "eme_codec_binhex.h"

#include "we_mime.h"


#define EME_MAX_HEADER_FORMART_SIZE    200
#define EME_MAX_BODYHEADER_SIZE        300
#define EME_MAX_EMAIL_FILENAME_LEN     30

#define EME_ENCODING_BUFFER_ONCE       (1024 * 48)
#define EME_DECODING_BUFFER_ONCE       (1024 * 48)

/*===========================================================================*/
/*                      static function prototype                            */
/*===========================================================================*/
static WE_INT32 EME_MailMsg_UpdateHeader(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn, 
                                         WE_CHAR* pcHeaderTag,
                                         WE_CHAR* pcHeaderVal);

static WE_INT32 EME_MailMsg_AppendHeader(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn,
                                         WE_CHAR* pcNewHeader);

static WE_INT32 EME_MailMsg_StrcatToBody(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn, 
                                         WE_CHAR* pcAddStr);

static WE_CHAR* EME_HeaderEncode2Base64(stEMSGHandle *pstEMsgHandle,WE_CHAR* pcSrc, E_Charset eCharset);

static WE_CHAR* EME_MailMsg_DecodeWord(stEMSGHandle *pstEMsgHandle,
                                       WE_CHAR* pcStart, 
                                       WE_CHAR* pcEnd);

static WE_CHAR* EME_MailMsg_GetHeader(stEMSGHandle *pstEMsgHandle,
                                      St_EmailMsg* pstEmailMsg);

static WE_INT32 EME_MailMsg_ClearHeader(stEMSGHandle *pstEMsgHandle,
                                        St_EmailMsg* pstIn);

static WE_CHAR* EME_MailMsg_LocalExpansion(stEMSGHandle *pstEMsgHandle,
                                           WE_CHAR* pcSrc);

static WE_INT32 EME_MailMsg_FindPos(WE_CHAR* pcTarget,
                                    WE_CHAR* pcSrc);

static WE_INT32 EME_MailMsg_FindHdFieldPos(WE_CHAR* pcTarget, 
                                           WE_CHAR* pcSrc);

static WE_INT32 EME_MailMsg_ParseHDField(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn, 
                                         WE_CHAR* pcHeaderField,
                                         WE_BOOL *pbRet);

static WE_INT32 EME_MailMsg_ParseHeaders(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn,
                                         WE_CHAR* pcHeaderStart,
                                         WE_CHAR* pcHeaderEnd);


static WE_INT32 EME_MailMsg_UnquoteString(stEMSGHandle *pstEMsgHandle,
                                          WE_CHAR* pcSrc, 
                                          WE_CHAR cUnquote1, 
                                          WE_CHAR cUnquote2);

static WE_INT32 EME_MailMsg_ParseMsgBody(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg *pstIn, 
                                         WE_CHAR *pcBodyPartBuf);

static WE_INT32 EME_MailMsg_ParseMimePart(stEMSGHandle *pstEMsgHandle,
                                          St_EmailMsg *pstIn,
                                          WE_CHAR* pcBodyData, 
                                          WE_CHAR* pcBoundary);

static WE_INT32 EME_MailMsg_AppendAddrField(stEMSGHandle *pstEMsgHandle,
                                            WE_CHAR** ppcAddrField,
                                            WE_CHAR* pcOneAddr);

static WE_INT32 EME_MailMsg_SetAddrField(stEMSGHandle *pstEMsgHandle,
                                         WE_CHAR** ppcField, 
                                         WE_CHAR* pcAddr, 
                                         WE_CHAR* pcName);

static WE_CHAR* EME_MailMsg_ParseBodyCharset(stEMSGHandle *pstEMsgHandle,
                                               WE_CHAR* pcWord, 
                                               WE_CHAR* pcCharset);

static WE_CHAR* EME_MailMsg_ParseSpecName(stEMSGHandle *pstEMsgHandle,
                                          WE_CHAR* pcSpecName);

static WE_CHAR* EME_MailMsg_CheckCHString(stEMSGHandle *pstEMsgHandle,
                                          WE_CHAR* pcSrc);

static WE_CHAR* EME_MailMsg_FindElem(WE_CHAR* pcPos);

static WE_INT32 EME_IsAppendTextPlain(stEMSGHandle *pstEMsgHandle,
                                      St_EmailMsg *pstEmailMsg,
                                      WE_BOOL bIsTextPlain);

static WE_INT32 EME_IsAppendTextPlainEx(stEMSGHandle *pstEMsgHandle,
                                      St_EmailMsg *pstEmailMsg,
                                      WE_BOOL bIsTextPlain);
/*===============not using in current ===============================*/                                   
/*
static WE_INT32 EME_MailMsg_ParseFromField(stEMSGHandle *pstEMsgHandle,
                                           WE_CHAR* pcFrom,
                                           WE_CHAR** ppcName,
                                           WE_CHAR** ppAddr);
 for ads compile*/
static WE_INT32 EME_MailMsg_FindAddr(WE_CHAR* line,
                                     WE_CHAR** pos,
                                     WE_CHAR** endPos);
/*
static WE_INT32 EME_MailMsg_GetField(stEMSGHandle *pstEMsgHandle,
                                     WE_CHAR** field, 
                                     WE_CHAR** ret, 
                                     WE_CHAR* charset);
for ads compile */

static WE_UINT32 EME_MailMsg_CaluBodySize(St_EmailMsg *pstEmailMsg);



/*************************************************************************
           Encoding & Decoding of Synchronism.
 ************************************************************************/
static void EME_MailMsg_EncodingPartsEnd(stEMSGHandle *pstEMsgHandle);
static void EME_MailMsg_EncodingPartsStart(stEMSGHandle *pstEMsgHandle);
static void EME_MailMsg_EncodingOneBufferCB(St_EncodingPartInfo *pstPartInfo);


static void EME_MailMsg_DecodingPartsStart(stEMSGHandle *pstEMsgHandle);
static void EME_MailMsg_DecodingPartsEnd(stEMSGHandle *pstEMsgHandle);
static void EME_MailMsg_DecodingOneBufferCB(St_DecodingPartInfo  *pstDecPartInfo);

static WE_INT32 EME_MailMsg_ParseMsgBodyEx(stEMSGHandle *pstEMsgHandle,
                                            St_EmailMsg *pstIn,
                                            void *pvIFile,
                                            WE_CHAR *pcDir);
static WE_INT32 EME_MailMsg_ParsePartHead(stEMSGHandle        *pstEMsgHandle,
                                          St_EmailMsg         *pstIn,
                                          WE_CHAR             *pcBodyHead,
                                          St_DecodingPartInfo *pstDecPartInfo);
static WE_INT32 EME_MailMsg_ParsePartBody(stEMSGHandle *pstEMsgHandle, St_EmailMsg  *pstIn);

static void EME_MailMsg_ClearDecodingPartInfo(stEMSGHandle *pstEMsgHandle, St_DecodingPartInfo *pstDecPartInfo);
static void EME_MailMsg_ClearEncodingPartInfo(stEMSGHandle *pstEMsgHandle, St_EncodingPartInfo *pstEncPartInfo);


static WE_INT32 EME_MailMsg_Get(St_BinhexCatch *pstBinhex);
static void EME_MailMsg_Write(St_BinhexCatch *pstBinhex, WE_INT32 c);
static void EME_MailMsg_ParseBinhexCB(WE_VOID *p, WE_BOOL bSuccess);

static WE_INT32 EME_MailMsg_NotifyResult(void *pvData);

static WE_BOOL EME_MailMsg_IsVaildUTF8(WE_CHAR * pUTF8);
/*===============not using in current ===============================*/ 


/*===================================================================
* 
*                    Functions implementation
* 
* ==================================================================*/
static void EME_MailMsg_AppendToExFIeld(St_EmailMsg* pstIn, WE_CHAR* pcNewHeader)
{
/* Currently we don't need Extra field, 
   * can do the complement as mailmessage_appendToHeaderField function */ 
}


static WE_INT32 EME_MailMsg_AppendHeader(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn,
                                         WE_CHAR* pcNewHeader)
{  
   WE_INT32 iResult = EME_SUCCESS; /*must be init success*/
   void *pvMemMgr = NULL;

   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcNewHeader)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendHeader(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      if (pstIn->uiHeaderFieldNum % 10 == 0)
      {
         pstIn->ppcHeaderFields = EME_UTILS_REALLOC(pvMemMgr,pstIn->ppcHeaderFields, 
            sizeof(WE_CHAR*) * (pstIn->uiHeaderFieldNum + 10));   
         EME_UTILS_BREAKNULL( pstIn->ppcHeaderFields,
            iResult,
            EME_NOMEMORY,
            "reallocate memory failed!!!");
      }   
      
      /* new header , add it */
      pstIn->ppcHeaderFields[++pstIn->uiHeaderFieldNum - 1] = EME_UTILS_STRDUP(pvMemMgr,pcNewHeader);
      EME_UTILS_BREAKNULL( pstIn->ppcHeaderFields[pstIn->uiHeaderFieldNum - 1],
         iResult,
         EME_NOMEMORY,
            "EME_UTILS_STRDUP allocate memory failed!!!");

   } while(0);
   
   return iResult;
}


static WE_INT32 EME_MailMsg_UpdateHeader(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn, 
                                         WE_CHAR* pcHeaderTag, 
                                         WE_CHAR* pcHeaderVal)
{
   WE_INT32   iResult      = EME_SUCCESS; /*must init success*/
   void      *pvMemMgr     = NULL;
   WE_CHAR   *pcNewHeader  = NULL;

   if (NULL == pstEMsgHandle ||  NULL == pstIn || NULL == pcHeaderTag || NULL == pcHeaderVal)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_UpdateHeader(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pcNewHeader = EME_UTILS_MALLOC(pvMemMgr,
         EME_UTILS_STRLEN(pcHeaderTag) + EME_UTILS_STRLEN(pcHeaderVal) + 4);      /* +4 means ' ' + '\r' + \n' + '\0' */
      EME_UTILS_BREAKNULL(pcNewHeader,
         iResult,
         EME_NOMEMORY,
         "malloc failed!!!");
      EME_UTILS_MEMSET(pcNewHeader,0x00,
         EME_UTILS_STRLEN(pcHeaderTag) + EME_UTILS_STRLEN(pcHeaderVal) + 4);

      EME_UTILS_SPRINTF(pcNewHeader, "%s %s\r\n", pcHeaderTag, pcHeaderVal);
      
      /* if find header is exist ,  don't need to add it  */
//       for (i = 0; i < pstIn->uiHeaderFieldNum; i++)
//       {
//          if ( EME_UTILS_STRSTR(pstIn->ppcHeaderFields[i], pcHeaderTag) == pstIn->ppcHeaderFields[i])
//          {
//             /*update the header field ,only need to update the data*/
//             EME_UTILS_FREEIF(pvMemMgr,pstIn->ppcHeaderFields[i]);
//             pstIn->ppcHeaderFields[i] = EME_UTILS_STRDUP(pvMemMgr,pcNewHeader);
//             EME_UTILS_BREAKNULL(pstIn->ppcHeaderFields[i],
//                iResult,
//                EME_NOMEMORY,
//                "EME_MailMsg_UpdateHeader,EME_UTILS_STRDUP failed!!!");
//             
//             return EME_SUCCESS;
//          }
//       }
      
      iResult = EME_MailMsg_AppendHeader(pstEMsgHandle,pstIn, pcNewHeader);
      EME_UTILS_BREAKIF(iResult,"EME_MailMsg_UpdateHeader,EME_MailMsg_AppendHeader");

   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr,pcNewHeader);

   return iResult;
}


static WE_INT32 EME_MailMsg_StrcatToBody(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn, 
                                         WE_CHAR* pcAddedStr)
{
   WE_UINT32 uiAddedSize   = 0; /* 1 means + '\n' */
   WE_UINT32 uiOldBufLen   = 0;
   void *pvMemMgr          = NULL;
   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcAddedStr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_StrcatToBody(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   uiAddedSize = EME_UTILS_STRLEN(pcAddedStr) + 1;
   uiOldBufLen = (pstIn->pcEmailDataBuf == NULL ? 0 : EME_UTILS_STRLEN(pstIn->pcEmailDataBuf));
   
   if (uiOldBufLen + uiAddedSize > pstIn->uiEMailDataSize)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_StrcatToBody(), msg size not enough!!!!!!"));
      return EME_NOMEMORY;
   }



//    /* if the size is not enough, realloc */
//    if (uiOldBufLen + uiAddedSize > pstIn->uiEMailDataSize)
//    {
//       uiNewSize = (uiOldBufLen + uiAddedSize - pstIn->uiEMailDataSize) / EME_MAXSTRLEN + 1;      
//       pstIn->uiEMailDataSize += EME_MAXSTRLEN * uiNewSize; /* recalculate the body size. */
//       pstIn->pcEmailDataBuf = EME_UTILS_REALLOC(pvMemMgr,pstIn->pcEmailDataBuf, pstIn->uiEMailDataSize);
//       if (NULL == pstIn->pcEmailDataBuf)
//       {
//          EME_UTILS_LOG_ERROR(("EME_MailMsg_StrcatToBody(),EME_UTILS_REALLOC failed!!!!"));
//          return EME_NOMEMORY;
//       }
//       pstIn->pcEmailDataBuf[EME_UTILS_STRLEN(pstIn->pcEmailDataBuf)] = '\0'; 
//    }
   
   /* add the str to body */ 
   EME_UTILS_STRCAT(pstIn->pcEmailDataBuf, pcAddedStr);
   return EME_SUCCESS;
}


static WE_CHAR* EME_HeaderEncode2Base64(stEMSGHandle *pstEMsgHandle,WE_CHAR* pcSrc, E_Charset eCharset)
{
   WE_CHAR *pcTmp       = NULL;
   WE_CHAR *pcRet       = NULL;
   void    *pvMemMgr    = NULL;
   WE_UINT32 uiSize     = 0;
   WE_CHAR  *pcCharset  = NULL;
   WE_CHAR  *pcOut      = NULL;
   WE_UINT32 uiOutSize  = 0;
   WE_INT32 iResult     = EME_SUCCESS;

   if (NULL == pstEMsgHandle || NULL == pcSrc ) 
   {  
      EME_UTILS_LOG_ERROR(("EME_Encode2Base64(),input parameters is invalid!!!!"));
      return NULL;
   }

   /* be care to do this strIn must use utf-8,
   * or EME_UTILS_STRLEN will calculate error */ 
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {  
      iResult = EME_Utf8ToXX(pstEMsgHandle->pICSC, 
                            pvMemMgr, 
                            pcSrc, 
                            EME_UTILS_STRLEN(pcSrc),
                            eCharset,
                            (WE_VOID*)&pcOut,
                            &uiOutSize);
      EME_UTILS_BREAKIF(iResult, "char set convert failed");
      EME_UTILS_BREAKNULL(pcOut, iResult, EME_FAILED, "char set convert failed");

      pcCharset = EME_GetCharSet(pvMemMgr, eCharset);

      uiSize = EME_Base64EncodeSize(uiOutSize) + 1;

      pcTmp = EME_UTILS_MALLOC(pvMemMgr,uiSize);
      if (NULL == pcTmp)
      {
         EME_UTILS_LOG_ERROR(("EME_Encode2Base64,malloc failed!!!!"));
         break;
      }
      EME_UTILS_MEMSET(pcTmp,0x00,uiSize);

      EME_ToBase64(pcTmp, pcOut, 0, TRUE);

      uiSize += EME_UTILS_STRLEN("=\?\?B?\?=") + EME_UTILS_STRLEN(pcCharset); /* be care the trigraph, the strlen is 12 */ 
      
      pcRet = EME_UTILS_MALLOC(pvMemMgr,uiSize);
      if (NULL == pcRet)
      {
         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
         EME_UTILS_LOG_ERROR(("EME_Encode2Base64,malloc failed!!!!"));
         break;
      }

      EME_UTILS_MEMSET(pcRet,0x00,uiSize);
      EME_UTILS_SPRINTF(pcRet, "=?%s?B?%s?=", pcCharset, pcTmp);
      
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr,pcTmp);
   EME_UTILS_FREEIF(pvMemMgr,pcOut);
   EME_UTILS_FREEIF(pvMemMgr,pcCharset);
   
   return pcRet;
}


static WE_CHAR* EME_MailMsg_GetHeader(stEMSGHandle *pstEMsgHandle,St_EmailMsg* pstEmailMsg)
{
   WE_UINT32 i              = 0;
   WE_UINT32 uiHeaderSize   = 0;
   WE_CHAR   *pcHeaderRet   = NULL;
   void *pvMemMgr           = NULL;
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetHeader(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   /* calculate size. */
   for (i = 0; i < pstEmailMsg->uiHeaderFieldNum; i++)
   {
      uiHeaderSize += EME_UTILS_STRLEN(pstEmailMsg->ppcHeaderFields[i]);
   }
   uiHeaderSize++; /* ++ means '\0'*/ 
   
   /* malloc mem */
   pcHeaderRet = EME_UTILS_MALLOC(pvMemMgr,uiHeaderSize + 2);/* +2 means "\r\n"*/
   if (NULL == pcHeaderRet)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetHeader,malloc failed!!!!"));
      return NULL;
   }
   
   EME_UTILS_MEMSET(pcHeaderRet,0x00,uiHeaderSize + 2);
   /* String cat... */
   for (i = 0; i < pstEmailMsg->uiHeaderFieldNum; i++)
   {
      EME_UTILS_STRCAT(pcHeaderRet, pstEmailMsg->ppcHeaderFields[i]);
   }
   EME_UTILS_STRCAT(pcHeaderRet, "\r\n");
   
   return pcHeaderRet;   
}


/* return index of position pStr in tar. */
static WE_INT32 EME_MailMsg_FindPos(WE_CHAR* pcTarget, WE_CHAR* pcSrc)
{
   WE_INT32 iRet  = -1;
   WE_CHAR* pcPos = NULL;
   
   if ( EME_UTILS_StrStrNC(pcSrc, pcTarget) == pcSrc ) /* find first word from Src. */
   {
//      if ( (pcPos = EME_UTILS_STRCHR(pcSrc, ' ')) != NULL )
      if((pcPos = EME_UTILS_STRCHR(pcSrc, ':')) != NULL && *(++pcPos) == ' ')
      {
         iRet = pcPos - pcSrc;
         while (pcSrc[++iRet] == ' ') {} /* find first none space. */
      }
      else if ( (pcPos = EME_UTILS_STRCHR(pcSrc, ':')) != NULL )
      {
         iRet = pcPos - pcSrc;
         iRet++;
      }
   }
   
   return iRet;
}


static WE_INT32 EME_MailMsg_FindHdFieldPos(WE_CHAR* pcTarget, WE_CHAR* pcSrc)
{
   WE_INT32 iRet  = -1;
   WE_CHAR *pcPos = NULL;
   
   if ( (pcPos = EME_UTILS_StrStrNC(pcSrc, pcTarget)) == NULL )
   {
      return iRet;
   }
   
   pcPos += EME_UTILS_STRLEN(pcTarget);
   iRet = pcPos - pcSrc;
   
   return iRet;
}


// static WE_CHAR *EME_MailMsg_DecodeWord(stEMSGHandle *pstEMsgHandle,
//                                     WE_CHAR* pcStart, 
//                                     WE_CHAR* pcEnd)
// {   
//    WE_CHAR *pcAlias                    = NULL;
//    WE_CHAR *pcWord                     = NULL;  
//    WE_CHAR *pcCharset                  = NULL;
//    WE_CHAR acEncodeId[2]               = {0};         /* "B" or "Q" */
//    void    *pvMemMgr                   = NULL;
//    WE_CHAR *pcQuet1Pos                 = NULL;
//    WE_CHAR *pcQuet2Pos                 = NULL;
//    WE_CHAR *pcRetWord                  = NULL;
//    WE_INT32 iResult                    = EME_SUCCESS; /*must init to success*/
//    E_Charset eCharset                  = E_CHARSET_UTF8;
//    WE_UINT32 uiOutSize                 = 0;
// 
//    if (NULL == pstEMsgHandle || NULL == pcStart || NULL == pcEnd )
//    {
//       EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeWord(),input parameters is invalid!!!!"));
//       return NULL;
//    }
//    pvMemMgr = pstEMsgHandle->pvMemMgr;
//    
//    if (pcEnd < pcStart)
//    {
//       EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeWord , the word decode invalid!!!!"));
//       return NULL;
//    }
// 
//    do 
//    {
//       pcAlias = EME_UTILS_MALLOC(pvMemMgr,pcEnd - pcStart + 2); /* there will be '\0' in last. */ 
//       EME_UTILS_BREAKNULL(pcAlias,
//                iResult,
//                EME_NOMEMORY,
//                "EME_MailMsg_DecodeWord  malloc failed!!!");
// 
//       EME_UTILS_MEMSET(pcAlias,0x00,pcEnd - pcStart + 2);
//       EME_UTILS_MEMMOVE(pcAlias, pcStart, pcEnd - pcStart + 1);
//       
//       if ( (pcQuet1Pos = EME_UTILS_STRCHR(pcAlias, '?')) != NULL )
//       {
//          if ( (pcQuet2Pos = EME_UTILS_STRCHR(pcQuet1Pos + 1, '?')) != NULL )
//          {
//             /*get the encoding charset*/
//             pcCharset = EME_UTILS_MALLOC(pvMemMgr,pcQuet1Pos - pcAlias + 1);
//             EME_UTILS_BREAKNULL(pcCharset,
//                      iResult,
//                      EME_NOMEMORY,
//                      "EME_MailMsg_DecodeWord: pcCharset Malloc failed!!!");
// 
//             EME_UTILS_MEMSET(pcCharset,0x00,pcQuet1Pos - pcAlias + 1);
// 
//             EME_UTILS_MEMMOVE(pcCharset, pcAlias, pcQuet1Pos - pcAlias);/* get charset */
//             EME_UTILS_MEMMOVE(acEncodeId, pcQuet1Pos + 1, 1);/* get encode type. */
//             EME_UTILS_STRUPPER(acEncodeId);
//             EME_UTILS_STRLOWER(pcCharset);
//             
//             if (EME_UTILS_STRSTR(acEncodeId, "Q") != NULL)
//             {
//                WE_UINT32 len = EME_QuotedDecodeSize(EME_UTILS_STRLEN(pcAlias));
//                pcWord = EME_UTILS_MALLOC(pvMemMgr,len);
//                EME_UTILS_BREAKNULL(pcWord,
//                         iResult,
//                         EME_NOMEMORY,
//                         "EME_MailMsg_DecodeWord malloc failed!!!");
// 
//                EME_UTILS_MEMSET(pcWord,0x00,len);
//                EME_FromQuoted(pcWord, pcQuet2Pos + 1);
//             }
//             else if (EME_UTILS_STRSTR(acEncodeId, "B") != NULL)
//             {
//                WE_UINT32 len = EME_Base64DecodeSize(EME_UTILS_STRLEN(pcAlias)) + 1;
//                pcWord = EME_UTILS_MALLOC(pvMemMgr,len);
//                EME_UTILS_BREAKNULL(pcWord,
//                   iResult,
//                   EME_NOMEMORY,
//                         "EME_MailMsg_DecodeWord malloc failed!!!");
// 
//                EME_UTILS_MEMSET(pcWord,0x00,len);
//                EME_FromBase64(pcWord, pcQuet2Pos + 1);
//             }
//             else
//             {
//                pcWord = EME_UTILS_STRDUP(pvMemMgr,pcQuet2Pos + 1);
//             }
//             
//             eCharset = EME_MailMsg_ParseChaset(pcCharset);
// 
//             /* charset convertion */ 
//             iResult = EME_XXToUtf8(pstEMsgHandle->pICSC,
//                                   pvMemMgr,
//                                   pcWord,
//                                   EME_UTILS_STRLEN(pcWord),
//                                   eCharset,
//                                   &pcRetWord,
//                                   &uiOutSize);
//             EME_UTILS_BREAKIF(iResult, "charset convert failed");
//             EME_UTILS_BREAKNULL(pcRetWord, iResult, EME_FAILED, "charset convert failed");
//             //pcRetWord = EME_MailMsg_DecodeCharset(pstEMsgHandle,pcWord, pcCharset);
//             
//             EME_UTILS_FREEIF(pvMemMgr,pcAlias);
//             EME_UTILS_FREEIF(pvMemMgr,pcWord);
//             EME_UTILS_FREEIF(pvMemMgr,pcCharset);
//             return pcRetWord;
//          }
//       }
//    } while(0);
//    
//    /*error handling*/
//    if (EME_SUCCESS != iResult)
//    {  
//       EME_UTILS_FREEIF(pvMemMgr,pcAlias);
//       EME_UTILS_FREEIF(pvMemMgr,pcWord);
//       EME_UTILS_FREEIF(pvMemMgr,pcCharset);
//    }
//    return NULL;    
// }


WE_INT32 EME_MailMsg_ParseLine(stEMSGHandle *pstEMsgHandle,
                               WE_CHAR* pcOut, 
                               WE_CHAR* pcLineStart, 
                               WE_CHAR* pcLineEnd)
{
   WE_INT32 iResult     = EME_SUCCESS;/* must be init SUCCESS*/
   WE_CHAR* pcEndPos    = NULL;
   WE_CHAR* pcQuetPos   = NULL;
   WE_CHAR* pcDecode    = NULL;
   void *pvMemMgr       = NULL;
   WE_CHAR* pcPos       = pcLineStart;
   WE_CHAR* pcTmp       = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pcOut || NULL == pcPos || NULL == pcLineEnd)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseLine(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      /* goto first actual alpha */ 
      while ((*pcPos) == ' ' || (*pcPos) == '\t' || (*pcPos) == '\r' ||(*pcPos) == '\n') ++pcPos;
      while ((*pcLineEnd) == ' '|| (*pcPos) == '\t' || (*pcLineEnd) == '\r' || (*pcLineEnd) == '\n') --pcLineEnd;
      
      /* modifid by jiangdingqing*/
      //if (pcLineEnd <= pcPos)
      if (pcLineEnd < pcPos)
      {
         *pcOut = 0;
         iResult = EME_FAILED;
         break;
      }
      
      pcQuetPos = EME_UTILS_STRSTR(pcPos, "=?");

      /*the string is ascii*/
      if (pcQuetPos == NULL || pcQuetPos >= pcLineEnd) 
      {
         EME_UTILS_MEMMOVE(pcOut, pcPos, pcLineEnd - pcPos + 1);
      }
      /*the string is may be none ascii*/
      else
      {
         EME_UTILS_MEMMOVE(pcOut, pcPos, pcQuetPos - pcPos); /* cpy word before "=?" */ 
         
         pcTmp = pcPos;
         while (pcTmp != NULL)
         {
            pcTmp = EME_UTILS_STRSTR(pcTmp, "?=");
            if (pcTmp >= pcLineEnd)
            {
               break;
            }
            /* This is a hack to handle encoded words which uses q-encoding and the first WE_CHAR is
            * the escape character = */
            if (pcTmp != NULL)
            {
               if ( *(pcTmp - 1) == '?' )
               {
                  //pcTmp = EME_UTILS_STRSTR(pcEndPos + 1, "?=");
                  pcTmp += 2;
               }
               else
               {
                  pcEndPos = pcTmp;
                  pcTmp++;
               }
            }
         }

         if (pcEndPos != NULL)
         {
            pcDecode = EME_MailMsg_DecodeWord(pstEMsgHandle,pcQuetPos, pcEndPos + 2);/* +2 and -1 means strip =? ?= */          
            EME_UTILS_BREAKNULL(pcDecode, iResult, EME_FAILED, "decode failed");

            EME_UTILS_STRCAT(pcOut, pcDecode);
            EME_UTILS_FREEIF(pvMemMgr,pcDecode);
            EME_UTILS_MEMMOVE(pcOut + EME_UTILS_STRLEN(pcOut), 
                              pcEndPos + 2, 
                              pcLineEnd - pcEndPos - 1);

         }
         /*the string is ascii*/
         else
         {
            /* there are some error here, just return. */ 
            EME_UTILS_STRCPY(pcOut, "");
//             iResult = EME_FAILED;
//             EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseLine,there are some unknow error !!!");
         }
      }
      
   } while(0);
   
   return iResult;
}


/* find the actual element for parse */
static WE_CHAR* EME_MailMsg_FindElem(WE_CHAR* pcPos)
{
   WE_INT32 iCounter = 0;
   
   while (EME_UTILS_STRLEN(pcPos) > 0)
   {
      if (*pcPos == '\"')
      {
         iCounter++;
      }
      
      if (*pcPos == ',' && iCounter % 2 == 0)
      {
         return pcPos;
      }
      pcPos++;
   }
   return NULL;
}


static WE_CHAR* EME_MailMsg_LocalExpansion(stEMSGHandle *pstEMsgHandle,WE_CHAR* pcSrc)
{   
   void    *pvMemMgr   = NULL;
   WE_CHAR *pcRet      = NULL;
   WE_CHAR *pcOutPos   = NULL;
   WE_CHAR *pcLineEnd  = NULL;
   WE_CHAR *pcPos      = pcSrc;
   WE_INT32 iResult  = SUCCESS;

   if (NULL == pstEMsgHandle || NULL == pcPos)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_LocalExpansion(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   /* value need to convert to utf8 */
   pcRet = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcSrc) * 3 + 1);
   if (NULL == pcRet)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_LocalExpansion(),malloc failed!!!!"));
      return NULL;
   }
   EME_UTILS_MEMSET(pcRet,0x00,EME_UTILS_STRLEN(pcSrc) * 3 + 1);
   
   pcOutPos = pcRet;
   while (pcPos != NULL)
   {
      pcLineEnd = EME_MailMsg_FindElem(pcPos);
      
      if (pcLineEnd)
      {
         iResult = EME_MailMsg_ParseLine(pstEMsgHandle,pcOutPos, pcPos, pcLineEnd);
         pcPos = pcLineEnd + 1; /* go to next line uiBegin */ 
         pcOutPos = pcRet + EME_UTILS_STRLEN(pcRet);/* update out pos to uiEnd of string. */                      
      }
      else
      {
         iResult = EME_MailMsg_ParseLine(pstEMsgHandle,pcOutPos, pcPos, pcSrc + EME_UTILS_STRLEN(pcSrc) -1);
         break; /* it is last line, break */ 
      }
   }
   
   /*parse line failed: the corresponding field is empty*/
   if ( EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pcRet);
   }

   /* may has some other spec situations... */
   return pcRet;    
}


/* find the actual address for parse */
static WE_INT32 EME_MailMsg_FindAddr(WE_CHAR* line, WE_CHAR** pPos, WE_CHAR** pEndPos)
{
   /*WE_CHAR* pStart = line;*/ /*old*/
   WE_CHAR* pStart = NULL;    /*new*/
   WE_CHAR* pEnd   = NULL;
   if (NULL == line || NULL == pPos || NULL == pEndPos)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_FindAddr(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   /*pEnd = line + EME_UTILS_STRLEN(line);*/ /*old*/
   
   while (EME_UTILS_STRLEN(line) > 0)
   {
      if (*line == '<')
      { 
         pStart = line;
      }
      if (*line == '>')
      { 
         pEnd = line;
      }
      line++;
   }
   if (pEnd < pStart)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_FindAddr error!!!!!!!"));
      return EME_FAILED;
   }
   
   *pPos = pStart;
   *pEndPos = pEnd;
   return EME_SUCCESS;
}


static WE_INT32 EME_MailMsg_ParseFromField(stEMSGHandle *pstEMsgHandle,
                                           WE_CHAR* pcFrom,
                                           WE_CHAR** ppcName, 
                                           WE_CHAR** ppAddr)
{   
   WE_INT32  iResult    = EME_SUCCESS; /* must be init SUCCESS*/ 
   WE_CHAR   *pos       = pcFrom;
   WE_CHAR   *endPos    = NULL;
   void      *pvMemMgr  = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pos || NULL == ppcName ||NULL == ppAddr )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseFromField(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {  
      if (NULL == EME_UTILS_STRCHR(pcFrom,'<') && NULL == EME_UTILS_STRCHR(pcFrom,'>'))
      {
         if (EME_UTILS_STRCHR(pcFrom, '@') != NULL)
         {
            *ppAddr = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcFrom) + 1); /* +1 means + '\0' */
            EME_UTILS_BREAKNULL(*ppAddr,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParseFromField,malloc failed!!!");
            EME_UTILS_MEMSET(*ppAddr,0x00,EME_UTILS_STRLEN(pcFrom) + 1);
            EME_UTILS_MEMMOVE(*ppAddr, pcFrom, EME_UTILS_STRLEN(pcFrom));
         }
         else
         {
            iResult = EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseFromField,the address id Invalid!!!!");
         }
         
      }
      
      else
      {
         iResult = EME_MailMsg_FindAddr(pcFrom, &pos, &endPos);
         if (EME_SUCCESS == iResult)
         {
            if (pos != pcFrom)
            {
               *ppcName = EME_UTILS_MALLOC(pvMemMgr,pos - pcFrom + 1); /* +1 means +'\0' */
               EME_UTILS_BREAKNULL(*ppcName,
                  iResult,
                  EME_NOMEMORY,
                  "EME_MailMsg_ParseFromField,malloc failed!!!");
               EME_UTILS_MEMSET(*ppcName,0x00,pos - pcFrom + 1);
               EME_UTILS_MEMMOVE(*ppcName, pcFrom, pos - pcFrom);       
               EME_MailMsg_UnquoteString(pstEMsgHandle,*ppcName, '\"', '\"');
            }
            else
            {
               *ppcName = NULL;
            }
            
            if ('<' == *pos)
            { 
               pos++; /* go behind '<' */
            }
            
            if (endPos == NULL)
            {
               endPos = pcFrom + EME_UTILS_STRLEN(pcFrom) - 1;
            }
            *ppAddr = EME_UTILS_MALLOC(pvMemMgr,endPos - pos + 1); /* +1 means + '\0' */
            EME_UTILS_BREAKNULL(*ppAddr,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParseFromField,malloc failed!!!");
            EME_UTILS_MEMSET(*ppAddr,0x00,endPos - pos + 1);
            EME_UTILS_MEMMOVE(*ppAddr, pos, endPos - pos);
         }
         
         else if (EME_UTILS_STRCHR(pcFrom, '@') != NULL) /* the readed line don't has name, just save their address */ 
         {
            *ppcName = NULL;
            *ppAddr = EME_UTILS_STRDUP(pvMemMgr,pcFrom);
            EME_UTILS_BREAKNULL(*ppAddr,
               iResult,
               EME_NOMEMORY,
               "EME_UTILS_STRDUP,failed!!!");
            
         }
         else /* the readed line don't has address, just save their name */ 
         {
            *ppcName = EME_UTILS_STRDUP(pvMemMgr,pcFrom);
            EME_UTILS_BREAKNULL(*ppcName,
               iResult,
               EME_NOMEMORY,
               "EME_UTILS_STRDUP,failed!!!");
            *ppAddr = NULL;
         }
      }
       
   } while(0);
   
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,*ppcName);
      EME_UTILS_FREEIF(pvMemMgr,*ppAddr);
   }
   
   return iResult ;
}

static WE_BOOL IsTextUTF8(WE_CHAR* str, WE_UINT32 length)
{
   WE_UINT32 i;
   WE_ULONG nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
   WE_UCHAR chr;
   WE_BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8
   for(i=0;i<length;i++)
   {
      chr= *(str+i);
      if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
         bAllAscii= FALSE;
      if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
      {
         if(chr>=0x80)
         {
            if(chr>=0xFC&&chr<=0xFD)
               nBytes=6;
            else if(chr>=0xF8)
               nBytes=5;
            else if(chr>=0xF0)
               nBytes=4;
            else if(chr>=0xE0)
               nBytes=3;
            else if(chr>=0xC0)
               nBytes=2;
            else
            {
               return FALSE;
            }
            nBytes--;
         }
      }
      else //多字节符的非首字节,应为 10xxxxxx
      {
         if( (chr&0xC0) != 0x80 )
         {
            return FALSE;
         }
         nBytes--;
      }
   }

   if( nBytes > 0 ) //违返规则
   {
      return FALSE;
   }
   return TRUE;
}

/* check whether the string is en/ch, and convert gb2312 to utf8, if is ch */
static WE_CHAR* EME_MailMsg_CheckCHString(stEMSGHandle *pstEMsgHandle,WE_CHAR* pcSrc)
{
   WE_BOOL bCh = FALSE;
   void *pvMemMgr = NULL;
   WE_UINT8* pVisitor = (WE_UINT8 *)pcSrc;
   WE_CHAR  *pcOut = NULL;
   WE_UINT32 uiOutSize = 0;
   
   if (NULL == pstEMsgHandle || NULL == pVisitor)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_CheckCHString(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   if(IsTextUTF8((WE_CHAR*)pVisitor, EME_UTILS_STRLEN((WE_CHAR*)pVisitor)))
   {
      return EME_UTILS_STRDUP(pvMemMgr,pcSrc);
   }
   
   for (; EME_UTILS_STRLEN((WE_CHAR*)pVisitor) > 0; pVisitor++)
   {
      if (*pVisitor > 128)
      {
         bCh = TRUE;
         break;
      }
   }

   if (bCh)
   {
      EME_XXToUtf8(pstEMsgHandle->pICSC,
                   pvMemMgr,
                   pcSrc,
                   EME_UTILS_STRLEN(pcSrc),
                   E_CHARSET_GB2312,
                   &pcOut,
                   &uiOutSize);

      return pcOut;
      //return EME_MailMsg_DecodeCharset(pstEMsgHandle,pcSrc, "gb2312");
   }
   else
   {
      return EME_UTILS_STRDUP(pvMemMgr,pcSrc);
   }
}


static WE_INT32 EME_MailMsg_ParseHDField(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn,
                                         WE_CHAR* pcHeaderField,
                                         WE_BOOL *pbRet)
{
   WE_INT32 iResult  = EME_SUCCESS; /*must be init success*/
   WE_BOOL   bTmp    = TRUE;
   WE_INT32 nFindPos = 0;
   void    *pvMemMgr = NULL;
   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcHeaderField || NULL == pbRet)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHDField(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if ( EME_UTILS_StrStrNC(pcHeaderField, "from:") != NULL )
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("from:", pcHeaderField)) != -1 )
         {           
            pstIn->pcFrom = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcHeaderField + nFindPos);
            EME_UTILS_BREAKNULL(pstIn->pcFrom,
               iResult,
               EME_FAILED,
               "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            /*EME_MailMsg_ParseFromField(pstEMsgHandle,pstIn->pcFrom, &pstIn->pcFromName, &pstIn->pcFromAddr);*/
            if (FALSE == EME_MailMsg_IsVaildUTF8(pstIn->pcFrom))
            {
               WE_CHAR * Temp = NULL;
               WE_UINT32 uiOutSize = 0;
               iResult = EME_XXToUtf8(pstEMsgHandle->pICSC,
                  pvMemMgr,
                  pstIn->pcFrom,
                  EME_UTILS_STRLEN(pstIn->pcFrom),
                  E_CHARSET_GBK,
                  &Temp,
                  &uiOutSize);
               if (iResult == SUCCESS )
               {
                  EME_UTILS_FREEIF(pvMemMgr,pstIn->pcFrom);
                  pstIn->pcFrom = Temp;
               }
            }
            break;
         }
      }
      
      if ( EME_UTILS_StrStrNC(pcHeaderField, "subject:") != NULL )
      {
         if ( (nFindPos = EME_MailMsg_FindPos("subject:", pcHeaderField)) != -1 )
         {
            WE_CHAR* pSubject = NULL;
            
            /*field end by "\r\n"*/  /*PB*/
            //             if (iSize > 2 ) 
            pSubject = EME_MailMsg_CheckCHString(pstEMsgHandle,pcHeaderField + nFindPos);
            pstIn->pcSubject = EME_MailMsg_LocalExpansion(pstEMsgHandle,pSubject); 
            EME_UTILS_FREEIF(pvMemMgr,pSubject);
#if 0
            if (FALSE == EME_MailMsg_IsVaildUTF8(pstIn->pcSubject))
            {
               WE_CHAR * Temp = NULL;
               WE_UINT32 uiOutSize = 0;
               iResult = EME_XXToUtf8(pstEMsgHandle->pICSC,
                  pvMemMgr,
                  pstIn->pcSubject,
                  EME_UTILS_STRLEN(pstIn->pcSubject),
                  E_CHARSET_GBK,
                  &Temp,
                  &uiOutSize);
               if (iResult == SUCCESS )
               {
                  EME_UTILS_FREEIF(pvMemMgr,pstIn->pcSubject);
                  pstIn->pcSubject = Temp;
               }
               
            }
#endif
            break;
         }
      }
      
      if ( EME_UTILS_StrStrNC(pcHeaderField, "date:") != NULL )
      {
         if ( (nFindPos = EME_MailMsg_FindPos("date:", pcHeaderField)) != -1 )
         {
            pstIn->pcDate = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcHeaderField + nFindPos);
//             EME_UTILS_BREAKNULL(pstIn->pcDate,
//                iResult,
//                EME_FAILED,
//                "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            if (NULL ==  pstIn->pcDate)
            {
               EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion Date failed!!!"));
            }
            break;
         }
      }

      if (EME_UTILS_StrStrNC(pcHeaderField, "to:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("to:", pcHeaderField)) != -1 )
         {
            WE_CHAR* pToList = NULL;
            pToList = EME_MailMsg_CheckCHString(pstEMsgHandle,pcHeaderField + nFindPos);
            pstIn->pcTo = EME_MailMsg_LocalExpansion(pstEMsgHandle,pToList);
//             EME_UTILS_BREAKNULL(pstIn->pcTo,
//                iResult,
//                EME_FAILED,
//                "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            if (NULL ==  pstIn->pcTo)
            {
               EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion TO failed!!!"));
            }
            EME_UTILS_FREEIF(pvMemMgr,pToList);
            break;
         }
      }
      
      if (EME_UTILS_StrStrNC(pcHeaderField, "cc:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("cc:", pcHeaderField)) != -1 )
         {
            WE_CHAR* pCcList = NULL;
            pCcList = EME_MailMsg_CheckCHString(pstEMsgHandle,pcHeaderField + nFindPos);
            pstIn->pcCc = EME_MailMsg_LocalExpansion(pstEMsgHandle,pCcList);
//             EME_UTILS_BREAKNULL(pstIn->pcCc,
//                iResult,
//                EME_FAILED,
//                "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            if (NULL ==  pstIn->pcCc)
            {
               EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion CC failed!!!"));
            }
            EME_UTILS_FREEIF(pvMemMgr,pCcList);
            break;
         }
      }
      
      if (EME_UTILS_StrStrNC(pcHeaderField, "bcc:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("bcc:", pcHeaderField)) != -1 )
         {
            pstIn->pcBcc = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcHeaderField + nFindPos);  
//             EME_UTILS_BREAKNULL(pstIn->pcBcc,
//                iResult,
//                EME_FAILED,
//                "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            if (NULL ==  pstIn->pcCc)
            {
               EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion CC failed!!!"));
            }
            break;
         }
      }
      
      if (EME_UTILS_StrStrNC(pcHeaderField, "reply-to:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("reply-to:", pcHeaderField)) != -1 )
         {
            pstIn->pcReplyTo = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcHeaderField + nFindPos);
            EME_UTILS_BREAKNULL(pstIn->pcReplyTo,
               iResult,
               EME_FAILED,
               "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            break;
         }
      }

      if (EME_UTILS_StrStrNC(pcHeaderField, "X-Priority:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("X-Priority:", pcHeaderField)) != -1 )
         {
            pstIn->pcPriority = EME_UTILS_STRDUP(pvMemMgr,pcHeaderField + nFindPos);
            EME_UTILS_BREAKNULL(pstIn->pcPriority,
               iResult,
               EME_FAILED,
               "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            break;
         }
      }

      if(EME_UTILS_StrStrNC(pcHeaderField, "Importance:") != NULL)
      {
         if ( (nFindPos = EME_MailMsg_FindPos("Importance:", pcHeaderField)) != -1 )
         {
            WE_CHAR *pcImport = EME_UTILS_STRDUP(pvMemMgr,pcHeaderField + nFindPos);
            if(NULL != pcImport)
            {
               if(EME_UTILS_StrStrNC(pcImport, "High") != NULL)
               {
                  pstIn->pcPriority = EME_UTILS_STRDUP(pvMemMgr, "1");
               }
               else if(EME_UTILS_StrStrNC(pcImport, "Low") != NULL)
               {
                  pstIn->pcPriority = EME_UTILS_STRDUP(pvMemMgr, "5");
               }
               else
               {
                  pstIn->pcPriority = EME_UTILS_STRDUP(pvMemMgr, "3");
               }
               EME_UTILS_FREEIF(pvMemMgr, pcImport);
            }
            break;
         }
      }

      if (EME_UTILS_StrStrNC(pcHeaderField, "Disposition-Notification-To:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("Disposition-Notification-To:", pcHeaderField)) != -1 )
         {
            WE_CHAR* pNotifyToList = NULL;
            pNotifyToList = EME_MailMsg_CheckCHString(pstEMsgHandle,pcHeaderField + nFindPos);
            pstIn->pcNotifyTo = EME_MailMsg_LocalExpansion(pstEMsgHandle,pNotifyToList);
            EME_UTILS_BREAKNULL(pstIn->pcNotifyTo,
               iResult,
               EME_FAILED,
               "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            EME_UTILS_FREEIF(pvMemMgr,pNotifyToList);
            break;
         }
      }

      if (EME_UTILS_StrStrNC(pcHeaderField, "mime-version:") != NULL)
      {       
         if ( (nFindPos = EME_MailMsg_FindPos("mime-version:", pcHeaderField)) != -1 )
         {
            pstIn->pcMimeVersion = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcHeaderField + nFindPos);
            EME_UTILS_BREAKNULL(pstIn->pcMimeVersion,
               iResult,
               EME_FAILED,
               "EME_MailMsg_ParseHDField,EME_MailMsg_LocalExpansion malloc failed!!!");
            break;
         }
      }
      
      if ( (nFindPos = EME_MailMsg_FindPos("content-type:", pcHeaderField)) != -1 )
      {
         if (EME_UTILS_STRSTR(pcHeaderField + nFindPos, "multipart/related"))
         {
            pstIn->bMultipartRelated = TRUE;
         }
         else if (EME_UTILS_STRSTR(pcHeaderField + nFindPos, "text/html"))
         {
            pstIn->bIsHtml = TRUE;
         }
         else if (EME_UTILS_STRSTR(pcHeaderField + nFindPos, "text/plain"))
         {
            pstIn->bIsHtml = FALSE;
         }
         else
         {
            pstIn->bIsAttachment = TRUE;
         }
         /* Don't return, fall through to get the boundary marker. */
      }
      
      /* if there are charset info in header record it */ 
      if ( (nFindPos = EME_MailMsg_FindHdFieldPos("charset=", pcHeaderField)) != -1 )
      {
         WE_CHAR* pEnd = NULL;
         WE_CHAR * pcCharset = NULL;
         if ( (pEnd = EME_UTILS_STRCHR(pcHeaderField + nFindPos, '\n')) != NULL )
         {
            if (!pstIn->bIsHtml)
            {
               pstIn->pcPlainCharset = EME_UTILS_MALLOC(pvMemMgr,pEnd - pcHeaderField - nFindPos + 1);
               EME_UTILS_BREAKNULL(pstIn->pcPlainCharset,
                        iResult,
                        EME_NOMEMORY,
                        "pstIn->pcPlainCharset malloc failed!!!");
               EME_UTILS_MEMSET(pstIn->pcPlainCharset,0x00,pEnd - pcHeaderField - nFindPos + 1);
               EME_UTILS_MEMMOVE(pstIn->pcPlainCharset, pcHeaderField + nFindPos, pEnd - pcHeaderField - nFindPos);
            }
            else
            {
               pstIn->pcHtmlCharset = EME_UTILS_MALLOC(pvMemMgr,pEnd - pcHeaderField - nFindPos + 1);
               EME_UTILS_BREAKNULL(pstIn->pcHtmlCharset,
                        iResult,
                        EME_NOMEMORY,
                        "pstIn->pcHtmlCharset malloc failed!!!");
               EME_UTILS_MEMSET(pstIn->pcHtmlCharset,0x00,pEnd - pcHeaderField - nFindPos + 1);
               EME_UTILS_MEMMOVE(pstIn->pcHtmlCharset, pcHeaderField + nFindPos, pEnd - pcHeaderField - nFindPos);
            }

            pcCharset = (pstIn->pcPlainCharset != NULL ? pstIn->pcPlainCharset : pstIn->pcHtmlCharset);
            if (NULL == EME_UTILS_STRCHR(pcCharset,'\"')  )
            {
               break;
            }
            else
            {
               EME_MailMsg_UnquoteString(pstEMsgHandle,pcCharset, '\"', '\"');
               break;
            }
            
         }       
      }
      
      if ( (nFindPos = EME_MailMsg_FindHdFieldPos("boundary=", pcHeaderField)) != -1 )
      {
         WE_CHAR* pEnd = NULL;
         pstIn->pcBoundary = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcHeaderField + nFindPos) + 3);/* +3 means "--" + '\0' */
         EME_UTILS_BREAKNULL(pstIn->pcBoundary,
                  iResult,
                  EME_NOMEMORY,
                  "EME_MailMsg_ParseHDField  malloc failed!!!");
         EME_UTILS_MEMSET(pstIn->pcBoundary ,0x00,EME_UTILS_STRLEN(pcHeaderField + nFindPos) + 3);
         if ( (pEnd = EME_UTILS_STRCHR(pcHeaderField + nFindPos, '\n')) != NULL )
         {
            EME_UTILS_STRCPY(pstIn->pcBoundary, "--");
            EME_UTILS_MEMMOVE(pstIn->pcBoundary + 2, pcHeaderField + nFindPos, pEnd - pcHeaderField - nFindPos);
            EME_MailMsg_UnquoteString(pstEMsgHandle,pstIn->pcBoundary + 2, '\"', '\"');
         } 
         break;
      }
      
      if ( (nFindPos = EME_MailMsg_FindPos("content-transfer-encoding:", pcHeaderField)) != -1 )
      {
         pstIn->eTransEncoding = EME_MailMsg_ParseTransEncoding(pcHeaderField + nFindPos);
         break;
      }
//       else
//       {
//          pstIn->eTransEncoding = E_ENCODETYPE_UNSET;
//       }

      bTmp = FALSE;
      
   } while(0);
  
   *pbRet = bTmp;

   return iResult;
}


E_EncodeType EME_MailMsg_ParseTransEncoding(WE_CHAR* pcTransEncoding)
{
   /*default of Content-Transfer-Encoding: "7bit"*/
   E_EncodeType eEncodeType = E_ENCODETYPE_UNSET; 
   
   if (pcTransEncoding == NULL)
   {
      return eEncodeType;
   }
   
   
   if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_8BIT) != NULL)
   {
      eEncodeType = E_ENCODETYPE_8BIT;
   }
   else if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_BASE64) != NULL)
   {
      eEncodeType = E_ENCODETYPE_BASE64;
   }
   else if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_QUOTED_PRINTABLE) != NULL)
   {
      eEncodeType = E_ENCODETYPE_QUOTEDPRINTABLE;
   }
   else if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_7BIT) != NULL)
   {
      eEncodeType = E_ENCODETYPE_7BIT;
   }
   else if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_UUENCODE) != NULL)
   {
      eEncodeType = E_ENCODETYPE_UUENCODE;
   }
   else if (EME_UTILS_STRSTRNC(pcTransEncoding, EME_TRANS_ENC_BINHEX) != NULL)
   {
      eEncodeType = E_ENCODETYPE_BINHEX;
   }
   else
   {
      eEncodeType = E_ENCODETYPE_UNKNOW;
   }

   return eEncodeType;
}

E_Charset EME_MailMsg_ParseChaset(WE_CHAR *pcBuf, WE_CHAR* pcCharset)
{
   /*default of charset: "us-ascii"*/
   E_Charset eCharset = E_CHARSET_US_ASCII; 

   if (pcCharset == NULL && pcBuf == NULL)
   {
      return eCharset;
   }
   else if (pcCharset == NULL && pcBuf != NULL)
   {
      while (*pcBuf != '\0')
      {
         if ((WE_UCHAR)(*pcBuf) >= 0x80)
         {
            eCharset = E_CHARSET_GB2312;
            break;
         }
         pcBuf++;
      }
      return eCharset;
   }
   
   if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_UTF8) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_UTF8_G))
   {
      eCharset = E_CHARSET_UTF8;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_US_ASCII) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_US_ASCII_G))
   {
      eCharset = E_CHARSET_US_ASCII;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_UTF16) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_UTF16_G))
   {
      eCharset = E_CHARSET_UTF16;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_BIG5) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_BIG5_G))
   {
      eCharset = E_CHARSET_BIG5;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GB2312) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GB2312_G))
   {
      eCharset = E_CHARSET_GB2312;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_HZ_GB_2312) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_HZ_GB_2312_G))
   {
      eCharset = E_CHARSET_HZ_GB_2312;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_ISO_8859_1) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_ISO_8859_1_G))
   {
      eCharset = E_CHARSET_ISO_8859_1;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GB18030) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GB18030_G))
   {
      eCharset = E_CHARSET_GB18030;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GBK) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_GBK_G))
   {
      eCharset = E_CHARSET_GBK;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_KOI8R) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_KOI8R_G))
   {
      eCharset = E_CHARSET_KOI8R;
   }
   else if (EME_UTILS_STRSTR(pcCharset, EME_CHARSET_WIN1252) || EME_UTILS_STRSTR(pcCharset, EME_CHARSET_WIN1252_G))
   {
      eCharset = E_CHARSET_WIN1251;
   }
   else
   {
      eCharset = E_CHARSET_UNKNOW;
   }

   return eCharset;
}
/* unquote the string between cUnquote1 and cUnquote2 */
/*
e.g  pstr = "234noted47"
     cUnquote1 = 'n'
     cUnquote2 = '4'
     ----->
     pstr = "noted4"
*/
static WE_INT32 EME_MailMsg_UnquoteString(stEMSGHandle *pstEMsgHandle,
                                          WE_CHAR* pcSrc,
                                          WE_CHAR cUnquote1, 
                                          WE_CHAR cUnquote2)
{
   WE_INT32  iResult       = EME_FAILED; 
   WE_UINT32 uiBegin       = 0;
   WE_UINT32 uiEnd         = 0;
   WE_UINT32 uiLenTmp      = 0;
   void      *pvMemMgr     = NULL;
   WE_UINT32 uiSrcLen      = 0;

   if (NULL == pcSrc || '\0' == cUnquote1 || '\0' == cUnquote2)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_UnquoteString(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      uiSrcLen = EME_UTILS_STRLEN(pcSrc);
      while ( pcSrc[uiBegin++] != cUnquote1 && uiBegin < uiSrcLen ) 
      {
         /*do nothing here*/
      }
      while ( pcSrc[uiSrcLen - 1 - uiEnd++] != cUnquote2 && uiEnd < uiSrcLen) 
      {
         /*do nothing here*/
      }
      
      if (uiBegin + uiEnd >= uiSrcLen)
      {
         iResult = EME_FAILED;
         EME_UTILS_BREAK(iResult,"EME_MailMsg_UnquoteString,exceed the size of pStr!!!! ");
      }
      else
      {
         WE_CHAR* pcReserve = EME_UTILS_MALLOC(pvMemMgr,uiSrcLen + 1);
         EME_UTILS_BREAKNULL(pcReserve,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_UnquoteString,malloc failed!!!!");
         EME_UTILS_MEMSET(pcReserve,0x00,uiSrcLen + 1);
         
         uiLenTmp = &pcSrc[uiSrcLen - uiEnd] - &pcSrc[uiBegin];
         EME_UTILS_MEMMOVE(pcReserve, &pcSrc[uiBegin], uiLenTmp);
         pcReserve[uiLenTmp + 1] = '\0';
         
         EME_UTILS_MEMCPY(pcSrc, pcReserve,uiLenTmp+1);
         EME_UTILS_FREEIF(pvMemMgr,pcReserve);
         iResult = EME_SUCCESS;
      }

   } while(0);
   
   return iResult;
}


static WE_INT32 EME_MailMsg_ParseHeaders(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg* pstIn,
                                         WE_CHAR* pcHeaderStart, 
                                         WE_CHAR* pcHeaderEnd)
{
   WE_INT32 iResult                     = EME_SUCCESS; /*must be init success*/
   WE_CHAR  *pcPos                      = NULL;
   WE_CHAR  *pcLineEnd                  = NULL;
   WE_CHAR  *pcFieldEndPos              = NULL;
   WE_BOOL  bContinue                   = FALSE;
   WE_BOOL  bRet                        = FALSE;
   WE_CHAR  *pcHeaderField              = NULL;
   WE_CHAR  *pcFieldHeap                = NULL;
   WE_CHAR  acFieldStack[EME_MAXSTRLEN]  = {0};
   void     *pvMemMgr                   = NULL;

   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcHeaderStart 
      || NULL == pcHeaderEnd || pcHeaderEnd < pcHeaderStart )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseHeaders(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   pcPos = pcLineEnd = pcFieldEndPos = pcHeaderStart;
   do 
   {
      
      while ( pcHeaderEnd - pcLineEnd > 0 )
      {
         do 
         {
            bContinue = FALSE;
            if ( (pcFieldEndPos = EME_UTILS_STRSTR(pcLineEnd, "\n")) == NULL )
            {
               pcFieldEndPos = pcHeaderEnd - 1;
            }
            
            pcLineEnd = pcFieldEndPos + 1;
            
            if (pcLineEnd != pcHeaderEnd)
            {
               if ( (*pcLineEnd) == ' ' || (*pcLineEnd) == '\t' )
               {
                  bContinue = TRUE;
               }
            }
         } while(bContinue);
         
         if (pcLineEnd - pcPos + 1 > EME_MAXSTRLEN)
         {
            pcFieldHeap = (WE_CHAR*)EME_UTILS_MALLOC(pvMemMgr,pcLineEnd - pcPos + 1);
            EME_UTILS_BREAKNULL(pcFieldHeap,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParseHeaders malloc failed!!!");
            
            EME_UTILS_MEMSET(pcFieldHeap,0x00,pcLineEnd - pcPos + 1);
            pcHeaderField = pcFieldHeap;
         }
         else
         {
            pcHeaderField = acFieldStack;
         }
         
         EME_UTILS_MEMMOVE(pcHeaderField, pcPos, pcLineEnd - pcPos);
         pcHeaderField[pcLineEnd - pcPos] = '\0'; 
         
         iResult = EME_MailMsg_ParseHDField(pstEMsgHandle,pstIn, pcHeaderField, &bRet);
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseHeaders,EME_MailMsg_ParseHDField failed!!!!");
         
         if ( !bRet )       
         {         
            EME_MailMsg_AppendToExFIeld(pstIn, pcHeaderField); /* the function do nothing */ 
         }
         else
         {
            iResult = EME_MailMsg_AppendHeader(pstEMsgHandle,pstIn, pcHeaderField);  
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseHeaders,EME_MailMsg_AppendHeader failed!!!!");
         }
         
         EME_UTILS_FREEIF(pvMemMgr,pcFieldHeap);
         pcPos = pcLineEnd;
      } 
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseHeaders,failed!!!!");
      
      
   } while(0);
   
   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pcFieldHeap);
   }
   
   return iResult;
}


static WE_INT32 EME_MailMsg_ParseMsgBody(stEMSGHandle *pstEMsgHandle,
                                         St_EmailMsg *pstIn,
                                         WE_CHAR *pcBodyPartBuf)  
{ 
   WE_INT32 iResult      = EME_SUCCESS ; /*must be init success*/
   void    *pvMemMgr     = NULL;
   WE_CHAR *pcBodyBuf    = pcBodyPartBuf;
  
   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcBodyBuf )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseMsgBody(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if (pstIn->pcBoundary != NULL )
      {
         iResult = EME_MailMsg_ParseMimePart(pstEMsgHandle,pstIn, pcBodyBuf, pstIn->pcBoundary);
         if (iResult != EME_NOMEMORY)
         {
            iResult = EME_SUCCESS;
         }
         EME_UTILS_BREAKIF(iResult, "no memory");
      }

      else /* just save the text body. */
      {
         WE_CHAR* pcDecodedWord = NULL;
         if (EME_UTILS_STRSTR(pcBodyBuf, "\n.\n") != NULL)
         {
            EME_UTILS_MEMSET(pcBodyBuf + EME_UTILS_STRLEN(pcBodyBuf) - 3, 0, 3);
         }
         
         pcDecodedWord = EME_MailMsg_DecodeBody(pstEMsgHandle,pcBodyBuf, EME_UTILS_STRLEN(pcBodyBuf), pstIn->eTransEncoding, NULL);
         if (!pstIn->bIsHtml)
         {
            pstIn->pcPlainTransEnc = EME_GetTransEncoding(pvMemMgr, pstIn->eTransEncoding);
            pstIn->pcTextPlain = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcPlainCharset);
         }
         else
         {
            pstIn->pcHtmlTransEnc = EME_GetTransEncoding(pvMemMgr, pstIn->eTransEncoding);
            pstIn->pcTextHtml = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcHtmlCharset);

         }
        
         EME_UTILS_FREEIF(pvMemMgr,pcDecodedWord);
      }
      

   } while(0);
      
   return iResult ;
}

/*
* The function first convert other charset to utf16 and then convert it to utf8,
* in order to we can save it by WE_CHAR*. So the returned value is utf8.
*/ 
// WE_CHAR* EME_MailMsg_DecodeCharset(stEMSGHandle *pstEMsgHandle,
//                                    WE_CHAR* pcSrc, 
//                                    WE_CHAR* pcCharset)
// {
//    
//    WE_CHAR   *pcRetVal   = NULL;
//    WE_UINT16 *pusTrans   = NULL; /*using for GB2312 to uint16*/
//    void      *pvMemMgr   = NULL;
// 
//    if (NULL == pstEMsgHandle || NULL == pcCharset || NULL == pcSrc)
//    {
//       EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeCharset(),input parameters is invalid!!!!"));
//       return NULL;
//    }
// 
//    pvMemMgr = pstEMsgHandle->pvMemMgr;
//    do 
//    {
//       if ( EME_UTILS_StrStrNC(pcCharset, "utf-8") != NULL
//          || EME_UTILS_StrStrNC(pcCharset, "iso-8859-1") != NULL ) /* iso8859 is compatable with utf-8 */
//       {
//          pcRetVal = EME_UTILS_STRDUP(pvMemMgr,pcSrc);
//          if (NULL == pcRetVal)
//          {
//             EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeCharset,EME_UTILS_STRDUP,failed!!!!"));
//          }
//          break;
//       }
//       else if ( EME_UTILS_StrStrNC(pcCharset, "utf-7") != NULL )
//       {
//          pcRetVal = EME_UTILS_STRDUP(pvMemMgr,pcSrc);
//          if (NULL == pcRetVal)
//          {
//             EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeCharset,EME_UTILS_STRDUP,failed!!!!"));
//          }
//          break;
// 
//       }
//       else
//       {
//          pusTrans = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcSrc) * 2 + 2 ); /* +2 means '\0' in wide WE_CHAR */
//          if (NULL == pusTrans)
//          {
//             EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeCharset,malloc failed!!!!"));
//             break;
//          }
//          EME_UTILS_MEMSET(pusTrans,0x00,EME_UTILS_STRLEN(pcSrc) * 2 + 2);
// 
//          if (EME_UTILS_StrStrNC(pcCharset, "big5") != NULL)
//          {
//             /* ??? jiangdingqing */
//             
// 
//             //EME_Big5ToUcs2(pcSrc, pusTrans);
//          }
//          else
//          {
//             /* ??? jiangdingqing */
//             //EME_GbToUcs2((const uint8 *)pcSrc, pusTrans);
//          }
// 
//          pcRetVal = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_WSTRLEN(pusTrans) * 3 + 1); /* +1 means '\0' */ 
//          if (NULL == pcRetVal)
//          {
//             EME_UTILS_FREEIF(pvMemMgr,pusTrans);
//             EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeCharset,pcRetVal malloc failed!!!!"));
//             break;
//          }
//          
//          EME_UTILS_MEMSET(pcRetVal,0x00,EME_UTILS_WSTRLEN(pusTrans) * 3 + 1);
//          EME_Ucs2ToUtf8(pusTrans, (uint8*)pcRetVal);
//          EME_UTILS_FREEIF(pvMemMgr,pusTrans);
// 
//       }
// 
//    } while(0);
//    
//    
//    return pcRetVal;
// }


/* convert all charset to utf16 */ 
static WE_CHAR* EME_MailMsg_ParseBodyCharset(stEMSGHandle *pstEMsgHandle,
                                               WE_CHAR* pcWord,
                                               WE_CHAR* pcCharset)
{
   void       *pvMemMgr          = NULL;
   E_Charset eCharset   = E_CHARSET_UTF8;
   WE_UINT32 uiOutSize  = 0;
   WE_CHAR *pcOut = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pcWord)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseBodyCharset(),input parameters is invalid!!!!"));
      return NULL;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      eCharset = EME_MailMsg_ParseChaset(pcWord, pcCharset);

      if(eCharset != E_CHARSET_UNKNOW)
      {
         EME_XXToUtf8(pstEMsgHandle->pICSC,
                   pvMemMgr,
                   pcWord,
                   EME_UTILS_STRLEN(pcWord),
                   eCharset,
                   (WE_CHAR**)&pcOut,
                   &uiOutSize);
      }
      else
      {
         pcOut = EME_UTILS_STRDUP(pvMemMgr, pcWord);
      }

   } while(0);
  
   return pcOut;
}


static WE_CHAR* EME_MailMsg_ParseSpecName(stEMSGHandle *pstEMsgHandle,WE_CHAR* pcSpecName)
{
   WE_CHAR *pcDelim     = NULL;
   WE_CHAR *pcCharset   = NULL;
   WE_CHAR *pcTrans     = NULL;
   WE_CHAR *pcRet       = NULL;
   void    *pvMemMgr    = NULL;
   WE_INT32 iResult     = EME_SUCCESS;
   E_Charset eCharset   = E_CHARSET_UTF8;
   WE_UINT32 uiOutSize  = 0;

   if (NULL == pstEMsgHandle || NULL == pcSpecName)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseSpecName(),input parameters is invalid!!!!"));
      return NULL;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pcDelim = EME_UTILS_STRSTR(pcSpecName, "\'\'");
      if (NULL != pcDelim)
      {
         WE_CHAR* pcTmp = NULL;
         WE_UINT8 nH, nL; /* high 4, and low 4 bit */ 
         
         pcCharset = EME_UTILS_MALLOC(pvMemMgr,pcDelim - pcSpecName + 1);
         EME_UTILS_BREAKNULL(pcCharset,
                  iResult,
                  EME_NOMEMORY,
                  "EME_MailMsg_ParseSpecName pcCharset malloc failed!!!");

         EME_UTILS_MEMSET(pcCharset,0x00,pcDelim - pcSpecName + 1);
         EME_UTILS_MEMMOVE(pcCharset, pcSpecName, pcDelim - pcSpecName);

         pcTrans = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcSpecName) * 2);
         EME_UTILS_BREAKNULL(pcTrans,
                  iResult,
                  EME_NOMEMORY,
                  "EME_MailMsg_ParseSpecName :pcTrans malloc failed!!!");
                  
         EME_UTILS_MEMSET(pcTrans,0x00,EME_UTILS_STRLEN(pcSpecName) * 2);
         pcDelim += 2; /* go behind "\'\'" */
         pcTmp = pcTrans;
         
         while ( '\0' != *pcDelim )
         {
            if (*pcDelim == '%' )
            {
               pcDelim++;
               continue;
            }       
            
            nH = EME_HexToDec(*pcDelim++);
            nL = EME_HexToDec(*pcDelim++);
            *pcTmp++ = (nH << 4) + nL;
            
            while ( *pcDelim && *pcDelim != '%')
               *pcTmp++ = *pcDelim++;
         }

         eCharset = EME_MailMsg_ParseChaset(pcTrans, pcCharset);
         EME_XXToUtf8(pstEMsgHandle->pICSC,
                      pvMemMgr,
                      pcTrans,
                      EME_UTILS_STRLEN(pcTrans),
                      eCharset,
                      &pcRet,
                      &uiOutSize);         
         //pcRet = EME_MailMsg_DecodeCharset(pstEMsgHandle,pcTrans, pcCharset);
         EME_UTILS_BREAKNULL(pcRet,
                  iResult,
                  EME_FAILED,
                  "EME_MailMsg_ParseSpecName, EME_MailMsg_DecodeCharset Failed!!!");
      }

      EME_UTILS_FREEIF(pvMemMgr,pcCharset);
      EME_UTILS_FREEIF(pvMemMgr,pcTrans);

   } while(0);
   
    if (EME_SUCCESS != iResult)
    {
       EME_UTILS_FREEIF(pvMemMgr,pcCharset);
       EME_UTILS_FREEIF(pvMemMgr,pcTrans);
       EME_UTILS_FREEIF(pvMemMgr,pcRet);
    }
   
   return pcRet;
}


static WE_INT32 EME_MailMsg_ParseMimePart(stEMSGHandle *pstEMsgHandle,
                                          St_EmailMsg *pstIn,
                                          WE_CHAR* pcBodyData, 
                                          WE_CHAR* pcBoundary)
{
   WE_INT32 iResult           = EME_SUCCESS; /*must be init success*/
   WE_CHAR* pcFindPos         = NULL;
   WE_INT32 iStartPos         = -1;
   WE_INT32 iEndPos           = -1;  
   WE_CHAR* pcName            = NULL;
   WE_CHAR* pcFileName        = NULL;
   WE_CHAR* pcContType        = NULL;
   WE_CHAR* pcMimeHeader      = NULL;
   WE_CHAR* pcMimeBody        = NULL;
   WE_CHAR* pcBoundaryVal     = NULL;
   WE_CHAR* pcContId          = NULL;
//    WE_CHAR* pcContLocat       = NULL;
   void   *pvMemMgr           = NULL;
   WE_BOOL havePartsAlready   = 0;
   WE_CHAR *pcTranEncoding    = NULL;
   E_EncodeType eEncoding     = E_ENCODETYPE_8BIT;
   WE_UINT32   uiBodySize     = 0;


   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcBodyData || NULL == pcBoundary)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseMimePart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   while ( EME_UTILS_STRLEN(pcBodyData) > 0 )
   {
      if ((pcFindPos = EME_UTILS_STRSTR(pcBodyData, pcBoundary)) != NULL)
      {
         WE_CHAR* pcTemp = pcFindPos;
         pcTemp = EME_UTILS_STRSTR(pcFindPos, "\n\n");
         if (pcTemp == NULL)
         {
            if ((pcTemp = EME_UTILS_STRSTR(pcFindPos, "\r\n\r\n")) != NULL)
            {
               pcFindPos = pcTemp;
               pcFindPos += 4;
            }
         }
         else
         {
            pcFindPos = pcTemp;
            pcFindPos += 2;
         }
         
      }       
      
      if (pcFindPos == NULL || *pcFindPos == '\0')
      { 
         iResult = EME_SUCCESS;
         break;
      }
      
      pcMimeHeader = EME_UTILS_MALLOC(pvMemMgr,pcFindPos - pcBodyData + 1);
      EME_UTILS_BREAKNULL(pcMimeHeader,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParseMimePart, pcMimeHeader malloc failed!!!");

      EME_UTILS_MEMSET(pcMimeHeader,0x00,pcFindPos - pcBodyData + 1);
      EME_UTILS_MEMMOVE(pcMimeHeader, pcBodyData, pcFindPos - pcBodyData);
      pcMimeHeader[pcFindPos - pcBodyData] = '\0';        
           
      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "name=") - pcMimeHeader) > 0)
      {
         WE_CHAR* pcTemp = NULL;
         iStartPos += 5; /* +"name=" */ 
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
        
         pcTemp = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcTemp,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");

         EME_UTILS_MEMSET(pcTemp,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTemp, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcTemp, '\"', '\"');
         pcName = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcTemp);

         EME_UTILS_FREEIF(pvMemMgr,pcTemp);  
         EME_UTILS_BREAKNULL(pcName,
                  iResult,
                  EME_FAILED,
                  "EME_MailMsg_ParseMimePart EME_MailMsg_LocalExpansion, malloc failed!!!");
      }
      
      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "filename=") - pcMimeHeader) > 0)
      {
         WE_CHAR* pcTemp = NULL;
         iStartPos += 9; /* +"filename=" */ 
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
         
         pcTemp = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcTemp,
            iResult,
            EME_NOMEMORY,
            " malloc failed!!!");
         EME_UTILS_MEMSET(pcTemp,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTemp, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcTemp, '\"', '\"');
         pcFileName = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcTemp);

         EME_UTILS_FREEIF(pvMemMgr,pcTemp);
         EME_UTILS_BREAKNULL(pcFileName,
            iResult,
            EME_FAILED,
            "EME_MailMsg_ParseMimePart EME_MailMsg_LocalExpansion, malloc failed!!!");
      }
           
      /* following name and filename is used for Thunder bird encode */ 
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "name*=") - pcMimeHeader) > 0 )
      {
         WE_CHAR* pcWord = NULL;
         iStartPos += 6;
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
         pcWord = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcWord,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");

         EME_UTILS_MEMSET(pcWord,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcWord, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         if (pcName == NULL)
         {
            pcName = EME_MailMsg_ParseSpecName(pstEMsgHandle,pcWord);
         }           
         EME_UTILS_FREEIF(pvMemMgr,pcWord);
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "filename*=") - pcMimeHeader) > 0 )
      {
         WE_CHAR* pcWord = NULL;
         iStartPos += 10;
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
         pcWord = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcWord,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");
         EME_UTILS_MEMSET(pcWord,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcWord, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         if (pcFileName == NULL)
         {
            pcFileName = EME_MailMsg_ParseSpecName(pstEMsgHandle,pcWord);
         }           
         EME_UTILS_FREEIF(pvMemMgr,pcWord);
      }
      /* uiEnd of name*= and filename*= */ 
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-type:") - pcMimeHeader) > 0 )
      {
         WE_INT32 iEndPos     = -1;
         WE_INT32 iEndPosTmp  = -1;
         
         iStartPos += 13; /* +"content-type:" */ 
         
         iEndPos = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, ';') - pcMimeHeader;
         iEndPosTmp = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, '\n') - pcMimeHeader;
         if (iEndPos < 0 && iEndPosTmp < 0)
         {
            iResult = EME_FAILED;
            break;
         }

         else if (iEndPosTmp > 0)
         {
            if (iEndPos < 0 || iEndPosTmp < iEndPos)
            {
               iEndPos = iEndPosTmp;
            }
         }
         while (pcMimeHeader[iStartPos] == ' ') iStartPos++;
         
         pcContType = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcContType,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");
         EME_UTILS_MEMSET(pcContType,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcContType, pcMimeHeader + iStartPos, iEndPos - iStartPos);            
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "Content-Transfer-Encoding:") - pcMimeHeader) > 0 )
      {
         iStartPos += 9;
         iEndPos = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, '\n') - pcMimeHeader;
         pcTranEncoding = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcTranEncoding,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");
         EME_UTILS_MEMSET(pcTranEncoding,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTranEncoding, pcMimeHeader + iStartPos, iEndPos - iStartPos );

         eEncoding = EME_MailMsg_ParseTransEncoding(pcTranEncoding);
         EME_UTILS_FREEIF(pvMemMgr, pcTranEncoding);
         pcTranEncoding = EME_GetTransEncoding(pvMemMgr, eEncoding);

         if (NULL != EME_UTILS_StrStrNC(pcMimeHeader, "text/plain"))
         {
            pstIn->pcPlainTransEnc = EME_UTILS_STRDUP(pvMemMgr, pcTranEncoding);
         }
         
         else if (NULL != EME_UTILS_StrStrNC(pcMimeHeader, "text/html"))
         {
            pstIn->pcHtmlTransEnc = EME_UTILS_STRDUP(pvMemMgr, pcTranEncoding);
         }
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-id:") - pcMimeHeader) > 0 )
      {            
         iStartPos += 11; /* +"content-id:" */ 
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
         pcContId = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcContId,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");

         EME_UTILS_MEMSET(pcContId,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcContId, pcMimeHeader + iStartPos, iEndPos - iStartPos);            
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcContId, '<', '>');            
      }
      
//       if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-location:") - pcMimeHeader) > 0 )
//       {            
//          iStartPos += 17; /* +"content-location:" */ 
//          iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
//          while (pcMimeHeader[iStartPos] == ' ') iStartPos++;
//          while (pcMimeHeader[iEndPos] == ' ') iEndPos--;
//          pcContLocat = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
//          EME_UTILS_BREAKNULL(pcContLocat,
//                   iResult,
//                   EME_NOMEMORY,
//                   " malloc failed!!!");
//          EME_UTILS_MEMSET(pcContLocat,0x00,iEndPos - iStartPos + 1);
//          EME_UTILS_MEMMOVE(pcContLocat, pcMimeHeader + iStartPos, iEndPos - iStartPos);            
//       }
      
      pcBoundaryVal = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcBoundary) + 2); /* +2 mean + '\0' and '\n' */
      EME_UTILS_BREAKNULL(pcBoundaryVal,
               iResult,
               EME_NOMEMORY,
               " malloc failed!!!");
      EME_UTILS_MEMSET(pcBoundaryVal,0x00,EME_UTILS_STRLEN(pcBoundary) + 2);
      EME_UTILS_STRCAT(pcBoundaryVal, "\n");
      EME_UTILS_STRCAT(pcBoundaryVal, pcBoundary);
      
      /* find next boundary and calculate this body size. */
      iStartPos = EME_UTILS_STRSTR(pcFindPos, pcBoundaryVal) - pcFindPos;
      EME_UTILS_FREEIF(pvMemMgr,pcBoundaryVal);
      
      if (iStartPos > 0)
      {
//          pcMimeBody = EME_UTILS_MALLOC(pvMemMgr,iStartPos + 1);
//          EME_UTILS_BREAKNULL(pcMimeBody,
//                   iResult,
//                   EME_NOMEMORY,
//                   " malloc failed!!!");
//          EME_UTILS_MEMSET(pcMimeBody,0x00,iStartPos + 1);
//          EME_UTILS_MEMMOVE(pcMimeBody, pcFindPos, iStartPos);
         pcMimeBody = pcFindPos;
         uiBodySize = iStartPos;
      }

      else
      {  
         /*don't use the pcMimeHeader */
         EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
         iResult = EME_SUCCESS;
         break;
      }
      
      
      /* parse over, uiBegin set each field. */     
      if (pcFileName && EME_UTILS_STRLEN(pcFileName) > 0)
      {
         St_EmailMsgPart *pstEMsgPart = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_EmailMsgPart));
         EME_UTILS_BREAKNULL(pstEMsgPart,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");

         EME_UTILS_MEMSET(pstEMsgPart,0x00, sizeof(St_EmailMsgPart));
         
         pstEMsgPart->pcContentType       = pcContType;
         pstEMsgPart->pcName              = pcName;
         pstEMsgPart->pcFileName          = pcFileName;
         pstEMsgPart->pcTransEnc          = pcTranEncoding;
         pstEMsgPart->pcOnePartData       = pcMimeBody;
         pstEMsgPart->pcContentId         = pcContId;
         pstEMsgPart->uiOnePartSize       = uiBodySize;
//         pstEMsgPart->pcContentLoc        = pcContLocat;
         
         EME_MailMsg_AddMsgPart(pstEMsgHandle,pstIn, pstEMsgPart);
         havePartsAlready = 1;
         pcContType = pcName = pcFileName = pcContId = NULL;
         pcBodyData = pcFindPos + iStartPos;/* go to next pstEMsgPart. */
         EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
         continue;
      }
      else if ( pcContType && EME_UTILS_StrStrNC(pcContType, "text") )
      {
         WE_CHAR* pcContText = NULL;
         WE_INT32 nFindPos   = 0;
         
         if ( (nFindPos = EME_UTILS_StrStrNC(pcMimeHeader, "charset=") - pcMimeHeader) > 0 )
         {               
            nFindPos += 8;
            iEndPos = EME_UTILS_STRCHR(pcMimeHeader + nFindPos, '\n') - pcMimeHeader;
            if (iEndPos > 0)
            {
               if (EME_UTILS_StrStrNC(pcContType, "text/plain"))
               {
                  pstIn->pcPlainCharset = EME_UTILS_MALLOC(pvMemMgr,iEndPos - nFindPos);
                  EME_UTILS_BREAKNULL(pstIn->pcPlainCharset,
                           iResult,
                           EME_NOMEMORY,
                           "pstIn->pcPlainCharset malloc failed!!!");
                  EME_UTILS_MEMSET(pstIn->pcPlainCharset,0x00,iEndPos - nFindPos);
                  EME_UTILS_MEMMOVE(pstIn->pcPlainCharset, pcMimeHeader + nFindPos, iEndPos - nFindPos -1); 
                  if (NULL != EME_UTILS_STRCHR(pstIn->pcPlainCharset,'\"')  )
                  {
                     EME_MailMsg_UnquoteString(pstEMsgHandle,pstIn->pcPlainCharset, '\"', '\"');
                  }
                  
               }

               else if (EME_UTILS_StrStrNC(pcContType, "text/html"))
               {
                  pstIn->pcHtmlCharset = EME_UTILS_MALLOC(pvMemMgr,iEndPos - nFindPos);
                  EME_UTILS_BREAKNULL(pstIn->pcHtmlCharset,
                           iResult,
                           EME_NOMEMORY,
                           "pstIn->pcHtmlCharset malloc failed!!!");
                  EME_UTILS_MEMSET(pstIn->pcHtmlCharset,0x00,iEndPos - nFindPos);
                  EME_UTILS_MEMMOVE(pstIn->pcHtmlCharset, pcMimeHeader + nFindPos, iEndPos - nFindPos -1);
                  if (NULL != EME_UTILS_STRCHR(pstIn->pcHtmlCharset,'\"')  )
                  {
                     EME_MailMsg_UnquoteString(pstEMsgHandle,pstIn->pcHtmlCharset, '\"', '\"');
                  }
               }
               
            }

         }
         
         /*get the text/plain or text/html Content*/
         pcContText = EME_MailMsg_DecodeBody(pstEMsgHandle,pcMimeBody, uiBodySize, eEncoding, NULL); /*need add "7bit"*/

         if ( EME_UTILS_StrStrNC(pcContType, "text/html") && pstIn->pcTextHtml == NULL)
         {
            pstIn->pcTextHtml = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcContText, pstIn->pcHtmlCharset);
            
            EME_UTILS_FREEIF(pvMemMgr,pcContText);
         }
         else if ( EME_UTILS_StrStrNC(pcContType, "text/plain") && pstIn->pcTextPlain == NULL && !havePartsAlready )
         {
                        
            pstIn->pcTextPlain = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcContText, pstIn->pcPlainCharset);
            EME_UTILS_FREEIF(pvMemMgr,pcContText);
         }
         else
         {               
            St_EmailMsgPart *pstEMsgPart = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_EmailMsgPart));
            EME_UTILS_BREAKNULL(pstEMsgPart,
                     iResult,
                     EME_NOMEMORY,
                     " malloc failed!!!");
            EME_UTILS_MEMSET(pstEMsgPart,0x00, sizeof(St_EmailMsgPart));
            
            pstEMsgPart->pcContentType       = pcContType;
            pstEMsgPart->pcName              = pcName;
            pstEMsgPart->pcFileName          = pcFileName;
            pstEMsgPart->pcTransEnc          = pcTranEncoding;
            pstEMsgPart->pcOnePartData       = pcMimeBody;
            pstEMsgPart->pcContentId         = pcContId;
            pstEMsgPart->uiOnePartSize       = uiBodySize;
//            pstEMsgPart->pcContentLoc        = pcContLocat;
            EME_UTILS_FREEIF(pvMemMgr,pcContText);
            
            EME_MailMsg_AddMsgPart(pstEMsgHandle,pstIn, pstEMsgPart);
            /* don't need free pcContType, pcName, pcFileName, pcMimeBody... */
            havePartsAlready = 1;
            pcContType = pcName = pcFileName = pcContId = NULL;
            pcBodyData = pcFindPos + iStartPos;/* go to next pstEMsgPart. */
            EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
            continue;
         }
      }
     
      else if ( pcContType && EME_UTILS_StrStrNC(pcContType, "multipart") )
      {
         WE_INT32 iBoundPos            = -1;
         WE_CHAR* pcLineEnd            = NULL;
         WE_CHAR* pcNextBoundary       = NULL;
         
         if ( EME_UTILS_StrStrNC(pcContType, "multipart/related") )
         {
            pstIn->bMultipartRelated = TRUE;
         }
         
         if ( (iBoundPos = EME_MailMsg_FindHdFieldPos("boundary=", pcMimeHeader)) > 0)
         {
            if ( (pcLineEnd = EME_UTILS_STRCHR(pcMimeHeader + iBoundPos, '\n')) != NULL)
            {
               /* ========================= +3 means + "--" + '\0'=================== */
               pcNextBoundary = EME_UTILS_MALLOC(pvMemMgr,pcLineEnd - pcMimeHeader - iBoundPos + 3); 
               EME_UTILS_BREAKNULL(pcNextBoundary,
                        iResult,
                        EME_NOMEMORY,
                        " malloc failed!!!");
               EME_UTILS_MEMSET(pcNextBoundary,0x00,pcLineEnd - pcMimeHeader - iBoundPos + 3);
               EME_UTILS_STRCPY(pcNextBoundary, "--");
               EME_UTILS_MEMMOVE(pcNextBoundary + 2, 
                                 pcMimeHeader + iBoundPos, 
                                 pcLineEnd - (pcMimeHeader + iBoundPos));
            }
            else
            {
               pcNextBoundary = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcMimeHeader + iBoundPos) + 3); 
               EME_UTILS_BREAKNULL(pcNextBoundary,
                        iResult,
                        EME_NOMEMORY,
                        " malloc failed!!!");
               EME_UTILS_MEMSET(pcNextBoundary,0x00,EME_UTILS_STRLEN(pcMimeHeader + iBoundPos) + 3);
               EME_UTILS_STRCPY(pcNextBoundary, "--");
               EME_UTILS_STRCAT(pcNextBoundary, pcMimeHeader + iBoundPos);
            }
            EME_MailMsg_UnquoteString(pstEMsgHandle,pcNextBoundary + 2, '\"', '\"');/* +2 mean go behind "--" */
         }
         else
         {
            pcNextBoundary = EME_UTILS_STRDUP(pvMemMgr,pcBoundary);
            EME_UTILS_BREAKNULL(pcNextBoundary,
                     iResult,
                     EME_NOMEMORY,
                     "EME_UTILS_STRDUP malloc failed!!!");
         }
         
         EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
         EME_MailMsg_ParseMimePart(pstEMsgHandle,pstIn, pcMimeBody, pcNextBoundary);
         EME_UTILS_FREEIF(pvMemMgr,pcNextBoundary);
      }

      else
      {
         St_EmailMsgPart *pstEMsgPart = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_EmailMsgPart)); 
         EME_UTILS_BREAKNULL(pstEMsgPart,
                  iResult,
                  EME_NOMEMORY,
                  " malloc failed!!!");
         EME_UTILS_MEMSET(pstEMsgPart,0x00,sizeof(St_EmailMsgPart));
         pstEMsgPart->pcContentType       = pcContType;
         pstEMsgPart->pcName              = pcName;
         pstEMsgPart->pcFileName          = pcFileName;
         pstEMsgPart->pcTransEnc          = pcTranEncoding;
         pstEMsgPart->pcOnePartData       = pcMimeBody;
         pstEMsgPart->pcContentId         = pcContId;
         pstEMsgPart->uiOnePartSize       = uiBodySize;
//         pstEMsgPart->pcContentLoc        = pcContLocat;      

         
         EME_MailMsg_AddMsgPart(pstEMsgHandle,pstIn, pstEMsgPart);
         havePartsAlready = 1;
         pcContType = pcName = pcFileName = pcContId = NULL;
         pcBodyData = pcFindPos + iStartPos;             /* go to next pstEMsgPart. */
         EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
         continue;
      }
      havePartsAlready = 1;
      
      pcBodyData = pcFindPos + iStartPos;/* go to next pstEMsgPart. */
      EME_UTILS_FREEIF(pvMemMgr,pcName);
      EME_UTILS_FREEIF(pvMemMgr,pcFileName);
      EME_UTILS_FREEIF(pvMemMgr,pcContType);
      EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
      //EME_UTILS_FREEIF(pvMemMgr,pcMimeBody);
      EME_UTILS_FREEIF(pvMemMgr,pcContId);
//      EME_UTILS_FREEIF(pvMemMgr,pcContLocat);
      EME_UTILS_FREEIF(pvMemMgr, pcTranEncoding);
    }
    /*error handling*/
    if (EME_SUCCESS != iResult)
    {
       EME_UTILS_FREEIF(pvMemMgr,pcName);
       EME_UTILS_FREEIF(pvMemMgr,pcFileName);
       EME_UTILS_FREEIF(pvMemMgr,pcContType);
       EME_UTILS_FREEIF(pvMemMgr,pcMimeHeader);
       //EME_UTILS_FREEIF(pvMemMgr,pcMimeBody);
       EME_UTILS_FREEIF(pvMemMgr,pcContId);
//       EME_UTILS_FREEIF(pvMemMgr,pcContLocat); 
       EME_UTILS_FREEIF(pvMemMgr, pcTranEncoding);
    }
    return iResult;
}


WE_CHAR* EME_MailMsg_DecodeBody(stEMSGHandle *pstEMsgHandle,
                                WE_CHAR* pcSrc,
                                WE_UINT32 uiOrigSize,
                                E_EncodeType eEncodeType,
                                WE_UINT32 *puiSize)
{
   WE_CHAR   *pcRet    = NULL;
   WE_UINT32 uiSize    = 0;
   void      *pvMemMgr = NULL;

   if (NULL == pstEMsgHandle || NULL == pcSrc || uiOrigSize == 0)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),input parameters is invalid!!!!"));
      return NULL;
   }
   
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   switch( eEncodeType ) 
   {
   case E_ENCODETYPE_8BIT:
      pcRet = EME_UTILS_MALLOC(pvMemMgr,uiOrigSize + 1);
      if (NULL == pcRet)
      {
         EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),EME_UTILS_STRDUP failed!!!"));
         break;
      }
      EME_UTILS_MEMSET(pcRet, 0, uiOrigSize + 1);
      EME_UTILS_MEMCPY(pcRet, pcSrc, uiOrigSize);

      uiSize = uiOrigSize;
      break;
      
   case E_ENCODETYPE_BASE64:
      uiSize = EME_Base64DecodeSize(uiOrigSize) + 1; /* +1 means + '\0' */
      pcRet = EME_UTILS_MALLOC(pvMemMgr,uiSize);
      if (NULL == pcRet)
      {
         EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),malloc failed!!!"));
         break;
      }
      
      EME_UTILS_MEMSET(pcRet,0x00,uiSize);
      uiSize = EME_FromBase64(pcRet, pcSrc, uiOrigSize);        
      break;
      
   case E_ENCODETYPE_QUOTEDPRINTABLE:
      uiSize = EME_QuotedDecodeSize(uiOrigSize) + 1; /* +1 means + '\0' */
      pcRet = EME_UTILS_MALLOC(pvMemMgr,uiSize);
      if (NULL == pcRet)
      {
         EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),malloc failed!!!"));
         break;
      }
      
      EME_UTILS_MEMSET(pcRet,0x00,uiSize);
      uiSize = EME_FromQuoted(pcRet, pcSrc, uiOrigSize);        
      break;

   case E_ENCODETYPE_UUENCODE:
      {
         UU_Decode_Buf  *pstDecodeBuf = NULL;

         pstDecodeBuf = EME_UU_Decode(pcSrc, uiOrigSize, pvMemMgr);
         if(pstDecodeBuf != NULL)
         {
            pcRet = pstDecodeBuf->pBuf;
            pstDecodeBuf->pBuf = NULL;
            uiSize = pstDecodeBuf->uiSize;
            UU_Decode_Buf_Release(pstDecodeBuf, pvMemMgr);
            pstDecodeBuf = NULL;
         }
         else
         {
            EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),malloc failed!!!"));
         }
      }
      break;

   case E_ENCODETYPE_BINHEX:
      break;
   
   /*default of Content-Transfer-Encoding: "7bit"*/
   default:
      pcRet = EME_UTILS_MALLOC(pvMemMgr,uiOrigSize + 1);
      if (NULL == pcRet)
      {
         EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeBody(),EME_UTILS_STRDUP failed!!!"));
         break;
      }
      EME_UTILS_MEMSET(pcRet, 0, uiOrigSize + 1);
      EME_UTILS_MEMCPY(pcRet, pcSrc, uiOrigSize);

      uiSize = uiOrigSize;      
      break;
   }
   if (puiSize != NULL)
   {
      *puiSize = uiSize;
   }

   return pcRet;
}


/* bExtra == 1, clear the Extra header field, or clear header field. */
static WE_INT32 EME_MailMsg_ClearHeader(stEMSGHandle *pstEMsgHandle,
                                        St_EmailMsg* pstIn)
{
   WE_UINT32      i = 0;
   void *pvMemMgr = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pstIn)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ClearHeader(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   for (i = 0; i < pstIn->uiHeaderFieldNum; i++)
   {
      EME_UTILS_FREEIF(pvMemMgr,pstIn->ppcHeaderFields[i]);
   }
   
   EME_UTILS_FREEIF(pvMemMgr,pstIn->ppcHeaderFields);
   pstIn->uiHeaderFieldNum = 0;
   
   return EME_SUCCESS;
  
}

/*set mail name and mail address to field(from/to/cc/pcBcc...) */
static WE_INT32 EME_MailMsg_SetAddrField(stEMSGHandle *pstEMsgHandle,
                                         WE_CHAR** ppcField, 
                                         WE_CHAR* pcAddr, 
                                         WE_CHAR* pcName)
{   
   WE_CHAR *pcTmp       = NULL;
   void    *pvMemMgr    = NULL;
   WE_INT32 iResult     = EME_SUCCESS; /*must be init success*/

   if (NULL == pstEMsgHandle || NULL == ppcField || NULL == pcAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetAddrField(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      
      if (NULL == pcName)
      {       
         *ppcField = EME_UTILS_STRDUP(pvMemMgr,pcAddr);
         EME_UTILS_BREAKNULL(*ppcField,
            iResult,
            EME_NOMEMORY,
            "EME_UTILS_STRDUP,failed!!!!");
      }

      else
      {
         pcTmp = EME_HeaderEncode2Base64(pstEMsgHandle, pcName, E_CHARSET_UTF8);
         EME_UTILS_BREAKNULL(pcTmp,
            iResult,
            EME_FAILED,
            "EME_MailMsg_SetAddrField,EME_Encode2Base64,failed!!!!!");
         
         /* +6 means + '"'+'"'+' '+'<'+'>''\0', the format will be "***" <abc@serv.com> */
         *ppcField = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcTmp) + EME_UTILS_STRLEN(pcAddr) + 6);
         EME_UTILS_BREAKNULL(*ppcField,
            iResult,
            EME_NOMEMORY,
            "EME_UTILS_STRDUP,failed!!!!");
         EME_UTILS_MEMSET(*ppcField,0x00,EME_UTILS_STRLEN(pcTmp) + EME_UTILS_STRLEN(pcAddr) + 6);
         EME_UTILS_SPRINTF(*ppcField, "\"%s\" <%s>", pcTmp, pcAddr);

         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      }
      
   } while(0);

   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pcTmp);
   }
   
   return iResult;
}


/*
* append one field to the field list (to, cc, pcBcc)
* exp. the format is like: "aaa" <aaa@ser.com>, "wxl" <www@mail.com>...
*/ 
static WE_INT32 EME_MailMsg_AppendAddrField(stEMSGHandle *pstEMsgHandle,
                                            WE_CHAR** ppcAddrField,
                                            WE_CHAR* pcOneAddr)
{
   WE_INT32    iResult       = EME_SUCCESS; /*must be init to SUCCESS;*/
   WE_UINT32   uiOldLen      = 0;
   WE_UINT32   uiNewLen      = 0;
   WE_CHAR     *pcAlready    = NULL;
   void        *pvMemMgr     = NULL;

   if (NULL == pstEMsgHandle || NULL == ppcAddrField || NULL == pcOneAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendAddrField(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      if (*ppcAddrField != NULL)
      {
         uiOldLen = EME_UTILS_STRLEN(*ppcAddrField);
      }
      pcAlready = *ppcAddrField;

      uiNewLen =  uiOldLen + EME_UTILS_STRLEN(pcOneAddr) + 3;/* +3 means + ','+' '+'\0' */

      *ppcAddrField = EME_UTILS_STRREALLOC(pvMemMgr,*ppcAddrField, uiNewLen); 
      EME_UTILS_BREAKNULL(*ppcAddrField,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_AppendAddrField,realloc memory failed!!!!");

      if (pcAlready != NULL)
      {
         if (NULL == EME_UTILS_StrStrNC(pcOneAddr,","))
         {
            EME_UTILS_STRCAT(*ppcAddrField, ",");
         }
      } 
      
      EME_UTILS_STRCAT(*ppcAddrField, pcOneAddr);

   } while(0);
   

   return iResult;
}


static WE_INT32 EME_MailMsg_GetField(stEMSGHandle *pstEMsgHandle,
                                     WE_CHAR** field, 
                                     WE_CHAR** ret, 
                                     WE_CHAR* charset)
{
   WE_INT32 iResult  = EME_SUCCESS; /*must be init to SUCCESS;*/
   void     *pvMemMgr = NULL;
   if (NULL  == pstEMsgHandle || NULL == field || NULL == ret || NULL == charset)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetField(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      if (*field != NULL)
      {
         if ( EME_UTILS_StrStrNC(charset, "utf-7") != NULL )
         {
            *ret = EME_MailMsg_ParseBodyCharset(pstEMsgHandle, *field, "utf-7");
            EME_UTILS_FREEIF(pvMemMgr,*field); /*PB???*/
         }
         else
         {
            *ret = *field;
            *field = NULL;
         }        
      }

   } while(0);
   
   return iResult;
}

/* ===================mail message functions===================== */
WE_INT32 EME_CreateMailMsg(stEMSGHandle *pstEMsgHandle,void** ppVoid)
{
   WE_INT32       iResult = EME_SUCCESS ; /*must be init success */
   void           *pvMemMgr = NULL;
   St_EmailMsg    *pstMailMsg = NULL;

   if (NULL == pstEMsgHandle || NULL == ppVoid)
   {
      EME_UTILS_LOG_ERROR(("EME_CreateMailMsg(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pstMailMsg = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_EmailMsg));
      EME_UTILS_BREAKNULL(pstMailMsg,
                          iResult,
                          EME_NOMEMORY,
                          "EME_CreateMailMsg, malloc failed!!!");
      EME_UTILS_MEMSET(pstMailMsg,0x00,sizeof(St_EmailMsg));
      
   } while(0);
   
   *ppVoid = pstMailMsg;

   return iResult;
}

WE_INT32 EME_DestroyMailMsg(stEMSGHandle *pstEMsgHandle,void** ppVoid)
{
   void        *pvMemMgr    = NULL;
   St_EmailMsg *pstEmailMsg = NULL;
   if (NULL == pstEMsgHandle || NULL == ppVoid)
   {
      EME_UTILS_LOG_ERROR(("EME_DestroyMailMsg(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pstEmailMsg = (St_EmailMsg*)(*ppVoid);
   if (NULL == pstEmailMsg)
   {
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);

   EME_FileClose(pstEmailMsg->pvMailFile);
   pstEmailMsg->pvMailFile = NULL;

   if(pstEmailMsg->bIsDecoding)
   {
      /*free part index list if there is not empty*/
      while(pstEmailMsg->pstPartIndexList != NULL)
      {
         St_PartIndexInfo *pstPartTmp = NULL;
         pstPartTmp = pstEmailMsg->pstPartIndexList;
         pstEmailMsg->pstPartIndexList = pstEmailMsg->pstPartIndexList->next;
         EME_UTILS_FREEIF(pvMemMgr, pstPartTmp);
      }
      
      /*clear the decoding part info structure data*/
      EME_MailMsg_ClearDecodingPartInfo(pstEMsgHandle, &pstEmailMsg->unPartInfo.astDecodingPartInfo);
   }
   else
   {
      EME_MailMsg_ClearEncodingPartInfo(pstEMsgHandle, &pstEmailMsg->unPartInfo.astEncodingPartInfo);
   }
   
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcFrom);

   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcTo);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcCc);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcBcc);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcSubject);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcDate);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcReplyTo);
   
   /*add new : priority  and disp-notification-to*/
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcPriority);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcNotifyTo);

   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcMimeVersion);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcTextPlain);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcTextHtml);

   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcBoundary);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcEmailDataBuf);

   /*free charset*/
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcHtmlCharset);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcPlainCharset);

   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcPlainTransEnc);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->pcHtmlTransEnc);
  
   EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->pcMailFileName);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg);
   
   *ppVoid = NULL;

   return EME_SUCCESS;
}


WE_INT32 EME_MailMsg_AddMsgPart(stEMSGHandle *pstEMsgHandle,
                                St_EmailMsg* pstIn, 
                                St_EmailMsgPart* pstEmlPart)
{
   WE_INT32    iResult         = EME_SUCCESS; /*must be init success*/
   St_EmailMsg *pstEmailMsg    = pstIn;
   void        *pvMemMgr       = NULL;

   if (NULL == pstEMsgHandle || NULL == pstEmailMsg  || NULL == pstEmlPart)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AddMsgPart(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      pstEmailMsg->ppstEMsgPart = EME_UTILS_REALLOC(pvMemMgr,pstEmailMsg->ppstEMsgPart, 
         sizeof(St_EmailMsgPart*) * (pstEmailMsg->uiEMsgPartNum + 1));
      EME_UTILS_BREAKNULL(pstEmailMsg->ppstEMsgPart,
         iResult,
         EME_NOMEMORY,
         "EME_MailMsg_AddMsgPart EME_UTILS_REALLOC failed!!!");
      
      pstEmailMsg->ppstEMsgPart[pstEmailMsg->uiEMsgPartNum++] = pstEmlPart;
      
   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsg_ClearMsgParts(stEMSGHandle *pstEMsgHandle,void* pstIn)
{
   WE_UINT32    i             = 0;
   void        *pvMemMgr      = NULL;
   St_EmailMsg *pstEmailMsg  = (St_EmailMsg*)pstIn;

   if (NULL == pstEMsgHandle || pstEmailMsg == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ClearMsgParts(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   for (i = 0; i < pstEmailMsg->uiEMsgPartNum; i++)
   {
      EME_MailMsgPart_Destroy(pstEMsgHandle,(void**)&pstEmailMsg->ppstEMsgPart[i]);
   }
   
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->ppstEMsgPart);
   
   pstEmailMsg->uiEMsgPartNum = 0;
   
   return EME_SUCCESS;
}


WE_INT32 EME_MailMsg_SetTextPlain(stEMSGHandle *pstEMsgHandle,
                                   void* pstIn,
                                   WE_CHAR* pcTextPlain,
                                   WE_CHAR* pcCharSet,
                                   WE_CHAR* pcTransEnc)
{   
   WE_INT32    iResult      = EME_SUCCESS; /*must be init SUCCESS*/
   void        *pvMemMgr    = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;  
   
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg ||NULL == pcTextPlain)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetTextPlain(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      pstEmailMsg->pcTextPlain = EME_UTILS_STRDUP(pvMemMgr,pcTextPlain);
      EME_UTILS_BREAKNULL(pstEmailMsg->pcTextPlain,
                        iResult,
                        EME_NOMEMORY,
                        "EME_MailMsg_SetTextPlain EME_UTILS_STRDUP failed!!1!");

      if (pcCharSet != NULL)
      {
         pstEmailMsg->pcPlainCharset = EME_UTILS_STRDUP(pvMemMgr, pcCharSet);
         EME_UTILS_BREAKNULL(pstEmailMsg->pcPlainCharset,
                           iResult,
                           EME_NOMEMORY,
                           "EME_MailMsg_SetTextPlain EME_UTILS_STRDUP failed!!1!");
      }
      else
      {
         pstEmailMsg->pcPlainCharset = EME_UTILS_STRDUP(pvMemMgr, EME_CHARSET_UTF8);
         EME_UTILS_BREAKNULL(pstEmailMsg->pcPlainCharset,
                           iResult,
                           EME_NOMEMORY,
                           "EME_MailMsg_SetTextPlain EME_UTILS_STRDUP failed!!1!");
      }

      pstEmailMsg->pcPlainTransEnc = EME_UTILS_STRDUP(pvMemMgr, EME_TRANS_ENC_BASE64);

   } while(0);
   

   return iResult;
}


WE_INT32 EME_MailMsg_SetTextHtml(stEMSGHandle *pstEMsgHandle,
                                 void  *pstIn, 
                                 WE_CHAR *pcTextHtml,
                                 WE_CHAR* pcCharSet,
                                 WE_CHAR* pcTransEnc)
{
   WE_INT32 iResult         = EME_SUCCESS; /*must be init SUCCESS*/
   void     *pvMemMgr       = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;  
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL  == pcTextHtml)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetTextHtml(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pstEmailMsg->pcTextHtml = EME_UTILS_STRDUP(pvMemMgr,pcTextHtml);
      EME_UTILS_BREAKNULL(pstEmailMsg->pcTextHtml ,
                           iResult,
                           EME_NOMEMORY,
                           "EME_MailMsg_SetTextHtml EME_UTILS_STRDUP,failed!!!!");

      if (pcCharSet != NULL)
      {
         pstEmailMsg->pcHtmlCharset = EME_UTILS_STRDUP(pvMemMgr,pcCharSet);
         EME_UTILS_BREAKNULL(pstEmailMsg->pcTextHtml ,
                              iResult,
                              EME_NOMEMORY,
                              "EME_MailMsg_SetTextHtml EME_UTILS_STRDUP,failed!!!!");
      }
      else
      {
         pstEmailMsg->pcHtmlCharset = EME_UTILS_STRDUP(pvMemMgr, EME_CHARSET_UTF8);
         EME_UTILS_BREAKNULL(pstEmailMsg->pcHtmlCharset ,
                              iResult,
                              EME_NOMEMORY,
                              "EME_MailMsg_SetTextHtml EME_UTILS_STRDUP,failed!!!!");
      }

      pstEmailMsg->pcHtmlTransEnc = EME_UTILS_STRDUP(pvMemMgr, EME_TRANS_ENC_BASE64);

   } while(0);
       
   return iResult;
}


WE_INT32 EME_MailMsg_SetFrom(stEMSGHandle *pstEMsgHandle,
                             void* pstIn, 
                             WE_CHAR* pcFromAddr,
                             WE_CHAR* pcFromName)
{
   WE_INT32    iResult      = EME_FAILED;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;  
   
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg|| NULL == pcFromAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetFrom(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pstEmailMsg->pcFrom, pcFromAddr, pcFromName);

   return iResult;
}


WE_INT32  EME_MailMsg_AppendNotifyTo(stEMSGHandle *pstEMsgHandle,
                              void* pstIn,
                              WE_CHAR* pcNotifyToAddr,
                              WE_CHAR* pcNotifyToName)
{
   WE_INT32    iResult      = EME_FAILED; 
   void        *pvMemMgr    = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;
   WE_CHAR     *pcField      = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcNotifyToAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendTo(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pcField, pcNotifyToAddr, pcNotifyToName);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendTo,EME_MailMsg_SetAddrField,failed!!!!");
      
      iResult = EME_MailMsg_AppendAddrField(pstEMsgHandle,&pstEmailMsg->pcNotifyTo, pcField);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendTo,EME_MailMsg_AppendAddrField failed!!!!");
      
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr,pcField);
   
   return iResult;
}

WE_INT32 EME_MailMsg_AppendTo(stEMSGHandle *pstEMsgHandle,
                              void *pstIn,
                              WE_CHAR *pcToAddr, 
                              WE_CHAR *pcToName)
{
   WE_INT32    iResult      = EME_FAILED; 
   void        *pvMemMgr    = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;
   WE_CHAR     *pcField      = NULL;
   
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcToAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendTo(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pcField, pcToAddr, pcToName);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendTo,EME_MailMsg_SetAddrField,failed!!!!");
      
      iResult = EME_MailMsg_AppendAddrField(pstEMsgHandle,&pstEmailMsg->pcTo, pcField);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendTo,EME_MailMsg_AppendAddrField failed!!!!");
      
   } while(0);

   EME_UTILS_FREEIF(pvMemMgr,pcField);
   
   return iResult;
}


WE_INT32 EME_MailMsg_AppendCc(stEMSGHandle *pstEMsgHandle,
                              void* pstIn, 
                              WE_CHAR* pcCcAddr,
                              WE_CHAR* pcCcName)
{
   WE_INT32 iResult          = EME_SUCCESS; /*must be init success*/
   void     *pvMemMgr        = NULL;
   WE_CHAR  *pcField          = NULL;
   St_EmailMsg *pstEmailMsg  = (St_EmailMsg*)pstIn;

   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcCcAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendCc(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pcField, pcCcAddr, pcCcName);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendCc,EME_MailMsg_SetAddrField,failed!!!!");
      
      iResult = EME_MailMsg_AppendAddrField(pstEMsgHandle,&pstEmailMsg->pcCc, pcField);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendCc,EME_MailMsg_AppendAddrField failed!!!!");

   } while(0);
  
   EME_UTILS_FREEIF(pvMemMgr,pcField);
   
   return iResult;
}

WE_INT32 EME_MailMsg_AppendBcc(stEMSGHandle *pstEMsgHandle,
                               void* pstIn,
                               WE_CHAR* pcBccAddr, 
                               WE_CHAR* pcBccName)
{
   WE_INT32    iResult          = EME_SUCCESS; /*must be init success*/
   void        *pvMemMgr        = NULL;
   WE_CHAR     *pcField          = NULL;
   St_EmailMsg *pstEmailMsg     = (St_EmailMsg*)pstIn;

   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcBccAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_AppendBcc(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pcField, pcBccAddr, pcBccName) ;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendBcc,EME_MailMsg_SetAddrField,failed!!!!");
      
      iResult = EME_MailMsg_AppendAddrField(pstEMsgHandle,&pstEmailMsg->pcBcc, pcField);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_AppendBcc,EME_MailMsg_AppendAddrField failed!!!!");
      
   } while(0);
    
   EME_UTILS_FREEIF(pvMemMgr,pcField);
   
   return iResult;
}


WE_INT32 EME_MailMsg_SetSubject(stEMSGHandle *pstEMsgHandle,
                                void* pstIn,
                                WE_CHAR* pcSubject)
{
   WE_INT32 iResult     = EME_SUCCESS; /*must be init to SUCCESS*/
   void *pvMemMgr       = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg *) pstIn;

   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcSubject)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetSubject(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pstEmailMsg->pcSubject = EME_UTILS_STRDUP(pvMemMgr,pcSubject);
      EME_UTILS_BREAKNULL(pstEmailMsg->pcSubject,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_SetSubject EME_UTILS_STRDUP failed!!!!");
      
   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsg_SetDateTime(stEMSGHandle *pstEMsgHandle,
                                 void* pstIn, WE_UINT16 week,
                                 WE_UINT16 dd, WE_UINT16 mm,
                                 WE_UINT16 yy, WE_UINT16 hour, 
                                 WE_UINT16 min, WE_UINT16 sec, 
                                 WE_INT16 difftime)
{
   WE_INT32     iResult        = EME_SUCCESS;/* must be init to SUCCESS*/
   void         *pvMemMgr      = NULL;
   const WE_CHAR weeks[7][4]   = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
   const WE_CHAR months[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
   WE_INT16      h = difftime / 3600;
   WE_INT16      m = ( EME_UTIS_ABS(difftime) - EME_UTIS_ABS(h * 3600) ) / 60;
   WE_CHAR       date[EME_MAXSTRLEN];
   WE_CHAR       diff[EME_MINSTRLEN];
   
   EME_UTILS_MEMSET(&date,0x00,sizeof(WE_CHAR) * EME_MAXSTRLEN);
   EME_UTILS_MEMSET(&diff,0x00,sizeof(WE_CHAR) * EME_MINSTRLEN);

   if (NULL == pstEMsgHandle || pstIn == NULL || week > 7 || week < 1 || mm > 12 || mm < 1)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetDateTime(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      /* calculate time zone diff format */ 
      EME_UTILS_SPRINTF(diff, "%+.2d%.2d", h, m);
      
      /* build date time format in Email */ 
      EME_UTILS_SPRINTF(date, "%s, %.2d %s %d %.2d:%.2d:%.2d %s",
         weeks[week - 1], dd, months[mm - 1], yy, 
         hour, min, sec, diff);
      
      ((St_EmailMsg*)pstIn)->pcDate = EME_UTILS_STRDUP(pvMemMgr,date);
      EME_UTILS_BREAKNULL(((St_EmailMsg*)pstIn)->pcDate,
         iResult,
         EME_NOMEMORY,
         "EME_MailMsg_SetDateTime EME_UTILS_STRDUP failed!!!");
   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsg_SetDateTimeEx(stEMSGHandle *pstEMsgHandle,
                                 void* pstIn, WE_UINT32 uiSecond,  WE_INT16 difftime)
{
   WE_INT32     iResult        = EME_SUCCESS;/* must be init to SUCCESS*/
   void             *pvMemMgr      = NULL;
   WE_INT16      h = difftime / 3600;
   WE_INT16      m = ( EME_UTIS_ABS(difftime) - EME_UTIS_ABS(h * 3600) ) / 60;
   WE_CHAR       date[EME_MAXSTRLEN];
   WE_CHAR       diff[EME_MINSTRLEN];
   WE_CHAR	   *pcTemp = NULL;
   
   EME_UTILS_MEMSET(&date,0x00,sizeof(WE_CHAR) * EME_MAXSTRLEN);
   EME_UTILS_MEMSET(&diff,0x00,sizeof(WE_CHAR) * EME_MINSTRLEN);

   if (NULL == pstEMsgHandle || pstIn == NULL )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetDateTime(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      /* calculate time zone diff format */ 
      EME_UTILS_SPRINTF(diff, "%+.2d%.2d", h, m);
      
      /* build date time format in Email */ 
       WeSCL_Time2Str(uiSecond, date);

	pcTemp = date;
	pcTemp = strstr(pcTemp, "GMT");
	EME_UTILS_BREAKNULL(pcTemp, iResult, EME_FAILED,  "get GMT string failed");

	EME_UTILS_MEMCPY(date + (pcTemp - date), diff, EME_UTILS_STRLEN(diff));
	  
      ((St_EmailMsg*)pstIn)->pcDate = EME_UTILS_STRDUP(pvMemMgr,date);
      EME_UTILS_BREAKNULL(((St_EmailMsg*)pstIn)->pcDate,
         iResult,
         EME_NOMEMORY,
         "EME_MailMsg_SetDateTime EME_UTILS_STRDUP failed!!!");
   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsg_SetReplyTo(stEMSGHandle *pstEMsgHandle,
                                void* pstIn, 
                                WE_CHAR* pcReplyAddr, 
                                WE_CHAR* pcReplyName)
{
   WE_INT32    iResult = EME_FAILED;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;  
   
   if (NULL == pstEMsgHandle || NULL == pstEmailMsg || NULL == pcReplyAddr)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_SetReplyTo(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }   

   iResult = EME_MailMsg_SetAddrField(pstEMsgHandle,&pstEmailMsg->pcReplyTo, pcReplyAddr, pcReplyName);
   
   return iResult;
}


/* set priority*/
WE_INT32 EME_SetPriority(stEMSGHandle *pstInHandle,void* pvstIn,  WE_UINT32 uiPriority)
{
   void        *pvMemMgr    = NULL;
   St_EmailMsg *pstEmailMsg = (St_EmailMsg *)pvstIn;
   
   if (NULL == pstInHandle || NULL == pstEmailMsg )
   {
      EME_UTILS_LOG_ERROR(("EME_SetPriority(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstInHandle->pvMemMgr;
   pstEmailMsg->pcPriority = EME_UTILS_MALLOC(pvMemMgr,sizeof(uiPriority));
   if (NULL == pstEmailMsg->pcPriority)
   {
      EME_UTILS_LOG_ERROR(("EME_SetPriority,malloc failed!!!"));
   }
   
   EME_UTILS_MEMSET(pstEmailMsg->pcPriority,0x00, sizeof(uiPriority));
   EME_UTILS_SPRINTF(pstEmailMsg->pcPriority,"%d",uiPriority);
   return EME_SUCCESS;
}

WE_INT32 EME_MailMsg_GetDateTime(St_EmailMsg *pstIn, TimeType *pstJulianDateType,  WE_INT16* difftime)
{
   WE_INT32 iResult            = EME_FAILED; 
   const WE_CHAR weeks[7][4]   = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
   const WE_CHAR months[12][4] = {"Jan","Feb","Mar","Apr",
                                    "May","Jun","Jul","Aug",
                                    "Sep","Oct","Nov","Dec"};
   WE_CHAR    *pos             = NULL;
   WE_INT16    diff            = 0;
   WE_UINT16   i               = 0;
   
   if (NULL == pstIn ||  NULL == pstJulianDateType || NULL == difftime)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetDateTime(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
    
   do 
   {
      pos  = pstIn->pcDate;
      EME_UTILS_BREAKNULL(pos,
               iResult,
               EME_FAILED,
               "EME_MailMsg_GetDateTime() ((St_EmailMsg*)pstIn)->pcDate is NULL!!!");
      
      /* parse week value */ 
      for (i = 0; i < 7; i++)
      {
         if ( EME_UTILS_StrStrNC(pos, weeks[i]) != NULL)
         {
            pos += 3; /* go behind week value */ 
            *(&pstJulianDateType->DayIndex) = i + 1;
            break;
         }
      }
      //if (i == 7)
      //{ 
      //   EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime,invalid week !!!!! "); 
      //}
      
      /* find next ' ' */
      //if ( (pos = EME_UTILS_STRCHR(pos, ',')) == NULL )
      //{
      //   EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime,parse week error!!!!! "); 
      //}
	  //pos++;
      /* find next ' ' , there is week string, e.g."Mon", jump to Date */
      if (i != 7)
      {
         if ( (pos = EME_UTILS_STRCHR(pos, ',')) == NULL )
         {
            EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime,parse week error!!!!! "); 
         }
         pos++;
      }      
      
      *(&pstJulianDateType->nDay) = (WE_UINT8)EME_UTILS_STRTOUL(pos, NULL, 10);
      /* find next ' ' */
      if ( (pos = EME_UTILS_STRCHR(pos, ' ')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime,parse week error!!!!! "); 
      }
      
      pos++;
      /* parse month value */
      for (i = 0; i < 12; i++)
      {
         if ( EME_UTILS_StrStrNC(pos, months[i]) != NULL)
         {
            pos += 3; /* go behind month value */ 
            *(&pstJulianDateType->nMonth) = i + 1;
            break;
         }
      }
      
      if (i == 12)
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime,invalid month !!!!! "); 
      }
      /* find next ' ' */
      if ( (pos = EME_UTILS_STRCHR(pos, ' ')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime, error!!!!! "); 
      }
      
      pos++;
      *(&pstJulianDateType->nYear) = (WE_UINT16)EME_UTILS_STRTOUL(pos, NULL, 10);
      
      if ( (pos = EME_UTILS_STRCHR(pos, ' ')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime, error!!!!! "); 
      }
      
      pos++;
      *(&pstJulianDateType->nHour) = (WE_UINT8)EME_UTILS_STRTOUL(pos++, NULL, 10);
      
      if ( (pos = EME_UTILS_STRCHR(pos, ':')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime, error!!!!! "); 
      }
      
      pos++;
      *(&pstJulianDateType->nMin) = (WE_UINT8)EME_UTILS_STRTOUL(pos++, NULL, 10);
      
      if ( (pos = EME_UTILS_STRCHR(pos, ':')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime, error!!!!! "); 
      }
      
      pos++;
      *(&pstJulianDateType->nSec) = (WE_UINT8)EME_UTILS_STRTOUL(pos++, NULL, 10);
      
      if ( (pos = EME_UTILS_STRCHR(pos, ' ')) == NULL )
      {
         EME_UTILS_BREAK(iResult,"EME_MailMsg_GetDateTime, error!!!!! "); 
      }
      
      pos++;
      diff = (WE_INT16)EME_UTILS_STRTOUL(pos, NULL, 10);
      
      /* calculate diff time in second */ 
      *difftime = (diff / 100) * 3600 + (diff % 100) * 60;
      iResult = EME_SUCCESS;
   } while(0);
   
   return iResult;
}

WE_INT32 EME_MailMsg_GetPlainText(stEMSGHandle *pstEMsgHandle,St_EmailMsg* pstIn, WE_CHAR** ppRet)
{
   St_EmailMsg *pstEmailMsg = pstIn;
   WE_INT32    iResult      = EME_SUCCESS;        /*must init to success*/
   void        *pvMemMgr    = NULL;

   if (NULL == pstEMsgHandle ||  NULL ==ppRet || NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetPlainText(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      if(NULL != pstEmailMsg->pcTextPlain)
      {
         *ppRet = EME_UTILS_STRDUP(pvMemMgr, pstEmailMsg->pcTextPlain);
         EME_UTILS_BREAKNULL(*ppRet,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_GetPlainText,malloc failed!!!!!");
      }
      else
      {
         EME_UTILS_LOG_INFO(("EME_MailMsg_GetPlainText--: Text Plain is NULL"));
         *ppRet = NULL;
      }
      
   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsg_GetHtmlText(stEMSGHandle *pstEMsgHandle,
                               St_EmailMsg* pstIn, 
                               WE_CHAR** ppRet)
{
   WE_INT32    iResult            = EME_SUCCESS; /*must init to success*/
   void        *pvMemMgr          = NULL;
   St_EmailMsg *pstEmailMsg       = pstIn;
   
   if (NULL == pstEMsgHandle ||  NULL ==ppRet || NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetHtmlText(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      if(NULL != pstEmailMsg->pcTextHtml)
      {
         *ppRet = EME_UTILS_STRDUP(pvMemMgr, pstEmailMsg->pcTextHtml);
         EME_UTILS_BREAKNULL(*ppRet,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_GetPlainText,malloc failed!!!!!");
      }
      else
      {
         EME_UTILS_LOG_INFO(("EME_MailMsg_GetPlainText--: Text html is NULL"));
         *ppRet = NULL;
      }
      
   } while(0);
   
   return iResult;
}

St_EmailMsgPart* EME_MailMsg_GetOnePartByIndex(void* pstIn, WE_UINT32 uiPartIndex)
{
   St_EmailMsg *pstEMailMsg = (St_EmailMsg*)pstIn;

   if (NULL == pstEMailMsg || pstEMailMsg->uiEMsgPartNum == 0)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetOnePartByIndex(),input parameters is invalid!!!!"));
      return NULL;
   }

   if (uiPartIndex < pstEMailMsg->uiEMsgPartNum)
   {
      return pstEMailMsg->ppstEMsgPart[uiPartIndex];
   }

   else
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetOnePartByIndex,the uiPartIndex is not exist!!!!"));
      return NULL;
   }
   
}

WE_INT32 EME_EncodingMailMsg(stEMSGHandle *pstEMsgHandle,void* pstIn)
{
   WE_INT32   iResult               = EME_FAILED;
   WE_UINT32  i                     = 0;
   void       *pvMemMgr             = NULL;
   WE_CHAR    *pcTmp                = NULL;
   WE_CHAR    *pcEncodeWord         = NULL;
   WE_CHAR    *pcHeaders            = NULL;
   WE_CHAR    *pcContentType = NULL;
   WE_CHAR    newStr[EME_MAXSTRLEN]  = {0};
   St_EmailMsg *pstEmailMsg         = (St_EmailMsg*)pstIn;
   WE_UINT32  uiBodySize            = 0;
   WE_UINT32  uiHeaderSize          = 0;
   
   if (NULL == pstEmailMsg || NULL == pstEMsgHandle)
   {
      EME_UTILS_LOG_ERROR(("EME_EncodingMailMsg(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   { 
      if (pstEmailMsg->pcFrom != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "From:", pstEmailMsg->pcFrom);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader FROM failed!!!!");
      }
      
      /*append To to header*/
      if (pstEmailMsg->pcTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "To:", pstEmailMsg->pcTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader TO failed!!!!");
      }
      
      /*append Cc to header*/
      if (pstEmailMsg->pcCc != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Cc:", pstEmailMsg->pcCc);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader CC failed!!!!");
      }
      
      /*append To Date time header*/
      if (pstEmailMsg->pcDate != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Date:", pstEmailMsg->pcDate);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader Date failed!!!!");
      }
      
      /*append Reply-To to header*/
      if (pstEmailMsg->pcReplyTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Reply-To:", pstEmailMsg->pcReplyTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader Reply-To failed!!!!");
      }
      
      /*append subject to header*/
      if (pstEmailMsg->pcSubject != NULL)
      {
         pcTmp = EME_HeaderEncode2Base64(pstEMsgHandle,pstEmailMsg->pcSubject,E_CHARSET_UTF8);
         EME_UTILS_BREAKNULL(pcTmp,
            iResult,
            EME_NOMEMORY,
            "EME_EncodingMailMsg, EME_Encode2Base64 malloc failed!!!");
         
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Subject:", pcTmp);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader Subject failed!!!!");
         
         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      }
      else
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Subject:", " ");
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader Subject failed!!!!");
      }
      
      /*append priority to header*/
      if (NULL != pstEmailMsg->pcPriority)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "X-Priority:", pstEmailMsg->pcPriority);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader X-Priority failed!!!!");
      }
      
      /*append Disposition-Notification-To to header*/
      if (pstEmailMsg->pcNotifyTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle, 
                                            pstEmailMsg, 
                                            "Disposition-Notification-To:", 
                                            pstEmailMsg->pcNotifyTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader Disposition-Notification-To failed!!!!");
      }
 
      iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "X-mailer:", "TF-Mail 1.0");
      EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_UpdateHeader X-Priority failed!!!!");      

      uiBodySize = EME_MailMsg_CaluBodySize(pstEmailMsg);

      /*append Mime-Version to header*/
      EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Mime-Version:", "1.0");
      
      /*Have no attachment */
      if (pstEmailMsg->uiEMsgPartNum == 0)
      {      
         if ( (NULL == pstEmailMsg->pcTextPlain  && NULL != pstEmailMsg->pcTextHtml)
             || (NULL != pstEmailMsg->pcTextPlain && NULL == pstEmailMsg->pcTextHtml))
         {
           
            if ( NULL != pstEmailMsg->pcTextPlain)
            {
               pcContentType = EME_UTILS_MALLOC(pvMemMgr, 
                                                EME_UTILS_STRLEN("text/plain;\r\n charset=\"\"") +
                                                EME_UTILS_STRLEN(pstEmailMsg->pcPlainCharset) + 1);
               EME_UTILS_BREAKNULL(pcContentType, iResult, EME_NOMEMORY, "no memory");
               
               EME_UTILS_MEMSET(pcContentType, 
                  0x00, 
                  EME_UTILS_STRLEN("text/plain;\r\n charset=\"\"") +
                  EME_UTILS_STRLEN(pstEmailMsg->pcPlainCharset) + 1);

               EME_UTILS_SPRINTF(pcContentType, "text/plain;\r\n charset=\"%s\"", pstEmailMsg->pcPlainCharset);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", pcContentType);
               EME_UTILS_FREEIF(pvMemMgr, pcContentType);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,
                                        pstEmailMsg, 
                                        "Content-Transfer-Encoding:", 
                                        pstEmailMsg->pcPlainTransEnc);
            }
            else
            {
               pcContentType = EME_UTILS_MALLOC(pvMemMgr, 
                                                EME_UTILS_STRLEN("text/html;\r\n charset=\"\"") +
                                                EME_UTILS_STRLEN(pstEmailMsg->pcPlainCharset) + 1);
               EME_UTILS_BREAKNULL(pcContentType, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_SPRINTF(pcContentType, "text/html;\r\n charset=\"%s\"", pstEmailMsg->pcHtmlCharset);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", pcContentType);
               EME_UTILS_FREEIF(pvMemMgr, pcContentType);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,
                                        pstEmailMsg, 
                                        "Content-Transfer-Encoding:", 
                                        pstEmailMsg->pcHtmlTransEnc);
            }

            
            /* add header End, build the header and add it to body */
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
                              iResult,
                              EME_NOMEMORY,
                              "EME_EncodingMailMsg, EME_MailMsg_GetHeader failed!!!");
            
            iResult = EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_ClearHeader failed!!!!");

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
            pstEmailMsg->pcEmailDataBuf = EME_UTILS_MALLOC(pvMemMgr, uiHeaderSize + uiBodySize);
            EME_UTILS_BREAKNULL(pstEmailMsg->pcEmailDataBuf, iResult, EME_NOMEMORY, "no memory for msg");
            EME_UTILS_MEMSET(pstEmailMsg->pcEmailDataBuf, 0, uiHeaderSize + uiBodySize);

            pstEmailMsg->uiEMailDataSize = uiHeaderSize + uiBodySize;

            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcHeaders);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
            
            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
            
            if (NULL != pstEmailMsg->pcTextPlain)
            {
               iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,TRUE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_IsAppendTextPlain failed!!!!");
            }
            else
            {
               iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,FALSE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_IsAppendTextPlain failed!!!!");
            }
         }
         
         /*text/plain and text/html both are exist*/
         else if (NULL != pstEmailMsg->pcTextPlain && NULL != pstEmailMsg->pcTextHtml)
         {
            pstEmailMsg->pcBoundary = EME_UTILS_STRDUP(pvMemMgr,"----=001_NextPart_000_003A_0495");      
            EME_UTILS_SPRINTF(newStr, "multipart/alternative; \r\n boundary=\"%s\"", pstEmailMsg->pcBoundary);
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", newStr);
            
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
               iResult,
               EME_NOMEMORY,
               "EME_EncodingMailMsg, EME_MailMsg_GetHeader failed!!!");
            
            EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
            pstEmailMsg->pcEmailDataBuf = EME_UTILS_MALLOC(pvMemMgr, uiHeaderSize + uiBodySize);
            EME_UTILS_BREAKNULL(pstEmailMsg->pcEmailDataBuf, iResult, EME_NOMEMORY, "no memory for msg");
            EME_UTILS_MEMSET(pstEmailMsg->pcEmailDataBuf, 0, uiHeaderSize + uiBodySize);

            pstEmailMsg->uiEMailDataSize = uiHeaderSize + uiBodySize;

            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcHeaders);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
            
            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "This is a multi-part message in MIME format.\r\n\r\n");
            
            /*add text/plain*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pstEmailMsg->pcBoundary, 
                              pstEmailMsg->pcPlainCharset, 
                              pstEmailMsg->pcPlainTransEnc);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

            iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,TRUE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_IsAppendTextPlain failed!!!!");
            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));

            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");

            /*add text/html*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pstEmailMsg->pcBoundary,
                              pstEmailMsg->pcHtmlCharset,
                              pstEmailMsg->pcHtmlTransEnc);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

            iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,FALSE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_IsAppendTextPlain failed!!!!");
            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));
   
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");
            EME_UTILS_SPRINTF(newStr, "--%s--", pstEmailMsg->pcBoundary);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
         }
         
         /*all is empty: no text/plain ,no text/plain, no attachment*/
         else
         {
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", "text/plain;\r\n   charset=\"utf-8\"");
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Transfer-Encoding:", "base64");
            
            /* add header End, build the header and add it to body */
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
                              iResult,
                              EME_NOMEMORY,
                              "EME_EncodingMailMsg, EME_MailMsg_GetHeader failed!!!");
            
            iResult = EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_ClearHeader failed!!!!");

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
            pstEmailMsg->pcEmailDataBuf = EME_UTILS_MALLOC(pvMemMgr, uiHeaderSize + uiBodySize);
            EME_UTILS_BREAKNULL(pstEmailMsg->pcEmailDataBuf, iResult, EME_NOMEMORY, "no memory for msg");
            EME_UTILS_MEMSET(pstEmailMsg->pcEmailDataBuf, 0, uiHeaderSize + uiBodySize);

            pstEmailMsg->uiEMailDataSize = uiHeaderSize + uiBodySize;

            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcHeaders);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
            
            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
         }
      }

      /*have some  attachment*/
      else
      {
         pstEmailMsg->pcBoundary = EME_UTILS_STRDUP(pvMemMgr,"----=001_NextPart_000_003A_0495");     
         
         /* we always use multipart/mixed for encode mail */     
         EME_UTILS_SPRINTF(newStr, "multipart/mixed;\r\n  boundary=\"%s\"", pstEmailMsg->pcBoundary);
         EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", newStr);
         
         pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
         EME_UTILS_BREAKNULL(pcHeaders,
            iResult,
            EME_NOMEMORY,
                  "EME_EncodingMailMsg, EME_MailMsg_GetHeader failed!!!");

         EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);

         uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
         pstEmailMsg->pcEmailDataBuf = EME_UTILS_MALLOC(pvMemMgr, uiHeaderSize + uiBodySize);
         EME_UTILS_BREAKNULL(pstEmailMsg->pcEmailDataBuf, iResult, EME_NOMEMORY, "no memory for msg");
         EME_UTILS_MEMSET(pstEmailMsg->pcEmailDataBuf, 0, uiHeaderSize + uiBodySize);

         pstEmailMsg->uiEMailDataSize = uiHeaderSize + uiBodySize;

         iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcHeaders);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
         
         EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
         
         EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "This is a multipart message in Mime 1.0 format.\r\n\r\n");
         
         /*either "text/plain" or "text/html" */
         if ( (NULL == pstEmailMsg->pcTextPlain  && NULL != pstEmailMsg->pcTextHtml)
             || (NULL != pstEmailMsg->pcTextPlain && NULL == pstEmailMsg->pcTextHtml))
         {

            if ( NULL != pstEmailMsg->pcTextPlain)
            {
               EME_UTILS_SPRINTF(newStr, 
                                 "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                                 pstEmailMsg->pcBoundary,
                                 pstEmailMsg->pcPlainCharset,
                                 pstEmailMsg->pcPlainTransEnc);
               iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

               iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,TRUE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,AppendTextPlain failed!!!!");
            }
            else
            {
               EME_UTILS_SPRINTF(newStr,
                                 "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                                 pstEmailMsg->pcBoundary,
                                 pstEmailMsg->pcHtmlCharset,
                                 pstEmailMsg->pcHtmlTransEnc);
               iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

               iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,FALSE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,AppendTextHtml failed!!!!");
            }

            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");
         }
         
         /*text/plain and text/html both are exist*/
         else if (NULL != pstEmailMsg->pcTextPlain && NULL != pstEmailMsg->pcTextHtml)
         {
            WE_CHAR *pcSecondBdary = EME_UTILS_STRDUP(pvMemMgr,"----=002_NextPart_000_003B_0495");
            
            EME_UTILS_SPRINTF(newStr, "--%s\r\nContent-Type: multipart/alternative;\r\n boundary=\"%s\"\r\n\r\n", 
               pstEmailMsg->pcBoundary,pcSecondBdary);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

            /*add text/plain*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pcSecondBdary,
                              pstEmailMsg->pcPlainCharset,
                              pstEmailMsg->pcPlainTransEnc);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

            iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,TRUE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,AppendTextPlain failed!!!!");

            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));
            
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");
            
            /*add text/html*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pcSecondBdary,
                              pstEmailMsg->pcHtmlCharset,
                              pstEmailMsg->pcHtmlTransEnc);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");

            iResult = EME_IsAppendTextPlain(pstEMsgHandle,pstEmailMsg,FALSE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,AppendTextHtml failed!!!!");

            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));

            EME_UTILS_SPRINTF(newStr, "\r\n\r\n--%s--", pcSecondBdary);
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
            EME_UTILS_FREEIF(pvMemMgr,pcSecondBdary);
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");
         }

         
        
         for (i = 0; i < pstEmailMsg->uiEMsgPartNum; i++)        
         {
            EME_UTILS_SPRINTF(newStr, "--%s\r\nContent-Type: %s", pstEmailMsg->pcBoundary, pstEmailMsg->ppstEMsgPart[i]->pcContentType);
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
            
            if (NULL != pstEmailMsg->ppstEMsgPart[i]->pcName)
            {
               WE_UINT32 uiSize = 0;
               pcEncodeWord = EME_HeaderEncode2Base64(pstEMsgHandle,pstEmailMsg->ppstEMsgPart[i]->pcName,E_CHARSET_UTF8);
               uiSize = EME_UTILS_STRLEN(pcEncodeWord) + EME_UTILS_STRLEN(";\r\n  name=\"\"\r\n")+ 1; /* +1 : + '\0' */
               pcTmp = EME_UTILS_MALLOC(pvMemMgr,uiSize);
               EME_UTILS_BREAKNULL(pcTmp,
                  iResult,
                  EME_NOMEMORY,
                  "EME_EncodingMailMsg, tmpStr malloc failed!!!");
               
               EME_UTILS_MEMSET(pcTmp,0x00,uiSize);
               EME_UTILS_SPRINTF(pcTmp, ";\r\n  name=\"%s\"\r\n", pcEncodeWord);
               EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);
               iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcTmp);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
               
               EME_UTILS_FREEIF(pvMemMgr,pcTmp);
            }
            else
            {
               EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n");
            }
            
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "Content-Transfer-Encoding: base64\r\n");
            
            if (NULL != pstEmailMsg->ppstEMsgPart[i]->pcFileName)
            {
               WE_UINT32 uiSize = 0;
               pcEncodeWord = EME_HeaderEncode2Base64(pstEMsgHandle,pstEmailMsg->ppstEMsgPart[i]->pcFileName,E_CHARSET_UTF8);
               uiSize   = EME_UTILS_STRLEN(pcEncodeWord) + EME_UTILS_STRLEN("Content-Disposition: attachment;\r\n  filename=\"\"\r\n") + 1; /* +1 : + '\0' */
               pcTmp     = EME_UTILS_MALLOC(pvMemMgr,uiSize);
               EME_UTILS_BREAKNULL(pcTmp,
                  iResult,
                  EME_NOMEMORY,
                  "EME_EncodingMailMsg,tmpStr malloc failed!!!");
               
               EME_UTILS_MEMSET(pcTmp,0x00,uiSize);
               EME_UTILS_SPRINTF(pcTmp, "Content-Disposition: attachment;\r\n  filename=\"%s\"\r\n", pcEncodeWord);
               EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);
               iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcTmp);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
               
               EME_UTILS_FREEIF(pvMemMgr,pcTmp);
            }
            else
            {
               EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "Content-Disposition: inline\r\n");
            }
            
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n");
            iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, 
               pstEmailMsg->ppstEMsgPart[i]->pcOnePartData);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
            
            EME_UTILS_FREEIF(pvMemMgr,pstEmailMsg->ppstEMsgPart[i]->pcOnePartData);
            EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n\r\n");
         }
         
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,failed!!!!");
         
         EME_UTILS_SPRINTF(newStr, "--%s--", pstEmailMsg->pcBoundary);
         iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, newStr);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
      }
      
      /* always use base64 encode, so "\n" not in encoded body. */
      EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, "\r\n");
      
   } while(0);

   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
      EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);
   }
   
   return iResult;
}

/* return encoded str to user. */
void *EME_MailMsg_GetEncodeBuf(void* pstIn)
{
   St_EmailMsg *pstEmailMsg = (St_EmailMsg*)pstIn;
   WE_CHAR     *pcBuf = NULL;

   if (NULL == pstEmailMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_GetEncodeBuf(),input parameters is invalid!!!!"));
      return NULL;
   }
   
   pcBuf = pstEmailMsg->pcEmailDataBuf;

   pstEmailMsg->pcEmailDataBuf = NULL;

   return (void*)pcBuf;
}

WE_INT32 EME_MailMsg_ParseDataBuf(stEMSGHandle *pstEMsgHandle,
                                  void* pstIn, 
                                  WE_CHAR* src, 
                                  WE_BOOL bOnlyHeader)
{   
   WE_INT32     iResult          = EME_SUCCESS ; /*must be init success*/
   WE_CHAR      *pos             = NULL;
   WE_CHAR      *pcBodyPartBuf   = NULL;
   void         *pvMemMgr        = NULL;
   St_EmailMsg *pstEmailMsg      = (St_EmailMsg*)pstIn;
   if (NULL == pstEMsgHandle || pstEmailMsg == NULL || NULL == src)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseDataBuf(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if ((pos = EME_UTILS_STRSTR(src, "\n\n")) != NULL|| 
          (pos = EME_UTILS_STRSTR(src, "\r\n\r\n")) != NULL)
      {
         /*demand only parse header*/
         if (bOnlyHeader)
         {
            iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, src, pos);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBuf,EME_MailMsg_ParseHeaders failed!!!!");
         }

         else
         {
            iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, src, pos);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBuf,EME_MailMsg_ParseHeaders,failed!!!!");
            
            //pcBodyPartBuf = EME_UTILS_STRDUP(pvMemMgr,pos + 2);/* +2 means + "\n\n" */ 
            pcBodyPartBuf = pos + 2;/* +2 means + "\n\n" */ 
            EME_UTILS_BREAKNULL(pcBodyPartBuf,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParseDataBuf,EME_UTILS_STRDUP  failed!!!");
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBuf,EME_MailMsg_ParseHeaders failed!!!!");
            
            iResult = EME_MailMsg_ParseMsgBody(pstEMsgHandle,pstEmailMsg,pcBodyPartBuf);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBuf, EME_MailMsg_ParseMsgBody failed!!!!");
            
            /* should free the body buff */
            //EME_UTILS_FREEIF(pvMemMgr,pcBodyPartBuf);

         }      
      }
      else /* the Email only has header */
      { 
         iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, src, src + EME_UTILS_STRLEN(src) -1);
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBuf,EME_MailMsg_ParseHeaders,failed!!!!");
      }

   } while(0);

   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
//      EME_UTILS_FREEIF(pvMemMgr,pcBodyPartBuf);
   }
   
   return iResult;
}

/* ====================mail message pstEMsgPart functions====================== */

WE_INT32 EME_MailMsgPart_Create(stEMSGHandle *pstEMsgHandle,void** ppVoid)
{
   WE_INT32          iResult       = EME_SUCCESS; /*must init to success*/
   void              *pvMemMgr     = NULL;
   St_EmailMsgPart   *pstMailpart  = NULL;

   if (NULL == pstEMsgHandle || NULL == ppVoid)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_Create(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      pstMailpart = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_EmailMsgPart));
      EME_UTILS_BREAKNULL(pstMailpart,
         iResult,
         EME_NOMEMORY,
         "memory malloc failed!!!!");
      
      EME_UTILS_MEMSET(pstMailpart,0x00,sizeof(St_EmailMsgPart));
      *ppVoid = pstMailpart;
      
   } while(0);
 
   return iResult;
}


WE_INT32 EME_MailMsgPart_Destroy(stEMSGHandle *pstEMsgHandle,void** ppVoid)
{
   void            *pvMemMgr         = NULL;
   St_EmailMsgPart *pstEmailMsgPart  = NULL;

   if (NULL == pstEMsgHandle || NULL == ppVoid)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_Destroy(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pstEmailMsgPart = (St_EmailMsgPart*)(*ppVoid);
   if (NULL == pstEmailMsgPart)
   {
      EME_UTILS_LOG_INFO(("EME_MailMsgPart_Destroy pstEMsgPart is Null ,do not need to free!!!!"));
      return EME_SUCCESS;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcOnePartData);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcName);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcFileName);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcContentType);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcContentId);
//   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcContentLoc);
   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart->pcTransEnc);
   EME_UTILS_FREEIF(pvMemMgr, pstEmailMsgPart->pcCharSet);

   EME_UTILS_FREEIF(pvMemMgr,pstEmailMsgPart);
   *ppVoid = NULL;

   return EME_SUCCESS;
}


WE_INT32 EME_MailMsgPart_SetName(stEMSGHandle *pstEMsgHandle,
                                 void *pstIn, 
                                 WE_CHAR *pcName)
{
   WE_INT32        iResult         = EME_SUCCESS ; /*must be init to SUCCESS*/
   St_EmailMsgPart *pstEmlMsgPart  = (St_EmailMsgPart*)pstIn;
   void            *pvMemMgr       = NULL;

   if ( NULL == pstEMsgHandle || NULL == pstEmlMsgPart ||NULL == pcName )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_SetName(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   
   do 
   {
      pstEmlMsgPart->pcName = EME_UTILS_STRDUP(pvMemMgr, pcName);
      EME_UTILS_BREAKNULL(pstEmlMsgPart->pcName, 
                          iResult,
                          EME_NOMEMORY,
                          "EME_MailMsgPart_SetName, no memory!!!");

   } while(0);
   
   return iResult;
}


WE_INT32 EME_MailMsgPart_SetFileName(stEMSGHandle *pstEMsgHandle,
                                     void* pstIn, 
                                     WE_CHAR* pcFileName)
{
   WE_INT32        iResult                = EME_SUCCESS ; /*must be init to SUCCESS*/
   St_EmailMsgPart *pstEmlMsgPart         = (St_EmailMsgPart*)pstIn;
   void            *pvMemMgr              = NULL;
   if ( NULL == pstEMsgHandle || NULL ==  pstEmlMsgPart || NULL == pcFileName )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_SetFileName(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      pstEmlMsgPart->pcFileName = EME_UTILS_STRDUP(pvMemMgr, pcFileName);
      EME_UTILS_BREAKNULL(pstEmlMsgPart->pcFileName, 
                          iResult,
                          EME_NOMEMORY,
                          "EME_MailMsgPart_SetName,no memory!!!");

   } while(0);
     
   return iResult;
}



/* type param is ext of file, exp: "jpg", "mp3", "asf"... */ 
WE_INT32 EME_MailMsgPart_SetContType(stEMSGHandle *pstEMsgHandle,
                                     void* pstIn, 
                                     WE_CHAR* pcMimeType)
{
   WE_INT32 iResult          = EME_SUCCESS;  /*must be init to SUCCESS*/
   void     *pvMemMgr        = NULL;
   St_EmailMsgPart *pstEmlMsgPart = (St_EmailMsgPart *)pstIn;

   if (NULL == pstEMsgHandle || NULL == pstEmlMsgPart || NULL == pcMimeType)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_SetContType(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      /* the content type is not important for decode attachment */ 
      pstEmlMsgPart->pcContentType = EME_UTILS_STRDUP(pvMemMgr,pcMimeType);
      EME_UTILS_BREAKNULL(pstEmlMsgPart->pcContentType,
                           iResult,
                           EME_NOMEMORY,
                           "EME_MailMsgPart_SetContType  EME_UTILS_STRDUP failed!!!");

   } while(0);
 
   return iResult;
}


WE_INT32 EME_MailMsgPart_SetBodyData(stEMSGHandle *pstEMsgHandle,
                                     void* pstIn, 
                                     WE_CHAR* pcBodyPartData, 
                                     WE_UINT32 uiSize)
{
   WE_INT32 iResult               = EME_SUCCESS; /*must init to SUCCESS*/
   WE_UINT32 uiOnePartSize        = 0;           /* be base64 format */
   WE_CHAR *pcBuff                = NULL;
   St_EmailMsgPart *pstEmlMsgPart = (St_EmailMsgPart*)pstIn;
   void *pvMemMgr                 = NULL;
   if (NULL == pstEMsgHandle || NULL == pstEmlMsgPart || pcBodyPartData == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_SetBodyData(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
     /* get encoded string size from original string */
      uiOnePartSize = EME_Base64EncodeSize(uiSize) + 1;   
      pcBuff = EME_UTILS_MALLOC(pvMemMgr,uiOnePartSize);
      EME_UTILS_BREAKNULL(pcBuff,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsgPart_SetBodyData malloc failed!!!");

      EME_UTILS_MEMSET(pcBuff,0x00,uiOnePartSize);
      pstEmlMsgPart->pcOnePartData = pcBuff;
      EME_ToBase64(pstEmlMsgPart->pcOnePartData, pcBodyPartData, uiSize, 0);

   } while(0);
   
   return iResult;
}


// WE_INT32 EME_MailMsgPart_GetDecodeBd(stEMSGHandle *pstEMsgHandle,
//                                      St_EmailMsgPart* pstIn, 
//                                      WE_CHAR** ppcRet)
// {
//    WE_INT32        iResult          = EME_SUCCESS; /*must be init success*/
//    WE_UINT32       uiBase64Size     = 0;
//    void            *pvMemMgr        = NULL;
//    St_EmailMsgPart *pstEmailMsgPart = pstIn;
//    
//    if (NULL == pstEMsgHandle || pstEmailMsgPart == NULL ||
//       pstEmailMsgPart->pcOnePartData == NULL || NULL == ppcRet)
//    {
//       EME_UTILS_LOG_ERROR(("EME_MailMsgPart_GetDecodeBd(),input parameters is invalid!!!!"));
//       return EME_FAILED;
//    }
//    pvMemMgr = pstEMsgHandle->pvMemMgr;
//    
//    do 
//    {
//       uiBase64Size = EME_Base64DecodeSize(EME_UTILS_STRLEN(pstEmailMsgPart->pcOnePartData)) + 1;   
//       *ppcRet = EME_UTILS_MALLOC(pvMemMgr,uiBase64Size); 
//       EME_UTILS_BREAKNULL(*ppcRet,
//          iResult,
//          EME_NOMEMORY,
//          "EME_MailMsgPart_GetDecodeBd, EME_Base64DecodeSize malloc failed!!!");
//       
//       EME_UTILS_MEMSET(*ppcRet,0x00,uiBase64Size);
//       
//       pstEmailMsgPart->uiOnePartSize = EME_FromBase64(*ppcRet, pstEmailMsgPart->pcOnePartData);
//       
//       
//    } while(0);
//    
//    return iResult;
// }

static WE_INT32 EME_IsAppendTextPlain(stEMSGHandle *pstEMsgHandle,
                                      St_EmailMsg *pstEmailMsg,
                                      WE_BOOL bIsTextPlain)
{
  void     *pvMemMgr           = NULL;
  WE_INT32 iResult             = EME_SUCCESS; /*must be init to SUCCESS*/
  WE_CHAR  *pcTextPLainOrHtml  = NULL;
  WE_CHAR  *pcTmp              = NULL;
  WE_UINT32 uiTobase64Len      = 0;
  WE_UINT32 uiSize             = 0;
  E_EncodeType eEncode         = E_ENCODETYPE_BASE64;
  E_Charset eCharset           = E_CHARSET_UTF8;
  WE_CHAR   *pcOut             = NULL;

  if (NULL == pstEMsgHandle || NULL == pstEmailMsg )
  {
     return EME_FAILED;
  }

  pvMemMgr = pstEMsgHandle->pvMemMgr;
  do 
  {
     if (bIsTextPlain)
     {
        uiSize = EME_UTILS_STRLEN(pstEmailMsg->pcTextPlain);
        pcTmp = pstEmailMsg->pcTextPlain;
        eCharset = EME_MailMsg_ParseChaset(pcTmp, pstEmailMsg->pcPlainCharset);
        eEncode = EME_MailMsg_ParseTransEncoding(pstEmailMsg->pcPlainTransEnc);
     }

     else
     {
        uiSize = EME_UTILS_STRLEN(pstEmailMsg->pcTextHtml);
        pcTmp = pstEmailMsg->pcTextHtml;
        eCharset = EME_MailMsg_ParseChaset(pcTmp, pstEmailMsg->pcHtmlCharset);
        eEncode = EME_MailMsg_ParseTransEncoding(pstEmailMsg->pcHtmlTransEnc);
     }

     iResult = EME_Utf8ToXX(pstEMsgHandle->pICSC,
                            pvMemMgr,
                            pcTmp,
                            EME_UTILS_STRLEN(pcTmp),
                            eCharset,
                            (WE_VOID**)&pcOut,
                            &uiSize);
     EME_UTILS_BREAKIF(iResult, "charset convert failed");
     EME_UTILS_BREAKNULL(pcOut, iResult, EME_FAILED, "charset convert failed");

     uiTobase64Len = EME_Base64EncodeSize(uiSize) + 1;
     pcTextPLainOrHtml = EME_UTILS_MALLOC(pvMemMgr,uiTobase64Len);
     EME_UTILS_BREAKNULL(pcTextPLainOrHtml,
                       iResult,
                       EME_NOMEMORY,
                       "EME_EncodingMailMsg newBody malloc failed!!!");
     EME_UTILS_MEMSET(pcTextPLainOrHtml,0x00,uiTobase64Len);

     EME_ToBase64(pcTextPLainOrHtml, pcOut, 0, 0);

     iResult = EME_MailMsg_StrcatToBody(pstEMsgHandle,pstEmailMsg, pcTextPLainOrHtml);
     EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_MailMsg_StrcatToBody failed!!!!");
     
     EME_UTILS_FREEIF(pvMemMgr,pcOut);
     EME_UTILS_FREEIF(pvMemMgr,pcTextPLainOrHtml);

  } while(0);

   return iResult;
}

static WE_UINT32 EME_MailMsg_CaluBodySize(St_EmailMsg *pstEmailMsg)
{
   WE_UINT32   uiSize = 0;
   WE_UINT32   uiPartsNum = 0;
               
   if (pstEmailMsg == NULL)
   {
      return 0;
   }

   uiSize += EME_MAX_HEADER_FORMART_SIZE;

   if (pstEmailMsg->pcTextPlain != NULL)
   {
      uiSize += EME_MAX_BODYHEADER_SIZE;
      uiSize += EME_Base64EncodeSize(EME_UTILS_STRLEN(pstEmailMsg->pcTextPlain) + 1);
   }
   if (pstEmailMsg->pwcTextHtml != NULL)
   {
      uiSize += EME_MAX_BODYHEADER_SIZE;
      uiSize += EME_Base64EncodeSize(EME_UTILS_STRLEN(pstEmailMsg->pcHtmlCharset) + 1);
   }

   for (uiPartsNum = 0; uiPartsNum < pstEmailMsg->uiEMsgPartNum; uiPartsNum++)
   {
      uiSize += EME_MAX_BODYHEADER_SIZE;

      if (pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcFileName != NULL)
      {
         uiSize += EME_Base64EncodeSize(EME_UTILS_STRLEN(pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcFileName));
      }
      if (pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcName != NULL)
      {
         uiSize += EME_Base64EncodeSize(EME_UTILS_STRLEN(pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcName));
      }

      if (pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcOnePartData != NULL)
      {
         uiSize += EME_UTILS_STRLEN(pstEmailMsg->ppstEMsgPart[uiPartsNum]->pcOnePartData);
      }
   }

   return uiSize;
}


static WE_CHAR *EME_MailMsg_DecodeWord(stEMSGHandle *pstEMsgHandle,
                                       WE_CHAR* pcStart, 
                                       WE_CHAR* pcEnd)
{
   WE_CHAR     *pcWord = NULL;
   WE_INT32    iResult = EME_SUCCESS;
   WE_CHAR     *pcCharset = NULL;
   WE_CHAR     cEncodeId = 0;         /* "B" or "Q" */
   WE_CHAR     *pcCurr = NULL;
   WE_CHAR     *pcText = NULL;
   WE_CHAR     *pcOut = NULL;
   WE_CHAR     *pcTmpBuf = NULL;
   WE_CHAR     *pcDecodedBuf = NULL;
   WE_VOID     *pvMemMgr = NULL;
   E_Charset   eCharset = E_CHARSET_UTF8;
   WE_UINT32   uiOutSize = 0;


   if (NULL == pstEMsgHandle || NULL == pcStart || NULL == pcEnd )
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeWord(),input parameters is invalid!!!!"));
      return NULL;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   if (pcEnd < pcStart)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodeWord , the word decode invalid!!!!"));
      return NULL;
   }

   do 
   {
   	pcWord = EME_UTILS_STRDUP(pvMemMgr, pcStart);
      EME_UTILS_BREAKNULL(pcWord, iResult, EME_NOMEMORY, "no memory");
      pcCurr = pcWord;

      while ((WE_INT32)pcCurr < (WE_INT32)(pcWord + (pcEnd - pcStart)))
      {
         /* get charset */
         pcCurr = EME_UTILS_STRSTR(pcCurr, "=?");
         EME_UTILS_BREAKNULL(pcCurr, iResult, EME_FAILED, "parse failed");
         pcCurr += 2;
         /* defualt charset */
         if (*pcCurr == '?')
         {
            pcCharset = NULL;
         }
         else
         {
            pcCharset = pcCurr;
         }
         pcCurr = EME_UTILS_STRCHR(pcCurr, '?');
         *pcCurr = '\0';
         pcCurr++;

         /* get encoding */
         if (*pcCurr != 'B' && *pcCurr != 'Q' && *pcCurr != 'b' && *pcCurr != 'q')
         {
            iResult = EME_FAILED;
            break;
         }
         cEncodeId = *pcCurr;

         /* get text */
         pcCurr += 2;   /* skip encoding and ? */
         pcText = pcCurr;
         pcCurr = EME_UTILS_STRSTR(pcCurr, "?=");
         *pcCurr = '\0';
         pcCurr += 2;   /* skip "?=" */

         if (cEncodeId == 'Q' || cEncodeId == 'q')
         {
            WE_UINT32 len = EME_QuotedDecodeSize(EME_UTILS_STRLEN(pcText));
            pcTmpBuf = EME_UTILS_MALLOC(pvMemMgr,len + 1);
            EME_UTILS_BREAKNULL(pcTmpBuf,
                     iResult,
                     EME_NOMEMORY,
                     "EME_MailMsg_DecodeWord malloc failed!!!");
            EME_UTILS_MEMSET(pcTmpBuf,0x00,len + 1);

            EME_FromQuoted(pcTmpBuf, pcText, EME_UTILS_STRLEN(pcText));
         }
         else
         {
            WE_UINT32 len = EME_Base64DecodeSize(EME_UTILS_STRLEN(pcText));
            pcTmpBuf = EME_UTILS_MALLOC(pvMemMgr,len + 1);
            EME_UTILS_BREAKNULL(pcTmpBuf,
                              iResult,
                              EME_NOMEMORY,
                              "EME_MailMsg_DecodeWord malloc failed!!!");

            EME_UTILS_MEMSET(pcTmpBuf,0x00,len + 1);
            EME_FromBase64(pcTmpBuf, pcText, EME_UTILS_STRLEN(pcText));
         }   
         
         /* charset convert */ 
         if (pcCharset != NULL)
         {
            eCharset = EME_MailMsg_ParseChaset(pcTmpBuf, pcCharset);
         }
         /*default charset*/
         else
         {
            WE_UINT8     *pucChByte = (WE_UINT8*)pcTmpBuf;

            eCharset = E_CHARSET_US_ASCII;
            for (; EME_UTILS_STRLEN((WE_CHAR*)pucChByte) > 0; pucChByte++)
            {
               if (*pucChByte > 128)
               {
                  eCharset = E_CHARSET_GB2312;
                  break;
               }
            }
         }
         
         iResult = EME_XXToUtf8(pstEMsgHandle->pICSC,
                               pvMemMgr,
                               pcTmpBuf,
                               EME_UTILS_STRLEN(pcTmpBuf),
                               eCharset,
                               &pcDecodedBuf,
                               &uiOutSize);
         EME_UTILS_BREAKIF(iResult, "charset convert failed");
         EME_UTILS_BREAKNULL(pcDecodedBuf, iResult, EME_FAILED, "charset convert failed");
         
         EME_UTILS_FREEIF(pvMemMgr, pcTmpBuf);

         if (pcOut == NULL)
         {
            pcOut = pcDecodedBuf;
            pcDecodedBuf = NULL;
         }
         else
         {
            WE_INT32    iOrigLen = 0;
            WE_INT32    iAddLen = 0;
            iOrigLen = EME_UTILS_STRLEN(pcOut);
            iAddLen = EME_UTILS_STRLEN(pcDecodedBuf);
            
            pcOut = EME_UTILS_STRREALLOC(pvMemMgr, pcOut, iOrigLen + iAddLen + 1);
            EME_UTILS_BREAKNULL(pcOut, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pcOut + iOrigLen, 0, iAddLen + 1);

            EME_UTILS_MEMCPY(pcOut + iOrigLen, pcDecodedBuf, iAddLen);
            EME_UTILS_FREEIF(pvMemMgr, pcDecodedBuf);
         }
      }

   } while(0);

   EME_UTILS_FREEIF(pvMemMgr, pcWord);
   EME_UTILS_FREEIF(pvMemMgr, pcTmpBuf);
   EME_UTILS_FREEIF(pvMemMgr, pcDecodedBuf);

   return pcOut;
}



/***********************************************************************************************
                                    Encoding & Decoding of Synchronism.
 ***********************************************************************************************/

/****************************** Encoding email message ******************************/
static WE_INT32 EME_IsAppendTextPlainEx(stEMSGHandle *pstEMsgHandle,
                                      St_EmailMsg *pstEmailMsg,
                                      WE_BOOL bIsTextPlain)
{
  void     *pvMemMgr           = NULL;
  WE_INT32 iResult             = EME_SUCCESS; /*must be init to SUCCESS*/
  WE_CHAR  *pcTextPLainOrHtml  = NULL;
  WE_CHAR  *pcTmp              = NULL;
  WE_UINT32 uiTobase64Len      = 0;
  WE_UINT32 uiSize             = 0;
  E_EncodeType eEncode         = E_ENCODETYPE_BASE64;
  E_Charset eCharset           = E_CHARSET_UTF8;
  WE_CHAR   *pcOut             = NULL;

  WE_UINT32 uiHadWrite = 0;

  EME_UTILS_LOG_INFO(("EME_IsAppendTextPlainEx---: Start"));

  if (NULL == pstEMsgHandle || NULL == pstEmailMsg )
  {
     EME_UTILS_LOG_ERROR(("EME_IsAppendTextPlainEx--: Bad parameter and return"));
     return EME_FAILED;
  }

  pvMemMgr = pstEMsgHandle->pvMemMgr;
  do 
  {
     if (bIsTextPlain)
     {
        uiSize = EME_UTILS_STRLEN(pstEmailMsg->pcTextPlain);
        pcTmp = pstEmailMsg->pcTextPlain;
        eCharset = EME_MailMsg_ParseChaset(pcTmp, pstEmailMsg->pcPlainCharset);
        eEncode = EME_MailMsg_ParseTransEncoding(pstEmailMsg->pcPlainTransEnc);
     }

     else
     {
        uiSize = EME_UTILS_STRLEN(pstEmailMsg->pcTextHtml);
        pcTmp = pstEmailMsg->pcTextHtml;
        eCharset = EME_MailMsg_ParseChaset(pcTmp, pstEmailMsg->pcHtmlCharset);
        eEncode = EME_MailMsg_ParseTransEncoding(pstEmailMsg->pcHtmlTransEnc);
     }

     iResult = EME_Utf8ToXX(pstEMsgHandle->pICSC,
                            pvMemMgr,
                            pcTmp,
                            EME_UTILS_STRLEN(pcTmp),
                            eCharset,
                            (WE_VOID**)&pcOut,
                            &uiSize);
     EME_UTILS_BREAKIF(iResult, "EME_IsAppendTextPlainEx---: charset convert failed");
     EME_UTILS_BREAKNULL(pcOut, iResult, EME_FAILED, "EME_IsAppendTextPlainEx---: charset convert failed");

     uiTobase64Len = EME_Base64EncodeSize(uiSize) + 1;
     pcTextPLainOrHtml = EME_UTILS_MALLOC(pvMemMgr,uiTobase64Len);
     EME_UTILS_BREAKNULL(pcTextPLainOrHtml,
                       iResult,
                       EME_NOMEMORY,
                       "EME_IsAppendTextPlainEx---: newBody malloc failed!!!");
     EME_UTILS_MEMSET(pcTextPLainOrHtml,0x00,uiTobase64Len);

     EME_ToBase64(pcTextPLainOrHtml, pcOut, 0, 0);

     uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, pcTextPLainOrHtml, EME_UTILS_STRLEN(pcTextPLainOrHtml));
     iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(pcTextPLainOrHtml)) ? EME_SUCCESS : EME_FAILED;
     EME_UTILS_BREAKIF(iResult, "EME_IsAppendTextPlainEx---: write to file failed");
     
     EME_UTILS_FREEIF(pvMemMgr,pcOut);
     EME_UTILS_FREEIF(pvMemMgr,pcTextPLainOrHtml);

  } while(0);

  EME_UTILS_LOG_INFO(("EME_IsAppendTextPlainEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_MailMsgPart_SetBodyDataEx(stEMSGHandle *pstEMsgHandle,
                                     void* pstIn, 
                                     WE_CHAR* pcBodyPartData, 
                                     WE_UINT32 uiSize)
{
   WE_INT32 iResult               = EME_SUCCESS; /*must init to SUCCESS*/
   St_EmailMsgPart *pstEmlMsgPart = (St_EmailMsgPart*)pstIn;
   void *pvMemMgr                 = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsgPart_SetBodyDataEx---: Start"));
   if (NULL == pstEMsgHandle || NULL == pstEmlMsgPart || pcBodyPartData == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsgPart_SetBodyDataEx---: Bad parameter and return"));
      return EME_FAILED;
   }
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
     /* get encoded string size from original string */
      pstEmlMsgPart->pcOnePartData = EME_UTILS_STRDUP(pvMemMgr, pcBodyPartData);
      EME_UTILS_BREAKNULL(pstEmlMsgPart->pcOnePartData, 
                          iResult,
                          EME_NOMEMORY,
                          "EME_MailMsgPart_SetBodyDataEx---: no memory!!!");

      pstEmlMsgPart->uiOnePartSize = uiSize;
   } while(0);
   
   EME_UTILS_LOG_INFO(("EME_MailMsgPart_SetBodyDataEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_EncodingMailMsgEx(stEMSGHandle *pstEMsgHandle,void* pstIn, WE_CHAR *pcDir)
{
   WE_INT32   iResult               = EME_FAILED;
   void       *pvMemMgr             = NULL;
   WE_CHAR    *pcTmp                = NULL;
   WE_CHAR    *pcEncodeWord         = NULL;
   WE_CHAR    *pcHeaders            = NULL;
   WE_CHAR    *pcContentType = NULL;
   WE_CHAR    newStr[EME_MAXSTRLEN]  = {0};
   St_EmailMsg *pstEmailMsg         = (St_EmailMsg*)pstIn;
   //WE_UINT32  uiBodySize            = 0;
   WE_UINT32  uiHeaderSize          = 0;
   
   WE_UINT32  uiFileNameSize = 0;
   WE_CHAR    *pcMark = NULL;

   WE_UINT32  uiHadWriteSize = 0;

   EME_UTILS_LOG_INFO(("EME_EncodingMailMsgEx---: Start"));
   if (NULL == pstEmailMsg || NULL == pstEMsgHandle || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EME_EncodingMailMsgEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      //EME_UTILS_BREAKNULL(pstEMsgHandle->pIFileMgr, iResult, EME_FAILED, "EME_EncodingMailMsgEx---: file mgr handler is null");
      pstEmailMsg->bIsDecoding = FALSE;
      /*create temp email file name*/
      uiFileNameSize += EME_UTILS_STRLEN(pcDir);
      uiFileNameSize += EME_MAX_EMAIL_FILENAME_LEN;

      pstEmailMsg->pcMailFileName = EME_UTILS_MALLOC(pvMemMgr, uiFileNameSize);
      EME_UTILS_BREAKNULL(pstEmailMsg->pcMailFileName, iResult, EME_NOMEMORY, "EME_EncodingMailMsgEx---: create tmp file name failed");
      EME_UTILS_MEMSET(pstEmailMsg->pcMailFileName, 0, uiFileNameSize);

      pcMark = pcDir + EME_UTILS_STRLEN(pcDir) - 1;
      if(pcMark != NULL && *pcMark == '\\')
      {
         EME_UTILS_SPRINTF(pstEmailMsg->pcMailFileName, "%seml%x.eml", pcDir, EME_UTILS_GETTIMEMS);
      }
      else
      {
         EME_UTILS_SPRINTF(pstEmailMsg->pcMailFileName, "%s\\eml%x.eml", pcDir, EME_UTILS_GETTIMEMS);
      }
      
      /*create temp file*/
      pstEmailMsg->pvMailFile = EME_FileOpen(pstEMsgHandle->pIFileMgr, pstEmailMsg->pcMailFileName, EME_OFM_CREATE);
      EME_UTILS_BREAKNULL(pstEmailMsg->pvMailFile, iResult, EME_FAILED, "EME_EncodingMailMsgEx---: create temp file faild");

      /*append from header*/
      if (pstEmailMsg->pcFrom != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "From:", pstEmailMsg->pcFrom);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader FROM failed!!!!");
      }
      
      /*append To to header*/
      if (pstEmailMsg->pcTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "To:", pstEmailMsg->pcTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader TO failed!!!!");
      }
      
      /*append Cc to header*/
      if (pstEmailMsg->pcCc != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Cc:", pstEmailMsg->pcCc);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader CC failed!!!!");
      }

      /*append Bcc to header*/
      if (pstEmailMsg->pcBcc != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Bcc:", pstEmailMsg->pcBcc);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Bcc failed!!!!");
      }
      
      /*append To Date time header*/
      if (pstEmailMsg->pcDate != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Date:", pstEmailMsg->pcDate);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Date failed!!!!");
      }
      
      /*append Reply-To to header*/
      if (pstEmailMsg->pcReplyTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Reply-To:", pstEmailMsg->pcReplyTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Reply-To failed!!!!");
      }
      
      iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "X-Mailer:", "TF-Mail 1.0");
      EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Reply-To failed!!!!");
      

      /*append subject to header*/
      if (pstEmailMsg->pcSubject != NULL)
      {
         pcTmp = EME_HeaderEncode2Base64(pstEMsgHandle,pstEmailMsg->pcSubject,E_CHARSET_UTF8);
         EME_UTILS_BREAKNULL(pcTmp,
            iResult,
            EME_NOMEMORY,
            "EME_EncodingMailMsgEx---: EME_Encode2Base64 malloc failed!!!");
         
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Subject:", pcTmp);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Subject failed!!!!");
         
         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      }
      else
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Subject:", " ");
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Subject failed!!!!");
      }
      
      /*append priority to header*/
      if (NULL != pstEmailMsg->pcPriority)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "X-Priority:", pstEmailMsg->pcPriority);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader X-Priority failed!!!!");
      }
      
      /*append Disposition-Notification-To to header*/
      if (pstEmailMsg->pcNotifyTo != NULL)
      {
         iResult = EME_MailMsg_UpdateHeader(pstEMsgHandle, 
                                            pstEmailMsg, 
                                            "Disposition-Notification-To:", 
                                            pstEmailMsg->pcNotifyTo);
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_UpdateHeader Disposition-Notification-To failed!!!!");
      }
      
      //uiBodySize = EME_MailMsg_CaluBodySize(pstEmailMsg);

      /*append Mime-Version to header*/
      EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Mime-Version:", "1.0");
      
      /*Have no attachment */
      if (pstEmailMsg->uiEMsgPartNum == 0)
      {
         if ( (NULL == pstEmailMsg->pcTextPlain  && NULL != pstEmailMsg->pcTextHtml)
             || (NULL != pstEmailMsg->pcTextPlain && NULL == pstEmailMsg->pcTextHtml))
         {
           
            if ( NULL != pstEmailMsg->pcTextPlain)
            {
               pcContentType = EME_UTILS_MALLOC(pvMemMgr, 
                                                EME_UTILS_STRLEN("text/plain;\r\n charset=\"\"") +
                                                EME_UTILS_STRLEN(pstEmailMsg->pcPlainCharset) + 1);
               EME_UTILS_BREAKNULL(pcContentType, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_SPRINTF(pcContentType, "text/plain;\r\n charset=\"%s\"", pstEmailMsg->pcPlainCharset);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", pcContentType);
               EME_UTILS_FREEIF(pvMemMgr, pcContentType);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,
                                        pstEmailMsg, 
                                        "Content-Transfer-Encoding:", 
                                        pstEmailMsg->pcPlainTransEnc);
            }
            else
            {
               pcContentType = EME_UTILS_MALLOC(pvMemMgr, 
                                                EME_UTILS_STRLEN("text/html;\r\n charset=\"\"") +
                                                EME_UTILS_STRLEN(pstEmailMsg->pcHtmlCharset) + 1);
               EME_UTILS_BREAKNULL(pcContentType, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_SPRINTF(pcContentType, "text/html;\r\n charset=\"%s\"", pstEmailMsg->pcHtmlCharset);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", pcContentType);
               EME_UTILS_FREEIF(pvMemMgr, pcContentType);

               EME_MailMsg_UpdateHeader(pstEMsgHandle,
                                        pstEmailMsg, 
                                        "Content-Transfer-Encoding:", 
                                        pstEmailMsg->pcHtmlTransEnc);
            }

            
            /* add header End, build the header and add it to body */
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
                              iResult,
                              EME_NOMEMORY,
                              "EME_EncodingMailMsgEx---: EME_MailMsg_GetHeader failed!!!");
            
            iResult = EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_ClearHeader failed!!!!");

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, pcHeaders, uiHeaderSize);
            iResult = (uiHeaderSize == uiHadWriteSize) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write head failed");
            
            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
            
            if (NULL != pstEmailMsg->pcTextPlain)
            {
               iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,TRUE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_IsAppendTextPlain failed!!!!");
            }
            else
            {
               iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,FALSE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_IsAppendTextPlain failed!!!!");
            }
         }
         
         /*text/plain and text/html both are exist*/
         else if (NULL != pstEmailMsg->pcTextPlain && NULL != pstEmailMsg->pcTextHtml)
         {
            pstEmailMsg->pcBoundary = EME_UTILS_STRDUP(pvMemMgr,"----=001_NextPart_000_003A_0495");      
            EME_UTILS_SPRINTF(newStr, "multipart/alternative; \r\n boundary=\"%s\"", pstEmailMsg->pcBoundary);
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", newStr);
            
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
               iResult,
               EME_NOMEMORY,
               "EME_EncodingMailMsgEx---: EME_MailMsg_GetHeader failed!!!");
            
            EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, pcHeaders, uiHeaderSize);
            iResult = (uiHeaderSize == uiHadWriteSize) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write head failed");
            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "This is a multi-part message in MIME format.\r\n\r\n",
               EME_UTILS_STRLEN("This is a multi-part message in MIME format.\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("This is a multi-part message in MIME format.\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
            
            /*add text/plain*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pstEmailMsg->pcBoundary, 
                              pstEmailMsg->pcPlainCharset, 
                              pstEmailMsg->pcPlainTransEnc);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,TRUE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,EME_IsAppendTextPlain failed!!!!");
            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "write to file failed");

            /*add text/html*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pstEmailMsg->pcBoundary,
                              pstEmailMsg->pcHtmlCharset,
                              pstEmailMsg->pcHtmlTransEnc);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,FALSE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_IsAppendTextPlain failed!!!!");
            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));
   
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            EME_UTILS_SPRINTF(newStr, "--%s--", pstEmailMsg->pcBoundary);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
         }
         
         /*all is empty: no text/plain ,no text/plain, no attachment*/
         else
         {
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", "text/plain;\r\n   charset=\"utf-8\"");
            EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Transfer-Encoding:", "base64");
            
            /* add header End, build the header and add it to body */
            pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKNULL(pcHeaders,
                              iResult,
                              EME_NOMEMORY,
                              "EME_EncodingMailMsgEx---: EME_MailMsg_GetHeader failed!!!");
            
            iResult = EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: EME_MailMsg_ClearHeader failed!!!!");

            uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, pcHeaders, uiHeaderSize);
            iResult = (uiHadWriteSize == uiHeaderSize) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
         }

         pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult = EME_SUCCESS;
         if(pstEMsgHandle->pfnEncoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd, pstEMsgHandle);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: set timer failed");

            /*register the callback*/
            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
         else
         {
            EME_MailMsg_EncodingPartsEnd(pstEMsgHandle);
         }
      }

      /*have some  attachment*/
      else
      {
         pstEmailMsg->pcBoundary = EME_UTILS_STRDUP(pvMemMgr,"----=001_NextPart_000_003A_0495");     
         
         /* we always use multipart/mixed for encode mail */     
         EME_UTILS_SPRINTF(newStr, "multipart/mixed;\r\n  boundary=\"%s\"", pstEmailMsg->pcBoundary);
         EME_MailMsg_UpdateHeader(pstEMsgHandle,pstEmailMsg, "Content-Type:", newStr);
         
         pcHeaders = EME_MailMsg_GetHeader(pstEMsgHandle,pstEmailMsg);
         EME_UTILS_BREAKNULL(pcHeaders,
            iResult,
            EME_NOMEMORY,
                  "EME_EncodingMailMsgEx---: EME_MailMsg_GetHeader failed!!!");

         EME_MailMsg_ClearHeader(pstEMsgHandle,pstEmailMsg);

         uiHeaderSize = EME_UTILS_STRLEN(pcHeaders);
         
         uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, pcHeaders, uiHeaderSize);
         iResult = (uiHeaderSize == uiHadWriteSize) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
         
         EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
         
         uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, 
            "This is a multipart message in Mime 1.0 format.\r\n\r\n", 
            EME_UTILS_STRLEN("This is a multipart message in Mime 1.0 format.\r\n\r\n"));
         iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("This is a multipart message in Mime 1.0 format.\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
         
         /*either "text/plain" or "text/html" */
         if ( (NULL == pstEmailMsg->pcTextPlain  && NULL != pstEmailMsg->pcTextHtml)
             || (NULL != pstEmailMsg->pcTextPlain && NULL == pstEmailMsg->pcTextHtml))
         {

            if ( NULL != pstEmailMsg->pcTextPlain)
            {
               EME_UTILS_SPRINTF(newStr, 
                                 "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                                 pstEmailMsg->pcBoundary,
                                 pstEmailMsg->pcPlainCharset,
                                 pstEmailMsg->pcPlainTransEnc);

               uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
               iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
               EME_UTILS_BREAKIF(iResult, "write to file failed");

               iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,TRUE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: AppendTextPlain failed!!!!");
            }
            else
            {
               EME_UTILS_SPRINTF(newStr,
                                 "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                                 pstEmailMsg->pcBoundary,
                                 pstEmailMsg->pcHtmlCharset,
                                 pstEmailMsg->pcHtmlTransEnc);
               uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
               iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

               iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,FALSE);
               EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsg,AppendTextHtml failed!!!!");
            }
            
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
         }
         
         /*text/plain and text/html both are exist*/
         else if (NULL != pstEmailMsg->pcTextPlain && NULL != pstEmailMsg->pcTextHtml)
         {
            WE_CHAR *pcSecondBdary = EME_UTILS_STRDUP(pvMemMgr,"----=002_NextPart_000_003B_0495");
            
            EME_UTILS_SPRINTF(newStr, "--%s\r\nContent-Type: multipart/alternative;\r\n boundary=\"%s\"\r\n\r\n", 
               pstEmailMsg->pcBoundary,pcSecondBdary);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            EME_UTILS_MEMSET(newStr, 0, EME_MAXSTRLEN);
            
            /*add text/plain*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/plain;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pcSecondBdary,
                              pstEmailMsg->pcPlainCharset,
                              pstEmailMsg->pcPlainTransEnc);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");

            iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,TRUE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: AppendTextPlain failed!!!!");

            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));
            
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
            
            /*add text/html*/
            EME_UTILS_SPRINTF(newStr, 
                              "--%s\r\nContent-Type: text/html;\r\n charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n", 
                              pcSecondBdary,
                              pstEmailMsg->pcHtmlCharset,
                              pstEmailMsg->pcHtmlTransEnc);

            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");      

            iResult = EME_IsAppendTextPlainEx(pstEMsgHandle,pstEmailMsg,FALSE);
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: AppendTextHtml failed!!!!");

            EME_UTILS_MEMSET(newStr,0x00,EME_MAXSTRLEN * sizeof(WE_CHAR));

            EME_UTILS_SPRINTF(newStr, "\r\n\r\n--%s--", pcSecondBdary);
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
            EME_UTILS_FREEIF(pvMemMgr,pcSecondBdary);
            
            uiHadWriteSize = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
            iResult = (uiHadWriteSize == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_EncodingMailMsgEx---: write to file failed");
         }

         EME_MailMsg_EncodingPartsStart(pstEMsgHandle);
      }
   } while(0);

   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_FileClose(pstEmailMsg->pvMailFile);
      pstEmailMsg->pvMailFile = NULL;

      if(pstEMsgHandle->pIFileMgr != NULL)
      {
         EME_DeleteFile(pstEMsgHandle->pIFileMgr, pstEmailMsg->pcMailFileName);
      }
      EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->pcMailFileName);

      EME_UTILS_FREEIF(pvMemMgr,pcHeaders);
      EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);
   }
   
   EME_UTILS_LOG_INFO(("EME_EncodingMailMsgEx---: End, Result = %d", iResult));
   return iResult;
}


static void EME_MailMsg_EncodingOneBufferCB(St_EncodingPartInfo *pstPartInfo)
{
   WE_INT32 iResult               = EME_SUCCESS; /*must init to SUCCESS*/
   WE_UINT32 uiOnePartSize        = 0;           /* be base64 format */
//   WE_CHAR *pcBuff                = NULL;
   void *pvMemMgr                 = NULL;

   stEMSGHandle *pstMsgHdl        = NULL;
   St_EmailMsg  *pstEmailMsg      = NULL;

   WE_UINT32    uiHadRead         = 0;
   WE_UINT32    uiHadWrite        = 0;

   WE_CHAR   *pcBase64Buffer      = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingOneBufferCB---: Start"));
   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingOneBufferCB---: Performance encoding one buffer %d", EME_UTILS_GETTIMEMS));
   if(NULL == pstPartInfo)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_EncodingOneBufferCB--: Bad parameter and return"));
      return;
   }

   do 
   {
      /*check the data*/
      EME_UTILS_BREAKNULL(pstPartInfo->pstMsgPart, iResult, EME_FAILED, "EME_MailMsg_EncodingOneBufferCB---: message part is null");
      EME_UTILS_BREAKNULL(pstPartInfo->pcReadOnceBuff, iResult, EME_FAILED, "Read once buffer is null");

      pstMsgHdl = (stEMSGHandle*)pstPartInfo->pvEMsgHdl;
      EME_UTILS_BREAKNULL(pstMsgHdl, iResult, EME_FAILED, "EME_MailMsg_EncodingOneBufferCB---: message handle is null");
      
      pstMsgHdl->pfnRegister = NULL;
      pstMsgHdl->pvRegisterData = NULL;

      pvMemMgr = pstMsgHdl->pvMemMgr;

      pstEmailMsg = (St_EmailMsg*)pstMsgHdl->pvEmailMsg;
      EME_UTILS_BREAKNULL(pstEmailMsg, iResult, EME_FAILED, "EME_MailMsg_EncodingOneBufferCB---: email message is null");
      
      if(NULL != pstPartInfo->pcReadOnceBuff)
      {
         EME_UTILS_MEMSET(pstPartInfo->pcReadOnceBuff, 0, pstPartInfo->uiReadOnceSize + 1);
      }
      
      /*open the file*/
      if(pstPartInfo->pIFile == NULL)
      {
         pstPartInfo->pIFile = EME_FileOpen(pstMsgHdl->pIFileMgr, pstPartInfo->pstMsgPart->pcOnePartData, EME_OFM_READ);
         EME_UTILS_BREAKNULL(pstPartInfo->pIFile, iResult, EME_FAILED, "EME_MailMsg_EncodingOneBufferCB---: open file failed");
         pstPartInfo->uiFileSize = EME_FileLength(pstPartInfo->pIFile);
      }

      if(pstPartInfo->uiReadOnceSize == 0)
      {
         pstPartInfo->uiReadOnceSize = (EME_ENCODING_BUFFER_ONCE - (EME_ENCODING_BUFFER_ONCE % 3));
      }
      
      if(pstPartInfo->uiFileSize == 0)
      {
         /*encoding end*/
         EME_FileClose(pstPartInfo->pIFile);
         pstPartInfo->pIFile = NULL;

         pstPartInfo->iResult = iResult;
         EME_MailMsg_EncodingPartsEnd(pstMsgHdl);
         EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingOneBufferCB---: End, Result = %d", iResult));
         return;
      }
      /*read data from object file*/
      uiHadRead = EME_FileRead(pstPartInfo->pIFile, pstPartInfo->pcReadOnceBuff, pstPartInfo->uiReadOnceSize);
      iResult = (uiHadRead == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingOneBufferCB---: read file failed");

      /*calculate the base 64 size*/
      uiOnePartSize = EME_Base64EncodeSize(uiHadRead) + 1;

      /*allocate new memory for base64*/
      pcBase64Buffer = EME_UTILS_MALLOC(pvMemMgr, uiOnePartSize);
      EME_UTILS_BREAKNULL(pcBase64Buffer, iResult, EME_NOMEMORY, "EME_MailMsg_EncodingOneBufferCB---: no memory for base64 buffer");
      EME_UTILS_MEMSET(pcBase64Buffer, 0, uiOnePartSize);

      /*encoding to base64*/
      EME_ToBase64(pcBase64Buffer, pstPartInfo->pcReadOnceBuff, uiHadRead, FALSE);
      
      /*write the data to email temp file*/
      uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, pcBase64Buffer, EME_UTILS_STRLEN(pcBase64Buffer));
      iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(pcBase64Buffer)) ? EME_SUCCESS : EME_NOSTORAGE;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingOneBufferCB---: write to file failed");

      pstPartInfo->uiHadRead += uiHadRead;
      
      /*check encoding have finished*/
      if(pstPartInfo->uiHadRead >= pstPartInfo->uiFileSize)
      {
         /*encoding end*/
         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n\r\n", EME_UTILS_STRLEN("\r\n\r\n"));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("\r\n\r\n")) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingOneBufferCB---: write to file failed");
         EME_FileClose(pstPartInfo->pIFile);
         pstPartInfo->pIFile = NULL;

         pstPartInfo->iResult = iResult;
         EME_MailMsg_EncodingPartsEnd(pstMsgHdl);
      }
      else
      {
         /*encoding the left data*/
         if(pstMsgHdl->pfnEncoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstMsgHdl->pICBMgr, (PFNNOTIFY)EME_MailMsg_EncodingOneBufferCB, pstPartInfo);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingOneBufferCB---: set timer failed");

            pstMsgHdl->pfnRegister = (PFNNOTIFY)EME_MailMsg_EncodingOneBufferCB;
            pstMsgHdl->pvRegisterData = pstPartInfo;
         }
         else
         {
            EME_MailMsg_EncodingOneBufferCB(pstPartInfo);
         }
      }
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcBase64Buffer);

   if(iResult != EME_SUCCESS)
   {
      (void)EME_FileClose(pstPartInfo->pIFile);
      pstPartInfo->pIFile = NULL;

      if(pstMsgHdl != NULL)
      {
         pstPartInfo->iResult = iResult;
         EME_MailMsg_EncodingPartsEnd(pstMsgHdl);
      }
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingOneBufferCB---: End, Result = %d", iResult));
}


static void EME_MailMsg_EncodingPartsStart(stEMSGHandle *pstEMsgHandle)
{
   WE_INT32 iResult = EME_SUCCESS;
   St_EmailMsg *pstEmailMsg = NULL;
   St_EmailMsgPart *pstEmailMsgPart = NULL;
   WE_UINT32       uiHadWrite = 0;
   WE_CHAR         newStr[EME_MAXSTRLEN] = {0};
   
   WE_CHAR         *pcTmp = NULL;
   WE_CHAR         *pcEncodeWord = NULL;
   void            *pvMemMgr = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsStart---: Start"));
   if(pstEMsgHandle == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_EncodingPartsStart--: Bad parameter and return"));
      return;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
   	pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
      EME_UTILS_BREAKNULL(pstEmailMsg, iResult, EME_FAILED, "EME_MailMsg_EncodingPartsStart---: email msg is null");
      if(pstEmailMsg->uiEMsgPartNum == 0)
      {
         pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult = EME_SUCCESS;
         if(pstEMsgHandle->pfnEncoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd, pstEMsgHandle);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: set timer failed");

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
         else
         {
            EME_MailMsg_EncodingPartsEnd(pstEMsgHandle);
         }
         return;
      }

      EME_UTILS_BREAKNULL(pstEmailMsg->ppstEMsgPart, iResult, EME_FAILED, "EME_MailMsg_EncodingPartsStart---: message part is null");

      /*get the new message part*/
      pstEmailMsgPart = pstEmailMsg->ppstEMsgPart[pstEmailMsg->uiCurrIndex];
      EME_UTILS_BREAKNULL(pstEmailMsgPart, iResult, EME_FAILED, "EME_MailMsg_EncodingPartsStart---: msg part is null");
      
      EME_UTILS_SPRINTF(newStr, "--%s\r\nContent-Type: %s", pstEmailMsg->pcBoundary, pstEmailMsgPart->pcContentType);

      uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
      iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
      
      if (NULL != pstEmailMsgPart->pcName)
      {
         WE_UINT32 uiSize = 0;
         pcEncodeWord = EME_HeaderEncode2Base64(pstEMsgHandle, pstEmailMsgPart->pcName,E_CHARSET_UTF8);
         uiSize = EME_UTILS_STRLEN(pcEncodeWord) + EME_UTILS_STRLEN(";\r\n  name=\"\"\r\n") + 1; /* +1 : + '\0' */
         pcTmp = EME_UTILS_MALLOC(pvMemMgr,uiSize);
         EME_UTILS_BREAKNULL(pcTmp,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_EncodingPartsStart---: tmpStr malloc failed!!!");
         
         EME_UTILS_MEMSET(pcTmp,0x00,uiSize);
         EME_UTILS_SPRINTF(pcTmp, ";\r\n  name=\"%s\"\r\n", pcEncodeWord);
         EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);
         
         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, pcTmp, EME_UTILS_STRLEN(pcTmp));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(pcTmp)) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      }
      else
      {
         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n", EME_UTILS_STRLEN("\r\n"));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("\r\n")) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
      }
      
      uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "Content-Transfer-Encoding: base64\r\n", EME_UTILS_STRLEN("Content-Transfer-Encoding: base64\r\n"));
      iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("Content-Transfer-Encoding: base64\r\n")) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
      
      if (NULL != pstEmailMsgPart->pcFileName)
      {
         WE_UINT32 uiSize = 0;
         pcEncodeWord = EME_HeaderEncode2Base64(pstEMsgHandle,pstEmailMsgPart->pcFileName,E_CHARSET_UTF8);
         uiSize   = EME_UTILS_STRLEN(pcEncodeWord) + EME_UTILS_STRLEN("Content-Disposition: attachment;\r\n  filename=\"\"\r\n") + 1; /* +1 : + '\0' */
         pcTmp     = EME_UTILS_MALLOC(pvMemMgr,uiSize);
         EME_UTILS_BREAKNULL(pcTmp,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_EncodingPartsStart---: tmpStr malloc failed!!!");
         
         EME_UTILS_MEMSET(pcTmp,0x00,uiSize);
         EME_UTILS_SPRINTF(pcTmp, "Content-Disposition: attachment;\r\n  filename=\"%s\"\r\n", pcEncodeWord);
         EME_UTILS_FREEIF(pvMemMgr,pcEncodeWord);

         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, pcTmp, EME_UTILS_STRLEN(pcTmp));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(pcTmp)) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
         EME_UTILS_FREEIF(pvMemMgr,pcTmp);
      }
      else
      {
         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "Content-Disposition: inline\r\n", EME_UTILS_STRLEN("Content-Disposition: inline\r\n"));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("Content-Disposition: inline\r\n")) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
      }

      uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n", EME_UTILS_STRLEN("\r\n"));
      iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("\r\n")) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: write to file failed");
      
      EME_UTILS_MEMSET(&pstEmailMsg->unPartInfo.astEncodingPartInfo, 0, sizeof(St_EncodingPartInfo));
      pstEmailMsg->unPartInfo.astEncodingPartInfo.pstMsgPart = pstEmailMsgPart;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.pvEMsgHdl = pstEMsgHandle;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.uiFileSize = 0;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.uiHadRead = 0;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.pIFile = NULL;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult = EME_SUCCESS;
      pstEmailMsg->unPartInfo.astEncodingPartInfo.uiReadOnceSize = EME_ENCODING_BUFFER_ONCE;

      pstEmailMsg->unPartInfo.astEncodingPartInfo.pcReadOnceBuff = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr,
         pstEmailMsg->unPartInfo.astEncodingPartInfo.uiReadOnceSize + 1);
      EME_UTILS_BREAKNULL(pstEmailMsg->unPartInfo.astEncodingPartInfo.pcReadOnceBuff, iResult, EME_NOMEMORY,
         "no memory for read once buffer");
      EME_UTILS_MEMSET(pstEmailMsg->unPartInfo.astEncodingPartInfo.pcReadOnceBuff, 0, 
         pstEmailMsg->unPartInfo.astEncodingPartInfo.uiReadOnceSize + 1);

      if(NULL != pstEMsgHandle->pfnEncoding)
      {
         iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_EncodingOneBufferCB, (void*)&pstEmailMsg->unPartInfo.astEncodingPartInfo);
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsStart---: set timer failed");

         pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_EncodingOneBufferCB;
         pstEMsgHandle->pvRegisterData = &pstEmailMsg->unPartInfo.astEncodingPartInfo;
      }
      else
      {
         EME_MailMsg_EncodingOneBufferCB(&pstEmailMsg->unPartInfo.astEncodingPartInfo);
      }

   } while(0);
   
   if(iResult != EME_SUCCESS)
   {
      pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult = iResult;
      if(NULL != pstEMsgHandle->pfnEncoding)
      {
         OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd, pstEMsgHandle);

         pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_EncodingPartsEnd;
         pstEMsgHandle->pvRegisterData = pstEMsgHandle;
      }
      else
      {
         EME_MailMsg_EncodingPartsEnd(pstEMsgHandle);
      }
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsStart---: End, Result = %d", iResult));
   return;
}

static void EME_MailMsg_EncodingPartsEnd(stEMSGHandle *pstEMsgHandle)
{
   St_EmailMsg *pstEmailMsg = NULL;
   WE_INT32    iResult = EME_SUCCESS;
   WE_UINT32   uiHadWrite = 0;
   void        *pvMemMgr = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsEnd---: Start"));
   if(NULL == pstEMsgHandle)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_EncodingPartsEnd--: Bad parameter and return"));
      return;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   do 
   {
      pstEMsgHandle->pfnRegister = NULL;
      pstEMsgHandle->pvRegisterData = NULL;

      pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
      EME_UTILS_BREAKNULL(pstEmailMsg, iResult, EME_FAILED, "EME_MailMsg_EncodingPartsEnd---: email msg  is null");

      iResult = pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsEnd---: ncoding one part failed");

      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstEmailMsg->unPartInfo.astEncodingPartInfo.pcReadOnceBuff);
      
      if(pstEmailMsg->uiEMsgPartNum == 0)
      {
         EmailOpt *pstOneOpt = NULL;

         uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n", EME_UTILS_STRLEN("\r\n"));
         iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("\r\n")) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsEnd---: write file data failed");
         
         pstEmailMsg->uiEMailDataSize = EME_FileLength(pstEmailMsg->pvMailFile);
         (void)EME_FileClose(pstEmailMsg->pvMailFile);
         pstEmailMsg->pvMailFile = NULL;
         
         pstEMsgHandle->pstEncodedOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);
         EME_UTILS_BREAKNULL(pstEMsgHandle->pstEncodedOpts,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_EncodingPartsEnd---: EME_EncodingMailMsg failed!!!!!!!!!!!");
         EME_UTILS_MEMSET(pstEMsgHandle->pstEncodedOpts, 0, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);
         
         pstOneOpt = pstEMsgHandle->pstEncodedOpts;
         
         pstOneOpt->iId = EMAILMESSAGE_OPT_SRC_TYPE;
         pstOneOpt->pVal = (void*)EMAIL_ENCODE_BYFILE;
         pstOneOpt++;
         
         pstOneOpt->iId  = EMAILMESSAGE_OPT_BUFF;
         pstOneOpt->pVal = (void*)pstEmailMsg->pcMailFileName;
         pstOneOpt++;

         pstOneOpt->iId = EMIALMESSAGE_OPT_BUFFER_SIZE;
         pstOneOpt->pVal = (void*)pstEmailMsg->uiEMailDataSize;
         pstOneOpt++;
         
         pstOneOpt->iId  = EMAILOPT_END;
         
         EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsEnd---: Performance End Encoding %d", EME_UTILS_GETTIMEMS));
         if(NULL != pstEMsgHandle->pfnEncoding)
         {
            (void)OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstEMsgHandle);

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
      }
      else
      {
         pstEmailMsg->uiCurrIndex++;
         if(pstEmailMsg->uiCurrIndex < pstEmailMsg->uiEMsgPartNum)
         {
            EME_MailMsg_EncodingPartsStart(pstEMsgHandle);
         }
         else
         {
            EmailOpt *pstOneOpt = NULL;
            WE_CHAR  newStr[EME_MAXSTRLEN] = {0};
            
            EME_UTILS_SPRINTF(newStr, "--%s--", pstEmailMsg->pcBoundary);
            uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, newStr, EME_UTILS_STRLEN(newStr));
            iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN(newStr)) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsEnd---: write file data failed");
            
            uiHadWrite = EME_FileWrite(pstEmailMsg->pvMailFile, "\r\n", EME_UTILS_STRLEN("\r\n"));
            iResult = (uiHadWrite == (WE_UINT32)EME_UTILS_STRLEN("\r\n")) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_EncodingPartsEnd---: write file data failed");
            
            pstEmailMsg->uiEMailDataSize = EME_FileLength(pstEmailMsg->pvMailFile);
            (void)EME_FileClose(pstEmailMsg->pvMailFile);
            pstEmailMsg->pvMailFile = NULL;
            
            pstEMsgHandle->pstEncodedOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);
            EME_UTILS_BREAKNULL(pstEMsgHandle->pstEncodedOpts,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_EncodingPartsEnd---: EME_EncodingMailMsg failed!!!!!!!!!!!");
            EME_UTILS_MEMSET(pstEMsgHandle->pstEncodedOpts, 0, sizeof(EmailOpt) * EMAIL_MAX_ENCODED_OPT);
            
            pstOneOpt = pstEMsgHandle->pstEncodedOpts;
            
            pstOneOpt->iId = EMAILMESSAGE_OPT_SRC_TYPE;
            pstOneOpt->pVal = (WE_VOID*)EMAIL_ENCODE_BYFILE;
            pstOneOpt++;
            
            pstOneOpt->iId  = EMAILMESSAGE_OPT_BUFF;
            pstOneOpt->pVal = pstEmailMsg->pcMailFileName;
            pstOneOpt++;
            
            pstOneOpt->iId = EMIALMESSAGE_OPT_BUFFER_SIZE;
            pstOneOpt->pVal = (void*)pstEmailMsg->uiEMailDataSize;
            pstOneOpt++;
            
            pstOneOpt->iId  = EMAILOPT_END;
            
            EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsEnd---: Performanec: End Encoding %d", EME_UTILS_GETTIMEMS));
            if(NULL != pstEMsgHandle->pfnEncoding)
            {
               (void)OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstEMsgHandle);

               pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
               pstEMsgHandle->pvRegisterData = pstEMsgHandle;
            }
         }
      }
   } while(0);

   if(iResult != SUCCESS)
   {
      if(NULL != pstEmailMsg)
      {
         (void)EME_FileClose(pstEmailMsg->pvMailFile);
         pstEmailMsg->pvMailFile = NULL;

         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstEmailMsg->pcMailFileName);
         pstEmailMsg->pcMailFileName = NULL;
         
         if(NULL != pstEmailMsg->pcMailFileName && NULL != pstEMsgHandle->pIFileMgr)
         {
            (void)EME_DeleteFile(pstEMsgHandle->pIFileMgr, pstEmailMsg->pcMailFileName);
         }
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstEmailMsg->unPartInfo.astEncodingPartInfo.pcReadOnceBuff);
      }

      if(NULL != pstEMsgHandle->pfnEncoding)
      {
         (void)OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstEMsgHandle);

         pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
         pstEMsgHandle->pvRegisterData = pstEMsgHandle;
      }
   }
   
   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingPartsEnd---: End, Result = %d", iResult));
   return;
}

/****************************** Decoding email message ******************************/
WE_INT32 EME_MailMsg_ParseJustPartEx(stEMSGHandle *pstEsgHandle)
{
   WE_INT32 iResult = EME_SUCCESS;
   St_EmailMsg  *pstEmailMsg = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseJustPartEx---: Start"));
   if(NULL == pstEsgHandle)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseJustPartEx--: Bad parameter and return"));
      return EME_INVALID_DATA;
   }

   pstEmailMsg = (St_EmailMsg*)pstEsgHandle->pvEmailMsg;
   if(NULL == pstEmailMsg)
   {
      return EME_FAILED;
   }

   do 
   {
   	pstEmailMsg->pvMailFile = EME_FileOpen(pstEsgHandle->pIFileMgr, pstEmailMsg->pcMailFileName, EME_OFM_READ);
      EME_UTILS_BREAKNULL(pstEmailMsg->pvMailFile, iResult, EME_FAILED, "EME_MailMsg_ParseJustPartEx---: open file failed");

      pstEmailMsg->unPartInfo.astDecodingPartInfo.uiBodySize = EME_FileLength(pstEmailMsg->pvMailFile);
//       iResult = (pstEmailMsg->unPartInfo.astDecodingPartInfo.uiBodySize > 0) ? EME_SUCCESS : EME_FAILED;
//       EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseJustPartEx---: get file size failed");

      iResult = EME_MailMsg_ParsePartBody(pstEsgHandle, pstEmailMsg);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseJustPartEx---: parse part body failed");
   } while(0);

   if(EME_SUCCESS != iResult)
   {
      EME_FileClose(pstEmailMsg->pvMailFile);
      pstEmailMsg->pvMailFile = NULL;
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseJustPartEx---: End, Result = %d", iResult));
   return iResult;
}

WE_INT32 EME_MailMsg_ParseDataBufEx(stEMSGHandle *pstEMsgHandle,
                                     void *pstIn, 
                                     WE_CHAR *src, 
                                     WE_CHAR *pcDir,
                                     WE_BOOL bOnlyHeader)
{   
   WE_INT32     iResult          = EME_SUCCESS ; /*must be init success*/
   void         *pvMemMgr        = NULL;
   St_EmailMsg  *pstEmailMsg      = (St_EmailMsg*)pstIn;

   WE_INT32     iDataSize = 0;
   void         *pIFileTmp = NULL;

   WE_CHAR      *pcDataBuff = NULL;
   WE_UINT32    uiFileSize = 0;

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseDataBufEx---: Start"));
   if (NULL == pstEMsgHandle || pstEmailMsg == NULL || NULL == src || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseDataBufEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      /*strdup the file name*/
      EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->pcMailFileName);
      pstEmailMsg->pcMailFileName = EME_UTILS_STRDUP(pvMemMgr, src);
      EME_UTILS_BREAKNULL(pstEmailMsg->pcMailFileName, iResult, EME_NOMEMORY, "EME_MailMsg_ParseDataBufEx---: strdup file name failed");

      /*open the email temp file*/
      pstEmailMsg->pvMailFile = EME_FileOpen(pstEMsgHandle->pIFileMgr, pstEmailMsg->pcMailFileName, EME_OFM_READ);
      EME_UTILS_BREAKNULL(pstEmailMsg->pvMailFile, iResult, EME_FAILED, "EME_MailMsg_ParseDataBufEx---: open file failed");
      
      uiFileSize = EME_FileLength(pstEmailMsg->pvMailFile);
      iResult = (uiFileSize == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "get file size failed");

      /*get head*/
      pIFileTmp = EMEFile_GetHeadData(pstEmailMsg->pvMailFile, pstEMsgHandle->pvMemMgr, uiFileSize, &iDataSize);
      EME_UTILS_BREAKNULL(pIFileTmp, iResult, EME_FAILED, "EME_MailMsg_ParseDataBufEx---: get head data from file failed");
      
      pcDataBuff = EME_UTILS_MALLOC(pvMemMgr, iDataSize + 1);
      EME_UTILS_BREAKNULL(pcDataBuff, iResult, EME_NOMEMORY, "EME_MailMsg_ParseDataBufEx---: no memory for data buffer");
      EME_UTILS_MEMSET(pcDataBuff, 0, iDataSize + 1);
      
      iResult = EMEFile_ReadDataFromFile(pIFileTmp, pcDataBuff, (WE_UINT32)iDataSize);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: read data from file failed");

      //iResult = EME_FileSeek(pstEmailMsg->pvMailFile, iDataSize, EME_SEEK_CURRENT);
      //EME_UTILS_BREAKIF(iResult, "seek file failed");

      if(iDataSize < (WE_INT32)uiFileSize)
      {
         if(bOnlyHeader)
         {
            iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, pcDataBuff, (pcDataBuff + iDataSize - 1));
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: ,EME_MailMsg_ParseHeaders failed!!!!");
            EME_UTILS_FREEIF(pvMemMgr, pcDataBuff);

            pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
            pstEmailMsg->unPartInfo.astDecodingPartInfo.bIsAttachment = FALSE;
            if(pstEMsgHandle->pfnDecoding != NULL)
            {
               iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: set timer failed");

               pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
               pstEMsgHandle->pvRegisterData = pstEMsgHandle;
            }
            else
            {
               EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
            }
         }
         else
         {
            iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, pcDataBuff, (pcDataBuff + iDataSize - 1));
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: ,EME_MailMsg_ParseHeaders,failed!!!!");
            EME_UTILS_FREEIF(pvMemMgr, pcDataBuff);
            
            iResult = EME_MailMsg_ParseMsgBodyEx(pstEMsgHandle,pstEmailMsg, pstEmailMsg->pvMailFile, pcDir);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: , EME_MailMsg_ParseMsgBody failed!!!!");
         }
      }
      else
      {
         iResult = EME_MailMsg_ParseHeaders(pstEMsgHandle,pstEmailMsg, pcDataBuff, (pcDataBuff + iDataSize - 1));
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: ,EME_MailMsg_ParseHeaders failed!!!!");
         EME_UTILS_FREEIF(pvMemMgr, pcDataBuff);
         
         pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
         pstEmailMsg->unPartInfo.astDecodingPartInfo.bIsAttachment = FALSE;
         if(pstEMsgHandle->pfnDecoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseDataBufEx---: set timer failed");

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
         else
         {
            EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
         }
      }

   } while(0);

   if(EME_SUCCESS != iResult)
   {
      EME_FileClose(pstEmailMsg->pvMailFile);
      pstEmailMsg->pvMailFile = NULL;
      
      EME_UTILS_FREEIF(pvMemMgr, pcDataBuff);
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseDataBufEx---: End, Result = %d", iResult));
   return iResult;
}

static WE_INT32 EME_MailMsg_ParseMsgBodyEx(stEMSGHandle *pstEMsgHandle,
                                            St_EmailMsg *pstIn,
                                            void *pvIFile,
                                            WE_CHAR *pcDir)
{ 
   WE_INT32 iResult      = EME_SUCCESS ; /*must be init success*/
   void    *pvMemMgr     = NULL;

   WE_CHAR *pcBodyBuf    = NULL;
   WE_CHAR *pcDecodedWord = NULL;

   WE_UINT32  uiFileSize = 0;
   WE_UINT32  uiHadRead = 0;
  
   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseMsgBodyEx---: Start"));
   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pvIFile || NULL == pcDir)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParseMsgBodyEx--: Bad parameter and return"));
      return EME_FAILED;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if (pstIn->pcBoundary != NULL )
      {
         WE_INT32 iPartCount = 0;
         PARTINDEXTB pvIndexTable = NULL;
         
         pstIn->unPartInfo.astDecodingPartInfo.pcDir = EME_UTILS_STRDUP(pvMemMgr, pcDir);
         EME_UTILS_BREAKNULL(pstIn->unPartInfo.astDecodingPartInfo.pcDir, iResult, EME_NOMEMORY, "EME_MailMsg_ParseMsgBodyEx---: no memory");
         
         pvIndexTable = EMEFile_GetPartIndexTable(pstIn->pvMailFile, pvMemMgr, pstIn->pcBoundary, 0, &iPartCount);
         if(pvIndexTable != NULL && iPartCount > 0)
         {
            WE_INT32 i = 0;
            WE_INT32 iOffset = 0;
            WE_INT32 iPartSize = 0;
            
            St_PartIndexInfo *pstPartIndexInfo = NULL;
            St_PartIndexInfo *pstIndexLast = NULL;
            
            /*create part list*/
            while(i < iPartCount)
            {
               iOffset = 0;
               iPartSize = 0;
               iResult = EMEFile_GetOnePartInfo(pvIndexTable, i, pstIn->pcBoundary, &iOffset, &iPartSize);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: get one part info failed");
               
               pstPartIndexInfo = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_PartIndexInfo));
               EME_UTILS_BREAKNULL(pstPartIndexInfo, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_MEMSET(pstPartIndexInfo, 0, sizeof(St_PartIndexInfo));
               
               pstPartIndexInfo->uiOffset = (WE_INT32)iOffset;
               pstPartIndexInfo->uiPartSize = (WE_INT32)iPartSize;
               
               if(NULL == pstIn->pstPartIndexList)
               {
                  pstIn->pstPartIndexList = pstPartIndexInfo;
                  pstIndexLast = pstPartIndexInfo;
               }
               else
               {
                  EME_UTILS_BREAKNULL(pstIndexLast, iResult, EME_FAILED, "pstIndexLast is null");
                  pstIndexLast->next = pstPartIndexInfo;
                  pstIndexLast = pstPartIndexInfo;
               }
               i++;
            }
            
            EMFile_FreePartIndexTable(pvIndexTable, pvMemMgr);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: create part index list failed");

            /*Decoding part start*/
            EME_MailMsg_DecodingPartsStart(pstEMsgHandle);
         }
         else
         {
            EMFile_FreePartIndexTable(pvIndexTable, pvMemMgr);
            iResult = EME_SUCCESS;
//            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: get part index table failed");
         }
      }
      else if(!pstIn->bIsAttachment) /* just save the text body. */
      {
         uiFileSize = EME_FileLength(pvIFile);
         iResult = (uiFileSize > 0) ? EME_SUCCESS : EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: get file size failed");

         pcBodyBuf = EME_UTILS_MALLOC(pvMemMgr, uiFileSize + 1);
         EME_UTILS_BREAKNULL(pcBodyBuf, iResult, EME_NOMEMORY, "EME_MailMsg_ParseMsgBodyEx---: no memory");
         EME_UTILS_MEMSET(pcBodyBuf, 0, uiFileSize + 1);

         uiHadRead = EME_FileRead(pvIFile, pcBodyBuf, uiFileSize);
         iResult = (uiHadRead == 0) ? EME_FAILED : EME_SUCCESS;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: read data from file failed");
         
         pcDecodedWord = EME_MailMsg_DecodeBody(pstEMsgHandle,pcBodyBuf, uiHadRead, pstIn->eTransEncoding, NULL);
         if (!pstIn->bIsHtml)
         {
            pstIn->pcPlainTransEnc = EME_GetTransEncoding(pvMemMgr, pstIn->eTransEncoding);
            pstIn->pcTextPlain = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcPlainCharset);
         }
         else
         {
            pstIn->pcHtmlTransEnc = EME_GetTransEncoding(pvMemMgr, pstIn->eTransEncoding);
            pstIn->pcTextHtml = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcHtmlCharset);

         }
         EME_UTILS_FREEIF(pvMemMgr,pcDecodedWord);

         pstIn->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;

         /*parse the body end*/
         if(pstEMsgHandle->pfnDecoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParseMsgBodyEx---: set timer failed");

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
         else
         {
            EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
         }
      }
      else /*only contain attachment part*/
      {
         iResult = EME_FileSeek(pstIn->pvMailFile, 0, EME_SEEK_START);
         EME_UTILS_BREAKIF(iResult, "seek to start failed");

         pstIn->unPartInfo.astDecodingPartInfo.pcDir = EME_UTILS_STRDUP(pvMemMgr, pcDir);
         EME_UTILS_BREAKNULL(pstIn->unPartInfo.astDecodingPartInfo.pcDir, iResult, EME_NOMEMORY, "EME_MailMsg_ParseMsgBodyEx---: no memory");

         pstIn->pstPartIndexList = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_PartIndexInfo));
         EME_UTILS_BREAKNULL(pstIn->pstPartIndexList, iResult, EME_NOMEMORY, "no memory");
         EME_UTILS_MEMSET(pstIn->pstPartIndexList, 0, sizeof(St_PartIndexInfo));
         pstIn->pstPartIndexList->uiOffset = EME_FileTell(pstIn->pvMailFile);
         pstIn->pstPartIndexList->uiPartSize = EME_FileLength(pstIn->pvMailFile);

         /*Decoding part start*/
         EME_MailMsg_DecodingPartsStart(pstEMsgHandle);
      }
      
   } while(0);

   EME_UTILS_FREEIF(pvMemMgr, pcBodyBuf);
   EME_UTILS_FREEIF(pvMemMgr, pcDecodedWord);
   
   if(iResult != SUCCESS)
   {
      St_PartIndexInfo *pstPartIndexTmp = NULL;
      
      while(pstIn->pstPartIndexList)
      {
         pstPartIndexTmp = pstIn->pstPartIndexList;
         pstIn->pstPartIndexList = pstIn->pstPartIndexList->next;
         EME_UTILS_FREEIF(pvMemMgr, pstPartIndexTmp);
      }
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParseMsgBodyEx---: End, Result = %d", iResult));
   return iResult ;
}

static void EME_MailMsg_DecodingPartsStart(stEMSGHandle *pstEMsgHandle)
{
   WE_INT32 iResult = EME_SUCCESS;
   St_EmailMsg  *pstEmailMsg = NULL;

   void         *pIFileTmp = NULL;
   WE_INT32     iHeadSize = 0;

   WE_CHAR      *pcHeadData = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingPartsStart---: Start"));
   if(pstEMsgHandle == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodingPartsStart--: Bad parameter and return"));
      return;
   }

   pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
   if(pstEmailMsg == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodingPartsStart--: email msg handle is null and return"));
      return;
   }

   do 
   {
      /*free the old message part info*/
      pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.iTransEnc = 0;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.uiHadRead = 0;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.uiBodySize = 0;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.uiReadOnceSize = 0;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.bIsAttachment = FALSE;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.pvEMsgHdl = pstEMsgHandle;

   	EME_UTILS_BREAKNULL(pstEmailMsg->pstPartIndexList, iResult, EME_FAILED, "EME_MailMsg_DecodingPartsStart---: part index tabale is null");
      EME_UTILS_BREAKNULL(pstEmailMsg->pvMailFile, iResult, EME_FAILED, "EME_MailMsg_DecodingPartsStart---: file pointer is null");

      iResult = EME_FileSeek(pstEmailMsg->pvMailFile, pstEmailMsg->pstPartIndexList->uiOffset, EME_SEEK_START);
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingPartsStart---: seek file failed");

      pIFileTmp = EMEFile_GetHeadData(pstEmailMsg->pvMailFile, 
         pstEMsgHandle->pvMemMgr, 
         pstEmailMsg->pstPartIndexList->uiPartSize,
         &iHeadSize);
      if(pIFileTmp != NULL && iHeadSize > 0)
      {
         pcHeadData = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, iHeadSize + 1);
         EME_UTILS_BREAKNULL(pcHeadData, iResult, EME_NOMEMORY, "EME_MailMsg_DecodingPartsStart---: no memory");
         EME_UTILS_MEMSET(pcHeadData, 0, iHeadSize + 1);

         iResult = EMEFile_ReadDataFromFile(pIFileTmp, pcHeadData, (WE_UINT32)iHeadSize);
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingPartsStart---: read head data from file failed");

         /*seek the file point at the body first*/
         //iResult = EME_FileSeek(pstEmailMsg->pvMailFile, iHeadSize, EME_SEEK_CURRENT);
         //EME_UTILS_BREAKIF(iResult, "seek file failed");
         
         /*parse part head firest*/
         //EME_UTILS_MEMSET(&pstEmailMsg->astDecodingPartInfo, 0, sizeof(St_DecodingPartInfo));
         iResult = EME_MailMsg_ParsePartHead(pstEMsgHandle, pstEmailMsg, pcHeadData, &pstEmailMsg->unPartInfo.astDecodingPartInfo);
         EME_UTILS_BREAKIF(iResult, "parse part head failed");
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcHeadData);

         pstEmailMsg->unPartInfo.astDecodingPartInfo.uiBodySize = pstEmailMsg->pstPartIndexList->uiPartSize - (WE_UINT32)iHeadSize;
//          if(pstEmailMsg->unPartInfo.astDecodingPartInfo.uiBodySize > 0)
//          {
         /*parse part data*/
         iResult = EME_MailMsg_ParsePartBody(pstEMsgHandle, pstEmailMsg);
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingPartsStart---: Parse part body failed");
//          }
//          else
//          {
//             pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
//             pstEmailMsg->unPartInfo.astDecodingPartInfo.bIsAttachment = TRUE;
//             if(pstEMsgHandle->pfnDecoding != NULL)
//             {
//                EME_UTILS_SETTIMER(pstEMsgHandle->pICBMgr, 0, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
//                pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
//                pstEMsgHandle->pvRegisterData = pstEMsgHandle;
//             }
//             else
//             {
//                EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
//             }
//          }
      }
      else
      {
         iResult = EME_FAILED;
         EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingPartsStart---: get head data failed");
      }
      
   } while(0);

   EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcHeadData);

   if(iResult != EME_SUCCESS)
   {
      pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult = iResult;
      if(pstEMsgHandle->pfnDecoding != NULL)
      {
         OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);

         pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
         pstEMsgHandle->pvRegisterData = pstEMsgHandle;
      }
      else
      {
         EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
      }
   }
   
   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingPartsStart---: End, Result = %d", iResult));
   return;
}

static void EME_MailMsg_DecodingPartsEnd(stEMSGHandle *pstEMsgHandle)
{
   WE_INT32 iResult = EME_SUCCESS;
   St_EmailMsg *pstEmailMsg = NULL;
   St_EmailMsgPart *pstEMsgPart = NULL;
   void            *pvMemMgr = NULL;

   St_MsgPartList *pMsgPart = NULL;
   St_MsgPartList *pstLast = NULL;
   WE_CHAR        *pcTextPlain = NULL;
   WE_CHAR        *pcTextHtml = NULL;
   WE_INT32       iMsgPartNum = 0;
   EmailOpt       *pstOneOpt = NULL;
   WE_INT32       i = 0;

   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingPartsEnd---: Start"));
   if(NULL == pstEMsgHandle)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodingPartsEnd--: Bad parameter and return"));
      return;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
      EME_UTILS_BREAKNULL(pstEmailMsg, iResult, EME_FAILED, "EME_MailMsg_DecodingPartsEnd---: email msg is null");

      /*delete one part from index part list*/
      if(NULL != pstEmailMsg->pstPartIndexList)
      {
         St_PartIndexInfo *pstTmp = NULL;
         pstTmp = pstEmailMsg->pstPartIndexList;
         pstEmailMsg->pstPartIndexList = pstEmailMsg->pstPartIndexList->next;
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstTmp);
      }
      
      iResult = pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingPartsEnd---: parse one part failed");

      if(pstEmailMsg->unPartInfo.astDecodingPartInfo.bIsAttachment)
      {
         pstEMsgPart = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr,sizeof(St_EmailMsgPart));
         EME_UTILS_BREAKNULL(pstEMsgPart,iResult,EME_NOMEMORY,"EME_MailMsg_DecodingPartsEnd---: malloc failed!!!");
         EME_UTILS_MEMSET(pstEMsgPart,0x00, sizeof(St_EmailMsgPart));
         
         /*add one part to email message*/
         pstEMsgPart->pcContentType       = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType;
         pstEMsgPart->pcName              = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcName;
         pstEMsgPart->pcFileName          = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName;
         pstEMsgPart->pcTransEnc          = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc;
         pstEMsgPart->pcOnePartData       = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData;
         pstEMsgPart->pcContentId         = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId;
         pstEMsgPart->uiOnePartSize       = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.uiOnePartSize;
         pstEMsgPart->pcCharSet           = pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet;
         EME_MailMsg_AddMsgPart(pstEMsgHandle,pstEmailMsg, pstEMsgPart);
      }
      else
      {
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcName);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcNextBoundary);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet);
      }
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcName        = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName    = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc    = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId   = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet     = NULL;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.uiOnePartSize = 0;
      pstEmailMsg->unPartInfo.astDecodingPartInfo.pcNextBoundary           = NULL;

      EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcReadOnceBuff);
      
      /*to check there had another part want to be parsed*/
      if(NULL != pstEmailMsg->pstPartIndexList)
      {
         EME_MailMsg_DecodingPartsStart(pstEMsgHandle);
      }
      else
      {
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcDir);
         EME_FileClose(pstEmailMsg->pvMailFile);
         pstEmailMsg->pvMailFile = NULL;
         /*all part finished*/
         EME_MailMsg_GetPlainText(pstEMsgHandle,pstEmailMsg, &pcTextPlain);
         if (NULL != pcTextPlain)
         {
            pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
            EME_UTILS_BREAKNULL(pMsgPart,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: failed!!");
            EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));
            
            pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
            EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: failed!!");
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
            
            pstEMsgHandle->uiPartsNum++;
            
            /* add msg part */
            if (pstEMsgHandle->pstPartList == NULL)
            {
               pstEMsgHandle->pstPartList = pMsgPart;
            }
            else
            {
               pstLast = pstEMsgHandle->pstPartList;
               while (pstLast->pstNext != NULL)
               {
                  pstLast = pstLast->pstNext;
               }
               
               pstLast->pstNext = pMsgPart;
            }
         }
         
         EME_MailMsg_GetHtmlText(pstEMsgHandle,pstEmailMsg, &pcTextHtml);
         if (NULL != pcTextHtml)
         {
            pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
            EME_UTILS_BREAKNULL(pMsgPart,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: failed!!");
            EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));
            
            pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
            EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: failed!!");
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
            
            pstEMsgHandle->uiPartsNum++;
            
            /* add msg part */
            if (pstEMsgHandle->pstPartList == NULL)
            {
               pstEMsgHandle->pstPartList = pMsgPart;
            }
            else
            {
               pstLast = pstEMsgHandle->pstPartList;
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
            
            pstEmailMsgPart = EME_MailMsg_GetOnePartByIndex(pstEMsgHandle->pvEmailMsg, i);
            EME_UTILS_BREAKNULL(pstEmailMsgPart,
               iResult,
               EME_FAILED,
               "EME_MailMsg_DecodingPartsEnd---: EME_MailMsg_GetOnePartByIndex failed!!!");
            
            pMsgPart = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_MsgPartList));
            EME_UTILS_BREAKNULL(pMsgPart,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: part list failed!!");
            EME_UTILS_MEMSET(pMsgPart, 0, sizeof(St_MsgPartList));
            
            pMsgPart->pstPartOpts = EME_UTILS_MALLOC(pvMemMgr, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
            EME_UTILS_BREAKNULL(pMsgPart->pstPartOpts,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_DecodingPartsEnd---: part opt failed!!");
            EME_UTILS_MEMSET(pMsgPart->pstPartOpts, 0, sizeof(EmailOpt) * EME_MAX_PART_OPTS);
            
            pstOneOpt = pMsgPart->pstPartOpts;
            
            /* part type */
            pstOneOpt->iId = EMAILPART_OPT_PART_TYPE;
            pstOneOpt->pVal = (WE_VOID*)EMAILPART_PARTTYPE_ATTACHMENT;
            pstOneOpt++;
            
            /* data buf */
            if(NULL != pstEmailMsgPart->pcOnePartData)
            {
               pstOneOpt->iId = EMAILPART_OPT_DATA_BUFF;
               pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr, pstEmailMsgPart->pcOnePartData);
               pstOneOpt++;
            }
            
            /* data size */
            pstOneOpt->iId = EMAILPART_OPT_DATA_SIZE;
            pstOneOpt->pVal = (WE_VOID*)pstEmailMsgPart->uiOnePartSize;
            pstOneOpt++;
            
            /* content type */
            if (pstEmailMsgPart->pcContentType != NULL)
            {
               pstOneOpt->iId = EMAILPART_OPT_MIME_TYPE;
               pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcContentType);
               pstOneOpt++;
            }
            
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
            else if(pstEmailMsgPart->pcFileName != NULL)
            {
               pstOneOpt->iId = EMAILPARAM_OPT_FILE_NAME;
               pstOneOpt->pVal = (WE_VOID*)EME_UTILS_STRDUP(pvMemMgr,pstEmailMsgPart->pcFileName);
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
            pstOneOpt->pVal = (WE_VOID*)EMAILPART_SRCTYPE_FILE;
            pstOneOpt++;
            
            pstOneOpt->iId = EMAILOPT_END;
         
            pstEMsgHandle->uiPartsNum++;
         
            /* add msg part */
            if (pstEMsgHandle->pstPartList == NULL)
            {
               pstEMsgHandle->pstPartList = pMsgPart;
            }
            else
            {
               pstLast = pstEMsgHandle->pstPartList;
               while (pstLast->pstNext != NULL)
               {
                  pstLast = pstLast->pstNext;
               }
            
               pstLast->pstNext = pMsgPart;
            }
         }
      
         EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingPartsEnd---: Performance: end decoding at time %d", EME_UTILS_GETTIMEMS));

         if(pstEMsgHandle->pfnDecoding != NULL)
         {
            (void)OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstEMsgHandle);

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
            pstEMsgHandle->pvRegisterData = pstEMsgHandle;
         }
      }

   } while(0);

   if(iResult != EME_SUCCESS)
   {
      if(NULL != pstEmailMsg)
      {
         /*delete one part from index part list*/
         while(NULL != pstEmailMsg->pstPartIndexList)
         {
            St_PartIndexInfo *pstTmp = NULL;
            pstTmp = pstEmailMsg->pstPartIndexList;
            pstEmailMsg->pstPartIndexList = pstEmailMsg->pstPartIndexList->next;
            EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstTmp);
         }

         EME_FileClose(pstEmailMsg->pvMailFile);
         pstEmailMsg->pvMailFile = NULL;

         EME_FileClose(pstEmailMsg->unPartInfo.astDecodingPartInfo.pIFile);
         pstEmailMsg->unPartInfo.astDecodingPartInfo.pIFile = NULL;
         
         EME_DeleteFile(pstEMsgHandle->pIFileMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcReadOnceBuff);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentId);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcName);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet);

         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcDir);
         EME_UTILS_FREEIF(pvMemMgr, pstEmailMsg->unPartInfo.astDecodingPartInfo.pcNextBoundary);
      }
      if (pMsgPart != NULL)
      {
         EME_FreeAllOptsValue(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart);
      }

      if(NULL != pstEMsgHandle->pfnDecoding)
      {
         (void)OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstEMsgHandle);

         pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
         pstEMsgHandle->pvRegisterData = pstEMsgHandle;
      }
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingPartsEnd---: End, Result = %d", iResult));
   return;
}


static WE_INT32 EME_MailMsg_ParsePartHead(stEMSGHandle        *pstEMsgHandle,
                                          St_EmailMsg         *pstIn,
                                          WE_CHAR             *pcBodyHead,
                                          St_DecodingPartInfo *pstDecPartInfo)
{
   WE_INT32 iResult           = EME_SUCCESS; /*must be init success*/
   WE_INT32 iStartPos         = -1;
   WE_INT32 iEndPos           = -1;  
   WE_CHAR* pcCharSet         = NULL;
   WE_CHAR* pcName            = NULL;
   WE_CHAR* pcFileName        = NULL;
   WE_CHAR* pcContType        = NULL;
   WE_CHAR* pcMimeHeader      = NULL;
   WE_CHAR* pcContId          = NULL;
   void   *pvMemMgr           = NULL;
   WE_CHAR *pcTranEncoding    = NULL;
   E_EncodeType eEncoding     = E_ENCODETYPE_UNKNOW;
   E_Charset    eCharset      = E_CHARSET_UNKNOW;
   
   EME_UTILS_LOG_INFO(("EME_MailMsg_ParsePartHead---: Start"));
   if (NULL == pstEMsgHandle || NULL == pstIn || NULL == pcBodyHead || pstDecPartInfo == NULL)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParsePartHead--: Bad parameter and return"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEMsgHandle->pvMemMgr;
   pcMimeHeader = pcBodyHead;
   
   do 
   {
      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "name=") - pcMimeHeader) > 0)
      {
         WE_CHAR* pcTemp = NULL;
         WE_CHAR* pcEnd = NULL;
         WE_INT32 iIndex = 0;

         iStartPos += 5; /* +"name=" */ 
         pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n");
         while(*(pcEnd + 1) == 0x09 || *(pcEnd + 1) == 0x20)
         {
            iEndPos = pcEnd - pcMimeHeader;
            pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iEndPos + 1, "\n");
         }
         iEndPos = pcEnd - pcMimeHeader;

         pcTemp = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcTemp,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         
         EME_UTILS_MEMSET(pcTemp,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTemp, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         while(iIndex < iEndPos - iStartPos + 1)
         {
            if(';' == pcTemp[iIndex])
            {
               pcTemp[iIndex] = 0x00;
            }
            iIndex++;
         }
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcTemp, '\"', '\"');
         pcName = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcTemp);
         
         EME_UTILS_FREEIF(pvMemMgr,pcTemp);  
         EME_UTILS_BREAKNULL(pcName,
            iResult,
            EME_FAILED,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
      }
      
      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "filename=") - pcMimeHeader) > 0)
      {
         WE_CHAR* pcTemp = NULL;
         WE_CHAR* pcEnd = NULL;
         WE_INT32 iIndex = 0;

         iStartPos += 9; /* +"filename=" */ 
         pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n");
         while(*(pcEnd + 1) == 0x09 || *(pcEnd + 1) == 0x20)
         {
            iEndPos = pcEnd - pcMimeHeader;
            pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iEndPos + 1, "\n");
         }
         iEndPos = pcEnd - pcMimeHeader;

         pcTemp = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcTemp,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         EME_UTILS_MEMSET(pcTemp,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTemp, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         while(iIndex < iEndPos - iStartPos + 1)
         {
            if(';' == pcTemp[iIndex])
            {
               pcTemp[iIndex] = 0x00;
            }
            iIndex++;
         }
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcTemp, '\"', '\"');
         pcFileName = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcTemp);
         
         EME_UTILS_FREEIF(pvMemMgr,pcTemp);
         EME_UTILS_BREAKNULL(pcFileName,
            iResult,
            EME_FAILED,
            "EME_MailMsg_ParseMimePart EME_MailMsg_LocalExpansion, malloc failed!!!");
      }
      
      /* following name and filename is used for Thunder bird encode */ 
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "name*=") - pcMimeHeader) > 0 )
      {
         WE_CHAR* pcWord = NULL;
         WE_CHAR* pcEnd = NULL;

         iStartPos += 6;
         pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n");
         while(*(pcEnd + 1) == 0x09 || *(pcEnd + 1) == 0x20)
         {
            iEndPos = pcEnd - pcMimeHeader;
            pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iEndPos + 1, "\n");
         }
         iEndPos = pcEnd - pcMimeHeader;

         pcWord = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcWord,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         
         EME_UTILS_MEMSET(pcWord,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcWord, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         if (pcName == NULL)
         {
            pcName = EME_MailMsg_ParseSpecName(pstEMsgHandle,pcWord);
         }           
         EME_UTILS_FREEIF(pvMemMgr,pcWord);
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "filename*=") - pcMimeHeader) > 0 )
      {
         WE_CHAR* pcWord = NULL;
         WE_CHAR* pcEnd = NULL;

         iStartPos += 10;
         pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n");
         while(*(pcEnd + 1) == 0x09 || *(pcEnd + 1) == 0x20)
         {
            iEndPos = pcEnd - pcMimeHeader;
            pcEnd = EME_UTILS_STRSTR(pcMimeHeader + iEndPos + 1, "\n");
         }
         iEndPos = pcEnd - pcMimeHeader;

         pcWord = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcWord,
            iResult,
            EME_NOMEMORY,
            " malloc failed!!!");
         EME_UTILS_MEMSET(pcWord,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcWord, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         if (pcFileName == NULL)
         {
            pcFileName = EME_MailMsg_ParseSpecName(pstEMsgHandle,pcWord);
         }           
         EME_UTILS_FREEIF(pvMemMgr,pcWord);
      }
      /* uiEnd of name*= and filename*= */ 
      
      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-type:") - pcMimeHeader) > 0 )
      {
         WE_INT32 iEndPos     = -1;
         WE_INT32 iEndPosTmp  = -1;
         
         iStartPos += 13; /* +"content-type:" */ 
         
         iEndPos = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, ';') - pcMimeHeader;
         iEndPosTmp = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, '\n') - pcMimeHeader;
         if (iEndPos < 0 && iEndPosTmp < 0)
         {
            iResult = EME_FAILED;
            break;
         }
         
         else if (iEndPosTmp > 0)
         {
            if (iEndPos < 0 || iEndPosTmp < iEndPos)
            {
               iEndPos = iEndPosTmp;
            }
         }
         while (pcMimeHeader[iStartPos] == ' ') iStartPos++;
         
         pcContType = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcContType,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         EME_UTILS_MEMSET(pcContType,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcContType, pcMimeHeader + iStartPos, iEndPos - iStartPos);

         iStartPos = EME_UTILS_STRLEN(pcContType);
         while(iStartPos > 0 && (pcContType[iStartPos - 1] == 0x0d ||pcContType[iStartPos - 1] == 0x0a))
         {
            pcContType[iStartPos - 1] = 0x00;
            iStartPos--;
         }
         
      }

      if ((iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "charset=") - pcMimeHeader) > 0)
      {
         WE_CHAR* pcTemp = NULL;
         iStartPos += 8; /* +"filename=" */ 
         iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
         
         pcTemp = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
         EME_UTILS_BREAKNULL(pcTemp,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         EME_UTILS_MEMSET(pcTemp,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTemp, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         EME_MailMsg_UnquoteString(pstEMsgHandle,pcTemp, '\"', '\"');
         pcCharSet = EME_MailMsg_LocalExpansion(pstEMsgHandle,pcTemp);
         eCharset = EME_MailMsg_ParseChaset(NULL, pcCharSet);
         EME_UTILS_FREEIF(pvMemMgr, pcCharSet);
         EME_UTILS_FREEIF(pvMemMgr,pcTemp);
         
         pcCharSet = EME_GetCharSet(pvMemMgr, eCharset);
         EME_UTILS_BREAKNULL(pcCharSet,
            iResult,
            EME_FAILED,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "Content-Transfer-Encoding:") - pcMimeHeader) > 0 )
      {
         iStartPos += 9;
         iEndPos = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, '\n') - pcMimeHeader;
         pcTranEncoding = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1);
         EME_UTILS_BREAKNULL(pcTranEncoding,
            iResult,
            EME_NOMEMORY,
            "EME_MailMsg_ParsePartHead---: malloc failed!!!");
         EME_UTILS_MEMSET(pcTranEncoding,0x00,iEndPos - iStartPos + 1);
         EME_UTILS_MEMMOVE(pcTranEncoding, pcMimeHeader + iStartPos, iEndPos - iStartPos );
         
         eEncoding = EME_MailMsg_ParseTransEncoding(pcTranEncoding);
         EME_UTILS_FREEIF(pvMemMgr, pcTranEncoding);
         pcTranEncoding = EME_GetTransEncoding(pvMemMgr, eEncoding);
         
      }
      
      if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-id:") - pcMimeHeader) > 0 )
      {
         WE_INT32 iEndPosTmp = 0;

         iStartPos += 11; /* +"content-id:" */ 
         iEndPosTmp = EME_UTILS_STRCHR(pcMimeHeader + iStartPos, '\n') - pcMimeHeader;
         if(iEndPosTmp > 0)
         {
            iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\r\n") - pcMimeHeader;
            if(iEndPos > 0)
            {
               if(iEndPosTmp < iEndPos)
                  iEndPos = iEndPosTmp;
            }
            else
            {
               iEndPos = iEndPosTmp;
            }

            while(*(pcMimeHeader + iStartPos) == ' ') iStartPos++;
            
            pcContId = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
            EME_UTILS_BREAKNULL(pcContId,
               iResult,
               EME_NOMEMORY,
               "EME_MailMsg_ParsePartHead---: malloc failed!!!");
            
            EME_UTILS_MEMSET(pcContId,0x00,iEndPos - iStartPos + 1);
            EME_UTILS_MEMMOVE(pcContId, pcMimeHeader + iStartPos, iEndPos - iStartPos);
         }
         else
         {
            iResult = EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "parse content id failed");
         }
      }
      
      //       if ( (iStartPos = EME_UTILS_StrStrNC(pcMimeHeader, "content-location:") - pcMimeHeader) > 0 )
      //       {            
      //          iStartPos += 17; /* +"content-location:" */ 
      //          iEndPos = EME_UTILS_STRSTR(pcMimeHeader + iStartPos, "\n") - pcMimeHeader;
      //          while (pcMimeHeader[iStartPos] == ' ') iStartPos++;
      //          while (pcMimeHeader[iEndPos] == ' ') iEndPos--;
      //          pcContLocat = EME_UTILS_MALLOC(pvMemMgr,iEndPos - iStartPos + 1); /* +1 means +'\0' */
      //          EME_UTILS_BREAKNULL(pcContLocat,
      //                   iResult,
      //                   EME_NOMEMORY,
      //                   " malloc failed!!!");
      //          EME_UTILS_MEMSET(pcContLocat,0x00,iEndPos - iStartPos + 1);
      //          EME_UTILS_MEMMOVE(pcContLocat, pcMimeHeader + iStartPos, iEndPos - iStartPos);            
      //       }


      if (pcContType && EME_UTILS_StrStrNC(pcContType, "multipart") )
      {
         WE_INT32 iBoundPos            = -1;
         WE_CHAR* pcLineEnd            = NULL;
         WE_CHAR* pcNextBoundary       = NULL;
         
         if (EME_UTILS_StrStrNC(pcContType, "multipart/related") )
         {
            pstIn->bMultipartRelated = TRUE;
         }
         
         if ((iBoundPos = EME_MailMsg_FindHdFieldPos("boundary=", pcMimeHeader)) > 0)
         {
            if ( (pcLineEnd = EME_UTILS_STRCHR(pcMimeHeader + iBoundPos, '\n')) != NULL)
            {
               /* ========================= +3 means + "--" + '\0'=================== */
               pcNextBoundary = EME_UTILS_MALLOC(pvMemMgr,pcLineEnd - pcMimeHeader - iBoundPos + 3); 
               EME_UTILS_BREAKNULL(pcNextBoundary,
                        iResult,
                        EME_NOMEMORY,
                        "EME_MailMsg_ParsePartHead---: malloc failed!!!");
               EME_UTILS_MEMSET(pcNextBoundary,0x00,pcLineEnd - pcMimeHeader - iBoundPos + 3);
               EME_UTILS_STRCPY(pcNextBoundary, "--");
               EME_UTILS_MEMMOVE(pcNextBoundary + 2, 
                                 pcMimeHeader + iBoundPos, 
                                 pcLineEnd - (pcMimeHeader + iBoundPos));
            }
            else
            {
               pcNextBoundary = EME_UTILS_MALLOC(pvMemMgr,EME_UTILS_STRLEN(pcMimeHeader + iBoundPos) + 3); 
               EME_UTILS_BREAKNULL(pcNextBoundary,
                        iResult,
                        EME_NOMEMORY,
                        "EME_MailMsg_ParsePartHead---: malloc failed!!!");
               EME_UTILS_MEMSET(pcNextBoundary,0x00,EME_UTILS_STRLEN(pcMimeHeader + iBoundPos) + 3);
               EME_UTILS_STRCPY(pcNextBoundary, "--");
               EME_UTILS_STRCAT(pcNextBoundary, pcMimeHeader + iBoundPos);
            }
            EME_MailMsg_UnquoteString(pstEMsgHandle,pcNextBoundary + 2, '\"', '\"');/* +2 mean go behind "--" */
         }

         pstDecPartInfo->pcNextBoundary = pcNextBoundary;
         pcNextBoundary = NULL;
      }
      
      pstDecPartInfo->astMsgPart.pcContentType       = pcContType;
      pstDecPartInfo->astMsgPart.pcName              = pcName;
      pstDecPartInfo->astMsgPart.pcFileName          = pcFileName;
      pstDecPartInfo->astMsgPart.pcTransEnc          = pcTranEncoding;
      pstDecPartInfo->astMsgPart.pcContentId         = pcContId;
      pstDecPartInfo->astMsgPart.pcOnePartData       = NULL;
      pstDecPartInfo->astMsgPart.uiOnePartSize       = 0;
      pstDecPartInfo->astMsgPart.pcCharSet           = pcCharSet;

      pstDecPartInfo->iTransEnc                      = (WE_INT32)eEncoding;
      pstDecPartInfo->iCharset                       = (WE_INT32)eCharset;

   } while(0);
   
   if(iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcContType);
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcName);
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcFileName);
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcTranEncoding);
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcContId);
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcCharSet);
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParsePartHead---: End, Result = %d", iResult));
   return iResult;
}

static WE_INT32 EME_MailMsg_ParsePartBody(stEMSGHandle *pstEMsgHandle,
                              St_EmailMsg  *pstIn)
{
   WE_INT32  iResult = EME_SUCCESS;
   WE_CHAR  *pcSuffix = NULL;
   WE_CHAR  *pcFileName = NULL;
   WE_UINT32 uiFileNameLen = 0;
   WE_CHAR  *pcMark = NULL;
   void      *pvMemMgr = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParsePartBody---: Start"));
   if(NULL == pstEMsgHandle || NULL == pstIn)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ParsePartBody--: Bad parameter and return"));
      return EME_INVALID_DATA;
   }

   pvMemMgr = pstEMsgHandle->pvMemMgr;

   do 
   {
      if(NULL != pstIn->unPartInfo.astDecodingPartInfo.pcNextBoundary)
      {
         WE_INT32 iPartCount = 0;
         PARTINDEXTB pvIndexTable = NULL;

         pvIndexTable = EMEFile_GetPartIndexTable(pstIn->pvMailFile, 
            pvMemMgr, 
            pstIn->unPartInfo.astDecodingPartInfo.pcNextBoundary, 
            pstIn->unPartInfo.astDecodingPartInfo.uiBodySize,
            &iPartCount);
         if(pvIndexTable != NULL && iPartCount > 0)
         {
            WE_INT32 i = 0;
            WE_INT32 iOffset = 0;
            WE_INT32 iPartSize = 0;
            
            St_PartIndexInfo *pstPartIndexInfo = NULL;
            St_PartIndexInfo *pstIndexLast = NULL;
            St_PartIndexInfo *pstIndexFirst = NULL;
            
            /*create part list*/
            while(i < iPartCount)
            {
               iResult = EMEFile_GetOnePartInfo(pvIndexTable, i, pstIn->unPartInfo.astDecodingPartInfo.pcNextBoundary, &iOffset, &iPartSize);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: get one part info failed");
               
               pstPartIndexInfo = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_PartIndexInfo));
               EME_UTILS_BREAKNULL(pstPartIndexInfo, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_MEMSET(pstPartIndexInfo, 0, sizeof(St_PartIndexInfo));
               
               pstPartIndexInfo->uiOffset = (WE_INT32)iOffset;
               pstPartIndexInfo->uiPartSize = (WE_INT32)iPartSize;
               
               if(NULL == pstIndexFirst)
               {
                  pstIndexFirst = pstPartIndexInfo;
                  pstIndexLast = pstPartIndexInfo;
               }
               else
               {
                  pstIndexLast->next = pstPartIndexInfo;
                  pstIndexLast = pstPartIndexInfo;
               }
               i++;
            }

            EMFile_FreePartIndexTable(pvIndexTable, pvMemMgr);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: create part index list failed");

            /*applend list to part index list*/
            if(NULL != pstIn->pstPartIndexList)
            {
               pstIndexLast->next = pstIn->pstPartIndexList->next;
               pstIn->pstPartIndexList->next = pstIndexFirst;
            }
            else
            {
               pstIn->pstPartIndexList = pstIndexFirst;
            }

            pstIn->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
            pstIn->unPartInfo.astDecodingPartInfo.bIsAttachment = FALSE;
            
            if(pstEMsgHandle->pfnDecoding != NULL)
            {
               iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: set timer failed");
               
               pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
               pstEMsgHandle->pvRegisterData = pstEMsgHandle;
            }
            else
            {
               EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
            }
         }
         else
         {
            EMFile_FreePartIndexTable(pvIndexTable, pvMemMgr);
            iResult = EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: get part index table failed");
         }
      }
      else
      {
         if(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcName == NULL &&
            pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName == NULL &&
            pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet != NULL)
         {
            WE_BOOL  bIsTextPlain = FALSE;
            WE_BOOL  bBodyIsValid = FALSE;
            WE_CHAR  *pcBodyBuff = NULL;
            WE_CHAR  *pcDecodedWord = NULL;

            WE_UINT32 uiHadRead = 0;

            pcBodyBuff = EME_UTILS_MALLOC(pvMemMgr, pstIn->unPartInfo.astDecodingPartInfo.uiBodySize + 1);
            EME_UTILS_BREAKNULL(pcBodyBuff, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pcBodyBuff, 0, pstIn->unPartInfo.astDecodingPartInfo.uiBodySize + 1);

            uiHadRead = EME_FileRead(pstIn->pvMailFile, pcBodyBuff, pstIn->unPartInfo.astDecodingPartInfo.uiBodySize);
            if(uiHadRead != pstIn->unPartInfo.astDecodingPartInfo.uiBodySize)
            {
               EME_UTILS_FREEIF(pvMemMgr, pcBodyBuff);
               iResult = EME_FAILED;
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: read body data failed");
            }

            if (NULL != EME_UTILS_StrStrNC(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType, "text/plain"))
            {
               bBodyIsValid = TRUE;
               bIsTextPlain = TRUE;
               
               EME_UTILS_FREEIF(pvMemMgr, pstIn->pcPlainTransEnc);
               pstIn->pcPlainTransEnc = pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc;
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc = NULL;

               EME_UTILS_FREEIF(pvMemMgr, pstIn->pcPlainCharset);
               pstIn->pcPlainCharset = pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet;
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet = NULL;
            }
            else if (NULL != EME_UTILS_StrStrNC(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType, "text/html"))
            {
               bBodyIsValid = TRUE;
               bIsTextPlain = FALSE;

               EME_UTILS_FREEIF(pvMemMgr, pstIn->pcHtmlTransEnc);
               pstIn->pcHtmlTransEnc = pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc;
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcTransEnc = NULL;

               EME_UTILS_FREEIF(pvMemMgr, pstIn->pcHtmlCharset);
               pstIn->pcHtmlCharset = pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet;
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcCharSet = NULL;
            }
            
            if(bBodyIsValid)
            {
               if(pstIn->unPartInfo.astDecodingPartInfo.uiBodySize > 0)
               {
                  pcDecodedWord = EME_MailMsg_DecodeBody(
                     pstEMsgHandle,
                     pcBodyBuff, 
                     pstIn->unPartInfo.astDecodingPartInfo.uiBodySize, 
                     pstIn->unPartInfo.astDecodingPartInfo.iTransEnc, NULL);
				  EME_UTILS_FREEIF(pvMemMgr, pcBodyBuff);

                  if(bIsTextPlain)
                  {
                     pstIn->pcTextPlain = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcPlainCharset);
                     if(NULL == pstIn->pcTextPlain)
                        pstIn->pcTextPlain = EME_UTILS_STRDUP(pvMemMgr, "");
                  }
                  else
                  {
                     pstIn->pcTextHtml = EME_MailMsg_ParseBodyCharset(pstEMsgHandle,pcDecodedWord, pstIn->pcHtmlCharset);
                     if(NULL == pstIn->pcTextHtml)
                        pstIn->pcTextHtml = EME_UTILS_STRDUP(pvMemMgr, "");
                  }
               }
               else
               {
                  /*the html or text plain body is empty */
                  if(bIsTextPlain)
                     pstIn->pcTextPlain = EME_UTILS_STRDUP(pvMemMgr, "");
                  else
                     pstIn->pcTextHtml = EME_UTILS_STRDUP(pvMemMgr, "");
               }
            }
            else
            {
               iResult = EME_FAILED;
            }

            EME_UTILS_FREEIF(pvMemMgr, pcBodyBuff);
            EME_UTILS_FREEIF(pvMemMgr, pcDecodedWord);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: the part is not text or html");

            pstIn->unPartInfo.astDecodingPartInfo.bIsAttachment = FALSE;
            /*parse the body end*/
            if(pstEMsgHandle->pfnDecoding != NULL)
            {
               iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd, pstEMsgHandle);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: set timer failed");

               pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingPartsEnd;
               pstEMsgHandle->pvRegisterData = pstEMsgHandle;
            }
            else
            {
               EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
            }
         }
         else
         {
            WE_CHAR acSuffix[20] = {0};
            WE_CHAR acFileNameTmp[EME_MAX_EMAIL_FILENAME_LEN] = {0};

            pstIn->unPartInfo.astDecodingPartInfo.uiReadOnceSize = EME_DECODING_BUFFER_ONCE;
            pstIn->unPartInfo.astDecodingPartInfo.pcReadOnceBuff = EME_UTILS_MALLOC(pvMemMgr, pstIn->unPartInfo.astDecodingPartInfo.uiReadOnceSize + 1);
            EME_UTILS_BREAKNULL(pstIn->unPartInfo.astDecodingPartInfo.pcReadOnceBuff, iResult, EME_NOMEMORY, "EME_MailMsg_ParsePartBody---: no memory for Read once buffer");
            EME_UTILS_MEMSET(pstIn->unPartInfo.astDecodingPartInfo.pcReadOnceBuff, 0,
                             pstIn->unPartInfo.astDecodingPartInfo.uiReadOnceSize + 1);
            
            /*used to parse body one buffer by one*/
            if(NULL == pcSuffix && NULL != pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName)
            {
               pcSuffix = EME_UTILS_STRRCHR(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName, '.');
            }

            if(NULL != pcSuffix && EME_UTILS_STRLEN(pcSuffix) > 5)
            {
               pcSuffix = NULL;
            }
            
            if(NULL == pcSuffix && NULL != pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcName)
            {
               pcSuffix = EME_UTILS_STRRCHR(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcName, '.');
            }

            if(NULL != pcSuffix && EME_UTILS_STRLEN(pcSuffix) > 5)
            {
               pcSuffix = NULL;
            }

            if(NULL == pcSuffix)
            {
               EME_UTILS_MEMSET(acSuffix, 0, 20);
               
               acSuffix[0] = '.';
               pcSuffix = (WE_CHAR*)WeMime_Mime2Ext(pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcContentType);
               if(NULL != pcSuffix)
               {
                  EME_UTILS_MEMCPY(&acSuffix[1], pcSuffix, EME_UTILS_STRLEN(pcSuffix));
                  EME_UTILS_FREEIF(NULL, pcSuffix);
                  pcSuffix = acSuffix;
               }
            }
            
            if(NULL != pcSuffix)
            {
               uiFileNameLen += EME_UTILS_STRLEN(pcSuffix);
            }
            
            pcMark = pstIn->unPartInfo.astDecodingPartInfo.pcDir + EME_UTILS_STRLEN(pstIn->unPartInfo.astDecodingPartInfo.pcDir) - 1;
            
            uiFileNameLen += EME_UTILS_STRLEN(pstIn->unPartInfo.astDecodingPartInfo.pcDir);
            uiFileNameLen += EME_MAX_EMAIL_FILENAME_LEN;
            
            pcFileName = EME_UTILS_MALLOC(pvMemMgr, uiFileNameLen);
            EME_UTILS_BREAKNULL(pcFileName, iResult, EME_NOMEMORY, "EME_MailMsg_ParsePartBody---: no memory for object name");
            EME_UTILS_MEMSET(pcFileName, 0, uiFileNameLen);

            /*create file name*/
            if(pcSuffix != NULL && *(++pcSuffix) != '\0')
            {
               EME_UTILS_SPRINTF(acFileNameTmp, "eml%x.%s", EME_UTILS_GETTIMEMS, pcSuffix);
            }
            else
            {
               EME_UTILS_SPRINTF(acFileNameTmp, "eml%x", EME_UTILS_GETTIMEMS);
            }

            /*create file name*/
            if(pcMark != NULL && *pcMark == '\\')
            {
               EME_UTILS_SPRINTF(pcFileName, "%s%s", pstIn->unPartInfo.astDecodingPartInfo.pcDir, acFileNameTmp);
            }
            else
            {
               EME_UTILS_SPRINTF(pcFileName, "%s\\%s", pstIn->unPartInfo.astDecodingPartInfo.pcDir, acFileNameTmp);
            }

            if(NULL == pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName && NULL == pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcName)
            {
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcFileName = EME_UTILS_STRDUP(pvMemMgr, acFileNameTmp);
               pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcName = EME_UTILS_STRDUP(pvMemMgr, acFileNameTmp);
            }
            
            /*create file*/
            pstIn->unPartInfo.astDecodingPartInfo.pIFile = EME_FileOpen(pstEMsgHandle->pIFileMgr, pcFileName, EME_OFM_CREATE);
            EME_UTILS_BREAKNULL(pstIn->unPartInfo.astDecodingPartInfo.pIFile, iResult, EME_FAILED, "EME_MailMsg_ParsePartBody---: open file failed");
            
            pstIn->unPartInfo.astDecodingPartInfo.astMsgPart.pcOnePartData = pcFileName;
            pcFileName = NULL;
            
            pstIn->unPartInfo.astDecodingPartInfo.iResult = EME_SUCCESS;
            pstIn->unPartInfo.astDecodingPartInfo.bIsAttachment = TRUE;

            if(pstEMsgHandle->pfnDecoding != NULL)
            {
               iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingOneBufferCB, &pstIn->unPartInfo.astDecodingPartInfo);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_ParsePartBody---: set timer failed");

               pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingOneBufferCB;
               pstEMsgHandle->pvRegisterData = &pstIn->unPartInfo.astDecodingPartInfo;
            }
            else
            {
               EME_MailMsg_DecodingOneBufferCB(&pstIn->unPartInfo.astDecodingPartInfo);

               EME_UTILS_FREEIF(pvMemMgr, pcFileName);
               EME_UTILS_LOG_INFO(("EME_MailMsg_ParsePartBody---: End, Result = %d", 
               pstIn->unPartInfo.astDecodingPartInfo.iResult));
               return pstIn->unPartInfo.astDecodingPartInfo.iResult;

            }
         }
      }
   } while(0);

   if(iResult != EME_SUCCESS)
   {
      EME_DeleteFile(pstEMsgHandle->pIFileMgr, pcFileName);
      EME_FileClose(pstIn->unPartInfo.astDecodingPartInfo.pIFile);
      pstIn->unPartInfo.astDecodingPartInfo.pIFile = NULL;
   }

   EME_UTILS_FREEIF(pvMemMgr, pcFileName);

   EME_UTILS_LOG_INFO(("EME_MailMsg_ParsePartBody---: End, Result = %d", iResult));
   return iResult;
}

static void EME_MailMsg_DecodingOneBufferCB(St_DecodingPartInfo  *pstDecPartInfo)
{
   St_EmailMsg *pstEmailMsg = NULL;
   stEMSGHandle *pstEMsgHandle = NULL;
   WE_INT32 iResult = EME_SUCCESS;
   //WE_CHAR  *pcFileBuffer = NULL;
   WE_CHAR  *pcDecBuff = NULL;

   WE_UINT32 uiSize = 0;

   WE_UINT32 uiWantReadSize = 0;
   WE_UINT32 uiHadReadSize = 0;
   WE_UINT32 uiHadWriteSize = 0;

   WE_UINT32 uiDecodingSize = 0;
   WE_UINT32  uiNewSize = 0;

   WE_BOOL   bIsEnd = FALSE;
   
   St_BinhexUseData  *pstBinUserData = NULL;
   St_BinhexCatch *pstGet = NULL;
   St_BinhexCatch *pstWrite = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingOneBufferCB---: Start"));
   EME_UTILS_LOG_INFO(("EME_MailMsg_EncodingOneBufferCB---: Performance decoding one buffer %d", EME_UTILS_GETTIMEMS));
   if(NULL == pstDecPartInfo)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_DecodingOneBufferCB--: Bad parameter and return"));
      return;
   }

   pstEMsgHandle = (stEMSGHandle*)pstDecPartInfo->pvEMsgHdl;
   if(NULL == pstEMsgHandle)
   {
      pstDecPartInfo->iResult = EME_FAILED;
      EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
      return;
   }

   pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
   if(NULL == pstEmailMsg)
   {
      pstDecPartInfo->iResult = EME_FAILED;
      EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
      return;
   }

   if(pstDecPartInfo->uiBodySize == 0)
   {
      EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingOneBufferCB--: End parse one part object"));
      pstDecPartInfo->astMsgPart.uiOnePartSize = EME_FileLength(pstDecPartInfo->pIFile);
      EME_FileClose(pstDecPartInfo->pIFile);
      pstDecPartInfo->pIFile = NULL;

      pstDecPartInfo->iResult = EME_SUCCESS;
      EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
      return;
   }
   do 
   {
      if(E_ENCODETYPE_UNKNOW == pstDecPartInfo->iTransEnc)
         pstDecPartInfo->iTransEnc = E_ENCODETYPE_7BIT;
      switch(pstDecPartInfo->iTransEnc)
      {
      case E_ENCODETYPE_QUOTEDPRINTABLE:
      case E_ENCODETYPE_BASE64:
      case E_ENCODETYPE_8BIT:
      case E_ENCODETYPE_7BIT:
         {
            uiWantReadSize = pstDecPartInfo->uiBodySize - pstDecPartInfo->uiHadRead;
            if(uiWantReadSize > pstDecPartInfo->uiReadOnceSize)
            {
               uiWantReadSize = pstDecPartInfo->uiReadOnceSize;
            }
            else
            {
               bIsEnd = TRUE;
            }
            
            uiHadReadSize= EME_FileRead(pstEmailMsg->pvMailFile, pstDecPartInfo->pcReadOnceBuff, uiWantReadSize);
            iResult = (uiHadReadSize == uiWantReadSize) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: read file failed");
            
            if(!bIsEnd)
            {
               iResult = EME_ResetOneSegment(pstDecPartInfo->pcReadOnceBuff, pstEmailMsg->unPartInfo.astDecodingPartInfo.iTransEnc, &uiNewSize);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: rest one segment failed");
               if(uiNewSize != uiHadReadSize)
               {
                  iResult = EME_FileSeek(pstEmailMsg->pvMailFile, -(WE_INT32)(uiHadReadSize - uiNewSize), EME_SEEK_CURRENT);
                  EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: seek file failed");
               }
               uiHadReadSize = uiNewSize;
            }
            
            pstDecPartInfo->uiHadRead += uiHadReadSize;
            uiDecodingSize = uiHadReadSize;
         }
         break;

      case E_ENCODETYPE_UUENCODE:
         {
            WE_UINT32  uiHeadSize = 0;
            uiHeadSize = EME_UTILS_STRLEN(pstDecPartInfo->pcReadOnceBuff);
            if(uiHeadSize >= pstDecPartInfo->uiReadOnceSize)
            {
               iResult = EME_FAILED;
               break;
            }
            
            if(uiHeadSize == 0)
            {
               uiWantReadSize = pstDecPartInfo->uiBodySize - pstDecPartInfo->uiHadRead;
               if(uiWantReadSize > pstDecPartInfo->uiReadOnceSize)
               {
                  uiWantReadSize = pstDecPartInfo->uiReadOnceSize;
               }
            }
            else
            {
               uiWantReadSize = pstDecPartInfo->uiBodySize - pstDecPartInfo->uiHadRead;
               if(uiWantReadSize > pstDecPartInfo->uiReadOnceSize - uiHeadSize)
               {
                  uiWantReadSize = pstDecPartInfo->uiReadOnceSize - uiHeadSize;
               }
            }
            
            uiHadReadSize= EME_FileRead(pstEmailMsg->pvMailFile, pstDecPartInfo->pcReadOnceBuff + uiHeadSize, uiWantReadSize);
            iResult = (uiHadReadSize == uiWantReadSize) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: read file failed");
            
            iResult = EME_ResetOneSegment(pstDecPartInfo->pcReadOnceBuff, pstDecPartInfo->iTransEnc, &uiNewSize);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: rest one segment failed");
            if(uiNewSize != (uiHadReadSize + uiHeadSize))
            {
               iResult = EME_FileSeek(pstEmailMsg->pvMailFile, -(WE_INT32)(uiHadReadSize + uiHeadSize - uiNewSize), EME_SEEK_CURRENT);
               EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: seek file failed");
            }
            
            uiHadReadSize = uiNewSize - uiHeadSize;
            pstDecPartInfo->uiHadRead += uiHadReadSize;

            uiDecodingSize = EME_UTILS_STRLEN(pstDecPartInfo->pcReadOnceBuff);

            break;
         }
      case E_ENCODETYPE_BINHEX:
         {
            WE_BOOL bRet = FALSE;

            /*get catch*/
            pstGet = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, sizeof(St_BinhexCatch));
            EME_UTILS_BREAKNULL(pstGet, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pstGet, 0, sizeof(St_BinhexCatch));
            pstGet->pcBuff = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, EME_DECODING_BUFFER_ONCE + 1);
            EME_UTILS_BREAKNULL(pstGet->pcBuff, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pstGet->pcBuff, 0, EME_DECODING_BUFFER_ONCE + 1);
            pstGet->piFile = pstEmailMsg->pvMailFile;
            pstGet->uiBodySize = pstDecPartInfo->uiBodySize;

            /*write catch*/
            pstWrite = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, sizeof(St_BinhexCatch));
            EME_UTILS_BREAKNULL(pstWrite, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pstWrite, 0, sizeof(St_BinhexCatch));
            pstWrite->pcBuff = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, EME_DECODING_BUFFER_ONCE + 1);
            EME_UTILS_BREAKNULL(pstWrite->pcBuff, iResult, EME_NOMEMORY, "no memory");
            EME_UTILS_MEMSET(pstWrite->pcBuff, 0, EME_DECODING_BUFFER_ONCE + 1);
            pstWrite->piFile = pstDecPartInfo->pIFile;
            
            pstBinUserData = EME_UTILS_MALLOC(pstEMsgHandle->pvMemMgr, sizeof(St_BinhexUseData));
            EME_UTILS_BREAKNULL(pstBinUserData, iResult, EME_NOMEMORY, "no memory for hex user data");
            EME_UTILS_MEMSET(pstBinUserData, 0, sizeof(St_BinhexUseData));

            pstBinUserData->pstGet = pstGet;
            pstBinUserData->pstWrite = pstWrite;
            pstBinUserData->pvData = pstEMsgHandle;

            bRet = parseBinHex(pstEMsgHandle->pICBMgr, (GET)EME_MailMsg_Get, pstGet, (WRITE)EME_MailMsg_Write,pstWrite,(BH_NOTIFY)EME_MailMsg_ParseBinhexCB,pstBinUserData);
            iResult = (bRet) ? EME_SUCCESS : EME_FAILED;
            EME_UTILS_BREAKIF(iResult, "parse binhex failed");

            return;
         }
         break;

      default:
         iResult = EME_FAILED;
         break;
      }
      
      EME_UTILS_BREAKIF(iResult, "read file failed");

      pcDecBuff = EME_MailMsg_DecodeBody(pstEMsgHandle, pstDecPartInfo->pcReadOnceBuff, uiDecodingSize, pstDecPartInfo->iTransEnc, &uiSize);
      EME_UTILS_BREAKNULL(pcDecBuff, iResult, EME_FAILED, "EME_MailMsg_DecodingOneBufferCB--: decoding failed");
      
      uiHadWriteSize = EME_FileWrite(pstDecPartInfo->pIFile, pcDecBuff, uiSize);
      iResult = (uiHadWriteSize == uiSize) ? EME_SUCCESS : EME_NOSTORAGE;
      EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: write to file failed");
      
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcDecBuff);

      switch(pstDecPartInfo->iTransEnc)
      {
      case E_ENCODETYPE_UUENCODE:
         {
            WE_CHAR *pcTmp = NULL;
            pcTmp = EME_UTILS_STRSTR(pstDecPartInfo->pcReadOnceBuff, "\r\n");
            if(pcTmp != NULL)
            {
               EME_UTILS_MEMSET(pcTmp + 2, 0, EME_UTILS_STRLEN(pcTmp + 2));
            }
            else
            {
               EME_UTILS_MEMSET(pstDecPartInfo->pcReadOnceBuff, 0, pstDecPartInfo->uiReadOnceSize + 1);
            }
         }
         break;
      default:
         EME_UTILS_MEMSET(pstDecPartInfo->pcReadOnceBuff, 0, pstDecPartInfo->uiReadOnceSize + 1);
         break;   
      }

      if(pstDecPartInfo->uiHadRead >= pstDecPartInfo->uiBodySize)
      {
         EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingOneBufferCB--: End parse one part object"));
         pstDecPartInfo->astMsgPart.uiOnePartSize = EME_FileLength(pstDecPartInfo->pIFile);
         EME_FileClose(pstDecPartInfo->pIFile);
         pstDecPartInfo->pIFile = NULL;

         pstDecPartInfo->iResult = EME_SUCCESS;
         EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
      }
      else
      {
         if(pstEMsgHandle->pfnDecoding != NULL)
         {
            iResult = OEM_AsyncProc_AddProc(pstEMsgHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_DecodingOneBufferCB, pstDecPartInfo);
            EME_UTILS_BREAKIF(iResult, "EME_MailMsg_DecodingOneBufferCB--: set timer failed");

            pstEMsgHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_DecodingOneBufferCB;
            pstEMsgHandle->pvRegisterData = pstDecPartInfo;
         }
         else
         {
            EME_MailMsg_DecodingOneBufferCB(pstDecPartInfo);
         }
      }
   	
   } while(0);

   if(iResult != SUCCESS)
   {
      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pcDecBuff);   
      EME_UTILS_MEMSET(pstDecPartInfo->pcReadOnceBuff, 0, pstDecPartInfo->uiReadOnceSize + 1);
      
      if(pstGet != NULL)
      {
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstGet->pcBuff);
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstGet);
      }

      if(pstWrite != NULL)
      {
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstWrite->pcBuff);
         EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstWrite);
      }

      EME_UTILS_FREEIF(pstEMsgHandle->pvMemMgr, pstBinUserData);

      EME_DeleteFile(pstEMsgHandle->pIFileMgr, pstDecPartInfo->astMsgPart.pcOnePartData);
      EME_FileClose(pstDecPartInfo->pIFile);
      pstDecPartInfo->pIFile = NULL;

      pstDecPartInfo->iResult = iResult;
      EME_MailMsg_DecodingPartsEnd(pstEMsgHandle);
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_DecodingOneBufferCB---: End, Result = %d", iResult));

   return;
}

static void EME_MailMsg_ClearDecodingPartInfo(stEMSGHandle *pstEMsgHandle, St_DecodingPartInfo *pstDecPartInfo)
{
   void *pvMemMgr = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_ClearDecodingPartInfo---: Start"));
   if(NULL == pstEMsgHandle || NULL == pstDecPartInfo)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ClearDecodingPartInfo--: Bad parameter and return"));
      return;
   }

   EME_FileClose(pstDecPartInfo->pIFile);
   pstDecPartInfo->pIFile = NULL;

   pvMemMgr = pstEMsgHandle->pvMemMgr;
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->pcDir);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->pcNextBoundary);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->pcReadOnceBuff);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcCharSet);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcContentId);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcContentType);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcFileName);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcName);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcOnePartData);
   EME_UTILS_FREEIF(pvMemMgr, pstDecPartInfo->astMsgPart.pcTransEnc);
   EME_UTILS_LOG_INFO(("EME_MailMsg_ClearDecodingPartInfo---: End"));
}

static void EME_MailMsg_ClearEncodingPartInfo(stEMSGHandle *pstEMsgHandle, St_EncodingPartInfo *pstEncPartInfo)
{
   EME_UTILS_LOG_INFO(("EME_MailMsg_ClearEncodingPartInfo---: Start"));
   if(NULL == pstEMsgHandle || NULL == pstEncPartInfo)
   {
      EME_UTILS_LOG_ERROR(("EME_MailMsg_ClearEncodingPartInfo--: Bad parameter and return"));
      return;
   }

   EME_FileClose(pstEncPartInfo->pIFile);
   pstEncPartInfo->pIFile = NULL;
   EME_UTILS_LOG_INFO(("EME_MailMsg_ClearEncodingPartInfo---: End"));
   
}


static WE_INT32 EME_MailMsg_Get(St_BinhexCatch *pstBinhex)
{
   WE_UINT32 uiWantRead = 0;

   if(NULL == pstBinhex || NULL == pstBinhex->pcBuff || NULL == pstBinhex->piFile)
   {
      return -1;
   }

   if(pstBinhex->uiLen == pstBinhex->uiCurr)
   {
      if(pstBinhex->uiBodySize <= pstBinhex->uiUsedSize)
      {
         return -1;
      }

      EME_UTILS_MEMSET(pstBinhex->pcBuff, 0, EME_DECODING_BUFFER_ONCE + 1);

      if((pstBinhex->uiBodySize - pstBinhex->uiUsedSize) > EME_DECODING_BUFFER_ONCE)
      {
         uiWantRead = EME_DECODING_BUFFER_ONCE;
      }
      else
      {
         uiWantRead = pstBinhex->uiBodySize - pstBinhex->uiUsedSize;
      }

      pstBinhex->uiLen = EME_FileRead(pstBinhex->piFile, pstBinhex->pcBuff, uiWantRead);
      if(pstBinhex->uiLen != uiWantRead)
      {
         return -1;
      }

      pstBinhex->uiUsedSize += uiWantRead;
      pstBinhex->uiCurr = 0;
   }

   return (WE_INT32)pstBinhex->pcBuff[pstBinhex->uiCurr++];
}

static void EME_MailMsg_Write(St_BinhexCatch *pstBinhex, WE_INT32 c)
{
   if(NULL == pstBinhex || NULL == pstBinhex->pcBuff || NULL == pstBinhex->piFile)
   {
      return;
   }

   pstBinhex->pcBuff[pstBinhex->uiLen++] = (char)c;
   if(pstBinhex->uiLen == EME_DECODING_BUFFER_ONCE)
   {
      EME_FileWrite(pstBinhex->piFile, pstBinhex->pcBuff, pstBinhex->uiLen);
      EME_UTILS_MEMSET(pstBinhex->pcBuff, 0, EME_DECODING_BUFFER_ONCE + 1);
      pstBinhex->uiLen = 0;
   }
}

static void EME_MailMsg_ParseBinhexCB(WE_VOID *p, WE_BOOL bSuccess)
{
   St_BinhexUseData *pstUseData = (St_BinhexUseData*)p;
   St_DecodingPartInfo *pstDecInfo = NULL;
   St_EmailMsg         *pstEmailMsg = NULL;

   stEMSGHandle     *pstMsgHdl = NULL;
   WE_INT32         iResult = EME_SUCCESS;
   void             *pvMemMgr = NULL;

   if(NULL == pstUseData)
   {
      return;
   }

   do 
   {
   	pstMsgHdl = (stEMSGHandle*)pstUseData->pvData;
      EME_UTILS_BREAKNULL(pstMsgHdl, iResult, EME_FAILED, "message handle is null");
      pvMemMgr = pstMsgHdl->pvMemMgr;

      pstEmailMsg = (St_EmailMsg*)pstMsgHdl->pvEmailMsg;
      EME_UTILS_BREAKNULL(pstEmailMsg, iResult, EME_FAILED, "mail msg is null");

      pstDecInfo = (St_DecodingPartInfo*)&pstEmailMsg->unPartInfo.astDecodingPartInfo;
      
      if(bSuccess)
      {
         if(NULL != pstUseData->pstWrite && 0 < pstUseData->pstWrite->uiLen)
         {
            EME_FileWrite(pstUseData->pstWrite->piFile, pstUseData->pstWrite->pcBuff, pstUseData->pstWrite->uiLen);
            pstUseData->pstWrite->piFile = NULL;
         }
         pstDecInfo->iResult = EME_SUCCESS;
         EME_FileClose(pstDecInfo->pIFile);
         pstDecInfo->pIFile = NULL;
      }
      else
      {
         EME_FileClose(pstDecInfo->pIFile);
         pstDecInfo->pIFile = NULL;
         EME_DeleteFile(pstMsgHdl->pIFileMgr, pstDecInfo->astMsgPart.pcOnePartData);
         pstDecInfo->iResult = EME_FAILED;
      }

      EME_MailMsg_DecodingPartsEnd(pstMsgHdl);

   } while(0);

   if(NULL != pstUseData->pstGet)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstUseData->pstGet->pcBuff);
      EME_UTILS_FREEIF(pvMemMgr, pstUseData->pstGet);
   }
   if(NULL != pstUseData->pstWrite)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstUseData->pstWrite->pcBuff);
      EME_UTILS_FREEIF(pvMemMgr, pstUseData->pstWrite);
   }

   EME_UTILS_FREEIF(pvMemMgr, pstUseData);
}

static WE_INT32 EME_MailMsg_NotifyResult(void *pvData)
{
   stEMSGHandle* pstEMsgHandle = (stEMSGHandle*)pvData;
   St_EmailMsg *pstEmailMsg = NULL;

   EME_UTILS_LOG_INFO(("EME_MailMsg_NotifyResult---: Start"));
   if(NULL == pstEMsgHandle)
   {
      return EME_FAILED;
   }

   pstEMsgHandle->pfnRegister = NULL;
   pstEMsgHandle->pvRegisterData = NULL;

   pstEmailMsg = (St_EmailMsg*)pstEMsgHandle->pvEmailMsg;
   if(NULL == pstEmailMsg)
   {
      return EME_FAILED;
   }

   if(pstEMsgHandle->pfnEncoding != NULL)
   {
      (pstEMsgHandle->pfnEncoding)(pstEMsgHandle, 
         pstEMsgHandle->pstEncodedOpts, 
         pstEMsgHandle->pvUserData, 
         pstEmailMsg->unPartInfo.astEncodingPartInfo.iResult);
   }
   else if(pstEMsgHandle->pfnDecoding != NULL)
   {
      (pstEMsgHandle->pfnDecoding)(pstEMsgHandle, pstEMsgHandle->pvUserData, pstEmailMsg->unPartInfo.astDecodingPartInfo.iResult);
   }
   else
   {
      return EME_FAILED;
   }

   EME_UTILS_LOG_INFO(("EME_MailMsg_NotifyResult---: End and Result = %d", EME_SUCCESS));
   return EME_SUCCESS;
}


WE_INT32 EME_MailMsg_DecodeBodyStruct(stEMSGHandle *pstInHandle, WE_CHAR* pBodyStructBuf, WE_CHAR *pcPath)
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

   WE_VOID              *piBodyStrFile    = NULL;
   WE_CHAR              *pcBodyStruct     = NULL;
   WE_UINT32            uiFileSize        = 0;
   
   if (NULL == pstEmailHandle || NULL == pBodyStructBuf || NULL == pcPath)
   {
      EME_UTILS_LOG_ERROR(("EME_DecodeBodyStruct(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   pvMemMgr = pstEmailHandle->pvMemMgr;

   do 
   {
      piBodyStrFile = EME_FileOpen(pstInHandle->pIFileMgr, pBodyStructBuf, EME_OFM_READ);
      EME_UTILS_BREAKNULL(piBodyStrFile, iResult, EME_FAILED, "open file failed");

      uiFileSize = EME_FileLength(piBodyStrFile);
      iResult = (uiFileSize > 0) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "file size is 0");

      pcBodyStruct = EME_UTILS_MALLOC(pvMemMgr, uiFileSize + 1);
      EME_UTILS_BREAKNULL(pcBodyStruct, iResult, EME_NOMEMORY, "no memory for body struct");
      EME_UTILS_MEMSET(pcBodyStruct, 0, uiFileSize + 1);

      iResult = EME_FileRead(piBodyStrFile, pcBodyStruct, uiFileSize);
      iResult = (iResult == (WE_INT32)uiFileSize) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "Read file failed");
      EME_FileClose(piBodyStrFile);
      piBodyStrFile = NULL;
      
      iResult = EME_BodyStruct_Create(pstEmailHandle,&pEmailBodyStruct);
      EME_UTILS_BREAKIF(iResult,"EME_BodyStruct_Create(),failed!!!!!!!");
      EME_UTILS_BREAKNULL(pEmailBodyStruct,
                          iResult,
                          EME_NOMEMORY,
                          "EME_BodyStruct_Create(),malloc memory failed!!!!!");
      
      iResult = EME_BodyStruct_Parse(pstEmailHandle,pEmailBodyStruct, pcBodyStruct); 
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

         /*mime type*/
         if(pBodyStructPart->eBodyStructType == E_BODYS_TEXTPLAIN)
         {
            pstOneOpt->iId = EMAILPART_OPT_MIME_TYPE;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr, "text/plain");
            pstOneOpt++;
         }
         else if (pBodyStructPart->eBodyStructType == E_BODYS_TEXTHTML)
         {
            pstOneOpt->iId = EMAILPART_OPT_MIME_TYPE;
            pstOneOpt->pVal = EME_UTILS_STRDUP(pvMemMgr, "text/html");
            pstOneOpt++;
         }
     
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

         if(NULL != pstInHandle->pfnDecoding)
         {
            iResult = OEM_AsyncProc_AddProc(pstEmailHandle->pICBMgr, (PFNNOTIFY)EME_MailMsg_NotifyResult, pstInHandle);
            
            pstInHandle->pfnRegister = (PFNNOTIFY)EME_MailMsg_NotifyResult;
            pstInHandle->pvRegisterData = pstInHandle;
         }
      }
      
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcBodyStruct);
   EME_BodyStruct_Destroy(pstEmailHandle,&pEmailBodyStruct);

   /*error handling*/
   if (EME_SUCCESS != iResult)
   {
      EME_FileClose(piBodyStrFile);
      piBodyStrFile = NULL;
      
      if (NULL != pMsgPart)
      {
         EME_FreeAllOptsValue(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr, pMsgPart->pstPartOpts);
         EME_UTILS_FREEIF(pvMemMgr,pMsgPart);
      }
   }
   return iResult;
}

static WE_BOOL EME_MailMsg_IsVaildUTF8(WE_CHAR * pUTF8)
{
   WE_INT32 nLen = 0;
   WE_INT32 i = 0;
   WE_UCHAR ucASCII = 0;
   WE_INT32 nCount = 0;
   WE_INT32 m = 0;

   if ( NULL == pUTF8)
   {
      return TRUE;
   }

   nLen = EME_UTILS_STRLEN(pUTF8);
   
   for(i = 0; i < nLen;)
   {
      ucASCII = (WE_UCHAR)pUTF8[i];
      if(ucASCII>=0 && ucASCII < 0x7f)
      {
         i++;
         continue;
      }
      if(ucASCII < 0xc0 || ucASCII>0xfd) 
         return FALSE;
      
      nCount = ucASCII>0xfc?5:ucASCII>0xf8?4:ucASCII>0xf0?3:ucASCII>0xe0?2:1;
      
      if(i+nCount > nLen) 
         return FALSE;
      
      i++;
      for(m=0;m<nCount;m++)
      {
         if(((WE_UCHAR)pUTF8[i])<0x80 || ((WE_UCHAR)pUTF8[i])>0xbf) 
            return FALSE;
         i++;
      }
   }
   return TRUE;
}
