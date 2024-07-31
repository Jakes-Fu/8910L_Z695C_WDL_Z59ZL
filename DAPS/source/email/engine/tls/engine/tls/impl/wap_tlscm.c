/*==================================================================================================
    FILE NAME   : wap_tlscm.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/

/*==================================================================================================
    Include File Section
==================================================================================================*/
#include "tls_config.h"

#ifdef WAP_CONFIG_TLS
#include "tls_def.h"
#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h"
#include "ISecW.h"
#include "we_libalg.h"

#include "itls.h"

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



/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static WE_INT32 WapTlsHm_CreateClientCertVerify
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateClientHello
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Get client's cipher suites, and wait the sec's response.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
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
WE_INT32 WapTlsHm_CreateClientHello
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:WapTlsHm_CreateClientHello begin\n");)
    if( NULL == pstConn || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:WapTlsHm_CreateClientHello parameter error\n");)
        return -1;
    }
    if( NULL != pstConn->pstHandshake )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:WapTlsHm_CreateClientHello if pstConn->pstHandshake is not NULL\n");)
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_GET_CIPHER_SUITE_RESP;

#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        ISecW_MakeCipherSuite( pstTlsHandle->hSecHandle,pstConn->iObjectId,G_SEC_TLS_CONNECTION_TYPE ,NULL ); 

#else
        ISecW_GetCipherSuite( pstTlsHandle->hSecHandle, pstConn->iObjectId,G_SEC_TLS_CONNECTION_TYPE);
#endif
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:WapTlsHm_CreateClientHello return 0\n");)
        return 0;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:WapTlsHm_CreateClientHello pstConn->pstHandshake is NULL,return -1\n");)
        return -1;
    }
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateClientKeyExch
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Create key_block invoking PRF function.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
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
WE_INT32 WapTlsHm_CreateClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    WE_UCHAR    aucTmpRand[64] = {'\0'};
    WE_INT32    iOptions = 0;
    St_WapTlsHandshake      *pstHs = NULL;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch begin\n");)
    if( NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch parameter error\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if (NULL == pstHs)
    {
        return -1;
    }
#ifdef WAP_CONFIG_SSL
    if( pstHs->ucFlags & M_WAP_TLS_FLAG_IS_SSL )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch Flag SSL\n");)
        iOptions |= G_SEC_TLS_OPTION_IS_SSL;
    }
    if( pstHs->ucFlags & M_WAP_TLS_FLAG_SSL_CLIENT_HELLO)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch Flag Client Hello\n");)
        iOptions |= G_SEC_TLS_OPTION_SSL_CLIENT_HELLO;
    }
#endif 

    WAP_MEMCPY( aucTmpRand, pstHs->taucClientRandom, 32);
    WAP_MEMCPY( aucTmpRand + 32, pstHs->taucServerRandom, 32);

    if( (pstHs->uiMsgs & M_BIT_SERVER_CERTIFICATE)
        && ( pstHs->stServerCert.ucCertNum > 0) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch:Copy server cert\n");)
        pstHs->stServerKeyExch.stParams.stSecCert.pucBuf = pstHs->stServerCert.pstCert[0].pucCert;
        pstHs->stServerKeyExch.stParams.stSecCert.usBufLen = (WE_UINT16)pstHs->stServerCert.pstCert[0].uiCertLen;
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm:CreateClientKeyExch:cert length is %d\n"),pstHs->stServerKeyExch.stParams.stSecCert.usBufLen);)
    }
    pstHs->stServerKeyExch.stParams.iOptions = iOptions;  
    
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CREATE_KEY_EXCH_RESP;
    
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    ISecW_ProcAsyAlgorithm( pstTlsHandle->hSecHandle, 
                        pstConn->iObjectId, 
                        &pstHs->stServerKeyExch.stParams,
                        pstHs->ucKeyExcMethod,
                        aucTmpRand,
                        G_SEC_TLS_CONNECTION_TYPE ); 

#else
    ISecW_KeyExchange( pstTlsHandle->hSecHandle, 
                        pstConn->iObjectId, 
                        &pstHs->stServerKeyExch.stParams,
                        pstHs->ucKeyExcMethod,
                        aucTmpRand,
                        G_SEC_TLS_CONNECTION_TYPE ); 
