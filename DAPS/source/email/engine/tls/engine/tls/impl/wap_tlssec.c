/*==================================================================================================
    FILE NAME   : wap_tlssec.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       The functions in this file would be used to process the response from SEC.
    
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
#include "tls_dbg.h"

#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h"
#include "ISecW.h"
#include "we_libalg.h"

#include "tls_ast.h"
#include "tls_ecdr.h" /*before wap_tlspub.h*/
#include "tls_msg.h"

#include "wap_tlsdef.h"
#include "wap_tlsst.h"
#include "wap_tlspub.h" 
#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h"
#endif
#include "wap_tlshmcore.h"

#include "itls.h"



/*==================================================================================================
FUNCTION: 
    WapTlsHm_PeerLookUpRespProc
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    Process the search peer's response from SEC module.
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    pResp: sec response
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
void WapTlsHm_PeerLookUpRespProc
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_SecSearchPeerResp *pstPeerResp = NULL;
#ifdef WAP_CONFIG_SSL    
    St_SecPeerOpt stPeerOpt = {G_SEC_REMOVE_PEER,NULL};
    St_SecSearchPeerPara stSearchPeer = {0};    
#endif
    WE_INT32 iResult = 0;
    St_SecSessionOpt stSessionOpt = {G_SEC_ENABLE_SESSION,NULL};
    St_SecSessionGetPara stGetSeession = {0};


    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc begin\n");)
    if (NULL == pResp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc response is NULL\n");)
        return;
    }    
    pstPeerResp = (St_SecSearchPeerResp*)pResp;
    if (NULL == pstTlsHandle)
    {
        WAP_FREE(pstPeerResp);
        return;
    }
    if ((NULL == pstConn)|| ( NULL == pstConn->pstHandshake ) )
    {
        WAP_FREE(pstPeerResp);
        return;
    }
  
    pstConn->pstHandshake->iSecurityId = pstPeerResp->iSecId;
 
    if (pstPeerResp->usResult != G_SEC_OK)
    {
        WAP_FREE(pstPeerResp);
#ifdef WAP_CONFIG_SSL
#ifndef WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION
        if (pstConn->pstHandshake->ucState == M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP)
        {

            pstConn->stProtocolVersion.ucMinor = M_WAP_SSL_PROTOCOL_VERSION_MINOR;
         
            stSearchPeer.iAddressLen = pstConn->iAddrLen;        
            stSearchPeer.pucAddress = pstConn->pucAddr;
            stSearchPeer.usPortNum = pstConn->usPort;
            stSearchPeer.iTargetID = pstConn->iObjectId;
            stPeerOpt.eType = G_SEC_SEARCH_PEER;
            stPeerOpt.hValue = &(stSearchPeer);
            /*modify the state just for ssl will look up peer at first time only*/
            pstConn->pstHandshake->ucState = (WE_UINT8)M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP_1;            
            ISecW_PeerOpts(pstTlsHandle->hSecHandle,&stPeerOpt, G_SEC_SSL_CONNECTION_TYPE);           

            return;   
          
        }
        else
        {
            pstConn->stProtocolVersion.ucMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR;

        }
#endif        
#endif
        iResult = WapTlsHm_CreateClientHello(pstTlsHandle,pstConn);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc create client hello error\n");)
            return;
        }
        return;
    }

    /*Find the session*/
    pstConn->pstPendingRead->iMasterSecretId = pstPeerResp->ucMasterSecretId;
    pstConn->pstPendingWrite->iMasterSecretId = pstPeerResp->ucMasterSecretId;

    stGetSeession.iMasterSecretId = pstPeerResp->ucMasterSecretId;
    stGetSeession.iTargetID = pstConn->iObjectId;
    stSessionOpt.eType = G_SEC_GET_SESSION;
    stSessionOpt.hValue = &(stGetSeession);

    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SESSION_FETCH_RESP;
    
    iResult = ISecW_SessionOpts(pstTlsHandle->hSecHandle, &stSessionOpt);
    if (iResult != G_SEC_OK)
    {
        WAP_FREE(pstPeerResp);
        pstPeerResp = NULL;
        return;
    }
    WAP_FREE(pstPeerResp);
    pstPeerResp = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc End\n");)
}

