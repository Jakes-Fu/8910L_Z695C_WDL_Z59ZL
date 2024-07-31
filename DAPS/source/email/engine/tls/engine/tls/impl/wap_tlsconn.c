/*==================================================================================================
    FILE NAME   : wap_tlsconn.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
      TLS connection operation
    
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
#include "wap_tlsst.h" /*must before wap_tlspub.h*/
#include "wap_tlspub.h" 
#include "wap_tlscore.h"
#include "wap_tlshmcore.h"

#include "itls.h"

#ifdef WAP_CONFIG_SSL
#include "wap_sslcore.h"
#endif
/**************************************************************************************************
* Prototype Define Section
***************************************************************************************************/

static void WapTls_DeleteSavedMsgs(St_WapTlsConn *pstConn);
static void WapTls_ProcSecResp
( 
    WE_INT32  iEvent,
    void     *pvSecEventData,
    WE_HANDLE hTlsHandle
);

/*==================================================================================================
FUNCTION: 
    WapTls_ProcSecResp
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Process SEC response
ARGUMENTS PASSED:
    hWapHandle: WAP handle
    iEvent:  From sec
    pvSecEventData: Sec response 
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    free in wap_tlssec.c
==================================================================================================*/
static void WapTls_ProcSecResp
( 
    WE_INT32  iEvent,
    void     *pvSecEventData,
    WE_HANDLE hTlsHandle
)
{
    St_WapTlsHandle *pstTlsHandle = NULL;
    
    pstTlsHandle = (St_WapTlsHandle*)hTlsHandle;

    if ( NULL == pvSecEventData || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp return for parameter invalid\n");)
        return ;
    }
   

    switch (iEvent)
    {
        case G_SEARCHPEER_RESP :
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_SEARCHPEER_RESP\n");)
            {                
                St_SecSearchPeerResp *pstSearchPeer = WAP_MALLOC(sizeof(St_SecSearchPeerResp));
                if (NULL == pstSearchPeer)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp SEARCHPEER--error-- no memory space!\n");)
                    return ;
                }
                WAP_MEMCPY( (void *)pstSearchPeer,(void *)pvSecEventData,sizeof(St_SecSearchPeerResp));  
               
                TlsMsg_PostMsg(
                    pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_HM_MSG_PEER_LOOKUP_RESP,
                    pstSearchPeer->iTargetID & 0x0000007F,
                    0,
                    0,
                    0,
                    pstSearchPeer,
                    NULL );
                break;
            }
        case G_SESSIONGET_RESP :
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_SESSIONGET_RESP\n");)
            {
                St_SecSessionGetResp *pstSessionFetch = WAP_MALLOC(sizeof(St_SecSessionGetResp));
                if (NULL == pstSessionFetch)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("G_SESSIONGET_RESP --error-- no memory space!\n");)
                    return ;
                }
                WAP_MEMCPY( (void *)pstSessionFetch,(void *)pvSecEventData,sizeof(St_SecSessionGetResp) );
                if ( 0 == pstSessionFetch->usSessionIdLen)
                {
                    pstSessionFetch->pucSessionId = NULL;
                }
                else
                {
                    pstSessionFetch->pucSessionId = WAP_MALLOC(pstSessionFetch->usSessionIdLen);
                    if (NULL == pstSessionFetch->pucSessionId)
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("G_SESSIONGET_RESP --error-- no memory space!\n");)
                        WAP_FREE(pstSessionFetch);
                        return ;
                    }

                    WAP_MEMCPY(
                        (void *)(pstSessionFetch->pucSessionId),
                        (void *)(((St_SecSessionGetResp *)pvSecEventData)->pucSessionId),
                        pstSessionFetch->usSessionIdLen);
                }
          
                TlsMsg_PostMsg(
                    pstTlsHandle->hMsgHandle,
                    E_WAP_MSG_MODULEID_TLS,
                    M_WAP_TLS_HM_MSG_SESSION_FETCH_RESP,
                    pstSessionFetch->iTargetID & 0x0000007F,
                    0,
                    0,
                    0,
                    pstSessionFetch,
                    NULL );
                break;
            }  
        case G_VERIFYSVRCERT_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_VERIFYSVRCERT_RESP\n");)
            {
                St_SecVerifySvrCertResp *pstVerSvrCert = WAP_MALLOC(sizeof(St_SecVerifySvrCertResp));
                if (NULL == pstVerSvrCert)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_VERIFYSVRCERT_RESP--error-- no memory space!\n");)
                    return ;
                }
                (WE_VOID)WAP_MEMCPY((void *)pstVerSvrCert,(void *)pvSecEventData,sizeof(St_SecVerifySvrCertResp));
                (WE_VOID)TlsMsg_PostMsg(
                                        pstTlsHandle->hMsgHandle,
                                        E_WAP_MSG_MODULEID_TLS,
                                        M_WAP_TLS_HM_MSG_SERVER_CERT_VERIFY_RESP,
                                        pstVerSvrCert->iTargetID & 0x000000FF ,
                                        0,
                                        pstVerSvrCert->usResult,
                                        0,
                                        NULL,
                                        NULL );
                /*remember free */
                WAP_FREE ( pstVerSvrCert );
                break;
            }
         case G_GETPRF_RESP:
             WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_GETPRF_RESP\n");)
            {
                St_SecGetPrfResp *pstPrf = WAP_MALLOC(sizeof(St_SecGetPrfResp));
                
                if (NULL == pstPrf)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp no memory space!\n");)
                    return ;
                }
                WAP_MEMCPY( (void *)pstPrf,(void *)pvSecEventData,sizeof(St_SecGetPrfResp));
                if (0 == pstPrf->usBufLen)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp prf 0 == pstPrf->usBufLen\n");)
                    pstPrf->pucBuf = NULL;
                }
                else
                {
                    pstPrf->pucBuf = (WE_UCHAR*)WAP_MALLOC(pstPrf->usBufLen);
                    if (NULL == pstPrf->pucBuf)
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp no memory space!\n");)
                        WAP_FREE(pstPrf);
                        return ;
                    }

                    WAP_MEMCPY(
                        (void *)(pstPrf->pucBuf),
                        (void *)(((St_SecGetPrfResp *)pvSecEventData)->pucBuf),
                        pstPrf->usBufLen);
                }

                TlsMsg_PostMsg(
                                pstTlsHandle->hMsgHandle,
                                E_WAP_MSG_MODULEID_TLS,
                                M_WAP_TLS_HM_MSG_PRF_RESP,
                                pstPrf->iTargetID & 0x000000FF ,
                                0,
                                0,
                                0,
                                pstPrf,
                                NULL );
                break;
            }
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case G_TLS_GETUSERCERT_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_GETUSERCERT_RESP\n");)
            {
               St_SecTlsGetUserCertResp *pstGetUserCert = WAP_MALLOC( sizeof(St_SecGetUserCertResp) );
               if (NULL == pstGetUserCert)
               {
                   WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_GETUSERCERT_RESP--error-- no memory space!\n");)
                   return ;
               }
               
               (WE_VOID)WAP_MEMCPY( (void *)pstGetUserCert,(void *)pvSecEventData,sizeof(St_SecTlsGetUserCertResp) );
               if (0 == pstGetUserCert->ucNbrCerts)
               {
                   pstGetUserCert->pstCert = NULL;
                   pstGetUserCert->pucPubKeyHash = NULL;
               }
               else
               {
                   pstGetUserCert->pstCert = WAP_MALLOC(sizeof(St_SecTlsAsn1Certificate));
                   if (NULL == pstGetUserCert->pstCert)
                   {
                       WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp--error-- no memory space!\n");)
                       WAP_FREE(pstGetUserCert);
                       return ;
                   }
                   
                   (WE_VOID)WAP_MEMCPY(
                   (void *)(pstGetUserCert->pstCert),
                   (void *)(((St_SecTlsGetUserCertResp *)pvSecEventData)->pstCert),
                   sizeof(St_SecTlsAsn1Certificate));

                   if (pstGetUserCert->pstCert->uiCertLen == 0)
                   {
                      pstGetUserCert->pstCert->pucCert = NULL;
                      pstGetUserCert->pucPubKeyHash = NULL;
                   }
                   else
                   {
                      pstGetUserCert->pstCert->pucCert = (WE_UCHAR*)WAP_MALLOC(pstGetUserCert->pstCert->uiCertLen);
                      if (NULL == pstGetUserCert->pstCert->pucCert)
                      {
                           WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp--error-- no memory space!\n");)
                           WAP_FREE(pstGetUserCert->pstCert);
                           WAP_FREE(pstGetUserCert);
                           return ;
                      }
                      WAP_MEMCPY(pstGetUserCert->pstCert->pucCert, 
                        (((St_SecTlsGetUserCertResp *)pvSecEventData)->pstCert)->pucCert,
                        pstGetUserCert->pstCert->uiCertLen);
                   }
                
                    pstGetUserCert->pucPubKeyHash = WAP_MALLOC(((St_SecTlsGetUserCertResp *)pvSecEventData)->usPubKeyHashLen);
                    if (NULL == pstGetUserCert->pucPubKeyHash)
                    {
                        if (pstGetUserCert->pstCert != NULL)
                        {
                            if (pstGetUserCert->pstCert->pucCert != NULL)
                            {
                                WAP_FREE(pstGetUserCert->pstCert->pucCert);                            
                            }
                            WAP_FREE(pstGetUserCert->pstCert);
                        }
                        WAP_FREE(pstGetUserCert);
                        return ;
                    }
                    WAP_MEMCPY(pstGetUserCert->pucPubKeyHash, (((St_SecTlsGetUserCertResp *)pvSecEventData)->pucPubKeyHash),
                        (((St_SecTlsGetUserCertResp *)pvSecEventData)->usPubKeyHashLen));
              }
               (WE_VOID)TlsMsg_PostMsg(
                                       pstTlsHandle->hMsgHandle,
                                       E_WAP_MSG_MODULEID_TLS,
                                       M_WAP_TLS_HM_MSG_GET_CLIENT_CERT_RESP,
                                       pstGetUserCert->iTargetID & 0x000000FF,
                                       0,
                                       0,
                                       0,
                                       pstGetUserCert,
                                       NULL );
               break;
           }