#endif
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientKeyExch end, return 0\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateClientCert
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Get client's cert after receive cert request from server.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
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
WE_INT32 WapTlsHm_CreateClientCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    St_WapTlsCertReq *pstCertReq = NULL; 
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    WE_CHAR     *pcBuf = NULL;
    WE_INT32    iLen = 0;
    St_TlsRespData  stResp = {0};
#endif
    
   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCert begin\n");)
  
    if( NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCert parameter error\n");)
        return -1;
    }  

    if( !(pstConn->pstHandshake->uiMsgs & M_BIT_SERVER_CERTIFICATE_REQUEST) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCert server didn't send cert Reqest,return -1\n");)
        return -1;
    }
    pstCertReq = &(pstConn->pstHandshake->stCertReq);
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    WapTls_EncodeCertReq(pstCertReq,&pcBuf,&iLen);
    stResp.iConnId= pstConn->iConnId;
    stResp.iErrCode = G_WAP_ERR_TLS_NEED_USER_CERT;;
    stResp.iRespLen = iLen;
    stResp.pvRespData = pcBuf;
    ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,
                                               (WE_VOID*)&stResp,
                                               pstTlsHandle->hPrivData); 
    WAP_FREE(pcBuf); //old: free by mgr
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_WAIT_CLIENT_USER_CERT;
#else      
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_GET_CLIENT_CERT_RESP;
    ISecW_GetUserCert(pstTlsHandle->hSecHandle, 
                        pstConn->iObjectId, 
                        (St_SecTlsGetUsrCertParam*)pstCertReq,
                        sizeof(St_SecTlsGetUsrCertParam),
                        G_SEC_TLS_CONNECTION_TYPE );
