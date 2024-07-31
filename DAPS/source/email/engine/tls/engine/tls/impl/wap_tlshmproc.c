/*==================================================================================================
    FILE NAME   : wap_tlshmproc.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
          Tls handshake layer main process function   
       
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


#include "wap_tlsdef.h"
#include "wap_tlsst.h"
#include "wap_tlspub.h"
#include "wap_tlshmcore.h" 
#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h" 
#endif

#include "itls.h"




/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static St_WapTlsHandshake* WapTlsHm_NewHandShake(void);
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
static WE_INT32 WapTlsHm_StoreSvrCert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UCHAR *pucDomain,
    WE_UCHAR *pucCert,
    WE_UINT32 uiCertLen
);
#endif

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcStartHandShake
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Start preapair handshake
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id 
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
WE_INT32 WapTlsHm_ProcStartHandShake
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    St_WapTlsHandshake *pstHandShake = NULL;
    WE_UINT32          uiUnixTime = 0;
    WE_INT32           iRet = 0;
    WE_UCHAR           aucAddr [18] = {0};
    St_SecPeerOpt stPeerOpt = {G_SEC_REMOVE_PEER,NULL};
    St_SecSearchPeerPara stSearchPeer = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake begin \n");)            

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake TLS Handle null \n");)
        return -1;
    }
     pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake can't get connection \n");)
        return -1;
    }

    pstConn->pstHandshake =pstHandShake = WapTlsHm_NewHandShake();
    if ( NULL == pstHandShake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake creat handshake error\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hClientMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hClientShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hServerMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hServerShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    } 
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hVerifyMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hVerifyShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }    

    pstConn->pstPendingRead = (St_WapTlsConnState*)WAP_MALLOC(sizeof(St_WapTlsConnState));
    if (NULL == pstConn->pstPendingRead)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    pstConn->pstPendingWrite = (St_WapTlsConnState*)WAP_MALLOC(sizeof(St_WapTlsConnState));
    if (NULL == pstConn->pstPendingWrite)
    {
        WAP_FREE(pstConn->pstPendingRead);
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    WapTls_ConnStateInit( pstConn->pstPendingRead );
    WapTls_ConnStateInit( pstConn->pstPendingWrite );    

    uiUnixTime = TLS_GET_CURRENT_TIME();

    pstHandShake->taucClientRandom[0] = 0;
    pstHandShake->taucClientRandom[1] = (WE_UCHAR)((uiUnixTime >> 16)&0xff);
    pstHandShake->taucClientRandom[2] = (WE_UCHAR)((uiUnixTime >> 8)&0xff);
    pstHandShake->taucClientRandom[3] = (WE_UCHAR)(uiUnixTime &0xff);

    iRet = We_LibGenerateRandom(  
                                  pstTlsHandle->hLibAlgHandle,
                                  (WE_CHAR*) &(pstHandShake->taucClientRandom[4]), 
                                  28 );
    if (iRet != G_SEC_OK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake generate client random error\n");)
        WAP_FREE(pstConn->pstPendingRead);
        WAP_FREE(pstConn->pstPendingWrite);
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,
                                  G_WAP_ERR_TLS_INTERNAL_ERROR,
                                  M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;        
    }
  
    stSearchPeer.iAddressLen = pstConn->iAddrLen;        
    stSearchPeer.pucAddress = pstConn->pucAddr;
    stSearchPeer.usPortNum = pstConn->usPort;
    stSearchPeer.iTargetID = pstConn->iObjectId;
    stPeerOpt.eType = G_SEC_SEARCH_PEER;
    stPeerOpt.hValue = &(stSearchPeer);
    
    pstHandShake->ucState = M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP;
#ifdef WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION
    if (pstTlsHandle->ucVerMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
    {  
        ISecW_PeerOpts(pstTlsHandle->hSecHandle,&stPeerOpt, G_SEC_SSL_CONNECTION_TYPE);
        return 0;
    }
#endif    
    ISecW_PeerOpts(pstTlsHandle->hSecHandle,&stPeerOpt, G_SEC_TLS_CONNECTION_TYPE);
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake end\n");)
    return 0;
}
#else
WE_INT32 WapTlsHm_ProcStartHandShake
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    St_WapTlsHandshake *pstHandShake = NULL;
    WE_UINT32          uiUnixTime = 0;
    WE_INT32           iRet = 0;
    WE_UCHAR           aucAddr [18] = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake begin \n");)            

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake TLS Handle null \n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake can't get connection \n");)
        return -1;
    }
    pstConn->pstHandshake =pstHandShake = WapTlsHm_NewHandShake();
    if ( NULL == pstHandShake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake creat handshake error\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hClientMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hClientShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hServerMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hServerShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    } 
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_MD5,&(pstHandShake->hVerifyMd5Handle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashInit(pstTlsHandle, E_WE_ALG_HASH_SHA1,&(pstHandShake->hVerifyShaHandle));
    if ( -1 == iRet)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }    

    pstConn->pstPendingRead = (St_WapTlsConnState*)WAP_MALLOC(sizeof(St_WapTlsConnState));
    if (NULL == pstConn->pstPendingRead)
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    pstConn->pstPendingWrite = (St_WapTlsConnState*)WAP_MALLOC(sizeof(St_WapTlsConnState));
    if (NULL == pstConn->pstPendingWrite)
    {
        WAP_FREE(pstConn->pstPendingRead);
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_INTERNAL_ERROR, M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    WapTls_ConnStateInit( pstConn->pstPendingRead );
    WapTls_ConnStateInit( pstConn->pstPendingWrite );    

    uiUnixTime = TLS_GET_CURRENT_TIME();

    pstHandShake->taucClientRandom[0] = 0;
    pstHandShake->taucClientRandom[1] = (WE_UCHAR)((uiUnixTime >> 16)&0xff);
    pstHandShake->taucClientRandom[2] = (WE_UCHAR)((uiUnixTime >> 8)&0xff);
    pstHandShake->taucClientRandom[3] = (WE_UCHAR)(uiUnixTime &0xff);

    iRet = We_LibGenerateRandom(  
                                  pstTlsHandle->hLibAlgHandle,
                                  (WE_CHAR*) &(pstHandShake->taucClientRandom[4]), 
                                  28 );
    if (iRet != G_SEC_OK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake generate client random error\n");)
        WAP_FREE(pstConn->pstPendingRead);
        WAP_FREE(pstConn->pstPendingWrite);
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,
                                  G_WAP_ERR_TLS_INTERNAL_ERROR,
                                  M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;        
    }

   
    pstHandShake->ucState = M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP;   
    
    ISecW_SearchPeer( pstTlsHandle->hSecHandle, 
                      (WE_UINT16)pstConn->iObjectId,                          
                      pstConn->pucAddr, 
                      pstConn->iAddrLen,                          
                      pstConn->usPort,
                      G_SEC_TLS_CONNECTION_TYPE);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake end\n");)
    return 0;
}
#endif
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcHandShakeFinished
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
   Receive server finished When full handshake, Send client finished when simple handshake, call 
   this function 
ARGUMENTS PASSED:
    pstTlsHandle : TLS module handle
    pstConn : connection
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:   
==================================================================================================*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE    
WE_INT32 WapTlsHm_ProcHandShakeFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{       
    WE_UCHAR  aucAddr[18] = {0};
    WE_UINT8  ucSessionOptions = 0;
    WE_UINT8   aucCipherSuite[2] = {0};
    St_WapTlsHandshake  *pstHSHK = NULL;
    WE_UINT8   ucConnType = '\0';
    St_SecPeerOpt stPeerOpt= {G_SEC_REMOVE_PEER,NULL};
    St_SecSessionOpt stSessionOpt = {G_SEC_ENABLE_SESSION,NULL}; 
    St_SecRemovePeerPara   stRemovePeer = { 0 };
    St_SecAttachPeerPara   stAttchPeer  = { 0 };
    St_SecSessionRenewPara  stSessionRenew = { 0 };
    St_SecEnableSessionPara stEnableSeesion = { 0 };

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished begin\n");)

    if (( NULL == pstTlsHandle)|| (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished parameter error\n");)        
        return -1;
    }
    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished parameter error\n");)        
        return -1;
    }
    ucConnType = (pstHSHK->ucFlags & M_WAP_TLS_FLAG_IS_SSL) ? G_SEC_SSL_CONNECTION_TYPE : G_SEC_TLS_CONNECTION_TYPE;

    if (pstHSHK->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE)
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished full handshake\n");)
         stRemovePeer.iMasterSecretId = pstConn->stWrite.iMasterSecretId;
         stPeerOpt.eType = G_SEC_REMOVE_PEER;
         stPeerOpt.hValue = &stRemovePeer;
         ISecW_PeerOpts(pstTlsHandle->hSecHandle, &stPeerOpt, ucConnType);

         if (pstHSHK->stSessionId.ucLength > 0)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished resumeable\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_RESUMABLE;
         }

         if (pstHSHK->uiMsgs & M_BIT_SERVER_CERTIFICATE)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished server auth\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_SERVER_AUTH;            
         }

         if (pstHSHK->uiMsgs & M_BIT_CLIENT_CERTIFICATE_VERIFY)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished server auth\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_CLIENT_AUTH;            
         }    

         aucCipherSuite[0] = (WE_UINT8)((pstHSHK->usCipherSuite >> 8)& 0xff);
         aucCipherSuite[1] = (WE_UINT8)(pstHSHK->usCipherSuite & 0xff);

         stSessionRenew.iMasterSecretId = pstConn->stWrite.iMasterSecretId;
         stSessionRenew.pucCipherSuite = aucCipherSuite;
         stSessionRenew.pucPrivateKeyId = ( WE_UCHAR*)"\x00\x00\x00\x00";
         stSessionRenew.pucSessionId = pstHSHK->stSessionId.aucId;
         stSessionRenew.ucCompressionAlg = pstConn->stWrite.ucCompressionAlg;
         stSessionRenew.ucSessionIdLen = (WE_UINT8)pstHSHK->stSessionId.ucLength;
         stSessionRenew.ucSessionOptions = ucSessionOptions;
         stSessionRenew.uiCreationTime = TLS_GET_CURRENT_TIME();
         stSessionOpt.eType = G_SEC_RENEW_SESSION;
         stSessionOpt.hValue = &(stSessionRenew);
         ISecW_SessionOpts(pstTlsHandle->hSecHandle, &stSessionOpt);    
    }

    stAttchPeer.iAddressLen = pstConn->iAddrLen;
    stAttchPeer.pucAddress = pstConn->pucAddr;
    stAttchPeer.usPortnum = pstConn->usPort;

    stAttchPeer.iMasterSecretId = pstConn->stWrite.iMasterSecretId;

    stPeerOpt.eType = G_SEC_ATTACH_PEER;
    stPeerOpt.hValue = &(stAttchPeer);
    ISecW_PeerOpts(pstTlsHandle->hSecHandle, &stPeerOpt,ucConnType);
   
    stEnableSeesion.iMasterSecretId = pstConn->stWrite.iMasterSecretId;
    stEnableSeesion.ucIsActive = 1;
    stSessionOpt.eType = G_SEC_ENABLE_SESSION;
    stSessionOpt.hValue = &(stEnableSeesion);
    ISecW_SessionOpts(pstTlsHandle->hSecHandle, &stSessionOpt);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake end\n");)
   return 0;  
}
#else 
WE_INT32 WapTlsHm_ProcHandShakeFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_UCHAR  aucAddr[18] = {0};
    WE_UINT8  ucSessionOptions = 0;
    WE_UINT8   aucCipherSuite[2] = {0};
    St_WapTlsHandshake  *pstHSHK = NULL;
    WE_UINT8   ucConnType = '\0';

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished begin\n");)

    if (( NULL == pstTlsHandle)|| (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished parameter error\n");)        
        return -1;
    }
    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished parameter error\n");)        
        return -1;
    }

    if (pstHSHK->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE)
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished full handshake\n");)        
         ( void )ISecW_RemovePeer( pstTlsHandle->hSecHandle, pstConn->stWrite.iMasterSecretId );

         if (pstHSHK->stSessionId.ucLength > 0)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished resumeable\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_RESUMABLE;
         }

         if (pstHSHK->uiMsgs & M_BIT_SERVER_CERTIFICATE)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished server auth\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_SERVER_AUTH;            
         }

         if (pstHSHK->uiMsgs & M_BIT_CLIENT_CERTIFICATE_VERIFY)
         {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeFinished server auth\n");)   
            ucSessionOptions |= G_SEC_SESSION_OPTIONS_CLIENT_AUTH;            
         }    

         aucCipherSuite[0] = (WE_UINT8)((pstHSHK->usCipherSuite >> 8)& 0xff);
         aucCipherSuite[1] = (WE_UINT8)(pstHSHK->usCipherSuite & 0xff);
         /*update session*/
         ISecW_SessionRenew( pstTlsHandle->hSecHandle, 
                            pstConn->stWrite.iMasterSecretId,
                            ucSessionOptions,
                            pstHSHK->stSessionId.aucId,
                            (WE_UINT8)pstHSHK->stSessionId.ucLength,
                            aucCipherSuite,
                            pstConn->stWrite.ucCompressionAlg,
                            ( WE_UCHAR*)"\x00\x00\x00\x00",
                            TLS_GET_CURRENT_TIME() );
      
    }
        
    ucConnType = (pstHSHK->ucFlags & M_WAP_TLS_FLAG_IS_SSL) ? G_SEC_SSL_CONNECTION_TYPE : G_SEC_TLS_CONNECTION_TYPE;
    ISecW_AttachPeerToSession( pstTlsHandle->hSecHandle,                 
                                    pstConn->pucAddr, 
                                    pstConn->iAddrLen,                
                                    pstConn->usPort,
                                    pstConn->stWrite.iMasterSecretId,
                                    ucConnType);

   ( void )ISecW_EnableSession( pstTlsHandle->hSecHandle, pstConn->stWrite.iMasterSecretId, 1 );
   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcStartHandShake end\n");)
   return 0;   
}
#endif