#endif            
        case G_COMPUTESIGN_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_COMPUTESIGN_RESP\n");)
            {
                St_SecCompSignResp *pstCompSig = WAP_MALLOC(sizeof(St_SecCompSignResp));
                if (NULL == pstCompSig)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( " G_COMPUTESIGN_RESP--error-- no memory space!\n");)
                    return ;
                }
                (WE_VOID)WAP_MEMCPY( (void *)pstCompSig,(void *)pvSecEventData,sizeof(St_SecCompSignResp) );

                if (0 == pstCompSig->usSignatureLen)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( " compute signature failed --error-- \n");)
                    return ;
                }
                else
                {                   
                    pstCompSig->pucSignature = WAP_MALLOC(pstCompSig->usSignatureLen);
                    if (NULL == pstCompSig->pucSignature)
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( " G_COMPUTESIGN_RESP--error-- no memory space!\n");)
                        return ;
                    }
                    (WE_VOID)WAP_MEMCPY( (void *)(pstCompSig->pucSignature), \
                                (void *)(((St_SecCompSignResp *)pvSecEventData)->pucSignature), \
                                pstCompSig->usSignatureLen);
                }

                (WE_VOID)TlsMsg_PostMsg(
                                        pstTlsHandle->hMsgHandle,
                                        E_WAP_MSG_MODULEID_TLS,
                                        M_WAP_TLS_HM_MSG_CLIENT_CERT_SIGNATURE_RESP,
                                        pstCompSig->iTargetID & 0x000000FF,
                                        0,
                                        0,
                                        0,
                                        pstCompSig,
                                        NULL );
                break;
            }
        case G_TLS_GETCIPHERSUITE_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_GETCIPHERSUITE_RESP\n");)
            {
                St_SecTlsGetCipherSuitesResp *pstGetCipherSuite = WAP_MALLOC(sizeof(St_SecTlsGetCipherSuitesResp));
                if (NULL == pstGetCipherSuite)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp GETCIPHERSUITE--error-- no memory space!\n");)
                    return ;
                }

                (WE_VOID)WAP_MEMCPY( (void *)pstGetCipherSuite,(void *)pvSecEventData,sizeof(St_SecTlsGetCipherSuitesResp));
                if (0 == pstGetCipherSuite->usCipherSuitesLen)
                {
                    pstGetCipherSuite->pucCipherSuites = NULL;
                }
                else
                {
                    pstGetCipherSuite->pucCipherSuites =WAP_MALLOC(pstGetCipherSuite->usCipherSuitesLen);
                    if (NULL == pstGetCipherSuite->pucCipherSuites)
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp GETCIPHERSUITE--error-- no memory space!\n");)
                        WAP_FREE(pstGetCipherSuite);
                        return ;
                    }
                    (WE_VOID)WAP_MEMCPY( \
                        (void *)(pstGetCipherSuite->pucCipherSuites), \
                        (void *)(((St_SecTlsGetCipherSuitesResp *)pvSecEventData)->pucCipherSuites), \
                        pstGetCipherSuite->usCipherSuitesLen );
                }
      

                (WE_VOID)TlsMsg_PostMsg( \
                                        pstTlsHandle->hMsgHandle, \
                                        E_WAP_MSG_MODULEID_TLS, \
                                        M_WAP_TLS_HM_MSG_GET_CIPHER_SUITES_RESP, \
                                        pstGetCipherSuite->iTargetID & 0x000000FF, \
                                        0, \
                                        0, \
                                        0, \
                                        pstGetCipherSuite, \
                                        NULL );
                break;
            }
            case G_KEYEXCH_RESP:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_KEYEXCH_RESP\n");)
            {
                St_SecKeyExchResp *pstKeyExch = WAP_MALLOC( sizeof(St_SecKeyExchResp) );
                if (NULL == pstKeyExch)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_KEYEXCH_RESP--error-- no memory space!\n");)
                    return ;
                }

                (WE_VOID)WAP_MEMCPY(pstKeyExch,pvSecEventData,sizeof(St_SecKeyExchResp));

                if (0 == pstKeyExch->pucPreMsKey)
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp  don't support public_key_exch\n");)
                    pstKeyExch->pucPreMsKey = NULL;
                }
                else
                {
                    pstKeyExch->pucPreMsKey = WAP_MALLOC(pstKeyExch->usPreMsKeyLen);
                    if (NULL == pstKeyExch->pucPreMsKey)
                    {
                        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_KEYEXCH_RESP--error-- no memory space!\n");)
                        return ;
                    }
                    
                    (WE_VOID)WAP_MEMCPY(
                        (void *)(pstKeyExch->pucPreMsKey),
                        (void *)(((St_SecKeyExchResp *)pvSecEventData)->pucPreMsKey),
                        pstKeyExch->usPreMsKeyLen);
                }

                (WE_VOID)TlsMsg_PostMsg(
                                        pstTlsHandle->hMsgHandle,
                                        E_WAP_MSG_MODULEID_TLS,
                                        M_WAP_TLS_HM_MSG_CREATE_KEY_EXCH_RESP,
                                        pstKeyExch->iTargetID & 0x000000FF ,
                                        0,
                                        0,
                                        0,
                                        pstKeyExch,
                                        NULL );
                break;
            }
