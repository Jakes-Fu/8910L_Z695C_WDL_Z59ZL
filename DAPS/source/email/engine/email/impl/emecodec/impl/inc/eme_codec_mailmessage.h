/*=====================================================================================
FILE NAME   : eme_codec_mailmessage.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File is the header file of eme_codec_mailmessage.c.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
          20070307       wxs                         create
        
                 
=====================================================================================*/

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_MAILMESSAGE_H_
#define _EME_CODEC_MAILMESSAGE_H_

/*=================================================================================
 *               Include Files                                                  *
 *=================================================================================*/
#include "eme_codec_def.h"
#include "we_def.h"
#include "eme_utils_api.h"

/*==================================================================================================
* macro Section
==================================================================================================*/
#define EMAIL_MAX_ENCODED_OPT          0x04
#define EME_MAX_HEAD_OPTS              0x20
#define EME_MAX_PART_OPTS              0x10
#define EME_DEFAULT_ATTACH_NAME        "attachment"
/*==================================================================================================
* Type Define Section
==================================================================================================*/  
typedef struct _St_EmailMsgPart
{
    WE_CHAR       *pcName;                            /* attachment custom name. */
    WE_CHAR       *pcFileName;                        /* attachment file name. */
    WE_CHAR       *pcContentType;                     /* type of att. */
    WE_CHAR       *pcTransEnc;                    
    WE_CHAR       *pcCharSet;

    WE_CHAR       *pcOnePartData;                     /* content of mail. */
    WE_CHAR       *pcContentId;                       /* content id of part */
//     WE_CHAR       *pcContentLoc;                      /* content location of part */
    WE_UINT32     uiOnePartSize;                      /* the size of body. */
    
}St_EmailMsgPart;

typedef struct _St_EncodingInfo
{
   WE_VOID         *pvEMsgHdl;
   St_EmailMsgPart *pstMsgPart;

   WE_UINT32       uiHadRead;
   WE_UINT32       uiFileSize;
   WE_VOID         *pIFile;

   WE_UINT32       uiReadOnceSize;
   WE_CHAR         *pcReadOnceBuff;
   WE_INT32        iResult;
}St_EncodingPartInfo;

typedef struct _St_DecodingPartInfo
{
   WE_VOID        *pvEMsgHdl;
   WE_VOID        *pIFile;
   WE_INT32       iResult;
   WE_CHAR        *pcNextBoundary;
   WE_CHAR        *pcDir;
   WE_INT32        iTransEnc;
   WE_INT32        iCharset;

   WE_UINT32      uiBodySize;
   WE_UINT32      uiHadRead;

   WE_UINT32      uiReadOnceSize;
   WE_CHAR        *pcReadOnceBuff;
   WE_CHAR        *pcUUHead;
   WE_BOOL        bIsAttachment;

   WE_CHAR        res[3];

   St_EmailMsgPart astMsgPart;

}St_DecodingPartInfo;

typedef struct _St_PartIndexInfo
{
   WE_UINT32  uiOffset;
   WE_UINT32  uiPartSize;
   struct _St_PartIndexInfo *next;
}St_PartIndexInfo;

typedef struct _St_EmailMsg
{
   WE_CHAR             *pcFrom;
   WE_CHAR             *pcTo;
   WE_CHAR             *pcCc;
   WE_CHAR             *pcBcc;
   WE_CHAR             *pcNotifyTo;
   WE_CHAR             *pcSubject;
   WE_CHAR             *pcDate;
   WE_CHAR             *pcReplyTo;
   WE_CHAR             *pcPriority;
   WE_CHAR             *pcMimeVersion;
   E_EncodeType        eTransEncoding;
   WE_CHAR             *pcTextPlain;
   WE_CHAR             *pcTextHtml;
   /* just used for decode email to utf16 */ 
   WE_UINT16           *pwcTextPlain;
   WE_UINT16           *pwcTextHtml;
   
   WE_CHAR             *pcPlainCharset;
   WE_CHAR             *pcHtmlCharset;
   WE_CHAR             *pcPlainTransEnc;
   WE_CHAR             *pcHtmlTransEnc;
   WE_CHAR             *pcBoundary;
   
   /* encoded body string. */
   WE_UINT32           uiEMailDataSize;
   WE_CHAR             *pcEmailDataBuf;

   /*just used for file operate*/
   WE_VOID             *pvMailFile;
   WE_CHAR             *pcMailFileName;

   union
   {
      St_EncodingPartInfo astEncodingPartInfo;      /*used for encoding message*/
      St_DecodingPartInfo astDecodingPartInfo;
   }unPartInfo;

   /*want to be decoding's part list*/
   St_PartIndexInfo    *pstPartIndexList;
   
   /* list of header items, each item is delimited by '\n' */
   WE_CHAR**          ppcHeaderFields;
   WE_UINT32          uiHeaderFieldNum;
   WE_UINT32          uiCurrIndex;
   
   /* list of X-header items, each item is delimited by '\n' */
   /*
   WE_CHAR **_extraFields;
   WE_UINT32 _nExtraCount;*/
   
   St_EmailMsgPart**   ppstEMsgPart;
   WE_UINT32           uiEMsgPartNum;
   
   WE_BOOL             bIsDecoding;
   WE_BOOL             bMultipartRelated;             /* whether is multipart related */
   WE_BOOL             bIsHtml;                       /* True:text/plain  False:text/html*/
   WE_BOOL             bIsAttachment;                 /*Only contain attachment*/
 
}St_EmailMsg;

