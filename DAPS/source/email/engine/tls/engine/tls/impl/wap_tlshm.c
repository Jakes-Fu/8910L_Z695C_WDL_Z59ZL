/*==================================================================================================
    FILE NAME   : wap_tlshm.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
          Handshake layer interface  
       
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
#include "wap_tlshmcore.h" 
#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h"
#endif

#include "itls.h"


/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static WE_INT32 WapTlsHm_ProcRecvCCS
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
);
static WE_INT32 WapTlsHm_ProcInput
(
   St_WapTlsHandle *pstTlsHandle,
   WE_INT32 iConnId,
   WE_UCHAR *pucData,
   WE_INT32  iDataLen
);
static WE_INT32 WapTlsHm_ProcHandShakeMsg
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR      *pucMsg,
    WE_UINT32       uiMsgLen
);
static WE_INT32 WapTlsHm_ProcRecvCCS
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
);

static WE_INT32 WapTlsHm_ProcCCSProcessed
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
);
static void WapTlsHm_ProcSavedMsgs
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn *pstConn
);
static void WapTlsHm_SaveMsgToList
(
    St_WapTlsConn *pstConn,
    St_TlsMsg *pstMsg
);

/*==================================================================================================
FUNCTION: 
    WapTlsHm_MsgProcess
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    message process function in handshake layer
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstWapMsgData : message 
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32  WapTlsHm_MsgProcess
( 
    WE_HANDLE     hTlsHandle,
    St_TlsMsg     *pstWapMsgData 
)
{
    St_WapTlsConn *pstConn = NULL;
    St_WapTlsHandle *pstTlsHandle = NULL;
    WE_INT32 iRet = -1;

    if (NULL == hTlsHandle || NULL == pstWapMsgData)
    {
        return -1;
    }
    
    pstTlsHandle = (St_WapTlsHandle*)hTlsHandle;
    pstConn = WapTls_FindConnById(pstTlsHandle, pstWapMsgData->iParam1);
    if (NULL == pstConn)
    {
        return -1;
    }

    switch (pstWapMsgData->uiMsgType)
    {
        case M_WAP_TLS_HM_MSG_START:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_START-- \n");)            
            WapTlsHm_ProcStartHandShake (pstTlsHandle,pstWapMsgData->iParam1);
            break;
        case M_WAP_TLS_HM_MSG_RECV_HSHK_MSG:
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_RECV_HSHK_MSG-- \n");)            
                if (NULL == pstConn)
                {
                    WAP_FREE(pstWapMsgData->pvParam1);                    
                }
                else if ((pstConn->pstHandshake == NULL) //new handshake
                         || (pstConn->pstHandshake->ucState <= M_WAP_TLS_HM_STATE_WAIT_FINISHED))/*Wait server PDU*/
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess process meessage\n");)
                    WapTlsHm_ProcInput(pstTlsHandle,pstWapMsgData->iParam1,pstWapMsgData->pvParam1,pstWapMsgData->uiParam1);
                }
                else /*wait sec response*/
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess save message \n");)
                    WapTlsHm_SaveMsgToList(pstConn,pstWapMsgData);
                    return 0;
                }
                break;
            }
        case M_WAP_TLS_HM_MSG_RECV_CCS:
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_RECV_CCS-- \n");)            
                if ((pstConn != NULL) && (pstConn->pstHandshake != NULL))
                {
                    if (pstConn->pstHandshake->ucState <= M_WAP_TLS_HM_STATE_WAIT_FINISHED)
                    {
                        WapTlsHm_ProcRecvCCS(pstTlsHandle,pstWapMsgData->iParam1);
                    }
                    else
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess save message to list\n");)
                        WapTlsHm_SaveMsgToList(pstConn, pstWapMsgData);
                        return 0;
                    }
                }
                break;
            }
        case M_WAP_TLS_HM_MSG_CCS_PROCESSED:
              WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_CCS_PROCESSED-- \n");)            
              iRet = WapTlsHm_ProcCCSProcessed(pstTlsHandle,pstWapMsgData->iParam1);
              break;
        /*-----------------------message from sec -------------------------------------start*/            
        case M_WAP_TLS_HM_MSG_PEER_LOOKUP_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_PEER_LOOKUP_RESP-- \n");)            
            WapTlsHm_PeerLookUpRespProc(pstTlsHandle,pstConn,pstWapMsgData->pvParam1);
            break;
        case M_WAP_TLS_HM_MSG_SESSION_FETCH_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_SESSION_FETCH_RESP-- \n");)            
            iRet = WapTlsHm_StoreSessionData(pstTlsHandle,pstConn,pstWapMsgData->pvParam1);
            iRet = WapTlsHm_CreateClientHello(pstTlsHandle,pstConn);
            break;
        case M_WAP_TLS_HM_MSG_GET_CIPHER_SUITES_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_GET_CIPHER_SUITES_RESP-- \n");)            
            iRet = WapTlsHm_ProcSendClientHello(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            break;
        case M_WAP_TLS_HM_MSG_SERVER_CERT_VERIFY_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_SERVER_CERT_VERIFY_RESP-- \n");)            
            iRet = WapTlsHm_CheckCertVertifyRespProc(pstTlsHandle, pstConn,pstWapMsgData->uiParam1);
            if (iRet == -1)
            {
                break;
            }
            WapTlsHm_CreateClientKeyExch(pstTlsHandle,pstConn);
            break;
        case M_WAP_TLS_HM_MSG_CREATE_KEY_EXCH_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_CREATE_KEY_EXCH_RESP-- \n");)            
            iRet = WapTlsHm_ClientKeyExchRespProc(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            if (iRet == -1)
            {
                break;
            }
            iRet = WapTlsHm_CreateClientCert(pstTlsHandle,pstConn);
            if (iRet == -1) /*No nedd client cert*/
            {
                WapTlsHm_SendClientKeyExch(pstTlsHandle,pstConn);
                break;
            }

            break;
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case M_WAP_TLS_HM_MSG_GET_CLIENT_CERT_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_GET_CLIENT_CERT_RESP-- \n");)            
            iRet = WapTlsHm_ProcSendClientCert(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            if (iRet == -1)
            {
                break;
            }
            WapTlsHm_SendClientKeyExch(pstTlsHandle,pstConn);
            break;
#endif            
        case M_WAP_TLS_HM_MSG_CLIENT_CERT_SIGNATURE_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_CLIENT_CERT_SIGNATURE_RESP-- \n");)            
            iRet = WapTlsHm_ProcSendCertVerify(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            if (iRet == -1)
            {
                break;
            }
            iRet = WapTlsHm_CreatePrf(pstTlsHandle,pstConn);
            break;
        case M_WAP_TLS_HM_MSG_PRF_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----M_WAP_TLS_HM_MSG_PRF_RESP-- \n");)            
            WapTlsHm_ProcPrfResp(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            break;
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case M_WAP_TLS_HM_MSG_SELECT_SEC_DIALOG_REQ:            
            WapTlsHm_NotifyAppSelect(pstTlsHandle,pstConn,pstWapMsgData->pvParam1);
            break;
#endif            
#ifdef WAP_CONFIG_SSL            
        case M_WAP_TLS_HM_MSG_SSL_GET_MASTER_SEC_RESP:
        //    WapSSLHm_ProcSSLSecRes(pstTlsHandle, pstConn,pstWapMsgData->pvParam1);
            break;
#endif
        /*-------------------msg from sec---------------------end------------------*/    
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case M_WAP_TLS_HM_MSG_SEC_SELECTED_DIALOG_RESULT:
            WapTlsHm_ContiuneNego(pstTlsHandle,pstConn,pstWapMsgData->pvParam1,pstWapMsgData->uiParam1);
            break;
#endif            
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case M_WAP_TLS_HM_MSG_PROC_TRUST_ERROR:
            if (NULL == pstConn->pstHandshake)
            {
                return -1;
            }
            else
            {
            if (pstConn->pstHandshake->ucState == M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM)
            {
                if (pstWapMsgData->iParam2== 1)
                {
                    /*wsh add at 20070711*/
                    pstConn->iOverride |= pstConn->pstHandshake->uiAlert;
                    
                    WapTlsHm_CreateClientKeyExch(pstTlsHandle,pstConn);
                }
                else
                {
                    WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_CANCEL, 
                        M_WAP_TLS_ALERT_DESC_USER_CANCELED);
                }
            }
            else
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, pstConn->pstHandshake->uiAlert, 
                        pstConn->pstHandshake->uiAlertDesc);

            }
            }
            break;
        case M_WAP_TLS_HM_MSG_PROC_CLIENT_AUTH_ERROR:
            if (NULL == pstConn->pstHandshake)
            {
                return -1;
            }
            else
            {
            if (pstConn->pstHandshake->ucState == M_WAP_TLS_HM_WAIT_CLIENT_USER_CERT)
            {
                iRet = WapTlsHm_ProcGetAppCertResp(pstTlsHandle, pstConn,
                                                    pstWapMsgData->uiParam1,
                                                    pstWapMsgData->uiParam2,
                                                    pstWapMsgData->pvParam1, //Get client cert resp
                                                    pstWapMsgData->pvParam2);
                if (iRet == -1)
                {
                    break;
                }
                WapTlsHm_SendClientKeyExch(pstTlsHandle,pstConn);
                break;
                }    
            }
        case M_WAP_TLS_HM_MSG_CANCLE_PROC_CLIENT_AUTH:  
            if (NULL == pstConn->pstHandshake)
            {
                return -1;
            }
            else
            {
            if (pstConn->pstHandshake->ucState == M_WAP_TLS_HM_WAIT_CLIENT_USER_CERT)
            {
                    WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn, G_WAP_ERR_TLS_CANCEL, 
                        M_WAP_TLS_ALERT_DESC_USER_CANCELED);
                }
            }
            break;
#endif        
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_MsgProcess receive----Unknown message-- \n");)
            break;
    }
    WapTlsHm_ProcSavedMsgs(pstTlsHandle,pstConn);
    return 0;       
    
}

