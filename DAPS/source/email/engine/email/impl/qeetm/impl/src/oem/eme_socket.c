/*=====================================================================================
FILE NAME   : eme_socket.c
MODULE NAME : Email engine

GENERAL DESCRIPTION
Provide functions for communicating with server.

TechFaith Software Confidential Proprietary
Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
Revision History

Modification                   Tracking
Date              Author       Number       Description of changes
----------   --------------   ---------   --------------------------------------
2007-03-02     Daniel                       create eme_socket.c

=====================================================================================*/
//pclint
/*lint -e119 */
/*lint -e611 */
/*lint -e752 */

#include "eme_socket_api.h"
#include "eme_utils_api.h"
#include "eme_error.h"
#include "oem_soc_api.h"
#include "eme_callback.h"
#include "oem_ppp_api.h"
#include "oem_asyncproc_api.h"
#include "itls.h"

/* global field for socket connect. */
#define EME_MAX_INBUF_LEN        10240
#define EME_MAX_OUTBUF_LEN       EME_MAX_DATAWRITE_LEN
#define EME_MAX_CONN_RETRY       2
#define EME_MAX_IPADDR_LEN		   20
#define EME_MAX_TLSIP_LEN		   6
#define EME_TLS_PORT             995

typedef struct tagSt_DataList
{
   /*WE_CHAR *pcBufferData;*/
   WE_CHAR acBufferData[EME_MAX_INBUF_LEN];
   WE_INT32 iBufferLen;
   WE_INT32 iReadLen;
   struct tagSt_DataList *next;
}St_DataList;

typedef struct tagSt_WriteDataList
{
   WE_CHAR acWriteData[EME_MAX_OUTBUF_LEN * 2];
   WE_INT32 iBufferLen;
   WE_INT32 iWriteLen;
   struct tagSt_WriteDataList *next;
}St_WriteDataList;

typedef struct Socket_t
{     
   HOEMSOCKETHANDLE phOEMSocketHdl; //Socket operate handler

   WE_VOID        *pvUser;          // user's handler   
   WE_VOID        *pvMemMgr;
   WE_VOID        *pvAsyncMgr;

   WE_CHAR        *pcHostName;
   WE_INT32       iRetryTimes;

   WE_CHAR        acInBuf[EME_MAX_INBUF_LEN];      //buffer for read
   WE_CHAR        acOutBuf[EME_MAX_OUTBUF_LEN];    //buffer for write

   WE_INT32       iOutBufSize;         //write buffer's size
   WE_INT32       iWrittenSize;        //written size in write buffer 
   WE_INT32       iInBufSize;          //read size from server to socket one time
   WE_INT32       iReadSize;           //read size from socket to user's buffer one time

   FUNCCONNECTIONOK  pfnCbConnectionOk;   //point to callback functions
   FUNCREADABLE      pfnCbReadable;
   FUNCWRITEABLE     pfnCbWriteable;
   FUNCERRORHANDLER  pfnCbErrorHandler;

   //   AEECallback    stCbLookup;       //the struct specifies the data and functions for a callback registered 
   OEMDNSResult   stDnsResult;      //the result of an INETMGR_GetHostByName() operation

   AEECallback    m_CB;             //need other encapsulation?????????

   ITls			   *pITls;

   St_DataList    *pstDataListHead;
   St_DataList    *pstDataListTail;

   St_WriteDataList *pstWriteDataList;

   INetPort       usPort;           //the port connected.  
   WE_BOOL        bUserCancel;      //?? UserCancel
   WE_BOOL        bSSLSupported;    //if SSL is supported : True or False 
   WE_BOOL        bSSLUsed;         //define socket use ssl handle to read or write
   WE_BOOL        bStartTls;

   WE_BOOL		  bSSLHandeShake;
   WE_CHAR        acRes[3];  
} St_Socket;

static WE_VOID Socket_GetHostCB(WE_VOID * pvSock);     
static WE_VOID Socket_ConnectCB(WE_VOID * pvUser, WE_INT iError);
static WE_VOID Socket_ReadCB(WE_VOID* pvSock);
static WE_VOID Socket_WriteCB(WE_VOID* pvSock);
static WE_VOID Socket_WriteCB_NoEnd(WE_VOID* pvSock);
static WE_VOID Socket_TimeOut(WE_VOID *pvData);

static WE_VOID Socket_TLSCB_Handeshake(E_TLS_DATA_TYPE eDataType, void* pvResp, WE_HANDLE hPrivData);
static WE_VOID Socket_ReadCB_Tls(WE_VOID* pvSock);
static WE_VOID Socket_WriteCB_Tls(WE_VOID* pvSock);
static WE_INT32 Socket_AddDataNode(WE_VOID* pvSock, WE_CHAR *pcData, WE_INT32 iDataLen);
static WE_VOID Socket_DelDataNode(WE_VOID* pvSock, WE_BOOL bAll);
/************************************************************************/
/* CreateSocket                                                         */
/* we need IShell obj to create instance about network, so pObj do this */
/************************************************************************/
HSocket EME_Socket_Create(WE_VOID* piShell, 
                          WE_VOID* piAsyncMgr,
                          WE_VOID* piNetMgr, 
                          WE_VOID* pvUser, 
                          WE_VOID* pvMemMgr, 
                          WE_INT32 iSSLType)
{
   St_Socket *pstSock = NULL;
   WE_INT32 iResult = EME_SUCCESS;

   EME_UTILS_LOG_INFO(("EME_Socket_Create : START!"));
   if (pvUser == NULL)
   {
      EME_UTILS_LOG_INFO(("EME_Socket_Create : BADPARAM!"));
      return NULL;
   }

   do 
   {
      // init the vars
      pstSock = (St_Socket *)EME_UTILS_MALLOC(pvMemMgr, (sizeof(St_Socket)));  
      EME_UTILS_LOG_INFO(("EME_Socket_Create : pstSock = %x", pstSock));
      if (pstSock == NULL)
      {
         EME_UTILS_LOG_INFO(("EME_Socket_Create :  MALLOC pstSock FAILED!"));
         iResult = EME_FAILED;
         break;   //malloc failed
      }
      EME_UTILS_MEMSET(pstSock, 0x00, sizeof(St_Socket));
      pstSock->pvAsyncMgr = piAsyncMgr;

      if(EME_SSL_TYPE_SSLTLS == iSSLType || EME_SSL_TYPE_STARTTLS == iSSLType)
      {
         iResult = ITls_ClsCreateInstance(&pstSock->pITls);
         if (NULL == pstSock->pITls || SUCCESS != iResult)
         {
            iResult = EME_FAILED;
            break; 
         }

         if(EME_SSL_TYPE_STARTTLS == iSSLType)
         {
            pstSock->bStartTls = TRUE;
         }
         
         pstSock->bSSLUsed = FALSE;
         pstSock->bSSLSupported = TRUE;
      }

      iResult = OEM_CreateSocHandle(&pstSock->phOEMSocketHdl);
      EME_UTILS_LOG_INFO(("OEM_OpenSocket: OEM_CreateSocHandle iret = %d !", iResult));
      EME_UTILS_BREAKIF(iResult, "OEM_OpenSocket :OEM_CreateSocHandle failed ! ");

      pstSock->pvUser = pvUser;
      pstSock->pvMemMgr = pvMemMgr;
   } while(0);

   if (EME_SUCCESS != iResult)
   {
      if (NULL != pstSock && NULL != pstSock->pITls)
      {
         ITls_Release(pstSock->pITls);
         pstSock->pITls = NULL;
      }

      EME_UTILS_FREEIF(pvMemMgr, pstSock);
      OEM_FreeSocHandle(&pstSock->phOEMSocketHdl);
      EME_UTILS_LOG_INFO(("EME_Socket_Create : RETURN :%d!", iResult));
      return (HSocket)NULL;
   }
   EME_UTILS_LOG_INFO(("EME_Socket_Create : RETURN :%d!", iResult));
   return (HSocket)pstSock;
}

