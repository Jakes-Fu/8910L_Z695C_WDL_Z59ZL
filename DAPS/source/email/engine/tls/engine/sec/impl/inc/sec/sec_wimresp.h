/*==================================================================================================
    FILE NAME : sec_wimresp.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file, define the response function prototype for WIM module, and will be updated later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                             Tracking
    Date              Author                Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-19    Stone An                             create sec_wimresp.h
    
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef SEC_WIMRESP_H
#define SEC_WIMRESP_H

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
#ifdef M_SEC_CFG_WTLS
void Sec_WimWtlsGetCipherSuiteResponse (WE_HANDLE hSecHandle,
                                WE_INT32 iTargetID, 
                                WE_INT32 iResult,
                                const WE_UCHAR * pucCipherMethods,
                                WE_INT32 iCipherMethodsLen,
                                const WE_UCHAR *pucKeyExchangeIds,
                                WE_INT32 iKeyExchangeIdsLen,
                                const WE_UCHAR *pucTrustedKeyIds,
                                WE_INT32 iTrustedKeyIdsLen, 
                                WE_INT32  iTooManyCerts);
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_WimComputeSignatureResponse(WE_HANDLE hSecHandle,
                                    WE_INT32 iTargetID, WE_INT32 iResult,
                                    const WE_UCHAR *pucSig, WE_INT32 iSigLen) ;
#endif
void Sec_WimKeyExchangeResponse (WE_HANDLE hSecHandle,
                                 WE_INT32 iTargetID, 
                                 WE_INT32 iResult,
                                 WE_INT32 iMasterSecretId,
                                 const WE_UCHAR* pucPublicValue,
                                 WE_INT32 iPublicValueLen);
void Sec_WimGetPrfResultResponse (WE_HANDLE hSecHandle,
                         WE_INT32 iTargetID, WE_INT32 iResult,
                         const WE_UCHAR *pucBuf, 
                         WE_INT32 iBufLen);
void Sec_WimVerifyCertChainResponse(WE_HANDLE hSecHandle,
                                           WE_INT32 iTargetID, 
                                           WE_INT32 iResult, WE_INT32 iConnType);
void Sec_WimReadViewResponse(WE_HANDLE hSecHandle,
                                      WE_INT32 iResult,
                                      const WE_UCHAR *pucData,
                                      WE_INT32 iLen);
void Sec_WimDeletePeerLinkSessionResponse (WE_HANDLE hSecHandle,
                                     WE_INT32 iTargetID,
                                     WE_INT32 iResult);
void Sec_WimPeerLinkSessionResponse (WE_HANDLE hSecHandle,
                                       WE_INT32 iResult);
void Sec_WimPeerSearchResponse (WE_HANDLE hSecHandle,
                                WE_INT32 iTargetID,
                                WE_INT32 iResult, 
                                WE_UINT8 ucConnectionType, 
                                WE_INT32 iMasterSecretId);
void Sec_WimSessionActiveResponse (WE_HANDLE hSecHandle,
                                   WE_INT32 iResult);
void Sec_WimSessionInvalidateResponse (WE_HANDLE hSecHandle,
                                       WE_INT32 iResult);
void Sec_WimGetSessionMastResponse (WE_HANDLE hSecHandle,
                                  WE_INT32 iTargetID,
                                  WE_INT32 iResult, 
                                  WE_UINT8 ucSessionOptions,
                                  const WE_UCHAR *pucSessionId,
                                  WE_UINT8 ucSessionIdLen, 
                                  WE_UINT8 aucCipherSuite[2],
                                  WE_UINT8 ucCompressionAlg,
                                  const WE_UCHAR *pucPrivateKeyId,
                                  WE_UINT32 uiCreationTime);
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_WimWtlsComputeSignatureResponse(WE_HANDLE hSecHandle,
                                            WE_INT32 iTargetID, WE_INT32 iResult,
                                            const WE_UCHAR *pucSig, WE_INT32 iSigLen) ;
#endif
void Sec_WimSessionUpdateResponse (WE_HANDLE hSecHandle,
                                   WE_INT32 iResult);

/*defied for tls*/
#ifdef M_SEC_CFG_TLS
void Sec_WimGetCipherSuitesResponse(WE_HANDLE hSecHandle,WE_INT32 iTargetID, WE_INT32 iResult, 
                                     WE_UCHAR *pucCipherSuites, 
                                     WE_INT32 iCipherSuitesLen);
#endif

#endif /* end of SEC_WIMRESP_H */