/*==================================================================================================
FUNCTION: 
    WapTlsHm_NewHandShake
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Create a new handshake
ARGUMENTS PASSED:
    None
RETURN VALUE:
    NULL: Malloc error
    or pointer of St_WapTlsHandShake
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:   
==================================================================================================*/
static St_WapTlsHandshake* WapTlsHm_NewHandShake(void)
{
    St_WapTlsHandshake *pstHSHK = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_NewHandShake begin \n");)
    pstHSHK = (St_WapTlsHandshake*)WAP_MALLOC(sizeof(St_WapTlsHandshake));
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_NewHandShake return NULL \n");)        
        return NULL;
    }
    pstHSHK->ucFlags = 0;
    pstHSHK->stSessionId.ucLength = 0;
    pstHSHK->pucPubKeyHash = NULL;
    pstHSHK->uiMsgs = 0;
    pstHSHK->pucCipherSuites = NULL;
    pstHSHK->usCipherSuitesLen = 0;
    pstHSHK->pucPubKeyHash = NULL;
    pstHSHK->usPubKeyHashLen = 0;
    pstHSHK->pucKeyExchPubValue = NULL;
    pstHSHK->usKeyExchPubValueLen = 0;
    pstHSHK->iSecurityId = -1;
    pstHSHK->ucSecurityClass = 1;
    pstHSHK->stServerCert.ucCertNum = 0;
    pstHSHK->stServerCert.pstCert = NULL;
    pstHSHK->stCertReq.ucCertTypeNum = 0;
    pstHSHK->stCertReq.pucCertType = NULL;
    pstHSHK->stCertReq.ucCertAuthNum = 0;    
    /*wang shuhua add at 20070111*/
    WAP_MEMSET(&(pstHSHK->stServerKeyExch),0,sizeof(St_WapTlsServerKeyExch));
    
    pstHSHK->stCertReq.pstDN = NULL;
    pstHSHK->hClientMd5Handle = NULL;
    pstHSHK->hClientShaHandle = NULL;
    pstHSHK->hServerMd5Handle = NULL;
    pstHSHK->hServerShaHandle = NULL; 
    pstHSHK->hVerifyMd5Handle = NULL;
    pstHSHK->hVerifyShaHandle = NULL;    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_NewHandShake end \n");)
    return pstHSHK;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ConfigurePendingWrite
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    make pending write state to current write state
ARGUMENTS PASSED:
    pstTlsHandle: Tls module handle
    pstConn:TLS connection
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:   
==================================================================================================*/
void  WapTlsHm_ConfigurePendingWrite
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingWrite begin \n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingWrite parameter error \n");)        
        return;
    }

    if (pstConn->pstPendingWrite != NULL)
    {
        if (pstConn->stWrite.hCipherHandle != NULL)
        {
            iRet = WapTls_EncryptStreamEnd(pstTlsHandle,pstConn->stWrite.hCipherHandle);
            if (iRet < 0)
            {
                WapTlsHm_ProcHandShakeErr(
                                            pstTlsHandle,
                                            pstConn,
                                            G_WAP_ERR_TLS_INTERNAL_ERROR,
                                            M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR
                                         );
            }
        }
        pstConn->stWrite= *(pstConn->pstPendingWrite);
        WAP_FREE(pstConn->pstPendingWrite);
        pstConn->pstPendingWrite = NULL;    
    }
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ConfigurePendingRead
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    make pending read state to current read state
ARGUMENTS PASSED:
    pstTlsHandle: Tls module handle
    pstConn:TLS connection
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:   
==================================================================================================*/
void  WapTlsHm_ConfigurePendingRead
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingRead begin \n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingRead parameter error \n");)        
        return;
    }

    if (pstConn->pstPendingRead != NULL)
    {
        if (pstConn->stRead.hCipherHandle != NULL)
        {
            iRet = WapTls_DecryptStreamEnd(pstTlsHandle,pstConn->stRead.hCipherHandle);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingRead decryptstreamend error \n");)
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,
                                          G_WAP_ERR_TLS_INTERNAL_ERROR,
                                          M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            }
        }
        pstConn->stRead= *(pstConn->pstPendingRead);
        WAP_FREE(pstConn->pstPendingRead);
        pstConn->pstPendingRead = NULL;    
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ConfigurePendingRead end \n");)
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_UpdateHandShakeHashes
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Update all of the hash data from all handshake messages.
ARGUMENTS PASSED:
    pstTlsHandle : TLS module handle
    hTlsConn : connection
    hTlsHandShake: handshake structure pointer
    pucBuf: buffer
    iLen: data length
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:   
==================================================================================================*/
void WapTlsHm_UpdateHandShakeHashes
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstTlsConn,
    St_WapTlsHandshake *pstHandShake,
    WE_UCHAR *pucBuf,
    WE_INT32 iLen
)
{
    WE_INT32 iTmp = -1;

    if ((NULL == pstTlsConn)||(NULL == pstHandShake))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_UpdateHandShakeHashes begin \n");)
        return;
    }
    if (pstHandShake->hClientMd5Handle != NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_UpdateHandShakeHashes Client MD5 handle not NULL \n");)
        iTmp = WapTls_HashUpdate(pstTlsHandle,
                                 pstHandShake->hClientMd5Handle,
                                 pucBuf,
                                 iLen
                                 );
        if (iTmp == -1)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return;
        }

        iTmp = WapTls_HashUpdate(pstTlsHandle,
                                 pstHandShake->hClientShaHandle,
                                 pucBuf,
                                 iLen
                                 );
        if (iTmp == -1)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return;
        }
    }
    if (pstHandShake->ucState != M_WAP_TLS_HM_SSL_WAIT_FOR_PROC_SERVER_FINISHED)
    {
        if (pstHandShake->hServerMd5Handle != NULL)
        {
             WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_UpdateHandShakeHashes Server MD5 handle not NULL \n");)
            iTmp = WapTls_HashUpdate(pstTlsHandle,
                                     pstHandShake->hServerMd5Handle,
                                     pucBuf,
                                     iLen
                                     );
            if (iTmp == -1)
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
                return;
            }

            iTmp = WapTls_HashUpdate(pstTlsHandle,
                                     pstHandShake->hServerShaHandle,
                                     pucBuf,
                                     iLen
                                     );
            if (iTmp == -1)
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
                return;
            }        
        }
    }
    if (pstHandShake->hVerifyMd5Handle != NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_UpdateHandShakeHashes verify handle not NULL \n");)
        iTmp = WapTls_HashUpdate(pstTlsHandle,
                                 pstHandShake->hVerifyMd5Handle,
                                 pucBuf,
                                 iLen
                                 );
        if (iTmp == -1)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return;
        }

        iTmp = WapTls_HashUpdate(pstTlsHandle,
                                 pstHandShake->hVerifyShaHandle,
                                 pucBuf,
                                 iLen
                                 );
        if (iTmp == -1)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstTlsConn,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return;
        }        
    }
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_FreeHandShake
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Free the curret handshake 
ARGUMENTS PASSED:
    hTlsHandle : TLS module handle
    pstTlsHandShake: handshake structure pointer
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/
void WapTlsHm_FreeHandShake( WE_HANDLE hTlsHandle,St_WapTlsHandshake*pstTlsHandShake)
{
    WE_UCHAR aucTmp[20] = {0};
    WE_INT32 iTmp = 0;

    if (NULL == pstTlsHandShake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_FreeHandShake handshake is null\n");)
        return;
    }

    if (pstTlsHandShake->pucPubKeyHash != NULL)
    {
        WAP_FREE(pstTlsHandShake->pucPubKeyHash);
    }
    if (pstTlsHandShake->pucCipherSuites != NULL)
    {
        WAP_FREE(pstTlsHandShake->pucCipherSuites);
    }
    if (pstTlsHandShake->pucKeyExchPubValue)
    {
        WAP_FREE(pstTlsHandShake->pucKeyExchPubValue);
    }
    
    WapTls_FreeServerCert(&(pstTlsHandShake->stServerCert));
    WapTls_FreeCertReq(&(pstTlsHandShake->stCertReq));
    /*Wang Shuhua add at 20070111*/
    WapTls_FreeServerKeyExch(pstTlsHandShake->ucKeyExcMethod,&(pstTlsHandShake->stServerKeyExch));
    
    if (pstTlsHandShake->hClientMd5Handle != NULL)
    {
        iTmp = WapTls_HashEnd(
                              hTlsHandle,
                              pstTlsHandShake->hClientMd5Handle,
                              aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash client md5 error\n");)
        }
    }
    if (pstTlsHandShake->hClientShaHandle != NULL)
    {
         iTmp = WapTls_HashEnd(
                               hTlsHandle,
                               pstTlsHandShake->hClientShaHandle,
                               aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash client SHA error\n");)
        }
    }
    if (pstTlsHandShake->hServerMd5Handle!= NULL)
    {
         iTmp = WapTls_HashEnd(
                               hTlsHandle,
                               pstTlsHandShake->hServerMd5Handle,
                               aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash Sever MD5 error\n");)
        }
    }
    if (pstTlsHandShake->hServerShaHandle!= NULL)
    {
         iTmp = WapTls_HashEnd(
                               hTlsHandle,
                               pstTlsHandShake->hServerShaHandle,
                               aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash Sever SHA error\n");)
        }
    } 
    if (pstTlsHandShake->hVerifyMd5Handle!= NULL)
    {
         iTmp = WapTls_HashEnd(
                               hTlsHandle,
                               pstTlsHandShake->hVerifyMd5Handle,
                               aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash Verify MD5 error\n");)
        }
    }  
    if (pstTlsHandShake->hVerifyShaHandle!= NULL)
    {
         iTmp = WapTls_HashEnd(
                               hTlsHandle,
                               pstTlsHandShake->hVerifyShaHandle,
                               aucTmp);
        if (-1 == iTmp)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_FreeHandShake hash Verify SHA error\n");)
        }
    }  
    WAP_FREE(pstTlsHandShake);  
    pstTlsHandShake = NULL;
}