/* utility function */
/************************************************************************/
/*  SocketConnection                  */
/*  Socket Connection/Disconnection             */
/************************************************************************/
WE_INT32 EME_Socket_GetHostByName(HSocket pvSocket)
{
   St_Socket *pstSock = (St_Socket*)pvSocket;
   if (NULL == pstSock)
   {
      return EME_BADPARM;
   }

   return  OEM_GetHostByName(pstSock->phOEMSocketHdl, pstSock->pcHostName, &pstSock->stDnsResult, Socket_GetHostCB,pstSock);
}
WE_INT32 EME_Socket_Connection(HSocket pvSocket, const WE_CHAR *pcHostName, WE_UINT16 usPort)
{
   // local variable definition
   St_Socket   *pstSock = (St_Socket *)pvSocket;
   WE_INT32     nProfId  = 1;
   EME_UTILS_LOG_INFO(("EME_Socket_Connection : START!!"));
   EME_UTILS_LOG_INFO(("EME_Socket_Connection : START!!"));
   // params check
   if (pstSock == NULL || pcHostName == NULL)
   {      
      EME_UTILS_LOG_INFO(("EME_Socket_Connection : BADPARAM!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_Connection : BADPARAM!!"));
      return EME_SOCKET_INVALID_DATA;
   }   

   /* Initialize the CB structure */    
   EME_UTILS_FREEIF(pstSock->pvMemMgr, pstSock->pcHostName);
   pstSock->pcHostName = EME_UTILS_STRDUP(pstSock->pvMemMgr, (WE_CHAR*)pcHostName);
   pstSock->pcHostName = EME_UTILS_STRLOWER(pstSock->pcHostName);

   EME_UTILS_LOG_INFO(("EME_Socket_Connection : pcHostName = %s!!", pstSock->pcHostName));

   if (pstSock->pcHostName == NULL)
   {
      EME_UTILS_LOG_INFO(("EME_Socket_Connection : pcHostName BADPARAM!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_Connection :pcHostName  BADPARAM!!"));
      return EME_NOMEMORY;
   }
   pstSock->iRetryTimes = EME_MAX_CONN_RETRY;
   pstSock->usPort = usPort;

   EME_UTILS_LOG_INFO(("EME_Socket_Connection : usPort = %d!!",    pstSock->usPort));
   EME_UTILS_LOG_INFO(("EME_Socket_Connection : usPort!!",    pstSock->usPort));

   OEM_GetHostByName(pstSock->phOEMSocketHdl, pstSock->pcHostName, &pstSock->stDnsResult, Socket_GetHostCB, pstSock);   
   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket connection host: %s", pstSock->pcHostName));

//   EME_Socket_GetHostByName(pvSocket);
   pstSock->iRetryTimes--;

   return EME_SUCCESS;
}

/************************************************************************/
/* SocketClose */
/* WE_VOID EME_Socket_Close (St_Socket *pstSock);        */
/************************************************************************/
typedef struct St_TlsCB
{
   AEECallback stCallback;
   ITls        *pITls;
}St_TlsCB;

static void EME_Socket_ReleaseTls(void *pv)
{
   St_TlsCB *pMe = (St_TlsCB*)pv;
   
   ITls_Release(pMe->pITls);
   
   EME_UTILS_FREEIF(NULL, pMe);
}

WE_INT32 EME_Socket_Close(HSocket pvSocket)
{   
   // local variable definition
   WE_INT32 nDialId = 0;
   St_Socket *pstSock = (St_Socket *)pvSocket;

   EME_UTILS_LOG_INFO(("EME_Socket_Close   :START!!"));
   EME_UTILS_LOG_INFO(("EME_Socket_Close   :START!!"));

   // params check
   if (pstSock == NULL)
   {      
      EME_UTILS_LOG_INFO(("EME_Socket_Close   :BADPARAM!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_Close   :BADPARAM!!"));

      return EME_SOCKET_INVALID_DATA;
   }   

   EME_UTILS_LOG_INFO(("EME_Socket_Close   :pcHostName = %s!!",  pstSock->pcHostName));
   EME_UTILS_LOG_INFO(("EME_Socket_Close   :pcHostName = %s!!",  pstSock->pcHostName));

   EME_UTILS_FREEIF(pstSock->pvMemMgr, pstSock->pcHostName);

   EME_UTILS_LOG_INFO(("EME_Socket_Close   :OEM_CloseSocket hsocket = %d!!",  pstSock->phOEMSocketHdl));
   EME_UTILS_LOG_INFO(("EME_Socket_Close   :OEM_CloseSocket hsocket = %d!!",  pstSock->phOEMSocketHdl));
   
   OEM_CloseSocket(pstSock->phOEMSocketHdl);

   /*删除TLS接口必须以异步的方式进行
     2011.03.04 jade*/
   if (pstSock->pITls != NULL)
   {
      St_TlsCB *pstCallBack = EME_UTILS_MALLOC(NULL, sizeof(St_TlsCB));
      
      ITls_RegClientEvtCB(pstSock->pITls, pstSock, (WE_HANDLE)NULL);
      
      if(NULL != pstCallBack)
      {
         EME_UTILS_MEMSET(pstCallBack, 0x00, sizeof(St_TlsCB));
         pstCallBack->pITls = pstSock->pITls;
         CALLBACK_Init(&pstCallBack->stCallback, EME_Socket_ReleaseTls, pstCallBack);
         ISHELL_EmailResume(&pstCallBack->stCallback);
      }
      pstSock->pITls = NULL;
   }
   
   Socket_DelDataNode(pstSock, TRUE);

   /*Delete write data block in list!!!*/
   while(NULL != pstSock->pstWriteDataList)
   {
      St_WriteDataList *pstDel = pstSock->pstWriteDataList;
      pstSock->pstWriteDataList = pstSock->pstWriteDataList->next;
      EME_UTILS_FREEIF(pstSock->pvMemMgr, pstDel);
   }

   OEM_AsyncProc_CancelProc(pstSock->pvAsyncMgr, pstSock->pfnCbReadable, pstSock->pvUser);
   OEM_AsyncProc_CancelProc(pstSock->pvAsyncMgr, pstSock->pfnCbWriteable, pstSock->pvUser);

   EME_UTILS_FREEIF(pstSock->pvMemMgr, pvSocket);
   pvSocket = NULL;

   return EME_SUCCESS;
}