#ifndef WAP_CONFIG_APP_MANAGE_CERTIFICATE
        case G_Evt_SHOW_DLG :
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_Evt_SHOW_DLG\n" );)
            {
                St_ShowDlg *pstShowDlg = WAP_MALLOC(sizeof(St_ShowDlg));
                if (NULL == pstShowDlg)
                {
                    return;
                }
                (WE_VOID)WAP_MEMCPY((void *)pstShowDlg,(void *)pvSecEventData,sizeof(St_ShowDlg));

                if ( 0 == pstShowDlg->uiLength )
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( 
                                  "WapTls_ProcSecResp G_Evt_SHOW_DLG --error-- NULL para \n");)
                    return ;
                }

                pstShowDlg->pvBuf = WAP_MALLOC( pstShowDlg->uiLength );
                if ( NULL == pstShowDlg->pvBuf )
                {
                    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_Evt_SHOW_DLG--error-- no memory space!\n");)
                    return ;
                }
                (WE_VOID)WAP_MEMCPY( (void *)(pstShowDlg->pvBuf),
                            (void *)(((St_ShowDlg *)pvSecEventData)->pvBuf) ,
                            pstShowDlg->uiLength );               
                (WE_VOID)TlsMsg_PostMsg(
                                pstTlsHandle->hMsgHandle,
                                E_WAP_MSG_MODULEID_TLS,
                                M_WAP_TLS_HM_MSG_SELECT_SEC_DIALOG_REQ,
                                pstShowDlg->iTargetId & 0x000000FF ,
                                0,
                                0,
                                0,
                                pstShowDlg,
                                NULL );                
                break;
            }  
