/*==================================================================================================
    FILE NAME   : wap_sslcore.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       SSL handshake functions that are different from TLS.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
#ifndef WAP_TLS_SSL_CORE_H
#define WAP_TLS_SSL_CORE_H

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
/*SSL protocol version*/
#define M_WAP_SSL_PROTOCOL_VERSION_MAJOR        M_WAP_TLS_PROTOCOL_VERSION_MAJOR
#define M_WAP_SSL_PROTOCOL_VERSION_MINOR        0

/*Hash algorithm Pad length*/
#define M_WAP_SSL_MD5_PAD_LEN   48
#define M_WAP_SSL_SHA_PAD_LEN   40


/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
/*================================================================================================
 * The parameter pad1 (0x36) to hash.
 *================================================================================================*/
void WapSSL_GetHashParaPad1( WE_UCHAR    *pucPad1 );

/*================================================================================================
 * The parameter pad2 (0x5c) to hash.
 *================================================================================================*/
void WapSSL_GetHashParaPad2( WE_UCHAR    *pucPad2 );

/*================================================================================================
 * Convert the Tls' alerts that aren't declared in SSL3.0 protocol to SSL's alerts.
 *================================================================================================*/
WE_INT32 WapSSL_ConvertAlertDesc (WE_INT32 iAlertDesc);

/*================================================================================================
 * SSL compute MAC.
 *================================================================================================*/
WE_INT32 WapSSL_ComputeMAC
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstState,
    WE_UCHAR           *pucMacSecret,
    St_WapTlsRecord    *pstRec,
    WE_UCHAR           *pucMac
);
/*================================================================================================
 * Cryptographic computations.
 *================================================================================================*/
/*wap_sslhm.c*/
WE_INT32 WapSSLHm_CreateKeys
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
);
/*================================================================================================
 * Create client cert verify message.
 *================================================================================================*/
WE_INT32 WapSSLHm_CreateClientCertVerify
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
);
/*================================================================================================
 *  Create client finished PDU.
 *================================================================================================*/
WE_INT32 WapSSLHm_CreateClientFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 * Check the serverFinished PDU when receive it.
 *================================================================================================*/
WE_INT32 WapSSLHm_ProcRecvServerFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen    
);
WE_INT32 WapSSLHm_ProcSSLSecRes
( 
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn *pstConn,
    void* pvResp
);

#endif
