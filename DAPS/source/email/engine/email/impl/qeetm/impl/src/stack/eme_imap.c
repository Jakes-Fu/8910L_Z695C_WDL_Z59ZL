/*=====================================================================================
    FILE: eme_imap.c
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Implement of IMAP Stack APIs

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
======================================================================================= 
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
//pclint
/*lint -e611 */

#include "eme_file_api.h"
#include "eme_imap_api.h"
#include "eme_imap_utils.h"
#include "eme_socket_api.h"
#include "eme_imap_cmdpool.h"
#include "eme_imap_parser.h"


/*****************************************************************************/
/*                            micro define                                   */                                           
/*****************************************************************************/

#define EME_IMAP_GET_RESPONSE(pstIMAP, iResult)\
iResult = EME_IMAP_GetResponse(pstIMAP);\
if (EME_SOCKET_RECV_NODATA == iResult)\
   return;\

/*****************************************************************************/
/*                       static function declare                             */                                           
/*****************************************************************************/

/*----------------------------utils functions----------------------------------------*/
static St_IMAPOpt* EME_IMAP_GetOpt(St_IMAPOpt *pstOpts, WE_INT32 iOptID);
static WE_INT32 EME_IMAP_GetResponse(WE_VOID* hIMAPHandle);
static WE_INT32 EME_IMAP_ResetReturnData(IMAPHandle hIMAPHandle, void* pvMemMgr);
static WE_VOID EME_IMAP_SendDataBuffer(WE_VOID* hIMAPHandle);

/*----------------------net related functions----------------------------------------*/
static WE_VOID EME_IMAP_NetAvailable(WE_VOID* hIMAPHandle);
static WE_VOID EME_IMAP_ErrorHandle(WE_VOID* hIMAPHandle, WE_INT32 pcErrorReason);
static WE_VOID EME_IMAP_Notify(WE_VOID* hIMAPHandle);

/*----------------------functions for dealing response----------------------------------------*/
static WE_VOID EME_IMAP_DealConnectResponse(WE_VOID* hIMAPHandle);
/* deal data response, include fetch header(not include bodystructure), one mail, mail part */
static WE_VOID EME_IMAP_DealFetchResponse(WE_VOID* hIMAPHandle);
/* deal all infomation response(except append) but not data response.So not include fetch header
   (not include bodystructure), one mail, mail part */
static WE_VOID EME_IMAP_DealInfomationResponse(WE_VOID* hIMAPHandle);

/* deal append command response */
static WE_VOID EME_IMAP_DealAppendResponse(WE_VOID* hIMAPHandle);

static WE_VOID EME_IMAP_DealSendDataResponse(WE_VOID* hIMAPHandle);

/* used when waiting for transation */
static WE_VOID EME_IMAP_IgnoreMessage(WE_VOID* hIMAPHandle);

/*****************************************************************************/
/*                          function implement                               */                                           
/*****************************************************************************/

/*----------------------------------------------------------------------------*/
/*                            init handle                                     */        
/*----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_InitHandle(IMAPHandle* phIMAPHandle, 
                             WE_VOID* pvMemMgr, 
                             WE_VOID* pvIShell,
                             WE_VOID* pvAsyncMgr,
                             WE_VOID* pvFileMgr, 
                             WE_VOID* pvINetMgr)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = NULL;
   St_IMAPInfo *pstIMAPInfo = NULL;

   if (NULL == phIMAPHandle)
      return EME_INVALID_DATA;

   do
   {
      /* allocate memory */
      pstIMAP = *phIMAPHandle = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_IMAP));
      EME_UTILS_BREAKNULL(*phIMAPHandle, iRet, EME_NOMEMORY, "IMAP inithandle failed");
      EME_UTILS_MEMSET(pstIMAP, 0, sizeof(St_IMAP));
      /* init mgr */
      pstIMAP->stIMAPMgr.pvMemMgr = pvMemMgr;
      pstIMAP->stIMAPMgr.pvFileMgr = pvFileMgr;
      pstIMAP->stIMAPMgr.pvIShell = pvIShell;
      pstIMAP->stIMAPMgr.pvNetMgr = pvINetMgr;
      pstIMAP->stIMAPMgr.pvAsyncMgr = pvAsyncMgr;
      pstIMAPInfo = &pstIMAP->stIMAPInfo;

      pstIMAPInfo->stBodystructure.pMemHdr = pvMemMgr;
      pstIMAPInfo->stBodystructure.pvFileMgr = pvFileMgr;

      pstIMAPInfo->stUIDString.pMemHdr = pvMemMgr;
      pstIMAPInfo->stUIDString.pvFileMgr = pvFileMgr;

      pstIMAPInfo->stHeader.pMemHdr = pvMemMgr;
      pstIMAPInfo->stHeader.pvFileMgr = pvFileMgr;
      
      pstIMAPInfo->stUIDBuf.pMemHdr = pvMemMgr;
      pstIMAPInfo->stUIDBuf.pFileMgr = pvFileMgr;

      pstIMAPInfo->stMailInfoArray.pMemHdr = pvMemMgr;	  
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      if (NULL != *phIMAPHandle)
      {
         EME_UTILS_FREEIF(pvMemMgr, *phIMAPHandle);
      }
      return iRet;
   }

   return EME_SUCCESS;
}


WE_INT32 EME_IMAP_SetCallback(IMAPHandle hIMAPHandle, PFNIMAPNOTIFY pfnNotify, WE_VOID* pvClientData)
{
   St_IMAP* pstIMAP = NULL;

   if (NULL == hIMAPHandle || NULL == pfnNotify)
      return EME_INVALID_DATA;
   
   pstIMAP = (St_IMAP*)hIMAPHandle;

   /* register callback */
   pstIMAP->stIMAPCallback.pfnNotify = pfnNotify;
   pstIMAP->stIMAPCallback.pvClientData = pvClientData;

   return EME_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*                          open IMAP flow                                  
    EME_IMAP_OpenIMAP(connect to server)     
            |
            |
            |
            V
    EME_IMAP_DealConnectResponse(deal connect response and send login CMD)
            |
            |
            |
            V
    EME_IMAP_DealInfomationResponse(deal login response)                        */
/*  ----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_OpenIMAP(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_VOID* pvMemMgr = NULL;
   HSocket hSocket = NULL;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPOpt *pstServerAddrOpt = NULL;
   St_IMAPOpt *pstServerPortOpt = NULL;
   St_IMAPOpt *pstUserNameOpt = NULL;
   St_IMAPOpt *pstPasswordOpt = NULL;
   St_IMAPOpt *pstUIDFilePathOpt = NULL;
   St_IMAPOpt *pstSizeLimitOpt = NULL;
   St_IMAPOpt *pstIsNeedSSLOpt = NULL;
   St_IMAPOpt *pstNumberLimitOpt = NULL;
   St_IMAPOpt *pstReceiveLastOpt = NULL;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   St_IMAPAccountInfo *pstIMAPAccountInfo = NULL;
   if (NULL == pstIMAP || NULL == pOpts)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pstIMAPAccountInfo = &pstIMAP->stIMAPAccountInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;

   /* make sure user registerd callback */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify)
      return EME_INVALID_DATA;
   
   do
   {
      pstIsNeedSSLOpt  = EME_IMAP_GetOpt(pOpts, EME_IMAP_IS_NEED_SSL);
      EME_UTILS_BREAKNULL(pstIsNeedSSLOpt, iRet, EME_INVALID_DATA, "imap_wft IMAP ssl option not found");

      pstIMAP->stIMAPSocket.hSocket = EME_Socket_Create(pstIMAP->stIMAPMgr.pvIShell, 
                                                        pstIMAP->stIMAPMgr.pvAsyncMgr,
                                                        pstIMAP->stIMAPMgr.pvNetMgr, 
                                                        pstIMAP, 
                                                        pvMemMgr, 
                                                        (WE_INT32)pstIsNeedSSLOpt->pvVal);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPSocket.hSocket, iRet, EME_NOMEMORY, "IMAP inithandle failed");
      
      hSocket = pstIMAP->stIMAPSocket.hSocket;
      /* register socket related callbacks */
      EME_Socket_RegCbConnectionOk(hSocket, (WE_VOID*)EME_IMAP_NetAvailable);
      EME_Socket_RegCbErrHandler(hSocket, (WE_VOID*)EME_IMAP_ErrorHandle);

      /* get opt from param pOpts for later use */
      pstServerAddrOpt  = EME_IMAP_GetOpt(pOpts, EME_IMAP_SERVER_ADDR);
      pstServerPortOpt  = EME_IMAP_GetOpt(pOpts, EME_IMAP_SERVER_PORT);
      pstUserNameOpt    = EME_IMAP_GetOpt(pOpts, EME_IMAP_USERNAME);
      pstPasswordOpt    = EME_IMAP_GetOpt(pOpts, EME_IMAP_PASSWORD);
      pstUIDFilePathOpt = EME_IMAP_GetOpt(pOpts, EME_IMAP_UID_FILE_PATH);
      pstSizeLimitOpt   = EME_IMAP_GetOpt(pOpts, EME_IMAP_SIZE_LIMIT);
      pstNumberLimitOpt = EME_IMAP_GetOpt(pOpts, EME_IMAP_NUMBER_LIMIT);
      pstReceiveLastOpt = EME_IMAP_GetOpt(pOpts, EME_IMAP_RECEIVE_LAST);

      /* make sure every param is valide */
      EME_UTILS_BREAKNULL(pstServerAddrOpt, iRet, EME_INVALID_DATA, "IMAP server addr invalide");
      EME_UTILS_BREAKNULL(pstUserNameOpt, iRet, EME_INVALID_DATA, "IMAP user name invalide");
      EME_UTILS_BREAKNULL(pstPasswordOpt, iRet, EME_INVALID_DATA, "IMAP pass word invalide");
      EME_UTILS_BREAKNULL(pstUIDFilePathOpt, iRet, EME_INVALID_DATA, "IMAP uid file invalide");
      EME_UTILS_BREAKNULL(pstServerPortOpt, iRet, EME_INVALID_DATA, "IMAP server port invalide");
      EME_UTILS_BREAKNULL(pstSizeLimitOpt, iRet, EME_INVALID_DATA, "IMAP size limit invalide");
      EME_UTILS_BREAKNULL(pstNumberLimitOpt, iRet, EME_INVALID_DATA, "IMAP number limit invalide");      
      EME_UTILS_BREAKNULL(pstReceiveLastOpt, iRet, EME_INVALID_DATA, "IMAP receive last invalide");

      /* save the account infomation that may use in later */
      pstIMAPAccountInfo->uiSizeLimit   = (WE_UINT32)pstSizeLimitOpt->pvVal;
      pstIMAPAccountInfo->uiNumberLimit = (WE_UINT32)pstNumberLimitOpt->pvVal;
      pstIMAPAccountInfo->iLast = (WE_UINT32)pstReceiveLastOpt->pvVal;
      pstIMAPAccountInfo->pcUIDFilePath = EME_UTILS_STRDUP(pvMemMgr, pstUIDFilePathOpt->pvVal);
      pstIMAPInfo->pcUIDFilePath        = EME_UTILS_STRDUP(pvMemMgr, pstUIDFilePathOpt->pvVal);
      pstIMAPAccountInfo->pcUserName    = EME_UTILS_STRDUP(pvMemMgr, pstUserNameOpt->pvVal);
      pstIMAPAccountInfo->pcPassword    = EME_UTILS_STRDUP(pvMemMgr, pstPasswordOpt->pvVal);
      EME_UTILS_BREAKNULL(pstIMAPAccountInfo->pcUIDFilePath, iRet, EME_NOMEMORY, "IMAP no memory")
      EME_UTILS_BREAKNULL(pstIMAPAccountInfo->pcUserName, iRet, EME_NOMEMORY, "IMAP no memory")
      EME_UTILS_BREAKNULL(pstIMAPAccountInfo->pcPassword, iRet, EME_NOMEMORY, "IMAP no memory")
      EME_UTILS_BREAKNULL(pstIMAPInfo->pcUIDFilePath, iRet, EME_NOMEMORY, "IMAP no memory")

      /* register EME_IMAP_DealConnectResponse to deal connection response(welcome infomation) */
      EME_Socket_RegCbReadable(pstIMAPSocket->hSocket, (WE_VOID*)EME_IMAP_DealConnectResponse);

      /* first, connect to server */
      iRet = EME_Socket_Connection(pstIMAPSocket->hSocket,
                                   pstServerAddrOpt->pvVal,
                                   (WE_UINT16)(WE_UINT32)pstServerPortOpt->pvVal);
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_OPEN_IMAP_END;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      return iRet;
   }

   return EME_SUCCESS;
}


