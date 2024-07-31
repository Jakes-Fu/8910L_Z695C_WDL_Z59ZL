/*==================================================================================================
    FILE NAME   : wap_tls.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       process interface for TLS module, data process interface
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
#include "tls_config.h"

#ifdef WAP_CONFIG_TLS
#include "tls_def.h"
#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h"
#include "ISecW.h"
#include "we_libalg.h"

#include "tls_ast.h"
#include "tls_ecdr.h" /*before wap_tlspub.h*/
#include "tls_msg.h"

#include "wap_tlsdef.h"  /*marco define*/
#include "wap_tlsst.h"  /*structer*/
#include "wap_tlspub.h"
#include "wap_tlshmcore.h"
#include "wap_tlscore.h" 
#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h"
#endif

#include "itls.h"
#include "wap_tls.h"
/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
#define WAP_TLS_OPT_CANCEL_INDEX   0
#define WAP_TLS_OPT_USER_CERT_INDEX 1
#define WAP_TLS_OPT_ERR_RESP_INDEX  2
#define WAP_TLS_OPT_PRIVATE_KEY_INDEX 3
#define WAP_TLS_OPT_ROOT_CERT_INDEX   4
#define WAP_TLS_OPT_VERSION_INDEX     5
#define WAP_TLS_OPT_USER_CERTS_INDEX  6

#define WAP_TLS_OPT_COUNT             7


/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static WE_INT32 WapTls_Process
(
    WE_HANDLE hTlsHandle,
    St_TlsMsg*pstMsg
);

static WE_HANDLE WapTls_Initialise(WE_HANDLE hMsgHandle);
static WE_INT32 WapTls_Terminate(St_WapTlsHandle *pstTlsHandle);
static WE_INT32  WapTls_SocketSend 
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId,
    WE_HANDLE pvBuf,
    WE_INT32 iLen
);
static WE_VOID WapTls_CertX509DERtoSize
(
    WE_UCHAR *pucCert, 
    WE_UINT16 *pusTmpStep,
    WE_UINT16 *pusSize
);
static WE_VOID WapTls_CertGetUINT16
(
    const WE_UINT8 *pucStr,
    WE_UINT16 *pusUint
);
static WE_INT32 WapTls_GetCertLen
(
    const WE_UCHAR *pucX509Cert,
    WE_UINT16 *pusCertLen
);
static WE_INT32 WapTls_GetOptIndex(WE_INT32 iId);
static WE_INT32 WapTls_DeleteRootCert(St_WapTlsHandle *pstTlsHandle);
static WE_INT32 WapTls_StoreRootCert
(
    St_WapTlsHandle *pstTlsHandle, 
    WE_VOID* pvBufffer
);

/*==================================================================================================
FUNCTION: 
    WapTls_Initialise
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Init Tls Module
ARGUMENTS PASSED:
    hAdaptHandle: adapt handle
    hMsgHandle  : message module handle
RETURN VALUE:
    if create success, return Tls Module handle
    else return NULL
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_HANDLE WapTls_Initialise(WE_HANDLE hMsgHandle)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    WE_INT32 i = 0;
    
   // WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise begin\n");)
    if (NULL == hMsgHandle)
    {
   //     WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise parameter error \n");)
        return NULL;
    }
    /*free when terminate*/
    pstTlsHandle = (St_WapTlsHandle*)WAP_MALLOC(sizeof(St_WapTlsHandle));
    if (NULL == pstTlsHandle)
    {
  //      WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise Malloc error\n");)
        return NULL;
    }

    pstTlsHandle->hMsgHandle = hMsgHandle;
    pstTlsHandle->hSecHandle = NULL;
    pstTlsHandle->hLibAlgHandle = NULL;
    pstTlsHandle->iOpenConnNumber = 0;
    for (i = 0; i < G_WAP_CFG_MAX_CONNECTIONS; i++)
    {
        pstTlsHandle->apstWapTlsConnTable[i]= NULL;
    }
    pstTlsHandle->pstHashInfoTab = WapTls_GetHashInfoTable();
    if (NULL == pstTlsHandle->pstHashInfoTab)
    {
        WAP_FREE(pstTlsHandle);
   //     WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise:Error HashInfo table NULL\n");)
        return NULL;
    }
    pstTlsHandle->pstCipherInfoTab = WapTls_GetCipherInfoTable();
    if (NULL == pstTlsHandle->pstCipherInfoTab)
    {
        WapTls_FreeHashInfoTable(pstTlsHandle->pstHashInfoTab);
        WAP_FREE(pstTlsHandle);
  //      WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise:Err CipherInfo table NULL\n");)
        return NULL;
    }
    pstTlsHandle->pstCipherSuTab = WapTls_GetCipherSuiteTable();
    if (NULL == pstTlsHandle->pstCipherSuTab)
    {
        WapTls_FreeHashInfoTable(pstTlsHandle->pstHashInfoTab);
        WapTls_FreeCipherInfoTable(pstTlsHandle->pstCipherInfoTab);
        WAP_FREE(pstTlsHandle);
   //     WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise:CipherSuite table NULL\n");)
        return NULL;
    }
    pstTlsHandle->ucVerMajor = M_WAP_TLS_PROTOCOL_VERSION_MAJOR;
    pstTlsHandle->ucVerMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR;  

    TlsMsg_RegisterProcess(hMsgHandle, E_WAP_MSG_MODULEID_TLS, WapTls_Process, pstTlsHandle);
 //   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Initialise end\n");)
    return pstTlsHandle;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_Terminate
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Terminate Tls Module
ARGUMENTS PASSED:
    hTlsHandle: Tls module handle
