/*==================================================================================================
    FILE NAME : sec_str.c
    MODULE NAME : SEC       
    
    GENERAL DESCRIPTION:
        This File will gather functions that free message queue and dp queue.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date                   Author           Number                 Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07      steven ding                                      create sec_str.c
    
==================================================================================================*/

#include "sec_comm.h"


/*************************************************************************************************
external function
*************************************************************************************************/
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/*==================================================================================================
FUNCTION: 
    Sec_TlHandleTiStructFree
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    free ue struct
ARGUMENTS PASSED:
    WE_VOID*                pvFunctionId[IN]:ue struct need be free
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_VOID Sec_TlHandleUeStructFree(WE_VOID* pvFncId)

{
    if (!pvFncId)
    {
        return;
    }
    
    switch (((St_SecTlUeFncId*)pvFncId)->eType)
    {
        case E_SEC_DP_CONFIRM_DLG:      
            break;
        default:
        {
            break;
        }    
    }
    M_SEC_SELF_FREE(pvFncId);

    return;
}

#endif

/*==================================================================================================
FUNCTION: 
    Sec_TlHandleDpStructFree
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    free dp struct 
ARGUMENTS PASSED:
    St_SecDpParameter*        pstPara[IN]:dp struct need to free
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID Sec_TlHandleDpStructFree(WE_VOID* pvPara)
{
    if (!pvPara)
    {
        return;
    }    

    switch (((St_SecDpParameter*)pvPara)->eType)
    {
        case E_SEC_DP_ESTABLISHED_CONN: 
        {
            St_SecDpConnEst* pstData = NULL;

            pstData = (St_SecDpConnEst*)pvPara;
            if(pstData->pstStrct)
            {
                if(NULL != pstData->pstStrct->stSessionInfo.pstCerts)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->stSessionInfo.pstCerts->pucCert);
                    M_SEC_SELF_FREE(pstData->pstStrct->stSessionInfo.pstCerts);
                }
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        
        case E_SEC_DP_TERMINATED_CONN:
        {
            St_SecDpConnTmt* pstData = NULL;

            pstData = (St_SecDpConnTmt*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
#ifdef M_SEC_CFG_WTLS           
        case E_SEC_DP_GET_CIPHERSUITE: 
        {
            St_SecDpGetWtlsCipherSuite* pstData = NULL;

            pstData = (St_SecDpGetWtlsCipherSuite*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }

        case E_SEC_DP_XCH_WTLS_KEY: 
        {
            St_SecDpWtlsKeyXch* pstData = NULL;

            pstData = (St_SecDpWtlsKeyXch*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucRandval)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucRandval);
                }
                if(pstData->pstStrct->pstParam)
                {
                    if ((E_SEC_WTLS_KEYEXCHANGE_RSA_ANON == pstData->pstStrct->pstParam->ucKeyExchangeSuite) ||
                        (E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512 == pstData->pstStrct->pstParam->ucKeyExchangeSuite) ||
                        (E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768 == pstData->pstStrct->pstParam->ucKeyExchangeSuite))
                    {
                        if ((pstData->pstStrct->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.usExpLen) > 0)
                        {
                            M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.pucExponent);
                        }    
                        if ((pstData->pstStrct->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.usModLen) > 0)
                        {
                            M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.pucModulus);
                        }    
                        if (255 == pstData->pstStrct->pstParam->stParams.stKeyParam.stParamSpecifier.ucParameterIndex)                    
                        {
                            if ((pstData->pstStrct->pstParam->stParams.stKeyParam.stParamSpecifier.usParamLen) > 0)
                            {
                                M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stParams.stKeyParam.stParamSpecifier.pucParams);
                            }    
                        }
                  }
                  else if ((E_SEC_WTLS_KEYEXCHANGE_RSA == pstData->pstStrct->pstParam->ucKeyExchangeSuite) ||
                        (E_SEC_WTLS_KEYEXCHANGE_RSA_512 == pstData->pstStrct->pstParam->ucKeyExchangeSuite) ||
                        (E_SEC_WTLS_KEYEXCHANGE_RSA_768 == pstData->pstStrct->pstParam->ucKeyExchangeSuite))
                    {
                        if ((pstData->pstStrct->pstParam->stParams.stCertificates.usBufLen) > 0)
                        {
                            M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stParams.stCertificates.pucBuf);
                         }    
                    }                  
                  WE_FREE(pstData->pstStrct->pstParam);
                  pstData->pstStrct->pstParam =NULL;
                }
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
#endif
#ifdef M_SEC_CFG_TLS
        case E_SEC_DP_GET_TLS_CIPHERSUITE: 
        {
            St_SecDpGetTlsCipherSuite* pstData = NULL;

            pstData = (St_SecDpGetTlsCipherSuite*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        case E_SEC_DP_XCH_TLS_KEY: 
        {
            St_SecDpTlsKeyXch* pstData = NULL;

            pstData = (St_SecDpTlsKeyXch*)pvPara;
            if(pstData->pstStrct)
            {
            switch(pstData->pstStrct->ucHashAlg)
            {
            case E_SEC_TLS_KEYEXCH_DHE_DSS: /* sec_tls_server_dh_params */
            case E_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT:
            case E_SEC_TLS_KEYEXCH_DHE_RSA:
            case E_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT: 
                if(pstData->pstStrct->pstParam)
                {
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHG )
                    {                   
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHG);
                    }
                    
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHYs)                   
                    {
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHYs);             
                    }
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHP)
                    {
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHP);              
                    }
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsSignature.pucSignature)
                    {
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsSignature.pucSignature);
                    }
                }
                break;
            case E_SEC_TLS_KEYEXCH_RSA:/* When RSA is used */
            case E_SEC_TLS_KEYEXCH_DH_DSS_EXPORT:
            case E_SEC_TLS_KEYEXCH_DH_DSS:
            case E_SEC_TLS_KEYEXCH_DH_RSA_EXPORT:
            case E_SEC_TLS_KEYEXCH_DH_RSA:
                break;
            case E_SEC_TLS_KEYEXCH_RSA_EXPORT:
                if(pstData->pstStrct->pstParam)
                 {
                    if(NULL != pstData->pstStrct->pstParam->stRSAParam.stTlsRSAParam.pucExponent)
                    {             
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stRSAParam.stTlsRSAParam.pucExponent);
                    }                
                    if(NULL != pstData->pstStrct->pstParam->stRSAParam.stTlsRSAParam.pucModulus )
                    {
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stRSAParam.stTlsRSAParam.pucModulus);
                    }
                    if(NULL != pstData->pstStrct->pstParam->stRSAParam.stTlsSignature.pucSignature )
                    {             
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stRSAParam.stTlsSignature.pucSignature );
                    }
                }
                break;
            case E_SEC_TLS_KEYEXCH_DH_ANON:
            case E_SEC_TLS_KEYEXCH_DH_ANON_EXPORT:             
                if(pstData->pstStrct->pstParam)
                {
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHG )
                    {             
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHG);
                    }                
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHYs)
                    {
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHYs);                
                    }
                    if(NULL != pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHP
                        && pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.usDHPLen > 0)
                    {              
                        M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stDHParam.stTlsDHParam.pucDHP);                
                    }
                }
                break;
            default:
                break;
                }
               M_SEC_SELF_FREE(pstData->pstStrct->pucRandval);
              if(pstData->pstStrct->pstParam)
            {
                M_SEC_SELF_FREE(pstData->pstStrct->pstParam->stSecCert.pucBuf);
            }
            M_SEC_SELF_FREE(pstData->pstStrct->pstParam);
            M_SEC_SELF_FREE(pstData->pstStrct); 
            }

           
            break;
        }
        case E_SEC_DP_TLS_PRF: 
        {
            St_SecDpTlsPrf* pstData = NULL;

            pstData = (St_SecDpTlsPrf*)pvPara;
            if(pstData->pstStrct)
            {
                if (((pstData->pstStrct->usSecretLen) > 0) && 
                    pstData->pstStrct->pucSecret)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucSecret);
                }    
                if(NULL!=(pstData->pstStrct->pcLabel))
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pcLabel);
                    pstData->pstStrct->pcLabel=NULL;
                }
                if(pstData->pstStrct->pucSeed)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucSeed);
                }
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
        case E_SEC_DP_VERIFY_TLS_CERT_CHAIN: 
        {
            WE_INT32 iLoop=0;
            St_SecDpVerifyTlsCertChain* pstData = NULL;

            pstData = (St_SecDpVerifyTlsCertChain*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucAddr)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucAddr);
                }    
                M_SEC_SELF_FREE(pstData->pstStrct->pucDomain);
                if(pstData->pstStrct->pstCerts)
                {
                    for(iLoop=0;iLoop<pstData->pstStrct->ucNumCerts;iLoop++)
                    {                        
                         M_SEC_SELF_FREE(pstData->pstStrct->pstCerts[iLoop].pucCert);
                    }
                   WE_FREE(pstData->pstStrct->pstCerts);
                   pstData->pstStrct->pstCerts =NULL;
                }
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }       
#endif
#ifdef M_SEC_CFG_WTLS
        case E_SEC_DP_WTLS_PRF: 
        {
            St_SecDpWtlsPrf* pstData = NULL;

            pstData = (St_SecDpWtlsPrf*)pvPara;
            if(pstData->pstStrct)
            {
                if (((pstData->pstStrct->usSecretLen) > 0) && 
                    pstData->pstStrct->pucSecret)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucSecret);
                }    
                if(pstData->pstStrct->pcLabel)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pcLabel);
                }
                if(pstData->pstStrct->pucSeed)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucSeed);
                }
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }

        case E_SEC_DP_VERIFY_WTLS_CERT_CHAIN: 
        {
            St_SecDpVerifyWtlsCertChain* pstData = NULL;

            pstData = (St_SecDpVerifyWtlsCertChain*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucBuf)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucBuf);
                }    
                if (pstData->pstStrct->pucAddr)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucAddr);
                }    
                if (pstData->pstStrct->pucDomain)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucDomain);
                }    
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
#endif
        case E_SEC_DP_RM_PEER_LINKS:
        {
            St_SecDpRmPeerLinks* pstData = NULL;

            pstData = (St_SecDpRmPeerLinks*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
        
        case E_SEC_DP_LINK_PEER_SESSION: 
        {
            St_SecDpPeerLinkSession* pstData = NULL;

            pstData = (St_SecDpPeerLinkSession*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucAddress)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucAddress);
                }    
                if (pstData->pstStrct->pucDomain)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucDomain);
                }    
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
        
        case E_SEC_DP_LOOKUP_PEER: 
        {
            St_SecDpLookupPeer* pstData = NULL;

            pstData = (St_SecDpLookupPeer*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucAddress)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucAddress);
                }    
                if (pstData->pstStrct->pucDomain)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucDomain);
                }    
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
        
        case E_SEC_DP_ACTIVE_SESSION: 
        {
            St_SecDpActiveSession* pstData = NULL;

            pstData = (St_SecDpActiveSession*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }    
        
        case E_SEC_DP_INVALID_SESSION: 
        {
            St_SecDpInvalidSession* pstData = NULL;

            pstData = (St_SecDpInvalidSession*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        
        case E_SEC_DP_FETCH_SESSION: 
        {
            St_SecDpfetchSession* pstData = NULL;

            pstData = (St_SecDpfetchSession*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        
        case E_SEC_DP_UPDATE_SESSION: 
        {
            St_SecDpUpdateSession* pstData = NULL;

            pstData = (St_SecDpUpdateSession*)pvPara;
            if(pstData->pstStrct)
            {
                if (pstData->pstStrct->pucSessionId)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucSessionId);
                }    
                if (pstData->pstStrct->pucPrivateKeyId)
                {
                    M_SEC_SELF_FREE(pstData->pstStrct->pucPrivateKeyId);
                }    
                M_SEC_SELF_FREE(pstData->pstStrct);
            }
            break;
        }
#ifdef M_SEC_CFG_WTLS         
        case E_SEC_DP_VIEW_CUR_CERT: 
        {
            St_SecDpViewCurCert* pstData = NULL;

            pstData = (St_SecDpViewCurCert*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        
        case E_SEC_DP_VIEW_SESSION: 
        {
            St_SecDpViewSessionInfo* pstData = NULL;

            pstData = (St_SecDpViewSessionInfo*)pvPara;
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
        
        case E_SEC_DP_GET_CUR_SEC_CLASS: 
        {
            St_SecDpGetCurSecClass* pstData = NULL;

            pstData = (St_SecDpGetCurSecClass*)pvPara;
            
            if(pstData->pstStrct)
            {
                M_SEC_SELF_FREE(pstData->pstStrct);
            }    
            break;
        }
#endif
        default:
        {
            break;
        }    
    }
    M_SEC_SELF_FREE(pvPara);
    
    return;
}

/*=====================================================================================
FUNCTION: 
	  Sec_FreeMsg
CREATE DATE: 
	  2006-07-04
AUTHOR: 
	  Sam
DESCRIPTION:
      the function is responsible for memory free after pkc response message is used.
ARGUMENTS PASSED:
	   ucID[IN]:the value of the id,no use.
	   usMsg[IN]:Value of the usMsg.
       pDatap[IN]:Pointer of data.
RETURN VALUE:
	  None
USED GLOBAL VARIABLES:
      None
USED STATIC VARIABLES:
      None
CALL BY:
    
IMPORTANT NOTES:
	  Describe anything that help understanding the function and important aspect of
	  using the function i.e side effect..etc
=====================================================================================*/
void Sec_FreeMsg (WE_UINT8 ucID, WE_INT32 iMsg, void* pvData)
{
    if (pvData == NULL)
    {
        return;
    }
    ucID = ucID;

    switch (iMsg)
    {
        case M_SEC_DP_MSG_ENCRYPT_PKC_RESP:
        {
            St_SecCrptEncryptPkcResp  *pstEncryptPkcResp = (St_SecCrptEncryptPkcResp *)pvData;
            if (pstEncryptPkcResp->pucBuf != NULL)
            {
                WE_FREE(pstEncryptPkcResp->pucBuf);
                pstEncryptPkcResp->pucBuf = NULL;
            }
            break;
        }

        case M_SEC_DP_MSG_VERIFY_SIGN_RESP:
        {
            ucID = ucID;
            break;
        }
        case M_SEC_DP_MSG_ESTABLISH_CONN:
        {
            St_SecSetupConnection* pst = (St_SecSetupConnection*)pvData;
            if(NULL != pst->stSessionInfo.pstCerts)
            {
                M_SEC_SELF_FREE(pst->stSessionInfo.pstCerts->pucCert);
                M_SEC_SELF_FREE(pst->stSessionInfo.pstCerts);
            }
        }
        break;
        case M_SEC_DP_MSG_TERMINATE_CONN:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_RM_PEER_LINKS:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_LINK_PEER_SESSION:
        {
            St_SecAttachPeer* pst = (St_SecAttachPeer*)pvData;
            M_SEC_SELF_FREE(pst->pucAddress);
        }
        break;
        case M_SEC_DP_MSG_LOOKUP_PEER:
        {
            St_SecSearchPeer* pst = (St_SecSearchPeer*)pvData;
            M_SEC_SELF_FREE(pst->pucAddress);
        }
        break;
        case M_SEC_DP_MSG_ACTIVE_SESSION:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_INVALID_SESSION:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_FETCH_SESSION:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_UPDATE_SESSION:
        {
            St_SecUpdateSession* pst = (St_SecUpdateSession*)pvData;
            M_SEC_SELF_FREE(pst->pucPrivateKeyId);
            M_SEC_SELF_FREE(pst->pucSessionId);
        }
        break;
        
        case M_SEC_DP_MSG_VIEW_CUR_CERT:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_VIEW_SESSION:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_GET_CUR_SEC_CLASS:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_GET_CIPHERSUITE:
        {
            ucID = ucID;
        }
        break;
#ifdef M_SEC_CFG_TLS
        case M_SEC_DP_MSG_GET_TLS_CIPHERSUITE:
        {
            ucID = ucID;
        }
        break;
        case M_SEC_DP_MSG_EXCHANGE_TLS_KEY:
        {
            St_SecTlsKeyExchange     * pst = (St_SecTlsKeyExchange*)pvData;
            if(pst->pucRandval)
            {
                M_SEC_SELF_FREE(pst->pucRandval);
            }
            switch(pst->ucHashAlg)
            {
            case E_SEC_TLS_KEYEXCH_DHE_DSS: /* sec_tls_server_dh_params */
            case E_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT:
            case E_SEC_TLS_KEYEXCH_DHE_RSA:
            case E_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT:                
                if(pst->pstParam)
                 {              
                        if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHG )
                        {                   
                            M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHG);
                        }
                        
                        if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHYs)                   
                        {
                            M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHYs);             
                        }
                        if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHP)
                        {
                            M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHP);              
                        }
                        if(NULL != pst->pstParam->stDHParam.stTlsSignature.pucSignature)
                        {
                            M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsSignature.pucSignature);
                        }
                }
                break;
            case E_SEC_TLS_KEYEXCH_RSA:/* When RSA is used */
            case E_SEC_TLS_KEYEXCH_DH_DSS_EXPORT:
            case E_SEC_TLS_KEYEXCH_DH_DSS:
            case E_SEC_TLS_KEYEXCH_DH_RSA_EXPORT:
            case E_SEC_TLS_KEYEXCH_DH_RSA:
                break;
            case E_SEC_TLS_KEYEXCH_RSA_EXPORT:
                if(pst->pstParam)
                {
                    if(NULL != pst->pstParam->stRSAParam.stTlsRSAParam.pucExponent)
                    {             
                        M_SEC_SELF_FREE(pst->pstParam->stRSAParam.stTlsRSAParam.pucExponent);
                    }                
                    if(NULL != pst->pstParam->stRSAParam.stTlsRSAParam.pucModulus )
                    {
                        M_SEC_SELF_FREE(pst->pstParam->stRSAParam.stTlsRSAParam.pucModulus);
                    }
                    if(NULL != pst->pstParam->stRSAParam.stTlsSignature.pucSignature )
                    {             
                        M_SEC_SELF_FREE(pst->pstParam->stRSAParam.stTlsSignature.pucSignature );
                    }
                }
                break;
            case E_SEC_TLS_KEYEXCH_DH_ANON:
            case E_SEC_TLS_KEYEXCH_DH_ANON_EXPORT:     
                if(pst->pstParam)
                {
                    if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHG )
                    {             
                        M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHG);
                    }                
                    if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHYs)
                    {
                        M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHYs);                
                    }
                    if(NULL != pst->pstParam->stDHParam.stTlsDHParam.pucDHP
                        && pst->pstParam->stDHParam.stTlsDHParam.usDHPLen > 0)
                    {              
                        M_SEC_SELF_FREE(pst->pstParam->stDHParam.stTlsDHParam.pucDHP);                
                    }
                }
                break;
            default:
                break;
                }
        }
        break;
        case M_SEC_DP_MSG_VERIFY_TLS_CERT_CHAIN:
        {
            WE_INT32 iIndex=0;
            St_SecTlsVerifySvrCert* pst = (St_SecTlsVerifySvrCert*)pvData;
            M_SEC_SELF_FREE(pst->pucDomain);
            M_SEC_SELF_FREE(pst->pucAddr);
            for(iIndex=0;iIndex<pst->ucNumCerts;iIndex++)
            {
                if(pst->pstCerts)
                {
                    M_SEC_SELF_FREE(pst->pstCerts[iIndex].pucCert);
                }
            }              
            M_SEC_SELF_FREE(pst->pstCerts);
        }
        break;
        case M_SEC_DP_MSG_TLS_PRF:
        {
            St_SecGetPrfResult* pst = (St_SecGetPrfResult*)pvData;
            M_SEC_SELF_FREE(pst->pucSecret);
            M_SEC_SELF_FREE(pst->pcLabel);
            M_SEC_SELF_FREE(pst->pucSeed);
        }
        break;
