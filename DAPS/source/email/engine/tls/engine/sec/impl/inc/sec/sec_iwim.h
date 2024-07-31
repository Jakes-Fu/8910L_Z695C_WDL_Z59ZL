/*==================================================================================================
    FILE NAME : sec_iwim.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file, define the initial function prototype for WIM module, and will be updated later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                           Tracking
    Date            Author                Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-19    Stone An                              create sec_iwim.h
    2007-05-17     tang                          Delete cert manage and signtext
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef SEC_IWIM_H
#define SEC_IWIM_H

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define M_SEC_SP_RSA              M_SEC_RSA_SIGN_CERT

#define M_SEC_HAVEREAD_SESSIONPEER    0x0002
#define M_SEC_HAVEREAD_VIEWINFO         0x0004

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
void Sec_iWimInit(WE_HANDLE hSecHandle);
void Sec_iWimEnd(WE_HANDLE hSecHandle);
void Sec_iWimGetPrfResult(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                              WE_INT32 iMasterSecretId, const WE_UCHAR *pucSecret, 
                              WE_INT32 iSecretLen, const WE_CHAR *pcLabel,
                              const WE_UCHAR *pucSeed, WE_INT32 iSeedLen,
                              WE_INT32 iOutputLen, WE_UINT8 ucConnectionType,WE_UINT8 ucAlg);
void Sec_iWimVerifyCertInterResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement, 
                                 St_SecCrptVerifySignatureResp *pstParam);
void Sec_iWimVerifyCertRootResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement,
                                St_SecCrptVerifySignatureResp *pstParam);
#ifdef M_SEC_CFG_WTLS
void  Sec_iWimWtlsVerifySvrCertChain(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                     const WE_UCHAR *pucBuf, WE_INT32 iBufLen);
#endif

#ifndef M_SEC_CFG_NO_PLATFORM 
void Sec_iWimReadView(WE_HANDLE hSecHandle, WE_UCHAR **ppucData, WE_INT32 *piDataLen);
void Sec_iWimStoreView(WE_HANDLE hSecHandle, const WE_UCHAR *pucData, 
                                            WE_INT32 iDataLen);
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_iWimCalSign(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                  const WE_UCHAR *pucKeyExId, WE_INT32 iKeyExIdLen,
                                  const WE_UCHAR *pucBuf, WE_INT32 iBufLen, WE_UINT8 ucAlg) ;

WE_VOID Sec_iWimWtlsCalSignResp(WE_HANDLE hSecHandle, 
                St_SecWimElement *pstElement, St_SecCrptComputeSignatureResp *pstParam) ;
#endif

#ifdef M_SEC_CFG_WTLS
void Sec_iWimWtlsGetCipherSuite(WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                            WE_UINT8 ucServerAuth, WE_HANDLE hTrustKeyIds);

void Sec_iWimWtlsKeyExchange(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                             St_SecWtlsKeyExchParams stParam, const WE_UCHAR *pucRandVal,WE_UINT8 ucHashAlg);

void Sec_iWimWtlsKeyExRsaResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement, 
                              St_SecCrptEncryptPkcResp *pstParam);
#endif
void Sec_WimFreeRsaElement(WE_HANDLE hSecHandle, St_SecWimElement* pstElement);

#ifdef M_SEC_CFG_TLS
WE_VOID Sec_iWimTlsKeyExRsaResp (WE_HANDLE hSecHandle,St_SecWimElement *pstElement, 
                                    St_SecCrptEncryptPkcResp *pstParam);

void Sec_iWimTLSkeyExchange(WE_HANDLE hSecHandle,WE_INT32 iTargetID, WE_UINT8 ucAlg,WE_INT32 iIsSsl,
                           St_SecTlsKeyExchangeParams *stParam, const WE_UCHAR *pucRandval);
WE_VOID Sec_iWimGetMasterSecret (WE_HANDLE hSecHandle,WE_HANDLE hHandle, WE_INT32 iMasterSecretID);
void Sec_iWimTlsGetCipherSuites(WE_HANDLE hSecHandle,WE_INT32 iTargetID);
void  Sec_iWimVerifyTLSCertificateChain(WE_HANDLE hSecHandle,WE_INT32 iTargetID, St_SecTlsAsn1Certificate *pstCerts, 
                                       WE_INT32 iNumCerts);
#endif
#endif/*endif SEC_IWIM_H*/


/*--------------------------END-----------------------------*/
