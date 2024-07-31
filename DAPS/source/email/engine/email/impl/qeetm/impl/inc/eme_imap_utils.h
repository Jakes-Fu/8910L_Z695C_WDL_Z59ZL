/*=====================================================================================
    FILE: eme_imap_utils.h
    MODULE NAME : IMAP


    GENERAL DESCRIPTION:
    Declaration of IMAP Stack utils functions

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================    
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/

#ifndef __EME_IMAP_UTILS_H__
#define __EME_IMAP_UTILS_H__

#include "eme_imap_def.h"

/******************************************************************************
                     Operate uid file
******************************************************************************/

/* used to add uid item to uid buffer in memory. If item has already exists(judge 
   pcMailboxName), set uiMaxUID the max one. 
   note: pstUIDBuf should provided but not a NULL pointer.
         pstUIDBuf->pMemHdr should has been initialized if need memory check. 
         pstUIDBuf->ppstUIDItem should be NULL and pstUIDBuf->uiItemNum should be
         0 if it is first time to add uid item */
WE_INT32 EME_IMAP_AddToUIDBuffer(St_UIDBuf* pstUIDBuf, St_UIDItem* pstUIDItem);

/* get max UID from pstUIDBuf according pcMailboxName */
WE_INT32 EME_IMAP_GetMaxUID(St_UIDBuf* pstUIDBuf, WE_CHAR* pcMailboxName, WE_UINT32* uiMaxUID);

/* used when list mailboxs */
WE_INT32 EME_IMAP_InvalideAll(St_UIDBuf* pstUIDBuf);

/* used when delete mailbox */
WE_INT32 EME_IMAP_InvalideMailbox(St_UIDBuf* pstUIDBuf, WE_CHAR* pcMailboxName);

/* used when rename mailbox */
WE_INT32 EME_IMAP_RenameMailbox(St_UIDBuf* pstUIDBuf, WE_CHAR* pcOrginName, WE_CHAR* pcDestName);

/* used to read uid file to memory and organize it to a list(St_UIDBuf is the 
   handle, ppstUIDItem is the actual list header, St_UIDItem is node).
   note: pstUIDBuf should provided but not a NULL pointer.
         pstUIDBuf should not NULL and pstUIDBuf->pMemHdr should has been 
         initialized if need memory check. pstUIDBuf->ppstUIDItem should be NULL.*/
WE_INT32 EME_IMAP_ReadUIDFileToBuffer(St_UIDBuf* pstUIDBuf, WE_CHAR* pcUIDFilePath);


/* used to write list(which handle is St_UIDBuf) to a specific file. 
   note: pstUIDBuf->pMemHdr should has been initialized if need memory check.*/
WE_INT32 EME_IMAP_WriteUIDBufferToFile( St_UIDBuf* pstUIDBuf, WE_CHAR* pcUIDFilePath);


/* free all memory allocated to uid buffer execept St_UIDBuf
   note: St_UIDBuf will not free */
WE_INT32 EME_IMAP_FreeUIDBuf(St_UIDBuf* ppstUIDBuf);


/******************************************************************************
                     Operate mails's infomation array
******************************************************************************/

/* note: pstMailInfoArray's members should be all 0 except pMemHdr if it is first time to add */
WE_INT32 EME_IMAP_AddToMailInfoArray(St_MailInfoArray* pstMailInfoArray, St_MailInfo stMailInfo,WE_UINT32 uiNumberLimit, WE_INT32 iReceLast);

/* This function will increase pstMailInfoArray.uiIndex. */
WE_INT32 EME_IMAP_GetUIDFromArray(St_MailInfoArray* pstMailInfoArray, WE_UINT32* puiUID);

WE_INT32 EME_IMAP_GetSizeFromArray(St_MailInfoArray* pstMailInfoArray, WE_UINT32 uiUID, WE_UINT32* puiSize);

/* free all memory that St_MailInfoArray contain except St_MailInfoArray itself 
   note: St_MailInfoArray will not be freed */
WE_INT32 EME_IMAP_ReleaseMailInfoArray(St_MailInfoArray* pstMailInfoArray);


/******************************************************************************
                     Operate long string
******************************************************************************/

/* used to add string to St_LongString in memory to form a long string.  
   note: St_LongString should provided but not a NULL pointer.
         pstLongString->pMemHdr should has been initialized if need memory check. 
         pstLongString->pcLongString should be NULL and pstLongString->uiLength should be
         0 if it is first time to add string */
WE_INT32 EME_IMAP_AddToLongString(St_LongString* pstLongString, WE_CHAR* pcString, WE_BOOL bFile);

/* free all memory allocated to long string execept St_LongString
   note: St_LongString will not free */
WE_CHAR* EME_IMAP_GetLongString(St_LongString* pstLongString);


/******************************************************************************
                     other utils
******************************************************************************/


/* note: this function is not the same as realloc will perform */
WE_INT32 EME_IMAP_ReallocMemory(WE_VOID **ppvOrginPointer, WE_UINT32 uiNewSize, WE_VOID* pMemMgr);

/* used to find substring of pcInString + (*puiIndex).
   The substring is located between cLeftChar and cRightChar that the string
   (pcInString + (*puiIndex)) first meet when scan from left to right.
   *puiIndex is changed when substring was found, and (pcInString + (*puiIndex)) 
   is point to the substring's right(exactly, right char of cRightChar). 
   so if pcInString is "hello,world!", and cLeftChar is ',' , cRightChar is '!',
   pcOutString will be "world", and *puiIndex is 12.
   Note: dest and src param should differ. */
WE_INT32 EME_IMAP_StrGetSubString(WE_CHAR* pcInString, 
                             WE_CHAR  cLeftChar,
                             WE_CHAR  cRightChar,
                             WE_CHAR* pcOutString,
                             WE_UINT32* puiIndex);

WE_INT32 EME_IMAP_StripLastLine(WE_CHAR* pcString);

WE_INT32 EME_IMAP_StringCat(WE_CHAR **ppcOrginString, WE_CHAR* pcAddedString, WE_VOID* pMemMgr);

WE_INT32 EME_IMAP_Utf8ToUtf7(WE_CHAR* pcUtf8, WE_CHAR* pcUtf7, WE_VOID* pvMemMgr);

WE_INT32 EME_IMAP_Utf7ToUtf8(WE_CHAR* pcUtf7, WE_CHAR* pcUtf8, WE_VOID* pvMemMgr);

#endif /* __EME_IMAP_UTILS_H__ */