static WE_VOID EME_IMAP_DealConnectResponse(WE_VOID* hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_INT32 iResult = EME_FAILED;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_CHAR* pcUserName = NULL;
   WE_CHAR* pcPassword = NULL;

   if (NULL == pstIMAP)
      return;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pcUserName = pstIMAP->stIMAPAccountInfo.pcUserName;
   pcPassword = pstIMAP->stIMAPAccountInfo.pcPassword;

   do 
   {
      /* get response */
      EME_IMAP_GET_RESPONSE(pstIMAP, iResult);

      /* if get no more response or response have not end (indicated by '\n'),
         return and waiting for more response */
      if (NULL == EME_UTILS_STRCHR(pstIMAP->stIMAPSocket.acResponse, '\n'))
         return;
      
      /* response end ('\n' found), set bConnected status TRUE and register EME_IMAP_DealLoginResponse 
         to deal login response and then send login command to login to user's account */
      pstIMAP->stIMAPStatus.bConnected = TRUE;
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_LOGIN;
      iRet = EME_IMAP_SendCMDLogin(pstIMAPSocket, 
                                   pcUserName,
                                   pcPassword,
                                   EME_IMAP_DealInfomationResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDLogin failed");
   
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      /* notify error */
      EME_IMAP_ErrorHandle(pstIMAP, iRet);
   }
}


/*----------------------------------------------------------------------------*/
/*                          select mailbox flow                                  
    EME_IMAP_SelectMailbox(send select CMD)     
            |
            |
            |
            V
    EME_IMAP_DealInfomationResponse(deal select response)                     */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_SelectMailbox(IMAPHandle hIMAPHandle, WE_CHAR* pcMailboxName)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   WE_CHAR* pcMailboxNameUtf7 = NULL;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_VOID* pvMemMgr = NULL;
   
   if (NULL == pstIMAP || NULL == pcMailboxName || '\0' == *pcMailboxName)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;

   /* make sure user registerd callback and make sure user has connected to 
      server and authenticated*/
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify 
       && TRUE != pstIMAP->stIMAPStatus.bAuthenticated)
      return EME_INVALID_DATA;

   do 
   {
      pcMailboxNameUtf7 = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pcMailboxName)*3+3);
      EME_UTILS_BREAKNULL(pcMailboxNameUtf7, iRet, EME_NOMEMORY, "IMAP no memory");
      EME_UTILS_MEMSET(pcMailboxNameUtf7, 0, EME_UTILS_STRLEN(pcMailboxName) * 3 + 3);
      /* mailbox of IMAP use utf7, so change utf8 to utf7 */
      iRet = EME_IMAP_Utf8ToUtf7(pcMailboxName, pcMailboxNameUtf7, pvMemMgr);
      EME_UTILS_BREAKIF(iRet, "IMAP utf8 to utf7 failed");

      /* register EME_IMAP_DealInfomationResponse to deal select response and send login command 
         to select one of user's mailboxs*/
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_SELECT_MAILBOX;
      iRet = EME_IMAP_SendCMDSelect(pstIMAPSocket,
                                    pcMailboxNameUtf7,
                                    EME_IMAP_DealInfomationResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDSelect failed");

      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_SELECT_MAILBOX_END;
      /* save the mailbox name for later use */
      EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMailboxName);
      pstIMAPInfo->pcMailboxName = EME_UTILS_STRDUP(pvMemMgr, pcMailboxName);
      EME_UTILS_BREAKNULL(pstIMAPInfo->pcMailboxName, iRet, EME_NOMEMORY, "IMAP no memory failed");

   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcMailboxNameUtf7);
   
   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMailboxName);
   }

   return iRet;
}
/*----------------------------------------------------------------------------*/
/*                          get infomation of new mails flow                                  
    EME_IMAP_GetInfoOfNewMails(send UID search CMD)     
            |
            |
            |
            V
    EME_IMAP_DealInfomationResponse
(deal UID search response and send fetch size CMD)
            |
            |
            |
            V
   EME_IMAP_DealInfomationResponse(deal fetch size response)
                                                                              */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_GetInfoOfNewMails(IMAPHandle hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;

   /* make sure user registerd callback and make sure user has connected to server 
      and authenticated and selected a mailbox */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
       && TRUE != pstIMAP->stIMAPStatus.bSelected)
      return EME_INVALID_DATA;
   
   do
   {
      EME_UTILS_LOG_INFO(("[IMAPLOG]EME_IMAP_GetInfoOfNewMails NumOfMails is %d uiNumberLimit is %d",pstIMAPInfo->NumOfMails,pstIMAP->stIMAPAccountInfo.uiNumberLimit));
      if (pstIMAPInfo->NumOfMails < 1 || 0 == pstIMAP->stIMAPAccountInfo.uiNumberLimit)
         return EME_NO_NEWMAIL;
      
      /* send uid search command to get mailbox's uid list */
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_SEARCH_UID;
      iRet = EME_IMAP_SendCMDUIDSearch(pstIMAPSocket,
                                       1,
                                       pstIMAPInfo->NumOfMails,
                                       EME_IMAP_DealInfomationResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDUIDSearch failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_GET_INFOMATION_OF_NEW_MAILS_END;
   } while(0);
   
   return EME_SUCCESS;
}
/*----------------------------------------------------------------------------*/
/*                          get one header of new mails flow                                  
    EME_IMAP_FetchHeader(send fetch bodystructure CMD)     
            |
            |
            |
            V
    EME_IMAP_DealInfomationResponse
(deal fetch bodystructure response and send fetch header CMD)
            |
            |
            |
            V
   EME_IMAP_DealFetchResponse(deal fetch header response)
                                                                              */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_FetchHeader(IMAPHandle hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_VOID* pvMemMgr = NULL;
   St_IMAPNumberNotify* pstNumberNotify = NULL;
   St_MailInfoArray* pstMailInfoArray = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   pstNumberNotify = &pstIMAP->stIMAPActRet.unRetData.stNumberNotify;
   pstMailInfoArray = &pstIMAP->stIMAPInfo.stMailInfoArray;

   /* make sure user registerd callback and make sure user has connected to server 
      and authenticated and selected a mailbox and has get enought infomation for getting new mails */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
       && TRUE != pstIMAP->stIMAPStatus.bInfoGeted)
      return EME_INVALID_DATA;
   
   do
   {
      /* if get uid from array success, it means there are new mails need download
         else no new mail */
      iRet = EME_IMAP_GetUIDFromArray(&pstIMAPInfo->stMailInfoArray,
                                 &pstIMAPInfo->uiCurrentUID);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_GetUIDFromArray failed");

      /* if uiUID is 0, there must be something wrong */
      if (0 == pstIMAPInfo->uiCurrentUID)
      {
         iRet = EME_FAILED;
         break;
      }

      iRet = EME_IMAP_GetSizeFromArray(&pstIMAP->stIMAPInfo.stMailInfoArray,
                                  pstIMAPInfo->uiCurrentUID,
                                  &pstIMAPInfo->uiTotalSize);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_GetSizeFromArray failed");
      if (0 == pstIMAPInfo->uiTotalSize)
      {
         iRet = EME_FAILED;
         break;
      }

      EME_IMAP_ResetReturnData(pstIMAP, pvMemMgr);
      pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_NUMBER_NOTIFY;
      pstNumberNotify->uiCurrentNumber = pstMailInfoArray->uiIndex;
      //pstNumberNotify->uiTotalNumber = pstMailInfoArray->uiLength;
      pstNumberNotify->uiTotalNumber = pstIMAPInfo->NumOfMails;
      EME_IMAP_Notify(hIMAPHandle);

      /* register to deal fetch bodystructure response and send fetch body structure 
         cmd to get bodystructure*/
      pstIMAPInfo->iSizeLimitExceed = FALSE;
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE;
      iRet = EME_IMAP_SendCMDBodystructure(pstIMAPSocket, 
                                           pstIMAPInfo->uiCurrentUID,
                                           EME_IMAP_DealInfomationResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDBodystructure failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_FETCH_HEADER_END;
   } while(0);
   
   return iRet;
}
/*----------------------------------------------------------------------------*/
/*                       get one mail of new mails flow   
                      
                           (if size limit exceed, 
                           only download header)
    EME_IMAP_FetchMail ==========================> EME_IMAP_DealInfomationResponse
    (send fetch mail CMD if size limit           (deal fetch bodystructure response 
     not exceed, else fetch mail header)          and send fetch header CMD)
            |                                                  |
            |                                                  |
            |(if size limit not exceed,                        |
            | download whole mail)                             |
            |                                                  |
            V                                                  V
   EME_IMAP_DealFetchResponse                       EME_IMAP_DealFetchResponse
   (deal fetch mail response)                       (deal fetch header response)
                                                                              */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_FetchMail(IMAPHandle hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_VOID* pvMemMgr = NULL;
   St_IMAPNumberNotify* pstNumberNotify = NULL;
   St_MailInfoArray* pstMailInfoArray = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   pstNumberNotify = &pstIMAP->stIMAPActRet.unRetData.stNumberNotify;
   pstMailInfoArray = &pstIMAP->stIMAPInfo.stMailInfoArray;

   /* make sure user registerd callback and make sure user has connected to server and authenticated
      and selected a mailbox and has get enought infomation for getting new mails  */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
       && TRUE != pstIMAP->stIMAPStatus.bInfoGeted)
      return EME_INVALID_DATA;
   
   do
   {
      /*---------------------------------------------------------------------
         if get uid from array success, it means there are new mails need download
         else no new mail 
       ---------------------------------------------------------------------*/
      iRet = EME_IMAP_GetUIDFromArray(&pstIMAP->stIMAPInfo.stMailInfoArray, 
                                      &pstIMAP->stIMAPInfo.uiCurrentUID);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_GetUIDFromArray failed");

      /* if uiUID is 0, there must be something wrong */
      if (0 == pstIMAP->stIMAPInfo.uiCurrentUID)
      {
         iRet = EME_FAILED;
         break;
      }

      /* allocate memory need mail's size */
      iRet = EME_IMAP_GetSizeFromArray(&pstIMAP->stIMAPInfo.stMailInfoArray,
                                  pstIMAP->stIMAPInfo.uiCurrentUID,
                                  &pstIMAP->stIMAPInfo.uiTotalSize);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_GetSizeFromArray failed");
      if (0 == pstIMAP->stIMAPInfo.uiTotalSize)
      {
         iRet = EME_FAILED;
         break;
      }
      
      EME_IMAP_ResetReturnData(pstIMAP, pvMemMgr);
      pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_NUMBER_NOTIFY;
      pstNumberNotify->uiCurrentNumber = pstMailInfoArray->uiIndex;
      //pstNumberNotify->uiTotalNumber = pstMailInfoArray->uiLength;
      pstNumberNotify->uiTotalNumber = pstIMAP->stIMAPInfo.NumOfMails;
      EME_IMAP_Notify(hIMAPHandle);

      pstIMAP->stIMAPInfo.pvCatchBuf = EME_UTILS_MALLOC(pvMemMgr, EME_IMAP_BUFFER_CATCH_SIZE + 1);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvCatchBuf, iRet, EME_NOMEMORY, "no memory for catch");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);
      
      pstIMAP->stIMAPInfo.pcFileName = EME_UTILS_MALLOC(pvMemMgr,
         EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) +
         EME_IMAP_MAX_FILE_NAME_LEN);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pcFileName, iRet, EME_NOMEMORY, "no memory for temp file");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pcFileName, 0, 
         EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) +
         EME_IMAP_MAX_FILE_NAME_LEN);
      /* if size limit not excced, fetch whole mail, else fetch header */
      if (pstIMAP->stIMAPInfo.uiTotalSize <= pstIMAP->stIMAPAccountInfo.uiSizeLimit*1024)
      {
         /*---------------------------------------------------------------------
            allocate memory according mail's size, "+EME_IMAP_ADDITIONAL_MEMORY" is for making sure
            that we allocate enough memory
          ---------------------------------------------------------------------*/

         EME_UTILS_SPRINTF(pstIMAP->stIMAPInfo.pcFileName, "%s\\eml%x.eml", 
            EME_IMAP_TEMP_MAIL_FILE_DIR,
            EME_UTILS_GETTIMEMS);

         pstIMAP->stIMAPInfo.pvTmpFile = EME_FileOpen(pstIMAP->stIMAPMgr.pvFileMgr, 
            pstIMAP->stIMAPInfo.pcFileName,
            EME_OFM_CREATE);
         EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvTmpFile, iRet, EME_FAILED, "create temp file failed");

         /* register to deal fetch mail response and send fetch mail cmd to get new mail*/
         pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_MAIL;
         iRet = EME_IMAP_SendCMDFetchMail(pstIMAPSocket, 
                                          pstIMAP->stIMAPInfo.uiCurrentUID,
                                          EME_IMAP_DealFetchResponse);
         EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDFetchMail failed");
         pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_FETCH_MAIL_END;
         pstIMAP->stIMAPInfo.iSizeLimitExceed = FALSE;
      }
      else
      {
         /* get bodystructure is the first step for getting mail's header.register EME_IMAP_DealInfomationResponse to deal 
            fetch bodystructure response and send fetch body structure cmd to get bodystructure*/
         EME_UTILS_SPRINTF(pstIMAP->stIMAPInfo.pcFileName, "%s\\eml%x.bdy", 
            EME_IMAP_TEMP_MAIL_FILE_DIR,
            EME_UTILS_GETTIMEMS);

         pstIMAP->stIMAPInfo.pvTmpFile = EME_FileOpen(pstIMAP->stIMAPMgr.pvFileMgr, 
            pstIMAP->stIMAPInfo.pcFileName,
            EME_OFM_CREATE);
         EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvTmpFile, iRet, EME_FAILED, "create temp file failed");
         pstIMAP->stIMAPInfo.iSizeLimitExceed = TRUE;
         pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE;
         iRet = EME_IMAP_SendCMDBodystructure(pstIMAPSocket, 
                                              pstIMAP->stIMAPInfo.uiCurrentUID,
                                              EME_IMAP_DealInfomationResponse);
         EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDBodystructure failed");
         pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_FETCH_MAIL_END;
      }

   } while(0);
    
   if(iRet != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPInfo.pcFileName);
      EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPInfo.pvCatchBuf);
      EME_FileClose(pstIMAP->stIMAPInfo.pvTmpFile);
      pstIMAP->stIMAPInfo.pvTmpFile = NULL;
   }

   return iRet;
}

