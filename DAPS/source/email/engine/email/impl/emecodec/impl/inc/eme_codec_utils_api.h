/*=====================================================================================
FILE NAME   : eme_codec_utils_api.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File is the header file of eme_codec_utils.c.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
        20070305     wxs                         create
        20070429     wxs                         improved
                 
=====================================================================================*/


/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_UTILS_API_H
#define _EME_CODEC_UTILS_API_H


/************************************************************************/
/*      include   files                                               */
/************************************************************************/
#include "we_def.h"
#include "eme_error.h"


/* ================function declaration=================== */
WE_INT32 EME_FreeMsgPartList(WE_VOID* pvMemHdr, St_MsgPartList *pstPartList);

WE_INT32 EME_DecodeMsg_JustHd(stEMSGHandle *pstInHandle,
                              WE_CHAR* pcEmailBuff,
                              WE_BOOL bOnlyHasHd);

WE_INT32 EME_DecodeMsg_JustPart(stEMSGHandle *pstInHandle,
                                EmailOpt *pstOpt,
                                WE_CHAR *pcPartBuf);

WE_INT32 EME_DecodeMsgPart(stEMSGHandle *pstInHandle,
                           WE_CHAR* pMsgPartBuf,
                           WE_CHAR* charset, 
                           WE_CHAR* transerEncoding);

WE_INT32 EME_DecodeBodyStruct(stEMSGHandle *pstInHandle, 
                              WE_CHAR* pBodystructureBuf);

WE_INT32 EME_DecodeEmailMsg(stEMSGHandle *pstInHandle, 
                            WE_CHAR* pcWholeMail);

WE_INT32 EME_SetAddrField(stEMSGHandle *pstInHandle, 
                            WE_CHAR* pcAddrList,
                            WE_INT32 iEmailOptID);

WE_INT32 EME_SetHeader(stEMSGHandle *pstInHandle); 

WE_INT32 EME_SetOneBodyPart(stEMSGHandle *pstInHandle, 
                            WE_UINT32 uiIndex);

// WE_INT32 EME_GetMsgOpts(EmailOpt *pstOpts,
//                         WE_INT32 iOptId, 
//                         EmailOpt *pstOptOut);


WE_INT32 EME_FreeAllOptsValue(WE_VOID *pvMemHdr ,EmailOpt *pstFreeOpts);
EmailOpt* EME_CopyOpts(WE_VOID* pvMemHdr, EmailOpt *pstOrigOpts);

WE_INT32 EME_GetOptsByIndex(EmailOpt *pstAllOpts, 
                            WE_INT32 iOptId,
                            WE_INT32 iIndex, 
                            EmailOpt *pstOpt);

WE_INT32 EME_UpdateOpt(EmailOpt *pstAllOpts, WE_INT32 iId, WE_VOID* pvVal, WE_VOID** ppvOrigVal);

//WE_INT32  EME_ToJulianDate(WE_UINT32 dwSecs,
//                           St_JulianDateType *pDate);

//WE_UINT32 EME_ToSeconds(St_JulianDateType *pDate);


WE_INT32 EME_GetHdFieldValue(stEMSGHandle *pstInHandle, 
                             WE_CHAR *pcIn,
                             WE_CHAR** ppcOut);

WE_CHAR* EME_GetTransEncoding(void *pvMemMgr, E_EncodeType eEncondingType);
WE_CHAR* EME_GetCharSet(void *pvMemMgr, E_Charset eCharset);

WE_INT32 EME_Utf8ToXX(void* pICSC, 
                      void *pvMemMgr, 
                      WE_CHAR *pcInBuf, 
                      WE_UINT32 uiInSize,
                      E_Charset eCharset, 
                      WE_VOID** ppvOutBuf,
                      WE_UINT32 *puiOutSize);
WE_INT32 EME_XXToUtf8(void* pICSC, 
                      void *pvMemMgr, 
                      WE_VOID *pcInBuf, 
                      WE_UINT32 uiInSize,
                      E_Charset eCharset, 
                      WE_CHAR** ppcOutBuf,
                      WE_UINT32 *puiOutSize);

/*****************************************************************************/
/*****************************************************************************/
WE_INT32 EME_SetOneBodyPartEx(stEMSGHandle *pstInHandle, WE_UINT32 uiIndex);
WE_INT32 EME_DecodeEmailMsgEx(stEMSGHandle *pstInHandle, WE_CHAR* pcWholeMail, WE_CHAR *pcDir);
WE_INT32 EME_DecodeMsg_JustHdEx(stEMSGHandle *pstInHandle, WE_CHAR* pcEmailBuff, WE_CHAR *pcDir, WE_BOOL bOnlyHasHd);
WE_INT32 EME_DecodeMsg_JustPartEx(stEMSGHandle *pstInHandle,EmailOpt *pstOpt, WE_CHAR *pcTmpFile, WE_CHAR *pcDir);
WE_INT32 EME_DecodeBodyStructEx(stEMSGHandle *pstInHandle, WE_CHAR* pBodyStructBuf, WE_CHAR *pcPath);