/* Socket Read and Write */
/************************************************************************/
/*  EME_Socket_BuildLine                 */
/************************************************************************/
WE_INT32 Socket_ReadLine(WE_CHAR* pcBuffer, WE_VOID* pvSock, WE_UINT32 uiBufLen)
{   
   St_Socket   *pstSock = (St_Socket*)pvSock;
   WE_UINT32   uiNeededSize = 0; 
   WE_BOOL     bOneLine = FALSE;

   if (pcBuffer == NULL || pstSock == NULL)
   {
      return EME_SOCKET_INVALID_DATA;
   }

   while (pstSock->acInBuf[pstSock->iReadSize + uiNeededSize] != '\0')
   {        
      if (pstSock->acInBuf[pstSock->iReadSize + uiNeededSize] =='\n')
      {
         uiNeededSize++;
         bOneLine = TRUE;
         break;
      }
      uiNeededSize++;
   }
   EME_UTILS_LOG_INFO(("IMAPLOG Socket_ReadLine uiNeededSize is %d uiBufLen is %d bOneLine is %d",uiNeededSize, uiBufLen, bOneLine));
   /* 1 for terminate char ('\0')*/
   if (uiNeededSize + 1 > uiBufLen)
   {
      return EME_SOCKET_INVALID_DATA;
   }

   if (bOneLine || uiNeededSize == EME_MAX_INBUF_LEN - 1)
   {
      EME_UTILS_MEMCPY(pcBuffer, &pstSock->acInBuf[pstSock->iReadSize], uiNeededSize);
      pstSock->iReadSize += uiNeededSize;

      *(pcBuffer + uiNeededSize) = '\0';

      if (bOneLine)
      {
         return EME_SOCKET_RECV_ONELINE;
      }
      else
      {
         return EME_SOCKET_RECV_PARTLINE;
      }
   }

   return EME_SOCKET_RECV_NODATA;
}

/************************************************************************/
/*  Socket_Read                   */
/************************************************************************/
WE_INT32 EME_Socket_Read(HSocket pvSocket, 
                         WE_CHAR *pcBuffer,
                         WE_UINT32 uiBufLen,
                         WE_UINT32 *puiReadSize)
{
   // local vars definition
   St_Socket *pstSock = (St_Socket *)pvSocket;
   WE_INT32 iResult = EME_SUCCESS;

   // params check 
   if (NULL == pstSock || NULL == pcBuffer)
   {
      return EME_SOCKET_INVALID_DATA;
   }

   EME_UTILS_MEMSET(pcBuffer, 0, uiBufLen);

   iResult = Socket_ReadLine(pcBuffer, pstSock, uiBufLen);
   if (EME_SOCKET_RECV_ONELINE == iResult)
   {
      EME_UTILS_MEMMOVE(pstSock->acInBuf, 
         pstSock->acInBuf + pstSock->iReadSize,
         pstSock->iInBufSize - pstSock->iReadSize);

      pstSock->iInBufSize -= pstSock->iReadSize;
      pstSock->iReadSize = 0; 

      EME_UTILS_MEMSET(pstSock->acInBuf + pstSock->iInBufSize, 
         0, 
         EME_MAX_INBUF_LEN - pstSock->iInBufSize);
   } 
   else if (EME_SOCKET_RECV_PARTLINE == iResult)
   {
      EME_UTILS_MEMSET(pstSock->acInBuf, 0x00, EME_MAX_INBUF_LEN);
      pstSock->iInBufSize = 0;
      pstSock->iReadSize  = 0;
   }
   else if (EME_SOCKET_RECV_NODATA == iResult)
   {
      EME_UTILS_LOG_INFO(("EME_Socket_Read--: NO DATA IN BUFFER, READ FROM SOCKET"));
      Socket_ReadCB(pvSocket);
   }

   return iResult;
}

/************************************************************************/
/* Socket_Write                  */
/************************************************************************/
WE_INT32 EME_Socket_Write(HSocket pvSocket,
                          WE_CHAR *pcBuffer,
                          WE_UINT32 uiDataSize,
                          WE_BOOL bIsEnd)
{
   // local variables definition
   St_Socket *pstSock = (St_Socket *)pvSocket;

   // params check
   if (pstSock == NULL || pcBuffer == NULL)
   {
      return EME_SOCKET_INVALID_DATA;
   }

   if (uiDataSize > EME_MAX_OUTBUF_LEN)
   {
      return EME_SOCKET_INVALID_DATA;
   }

   if (pstSock->bSSLSupported && pstSock->bSSLHandeShake && pstSock->bSSLUsed)
   {
      WE_INT32 iResult = EME_SUCCESS;
      iResult = ITls_ProcTlsData(pstSock->pITls, 0, 
         E_TLS_DATA_FROM_APP, 
         (WE_VOID *)pcBuffer, uiDataSize);
      EME_UTILS_LOG_ERROR(("TLSSSSSSSSSSSSSSSSSSocket ITls_ProcTlsData: %d", (WE_INT)iResult));

      if (FALSE == bIsEnd && NULL != pstSock->pfnCbWriteable)
      {
         OEM_AsyncProc_AddProc(pstSock->pvAsyncMgr, pstSock->pfnCbWriteable, pstSock->pvUser);
//         pstSock->pfnCbWriteable(pstSock->pvUser);
      }
      return EME_SUCCESS;
   }

   // clear the acInBuf start
   EME_UTILS_MEMSET(pstSock->acInBuf, 0x00, EME_MAX_INBUF_LEN);
   EME_UTILS_MEMSET(pstSock->acOutBuf, 0x00, EME_MAX_OUTBUF_LEN);
   pstSock->iInBufSize = 0;
   pstSock->iReadSize = 0;
   pstSock->iOutBufSize = 0;
   pstSock->iWrittenSize = 0;


   /* store buffer addr for socket_writeCB */
   EME_UTILS_MEMCPY(pstSock->acOutBuf, pcBuffer, uiDataSize);   
   pstSock->iOutBufSize = uiDataSize;

   if (FALSE == bIsEnd)
   {
      Socket_WriteCB_NoEnd   ((WE_VOID *)pstSock);
   } 
   else
   {
      Socket_WriteCB   ((WE_VOID *)pstSock);
   }   

   return EME_SUCCESS;
}

