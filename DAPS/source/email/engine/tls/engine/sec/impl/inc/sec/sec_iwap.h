/*==================================================================================================
    FILE NAME : sec_iwap.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the function prototype ,to support the wtls stack.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                           Tracking
    Date             Author               Number         Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-06-29   Kevin Yang                            create sec_iwap.h
    2007-05-17     tang                          Delete cert manage and signtext
    2007-06-19    bluefire                         four bytes align for dragonfly
==================================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef _SEC_IWAP_H
#define _SEC_IWAP_H

/***************************************************************************************************
*   macro Define Section
***************************************************************************************************/
/* Bulk encryption alg */
typedef E_WeCipherAlgType      E_SecCipherAlgType;
typedef E_WeHashAlgType        E_SecHashAlgType;
typedef E_WeHmacAlgType        E_SecHmacAlgType;
#define E_SEC_ALG_HASH_NULL    E_WE_ALG_HASH_NULL
#define E_SEC_ALG_HASH_MD5     E_WE_ALG_HASH_MD5 
#define E_SEC_ALG_HASH_SHA1    E_WE_ALG_HASH_SHA1
#define E_SEC_ALG_HASH_MD2     E_WE_ALG_HASH_MD2

#define M_SEC_RSA_SIGN_CERT          0x01
#define M_SEC_DSS_SIGN_CERT          0x02



/* the type of connection , internal used. */
#define M_SEC_WTLS_CONNECTION_TYPE                 G_SEC_WTLS_CONNECTION_TYPE   
#define M_SEC_TLS_CONNECTION_TYPE                  G_SEC_TLS_CONNECTION_TYPE        
#define M_SEC_SSL_CONNECTION_TYPE                  G_SEC_SSL_CONNECTION_TYPE

#ifdef M_SEC_CFG_TLS

/*the version of tls protocol*/
#define M_SEC_TLS_PROTOCOL_VERSION_MAJOR                3
#define M_SEC_TLS_PROTOCOL_VERSION_MINOR                1
#define M_SEC_SSL_PROTOCOL_VERSION_MINOR                0

#endif

/* Function return values , internal used. */
#define M_SEC_ERR_OK                               G_SEC_OK            
#define M_SEC_ERR_GENERAL_ERROR                    G_SEC_GENERAL_ERROR         
#define M_SEC_ERR_UNSUPPORTED_METHOD               G_SEC_UNSUPPORTED_METHOD       
#define M_SEC_ERR_INSUFFICIENT_MEMORY              G_SEC_NOTENOUGH_MEMORY      
#define M_SEC_ERR_NOT_IMPLEMENTED                  G_SEC_NOT_SUPPORT          
#define M_SEC_ERR_INVALID_PARAMETER                G_SEC_INVALID_PARAMETER        
#define M_SEC_ERR_DATA_LENGTH                      G_SEC_DATA_LENGTH              
#define M_SEC_ERR_INVALID_KEY                      G_SEC_INVALID_KEY              

#define M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE         G_SEC_UNKNOWN_CERTTYPE
#define M_SEC_ERR_NO_MATCHING_ROOT_CERTIFICATE     G_SEC_NOMATCH_ROOTCERT
#define M_SEC_ERR_BAD_CERTIFICATE                  G_SEC_INVALIDATE_CERT
#define M_SEC_ERR_CERTIFICATE_EXPIRED              G_SEC_CERTEXPIRED
#define M_SEC_ERR_NOT_FOUND                        G_SEC_NOTFOUND
#define M_SEC_ERR_INVALID_COMMON_NAME              G_SEC_INVALID_COMMON_NAME

#define M_SEC_ERR_DOWNGRADED_TO_ANON               G_SEC_DEGRADED_TO_ANON

