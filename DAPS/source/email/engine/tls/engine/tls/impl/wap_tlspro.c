/*==================================================================================================
    FILE NAME   : wap_tlspro.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       process interface for TLS module, data process interface
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/

#include "tls_config.h"
#ifdef WAP_CONFIG_TLS
#include "tls_def.h"

#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h"
#include "ISecW.h"
#include "we_libalg.h"

#include "tls_ast.h"
#include "tls_ecdr.h" /*before wap_tlspub.h*/
#include "tls_msg.h"

#include "wap_tlsdef.h"
#include "wap_tlsst.h"
#include "wap_tlspub.h"
#include "wap_tlscore.h"
#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h"
#endif
#include "wap_tlshmcore.h"

#include "itls.h"


// #include "wap_tls.h"


/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/

static WE_INT32 WapTls_ProcInputRecord
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
);
static WE_INT32 WapTls_GetNodeFromList
(
    St_WapTlsConn *pstConn
);
static WE_INT32 WapTls_AddDataNode
(
    St_WapTlsSocketList *pstList, 
    WE_CHAR     *pvData, 
    WE_INT32     iLength
);

static WE_INT32 WapTls_SendFromSocket
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
);


/*==================================================================================================
FUNCTION: 
    WapTls_ProcHmOpend
CREATE DATE:
    2007-01-07
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Store the handshake info and notify CM that TLS module has opend
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 WapTls_ProcHmOpend
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32   iConnId
)
{
    St_WapTlsConn * pstConn = NULL;
    St_SecSessionInformation stSessionInfo = {0};
    St_WapTlsConnState *pstConnState = NULL;
    St_WapTlsHashInfo  *pstHashInfo = NULL;
    St_WapTlsCipherInfo *pstCipherInfo = NULL;
    St_WapTlsHandshake  *pstHandShake = NULL;
    WE_INT32   iFullHandShake = -1;
    WE_UINT8   ucConnType = '\0';
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_SecSetupConnectPara  stSetUpConn = {0};
    St_SecConnectOpt stConnOpt = {G_SEC_SETUP_CONNECTION,NULL};
#endif
    St_TlsRespData stResp = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend begin\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend TLS handle NULL\n" );)
        return -1;
    }

    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if ((NULL == pstConn ) || (M_WAP_TLS_STATE_CLOSING == pstConn->ucState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend tls conn is NULL or closing\n" );)
        return -1;
    }
    pstConnState = &(pstConn->stRead);
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend tls conn state is NULL\n" );)
        return -1;
    }

    pstHandShake = pstConn->pstHandshake;
    if (NULL == pstHandShake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend handshake is null\n" );)
        return -1;
    }
    pstHashInfo = WapTls_FindHash(pstTlsHandle,pstConnState->ucHashAlg);
    if (NULL == pstHashInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend handshake no hashinfo \n" );)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg);
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend handshake no cipherinfo \n" );)
        return -1;
    }

    stSessionInfo.ucHmacAlg = pstHashInfo->ucHashAlg;
    stSessionInfo.usHmacLen = pstHashInfo->ucHashSize;
    stSessionInfo.ucEncryptionAlg = pstCipherInfo->ucCipherAlg;
    stSessionInfo.usEncryptionKeyLen = pstCipherInfo->ucKeyMaterialLen;
    
    if (NULL == pstHandShake->stServerCert.pstCert)
    {
        stSessionInfo.ucNumCerts = 0;
        stSessionInfo.pucCert = NULL;
        stSessionInfo.ucCertFormat = 0;
        stSessionInfo.uiCertLen = 0;
    }
    else 
   {
        stSessionInfo.ucNumCerts = 1;
        stSessionInfo.pucCert = pstHandShake->stServerCert.pstCert->pucCert;
        stSessionInfo.ucCertFormat = pstHandShake->stServerCert.pstCert->ucFormat;
        stSessionInfo.uiCertLen = pstHandShake->stServerCert.pstCert->uiCertLen;
    }
    stSessionInfo.ucKeyExchangeAlg = pstHandShake->ucKeyExcMethod;
    stSessionInfo.usKeyExchangeKeyLen = pstHandShake->usKeyExchPubValueLen;
    stSessionInfo.ucSecurityClass = pstHandShake->ucSecurityClass;

    if (M_WAP_TLS_PROTOCOL_VERSION_MINOR == pstConn->stProtocolVersion.ucMinor)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend TLS\n" );)
        ucConnType = G_SEC_TLS_CONNECTION_TYPE;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend SSL\n" );)
        ucConnType = G_SEC_SSL_CONNECTION_TYPE;
    }    
    iFullHandShake = pstHandShake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcHmOpend iFullHandshake is %d\n"),iFullHandShake);)
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    stSetUpConn.iFullHandshake = iFullHandShake;
    stSetUpConn.iMasterSecretId = pstConn->stRead.iMasterSecretId;
    stSetUpConn.iSecId = pstHandShake->iSecurityId;
    stSetUpConn.iTargetID = pstConn->iObjectId;
    stSetUpConn.pstSessionInfo = &stSessionInfo;
    stConnOpt.eType = G_SEC_SETUP_CONNECTION;
    stConnOpt.hParaValue = &(stSetUpConn);
    ISecW_ConnectionOpts(pstTlsHandle->hSecHandle,&stConnOpt,ucConnType);
#else
    ISecW_SetUpConnection( pstTlsHandle->hSecHandle,
                           pstConn->iObjectId,
                           pstConn->stRead.iMasterSecretId,
                           pstHandShake->iSecurityId,
                           iFullHandShake,
                           stSessionInfo, 
                           ucConnType); 
#endif

    pstConn->ucFlags |= M_WAP_TLS_FLAG_ESTABLISHED;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend notify cm opened\n" );)

    stResp.iConnId = pstConn->iConnId;
    stResp.iErrCode = G_WAP_ERR_TLS_FINISH_HANDSHAKE_OK;
    ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_INFO,&(stResp),pstTlsHandle->hPrivData);
    
   /* (WE_VOID)TlsMsg_PostMsg( pstTlsHandle->hMsgHandle, 
                                E_WAP_MSG_MODULEID_CM, 
                                G_WAP_CM_MSG_OPENED, 
                                pstConn->iCidx, 0, 0, 0, NULL, NULL );*/
    pstConn->ucState = M_WAP_TLS_STATE_OPEN;

    // for stop connection
    pstConn->iSecId = pstHandShake->iSecurityId;
    
    WapTlsHm_FreeHandShake(pstTlsHandle,pstHandShake);
    pstConn->pstHandshake = NULL;   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcHmOpend end return 0\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcSendMsg
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send message according to the content type assigned
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    ucContentType : content type
    pucData : data will be sent
    iLen: data length
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ProcSendMsg
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32         iConnId,
    WE_UINT8         ucContentType,
    WE_UCHAR        *pucData,
    WE_INT32        iLen
)
{
    St_WapTlsConn *pstConn = NULL;
    St_WapTlsRecord  *pstRecord = NULL;
    WE_INT32   iPos = 0;
    WE_INT32   iRecLen = 0;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcSendMsg:begin contenttype is %d\n"),ucContentType);)

    if ((NULL == pstTlsHandle) || (NULL == pucData))
    {   
        if (pucData != NULL)
        {
            WAP_FREE(pucData);
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:parameter error\n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if ((NULL == pstConn) || (pstConn->ucState == M_WAP_TLS_STATE_CLOSING))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:can't find connection\n");)
        if (NULL != pucData)
        {
            WAP_FREE(pucData);
        }        
        return -1;
    }
    /*HandShake is in process*/
    if ((ucContentType == M_WAP_TLS_CONTENT_TYPE_HANDSHAKE)
        && (pstConn->ucState == M_WAP_TLS_STATE_HANDSHAKE_INITIATED))
    {
        pstConn->ucState = M_WAP_TLS_STATE_HANDSHAKE_IN_PROGRESS;
    }
    else if (ucContentType == M_WAP_TLS_CONTENT_TYPE_DATA) 
    {
        pstConn->ucHttpSendProcessing = TRUE;/*application data can be sent*/
    }
    /*Construct record list*/
    while (iPos < iLen) 
    {
        pstRecord = WapTls_NewRecord(ucContentType,
                                     pstConn->stWrite.iSeqnum++,
                                     pstConn->stProtocolVersion);
        if (NULL == pstRecord)
        {
            if (NULL != pucData)
            {
                WAP_FREE(pucData);
            }
            return -1;
        }
        iRecLen = MIN(G_WAP_CFG_TLS_MAX_RECORD_LENGTH,iLen - iPos);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcSendMsg:record length is %d\n"),iRecLen);)
        pstRecord->usLength = (WE_UINT16)iRecLen;
        /*Remember free it in the sendrec*/
        if (iRecLen == iLen) /*one record*/
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:one record\n");)
            pstRecord->pucFragment = (WE_UCHAR*)WAP_MALLOC(iRecLen);
            if( NULL == pstRecord->pucFragment )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:malloc fail\n");)
                WAP_FREE(pucData);
                pucData = NULL;
                return -1;
            }
            WAP_MEMCPY( pstRecord->pucFragment, pucData, iRecLen );
        }
        else
        {
            /*Free in WapTls_EncryptRecord or free in this function*/
            pstRecord->pucFragment = (WE_UCHAR*)WAP_MALLOC(iRecLen);
            if( NULL == pstRecord->pucFragment )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:malloc fail\n");)
                WAP_FREE(pucData);
                pucData = NULL;
                return -1;
            }
            WAP_MEMCPY(pstRecord->pucFragment,pucData + iPos, iRecLen);
        }
        iPos += iRecLen;
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcSendMsg:record pos is %d\n"),iPos);)

        iRet = WapTls_EncryptRecord( pstTlsHandle,pstConn,pstRecord);
        if (iRet < 0)/*encrypt error*/
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:record encrypt error\n");)
            WapTls_ProcErr(pstTlsHandle, iConnId, G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
            WAP_FREE(pucData);
            pucData = NULL;
            WapTls_DeleRecord(pstRecord);
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:record encrypt error,return -1");)
            return -1;
        }

        /*add to send record list*/
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:add to send record list\n");)
        WapTls_AddRecToList(&(pstConn->pstSendRecs),pstRecord);        
    }/*end while ,pstConn->pstSendRecs may have more than one record*/

    WAP_FREE(pucData);
    pucData = NULL;

    iRet = WapTls_ProcSendOneRecord(pstTlsHandle, pstConn);


    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendMsg:End\n");)
    return iRet;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcSendCCS
