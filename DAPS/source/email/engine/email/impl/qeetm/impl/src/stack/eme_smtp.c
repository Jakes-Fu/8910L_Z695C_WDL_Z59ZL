/*=====================================================================================
    FILE: eme_smtp.c
    MODULE NAME : SMTP

  
    GENERAL DESCRIPTION:
    Implement of smtp Stack
 
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
======================================================================================= 
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/05/2007        EchoChen                         creat this file
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/

/*===================================================================
 * 
 *                     Include Files
 * 
====================================================================*/
/*Include System head file*/
/*Include Program Global head file*/

/*Include Module head file*/
#include "eme_smtp_priv.h"
#include "eme_error.h"
#include "eme_utils_api.h"
#include "eme_smtp_api.h"
#include "eme_tm_api.h"
/*Include Private head file*/


/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/


/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/


/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/

/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/




/*===================================================================
* 
*                    Functions
* 
* ==================================================================*/

/*interface fuction*/
WE_INT32 EME_SMTP_InitHandle
(
 ISMTPHandle* ppHandle,
 WE_VOID* pvMemMgr,
 WE_VOID* pvIShell,
 WE_VOID* pvAsyncMgr,
 WE_VOID* pvFileMgr,
 WE_VOID* pvNetMgr
 )
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = NULL;

   /* check input parameters */
   if (NULL == ppHandle)
   {
      return EME_INVALID_DATA;
   }
   /* init SMTP handle operation */
   do 
   {
      /* malloc memory for SMTP handle*/
      po = EME_UTILS_MALLOC(pvMemMgr,sizeof(St_SMTPHandle));
      EME_UTILS_BREAKNULL(po,
                          iRet,
                          EME_NOMEMORY,
                          "EME_SMTP_InitHandle,memory allocate failed!");

      EME_UTILS_MEMSET(po,0,sizeof(St_SMTPHandle));

      po->pvMemMgr = pvMemMgr;
      po->pvIShell = pvIShell;
      po->pvFileMgr = pvFileMgr;
      po->pvNetMgr = pvNetMgr;
      po->pvAsyncMgr = pvAsyncMgr;
      po->uiActType = SMTP_ACTTYPE_NONE;
      *ppHandle = po;
      
   } while(0);
   
   return iRet;
}


WE_INT32 EME_SMTP_SetCallBack(ISMTPHandle pHandle,ActionCB pActCB,void* pvPrivData)
{
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   /* check input parameters */
   if (NULL == po || NULL == pActCB || NULL == pvPrivData)
   {
      return EME_INVALID_DATA;
   }
   po->pCB = pActCB;
   po->pCBCx = pvPrivData;
   
   return EME_SUCCESS;
}

WE_INT32 EME_SMTP_Open(ISMTPHandle pHandle,St_SMTPConfigInfo* pstConfigInfo)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   /* check input parameters */
   if (NULL == po 
      || NULL == pstConfigInfo || NULL == pstConfigInfo->pcPassword \
      || NULL == pstConfigInfo->pcSmtpSerName || NULL == pstConfigInfo->pcUserName)
   {
    EME_UTILS_LOG_INFO(("EME_SMTP_Open : BADPARAM!"));
      return EME_INVALID_DATA;
   }
   do {
      if (po->uiActType != SMTP_ACTTYPE_NONE)
      {
         EME_UTILS_LOG_INFO(("EME_SMTP_Open,socket has connect"));
         return EME_FAILED;
      }

      if(EME_AUTHENTICATE_TYPE_NAME_PWD == pstConfigInfo->iIsAuth ||
         EME_AUTHENTICATE_TYPE_NAME_PWD_SSL == pstConfigInfo->iIsAuth ||
         EME_AUTHENTICATE_TYPE_SSL == pstConfigInfo->iIsAuth)
      {
         po->bAuth = TRUE;
      }
      else
      {
         po->bAuth = FALSE;
      }
      
      po->stConfigInfo.usPort = pstConfigInfo->usPort;
      po->stConfigInfo.iIsNeedSSL = pstConfigInfo->iIsNeedSSL;
      po->stConfigInfo.iIsAuth = pstConfigInfo->iIsAuth;

      /*get configinfo from input param*/
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcPassword);
      po->stConfigInfo.pcPassword = EME_UTILS_STRDUP(po->pvMemMgr,pstConfigInfo->pcPassword);
      EME_UTILS_BREAKNULL(po->stConfigInfo.pcPassword,
                           iRet,
                           EME_NOMEMORY,
                           "EME_SMTP_Open,stConfigInfo->pcPassword memory allocate failed!");
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcUserName);
      po->stConfigInfo.pcUserName = EME_UTILS_STRDUP(po->pvMemMgr,pstConfigInfo->pcUserName);
      EME_UTILS_BREAKNULL(po->stConfigInfo.pcUserName,
                           iRet,
                           EME_FAILED,
                           "EME_SMTP_Open,stConfigInfo->pcUserName memory allocate failed!");
      /*create socket*/
      po->pstSock = EME_Socket_Create(po->pvIShell,po->pvAsyncMgr,po->pvNetMgr,pHandle,po->pvMemMgr, pstConfigInfo->iIsNeedSSL);
      EME_UTILS_BREAKNULL(po->pstSock,
                          iRet,
                          EME_NOMEMORY,
                          "EME_SMTP_Open,create socket failed");
     /* reg call back */
      EME_Socket_RegCbConnectionOk(po->pstSock, (void*) EME_SMTP_connectEstablish);
      EME_Socket_RegCbReadable(po->pstSock, (void*) EME_SMTP_ConnectSocket);
      EME_Socket_RegCbErrHandler(po->pstSock, (void*) EME_SMTP_SockErrorHandle);
      EME_Socket_RegCbWriteable(po->pstSock, (void*) EME_SMTP_SendDataToSer);
     
      iRet = EME_Socket_Connection(po->pstSock,
                                   pstConfigInfo->pcSmtpSerName,
                                   pstConfigInfo->usPort);
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_Open,connect socket error");

      /*set action type*/
      po->uiActType = SMTP_ACTTYPE_OPEN;
      /**/
   } while(0);
   if (EME_SUCCESS != iRet)
   {/*may be */
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcPassword);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcUserName);
//      EME_Socket_Close(po->pstSock);
//      po->pstSock = NULL;
   }
   return iRet;
}