#else
void WapTlsHm_PeerLookUpRespProc
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_SecSearchPeerResp *pstPeerResp = NULL;
    WE_INT32 iResult = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc begin\n");)
    if (NULL == pResp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc response is NULL\n");)
        return;
    }    
    pstPeerResp = (St_SecSearchPeerResp*)pResp;
    if (NULL == pstTlsHandle)
    {
        WAP_FREE(pstPeerResp);
        return;
    }
    if ((NULL == pstConn)|| ( NULL == pstConn->pstHandshake ) )
    {
        WAP_FREE(pstPeerResp);
        return;
    }
    
    pstConn->pstHandshake->iSecurityId = pstPeerResp->iSecId;
 
    if (pstPeerResp->usResult != G_SEC_OK)
    {
        WAP_FREE(pstPeerResp);
#ifdef WAP_CONFIG_SSL
        if (pstConn->pstHandshake->ucState == M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP)
        {
            pstConn->stProtocolVersion.ucMinor = M_WAP_SSL_PROTOCOL_VERSION_MINOR;
            /*modify the state just for ssl will look up peer at first time only*/
            pstConn->pstHandshake->ucState = (WE_UINT8)M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP_1;
            
            ISecW_SearchPeer( pstTlsHandle->hSecHandle, 
                               (WE_UINT16)pstConn->iObjectId,                          
                               pstConn->pucAddr, 
                               (WE_UINT8)pstConn->iAddrLen,                          
                                pstConn->usPort,
                                G_SEC_SSL_CONNECTION_TYPE );            

            return;
        }
        else
        {
            pstConn->stProtocolVersion.ucMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR;

        }
#endif
        iResult = WapTlsHm_CreateClientHello(pstTlsHandle,pstConn);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc create client hello error\n");)
            return;
        }
        return;
    }

    /*Find the session*/
    pstConn->pstPendingRead->iMasterSecretId = pstPeerResp->ucMasterSecretId;
    pstConn->pstPendingWrite->iMasterSecretId = pstPeerResp->ucMasterSecretId;
    
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SESSION_FETCH_RESP;
    iResult = ISecW_SessionGet( pstTlsHandle->hSecHandle, pstConn->iObjectId,
                    pstPeerResp->ucMasterSecretId );
    if (iResult != G_SEC_OK)
    {
        WAP_FREE(pstPeerResp);
        pstPeerResp = NULL;
        return;
    }
    WAP_FREE(pstPeerResp);
    pstPeerResp = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_PeerLookUpRespProc End\n");)
}
#endif
/*==================================================================================================
FUNCTION: 
    WapTlsHm_StoreSessionData
CREATE DATE: 
    2006-12-28
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    Fetech the session ,store the session information
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    pResp: sec response
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_StoreSessionData
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_SecSessionGetResp* pstSessResp = NULL;
    St_WapTlsCipherSuiteInfo *pstCipherSuiteInfo = NULL;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData begin\n");)
    if (NULL == pResp)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData response is null\n");)
        return -1;
    }

    pstSessResp = (St_SecSessionGetResp*)pResp;

    if (NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData handle is null\n");)
        if( NULL != pstSessResp->pucSessionId )
        {
            WAP_FREE(pstSessResp->pucSessionId);
        }
        WAP_FREE(pstSessResp);
        return -1;
    }

    if ( NULL == pstConn->pstHandshake || NULL == pstSessResp->pucSessionId )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData parameter error\n");)
        if( NULL != pstSessResp->pucSessionId )
        {
            WAP_FREE( pstSessResp->pucSessionId );
        }
        WAP_FREE( pstSessResp );
        return -1;
    }
    if ((pstSessResp->usResult == G_SEC_OK) 
        && (pstSessResp->ucSessionOptions & G_SEC_SESSION_OPTIONS_RESUMABLE)
        && (0 != pstSessResp->usSessionIdLen ) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData get session to resume\n");)
        WAP_MEMCPY(pstConn->pstHandshake->stSessionId.aucId, pstSessResp->pucSessionId,pstSessResp->usSessionIdLen);
        pstConn->pstHandshake->stSessionId.ucLength = (WE_UINT8)pstSessResp->usSessionIdLen;

        pstCipherSuiteInfo = WapTls_FindCipherSuite(pstTlsHandle,
                                                    (WE_UINT16)((pstSessResp->aucCipherSuite[0] << 8)
                                                             | (pstSessResp->aucCipherSuite[1])));

        pstConn->pstHandshake->usCipherSuite = pstCipherSuiteInfo->usCipherSuite;
        pstConn->pstPendingRead->ucHashAlg = pstCipherSuiteInfo->ucHashAlg;
        pstConn->pstPendingWrite->ucHashAlg = pstCipherSuiteInfo->ucHashAlg;
        pstConn->pstPendingRead->ucCipherAlg = pstCipherSuiteInfo->ucCipherAlg;
        pstConn->pstPendingWrite->ucCipherAlg = pstCipherSuiteInfo->ucCipherAlg;
        pstConn->pstPendingRead->ucCompressionAlg = pstSessResp->ucCompressionAlg;
        pstConn->pstPendingWrite->ucCompressionAlg = pstSessResp->ucCompressionAlg;
        
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:StoreSessionData no session to resume\n");)
        pstConn->pstHandshake->stSessionId.ucLength = 0;
    }
    if( NULL != pstSessResp->pucSessionId )
    {
        WAP_FREE(pstSessResp->pucSessionId);
    }
    WAP_FREE(pstSessResp);        

    return 0;   
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSendClientHello
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Fill the client hello structure.And notify tls to send client hello. 
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    This funcation will free the parameter 'pResp'.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientHello
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_WapTlsHandshake      *pstHs = NULL;
    St_WapTlsClientHello    stCHello = {0};
    St_WapEcdrEncoder       stEncoder = {0};
    St_SecTlsGetCipherSuitesResp   *pstGetCsResp = NULL;
    St_TlsRespData stResp = {0};
    WE_UCHAR    *pucBuffer = NULL;
    WE_UINT32    uiBufLen = 0;
    WE_UINT8    ucMsgType = 0;
    M_WAP_TLS_UINT24    tMsgLen = 0;
    WE_INT32        iResult = -1;/*for check funcation result*/
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello begin\n");)
    if ( NULL == pResp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello parameter error\n");)
        return -1;
    }
    pstGetCsResp = (St_SecTlsGetCipherSuitesResp *)pResp;
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello TLS handle NULL\n");)
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE(pstGetCsResp->pucCipherSuites);
        }
        WAP_FREE(pstGetCsResp);
        return -1;
    }
    pstHs = pstConn->pstHandshake;
    
    if( pstGetCsResp->usResult != G_SEC_OK || NULL == pstConn->pstHandshake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello parameter error\n");)
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, \
                                        G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                        M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
        return -1;
    }
    stCHello.stClientVer = pstConn->stProtocolVersion;
    WAP_MEMCPY( stCHello.taucRandom, pstHs->taucClientRandom, 32);
    stCHello.stSessionId = pstHs->stSessionId;

    stCHello.aucCompressMethods[0] = M_WAP_TLS_COMPRESSION_NULL;
    stCHello.ucNumCompressMethods = 1;
    
    stCHello.pucCipherSuit = pstHs->pucCipherSuites = pstGetCsResp->pucCipherSuites;
    stCHello.usCipherSuitLen = pstHs->usCipherSuitesLen = pstGetCsResp->usCipherSuitesLen;

    iResult = WapTls_GetClientHelloSize( &stEncoder, &stCHello);/*not need create encoder*/
    if( iResult < 0)
    {
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello:Encode clientHello size error\n");)
        return -1;
    }
    ucMsgType = M_WAP_TLS_HANDSHK_CLIENT_HELLO;
    tMsgLen = stEncoder.iPosition;
    uiBufLen = (WE_UINT32)(tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE);
    pucBuffer = (WE_UCHAR *)WAP_MALLOC( uiBufLen );/*free in WapTls_ProcSendMsg*/
    if( NULL == pucBuffer )
    {
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello malloc error\n");)

        stResp.iConnId = pstConn->iConnId;
        stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                               (WE_VOID*)&stResp,
                               pstTlsHandle->hPrivData);  
         
        return -1;
    }

    WapTls_EncodeInit( &stEncoder, pucBuffer, uiBufLen );
    iResult = TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
    if( iResult < 0)
    {
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello EncodeUint8 error\n");)
        return -1;
    }
    iResult = WapTls_EncodeUint24( &stEncoder, &tMsgLen );
    if( iResult < 0)
    {
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello EncodeUint24 error\n");)
        return -1;
    }
    iResult = WapTls_EncodeClientHello( &stEncoder, &stCHello );
    if( iResult < 0)
    {
        if( NULL != pstGetCsResp->pucCipherSuites )
        {
            WAP_FREE( pstGetCsResp->pucCipherSuites );
        }
        WAP_FREE( pstGetCsResp );
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientHello EncodeclientHello error\n");)
        return -1;
    }
    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_HELLO;
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO;

    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, pstConn->pstHandshake,
                                    stEncoder.pucData, stEncoder.iLength );
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendClientHello---send handshake message \n");)
    TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                    E_WAP_MSG_MODULEID_TLS, 
                    M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG,
                    pstConn->iConnId,
                    0,
                    stEncoder.iLength,
                    0,
                    (void *)stEncoder.pucData,
                    NULL );
    WAP_FREE(pResp);
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSendClientKeyExch
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    Fill the client key exchange structure.And notify tls to send client key exchange. 
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    St_WapTlsHandshake  *pstHs = NULL;
    St_WapEcdrEncoder   stEncoder = {0};
    St_TlsRespData stResp = {0};
    WE_UINT16           usBufLen = 0;
    WE_UCHAR            *pucBuffer = NULL;
    WE_UINT8            ucMsgType = 0;
    M_WAP_TLS_UINT24    tMsgLen = 0;
    WE_INT32            iResult = 0;/*just for check funcations' result*/
    

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch begin\n");)
    
    if( NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch parameter error\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if (NULL == pstHs)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch parameter error\n");)
        return -1;
    }
    ucMsgType = M_WAP_TLS_HANDSHK_CLIENT_KEY_EXCHANGE;
