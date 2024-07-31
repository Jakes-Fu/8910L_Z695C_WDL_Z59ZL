/*==================================================================================================
    FILE NAME : iSecW.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        In this file,define the interface function prototype ,and the related structure 
        used in these interfaces.This is a public head file of sec module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date             Author               Number         Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang                             create isecw.h
    2007-05-17     tang                                 Delete cert manage and signtext
==================================================================================================*/

/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/

#ifndef _ISECW_H_
#define _ISECW_H_

/*******************************************************************************
*   macro Define Section
*******************************************************************************/
typedef struct _ISecW ISecW;
#define G_SEC_WTLS_CONNECTION_TYPE        0x00
#define G_SEC_TLS_CONNECTION_TYPE         0x01
#define G_SEC_SSL_CONNECTION_TYPE         0x02

/* broswer pass the type of cert. */
#define M_SEC_IWAPOPT_WTLS_ROOT_CERTS         ((WE_INT32)0x00013053)
#define M_SEC_IWAPOPT_X509_ROOT_CERTS          ((WE_INT32)0x00052001)

/* Options for key exchange */
#define G_SEC_TLS_OPTION_IS_SSL                         1
#define G_SEC_TLS_OPTION_SSL_CLIENT_HELLO               2
/***********************************************************************
*   Response Event
************************************************************************/
#define   G_SEARCHPEER_RESP             0x00
#define   G_SESSIONGET_RESP             0x01 
#define   G_VERIFYSVRCERT_RESP          0x02 
#define   G_GETPRF_RESP                 0x03 
#define   G_COMPUTESIGN_RESP            0x05 
#define   G_GETCIPHERSUITE_RESP         0x06 
#define   G_KEYEXCH_RESP                0x07 
/*add by tang070320*/
#define   G_TLS_GETCIPHERSUITE_RESP     0x09 
#define   G_TLS_GETMASTERSECRET_RESP    0x0a 



/***************************************************************************
*   The result value from response
***************************************************************************/
#define G_SEC_NOTFOUND                      0x11
#define G_SEC_INVALID_COMMON_NAME           0x12
#define G_SEC_MISSING_AUTKEYHANDSHAKE       0x13
#define G_SEC_DEGRADED_TO_ANON              0x14
/*---------------------------------------------------
 * All the cipher suites for tls
 *---------------------------------------------------*/
#ifdef M_SEC_CFG_TLS

#define G_SEC_TLS_NULL_WITH_NULL_NULL                   0x0000
#define G_SEC_TLS_RSA_WITH_NULL_MD5                     0x0001
#define G_SEC_TLS_RSA_WITH_NULL_SHA                     0x0002
#define G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5            0x0003
#define G_SEC_TLS_RSA_WITH_RC4_128_MD5                  0x0004
#define G_SEC_TLS_RSA_WITH_RC4_128_SHA                  0x0005
#define G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5        0x0006
#define G_SEC_TLS_RSA_WITH_IDEA_CBC_SHA                 0x0007
#define G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA         0x0008
#define G_SEC_TLS_RSA_WITH_DES_CBC_SHA                  0x0009
#define G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA             0x000A
#define G_SEC_TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA      0x000B
#define G_SEC_TLS_DH_DSS_WITH_DES_CBC_SHA               0x000C
#define G_SEC_TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA          0x000D
#define G_SEC_TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA      0x000E
#define G_SEC_TLS_DH_RSA_WITH_DES_CBC_SHA               0x000F
#define G_SEC_TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA          0x0010
#define G_SEC_TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA     0x0011
#define G_SEC_TLS_DHE_DSS_WITH_DES_CBC_SHA              0x0012
#define G_SEC_TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA         0x0013
#define G_SEC_TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA     0x0014
#define G_SEC_TLS_DHE_RSA_WITH_DES_CBC_SHA              0x0015
#define G_SEC_TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA         0x0016
#define G_SEC_TLS_DH_ANON_EXPORT_WITH_RC4_40_MD5        0x0017
#define G_SEC_TLS_DH_ANON_WITH_RC4_128_MD5              0x0018
#define G_SEC_TLS_DH_ANON_EXPORT_WITH_DES40_CBC_SHA     0x0019
#define G_SEC_TLS_DH_ANON_WITH_DES_CBC_SHA              0x001A
#define G_SEC_TLS_DH_ANON_WITH_3DES_EDE_CBC_SHA         0x001B
#endif
/************************************************************************
*   The key exchange algorithms defined by WTLS                                                     *
************************************************************************/
#define G_SEC_KEYEXCH_NULL                  0x00
#define G_SEC_KEYEXCH_SHARED_SECRET         0x01
#define G_SEC_KEYEXCH_DH_ANON               0x02
#define G_SEC_KEYEXCH_DH_ANON_512           0x03
#define G_SEC_KEYEXCH_DH_ANON_768           0x04
#define G_SEC_KEYEXCH_RSA_ANON              0x05
#define G_SEC_KEYEXCH_RSA_ANON_512          0x06
#define G_SEC_KEYEXCH_RSA_ANON_768          0x07
#define G_SEC_KEYEXCH_RSA                   0x08
#define G_SEC_KEYEXCH_RSA_512               0x09
#define G_SEC_KEYEXCH_RSA_768               0x0a
#define G_SEC_KEYEXCH_ECDH_ANON             0x0b
#define G_SEC_KEYEXCH_ECDH_ANON_113         0x0c
#define G_SEC_KEYEXCH_ECDH_ANON_131         0x0d
#define G_SEC_KEYEXCH_ECDH_ECDSA            0x0e