/* call back functions... */
/************************************************************************/
/* Socket_GetHostCB                                                   */
/************************************************************************/

static WE_VOID Socket_GetHostCB(WE_VOID * pvSock)
{
   St_Socket* pstSock = (St_Socket*) pvSock;
   WE_INT32 iReturn = EME_ERR_NET_START;    // init net err.
   /*  ISocket * pISocket = NULL;*/

   EME_UTILS_LOG_INFO(("Socket_GetHostCB :START!!!!"));
   EME_UTILS_LOG_INFO(("Socket_GetHostCB :START!!!!"));

   if (NULL == pstSock /*|| pstSock->pINetMgr == NULL*/)
   {
      EME_UTILS_LOG_ERROR(("Socket_GetHostCB :badparm!!"));
      return;
   } 
   // nResult: 1..AEEDNSMAXADDRS, Number of addresses successfully retrieved.
   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket Get Host DNS result: %x", pstSock->stDnsResult.nResult));   

   EME_UTILS_LOG_ERROR(("Socket_GetHostCB :stDnsResult.nResult = %d!!", pstSock->stDnsResult.nResult));
   EME_UTILS_LOG_ERROR(("Socket_GetHostCB :pstSock.iRetryTimes = %d!!", pstSock->iRetryTimes));

   if(pstSock->stDnsResult.nResult < 0)  
   {
      if (pstSock->pfnCbErrorHandler != NULL)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_DNS_ERR));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_DNS_ERR);
      }
      return;
   }
   else if (pstSock->stDnsResult.nResult == OEM_SOC_NOTCONN && pstSock->iRetryTimes > 0)
   {
      OEM_GetHostByName(pstSock->phOEMSocketHdl, pstSock->pcHostName, &pstSock->stDnsResult, Socket_GetHostCB, pstSock);
      pstSock->iRetryTimes--;
      return;
   }

   iReturn = OEM_OpenSocket(pstSock->phOEMSocketHdl, OEM_SOCK_STREAM); 
   if (iReturn != OEM_SOC_SUCCESS)
   {
      EME_UTILS_LOG_ERROR(("+++++++++socket_getHostCB, INETMGR_OpenSocket failed"));
      if (pstSock->pfnCbErrorHandler != NULL)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_OPEN_FAILED));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_OPEN_FAILED);
      }
      return;
   }

   iReturn = OEM_Connect(pstSock->phOEMSocketHdl, pstSock->stDnsResult.addrs[0], pstSock->usPort, Socket_ConnectCB, pstSock);
   EME_UTILS_LOG_INFO(("Socket_GetHostCB :OEM_Connect iReturn= %d!!", iReturn));
   if (iReturn != OEM_SOC_SUCCESS && iReturn != OEM_SOC_WOULDBLOCK)
   {
      /* If it's WOULDBLOCK, then it's still connecting. Don't cleanup. */
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_CONNECTION_ERR));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_CONNECTION_ERR);
      }
         return;
   }

   return;
}

/************************************************************************/
/* Socket_ConnectCB                  */
/************************************************************************/
static WE_VOID Socket_ConnectCB(WE_VOID * pvUser, WE_INT iError)
{
   St_Socket* pstSock = (St_Socket*)pvUser;
   WE_INT32   iReturn = EME_SUCCESS;
   if (NULL == pstSock)
   {
      return;
   }
   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket Connection result: %x", iError)); 

   if (OEM_SOC_SUCCESS != iError)
   {
      if (iError == OEM_SOC_ERROR && pstSock->iRetryTimes > 0 && pstSock->phOEMSocketHdl != NULL)
      {
         iReturn =  OEM_Connect(pstSock->phOEMSocketHdl,
            pstSock->stDnsResult.addrs[0],
            pstSock->usPort,
            Socket_ConnectCB,
            pstSock);

         if (iReturn != OEM_SOC_SUCCESS && iReturn != OEM_SOC_WOULDBLOCK)
         {
            /* If it's WOULDBLOCK, then it's still connecting. Don't cleanup. */
            if (NULL != pstSock->pfnCbErrorHandler)
            {
               EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_CONNECTION_ERR));
               pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_CONNECTION_ERR);
            }
            return;
         }
         pstSock->iRetryTimes--;
         return;
      }
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_CONNECTION_ERR));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_CONNECTION_ERR);
      }
      return;
   } 

   /* Add Supporting to SSL */
   /*Add by jade for support hotmail ssl feature, 2011.03.24
     if ssl need and port is 25, we should send STARTTLS command before use ssl!!!*/
   if (pstSock->bSSLSupported && !pstSock->bStartTls)
   {
      WE_UCHAR acTempIP[EME_MAX_IPADDR_LEN] = {0};
      WE_UCHAR acIPAddr[EME_MAX_TLSIP_LEN] = {0};
      WE_UCHAR *pcIPTemp = NULL;
      WE_UCHAR *pcPrev = NULL;
      WE_UCHAR i = 0;

      inet_ntoa(pstSock->stDnsResult.addrs[0], (WE_CHAR*)acTempIP, EME_MAX_IPADDR_LEN);
      acIPAddr[i++] = 0;
      acIPAddr[i++] = 4;
      pcIPTemp = acTempIP;
      pcPrev = acTempIP;
      while (pcIPTemp != NULL)
      {
         WE_UCHAR acIPStr[EME_MAX_TLSIP_LEN] = {0};
         WE_UINT32 uiIPstrLen = 0;
         pcIPTemp = (WE_UCHAR*)EME_UTILS_STRCHR((WE_CHAR*)pcIPTemp, '.');
         if (pcIPTemp == NULL)
         {
            acIPAddr[i] = (WE_UCHAR)EME_UTILS_ATOI((const WE_CHAR*)pcPrev);
            break;
         }
         uiIPstrLen = (WE_UINT32)(pcIPTemp - pcPrev);
         EME_UTILS_MEMCPY(acIPStr, pcPrev, uiIPstrLen);
         acIPAddr[i++] = (WE_UCHAR)EME_UTILS_ATOI((const WE_CHAR*)acIPStr);
         pcIPTemp++;
         pcPrev = pcIPTemp;
      }

      /*Handshake begin*/
      ITls_RegClientEvtCB(pstSock->pITls, pstSock, (WE_HANDLE)Socket_TLSCB_Handeshake);
      ITls_BeginHandeShake(pstSock->pITls, 0, (WE_UCHAR*)pstSock->pcHostName, FALSE, acIPAddr, (WE_UINT16)EME_TLS_PORT);

      pstSock->bSSLUsed = TRUE;

      return;
   }
   Socket_ReadCB((WE_VOID *)pstSock);

   if (NULL != pstSock->pfnCbConnectionOk)
   {
      pstSock->pfnCbConnectionOk(pstSock->pvUser);
      EME_UTILS_LOG_ERROR(("+++++++++socket_connectCB, connected"));
   }  

}