/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcInput
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Construct one completely handshake record by the data passed in
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id
    pucData: data from recv_rec.pcframent
    iDataLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    pucData may contain more than one record data
    or less than one, in this function ,every time process one record to WapTlsHm_ProcHandShakeMsg
==================================================================================================*/
static WE_INT32 WapTlsHm_ProcInput
(
   St_WapTlsHandle *pstTlsHandle,
   WE_INT32 iConnId,
   WE_UCHAR *pucData,
   WE_INT32  iDataLen
)
{
    St_WapTlsConn *pstConn = NULL;
    WE_INT32 iBytesLeft = 0;
    WE_UCHAR *pucPointer = NULL;
    WE_INT32 iMinLen = 0;
    WE_UCHAR *pucTmp = NULL;
    WE_INT32  iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcInput begin\n");) 

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcInput parameter error\n");) 
        WAP_FREE(pucData);
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcInput no connection\n");) 
        WAP_FREE(pucData);
        return -1;
    }
    iBytesLeft = iDataLen;
    pucPointer = pucData;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcInput data left is %d \n"),iBytesLeft);) 

    while (iBytesLeft > 0)
    {
        switch (pstConn->ucHmInputState)// state of handshake layer process data
        {
            case M_WAP_TLS_HM_INPUT_STATE_START:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcInput M_WAP_TLS_INPUT_STATE_START\n");) 
                pstConn->iHmInputBytesRequired = M_WAP_TLS_HANDSHAKE_HEADER_SIZE;
                pstConn->iHmInputBytesRead = 0;
                /*fall through*/
            case M_WAP_TLS_HM_INPUT_STATE_READING_HEADER:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcInput M_WAP_TLS_INPUT_STATE_READING_HEADER\n");) 
                iMinLen = MIN(pstConn->iHmInputBytesRequired,iBytesLeft);
                if (iMinLen > 0)
                {
                    /*copy header to aucHmInputHeaderBuf*/
                    WAP_MEMCPY(pstConn->aucHmInputHeaderBuf + pstConn->iHmInputBytesRead,
                                pucPointer, iMinLen);
                    pstConn->iHmInputBytesRead += iMinLen;
                    pstConn->iHmInputBytesRequired -= iMinLen;
                    pucPointer += iMinLen; /*move pointer*/
                    iBytesLeft -= iMinLen;
                }
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcInput data left is %d \n"),iBytesLeft);) 
                /*required is not full*/
                if (pstConn->iHmInputBytesRequired > 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput required is not full break\n");) 
                    pstConn->ucHmInputState = M_WAP_TLS_HM_INPUT_STATE_READING_HEADER;
                    break;
                }

                pucTmp = pstConn->aucHmInputHeaderBuf;
                pstConn->tiHmInputMsgLen = (((WE_UINT32)pucTmp[1])<< 16) | (((WE_UINT32)pucTmp[2])<< 8)
                                                         | pucTmp[3];

                pstConn->iHmInputBytesRequired = pstConn->tiHmInputMsgLen;
                pstConn->iHmInputBytesRead = 0;
                
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcInput record length is %d \n"),pstConn->tiHmInputMsgLen);) 

                /*just one record*/
                if ((pstConn->tiHmInputMsgLen == (M_WAP_TLS_UINT24)iBytesLeft)
                      && (iBytesLeft + M_WAP_TLS_HANDSHAKE_HEADER_SIZE == iDataLen))
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput one record\n");) 
                    pstConn->pucHmInputMsg = pucData;
                    pucData = NULL;
                }
                else /*more than one record, copy first record, data is large */
                {
                     WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput more than one record\n");) 
                     /*free in this function*/
                     pstConn->pucHmInputMsg = WAP_MALLOC(pstConn->tiHmInputMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE);
                     if (NULL == pstConn->pucHmInputMsg)
                     {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput malloc error\n");)
                        WAP_FREE(pucData);
                        return -1;
                     }
                }
                /*fall throuth*/
           case M_WAP_TLS_HM_INPUT_STATE_READING_DATA:
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcInput M_WAP_TLS_INPUT_STATE_READING_DATA (required,left) is %d ,%d\n"),pstConn->iHmInputBytesRequired,iBytesLeft);) 
   
                iMinLen = MIN(pstConn->iHmInputBytesRequired, iBytesLeft);
                /***************************************************************************
                  if have more than one record, bytes required < iBytesLeft, so after copy, 
                  bytes required is 0,bytes left > 0
                  if have one record, pucData == NULL; iMinLen = iBytesLeft = pstConn->ucHmInputBytesRequired,
                  after copy,bytes required is 0
                  if less than one record, bytes required  > iBytesLeft, so after copy,
                  bytes required > 0, so contiune wating data coming
                *************************************************************************/
                if (iMinLen > 0)
                {
                    if (pucData != NULL) /*bytes left is larger than required and have more than one record*/
                    {
                        WAP_MEMCPY(pstConn->pucHmInputMsg + pstConn->iHmInputBytesRead + M_WAP_TLS_HANDSHAKE_HEADER_SIZE,
                                        pucPointer,iMinLen);
                        pucPointer += iMinLen; /*Move pointer*/
                    }
                    pstConn->iHmInputBytesRead += iMinLen;
                    pstConn->iHmInputBytesRequired -= iMinLen;
                    iBytesLeft -= iMinLen;
                }
                /*data is not full*/
                if (pstConn->iHmInputBytesRequired > 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput requird is not full break\n");) 
                    pstConn->ucHmInputState = M_WAP_TLS_HM_INPUT_STATE_READING_DATA;
                    break;
                }
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput proc handshake message\n");) 
                /*is a completely record, start parse,buffer->structure*/
             //   WAP_MEMCPY(pstConn->pucHmInputMsg, pstConn->aucHmInputHeaderBuf, M_WAP_TLS_HANDSHAKE_HEADER_SIZE);
                iRet = WapTlsHm_ProcHandShakeMsg(pstTlsHandle, 
                                                 pstConn, 
                                                 pstConn->pucHmInputMsg + M_WAP_TLS_HANDSHAKE_HEADER_SIZE ,
                                                 pstConn->tiHmInputMsgLen);
                if (iRet >= 0)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput proc handshake layer success\n");)
                    WAP_MEMCPY(pstConn->pucHmInputMsg, pstConn->aucHmInputHeaderBuf, M_WAP_TLS_HANDSHAKE_HEADER_SIZE);
                    WapTlsHm_UpdateHandShakeHashes( pstTlsHandle, 
                                                    pstConn,
                                                    pstConn->pstHandshake,
                                                    pstConn->pucHmInputMsg, /*Contain the four bytes headr*/
                                                    pstConn->tiHmInputMsgLen + M_WAP_TLS_HANDSHAKE_HEADER_SIZE);

                }
                pstConn->ucHmInputState = M_WAP_TLS_HM_INPUT_STATE_START;
                WAP_FREE(pstConn->pucHmInputMsg);
                pstConn->pucHmInputMsg = NULL;
                
        }/*end switch*/
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcInput bytes left is %d\n"),iBytesLeft);)
    }/*end while*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcInput end while \n");)
    if (NULL != pucData)
    {
        WAP_FREE(pucData);
        pucData = NULL;
    }
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcHandShakeMsg
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    process handshake protocol of one record
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    pstConn : connection 
    pucMsg: data of handshake protocol with out four bytes header
    iMsgLen: data length
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   process accroding to the handshake type
==================================================================================================*/