/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvServerHello
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Parse server hello message
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection
    pucMsg: meesage buffer
    uiMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/

WE_INT32 WapTlsHm_ProcRecvServerHello
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
)
{
    St_WapTlsHandshake *pstHandShake = NULL;
    St_WapTlsServerHello stServerHello = {0};
    St_WapEcdrDecoder *pstDecoder = NULL;
    WE_UINT16 usCipherSuite = 0;
    WE_INT32  iRet = 0;

    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerHello parameter error\n");)
        return -1;
    }
    pstHandShake = pstConn->pstHandshake;
    if (NULL == pstHandShake)
    {
        return -1;
    }
    
   /*pucMsg->stServerHello*/
    pstDecoder = TlsEcdr_CreateDecoder(pucMsg, uiMsgLen);
    if (NULL == pstDecoder)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerHello malloc fail\n" );)
        return -1;
    }

    iRet = WapTls_DecodeServerHello(pstDecoder,&stServerHello);
    TlsEcdr_DestroyDecoder(pstDecoder);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerHello decode error\n" );)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
        return -1;
    }   

    /*Get server version*/
    if ((stServerHello.stServerVer.ucMajor != M_WAP_TLS_PROTOCOL_VERSION_MAJOR)
            || ((stServerHello.stServerVer.ucMinor != M_WAP_TLS_PROTOCOL_VERSION_MINOR)
#ifdef WAP_CONFIG_SSL
                 &&(stServerHello.stServerVer.ucMinor != M_WAP_SSL_PROTOCOL_VERSION_MINOR)
#endif
        ))
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_PROTOCOL_VERSION);
        return -1;        
    }

