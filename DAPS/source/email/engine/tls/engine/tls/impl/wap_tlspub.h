/*==================================================================================================
    FILE NAME   : wap_tlspub.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       Common function for tls module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/

#ifndef WAP_TLS_PUB_H
#define WAP_TLS_PUB_H



/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/

/***************************************************************************************************
wap_tlsconn.c
***************************************************************************************************/
/*================================================================================================
 * create a new TLS connection
 *================================================================================================*/
St_WapTlsConn* WapTls_ConnNew
(
    St_WapTlsHandle *pstTlsHandle, 
    WE_INT32 iConnId
);
/*================================================================================================
 * delete the TLS connection assigned with parameter passed in
 *================================================================================================*/
WE_INT32 WapTls_ConnDelete
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iCidx
);
/*================================================================================================
 * delete the TLS connection assigned with parameter passed in
 *================================================================================================*/
St_WapTlsConn *WapTls_FindConnById
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
);

/*================================================================================================
 * Init the members of a new TLS connection
 *================================================================================================*/
WE_INT32 WapTls_ConnStateInit( St_WapTlsConnState *pstConnState );

/***************************************************************************************************
wap_tlsrec.c
*************************************************************************************************/
/*================================================================================================
 * parse the buffer passed in to the record structure
 *================================================================================================*/
WE_INT32 WapTls_ParseRecordHeader
( 
    St_WapTlsRecord *pstRecord,
    WE_UCHAR *pucHeader
);
/*================================================================================================
 * create a new Record
 *================================================================================================*/
St_WapTlsRecord *WapTls_NewRecord
(
    WE_UINT8 ucContentType,
    WE_UINT32 uiSeqNum,
    St_WapTlsProVer stProVer
);

/*================================================================================================
 *  Delete the record passed in
 *================================================================================================*/
WE_INT32 WapTls_DeleRecord( St_WapTlsRecord *pstRecord );

/*================================================================================================
 * Get First Record from record list
 *================================================================================================*/
St_WapTlsRecord* WapTls_GetRecFromList( St_WapTlsRecord **ppstRecList );
/*================================================================================================
 * Add Record to the list tail
 *================================================================================================*/
void WapTls_AddRecToList
(
    St_WapTlsRecord **ppstRecList, 
    St_WapTlsRecord* pstRec
);
/*================================================================================================
 * Delete the record list
 *================================================================================================*/
void WapTls_DeleRecList( St_WapTlsRecord **ppstRecList );
/*================================================================================================
 * Decrypt record from network data
 *================================================================================================*/
WE_INT32 WapTls_DecryptRecord 
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn, 
    St_WapTlsRecord* pstRecord
);
/*================================================================================================
 * Encrypt record at client
 *================================================================================================*/
WE_INT32 WapTls_EncryptRecord
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    St_WapTlsRecord *pstRecord
);

/**************************************************************************************************
wap_tlscvt.c
***************************************************************************************************/
/*================================================================================================
 * Init a encoder.
 *================================================================================================*/
void WapTls_EncodeInit
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UCHAR    *pucData,
    WE_INT32     iLength
);
/*================================================================================================
 * Encode a TLS record layer data.
 *================================================================================================*/
WE_INT32 WapTls_EncodeRecord
(
    St_WapEcdrEncoder *pstEncoder, 
    St_WapTlsRecord  *pstRecord
);
/*================================================================================================
 * Encode a 24 bit unsigned int
 *================================================================================================*/
WE_INT32 WapTls_EncodeUint24
(
    St_WapEcdrEncoder  *pstEncoder,
    M_WAP_TLS_UINT24   *puiPointer
);
/*================================================================================================
 * Decode a 24 bit unsigned int
 *================================================================================================*/
WE_INT32 WapTls_DecodeUint24
(
    St_WapEcdrDecoder *pstDecoder,
    M_WAP_TLS_UINT24  *ptPointer
);
/*================================================================================================
 * Encode a unsigned char vector
 *================================================================================================*/
WE_INT32 WapTls_EncodeUcVector
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32           iLength,
    WE_UCHAR           **ppPointer
);

/*================================================================================================
 * Encode a handshake layer clientHello PDU.
 *================================================================================================*/
WE_INT32 WapTls_EncodeClientHello
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientHello *pstClientHello
);
/*================================================================================================
 * Change the encoder's position as the size of clientHello to get the size of it.
 *================================================================================================*/
WE_INT32 WapTls_GetClientHelloSize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientHello *pstClientHello
);
/*================================================================================================
 * Decode the handshake layer serverCert PDU.
 *================================================================================================*/
WE_INT32 WapTls_DecodeServerCert
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsServerCert *pstCert
);
/*================================================================================================
 * Free the member of a serverCert structure and itself.
 *================================================================================================*/
WE_INT32 WapTls_FreeServerCert( St_WapTlsServerCert *pstCert );
/*================================================================================================
 * Decode the handshake layer serverKeyExch PDU.
 *================================================================================================*/
WE_INT32 WapTls_DecodeServerKeyExch
(
    St_WapEcdrDecoder *pstDecoder,
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc
);
/*================================================================================================
 * Free the member of a serverKeyExchange structure and itself.
 *================================================================================================*/
WE_INT32 WapTls_FreeServerKeyExch
(
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc    
);
/*================================================================================================
 * Decode the handshake layer serverCertRequest PDU.
 *================================================================================================*/
WE_INT32 WapTls_DecodeCertReq
(
      St_WapEcdrDecoder *pstDecoder,
      St_WapTlsCertReq *pstCertReq
);
/*================================================================================================
 * free the cert request structure and it's member
 *================================================================================================*/
WE_INT32 WapTls_FreeCertReq( St_WapTlsCertReq *pstCertReq );
/*================================================================================================
 * Encode the client's cert as the net's order.
 *================================================================================================*/