#ifdef WAP_CONFIG_SSL
    if( pstHs->ucFlags & M_WAP_TLS_FLAG_IS_SSL )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch: Is SSL\n");)
        tMsgLen = pstHs->usKeyExchPubValueLen;
    }
    else
#endif
    {
   
        tMsgLen = pstHs->usKeyExchPubValueLen + 2; /*Two bytes for length*/
    }
    usBufLen = (WE_UINT16)(tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE);
    pucBuffer = (WE_UCHAR*)WAP_MALLOC(usBufLen);/*free in WapTls_ProcSendMsg*/
    if( NULL == pucBuffer )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch: Malloc fail\n");)
        stResp.iConnId = pstConn->iConnId;
        stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                               (WE_VOID*)&stResp,
                               pstTlsHandle->hPrivData);              
        return -1;
    }
    WapTls_EncodeInit(&stEncoder, pucBuffer, usBufLen);
    iResult = TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
    if(iResult < 0)
    {
        WAP_FREE(pucBuffer);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch encoder error\n");)
        return -1;
    }
    iResult = WapTls_EncodeUint24( &stEncoder, &tMsgLen );
    if(iResult < 0)
    {
        WAP_FREE(pucBuffer);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch encoder error\n");)
        return -1;
    }
#ifdef WAP_CONFIG_SSL
    if( !(pstHs->ucFlags & M_WAP_TLS_FLAG_IS_SSL) )