CREATE DATE:
    2006-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send CCS
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    iCidx : Connection id
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ProcSendCCS
(
      St_WapTlsHandle *pstTlsHandle,
      WE_INT32  iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    St_WapTlsRecord *pstRecord = NULL;
    WE_UCHAR *pucBuf = NULL;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:begin\n");)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:TLS handle NULL\n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if ((NULL == pstConn) || (M_WAP_TLS_STATE_CLOSING == pstConn->ucState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:Conn NULL\n");)
        return -1;
    }
    pucBuf = (WE_UCHAR*)WAP_MALLOC(1); /*free in WapTls_EncryptRecord*/
    if (NULL == pucBuf)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:Malloc error\n");)
        return -1;
    }
    pucBuf[0] = 1;
    /*
       CCS is one byte, value is 1
    */
    
    pstRecord = WapTls_NewRecord(M_WAP_TLS_CONTENT_TYPE_CCS,
                                  pstConn->stWrite.iSeqnum++,
                                  pstConn->stProtocolVersion);
    if (NULL == pstRecord)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:new record NULL\n");)
        WAP_FREE(pucBuf);
        return -1;
    }
    pstRecord->usLength = 1;
    pstRecord->pucFragment = pucBuf;

    iRet = WapTls_EncryptRecord(pstTlsHandle,pstConn, pstRecord);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:encrypt error\n");)
        WapTls_ProcErr(pstTlsHandle,iConnId,G_WAP_ERR_TLS_INTERNAL_ERROR,M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR);
        WapTls_DeleRecord(pstRecord);
        return -1;
    }
    WapTls_AddRecToList(&(pstConn->pstSendRecs), pstRecord);
    /*After send CCS, all the message will be encrypted with the key negotiated before*/
    WapTlsHm_ConfigurePendingWrite(pstTlsHandle, pstConn);
    TlsMsg_PostMsg(
                    pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_HM_MSG_CCS_PROCESSED,
                    iConnId,0,
                    0,0,NULL,NULL);
    WapTls_ProcSendOneRecord(pstTlsHandle,pstConn);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendCCS:End\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcCCSProcessed
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Notify handshake layer that has sent CCS or has processed the received CCS PDU
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 WapTls_ProcCCSProcessed
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32  iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    
     WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcCCSProcessed begin\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcCCSProcessed parameter error\n" );)
        return -1;
    }
     pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if ((NULL == pstConn) || (M_WAP_TLS_STATE_CLOSING == pstConn->ucState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcCCSProcessed Conn error\n" );)
        return -1;
    }
    /*Make sure change the input state to continue processing the data in the input buffer*/
    pstConn->ucInputState = M_WAP_TLS_INPUT_STATE_START;
    WapTls_ProcInput(pstTlsHandle,pstConn);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcCCSProcessed end return 0\n" );)    
    return 0;    
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcSendRecord
CREATE DATE:
    2007-01-11
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send record (handshake or application data)
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    pstConn      : connection
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None  
==================================================================================================*/
WE_INT32 WapTls_ProcSendOneRecord
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    St_WapTlsRecord      *pstRecord = NULL;
    St_WapEcdrEncoder     stEncoder = {0};
    WE_UCHAR             *pucBuf = NULL;
    WE_INT32              iBufLen = 0;
    WE_INT32              iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord Begin\n" );) 

    if ((pstTlsHandle == NULL) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord Parameter error\n" );)         
        return -1;
    }
    if ((pstConn->ucTcpSendReady)&& (pstConn->pstSendRecs != NULL))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord can send data\n" );) 
        /*Get first record*/
        pstRecord = WapTls_GetRecFromList(&(pstConn->pstSendRecs));
        if (NULL == pstRecord)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord First node NULL\n" );) 
            return -1;
        }
        iBufLen = pstRecord->usLength + M_WAP_TLS_RECORD_HEADER_SIZE;
        /*Free in WapTls_SendFromSocket*/     
        pucBuf = (WE_UCHAR*)WAP_MALLOC(iBufLen);
        if (NULL == pucBuf)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord malloc error\n" );) 
            return -1;
        }
        /*record->buffer*/
        WapTls_EncodeInit(&stEncoder, pucBuf, iBufLen);
        iRet = WapTls_EncodeRecord( &stEncoder,pstRecord);
        if (iRet == -1)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord Encode error\n" );)             
            WAP_FREE(pucBuf);
            return -1;
        }

        if(pstConn->pucOutputBuf)
        {
           WAP_FREE(pstConn->pucOutputBuf);
        }

        pstConn->pucOutputBuf = pucBuf;
        pstConn->iOutputBufPos = 0;
        pstConn->iOutputBytesLeft = iBufLen;
        /*after data in the output buffer has been sent, it will be TRUE,
          so it can allow to continue sending data*/
        pstConn->ucTcpSendReady = FALSE;
        WapTls_SendFromSocket(pstTlsHandle,pstConn);
        WapTls_DeleRecord(pstRecord);
    }
    /*Send application data*/
    /*SendRec list is empty, yet,data stored has been sent completely*/
    if (NULL == pstConn->pstSendRecs )
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord send list null\n" );) 
         pstConn->ucHttpSendProcessing = FALSE;    /*no data is being sent*/
    }
    else
    { 
        WapTls_ProcSendOneRecord(pstTlsHandle, pstConn);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSendOneRecord End return 0\n" );) 
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_SendFromSocket
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Real send data from socket
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    pstConn : Connection 
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
  
==================================================================================================*/
static WE_INT32 WapTls_SendFromSocket
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    St_TlsRespData stResp = {0};
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendFromSocket:begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendFromSocket:Parameter error\n");)
        return -1;
    }


    stResp.iConnId = pstConn->iConnId;
    stResp.iRespLen = pstConn->iOutputBytesLeft;
    stResp.pvRespData = (WE_VOID*)pstConn->pucOutputBuf;

    ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_DATA_TO_SVR,(WE_VOID*)&stResp,pstTlsHandle->hPrivData);
    pstConn->ucTcpSendReady = TRUE;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendFromSocket:End\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_ProcCloseConn
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Close TLS module, if has data received from socket
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    iCidx      : connection ID
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 WapTls_ProcCloseConn
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
)
{
   St_WapTlsConn *pstConn = NULL;
   
   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcCloseConn:begin\n");)
   pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

   if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcCloseConn:connection null notify cm closed\n");)
        return -1;
    }
    /*No more send or receive data*/
    pstConn->ucTcpSendReady = FALSE;
    WAP_FREE(pstConn->pucOutputBuf);
    pstConn->pucOutputBuf = NULL;
    if (pstConn->ucState == M_WAP_TLS_STATE_HANDSHAKE_IN_PROGRESS)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcCloseConn:in handshake\n");)
        WapTls_ProcSendAlert(pstTlsHandle, iConnId,M_WAP_TLS_ALERT_DESC_USER_CANCELED);
    }

    /*in this case,http has received all tls data completely*/
    WapTls_ConnDelete(pstTlsHandle,pstConn->iCidx);
    return 0;
 }