static WE_INT32 WapTlsHm_ProcHandShakeMsg
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR      *pucMsg,
    WE_UINT32      uiMsgLen
)
{
    St_WapTlsHandshake *pstHandShake = NULL;
    WE_UINT8           ucMsgType = '\0';
    WE_INT32           iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg begin\n");)
    if ((NULL == pstTlsHandle) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcHandShakeMsg parmeter error\n");)            
        return -1;
    }
    
    ucMsgType = pstConn->aucHmInputHeaderBuf[0];
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcHandShakeMsg message type is %d\n"),ucMsgType);)            

    pstHandShake = pstConn->pstHandshake;
    if (NULL == pstHandShake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg no handshake\n");)
        if (ucMsgType == M_WAP_TLS_HANDSHK_HELLO_REQUEST)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("hellorequest---message length is %d\n"),uiMsgLen);)
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg start handshake\n");)            
            WapTlsHm_ProcStartHandShake(pstTlsHandle, pstConn->iConnId);
        }
        else
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg unexpected message\n");)
            WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_HANDSHAKE_FAILURE, 
                                        M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
        }
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTlsHm_ProcHandShakeMsg state is %d\n"),pstHandShake->ucState);)            
    switch (pstHandShake->ucState)
    {
        case M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO:
        {
            if (ucMsgType != M_WAP_TLS_HANDSHK_SERVER_HELLO)
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_HANDSHAKE_FAILURE, 
                                                        M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
                return -1;
            }
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("server hello---message length is %d\n"),uiMsgLen);)
            iRet = WapTlsHm_ProcRecvServerHello(pstTlsHandle,pstConn, pucMsg,uiMsgLen);
            if (iRet < 0)
            {
                return -1;
            }
            break;
        }
        case M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE:
        {
            if (ucMsgType == M_WAP_TLS_HANDSHK_CERTIFICATE)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("server cert---message length is %d\n"),uiMsgLen);)
                iRet = WapTlsHm_ProcRecvServerCert(pstTlsHandle,pstConn,pucMsg,uiMsgLen);
                break;
            }
        }
        case M_WAP_TLS_HM_STATE_WAIT_SERVER_KEY_EXCHANGE:
        {
            if (ucMsgType == M_WAP_TLS_HANDSHK_SERVER_KEY_EXCHANGE)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("serverkeyexch---message length is %d\n"),uiMsgLen);)
                iRet = WapTlsHm_ProcRecvServerKeyExch(pstTlsHandle,pstConn,pucMsg,uiMsgLen);
                break;
            }    
        }
        case M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE_REQUEST:
        {
            if (ucMsgType == M_WAP_TLS_HANDSHK_CERTIFICATE_REQUEST)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("cert request---message length is %d\n"),uiMsgLen);)
                iRet = WapTlsHm_ProcRecvCertRequest(pstTlsHandle,pstConn,pucMsg,uiMsgLen); 
                break;
            }
        }
        case M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO_DONE:
        {
            if (ucMsgType == M_WAP_TLS_HANDSHK_SERVER_HELLO_DONE)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("serverhellodone---message length is %d\n"),uiMsgLen);)
                pstHandShake->uiMsgs |= M_BIT_SERVER_HELLO_DONE;                
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg process server hello done,verify cert\n");)
                iRet = WapTlsHm_VerifyCert( pstTlsHandle,pstConn);
                if( 1 == iRet )/*verify cert no need,no server cert*/
                {
                    iRet = WapTlsHm_CreateClientKeyExch( pstTlsHandle,pstConn);
                }
                break;
            }
        }
        case M_WAP_TLS_HM_STATE_WAIT_FINISHED:
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTlsHm_ProcHandShakeMsg M_WAP_TLS_HM_STATE_WAIT_FINISHED\n");)
            if (ucMsgType != M_WAP_TLS_HANDSHK_FINISHED)
            {
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                                    M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
                break;
            }