#endif
    {
        iResult = TlsEcdr_EncodeUint16( &stEncoder, &(pstHs->usKeyExchPubValueLen) );
        if(iResult < 0)
        {
            WAP_FREE(pucBuffer);
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch encoder error\n");)
            return -1;
        }

    }
    iResult = TlsEcdr_EncodeOctets(&stEncoder, (WE_CHAR *)pstHs->pucKeyExchPubValue, \
                                                (WE_INT32)pstHs->usKeyExchPubValueLen);
    if(iResult < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch encoder error\n");)
        return -1;
    }
    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_KEY_EXCHANGE;

    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, \
                                    pstConn->pstHandshake, pucBuffer, usBufLen );

   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendClientKeyExch---send handshake message \n");)

   TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                            E_WAP_MSG_MODULEID_TLS, 
                            M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG, 
                            pstConn->iConnId, 
                            0, 
                            (WE_UINT16)usBufLen, 
                            0, 
                            (void *)pucBuffer, 
                            NULL);

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientKeyExch return 0\n");)
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcPrfResp
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    process the PRF function response from SEC
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    This function will free the response from sec.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcPrfResp
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_SecGetPrfResp    *pstPrfResp = NULL;
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp begin\n");)
    if( NULL == pResp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp pResp is null\n");)
        return -1;
    }
    pstPrfResp = (St_SecGetPrfResp *)pResp;
    if( NULL == pstTlsHandle || NULL == pstConn || NULL == pstConn->pstHandshake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp TlsHandle/Conn is null\n");)
        if( NULL != pstPrfResp->pucBuf )
        {
            WAP_FREE( pstPrfResp->pucBuf );
        }
        WAP_FREE( pstPrfResp );
        return -1;
    }
    
    if( pstPrfResp->usResult != G_SEC_OK || NULL == pstPrfResp->pucBuf )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp Prf response error\n");)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn, \
            G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
        if( NULL != pstPrfResp->pucBuf )
        {
            WAP_FREE( pstPrfResp->pucBuf );
        }
        WAP_FREE( pstPrfResp );
        return -1;
    }
    switch( pstConn->pstHandshake->ucState )
    {
        case M_WAP_TLS_HM_STATE_CREATE_KEYEXPANS:
            iResult = WapTlsHm_CreateCWriteKey( pstTlsHandle, pstConn, pstPrfResp );
            if( 0 == iResult )/*Not exportable*/
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp key exchange not exportable\n");)
                if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
                {
                    TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                                   E_WAP_MSG_MODULEID_TLS, 
                                   M_WAP_TLS_TLSHM_MSG_SEND_CCS, 
                                   pstConn->iConnId, 
                                   0, 
                                   0, 
                                   0, 
                                   NULL, 
                                   NULL);
                    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;
                }
                else/*It's simple handshake.*/
                {
                    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
                }
            }
            else if( 1 == iResult )/*The key exchange algorithm is exportable*/
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp Exportable\n");)
            }
            break;
        case M_WAP_TLS_HM_STATE_CREATE_CWKEY:
            iResult = WapTlsHm_CreateSWriteKey( pstTlsHandle, pstConn, pstPrfResp);
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp CreateSWriteKey error\n");)
            }
            break;
        case M_WAP_TLS_HM_STATE_CREATE_SWKEY:
            iResult = WapTlsHm_CreateIVBlock( pstTlsHandle, pstConn, pstPrfResp);
            if( 0 == iResult )/*stream cipher, no need create IV*/
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp CreateIVBlock return 0\n");)
                if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
                {
                    TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                                   E_WAP_MSG_MODULEID_TLS, 
                                   M_WAP_TLS_TLSHM_MSG_SEND_CCS, 
                                   pstConn->iConnId, 
                                   0, 
                                   0, 
                                   0, 
                                   NULL, 
                                   NULL);
                    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;

                }
                else
                {
                    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
                }
                break;
            }
            
            break;
        case M_WAP_TLS_HM_STATE_CREATE_IVBLOCK:
            iResult = WapTlsHm_CreateKeysFinal( pstTlsHandle, pstConn, pstPrfResp );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp CreateKeys_5 error\n");)
            }
            if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
            {
                TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                               E_WAP_MSG_MODULEID_TLS, 
                               M_WAP_TLS_TLSHM_MSG_SEND_CCS, 
                               pstConn->iConnId, 
                               0, 
                               0, 
                               0, 
                               NULL, 
                               NULL);
                pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;

            }
            else
            {
                pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
            }
            break;


        case M_WAP_TLS_HM_STATE_CREATE_CLIENT_FINISHED:
            WapTlsHm_ProcSendClientFinished( pstTlsHandle, pstConn, \
                                             pstPrfResp->pucBuf, (WE_UINT32)pstPrfResp->usBufLen );
            if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
            {
                pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
            }
            else
            {
                WapTlsHm_ProcHandShakeFinished( pstTlsHandle, pstConn );
                TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                               E_WAP_MSG_MODULEID_TLS, 
                               M_WAP_TLS_TLSHM_MSG_OPEND, 
                               pstConn->iConnId, 
                               0, 
                               0, 
                               0, 
                               NULL, 
                               NULL);
            }
            break;
        case M_WAP_TLS_HM_STATE_CHECK_SERVER_FINISHED:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp check server finished\n");)
            iResult = WapTlsHm_CheckPrfServerFinished( pstConn, pstPrfResp );
            if( 0 == iResult )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp error\n");)
                WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn, \
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE, \
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
                break;
            }
            if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp notify tls finished\n");)
                WapTlsHm_ProcHandShakeFinished( pstTlsHandle, pstConn );
                TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                               E_WAP_MSG_MODULEID_TLS, 
                               M_WAP_TLS_TLSHM_MSG_OPEND, 
                               pstConn->iConnId, 
                               0, 
                               0, 
                               0, 
                               NULL, 
                               NULL);
            }
            else
            {
                TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                               E_WAP_MSG_MODULEID_TLS, 
                               M_WAP_TLS_TLSHM_MSG_SEND_CCS, 
                               pstConn->iConnId, 
                               0, 
                               0, 
                               0, 
                               NULL, 
                               NULL);
                pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;
            }
            break;
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp default!!!\n");)
            break;
    }/*end switch*/
    if( NULL != pstPrfResp )
    {
        if( NULL != pstPrfResp->pucBuf )
        {
            WAP_FREE( pstPrfResp->pucBuf );
        }
        WAP_FREE( pstPrfResp );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcPrfResp end\n");)
    return 0;
}
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSendClientCert
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Notify Record to send ClientCert PDU
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    This funcation will free the parameter 'pResp'.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientCert
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
)
{
    St_WapTlsClientCert     stCCert = {0};
    St_WapEcdrEncoder       stEncoder = {0};
    St_SecTlsGetUserCertResp      *pstGetResp = NULL;
    WE_UCHAR    *pucBuf = NULL;
    WE_INT32    iBufLen = 0;
    WE_UINT8    ucMsgType = 0;
    M_WAP_TLS_UINT24    tMsgLen = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert begin\n");)

    if (NULL == pResp)
    {
        return -1;
    }
    pstGetResp = (St_SecTlsGetUserCertResp *)pResp;
  
    if( NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert parameter error\n");)
        if( NULL != pstGetResp->pucPubKeyHash )
        {
            WAP_FREE( pstGetResp->pucPubKeyHash );
        }
        if( NULL != pstGetResp->pstCert )
        {
            if( NULL != pstGetResp->pstCert->pucCert )
            {
                WAP_FREE(pstGetResp->pstCert->pucCert);
            }
            WAP_FREE( pstGetResp->pstCert);
        }
        WAP_FREE( pstGetResp );
        return -1;
    }

    
    if( NULL == pstConn || NULL == pstConn->pstHandshake 
        || ((G_SEC_OK != pstGetResp->usResult) && (pstGetResp->usResult != G_SEC_NOTFOUND_CERT)) )
    {
        if( NULL != pstGetResp->pucPubKeyHash )
        {
            WAP_FREE( pstGetResp->pucPubKeyHash );
        }
        if( NULL != pstGetResp->pstCert )
        {
            if( NULL != pstGetResp->pstCert->pucCert )
            {
               WAP_FREE(pstGetResp->pstCert->pucCert);
            }
            WAP_FREE( pstGetResp->pstCert);
        }
        WAP_FREE( pstGetResp );
        if( NULL != pstConn )
        {
            if( G_SEC_CANCEL == pstGetResp->usResult )
            {
                WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn, 
                                        G_WAP_ERR_TLS_CANCEL, 
                                        M_WAP_TLS_ALERT_DESC_USER_CANCELED);
            }
            else
            {
                WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn, 
                                        G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                        M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
            }
        }
        return -1;
    }
    if( G_SEC_NOTFOUND_CERT == pstGetResp->usResult )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert missing cert error\n");)
        pstConn->pstHandshake->pucPubKeyHash = NULL;
        stCCert.ucCertNum = 0;
        stCCert.pstCert = NULL;
    }
    else
    {
        pstConn->pstHandshake->pucPubKeyHash = pstGetResp->pucPubKeyHash;
        pstConn->pstHandshake->usPubKeyHashLen = pstGetResp->usPubKeyHashLen;
        stCCert.pstCert = pstGetResp->pstCert;
        stCCert.ucCertNum = (WE_UINT8)pstGetResp->ucNbrCerts ;
    }

    WAP_FREE( pstGetResp );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlssec:client cert num is %d\n"),stCCert.ucCertNum);)