RETURN VALUE:
    -1: error
    0 : success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_Terminate(St_WapTlsHandle *pstTlsHandle)
{
    WE_INT32 i = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Terminate begin\n");)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Terminate TLS handle NULL\n");)
        return -1;
    }

    for (i = 0; i<G_WAP_CFG_MAX_CONNECTIONS;i++)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Terminate delete connection\n");)
        WapTls_ConnDelete(pstTlsHandle,i);
    }
    WapTls_FreeHashInfoTable(pstTlsHandle->pstHashInfoTab);
    WapTls_FreeCipherInfoTable(pstTlsHandle->pstCipherInfoTab);
    WapTls_FreeCipherSuiteTable(pstTlsHandle->pstCipherSuTab);
    TlsMsg_DeregisterProcess(pstTlsHandle->hMsgHandle, E_WAP_MSG_MODULEID_TLS);
    WAP_FREE(pstTlsHandle->hMsgHandle);
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
     for(i = 0; i < MAX_SERVER_CERT_NUM; i++)
     {
         if (pstTlsHandle->apstServerCert[i] != NULL)
         {
             WAP_FREE(pstTlsHandle->apstServerCert[i]->pcHost);
             if (NULL != pstTlsHandle->apstServerCert[i]->pvCertContent)
             {
                 WAP_FREE(pstTlsHandle->apstServerCert[i]->pvCertContent);
             }
 
             WAP_FREE(pstTlsHandle->apstServerCert[i]);
         }        
     }
#endif

    WAP_FREE( pstTlsHandle );
    pstTlsHandle = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Terminate End\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_Process
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process Messages of TLS module
ARGUMENTS PASSED:
    hTlsHandle: TLS module handle
    pstMsg    : Message
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    Messages contain to TLSHM 
==================================================================================================*/
static WE_INT32 WapTls_Process(WE_HANDLE hTlsHandle,St_TlsMsg*pstMsg)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process begin\n");)

    if (NULL == hTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process TLS handle NULL\n");)        
        return -1;
    }
    pstTlsHandle = (St_WapTlsHandle*)hTlsHandle;
    
   if ( (pstMsg->uiMsgType >= M_WAP_TLS_MSG_HM_BASE) && (pstMsg->uiMsgType <= M_WAP_TLS_MSG_LIMIT) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process send to TLSHM\n");)

        iRet = SendMsgToTlsHM(hTlsHandle, pstMsg);

        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_Process iRet = %d\n"),iRet);)
        return iRet;
    }
    switch (pstMsg->uiMsgType)
    {
        /*msg from TLSHM*/       
        case M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG:
             WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG\n");)
             iRet = WapTls_ProcSendMsg(pstTlsHandle,pstMsg->iParam1,M_WAP_TLS_CONTENT_TYPE_HANDSHAKE,
                                   pstMsg->pvParam1,pstMsg->uiParam1);
             break;    
        case M_WAP_TLS_TLSHM_MSG_SEND_CCS:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_SEND_CCS\n");)            
            iRet = WapTls_ProcSendCCS(pstTlsHandle,pstMsg->iParam1);
            break;
        case M_WAP_TLS_TLSHM_MSG_SEND_ALERT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_SEND_ALERT\n");)
            iRet = WapTls_ProcSendAlert(pstTlsHandle,pstMsg->iParam1,pstMsg->uiParam1);
            break;
        case M_WAP_TLS_TLSHM_MSG_CCS_PROCESSED:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_CCS_PROCESSED\n");)
            iRet = WapTls_ProcCCSProcessed(pstTlsHandle, pstMsg->iParam1);
            break;
        case M_WAP_TLS_TLSHM_MSG_OPEND:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_OPEND\n");)
            iRet = WapTls_ProcHmOpend(pstTlsHandle, pstMsg->iParam1);  
            break;
        case M_WAP_TLS_TLSHM_MSG_ERROR:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process M_WAP_TLS_TLSHM_MSG_ERROR\n");)
            iRet = WapTls_ProcErr(pstTlsHandle,pstMsg->iParam1, pstMsg->uiParam1, pstMsg->uiParam2);
            break;
        default:
             WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_Process unknown message\n");)
             break;            
    }
    return iRet;
}