#ifdef WAP_CONFIG_SSL
    if (stServerHello.stServerVer.ucMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
    {
        pstHandShake->ucFlags |= M_WAP_TLS_FLAG_IS_SSL;
    }
    if (pstConn->stProtocolVersion.ucMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
    {
        pstHandShake->ucFlags |= M_WAP_TLS_FLAG_SSL_CLIENT_HELLO;
    }
#endif
    pstConn->stProtocolVersion = stServerHello.stServerVer;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcRecvServerHello min version is %d\n"),stServerHello.stServerVer.ucMinor);)

    WAP_MEMCPY(pstHandShake->taucServerRandom, stServerHello.taucRandom,32);//32 bytes

    /*session exists*/
    if ((pstHandShake->stSessionId.ucLength > 0)
          && (pstHandShake->stSessionId.ucLength == stServerHello.stSessionId.ucLength)
             && (WAP_MEMCMP (pstHandShake->stSessionId.aucId, stServerHello.stSessionId.aucId,
                                    stServerHello.stSessionId.ucLength) == 0)
       )
    {
        pstHandShake->ucFlags &= ~M_WAP_TLS_FLAG_FULL_HANDSHAKE; /*simple handshake*/
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerHello Full handshake\n" );)
        pstHandShake->ucFlags |= M_WAP_TLS_FLAG_FULL_HANDSHAKE; /*full handshake*/
        pstHandShake->stSessionId = stServerHello.stSessionId;
    }

    /*Get cipher suite from server hello*/
    usCipherSuite = (WE_UINT16)((stServerHello.taucCipherSuite[0] << 8) | stServerHello.taucCipherSuite[1]);

    iRet = WapTlsHm_CheckCipherSuite(pstTlsHandle,pstConn, usCipherSuite);
    if (iRet < 0 || stServerHello.ucCompressMethod != M_WAP_TLS_COMPRESSION_NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerHello no cipher suite\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_ILLEGAL_PARAMETER);
        return -1;

    }

    pstConn->pstPendingRead->ucCompressionAlg = stServerHello.ucCompressMethod;
    pstConn->pstPendingWrite->ucCompressionAlg = stServerHello.ucCompressMethod;

    pstConn->pstHandshake->uiMsgs |= M_BIT_SERVER_HELLO;

    if (0 == (pstHandShake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE)) /*simple handshake*/
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerHello simple handshake\n");)
#ifdef WAP_CONFIG_SSL
         if(pstHandShake->ucFlags & M_WAP_TLS_FLAG_IS_SSL)
         {
            WapSSLHm_CreateKeys(pstTlsHandle,pstConn);
/*delete by wsh 0405*/  
#if 1
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
#endif
         }
         else
#endif
        {
            
            pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_KEYEXPANS;
            WapTlsHm_CreateKeyExpan(pstTlsHandle,pstConn);
        }
    }
    else /*full handshake*/
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerHello full handshake\n");)
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerHello return 0\n");)
    return 0;    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvServerCert
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Parse certificate message
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection
    pucMsg: meesage buffer
    uiMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/
 WE_INT32 WapTlsHm_ProcRecvServerCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR  *pucMsg,
    WE_UINT32 uiMsgLen
)
{
    St_WapTlsHandshake *pstHSHK = NULL;
    St_WapTlsServerCert *pstServerCert = NULL;
    St_WapEcdrDecoder *pstDecoder = NULL;
    WE_INT32 iRet = 0;
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
        WE_UCHAR            *pucDomain = NULL;
        WE_UINT16           usDomainLen = 0;
        WE_UCHAR *pucCert = NULL;
#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerCert begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerCert parameter error\n");)
        return -1;
    }

    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerCert handshake is null\n");)
        return -1;
    }
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
    pucDomain = pstConn->pucDomain;
    if( NULL != pucDomain )
    {
        usDomainLen = (WE_UINT16)WAP_STRLEN( (WE_CHAR *)pucDomain );
    }
    else
    {
        usDomainLen = 0;
    }
    pucCert = (WE_UCHAR*)WAP_MALLOC(uiMsgLen);//free when release tls handle
    if (NULL == pucCert)
    {
        return -1;
    }
    WAP_MEMCPY(pucCert,pucMsg,uiMsgLen);
    WapTlsHm_StoreSvrCert(pstTlsHandle,pucDomain,pucCert,uiMsgLen);