#ifdef WAP_CONFIG_SSL
    if( (0 == stCCert.ucCertNum) && (pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_IS_SSL) )
    {
        TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                        E_WAP_MSG_MODULEID_TLS, 
                        M_WAP_TLS_TLSHM_MSG_SEND_ALERT,
                        pstConn->iConnId,
                        0,
                        M_WAP_SSL_ALERT_DESC_NO_CERTIFICATE,
                        0,
                        NULL,
                        NULL );
        return 0;
    }
#endif
    if( (stCCert.ucCertNum > 0) && ( 0 != stCCert.pstCert[0].uiCertLen ))
    {
        pstConn->pstHandshake->ucFlags |= M_WAP_TLS_FLAG_NEED_CERT_VERIFY;
    }

    WapTls_GetClientCertSize( &stEncoder, &stCCert );

    ucMsgType = M_WAP_TLS_HANDSHK_CERTIFICATE;
    tMsgLen = stEncoder.iPosition;
    iBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
    
    pucBuf = (WE_UCHAR *)WAP_MALLOC(iBufLen);
    if( NULL == pucBuf )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:WapTlsHm_ProcSendClientCert malloc error\n");)
        return -1;
    }
 
   
    WapTls_EncodeInit( &stEncoder, pucBuf, iBufLen );
    
    WapEcdr_EncodeUint8( &stEncoder, &ucMsgType );
    WapTls_EncodeUint24( &stEncoder, &tMsgLen );
    /*When not found cert,tMsgLen is 3 and the cert's length is 0*/
    WapTls_EncodeClientCert( &stEncoder, &stCCert );
    
    WapTls_FreeClientCert( &stCCert );

    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_CERTIFICATE;
    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, pstConn->pstHandshake, \
                                    stEncoder.pucData, stEncoder.iLength );
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendClientCert---send handshake message \n");)
    TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                    E_WAP_MSG_MODULEID_TLS, 
                    M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG,
                    pstConn->iConnId,
                    0,
                    stEncoder.iLength,
                    0,
                    stEncoder.pucData,
                    NULL 
                    );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:WapTlsHm_ProcSendClientCert end\n");)
    return 0;
}
#endif
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSendCertVerify
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Fill the clientCertVerify structure.And notify tls to send clientCertVerify.
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcSendCertVerify
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn   *pstConn,
    void            *pResp
)
{
    St_WapTlsCertVerify stCertV = {0};
    St_WapEcdrEncoder   stEncoder = {0};
    WE_UCHAR    *pucBuf = NULL;
    WE_UINT32   uiBufLen = 0;
    WE_UINT8    ucMsgType = 0;
    M_WAP_TLS_UINT24    tMsgLen = 0;
    St_SecCompSignResp  *pstSignResp = NULL;
    St_TlsRespData stResp = {0};
   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendCertVerify begin\n");)
    if (NULL == pResp)
    {
        return -1;
    }
    
    pstSignResp = (St_SecCompSignResp *)pResp;
    if( NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendCertVerify parameter error\n");)
        if( NULL != pstSignResp->pucSignature )
        {
            WAP_FREE(pstSignResp->pucSignature);
        }
        WAP_FREE(pstSignResp);
        return -1;
    }
   
    if( NULL == pstConn || NULL == pstConn->pstHandshake || G_SEC_OK != pstSignResp->usResult )
    {
        if( NULL != pstConn )
        {
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
            if( G_SEC_CANCEL != pstSignResp->usResult )
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_CANCEL, 
                    M_WAP_TLS_ALERT_DESC_USER_CANCELED );
            }
            else
#endif                  
            {             
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );  
            }
            if( NULL != pstSignResp->pucSignature )
            {
                WAP_FREE(pstSignResp->pucSignature);
            }
            WAP_FREE( pstSignResp );
            return -1;
        }
    }

    stCertV.pucSignature = pstSignResp->pucSignature;
    stCertV.usSiglen = pstSignResp->usSignatureLen;

    WAP_FREE( pstSignResp );

    ucMsgType = M_WAP_TLS_HANDSHK_CERTIFICATE_VERIFY;

    WapTls_GetCertVerifySize(&stEncoder, &stCertV );
    tMsgLen = (M_WAP_TLS_UINT24)stEncoder.iPosition;

    uiBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
    pucBuf = (WE_UCHAR *)WAP_MALLOC( uiBufLen );/*Free in WapTls_ProcSendMsg*/
    if( NULL == pucBuf )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendCertVerify malloc error\n");)
        stResp.iConnId = pstConn->iConnId;
        stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                               (WE_VOID*)&stResp,
                               pstTlsHandle->hPrivData);              
        return -1;
    }

    WapTls_EncodeInit( &stEncoder, pucBuf, uiBufLen );
    TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
    WapTls_EncodeUint24( &stEncoder, &tMsgLen );
    WapTls_EncodeCertVerify( &stEncoder, &stCertV );

    if( NULL != stCertV.pucSignature )
    {
        WAP_FREE( stCertV.pucSignature );
    }

    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_CERTIFICATE_VERIFY;    

    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, pstConn->pstHandshake, \
                                    stEncoder.pucData, stEncoder.iLength );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendCertVerify---send handshake message \n");)
    TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                        E_WAP_MSG_MODULEID_TLS, 
                        M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG, 
                        pstConn->iConnId, 
                        0, 
                        (WE_UINT32)stEncoder.iLength, 
                        0, 
                        (void *)stEncoder.pucData,
                        NULL);
    return 0;    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSendClientFinished
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Fill the clientFinished structure.And notify tls to send clientFinished.
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientFinished
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn   *pstConn,
    WE_UCHAR         *pucVerifyData,
    WE_UINT32       uiVerifyDataLen
)
{
    St_WapEcdrEncoder   stEncoder = {0};
    St_TlsRespData stResp = {0};
    M_WAP_TLS_UINT24    tMsgLen = 0;
    WE_UCHAR    *pucBuf = NULL;
    WE_UINT32   uiBufLen = 0;
    WE_UINT8    ucMsgType = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientFinished begin\n");)
    if( NULL == pstTlsHandle || NULL == pstConn || NULL == pucVerifyData)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ProcSendClientFinished parameter error\n");)
        return -1;
    }

    if(  (12 != uiVerifyDataLen)
#ifdef WAP_CONFIG_SSL
    && (36 != uiVerifyDataLen)
#endif
      )
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
            M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
        return -1;
    }

    ucMsgType = M_WAP_TLS_HANDSHK_FINISHED;
    tMsgLen = uiVerifyDataLen;
    uiBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
    pucBuf = (WE_UCHAR *)WAP_MALLOC( uiBufLen );
    if( NULL == pucBuf )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientFinished malloc error\n");)
        stResp.iConnId = pstConn->iConnId;
        stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                               (WE_VOID*)&stResp,
                               pstTlsHandle->hPrivData);              
        return -1;
    }

    WapTls_EncodeInit( &stEncoder, pucBuf, uiBufLen );
    TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
    WapTls_EncodeUint24( &stEncoder, &tMsgLen );
    WapTls_EncodeUcVector( &stEncoder, uiVerifyDataLen, &pucVerifyData );

    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_FINISHED;
    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, pstConn->pstHandshake, 
                                    stEncoder.pucData, stEncoder.iLength );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendClientFinished---send handshake message \n");)
    TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                   E_WAP_MSG_MODULEID_TLS, 
                   M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG, 
                   pstConn->iConnId, 
                   0, 
                   (WE_UINT32)stEncoder.iLength, 
                   0, 
                   (void *)stEncoder.pucData,
                   NULL);
    return 0; 
}