#endif            
        case G_TLS_GETMASTERSECRET_RESP:
         WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "receive sec G_TLS_GETMASTERSECRET_RESP\n" );)
        {
            St_SecSSLGetMasterSecretResp *pstSslMasterSec = WAP_MALLOC(sizeof(St_SecSSLGetMasterSecretResp));
            if (NULL == pstSslMasterSec)
            {
                return;
            }
            (WE_VOID)WAP_MEMCPY((void *)pstSslMasterSec,(void *)pvSecEventData,sizeof(St_SecSSLGetMasterSecretResp));

            if ( 0 == pstSslMasterSec->iMasterSecretLen)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( 
                              "WapTls_ProcSecResp G_TLS_GETMASTERSECRET_RESP --error--  \n");)
                return ;
            }

            pstSslMasterSec->pucMasterSecret = WAP_MALLOC( pstSslMasterSec->iMasterSecretLen);
            if ( NULL == pstSslMasterSec->pucMasterSecret )
            {
                WAP_FREE(pstSslMasterSec);
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp G_TLS_GETMASTERSECRET_RESP--error-- no memory space!\n");)
                return ;
            }
            (WE_VOID)WAP_MEMCPY( (void *)(pstSslMasterSec->pucMasterSecret),
                        (void *)(((St_SecSSLGetMasterSecretResp *)pvSecEventData)->pucMasterSecret) ,
                        pstSslMasterSec->iMasterSecretLen);               
            (WE_VOID)TlsMsg_PostMsg(
                            pstTlsHandle->hMsgHandle,
                            E_WAP_MSG_MODULEID_TLS,
                            M_WAP_TLS_HM_MSG_SSL_GET_MASTER_SEC_RESP,
                            pstSslMasterSec->iTargetID & 0x000000FF ,
                            0,
                            0,
                            0,
                            pstSslMasterSec,
                            NULL );                
            break;
         }
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ProcSecResp receive unkonwn message!\n");)            
            break;
    }
}