#endif    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCert end return 0\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateClientCertVerify
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Compute a digital signature to create client cert verify.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn       
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
static WE_INT32 WapTlsHm_CreateClientCertVerify
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    WE_UCHAR            aucMd[36] = {'\0'};
    WE_INT32            iLength = 0;
    WE_INT32            iTmp = 0;/*for check the result of Hash funcation*/

    St_WapTlsHandshake  *pstHs = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify begin\n");)
    
    if( NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify parameter error,return -1\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;

    if( !( pstHs->ucFlags & M_WAP_TLS_FLAG_NEED_CERT_VERIFY ) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify Flag not NeedCertVerify,return 1\n");)
        return -1;
    }

    /*
      Get the hash result of handshake message which are used to compute sign,
      here handshake_messages refers to all handshake messages sent or received
      at client hello up to but not including this message, it also including the
      type and length fields of the handshake messages
    */
    switch( pstHs->ucKeyExcMethod)
    {
        case G_SEC_TLS_KEYEXCH_RSA:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_RSA_EXPORT:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DH_RSA_EXPORT:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DH_RSA:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DHE_RSA:
            iTmp = WapTls_HashEnd(pstTlsHandle, pstHs->hVerifyMd5Handle, aucMd);
            if( iTmp < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify MD5 HashEnd fail,return -1\n");)
                return -1;
            }
            
            iTmp = WapTls_HashEnd(pstTlsHandle, pstHs->hVerifyShaHandle, aucMd + 16);
            if( iTmp < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify SHA HashEnd fail,return -1\n");)
                return -1;
            }
            pstHs->hVerifyMd5Handle = NULL;
            pstHs->hVerifyShaHandle = NULL;
            iLength = 36;
            break;
        case G_SEC_TLS_KEYEXCH_DH_DSS_EXPORT:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DH_DSS:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT:/*lint -fallthrough */
        case G_SEC_TLS_KEYEXCH_DHE_DSS:
            iTmp = WapTls_HashEnd(pstTlsHandle, pstHs->hVerifyShaHandle, aucMd);
            if( iTmp < 0)
            {
                return -1;
            }
            pstHs->hVerifyShaHandle = NULL ;
            iLength = 20;
            break;
        default:
            return -1;
    }

    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CERT_SIGNATURE_RESP;
    
    ISecW_CompSign(pstTlsHandle->hSecHandle, 
                   pstConn->iObjectId,
                   pstHs->pucPubKeyHash,
                   pstHs->usPubKeyHashLen,
                   aucMd, iLength
                   );

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientCertVerify end,return 0\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_SendClientKeyExch
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Send client key exchange and create the cert verify.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
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
WE_INT32 WapTlsHm_SendClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch begin\n");)
    if ( (NULL == pstTlsHandle)|| (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch parameter error\n");)
        return -1;
    }
    if (NULL == pstConn->pstHandshake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch parameter pstHandshake is null\n");)
        return -1;
    }
    WapTlsHm_ProcSendClientKeyExch(pstTlsHandle,pstConn);
#ifdef WAP_CONFIG_SSL
    if (pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_IS_SSL)
    {
       WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch is SSL\n");)
       iRet = WapSSLHm_CreateClientCertVerify(pstTlsHandle,pstConn);
       /*No need cert vertify*/
       if (iRet == -1)
       {
          iRet = WapTlsHm_CreatePrf(pstTlsHandle,pstConn);/*PRF to create keys*/
          return iRet;
       }
    }
    else
#endif
    {
        iRet = WapTlsHm_CreateClientCertVerify(pstTlsHandle,pstConn);
        if (iRet == -1)/*no need cert signature*/
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch no need send client verify\n");)
            iRet = WapTlsHm_CreatePrf(pstTlsHandle,pstConn);
            return iRet;
        }

    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:SendClientKeyExch end, return 0\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreatePrf
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    If ssl, create the ccs and send it.
    If tls, call prf function to create key_block.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn        
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

WE_INT32 WapTlsHm_CreatePrf
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn
)
{

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreatePrf begin\n");)
    if ( (NULL == pstTlsHandle)|| (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreatePrf parameter error\n");)
        return -1;
    }
    if (NULL == pstConn->pstHandshake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreatePrf parameter error\n");)
        return -1;
    }
#ifdef WAP_CONFIG_SSL
    if (pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_IS_SSL)
    {
        WapSSLHm_CreateKeys(pstTlsHandle,pstConn);
/*delete by wsh 0405*/  
#if 1   
        TlsMsg_PostMsg(
                          pstTlsHandle->hMsgHandle,
                          E_WAP_MSG_MODULEID_TLS,
                          M_WAP_TLS_TLSHM_MSG_SEND_CCS,
                          pstConn->iConnId,
                          0,0,0,NULL,NULL);
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;
#endif         
            
    }
    else
#endif
    {
         pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_KEYEXPANS;
        /*Start generate the key include MAC key, encrypt key, decrypt key*/
        WapTlsHm_CreateKeyExpan(pstTlsHandle,pstConn);
       
    }
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateKeyExpan
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    PRF( "key expansion" ) to compute the key_block.
ARGUMENTS PASSED:
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
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
WE_INT32 WapTlsHm_CreateKeyExpan
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
)
{
    St_WapTlsConnState      *pstConnState = NULL;
    St_WapTlsHashInfo       *pstHashInfo = NULL;
    St_WapTlsCipherInfo     *pstCipherInfo = NULL;
    WE_INT32                iKeyBlockSize = 0;
    WE_UCHAR                aucRandom[64] = {'\0'};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan begin\n");)

    if( (NULL == pstTlsHandle) || (NULL == pstConn ))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan parameter error\n");)
        return -1;
    }

    pstConnState = pstConn->pstPendingWrite;
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan Connstate error\n");)
        return -1;
    }
    pstHashInfo = WapTls_FindHash( pstTlsHandle,pstConnState->ucHashAlg );
    if (NULL == pstHashInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan Hash info null\n");)        
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher( pstTlsHandle,pstConnState->ucCipherAlg );
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan Cipher info NULL\n");)
        return -1;
    }
    /*not be exportable, compute IV later*/
    if( TRUE == pstCipherInfo->ucExportAble )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan exportable\n");)        
        iKeyBlockSize = 2 * ( pstHashInfo->ucHashSize \
                              + pstCipherInfo->ucKeyMaterialLen );
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan not exportable\n");)
        iKeyBlockSize = 2 * ( pstHashInfo->ucHashSize \
                              + pstCipherInfo->ucKeyMaterialLen \
                              + pstCipherInfo->ucIvSize );
    }

    /*random = server_random + client_random, rfc2246*/
    WAP_MEMCPY( (void *)aucRandom, (void *)pstConn->pstHandshake->taucServerRandom, 32);
    WAP_MEMCPY( (void *)(aucRandom + 32), (void *)pstConn->pstHandshake->taucClientRandom, 32);
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                       pstConn->iObjectId,
                       0,
                       pstConnState->iMasterSecretId,
                       NULL,
                       0,
                       "key expansion", 
                       aucRandom,
                       64, 
                       iKeyBlockSize,
                       G_SEC_TLS_CONNECTION_TYPE);