/*==================================================================================================
FUNCTION: 
    WapTlsHm_CheckCertVertifyRespProc
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Process the cert vertify's result.
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    uiResult     :  the result of cert vertify
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_CheckCertVertifyRespProc
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn   *pstConn,
    WE_UINT32       uiResult
)
{
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_TlsRespData stResp = {0};
#endif
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCertVertifyRespProc begin\n");)
    if( NULL == pstTlsHandle || NULL == pstConn || NULL == pstConn->pstHandshake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCertVertifyRespProc parameter error\n");)
        return -1;
    }
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    if (uiResult != G_SEC_OK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCertVertifyRespProc verifiy result is %d\n",uiResult);)
        stResp.iConnId = pstConn->iConnId;
    }
#endif
    switch( uiResult )
    {
        case G_SEC_OK:
            pstConn->pstHandshake->ucSecurityClass = 2;
            return 0;
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case G_SEC_UNKNOWN_CERTTYPE:
            if (pstConn->iOverride & G_WAP_ERR_TLS_UNSUPPORTED_CERT)
            {
                return 0;
            }
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM;
            pstConn->pstHandshake->uiAlert = G_WAP_ERR_TLS_UNSUPPORTED_CERT;
            pstConn->pstHandshake->uiAlertDesc= M_WAP_TLS_ALERT_DESC_UNSUPPORTED_CERTIFICATE;
            stResp.iErrCode = pstConn->pstHandshake->uiAlert;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                            (WE_VOID*)&stResp,
                                            pstTlsHandle->hPrivData); 
            break;
        case G_SEC_NOMATCH_ROOTCERT:
            if (pstConn->iOverride & G_WAP_ERR_TLS_UNKNOWN_CA)
            {
                return 0;
            }
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM;
            pstConn->pstHandshake->uiAlert = G_WAP_ERR_TLS_UNKNOWN_CA;
            pstConn->pstHandshake->uiAlertDesc= M_WAP_TLS_ALERT_DESC_UNKNOWN_CA;    
            stResp.iErrCode = pstConn->pstHandshake->uiAlert;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                            (WE_VOID*)&stResp,
                                            pstTlsHandle->hPrivData);             
            break;
        case G_SEC_INVALIDATE_CERT:/*lint fall through*/
        case G_SEC_INVALID_COMMON_NAME:
            if (pstConn->iOverride & G_WAP_ERR_TLS_BAD_CERTIFICATE)
            {
                return 0;
            }
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM;
            pstConn->pstHandshake->uiAlert = G_WAP_ERR_TLS_BAD_CERTIFICATE;
            pstConn->pstHandshake->uiAlertDesc= M_WAP_TLS_ALERT_DESC_BAD_CERTIFICATE; 
            stResp.iErrCode = pstConn->pstHandshake->uiAlert;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                            (WE_VOID*)&stResp,
                                            pstTlsHandle->hPrivData);             
            break;            
        case G_SEC_CERTEXPIRED:
            if (pstConn->iOverride & G_WAP_ERR_TLS_CERTIFICATE_EXPIRED)
            {
                return 0;
            }            
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM;
            pstConn->pstHandshake->uiAlert = G_WAP_ERR_TLS_CERTIFICATE_EXPIRED;
            pstConn->pstHandshake->uiAlertDesc= M_WAP_TLS_ALERT_DESC_CERTIFICATE_EXPIRED;  
            stResp.iErrCode = pstConn->pstHandshake->uiAlert;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                            (WE_VOID*)&stResp,
                                            pstTlsHandle->hPrivData);             
            break;              
