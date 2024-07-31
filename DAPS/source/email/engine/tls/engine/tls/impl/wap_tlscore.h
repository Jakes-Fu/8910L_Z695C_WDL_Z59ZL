/*==================================================================================================
    FILE NAME   : wap_tlscore.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        Funcations for tls open/close connection, send data to server, process data from server.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
#ifndef WAP_TLS_CORE_H
#define WAP_TLS_CORE_H

/***************************************************************************************************
*   Prototype Declare Section
**************************************************************************************************/
/*================================================================================================
 * Store the handshake info and notify CM that TLS module has opend
 *================================================================================================*/
WE_INT32 WapTls_ProcHmOpend
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32   iConnId
);
/*================================================================================================
 * Send message according to the content type assigned
 *================================================================================================*/
WE_INT32 WapTls_ProcSendMsg
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32         iConnId,
    WE_UINT8         ucContentType,
    WE_UCHAR        *pucData,
    WE_INT32        iLen
);
/*================================================================================================
 * Init TLS Module
 *================================================================================================*/
WE_INT32 WapTls_ProcSendCCS
(
      St_WapTlsHandle *pstTlsHandle,
      WE_INT32  iConnId
);
/*================================================================================================
 * Send ChangeCipherSpec
 *================================================================================================*/
WE_INT32 WapTls_ProcSendOneRecord
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 *  Notify handshake layer that has sent CCS or has processed the received CCS PDU
 *================================================================================================*/
WE_INT32 WapTls_ProcCCSProcessed
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32  iConnId
);

/*================================================================================================
 * Process the send alert message from handshake layer
 *================================================================================================*/
WE_INT32 WapTls_ProcSendAlert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId,
    WE_INT32 iAlertDesc
);
/*================================================================================================
 * Send alert to server,convert the alert and add it to send list.
 *================================================================================================*/
WE_INT32 WapTls_SendAlert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_INT32 iAlertLevel,
    WE_INT32 iAlertDesc
) ;
/*================================================================================================
 * TLS receive the alert message from server
 *================================================================================================*/
WE_INT32 WapTls_ProcReceivedAlert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_INT32 iAlertLevel,
    WE_INT32 iAlertDesc
) ;
/*================================================================================================
 * Process data in input buffer
 *================================================================================================*/
void  WapTls_ProcInput
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 * Delete socket data list
 *================================================================================================*/
WE_INT32 WapTls_DeleteSocketData(St_WapTlsSocketList *pstList);
/*================================================================================================
 * Process data from socket
 *================================================================================================*/
WE_INT32 WapTls_ProcDataFromSocket
(
    WE_HANDLE hTlsHandle,
    WE_INT32 iConnId,
    WE_VOID* pvData,
    WE_INT32 iDataLen
);
/*================================================================================================
 * Process err condition
 *================================================================================================*/
WE_INT32 WapTls_ProcErr
(
    WE_HANDLE hTlsHandle,
    WE_INT32 iConnId,
    WE_INT32 iErrCode,
    WE_INT32 iErrDesc
);


#endif/*endif WAP_TLS_CORE_H*/