#else
    ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                       pstConn->iObjectId,
                       NULL,
                       pstConnState->iMasterSecretId,
                       NULL,
                       0,
                       "key expansion", 
                       aucRandom,
                       64, 
                       iKeyBlockSize,
                       G_SEC_TLS_CONNECTION_TYPE);

#endif
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeyExpan end\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateCWriteKey
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    If KeyExchange algorithm is exportable, PRF("client write key").
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn,
    St_SecGetPrfResp  *pstPrfRsp    : PRF key_block response
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
WE_INT32 WapTlsHm_CreateCWriteKey
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn,
    St_SecGetPrfResp  *pstPrfRsp
)
{
    St_WapTlsConnState      *pstConnState = NULL;
    St_WapTlsHashInfo       *pstHashInfo = NULL;
    St_WapTlsCipherInfo      *pstCipherInfo = NULL;

    WE_INT32        iReturn = 0;
    WE_UCHAR        aucRandom[64] = {'\0'};
    WE_UCHAR        *pucBuffer = NULL;
    WE_INT32        iTmp = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey begin\n");)
    if( (NULL ==  pstTlsHandle) || (NULL == pstConn ) || (NULL == pstPrfRsp)||(NULL == pstPrfRsp->pucBuf))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey parameter error\n");)
        return -1;
    }
    if( pstPrfRsp->usResult != G_SEC_OK )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey PRF Err\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, \
                                   M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }

    pstConnState = pstConn->pstPendingWrite;
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey Connstate NULL\n");)
        return -1;
    }
    pstHashInfo = WapTls_FindHash( pstTlsHandle,pstConnState->ucHashAlg );
    if (NULL == pstHashInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey Hash info NULL\n");)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher( pstTlsHandle,pstConnState->ucCipherAlg );
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey Cipher info NULL\n");)
        return -1;
    }
    
    pucBuffer = pstPrfRsp->pucBuf;

    WAP_MEMCPY( (void *)pstConn->pstPendingWrite->aucMacSecret, (void *)pucBuffer, (WE_UINT32)pstHashInfo->ucHashSize );
    pucBuffer += pstHashInfo->ucHashSize;
    
    WAP_MEMCPY( (void *)pstConn->pstPendingRead->aucMacSecret, (void *)pucBuffer, (WE_UINT32)pstHashInfo->ucHashSize );
    pucBuffer += pstHashInfo->ucHashSize;

    WAP_MEMCPY( pstConn->pstPendingWrite->aucEncryptionKey, pucBuffer, \
                (WE_UINT32)pstCipherInfo->ucKeyMaterialLen );
    pucBuffer += pstCipherInfo->ucKeyMaterialLen;

    WAP_MEMCPY( pstConn->pstPendingRead->aucEncryptionKey, pucBuffer, \
                (WE_UINT32)pstCipherInfo->ucKeyMaterialLen );
    pucBuffer += pstCipherInfo->ucKeyMaterialLen;

    if( TRUE == pstCipherInfo->ucExportAble )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey exportable\n");)
        /*random = client_random + server_random, rfc2246*/
        WAP_MEMCPY( aucRandom, pstConn->pstHandshake->taucClientRandom, 32);
        WAP_MEMCPY( aucRandom + 32, pstConn->pstHandshake->taucServerRandom, 32);
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_CWKEY;        
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                           pstConn->iObjectId,
                           0,
                           0,
                           pstConn->pstPendingWrite->aucEncryptionKey,
                           pstCipherInfo->ucKeyMaterialLen,
                           "client write key", 
                           aucRandom, 64, 
                           pstCipherInfo->ucKeySize,
                           G_SEC_TLS_CONNECTION_TYPE);