#else
        case G_SEC_UNKNOWN_CERTTYPE:
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_UNSUPPORTED_CERT, 
                    M_WAP_TLS_ALERT_DESC_UNSUPPORTED_CERTIFICATE );
            break;
        case G_SEC_NOMATCH_ROOTCERT:
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_UNKNOWN_CA, 
                    M_WAP_TLS_ALERT_DESC_UNKNOWN_CA);
            break;
        case G_SEC_INVALIDATE_CERT:/*lint fall through*/
        case G_SEC_INVALID_COMMON_NAME:
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_BAD_CERTIFICATE, 
                    M_WAP_TLS_ALERT_DESC_BAD_CERTIFICATE);
            break;
        case G_SEC_CERTEXPIRED:
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_CERTIFICATE_EXPIRED, 
                    M_WAP_TLS_ALERT_DESC_CERTIFICATE_EXPIRED );
            break;
        case G_SEC_CANCEL:
            WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn, G_WAP_ERR_TLS_CANCEL, 
                    M_WAP_TLS_ALERT_DESC_USER_CANCELED );
            break;
#endif            
        default:
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE, 
                    M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE);
    }
    return -1;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ClientKeyExchRespProc
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Process the key exchange response from sec.
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    pResp   :   the response
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/

WE_INT32 WapTlsHm_ClientKeyExchRespProc
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,    
    void *pResp
)
{  
    St_SecKeyExchResp *pstKeyExchResp = NULL;
 #ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_SecDisableSessionPara stDisableSess = {0};
    St_SecSessionOpt stSessionOpt = {G_SEC_ENABLE_SESSION,NULL};
#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ClientKeyExchRespProc begin\n");)
    if( NULL == pResp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:ClientKeyExchRespProc parameter error\n");)
        return -1;
    }    
    pstKeyExchResp = (St_SecKeyExchResp *)pResp;
    if (NULL == pstTlsHandle)
    {
        if( NULL != pstKeyExchResp->pucPreMsKey )
        {
            WAP_FREE(pstKeyExchResp->pucPreMsKey);
        }
        WAP_FREE(pstKeyExchResp);
        return -1;
    }
    if((NULL == pstConn) || (NULL == pstConn->pstHandshake) || (G_SEC_OK != pstKeyExchResp->usResult))
    {
        if( NULL != pstConn )
        {
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
            if( G_SEC_CANCEL == pstKeyExchResp->usResult )
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_CANCEL, 
                    M_WAP_TLS_ALERT_DESC_USER_CANCELED );
            }
            else
#endif                
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
            }
        }
        if( NULL != pstKeyExchResp->pucPreMsKey )
        {
            WAP_FREE(pstKeyExchResp->pucPreMsKey);
        }
        WAP_FREE(pstKeyExchResp);
        return -1;
    }

    pstConn->pstPendingRead->iMasterSecretId = pstKeyExchResp->ucMasterSecretId;
    pstConn->pstPendingWrite->iMasterSecretId = pstKeyExchResp->ucMasterSecretId;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("ClientKeyExchRespProc master secret id is %d\n"),pstKeyExchResp->ucMasterSecretId);)
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    stDisableSess.iMasterSecretId = pstKeyExchResp->ucMasterSecretId;
    stSessionOpt.eType = G_SEC_DISABLE_SESSION;
    stSessionOpt.hValue = &(stDisableSess);
    ISecW_SessionOpts(pstTlsHandle->hSecHandle,&stSessionOpt);
#else  
    ISecW_DisableSession( pstTlsHandle->hSecHandle, pstKeyExchResp->ucMasterSecretId );