#ifdef WAP_CONFIG_SSL
            if (pstHandShake->ucFlags & M_WAP_TLS_FLAG_IS_SSL)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("SSLserverfinished---message length is %d\n"),uiMsgLen);)
                WapSSLHm_ProcRecvServerFinished(pstTlsHandle,pstConn,pucMsg,uiMsgLen);
            }
            else
#endif
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("serverfinished---message length is %d\n"),uiMsgLen);)
                WapTlsHm_ProcRecvServerFinished(pstTlsHandle,pstConn,pucMsg,uiMsgLen);
            }
            break;
         }
         default:
         {
            if (ucMsgType != M_WAP_TLS_HANDSHK_HELLO_REQUEST)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("unknown message\n");)
                WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                    M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
            }
            return -1;
         }
    }/*end switch */
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcRecvCCS
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    process handshake protocol of CCS record, from server
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
   process accroding to the handshake type
==================================================================================================*/

static WE_INT32 WapTlsHm_ProcRecvCCS
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS begin\n");)

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS TLSHANDLE is null\n");)
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS no connection\n");)
        return -1;
    }
    if (NULL == pstConn->pstHandshake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS no handshake\n");)
        return -1;
    }
    if (pstConn->pstHandshake->ucState != M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS state not correct\n");)        
        WapTlsHm_ProcHandShakeErr(pstTlsHandle, pstConn,G_WAP_ERR_TLS_HANDSHAKE_FAILURE,M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
        return -1;
    }

    /*connection state convert, after reecive server CCS, all the PDU received from server have been
      encrypted*/
    WapTlsHm_ConfigurePendingRead(pstTlsHandle,pstConn);
    TlsMsg_PostMsg(
                    pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_TLSHM_MSG_CCS_PROCESSED,
                    pstConn->iConnId,
                    0,0,0,NULL,NULL
                  );
    pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_FINISHED;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcRecvCCS end with state M_WAP_TLS_HM_STATE_WAIT_FINISHED\n");)
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcCCSProcessed
CREATE DATE:
    2006-12-20
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    after client send ccs, notify client to creat other handshake protocol
ARGUMENTS PASSED:
    hTlshandle : TLS module handle
    iCidx : connection id
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
==================================================================================================*/
static WE_INT32 WapTlsHm_ProcCCSProcessed
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcCCSProcessed begin\n");)    
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcCCSProcessed parameter error\n");)    
        return -1;
    }
    pstConn = WapTls_FindConnById(pstTlsHandle,iConnId);

    //pstConn = M_WAP_TLS_CONN_GET(pstTlsHandle, iCidx);
    if (pstConn == NULL)
    {
        return -1;
    }
    if (NULL == pstConn->pstHandshake)
    {
        return -1;
    }