#else
         ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                           pstConn->iObjectId,
                           NULL,
                           0,
                           pstConn->pstPendingWrite->aucEncryptionKey,
                           pstCipherInfo->ucKeyMaterialLen,
                           "client write key", 
                           aucRandom, 64, 
                           pstCipherInfo->ucKeySize,
                           G_SEC_TLS_CONNECTION_TYPE);
#endif

        iReturn = 1;
    }
    else if( M_WAP_TLS_CIPHER_TYPE_BLOCK == pstCipherInfo->ucType)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey not exportable block\n");)
        WAP_MEMCPY( pstConn->pstPendingWrite->aucIV, pucBuffer, (WE_UINT32)pstCipherInfo->ucIvSize);
        pucBuffer += pstCipherInfo->ucIvSize;
        WAP_MEMCPY( pstConn->pstPendingRead->aucIV, pucBuffer, (WE_UINT32)pstCipherInfo->ucIvSize);
    }   
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateCWriteKey not exportable stream\n");)
        iTmp = WapTls_DecryptStreamInit(pstTlsHandle, 
                                        pstCipherInfo->ucCipherAlg, 
                                        pstConn->pstPendingRead->aucEncryptionKey, 
                                        pstCipherInfo->ucKeySize, 
                                        &(pstConn->pstPendingRead->hCipherHandle));
        if( iTmp < 0)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return -1;
        }
        iTmp = WapTls_EncryptStreamInit(pstTlsHandle, 
                                        pstCipherInfo->ucCipherAlg, 
                                        pstConn->pstPendingWrite->aucEncryptionKey, 
                                        pstCipherInfo->ucKeySize, 
                                        &(pstConn->pstPendingWrite->hCipherHandle));
        if( iTmp < 0)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return -1;
        }
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_CreateCWriteKey end return %d\n"),iReturn);)
    return iReturn;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateSWriteKey
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    If KeyExchange algorithm is exportable, PRF("server write key").
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp
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
WE_INT32 WapTlsHm_CreateSWriteKey
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp
)
{
    St_WapTlsConnState      *pstConnState = NULL;
    St_WapTlsCipherInfo      *pstCipherInfo = NULL;
    WE_UCHAR        aucRandom[64] = {'\0'};
    WE_UCHAR        *pucBuffer = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey Begin\n");)

    if( (NULL ==  pstTlsHandle) || (NULL == pstConn ) || (NULL == pstPrfRsp) || (NULL == pstPrfRsp->pucBuf))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey parameter error\n");)
        return -1;
    }
    if( pstPrfRsp->usResult != G_SEC_OK )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey PRF result != OK\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, \
                                M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    pstConnState = pstConn->pstPendingWrite;
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey Conn state NULL\n");)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg );
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey Cipher info NULL\n");)
        return -1;
    }
    pucBuffer = pstPrfRsp->pucBuf;
    

    WAP_MEMCPY( pstConn->pstPendingWrite->aucEncryptionKey, pucBuffer, pstCipherInfo->ucKeySize );
    pucBuffer += pstCipherInfo->ucKeySize;
    
    WAP_MEMCPY( aucRandom, pstConn->pstHandshake->taucClientRandom, 32 );
    WAP_MEMCPY( aucRandom + 32, pstConn->pstHandshake->taucServerRandom, 32 );
    
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_SWKEY;
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                      pstConn->iObjectId,
                      0,
                      0,
                      pstConn->pstPendingRead->aucEncryptionKey,
                      pstCipherInfo->ucKeyMaterialLen,
                      "server write key", 
                      aucRandom, 64, 
                      pstCipherInfo->ucKeySize,
                      G_SEC_TLS_CONNECTION_TYPE );
#else
    ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                      pstConn->iObjectId,
                      NULL,
                      0,
                      pstConn->pstPendingRead->aucEncryptionKey,
                      pstCipherInfo->ucKeyMaterialLen,
                      "server write key", 
                      aucRandom, 64, 
                      pstCipherInfo->ucKeySize,
                      G_SEC_TLS_CONNECTION_TYPE );

