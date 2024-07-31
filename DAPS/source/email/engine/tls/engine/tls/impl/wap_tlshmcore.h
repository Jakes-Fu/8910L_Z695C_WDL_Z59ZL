/*==================================================================================================
    FILE NAME   : wap_tlshmcore.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        Functions for TLS handshake layer.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
#ifndef WAP_TLS_HM_CORE_H
#define WAP_TLS_HM_CORE_H
/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/

#define TLS_HM_OFFSET                      (0x01 << 6)
#define WAP_MSG_MODULEID_TLS                  7
#define M_WAP_TLS_MSG_BASE                 (WAP_MSG_MODULEID_TLS << 16)
#define M_WAP_TLS_MSG_LIMIT                (M_WAP_TLS_MSG_BASE | 0xffff)
#define M_WAP_TLS_MSG_HM_BASE              (M_WAP_TLS_MSG_BASE + TLS_HM_OFFSET)

/*Message from HandShake*/
#define M_WAP_TLS_TLSHM_MSG_SEND_HSHK_MSG           (M_WAP_TLS_MSG_BASE + 11)           
#define M_WAP_TLS_TLSHM_MSG_SEND_CCS                (M_WAP_TLS_MSG_BASE + 12)
#define M_WAP_TLS_TLSHM_MSG_SEND_ALERT              (M_WAP_TLS_MSG_BASE + 13)
#define M_WAP_TLS_TLSHM_MSG_CCS_PROCESSED           (M_WAP_TLS_MSG_BASE + 14)                                       /*tlshm to tls, for notify client have received ccs*/
#define M_WAP_TLS_TLSHM_MSG_OPEND                   (M_WAP_TLS_MSG_BASE + 15)
#define M_WAP_TLS_TLSHM_MSG_ERROR                   (M_WAP_TLS_MSG_BASE + 16)

/*Message TO TLSHM*/
#define M_WAP_TLS_HM_MSG_START                      (M_WAP_TLS_MSG_HM_BASE + 0)
#define M_WAP_TLS_HM_MSG_RECV_HSHK_MSG              (M_WAP_TLS_MSG_HM_BASE + 1)
#define M_WAP_TLS_HM_MSG_RECV_CCS                   (M_WAP_TLS_MSG_HM_BASE + 2)
#define M_WAP_TLS_HM_MSG_CCS_PROCESSED              (M_WAP_TLS_MSG_HM_BASE + 3) /*tls to tlshm, for notify client have sent ccs*/

#define M_WAP_TLS_HM_MSG_PEER_LOOKUP_RESP           (M_WAP_TLS_MSG_HM_BASE + 4)         
#define M_WAP_TLS_HM_MSG_SESSION_FETCH_RESP         (M_WAP_TLS_MSG_HM_BASE + 5)
#define M_WAP_TLS_HM_MSG_GET_CIPHER_SUITES_RESP     (M_WAP_TLS_MSG_HM_BASE + 6)
#define M_WAP_TLS_HM_MSG_SERVER_CERT_VERIFY_RESP    (M_WAP_TLS_MSG_HM_BASE + 7)
#define M_WAP_TLS_HM_MSG_CREATE_KEY_EXCH_RESP       (M_WAP_TLS_MSG_HM_BASE + 8)
#define M_WAP_TLS_HM_MSG_GET_CLIENT_CERT_RESP       (M_WAP_TLS_MSG_HM_BASE + 9)
#define M_WAP_TLS_HM_MSG_CLIENT_CERT_SIGNATURE_RESP (M_WAP_TLS_MSG_HM_BASE + 10)
#define M_WAP_TLS_HM_MSG_PRF_RESP                   (M_WAP_TLS_MSG_HM_BASE + 11)
#define M_WAP_TLS_HM_MSG_SELECT_SEC_DIALOG_REQ      (M_WAP_TLS_MSG_HM_BASE + 12)   
#define M_WAP_TLS_HM_MSG_SEC_SELECTED_DIALOG_RESULT (M_WAP_TLS_MSG_HM_BASE + 13)
#define M_WAP_TLS_HM_MSG_SSL_GET_MASTER_SEC_RESP    (M_WAP_TLS_MSG_HM_BASE + 14)