WE_INT32 EME_ResetOneSegment(WE_CHAR *pcSrc, WE_INT32 iTransEnc, WE_UINT32 *puiNewSize);
/*===========================================================================
 INTERFACES DOCUMENTATION  (Hubin 20070423 added)
===========================================================================
   EME_CreateMsgPartList()
Description:
   This Function create a default configed message body
Prototype:
   WE_INT32 EME_CreateMsgPartList(stEMSGHandle *pstEMsgHandle,void **ppVoid)
Parameters:
   pstEMsgHandle:The message handler
   ppVoid:The created default message body point returned
Return Value:
   Operation state
Side Effects:
   None

   EME_FreeOneMsgPart()
Description:
   This Function frees the memory belonging to
   the designated message body(not including the body itself)
Prototype:
   WE_INT32 EME_FreeOneMsgPart(stEMSGHandle *pstEMsgHandle,St_MsgPartList* pMsgPartList)
Parameters:
   pstEMsgHandle:The message handler
   pMsgPartList:The message body point
Return Value:
   Operation state
Side Effects:
   None

   EME_AppendMsgPart()
Description:
   This Function append the designated body to the body list
Prototype:
   WE_INT32 EME_AppendMsgPart(St_MsgPartList** ppTransDataHeader,
                              St_MsgPartList** ppMsgPartListTail,
                              St_MsgPartList* pAppendTransData)
Parameters:
   ppTransDataHeader:The body list header
   ppMsgPartListTail:The body list tail
   pAppendTransData:The body point want to append to the body list
Return Value:
   Operation state
Side Effects:
   None

   EME_FreeHeader()
Description:
   This Function frees the memory belonging to the header structure(not including the header itself)
Prototype:
   WE_INT32 EME_FreeHeader(stEMSGHandle *pstInHandle)
Parameters:
   pstInHandle:The header point
Return Value:
   Operation state
Side Effects:
   None

   EME_FreeMsgPartList()
Description:
   This Function frees the whole message body list
Prototype:
   WE_INT32 EME_FreeMsgPartList(stEMSGHandle *pstInHandle)
Parameters:
   pstInHandle:The body list
Return Value:
   Operation state
Side Effects:
   None

   EME_DecodeMsg_JustHd()
Description:
   This Function decode the header
Prototype:
   WE_INT32 EME_DecodeMsg_JustHd(stEMSGHandle *pstInHandle, WE_CHAR* pcEmailBuff,WE_BOOL bOnlyHasHd)
Parameters:
   pstInHandle:The message handler
   pcEmailBuff:Buffer want to be processed
   bOnlyHasHd:If only has the header
Return Value:
   Operation state
Side Effects:
   None

   EME_DecodeMsg_JustPart()
Description:
   This Function decode the body
Prototype:
   WE_INT32 EME_DecodeMsg_JustPart(stEMSGHandle *pstInHandle,EmailOpt *pstOpt,WE_CHAR *pcPartBuf)
Parameters:
   pstInHandle:The message handler
   pstOpt:Body data opts wants to be processed
   pcPartBuf:Data buffer want to be processed
Return Value:
   Operation state
Side Effects:
   None

   EME_DecodeMsgPart()
Description:
   This Function decode the body
Prototype:
   WE_INT32 EME_DecodeMsgPart(stEMSGHandle *pstInHandle,
                              WE_CHAR* pMsgPartBuf,
                              WE_CHAR* charset, 
                              WE_CHAR* transerEncoding)
Parameters:
   pstInHandle:The message handler
   pMsgPartBuf:Data buffer want to be processed
   charset:Charset
   transerEncoding:Encoding type
Return Value:
   Operation state
Side Effects:
   None

   EME_DecodeBodyStruct()
Description:
   This Function decode the body structure
Prototype:
   WE_INT32 EME_DecodeBodyStruct(stEMSGHandle *pstInHandle, WE_CHAR* pBodystructureBuf)
Parameters:
   pstInHandle:The message handler
   pBodystructureBuf:Body structure buffer want to be processed
Return Value:
   Operation state
Side Effects:
   None

   EME_DecodeEmailMsg()
Description:
   This Function decode the whole mail
Prototype:
   WE_INT32 EME_DecodeEmailMsg(stEMSGHandle *pstInHandle, WE_CHAR* pWholeMailBuf)
Parameters:
   pstInHandle:The message handler
   pWholeMailBuf:Whole mail data buffer want to be processed
Return Value:
   Operation state
Side Effects:
   None

   EME_SetAddrField()
Description:
   This Function set the header's field characters
Prototype:
   WE_INT32 EME_SetAddrField(stEMSGHandle *pstInHandle, WE_CHAR* field, WE_INT32 iOpt)
Parameters:
   pstInHandle:The message handler
   field:Field data buffer
   iOpt:Opt id
Return Value:
   Operation state
Side Effects:
   None

   EME_SetHeader()
Description:
   This Function set the header's default config
Prototype:
   WE_INT32 EME_SetHeader(stEMSGHandle *pstInHandle)
Parameters:
   pstInHandle:The message handler
Return Value:
   Operation state
Side Effects:
   None

   EME_SetOneBodyPart()
Description:
   This Function set the body's config
Prototype:
   WE_INT32 EME_SetOneBodyPart(stEMSGHandle *pstInHandle, WE_UINT32 uiIndex)
Parameters:
   pstInHandle:The message handler
   uiIndex:Body index number
Return Value:
   Operation state
Side Effects:
   None

   EME_GetMsgOpts()
Description:
   This Function set the body's config
Prototype:
   WE_INT32 EME_GetMsgOpts(EmailOpt *pstOpts, WE_INT32 iOptId, EmailOpt *opt)
Parameters:
   pstInHandle:The message handler
   iOptId:Opt id
   pstOptOut:Opts point returned
Return Value:
   Operation state
Side Effects:
   None

   EME_FreeAllOpts()
Description:
   This Function set the body's config
Prototype:
   WE_INT32 EME_FreeAllOpts(stEMSGHandle *pstEMsgHandle,EmailOpt *pstOpts, WE_UINT32 uiOptsNum)
Parameters:
   pstInHandle:The message handler
   iOptId:Opt id
   opt:Opts point returned
Return Value:
   Operation state
Side Effects:
   None



===========================================================================*/

#endif