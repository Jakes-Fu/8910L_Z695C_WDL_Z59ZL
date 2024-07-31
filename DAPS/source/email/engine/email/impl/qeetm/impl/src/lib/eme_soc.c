/*=====================================================================================
FILE NAME   : eme_soc.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some function for managing transaction.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-04-17     Rachel                      create eme_soc.c
          
=====================================================================================*/
//pclint
/*lint -e570 */

/*===================================================================
 * 
 *                     Include Files
 * 
====================================================================*/
/*Include System head file*/
#include "socket_api.h"
#include "socket_types.h"
#include "in_message.h"
#include "oem_soc_api.h"
#include "eme_utils_api.h"
#include "brweng_tf2hz_email.h"
#include "oem_ppp_api.h"
#include "eme_callback.h"
/*Include Program Global head file*/ 

/* Define Constant Macro Start */
#define NTW_ACCOUNT_ID 19
/* Define Constant Macro End */

#define EME_SOC_USER_MULIT_INSTANCE

/*============================================================================
 *	Private/Local Function Declarations
 *===========================================================================*/

typedef WE_INT32 SOCKET_HANDLE;

#define EME_SOCKET_CB_MAX          10
#define EME_SOCKET_MAX_TIMEROUT    60000

typedef struct {
   WE_VOID  *pUser;
   PFNNOTIFY pHostFn;
   PFNNOTIFY pReadFn;
   PFNNOTIFY pWriteFn;
   PFNCONNECTCB pConnectFn;
   WE_UINT32 uiEvtType;
   St_OemSockAddr astOemSocAddr;
   OEMDNSResult *pres;
   SOCKET_HANDLE hSocket;
   AEECallback m_SocketCallback[EME_SOCKET_CB_MAX];
   WE_VOID    *m_SocketCallbackData[EME_SOCKET_CB_MAX];
   SCI_TIMER_PTR    *m_SocketTimer[OEM_TIMER_MAX];
   WE_INT32          m_SocketTimerError[OEM_TIMER_MAX];
   WE_UINT32        uiCurrType;
}St_OemSocketHanlde;

static HOEMSOCKETHANDLE *g_phOemSocketHdl = NULL;
extern BLOCK_ID         g_email_task_id;
extern WE_UINT32        g_email_netid;

static WE_VOID OEM_SocketTimerOut(WE_UINT32 uiData);

static WE_VOID OEM_CreateTimer(St_OemSocketHanlde *pstOEMSocketHdl, WE_UINT32 type)
{
   char  acTimeName[32] = {0};
   if(NULL == pstOEMSocketHdl)
   {
       EME_UTILS_LOG_INFO(("OEM_CreateTimer : BADPARAM"));
       return ;
   }

   EME_UTILS_LOG_INFO(("OEMSocTimer_%d", type));
   if(NULL != pstOEMSocketHdl->m_SocketTimer[type])
   {
   	  SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[type]);
	  pstOEMSocketHdl->m_SocketTimer[type] = NULL;
   }
      pstOEMSocketHdl->m_SocketTimer[type] = SCI_CreateTimer(acTimeName, OEM_SocketTimerOut, pstOEMSocketHdl, (EME_SOCKET_MAX_TIMEROUT * 2), SCI_AUTO_ACTIVATE);
      pstOEMSocketHdl->m_SocketTimerError[type] = OEM_SOC_SUCCESS;
      pstOEMSocketHdl->uiCurrType = type;

   return;
}