WE_INT32 WapTls_EncodeClientCert
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
);
/*================================================================================================
 * Change the encoder's position as the size of client cert.
 *================================================================================================*/
WE_INT32 WapTls_GetClientCertSize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
);
/*================================================================================================
 * free the client cert structure and it's member
 *================================================================================================*/
WE_INT32 WapTls_FreeClientCert( St_WapTlsClientCert *pstClientCert );
/*================================================================================================
 * Encode a client cert verify PDU.
 *================================================================================================*/
WE_INT32 WapTls_EncodeCertVerify
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsCertVerify *pstClientCert
);
/*================================================================================================
 * Change the encoder's position as the size of client cert verify.
 *================================================================================================*/
WE_INT32 WapTls_GetCertVerifySize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsCertVerify *pstClientCert
);
/*================================================================================================
 * decode a handshake layer server hello PDU.
 *================================================================================================*/
WE_INT32 WapTls_DecodeServerHello
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsServerHello *pstSerHello
);

/**************************************************************************************************
wap_tlsp.c
***************************************************************************************************/
/*================================================================================================
 * Find the cipher suite info according to the ciphersuite passed in
 *================================================================================================*/
St_WapTlsCipherSuiteInfo *WapTls_FindCipherSuite
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT16 usCipherSuite
);

/*================================================================================================
 * Find the hash info according to the hash algorithm
 *================================================================================================*/
 St_WapTlsHashInfo* WapTls_FindHash
 (
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT8 ucHashAlg
 );

/*================================================================================================
 *  Find the cipher info in the array according to the cipher passed in
 *================================================================================================*/
St_WapTlsCipherInfo* WapTls_FindCipher
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT8 ucCipher
);

/*================================================================================================
 * Init Hash operation
 *================================================================================================*/
WE_INT32  WapTls_HashInit
(
    St_WapTlsHandle *pstTlsHandle,
    E_WeHashAlgType eAlgType, 
    WE_HANDLE *phHandle
);
/*================================================================================================
 * Update Hash value as the parameter passed in.
 *================================================================================================*/
WE_INT32 WapTls_HashUpdate
( 
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHandle,
    WE_UCHAR *pucBuf, 
    WE_INT32 iLen
);
/*================================================================================================
 * Get hash result
 *================================================================================================*/
WE_INT32  WapTls_HashEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE       hHandle,
    WE_UINT8        *pucDigest
);
/*================================================================================================
 * Compute MAC value
 *================================================================================================*/
WE_INT32 WapTls_ComputeMAC
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucMacSecret,
    St_WapTlsRecord  *pstRec,
    WE_UCHAR   *pucMAC
);
/*================================================================================================
 * Encrypt data with block cipher
 *================================================================================================*/
WE_INT32 WapTls_EncryptBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
);
/*================================================================================================
 *  Init Stream cipher for encrypt
 *================================================================================================*/
WE_INT32 WapTls_EncryptStreamInit
(
   St_WapTlsHandle *pstTlsHandle,
   WE_UCHAR ucCipherAlg,
   WE_UCHAR *pucEncryKey,
   WE_UCHAR ucKeySize,
   WE_HANDLE  *phCipherHandle
);
/*================================================================================================
 * Update stream encrypt
 *================================================================================================*/
WE_INT32 WapTls_EncryptStreamUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
);
/*================================================================================================
 * End stream encrypt
 *================================================================================================*/
WE_INT32 WapTls_EncryptStreamEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hCipherHandle
);
/*================================================================================================
 * Decrypt data with block cipher
 *================================================================================================*/
WE_INT32 WapTls_DecryptBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
);

/*================================================================================================
 * Init Stream cipher for decrypt
 *================================================================================================*/
WE_INT32 WapTls_DecryptStreamInit
(
   St_WapTlsHandle *pstTlsHandle,
   WE_UCHAR ucCipherAlg,
   WE_UCHAR *pucEncryKey,
   WE_UCHAR ucKeySize,
   WE_HANDLE  *phCipherHandle
);
/*================================================================================================
 * Update stream decrypt
 *================================================================================================*/
WE_INT32 WapTls_DecryptStreamUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
);
/*================================================================================================
 * End stream decrypt
 *================================================================================================*/
WE_INT32 WapTls_DecryptStreamEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hCipherHandle
);
/*================================================================================================
 * Get the hash info table
 *================================================================================================*/
St_WapTlsHashInfo* WapTls_GetHashInfoTable(void);

/*================================================================================================
 * Get the cipher suite table
 *================================================================================================*/
St_WapTlsCipherSuiteInfo* WapTls_GetCipherSuiteTable(void);

/*================================================================================================
 * Get the cipher Info table
 *================================================================================================*/
St_WapTlsCipherInfo* WapTls_GetCipherInfoTable(void);

/*================================================================================================
 * Free the cipher Info table
 *================================================================================================*/
WE_INT32 WapTls_FreeCipherInfoTable(St_WapTlsCipherInfo *pstCipherInfoTab);

/*================================================================================================
 * Free the hash Info table
 *================================================================================================*/
WE_INT32 WapTls_FreeHashInfoTable(St_WapTlsHashInfo *pstHashInfoTab);

/*================================================================================================
 * Free the cipher suite table
 *================================================================================================*/
WE_INT32 WapTls_FreeCipherSuiteTable(St_WapTlsCipherSuiteInfo *pstCipherSuTab);
/*================================================================================================
 * Encode certificate request to one buffer
 *================================================================================================*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
WE_INT32 WapTls_EncodeCertReq
(
    St_WapTlsCertReq   *pstCertReq,
    WE_CHAR            **ppcBuffer,
    WE_INT32  *piLen
);
#endif

#endif