/*==================================================================================================
FUNCTION: 
    WapTls_SocketSend
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send data from HTTP module
ARGUMENTS PASSED:
    hTlsHandle: TLS module handle
    iCidx    : Connection id
    pvBuf    : Data to be sent
    iLen     : Data length
    piResult : operation result
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32  WapTls_SocketSend 
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId,
    WE_HANDLE pvBuf,
    WE_INT32 iLen
)
{
    void *pvBufCopy = NULL;
    St_WapTlsConn *pstTlsConn = NULL;
   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend begin\n");)

    if ((NULL == pstTlsHandle)|| ( NULL == pvBuf)|| (iLen <= 0))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend TLS handle NULL\n");)
        return -1; 
    }
    pstTlsConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if (NULL == pstTlsConn)
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend TLS conn NULL\n");)
        return -1;
    }
    /*data last time is being sent,so this time will delay*/
    if (pstTlsConn->ucHttpSendProcessing) /*have data being sent*/
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend Delay\n");)
        return -2;
    }   

    /*free in WapTls_ProcSendMsg*/
    pvBufCopy = WAP_MALLOC(iLen);
    if (NULL == pvBufCopy)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend malloc error\n");)
        return -1;        
    }
    WAP_MEMCPY(pvBufCopy, pvBuf, iLen);
    /*data->record->sendrecs->record->outputbuf*/
    WapTls_ProcSendMsg(
                          pstTlsHandle,
                          iConnId,
                          M_WAP_TLS_CONTENT_TYPE_DATA,
                          pvBufCopy,
                          iLen);

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_SocketSend end ok\n");)

    return iLen;                     
}

static WE_VOID WapTls_CertX509DERtoSize
(
    WE_UCHAR *pucCert, 
    WE_UINT16 *pusTmpStep,
    WE_UINT16 *pusSize
)
{
    WE_UINT8 ucTmpLen=0;
    
    if (!pucCert||!pusTmpStep||!pusSize)
    {
        return;
    }
    pusTmpStep[0]=0;
    ucTmpLen = *pucCert;
    pucCert++;
    pusTmpStep[0]++;
    if (ucTmpLen < 128)                                
    {
        *pusSize = (WE_UINT16)ucTmpLen;                        
    }
    else
    {
        ucTmpLen -= 128;                    
        if (ucTmpLen==2)
        {
            WapTls_CertGetUINT16(pucCert,pusSize);
        }
        else
        {
            *pusSize=*pucCert; 
        }
        pusTmpStep[0] = (WE_UINT16)(pusTmpStep[0] + ucTmpLen);
        pucCert+=ucTmpLen;        
    }
}
static WE_VOID WapTls_CertGetUINT16
(
    const WE_UINT8 *pucStr,
    WE_UINT16 *pusUint
)
{
    WE_UINT16 usU = 0x0102; 
    
    if (!pucStr||!pusUint)
    {
        return;
    }
    if (0x01 == *((WE_UINT8*)&(usU))) 
    {
        *((WE_UINT8*)pusUint) = pucStr[0];
        *(((WE_UINT8*)pusUint)+1) = pucStr[1];
    }
    else                        
    {
        *((WE_UINT8*)pusUint) = pucStr[1];
        *(((WE_UINT8*)pusUint)+1) = pucStr[0];
    }
}