WE_INT32 EME_SMTP_Send(ISMTPHandle pHandle,St_RawMailInfo* pstRawInfo)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_BOOL bGetRet = FALSE;
   /* check input parameters */
   if (NULL == po || NULL == pstRawInfo->pcFrom || NULL == pstRawInfo->pcRawMail\
      || (NULL == pstRawInfo->pcTo && NULL == pstRawInfo->pcCc && NULL == pstRawInfo->pcBcc))
   {
      return EME_INVALID_DATA;
   }
   do 
   {
      /*get configinfo from input param*/
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcFrom);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcTo);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcCc);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcBcc);
      po->stRawMailInfo.pcFrom = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcFrom);
      EME_UTILS_BREAKNULL(po->stRawMailInfo.pcFrom,
                           iRet,
                           EME_NOMEMORY,
                           "EME_SMTP_Send,stRawMailInfo.pcFrom memory allocate failed!");
      if (pstRawInfo->pcTo != NULL)
      {
         po->stRawMailInfo.pcTo = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcTo);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcTo,
                              iRet,
                              EME_NOMEMORY,
                              "EME_SMTP_Send,stRawMailInfo.pcFrom memory allocate failed!");
      }
      if (pstRawInfo->pcCc != NULL)
      {
         po->stRawMailInfo.pcCc = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcCc);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcCc,
                              iRet,
                              EME_NOMEMORY,
                              "EME_SMTP_Send,stRawMailInfopcCc memory allocate failed!");
      }
      if (pstRawInfo->pcBcc != NULL)
      {
         po->stRawMailInfo.pcBcc = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcBcc);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcBcc,
                              iRet,
                              EME_NOMEMORY,
                              "EME_SMTP_Send,stRawMailInfopcBcc memory allocate failed!");
      }
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcRawMail);

      if(SMTP_RAWMAIL_SRCTYPE_FILE == pstRawInfo->uiSrcType)
      {
         po->stRawMailInfo.pcRawMail = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcRawMail);
      }
      else
      {
         po->stRawMailInfo.pcRawMail = pstRawInfo->pcRawMail;     
      }
      EME_UTILS_BREAKNULL(po->stRawMailInfo.pcRawMail,
                           iRet,
                           EME_NOMEMORY,
                           "EME_SMTP_Send,stConfigInfo->pcUserName memory allocate failed!");
      po->stRawMailInfo.uiSrcType = pstRawInfo->uiSrcType;

      EME_UTILS_MEMSET(po->acToStr,0,EM_MAXSTRLEN);
      po->pcToStrPos = po->stRawMailInfo.pcFrom;
      bGetRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
      if (FALSE == bGetRet)
      {
         iRet = EME_RAWMAIL_INVAL;
      }
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_MailFromSocket,no to input param");
      iRet  = EME_SMTP_MailFrom(pHandle,(WE_VOID*)EME_SMTP_MailFromSocket);
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_Send,mail from failed");
      /*set action type*/
      po->uiActType = SMTP_ACTTYPE_SENDREADY;
      po->uiSentSize = 0;
      po->uiTotalSize = 0;
      po->bCc = FALSE;
      po->bBcc = FALSE;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcFrom);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcTo);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcRawMail);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);
   }
   return iRet;   
}

WE_INT32 EME_SMTP_Forward(ISMTPHandle pHandle,St_RawMailInfo* pstRawInfo)
{
   WE_INT32      iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_BOOL       bGetRet = FALSE;
   /* check input parameters */
   if (NULL == po || NULL == pstRawInfo->pcFrom || NULL == pstRawInfo->pcUID \
      || (NULL == pstRawInfo->pcTo && NULL == pstRawInfo->pcCc && NULL == pstRawInfo->pcBcc))
   {
      EME_UTILS_LOG_ERROR(("EME_SMTP_Forward--: Bad parameter and return"));
      return EME_INVALID_DATA;
   }
   
   do 
   {
      /*get configinfo from input param*/
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcFrom);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcTo);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcCc);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcBcc);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);

      /*transaction type*/
      po->stRawMailInfo.uiTranType = pstRawInfo->uiTranType;

      /*email UID*/
      po->stRawMailInfo.pcUID = EME_UTILS_STRDUP(po->pvMemMgr, pstRawInfo->pcUID);
      EME_UTILS_BREAKNULL(po->stRawMailInfo.pcUID,
         iRet,
         EME_NOMEMORY,
         "EME_SMTP_Forward--,stRawMailInfo.pcUID memory allocate failed!");
      /*from*/
      po->stRawMailInfo.pcFrom = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcFrom);
      EME_UTILS_BREAKNULL(po->stRawMailInfo.pcFrom,
         iRet,
         EME_NOMEMORY,
         "EME_SMTP_Forward--,stRawMailInfo.pcFrom memory allocate failed!");
      /*to*/
      if (pstRawInfo->pcTo != NULL)
      {
         po->stRawMailInfo.pcTo = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcTo);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcTo,
            iRet,
            EME_NOMEMORY,
            "EME_SMTP_Forward--,stRawMailInfo.pcFrom memory allocate failed!");
      }
      /*cc*/
      if (pstRawInfo->pcCc != NULL)
      {
         po->stRawMailInfo.pcCc = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcCc);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcCc,
            iRet,
            EME_NOMEMORY,
            "EME_SMTP_Forward--,stRawMailInfopcCc memory allocate failed!");
      }
      /*bcc*/
      if (pstRawInfo->pcBcc != NULL)
      {
         po->stRawMailInfo.pcBcc = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcBcc);
         EME_UTILS_BREAKNULL(po->stRawMailInfo.pcBcc,
            iRet,
            EME_NOMEMORY,
            "EME_SMTP_Forward--,stRawMailInfopcBcc memory allocate failed!");
      }
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcRawMail);
      /*raw mail buffer*/
      po->stRawMailInfo.pcRawMail = pstRawInfo->pcRawMail;
      //po->stRawMailInfo.pcRawMail = EME_UTILS_STRDUP(po->pvMemMgr,pstRawInfo->pcRawMail);
#if 0
      EME_UTILS_BREAKNULL(po->stRawMailInfo.pcRawMail,
         iRet,
         EME_NOMEMORY,
         "EME_SMTP_Forward--,stConfigInfo->pcUserName memory allocate failed!");