#define M_SEC_ERR_BUFFER_TOO_SMALL                 (0x20)
/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
typedef enum tagE_BulkEncryAlg
{
    E_SEC_BULK_NULL = 0x00,
    E_SEC_BULK_RC5_CBC_40,
    E_SEC_BULK_RC5_CBC_56,
    E_SEC_BULK_RC5_CBC,
    E_SEC_BULK_DES_CBC_40,
    E_SEC_BULK_DES_CBC,
    E_SEC_BULK_3DES_CBC_EDE,
    E_SEC_BULK_IDEA_CBC_40,
    E_SEC_BULK_IDEA_CBC_56,
    E_SEC_BULK_IDEA_CBC,
    E_SEC_BULK_RC5_CBC_64,
    E_SEC_BULK_IDEA_CBC_64,
    E_SEC_BULK_RC4_40 = 0x15,
    E_SEC_BULK_RC4_128 = 0x17,
    E_SEC_BULK_AES_CBC_128 = 0x1e,
    E_SEC_BULK_DEFAULT = 0xFFFFFFFF /* four bytes align */
}E_BulkEncryAlg;

#ifdef M_SEC_CFG_WTLS
/* WTLS Key exchange alg */
typedef enum tagE_WTLSKeyExchMethods
{
    E_SEC_WTLS_KEYEXCHANGE_NULL = 0x00,
    E_SEC_WTLS_KEYEXCHANGE_SHARED_SECRET,
    E_SEC_WTLS_KEYEXCHANGE_DH_ANON,
    E_SEC_WTLS_KEYEXCHANGE_DH_ANON_512,
    E_SEC_WTLS_KEYEXCHANGE_DH_ANON_768,
    E_SEC_WTLS_KEYEXCHANGE_RSA_ANON,
    E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512,
    E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768,
    E_SEC_WTLS_KEYEXCHANGE_RSA,
    E_SEC_WTLS_KEYEXCHANGE_RSA_512,
    E_SEC_WTLS_KEYEXCHANGE_RSA_768,
    E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON,
    E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON_113,
    E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON_131,
    E_SEC_WTLS_KEYEXCHANGE_ECDH_ECDSA,
    E_SEC_WTLS_KEYEXCHANGE_DEFAULT = 0xFFFFFFFF /* four bytes align */
}E_WTLSKeyExchMethods;
#endif

#ifdef M_SEC_CFG_TLS
/* TLS Key exchange alg */
typedef enum tagE_TLSKeyExchMethods
{
   E_SEC_TLS_KEYEXCH_NULL = 0x00,                     
   E_SEC_TLS_KEYEXCH_RSA ,  
   E_SEC_TLS_KEYEXCH_RSA_EXPORT ,  
   E_SEC_TLS_KEYEXCH_DH_DSS_EXPORT,
   E_SEC_TLS_KEYEXCH_DH_DSS, 
   E_SEC_TLS_KEYEXCH_DH_RSA_EXPORT , 
   E_SEC_TLS_KEYEXCH_DH_RSA  ,   
   E_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT   ,   
   E_SEC_TLS_KEYEXCH_DHE_DSS     ,
   E_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT , 
   E_SEC_TLS_KEYEXCH_DHE_RSA ,
   E_SEC_TLS_KEYEXCH_DH_ANON_EXPORT , 
   E_SEC_TLS_KEYEXCH_DH_ANON,
   E_SEC_TLS_KEYEXCH_DEFAULT = 0xFFFFFFFF /* four bytes align */
}E_TLSKeyExchMethods;

