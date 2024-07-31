/*==================================================================================================
    FILE NAME : sec_resp.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the response function prototype.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date             Author               Number         Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-06-29   Kevin Yang                            create sec_resp.h
    2007-05-17     tang                          Delete cert manage and signtext
==================================================================================================*/
/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef _SEC_RESP_H
#define _SEC_RESP_H

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
void Sec_SearchPeerResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                        WE_UINT8 ucConnectionType,WE_INT32 iMasterSecretID,
                        WE_INT32 iSecurityID);

void Sec_SessionGetResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                        WE_UINT8 ucSessionOptions, const WE_UCHAR * pucSessionId,
                        WE_UINT8 ucSessionIdLen, WE_UINT8 *ucCipherSuite,
                        WE_UINT8 ucCompressionAlg, const WE_UCHAR * pucPrivateKeyId,
                        WE_UINT32 uiCreationTime);
#ifdef M_SEC_CFG_WTLS
void Sec_GetCurSvrCertResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult);

void Sec_GetSessionInfoResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult);

void Sec_WtlsCurClassResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecClass,
                         WE_INT32 iInfoAvailable, WE_INT32 iCertAvailable);

void Sec_WtlsGetCipherSuiteResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                                const WE_UCHAR * pucCipherMethods, WE_INT32 iCipherMethodsLen,
                                const WE_UCHAR * pucKeyExchangeIds, WE_INT32 iKeyExchangeIdsLen,
                                const WE_UCHAR * pucTrustedKeyIds, WE_INT32 iTrustedKeyIdsLen);
#endif
void Sec_KeyExchangeResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                             WE_INT32 iMasterSecretId, const WE_UCHAR * pucPublicValue,
                             WE_INT32 iPublicValueLen);
void Sec_WtlsCompSigResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                         const WE_UCHAR * pucSignature, WE_INT32 iSignatureLen) ;

void Sec_GetPrfResultResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                              const WE_UCHAR * pucBuf, WE_INT32 iBufLen);

void Sec_VerifySvrCertResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult);

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
void Sec_CompSigResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                         const WE_UCHAR * pucSignature, WE_INT32 iSignatureLen);
#endif

#ifdef M_SEC_CFG_TLS
WE_VOID Sec_TlsGetCipherSuiteResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                                const WE_UCHAR * pucCipherSuites, WE_INT32 iCipherSuitesLen);
void Sec_SSLGetMasterSecretResp(WE_HANDLE hSecHandle,WE_INT32 iResult,WE_INT32 iTargetID, WE_UCHAR *pucMasterSecret);
#endif

WE_VOID Sec_ReleaseRespCB(WE_VOID* pvData);

#endif