/************************************************************************
*   The key exchange algorithms defined by TLS                                                     *
************************************************************************/
#ifdef M_SEC_CFG_TLS
#define    G_SEC_TLS_KEYEXCH_NULL                0x00                     
#define    G_SEC_TLS_KEYEXCH_RSA                 0x01  
#define    G_SEC_TLS_KEYEXCH_RSA_EXPORT          0x02
#define    G_SEC_TLS_KEYEXCH_DH_DSS_EXPORT       0x03
#define    G_SEC_TLS_KEYEXCH_DH_DSS              0x04   
#define    G_SEC_TLS_KEYEXCH_DH_RSA_EXPORT       0x05
#define    G_SEC_TLS_KEYEXCH_DH_RSA              0x06
#define    G_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT      0x07
#define    G_SEC_TLS_KEYEXCH_DHE_DSS             0x08  
#define    G_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT      0x09
#define    G_SEC_TLS_KEYEXCH_DHE_RSA             0x0a 
#define    G_SEC_TLS_KEYEXCH_DH_ANON_EXPORT      0x0b
#define    G_SEC_TLS_KEYEXCH_DH_ANON             0x0c 
#endif
/************************************************************************
* The cipher algorithms macro defined for WTLS and tls                                                          *
************************************************************************/
#define G_SEC_ALG_CIPHER_NULL               0x00        
#define G_SEC_ALG_CIPHER_RC5_CBC_40         0x01  
#define G_SEC_ALG_CIPHER_RC5_CBC_56         0x02  
#define G_SEC_ALG_CIPHER_RC5_CBC            0x03     
#define G_SEC_ALG_CIPHER_DES_CBC_40         0x04  
#define G_SEC_ALG_CIPHER_DES_CBC            0x05     
#define G_SEC_ALG_CIPHER_3DES_CBC_EDE       0x06
#define G_SEC_ALG_CIPHER_IDEA_CBC_40        0x07 
#define G_SEC_ALG_CIPHER_IDEA_CBC_56        0x08 
#define G_SEC_ALG_CIPHER_IDEA_CBC           0x09    
#define G_SEC_ALG_CIPHER_RC5_CBC_64         0x0A  
#define G_SEC_ALG_CIPHER_IDEA_CBC_64        0x0B 
#define G_SEC_ALG_CIPHER_RC2_CBC_40         0x14  
#define G_SEC_ALG_CIPHER_RC4_40             0x15      
#define G_SEC_ALG_CIPHER_RC4_128            0x16  