/*====================================================================
DESCRIPTION
    OEM_CreateSocHandle
       create the oem soc handle 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
WE_INT32   OEM_CreateSocHandle(HOEMSOCKETHANDLE* ppOEMSocketHdl)
{ 
   WE_INT  iRet = 0;

   St_OemSocketHanlde *pstOEMSocketHdl = NULL;

   if(NULL == ppOEMSocketHdl)
      return OEM_SOC_ERROR;

   EME_UTILS_LOG_INFO(("OEM_CreateSocHandle : START!"));

   do 
   {
      pstOEMSocketHdl = WE_SCL_MALLOC(sizeof(St_OemSocketHanlde));
      if(NULL == pstOEMSocketHdl)
      {
         EME_UTILS_LOG_INFO(("OEM_CreateSocHandle : BADPARAM"));
         break;
      }
      WE_SCL_MEMSET(pstOEMSocketHdl, 0x00, sizeof(St_OemSocketHanlde));

   } while(0);

   if(iRet != OEM_SOC_SUCCESS)
   {
      EME_UTILS_FREEIF(NULL, pstOEMSocketHdl);
   }

   g_phOemSocketHdl = pstOEMSocketHdl;

   *ppOEMSocketHdl = pstOEMSocketHdl;

   EME_UTILS_LOG_INFO(("OEM_CreateSocHandle : END!"));
   return iRet;
}

/*====================================================================
DESCRIPTION
    OEM_FreeSocHandle
       free the oem soc handle 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
WE_VOID OEM_FreeSocHandle(HOEMSOCKETHANDLE* ppOEMSocketHdl)
{
   St_OemSocketHanlde *pstOEMSocketHdl = NULL;
   EME_UTILS_LOG_INFO(("OEM_FreeSocHandle: START!"));
   if(NULL == ppOEMSocketHdl || NULL == *ppOEMSocketHdl)
   {
      EME_UTILS_LOG_INFO(("OEM_FreeSocHandle: BADPARAM!"));
      return;
   }
   
   pstOEMSocketHdl = (St_OemSocketHanlde*)*ppOEMSocketHdl;

   if(pstOEMSocketHdl->hSocket > 0)
      sci_sock_socketclose(pstOEMSocketHdl->hSocket);

   pstOEMSocketHdl->hSocket = 0;
   pstOEMSocketHdl->pHostFn = NULL;
   pstOEMSocketHdl->pReadFn = NULL;
   pstOEMSocketHdl->pWriteFn = NULL;
   pstOEMSocketHdl->pConnectFn = NULL;

   g_phOemSocketHdl = NULL;
   
   WE_SCL_FREEIF(*ppOEMSocketHdl);
   EME_UTILS_LOG_INFO(("OEM_FreeSocHandle: END!"));	     
}


/*====================================================================
DESCRIPTION
    OEM_OpenSocket
         open the socket by the network type
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
WE_INT32 OEM_OpenSocket(HOEMSOCKETHANDLE phOEMSocketHdl, NetSocketType Type)
{
   TCPIP_SOCKET_T socketid = 0;
   WE_INT  iRet = 0;
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;

   EME_UTILS_LOG_INFO(("OEM_OpenSocket : START!"));
   if(NULL == phOEMSocketHdl)
      return OEM_SOC_ERROR;
   /*
   step1:judge the type
   */
   
   EME_UTILS_LOG_INFO(("OEM_OpenSocket: type is :%d", Type));
   EME_UTILS_LOG_INFO(("OEM_OpenSocket: netid is :%d", g_email_netid));
   
   if (SOCK_DGRAM == Type || SOCK_STREAM == Type)
   {
      socketid = sci_sock_socket(AF_INET, Type, 0, g_email_netid);
      EME_UTILS_LOG_INFO(("OEM_OpenSocket: socket id is:%d", socketid));
      EME_UTILS_LOG_INFO(("OEM_OpenSocket: socket id is:%d", socketid));
   }
   else
   {
      return OEM_SOC_ERROR;
   }

   if (socketid < 0)
   {
      EME_UTILS_LOG_INFO(("OEM_OpenSocket: get last error code is:%d", socketid));
      return OEM_SOC_ERROR;
   }

   EME_UTILS_LOG_INFO(("OEM_OpenSocket : END!"));

   pstOEMSocketHdl->hSocket = socketid;

   return OEM_SOC_SUCCESS;
}