/*----------------------------------------------------------------------------*/
/*                get one mail which has header aleady, main flow   
                      
                           (if size limit exceed)
    EME_IMAP_FetchSpecificMail ===================> return EME_SIZE_LIMIT_EXCEED to 
    (send fetch mail CMD if size limit              notify failed.
     not exceed, else return failed)        
            |                                   
            |                                   
            |(if size limit not exceed,         
            | download whole mail)              
            |                                   
            V                                   
   EME_IMAP_DealFetchResponse                   
   (deal fetch specific mail response)                   
                                                                              */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_FetchSpecificMail(IMAPHandle hIMAPHandle, WE_UINT32 uiUID, WE_UINT32 uiSize)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP || 0 == uiUID || 0 == uiSize)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;

   /* make sure user registerd callback and make sure user has connected to server and authenticated 
      and selected a mailbox and has get enought infomation for getting new mails*/
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify && pstIMAP->stIMAPStatus.bInfoGeted)
      return EME_INVALID_DATA;
   
   do
   {
      pstIMAP->stIMAPInfo.uiTotalSize = uiSize;
      pstIMAP->stIMAPInfo.uiSpecificUID = uiUID;

      /* exceed size limit  */
      if (uiSize > pstIMAP->stIMAPAccountInfo.uiSizeLimit*1024)
      {
         return EME_SIZE_LIMIT_EXCEED;
      }

      pstIMAP->stIMAPInfo.pcFileName = EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr,
         EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) +
         EME_IMAP_MAX_FILE_NAME_LEN);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pcFileName, iRet, EME_NOMEMORY, "no memory for file name");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pcFileName, 0, EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) + EME_IMAP_MAX_FILE_NAME_LEN);

      pstIMAP->stIMAPInfo.pvCatchBuf = EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr, EME_IMAP_BUFFER_CATCH_SIZE + 1);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvCatchBuf, iRet, EME_NOMEMORY, "no memory for catch buff");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);

      EME_UTILS_SPRINTF(pstIMAP->stIMAPInfo.pcFileName, "%s\\eml%x.eml", EME_IMAP_TEMP_MAIL_FILE_DIR, EME_UTILS_GETTIMEMS);

      pstIMAP->stIMAPInfo.pvTmpFile = EME_FileOpen(pstIMAP->stIMAPMgr.pvFileMgr, pstIMAP->stIMAPInfo.pcFileName, EME_OFM_CREATE);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvTmpFile, iRet, EME_FAILED, "create file failed");

      pstIMAP->stIMAPInfo.uiUsedBufSize = 0;

      /* register to deal fetch specific mail response, send fetch mail cmd to get specific mail */
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_SPECIFIC_MAIL;
      iRet = EME_IMAP_SendCMDFetchMail(pstIMAPSocket, uiUID, EME_IMAP_DealFetchResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDFetchMail failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_FETCH_SPECIFIC_MAIL_END;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {   
      EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAP->stIMAPInfo.pcFileName);
      EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAP->stIMAPInfo.pvCatchBuf);
      EME_FileClose(pstIMAP->stIMAPInfo.pvTmpFile);
      pstIMAP->stIMAPInfo.pvTmpFile = NULL;
      return iRet;
   }
   return EME_SUCCESS;
}
/*----------------------------------------------------------------------------*/
/*                get specific mail's part, main flow   
                      
                           (if size limit exceed)
    EME_IMAP_FetchSpecificPart ===================> return EME_SIZE_LIMIT_EXCEED to 
    (send fetch part CMD if size limit              notify failed.
     not exceed, else return failed)        
            |                                   
            |                                   
            |(if size limit not exceed,         
            | download mail's specific part)              
            |                                   
            V                                   
   EME_IMAP_DealFetchResponse                   
   (deal fetch mail's specific part response)                   
                                                                              */
/*  --------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_FetchSpecificPart(IMAPHandle hIMAPHandle, WE_UINT32 uiUID, WE_CHAR* pcSectionID, WE_UINT32 uiSize)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP || 0 == uiUID || 0 == uiSize || NULL == pcSectionID)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;

   /* make sure user registerd callback and make sure user has connected to server and authenticated 
      and selected a mailbox and has get enought infomation for getting new mails*/
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify 
      && pstIMAP->stIMAPStatus.bInfoGeted)
      return EME_INVALID_DATA;
   
   do
   {
      pstIMAP->stIMAPInfo.uiTotalSize = uiSize;
      pstIMAP->stIMAPInfo.uiSpecificUID = uiUID;

      /* exceed size limit  */
      if (uiSize > pstIMAP->stIMAPAccountInfo.uiSizeLimit*1024)
      {
         return EME_SIZE_LIMIT_EXCEED;
      }

      pstIMAP->stIMAPInfo.pcFileName = EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr,
         EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) +
         EME_IMAP_MAX_FILE_NAME_LEN);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pcFileName, iRet, EME_NOMEMORY, "no memory for file name");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pcFileName, 0, EME_UTILS_STRLEN(EME_IMAP_TEMP_MAIL_FILE_DIR) + EME_IMAP_MAX_FILE_NAME_LEN);

      pstIMAP->stIMAPInfo.pvCatchBuf = EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr, EME_IMAP_BUFFER_CATCH_SIZE + 1);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvCatchBuf, iRet, EME_NOMEMORY, "no memory for catch buff");
      EME_UTILS_MEMSET(pstIMAP->stIMAPInfo.pvCatchBuf, 0, EME_IMAP_BUFFER_CATCH_SIZE + 1);

      EME_UTILS_SPRINTF(pstIMAP->stIMAPInfo.pcFileName, "%s\\eml%x.prt", EME_IMAP_TEMP_MAIL_FILE_DIR,  EME_UTILS_GETTIMEMS);

      pstIMAP->stIMAPInfo.pvTmpFile = EME_FileOpen(pstIMAP->stIMAPMgr.pvFileMgr, pstIMAP->stIMAPInfo.pcFileName, EME_OFM_CREATE);
      EME_UTILS_BREAKNULL(pstIMAP->stIMAPInfo.pvTmpFile, iRet, EME_FAILED, "create file failed");

      pstIMAP->stIMAPInfo.uiUsedBufSize = 0;

      /* register to deal fetch specific mail response, send fetch mail cmd to get specific mail */
      pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_MAILPART;
      iRet = EME_IMAP_SendCMDFetchPart(pstIMAPSocket,
                                       uiUID,
                                       pcSectionID,
                                       EME_IMAP_DealFetchResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDFetchPart failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_FETCH_SPECIFIC_PART_END;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {   
      return iRet;
   }
   return EME_SUCCESS;
}