#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateSWriteKey end return 0\n");)

    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateIVBlock
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    PRF( " IV block" ) to generate IV_block.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp : PRF("server write key") response 
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
 WE_INT32 WapTlsHm_CreateIVBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp
)
{
    St_WapTlsConnState      *pstConnState = NULL;
    St_WapTlsCipherInfo      *pstCipherInfo = NULL;
    WE_UCHAR        aucRandom[64] = {'\0'};
    WE_UCHAR        *pucBuffer = NULL;
    WE_INT32        iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock Begin\n");)

    if( (NULL ==  pstTlsHandle) || (NULL == pstConn ) || (NULL == pstPrfRsp)|| (NULL == pstPrfRsp->pucBuf))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock Parameter error\n");)
        return -1;
    }
    if( pstPrfRsp->usResult != G_SEC_OK )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock PRF result != OK\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, \
                                M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        return -1;
    }
    
    pstConnState = pstConn->pstPendingWrite;
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock Conn state NULL\n");)
        return -1;
    }
    
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg );
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock Cipher Info NULL\n");)
        return -1;
    }
    
    pucBuffer = pstPrfRsp->pucBuf;    
    WAP_MEMCPY( pstConn->pstPendingRead->aucEncryptionKey, pucBuffer, pstCipherInfo->ucKeySize );
    
    WAP_MEMCPY( aucRandom, pstConn->pstHandshake->taucClientRandom, 32 );
    WAP_MEMCPY( aucRandom + 32, pstConn->pstHandshake->taucServerRandom, 32 );

    if( M_WAP_TLS_CIPHER_TYPE_BLOCK == pstCipherInfo->ucType)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock block cipher\n");)
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_IVBLOCK;            
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                             pstConn->iObjectId,
                             0,
                             0,
                             (WE_UCHAR *)"",
                             0,
                             "IV block", 
                             aucRandom, 64,
                             2 * pstCipherInfo->ucIvSize,
                             G_SEC_TLS_CONNECTION_TYPE );
#else
        ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                                     pstConn->iObjectId,
                                     NULL,
                                     0,
                                     (WE_UCHAR *)"",
                                     0,
                                     "IV block", 
                                     aucRandom, 64,
                                     2 * pstCipherInfo->ucIvSize,
                                     G_SEC_TLS_CONNECTION_TYPE );

#endif
        return 1;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock stream cipher\n");)
        iTmp = WapTls_DecryptStreamInit(pstTlsHandle,
                                        pstCipherInfo->ucCipherAlg,
                                        pstConn->pstPendingRead->aucEncryptionKey, 
                                        pstCipherInfo->ucKeySize, 
                                        &(pstConn->pstPendingRead->hCipherHandle));
        if( iTmp < 0 )
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return -1;
        }
        iTmp = WapTls_EncryptStreamInit(pstTlsHandle,
                                        pstCipherInfo->ucCipherAlg,
                                        pstConn->pstPendingWrite->aucEncryptionKey, 
                                        pstCipherInfo->ucKeySize, 
                                        &(pstConn->pstPendingWrite->hCipherHandle));        
        if( iTmp < 0)
        {
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                     M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            return -1;
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateIVBlock end\n");)
        return 0;
    }

}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CreateKeysFinal
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Final PRF compute, process the PRF("IV block") response.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn,
    St_SecGetPrfResp  *pstPrfRsp
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
 WE_INT32 WapTlsHm_CreateKeysFinal