#endif

    pstServerCert = &(pstHSHK->stServerCert);
    pstDecoder = TlsEcdr_CreateDecoder(pucMsg, uiMsgLen);
    if (NULL == pstDecoder)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerCert malloc fail\n" );)
        return -1;
    }
    iRet = WapTls_DecodeServerCert(pstDecoder,pstServerCert);
    TlsEcdr_DestroyDecoder(pstDecoder);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerCert decode error\n" );)

        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
        return -1;
    }  
    pstHSHK->uiMsgs |= M_BIT_SERVER_CERTIFICATE;
    pstHSHK->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_KEY_EXCHANGE;
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvServerKeyExch
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Parse server key exchange message
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection
    pucMsg: meesage buffer
    uiMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_INT32 uiMsgLen
)
{
    St_WapTlsHandshake *pstHSHK = NULL;
    St_WapTlsServerKeyExch *pstSKExch = NULL;
    St_WapEcdrDecoder *pstDecoder = NULL;
    WE_INT32    iKeyExchAlg = 0;
    WE_INT32 iRet = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerKeyExch begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerKeyExch parameter error\n");)
        return -1;
    }

    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerKeyExch handshake is null\n");)
        return -1;
    }    

    pstSKExch = &(pstHSHK->stServerKeyExch);
    if (NULL == pstSKExch)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerKeyExch SERVERKEYEXCHANGE is null\n");)
        return -1;
    }
    iKeyExchAlg = pstHSHK->ucKeyExcMethod;
    pstDecoder = TlsEcdr_CreateDecoder(pucMsg, uiMsgLen);
    if (NULL == pstDecoder)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerKeyExch malloc fail\n" );)
        return -1;
    }
    /*pstSKExch有内存分配，还不知道怎么释放*/
    iRet = WapTls_DecodeServerKeyExch(pstDecoder,iKeyExchAlg,pstSKExch);
    TlsEcdr_DestroyDecoder(pstDecoder);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvServerKeyExch decode error\n" );)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
        return -1;
    }
   
    pstHSHK->uiMsgs |= M_BIT_SERVER_KEY_EXCHANGE;
    pstHSHK->ucState = M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE_REQUEST;

    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvCertRequest
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Parse CERT REQUEST message
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection
    pucMsg: meesage buffer
    uiMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvCertRequest
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32  uiMsgLen
)
{
    St_WapTlsHandshake *pstHSHK = NULL;
    St_WapTlsCertReq  *pstCertReq = NULL;
    St_WapEcdrDecoder *pstDecoder = NULL;
    WE_INT32 iRet = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvCertRequest begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvCertRequest parameter error\n");)
        return -1;
    }

    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvCertRequest handshake is null\n");)
        return -1;
    }    

    pstCertReq = &(pstHSHK->stCertReq);
    if (NULL == pstCertReq)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvCertRequest CERT Request is null\n");)
        return -1;
    }
    pstDecoder = TlsEcdr_CreateDecoder(pucMsg, uiMsgLen);
    if (NULL == pstDecoder)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCertRequest malloc fail\n" );)
        return -1;
    }
    iRet = WapTls_DecodeCertReq(pstDecoder,pstCertReq);
    TlsEcdr_DestroyDecoder(pstDecoder);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCertRequest decode error\n" );)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                         M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
        return -1;
    }
    
    pstHSHK->uiMsgs |= M_BIT_SERVER_CERTIFICATE_REQUEST;
    pstHSHK->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO_DONE;

    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvServerFinished
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Parse Finished message
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection
    pucMsg: meesage buffer
    uiMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   