St_WapTlsConn *WapTls_FindConnById(St_WapTlsHandle *pstTlsHandle,WE_INT32 iConnId)
{
    WE_INT32 i = 0;

    if (NULL == pstTlsHandle)
    {
        return NULL;
    }
    
    for(i = 0; i< G_WAP_CFG_MAX_CONNECTIONS;i++)
    {
        if (pstTlsHandle->apstWapTlsConnTable[i] != NULL)
        {
            if (pstTlsHandle->apstWapTlsConnTable[i]->iConnId == iConnId)
            {
                return pstTlsHandle->apstWapTlsConnTable[i];
            }
        }
    }
    return NULL;
     
}
/*==================================================================================================
FUNCTION: 
    WapTls_ConnNew
CREATE DATE:
    2006-12-15
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    create a new TLS connecton
ARGUMENTS PASSED:
    hTlsHandle : TLS handle
    iCidx      : TLS connection id
RETURN VALUE:
    if create successfully, return connection address
    else return NULL 
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsConn* WapTls_ConnNew
(
    St_WapTlsHandle *pstTlsHandle, 
    WE_INT32 iConnId
)
{
    St_WapTlsConn *pstConn = NULL;
    WE_INT32 i = 0;
    WE_INT32   iTmp = 0;

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnNew return -input NULL--.\n");)
        return NULL;
    }    
 
    WAP_LOG_PRINT((E_WAP_DBG_TLS, E_WAP_DBG_LEVEL_LOW)(("WapTls_ConnNew Connection num is %d\n"),pstTlsHandle->iOpenConnNumber);)
    /*If has more than one connection, create once for sec handle and algorithm handle*/
    if ( 0 == pstTlsHandle->iOpenConnNumber )
    {
        
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnNew create sec module\n");)

        ISec_Create(E_SEC_WAP,&( pstTlsHandle->hSecHandle ));

        
        if ( NULL == pstTlsHandle->hSecHandle )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnNew create sec module fail\n");)
            return NULL;                
        }
        ISec_RegClientEventCB(pstTlsHandle->hSecHandle,
                      pstTlsHandle, 
                      (Fn_ISecEventHandle)WapTls_ProcSecResp);
        We_LibCryptInitialise(NULL, &( pstTlsHandle->hLibAlgHandle));
       
        if ( NULL == pstTlsHandle->hLibAlgHandle )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnNew create alg module fail\n");)
            ISec_Release(pstTlsHandle->hSecHandle);
            return NULL;
        }            
    }
    if (pstTlsHandle->iOpenConnNumber >= G_WAP_CFG_MAX_CONNECTIONS)
    {
        WapTls_ConnDelete(pstTlsHandle,0);
    }
    pstConn = WapTls_FindConnById(pstTlsHandle, iConnId);
    if (NULL == pstConn)
    {
        pstConn = (St_WapTlsConn*)WAP_MALLOC(sizeof(St_WapTlsConn));
        if (NULL == pstConn)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnNew CM MALLOC error\n");)
            ISec_Release(pstTlsHandle->hSecHandle);
            We_LibCryptTerminate( pstTlsHandle->hLibAlgHandle);
            pstTlsHandle->hSecHandle = NULL;
            pstTlsHandle->hLibAlgHandle = NULL;
            return NULL;
        }
        pstConn->iObjectId = iConnId;
        pstConn->iConnId = iConnId;
        
        pstConn->iSecId = 0;
        pstConn->ucState = M_WAP_TLS_STATE_NULL;
        pstConn->ucHmInputState = M_WAP_TLS_HM_INPUT_STATE_START;
        pstConn->ucFlags = 0;
        pstConn->pstSendRecs = NULL;
        pstConn->stRecvRec.pucFragment = NULL;
        pstConn->stRecvRec.usLength = 0;
        pstConn->pucHmInputMsg = NULL;
        pstConn->ucHmInputState = M_WAP_TLS_HM_INPUT_STATE_START;
        
        pstConn->ucTcpSendReady = TRUE;
        pstConn->ucHttpSendProcessing = FALSE;
        pstConn->ucHttpSendSelect = FALSE;
        pstConn->usRecvRecStartPos = 0;

        pstConn->ucInputState = M_WAP_TLS_INPUT_STATE_START;
        pstConn->pucInputBuf = (WE_UCHAR*)WAP_MALLOC(M_WAP_TLS_BUF_SIZE);
        if (NULL == pstConn->pucInputBuf)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnNew MALLOC error\n");)
            ISec_Release( pstTlsHandle->hSecHandle);
            We_LibCryptTerminate( pstTlsHandle->hLibAlgHandle);
            pstTlsHandle->hSecHandle = NULL;
            pstTlsHandle->hLibAlgHandle = NULL;
            WAP_FREE(pstConn);
            return NULL;
        }
        pstConn->iInputBytesLeft = 0;
        pstConn->iInputBufPos = 0;
        
        pstConn->pucOutputBuf = NULL;
        pstConn->iOutputBytesLeft = 0;
        pstConn->iOutputBufPos = 0;

        pstConn->pstSavedMsgFirst = NULL;
        pstConn->pstSavedMsgLast = NULL;