(
    St_WapTlsHandle   *pstTlsHandle,
    St_WapTlsConn     *pstConn,
    St_SecGetPrfResp  *pstPrfRsp
)
{
    St_WapTlsConnState      *pstConnState = NULL;
    St_WapTlsCipherInfo     *pstCipherInfo = NULL;
    WE_UCHAR        *pucBuffer = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal begin\n");)

    if( NULL == pstPrfRsp )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal response is null\n");)
        return -1;
    }
    if( (NULL ==  pstTlsHandle) || (NULL == pstConn )|| (NULL == pstPrfRsp->pucBuf))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal parameter error\n");)
        return -1;
    }
    if( pstPrfRsp->usResult != G_SEC_OK )
    {
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_INTERNAL_ERROR, \
                                M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);

        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal PRF result != OK\n");)
        return -1;
    }
    pstConnState = pstConn->pstPendingWrite;
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal Conn state NULL\n");)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg );
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal Cipher Info NULL\n");)
        return -1;
    }
    pucBuffer = pstPrfRsp->pucBuf;    

    WAP_MEMCPY( pstConn->pstPendingWrite->aucIV, pucBuffer, pstCipherInfo->ucIvSize );
    pucBuffer += pstCipherInfo->ucIvSize;

    WAP_MEMCPY( pstConn->pstPendingRead->aucIV, pucBuffer, pstCipherInfo->ucIvSize );

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_CreateKeysFinal End\n");)
    return 0;
}
/*==================================================================================================
FUNCTION:
    WapTlsHm_CreateClientFinished
CREATE DATE: 
    2006-12-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Create client finished.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn
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
WE_INT32 WapTlsHm_CreateClientFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn
)    
{

    St_WapTlsHandshake  *pstHs = NULL;
    WE_UCHAR    aucMd[36] = {'\0'};
    WE_INT32    iResult = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished begin\n");)
    if( NULL == pstTlsHandle || NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished parameter error\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if( NULL == pstHs )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished handshake NULL\n");)
        return -1;
    }
    
    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hClientMd5Handle, aucMd );
    if( iResult < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished HashEnd fail\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, 
                                    G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hClientShaHandle, aucMd + 16 );
    if( iResult < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished HashEnd fail\n");)
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, 
                                    G_WAP_ERR_TLS_INTERNAL_ERROR, 
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR );
        return -1;
    }
    /*in WapTls_HashEnd, has release Hash handle*/
    pstHs->hClientMd5Handle = NULL;
    pstHs->hClientShaHandle = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm:CreateClientFinished master secret id is %d\n"),pstConn->stWrite.iMasterSecretId);)

    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_CLIENT_FINISHED;

#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    ISecW_ComputePrf(pstTlsHandle->hSecHandle, 
                     pstConn->iObjectId,
                     0,
                     pstConn->stWrite.iMasterSecretId,
                     NULL,
                     0,
                     "client finished", 
                     aucMd, 36, 
                     12,
                     G_SEC_TLS_CONNECTION_TYPE);
#else
    ISecW_GetPrfResult(pstTlsHandle->hSecHandle, 
                       pstConn->iObjectId,
                       0,
                       pstConn->stWrite.iMasterSecretId,
                       NULL,
                       0,
                       "client finished", 
                       aucMd, 36, 
                       12,
                       G_SEC_TLS_CONNECTION_TYPE);

#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CreateClientFinished END\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_VerifyCert
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Verify server's cert chain.
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
RETURN VALUE:
    1   :   verify cert no need
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
WE_INT32 WapTlsHm_VerifyCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_UCHAR            aucAddr[18] = {'\0'};
    St_WapTlsHandshake  *pstHs = NULL;
    WE_UINT16           usAddrLen = 0;
    St_SecTlsVerifyCertChainParam  stParams = {0};
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE    
    void*             pvRootCert = NULL;
    WE_INT32  iFlag = 0;
#endif
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:Verify Cert begin\n");)
    
    if( NULL == pstTlsHandle || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:Verify Cert parameter error\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;

    if (NULL == pstHs)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:Verify Cert parameter error\n");)
        return -1;
    }

    if( !(pstHs->uiMsgs & M_BIT_SERVER_CERTIFICATE) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:Verify Cert no need\n");)
        return 1;
    }
  
    if (!(pstConn->bUseProxy))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:Verify Cert not using proxy\n");)
        usAddrLen = (WE_UINT16)pstConn->iAddrLen;
        WAP_MEMCPY(aucAddr,pstConn->pucAddr,pstConn->iAddrLen);
    }

    stParams.pstCerts = pstHs->stServerCert.pstCert;
    stParams.ucNumCerts = pstHs->stServerCert.ucCertNum;
    stParams.pucAddr = aucAddr;
    stParams.usAddrLen = usAddrLen;
    stParams.pucDomain = pstConn->pucDomain;
    stParams.usDomainLen = pstConn->usDomainLen;

    // wsh 20070614 add for Sec operation changed to synchronization
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CERT_VERIFY_RESULT;