#endif
/* SEC Message Used in The Module */
typedef enum tagE_SecModuleMsg
{
    E_SEC_MSG_WTLSGETCIPHERSUITE = 0x01,
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
    E_SEC_MSG_COMPSIG,
#endif
    E_SEC_MSG_WTLSVERIFYSVRCERT,
    E_SEC_MSG_WTLSGETPRFRESULT,
    E_SEC_MSG_WTLSKEYEXCHANGE,
    E_SEC_MSG_SETUPCON,
    E_SEC_MSG_STOPCON,
    E_SEC_MSG_UPSESSION,
    E_SEC_MSG_GETSESINFO,
    E_SEC_MSG_DISSESSION,
    E_SEC_MSG_DELPEER,
    E_SEC_MSG_ATTACHPEER,
    E_SEC_MSG_SEARCHPEER,
    E_SEC_MSG_ENVSESSION,
    E_SEC_MSG_VIEW_CURCERT,
    E_SEC_MSG_VIEW_INFOSESSION,
    E_SEC_MSG_GET_CURCLASS,
    E_SEC_MSG_ENCRYPT_PKC_RESP,
    E_SEC_MSG_VERIFY_SIGN_RESP,    
    E_SEC_MSG_COMPUTE_SIGN_RESP,
 #ifdef M_SEC_CFG_TLS
    E_SEC_MSG_TLSGETCIPHERSUITE,
    E_SEC_MSG_TLSVERIFYCERTCHAIN,
    E_SEC_MSG_TLSKEYEXCHANGE,
    E_SEC_MSG_TLSGETPRFRESULT,
    E_SEC_MSG_DEFAULT = 0xFFFFFFFF /* four bytes align */
 #endif
}E_SecModuleMsg;

typedef St_WeCipherCrptKey     St_SecCipherCrptKey;

typedef struct tagSt_SecAsn1Certificate
{    
    WE_UINT32 uiCertLen;
    WE_UCHAR * pucCert;
    WE_UINT8 ucFormat;
    WE_UINT8 aucReserved[3]; /* four bytes align */
}St_SecAsn1Certificate;

typedef struct tagSt_SecSessionInfo
{
    St_SecAsn1Certificate * pstCerts;
    WE_UINT16     usHmacLen;
    WE_UINT16     usEncryptionKeyLen;
    WE_UINT16     usKeyExchangeKeyLen;
    WE_UINT8      ucHmacAlg;
    WE_UINT8      ucEncryptionAlg;
    WE_UINT8      ucConnectionType;
    WE_UINT8      ucSecurityClass;
    WE_UINT8      ucKeyExchangeAlg;
    WE_UINT8      ucNumCerts;
}St_SecSessionInfo;

typedef struct tagSt_SecSetupConnection
{
    WE_INT32 iTargetID;
    WE_INT32 iSecurity_id;
    St_SecSessionInfo stSessionInfo;
    WE_UINT8 ucFull_handshake;
    WE_UINT8 ucMasterSecretId;
    WE_UINT8 aucReserved[2]; /* four bytes align */
    
}St_SecSetupConnection;

typedef struct tagSt_SecStopConnection
{
    WE_INT32 iTargetID;
    WE_INT32 iSecurityId;
    WE_UINT8 ucConnectionType;
    WE_UINT8 aucReserved[3]; /* four bytes align */
}St_SecStopConnection;

typedef struct tagSt_SecWtlsVerifySvrCert
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucBuf;
    WE_UCHAR * pucAddr;
    WE_UCHAR * pucDomain;
    WE_UINT16  usBufLen;
    WE_UINT16  usAddrLen;
    WE_UINT16  usDomainLen;
    WE_UINT8 aucReserved[2]; /* four bytes align */
}St_SecWtlsVerifySvrCert;

typedef struct tagSt_SecWtlsGetCipherSuite
{
    WE_INT32 iTargetID;
    WE_HANDLE hTrustKeyIds;
}St_SecWtlsGetCipherSuite;

typedef struct tagSt_SecGetPrfResult
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucSecret;
    WE_CHAR  * pcLabel;
    WE_UCHAR * pucSeed;
    WE_UINT16  usSeedLen;
    WE_UINT16  usOutputLen;
    WE_UINT16  usSecretLen;
    WE_UINT8   ucAlg;
    WE_UINT8   ucMasterSecretId;
}St_SecGetPrfResult;