#endif

      po->stRawMailInfo.uiSrcType = pstRawInfo->uiSrcType;

      EME_UTILS_MEMSET(po->acToStr, 0, EM_MAXSTRLEN);

      po->pcToStrPos = po->stRawMailInfo.pcFrom;

      bGetRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
      if (FALSE == bGetRet)
      {
         iRet = EME_RAWMAIL_INVAL;
      }
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_Forward--,no to input param");

      iRet  = EME_SMTP_MailFrom(pHandle,(WE_VOID*)EME_SMTP_MailFromSocket);
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_Forward--,mail from failed");

      /*set action type*/
      po->uiActType = SMTP_ACTTYPE_FORWARDREADY;
      po->uiSentSize = 0;
      po->uiTotalSize = 0;
      po->bCc = FALSE;
      po->bBcc = FALSE;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcFrom);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcTo);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcRawMail);
   }
   return iRet;   
}


WE_INT32 EME_SMTP_Close(ISMTPHandle pHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   /* check input parameters */
   if (NULL == po)
   {
      return EME_INVALID_DATA;
   }
      /*send quit cmd*/
   iRet = EME_SMTP_Quit(pHandle,(WE_VOID*)EME_SMTP_QuitSocket);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Close,quit failed"));
   }
   /*set action type*/
   po->uiActType = SMTP_ACTTYPE_CLOSE;

   return iRet;
}

WE_INT32 EME_SMTP_Cancel(ISMTPHandle pHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   /* check input parameters */
   if (NULL == po)
   {
      return EME_INVALID_DATA;
   }
//   iRet = EME_SMTP_Rset(pHandle,EME_SMTP_RsetSocket);
//////////////////////use rset cmd or quit??////////////////////////////////////////
   do {
   /* or    iRet = EME_SMTP_Quit(pHandle,EME_SMTP_QuitSocket);
   */   
//      if (po->uiActType == SMTP_ACTTYPE_SENDOVER || po->uiActType == SMTP_ACTTYPE_CLOSE || po->uiActType == SMTP_ACTTYPE_NONE)
//      {
//         EME_UTILS_LOG_INFO(("EME_SMTP_Cancel,cancel failed"));
//         iRet = EME_FAILED;
//      }
//      else
//      {
         iRet = EME_Socket_Close(po->pstSock);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_Cancel,close socket failed");
         po->pstSock = NULL;
         po->uiActType = SMTP_ACTTYPE_NONE;
//      }
   } while(0);
   
   return iRet;
}

WE_INT32 EME_SMTP_DeleteHandle(ISMTPHandle *ppHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = NULL;

   /* check input parameters */
   if (NULL == ppHandle || *ppHandle == NULL)
   {
      return EME_INVALID_DATA;
   }
   do 
   {
      
      po = *ppHandle;
      
      if (po->pstSock != NULL)
      {
         iRet = EME_Socket_Close(po->pstSock);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_DeleteHandle,close socket failed");
         po->pstSock = NULL;
      }
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcPassword);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcSmtpSerName);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stConfigInfo.pcUserName);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcFrom);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcRawMail);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcTo);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcCc);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcBcc);
      EME_UTILS_FREEIF(po->pvMemMgr,po->stRawMailInfo.pcUID);
      EME_FileClose(po->pvSendFile);
      po->pvSendFile = NULL;
      EME_UTILS_FREEIF(po->pvMemMgr,po->pcSendBuf);
      EME_UTILS_FREEIF(po->pvMemMgr,po);

      *ppHandle = NULL;
   } while(0);
   return iRet;
}

//////////////////////////////////////////////////////////////////
///////////////////*socket back function*/////////////////////////
//////////////////////////////////////////////////////////////////
/*error handling from socket*/

WE_VOID EME_SMTP_SockErrorHandle(ISMTPHandle pHandle,WE_INT32 iErrcode)
{
   WE_UINT32 uiActType = 0;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   if (NULL == po)
   {
      return ;
   }
/**/
   switch(iErrcode)
   {
   case EME_SOCKET_SHUTDOWN:
      uiActType = SMTP_ACTTYPE_NOTIFY;
   	break;

   default:
      uiActType = po->uiActType;
      break;
   }
   {
      EME_SMTP_NotifyToMGR(pHandle,uiActType,iErrcode);
   }   
}
/*prepare for sending*/
WE_VOID EME_SMTP_connectEstablish(ISMTPHandle pHandle)
{
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   if (NULL == po)
   {
      return ;
   }

   return ;
}
/*connect to smtp server*/
WE_VOID EME_SMTP_ConnectSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA) return;
      ///////FZ///////////
      if (EME_UTILS_STRSTR(po->acResponse,SERVER_READY))
      {
         if (po->bAuth == TRUE)
         {
            iRet = EME_SMTP_Ehlo(pHandle,(WE_VOID*)EME_SMTP_EhloSocket);
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_ConnectSocket,ehlo failed");
         }
         else
         {
            iRet = EME_SMTP_Helo(pHandle,(WE_VOID*)EME_SMTP_HeloSocket);
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_ConnectSocket,ehlo failed");
         }
      }
      else
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_SERVER_RESPONSE_ERR);
      }
   } while(0);
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

/*connect to smtp server*/
WE_VOID EME_SMTP_TlsSocketCB(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   
   do {
      if (po->bAuth)
      {
         /*Add by jade for support hotmail ssl feature, 2011.03.24
           After handshake with server, send data by ssl*/
         iRet = EME_SMTP_Auth(pHandle,(WE_VOID*)EME_SMTP_AuthSocket);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_Auth,ehlo failed");
      }
      else
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_SERVER_RESPONSE_ERR);
      }
   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

