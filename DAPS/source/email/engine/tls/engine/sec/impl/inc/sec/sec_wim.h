/*==================================================================================================
    FILE NAME : sec_wim.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the wim function prototype 
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                             Tracking
    Date              Author                Number      Description of changes
    ----------   ------------    ---------   --------------------------------------------------
    2006-07-16  tang                    None           create sec_wim.h
     
 =================================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_WIM_H
#define SEC_WIM_H

/***************************************************************************************************
*   macro Define Section
***************************************************************************************************/
#define M_SEC_SESSION_ID_LEN                32
#define M_SEC_PRIVATE_KEY_ID_LEN            4
#define M_SEC_ADDRESS_LEN                   18
#define M_SEC_WTLS_MASTER_SECRET_LEN        20

#define M_SEC_TLS_MASTER_SECRET_LEN         48

/* CA certs */
#define M_SEC_LOCAL              0
#define M_SEC_OUTER             1

/*Root Dir*/
#define M_SEC_FILE_PATH                      "/sec/"

/* session, peer and view information*/
#define M_SEC_SH_NAME                          "/sec/session"

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
typedef struct tagSt_SecTrustedKeyIds
{  
    WE_INT32   iTrustKeyIdsLen;
    WE_UCHAR *pucTrustedKeyIds;
    WE_INT32   iTooManyCerts;
}St_SecTrustedKeyIds;
  
/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
#ifndef M_SEC_CFG_NO_PLATFORM 
WE_INT32 Sec_WimCheckFileExist(WE_HANDLE hSecHandle,WE_CHAR *pcFileName);
WE_INT32 Sec_WimEncryptDataToFile(WE_HANDLE hSecHandle,
                                  WE_HANDLE hFileHandle, 
                                  void *pvData, 
                                  WE_INT32 iPos,
                                  WE_INT32 iDataLen,
                                  WE_INT32 *piWriteLength);
WE_INT32 Sec_WimDecryptDataFromFile(WE_HANDLE hSecHandle,
                                    WE_HANDLE hFileHandle, 
                                    WE_UINT8 *pucDecryptedData, 
                                    WE_INT32 iPos, 
                                    WE_INT32 iDataLen,
                                    WE_INT32 *piReaderLength);
#endif
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimChkWTLSCert(WE_HANDLE hSecHandle,
                            WE_INT32 iTargetID, 
                            St_SecPubKeyRsa stKey, 
                            const WE_UINT8 *pucCert);
#endif

WE_INT32 Sec_WimChkX509Cert(WE_HANDLE hSecHandle,WE_INT32 iTargetID, 
                               WE_UINT8 *pucPublicKeyVal, 
                               const WE_UINT8 *pucCert, 
                               WE_INT32 iConnectionType,
                               WE_INT32 iChainDepth,
                               WE_INT32 iCertUsage);
WE_INT32 Sec_WimVerifiCertChain(WE_HANDLE hSecHandle,
                                const WE_UINT8 *pucChain, 
                                WE_INT32 iChainLen, 
                                St_SecAsn1Certificate *pstCerts, 
                                WE_UINT8 *pucNumCerts);
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimGetX509Sub(WE_HANDLE hSecHandle,WE_UINT8 *pucX509Cert, 
                           WE_UINT8 **ppucSubject, WE_UINT16 *pusSubjectLen);
#endif
WE_INT32 Sec_WimGetX509PubKey(WE_HANDLE hSecHandle,WE_UINT8 *pucX509Cert,
                              WE_UINT16 *pusExpLen, WE_UINT8** ppucExponent,
                              WE_UINT16 *pusModLen,WE_UINT8** ppucModulus);
WE_INT32 Sec_WimVerifyHashedSign (WE_HANDLE  hSecHandle,
                                  WE_INT32 iHashAlg,
                                  WE_INT32 iTargetID,
                                  St_SecCrptPubKey stPubKey, 
                                  const WE_UINT8 *pucMsg,
                                  WE_INT32 iMsgLen, 
                                  const WE_UINT8 *pucSig, 
                                  WE_INT32 iSigLen);
WE_INT32 Sec_WimConvPubKey(St_SecPubKeyRsa stKey,
                           WE_UINT8 **ppucPubKey, WE_UINT16 *pusOutPubKeyLen);
WE_INT32 Sec_WimGetCACert(WE_HANDLE hSecHandle,WE_INT32 iIndex, WE_UINT8 **ppucCert,
                          WE_UINT16 *pusCertLen, WE_INT32 *piCertType, WE_CHAR cFlag, WE_HANDLE hCACerts);
#ifdef M_SEC_CFG_WTLS
void Sec_WimGetKeyExgIds(WE_HANDLE hSecHandle,
                         WE_UINT8 *pucKeyExchangeIds, WE_INT32 *piKeyExIdLen);

WE_INT32 Sec_WimGetWTLSMasterSecret( WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId,
                                    const WE_UINT8 *pucSecret, WE_INT32 iSecretLen, 
                                    const WE_CHAR *pcLabel, const WE_UINT8 *pucSeed,
                                    WE_INT32 iSeedLen,WE_INT32 iOutputLen, WE_UINT8 *pucBuf,WE_UINT8 ucHashAlg);
#endif
WE_INT32 Sec_WimGetNbrOfCACerts(WE_HANDLE hSecHandle, WE_INT32 *piCACertNum, 
                                WE_CHAR cFlag, WE_HANDLE hCACerts);

#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
WE_INT32  Sec_WimAddCACert(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                    WE_HANDLE hCACerts, WE_CHAR cFlag);
#endif
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimGetTrustedKeyIds(WE_HANDLE hSecHandle, 
                                    WE_HANDLE hCACerts, WE_HANDLE *phTrustKeyIds);
#endif

#ifdef M_SEC_CFG_TLS
WE_INT32 Sec_WimSSLPRF(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId, 
                       const WE_UCHAR *pucSecret, WE_INT32 iSecretLen, 
                       const WE_UCHAR *pucSeed, WE_INT32 iSeedLen, 
                       WE_INT32 iOutputLen, WE_UCHAR *pucBuf);
WE_INT32 Sec_WimTLSPRF(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId,
                      const WE_UCHAR *pucSecret, WE_INT32 iSecretLen, const WE_CHAR *pcLabel, 
                      const WE_UCHAR *pucSeed, WE_INT32 iSeedLen, 
                      WE_INT32 iOutputLen, WE_UCHAR *pucBuf);

#endif /*M_SEC_CFG_TLS*/
#endif /* end of SEC_WIM_H */