#ifdef M_SEC_CFG_WTLS
typedef struct tagSt_SecWtlsKeyExchange
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucRandval;    
    St_SecWtlsKeyExchParams *pstParam;
    WE_UINT8   ucHashAlg;
    WE_UINT8 aucReserved[3]; /* four bytes align */
}St_SecWtlsKeyExchange;
#endif
typedef struct tagSt_SecRemovePeer
{
    WE_INT32   iTargetID;
    WE_UINT8 ucMasterSecretId;
    WE_UINT8 aucReserved[3]; /* four bytes align */
}St_SecRemovePeer;

typedef struct tagSt_SecAttachPeer
{
    WE_UCHAR * pucAddress;
    WE_UCHAR * pucDomain;
    WE_UINT16  usDomainLen;
    WE_UINT16  usPortnum;
    WE_UINT16  usAddressLen;
    WE_UINT8   ucConnectionType;
    WE_UINT8   ucMasterSecretId;
}St_SecAttachPeer;

typedef struct tagSt_SecSearchPeer
{
    WE_INT32   iTargetID;
    WE_UCHAR * pucAddress;
    WE_UCHAR * pucDomain;
    WE_UINT16  usDomainLen;
    WE_UINT16  usPortnum;
    WE_UINT16  usAddressLen;
    WE_UINT8   ucConnectionType;
    WE_UINT8   ucReserved; /* four bytes align */
}St_SecSearchPeer;

typedef struct tagSt_SecEnableSession
{
    WE_UINT8 ucMasterSecretId;
    WE_UINT8 ucIsActive;
    WE_UINT8   ucReserved[2]; /* four bytes align */
}St_SecEnableSession;

typedef struct tagSt_SecDisableSession
{
    WE_UINT8 ucMasterSecretId;
    WE_UINT8   ucReserved[3]; /* four bytes align */
}St_SecDisableSession;

typedef struct tagSt_SecFetchSessionInfo
{
    WE_INT32 iTargetID;
    WE_UINT8 ucMasterSecretId;
    WE_UINT8   ucReserved[3]; /* four bytes align */
}St_SecFetchSessionInfo;

typedef struct tagSt_SecUpdateSession
{
    WE_UCHAR * pucPrivateKeyId;
    WE_UCHAR * pucSessionId;
    WE_UINT32  uiCreationTime;    
    WE_UINT8   ucMasterSecretId;
    WE_UINT8   ucSessionOptions;    
    WE_UINT8   ucCipherSuite[2];    
    WE_UINT8   ucCompressionAlg;
    WE_UINT8   ucSessionIdLen;
    WE_UINT8   ucReserved[2]; /* four bytes align */
}St_SecUpdateSession;

/*********************************************
*     stucture defined for tls
*********************************************/
#ifdef M_SEC_CFG_TLS
typedef struct tagSt_SecTlsVerifySvrCert
{
    WE_INT32       iTargetID;
    WE_UCHAR      *pucAddr;
    WE_UCHAR      *pucDomain;
    St_SecTlsAsn1Certificate *pstCerts;
    WE_UINT16     usAddrLen;
    WE_UINT16     usDomainLen;
    WE_UINT8      ucNumCerts;
    WE_UINT8   ucReserved[3]; /* four bytes align */
}St_SecTlsVerifySvrCert;

typedef struct tagSt_SecTlsGetCipherSuite
{
    WE_INT32 iTargetID;
}St_SecTlsGetCipherSuite;

typedef struct tagSt_SecTlsKeyExchange
{
    WE_INT32   iTargetID;
    WE_UCHAR   *pucRandval;    
    St_SecTlsKeyExchangeParams *pstParam;
    WE_INT32   iOptions;
    WE_UINT8   ucHashAlg;
    WE_UINT8   ucReserved[3]; /* four bytes align */
}St_SecTlsKeyExchange;