WE_VOID EME_SMTP_TlsSocket(ISMTPHandle pHandle)
{
	WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   
   do {
      if (po->bAuth)
      {
         /*Add by jade for support hotmail ssl feature, 2011.03.24
           After handshake with server, send data by ssl*/
         iRet = OEM_AsyncProc_AddProc(po->pvAsyncMgr,EME_SMTP_TlsSocketCB, pHandle);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_Auth,ehlo failed");
      }
      else
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_SERVER_RESPONSE_ERR);
      }
   } while(0);

   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }

   return ;
}
/*ehlo   esmtp*/
WE_VOID EME_SMTP_EhloSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_Socket_RegCbReadable(po->pstSock, (void*) NULL);
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_EhloSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA) 
      {
         EME_Socket_RegCbReadable(po->pstSock, (void*)EME_SMTP_EhloSocket);
         break;
      }

      while (po->acResponse[0] != 0)
      {
         if (EME_UTILS_STRSTR(po->acResponse,EHLO_DONE))
         {
            /*Add by jade for support hotmail ssl feature, 2011.03.24
              if SSL need and port is 25, we should send STARTTLS command to server*/
            if(po->stConfigInfo.iIsNeedSSL && EME_SSL_TYPE_STARTTLS == po->stConfigInfo.iIsNeedSSL)
            {
               iRet = EME_SMTP_StartTls(pHandle, (WE_VOID*)EME_SMTP_StartTlsSocket);
            }
            else
            {
               iRet = EME_SMTP_Auth(pHandle,(WE_VOID*)EME_SMTP_AuthSocket);
            }
            
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_EhloSocket,auth failed");
            break;
         }
         else if (EME_UTILS_STRSTR(po->acResponse,EHLO_DOING)) 
         {
            EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
            iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
            if (iRet == EME_SOCKET_RECV_NODATA) 
            {
               EME_Socket_RegCbReadable(po->pstSock, (void*) EME_SMTP_EhloSocket);
               break;       
            }  
         }
         else if (EME_UTILS_STRSTR(po->acResponse,CMD_UNDONE))
         {                
            iRet = EME_SMTP_Helo(pHandle,(WE_VOID*)EME_SMTP_HeloSocket);
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_EhloSocket,helo failed");
            break;
         }
         else if (EME_UTILS_STRSTR(po->acResponse,SERVER_NOT_AVAILABLE))
         {
            iRet = EME_SERVER_RESPONSE_ERR;
            break;
         }
      }

   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet= EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

/*helo  not support esmtp*/
WE_VOID EME_SMTP_HeloSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_HeloSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,CMD_DONE))
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_SUCCESS);
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_LOGIN_ERROR);            
      }
   } while(0);
   return ;
}

/*startls   esmtp*/
WE_VOID EME_SMTP_StartTlsSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_Socket_RegCbReadable(po->pstSock, (void*) NULL);
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_EhloSocket~~~~~~~~~~~~"));
   
   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA) 
      {
         EME_Socket_RegCbReadable(po->pstSock, (void*)EME_SMTP_StartTlsSocket);
         break;
      }
      
      while (po->acResponse[0] != 0)
      {
         if (EME_UTILS_STRSTR(po->acResponse,SERVER_READY))
         {
            /*Add by jade for support hotmail ssl feature, 2011.03.24
              After send STARTTLS command, we shold start ssl handshake with server*/
            EME_Socket_StartTls(po->pstSock);
            EME_Socket_RegCbReadable(po->pstSock, (void*)EME_SMTP_TlsSocket);
            EME_Socket_RegCbErrHandler(po->pstSock, (void*) EME_SMTP_SockErrorHandle);
            EME_Socket_RegCbWriteable(po->pstSock, (void*) EME_SMTP_SendDataToSer);
            break;
         }
         else if (EME_UTILS_STRSTR(po->acResponse,SERVER_NOT_AVAILABLE))
         {
            iRet = EME_SERVER_RESPONSE_ERR;
            break;
         }
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet= EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

/*auth login*/
WE_VOID EME_SMTP_AuthSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po  || NULL == po->pstSock|| NULL == po->stConfigInfo.pcUserName)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_AuthSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA) break;

      if (EME_UTILS_STRSTR(po->acResponse,AUTH_DOING))
      {
         iRet = EME_SMTP_AuthNameOrPwd(pHandle,(WE_VOID*)EME_SMTP_AuthNameSocket,po->stConfigInfo.pcUserName);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_AuthSocket,auth name failed");
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_LOGIN_ERROR);            
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet=EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

WE_VOID EME_SMTP_AuthNameSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock|| NULL == po->stConfigInfo.pcPassword)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_AuthNameSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,AUTH_DOING))
      {
         iRet = EME_SMTP_AuthNameOrPwd(pHandle,(WE_VOID*)EME_SMTP_AuthPwdSocket,po->stConfigInfo.pcPassword);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_AuthNameSocket,auth pwd failed");
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_LOGIN_ERROR);            
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet=EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,iRet);
   }
   return ;
}

WE_VOID EME_SMTP_AuthPwdSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_AuthPwdSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,AUTH_DONE))
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_SUCCESS);
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_OPEN,EME_LOGIN_ERROR);            
      }
   } while(0);
   return ;
}