/*Message from cm interact with client*/
#define M_WAP_TLS_HM_MSG_PROC_TRUST_ERROR              (M_WAP_TLS_MSG_HM_BASE + 15)
#define M_WAP_TLS_HM_MSG_PROC_CLIENT_AUTH_ERROR        (M_WAP_TLS_MSG_HM_BASE + 16)
#define M_WAP_TLS_HM_MSG_CANCLE_PROC_CLIENT_AUTH       (M_WAP_TLS_MSG_HM_BASE + 17)


/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
#define SendMsgToTlsHM WapTlsHm_MsgProcess 


/***************************************************************************************************
*   Prototype Declare Section
**************************************************************************************************/
/*================================================================================================
 * message process function in handshake layer
 *================================================================================================*/
WE_INT32  WapTlsHm_MsgProcess
( 
    WE_HANDLE     hTlsHandle,
    St_TlsMsg     *pstWapMsgData 
);
/*================================================================================================
 * Process error during handshake
 *================================================================================================*/
void WapTlsHm_ProcHandShakeErr
( 
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstTlsConn, 
    WE_INT32 iErrCode, 
    WE_INT32 iAlerDesc
);
/*================================================================================================
 * Get client's cipher suites, and wait the sec's response.
 *================================================================================================*/
/*wap_tlscm.c*/
WE_INT32 WapTlsHm_CreateClientHello
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 * Create key_block invoking PRF function.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 *  Get client's cert after receive cert request from server.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateClientCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 * Send client key exchange and create the cert verify.
 *================================================================================================*/
WE_INT32 WapTlsHm_SendClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 *  If ssl, create the ccs and send it.
 *  If tls, call prf function to create key_block.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreatePrf
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn   *pstConn
);
/*================================================================================================
 * Create client finished.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateClientFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 * Verify server's cert chain.
 *================================================================================================*/
WE_INT32 WapTlsHm_VerifyCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 * Check the cipher suite from server hello,and then get the information.
 *================================================================================================*/
WE_INT32 WapTlsHm_CheckCipherSuite
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UINT16   usCipherSuite
);
/*================================================================================================
 * PRF( "key expansion" ) to compute the key_block.
 *================================================================================================*/
 WE_INT32 WapTlsHm_CreateKeyExpan
(
    St_WapTlsHandle   *pstTlsHandle,
    St_WapTlsConn     *pstConn
);
/*================================================================================================
 * If KeyExchange algorithm is exportable, PRF("client write key").
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateCWriteKey
(
    St_WapTlsHandle   *pstTlsHandle,
    St_WapTlsConn     *pstConn,
    St_SecGetPrfResp  *pstPrfRsp
);
/*================================================================================================
 * If KeyExchange algorithm is exportable, PRF("server write key").
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateSWriteKey
(
    St_WapTlsHandle   *pstTlsHandle,
    St_WapTlsConn     *pstConn,
    St_SecGetPrfResp  *pstPrfRsp
);
/*================================================================================================
 * Check the response of PRF( "server finished" ).
 *================================================================================================*/
WE_INT32 WapTlsHm_CheckPrfServerFinished
(
    St_WapTlsConn *pstConn,
    St_SecGetPrfResp  *pstPrfResp   
);
/*================================================================================================
 * PRF( " IV block" ) to generate IV_block.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateIVBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp
);
/*================================================================================================
 * Final PRF compute, process the PRF("IV block") response.
 *================================================================================================*/
WE_INT32 WapTlsHm_CreateKeysFinal
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn,
    St_SecGetPrfResp  *pstPrfRsp
);
/*================================================================================================
 * 
 *================================================================================================*/