/************************************************************************
*   Hash algorithm macro                                                                                        *
************************************************************************/
#define G_SEC_HASH_NULL                     0x00
#define G_SEC_HASH_MD5                      0x01 
#define G_SEC_HASH_SHA                      0x02 
#define G_SEC_HASH_MD2                      0x03

/************************************************************************
*   Session options                                                                                                 *
************************************************************************/
#define G_SEC_SESSION_OPTIONS_RESUMABLE     0x80
#define G_SEC_SESSION_OPTIONS_SERVER_AUTH   0x20
#define G_SEC_SESSION_OPTIONS_CLIENT_AUTH   0x10

/* ca certs have no change. */
#define G_SEC_CA_NO_CHANGE              0x00
#define G_SEC_CA_CHANGE                     0x01


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
/************************************************
*   structure used in i interface for wtls                          *
************************************************/
typedef struct tagSt_SecSessionInformation
{
    WE_UINT32     uiCertLen;
    WE_UCHAR  *   pucCert;
    WE_UINT16     usKeyExchangeKeyLen;
    WE_UINT16     usHmacLen;
    WE_UINT16     usEncryptionKeyLen;
    WE_UINT8      ucSecurityClass;
    WE_UINT8      ucKeyExchangeAlg;
    WE_UINT8      ucHmacAlg;
    WE_UINT8      ucEncryptionAlg;
    WE_UINT8      ucNumCerts;
    WE_UINT8      ucCertFormat;
}St_SecSessionInformation;

typedef struct tagSt_SecCrptPubKeyRsa
{
    WE_UCHAR * pucExponent;
    WE_UCHAR * pucModulus;  
    WE_UINT16  usExpLen;
    WE_UINT16  usModLen;
}St_SecCrptPubKeyRsa;
typedef St_SecCrptPubKeyRsa St_SecPubKeyRsa;

typedef union tagSt_SecWtlsPublicKeyRsa
{
    St_SecPubKeyRsa     stRsaKey;
}St_SecWtlsPublicKeyRsa;

typedef struct tagSt_SecWtlsPublicKey
{
    St_SecWtlsPublicKeyRsa stPubKey;    
}St_SecWtlsPublicKey;

typedef struct tagSt_SecWtlsParamSpecifier
{
    WE_UCHAR * pucParams;
    WE_UINT16  usParamLen;
    WE_UINT8   ucParameterIndex;
}St_SecWtlsParamSpecifier;

typedef struct tagSt_SecSecretKey
{
    WE_UCHAR * pucIdentifier;
    WE_UINT16  usIdLen;
}St_SecSecretKey;

typedef struct tagSt_SecKeyParam
{
    St_SecWtlsPublicKey      stPubKey;
    St_SecWtlsParamSpecifier stParamSpecifier;
}St_SecKeyParam;

typedef struct tagSt_SecCertificates
{
    WE_UCHAR * pucBuf;
    WE_UINT16  usBufLen;
}St_SecCertificates;

#ifdef M_SEC_CFG_WTLS
typedef union tagSt_SecKeyExchParams
{
    St_SecKeyParam     stKeyParam;
    St_SecCertificates stCertificates;
    St_SecSecretKey    stSecretKey;
}St_SecKeyExchParams;

typedef struct tagSt_SecWtlsKeyExchParams
{
    St_SecKeyExchParams stParams;
    WE_UINT8            ucKeyExchangeSuite;
}St_SecWtlsKeyExchParams;

typedef struct tagSt_SecWtlsVerifyCertChainParam
{
  WE_UCHAR     *pucBuf;    
  WE_INT32     iBufLen;
  WE_UCHAR     *pucAddr;
  WE_UCHAR     *pucDomain;
  WE_INT32     iAddrLen;  
  WE_UINT16    usDomainLen;
} St_SecWtlsVerifyCertChainParam;
#endif
/****************************************
*        structure used in i interface for tls                 *
********************************************/
#ifdef M_SEC_CFG_TLS
typedef struct tagSt_SecTlsDistingubshedName
{
   WE_UCHAR       *pucName;  
   WE_UINT16      usNameLen;
   WE_UINT16      usReserved;
} St_SecTlsDistingubshedName;

