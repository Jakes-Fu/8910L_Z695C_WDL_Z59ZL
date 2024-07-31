/*==================================================================================================
    FILE NAME : sec_iwapim.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the function prototype 
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                          Tracking
    Date              Author            Number           Description of changes
    ----------   ------------ ---------   -------------------------------------------------------
    2006-06-29   Kevin Yang                            create sec_iwapim.h
    2007-05-17     tang                          Delete cert manage and signtext
    2007-06-19    bluefire                         four bytes align for dragonfly
==================================================================================================*/
/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef SEC_MSGIM_H
#define SEC_MSGIM_H

/*******************************************************************************
*   Type Define Section
*******************************************************************************/
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

enum tagE_VerifySvrCertIm_State
{
    E_STATE_VERIFY_SVR_CERT_DIRECT_RETURN = 0x00, 
    E_STATE_WTLS_VERIFY_SVR_CERT_CHAIN, 
    E_STATE_HANDLE_UE_CONFIRM,
    E_STATE_VERIFYSVRCERT_DEFAULT = 0xFFFFFFFF /* four bytes align */
};

enum tagE_GetCipherSuiteIm_State
{
    E_STATE_CIPHER_SUITE_DIRECT_RETURN = 0x00, 
    E_STATE_WTLS_GET_CIPHER_SUITE,  
    E_STATE_UE_CONFIRM, 
    E_STATE_UE_CONFIRM_HANDLE,
    E_STATE_GETCIPHER_DEFAULT = 0xFFFFFFFF /* four bytes align */    
};

enum tagE_CompSignatureIm_State
{
    E_STATE_COMPUTE_SIGN_DIRECT_RETURN = 0x00, 
    E_STATE_WTLS_COMPUTE_SIGN, 
    E_STATE_WTLS_COMPUTE_SIGN_RESP, 
    E_STATE_WTLS_COMPUTE_SIGN_AGAIN ,
    E_STATE_COMPSIG_DEFAULT = 0xFFFFFFFF /* four bytes align */
};


#ifdef M_SEC_CFG_TLS

enum tagE_TlsCompSignatureIm_State
{
    E_STATE_TLS_COMPUTE_SIGN_DIRECT_RETURN = 0x00, 
    E_STATE_TLS_COMPUTE_SIGN, 
    E_STATE_TLS_COMPUTE_SIGN_RESP, 
    E_STATE_TLS_COMPUTE_SIGN_AGAIN  ,
    E_STATE_TLS_COMPUTE_SIGN_DEFAULT = 0xFFFFFFFF /* four bytes align */
};

enum tagE_TlsGetCipherSuiteIm_State
{
    E_STATE_TLS_CIPHER_SUITE_DIRECT_RETURN = 0x00, 
    E_STATE_TLS_GET_CIPHER_SUITE,  
    E_STATE_TLS_UE_CONFIRM, 
    E_STATE_TLS_UE_CONFIRM_HANDLE ,
    E_STATE_TLS_DEFAULT = 0xFFFFFFFF /* four bytes align */
};

enum tagE_TlsVerifySvrCertIm_State
{
    E_STATE_TLS_VERIFY_SVR_CERT_DIRECT_RETURN = 0x00, 
    E_STATE_TLS_VERIFY_SVR_CERT_CHAIN_COMNAME, 
    E_STATE_TLS_VERIFY_SVR_CERT_CHAIN,
    E_STATE_TLS_HANDLE_UE_CONFIRM  ,
    E_STATE_TLS_VERIFY_SVR_DEFAULT = 0xFFFFFFFF /* four bytes align */
};
#endif


#endif

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
#ifdef M_SEC_CFG_WTLS
void Sec_GetCipherSuiteIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId,WE_HANDLE hTrustKeyIds);

void Sec_KeyExchangeIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId, 
                        St_SecWtlsKeyExchParams *pstParam,  WE_UINT8 ucHashAlg, 
                        const WE_UCHAR * pucRandval);
#endif

void Sec_GetPrfResultIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId, WE_UINT8 ucAlg, WE_INT32 iMasterSecretId,
                         const WE_UCHAR * pucSecret, WE_INT32 iSecretLen, const WE_CHAR * pcLabel,
                         const WE_UCHAR * pucSeed, WE_INT32 iSeedLen, WE_INT32 iOutputLen);
#ifdef M_SEC_CFG_WTLS
void Sec_VerifySvrCertIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId, const WE_UCHAR * pucBuf, 
                          WE_INT32 iBufLen,  const WE_UCHAR * pucAddr, WE_INT32 iAddrLen,
                          const WE_CHAR * pucDomain, WE_INT32 iDomainLen);
#endif
void Sec_CompSignatureIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                            const WE_UCHAR * pucPubKeyHash,WE_UINT16  usPubKeyHashLen,
                            const WE_UCHAR *pucData,WE_UINT16 usDataLen,WE_UCHAR  ucAlg);
#ifdef M_SEC_CFG_TLS
WE_VOID Sec_GetTlsCipherSuiteIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID);
void Sec_VerifyTlsCertChainIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                              const WE_UCHAR * pucAddr, WE_UINT16 usAddrLen,  
                              const WE_UCHAR * pucDomain, WE_UINT16 usDomainLen,
                              const St_SecTlsAsn1Certificate *pstCerts,
                              WE_UINT8 ucNumCerts);
void Sec_GetTlsPrfResultIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucAlg, WE_INT32 iMasterSecretId,
                            const WE_UCHAR * pucSecret, WE_INT32 iSecretLen, const WE_CHAR * pcLabel,
                            const WE_UCHAR * pucSeed, WE_INT32 iSeedLen, WE_INT32 iOutputLen);

WE_VOID Sec_TlsKeyExchangeIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                              WE_UINT8 ucKeyExchangeAlgorithm, 
                              WE_INT32  iOptions, 
                              St_SecTlsKeyExchangeParams *pstParam,  
                              const WE_UCHAR *pucRandval);
#endif

void Sec_SetupConIm (WE_HANDLE hSecHandle, St_SecSetupConnection * pstPara);

void Sec_StopCon (WE_HANDLE hSecHandle, WE_INT32 iTargetId, WE_INT32 iSecurityId, 
                  WE_UINT8 ucConnectionType);


void Sec_RemovePeer (WE_HANDLE hSecHandle, WE_INT32 iTargetId,WE_INT32 iMasterSecretId);

void Sec_AttachPeerToSessionIm (WE_HANDLE hSecHandle, WE_UINT8 ucConnectionType,
                                const WE_UCHAR * pucAddress, WE_INT32 iAddressLen,
                                WE_UINT16 usPortnum,  WE_INT32 iMasterSecretId);
void Sec_SearchPeerIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId, WE_UINT8 ucConnectionType,
                       const WE_UCHAR * pucAddress, WE_INT32 iAddressLen, WE_UINT16 usPortnum);
void Sec_EnableSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucIsActive);
void Sec_DisableSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId);
void Sec_FetchSessionInfoIm (WE_HANDLE hSecHandle, WE_INT32 iTargetId, WE_INT32 iMasterSecretId);
void Sec_UpdateSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucSessionOptions,
                          WE_UCHAR * pucSessionId, WE_UINT8 ucSessionIdLen,
                          WE_UINT8 aucCipherSuite[2], WE_UINT8 ucCompressionAlg,
                          WE_UCHAR * pucPrivateKeyId, WE_UINT32 uiCreationTime);
#endif 
