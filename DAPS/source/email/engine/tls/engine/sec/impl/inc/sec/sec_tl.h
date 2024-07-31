/*==================================================================================================
    FILE NAME : sec_tl.h
    MODULE NAME : SEC
    
    
    GENERAL DESCRIPTION
        In this file,define the initial function prototype ,and will be update later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                           Tracking
    Date              Author              Number         Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   steven ding                           create sec_tl.h
    2007-06-19    bluefire                         four bytes align for dragonfly
    
==================================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_TL_H
#define SEC_TL_H

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
typedef struct tagSt_SecTlFncId 
{
    E_SecDpFncId                eType;
}St_SecTlFncId;
typedef St_SecTlFncId       St_SecTlFunctionId ;

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
typedef struct tagSt_SecTlUeFncId
{
    E_SecTlUeFncId           eType;
}St_SecTlUeFncId;
typedef St_SecTlUeFncId     St_SecTlUeFunctionId;
#endif

typedef struct tagSt_SecTlWimFncId 
{
  E_SecDpFncId                  eType;
}St_SecTlWimFncId;
typedef St_SecTlWimFncId    St_SecTlWimFunctionId ;

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
typedef struct tagSt_SecTlConfirmDlgResp
{
    E_SecTlUeFncId       eType;
    WE_UINT8                  ucAnswer;
}St_SecTlConfirmDlgResp;
#endif

typedef struct tagSt_SecTlInitWimResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
}St_SecTlInitWimResp;

typedef struct tagSt_SecTlGetWtlsMethodsResp
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_UCHAR*                   pucCipherMethods;
    WE_INT32                    iCipherMethodsLen;
    WE_UCHAR*                   pucKeyExchangeIds;
    WE_INT32                    iKeyExchangeIdsLen;
    WE_UCHAR*                   pucTrustedKeyIds;
    WE_INT32                    iTrustedKeyIdsLen;
    WE_INT32                    iTooManyCerts;
}St_SecTlGetWtlsMethodsResp;

typedef struct tagSt_SecTlComputeSignResp
{
  E_SecDpFncId                  eType;
  WE_INT32                      iTargetID;
  WE_INT32                      iResult;
  WE_UCHAR*                     pucSig;
  WE_INT32                      iSigLen;
}St_SecTlComputeSignResp;


/***************************************
*structure defined for tls
****************************************/
typedef struct tagSt_SecTlGetTlsCipherSuitesResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_UCHAR                    *pucCipherSuites;
    WE_INT32                    iCipherSuitesLen;
}St_SecTlGetTlsCipherSuitesResp;

typedef struct tagSt_SecTlsTlKeyXchResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_INT32                    iMasterSecretId;
    WE_UCHAR*                   pucPublicValue;
    WE_INT32                    iPublicValueLen;
}St_SecTlsTlKeyXchResp;
typedef struct tagSt_SecTlKeyXchResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_INT32                    iMasterSecretId;
    WE_UCHAR*                   pucPublicValue;
    WE_INT32                    iPublicValueLen;
}St_SecTlKeyXchResp;

typedef struct tagSt_SecTlPrfResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_UCHAR*                   pucBuf;
    WE_INT32                    iBufLen;
}St_SecTlPrfResp;

typedef struct tagSt_SecTlVerifyCertChainResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_INT32                    iCertTooOld;
    WE_INT32                    iCertTooOldId;
    WE_UINT16                   usKeyExchKeyLen;
    WE_UINT16                   usReserved; /* four bytes align */
}St_SecTlVerifyCertChainResp;

typedef struct tagSt_SecTlVerifyRootCertresp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
}St_SecTlVerifyRootCertresp;

typedef struct tagSt_SecTlRmPeerLinksResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlRmPeerLinksResp;

typedef struct tagSt_SecTlLinkPeerSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlLinkPeerSessionResp;

typedef struct tagSt_SecTlActiveSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlActiveSessionResp;

typedef struct tagSt_SecTlInvalidSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlInvalidSessionResp;

typedef struct tagSt_SecTlCleanUpSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlCleanUpSessionResp;

typedef struct tagSt_SecTlUpdateSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
}St_SecTlUpdateSessionResp;

typedef struct tagSt_SecTlLookupPeerResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_INT32                    iMasterSecretId;
    WE_UINT8                    ucConnectionType;
    WE_UINT8                    aucReserved[3]; /* four bytes align */
}St_SecTlLookupPeerResp;

typedef struct tagSt_SecTlFetchSessionResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID;
    WE_INT32                    iResult;
    WE_UCHAR*                pucSessionId;
    WE_UCHAR*                pucPrivateKeyId;
    WE_UINT32                  uiCreationTime;
    WE_UINT8                    ucSessionOptions;
    WE_UINT8                    ucSessionIdLen;
    WE_UINT8                    aucCipherSuite[2];
    WE_UINT8                    ucCompressionAlg;
}St_SecTlFetchSessionResp;

typedef struct tagSt_SecTlReadViewsCertsResp 
{
    E_SecDpFncId                eType;
    WE_INT32                    iResult;
    WE_UCHAR*                   pucData;
    WE_INT32                    iLen;
}St_SecTlReadViewsCertsResp;

typedef struct tagSt_SecTlVerifySignResp
{
    E_SecDpFncId                eType;
    WE_INT32                    iTargetID; 
    WE_INT32                    iResult; 
}St_SecTlVerifySignResp;

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
WE_VOID Sec_TlHandleUeStructFree(WE_VOID* pvFncId);

WE_VOID Sec_TlHandleDpStructFree(WE_VOID* pvPara);

void Sec_StoreStrUint8to16( const WE_UINT8 *pucStr8, WE_UINT16 *pusAdd16 );

void Sec_StoreStrUint8to32( const WE_UINT8 *pucStr8, WE_UINT32 *puiAdd32 );

void Sec_ExportStrUint16to8( WE_UINT16 *pusAdd16, WE_UINT8 *pucStr8 );

void Sec_ExportStrUint32to8( WE_UINT32 *puiAdd32, WE_UINT8 *pucStr8 );

#endif