typedef struct _St_BinhexCatch
{
   void      *piFile;
   WE_CHAR   *pcBuff;
   WE_UINT32 uiBodySize;
   WE_UINT32 uiUsedSize;
   WE_UINT32 uiLen;
   WE_UINT32 uiCurr;
}St_BinhexCatch;

typedef struct _St_BinhexUseData
{
   St_BinhexCatch *pstGet;
   St_BinhexCatch *pstWrite;
   void           *pvData;
}St_BinhexUseData;

/*=====================mail Msg interfaces===============*/
WE_INT32  EME_CreateMailMsg(stEMSGHandle *pstEMsgHandle,
                            void** ppVoid); 

WE_INT32  EME_DestroyMailMsg(stEMSGHandle *pstEMsgHandle,
                             void** ppVoid); 
 
 
/* --------set/get fields for mail------- */
WE_INT32  EME_MailMsg_SetFrom(stEMSGHandle *pstEMsgHandle,
                              void* pvstIn,
                              WE_CHAR* pcFromAddr,
                              WE_CHAR* pcFromName);

WE_INT32  EME_MailMsg_SetReplyTo(stEMSGHandle *pstEMsgHandle,
                                 void* pvstIn, 
                                 WE_CHAR* pcReplyAddr,
                                 WE_CHAR* pcReplyName);

WE_INT32  EME_MailMsg_AppendNotifyTo(stEMSGHandle *pstEMsgHandle,
                              void* pvstIn,
                              WE_CHAR* pcNotifyToAddr,
                              WE_CHAR* pcNotifyToName);

WE_INT32  EME_MailMsg_AppendTo(stEMSGHandle *pstEMsgHandle,
                               void* pvstIn,
                               WE_CHAR* pcToAddr, 
                               WE_CHAR* pcToName);

WE_INT32  EME_MailMsg_AppendCc(stEMSGHandle *pstEMsgHandle,
                               void* pvstIn,
                               WE_CHAR* pcCcAddr,
                               WE_CHAR* pcCcName);

WE_INT32  EME_MailMsg_AppendBcc(stEMSGHandle *pstEMsgHandle,
                                void* pvstIn,
                                WE_CHAR* pcBccAddr, 
                                WE_CHAR* pcBccName);

WE_INT32  EME_MailMsg_SetSubject(stEMSGHandle *pstEMsgHandle,
                                 void* pvstIn, 
                                 WE_CHAR* pcSubject);

WE_INT32  EME_MailMsg_SetDateTime(stEMSGHandle *pstEMsgHandle,
                                  void* pvstIn, WE_UINT16 week,
                                  WE_UINT16 dd, WE_UINT16 mm,
                                  WE_UINT16 yy, WE_UINT16 hour, 
                                  WE_UINT16 min, WE_UINT16 sec, 
                                  WE_INT16 difftime);

WE_INT32 EME_MailMsg_SetDateTimeEx(stEMSGHandle *pstEMsgHandle,
                                 void* pstIn, WE_UINT32 uiSecond,  WE_INT16 difftime);

WE_INT32 EME_SetPriority(stEMSGHandle *pstInHandle,
                         void *pvstIn, 
                         WE_UINT32 uiPriority);

WE_INT32  EME_MailMsg_SetTextPlain(stEMSGHandle *pstEMsgHandle,
                                    void* pvstIn,
                                    WE_CHAR* pcTextPlain,
                                    WE_CHAR* pcCharSet,
                                    WE_CHAR* pcTransEnc);

WE_INT32  EME_MailMsg_SetTextHtml(stEMSGHandle *pstEMsgHandle,
                                  void*pvstIn,
                                  WE_CHAR* pcTextHtml,
                                  WE_CHAR* pcCharSet,
                                  WE_CHAR* pcTransEnc);

WE_INT32 EME_EncodingMailMsg(stEMSGHandle *pstEMsgHandle,void* pvstIn);


WE_INT32 EME_MailMsg_GetDateTime(St_EmailMsg *pstIn, 
                                 TimeType *pstJulianDateType, 
                                 WE_INT16* difftime);

