/*=====================================================================================
    FILE: eme_imap_cmdpool.h
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Declaration of IMAP Stack cmdpool functions

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================  
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/

#ifndef __EME_IMAP_CMDPOOL_H__
#define __EME_IMAP_CMDPOOL_H__



#include "eme_imap_def.h"

#include "eme_socket_api.h"

#define EME_IMAP_CMD_TAG  "-tfMail-"

WE_INT32 EME_IMAP_SendCMDLogin(St_IMAPSocket* hSocketHandle,
                               WE_CHAR* pcUserName,
                               WE_CHAR* pcPassword,
                               FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDLogout(St_IMAPSocket* hSocketHandle, FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDSelect(St_IMAPSocket* hSocketHandle, 
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDUIDSearch(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiStartUID,
                                   WE_UINT32 uiEndUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDFetchSize(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiStartUID, 
                                   WE_UINT32 uiEndUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDFetchHeader(St_IMAPSocket* hSocketHandle, 
                                     WE_UINT32 uiUID,
                                     FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDBodystructure(St_IMAPSocket* hSocketHandle, 
                                       WE_UINT32 uiUID,
                                       FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDFetchMail(St_IMAPSocket* hSocketHandle,
                                   WE_UINT32 uiUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDFetchPart(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiUID, 
                                   WE_CHAR* pcSectionID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDMarkSeen(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDUnmarkSeen(St_IMAPSocket* hSocketHandle, 
                                  WE_UINT32 uiUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDMarkDeleted(St_IMAPSocket* hSocketHandle, 
                                  WE_UINT32 uiUID,
                                   FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDUnmarkDeleted(St_IMAPSocket* hSocketHandle, 
                                       WE_UINT32 uiUID,
                                       FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDCopy(St_IMAPSocket* hSocketHandle, 
                              WE_UINT32 uiUID,
                              WE_CHAR* pcMailboxName,
                              FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDAppend(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                WE_UINT32 uiSize,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDCreate(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDRename(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                WE_CHAR* pcMailboxNameRename,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDDelete(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDSubscribe(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDUnsubscribe(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDExpung(St_IMAPSocket* hSocketHandle, FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDList(St_IMAPSocket* hSocketHandle,
                              FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDLsub(St_IMAPSocket* hSocketHandle,
                              FUNCREADABLE pfnCallback);
#endif /* __EME_IMAP_CMDPOOL_H__ */