typedef struct tagSt_SecTlsAsn1Certificate
{
  WE_UINT32       uiCertLen;
  WE_UCHAR        *pucCert;
  WE_UINT8        ucFormat;
  WE_UINT8        ucReserved1;
  WE_UINT16       usReserved2;
} St_SecTlsAsn1Certificate;

typedef struct tagSt_SecTlsVerifyCertChainParam
{
  WE_UCHAR      *pucAddr;
  WE_UCHAR      *pucDomain;
  St_SecTlsAsn1Certificate *pstCerts;
  WE_UINT16     usAddrLen;
  WE_UINT16     usDomainLen;
  WE_UINT8      ucNumCerts;
  WE_UINT8      ucReserved1;
  WE_UINT16     usReserved2;
} St_SecTlsVerifyCertChainParam;

/*for St_SecTlsKeyExchangeParams*/
typedef struct tagSt_SecTlsSignature
{
   WE_UINT8      *pucSignature;
   WE_UINT16     usSignLen;
   WE_UINT16     usReserved;
}St_SecTlsSignature;

typedef struct tagSt_SecTlsSeverRSAParams
{
   WE_UINT8     *pucModulus;
   WE_UINT8     *pucExponent;
   WE_UINT16    usModLen;
   WE_UINT16    usExpLen;
}St_SecTlsSeverRSAParams;

typedef struct tagSt_SecTlsRSAParam
{
   St_SecTlsSeverRSAParams stTlsRSAParam;
   St_SecTlsSignature      stTlsSignature;
}St_SecTlsRSAParam;

typedef struct tagSt_SecTlsSeverDHParams
{
   WE_UINT8    *pucDHP;
   WE_UINT8    *pucDHG;
   WE_UINT8    *pucDHYs;
   WE_UINT16   usDHPLen;
   WE_UINT16   usDHGLen;
   WE_UINT16   usDHYsLen;
   WE_UINT16   usReserved1;
}St_SecTlsSeverDHParams;

typedef struct tagSt_SecTlsDHParam
{
    St_SecTlsSeverDHParams stTlsDHParam;
    St_SecTlsSignature     stTlsSignature;
}St_SecTlsDHParam;

typedef struct tagSt_SecTlsKeyExchangeParams
{
    St_SecTlsRSAParam       stRSAParam;
    St_SecTlsDHParam        stDHParam;
    St_SecCertificates      stSecCert;
    WE_INT32                iOptions;    
}St_SecTlsKeyExchangeParams;

#endif

typedef struct tagSt_SecCACert
{  
    WE_INT32   iCertType;
    WE_INT32   iCertLen;
    WE_UCHAR *pucCert;
    struct tagSt_SecCACert *pstNext;
}St_SecCACert;


/************************************************
*   structure used in response                                          *
************************************************/
typedef struct tagSt_SecGetCipherSuiteResp
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucCipherMethods;
    WE_UCHAR * pucKeyExchangeIds;
    WE_UCHAR * pucTrustedKeyIds;
    WE_UINT16  usResult;
    WE_UINT16  usCipherMethodsLen;
    WE_UINT16  usKeyExchangeIdsLen;
    WE_UINT16  usTrustedKeyIdsLen;
}St_SecGetCipherSuiteResp;

typedef struct tagSt_SecKeyExchResp
{
    WE_INT32   iTargetID;    
    WE_UCHAR * pucPreMsKey;
    WE_UINT16  usResult;
    WE_UINT16  usPreMsKeyLen;
    WE_UINT8   ucMasterSecretId;
}St_SecKeyExchResp;

typedef struct tagSt_SecGetPrfResp
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucBuf;
    WE_UINT16  usResult;
    WE_UINT16  usBufLen;
}St_SecGetPrfResp;