/*----------------------------------------------------------------------------*/
/*                   handle mail, main flow   
                      
                        (if operation is APPEND)
    EME_IMAP_HandleMail ======================> EME_IMAP_DealAppendResponse
    (send CMD according operation type)         (deal append response and call
            |                                    EME_IMAP_SendDataBuffer to send
            |                                    mail's data)
            |                                                |
            |(if operation is not APPEND)                    |
            |                                                |
            |                                                |
            V                                                V <--------------------+
    EME_IMAP_DealInfomationResponse             EME_IMAP_SendDataBuffer             |
    (deal CMD response)                         (call EME_IMAP_SendDataBuffer       |
        EME_IMAP_DealSendDataResponse            repeatly to send mail's data       |
                                                 until no data need send)___________|
                                                             |
                                                             |
                                                             |
                                                             V
                                                EME_IMAP_DealSendDataResponse
                                                (send data end, deal send mail's data response)
                                                                              */
/*----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_HandleMail(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_VOID* pvMemMgr = NULL;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPOpt *pstUID = NULL;
   St_IMAPOpt *pstMailbox = NULL;
   WE_CHAR* pcMailboxNameUtf7 = NULL;
   St_IMAPOpt *pstMailPath = NULL;
   St_IMAPOpt *pstHandleMailType = NULL;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP || NULL == pOpts)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   
   pstHandleMailType = EME_IMAP_GetOpt(pOpts, EME_IMAP_HANDLE_MAIL_TYPE);
   if (NULL == pstHandleMailType)
      return EME_INVALID_DATA;

   /* make sure user registerd callback and make sure user has connected to server 
   and authenticated and selected a mailbox(not include append, it do not need select mailbox) */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
      && TRUE != pstIMAP->stIMAPStatus.bSelected
      /* E_EME_IMAP_APPEND operation do not need select mailbox) */
      && E_EME_IMAP_APPEND != (WE_UINT32)pstHandleMailType->pvVal)
      return EME_INVALID_DATA;
   
   do 
   {
      /* get opt from param pOpts for later use */
      if (E_EME_IMAP_APPEND != (WE_INT32)pstHandleMailType->pvVal)
      {
         pstUID  = EME_IMAP_GetOpt(pOpts, EME_IMAP_UID);
         EME_UTILS_BREAKNULL(pstUID, iRet, EME_INVALID_DATA, "imap get uid failed");
      }
      
      switch((WE_INT32)pstHandleMailType->pvVal)
      {
      case E_EME_IMAP_MARK_SEEN:
         {
            iRet = EME_IMAP_SendCMDMarkSeen(pstIMAPSocket, 
                                           (WE_UINT32)pstUID->pvVal, 
                                           EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_MARK_SEEN;
            break;
         }
      case E_EME_IMAP_UNMARK_SEEN:
         {
            iRet = EME_IMAP_SendCMDUnmarkSeen(pstIMAPSocket, 
                                           (WE_UINT32)pstUID->pvVal, 
                                           EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_UNMARK_SEEN;
            break;
         }
      case E_EME_IMAP_MARK_DELETED:
         {
            iRet = EME_IMAP_SendCMDMarkDeleted(pstIMAPSocket, 
                                            (WE_UINT32)pstUID->pvVal, 
                                            EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_MARK_DELETED;
            break;
         }
      case E_EME_IMAP_UNMARK_DELETED:
         {
            iRet = EME_IMAP_SendCMDUnmarkDeleted(pstIMAPSocket, 
                                            (WE_UINT32)pstUID->pvVal, 
                                            EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_UNMARK_DELETED;
            break;
         }
      case E_EME_IMAP_COPY:
         {
            pstMailbox = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAILBOX_NAME);
            EME_UTILS_BREAKNULL(pstMailbox, iRet, EME_INVALID_DATA, "imap get param mailbox failed");
            EME_UTILS_BREAKNULL(pstMailbox->pvVal, iRet, EME_INVALID_DATA, "imap get param mailbox failed");

            pcMailboxNameUtf7 = 
               EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr, EME_UTILS_STRLEN(pstMailbox->pvVal)*3+3);
            EME_UTILS_BREAKNULL(pcMailboxNameUtf7, iRet, EME_NOMEMORY, "IMAP no memory");
            
            /* mailbox of IMAP use utf7, so change utf8 to utf7 */
            iRet = EME_IMAP_Utf8ToUtf7((WE_CHAR*)pstMailbox->pvVal, pcMailboxNameUtf7, pstIMAP->stIMAPMgr.pvMemMgr);
            EME_UTILS_BREAKIF(iRet, "IMAP utf8 to utf7 failed");
            
            iRet = EME_IMAP_SendCMDCopy(pstIMAPSocket, 
                                       (WE_UINT32)pstUID->pvVal,
                                       pcMailboxNameUtf7,
                                       EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_COPY;
            break;
         }
      case E_EME_IMAP_APPEND:
         {
            St_IMAPOpt *pstMailBuffer = NULL;
            St_IMAPOpt *pstMailSize = NULL;

            /* get append param, mailbox and file */
            pstMailbox = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAILBOX_NAME);
            pstMailBuffer = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAIL_BUFFER);
            pstMailSize = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAIL_BUFFER_SIZE);
            EME_UTILS_BREAKNULL(pstMailbox, iRet, EME_INVALID_DATA, "imap get param mailbox failed");
            EME_UTILS_BREAKNULL(pstMailbox->pvVal, iRet, EME_INVALID_DATA, "imap get param mailbox failed");

            pcMailboxNameUtf7 = 
               EME_UTILS_MALLOC(pstIMAP->stIMAPMgr.pvMemMgr, EME_UTILS_STRLEN(pstMailbox->pvVal)*3+3);
            EME_UTILS_BREAKNULL(pcMailboxNameUtf7, iRet, EME_NOMEMORY, "IMAP no memory");
            
            /* mailbox of IMAP use utf7, so change utf8 to utf7 */
            iRet = EME_IMAP_Utf8ToUtf7((WE_CHAR*)pstMailbox->pvVal, pcMailboxNameUtf7, pstIMAP->stIMAPMgr.pvMemMgr);
            EME_UTILS_BREAKIF(iRet, "IMAP utf8 to utf7 failed");

            if (NULL == pstMailBuffer)
            {
               pstMailPath = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAIL_PATH);
               EME_UTILS_BREAKNULL(pstMailPath, iRet, EME_INVALID_DATA, "imap get param failed");
               EME_UTILS_BREAKNULL(pstMailPath->pvVal, iRet, EME_INVALID_DATA, "imap get param failed");
               
               pstIMAPInfo->pvFile = EME_FileOpen(pstIMAP->stIMAPMgr.pvFileMgr, 
                                                 (WE_CHAR*)pstMailPath->pvVal,
                                                 EME_OFM_READ);
               EME_UTILS_BREAKNULL(pstIMAPInfo->pvFile, iRet, EME_INVALID_DATA, "imap open file failed");
               
               /* make sure file valide */
               pstIMAPInfo->uiTotalSize = EME_FileLength(pstIMAPInfo->pvFile);
               if (0 == pstIMAPInfo->uiTotalSize)
               {
                  iRet = EME_INVALID_DATA;
                  break;
               }
            }
            else
            {
               EME_UTILS_BREAKNULL(pstMailBuffer->pvVal, iRet, EME_INVALID_DATA, "imap get param failed");
               EME_UTILS_BREAKNULL(pstMailSize, iRet, EME_INVALID_DATA, "imap get param failed");
               
               pstIMAPInfo->uiTotalSize = (WE_UINT32)pstMailSize->pvVal;
               pstIMAPInfo->pvAppendMailBuffer = pstMailBuffer->pvVal;
//                pstIMAPInfo->pvAppendMailBuffer = EME_UTILS_MALLOC(pvMemMgr, pstIMAPInfo->uiTotalSize+4);
//                EME_UTILS_BREAKNULL(pstIMAPInfo->pvAppendMailBuffer, iRet, EME_NOMEMORY, "imap nomemory failed");   
//                EME_UTILS_MEMCPY(pstIMAPInfo->pvAppendMailBuffer, pstMailBuffer->pvVal, pstIMAPInfo->uiTotalSize);
            }
            
            pstIMAPInfo->uiCurrentSize = 0;
            pstIMAP->stIMAPSocket.pcSendMailBuffer = EME_UTILS_MALLOC(pvMemMgr, EME_MAX_DATAWRITE_LEN+4);
            EME_UTILS_BREAKNULL(pstIMAP->stIMAPSocket.pcSendMailBuffer, iRet, EME_NOMEMORY, "imap no memory");
            EME_UTILS_MEMSET(pstIMAP->stIMAPSocket.pcSendMailBuffer, 0, EME_MAX_DATAWRITE_LEN+4);
            iRet = EME_IMAP_SendCMDAppend(pstIMAPSocket, 
                                          pcMailboxNameUtf7,
                                          pstIMAPInfo->uiTotalSize,
                                          EME_IMAP_DealAppendResponse);
            break;
         }
      default:
          break;
      }
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_HANDLE_MAIL_END;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      EME_FileClose(pstIMAPInfo->pvFile);
      EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPSocket.pcSendMailBuffer);
   }
   
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pcMailboxNameUtf7);

   return iRet;
}


static WE_VOID EME_IMAP_DealAppendResponse(WE_VOID* hIMAPHandle)
{
   WE_INT32 iResult = EME_FAILED;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_CHAR* pcResponse = NULL;
   if (NULL == pstIMAP)
      return;
   
   pcResponse = pstIMAP->stIMAPSocket.acResponse;
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   
   /* get response */
   EME_IMAP_GET_RESPONSE(pstIMAP, iResult);
   
   /* server is ready for user to send mail data */
   if (pcResponse == EME_UTILS_StrStrNC(pcResponse, "+"))
   {
      pstIMAP->stIMAPInfo.uiCurrentSize = 0;
      EME_IMAP_SendDataBuffer(hIMAPHandle);
   }
   else if (NULL != EME_UTILS_StrStrNC(pcResponse, "[TRYCREATE]"))
   {
      pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_HANDLE_MAIL_END;
      EME_IMAP_ErrorHandle(pstIMAP, EME_MAILBOX_NOT_EXISTS);
   }
   else
   {
      pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_HANDLE_MAIL_END;
      EME_IMAP_ErrorHandle(pstIMAP, EME_SERVER_RESPONSE_ERR);
   }
}