==================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
)
{
    St_WapTlsHandshake *pstHSHK = NULL;
    WE_UCHAR        aucMD[36] = {0};
    WE_INT32 iRet = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished parameter error\n");)
        return -1;
    }

    pstHSHK = pstConn->pstHandshake;
    if (NULL == pstHSHK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished handshake is null\n");)
        return -1;
    } 

    iRet = WapTls_HashEnd(pstTlsHandle,pstHSHK->hServerMd5Handle,aucMD);
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished MD5 hash error\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR,
                                                 M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    iRet = WapTls_HashEnd(pstTlsHandle,pstHSHK->hServerShaHandle,aucMD + 16);
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished SHA hash error\n");)        
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR,
                                                 M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    pstHSHK->hServerMd5Handle = NULL;
    pstHSHK->hServerShaHandle = NULL;

    if (uiMsgLen != 12)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcRecvServerFinished message length is %d\n"),uiMsgLen);)        
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                 M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
        return -1;
    }
    WAP_MEMCPY(pstHSHK->aucServerVerifyData, pucMsg, 12);
    pstHSHK->uiMsgs |= M_BIT_SERVER_FINISHED;
    pstHSHK->ucState = M_WAP_TLS_HM_STATE_CHECK_SERVER_FINISHED;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcRecvServerFinished Call PRF\n");) 
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                      pstConn->iObjectId,
                      0,
                      pstConn->stRead.iMasterSecretId,
                      NULL,
                      0,
                      "server finished", 
                      aucMD, 36, 12,
                      G_SEC_TLS_CONNECTION_TYPE                      
                      );