WE_VOID EME_Socket_StartTls(HSocket pvSock)
{
   St_Socket* pstSock = (St_Socket*)pvSock;

   if (pstSock->bSSLSupported)
   {
      WE_UCHAR acTempIP[EME_MAX_IPADDR_LEN] = {0};
      WE_UCHAR acIPAddr[EME_MAX_TLSIP_LEN] = {0};
      WE_UCHAR *pcIPTemp = NULL;
      WE_UCHAR *pcPrev = NULL;
      WE_UCHAR i = 0;
      
      inet_ntoa(pstSock->stDnsResult.addrs[0], (WE_CHAR*)acTempIP, EME_MAX_IPADDR_LEN);
      acIPAddr[i++] = 0;
      acIPAddr[i++] = 4;
      pcIPTemp = acTempIP;
      pcPrev = acTempIP;
      while (pcIPTemp != NULL)
      {
         WE_UCHAR acIPStr[EME_MAX_TLSIP_LEN] = {0};
         WE_UINT32 uiIPstrLen = 0;
         pcIPTemp = (WE_UCHAR*)EME_UTILS_STRCHR((WE_CHAR*)pcIPTemp, '.');
         if (pcIPTemp == NULL)
         {
            acIPAddr[i] = (WE_UCHAR)EME_UTILS_ATOI((const WE_CHAR*)pcPrev);
            break;
         }
         uiIPstrLen = (WE_UINT32)(pcIPTemp - pcPrev);
         EME_UTILS_MEMCPY(acIPStr, pcPrev, uiIPstrLen);
         acIPAddr[i++] = (WE_UCHAR)EME_UTILS_ATOI((const WE_CHAR*)acIPStr);
         pcIPTemp++;
         pcPrev = pcIPTemp;
      }
      
      /*Handshake begin*/
      ITls_RegClientEvtCB(pstSock->pITls, pstSock, (WE_HANDLE)Socket_TLSCB_Handeshake);
      ITls_BeginHandeShake(pstSock->pITls, 0, (WE_UCHAR*)pstSock->pcHostName, FALSE, acIPAddr, (WE_UINT16)EME_TLS_PORT);

      pstSock->bSSLUsed = TRUE;
      
      return;
   }

}
/************************************************************************/
/* Socket_ReadCB                    */
/************************************************************************/
static WE_VOID Socket_ReadCB(WE_VOID* pvSock)
{
   WE_INT32 iRecvLen = 0;   
   St_Socket *pstSock = (St_Socket*)pvSock;

   if (NULL == pstSock || NULL == pstSock->phOEMSocketHdl)
   {
      return;
   }

   if (TRUE == pstSock->bUserCancel)
   {
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_CANCEL));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_CANCEL);
      }
      return;
   }
   
   if (EME_MAX_INBUF_LEN - pstSock->iInBufSize - 1 < 0)
   {
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_INVALID_DATA));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_INVALID_DATA);
      }
      return;
   }
    /* Following cods  are  cleared to resolve not receive mail bugs in imap :BUG 338892 */
   /*
   if (EME_MAX_INBUF_LEN - pstSock->iInBufSize - 1 == 0)
   {
      EME_UTILS_LOG_INFO(("Socket_ReadCB--: BUFFER IS MEET MAX LEN"));
      OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB, (WE_VOID*)pstSock);
      return;
   }
   */

   /*Use SSL*/
   if (pstSock->bSSLSupported && pstSock->bSSLUsed)
   {
      if (NULL != pstSock->pstDataListHead)
      {
         if (0 != pstSock->pstDataListHead->iBufferLen)
         {
            int iInBufSize = 0;
            int iNodeBufSize = 0;
            int iCopySize = 0;

            iInBufSize = EME_MAX_INBUF_LEN - pstSock->iInBufSize - 1;
            iNodeBufSize = pstSock->pstDataListHead->iBufferLen - pstSock->pstDataListHead->iReadLen;

            if (iNodeBufSize > iInBufSize)
            {
               iCopySize = iInBufSize;
               EME_UTILS_MEMCPY(pstSock->acInBuf + pstSock->iInBufSize, 
                  pstSock->pstDataListHead->acBufferData + pstSock->pstDataListHead->iReadLen,
                  iCopySize);
               pstSock->pstDataListHead->iReadLen += iCopySize;
               pstSock->iInBufSize += iCopySize;
            }
            else
            {
               iCopySize = iNodeBufSize;
               EME_UTILS_MEMCPY(pstSock->acInBuf + pstSock->iInBufSize, 
                  pstSock->pstDataListHead->acBufferData + pstSock->pstDataListHead->iReadLen,
                  iCopySize);
               pstSock->pstDataListHead->iReadLen += iCopySize;
               pstSock->iInBufSize += iCopySize;
               Socket_DelDataNode(pvSock, FALSE);
            }			   

         }
         else
         {
            if (NULL != pstSock->pfnCbErrorHandler)
            {
               /* There is no more data to be received. */
               /* The peer has shut down the connection. */
               EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SHUTDOWN));
               pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SHUTDOWN);
            }
            return;   
         }
         if (pstSock->bSSLHandeShake)
         {
            if (0 != pstSock->iInBufSize && NULL != pstSock->pfnCbReadable)
            {
               pstSock->pfnCbReadable(pstSock->pvUser);
            }     
         }
         return;
      }
      return;
   }

   iRecvLen =  OEM_SocketRead(pstSock->phOEMSocketHdl, 
      (WE_UCHAR *)(pstSock->acInBuf + pstSock->iInBufSize),
      (WE_UINT)(EME_MAX_INBUF_LEN - pstSock->iInBufSize - 1));