static WE_VOID EME_IMAP_SendDataBuffer(WE_VOID* hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_UINT32 iBytesRead = 0;
   WE_BOOL bIsEnd = FALSE;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo* pstIMAPInfo = NULL;
   WE_INT32 bAppendFromBuffer = FALSE;
   if (NULL == pstIMAP)
      return;
   
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   
   if (NULL == pstIMAPInfo || NULL == pstIMAP->stIMAPSocket.pcSendMailBuffer)
      return;
   
   if(NULL != pstIMAPInfo->pvAppendMailBuffer)
   {
      bAppendFromBuffer = TRUE;
   }
   do
   {
      /*--------------------------------------------------------------------*/
      /*                            get data                                */
      /*--------------------------------------------------------------------*/
      EME_UTILS_MEMSET(pstIMAP->stIMAPSocket.pcSendMailBuffer, 0, EME_MAX_DATAWRITE_LEN);
      if (TRUE == bAppendFromBuffer)
      {
         iBytesRead = pstIMAPInfo->uiTotalSize-pstIMAPInfo->uiCurrentSize;
         /* '-4' means leave enough memory for contain '\r' '\n' '\0' */
         if (iBytesRead > EME_MAX_DATAWRITE_LEN-4)
         {
            iBytesRead = iBytesRead % (EME_MAX_DATAWRITE_LEN-4);
            if (0 == iBytesRead)
               iBytesRead = EME_MAX_DATAWRITE_LEN-4;
         }
      }
      else if( pstIMAPInfo->pvFile)
      {
         /* '-4' means leave enough memory for contain '\r' '\n' '\0' */
         iBytesRead = EME_FileRead(pstIMAPInfo->pvFile, pstIMAP->stIMAPSocket.pcSendMailBuffer, EME_MAX_DATAWRITE_LEN-4);
         if (0 == iBytesRead)
         {
            iRet = EME_FILE_ERROR;
            EME_UTILS_BREAK(iRet, "imap file read error");
         }
      }
      else
      {
         iRet = EME_FAILED;
         EME_UTILS_BREAK(iRet, "imap_wft unknow error occured");
      }
      pstIMAPInfo->uiCurrentSize += iBytesRead;

      /*--------------------------------------------------------------------*/
      /*                      notify transfer size                          */
      /*--------------------------------------------------------------------*/
      EME_IMAP_ResetReturnData(pstIMAP, pstIMAP->stIMAPMgr.pvMemMgr);
      pstIMAP->stIMAPActRet.iResult = iRet;
      pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_TRANSFER_SIZE_NOTIFY;
      pstIMAP->stIMAPActRet.unRetData.stTransferNotify.uiTransferedSize = pstIMAPInfo->uiCurrentSize;
      pstIMAP->stIMAPActRet.unRetData.stTransferNotify.uiTotalSize = pstIMAPInfo->uiTotalSize;
      /* notify result */
      EME_IMAP_Notify(pstIMAP);
      
      /*--------------------------------------------------------------------*/
      /*                      send next data buffer                         */
      /*--------------------------------------------------------------------*/
      if (pstIMAPInfo->uiCurrentSize >= pstIMAPInfo->uiTotalSize)
      {
         pstIMAPInfo->uiCurrentSize = pstIMAPInfo->uiTotalSize;
         if (TRUE == bAppendFromBuffer)
         {
            EME_UTILS_STRCAT((WE_CHAR*)pstIMAPInfo->pvAppendMailBuffer+pstIMAPInfo->uiTotalSize-1, "\r\n");
            iBytesRead += 2;
            pstIMAPInfo->uiCurrentSize += 2;
         }
         else
         {
            EME_UTILS_STRCAT(pstIMAP->stIMAPSocket.pcSendMailBuffer, "\r\n");
         }
         /* register other function to deal send data response */
         EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_DealSendDataResponse);
         bIsEnd = TRUE;
      }
      else
      {
         EME_Socket_RegCbWriteable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_SendDataBuffer);
      }
      
      if (TRUE == bAppendFromBuffer)
      {
         /* pstIMAPInfo->uiCurrentSize-iBytesRead is the actual sended size */
         iRet = EME_Socket_Write(pstIMAP->stIMAPSocket.hSocket,
                                 (WE_CHAR*)pstIMAPInfo->pvAppendMailBuffer+(pstIMAPInfo->uiCurrentSize-iBytesRead),
                                 iBytesRead,
                                 bIsEnd);
      }
      else
      {
         iRet = EME_Socket_Write(pstIMAP->stIMAPSocket.hSocket,
                                 pstIMAP->stIMAPSocket.pcSendMailBuffer,
                                 EME_UTILS_STRLEN(pstIMAP->stIMAPSocket.pcSendMailBuffer),
                                 bIsEnd);
      }
      
      EME_UTILS_BREAKIF(iRet, "imap write socket failed");
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {
      EME_IMAP_ErrorHandle(pstIMAP, iRet);
   }
}


static WE_VOID EME_IMAP_DealSendDataResponse(WE_VOID* hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_INT32 iResult = EME_FAILED;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   WE_CHAR* pcResponse = NULL;
   if (NULL == pstIMAP)
      return;
   
   pcResponse = pstIMAP->stIMAPSocket.acResponse;
   
   do
   {
      EME_IMAP_GET_RESPONSE(pstIMAP, iResult);  
      
      EME_IMAP_BREAK_IF_RESPONSE_END(pcResponse);
   } while(0);
   
   /* free temp buffer which contain sending data when appending mail to server */
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAP->stIMAPSocket.pcSendMailBuffer);
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAP->stIMAPInfo.pvAppendMailBuffer);
   /* reinit some data */
   pstIMAP->stIMAPInfo.uiCurrentSize = 0;
   pstIMAP->stIMAPInfo.uiTotalSize = 0;
   /* close file which contain mail data */
   EME_FileClose(pstIMAP->stIMAPInfo.pvFile);
   
   /* set return result */
   EME_IMAP_ResetReturnData(pstIMAP, pstIMAP->stIMAPMgr.pvMemMgr);
   pstIMAP->stIMAPActRet.iResult = iRet;
   pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_HANDLE_MAIL_END;
   EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
   /* notify result */
   EME_IMAP_Notify(pstIMAP);
}

/*----------------------------------------------------------------------------*/
/*                   handle mailbox, main flow
                           
                          (if expunge, select mailbox)
    EME_IMAP_HandleMailbox ======================> EME_IMAP_SelectMailbox
    (send CMD according operation type)                     |
            |                                               |
            |                                               |
            |(if not expunge)                               |
            |                                               |
            V                                               V
    EME_IMAP_DealInfomationResponse             EME_IMAP_DealInfomationResponse
    (deal CMD response, if operation is LIST     (deal select response and
     or LSUB, notify user about mailboxs got)     send expunge cmd to server)
                                                            |
                                                            |
                                                            V
                                                EME_IMAP_DealInfomationResponse
                                                   (deal expunge response)
                                                                              */
