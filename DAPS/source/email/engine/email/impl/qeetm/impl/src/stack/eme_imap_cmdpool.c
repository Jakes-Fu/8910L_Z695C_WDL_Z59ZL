/*=====================================================================================
    FILE: eme_imap_cmdpool.c
    MODULE NAME : IMAP  


    GENERAL DESCRIPTION:
    Implement of IMAP Stack Cmdpool

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#include "eme_IMAP_cmdpool.h"

typedef enum
{
   E_EME_IMAP4_CMD_MARK_SEEN,
   E_EME_IMAP4_CMD_UNMARK_SEEN,
   E_EME_IMAP4_CMD_MARK_DELETED,
   E_EME_IMAP4_CMD_UNMARK_DELETED,
   E_EME_IMAP_CMD_CHANGE_FLAG_RESERVE  =  0x7FFFFFFF
}E_IMAP4ChangeFlagType;


static WE_INT32 EME_IMAP_SendCMDChangeFlag(St_IMAPSocket* hSocketHandle, 
                                          WE_UINT32 uiUID,
                                          E_IMAP4ChangeFlagType eChangeFlagType,
                                          FUNCREADABLE pfnCallback);

static WE_INT32 EME_IMAP_SendCMDHandleMailbox(St_IMAPSocket* hSocketHandle, 
                                              WE_CHAR* pcMailboxName,
                                              WE_UINT32 eHandleMailboxType,
                                              FUNCREADABLE pfnCallback);

WE_INT32 EME_IMAP_SendCMDLogin(St_IMAPSocket* hSocketHandle,
                               WE_CHAR* pcUserName, 
                               WE_CHAR* pcPassword,
                               FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pcUserName || NULL == pcPassword || NULL == pfnCallback)
      return EME_INVALID_DATA;
      
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s LOGIN \"%s\" \"%s\"\r\n", EME_IMAP_CMD_TAG, pcUserName, pcPassword);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);

   return iRet;
}


WE_INT32 EME_IMAP_SendCMDLogout(St_IMAPSocket* hSocketHandle, FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s LOGOUT\r\n", EME_IMAP_CMD_TAG);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDSelect(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pcMailboxName || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s SELECT \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDUIDSearch(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiStartUID,
                                   WE_UINT32 uiEndUID,
                                   FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   //EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID SEARCH %d:%d\r\n", EME_IMAP_CMD_TAG, uiStartUID, uiEndUID);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID SEARCH %d:*\r\n", EME_IMAP_CMD_TAG, uiStartUID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDFetchSize(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiStartUID, 
                                   WE_UINT32 uiEndUID,
                                   FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID FETCH %d:%d (UID RFC822.SIZE)\r\n", EME_IMAP_CMD_TAG, uiStartUID, uiEndUID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDFetchHeader(St_IMAPSocket* hSocketHandle, 
                                     WE_UINT32 uiUID,
                                     FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID FETCH %d:%d (UID BODY.PEEK[HEADER])\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDBodystructure(St_IMAPSocket* hSocketHandle, 
                                       WE_UINT32 uiUID,
                                       FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;

   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID FETCH %d:%d (BODYSTRUCTURE)\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDFetchMail(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiUID,
                                   FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID FETCH %d:%d (BODY.PEEK[])\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDFetchPart(St_IMAPSocket* hSocketHandle, 
                                   WE_UINT32 uiUID, 
                                   WE_CHAR* pcSectionID,
                                   FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID FETCH %d:%d (BODY.PEEK[%s])\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID, pcSectionID);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDMarkSeen(St_IMAPSocket* hSocketHandle, 
                                  WE_UINT32 uiUID,
                                  FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDChangeFlag(hSocketHandle, uiUID, E_EME_IMAP4_CMD_MARK_SEEN, pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDUnmarkSeen(St_IMAPSocket* hSocketHandle, 
                                    WE_UINT32 uiUID,
                                    FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDChangeFlag(hSocketHandle, uiUID, E_EME_IMAP4_CMD_UNMARK_SEEN, pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDMarkDeleted(St_IMAPSocket* hSocketHandle, 
                                     WE_UINT32 uiUID,
                                     FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDChangeFlag(hSocketHandle, uiUID, E_EME_IMAP4_CMD_MARK_DELETED, pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDUnmarkDeleted(St_IMAPSocket* hSocketHandle, 
                                       WE_UINT32 uiUID,
                                       FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDChangeFlag(hSocketHandle, uiUID, E_EME_IMAP4_CMD_UNMARK_DELETED, pfnCallback);   
}


static WE_INT32 EME_IMAP_SendCMDChangeFlag(St_IMAPSocket* hSocketHandle, 
                                           WE_UINT32 uiUID,
                                           E_IMAP4ChangeFlagType eChangeFlagType,
                                           FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);

   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   switch(eChangeFlagType)
   {
   case E_EME_IMAP4_CMD_MARK_SEEN:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID STORE %d:%d +FLAGS (\\SEEN)\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
      }
   	  break;
   case E_EME_IMAP4_CMD_UNMARK_SEEN:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID STORE %d:%d -FLAGS (\\SEEN)\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
      }
      break;
   case E_EME_IMAP4_CMD_MARK_DELETED:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID STORE %d:%d +FLAGS (\\DELETED)\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
      }
      break;
   case E_EME_IMAP4_CMD_UNMARK_DELETED:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID STORE %d:%d -FLAGS (\\DELETED)\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID);
      }
      break;
   default:
       break;
   }
   
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDCopy(St_IMAPSocket* hSocketHandle, 
                              WE_UINT32 uiUID,
                              WE_CHAR* pcMailboxName,
                              FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pcMailboxName || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   
   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UID COPY %d:%d \"%s\"\r\n", EME_IMAP_CMD_TAG, uiUID, uiUID, pcMailboxName);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDAppend(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                WE_UINT32 uiSize,
                                FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pcMailboxName || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   
   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s APPEND \"%s\" {%d}\r\n", EME_IMAP_CMD_TAG, pcMailboxName, uiSize);
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDCreate(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle, 
                                        pcMailboxName,
                                        E_EME_IMAP_OPTYPE_CREATE,
                                        pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDRename(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                WE_CHAR* pcMailboxNameRename,
                                FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pcMailboxName || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   
   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s RENAME \"%s\" \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName, pcMailboxNameRename); 
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDDelete(St_IMAPSocket* hSocketHandle,
                                WE_CHAR* pcMailboxName,
                                FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle, 
                                        pcMailboxName,
                                        E_EME_IMAP_OPTYPE_DELETE,
                                        pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDSubscribe(St_IMAPSocket* hSocketHandle,
                                   WE_CHAR* pcMailboxName,
                                   FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle, 
                                        pcMailboxName,
                                        E_EME_IMAP_OPTYPE_SUBSCRIBE,
                                        pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDUnsubscribe(St_IMAPSocket* hSocketHandle,
                                     WE_CHAR* pcMailboxName,
                                     FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle, 
                                        pcMailboxName,
                                        E_EME_IMAP_OPTYPE_UNSUBSCRIBE,
                                        pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDExpung(St_IMAPSocket* hSocketHandle,
                                FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle ||NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   
   EME_UTILS_MEMSET(hSocketHandle->acCMD, 0, EME_IMAP_MAX_CMD_LENGTH);
   EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s EXPUNGE\r\n", EME_IMAP_CMD_TAG); 
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}


WE_INT32 EME_IMAP_SendCMDList(St_IMAPSocket* hSocketHandle,
                              FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle,
                                        NULL,
                                        E_EME_IMAP_OPTYPE_LIST,
                                        pfnCallback);
}


WE_INT32 EME_IMAP_SendCMDLsub(St_IMAPSocket* hSocketHandle,
                              FUNCREADABLE pfnCallback)
{
   return EME_IMAP_SendCMDHandleMailbox(hSocketHandle, 
                                        NULL,
                                        E_EME_IMAP_OPTYPE_LSUB,
                                        pfnCallback);
}


static WE_INT32 EME_IMAP_SendCMDHandleMailbox(St_IMAPSocket* hSocketHandle, 
                                              WE_CHAR* pcMailboxName,
                                              WE_UINT32 eHandleMailboxType,
                                              FUNCREADABLE pfnCallback)
{
   WE_UINT32 iRet = EME_SUCCESS;
   if (NULL == hSocketHandle || NULL == pfnCallback)
      return EME_INVALID_DATA;
   
   EME_Socket_RegCbReadable(hSocketHandle->hSocket, (WE_VOID*)pfnCallback);
   
   
   if (NULL == pcMailboxName
       && E_EME_IMAP_OPTYPE_LIST != eHandleMailboxType
       && E_EME_IMAP_OPTYPE_LSUB != eHandleMailboxType)
      return EME_INVALID_DATA;

   switch(eHandleMailboxType)
   {
   case E_EME_IMAP_OPTYPE_CREATE:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s CREATE \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName); 
         break;
      }
   case E_EME_IMAP_OPTYPE_DELETE:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s DELETE \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName);     
         break;
      }
   case E_EME_IMAP_OPTYPE_SUBSCRIBE:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s SUBSCRIBE \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName);    
         break;
      }
   case E_EME_IMAP_OPTYPE_UNSUBSCRIBE:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s UNSUBSCRIBE \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName);
         break;
      }
   case E_EME_IMAP_OPTYPE_LIST:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s LIST \"\" *\r\n", EME_IMAP_CMD_TAG);
         break;
      }
   case E_EME_IMAP_OPTYPE_LSUB:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s LSUB \"\" *\r\n", EME_IMAP_CMD_TAG);
         break;
      }
   case E_EME_IMAP_OPTYPE_EXPUNGMSG:
      {
         EME_UTILS_SPRINTF(hSocketHandle->acCMD, "%s EXPUNGE \"%s\"\r\n", EME_IMAP_CMD_TAG, pcMailboxName);
         break;
      }
   default:
      break;
   }
   
   iRet = EME_Socket_Write(hSocketHandle->hSocket, hSocketHandle->acCMD, EME_UTILS_STRLEN(hSocketHandle->acCMD), TRUE);
   
   return iRet;
}