WE_INT32  EME_MailMsg_GetReplyTo(stEMSGHandle *pstEMsgHandle,
                                 St_EmailMsg* pstIn,
                                 WE_CHAR** ppRet);

WE_INT32  EME_MailMsg_GetPlainText(stEMSGHandle *pstEMsgHandle,
                                    St_EmailMsg* pstIn, 
                                    WE_CHAR** ppRet);

WE_INT32  EME_MailMsg_GetHtmlText(stEMSGHandle *pstEMsgHandle,
                                St_EmailMsg* pstIn,
                                WE_CHAR** ppRet);

WE_INT32 EME_MailMsg_AddMsgPart(stEMSGHandle *pstEMsgHandle,
                                St_EmailMsg* pstIn, 
                                St_EmailMsgPart* pstEmlPart);

St_EmailMsgPart* EME_MailMsg_GetOnePartByIndex(void* pvstIn, 
                                               WE_UINT32 uiPartIndex);

void* EME_MailMsg_GetEncodeBuf(void* pvstIn);

WE_INT32 EME_MailMsg_ParseDataBuf(stEMSGHandle *pstEMsgHandle,
                                  void* pvstIn, 
                                  WE_CHAR* src, 
                                  WE_BOOL bOnlyHeader);

WE_INT32 EME_MailMsg_ClearMsgParts(stEMSGHandle *pstEMsgHandle,void* pvstIn);

WE_INT32 EME_MailMsgPart_Create(stEMSGHandle *pstEMsgHandle,void** ppVoid);
WE_INT32 EME_MailMsgPart_Destroy(stEMSGHandle *pstEMsgHandle,void** ppVoid);

WE_INT32 EME_MailMsgPart_SetName(stEMSGHandle *pstEMsgHandle,
                                 void* pvstIn, 
                                 WE_CHAR* pcName);

WE_INT32 EME_MailMsgPart_SetFileName(stEMSGHandle *pstEMsgHandle,
                                     void* pvstIn, 
                                     WE_CHAR* pcFileName);

WE_INT32 EME_MailMsgPart_SetContType(stEMSGHandle *pstEMsgHandle,
                                     void* pvstIn,
                                     WE_CHAR* pcMimeType);

WE_INT32 EME_MailMsgPart_SetBodyData(stEMSGHandle *pstEMsgHandle,
                                     void* pvstIn,
                                     WE_CHAR* pcBodyPartData, 
                                     WE_UINT32 uiSize);

// WE_INT32 EME_MailMsgPart_GetDecodeBd(stEMSGHandle *pstEMsgHandle,
//                                      St_EmailMsgPart* pstIn, 
//                                      WE_CHAR** ppcRet);

// WE_CHAR* EME_MailMsg_DecodeCharset(stEMSGHandle *pstEMsgHandle,
//                                    WE_CHAR* pcSrc,
//                                    WE_CHAR* pcCharset);

WE_CHAR* EME_MailMsg_DecodeBody(stEMSGHandle *pstEMsgHandle,
                                WE_CHAR* pcSrc, 
                                WE_UINT32 uiOrigSize,
                                E_EncodeType eEncodeType,
                                WE_UINT32 *puiSize);

WE_INT32 EME_MailMsg_ParseLine(stEMSGHandle *pstEMsgHandle,
                               WE_CHAR* pcOut, 
                               WE_CHAR* pcLineStart, 
                               WE_CHAR* pcLineEnd);

/* additional function */
E_EncodeType EME_MailMsg_ParseTransEncoding(WE_CHAR* pcTransEncoding);
E_Charset EME_MailMsg_ParseChaset(WE_CHAR *pcBuf, WE_CHAR* pcCharset);


/***********************************************************************************
   Encoding & Decoding of Synchronism.
 ***********************************************************************************/
WE_INT32 EME_EncodingMailMsgEx(stEMSGHandle *pstEMsgHandle,void *pstIn, WE_CHAR *pcDir);

WE_INT32 EME_MailMsgPart_SetBodyDataEx(stEMSGHandle *pstEMsgHandle,
                                        void        *pstIn, 
                                        WE_CHAR     *pcBodyPartData, 
                                        WE_UINT32   uiSize);

WE_INT32 EME_MailMsg_ParseDataBufEx(stEMSGHandle *pstEMsgHandle,
                                     void        *pstIn, 
                                     WE_CHAR     *src, 
                                     WE_CHAR     *pcDir,
                                     WE_BOOL     bOnlyHeader);
WE_INT32 EME_MailMsg_ParseJustPartEx(stEMSGHandle *pstEsgHandle);

WE_INT32 EME_MailMsg_DecodeBodyStruct(stEMSGHandle *pstInHandle, WE_CHAR* pBodyStructBuf, WE_CHAR *pcPath);
#endif /* _EME_CODEC_MAILMESSAGE_H_ */