#ifdef WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION
        pstConn->stProtocolVersion.ucMajor = pstTlsHandle->ucVerMajor;
        pstConn->stProtocolVersion.ucMinor = pstTlsHandle->ucVerMinor; 
#else
        pstConn->stProtocolVersion.ucMajor = M_WAP_TLS_PROTOCOL_VERSION_MAJOR;
        pstConn->stProtocolVersion.ucMinor = M_WAP_TLS_PROTOCOL_VERSION_MINOR; 
#endif        
        pstConn->pstHandshake = NULL;

        WapTls_ConnStateInit(&(pstConn->stRead));
        iTmp = WapTls_DecryptStreamInit(pstTlsHandle, 
                                         E_WE_ALG_CIPHER_NULL,
                                         NULL, 
                                         0,
                                         &(pstConn->stRead.hCipherHandle));
        if (iTmp < 0 )
        {
            WapTlsHm_ProcHandShakeErr(
                                        pstTlsHandle, 
                                        pstConn,
                                        G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                        M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR
                                     );
        }
        
        WapTls_ConnStateInit(&(pstConn->stWrite));
        iTmp = WapTls_EncryptStreamInit(pstTlsHandle, 
                                         E_WE_ALG_CIPHER_NULL,NULL, 0,&(pstConn->stWrite.hCipherHandle));
        if (iTmp < 0 )
        {
            WapTlsHm_ProcHandShakeErr(
                                        pstTlsHandle,
                                        pstConn, 
                                        G_WAP_ERR_TLS_HANDSHAKE_FAILURE,
                                        M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR
                                     );
        } 
        pstConn->pstPendingRead= NULL;
        pstConn->pstPendingWrite= NULL;   
        for(i = 0; i< G_WAP_CFG_MAX_CONNECTIONS;i++)
        {
           if ( pstTlsHandle->apstWapTlsConnTable[i] == NULL)
           {
               pstConn->iCidx = i;
               pstTlsHandle->apstWapTlsConnTable[i] = pstConn;
                break;
           }
        }

        pstTlsHandle->iOpenConnNumber++;        
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnNew end\n");)
    return pstConn;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_ConnDelete