/*mail from*/
WE_VOID EME_SMTP_MailFromSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_BOOL bGetToRet = FALSE;
   WE_UINT32  uiActType = SMTP_ACTTYPE_NONE;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_MailFromSocket~~~~~~~~~~~~"));

   do 
   {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)
      {
         break;
      }

      if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL ||
         po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_ATTACHMENT)
      {
         uiActType = SMTP_ACTTYPE_FORWARDREADY;
      }
      else
      {
         uiActType = SMTP_ACTTYPE_SENDREADY;
      }

      if (EME_UTILS_STRSTR(po->acResponse,CMD_DONE))
      {
         EME_UTILS_MEMSET(po->acToStr,0,EM_MAXSTRLEN);
         po->pcToStrPos = po->stRawMailInfo.pcTo;
         bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
         if (FALSE == bGetToRet)
         {
            po->pcToStrPos = po->stRawMailInfo.pcCc;
            po->bCc =TRUE;
            bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
            if (FALSE == bGetToRet)
            {
               po->pcToStrPos = po->stRawMailInfo.pcBcc;
               po->bBcc =TRUE;
               bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
               if (FALSE == bGetToRet)
               {
                  EME_UTILS_LOG_INFO(("EME_SMTP_MailFromSocket,no to input param"));
                  EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_RAWMAIL_INVAL);
                  return ;              
               }
            }
         }
         iRet = EME_SMTP_RcptTo(pHandle,(WE_VOID*)EME_SMTP_RcptToSocket);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_MailFromSocket,rcpt to failed");
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SERVER_RESPONSE_ERR);            
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet=EME_SUCCESS;

   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,uiActType,iRet);
   }
   return ;
}
/*rcpt to*/
WE_VOID EME_SMTP_RcptToSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_BOOL bGetToRet = FALSE;

   WE_UINT32  uiActType = SMTP_ACTTYPE_NONE;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_RcptToSocket~~~~~~~~~~~~"));

   do 
   {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)
      {
         break;
      }

      if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_ATTACHMENT ||
         po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL)
      {
         uiActType = SMTP_ACTTYPE_FORWARDREADY;
      }
      else
      {
         uiActType = SMTP_ACTTYPE_SENDREADY;
      }

      if (EME_UTILS_STRSTR(po->acResponse,CMD_DONE))
      {
         EME_UTILS_MEMSET(po->acToStr,0,EM_MAXSTRLEN);
         bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
         if (FALSE == bGetToRet)
         {
            if (po->bCc == FALSE)
            {
               po->pcToStrPos = po->stRawMailInfo.pcCc;
               po->bCc =TRUE;
            }
            EME_UTILS_MEMSET(po->acToStr,0,EM_MAXSTRLEN);
            bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
            if (FALSE == bGetToRet)
            {
               if (po->bBcc == FALSE)
               {
                  po->pcToStrPos = po->stRawMailInfo.pcBcc;
                  po->bBcc =TRUE;
               }
               EME_UTILS_MEMSET(po->acToStr,0,EM_MAXSTRLEN);
               bGetToRet = EME_SMTP_GetToStr(pHandle,po->acToStr);
               if (FALSE == bGetToRet)
               {
                  if(uiActType == SMTP_ACTTYPE_FORWARDREADY)
                  {
                     iRet = EME_SMTP_Efwd(pHandle, (WE_VOID*)EME_SMTP_DataSocket);
                     EME_UTILS_BREAKIF(iRet, "forward mail failed");
                  }
                  else
                  {
                     iRet = EME_SMTP_Data(pHandle,(WE_VOID*)EME_SMTP_DataSocket);
                     EME_UTILS_BREAKIF(iRet,"EME_SMTP_RcptToSocket,data failed");
                  }
                  break;              
               }
            }
         }
         iRet = EME_SMTP_RcptTo(pHandle,(WE_VOID*)EME_SMTP_RcptToSocket);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_RcptToSocket,rcpt to failed");
      }
      else 
      {
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SERVER_RESPONSE_ERR);            
      }
   } while(0);

   if (iRet & EME_INFO_SOCKET_START)
   {
      iRet = EME_SUCCESS;
   }

   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,uiActType,iRet);
   }
   return ;
}

/*data*/
WE_VOID EME_SMTP_DataSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_UINT32     uiActType = SMTP_ACTTYPE_NONE;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_DataSocket~~~~~~~~~~~~"));

   do 
   {
      if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL ||
         po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_ATTACHMENT)
      {
         uiActType = SMTP_ACTTYPE_FORWARDING;
      }
      else
      {
         uiActType = SMTP_ACTTYPE_SENDING;
      }

      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,DATA_READY))
      {
         iRet = EME_SMTP_SendData(pHandle,(WE_VOID*)EME_SMTP_SendDataSocket);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_DataSocket,send data failed");
      }
      else if (EME_UTILS_STRSTR(po->acResponse, CMD_DONE))
      {
          EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SUCCESS);            
      }
      else 
      {
         /*data error*/
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SERVER_RESPONSE_ERR);            
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)
   {
      iRet=EME_SUCCESS;
   }

   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,uiActType,iRet);
   }
   return ;
}

/*efwd*/
WE_VOID EME_SMTP_EfwdSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_DataSocket~~~~~~~~~~~~"));

   do 
   {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,DATA_READY))
      {
         iRet = EME_SMTP_SendData(pHandle,(WE_VOID*)EME_SMTP_SendDataSocket);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_DataSocket,send data failed");
      }
      else 
      {
         /*data error*/
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_SENDING,EME_SERVER_RESPONSE_ERR);            
      }
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet=EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_SENDING,iRet);
   }
   return ;
}

/*send data*/
WE_VOID EME_SMTP_SendDataSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_UINT32     uiActType = SMTP_ACTTYPE_NONE;

   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_SendDataSocket~~~~~~~~~~~~"));

   do 
   {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)
      {
         break;
      }

      if(SMTP_ACTTYPE_FORWARDING == po->uiActType)
      {
         uiActType = SMTP_ACTTYPE_FORWARDOVER;
         po->uiActType = SMTP_ACTTYPE_FORWARDOVER;
      }
      else
      {
         uiActType = SMTP_ACTTYPE_SENDOVER;
         po->uiActType = SMTP_ACTTYPE_SENDOVER;
      }

      if (EME_UTILS_STRSTR(po->acResponse,CMD_DONE))
      {
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SUCCESS);                     
      }
      else if (EME_UTILS_STRSTR(po->acResponse,REQUEST_ABORT))
      {
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SMTP_REQUEST_ABORT);
      }
      else 
      {
         /*data error*/
         EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SERVER_RESPONSE_ERR);            
      }
   } while(0);

   return ;
}
/*quit*/
WE_VOID EME_SMTP_QuitSocket(ISMTPHandle pHandle)
{
   WE_INT32  iRet = EME_SUCCESS;      
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po || NULL == po->pstSock)
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_QuitSocket~~~~~~~~~~~~"));

   do {
      EME_UTILS_MEMSET(po->acResponse,0,EM_RESPLEN);
      iRet = EME_Socket_Read(po->pstSock,po->acResponse,EM_RESPLEN,NULL);
      if (iRet == EME_SOCKET_RECV_NODATA)break;

      if (EME_UTILS_STRSTR(po->acResponse,SERVER_CLOSE))
      {
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_CLOSE,EME_SUCCESS);                     
      }
      else /////////////////////////////////////////////
      {
         /*data error*/
         EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_CLOSE,EME_FAILED);            
      }
      /*close socket*/
      if (po->pstSock != NULL)
      {
         iRet = EME_Socket_Close(po->pstSock);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_QuitSocket,close socket failed");
   	   po->pstSock = NULL;
      }

      po->uiActType = SMTP_ACTTYPE_NONE;
   } while(0);
   if (iRet&EME_INFO_SOCKET_START)iRet=EME_SUCCESS;
   if (iRet != EME_SUCCESS)
   {
      EME_SMTP_NotifyToMGR(pHandle,SMTP_ACTTYPE_CLOSE,iRet);
   }

   return ;
}