/*----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_HandleMailbox(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_VOID* pvMemMgr = NULL;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPOpt *pstMailbox = NULL;
   St_IMAPOpt *pstMailboxRename = NULL;
   St_IMAPOpt *pstHandleMailboxType = NULL;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   WE_CHAR* pcMailboxUtf7 = NULL;
   WE_CHAR* pcMailboxRenameUtf7 = NULL;
   if (NULL == pstIMAP || NULL == pOpts)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   
   /* make sure user registerd callback and make sure user has connected to server 
   and authenticated and selected a mailbox */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
      && TRUE != pstIMAP->stIMAPStatus.bAuthenticated)
      return EME_INVALID_DATA;
   
   do
   {
      /* get opt from param pOpts for later use */
      pstHandleMailboxType = EME_IMAP_GetOpt(pOpts, EME_IMAP_HANDLE_MAILBOX_TYPE);
      EME_UTILS_BREAKNULL(pstHandleMailboxType, iRet, EME_INVALID_DATA, "imap get handle mailbox type failed");

      if (E_EME_IMAP_LIST_MAILBOXS != (WE_UINT32)pstHandleMailboxType->pvVal
          && E_EME_IMAP_LIST_SUBSCRIBE_MAILBOXS != (WE_UINT32)pstHandleMailboxType->pvVal)
      {
         pstMailbox  = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAILBOX_NAME);
         EME_UTILS_BREAKNULL(pstMailbox, iRet, EME_INVALID_DATA, "imap get mailbox failed");
         EME_UTILS_BREAKNULL(pstMailbox->pvVal, iRet, EME_INVALID_DATA, "imap get mailbox failed");

         pcMailboxUtf7 = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pstMailbox->pvVal)*3+3);
         EME_UTILS_BREAKNULL(pcMailboxUtf7, iRet, EME_NOMEMORY, "IMAP no memory");
         EME_UTILS_MEMSET(pcMailboxUtf7, 0, EME_UTILS_STRLEN(pstMailbox->pvVal)*3+3);
         /* mailbox of IMAP use utf7, so change utf8 to utf7 */
         iRet = EME_IMAP_Utf8ToUtf7(pstMailbox->pvVal, pcMailboxUtf7, pvMemMgr);
         EME_UTILS_BREAKIF(iRet, "IMAP utf8 to utf7 failed");

         if (E_EME_IMAP_RENAME == (WE_UINT32)pstHandleMailboxType->pvVal)
         {
            pstMailboxRename  = EME_IMAP_GetOpt(pOpts, EME_IMAP_MAILBOX_RENAME);
            EME_UTILS_BREAKNULL(pstMailboxRename, iRet, EME_INVALID_DATA, "imap get uid failed");
            EME_UTILS_BREAKNULL(pstMailboxRename->pvVal, iRet, EME_INVALID_DATA, "imap get mailbox failed");

            pcMailboxRenameUtf7 = EME_UTILS_MALLOC(pvMemMgr, EME_UTILS_STRLEN(pstMailboxRename->pvVal)*3+3);
            EME_UTILS_BREAKNULL(pcMailboxRenameUtf7, iRet, EME_NOMEMORY, "IMAP no memory");
            EME_UTILS_MEMSET(pcMailboxRenameUtf7, 0, EME_UTILS_STRLEN(pstMailboxRename->pvVal)*3+3);
            /* mailbox of IMAP use utf7, so change utf8 to utf7 */
            iRet = EME_IMAP_Utf8ToUtf7(pstMailboxRename->pvVal, pcMailboxRenameUtf7, pvMemMgr);
            EME_UTILS_BREAKIF(iRet, "IMAP utf8 to utf7 failed");
         }
      }
     
      switch((WE_UINT32)pstHandleMailboxType->pvVal)
      {
      case E_EME_IMAP_CREATE:
         {
            iRet = EME_IMAP_SendCMDCreate(pstIMAPSocket,
                                          (WE_CHAR*)pstMailbox->pvVal,
                                          EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_CREATE;
            break;
         }
      case E_EME_IMAP_RENAME:
         {
            iRet = EME_IMAP_ReadUIDFileToBuffer(&pstIMAP->stIMAPInfo.stUIDBuf, 
               pstIMAP->stIMAPInfo.pcUIDFilePath);
            EME_UTILS_BREAKIF(iRet, "IMAP read uid file failed");
            EME_IMAP_RenameMailbox(&pstIMAP->stIMAPInfo.stUIDBuf, 
                                          (WE_CHAR*)pstMailbox->pvVal, 
                                          (WE_CHAR*)pstMailboxRename->pvVal);
            iRet = EME_IMAP_SendCMDRename(pstIMAPSocket,
                                          (WE_CHAR*)pstMailbox->pvVal,
                                          (WE_CHAR*)pstMailboxRename->pvVal,
                                          EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_RENAME;
            break;
         }
      case E_EME_IMAP_DELETE:
         {
            iRet = EME_IMAP_ReadUIDFileToBuffer(&pstIMAP->stIMAPInfo.stUIDBuf, 
                                                pstIMAP->stIMAPInfo.pcUIDFilePath);
            EME_UTILS_BREAKIF(iRet, "IMAP read uid file failed");
            EME_IMAP_InvalideMailbox(&pstIMAP->stIMAPInfo.stUIDBuf, (WE_CHAR*)pstMailbox->pvVal);
            iRet = EME_IMAP_SendCMDDelete(pstIMAPSocket,
                                          (WE_CHAR*)pstMailbox->pvVal,
                                          EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_DELETE;
            break;
         }
      case E_EME_IMAP_SUBSCRIBE:
         {
            iRet = EME_IMAP_SendCMDSubscribe(pstIMAPSocket,
                                            (WE_CHAR*)pstMailbox->pvVal,
                                            EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_SUBSCRIBE;
            break;
         }
      case E_EME_IMAP_UNSUBSCRIBE:
         {
            iRet = EME_IMAP_SendCMDUnsubscribe(pstIMAPSocket,
                                              (WE_CHAR*)pstMailbox->pvVal,
                                              EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_UNSUBSCRIBE;
            break;
         }
      case E_EME_IMAP_LIST_MAILBOXS:
         {
            iRet = EME_IMAP_ReadUIDFileToBuffer(&pstIMAP->stIMAPInfo.stUIDBuf, 
                                                pstIMAP->stIMAPInfo.pcUIDFilePath);
            EME_UTILS_BREAKIF(iRet, "IMAP read uid file failed");
            EME_IMAP_InvalideAll(&pstIMAP->stIMAPInfo.stUIDBuf);
            iRet = EME_IMAP_SendCMDList(pstIMAPSocket, EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_LIST;
            break;
         }
      case E_EME_IMAP_LIST_SUBSCRIBE_MAILBOXS:
         {
            iRet = EME_IMAP_SendCMDLsub(pstIMAPSocket, EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_LSUB;
            break;
         }
      case E_EME_IMAP_EXPUNGMSG:
         {
            /* first select mailbox before expunge mailbox */
            iRet = EME_IMAP_SelectMailbox(pstIMAP, pstMailbox->pvVal);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_EXPUNGMSG_SELECT;
            break;
         }
      default:
          break;
      }
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMD failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_HANDLE_MAILBOX_END;
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pcMailboxUtf7);
   EME_UTILS_FREEIF(pvMemMgr, pcMailboxRenameUtf7);

   return iRet;
}

/*----------------------------------------------------------------------------
                           close IMAP flow
   EME_IMAP_CloseIMAP(send logout CMD)
         |
         |
         |
         V
   EME_IMAP_DealInfomationResponse(deal logout response and release relate memory)
  ----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_CloseIMAP(IMAPHandle hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   pstIMAPSocket = &pstIMAP->stIMAPSocket;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;

   /* make sure user registerd callback and make sure user has connected to server */
   if (NULL == pstIMAP->stIMAPCallback.pfnNotify
       && TRUE != pstIMAP->stIMAPStatus.bConnected)
      return EME_INVALID_DATA;
   
   do
   {
      /* register to deal fetch mail response and send fetch mail cmd to get new mail*/
      pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_LOGOUT;
      iRet = EME_IMAP_SendCMDLogout(pstIMAPSocket, EME_IMAP_DealInfomationResponse);
      EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDLogout failed");
      pstIMAP->stIMAPInfo.eTransationType = E_EME_IMAP_CLOSE_IMAP_END;
   } while(0);
   
   if (EME_SUCCESS != iRet)
   {   
      return iRet;
   }
   return EME_SUCCESS;
}


/*----------------------------------------------------------------------------
    deal most response except the data which will be dealed by EME_IMAP_DealFetchResponse
    (fetch mail's header response, fetch mail's response, fetch mail's part response).
    The infomation this function will deal include:
      1. all '* ' prefixed response(include bodystructure and uid string).
      2. all tagged response, prefixed with '-tfMail-'.
  ----------------------------------------------------------------------------*/
static WE_VOID EME_IMAP_DealInfomationResponse(WE_VOID* hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_INT32 iResult = EME_FAILED;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPSocket* pstIMAPSocket = NULL;
   if (NULL == pstIMAP)
      return;
   
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pstIMAPSocket = &pstIMAP->stIMAPSocket;

   do 
   {
      /* get response */
      EME_IMAP_GET_RESPONSE(pstIMAP, iResult);
      
      /* if login not response */
//      if (pstIMAP->stIMAPSocket.acResponse != EME_UTILS_STRSTR(pstIMAP->stIMAPSocket.acResponse, "-tfMail-")
//          && E_EME_IMAP_OPTYPE_LOGIN == pstIMAP->stIMAPInfo.eOperationType)
//      {
//         iRet = EME_LOGIN_ERROR;
//         break;
//      }

      while (1)
      {
         /* if response is end(tagged with "-tfMail-"), set result to iRet and break */
         EME_IMAP_BREAK_IF_RESPONSE_END(pstIMAP->stIMAPSocket.acResponse);
         
         /* if response is not end, save useful infomation */
         iRet = EME_IMAP_SaveResponseData(pstIMAP, pstIMAP->stIMAPSocket.acResponse);
         EME_UTILS_BREAKIF(iRet, "IMAP select response parser error");
         
         /* get response */
         EME_IMAP_GET_RESPONSE(pstIMAP, iResult);
      }
      
   } while(0);
   
   do 
   {
      /* set callback results if no other CMD need send */
      switch(pstIMAP->stIMAPInfo.eOperationType)
      {
      case E_EME_IMAP_OPTYPE_LOGIN:
         {
            iRet = (iRet == EME_SUCCESS) ? EME_SUCCESS : EME_LOGIN_ERROR;
            EME_UTILS_BREAKIF(iRet, "login error");

            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_OPEN_IMAP_END;
            pstIMAP->stIMAPStatus.bAuthenticated = TRUE;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      case E_EME_IMAP_OPTYPE_LOGOUT:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_CLOSE_IMAP_END;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      case E_EME_IMAP_OPTYPE_SELECT_MAILBOX:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_SELECT_MAILBOX_END;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      case E_EME_IMAP_OPTYPE_SEARCH_UID:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            /* if saved uid is equal or larger than max uid on server, it means no new mail */
            if (pstIMAPInfo->uiMaxSavedUID >= pstIMAPInfo->uiMaxDownloadUID)
            {
               iRet = EME_NO_NEWMAIL;
               break;
            }

            /*-------------------------------------------------------------------------
             if there are new mails on server, send fetch CMD to fetch new mails's size
             and register function EME_IMAP_DealInfomationResponse to deal response
             -------------------------------------------------------------------------*/
            pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_SIZE;
            iRet = EME_IMAP_SendCMDFetchSize(pstIMAPSocket,
                                             pstIMAPInfo->uiMaxSavedUID+1, /* '+1' means new messages */
                                             pstIMAPInfo->uiMaxDownloadUID,
                                             EME_IMAP_DealInfomationResponse);
            EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDFetchSize failed");
            /* return and wait for fetching size response if no error occured */
            return;
         }
      case E_EME_IMAP_OPTYPE_FETCH_SIZE:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_GET_INFOMATION_OF_NEW_MAILS_END;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      case E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            /* register to deal fetch header response and send fetch header cmd to get header*/
            pstIMAP->stIMAPInfo.eOperationType = E_EME_IMAP_OPTYPE_FETCH_HEADER;
            iRet = EME_IMAP_SendCMDFetchHeader(pstIMAPSocket, 
                                               pstIMAPInfo->uiCurrentUID,
                                               EME_IMAP_DealFetchResponse);
            EME_UTILS_BREAKIF(iRet, "IMAP EME_IMAP_SendCMDFetchHeader failed");
            /* return and wait for fetching bodystructure response if no error occured */
            return;
         }
      case E_EME_IMAP_OPTYPE_EXPUNGMSG_SELECT:
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            iRet = EME_IMAP_SendCMDExpung(pstIMAPSocket, EME_IMAP_DealInfomationResponse);
            pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_EXPUNGMSG;
         }
         break;
      case E_EME_IMAP_OPTYPE_CREATE: /* fall though */
      case E_EME_IMAP_OPTYPE_RENAME: /* fall though */
      case E_EME_IMAP_OPTYPE_DELETE: /* fall though */
      case E_EME_IMAP_OPTYPE_SUBSCRIBE: /* fall though */
      case E_EME_IMAP_OPTYPE_UNSUBSCRIBE: /* fall though */
      case E_EME_IMAP_OPTYPE_LIST: /* fall though */
      case E_EME_IMAP_OPTYPE_LSUB: /* fall though */
      case E_EME_IMAP_OPTYPE_EXPUNGMSG: /* fall though */
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            if (E_EME_IMAP_OPTYPE_LIST == pstIMAP->stIMAPInfo.eOperationType
               || E_EME_IMAP_OPTYPE_DELETE == pstIMAP->stIMAPInfo.eOperationType
               || E_EME_IMAP_OPTYPE_RENAME == pstIMAP->stIMAPInfo.eOperationType)
            {
               iRet = EME_IMAP_WriteUIDBufferToFile(&pstIMAP->stIMAPInfo.stUIDBuf, 
                                                    pstIMAP->stIMAPInfo.pcUIDFilePath);
            }
            EME_UTILS_BREAKIF(iRet, "imap uid operate failed");
            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_HANDLE_MAILBOX_END;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      case E_EME_IMAP_OPTYPE_MARK_DELETED: /* fall though */
      case E_EME_IMAP_OPTYPE_UNMARK_DELETED: /* fall though */
      case E_EME_IMAP_OPTYPE_MARK_SEEN: /* fall though */
      case E_EME_IMAP_OPTYPE_UNMARK_SEEN: /* fall though */
      case E_EME_IMAP_OPTYPE_APPEND: /* fall though */
      case E_EME_IMAP_OPTYPE_COPY: /* fall though */
         {
            /* is response is not ok, break and notify */
            EME_UTILS_BREAKIF(iRet, "imap failed");

            pstIMAP->stIMAPActRet.eEvtCode = E_EME_IMAP_HANDLE_MAIL_END;
            EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);
            break;
         }
      default:
         pstIMAPInfo->eOperationType = E_EME_IMAP_OPTYPE_NO_OPERATION;
          break;
      }
   } while(0);
   
   pstIMAP->stIMAPActRet.iResult = iRet;
   /* notify result */
   if (EME_SUCCESS != iRet)
   {
      EME_IMAP_ErrorHandle(pstIMAP, iRet);
   }
   else
   {
      EME_IMAP_Notify(pstIMAP);
   }

   if (E_EME_IMAP_OPTYPE_LOGOUT == pstIMAP->stIMAPInfo.eOperationType)
   {
      EME_IMAP_Cancel(pstIMAP);
   }
}

/*-------------------------------------------------------------------------------
   This function is mainly used to deal fetch response, include 
   fetch header(not include bodystructure), fetch mail(include fetch specific mail),
   fetch mail's part. This function save the response data of above operation and 
   notify user. 
   -------------------------------------------------------------------------------*/
static WE_VOID EME_IMAP_DealFetchResponse(WE_VOID* hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   WE_INT32 iResult = EME_FAILED;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_IMAPActRet *pstIMAPActRet = NULL;
   St_IMAPHeader *pstHeader = NULL;
   St_IMAPMail   *pstMail = NULL;
   St_IMAPMailPart *pstMailPart = NULL;

   if (NULL == pstIMAP)
      return;
   
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   pstIMAPActRet = &pstIMAP->stIMAPActRet;
   pstHeader = &pstIMAPActRet->unRetData.stHeader;
   pstMail = &pstIMAPActRet->unRetData.stMail;
   pstMailPart = &pstIMAPActRet->unRetData.stMailPart;
   
   do 
   {
      /* get response */
      EME_IMAP_GET_RESPONSE(pstIMAP, iResult);
      
      while (1)
      {
         /* if response is end(tagged with "-tfMail-"), set result to iRet and break */
         EME_IMAP_BREAK_IF_RESPONSE_END(pstIMAP->stIMAPSocket.acResponse);
         
         /* if response is not end, save useful infomation */
         iRet = EME_IMAP_SaveResponseData(pstIMAP, pstIMAP->stIMAPSocket.acResponse);
         EME_UTILS_BREAKIF(iRet, "IMAP select response parser error");
         
         /* if response is not untagged response and not continue request, save mail's data */
         iRet = EME_IMAP_SaveMailData(pstIMAP, pstIMAP->stIMAPSocket.acResponse);
         EME_UTILS_BREAKIF(iRet, "IMAP select response parser error");
         
         /* get response */
         EME_IMAP_GET_RESPONSE(pstIMAP, iResult);
      }
   } while(0);

   /* set callback results */
   pstIMAPActRet->iResult = iRet;

   EME_Socket_RegCbReadable(pstIMAP->stIMAPSocket.hSocket, (WE_VOID*)EME_IMAP_IgnoreMessage);

   /*head file*/
   if(pstIMAPInfo->stHeader.uiHadUsedSize > 0)
   {
      EME_FileWrite(pstIMAPInfo->stHeader.pvTmpFile, pstIMAPInfo->stHeader.pvCatchBuf,
         pstIMAPInfo->stHeader.uiHadUsedSize);
   }
   EME_FileClose(pstIMAPInfo->stHeader.pvTmpFile);
   pstIMAPInfo->stHeader.pvTmpFile = NULL;
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAPInfo->stHeader.pvCatchBuf);
   pstIMAPInfo->stHeader.uiHadUsedSize = 0;

   /*body structure*/
   if(pstIMAPInfo->stBodystructure.uiHadUsedSize > 0)
   {
      EME_FileWrite(pstIMAPInfo->stBodystructure.pvTmpFile, pstIMAPInfo->stBodystructure.pvCatchBuf,
         pstIMAPInfo->stBodystructure.uiHadUsedSize);
   }
   EME_FileClose(pstIMAPInfo->stBodystructure.pvTmpFile);
   pstIMAPInfo->stBodystructure.pvTmpFile = NULL;
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAPInfo->stBodystructure.pvCatchBuf);
   pstIMAPInfo->stBodystructure.uiHadUsedSize = 0;

   /*the whole maile*/
   if(pstIMAPInfo->uiUsedBufSize > 0)
   {
      EME_FileWrite(pstIMAPInfo->pvTmpFile, pstIMAPInfo->pvCatchBuf, pstIMAPInfo->uiUsedBufSize);
   }
   EME_FileClose(pstIMAPInfo->pvTmpFile);
   pstIMAPInfo->pvTmpFile = NULL;
   EME_UTILS_FREEIF(pstIMAP->stIMAPMgr.pvMemMgr, pstIMAPInfo->pvCatchBuf);
   pstIMAPInfo->uiUsedBufSize = 0;

   if (EME_SUCCESS != iRet)
   {
      EME_IMAP_ErrorHandle(pstIMAP, iRet);
      return;
   }
   switch(pstIMAPInfo->eOperationType)
   {
   case E_EME_IMAP_OPTYPE_FETCH_HEADER:
      {

         pstIMAPActRet->eEvtCode = pstIMAPInfo->eTransationType;
         pstHeader->pvHeader = pstIMAPInfo->pcHeader = pstIMAPInfo->stHeader.pcFileName;
         pstHeader->pvBodystructure = pstIMAPInfo->pcBodystructure = pstIMAPInfo->stBodystructure.pcFileName;
         pstIMAPInfo->stHeader.pcFileName = NULL;
         pstIMAPInfo->stBodystructure.pcFileName = NULL;
         
         pstHeader->uiUID = pstIMAPInfo->uiCurrentUID;
         pstHeader->uiMailSize = pstIMAPInfo->uiTotalSize;
         if (TRUE == pstIMAPInfo->iSizeLimitExceed)
         {
            pstIMAPActRet->iResult = EME_SIZE_LIMIT_EXCEED;
         }
      }
       break;
   case E_EME_IMAP_OPTYPE_FETCH_MAIL:
      {
         pstIMAPActRet->eEvtCode = E_EME_IMAP_FETCH_MAIL_END;

         /* pstIMAPInfo->pcMail will be freed after callback return
            but pstMail->pvWholeMail will do nothing */
         pstMail->pvWholeMail = pstIMAPInfo->pcMail = pstIMAPInfo->pcFileName;
         pstIMAPInfo->pcFileName = NULL;

         pstMail->uiUID = pstIMAPInfo->uiCurrentUID;
         pstMail->uiMailSize = pstIMAPInfo->uiTotalSize;
      }
   	  break;
   case E_EME_IMAP_OPTYPE_FETCH_SPECIFIC_MAIL:
      {
         pstIMAPActRet->eEvtCode = E_EME_IMAP_FETCH_SPECIFIC_MAIL_END;

         /* pstIMAPInfo->pcSpecificMail will be freed after callback return
            but pstMail->pvWholeMail will do nothing */
         pstMail->pvWholeMail = pstIMAPInfo->pcSpecificMail = pstIMAPInfo->pcFileName;
         pstIMAPInfo->pcFileName = NULL;

         pstMail->uiUID = pstIMAPInfo->uiSpecificUID;
         pstMail->uiMailSize = pstIMAPInfo->uiTotalSize;
      }
   	  break;
   case E_EME_IMAP_OPTYPE_FETCH_MAILPART:
      {
         pstIMAPActRet->eEvtCode = E_EME_IMAP_FETCH_SPECIFIC_PART_END;

         pstMailPart->pvMailPart = pstIMAPInfo->pcMailPart = pstIMAPInfo->pcFileName;
         pstIMAPInfo->pcFileName = NULL;

         pstMailPart->uiUID = pstIMAPInfo->uiSpecificUID;
         pstMailPart->uiPartSize = pstIMAPInfo->uiTotalSize;      
      }
       break;
   default:
       break;
   }
   
   pstIMAPInfo->uiCurrentSize = 0;
   pstIMAPInfo->uiTotalSize = 0;

   /* notify result */
   EME_IMAP_Notify(pstIMAP);
}