CREATE DATE:
    2006-12-15
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    delete the TLS connecton assigned with parameter passed in
ARGUMENTS PASSED:
    hTlsHandle : TLS handle
    iCidx      : TLS connection index
RETURN VALUE:
    None 
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ConnDelete
(
    St_WapTlsHandle *pstTlsHandle,
    WE_INT32 iCidx
)
{
    St_WapTlsConn *pstConn = NULL;
    WE_UINT8 ucConnType = '\0';
    WE_INT32 iRet = 0;
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    St_SecStopConnectPara  stStopConn = {0};
    St_SecConnectOpt stConnOpt = {G_SEC_SETUP_CONNECTION,NULL};
#endif

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete begin\n");)

    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete parmeter error\n");)
        return -1;
    }
    pstConn = M_WAP_TLS_CONN_GET(pstTlsHandle, iCidx);
    if (pstConn != NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete Connection not NULL\n");)
        if (pstConn->ucFlags & M_WAP_TLS_FLAG_ESTABLISHED)
        {
            if (M_WAP_TLS_PROTOCOL_VERSION_MINOR == pstConn->stProtocolVersion.ucMinor)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete TLS\n");)
                ucConnType = G_SEC_TLS_CONNECTION_TYPE;
            }
            else
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete SSL\n");)
                ucConnType = G_SEC_SSL_CONNECTION_TYPE;
            }
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
            stStopConn.iSecId= pstConn->iSecId;
            stStopConn.iTargetID= pstConn->iObjectId;
            stConnOpt.eType = G_SEC_STOP_CONNECTION;
            stConnOpt.hParaValue = &(stConnOpt);
            ISecW_ConnectionOpts(pstTlsHandle->hSecHandle,&stConnOpt,ucConnType);            
#else            
            ISecW_StopConnection( pstTlsHandle->hSecHandle, 
                                  pstConn->iObjectId,
                                  pstConn->iSecId,
                                  ucConnType
                                  );