#endif
    pstConn->pstHandshake->usKeyExchPubValueLen = pstKeyExchResp->usPreMsKeyLen;
    pstConn->pstHandshake->pucKeyExchPubValue = pstKeyExchResp->pucPreMsKey;
    WAP_FREE( pstKeyExchResp );
    return 0;
}
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
/*==================================================================================================
FUNCTION: 
    WapTlsHm_NotifyAppSelect
CREATE DATE: 
    2007-01-10
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    Notify APP to select when encouter trust error or need client auth
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTlsHm_NotifyAppSelect
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,    
    void *pResp
)
{
    St_ShowDlg *pstShowDlg = NULL;

    if (NULL == pResp)
    {
        return -1;
    }
    pstShowDlg = (St_ShowDlg*)pResp;
    if ( (NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( 
                          "WapTlsHm_NotifyAppSelect ---error--- para is NULL   \n");)
        WAP_FREE( pstShowDlg->pvBuf);
        WAP_FREE(pstShowDlg);
        return -1 ;
    }
    TlsMsg_PostMsg( pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_CM,
                    G_WAP_CM_MSG_NEED_SELECT_SEC,
                    pstConn->iCidx,
                    0,pstShowDlg->uiLength,
                    0,pstShowDlg->pvBuf,NULL );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_NotifyAppSelect end! \n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ContiuneNego
CREATE DATE: 
    2007-01-10
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    After APP select, continue handshake
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    uiBufferLen: buffer length
    pucBuffer: buffer
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
void  WapTlsHm_ContiuneNego
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn,  
    WE_UINT8        *pucBuffer,
    WE_UINT32        uiBufferLength
)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ContiuneNego beginning \n");)
    if ( (NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ContiuneNego ---error--- para is NULL\n" );)
        WAP_FREE( pucBuffer );
        return ;
    }
    ISecW_EvtShowDlgAction( pstTlsHandle->hSecHandle,pstConn->iObjectId,pucBuffer,uiBufferLength);
    WAP_FREE( pucBuffer );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ContiuneNego end! \n" );)
}
#endif

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcGetAppCertResp
CREATE DATE: 
    2007-06-29
AUTHOR: 
    Wang Shuhua
DESCRIPTION:
    APP give user cert and private key
ARGUMENTS PASSED:    
    pstTlsHandle : TLS handle
    pstConn:   connection
    uiCertOpt: identify cert or cert list
    uiCertLen: cert/cert list length
    pvCert:  cert content
    pvPrivaKey: private key
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
WE_INT32 WapTlsHm_ProcGetAppCertResp
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    WE_UINT32 uiCertOpt,
    WE_UINT32 uiCertLen,
    void *pvCert,    
    void *pvPrivaKey
)
{
    St_WapEcdrEncoder       stEncoder = {0};
    WE_UCHAR    *pucBuf = NULL;
    WE_INT32    iBufLen = 0;
    WE_UINT8    ucMsgType = 0;
    M_WAP_TLS_UINT24    tMsgLen = 0;
    M_WAP_TLS_UINT24    tCertLen = 0;
    WE_UINT16 usPrivKeyLen = 0;
    WE_UINT16 *pusPrivKeyLen = &usPrivKeyLen;
    WE_UCHAR *pucPrivVal = NULL;
    St_TlsRespData  stResp = {0};    

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert begin\n");)
    if( NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert parameter error\n");)
        if( NULL != pvCert )
        {
            WAP_FREE( pvCert );
        }
        if( NULL != pvPrivaKey)
        {
            WAP_FREE( pvPrivaKey);
        }
        return -1;
    }

    if( pvCert == NULL && 0 == uiCertLen)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:ProcSendClientCert missing cert error\n");)
        pstConn->pstHandshake->pucPubKeyHash = NULL;
    }
    else
    {
        if (pvPrivaKey != NULL)
        {
            pucPrivVal = (WE_UINT8*)pvPrivaKey;
            *((WE_UINT8 *)pusPrivKeyLen) = pucPrivVal[0];
            *(((WE_UINT8 *)pusPrivKeyLen)+1) = pucPrivVal[1];
        }
        pstConn->pstHandshake->usPubKeyHashLen = *pusPrivKeyLen;
        pstConn->pstHandshake->pucPubKeyHash = (WE_UCHAR*)WAP_MALLOC(*pusPrivKeyLen);
        if (NULL == pstConn->pstHandshake->pucPubKeyHash)
        {
            stResp.iConnId = pstConn->iConnId;
            stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                   (WE_VOID*)&stResp,
                                   pstTlsHandle->hPrivData);              
            return -1;
        }
        WAP_MEMCPY(pstConn->pstHandshake->pucPubKeyHash,(WE_UCHAR*)pvPrivaKey + 2,*pusPrivKeyLen);
        WAP_FREE(pvPrivaKey);
    }

#ifdef WAP_CONFIG_SSL
    if( (NULL == pvCert) && (pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_IS_SSL) )
    {
        TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                        E_WAP_MSG_MODULEID_TLS, 
                        M_WAP_TLS_TLSHM_MSG_SEND_ALERT,
                        pstConn->iConnId,
                        0,
                        M_WAP_SSL_ALERT_DESC_NO_CERTIFICATE,
                        0,
                        NULL,
                        NULL );
        return 0;
    }
#endif
    if (pvCert != NULL)
    {
        pstConn->pstHandshake->ucFlags |= M_WAP_TLS_FLAG_NEED_CERT_VERIFY;
    }
    ucMsgType = M_WAP_TLS_HANDSHK_CERTIFICATE;

    if (uiCertOpt == E_TLS_OPT_USER_CERT)
    {
        if (uiCertLen ==  0)
        {
            tMsgLen = 3;
            iBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
            
            pucBuf = (WE_UCHAR *)WAP_MALLOC(iBufLen);
            if (NULL == pucBuf)
            {
                stResp.iConnId = pstConn->iConnId;
                stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
                ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                       (WE_VOID*)&stResp,
                                       pstTlsHandle->hPrivData);  
                return -1;
            }            
            WapTls_EncodeInit( &stEncoder, pucBuf, iBufLen );
            
            TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
            WapTls_EncodeUint24( &stEncoder, &tMsgLen );
            WapTls_EncodeUint24( &stEncoder, &(uiCertLen));             
        }
        else
        {
            tMsgLen = (M_WAP_TLS_UINT24)uiCertLen + 2*3;
            tCertLen = (M_WAP_TLS_UINT24)uiCertLen + 3;
            iBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
            pucBuf = (WE_UCHAR *)WAP_MALLOC(iBufLen);
            if (NULL == pucBuf)
            {
                stResp.iConnId = pstConn->iConnId;
                stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
                ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                       (WE_VOID*)&stResp,
                                       pstTlsHandle->hPrivData);  
                return -1;
            }
            WapTls_EncodeInit( &stEncoder, pucBuf, iBufLen );
            
            TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
            WapTls_EncodeUint24( &stEncoder, &tMsgLen );
            WapTls_EncodeUint24( &stEncoder, &tCertLen);
            WapTls_EncodeUint24( &stEncoder, &(uiCertLen));
            WapTls_EncodeUcVector(&stEncoder,uiCertLen,(WE_UCHAR**)&(pvCert));         
        }
    }
    else
    {
         tMsgLen = (M_WAP_TLS_UINT24)uiCertLen;
        iBufLen = tMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
        pucBuf = (WE_UCHAR *)WAP_MALLOC(iBufLen);
        if( NULL == pucBuf )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:WapTlsHm_ProcSendClientCert malloc error\n");)
            stResp.iConnId = pstConn->iConnId;
            stResp.iErrCode = G_WAP_ERR_TLS_MALLOC_FAIL;
            ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                   (WE_VOID*)&stResp,
                                   pstTlsHandle->hPrivData);  

            return -1;
        }
        WapTls_EncodeInit( &stEncoder, pucBuf, iBufLen );
        
        TlsEcdr_EncodeUint8( &stEncoder, &ucMsgType );
        WapTls_EncodeUint24( &stEncoder, &tMsgLen );
        WapTls_EncodeUcVector(&stEncoder,uiCertLen,(WE_UCHAR**)&(pvCert)); 
    }
    if (pvCert != NULL)
    {
        WAP_FREE(pvCert);
        pvCert = NULL;
    }
    pstConn->pstHandshake->uiMsgs |= M_BIT_CLIENT_CERTIFICATE;
    WapTlsHm_UpdateHandShakeHashes(pstTlsHandle, pstConn, pstConn->pstHandshake, \
                                    stEncoder.pucData, stEncoder.iLength);

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("SendClientCert---send handshake message \n");)
    TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                    E_WAP_MSG_MODULEID_TLS, 
                    M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG,
                    pstConn->iConnId,
                    0,
                    stEncoder.iLength,
                    0,
                    stEncoder.pucData,
                    NULL 
                    );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlssec:WapTlsHm_ProcSendClientCert end\n");)
    return 0;
}
#endif
#endif






