/*----------------------------------------------------------------------------------
   save the UID of downloaded mail, in fact, it only save the max downloaded mail's UID
   of the mailbox 
   --------------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_SaveUID(IMAPHandle hIMAPHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo *pstIMAPInfo = NULL;
   St_UIDItem stUIDItem = {0};
   
   pstIMAPInfo = &pstIMAP->stIMAPInfo;

   if (E_EME_IMAP_FETCH_HEADER_END != pstIMAPInfo->eTransationType
       && E_EME_IMAP_FETCH_MAIL_END != pstIMAPInfo->eTransationType)
   {
      return EME_INVALID_DATA;
   }

   do
   {
      /* save uid downloaded in buffer */
      stUIDItem.pcMailboxName = pstIMAPInfo->pcMailboxName;
      stUIDItem.uiMaxUID = pstIMAPInfo->uiCurrentUID;
      stUIDItem.iBeValide = TRUE;
      iRet = EME_IMAP_AddToUIDBuffer(&pstIMAPInfo->stUIDBuf, &stUIDItem);
      EME_UTILS_BREAKIF(iRet, "imap add uid to buffer failed");

      /* save uid downloaded in file */
      iRet = EME_IMAP_WriteUIDBufferToFile(&pstIMAPInfo->stUIDBuf, pstIMAPInfo->pcUIDFilePath);
      EME_UTILS_BREAKIF(iRet, "imap write uid to file failed");
   } while(0);
   
   return iRet;
}


/*----------------------------------------------------------------------------
                           cancel operation
  ----------------------------------------------------------------------------*/