///////////////////////////////////////////////////////
////////////*CMD POOL funtion*/////////////////////////
///////////////////////////////////////////////////////

/*ehlo*/
WE_INT32 EME_SMTP_Ehlo(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po  || NULL == po->pstSock|| NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Ehlo~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);   
   EME_UTILS_SPRINTF(po->acCMD,"%s%s%s",EHLO,TFMAIL,CMD_END);
   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}
/*helo*/
WE_INT32 EME_SMTP_Helo(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Helo~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,"%s %s%s",HELO,TFMAIL,CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

/*starttls*/
WE_INT32 EME_SMTP_StartTls(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   /* check input parameters */
   if (NULL == po  || NULL == po->pstSock|| NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Ehlo~~~~~~~~~~~~"));
   
   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);   
   EME_UTILS_SPRINTF(po->acCMD,"%s%s",STARTTLS,CMD_END);
   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_StartTls,write to socket failed"));
   }
   return iRet;
}

/*auth login*/
WE_INT32 EME_SMTP_Auth(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Auth~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,"%s%s",AUTH_LOGIN,CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

WE_INT32 EME_SMTP_AuthNameOrPwd(ISMTPHandle pHandle,void* pvSockBack,char* pcStr)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   
   WE_UINT32 uiEncodeSize = 0;
   WE_CHAR* pcEncodeStr = NULL;
   int iSize = 0;
   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack || pcStr == NULL)
   {
      return EME_INVALID_DATA;
   }
   do {
      EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_AuthNameOrPwd~~~~~~~~~~~~"));
      EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
      //////////////*change username to base64 format*////////////        
      uiEncodeSize = EME_UTILS_Base64EncodeLen(EME_UTILS_STRLEN(pcStr));
      EME_UTILS_LOG_INFO(("EME_SMTP_AuthNameOrPwd : EME_UTILS_Base64EncodeLen len = %d", uiEncodeSize));
      if (uiEncodeSize == 0)
      {
         iRet = EME_LOGIN_ERROR;
         EME_UTILS_LOG_ERROR(("EME_SMTP_AuthNameOrPwd, username or password is empty"));
         break;
      }

      pcEncodeStr = EME_UTILS_MALLOC(po->pvMemMgr,uiEncodeSize +1);
      EME_UTILS_BREAKNULL(pcEncodeStr,iRet,EME_NOMEMORY,"EME_SMTP_AuthName,malloc for encodestr failed");
      EME_UTILS_MEMSET(pcEncodeStr,0,uiEncodeSize+1);
      iSize = uiEncodeSize + 1;
      iRet = EME_UTILS_Base64Encode(pcStr, EME_UTILS_STRLEN(pcStr), pcEncodeStr, &iSize);		
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_AuthName,tobase64 failed");
      
      EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
      EME_UTILS_SPRINTF(po->acCMD,"%s%s",pcEncodeStr,CMD_END);
      
      iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_AuthName,write to socket failed");
   } while(0);
   EME_UTILS_FREEIF(po->pvMemMgr,pcEncodeStr);

   return iRet;
}


/*mail from*/
WE_INT32 EME_SMTP_MailFrom(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack || NULL == po->stRawMailInfo.pcFrom)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_MailFrom~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,
                     "%s%c%c%s%c%s",
                     MAIL_FROM,
                     COLON_MARK,
                     LEFT_BRACKET,
                     po->acToStr,
                     RIGHT_BRACKET,
                     CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));      
   }
   return iRet;
}
/*rcpt to*/
WE_INT32 EME_SMTP_RcptTo(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_RcptTo~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);

   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,
                     "%s%c%c%s%c%s",
                     RCPT_TO,
                     COLON_MARK,
                     LEFT_BRACKET,
                     po->acToStr,
                     RIGHT_BRACKET,
                     CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

/*data*/
WE_INT32 EME_SMTP_Data(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Data~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,"%s%s",DATA,CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

/*efwd*/
WE_INT32 EME_SMTP_Efwd(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Data~~~~~~~~~~~~"));

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
//   EME_UTILS_SPRINTF(po->acCMD,"%s %s%s",EFWD, po->stRawMailInfo.pcUID, CMD_END);
   EME_UTILS_SPRINTF(po->acCMD,"%s %s",EFWD, po->stRawMailInfo.pcUID); //pcUID already contain \r\n

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

/*send data*/
WE_INT32 EME_SMTP_SendData(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   //WE_INT32 uiActType = SMTP_ACTTYPE_NONE;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack )//|| NULL == po->stRawMailInfo.pcRawMail)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_SendData~~~~~~~~~~~~"));

   do 
   {
      if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_ATTACHMENT ||
         po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL)
      {
         po->uiActType = SMTP_ACTTYPE_FORWARDING;
      }
      else
      {
         po->uiActType = SMTP_ACTTYPE_SENDING;
      }

      EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);

      if (po->stRawMailInfo.uiSrcType == SMTP_RAWMAIL_SRCTYPE_FILE)
      {
         /*EME_FileClose*/
         /*iRet = */EME_FileClose(po->pvSendFile);   
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendData,close file failed");

         po->pvSendFile = EME_FileOpen(po->pvFileMgr,po->stRawMailInfo.pcRawMail,EME_OFM_READWRITE);
         EME_UTILS_BREAKNULL(po->pvSendFile,iRet,EME_FILE_ERROR,"EME_SMTP_SendData ,open rawmail file failed");
         po->uiTotalSize = EME_FileLength(po->pvSendFile);
      }
      else
      {
         if(NULL != po->stRawMailInfo.pcRawMail && po->stRawMailInfo.uiTranType != EME_TRANS_SMTP_FORWARD_WHOLEMAIL)
         {
            po->uiTotalSize = EME_UTILS_STRLEN(po->stRawMailInfo.pcRawMail);
         }
         else if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL)
         {
            po->uiTotalSize = 0;
         }
         else
         {
            iRet = EME_FAILED;
            EME_UTILS_BREAKIF(iRet, "don't support type");
         }
      }

      po->uiSentSize = 0;

      EME_UTILS_FREEIF(po->pvMemMgr,po->pcSendBuf);
      po->pcSendBuf = EME_UTILS_MALLOC(po->pvMemMgr,SEND_BUFFER_SIZE + 1);
      EME_UTILS_BREAKNULL(po->pcSendBuf,iRet,EME_FILE_ERROR,"EME_SMTP_SendData ,malloc send buffer failed");
      EME_UTILS_MEMSET(po->pcSendBuf, 0, SEND_BUFFER_SIZE + 1);

      EME_SMTP_SendDataToSer(pHandle);
   } while(0);
   ///////////////more
   if (EME_SUCCESS != iRet)
   {
   }
   return iRet;
}
/*quit*/
WE_INT32 EME_SMTP_Quit(ISMTPHandle pHandle,void* pvSockBack)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;

   if (NULL == po || NULL == po->pstSock || NULL == pvSockBack)
   {
      return EME_INVALID_DATA;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_Quit~~~~~~~~~~~~"));
   if (po->uiActType == SMTP_ACTTYPE_SENDING)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Quit,it was sending...,can not quit"));
      return EME_FAILED;
   }   

   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
   
   EME_UTILS_MEMSET(po->acCMD,0,EM_MAXSTRLEN);
   EME_UTILS_SPRINTF(po->acCMD,"%s%s",QUIT,CMD_END);

   iRet = EME_Socket_Write(po->pstSock,po->acCMD,EME_UTILS_STRLEN(po->acCMD),TRUE);
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_LOG_INFO(("EME_SMTP_Ehlo,write to socket failed"));
   }
   return iRet;
}