/*==================================================================================================
FUNCTION: 
    WapTls_AddDataNode
CREATE DATE:
    2007-01-19
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Add socket data to a list
ARGUMENTS PASSED:
    pstList : Socket data list
    pvData: data received
    iLength: data length
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_AddDataNode
(
    St_WapTlsSocketList *pstList, 
    WE_CHAR     *pvData, 
    WE_INT32     iLength
)
{
    St_WapTlsData *pstNode = NULL;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_AddDataNode begin\n");)
    
    if( NULL == pstList )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_AddDataNode parameter error\n");)
        return -1;
    }

    pstNode = (St_WapTlsData *)WAP_MALLOC(sizeof(St_WapTlsData));
    if(NULL==pstNode)
    {
        WAP_FREE(pvData);
        return -1;
    }

    pstNode->pstNext = NULL;    
    pstNode->iBuffLength = iLength;
    pstNode->pcBuffer = WAP_MALLOC(iLength);
    WAP_MEMCPY(pstNode->pcBuffer,pvData,iLength);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_AddDataNode data length is %d\n"),iLength);)


    if( NULL == pstList->pstHeader )
    {
        pstList->pstHeader = pstList->pstTail = pstNode;
    }
    else
    {
        pstList->pstTail->pstNext = pstNode;
        pstList->pstTail = pstNode;
    }
   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_AddDataNode end\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetNodeFromList
CREATE DATE:
    2007-01-19
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Get first node from data list to input buffer
ARGUMENTS PASSED:
    pstConn: Tls connection
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_GetNodeFromList(St_WapTlsConn *pstConn)
{
    St_WapTlsData *pstNode = NULL;
    St_WapTlsSocketList *pstList = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetNodeFromList begin\n");)

    pstList = &(pstConn->stSocketData);
    if (NULL == pstList)
    {
        return -1;
    }
    if(NULL != pstList->pstHeader)
    {
        pstNode = pstList->pstHeader;
        pstList->pstHeader = pstNode->pstNext;
        pstNode->pstNext = NULL;
        if (pstNode->iBuffLength > 0)
        {            
            WAP_MEMCPY(pstConn->pucInputBuf,pstNode->pcBuffer,pstNode->iBuffLength);
        }
        pstConn->iInputBytesLeft = pstNode->iBuffLength;
        WAP_FREE(pstNode->pcBuffer);
        WAP_FREE(pstNode);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_GetNodeFromList end with %d\n"),pstConn->iInputBytesLeft);)
        return pstConn->iInputBytesLeft;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetNodeFromList end with no node\n");)

    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DeleteSocketData
CREATE DATE:
    2007-01-19
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Delete socket data list
ARGUMENTS PASSED:
    pstConn: Tls connection
RETURN VALUE:
    -1: err
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DeleteSocketData(St_WapTlsSocketList *pstList)
{
    St_WapTlsData *pstNode = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DeleteSocketData begin\n");)

    if (NULL == pstList)
    {
        return -1;
    }
    while(NULL != pstList->pstHeader)
    {
        pstNode = pstList->pstHeader;
        pstList->pstHeader = pstNode->pstNext;
        pstNode->pstNext = NULL;
        if (pstNode->pcBuffer != NULL)
        {
            WAP_FREE(pstNode->pcBuffer);            
        }           
        WAP_FREE(pstNode);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DeleteSocketData end\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_ProcInput
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process data in input buffer
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    pstConn      : TLS connection 
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void  WapTls_ProcInput
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_INT32 iMinLen = 0;
    WE_INT32 iRes = 0;
    WE_INT32 iRet = 0;
    WE_INT32 iHandShakeLen = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:Begin\n");)

    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:parameter error\n");)
        return;
    }
   /*Input buffer which store the data received from socket is not null*/
    while (pstConn->iInputBytesLeft > 0 )
    {
         switch (pstConn->ucInputState)
        {
            /*Receive CCS or
              receive application data(but doesn't give to http),so didn't parse and decrypt to
              overidde pstConn->stRecRec.pucFrament*/
            case M_WAP_TLS_INPUT_STATE_WAIT_CCS_PROCESSED:
           // case M_WAP_TLS_INPUT_STATE_WAIT_HTTP_READY:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:state is %d\n"),pstConn->ucInputState);)
                 return;

            /*First Require five bytes to parse, so can get the content-type, version and length*/
            case M_WAP_TLS_INPUT_STATE_START:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:M_WAP_TLS_INPUT_STATE_START\n");)
                pstConn->iInputBytesRequired = M_WAP_TLS_RECORD_HEADER_SIZE;
                pstConn->iInputBytesRead = 0; 
                /*fall throgh*/
            case M_WAP_TLS_INPUT_STATE_READING_HEADER:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:M_WAP_TLS_INPUT_STATE_READING_HEADER\n");)
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:Bytes left is %d\n"),pstConn->iInputBytesLeft);)
                iMinLen = MIN(pstConn->iInputBytesRequired, pstConn->iInputBytesLeft);
                if (iMinLen > 0)
                {
                    WAP_MEMCPY(pstConn->aucInputHeaderBuf + pstConn->iInputBytesRead,
                                pstConn->pucInputBuf + pstConn->iInputBufPos, iMinLen);
                    pstConn->iInputBytesRead += iMinLen;
                    pstConn->iInputBytesRequired -= iMinLen;
                    pstConn->iInputBufPos += iMinLen;
                    pstConn->iInputBytesLeft -= iMinLen;
                }
                /*No data in the input buffer, contiune read from socket*/
                if (pstConn->iInputBytesLeft == 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:first Get from list\n");)
                    pstConn->iInputBufPos = pstConn->iInputBytesLeft = 0;
                    iRet = WapTls_GetNodeFromList(pstConn);
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:first get %d\n"),iRet);)
                }

                 /*data in the input buffer is not enough for required*/
                if (pstConn->iInputBytesRequired > 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:required not full 1\n");)
                    pstConn->ucInputState = M_WAP_TLS_INPUT_STATE_READING_HEADER;
                    break;
                }

                /*
                  [IN] aucInputHeaderBuf
                  [OUT] stRecvRec
                */
                iRet = WapTls_ParseRecordHeader(&(pstConn->stRecvRec), pstConn->aucInputHeaderBuf);
                if (iRet != -1)
                {
                    iHandShakeLen = pstConn->stRecvRec.usLength;
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:handshake length is %d\n"),iHandShakeLen);)
                    /*free in WapTls_ProcInputRecord*/
                    pstConn->stRecvRec.pucFragment = (WE_UCHAR*)WAP_MALLOC(iHandShakeLen);
                    if (NULL == pstConn->stRecvRec.pucFragment)
                    {
                        return;
                    }
                    /*requier get the handshake protocol data*/
                    pstConn->iInputBytesRequired = iHandShakeLen;
                    pstConn->iInputBytesRead = 0;
                }
                /*fall througth*/
            case M_WAP_TLS_INPUT_STATE_READING_DATA:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:M_WAP_TLS_INPUT_STATE_READING_DATA\n");)
                iMinLen = MIN(pstConn->iInputBytesRequired, pstConn->iInputBytesLeft);

                if (iMinLen > 0)
                {
                    WAP_MEMCPY(pstConn->stRecvRec.pucFragment + pstConn->iInputBytesRead,
                                 pstConn->pucInputBuf + pstConn->iInputBufPos, iMinLen);

                    pstConn->iInputBytesRead += iMinLen;
                    pstConn->iInputBytesRequired -= iMinLen;
                    pstConn->iInputBufPos += iMinLen;
                    pstConn->iInputBytesLeft -= iMinLen;
                }
                

                /*no data in the input buffer*/
                if (pstConn->iInputBytesLeft == 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:second get form list\n");)
                    pstConn->iInputBufPos = pstConn->iInputBytesLeft = 0;
                    iRet = WapTls_GetNodeFromList(pstConn);
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:second get %d\n"),iRet);)
                }
               
                /*data in the input buffer is not enough for required*/
                if (pstConn->iInputBytesRequired > 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:required not full 2\n");)
                    pstConn->ucInputState = M_WAP_TLS_INPUT_STATE_READING_DATA;
                    break;
                }

                /*data required is full*/
                pstConn->stRecvRec.uiSeqnum = pstConn->stRead.iSeqnum++;

                /*have known the content-type after WapTls_ParseRecordHeader */
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:record seqnum is %d\n"),pstConn->stRecvRec.uiSeqnum);)
                iRes = WapTls_DecryptRecord(pstTlsHandle,pstConn, &(pstConn->stRecvRec));
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:after decrypt record length is %d\n"),pstConn->stRecvRec.usLength);)
                if (iRes < 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:decrypt error\n");)
                    WapTls_ProcErr(pstTlsHandle,pstConn->iConnId,G_WAP_ERR_TLS_DECRYPTION_FAILED, -iRes);
                    return;
                }
                
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:Process one record\n");)
                /*process record content according to content-type*/
                iRes = WapTls_ProcInputRecord(pstTlsHandle, pstConn);
                if (iRes< 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:Process one record error\n");)
                    return;
                }
                else
                {                    
                    pstConn->ucInputState = (WE_UCHAR)iRes;
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:Input state is %d\n"),pstConn->ucInputState);)
                    
                }
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:bytes left is %d\n"),pstConn->iInputBytesLeft);)
    } /*end while*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:bytes left %d\n"),pstConn->iInputBytesLeft);)
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:Input State %d\n"),pstConn->ucInputState);)
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInput:Connection state %d\n"),pstConn->ucState);)
   
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInput:End\n");)
} 
/*==================================================================================================
FUNCTION: 
    WapTls_ProcReceivedAlert
CREATE DATE:
    2007-01-11
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    TLS receive the alert message 
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    pstConn      : TLS connection 
    iAlertLevel  : Alert Level
    iAlertDesc   : Alert Description
RETURN VALUE:

USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ProcReceivedAlert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_INT32 iAlertLevel,
    WE_INT32 iAlertDesc
) 
{
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_SecDisableSessionPara stDisableSess = {0};
    St_SecSessionOpt stSessionOpt = {G_SEC_ENABLE_SESSION,NULL};
#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert:begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert:parameter error\n");)
        return -1;
    }


    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcRecvAlert received alert (%d, %d)\n"),iAlertLevel,iAlertDesc);)

    if (iAlertLevel == M_WAP_TLS_ALERT_LEVEL_FATAL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert:FATAL\n");)
        if (pstConn->stRead.iMasterSecretId >= 0)
        {
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        stDisableSess.iMasterSecretId = pstConn->stRead.iMasterSecretId;
        stSessionOpt.eType = G_SEC_DISABLE_SESSION;
        stSessionOpt.hValue = &(stDisableSess);
        ISecW_SessionOpts(pstTlsHandle->hSecHandle,&stSessionOpt);
#else        
        ISecW_DisableSession( pstTlsHandle->hSecHandle, pstConn->stRead.iMasterSecretId );

#endif
         return -1;
        }
    }
    else if (iAlertDesc == M_WAP_TLS_ALERT_DESC_USER_CANCELED)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert Free handshake\n");)
        pstConn->iSecId = pstConn->pstHandshake->iSecurityId;            
        WapTlsHm_FreeHandShake(pstTlsHandle,pstConn->pstHandshake);
        pstConn->pstHandshake = NULL;
        return 0;
    }
    else if (iAlertDesc == M_WAP_TLS_ALERT_DESC_CLOSE_NOTIFY)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert:send alert\n");)
        /* because when client send close alert to server, server doesn't reponse with close alert,
           but in rfc2246, it describles we need send
           WapTls_SendAlert(pstTlsHandle,pstConn,M_WAP_TLS_ALERT_LEVEL_WARNING,M_WAP_TLS_ALERT_DESC_CLOSE_NOTIFY);
        
        */
  
        return -1;
        
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcRecvAlert other alert\n");)
    }

    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcSendAlert
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send alert to server
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    iCidx :   Connection id
    iAlertDesc   : Alert Description