#ifdef WAP_CONFIG_SSL
    if (pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_IS_SSL)
    {
        WapSSLHm_CreateClientFinished(pstTlsHandle,pstConn);
    }
    else
#endif
    {
        WapTlsHm_CreateClientFinished(pstTlsHandle,pstConn);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcCCSProcessed end with state M_WAP_TLS_HM_STATE_CREATE_FINISHED\n");) 
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_CREATE_CLIENT_FINISHED;
    }
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcSavedMsgs
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Resend the message store in the message list
ARGUMENTS PASSED:
    pstTlsHandle : TLS module handle
    pstConn : tls connection 
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static void WapTlsHm_ProcSavedMsgs(St_WapTlsHandle *pstTlsHandle,St_WapTlsConn *pstConn)
{
    St_WapTlsMsgList *pstMsgList = NULL;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcSavedMsgs begin\n");) 
    if (NULL == pstConn)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcSavedMsgs parameter error\n");) 
        return;
    }
    if (NULL == pstConn->pstHandshake)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcSavedMsgs parameter error\n");) 
        return;
    }
    if ((pstConn->pstSavedMsgFirst != NULL)
          &&(pstConn->pstHandshake->ucState >= M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO)
             && (pstConn->pstHandshake->ucState <= M_WAP_TLS_HM_STATE_WAIT_FINISHED))
    {
        pstMsgList = pstConn->pstSavedMsgFirst;

        pstConn->pstSavedMsgFirst = pstMsgList->pstNext;
        if (NULL == pstConn->pstSavedMsgFirst)
        {
            pstConn->pstSavedMsgLast = NULL;
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcSavedMsgs prepend first message node\n");) 
        TlsMsg_PrependMsg(
                           pstTlsHandle->hMsgHandle,
                           E_WAP_MSG_MODULEID_TLS,
                           pstMsgList->stMsg.uiMsgType,
                           pstMsgList->stMsg.iParam1,
                           pstMsgList->stMsg.iParam2,
                           pstMsgList->stMsg.uiParam1,
                           pstMsgList->stMsg.uiParam2,
                           pstMsgList->stMsg.pvParam1,
                           pstMsgList->stMsg.pvParam2
                           ); 
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcSavedMsgs free first node\n");) 
        WAP_FREE(pstMsgList);
           
    }
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_SaveMsgToList
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    save the message to the message list
ARGUMENTS PASSED:
    pstTlsHandle : TLS module handle
    pstMsg : tls connection 
RETURN VALUE:
    error: -1
    success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static void WapTlsHm_SaveMsgToList
(
    St_WapTlsConn *pstConn,
    St_TlsMsg *pstMsg
)
{
    St_WapTlsMsgList *pstMsgList = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_SaveMsgToList begin\n");)
    if ((NULL == pstConn)|| (NULL == pstMsg))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_SaveMsgToList parameter error\n");)
        return;
    }
    /*Free when delete msg from list*/
    pstMsgList = (St_WapTlsMsgList*)WAP_MALLOC(sizeof(St_WapTlsMsgList));
    if (NULL == pstMsgList)
    {
        return;
    }
    WAP_MEMCPY(&(pstMsgList->stMsg),pstMsg,sizeof(St_TlsMsg));
    pstMsgList->pstNext = NULL;

    if (pstConn->pstSavedMsgLast != NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_SaveMsgToList save as the last\n");)
        pstConn->pstSavedMsgLast->pstNext = pstMsgList;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_SaveMsgToList as first\n");)
        pstConn->pstSavedMsgFirst = pstMsgList;
    }
    pstConn->pstSavedMsgLast = pstMsgList;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_SaveMsgToList end\n");)
}
/*==================================================================================================
FUNCTION: 
    WapTlsHm_ProcHandShakeErr
CREATE DATE:
    2006-12-22
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process error while handshake
ARGUMENTS PASSED:
    pstTlsHandle : TLS module handle
    pstTlsConn : tls connection 
    iErrCode   : Err code
    iAlertDesc : Err description
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void WapTlsHm_ProcHandShakeErr
( 
    St_WapTlsHandle *pstTlsHandle, 
    St_WapTlsConn *pstTlsConn, 
    WE_INT32 iErrCode, 
    WE_INT32 iAlerDesc
)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcHandShakeErr begin\n");)            
    if( NULL == pstTlsConn || NULL == pstTlsHandle )
    {
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcHandShakeErr parameter error \n");)            
         return;
    }
    pstTlsConn->iSecId = pstTlsConn->pstHandshake->iSecurityId;
    WapTlsHm_FreeHandShake( pstTlsHandle, pstTlsConn->pstHandshake );
    pstTlsConn->pstHandshake = NULL;
    TlsMsg_PostMsg(
                    pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_TLSHM_MSG_ERROR,
                    pstTlsConn->iConnId,
                    0,
                    (WE_UINT32)iErrCode,
                    (WE_UINT32)iAlerDesc,
                    NULL,
                    NULL);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTlsHm_ProcHandShakeErr End\n");)            
    
}
#endif