///////////////////////////////////////////////
//////////////////*utils function*/////////////
///////////////////////////////////////////////
WE_INT32 EME_SMTP_NotifyToMGR(ISMTPHandle pHandle,WE_UINT32 uiActType,WE_INT32 iRet)
{
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   St_SMTPActRet stActRet = {0};
   if (NULL == po || NULL == po->pstSock)
   {
      return EME_INVALID_DATA;
   }

   if(iRet != EME_SUCCESS && EME_UTILS_STRLEN(po->acResponse) > 0)
      EME_UTILS_MEMCPY(stActRet.acRespDesc, po->acResponse, EME_UTILS_STRLEN(po->acResponse));
   EME_UTILS_MEMSET(po->acResponse, 0, EM_RESPLEN);

   stActRet.iRet = iRet;
   stActRet.uiActType = uiActType;
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_NotifyToMGR~~~~~~~~~~~~"));
   if (po->uiActType == SMTP_ACTTYPE_SENDING && iRet == EME_SUCCESS)
   {
      stActRet.stDataSent.uiSentData = po->uiSentSize;
      stActRet.stDataSent.uiTotalData = po->uiTotalSize;
   }
   if (po->pCB == NULL || po->pCBCx == NULL)
   {
      EME_Socket_Close(po->pstSock);
      EME_UTILS_LOG_INFO(("EME_SMTP_NotifyToMGR,no mgr register function")); 
      po->pstSock = NULL;
      return EME_FAILED;
   }
   po->pCB(po->pCBCx,&stActRet);
   return EME_SUCCESS;
}