typedef struct tagSt_SecVerifySvrCertResp
{
    WE_INT32 iTargetID;    
    WE_UINT16 usResult;
}St_SecVerifySvrCertResp;

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
typedef struct tagSt_SecCompSignResp
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucSignature;
    WE_UINT16  usResult;
    WE_UINT16  usSignatureLen;
}St_SecCompSignResp;
#endif

typedef struct tagSt_SecSearchPeerResp
{
    WE_INT32  iTargetID;    
    WE_INT32  iSecId;
    WE_UINT16 usResult;
    WE_UINT8  ucConnectionType;
    WE_UINT8  ucMasterSecretId;
}St_SecSearchPeerResp;

typedef struct tagSt_SecSessionGetResp
{
    WE_INT32   iTargetID;    
    WE_UCHAR * pucSessionId;
    WE_UCHAR * pucPrivateKeyId;
    WE_UINT32  uiCreationTime;
    WE_UINT8   aucCipherSuite[2];
    WE_UINT16  usResult;
    WE_UINT16  usSessionIdLen;
    WE_UINT16  usPrivateKeyIdLen; 
    WE_UINT8   ucSessionOptions;
    WE_UINT8   ucCompressionAlg;
}St_SecSessionGetResp;

#ifdef M_SEC_CFG_TLS
typedef struct  tagSt_SecTlsGetCipherSuitesResp
{
   WE_INT32     iTargetID; 
   WE_UCHAR     *pucCipherSuites;
   WE_UINT16    usCipherSuitesLen;
   WE_UINT16    usResult;
}St_SecTlsGetCipherSuitesResp;

typedef struct tagSt_SecSSLGetMasterSecretResp
{
    WE_INT32    iTargetID;  
    WE_UCHAR    *pucMasterSecret ;
    WE_INT32    iMasterSecretLen ;
    WE_UINT16   usResult;
}St_SecSSLGetMasterSecretResp;

#endif
/*add for linux without cert manage*/
/* type of connection operation */

typedef enum tagE_SecConnectType
{             
    G_SEC_SETUP_CONNECTION, 
    G_SEC_STOP_CONNECTION 
}E_SecConnectType;

/* parameters in connection */
typedef struct tagSt_SecSetupConnectPara
{
    WE_INT32 iTargetID;
    WE_INT32 iMasterSecretId;
    WE_INT32 iSecId;
    WE_INT32 iFullHandshake;
    St_SecSessionInformation *pstSessionInfo;
} St_SecSetupConnectPara;

typedef struct tagSt_SecStopConnectPara
{
    WE_INT32 iTargetID;
    WE_INT32 iSecId;
} St_SecStopConnectPara;

typedef struct tagSt_SecConnectOpt
{
   E_SecConnectType   eType; 
   WE_HANDLE             hParaValue;
} St_SecConnectOpt;

/* type of peer operation */
typedef enum tagE_SecPeerType
{ 
    G_SEC_REMOVE_PEER, 
    G_SEC_ATTACH_PEER,
    G_SEC_SEARCH_PEER
}E_SecPeerType;

/* parameters in peer */
typedef struct tagSt_SecRemovePeerPara
{
    WE_INT32 iMasterSecretId;
} St_SecRemovePeerPara;

typedef struct tagSt_SecAttachPeerPara
{
    WE_UCHAR  *pucAddress;
    WE_INT32    iAddressLen;
    WE_UINT16  usPortnum;
    WE_INT32    iMasterSecretId;
} St_SecAttachPeerPara;

typedef struct tagSt_SecSearchPeerPara
{
    WE_INT32   iTargetID;
    WE_UCHAR *pucAddress;
    WE_INT32   iAddressLen;
    WE_UINT16 usPortNum;
} St_SecSearchPeerPara;

typedef struct tagSt_SecPeerOpt
{
   E_SecPeerType        eType; 
   WE_HANDLE             hValue;
} St_SecPeerOpt;

/* type of session operation */
typedef enum tagE_SecSessionType
{ 
    G_SEC_ENABLE_SESSION, 
    G_SEC_DISABLE_SESSION,
    G_SEC_GET_SESSION,
    G_SEC_RENEW_SESSION
}E_SecSessionType;