#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
typedef struct tagSt_SecTlsComputeSign
{
    WE_INT32   iTargetID;
    WE_UCHAR   *pucPubKeyHash;    
    WE_UCHAR   *pucData;
    WE_UINT16  usPubKeyHashLen;
    WE_UINT16  usDataLen;
    WE_UINT8   ucAlg;
    WE_UINT8   ucReserved[3]; /* four bytes align */
}St_SecTlsComputeSign;
#endif

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
WE_INT32 Sec_MsgSetupCon (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iMasterSecretId, 
                          WE_INT32 iSecurityId, WE_INT32 iFullHandshake,
                          St_SecSessionInfo stSessionInfo);

WE_INT32 Sec_MsgStopCon (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId,
                         WE_UINT8 ucConnectionType);
WE_INT32 Sec_MsgEnableSession (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucIsActive);

WE_INT32 Sec_MsgDisableSession (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId);
WE_INT32 Sec_MsgRemovePeer(WE_HANDLE hSecHandle, WE_INT32 iTargetID,WE_INT32 iMasterSecretId);

WE_INT32 Sec_MsgAttachPeerToSession (WE_HANDLE hSecHandle, WE_UINT8 ucConnectionType,
                                     const WE_UCHAR * pucAddress, WE_INT32 iAddressLen,
                                     const WE_CHAR * pcDomain, WE_UINT16 usPortnum,
                                     WE_INT32 iMasterSecretId);

WE_INT32 Sec_MsgSearchPeer (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucConnectionType,
                            const WE_UCHAR * pucAddress, WE_INT32 iAddressLen,
                            const WE_CHAR * pcDomain, WE_UINT16 usPortnum);
WE_INT32 Sec_MsgRenewSession (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, 
                               WE_UINT8 ucSessionOptions, const WE_UCHAR * pucSessionId, 
                               WE_UINT8 ucSessionIdLen,  WE_UINT8 aucCipherSuite[2],
                               WE_UINT8 ucCompressionAlg, const WE_UCHAR * pucPrivateKeyId,
                               WE_UINT32 uiCreationTime);
WE_INT32 Sec_MsgGetSessionInfo (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iMasterSecretId);

#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_MsgWtlsGetPrfResult(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucAlg, 
                                WE_INT32 iMasterSecretId, const WE_UCHAR * pucSecret, WE_INT32 iSecretLen,
                                const WE_CHAR * pcLabel, const WE_UCHAR * pucSeed, WE_INT32 iSeedLen,
                                WE_INT32 iOutputLen);

WE_INT32 Sec_MsgWtlsVerifySvrCert(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                   const WE_VOID *pvBuf, WE_INT32 iBufLen);
WE_INT32 Sec_MsgWtlsGetCipherSuite (WE_HANDLE hSecHandle, WE_INT32 iTargetID ,WE_HANDLE hTrustKeyIds);

WE_INT32 Sec_MsgWtlsKeyExchange (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                 const WE_VOID *pvParam, WE_UINT8 ucAlg, 
                                 const WE_UCHAR * pucRandval);
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_INT32 Sec_MsgCompSign (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                             const WE_UCHAR * pucPubKeyHash,WE_INT32 iPubKeyHashLen,
                             const WE_UCHAR *pucData,WE_INT32 iDataLen, 
                             WE_INT32 iAlg) ;
#endif

#ifdef M_SEC_CFG_TLS
WE_INT32 Sec_MsgTlsVerifyCertChain (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                   const WE_VOID *pvBuf, WE_INT32 iBufLen);
WE_INT32 Sec_MsgTlsGetCipherSuite (WE_HANDLE hSecHandle, WE_INT32 iTargetID);

WE_INT32 Sec_MsgTlsKeyExchange (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                 const WE_VOID *pvParam, WE_UINT8 ucAlg, 
                                 const WE_UCHAR * pucRandval);
WE_INT32 Sec_MsgTlsGetPrfResult(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucAlg, 
                                 WE_INT32 iMasterSecretId, const WE_UCHAR * pucSecret, WE_INT32 iSecretLen,
                                 const WE_CHAR * pcLabel, const WE_UCHAR * pucSeed, WE_INT32 iSeedLen,
                                 WE_INT32 iOutputLen);


#endif
#endif