#endif
#ifdef M_SEC_CFG_WTLS
        case M_SEC_DP_MSG_EXCHANGE_WTLS_KEY:
         {
            St_SecWtlsKeyExchange * pst = (St_SecWtlsKeyExchange*)pvData;
            if (pst->pucRandval)
            {
                M_SEC_SELF_FREE(pst->pucRandval);
            }
            if(pst->pstParam)
            {
                if ((E_SEC_WTLS_KEYEXCHANGE_RSA_ANON == pst->pstParam->ucKeyExchangeSuite) ||
                    (E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512 == pst->pstParam->ucKeyExchangeSuite) ||
                    (E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768 == pst->pstParam->ucKeyExchangeSuite))
                {
                    if ((pst->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.usExpLen) > 0)
                    {
                        M_SEC_SELF_FREE(pst->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.pucExponent);
                    }    
                    if ((pst->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.usModLen) > 0)
                    {
                        M_SEC_SELF_FREE(pst->pstParam->stParams.stKeyParam.stPubKey.stPubKey.stRsaKey.pucModulus);
                    }    
                    if (255 == pst->pstParam->stParams.stKeyParam.stParamSpecifier.ucParameterIndex)                    
                    {
                        if ((pst->pstParam->stParams.stKeyParam.stParamSpecifier.usParamLen) > 0)
                        {
                            M_SEC_SELF_FREE(pst->pstParam->stParams.stKeyParam.stParamSpecifier.pucParams);
                        }    
                    }
                }
                else if ((E_SEC_WTLS_KEYEXCHANGE_RSA == pst->pstParam->ucKeyExchangeSuite) ||
                    (E_SEC_WTLS_KEYEXCHANGE_RSA_512 == pst->pstParam->ucKeyExchangeSuite) ||
                    (E_SEC_WTLS_KEYEXCHANGE_RSA_768 == pst->pstParam->ucKeyExchangeSuite))
                {
                    if ((pst->pstParam->stParams.stCertificates.usBufLen) > 0)
                    {
                        M_SEC_SELF_FREE(pst->pstParam->stParams.stCertificates.pucBuf);
                    }    
                }
            }
            M_SEC_SELF_FREE(pst->pstParam);
        }
        break;
        case M_SEC_DP_MSG_WTLS_PRF:
        {
            St_SecGetPrfResult* pst = (St_SecGetPrfResult*)pvData;
            M_SEC_SELF_FREE(pst->pucSecret);
            M_SEC_SELF_FREE(pst->pcLabel);
            M_SEC_SELF_FREE(pst->pucSeed);
        }
        break;

        case M_SEC_DP_MSG_VERIFY_WTLS_CERT_CHAIN:
        {
            St_SecWtlsVerifySvrCert* pst = (St_SecWtlsVerifySvrCert*)pvData;
            M_SEC_SELF_FREE(pst->pucBuf);
            M_SEC_SELF_FREE(pst->pucAddr);
            
        }
        break;
#endif
        default:
            break;
    }
    WE_FREE(pvData);

}