WE_BOOL EME_SMTP_GetToStr(ISMTPHandle pHandle,WE_CHAR* pcOutStr)
{
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   char* pcLeftBracket  = NULL;
   char* pcRightBracket = NULL;
   char* pcNowPos = NULL;
   
   if (NULL == po || NULL == pcOutStr || po->pcToStrPos == NULL)
   {
      return FALSE;//////////////////////
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_GetToStr~~~~~~~~~~~~"));
   if ((pcNowPos = EME_UTILS_STRCHR(po->pcToStrPos, COMMA_MARK)) != NULL)/* addr in list */
   {
      pcLeftBracket  = EME_UTILS_STRCHR(po->pcToStrPos, LEFT_BRACKET);
      pcRightBracket = EME_UTILS_STRCHR(po->pcToStrPos, RIGHT_BRACKET);
      
      if (NULL != pcLeftBracket && NULL != pcRightBracket)
      {
         EME_UTILS_MEMCPY(pcOutStr,pcLeftBracket + 1,pcRightBracket-pcLeftBracket-1);
      }
      /*0131 by echo if there are no <> around address*/
      else if (NULL == pcLeftBracket && NULL == pcRightBracket)
      {
         EME_UTILS_MEMCPY(pcOutStr,po->pcToStrPos,pcNowPos-po->pcToStrPos);
      }      
      po->pcToStrPos = pcNowPos + 1; /* go to next To str. */
      return TRUE;
   }
   else if (*(po->pcToStrPos) != END_STR)/* has last addr in list */
   {
      pcLeftBracket  = EME_UTILS_STRCHR(po->pcToStrPos, LEFT_BRACKET);
      pcRightBracket = EME_UTILS_STRCHR(po->pcToStrPos, RIGHT_BRACKET);
      
      if (NULL != pcLeftBracket && NULL != pcRightBracket)
      {
         EME_UTILS_MEMCPY(pcOutStr,pcLeftBracket + 1,pcRightBracket-pcLeftBracket-1);
         po->pcToStrPos = pcRightBracket + 1;
      }
      /*by echo  more 000*/
      else if(NULL == pcLeftBracket && NULL == pcRightBracket)
      {
         EME_UTILS_MEMCPY(pcOutStr,po->pcToStrPos,EME_UTILS_STRLEN(po->pcToStrPos));
         po->pcToStrPos = po->pcToStrPos + EME_UTILS_STRLEN(po->pcToStrPos); /* find end go to END_STR */
      }
      return TRUE;
   }
   return FALSE;
}

WE_VOID EME_SMTP_SendDataToSer(ISMTPHandle pHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
   WE_UINT32 uiReadNum = 0;
   WE_UINT32 uiActType = SMTP_ACTTYPE_NONE;

   if (NULL == po || po->pcSendBuf == NULL ||\
      (po->pvSendFile == NULL && po->stRawMailInfo.uiSrcType == SMTP_RAWMAIL_SRCTYPE_FILE))
   {
      return ;
   }
   EME_UTILS_LOG_INFO(("~~~~~~~~EME_SMTP_SendDataToSer~~~~~~~~~~~~"));
   EME_UTILS_LOG_INFO(("~~~~~~~~have send %d,total %d~~",po->uiSentSize,po->uiTotalSize));

   do
   {
      if(po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_WHOLEMAIL ||
         po->stRawMailInfo.uiTranType == EME_TRANS_SMTP_FORWARD_ATTACHMENT)
      {
         po->uiActType = SMTP_ACTTYPE_FORWARDING;
         uiActType = SMTP_ACTTYPE_FORWARDING;
      }
      else
      {
         po->uiActType = SMTP_ACTTYPE_SENDING;
         uiActType = SMTP_ACTTYPE_SENDING;
      }

      iRet = EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SUCCESS);   
      EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,notify to mgr failed");

      EME_UTILS_MEMSET(po->pcSendBuf,0,SEND_BUFFER_SIZE + 1);

      if (po->uiTotalSize == po->uiSentSize)
      {
         if(SMTP_ACTTYPE_FORWARDING==uiActType)
         {
             EME_UTILS_MEMCPY(po->pcSendBuf, ENDOFFWD_TAG, ENDOFFWD_TAG_SIZE);
         }
         else
         {
             EME_UTILS_MEMCPY(po->pcSendBuf, ENDOFMAIL_TAG, ENDOFMAIL_TAG_SIZE);
         }
         iRet = EME_Socket_Write(po->pstSock,po->pcSendBuf,EME_UTILS_STRLEN(po->pcSendBuf),TRUE);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,write to socket failed");
         
         EME_UTILS_FREEIF(po->pvMemMgr, po->pcSendBuf);
         if (po->stRawMailInfo.uiSrcType == SMTP_RAWMAIL_SRCTYPE_FILE && po->pvSendFile != NULL)
         {
            EME_FileClose(po->pvSendFile);
            po->pvSendFile = NULL;
         }
      }
   	else if (po->uiTotalSize - po->uiSentSize < SEND_BUFFER_SIZE)
   	{
         if (po->stRawMailInfo.uiSrcType == SMTP_RAWMAIL_SRCTYPE_FILE)
         {
            uiReadNum = EME_FileRead(po->pvSendFile,(WE_VOID*)po->pcSendBuf,po->uiTotalSize - po->uiSentSize);
            if (uiReadNum != po->uiTotalSize - po->uiSentSize)
            {
               iRet = EME_FILE_ERROR;
            }
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,read file error last");
         }
         else
         {
            if(po->stRawMailInfo.uiTranType != EME_TRANS_SMTP_FORWARD_ATTACHMENT &&
               po->stRawMailInfo.pcRawMail != NULL)
            {
               EME_UTILS_MEMCPY(po->pcSendBuf,po->stRawMailInfo.pcRawMail+po->uiSentSize,po->uiTotalSize - po->uiSentSize);
            }
         }

         po->uiSentSize = po->uiTotalSize;
         iRet = EME_Socket_Write(po->pstSock,po->pcSendBuf,EME_UTILS_STRLEN(po->pcSendBuf),FALSE);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,write to socket failed");
         //EME_UTILS_FREEIF(po->pvMemMgr,po->pcSendBuf);

         iRet = EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_SUCCESS);   
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,notify to mgr failed");
   	}
      else
      {
         if (po->stRawMailInfo.uiSrcType == SMTP_RAWMAIL_SRCTYPE_FILE)
         {
            uiReadNum = 0;
            uiReadNum = EME_FileRead(po->pvSendFile,(WE_VOID*)po->pcSendBuf,SEND_BUFFER_SIZE);
            if (uiReadNum != SEND_BUFFER_SIZE)
            {
               iRet = EME_FILE_ERROR;
            }
            EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,read file error ");
         }
         else
         {
            if(po->stRawMailInfo.uiTranType != EME_TRANS_SMTP_FORWARD_ATTACHMENT &&
               po->stRawMailInfo.pcRawMail != NULL)
            {
               EME_UTILS_MEMCPY(po->pcSendBuf,po->stRawMailInfo.pcRawMail+po->uiSentSize,SEND_BUFFER_SIZE);
            }
         }

         po->uiSentSize += SEND_BUFFER_SIZE;
         iRet = EME_Socket_Write(po->pstSock,po->pcSendBuf,EME_UTILS_STRLEN(po->pcSendBuf),FALSE);
         EME_UTILS_BREAKIF(iRet,"EME_SMTP_SendDataToSer,write to socket failed");
      }
   } while(0);

   if (EME_SUCCESS != iRet)
   {
      EME_SMTP_NotifyToMGR(pHandle,uiActType,EME_FILE_ERROR);
   }
   return ;
}


/*rset*/
//WE_INT32 EME_SMTP_Rset(ISMTPHandle pHandle,void* pvSockBack)
//{
//   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
//   WE_CHAR po->po->acCMD[EM_MAXSTRLEN] = {0};
//
//   if (NULL == pHandle || NULL == pvSockBack)
//   {
//      EME_UTILS_LOG("EME_SMTP_Rset,input parameters null");
//      return EME_INVALID_DATA;
//   }
//
//   EME_Socket_RegCbReadable(po->pstSock, (void*) pvSockBack);
//   
//   EME_UTILS_SPRINTF(po->po->acCMD,"%s%s",RSET,CMD_END);
//
//   EME_Socket_CommWithServer(po->pstSock, 
//                   SENT_COMMAND,
//                   po->po->acCMD,
//                   NULL,  /*may be*/
//                   0,
//                   0);
//   return EME_SUCCESS;
//}
/*rset*/
//WE_INT32 EME_SMTP_RsetSocket(ISMTPHandle pHandle)
//{
//   WE_INT32  iRet = EME_SUCCESS;      
//   St_SMTPHandle *po = (St_SMTPHandle*)pHandle;
//   WE_CHAR po->acResponse[EM_RESPLEN] = {0};
//   /* check input parameters */
//   if (NULL == pHandle)
//   {
//      EME_UTILS_LOG("EME_SMTP_RsetSocket,input parameters null!!");
//      return EME_INVALID_DATA;
//   }
//
//   do {
//   	iRet = EME_Socket_CommWithServer(po->pstSock,GET_RESPONSE,NULL,po->acResponse,EM_RESPLEN,0);
//      ///////FZ///////////
//      if (po->acResponse[0] != 0 && EME_UTILS_STRSTR(po->acResponse,CMD_DONE))
//      {
//                     
//      }
//      else 
//      {
//         /*data error*/
//    
//      }
//   } while(0);
//
//   return iRet;
//}

#ifdef DEBUG_UNITTEST
#include "../../../../../test/emailut/stack_ut/stackapp/impl/src/ut_smtp.c"
#endif