static WE_INT32 WapTls_GetCertLen
(
    const WE_UCHAR *pucX509Cert,
    WE_UINT16 *pusCertLen
)
{
    WE_UCHAR *pucCert = NULL;
    WE_UINT16 usTmpStep=0;
    WE_UINT16 usSize=0;    
    
    if ((NULL == pucX509Cert) || (NULL == pusCertLen))
    {
        return -1;
    }

    pucCert = (WE_UCHAR *)pucX509Cert;
    
    if(*pucCert != 0x30)                
    {
        return -1;
    }
    pucCert ++;
    
    WapTls_CertX509DERtoSize(pucCert, &usTmpStep, &usSize);   
    usSize += 1 + usTmpStep; /* addd 0x30 and length code. */
    if (*pusCertLen == 0)
    {
        *pusCertLen = usSize;
    }
    else if (*pusCertLen != usSize)
    {
        return -1;
    }
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetOptIndex
CREATE DATE:
    2007-06-29
AUTHOR:
    Wang Shuhua
DESCRIPTION:
   Get opt index for store it in array
ARGUMENTS PASSED:
    pstTlsHandle: tls handle
RETURN VALUE:
    -1: parameter error
    -2: malloc error 
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32 WapTls_GetOptIndex(WE_INT32 iId)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_GetOptIndex OptId is %d\n",iId);) 
    switch (iId)
    {
        case E_TLS_OPT_CANCEL:
            return WAP_TLS_OPT_CANCEL_INDEX;
        case E_TLS_OPT_ERR_RESP:
            return WAP_TLS_OPT_ERR_RESP_INDEX;                
        case E_TLS_OPT_PRIVATE_KEY:
            return WAP_TLS_OPT_PRIVATE_KEY_INDEX;
        case E_TLS_OPT_ROOT_CERT:
            return WAP_TLS_OPT_ROOT_CERT_INDEX;
        case E_TLS_OPT_USER_CERT:
            return WAP_TLS_OPT_USER_CERT_INDEX;
        case E_TLS_OPT_USER_CERTS:
            return WAP_TLS_OPT_USER_CERTS_INDEX;
        case E_TLS_OPT_VERSION:
            return WAP_TLS_OPT_VERSION_INDEX;
        default:
            break;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_GetOptIndex unknown optid\n");) 
    return -1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DeleteRootCert
CREATE DATE:
    2007-06-29
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Delete root cert in tls handle
ARGUMENTS PASSED:
    pstTlsHandle: tls handle
RETURN VALUE:
    -1: parameter error
    -2: malloc error 
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32 WapTls_DeleteRootCert(St_WapTlsHandle *pstTlsHandle)
{
    St_WapTlsCertList *pstList = NULL;
    St_WapTlsCertNode *pstNodeTmp = NULL;
    St_WapTlsCertNode *pstNode = NULL;
    
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleteRootCert null parameter\n");) 
        return -1;
    }

    pstList = pstTlsHandle->pstRootCert;
    if (NULL == pstList)
    {
        return 0; // no node
    }
    pstNodeTmp = pstList->pstHead;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleteRootCert head null\n");)
    while(NULL!=pstNodeTmp)
    {
        pstNode = pstNodeTmp;
        pstNodeTmp = pstNodeTmp->pstNext;

        if(NULL!=pstNode->pvBuffer)
        {
            WAP_FREE(pstNode->pvBuffer);
            pstNode->pvBuffer = NULL;
        }
        WAP_FREE(pstNode);
        pstNode = NULL;
        
    }

    pstList->pstHead = NULL;

    WAP_FREE(pstList);
    pstTlsHandle->pstRootCert = NULL;
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_StoreRootCert
CREATE DATE:
    2007-06-29
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Store root cert in tls handle
ARGUMENTS PASSED:
    pstTlsHandle: tls handle
    pvBuffer: root cert
RETURN VALUE:
    -1: parameter error
    -2: malloc error 
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32 WapTls_StoreRootCert
(
    St_WapTlsHandle *pstTlsHandle, 
    WE_VOID* pvBufffer
)
{
    St_WapTlsCertNode *pstNodeNew = NULL;
    St_WapTlsCertList *pstList = NULL;
    St_WapTlsCertNode *pstNodeLast = NULL;
    
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert null parameter\n");) 
        return -1;
    }
    if (NULL == pvBufffer)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert null parameter\n");)
        return 0;
    }
    pstNodeNew = (St_WapTlsCertNode*)WAP_MALLOC(sizeof(St_WapTlsCertNode));
    if (NULL == pstNodeNew)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert malloc fail\n");)
        return -2;
    }
    WapTls_GetCertLen(pvBufffer,(WE_UINT16*)&(pstNodeNew->iLength));
    if (0 == pstNodeNew->iLength)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert cert len 0\n");)
        return 0;
    }
    pstNodeNew->pvBuffer = WAP_MALLOC(pstNodeNew->iLength);
    if (NULL == pstNodeNew->pvBuffer)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert malloc fail\n");)
        return -2;
    }
    WAP_MEMCPY(pstNodeNew->pvBuffer,pvBufffer,pstNodeNew->iLength);
    pstNodeNew->iCertType = M_SEC_IWAPOPT_X509_ROOT_CERTS;
    pstNodeNew->pstNext = NULL;
    
    pstList = pstTlsHandle->pstRootCert;
    if (NULL == pstList)
    {
        pstTlsHandle->pstRootCert = pstList = (St_WapTlsCertList*)WAP_MALLOC(sizeof(St_WapTlsCertList));
        if (NULL == pstTlsHandle->pstRootCert)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert malloc fail\n");)
            return -2;
        }
        WAP_MEMSET(pstTlsHandle->pstRootCert,0,sizeof(St_WapTlsCertList));
    }
    pstNodeLast = pstList->pstHead;
    if( pstNodeLast != NULL )
    {
        while(NULL!=pstNodeLast->pstNext)
        {
            pstNodeLast = pstNodeLast->pstNext;
        }
    }
    
    if(NULL==pstNodeLast)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert insert head\n");)
        pstList->pstHead = pstNodeNew;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_StoreRootCert insert last\n");)
        pstNodeLast->pstNext = pstNodeNew;
    }
    pstTlsHandle->pstRootCert->iNew = TRUE;
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_RunMain
CREATE DATE:
    2007-06-29
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Dispatch one message to tls module
ARGUMENTS PASSED:
    pstTlsHandle: tls handle
