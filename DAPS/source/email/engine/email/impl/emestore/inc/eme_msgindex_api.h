/*==============================================================================
    FILE NAME   : eme_msgindex_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        This file contain some API for store and get message.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-26   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_MSGINDEX_API_H
#define EME_MSGINDEX_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_multipartmsg_api.h"
#include "eme_dataaccess_api.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgIndex_IndexMsg(WE_VOID *pvMemMgr, 
                               FILEMGR_HANDLE hMgr, 
                               HMultiPartMsg hMsg, 
                               WE_BOOL bIsNew);

WE_INT32 EME_MsgIndex_GetMsgIndexInfo(FILEMGR_HANDLE hMgr, 
                                      WE_UINT32 dwMsgId, 
                                      HMultiPartMsg hMsg);

WE_INT32 EME_MsgIndex_MoveMsg(WE_VOID *pvMemMgr, 
                              FILEMGR_HANDLE hMgr, 
                              WE_UINT32 dwActId, 
                              WE_UINT32 dwSrcBoxId, 
                              WE_UINT32 dwDestBoxId, 
                              WE_UINT32 dwMsgId);

WE_INT32 EME_MsgIndex_MoveMsgEx(WE_VOID *pvMemMgr, 
                                FILEMGR_HANDLE hMgr, 
                                WE_UINT32 dwActId, 
                                WE_UINT32 dwSrcBoxId, 
                                WE_UINT32 dwDestBoxId, 
                                WE_UINT32 *pdwMsgId,
                                WE_UINT32 dwMsgNum);

WE_INT32 EME_MsgIndex_RemoveMsg(WE_VOID *pvMemMgr, 
                                FILEMGR_HANDLE hMgr, 
                                WE_UINT32 dwMsgId);

WE_INT32 EME_MsgIndex_RemoveMsgEx(WE_VOID *pvMemMgr, 
                                  FILEMGR_HANDLE hMgr, 
                                  WE_UINT32 *pdwMsgId,
                                  WE_UINT32 dwMsgNum);

WE_INT32 EME_MsgIndex_BoxNameToId(FILEMGR_HANDLE hMgr, 
                                  WE_UINT16 *pwszBoxName, 
                                  WE_UINT32 dwActId, 
                                  WE_UINT32 *pdwBoxId);

WE_INT32 EME_MsgIndex_GetMsgInfoNum(FILEMGR_HANDLE hMgr, 
                                    WE_UINT32 dwActId, 
                                    WE_UINT32 dwBoxId, 
                                    WE_UINT32 *pdwNum);

WE_INT32 EME_MsgIndex_CreateEmptyMsgIndex(FILEMGR_HANDLE hMgr, 
                                          WE_UINT32 dwActId);

WE_INT32 EME_MsgIndex_GetMsgInfo(FILEMGR_HANDLE hMgr, 
                                 WE_UINT32 dwActId, 
                                 WE_UINT32 dwBoxId, 
                                 WE_UINT32 dwNum, 
                                 EMS_MsgInfo *pastMsgInfo);

WE_INT32 EME_MsgIndex_SyncMsgIndexInfo(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId);

WE_INT32 EME_MsgIndex_SearchMsgInfo(FILEMGR_HANDLE hMgr, 
                                    WE_UINT32      dwActId,
                                    WE_UINT32      dwBoxId, 
                                    WE_CHAR        *pcFileter, 
                                    EMS_MsgInfo    *pstMsgInfo,
                                    WE_UINT32      *pdwNum
                                    );

WE_INT32 EME_MsgIndex_RecoverMsg(WE_VOID *pvMemMgr, 
                                 FILEMGR_HANDLE hMgr,
                                 WE_UINT32 dwActId,
                                 WE_UINT32 *pdwMsgId,
                                 WE_UINT32 dwMsgNum);

WE_INT32 EME_MsgIndex_CleanMsg(WE_VOID *pvMemMgr, 
                               FILEMGR_HANDLE hMgr,
                               WE_UINT32 dwActId,
                               WE_UINT32 dwBoxId,
                               WE_UINT32 dwSaveplace);
/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
#endif /* EME_MSGINDEX_API_H */