WE_INT32 EME_IMAP_Cancel(IMAPHandle hIMAPHandle)
{
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo* pstIMAPInfo = NULL;
   PFNIMAPNOTIFY pfnNotify = NULL;
   WE_VOID* pvClientData = NULL;
   WE_VOID* pvMemMgr = NULL;
   WE_VOID* pvFileMgr = NULL;
   WE_VOID* pvIShell = NULL;
   WE_VOID* pvNetMgr = NULL;
	WE_VOID* pvAsyncMgr = NULL;

   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   /* register callback */
   pfnNotify = pstIMAP->stIMAPCallback.pfnNotify;
   pvClientData = pstIMAP->stIMAPCallback.pvClientData;

   pvMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   pvFileMgr = pstIMAP->stIMAPMgr.pvFileMgr;
   pvIShell = pstIMAP->stIMAPMgr.pvIShell;
   pvNetMgr = pstIMAP->stIMAPMgr.pvNetMgr;
	pvAsyncMgr = pstIMAP->stIMAPMgr.pvAsyncMgr;

   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   
   EME_Socket_Close(pstIMAP->stIMAPSocket.hSocket);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPAccountInfo.pcPassword);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPAccountInfo.pcUIDFilePath);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPAccountInfo.pcUserName);
   
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcBodystructure);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcHeader);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMail);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMailboxName);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMailPart);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvCatchBuf);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcFileName);
   EME_FileClose(pstIMAPInfo->pvTmpFile);
   pstIMAPInfo->pvTmpFile = NULL;
   
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcSpecificMail);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcUIDFilePath);
   
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvMail);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvMailPart);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvSpecificMail);
   
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stBodystructure.pcLongString);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stBodystructure.pvCatchBuf);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stBodystructure.pcFileName);
   EME_FileClose(pstIMAPInfo->stBodystructure.pvTmpFile);
   pstIMAPInfo->stBodystructure.pvTmpFile = NULL;;

   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stHeader.pcLongString);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stHeader.pvCatchBuf);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stHeader.pcFileName);
   EME_FileClose(pstIMAPInfo->stHeader.pvTmpFile);
   pstIMAPInfo->stHeader.pvTmpFile = NULL;;

   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->stUIDString.pcLongString);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvAppendMailBuffer);

   EME_UTILS_FREEIF(pvMemMgr, pstIMAP->stIMAPSocket.pcSendMailBuffer);
   EME_FileClose(pstIMAPInfo->pvFile);
   pstIMAPInfo->pvFile = NULL;
   EME_IMAP_ReleaseMailInfoArray(&pstIMAPInfo->stMailInfoArray);
   EME_IMAP_FreeUIDBuf(&pstIMAPInfo->stUIDBuf);

   EME_UTILS_MEMSET(pstIMAP, 0, sizeof(St_IMAP));

   do
   {
      /* init mgr */
      pstIMAP->stIMAPMgr.pvMemMgr = pvMemMgr;
      pstIMAP->stIMAPMgr.pvFileMgr = pvFileMgr;
      pstIMAP->stIMAPMgr.pvIShell = pvIShell;
      pstIMAP->stIMAPMgr.pvNetMgr = pvNetMgr;
		pstIMAP->stIMAPMgr.pvAsyncMgr = pvAsyncMgr;
      pstIMAPInfo = &pstIMAP->stIMAPInfo;
      /*init body structure*/
      pstIMAPInfo->stBodystructure.pMemHdr = pvMemMgr;
      pstIMAPInfo->stBodystructure.pvFileMgr = pvFileMgr;
      /*init UID string*/
      pstIMAPInfo->stUIDString.pMemHdr = pvMemMgr;
      pstIMAPInfo->stUIDString.pvFileMgr = pvFileMgr;
      /*init Header*/
      pstIMAPInfo->stHeader.pMemHdr = pvMemMgr;
      pstIMAPInfo->stHeader.pvFileMgr = pvFileMgr;
      /*init UID buffer*/
      pstIMAPInfo->stUIDBuf.pMemHdr = pvMemMgr;
      pstIMAPInfo->stUIDBuf.pFileMgr = pvFileMgr;
      /*init  mailinfo array*/
      pstIMAPInfo->stMailInfoArray.pMemHdr = pvMemMgr;
      /* register callback */
      pstIMAP->stIMAPCallback.pfnNotify = pfnNotify;
      pstIMAP->stIMAPCallback.pvClientData = pvClientData;
   } while(0);

   return EME_SUCCESS;
}


WE_INT32 EME_IMAP_DeleteHandle(IMAPHandle* phIMAPHandle)
{
   St_IMAP* pstIMAP = (St_IMAP*)(*phIMAPHandle);
   St_IMAPInfo* pstIMAPInfo = NULL;
   void* pMemMgr = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;
   
   pMemMgr = pstIMAP->stIMAPMgr.pvMemMgr;
   pstIMAPInfo = &pstIMAP->stIMAPInfo;
   EME_IMAP_SaveUID(*phIMAPHandle);
   EME_IMAP_Cancel(pstIMAP);
   EME_Socket_Close(pstIMAP->stIMAPSocket.hSocket);
   EME_UTILS_FREEIF(pMemMgr, pstIMAP);

   *phIMAPHandle = NULL;
   return EME_SUCCESS;
}


static WE_INT32 EME_IMAP_ResetReturnData(IMAPHandle hIMAPHandle, void* pvMemMgr)
{
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   St_IMAPInfo* pstIMAPInfo = NULL;
   if (NULL == pstIMAP)
      return EME_INVALID_DATA;

   pstIMAPInfo = &pstIMAP->stIMAPInfo;

   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcBodystructure);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcHeader);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMail);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcMailPart);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcSpecificMail);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pcFileName);
   EME_UTILS_FREEIF(pvMemMgr, pstIMAPInfo->pvCatchBuf);
   EME_FileClose(pstIMAPInfo->pvTmpFile);
   pstIMAPInfo->pvTmpFile = NULL;
   pstIMAPInfo->uiUsedBufSize = 0;

//    EME_UTILS_MEMSET(pstIMAP->stIMAPSocket.acResponse, 0, EME_IMAP_MAX_STRING_LENGTH);
   EME_UTILS_MEMSET(&pstIMAP->stIMAPActRet, 0, sizeof(St_IMAPActRet));

   return EME_SUCCESS;
}

static St_IMAPOpt* EME_IMAP_GetOpt(St_IMAPOpt *pstOpts, WE_INT32 iOptID)
{
   WE_INT32 iIndex = 0;
   if (NULL == pstOpts)
      return NULL;
   
   while (iOptID != pstOpts[iIndex].iId)
   {
      switch(pstOpts[iIndex].iId)
      {
      case EME_IMAP_SERVER_ADDR: /* fall throught */
      case EME_IMAP_SERVER_PORT: /* fall throught */
      case EME_IMAP_USERNAME: /* fall throught */
      case EME_IMAP_PASSWORD: /* fall throught */
      case EME_IMAP_UID_FILE_PATH: /* fall throught */
      case EME_IMAP_SIZE_LIMIT: /* fall throught */
      case EME_IMAP_MAILBOX_NAME: /* fall throught */
      case EME_IMAP_MAILBOX_RENAME:/* fall throught */
      case EME_IMAP_UID: /* fall throught */
      case EME_IMAP_HANDLE_MAIL_TYPE: /* fall throught */
      case EME_IMAP_HANDLE_MAILBOX_TYPE: /* fall throught */
      case EME_IMAP_MAIL_BUFFER_SIZE:/* fall throught */
      case EME_IMAP_MAIL_BUFFER:/* fall throught */
      case EME_IMAP_MAIL_PATH:/* fall throught */
      case EME_IMAP_NUMBER_LIMIT:/* fall throught */
      case EME_IMAP_IS_NEED_SSL:/* fall throught */
         break;
         
      default:
         return NULL;
         break;
      }
      iIndex++;
   }
   
   return &pstOpts[iIndex];
}


static WE_VOID EME_IMAP_NetAvailable(WE_VOID* hIMAPHandle)
{
   /* do nothing */
   return;
}


static WE_VOID EME_IMAP_IgnoreMessage(WE_VOID* hIMAPHandle)
{
   WE_INT32 iResult = EME_FAILED;

   while (1)
   {
      /* get all response message and do nothing until no message got */
      EME_IMAP_GET_RESPONSE(hIMAPHandle, iResult);
   }
}


static WE_VOID EME_IMAP_ErrorHandle(WE_VOID* hIMAPHandle, WE_INT32 pcErrorReason)
{
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   WE_VOID* pvMemMgr = NULL;
   St_IMAPServerResponse *pstIMAPServerResponse;

   if (NULL == hIMAPHandle)
      return;

   EME_IMAP_ResetReturnData(pstIMAP, pvMemMgr);

   EME_UTILS_MEMSET(pstIMAP->stIMAPActRet.acRespDesc, 0, 512);

   if(EME_UTILS_STRLEN(pstIMAP->stIMAPSocket.acResponse) > 0)
   {
      EME_UTILS_MEMCPY(pstIMAP->stIMAPActRet.acRespDesc, pstIMAP->stIMAPSocket.acResponse, 
         EME_UTILS_STRLEN(pstIMAP->stIMAPSocket.acResponse));
   }
   EME_UTILS_MEMSET(pstIMAP->stIMAPSocket.acResponse, 0, EME_IMAP_MAX_STRING_LENGTH);

   pstIMAPServerResponse = &pstIMAP->stIMAPActRet.unRetData.stServerResponse;

   switch(pcErrorReason)
   {
   case EME_SOCKET_SHUTDOWN:
      {
         pstIMAP->stIMAPActRet.iResult = EME_SOCKET_SHUTDOWN;
         EME_UTILS_MEMSET(&pstIMAP->stIMAPStatus, 0, sizeof(St_IMAPStatus));
         break;
      }
   case EME_MAILBOX_NOT_EXISTS:/* fall thought */
   case EME_SERVER_RESPONSE_ERR:
      {
         pstIMAP->stIMAPActRet.iResult = EME_SERVER_RESPONSE_ERR;
         pstIMAPServerResponse->pcResponse = pstIMAP->stIMAPSocket.acResponse;
         pstIMAPServerResponse->pcCMD = pstIMAP->stIMAPSocket.acCMD;
         break;
      }
   default:
      {
         pstIMAP->stIMAPActRet.iResult = pcErrorReason;
         break;
      }
   }

   pstIMAP->stIMAPActRet.eEvtCode = pstIMAP->stIMAPInfo.eTransationType;
   EME_IMAP_Notify(pstIMAP);
   return;
}


static WE_VOID EME_IMAP_Notify(WE_VOID* hIMAPHandle)
{
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   if (NULL == hIMAPHandle)
      return;

   if (NULL != pstIMAP->stIMAPCallback.pfnNotify)
   {
      pstIMAP->stIMAPCallback.pfnNotify(pstIMAP->stIMAPCallback.pvClientData,
         &pstIMAP->stIMAPActRet);
   }
   
   //EME_IMAP_ResetReturnData(pstIMAP, pstIMAP->stIMAPMgr.pvMemMgr);
   return;
}


static WE_INT32 EME_IMAP_GetResponse(WE_VOID* hIMAPHandle)
{
   St_IMAP* pstIMAP = (St_IMAP*)hIMAPHandle;
   HSocket hSocket = NULL;
   WE_UINT32 uiReadSize = 0;
   if (NULL == hIMAPHandle)
      return EME_INVALID_DATA;
   
   hSocket = pstIMAP->stIMAPSocket.hSocket;

   EME_UTILS_MEMSET(pstIMAP->stIMAPSocket.acResponse, 0, EME_IMAP_MAX_STRING_LENGTH);
   return EME_Socket_Read(hSocket,
                          pstIMAP->stIMAPSocket.acResponse,
                          EME_IMAP_MAX_STRING_LENGTH,
                          &uiReadSize);
}