#endif
            pstConn->ucFlags &= ~M_WAP_TLS_FLAG_ESTABLISHED;
        }
        WapTlsHm_FreeHandShake(pstTlsHandle,pstConn->pstHandshake);
        WapTls_DeleRecList(&(pstConn->pstSendRecs));
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete delete socket data list\n");)
        WapTls_DeleteSocketData(&(pstConn->stSocketData));
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnDelete delete msg list\n");)
        WapTls_DeleteSavedMsgs(pstConn);
        if (pstConn->pucInputBuf != NULL)
        {
            WAP_FREE(pstConn->pucInputBuf);
        }
      
        if (pstConn->pucOutputBuf != NULL)
        {
            WAP_FREE(pstConn->pucOutputBuf);
        }
        
        if (pstConn->pucHmInputMsg != NULL)
        {
            WAP_FREE(pstConn->pucHmInputMsg);
        }
        
        if (pstConn->stRecvRec.pucFragment != NULL)
        {
            WAP_FREE(pstConn->stRecvRec.pucFragment);
        }       
      
        if ((pstConn->pstPendingRead != NULL)
                && (pstConn->pstPendingRead->hCipherHandle != NULL))
        {
            iRet = WapTls_DecryptStreamEnd(pstTlsHandle,pstConn->pstPendingRead->hCipherHandle);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete error\n");)
            }
        }
        if ((pstConn->pstPendingWrite!= NULL)
            && (pstConn->pstPendingWrite->hCipherHandle != NULL)) 
        {
            iRet = WapTls_EncryptStreamEnd(pstTlsHandle,pstConn->pstPendingWrite->hCipherHandle);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete error-----\n");)
            }            
        }
        WAP_FREE(pstConn->pstPendingRead);
        WAP_FREE(pstConn->pstPendingWrite);
      
        if (pstConn->stRead.hCipherHandle != NULL)
        {
            iRet = WapTls_DecryptStreamEnd(pstTlsHandle,pstConn->stRead.hCipherHandle);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete error\n");)
            }        
        }
        if (pstConn->stWrite.hCipherHandle != NULL) 
        {
            iRet = WapTls_EncryptStreamEnd(pstTlsHandle,pstConn->stWrite.hCipherHandle);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete error\n");)
            }        
        }
        if (pstConn->pucAddr != NULL)
        {
            WAP_FREE(pstConn->pucAddr);
            pstConn->pucAddr = NULL;
        }
        if (pstConn->pucDomain != NULL)
        {
            WAP_FREE(pstConn->pucDomain);
            pstConn->pucDomain = NULL;
        }
        WAP_FREE(pstConn);
        pstTlsHandle->apstWapTlsConnTable[iCidx] = NULL; 
      
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnDelete connection NULL\n");)
        return 0;
    }
     
    pstTlsHandle->iOpenConnNumber--;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ConnDelete open connection num is %d\n"),pstTlsHandle->iOpenConnNumber);)
    if ( 0 == pstTlsHandle->iOpenConnNumber )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnDelete release sec and alg moudle\n");)
        ISec_Release( pstTlsHandle->hSecHandle);
        We_LibCryptTerminate( pstTlsHandle->hLibAlgHandle);

        pstTlsHandle->hSecHandle = NULL;
        pstTlsHandle->hLibAlgHandle = NULL;
    } 
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnDelete end\n");)
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_DeleteSavedMsgs
CREATE DATE:
    2006-12-15
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    delete the message saved in list when delete connection
ARGUMENTS PASSED:
    pstConn: Tls connection
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static void WapTls_DeleteSavedMsgs(St_WapTlsConn *pstConn)
{
    
    St_WapTlsMsgList *pstMsgList = NULL;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DeleteSavedMsgs begin\n");)
    while (pstConn->pstSavedMsgFirst != NULL)
    {
        pstMsgList = pstConn->pstSavedMsgFirst;
        pstConn->pstSavedMsgFirst = pstMsgList->pstNext;
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DeleteSavedMsgs first node\n");)
        if (pstMsgList->stMsg.uiMsgType == M_WAP_TLS_HM_MSG_RECV_HSHK_MSG)
        {
            WAP_FREE(pstMsgList->stMsg.pvParam1);
        }
        WAP_FREE(pstMsgList);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DeleteSavedMsgs end with no node\n");)
}
/*==================================================================================================
FUNCTION: 
    WapTls_ConnStateInit
CREATE DATE:
    2006-12-15
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Initialize a  connection state.
ARGUMENTS PASSED:
    pstConnState: Tls connection state
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_ConnStateInit(St_WapTlsConnState *pstConnState)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnStateInit begin\n");)
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnStateInit parmeter error\n");)
        return -1;
    }

    pstConnState->ucCipherAlg = M_WAP_TLS_CIPHER_NULL;
    pstConnState->ucHashAlg = M_WAP_TLS_HASH_NULL;
    pstConnState->ucCompressionAlg = M_WAP_TLS_COMPRESSION_NULL;
    pstConnState->iMasterSecretId = -1;
    if (NULL == pstConnState->aucMacSecret || NULL == pstConnState->aucEncryptionKey
        || NULL == pstConnState->aucIV)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ConnStateInit error\n");)
        return -1;
    }
    WAP_MEMSET(pstConnState->aucMacSecret,0,M_WAP_TLS_MAX_HASH_KEY_SIZE);
    WAP_MEMSET(pstConnState->aucEncryptionKey,0,M_WAP_TLS_MAX_KEY_SIZE);
    WAP_MEMSET(pstConnState->aucIV,0,M_WAP_TLS_MAX_IV_SIZE);
    pstConnState->hCipherHandle = NULL;
    pstConnState->iSeqnum = 0; 
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ConnStateInit end\n");)
    return 0;
  
}

#endif