#else
    ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                          pstConn->iObjectId,
                          NULL,
                          pstConn->stRead.iMasterSecretId,
                          NULL,
                          0,
                          "server finished", 
                          aucMD, 36, 12,
                          G_SEC_TLS_CONNECTION_TYPE                      
                          );
#endif
    return 0;
}
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
static WE_INT32 WapTlsHm_StoreSvrCert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UCHAR *pucDomain,
    WE_UCHAR *pucCert,
    WE_UINT32 uiCertLen
)
{
    WE_INT32 i = 0;
    WE_INT32 j = 0;
    St_WapTlsSvrCert *pstSerCert = NULL;
    
    if (NULL == pstTlsHandle)
    {
        return -1;
    }
    for (i = 0; i< MAX_SERVER_CERT_NUM; i++)
    {
        pstSerCert = pstTlsHandle->apstServerCert[i];
        if (NULL == pstSerCert)
        {
            break;
        }
        else
        {
            /*if find one has stored match, delete the old*/
            if (WAP_MEMCMP(pstSerCert->pcHost, pucDomain,WAP_STRLEN((WE_CHAR*)pucDomain)) == 0)
            {
                WAP_FREE(pstSerCert->pcHost);
                WAP_FREE(pstSerCert->pvCertContent);
                break;
            }
        }        
    }
    if (i >= MAX_SERVER_CERT_NUM) /*Over flow, delete the first*/
    {
        j = i - MAX_SERVER_CERT_NUM;
        pstSerCert = pstTlsHandle->apstServerCert[j];
        WAP_FREE(pstSerCert->pcHost);
        WAP_FREE(pstSerCert->pvCertContent);
    }
    else
    {
        j = i;
        if (NULL == pstTlsHandle->apstServerCert[j] )
        {
            pstTlsHandle->apstServerCert[j] = (St_WapTlsSvrCert*)WAP_MALLOC(sizeof(St_WapTlsSvrCert));
            if (NULL == pstTlsHandle->apstServerCert[j])
            {
                return -1;
            }    
        }
    }

    pstTlsHandle->apstServerCert[j]->pcHost = (WE_CHAR*)WAP_MALLOC(WAP_STRLEN((WE_CHAR*)pucDomain));
    if (NULL == pstTlsHandle->apstServerCert[j]->pcHost)
    {
        return -1;
    }
    WAP_MEMCPY(pstTlsHandle->apstServerCert[j]->pcHost,pucDomain,WAP_STRLEN((WE_CHAR*)pucDomain));
    pstTlsHandle->apstServerCert[j]->pvCertContent = pucCert;
    pstTlsHandle->apstServerCert[j]->uiCertLen = uiCertLen;
    return 0;
}
WE_INT32 WapTlsHm_GetFitCert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_CHAR *pcHost,
    WE_VOID **ppvServerCert
)
{
    WE_INT32 i = 0;
    St_WapTlsSvrCert *pstSerCert = NULL;
    
    
    if (NULL == pstTlsHandle || NULL == pcHost || NULL == ppvServerCert)
    {
        return -1;
    }
    *ppvServerCert = NULL;

    for (i = 0; i< MAX_SERVER_CERT_NUM; i++)
    {
        pstSerCert = pstTlsHandle->apstServerCert[i];
        if (NULL == pstSerCert)/*can't find*/
        {
            return 0;
        }
        else
        {
            if (WAP_MEMCMP(pstSerCert->pcHost, pcHost,WAP_STRLEN(pcHost)) == 0)
            {
               break;
            }
        }        
    }  
    if (i >= MAX_SERVER_CERT_NUM) /*no store*/
    {
        return 0;
    }
    *ppvServerCert = pstSerCert->pvCertContent;
    /* 
     because browser doesn't want to free our memory, we needn't copy,
     with terminate TLS module(wap release ,free server content
    *ppvServerCert = WAP_MALLOC(pstSerCert->uiCertLen);
    if (NULL == *ppvServerCert)
    {
        return -1;
    }
    WE_MEMCPY(*ppvServerCert,pstSerCert->pvCertContent,pstSerCert->uiCertLen);
    */
    return 0;
}


#endif

#endif





