/*====================================================================
DESCRIPTION
    OEM_CloseSocket
       close the socket and free the pppdata and sockethandle 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/
WE_INT32  OEM_CloseSocket(HOEMSOCKETHANDLE phOEMSocketHdl)
{
   WE_INT32 nDialId = 0;
   WE_INT32 nRet = OEM_SOC_ERROR;
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;
   WE_INT32 iIndex = 0;

   EME_UTILS_LOG_INFO(("OEM_CloseSocket   : START!!"));
   EME_UTILS_LOG_INFO(("OEM_CloseSocket   : START!!"));
   
   if(NULL == phOEMSocketHdl)
      return OEM_SOC_ERROR;

   EME_UTILS_LOG_INFO(("OEM_CloseSocket   : hSocket = %d!!", pstOEMSocketHdl->hSocket));
   EME_UTILS_LOG_INFO(("OEM_CloseSocket   : hSocket = %d!!", pstOEMSocketHdl->hSocket));

   if(pstOEMSocketHdl->hSocket > 0)
   {
      nRet = sci_sock_socketclose(pstOEMSocketHdl->hSocket);
      EME_UTILS_LOG_INFO(("OEM_CloseSocket   : soc_close iret = %d!!", nRet));
      pstOEMSocketHdl->hSocket = 0;
   }

   while(iIndex < EME_SOCKET_CB_MAX)
   {
      CALLBACK_Cancel(&pstOEMSocketHdl->m_SocketCallback[iIndex]);
      EME_UTILS_FREEIF(NULL, pstOEMSocketHdl->m_SocketCallbackData[iIndex]);
      iIndex++;
   }

   iIndex = 0;
   while(iIndex < OEM_TIMER_MAX)
   {
      if(pstOEMSocketHdl->m_SocketTimer[iIndex] != NULL)
      {
         SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[iIndex]);
         pstOEMSocketHdl->m_SocketTimer[iIndex] = NULL;
      }

      iIndex++;
   }

   EME_UTILS_LOG_INFO(("OEM_CloseDial:OEM_FreeSocHandle End"));
   OEM_FreeSocHandle(&phOEMSocketHdl);
//    EME_GetDialId(&nDialId);
//    EME_UTILS_LOG_INFO(("OEM_CloseDial   : EME_DisconnectPPP!!"));	
//    EME_DisconnectPPP(nDialId);
//    EME_UTILS_LOG_INFO(("OEM_CloseDial   : EME_DisconnectPPP!"));
//    EME_FreePPPData();
//    EME_UTILS_LOG_INFO(("OEM_CloseDial   : EME_FreePPPData END !!"));
   return OEM_SOC_SUCCESS;
}

/*====================================================================
DESCRIPTION
    OEM_GetHostByName
         get the host by the name 
         if soc_gethostbyname return success, syn to start the callback registered 
         else asyn to start the callback registered
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/
 
WE_INT32 OEM_GetHostByName(HOEMSOCKETHANDLE phOEMSocketHdl, const char *pcHostName, OEMDNSResult *pres, PFNNOTIFY pcb, void *pUser)
{
   WE_UINT8 acIPBuff[16] = {0},uiBuffLen = 0;
   WE_INT32 nrequestid = 0;
   WE_INT8   nRet = OEM_SOC_SUCCESS;
   OEMDNSResult *pstDnsRet = {0};
   struct sci_hostent *pstHostent = NULL;
   TCPIP_HOST_HANDLE hHostHdl = 0;

    WE_INT32 i = 0;
    const WE_CHAR *pcTmpStr = NULL;

   St_OemSocketHanlde *pstOEMSocketHdl =(St_OemSocketHanlde*)phOEMSocketHdl;

   EME_UTILS_LOG_INFO(("OEM_GetHostByName: START! "));
   EME_UTILS_LOG_INFO(("OEM_GetHostByName: START! "));
   
   EME_UTILS_LOG_INFO(("OEM_GetHostByName: pcHostName = %s\n!", pcHostName));
   EME_UTILS_LOG_INFO(("OEM_GetHostByName: pUser = 0x%x\n!", pUser));

   if (NULL == pcHostName || NULL ==pres || NULL == pUser || phOEMSocketHdl == NULL)
   {
      EME_UTILS_LOG_INFO(("OEM_GetHostByName:  bad parameters!"));
      return OEM_SOC_ERROR;
   } 

   pstOEMSocketHdl->pres = pres;
   pstOEMSocketHdl->uiEvtType = OEM_SOCK_EVT_GETHOSTNAME;
   pstOEMSocketHdl->pHostFn = pcb;
   pstOEMSocketHdl->pUser = pUser;
   
   hHostHdl = sci_async_gethostbyname(pcHostName, g_email_task_id, EME_SOCKET_MAX_TIMEROUT, g_email_netid);
   if(hHostHdl == 0)
   {
       nRet = OEM_SOC_ERROR;
       EME_UTILS_LOG_INFO(("OEM_GetHostByName: fail, err = 0x%08x, %d", nRet, nRet));
   }

    if(nRet != EME_SUCCESS)
    {
       pstDnsRet = EME_UTILS_MALLOC(NULL, sizeof(OEMDNSResult));
       if(NULL == pstDnsRet)
          return EME_NOMEMORY;
       EME_UTILS_MEMSET(pstDnsRet, 0x00, sizeof(OEMDNSResult));
       
       pstDnsRet->nResult = OEM_SOC_ERROR;
       pstDnsRet->sAddrLen = 4;
       pstDnsRet->addrs[0] = (WE_UINT32)acIPBuff[3]
          | (WE_UINT32)acIPBuff[2] << 8		
          | (WE_UINT32)acIPBuff[1] << 16
          | (WE_UINT32)acIPBuff[0] << 24;
       
       OEM_SocketProcess(phOEMSocketHdl, OEM_SOCK_EVT_GETHOSTNAME, (void*)pstDnsRet);
    }
    else
    {
       OEM_CreateTimer(phOEMSocketHdl, OEM_TIMER_GETHOSTNAME);
    }
    
    EME_UTILS_LOG_INFO(("OEM_GetHostByName : END!"));

   return nRet;
}
 
/*====================================================================
DESCRIPTION
    OEM_Connect
        connet the network 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/
WE_INT  OEM_Connect(HOEMSOCKETHANDLE phOEMSocketHdl, INetAddr uiIp, INetPort usPort, PFNCONNECTCB pfn, WE_VOID *pUser)
{
   WE_INT nErr = OEM_SOC_SUCCESS;
   struct sci_sockaddr staddr = {0};
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;
   int iTunnel = 1;
   sci_fd_set rfds = {0}; 
   sci_fd_set wfds = {0}; 
   sci_fd_set efds = {0}; 
   int i = 0;
   int iRet = 0;

   if (NULL == pUser || phOEMSocketHdl == NULL)
   {
      EME_UTILS_LOG_INFO(("OEM_Connect : BADPARM!!!"));
      return OEM_SOC_ERROR;
   }

   if(pstOEMSocketHdl->hSocket < 0)
      return OEM_SOC_ERROR;
   
   EME_UTILS_LOG_INFO(("OEM_Connect : START!!!"));
   
   pstOEMSocketHdl->pConnectFn = pfn;
   pstOEMSocketHdl->pUser = pUser;
   EME_UTILS_MEMSET(&pstOEMSocketHdl->astOemSocAddr, 0x00, sizeof(St_OemSockAddr));
   
   EME_UTILS_LOG_INFO(("OEM_Connect : uiIp =%d", uiIp));
   EME_UTILS_LOG_INFO(("OEM_Connect : uiIp =%d", uiIp));
   
   EME_UTILS_LOG_INFO(("OEM_Connect : usPort =%d", usPort));
   EME_UTILS_LOG_INFO(("OEM_Connect : usPort =%d", usPort));

   pstOEMSocketHdl->astOemSocAddr.sAddrLen = 4;
   pstOEMSocketHdl->astOemSocAddr.aucAddr[0] = (WE_UINT8)(uiIp >>24 & 0xff);
   pstOEMSocketHdl->astOemSocAddr.aucAddr[1] = (WE_UINT8)(uiIp >>16 & 0xff );
   pstOEMSocketHdl->astOemSocAddr.aucAddr[2] = (WE_UINT8)(uiIp >>8 & 0xff);
   pstOEMSocketHdl->astOemSocAddr.aucAddr[3] = (WE_UINT8)(uiIp & 0xff);
   pstOEMSocketHdl->astOemSocAddr.usPort = usPort;
   
   
   EME_UTILS_LOG_INFO(("OEM_Connect :astOemSocAddr.aucAddr[0] = %d", pstOEMSocketHdl->astOemSocAddr.aucAddr[0] ));
   EME_UTILS_LOG_INFO(("OEM_Connect :astOemSocAddr.aucAddr[1] = %d", pstOEMSocketHdl->astOemSocAddr.aucAddr[1] ));
   EME_UTILS_LOG_INFO(("OEM_Connect :astOemSocAddr.aucAddr[2] = %d", pstOEMSocketHdl->astOemSocAddr.aucAddr[2] ));
   EME_UTILS_LOG_INFO(("OEM_Connect :astOemSocAddr.aucAddr[3] = %d", pstOEMSocketHdl->astOemSocAddr.aucAddr[3] ));

   staddr.family = AF_INET;
   staddr.port=htons(usPort);
   TF3RD_MEMCPY(&staddr.ip_addr,pstOEMSocketHdl->astOemSocAddr.aucAddr,pstOEMSocketHdl->astOemSocAddr.sAddrLen);

    if(!iTunnel)
    {
       nErr = sci_sock_connect(pstOEMSocketHdl->hSocket, ( struct sci_sockaddr * )&staddr,sizeof(staddr));
       if(nErr < 0)
       {
          EME_UTILS_LOG_INFO(("OEM_Connect : Error code =%d", nErr));
          return OEM_SOC_ERROR;
       }
    }
    else   //http connect, tunnel
    {
       char acOutputBuffer[512] = {0};
       int iNum = 0;
       unsigned long ulTmp = 1;
       
       //         pstOEMSocketHdl->astOemSocAddr.aucAddr[0] = 10;
       //         pstOEMSocketHdl->astOemSocAddr.aucAddr[1] = 0;
       //         pstOEMSocketHdl->astOemSocAddr.aucAddr[2] = 0;
       //         pstOEMSocketHdl->astOemSocAddr.aucAddr[3] = 172;
       //         pstOEMSocketHdl->astOemSocAddr.usPort = 80;
       // 
       //         staddr.port=htons(pstOEMSocketHdl->astOemSocAddr.usPort);
       //         TF3RD_MEMCPY(&staddr.ip_addr,pstOEMSocketHdl->astOemSocAddr.aucAddr,pstOEMSocketHdl->astOemSocAddr.sAddrLen);
       // 
       //         EME_UTILS_LOG_INFO(("OEM_Connect : 10.0.0.172:80"));
       
       nErr = sci_sock_setsockopt(pstOEMSocketHdl->hSocket, SO_NBIO, 0);
       if(nErr < 0)
       {
          WE_INT iError = 0;
          iError = sci_sock_errno(pstOEMSocketHdl->hSocket);
          EME_UTILS_LOG_ERROR(("OEM_Connect set socket opt failed"));
          return OEM_SOC_ERROR;
       }
       
       sci_sock_asyncselect(pstOEMSocketHdl->hSocket, g_email_task_id, AS_READ|AS_WRITE|AS_CONNECT);
       
       nErr = sci_sock_connect(pstOEMSocketHdl->hSocket, ( struct sci_sockaddr * )&staddr, sizeof(staddr));
       if(nErr != 0)
       {
          nErr = sci_sock_errno(pstOEMSocketHdl->hSocket);
          if(EINPROGRESS == nErr)
          {
             OEM_CreateTimer(pstOEMSocketHdl, OEM_TIMER_CONNECT);
             return EME_SUCCESS;
          }
          else
          {
             EME_UTILS_LOG_INFO(("OEM_Connect get laster error = %d", nErr));
             nErr = EME_FAILED;
             OEM_SocketProcess(phOEMSocketHdl, OEM_SOCK_EVT_CONNECT, (void*)OEM_SOC_ERROR);
          }
       }
       else
       {
          OEM_SocketProcess(phOEMSocketHdl, OEM_SOCK_EVT_CONNECT, (void*)OEM_SOC_SUCCESS);
       }
    }

//         TF3RD_SPRINTF( acOutputBuffer, "CONNECT %d.%d.%d.%d:%d HTTP/1.0\r\nHost: %d.%d.%d.%d\r\n\r\n",
//             (uiIp >>24 & 0xff), (uiIp >>16 & 0xff), (uiIp >>8 & 0xff), (uiIp & 0xff), usPort, 
//             (uiIp >>24 & 0xff), (uiIp >>16 & 0xff), (uiIp >>8 & 0xff), (uiIp & 0xff));
// 
//         iNum = sci_sock_send(pstOEMSocketHdl->hSocket, (WE_UINT8*)acOutputBuffer, (WE_UINT16)strlen(acOutputBuffer), 0); 
//         if(iNum < 0)
//         {
//             iNum = sci_sock_errno(pstOEMSocketHdl->hSocket);
//             EME_UTILS_LOG_INFO(("OEM_Connect error, code=%d", iNum));
//             return OEM_SOC_ERROR;
//         }
//         EME_UTILS_LOG_INFO(("OEM_Connect: HTTP connect send %s!", acOutputBuffer));    
// 
//         i = 0;
//         while(i < 50) // 50*100ms = 5s
//         {
//             SCI_FD_ZERO(&rfds);
//             SCI_FD_SET(pstOEMSocketHdl->hSocket, &rfds);
//             iRet = sci_sock_select(&rfds, NULL, NULL, 0);
//             if(iRet > 0)
//             {
//                 EME_UTILS_LOG_INFO(("OEM_Connect: select recv ok" ));
//                 break;
//             }
//             else if(iRet == 0)
//             {
//                 EME_UTILS_LOG_INFO(("OEM_Connect: select recv timeout" ));
//                 i++;
//                 continue;
//             }
//             else
//             {
//                 EME_UTILS_LOG_INFO(("OEM_Connect: select recv err" ));
//                 return OEM_SOC_ERROR;
//             }
//         }
//         
//         if(i==50)
//         {
//             EME_UTILS_LOG_INFO(("OEM_Connect: select recv timeout" ));
//             return OEM_SOC_ERROR;
//         }
// 
//         TF3RD_MEMSET(acOutputBuffer, 0, 512);
//         EME_UTILS_LOG_INFO(("OEM_Connect: soc_connect recv..."));
//         iNum = sci_sock_recv(pstOEMSocketHdl->hSocket, acOutputBuffer, 512, 0);
//         if(iNum < 0)
//         {
//             iNum = sci_sock_errno(pstOEMSocketHdl->hSocket);
//             EME_UTILS_LOG_INFO(("OEM_Connect error, code=%d", iNum));
//             return OEM_SOC_ERROR;
//         }
//         EME_UTILS_LOG_INFO(("OEM_Connect: HTTP connect reply =%s!", acOutputBuffer));    
//         if(NULL == TF3RD_STRSTR(acOutputBuffer, "220"))
//         {
//             return OEM_SOC_ERROR;
//         }
//         else
//         {
//             nErr = 0;
//         }

   EME_UTILS_LOG_INFO(("OEM_Connect: soc_connect return :%d!", nErr));    
   return nErr;
}

/*====================================================================
DESCRIPTION
    OEM_SocketWrite
        Write the data, return the size wrote
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/

WE_INT  OEM_SocketWrite(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UCHAR * pBuffer, WE_UINT uiBytes)
{
   WE_INT32   nSendNum = OEM_SOC_SUCCESS;
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;

   EME_UTILS_LOG_INFO(("OEM_SocketWrite : START!!!"));

   if (phOEMSocketHdl == NULL)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketWrite: BADPARM!"));
      return OEM_SOC_ERROR;
   }

   if(pstOEMSocketHdl->hSocket <= 0)
      return OEM_SOC_ERROR;
  
   if(NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW])
   {
      SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
      pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
   }
   
   if(OEM_SOC_TIMEOUT == pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW])
   {
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW] = OEM_SOC_SUCCESS;
      return OEM_SOC_TIMEOUT;
   }
   
   nSendNum = sci_sock_send(pstOEMSocketHdl->hSocket, (WE_UINT8*)pBuffer, (WE_UINT16)uiBytes, 0); 
   if(nSendNum < 0)
   {
      nSendNum = sci_sock_errno(pstOEMSocketHdl->hSocket);
      EME_UTILS_LOG_INFO(("OEM_SocketWrite: soc_send error :%d!", nSendNum));
      if(nSendNum == EWOULDBLOCK)
      {
         return OEM_SOC_WOULDBLOCK;
      }
   }
   EME_UTILS_LOG_INFO(("OEM_SocketWrite: soc_send return :%d!", nSendNum));

   EME_UTILS_LOG_INFO(("OEM_SocketWrite : END!!!"));

   return nSendNum;
}

/*====================================================================
DESCRIPTION
    OEM_SocketWriteable
        only register the callback of writeable
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/

WE_VOID OEM_SocketWriteable(HOEMSOCKETHANDLE phOEMSocketHdl, PFNNOTIFY pfn, WE_VOID * pUser)
{
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;

   if (NULL == pUser || phOEMSocketHdl == NULL || pstOEMSocketHdl->hSocket <= 0)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketWriteable:  BADPARAM!"));
      return ;
   }
   pstOEMSocketHdl->pUser = pUser;
   pstOEMSocketHdl->pWriteFn = pfn;
   pstOEMSocketHdl->uiEvtType = OEM_SOCK_EVT_WRITE;
   OEM_CreateTimer(pstOEMSocketHdl, OEM_TIMER_RW);

   return;
}

/*====================================================================
DESCRIPTION
    OEM_SocketRead
       
PARAMETERS
   
RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/
#define SOCKET_NON_BLOCK

WE_INT  OEM_SocketRead(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UCHAR * pBuffer, WE_UINT uiBytes)
{
   WE_INT32 nRecNum = (WE_INT32)OEM_SOC_SUCCESS;
   unsigned long argp = 1;
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;
   int left_len = 0;

    sci_fd_set rfds = {0}; 
   
   EME_UTILS_LOG_INFO(("OEM_SocketRead : START!!!"));
   if (NULL == pBuffer || phOEMSocketHdl == NULL || pstOEMSocketHdl->hSocket <= 0)
   {
      return OEM_SOC_ERROR;
   }
   
   if(NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW])
   {
      SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
      pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
   }

   if(OEM_SOC_TIMEOUT == pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW])
   {
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW] = OEM_SOC_SUCCESS;
      return OEM_SOC_TIMEOUT;
   }

#ifdef SOCKET_NON_BLOCK
//    sci_sock_setsockopt(pstOEMSocketHdl->hSocket, SO_NBIO, 0);
//    sci_sock_asyncselect(pstOEMSocketHdl->hSocket, g_email_task_id, AS_READ|AS_WRITE);
#else
    SCI_FD_ZERO(&rfds);
    SCI_FD_SET(pstOEMSocketHdl->hSocket, &rfds);
    sci_sock_select(&rfds, NULL, NULL, 0);

    if(!SCI_FD_ISSET(pstOEMSocketHdl->hSocket, &rfds))
    {
        //EWOULDBLOCK
        EME_UTILS_LOG_INFO(("OEM_SocketRead: block" ));
        return -2;
    }
#endif

   nRecNum = sci_sock_asyncrecv(pstOEMSocketHdl->hSocket, pBuffer, uiBytes, 0, &left_len);
   EME_UTILS_LOG_INFO(("OEM_SocketRead: soc_recv want: %d, return :%d!", uiBytes, nRecNum));
   EME_UTILS_LOG_INFO(("OEM_SocketRead : END!!!"));

   if(nRecNum == -1)
   {
        nRecNum = sci_sock_errno(pstOEMSocketHdl->hSocket);
        EME_UTILS_LOG_INFO(("OEM_SocketRead: soc_recv last error :%d!", nRecNum));
        if(EWOULDBLOCK == nRecNum)
        {
            nRecNum = -2;
        }
        else
        {
            nRecNum = -1;
        }
   }

   if(left_len != 0)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketRead--: socket left length = %d", left_len));
      sci_sock_asyncselect(pstOEMSocketHdl->hSocket, g_email_task_id, AS_READ);
   }
   
#ifdef SOCKET_NON_BLOCK
//   sci_sock_setsockopt(pstOEMSocketHdl->hSocket, SO_NBIO, 0);
#endif

   return nRecNum;
}

/*====================================================================
DESCRIPTION
    OEM_SocketReadable
       Insert the item by the index 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/

WE_VOID OEM_SocketReadable(HOEMSOCKETHANDLE phOEMSocketHdl, PFNNOTIFY pfn, WE_VOID * pUser)
{
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;

   if (NULL == pUser || phOEMSocketHdl == NULL || pstOEMSocketHdl->hSocket <= 0)
   {
      return;
   }
   pstOEMSocketHdl->pUser = pUser;
   pstOEMSocketHdl->pReadFn = pfn;
   pstOEMSocketHdl->uiEvtType = OEM_SOCK_EVT_READ;
   OEM_CreateTimer(pstOEMSocketHdl, OEM_TIMER_RW);

   return ;
}


/*====================================================================
DESCRIPTION
    OEM_CloseDial
        close the dial 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/
static WE_INT8 OEM_CloseDial(WE_VOID)
{
   return OEM_SOC_SUCCESS; 
}
/*====================================================================
DESCRIPTION
    post_msg_to_socket
       Insert the item by the index 
PARAMETERS
   

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
typedef struct _St_SocketCB
{
   HOEMSOCKETHANDLE phOEMSocketHdl;
   WE_INT32 uiEvtType;
   WE_VOID  *pvData;
   WE_INT32 iIndex;
   WE_UINT32 uiTime;
}St_SocketCB;

WE_UINT32 OEM_SocketProcessCB(WE_VOID *pvUserData);

WE_INT32 OEM_SocketProcess(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UINT32 uiEvtType, WE_VOID *pvData)
{
   St_SocketCB *pstSocketCB = NULL;
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)phOEMSocketHdl;
   WE_INT32    iIndex = 0;

   if(NULL == phOEMSocketHdl)
      return OEM_SOC_ERROR;

   pstSocketCB = EME_UTILS_MALLOC(NULL, sizeof(St_SocketCB));
   if(NULL == pstSocketCB)
      return OEM_SOC_ERROR;

   EME_UTILS_MEMSET(pstSocketCB, 0x00, sizeof(St_SocketCB));
   pstSocketCB->pvData    = pvData;
   pstSocketCB->uiEvtType = uiEvtType;
   pstSocketCB->phOEMSocketHdl = phOEMSocketHdl;
   pstSocketCB->uiTime = EME_UTILS_GETTIMEMS;

   EME_UTILS_LOG_INFO(("OEM_SocketProcess--: Process start!!! EvtType = 0x%08x, time = 0x%08x", uiEvtType, pstSocketCB->uiTime));

   while(iIndex < EME_SOCKET_CB_MAX)
   {
      if(!CALLBACK_IsQueued(&pstOEMSocketHdl->m_SocketCallback[iIndex]))
      {
         EME_UTILS_LOG_INFO(("OEM_SocketProcess--: socket callback is not queued iIndex = %d", iIndex));
         break;
      }
      iIndex++;
   }

   if(iIndex == EME_SOCKET_CB_MAX)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketProcess--: socket callback full"));
      EME_UTILS_FREEIF(NULL, pstSocketCB);
      return OEM_SOC_ERROR;
   }

   pstSocketCB->iIndex = iIndex;
   pstOEMSocketHdl->m_SocketCallbackData[iIndex] = pstSocketCB;
   CALLBACK_Init(&pstOEMSocketHdl->m_SocketCallback[iIndex], OEM_SocketProcessCB, (WE_VOID*)pstSocketCB);
   ISHELL_EmailResume(&pstOEMSocketHdl->m_SocketCallback[iIndex]);

   EME_UTILS_LOG_INFO(("OEM_SocketProcess--: Process end!!!"));
   
   return OEM_SOC_SUCCESS;
}

static WE_VOID OEM_SocketTimerOutCB(WE_VOID *pvData)
{
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)pvData;

   EME_UTILS_LOG_INFO(("OEM_SocketTimerOutCB--: entry!!!"));

//   g_laster_error = OEM_SOC_TIMEOUT;

   switch(pstOEMSocketHdl->uiCurrType)
   {
   case OEM_SOCK_EVT_GETHOSTNAME:
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_GETHOSTNAME] = OEM_SOC_TIMEOUT;
      OEM_SocketProcess(pstOEMSocketHdl, OEM_SOCK_EVT_GETHOSTNAME, NULL);
      if(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME] != NULL)
      {
          SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME]);
          pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME] = NULL;
      }
      break;
      
   case OEM_SOCK_EVT_CONNECT:
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_CONNECT] = OEM_SOC_TIMEOUT;
      OEM_SocketProcess(pstOEMSocketHdl, OEM_SOCK_EVT_CONNECT, (void*)OEM_SOC_TIMEOUT);
      if(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT] != NULL)
      {
          SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT]);
          pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT] = NULL;
      }
      break;
   case OEM_SOCK_EVT_READ:
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW] = OEM_SOC_TIMEOUT;
      OEM_SocketProcess(pstOEMSocketHdl, OEM_SOCK_EVT_READ, (void*)OEM_SOC_TIMEOUT);
	if(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] != NULL)
      {
          SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
          pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
      }
      break;
   case OEM_SOCK_EVT_WRITE:
      pstOEMSocketHdl->m_SocketTimerError[OEM_TIMER_RW] = OEM_SOC_TIMEOUT;
      OEM_SocketProcess(pstOEMSocketHdl, OEM_SOCK_EVT_WRITE, (void*)OEM_SOC_TIMEOUT);
	if(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] != NULL)
      {
          SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
          pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
      }
      break;
   default:
      break;
   }

   return;
}

static WE_VOID OEM_SocketTimerOut(WE_UINT32 uiData)
{
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)uiData;
   int iIndex = 0;

   if(NULL == pstOEMSocketHdl)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketTimerOut--: bad parameter !!"));
      return;
   }

   while(iIndex < EME_SOCKET_CB_MAX)
   {
      if(!CALLBACK_IsQueued(&pstOEMSocketHdl->m_SocketCallback[iIndex]))
      {
         EME_UTILS_LOG_INFO(("OEM_SocketTimerOut--: socket callback is not queued iIndex %d", iIndex));
         break;
      }
      iIndex++;
   }

   if(iIndex == EME_SOCKET_CB_MAX)
   {
      EME_UTILS_LOG_INFO(("OEM_SocketTimerOut--: CALLBACK IS FULL"));
      return;
   }

   switch(pstOEMSocketHdl->uiCurrType)
   {
   case OEM_SOCK_EVT_GETHOSTNAME:
   case OEM_SOCK_EVT_CONNECT:
   case OEM_SOCK_EVT_READ:
   case OEM_SOCK_EVT_WRITE:
      EME_UTILS_LOG_INFO(("OEM_SocketTimerOut--: %d", pstOEMSocketHdl->uiCurrType));
      CALLBACK_Init(&pstOEMSocketHdl->m_SocketCallback[iIndex], OEM_SocketTimerOutCB, pstOEMSocketHdl);
      ISHELL_EmailResume(&pstOEMSocketHdl->m_SocketCallback[iIndex]);
      break;
   default:
      EME_UTILS_LOG_INFO(("OEM_SocketTimerOut--: Type = %d", pstOEMSocketHdl->uiCurrType));
      break;
   }
}

WE_UINT32 OEM_SocketProcessCB(WE_VOID *pvUserData)
{
   WE_UINT32      uiErr = OEM_SOC_ERROR;
   St_SocketCB     *pstSocketCB = (St_SocketCB*)pvUserData;
   WE_UINT32 uiEvtType = 0;
   WE_VOID *pvData = NULL;
   St_OemSocketHanlde *pstOEMSocketHdl = NULL;
   
   EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :  START ! "));
   if (NULL == pvUserData || pstSocketCB->phOEMSocketHdl == NULL)
   {   
      EME_UTILS_LOG_INFO(("OEM_SocketProcessCB : BADPARAM ! "));
      return OEM_SOC_ERROR;
   }

   EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :  SocketProcess Time = 0x%08x ! ", pstSocketCB->uiTime));
   pstOEMSocketHdl = (St_OemSocketHanlde*)pstSocketCB->phOEMSocketHdl;
   uiEvtType = pstSocketCB->uiEvtType;
   pvData    = pstSocketCB->pvData;
   pstOEMSocketHdl->m_SocketCallbackData[pstSocketCB->iIndex] = NULL;
   EME_UTILS_FREEIF(NULL, pstSocketCB);

   switch(uiEvtType) 
   {
   case OEM_SOCK_EVT_GETHOSTNAME:
      {
         if (pstOEMSocketHdl->pHostFn)
         {
            EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_GETHOSTNAME ! "));
            EME_UTILS_MEMCPY(pstOEMSocketHdl->pres, pvData, sizeof(OEMDNSResult));
            EME_UTILS_FREEIF(NULL, pvData);
            pstOEMSocketHdl->pHostFn(pstOEMSocketHdl->pUser);
            uiErr = OEM_SOC_SUCCESS;
         }
      }
      break;
      
   case OEM_SOCK_EVT_CONNECT:
      {
         EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_CONNECT start! ! "));
         if (pstOEMSocketHdl->pConnectFn)
         {
            EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_CONNECT callback! ! "));
            EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_CONNECT pvData = %d! ! ", pvData));
            pstOEMSocketHdl->pConnectFn(pstOEMSocketHdl->pUser, (WE_INT)pvData);
            uiErr = OEM_SOC_SUCCESS;
         }
      }
      break;
      
   case OEM_SOCK_EVT_READ:
      {
         EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_READ  ! ! "));
         
         if (pstOEMSocketHdl->pReadFn)
         {
            EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_READ  START! ! "));
            pstOEMSocketHdl->pReadFn(pstOEMSocketHdl->pUser);
            uiErr = OEM_SOC_SUCCESS;
         }   
      }
      break;
      
   case OEM_SOCK_EVT_WRITE:
      {
         EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_WRITE  ! ! "));
         if (pstOEMSocketHdl->pWriteFn)
         {
            EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_WRITE  callback! ! "));
            pstOEMSocketHdl->pWriteFn(pstOEMSocketHdl->pUser);
            uiErr = OEM_SOC_SUCCESS;
         }
      }
      break;
      
   case OEM_SOCK_EVT_DISDIAL:
      {
         EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_DISDIAL ! "));
         OEM_CloseDial();
         EME_UTILS_LOG_INFO(("OEM_SocketProcessCB :OEM_SOCK_EVT_DISDIAL  END ! "));
         uiErr = OEM_SOC_SUCCESS;
      }
      break;
      
	  default:
        break;
   }

   return uiErr;
}

int OEM_Socket_HandleEvent(xSignalHeaderRec *sig_ptr)
{
   St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)g_phOemSocketHdl;

   if(!sig_ptr || !g_phOemSocketHdl)
   {
      return -1;
   }

   switch(sig_ptr->SignalCode)
   {
   case SOCKET_READ_EVENT_IND:
      {
         EME_UTILS_LOG_INFO(("OEM_Socket_HandleEvent--: SOCKET_READ_EVENT_IND"));
         OEM_SocketProcess(g_phOemSocketHdl, OEM_SOCK_EVT_READ, EME_SUCCESS);
         if(NULL != pstOEMSocketHdl && NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW])
         {
            SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
            pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
         }
      }
      return 0;

   case SOCKET_WRITE_EVENT_IND:
      {
         EME_UTILS_LOG_INFO(("OEM_Socket_HandleEvent--: SOCKET_WRITE_EVENT_IND"));
         OEM_SocketProcess(g_phOemSocketHdl, OEM_SOCK_EVT_WRITE, EME_SUCCESS);
         if(NULL != pstOEMSocketHdl && NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW])
         {
            SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW]);
            pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_RW] = NULL;
         }
      }
      return 0;

   case SOCKET_CONNECT_EVENT_IND:
      {
         SOCKET_CONNECT_EVENT_IND_SIG_T *con_sig_ptr = (SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr;

         EME_UTILS_LOG_INFO(("OEM_Socket_HandleEvent--: connect %d", con_sig_ptr->error_code));
         if(con_sig_ptr->error_code == 0)
         {
            OEM_SocketProcess(g_phOemSocketHdl, OEM_SOCK_EVT_CONNECT, (void*)OEM_SOC_SUCCESS);
         }
         else
         {
            OEM_SocketProcess(g_phOemSocketHdl, OEM_SOCK_EVT_CONNECT, (void*)OEM_SOC_ERROR);
         }

         if(NULL != pstOEMSocketHdl && NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT])
         {
            SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT]);
            pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_CONNECT] = NULL;
         }
      }
      return 0;

   case SOCKET_CONNECTION_CLOSE_EVENT_IND:
      return -1;
      
   case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
      {
         ASYNC_GETHOSTBYNAME_CNF_SIG_T *dns_sig_ptr = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr;
         WE_UINT8 acIPBuff[16] = {0};
         WE_UINT8 uiBuffLen = 0;
         OEMDNSResult *pstDnsRet = NULL;
         
         pstDnsRet = EME_UTILS_MALLOC(NULL, sizeof(OEMDNSResult));
         EME_UTILS_MEMSET(pstDnsRet, 0x00, sizeof(OEMDNSResult));

         EME_UTILS_LOG_INFO(("OEM_Socket_HandleEvent--: get host by name %d", dns_sig_ptr->error_code));

         if(dns_sig_ptr->error_code == 0)
         {
            acIPBuff[0] = dns_sig_ptr->hostent.h_addr_list[0][0];
            acIPBuff[1] = dns_sig_ptr->hostent.h_addr_list[0][1];
            acIPBuff[2] = dns_sig_ptr->hostent.h_addr_list[0][2];
            acIPBuff[3] = dns_sig_ptr->hostent.h_addr_list[0][3];
            
            pstDnsRet->nResult = EME_SUCCESS;
            pstDnsRet->sAddrLen = 4;
            pstDnsRet->addrs[0] = (WE_UINT32)acIPBuff[3]
               | (WE_UINT32)acIPBuff[2] << 8		
               | (WE_UINT32)acIPBuff[1] << 16
               | (WE_UINT32)acIPBuff[0] << 24;	    
         }
         else
         {
            if(dns_sig_ptr->error_code == -24)
            {
               pstDnsRet->nResult = OEM_SOC_TIMEOUT;
            }
            else
            {
               pstDnsRet->nResult = OEM_SOC_ERROR;
            }
         }

         OEM_SocketProcess(g_phOemSocketHdl, OEM_SOCK_EVT_GETHOSTNAME, (void*)pstDnsRet);
         if(NULL != pstOEMSocketHdl && NULL != pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME])
         {
            SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME]);
            pstOEMSocketHdl->m_SocketTimer[OEM_TIMER_GETHOSTNAME] = NULL;
         }
      }
      return 0;

   default:
      return -1;
   }

//    if(iRet == 0)
//    {
//       St_OemSocketHanlde *pstOEMSocketHdl = (St_OemSocketHanlde*)g_phOemSocketHdl;
//       if(NULL != pstOEMSocketHdl->m_SocketTimer[pstOEMSocketHdl->uiCurrType])
//       {
//          SCI_DeleteTimer(pstOEMSocketHdl->m_SocketTimer[pstOEMSocketHdl->uiCurrType]);
//       }
//       pstOEMSocketHdl->m_SocketTimer[pstOEMSocketHdl->uiCurrType] = NULL;
//    }

   return -1;
}