#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    if (pstTlsHandle->pstRootCert != NULL)
    {
        pvRootCert = (void*)pstTlsHandle->pstRootCert->pstHead;
        iFlag = pstTlsHandle->pstRootCert->iNew;
    }
    ISecW_ValidateSvrCert( pstTlsHandle->hSecHandle, 
                           pstConn->iObjectId,
                           (void*)&stParams,
                           sizeof(St_SecTlsVerifyCertChainParam),                               
                           pvRootCert,
                           (WE_CHAR)iFlag,
                           G_SEC_TLS_CONNECTION_TYPE
                           );   
    if (pstTlsHandle->pstRootCert != NULL)
    {
        pstTlsHandle->pstRootCert->iNew = FALSE; // after use,change to FALSE
    }
#else
    ISecW_VerifySvrCertChain(pstTlsHandle->hSecHandle,
                             pstConn->iObjectId, 
                             (void*)&stParams,
                             sizeof(St_SecTlsVerifyCertChainParam),
                             G_SEC_TLS_CONNECTION_TYPE
                            );
#endif
    
    /*SEC will memcpy the pucDomain before send message to tls,so here free the pointer*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:VerifyCert end, return 0\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CheckPrfServerFinished
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Check the response of PRF( "server finished" ).
ARGUMENTS PASSED:    
   St_WapTlsConn *pstConn,
   Sec_tls_prf_response  *pstPrfResp        
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
    pstPrfResp will be freed in ProcPrf Response function in tlssec.c .
==================================================================================================*/
WE_INT32 WapTlsHm_CheckPrfServerFinished
(
   St_WapTlsConn *pstConn,   
   St_SecGetPrfResp  *pstPrfResp   
)
{

    WE_INT32    iResult = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckPrfServerFinished begin\n");)
    if( NULL == pstPrfResp || NULL == pstConn )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckPrfServerFinished parameter error\n");)
        return -1;
    }
    
    iResult = ( (G_SEC_OK == pstPrfResp->usResult)
                && ( 12 == pstPrfResp->usBufLen )
                && ( 0 == WAP_MEMCMP(pstPrfResp->pucBuf, pstConn->pstHandshake->aucServerVerifyData, 12) )
              );

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckPrfServerFinished end\n");)
    return iResult;
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_CheckCipherSuite
CREATE DATE: 
    2006-12-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Check the cipher suite from server hello,and then get the information.
ARGUMENTS PASSED:    
    St_WapTlsConn *pstConn,
    WE_UINT16   usCipherSuite
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
WE_INT32 WapTlsHm_CheckCipherSuite
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UINT16   usCipherSuite
)
{
    St_WapTlsHandshake          *pstHs = NULL;
    St_WapTlsCipherSuiteInfo    *pstCsInfo = NULL;
    WE_INT32        i = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite begin\n");)
    if( NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite parameter error\n");)
        return -1;
    }

    pstHs = pstConn->pstHandshake;

    if( NULL == pstHs )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite parameter error\n");)
        return -1;
    }

    for( i = 0; i < pstHs->usCipherSuitesLen; i += 2)
    {
        if( pstHs->pucCipherSuites[i] == ( usCipherSuite >> 8) 
            && (pstHs->pucCipherSuites[i + 1] == (usCipherSuite & 0xFF) ))
        {
            break;
        }
    }
    if( i >= pstHs->usCipherSuitesLen )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite CipherSuites error\n");)
        return -1;
    }

    pstCsInfo = WapTls_FindCipherSuite(pstTlsHandle, usCipherSuite);
    if( NULL == pstCsInfo )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite:FindCipherSuite error\n");)
        return -1;
    }

    pstConn->pstHandshake->ucKeyExcMethod = pstCsInfo->ucKeyExcMethod;
    
    pstConn->pstHandshake->usCipherSuite = usCipherSuite;
    pstConn->pstPendingRead->ucCipherAlg = pstCsInfo->ucCipherAlg;
    
    pstConn->pstPendingWrite->ucCipherAlg = pstCsInfo->ucCipherAlg;
    pstConn->pstPendingRead->ucHashAlg = pstCsInfo->ucHashAlg;
    pstConn->pstPendingWrite->ucHashAlg = pstCsInfo->ucHashAlg;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm:CheckCipherSuite:End,return 0\n");)
    return 0;

}
#endif