/*wap_tlsproc.c*/
WE_INT32 WapTlsHm_ProcStartHandShake
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iCidx
);
/*================================================================================================
 * Start preapair handshake
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcHandShakeFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);

/*================================================================================================
 * make pending write state to current write state
 *================================================================================================*/
void  WapTlsHm_ConfigurePendingWrite
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 * make pending read state to current read state
 *================================================================================================*/
void  WapTlsHm_ConfigurePendingRead
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);
/*================================================================================================
 * Update all of the hash data from all handshake messages.
 *================================================================================================*/
void WapTlsHm_UpdateHandShakeHashes
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstTlsConn,
    St_WapTlsHandshake *pstHandShake,
    WE_UCHAR *pucBuf,
    WE_INT32 iLen
);
/*================================================================================================
 * Free the curret handshake 
 *================================================================================================*/
void WapTlsHm_FreeHandShake
( 
    WE_HANDLE hTlsHandle,
    St_WapTlsHandshake*pstTlsHandShake
);
/*================================================================================================
 * Parse server hello message
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerHello
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
);
/*================================================================================================
 * Parse server cert message
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerCert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
);
/*================================================================================================
 * Parse server key exchange message
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_INT32 uiMsgLen
);
/*================================================================================================
 * Parse CERT REQUEST message
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvCertRequest
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
);
/*================================================================================================
 * Parse Finished message
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcRecvServerFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen
);

/*================================================================================================
 * Process the search peer's response from SEC module.
 *================================================================================================*/
/*wap_tlssec.c*/
void WapTlsHm_PeerLookUpRespProc
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
/*================================================================================================
 * Fetech the session ,store the session information
 *================================================================================================*/
WE_INT32 WapTlsHm_StoreSessionData
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
/*================================================================================================
 * Fill the client hello structure.And notify tls to send client hello. 
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientHello
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
/*================================================================================================
 * Fill the client key exchange structure.And notify tls to send client key exchange.
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientKeyExch
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn*pstConn
);
/*================================================================================================
 * process the PRF function response from SEC
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcPrfResp
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
/*================================================================================================
 * Notify Record to send ClientCert PDU
 *================================================================================================*/
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE 
WE_INT32 WapTlsHm_ProcSendClientCert
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
#endif
/*================================================================================================
 * Fill the clientCertVerify structure.And notify tls to send clientCertVerify.
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcSendCertVerify
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    void *pResp
);
/*================================================================================================
 * Fill the clientFinished structure.And notify tls to send clientFinished.
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcSendClientFinished
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn   *pstConn,
    WE_UCHAR        *pucVerifyData,
    WE_UINT32       uiVerifyDataLen
);
/*================================================================================================
 * Process the cert vertify's result.
 *================================================================================================*/
WE_INT32 WapTlsHm_CheckCertVertifyRespProc
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn   *pstConn,
    WE_UINT32       uiResult
);
/*================================================================================================
 * Process the key exchange response from sec.
 *================================================================================================*/
WE_INT32 WapTlsHm_ClientKeyExchRespProc
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,    
    void *pResp
);
/*================================================================================================
 * Notify APP to select when encouter trust error or need client auth
 *================================================================================================*/
WE_INT32 WapTlsHm_NotifyAppSelect
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,    
    void *pResp
);
/*================================================================================================
 * Continue negotiate after user select
 *================================================================================================*/
void  WapTlsHm_ContiuneNego
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,  
    WE_UINT8          *pucBuffer,
    WE_UINT32          uiBufferLength
);
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE 
/*================================================================================================
 * Process the cert got from APP.
 *================================================================================================*/
WE_INT32 WapTlsHm_ProcGetAppCertResp
(
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstConn,
    WE_UINT32 uiCertOpt,
    WE_UINT32 uiCertLen,
    void *pvCert,    
    void *pvPrivaKey
);
#endif
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
WE_INT32 WapTlsHm_GetFitCert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_CHAR *pcHost,
    WE_VOID **ppvServerCert
);
#endif
#endif/*end if WAP_TLS_HM_CORE_H*/