//   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket read: %s", pstSock->acInBuf + pstSock->iInBufSize));

   if (iRecvLen == OEM_SOC_WOULDBLOCK || 0 == iRecvLen)
   {
      EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket read: block"));
      OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB, (WE_VOID*)pstSock);
      return;
   } 
   else if (iRecvLen == OEM_SOC_ERROR) 
   {
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_NET_ERR));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_NET_ERR);
      }
      return;
   }
   else if (iRecvLen > 0) /* data has been read */
   {
      WE_INT32 iIndex = 0;
      /* The data has been read from the socket, notify client to process the buffer. */
      pstSock->iInBufSize = pstSock->iInBufSize + iRecvLen; 

      /*if receive data has 0x00, we should convert to 0x20*/
      while(iIndex < pstSock->iInBufSize)
      {
         if(0x00 == pstSock->acInBuf[iIndex])
         {
            pstSock->acInBuf[iIndex] = 0x20;
         }
         iIndex++;
      }

      OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB, (WE_VOID*)pstSock);
      if (0 != pstSock->iInBufSize && NULL != pstSock->pfnCbReadable)
      {
         OEM_AsyncProc_AddProc(pstSock->pvAsyncMgr, pstSock->pfnCbReadable, pstSock->pvUser);
      }
      return;
   }
   else if (NULL != pstSock->pfnCbErrorHandler)/* recvlen == 0 */
   {
      /* There is no more data to be received. */
      /* The peer has shut down the connection. */
      EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SHUTDOWN));

      if(iRecvLen == OEM_SOC_TIMEOUT)
      {
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_TIME_OUT);
      }
      else
      {
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SHUTDOWN);
      }

      return;
   }
}

/************************************************************************/
/* Socket_WriteCB                    */
/************************************************************************/
static WE_VOID Socket_WriteCB   (WE_VOID* pvSock)
{
   // local vars. definition
   WE_INT32  iRv = 0;
   St_Socket *pstSock = (St_Socket*)pvSock;
   // params check
   if (NULL == pstSock || NULL == pstSock->phOEMSocketHdl)
   {
      return;
   }
   /* Loop till all the bytes are written. */
   while (pstSock->iWrittenSize < pstSock->iOutBufSize)
   {
      iRv = OEM_SocketWrite(pstSock->phOEMSocketHdl, 
         (WE_UCHAR *)pstSock->acOutBuf + pstSock->iWrittenSize, 
         (WE_UINT32)pstSock->iOutBufSize - pstSock->iWrittenSize);

      EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket write: %s", pstSock->acOutBuf + pstSock->iWrittenSize));

      EME_UTILS_LOG_INFO(("Socket_WriteCB :  OEM_SocketWrite iret  = %d !!!", iRv ));
      EME_UTILS_LOG_INFO(("Socket_WriteCB :  OEM_SocketWrite iret    = %d !!!", iRv ));

      if (iRv == OEM_SOC_WOULDBLOCK) 
      {
         /* No byte were written successfully. Register callback to 
         * try again later. */
         /* set timer for time out */
         OEM_SocketWriteable(pstSock->phOEMSocketHdl, Socket_WriteCB, (WE_VOID *)pvSock);
         return;
      } 
      else if (iRv == OEM_SOC_ERROR) 
      {
         /* Write Error. */
         if (NULL != pstSock->pfnCbErrorHandler)
         {
            EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SEND_FAIL));
            pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
         }
         return;
      }
      /* In case of parital write, loop and write the rest */
      pstSock->iWrittenSize += iRv;
   }

   EME_UTILS_MEMSET(pstSock->acOutBuf, 0, EME_MAX_OUTBUF_LEN);
   pstSock->iOutBufSize = 0;
   pstSock->iWrittenSize = 0;

   /*
   Project Name: WINCE_EMAIL
   Add by jade, if send cmd successful, then we read response at now
   Read response from socket
   8/29/2008
   */
   OEM_SocketProcess(pstSock->phOEMSocketHdl, OEM_SOCK_EVT_READ, EME_SUCCESS);

   return;
}

/************************************************************************/
/* Socket_WriteCB_NoEnd    is not end ,need init callback to   pvUser    */
/************************************************************************/
static WE_VOID Socket_WriteCB_NoEnd   (WE_VOID* pvSock)
{
   WE_INT32  iRv = 0;
   St_Socket *pstSock = (St_Socket*)pvSock;

   // params check
   if (NULL == pstSock || NULL == pstSock->phOEMSocketHdl /*||!pstSock->bSocketState*/)
   {
      return;
   }
   
   if (TRUE == pstSock->bUserCancel)
   {
      pstSock->bUserCancel = FALSE;

      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_CANCEL));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_CANCEL);
      }
      return;
   }
   /* Loop till all the bytes are written. */
   while (pstSock->iWrittenSize < pstSock->iOutBufSize)
   {
      /* Write data to the echo-port of the remote host */
      iRv = OEM_SocketWrite(pstSock->phOEMSocketHdl , 
         (WE_UCHAR *)pstSock->acOutBuf + pstSock->iWrittenSize,
         (WE_UINT32)pstSock->iOutBufSize - pstSock->iWrittenSize);
      EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket write: %s", pstSock->acOutBuf + pstSock->iWrittenSize));

      if (iRv == OEM_SOC_WOULDBLOCK) 
      {
         /* No byte were written successfully. Register callback to 
         * try again later. */
         /* set timer for time out */
         OEM_SocketWriteable(pstSock->phOEMSocketHdl,  Socket_WriteCB_NoEnd, (WE_VOID *)pvSock);

         return;
      } 
      else if (iRv == OEM_SOC_ERROR) 
      {
         /* Write Error. */
         if (NULL != pstSock->pfnCbErrorHandler)
         {
            EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SEND_FAIL));
            pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
         }
         return;
      }
      /* In case of parital write, loop and write the rest */
      pstSock->iWrittenSize += iRv;
   }

   EME_UTILS_MEMSET(pstSock->acOutBuf, 0, EME_MAX_OUTBUF_LEN);
   pstSock->iOutBufSize = 0;
   pstSock->iWrittenSize = 0;

   if (NULL != pstSock->pfnCbWriteable)
   {
      //pstSock->pfnCbWriteable(pstSock->pvUser);
      OEM_AsyncProc_AddProc(pstSock->pvAsyncMgr, pstSock->pfnCbWriteable, pstSock->pvUser);
   }

   return;
}

/* reg needed call back */
/************************************************************************/
/*  EME_Socket_RegConnectionOk               */
/************************************************************************/
WE_VOID EME_Socket_RegCbConnectionOk(HSocket pvSocket, WE_VOID* pvCallback)
{
   St_Socket *pstSock = (St_Socket*)pvSocket;

   EME_UTILS_LOG_INFO(("EME_Socket_RegCbConnectionOk : START!!!"));
   EME_UTILS_LOG_INFO(("EME_Socket_RegCbConnectionOk : START!!!"));
   if (NULL != pstSock)
   {
      pstSock->pfnCbConnectionOk = (FUNCCONNECTIONOK)pvCallback;
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbConnectionOk : CALLBACK!!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbConnectionOk : CALLBACK!!!"));
   }      
}

