/*==============================================================================
    FILE NAME   : eme_msgstore_api.h
    MODULE NAME : 


    GENERAL DESCRIPTION:
        This file contain the implementation for index the messages.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-26   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_MSGSTORE_API_H
#define EME_MSGSTORE_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_multipartmsg_api.h"
#include "eme_dataaccess_api.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
#define TEXTPLAIN_EXT      "txt"
#define TEXTHTML_EXT       "html"

#define EME_MSGID_MAX_NUM       0xFFF

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgStore_SaveMsgHeader(WE_VOID *pvMemMgr, 
                                    FILEMGR_HANDLE hMgr, 
                                    HMultiPartMsg hMsg, 
                                    WE_UINT32 dwMsgId);
WE_INT32 EME_MsgStore_SaveMsgPart(WE_VOID *pvMemMgr, 
                                  FILEMGR_HANDLE hMgr, 
                                  HMsgPart hMsgPart, 
                                  WE_UINT32 dwMsgId);
WE_INT32 EME_MsgStore_SaveMsg(WE_VOID *pvMemMgr, 
                              FILEMGR_HANDLE hMgr, 
                              HMultiPartMsg hMsg, 
                              WE_BOOL bIsNew, 
                              WE_BOOL bClean);
WE_INT32 EME_MsgStore_WindUp(WE_VOID *pvMemMgr, 
                             FILEMGR_HANDLE hMgr, 
                             HMultiPartMsg hMsg);

WE_INT32 EME_MsgStore_GetMsgHeader(WE_VOID *pvMemMgr, 
                                   FILEMGR_HANDLE hMgr, 
                                   WE_UINT32 dwMsgId, 
                                   HMultiPartMsg hMsg);
WE_UINT32 EME_MsgStore_GetMsgPartNum(FILEMGR_HANDLE hMgr, 
                                     WE_UINT32 dwMsgId);
WE_INT32 EME_MsgStore_GetMsgPart(WE_VOID *pvMemMgr, 
                                 FILEMGR_HANDLE hMgr, 
                                 WE_UINT32 dwMsgId, 
                                 WE_UINT32 dwPartId, 
                                 HMsgPart hMsgPart);
WE_INT32 EME_MsgStore_GetMsg(WE_VOID *pvMemMgr, 
                             FILEMGR_HANDLE hMgr, 
                             WE_UINT32 dwMsgId, 
                             HMultiPartMsg hMsg);

WE_INT32 EME_MsgStore_RemoveMsg(FILEMGR_HANDLE hMgr, WE_UINT32 dwMsgId);

WE_INT32 EME_MsgStore_GenerateMsgPath(WE_UINT32 dwMsgId, WE_CHAR *pcMsgPath, WE_CHAR *pcMsgFile);

WE_INT32 EME_MsgStore_NewMsg(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwMsgId);

WE_UINT32 EME_MsgStore_DetermMsgId(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 uiMsgId);

WE_UINT32 EME_MsgStore_DetermSavePlace(FILE_HANDLE hMgr);
/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
#endif /* EME_MSGSTORE_API_H */