/* parameters in session */
typedef struct tagSt_SecEnableSessionPara
{
    WE_INT32 iMasterSecretId;
    WE_UINT8 ucIsActive;
} St_SecEnableSessionPara;

typedef struct tagSt_SecDisableSessionPara
{
    WE_INT32 iMasterSecretId;
} St_SecDisableSessionPara;

typedef struct tagSt_SecSessionGetPara
{
    WE_INT32 iTargetID;
    WE_INT32 iMasterSecretId;
} St_SecSessionGetPara;

typedef struct tagSt_SecSessionRenewPara
{
    WE_INT32 iMasterSecretId;
    WE_UINT8 ucSessionOptions;
    WE_UCHAR * pucSessionId;
    WE_UINT8 ucSessionIdLen;
    WE_UINT8 *pucCipherSuite;
    WE_UINT8 ucCompressionAlg;
    WE_UCHAR * pucPrivateKeyId;
    WE_UINT32 uiCreationTime;
} St_SecSessionRenewPara;

typedef struct tagSt_SecSessionOpt
{
   E_SecSessionType    eType; 
   WE_HANDLE             hValue;
} St_SecSessionOpt;

/************************************************
*   ISecW interface definition                  *
************************************************/
#ifdef M_SEC_CFG_TLS
WE_INT32 ISecW_SslHashUpdateWMasterSec(WE_HANDLE pMe, WE_HANDLE hHandle,WE_INT32 iMasterSecretID);
#endif

WE_UINT32 ISecW_ProcAsyAlgorithm(WE_HANDLE pMe, WE_INT32 iTargetID, WE_VOID *pvParam,
                           WE_UINT8 ucAlg,WE_UCHAR * pucRandval,WE_UINT8 ucConnType);
WE_UINT32 ISecW_MakeCipherSuite (WE_HANDLE pMe, WE_INT32 iTargetID,WE_UINT8   ucConnType ,WE_HANDLE hCACerts);
WE_UINT32 ISecW_ComputePrf(WE_HANDLE pMe, WE_INT32 iTargetID, WE_UINT8 ucAlg, 
                            WE_INT32 iMasterSecretId,  WE_UCHAR * pucSecret, 
                            WE_INT32 iSecretLen,  WE_CHAR * pcLabel, 
                            WE_UCHAR * pucSeed, WE_INT32 iSeedLen,
                            WE_INT32 iOutputLen,WE_UINT8 ucConnType);

WE_UINT32 ISecW_ValidateSvrCert(WE_HANDLE pMe, WE_INT32 iTargetID,
                                          WE_VOID *pvBuf,  WE_INT32 iBufLen,
                                          WE_HANDLE hCACerts, WE_CHAR cFlag ,
                                          WE_UINT8 ucConnType) ;

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_UINT32 ISecW_CompSign(WE_HANDLE pMe, WE_INT32 iTargetID,
                               WE_UCHAR * pucKeyId, WE_INT32 iKeyIdLen,
                               WE_UCHAR * pucBuf, WE_INT32 iBufLen) ;
#endif

WE_UINT32 ISecW_ConnectionOpts(WE_HANDLE pMe, St_SecConnectOpt *pstConnectOpt ,WE_UINT8 ucConnType) ;
WE_UINT32 ISecW_PeerOpts(WE_HANDLE pMe, St_SecPeerOpt *pstPeerOpt ,WE_UINT8   ucConnType) ;
WE_UINT32 ISecW_SessionOpts(WE_HANDLE pMe, St_SecSessionOpt *pstSessionOpt );

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
WE_UINT32 ISecW_EvtConfirmAction(WE_HANDLE pMe,WE_INT32 iTargetID,St_ConfirmAction stConfirm);
WE_UINT32 ISecW_EvtShowDlgAction(WE_HANDLE pMe,WE_INT32 iTargetId,
                                WE_VOID* pvSrcData,WE_UINT32 uiLength);
#endif

#endif

