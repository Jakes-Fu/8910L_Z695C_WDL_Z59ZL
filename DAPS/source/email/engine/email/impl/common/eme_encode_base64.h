/*=====================================================================================
FILE NAME   : eme_encode_base64.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Supply api for base64 encode and decode.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     wangxiushan                  create eme_encode_base64.h
          
=====================================================================================*/

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/

#ifndef _EME_CODEC_ENCODE_BASE64_H
#define _EME_CODEC_ENCODE_BASE64_H

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
#include "eme_utils_api.h"
#include "we_def.h"


/* =============function declaration =============== */

/* lenSrc is the length of pSrc, if the pSrc is the string end with 0,
   could assign it to 0. bChng is judge whether is the changed base64,
   about changed base64 please refer to rfc2045. */
WE_INT32 EME_ToBase64(WE_CHAR *pDest, WE_CHAR *pSrc, WE_UINT32 lenSrc, WE_BOOL bChng);

WE_INT32 EME_FromBase64(WE_CHAR *pDest, WE_CHAR *pSrc, WE_UINT32 uiOrigSize);

/*get encoded string size from original string*/
WE_UINT32 EME_Base64EncodeSize(WE_UINT32 iSize);

/*get decoded string size from original string*/
WE_UINT32 EME_Base64DecodeSize(WE_UINT32 iSize);

/* bModify - check whether is modified utf-7, it is used in imap mailbox */
WE_INT32 EME_Utf7ToUcs2(const WE_CHAR* strIn, WE_UINT16* strOut, WE_BOOL bModify);
/* bModify - check whether is modified utf-7, it is used in imap mailbox */
WE_INT32 EME_Ucs2ToUtf7(const WE_UINT16* strIn, WE_CHAR* strOut, WE_BOOL bModify);



/* convert utf16 to utf8 */
WE_INT32 EME_Ucs2ToUtf8(const WE_UINT16* strIn, WE_UINT8* strOut);

/* convert utf8 to utf16 */
WE_INT32 EME_Utf8ToUcs2(const WE_UINT8* strIn, WE_UINT16* strOut);






#endif /* BASE64_H */