/************************************************************************/
/*  EME_Socket_RegReadable                 */
/************************************************************************/
WE_VOID EME_Socket_RegCbReadable(HSocket pvSocket, WE_VOID* pvCallback)
{
   St_Socket *pstSock = (St_Socket*)pvSocket;

   EME_UTILS_LOG_INFO(("EME_Socket_RegCbReadable : START!!!"));
   EME_UTILS_LOG_INFO(("EME_Socket_RegCbReadable : START!!!"));
   if (NULL != pstSock)
   {
      if (pstSock->pfnCbReadable != NULL)
      {
         OEM_AsyncProc_CancelProc(pstSock->pvAsyncMgr, pstSock->pfnCbReadable, NULL);
      }
      pstSock->pfnCbReadable = (FUNCREADABLE)pvCallback;

      EME_UTILS_LOG_INFO(("EME_Socket_RegCbReadable : CALLBACK!!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbReadable : CALLBACK!!!"));
   }

}

/************************************************************************/
/* EME_Socket_RegErrHandler               */
/************************************************************************/
WE_VOID EME_Socket_RegCbErrHandler   (HSocket pvSocket, WE_VOID* pvCallback)
{
   St_Socket *pstSock = (St_Socket*)pvSocket;

   if (NULL != pstSock)
   {
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbErrHandler : CALLBACK!!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbErrHandler : CALLBACK!!!"));
      ((St_Socket *)pvSocket)->pfnCbErrorHandler = (FUNCERRORHANDLER)pvCallback;
   }

}

/************************************************************************/
/*  EME_Socket_RegWriteable               */
/************************************************************************/
WE_VOID EME_Socket_RegCbWriteable   (HSocket pvSocket, WE_VOID* pvCallback)
{
   St_Socket *pstSock = (St_Socket*)pvSocket;

   if (NULL != pstSock)
   {
      if (pstSock->pfnCbWriteable != NULL)
      {
         OEM_AsyncProc_CancelProc(pstSock->pvAsyncMgr, pstSock->pfnCbWriteable, NULL);
      }
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbWriteable : CALLBACK!!!"));
      EME_UTILS_LOG_INFO(("EME_Socket_RegCbWriteable : CALLBACK!!!"));

      pstSock->pfnCbWriteable = (FUNCWRITEABLE)pvCallback;  
   }

}


static WE_VOID Socket_TLSCB_Handeshake(E_TLS_DATA_TYPE eDataType, void* pvResp, WE_HANDLE hPrivData)
{
   St_Socket *pstSock = (St_Socket *)hPrivData;
   St_TlsRespData *pstRespData = (St_TlsRespData *)pvResp;
   WE_INT32 iLen = 0;
   WE_INT32 iRespDataLen = 0;
   WE_CHAR  *pcRespData = NULL;
   WE_INT32 iret = 0;

   pcRespData = (WE_CHAR *)pstRespData->pvRespData;
   iLen = pstRespData->iRespLen; 

   switch(eDataType)
   {
   case E_TLS_DATA_TO_APP:
      {
         while( iLen > 0 )
         {   
            if( iLen > EME_MAX_INBUF_LEN - 1)
            {
               iRespDataLen = EME_MAX_INBUF_LEN - 1;
            }
            else
            {
               iRespDataLen = iLen;
            }
            EME_UTILS_LOG_INFO(("ssl read data = %s", pcRespData));
            Socket_AddDataNode(pstSock, pcRespData, iRespDataLen);
            iLen = iLen - iRespDataLen;
            pcRespData += iRespDataLen;
         }	
         Socket_ReadCB(pstSock);

         return;
      }
   case E_TLS_DATA_TO_SVR:
      {
         St_WriteDataList *pstDataTmp = pstSock->pstWriteDataList;
         St_WriteDataList *pstWriteData = EME_UTILS_MALLOC(pstSock->pvMemMgr, sizeof(St_WriteDataList));
         
         if(NULL != pstWriteData)
         {
            EME_UTILS_MEMSET(pstWriteData, 0x00, sizeof(St_WriteDataList));
            EME_UTILS_MEMCPY(pstWriteData->acWriteData, pcRespData, iLen);
            pstWriteData->iBufferLen = iLen;
            pstWriteData->iWriteLen = 0;
            
            if(NULL == pstSock->pstWriteDataList)
            {
               pstSock->pstWriteDataList = pstWriteData;
            }
            else
            {
               while(pstDataTmp->next != NULL)
               {
                  pstDataTmp = pstDataTmp->next;
               }
               
               pstDataTmp->next = pstWriteData;
            }
            
            OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB_Tls, pstSock);
            OEM_SocketWriteable(pstSock->phOEMSocketHdl, Socket_WriteCB_Tls, pstSock);
            Socket_WriteCB_Tls(pstSock);
            break;
         }
         else
         {
            EME_UTILS_LOG_ERROR(("no memory!!!"));
            if(NULL != pstSock->pfnCbErrorHandler)
            {
               pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
            }
         }
         
//         Socket_ReadCB_Tls((WE_VOID *)pstSock); 
      }
      break;
   case E_TLS_INFO:
      {
         if(pstRespData->iErrCode == G_WAP_ERR_TLS_FINISH_HANDSHAKE_OK)/*Handshake end*/
         {
            pstSock->bSSLHandeShake = TRUE;
            if (NULL != pstSock->pfnCbReadable)
            {
               pstSock->pfnCbReadable(pstSock->pvUser);
            } 
            return;
         }
         else if (pstRespData->iErrCode == G_WAP_ERR_TLS_VERIFY_CERT_ERR)
         {
            St_TlsOpt stTlsOPt[2] = {0};
            WE_UINT32 uiContinue = 1;
            stTlsOPt[0].eOptId = E_TLS_OPT_ERR_RESP;
            stTlsOPt[0].pvOpt = (WE_VOID *)&uiContinue;
            stTlsOPt[1].eOptId = E_TLS_OPT_END;

            ITls_ResumeTlsHandShake(pstSock->pITls, 0, stTlsOPt);

            EME_UTILS_LOG_ERROR(("Msg process server trust error: %x", (WE_INT)pstRespData->iErrCode));
            return;
         }

      }
      break;
   default:
      break;
   }
}