RETURN VALUE:
    -1: parameter error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32 WapTls_RunMain(St_WapTlsHandle *pstTlsHandle)
{
    if (NULL == pstTlsHandle)
    {
        return -1;
    }
    if (NULL == pstTlsHandle->hMsgHandle)
    {
        return -1;
    }
    while (TlsMsg_HasMsg(pstTlsHandle->hMsgHandle))// into cycle
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_RunMain dispach one message\n");)
        TlsMsg_DispatchMsg(pstTlsHandle->hMsgHandle);
    }
    return 0;
}
/*==================================================================================================
FUNCTION: 
    ITls_Create
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Create TLS interface instance
ARGUMENTS PASSED:
    ITLS** ppITls[in/out]
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_Create(ITLS **ppITls)
{
    WE_HANDLE hMsgHandle = NULL;
    
    if (NULL == ppITls)
    {
      //  WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Create null parameter\n");)        
        return -1;
    }
    
    TlsMsg_Init(&hMsgHandle);
    if (NULL == hMsgHandle)
    {
      //  WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Create Init message fail\n");)        
        return -1;
    }
    *ppITls = WapTls_Initialise(hMsgHandle);
   // WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Create end\n");)    
    return 0;
}
/*==================================================================================================
FUNCTION: 
    ITls_Release
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Release tls instance
ARGUMENTS PASSED:
    pITls: tls instance
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_ReleaseHandle(ITLS *pITls)
{
    St_WapTlsHandle * pstTlsHandle = NULL;
    WE_INT32 iRet = -1;

    pstTlsHandle = (St_WapTlsHandle*)pITls;
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Release null parameter\n");) 
        return -1;
    }

    iRet = WapTls_Terminate(pstTlsHandle);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Release with iRet %d\n",iRet);) 
    return iRet;
}
/*==================================================================================================
FUNCTION: 
    ITls_Close
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Close one special connection
ARGUMENTS PASSED:
    pITls: TLS instance
    iConnId: connection id
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/

WE_INT32 ITls_Close(ITLS *pITls,WE_INT32 iConnId)
{
    St_WapTlsHandle * pstTlsHandle = NULL;
    St_WapTlsConn *pstConn = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Close begin with close %d\n",iConnId);) 

    pstTlsHandle = (St_WapTlsHandle*)pITls;
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Close null parameter\n");) 
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle, iConnId);
    if ( NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Close can't find conn,return\n");) 
        return 0;
    }    
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Close find conn,delete\n");) 
        WapTls_ConnDelete(pstTlsHandle,pstConn->iCidx);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_Close end\n");) 
    return 0;
}
/*==================================================================================================
FUNCTION: 
    ITls_RegRespCB
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Register callback function for app
ARGUMENTS PASSED:
    pITls: Tls instance
    pcb: callback function
    hPrivData: private data keep for callback registered
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_RegRespCB
(
    ITLS *pITls,
    Fn_ITlsRespCb pcb,
    WE_HANDLE hPrivData
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    if (NULL == pITls)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_RegRespCB null parameter\n");) 
        return -1;
    }
    pstTlsHandle = (St_WapTlsHandle*)pITls;
    
    pstTlsHandle->hcbRespunc = (WE_HANDLE)pcb;
    pstTlsHandle->hPrivData = hPrivData;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_RegRespCB end\n");)     
    return 0;
}
/*==================================================================================================
FUNCTION: 
    ITls_ProcData
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process data from socket or app
ARGUMENTS PASSED:
    pITls: Tls instance
    iConnId:Connection id
    eDataType: identify data type
    pvData: data to be processed
    iDataLen: data length
RETURN VALUE:
    -1: error
    >=0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_ProcData
(
    ITLS *pITls,
    WE_INT32 iConnId,
    E_TLS_DATA_TYPE eDataType,
    WE_VOID* pvData,
    WE_INT32 iDataLen
)
{
    WE_INT32 iResult = -1;
    St_WapTlsHandle *pstTlsHandle = NULL;
    if (NULL == pITls)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ProcData null parameter\n");) 
        return -1;
    }
    pstTlsHandle = (St_WapTlsHandle*)pITls;    
    switch (eDataType)
    {
        case E_TLS_DATA_FROM_APP:// APP DATA
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ProcData from app\n");) 
            iResult = WapTls_SocketSend(pstTlsHandle, iConnId, pvData,iDataLen);
            break;
        case E_TLS_DATA_FROM_SVR:   
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ProcData from SVR\n");) 
            iResult = WapTls_ProcDataFromSocket(pstTlsHandle,iConnId,pvData,iDataLen);
            break; 
        
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ProcData other data type\n");) 
            break;
            
    }
    WapTls_RunMain(pstTlsHandle);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ProcData end with %d\n",iResult);) 
    return iResult;
}
/*==================================================================================================
FUNCTION: 
    ITls_ResumeHandShake
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Resume handshake when user give user cert or user response
ARGUMENTS PASSED:
    pITls: tls instance
    iConnId: connection id
    pstOpt: opt array
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_ResumeHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    St_TlsOpt *pstOpt
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    St_TlsOpt stOpt[WAP_TLS_OPT_COUNT + 1] = {0};
    St_TlsOpt *pstOptTmp = NULL;
    WE_INT32 iId = 0;
    WE_INT32  iIndex = 0;
    WE_INT32 iDel = FALSE;
    WE_UINT16 usLen = 0;
    WE_UCHAR *pucCert = NULL;
    WE_UCHAR* pucTmp = NULL;
    WE_VOID* pvTmpPrivKey = NULL;  
    WE_UINT16 usKeyLen = 0;
    St_TlsRespData stResp = {0};
    WE_INT32 iRet = -1;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake begin %d\n",iConnId);) 
    if ((NULL == pstOpt) || (NULL == pITls))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake null parameter\n");)
        return -1;
    }
    pstOptTmp = pstOpt;
    pstTlsHandle = (St_WapTlsHandle*)pITls;
    while(1)
    {
        iId = pstOptTmp->eOptId;
        iIndex = WapTls_GetOptIndex(iId);
  
        if (iId == E_TLS_OPT_ROOT_CERT)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake Root cert \n");)
            iDel = TRUE;
        }
        else /*store other opt except root cert*/
        {
			if (iIndex != -1)
			{
				stOpt[iIndex].eOptId = iId;
				stOpt[iIndex].pvOpt = pstOptTmp->pvOpt;
			}
        }
  
        if (iId == E_TLS_OPT_END)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt end,break\n");)
            break;
        }
        else
        {
            pstOptTmp ++;
        }
    }

    /*For update root cert store in tls handle*/
    pstOptTmp = pstOpt;
    if (iDel == TRUE)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake delete root cert stored\n");)
        WapTls_DeleteRootCert(pstTlsHandle);
        iDel = FALSE;
        while(1)
        {
            iId = pstOptTmp->eOptId;
            if (iId == E_TLS_OPT_ROOT_CERT)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake store root cert\n");)
                iRet = WapTls_StoreRootCert(pstTlsHandle,pstOptTmp->pvOpt);
                if (iRet == -2)//malloc error
                {
                   stResp.iConnId = iConnId;
                   stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
                   ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                                               (WE_VOID*)&stResp,
                                                               pstTlsHandle->hPrivData); 
                   return -1;
                }
				pstOptTmp ++;
            }
            else if (iId == E_TLS_OPT_END)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt end,break\n");)
                break;
            }
            else
            {
                pstOptTmp ++;
            }
        }
    }
  
  
   if (stOpt[WAP_TLS_OPT_CANCEL_INDEX].eOptId == E_TLS_OPT_CANCEL)
   {
       WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt cancel\n");)
       TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,
                     E_WAP_MSG_MODULEID_TLS,
                     M_WAP_TLS_HM_MSG_CANCLE_PROC_CLIENT_AUTH,
                     iConnId, 0,
                     0, 0,
                     NULL,NULL);
  
   }
   else if (stOpt[WAP_TLS_OPT_ERR_RESP_INDEX].eOptId == E_TLS_OPT_ERR_RESP)
   {
       WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt err resp\n");)
       TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,
                       E_WAP_MSG_MODULEID_TLS,
                       M_WAP_TLS_HM_MSG_PROC_TRUST_ERROR,
                       iConnId, *(WE_INT32*)stOpt[WAP_TLS_OPT_ERR_RESP_INDEX].pvOpt,
                       0, 0,
                       NULL,NULL);
   }
   else 
   {
       WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt user cert or certs\n");)
       /*copy private key*/
       pucTmp = (WE_UINT8*)stOpt[WAP_TLS_OPT_PRIVATE_KEY_INDEX].pvOpt; 
       if (NULL == pucTmp)
       {
           usKeyLen = 0;
       }
       else
       {
           *((WE_UINT8*)(&usKeyLen))= (WE_UINT8)pucTmp[0];
           *((WE_UINT8*)(&usKeyLen) + 1) = (WE_UINT8)pucTmp[1];
           pvTmpPrivKey = WAP_MALLOC(usKeyLen + 2);
           if(NULL==pvTmpPrivKey)
           {
               stResp.iConnId = iConnId;
               stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
               ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                                           (WE_VOID*)&stResp,
                                                           pstTlsHandle->hPrivData);            
               return -1;
           }
           WAP_MEMCPY(pvTmpPrivKey, stOpt[WAP_TLS_OPT_PRIVATE_KEY_INDEX].pvOpt, usKeyLen + 2);
       }    
       if (stOpt[WAP_TLS_OPT_USER_CERT_INDEX].pvOpt != NULL)
       {
          WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt user cert \n");)
          /*Copy cert*/
           WapTls_GetCertLen(stOpt[WAP_TLS_OPT_USER_CERT_INDEX].pvOpt,&usLen);
         
           WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake user cert length %d\n",usLen);)
           pucCert = WAP_MALLOC(usLen);//free by tlshm
           if (NULL == pucCert)
           {
               stResp.iConnId = iConnId;
               stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
               ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                                           (WE_VOID*)&stResp,
                                                           pstTlsHandle->hPrivData);             
              return -1;
           }
           WAP_MEMCPY(pucCert,stOpt[WAP_TLS_OPT_USER_CERT_INDEX].pvOpt,usLen);   
  
           TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,
                           E_WAP_MSG_MODULEID_TLS,
                           M_WAP_TLS_HM_MSG_PROC_CLIENT_AUTH_ERROR,
                           iConnId, 0,
                           E_TLS_OPT_USER_CERT, 
                           usLen,
                           pucCert,
                           pvTmpPrivKey);            
       }
       else if (stOpt[WAP_TLS_OPT_USER_CERTS_INDEX].pvOpt != NULL)
       {
          WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt user certs\n");)
          pucTmp = (WE_UINT8*)stOpt[WAP_TLS_OPT_USER_CERTS_INDEX].pvOpt; 
          usLen = ((*pucTmp) << 16)|((*(pucTmp+1)) << 8)|(*(pucTmp+2));
          WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake user certs length %d\n",usLen);)
          pucCert = WAP_MALLOC(usLen); //free by tlshm
          if (NULL == pucCert)
          {
               stResp.iConnId = iConnId;
               stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
               ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                                           (WE_VOID*)&stResp,
                                                           pstTlsHandle->hPrivData);             
              return -1;
          }
          WAP_MEMCPY(pucCert,stOpt[WAP_TLS_OPT_USER_CERTS_INDEX].pvOpt,usLen); 

          TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,
                           E_WAP_MSG_MODULEID_TLS,
                           M_WAP_TLS_HM_MSG_PROC_CLIENT_AUTH_ERROR,
                           iConnId, 0,
                           E_TLS_OPT_USER_CERTS, 
                           usLen,
                           pucCert,
                           pvTmpPrivKey);               
       }
       else
       {
           WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake opt user cert null\n");)
           TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,
                           E_WAP_MSG_MODULEID_TLS,
                           M_WAP_TLS_HM_MSG_PROC_CLIENT_AUTH_ERROR,
                           iConnId, 0,
                           E_TLS_OPT_USER_CERT, 
                           0,
                           NULL,
                           pvTmpPrivKey);             
       }
      
   }   
   WapTls_RunMain(pstTlsHandle);
   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_ResumeHandShake end\n");)
   return 0;
  }