RETURN VALUE:
    -1: error
    0 :success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ProcSendAlert
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId,
    WE_INT32 iAlertDesc
) 
{
    St_WapTlsConn *pstConn = NULL;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendAlert begin\n");)

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendAlert TLS handle NULL\n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if ((NULL == pstConn )|| (M_WAP_TLS_STATE_CLOSING == pstConn->ucState)
                           || (M_WAP_TLS_STATE_SHUTDOWN== pstConn->ucState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcSendAlert Connection null or state closeing\n");)
        return -1;
    }
    
    iRet = WapTls_SendAlert(pstTlsHandle,pstConn,M_WAP_TLS_ALERT_LEVEL_WARNING,iAlertDesc);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcSendAlert Send result is %d\n"),iRet);)
    return iRet;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_SendAlert
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Send alert to server
ARGUMENTS PASSED:
    pstTlsHandle : TLS handle
    pstConn      : TLS connection 
    iAlertLevel  : Alert Level
    iAlertDesc   : Alert Description
RETURN VALUE:
    Err: -1
    else 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_SendAlert
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_INT32 iAlertLevel,
    WE_INT32 iAlertDesc
) 
{
    St_WapTlsRecord *pstRecord = NULL;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert begin\n");)

    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert parameter error\n");)
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_SendAlert received alert (%d, %d)\n"),iAlertLevel,iAlertDesc);)
#ifdef WAP_CONFIG_SSL
    if (pstConn->stProtocolVersion.ucMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
    {
        iAlertDesc = WapSSL_ConvertAlertDesc(iAlertDesc);
        if (iAlertDesc < 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert SSL error\n");)
            return -1;
        }
    }
#endif

    pstRecord = WapTls_NewRecord(M_WAP_TLS_CONTENT_TYPE_ALERT,
                                  pstConn->stWrite.iSeqnum++,
                                  pstConn->stProtocolVersion);    

    if (pstRecord == NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert Record NULL\n");)
        return -1;
    }
    pstRecord->usLength = 2;
    pstRecord->pucFragment = (WE_UCHAR*)WAP_MALLOC(2);
    if (NULL == pstRecord->pucFragment)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert malloc error\n");)
        return -1;
    }
    pstRecord->pucFragment[0] = (WE_CHAR)iAlertLevel;
    pstRecord->pucFragment[1] = (WE_CHAR)iAlertDesc;

    iRet = WapTls_EncryptRecord(pstTlsHandle,pstConn,pstRecord);
    if (iRet < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_SendAlert Encrypt error\n");)
        WapTls_DeleRecord(pstRecord);
        return iRet;
    }
    WapTls_AddRecToList(&(pstConn->pstSendRecs), pstRecord);
    iRet = WapTls_ProcSendOneRecord( pstTlsHandle,pstConn);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_SendAlert end with %d\n"),iRet);)
    return iRet;

}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcInputRecord
CREATE DATE:
    2007-01-11
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    process record layer data according to the content-type
ARGUMENTS PASSED:
    pstConn : connection
    pstTlsHandle : TLS module handle
RETURN VALUE:
    -1: parameter err
    >0: record layer input state
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    content-type : handshake,ccs,alert,application data
==================================================================================================*/
static WE_INT32 WapTls_ProcInputRecord
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
)
{
    St_TlsRespData stResp = {0};
    WE_INT32 iAlertLevel = 0;
    WE_INT32 iAlertDesc = 0;
    WE_INT32 iRet = 0;

    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        return -1;
    }

    /*protocol version is not match*/
    if ( (pstConn->stRecvRec.stProVer.ucMajor != pstConn->stProtocolVersion.ucMajor)
           || (pstConn->stRecvRec.stProVer.ucMinor > M_WAP_TLS_PROTOCOL_VERSION_MINOR))
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInputRecord:protocol version is not match\n");)
         if (pstConn->stRecvRec.pucFragment != NULL)
         {
            WAP_FREE(pstConn->stRecvRec.pucFragment);
            pstConn->stRecvRec.pucFragment = NULL;
         }

         WapTls_ProcErr(
                           pstTlsHandle,
                           pstConn->iConnId,
                           G_WAP_ERR_TLS_BAD_RECORD,
                           M_WAP_TLS_ALERT_DESC_PROTOCOL_VERSION
                       );
         return -1;
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInputRecord:Content-type is %d\n"),pstConn->stRecvRec.ucContentType);)
    switch (pstConn->stRecvRec.ucContentType)
    {
        case M_WAP_TLS_CONTENT_TYPE_CCS:
        {
            TlsMsg_PostMsg(
                              pstTlsHandle->hMsgHandle,
                              E_WAP_MSG_MODULEID_TLS,
                              M_WAP_TLS_HM_MSG_RECV_CCS,
                              pstConn->iConnId,
                              0,0,0,NULL,NULL
                          );
            if (pstConn->stRecvRec.pucFragment != NULL)
            {
                WAP_FREE(pstConn->stRecvRec.pucFragment);/*only one byte to store ccs*/
                pstConn->stRecvRec.pucFragment = NULL;
                pstConn->stRecvRec.usLength = 0;
            }
            return M_WAP_TLS_INPUT_STATE_WAIT_CCS_PROCESSED;
        }
        case M_WAP_TLS_CONTENT_TYPE_ALERT:/*only two bytes to store alert*/
        {
            if (pstConn->stRecvRec.usLength != 2)
            {
                if (pstConn->stRecvRec.pucFragment != NULL)
                {
                    WAP_FREE(pstConn->stRecvRec.pucFragment);
                    pstConn->stRecvRec.pucFragment = NULL;
                } 
                WapTls_ProcErr(pstTlsHandle,
                               pstConn->iConnId,
                               G_WAP_ERR_TLS_BAD_RECORD,
                               M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
                return -1;
            }
            iAlertLevel = (WE_INT32)(pstConn->stRecvRec.pucFragment[0]);
            iAlertDesc = (WE_INT32)(pstConn->stRecvRec.pucFragment[1]);
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInputRecord:Alert level is %d\n"),iAlertLevel);)
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ProcInputRecord:Alert desc is %d\n"),iAlertDesc);)
            if (pstConn->stRecvRec.pucFragment != NULL)
            {
                WAP_FREE(pstConn->stRecvRec.pucFragment);
                pstConn->stRecvRec.pucFragment = NULL;
                pstConn->stRecvRec.usLength = 0;
            } 
            iRet = WapTls_ProcReceivedAlert(pstTlsHandle,pstConn,iAlertLevel,iAlertDesc);
            if (iRet == -1)
            {
                return -1;
            }
            break;
        }
        /*handshake data, so need handshake layer to process*/
        case M_WAP_TLS_CONTENT_TYPE_HANDSHAKE:
        {
            TlsMsg_PostMsg(
                 pstTlsHandle->hMsgHandle,
                 E_WAP_MSG_MODULEID_TLS,
                 M_WAP_TLS_HM_MSG_RECV_HSHK_MSG,
                 pstConn->iConnId,0,                 
                 pstConn->stRecvRec.usLength,0,pstConn->stRecvRec.pucFragment,NULL); 
            /*pstConn->stRecvRec.pucFragment free in WapTlsHm_ProcInput*/
            pstConn->stRecvRec.pucFragment = NULL;
            pstConn->stRecvRec.usLength = 0;
            break;
        }
        /*socket receive application data*/
        case M_WAP_TLS_CONTENT_TYPE_DATA:
        {
            /*wang shuhua add at 20070119 for ytht.net get 24 byts, but no real http data*/
            if (pstConn->stRecvRec.usLength == 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInputRecord:no real http data\n");)
                WAP_FREE(pstConn->stRecvRec.pucFragment);
                pstConn->stRecvRec.pucFragment = NULL;
                return M_WAP_TLS_INPUT_STATE_START;
            }
            else
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ProcInputRecord:Notify http has data\n");)

                stResp.iConnId= pstConn->iConnId;
                stResp.iRespLen = pstConn->stRecvRec.usLength;
                stResp.pvRespData = (WE_VOID*)pstConn->stRecvRec.pucFragment;

               ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_DATA_TO_APP,
                                                           (WE_VOID*)&stResp,
                                                           pstTlsHandle->hPrivData); 
               WAP_FREE(pstConn->stRecvRec.pucFragment);
               pstConn->stRecvRec.usLength = 0;
               pstConn->stRecvRec.pucFragment = NULL;
               break;
            }
        }
        default:
            WapTls_ProcErr(pstTlsHandle,pstConn->iConnId, 
                            G_WAP_ERR_TLS_BAD_RECORD,
                            M_WAP_TLS_ALERT_DESC_DECODE_ERROR);
            return -1;
             
    } /*end switch*/
    return M_WAP_TLS_INPUT_STATE_START;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcErr
CREATE DATE:
    2007-06-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process data from socket
ARGUMENTS PASSED:
    hTlsHandle: TLS module handle
    iConnId  : Connection id
    pvData : data
    iDataLen : data length
RETURN VALUE:G
    Err: -1
    Success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
WE_INT32 WapTls_ProcDataFromSocket
(
    WE_HANDLE hTlsHandle,
    WE_INT32 iConnId,
    WE_VOID* pvData,
    WE_INT32 iDataLen
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    St_WapTlsConn *pstConn = NULL;
    WE_INT32 iRet =0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcDataFromSocket begin\n");)

    if (NULL == hTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcDataFromSocket TLS handle NULL\n");)        
        return -1;
    }
    pstTlsHandle = (St_WapTlsHandle*)hTlsHandle;
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);
    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcDataFromSocket Connection NULL\n");)        
        return -1;
    } 
    WapTls_AddDataNode(&pstConn->stSocketData, pvData, iDataLen);
    if (pstConn->iInputBytesLeft == 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSocketRecvReady get first node\n");)
        WapTls_GetNodeFromList(pstConn);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSocketRecvReady start process\n");)
    WapTls_ProcInput(pstTlsHandle,pstConn);
     
    return iRet;

}
/*==================================================================================================
FUNCTION: 
    WapTls_ProcErr
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process error case
ARGUMENTS PASSED:
    hTlsHandle: TLS module handle
    iCidx    : Connection id
    iErrCode : Err code
    iErrDesc : Err description
RETURN VALUE:G
    Err: -1
    Success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    iErrDesc < 0 : decrypt error  
==================================================================================================*/
WE_INT32 WapTls_ProcErr
(
    WE_HANDLE hTlsHandle,
    WE_INT32 iConnId,
    WE_INT32 iErrCode,
    WE_INT32 iErrDesc
)
{
    St_WapTlsHandle   *pstTlsHandle = NULL;
    St_WapTlsConn         *pstConn = NULL;   
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_SecDisableSessionPara stDisableSess = {0};
    St_SecSessionOpt stSessionOpt = {G_SEC_ENABLE_SESSION,NULL};
#endif
    St_TlsRespData stResp = {0};

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr begin\n");)

    pstTlsHandle = (St_WapTlsHandle*)hTlsHandle;
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr TLS handle NULL\n");)
        return -1;
    }
     pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if ((NULL == pstConn) || (pstConn->ucState == M_WAP_TLS_STATE_CLOSING))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr connection error\n");)
        return -1;
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(( "WapTls_ProcErr err is %d %d\n"),iErrCode,iErrDesc);)
    if (iErrDesc > 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr Send alert\n");)
        WapTls_SendAlert(pstTlsHandle,pstConn,M_WAP_TLS_ALERT_LEVEL_FATAL,iErrDesc);
    }

    /*
      Init: -1
      When create client key exchange, generate pre_master secret id
    */
    if (pstConn->stRead.iMasterSecretId >= 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr Post CM failed\n");)
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        stDisableSess.iMasterSecretId = pstConn->stRead.iMasterSecretId;
        stSessionOpt.eType = G_SEC_DISABLE_SESSION;
        stSessionOpt.hValue = &(stDisableSess);
        ISecW_SessionOpts(pstTlsHandle->hSecHandle,&stSessionOpt);
#else
        (WE_VOID)ISecW_DisableSession( (WE_HANDLE)(pstTlsHandle->hSecHandle), pstConn->stRead.iMasterSecretId );
#endif
         stResp.iConnId= pstConn->iConnId;
         stResp.iErrCode = iErrCode;
        
        ((Fn_ITlsRespCb)(pstTlsHandle->hcbRespunc))(E_TLS_DATA_TO_APP,
                                                    (WE_VOID*)&stResp,
                                                    pstTlsHandle->hPrivData); 
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcErr end\n");)
    return 0;
}

#endif