static WE_VOID Socket_WriteCB_Tls(WE_VOID* pvSock)
{
   St_Socket *pstSock = (St_Socket*)pvSock;
   St_WriteDataList *pstWriteData = pstSock->pstWriteDataList;
   WE_INT32 iWriteLen = 0;

   if(NULL == pstSock->pstWriteDataList)
   {
      EME_UTILS_LOG_ERROR(("no write data to send!!!"));
      if(OEM_SOC_TIMEOUT == OEM_SocketWrite(pstSock->phOEMSocketHdl, NULL, 0) && pstSock->pfnCbErrorHandler)
      {
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
      }
      return;
   }

   while(pstSock->pstWriteDataList)
   {
      pstWriteData = pstSock->pstWriteDataList;

      iWriteLen = OEM_SocketWrite(pstSock->phOEMSocketHdl, 
         (WE_UCHAR*)pstWriteData->acWriteData + pstWriteData->iWriteLen, 
         pstWriteData->iBufferLen - pstWriteData->iWriteLen);

      if(iWriteLen > 0)
      {
         pstWriteData->iWriteLen += iWriteLen;

         if(pstWriteData->iWriteLen >= pstWriteData->iBufferLen)
         {
            pstSock->pstWriteDataList = pstWriteData->next;
            EME_UTILS_FREEIF(pstSock->pvMemMgr, pstWriteData);
         }
      }
      else if(iWriteLen == OEM_SOC_WOULDBLOCK)
      {
         OEM_SocketWriteable(pstSock->phOEMSocketHdl, Socket_WriteCB_Tls, pstSock);
         break;
      }
      else if(pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SEND_FAIL));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
         break;
      }
   }

   return;
}

static WE_VOID Socket_ReadCB_Tls(WE_VOID* pvSock)
{
   WE_INT32 iRecvLen = 0;   
   WE_INT32 iResult = EME_SUCCESS;
   St_Socket *pstSock = (St_Socket*)pvSock;
   WE_CHAR *pacInBuf = EME_UTILS_MALLOC(NULL, EME_MAX_INBUF_LEN);

   if (NULL == pacInBuf || NULL == pstSock || NULL == pstSock->phOEMSocketHdl /*|| !pstSock->bSocketState*/)
   {
      return;
   }
   
   EME_UTILS_MEMSET(pacInBuf, 0x00, EME_MAX_INBUF_LEN);
   iRecvLen = OEM_SocketRead(pstSock->phOEMSocketHdl, 
      (WE_UCHAR *)pacInBuf, 
      EME_MAX_INBUF_LEN - 1);
   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket read: %s", pacInBuf));
   EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket readlen: %d", iRecvLen));

   if (iRecvLen == OEM_SOC_WOULDBLOCK || 0 == iRecvLen)
   {
      /* WOULDBLOCK => there is no more data available at the moment */
      /* Register the callback to read the data later. */
      OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB_Tls, pvSock);
      EME_UTILS_FREEIF(NULL, pacInBuf);
      return;
   } 
   else if (iRecvLen == OEM_SOC_ERROR) 
   {
      if (NULL != pstSock->pfnCbErrorHandler)
      {
         EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_NET_ERR));
         pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_NET_ERR);
      }
      EME_UTILS_FREEIF(NULL, pacInBuf);
      return;
   }
   else if (iRecvLen > 0) /* data has been read */
   {
      /* The data has been read from the socket, notify client to process the buffer. */
      OEM_SocketReadable(pstSock->phOEMSocketHdl, Socket_ReadCB_Tls, (WE_VOID*)pvSock); 

      /*SSL process.*/
      {
         //ITls_RegClientEvtCB(pstSock->pITls, pstSock, Socket_TLSCB);
         iResult = ITls_ProcTlsData(pstSock->pITls, 0,
            E_TLS_DATA_FROM_SVR, (WE_VOID *)pacInBuf,
            iRecvLen);
         if (iResult != EME_SUCCESS)
         {
            EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SEND_FAIL));
            pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SEND_FAIL);
         }
      }
      EME_UTILS_FREEIF(NULL, pacInBuf);
      return;
   }
   else if (NULL != pstSock->pfnCbErrorHandler)/* recvlen == 0 */
   {
      /* There is no more data to be received. */
      /* The peer has shut down the connection. */
      EME_UTILS_FREEIF(NULL, pacInBuf);
      EME_UTILS_LOG_ERROR(("SSSSSSSSSSSSSSSSSocket error: %x", EME_SOCKET_SHUTDOWN));
      pstSock->pfnCbErrorHandler(pstSock->pvUser, EME_SOCKET_SHUTDOWN);
      return;
   }
}

static WE_INT32 Socket_AddDataNode(WE_VOID* pvSock, WE_CHAR *pcData, WE_INT32 iDataLen)
{
   WE_INT32 iResult = EME_SUCCESS;
   St_Socket *pstScoket = (St_Socket *)pvSock;
   St_DataList *pstAddDataNode = NULL;

   if (NULL == pstScoket || NULL == pcData)
   {
      return EME_FAILED;
   }

   do 
   {
      pstAddDataNode = EME_UTILS_MALLOC(pstScoket->pvMemMgr, sizeof(St_DataList));
      EME_UTILS_BREAKNULL(pstAddDataNode, 
         iResult, 
         EME_FAILED, "Socket_AddDataNode, pstAddDataNode is Null");
      EME_UTILS_MEMSET(pstAddDataNode, 0, sizeof(St_DataList));

      pstAddDataNode->iBufferLen = iDataLen;

      EME_UTILS_MEMSET(pstAddDataNode->acBufferData, 0, EME_MAX_INBUF_LEN);
      EME_UTILS_MEMCPY((WE_VOID *)pstAddDataNode->acBufferData, (WE_VOID *)pcData, iDataLen);

      if (NULL == pstScoket->pstDataListHead)
      {
         pstScoket->pstDataListHead = pstAddDataNode;
         pstScoket->pstDataListTail = pstAddDataNode;
         pstScoket->pstDataListHead->next = NULL;
         pstScoket->pstDataListTail->next = NULL;
      }
      else
      {
         pstScoket->pstDataListTail->next = pstAddDataNode;
         pstScoket->pstDataListTail = pstAddDataNode;
         pstScoket->pstDataListTail->next = NULL;
      }

   } while(0);

   return iResult;

}

static WE_VOID Socket_DelDataNode(WE_VOID* pvSock, WE_BOOL bAll)
{
   St_Socket *pstScoket = (St_Socket *)pvSock;
   St_DataList *pstDataListHead = NULL;
   St_DataList *pstDataListFree = NULL;

   if (NULL == pstScoket)
   {
      return;
   }

   if (NULL == pstScoket->pstDataListHead)
   {
      return;
   }

   pstDataListHead = pstScoket->pstDataListHead;

   if (bAll)
   {
      while (NULL != pstDataListHead)
      {
         pstDataListFree = pstDataListHead;
         pstDataListHead = pstDataListHead->next;
         EME_UTILS_FREEIF(pstScoket->pvMemMgr, pstDataListFree);
      }
      pstScoket->pstDataListHead = pstDataListHead;
      pstScoket->pstDataListTail = pstDataListHead;
   }
   else
   {
      if (NULL != pstDataListHead)
      {
         pstDataListFree = pstDataListHead;
         pstDataListHead = pstDataListHead->next;
         EME_UTILS_FREEIF(pstScoket->pvMemMgr, pstDataListFree);
      }
      pstScoket->pstDataListHead = pstDataListHead;
   }
}