/*==================================================================================================
FUNCTION: 
    ITls_SetPara
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Set permanent parameter for tls handle
ARGUMENTS PASSED:
    pITls: tls instance 
    pstOpt: opt array
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_SetPara
(
    ITLS *pITls,
    St_TlsOpt *pstOpt
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    St_TlsOpt stOpt[WAP_TLS_OPT_COUNT + 1] = {0};
    St_TlsOpt *pstOptTmp = NULL;
    WE_INT32 iId = 0;
    WE_INT32  iIndex = 0;
    WE_INT32 iDel = FALSE;
    WE_INT32 iVersion = 0;
    
    if ((NULL == pstOpt) || (NULL == pITls))
    {
        return -1;
    }
    pstOptTmp = pstOpt;
    pstTlsHandle = (St_WapTlsHandle*)pITls;
    while(1)
    {
        iId = pstOptTmp->eOptId;
        iIndex = WapTls_GetOptIndex(iId);

        if (iId == E_TLS_OPT_ROOT_CERT)
        {
            iDel = TRUE;
        }
        else 
        {
            stOpt[iIndex].eOptId = iId;
            stOpt[iIndex].pvOpt = pstOptTmp->pvOpt;
        }

        if (iId == E_TLS_OPT_END)
        {
            break;
        }
        else
        {
            pstOptTmp ++;
        }
    }
    pstOptTmp = pstOpt;
    if (iDel == TRUE)
    {
        WapTls_DeleteRootCert(pstTlsHandle);
        iDel = FALSE;
        while(1)
        {
            iId = pstOptTmp->eOptId;
            if (iId == E_TLS_OPT_ROOT_CERT)
            {
                WapTls_StoreRootCert(pstTlsHandle,pstOptTmp->pvOpt);
                pstOptTmp ++;
            }
            else if (iId == E_TLS_OPT_END)
            {
                break;
            }
            else
            {
                pstOptTmp ++;
            }
        }
    }
#ifdef WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION
    /*user assign the protocol version*/
    if (stOpt[WAP_TLS_OPT_VERSION_INDEX].eOptId == E_TLS_OPT_VERSION)
    {
        iVersion = *(WE_INT32*)stOpt[WAP_TLS_OPT_VERSION_INDEX].pvOpt;
        if (iVersion == G_WAP_TLS_PRO_TLS_VERSION)
        {
            pstTlsHandle->ucVerMajor = M_WAP_TLS_PROTOCOL_VERSION_MAJOR;
            pstTlsHandle->ucVerMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR;
        }
        else if (iVersion == G_WAP_TLS_PRO_SSL_VERSION)
        {
            pstTlsHandle->ucVerMajor = M_WAP_SSL_PROTOCOL_VERSION_MAJOR;
            pstTlsHandle->ucVerMinor = M_WAP_SSL_PROTOCOL_VERSION_MINOR;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        pstTlsHandle->ucVerMajor = M_WAP_TLS_PROTOCOL_VERSION_MAJOR;
        pstTlsHandle->ucVerMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR;        
    }
#endif    
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    ITls_StartHandShake
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Start handshake(Prepare client hello pdu)
ARGUMENTS PASSED:
    pITls[in/out]
    iConnId:Connection id (for app to find socket)
    pucHost: host of request url
    bUseProxy: whether use proxy
    pucAddr: proxy ip address or host ip
    usPort: proxy port or host port
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_StartHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    WE_UCHAR* pucHost,
    WE_BOOL bUseProxy,
    WE_UCHAR *pucAddr,
    WE_UINT16 usPort
)
{
    St_WapTlsHandle * pstTlsHandle = NULL;
    St_WapTlsConn *pstConn = NULL;
    St_TlsRespData stResp = {0};
    
    pstTlsHandle = (St_WapTlsHandle*)pITls;
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake null parameter\n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle, iConnId);
    if ( NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake create new conn\n");)
        pstConn = WapTls_ConnNew( pstTlsHandle, iConnId);
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake have conn yet,delete\n");)
        WapTls_ConnDelete(pstTlsHandle, pstConn->iCidx);
        pstConn = WapTls_ConnNew( pstTlsHandle, iConnId);
    }
    if (NULL == pstConn)
    {
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake bUseProxy = %d\n",bUseProxy);)
    pstConn->bUseProxy = bUseProxy;
    pstConn->iAddrLen = (WE_INT32)(pucAddr[1]) + 2;
    pstConn->pucAddr = (WE_UCHAR*)WAP_MALLOC(pstConn->iAddrLen);
    if (NULL == pstConn->pucAddr)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake malloc fail\n");)
        stResp.iConnId = iConnId;
        stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                                    (WE_VOID*)&stResp,
                                                    pstTlsHandle->hPrivData);  
        return -1;
    }
    WAP_MEMCPY(pstConn->pucAddr,pucAddr,pucAddr[1] + 2);
    pstConn->usPort = usPort;
    if (NULL == pucHost)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake host null\n");)
        pstConn->pucDomain = NULL;
        pstConn->usDomainLen = 0;
    }
    else
    {
        pstConn->usDomainLen = WAP_STRLEN((WE_CHAR*)pucHost);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake host %s\n",pucHost);)
        pstConn->pucDomain = WAP_MALLOC(pstConn->usDomainLen);
        if (NULL == pstConn->pucDomain)
        {
            stResp.iConnId = iConnId;
            stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                (WE_VOID*)&stResp,
                                pstTlsHandle->hPrivData);
             return -1;
            
        }
        WAP_MEMCPY(pstConn->pucDomain,pucHost,pstConn->usDomainLen);
    }
    TlsMsg_PostMsg( pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_HM_MSG_START,
                    iConnId,0,0,0,NULL,NULL
                  );
    /*init connection state*/
    pstConn->ucState = M_WAP_TLS_STATE_HANDSHAKE_INITIATED;
    WapTls_RunMain(pstTlsHandle);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "ITls_StartHandShake end\n");)
    return 0;
}
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
/*==================================================================================================
FUNCTION: 
    ITls_StartHandShake
CREATE DATE:
    2007-06-28
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Start handshake(Prepare client hello pdu)
ARGUMENTS PASSED:
    pITls[in/out]
    iConnId:Connection id (for app to find socket)
    pucHost: host of request url
    bUseProxy: whether use proxy
    pucAddr: proxy ip address or host ip
    usPort: proxy port or host port
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 ITls_ViewSvrCert
(
    ITLS *pITls,
    WE_CHAR* pcHost,
    WE_VOID **ppvContent
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    WE_INT32 iRet = -1;
    
    if ((NULL == pITls) || (NULL == pcHost) || (NULL == ppvContent))
    {
        return iRet;
    }

    pstTlsHandle = (St_WapTlsHandle*)pITls;
    iRet = WapTlsHm_GetFitCert(pstTlsHandle,pcHost,ppvContent);

    return iRet;
}
#endif
#endif





















