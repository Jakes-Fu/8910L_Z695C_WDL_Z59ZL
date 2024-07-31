/*==================================================================================================
    FILE NAME : sec_dp.c
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        This File will gather functions that handle the dp queue.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date                    Author          Number                      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07     steven ding                                             create sec_dp.c      
    2007-05-17     tang                                              Delete cert manage and signtext
==================================================================================================*/

/***************************************************************************************
*   Include File Section
****************************************************************************************/

#include "sec_comm.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define SEC_STATES_STOP                                         0
#define SEC_STATES_START                                        1

typedef St_PublicData                        St_SecGlobalData;

#define SEC_CUR_UE_RESP                     (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->pvCurUeResp)
#define SEC_STATES                          (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->iStates)
#define SEC_WANTS_TO_RUN                    (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->iWantsToRun)
#define SEC_CUR_FUNCTION                    (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->pvCurFnc)
#define SEC_QUEUE_HEAD                      (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->stQueueHead)
#define SEC_QUEUE_TAIL                      (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->stQueueTail)
#define SEC_WE_HANDLE                       (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->hWeHandle) 

/*******************************************************************************
*   Function Define Section
*******************************************************************************/

/************************************************************************/
/*                 internal function declare                                                     */
/************************************************************************/
static WE_VOID Sec_DpHandleQueueInit(WE_HANDLE hSecHandle);


static WE_VOID Sec_DpHandleQueueNodeRemove(St_SecDpParameter* pstNode);


static WE_VOID Sec_DpHandleQueueCleanUp(WE_HANDLE hSecHandle);


static St_SecDpParameter* Sec_DpHandleQueueNextNodeGet(WE_HANDLE hSecHandle);


static WE_VOID Sec_DpHandleGlobalVarialbeInit(WE_HANDLE hSecHandle);

static WE_INT32 Sec_DpHandleInterVariableInit (WE_HANDLE hSecHandle);

static WE_INT32 Sec_DpHandleInterVariableCleanUp (WE_HANDLE hSecHandle);

static WE_VOID Sec_DpHandleSecInfoLog (WE_VOID); 
#ifdef M_SEC_CFG_WTLS
static WE_VOID Sec_DpHandleCurrentCertGet(WE_HANDLE hSecHandle,
                                          WE_INT32 iTargetID, 
                                          WE_INT32 iSecurityId,
                                          WE_INT32 iOptions);

static WE_VOID Sec_DpHandleSessionInfoView(WE_HANDLE hSecHandle,
                                           WE_INT32 iTargetID, 
                                           WE_INT32 iSecurityId ,
                                           WE_INT32 iOptions);

static WE_VOID Sec_DpHandleCurrentSecClassGet(WE_HANDLE hSecHandle, 
                                              WE_INT32 iTargetID, 
                                              WE_INT32 iSecurityId);
#endif
static WE_VOID Sec_DpHandleQueueNodeAdd(WE_HANDLE hSecHandle, St_SecDpParameter* pstNode);


static WE_INT32 Sec_DpHandleQueueIsEmpty(WE_HANDLE hSecHandle);

#ifdef M_SEC_CFG_WTLS
static WE_VOID Sec_DpHandleCurrentCertDeliver (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                               WE_INT32 iResult, WE_INT32 iOptions, 
                                               WE_UCHAR* pucCert);

static WE_VOID Sec_DpHandleSessionInfoDeliver (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                               WE_INT32 iResult, WE_INT32 iOptions, 
                                               St_SecSessionInfo* pstSessInf);
#endif

static WE_VOID Sec_DpHandleMain (WE_HANDLE hSecHandle, St_SecMessage* pstSig);

/*************************************************************************************************
external function
*************************************************************************************************/

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleInternal
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    handle message in queue
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID Sec_DpHandleInternal(WE_HANDLE hSecHandle)
{
    WE_INT32 iResult = -1;
    if(!hSecHandle)
    {
        return ;
    }

    if( (SEC_STATES_START == SEC_STATES) && (!SEC_CUR_FUNCTION) )
    {
        SEC_CUR_FUNCTION = Sec_DpHandleQueueNextNodeGet(hSecHandle);  
    }
    if(!SEC_CUR_FUNCTION)
    {
        return;
    }
    hSecHandle = ((St_SecDpParameter*)SEC_CUR_FUNCTION)->hItype;
    switch(((St_SecDpParameter*)SEC_CUR_FUNCTION)->eType)                     
    {
        case E_SEC_DP_NONE:
        {    
            break;
        }
#ifdef M_SEC_CFG_WTLS
        case E_SEC_DP_GET_CIPHERSUITE:    
        {
            St_SecDpGetWtlsCipherSuite*             pstData = NULL;
            
            pstData = (St_SecDpGetWtlsCipherSuite*)SEC_CUR_FUNCTION;
            Sec_GetCipherSuiteIm (hSecHandle, pstData->pstStrct->iTargetID, pstData->pstStrct->hTrustKeyIds);
            break;
        }    

        case E_SEC_DP_XCH_WTLS_KEY:    
        {
            St_SecDpWtlsKeyXch*                pstData = NULL;

            pstData = (St_SecDpWtlsKeyXch *)SEC_CUR_FUNCTION;
            Sec_KeyExchangeIm (hSecHandle, 
                                    pstData->pstStrct->iTargetID, 
                                    pstData->pstStrct->pstParam, 
                                    pstData->pstStrct->ucHashAlg, 
                                    pstData->pstStrct->pucRandval);
            break;
        }    
            
        case E_SEC_DP_WTLS_PRF:   
        {
            St_SecDpWtlsPrf*                    pstData = NULL;
            
            pstData = (St_SecDpWtlsPrf *)SEC_CUR_FUNCTION;
            Sec_GetPrfResultIm (hSecHandle, 
                            pstData->pstStrct->iTargetID, 
                            pstData->pstStrct->ucAlg, 
                            (WE_INT32)pstData->pstStrct->ucMasterSecretId,
                            pstData->pstStrct->pucSecret, 
                            (WE_INT32)pstData->pstStrct->usSecretLen,
                            pstData->pstStrct->pcLabel, 
                            pstData->pstStrct->pucSeed, 
                            (WE_INT32)pstData->pstStrct->usSeedLen, 
                            (WE_INT32)pstData->pstStrct->usOutputLen);
            break;
        }    
            
        case E_SEC_DP_VERIFY_WTLS_CERT_CHAIN: 
        {
            St_SecDpVerifyWtlsCertChain*        pstData = NULL;

            pstData =  \
                    (St_SecDpVerifyWtlsCertChain *)SEC_CUR_FUNCTION;
            Sec_VerifySvrCertIm (hSecHandle,
                                pstData->pstStrct->iTargetID,
                                pstData->pstStrct->pucBuf,
                                (WE_INT32)pstData->pstStrct->usBufLen,
                                pstData->pstStrct->pucAddr, 
                                (WE_INT32)pstData->pstStrct->usAddrLen,
                                (const WE_CHAR*)pstData->pstStrct->pucDomain,
                                (WE_INT32)pstData->pstStrct->usDomainLen);
            break;
        }   
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
        case E_SEC_DP_COMPUTE_SIGN:
            {
             St_SecDpComputeTlsSign*             pstData = NULL;
    
             pstData = (St_SecDpComputeTlsSign *)SEC_CUR_FUNCTION;
             Sec_CompSignatureIm (hSecHandle,  
                                 pstData->pstStrct->iTargetID, 
                                 pstData->pstStrct->pucPubKeyHash, 
                                 pstData->pstStrct->usPubKeyHashLen,
                                 pstData->pstStrct->pucData, 
                                 pstData->pstStrct->usDataLen,  
                                 pstData->pstStrct->ucAlg);
            break;
        } 
 #endif
 
#ifdef M_SEC_CFG_TLS
        case E_SEC_DP_GET_TLS_CIPHERSUITE:
        {
            St_SecDpGetTlsCipherSuite*             pstData = NULL;
            
            pstData = (St_SecDpGetTlsCipherSuite*)SEC_CUR_FUNCTION;
            Sec_GetTlsCipherSuiteIm (hSecHandle, pstData->pstStrct->iTargetID);
            break;
        }
        case E_SEC_DP_VERIFY_TLS_CERT_CHAIN:
        {
            St_SecDpVerifyTlsCertChain*        pstData = NULL;
            pstData =  \
                    (St_SecDpVerifyTlsCertChain *)SEC_CUR_FUNCTION;
            Sec_VerifyTlsCertChainIm (hSecHandle,
                                pstData->pstStrct->iTargetID,
                                pstData->pstStrct->pucAddr,
                                pstData->pstStrct->usAddrLen,
                                pstData->pstStrct->pucDomain, 
                                pstData->pstStrct->usDomainLen,
                                pstData->pstStrct->pstCerts,
                                pstData->pstStrct->ucNumCerts);
            break;
        }

        case E_SEC_DP_XCH_TLS_KEY:
            {
            St_SecDpTlsKeyXch          *pstData = NULL;  
            
            pstData = (St_SecDpTlsKeyXch *)SEC_CUR_FUNCTION;            
            Sec_TlsKeyExchangeIm (hSecHandle, 
                                  pstData->pstStrct->iTargetID,
                                  pstData->pstStrct->ucHashAlg,
                                  pstData->pstStrct->iOptions,
                                  pstData->pstStrct->pstParam,
                                  pstData->pstStrct->pucRandval);           

            break;

        }
        case E_SEC_DP_TLS_PRF:   
        {
            St_SecDpTlsPrf*                    pstData = NULL;
            
            pstData = (St_SecDpTlsPrf *)SEC_CUR_FUNCTION;
            Sec_GetTlsPrfResultIm (hSecHandle, 
                                   pstData->pstStrct->iTargetID, 
                                   pstData->pstStrct->ucAlg, 
                                   (WE_INT32)pstData->pstStrct->ucMasterSecretId,
                                   pstData->pstStrct->pucSecret, 
                                   (WE_INT32)pstData->pstStrct->usSecretLen,
                                   pstData->pstStrct->pcLabel, 
                                   pstData->pstStrct->pucSeed, 
                                   (WE_INT32)pstData->pstStrct->usSeedLen, 
                                   (WE_INT32)pstData->pstStrct->usOutputLen);
            break;
        }  
#endif
        case E_SEC_DP_ESTABLISHED_CONN:   
        {
            St_SecDpConnEst*            pstData = NULL;

            pstData = (St_SecDpConnEst *)SEC_CUR_FUNCTION;
            Sec_SetupConIm (hSecHandle, pstData->pstStrct);
            break;
        }
        
        case E_SEC_DP_TERMINATED_CONN: 
        {
            St_SecDpConnTmt*             pstData = NULL;

            pstData = (St_SecDpConnTmt *)SEC_CUR_FUNCTION;
            Sec_StopCon (hSecHandle,  
                                         pstData->pstStrct->iTargetID,  
                                         pstData->pstStrct->iSecurityId, 
                                         pstData->pstStrct->ucConnectionType);    
            break;
        }    
            
        case E_SEC_DP_RM_PEER_LINKS:  
        {
            St_SecDpRmPeerLinks*            pstData = NULL;

            pstData = (St_SecDpRmPeerLinks *)SEC_CUR_FUNCTION;
            Sec_RemovePeer (hSecHandle, 
                                    pstData->pstStrct->iTargetID,  
                                    (WE_INT32)pstData->pstStrct->ucMasterSecretId);
            break;
        }    
            
        case E_SEC_DP_LINK_PEER_SESSION:    
        {
            St_SecDpPeerLinkSession*          pstData = NULL;

            pstData = \
                (St_SecDpPeerLinkSession *)SEC_CUR_FUNCTION;
            Sec_AttachPeerToSessionIm (hSecHandle,  
                                      pstData->pstStrct->ucConnectionType, 
                                      pstData->pstStrct->pucAddress,  
                                      (WE_INT32)pstData->pstStrct->usAddressLen, 
                                      pstData->pstStrct->usPortnum,  
                                      (WE_INT32)pstData->pstStrct->ucMasterSecretId);
            break;
        }    
            
        case E_SEC_DP_LOOKUP_PEER:  
        {
            St_SecDpLookupPeer*                 pstData = NULL;

            pstData = (St_SecDpLookupPeer *)SEC_CUR_FUNCTION;
            Sec_SearchPeerIm (hSecHandle, 
                               pstData->pstStrct->iTargetID,  
                               pstData->pstStrct->ucConnectionType, 
                               pstData->pstStrct->pucAddress, 
                               (WE_INT32)pstData->pstStrct->usAddressLen, 
                               pstData->pstStrct->usPortnum);
            break;
        }    
            
        case E_SEC_DP_ACTIVE_SESSION:    
        {
            St_SecDpActiveSession*              pstData = NULL;

            pstData = (St_SecDpActiveSession *)SEC_CUR_FUNCTION;
            Sec_EnableSessionIm (hSecHandle,  
                                  (WE_INT32)pstData->pstStrct->ucMasterSecretId,  
                                  pstData->pstStrct->ucIsActive);
            break;
        }    
            
        case E_SEC_DP_INVALID_SESSION: 
        {
            St_SecDpInvalidSession*          pstData = NULL;

            pstData = \
                (St_SecDpInvalidSession *)SEC_CUR_FUNCTION;
            Sec_DisableSessionIm (hSecHandle,  
                                      (WE_INT32)pstData->pstStrct->ucMasterSecretId);
            break;
        }    
            
        case E_SEC_DP_FETCH_SESSION:   
        {
            St_SecDpfetchSession*               pstData = NULL;
            
            pstData = (St_SecDpfetchSession *)SEC_CUR_FUNCTION;
            Sec_FetchSessionInfoIm (hSecHandle, 
                                 pstData->pstStrct->iTargetID,  
                                 (WE_INT32)pstData->pstStrct->ucMasterSecretId);
            break;
        }    
            
        case E_SEC_DP_UPDATE_SESSION:  
        {
            St_SecDpUpdateSession*              pstData = NULL;
            
            pstData = (St_SecDpUpdateSession *)SEC_CUR_FUNCTION;
            Sec_UpdateSessionIm (hSecHandle,  
                                  (WE_INT32)pstData->pstStrct->ucMasterSecretId,  
                                  pstData->pstStrct->ucSessionOptions, 
                                  pstData->pstStrct->pucSessionId,  
                                  pstData->pstStrct->ucSessionIdLen, 
                                  pstData->pstStrct->ucCipherSuite, 
                                  pstData->pstStrct->ucCompressionAlg, 
                                  pstData->pstStrct->pucPrivateKeyId, 
                                  pstData->pstStrct->uiCreationTime);
            break;
        }    
#ifdef M_SEC_CFG_WTLS
        case E_SEC_DP_VIEW_CUR_CERT: 
        {
            St_SecDpViewCurCert*        pstData = NULL;

            pstData = \
                (St_SecDpViewCurCert *)SEC_CUR_FUNCTION;
            Sec_DpHandleCurrentCertGet(hSecHandle, 
                                     pstData->pstStrct->iTargetID, 
                                     pstData->pstStrct->iSecurityId, 
                                     pstData->iOptions);
            break;
        }    
            
        case E_SEC_DP_VIEW_SESSION:
        {
            St_SecDpViewSessionInfo*        pstData = NULL;

            pstData = \
                (St_SecDpViewSessionInfo *)SEC_CUR_FUNCTION;
            Sec_DpHandleSessionInfoView(hSecHandle, 
                                      pstData->pstStrct->iTargetID, 
                                      pstData->pstStrct->iSecurityId, 
                                      pstData->iOptions);
            break;
        }    
            
        case E_SEC_DP_GET_CUR_SEC_CLASS: 
        {
            St_SecDpGetCurSecClass*     pstData = NULL;

            pstData =  
                (St_SecDpGetCurSecClass *)SEC_CUR_FUNCTION;
            Sec_DpHandleCurrentSecClassGet(hSecHandle,  
                                         pstData->pstStrct->iTargetID, 
                                         pstData->pstStrct->iSecurityId);
            break;
        }    
#endif
        default:
        {
            break;
        }     
    }
    iResult = iResult;
    return;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    init sec module
ARGUMENTS PASSED:
    WE_HANDLE           hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_DpHandleInit (WE_HANDLE hSecHandle)
{
    WE_INT32 iRes = 0;
    
    if(!hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    Sec_MsgInit (hSecHandle);
    Sec_MsgRegister (hSecHandle,M_SEC_MSG_DST_HANDLE_MAIN, (Fn_SecMessageFunc*)Sec_DpHandleMain);
    iRes = Sec_DpHandleInterVariableInit(hSecHandle);    
    if (M_SEC_ERR_OK != iRes)
    {
        Sec_MsgTerminate(hSecHandle);                                
        return iRes; 
    }
    
    Sec_DpHandleSecInfoLog();
    
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleCleanUp
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    when sec terminate, cleanup
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_VOID Sec_DpHandleCleanUp(WE_HANDLE hSecHandle)
{
    if(!hSecHandle)
    {
        return ;
    }
    
    if (Sec_DpHandleQueueIsEmpty(hSecHandle))
    {
        SEC_STATES = SEC_STATES_STOP;
        SEC_WANTS_TO_RUN = FALSE;
    }
    else
    {
        SEC_STATES = SEC_STATES_START;
        SEC_WANTS_TO_RUN = TRUE;
    }
    Sec_TlHandleDpStructFree(SEC_CUR_FUNCTION);
    SEC_CUR_FUNCTION = NULL;
    
    return;
} 

/*************************************************************************************************
internal function
*************************************************************************************************/
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    handle dp queue init
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_DpHandleQueueInit(WE_HANDLE hSecHandle)
{
    if(!hSecHandle)
    {
        return ;
    }

    SEC_QUEUE_HEAD.eType = E_SEC_DP_NONE;
    SEC_QUEUE_HEAD.pstNext = &SEC_QUEUE_TAIL;
    SEC_QUEUE_HEAD.pstPrev = &SEC_QUEUE_HEAD;
    SEC_QUEUE_TAIL.eType = E_SEC_DP_NONE;
    SEC_QUEUE_TAIL.pstNext = NULL;
    SEC_QUEUE_TAIL.pstPrev = &SEC_QUEUE_HEAD;

    return;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueNodeRemove
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    remove a node from dp queue
ARGUMENTS PASSED:
    St_SecDpParameter*        pstNode[IN]:STRUCT OF DP QUEUE NODE
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_DpHandleQueueNodeRemove(St_SecDpParameter* pstNode)
{
    if (!pstNode)
    {
        return;
    }

    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = pstNode->pstPrev;
    pstNode->pstNext = NULL;
    pstNode->pstPrev = NULL;

    return;
}
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueDelById
CREATE DATE:
    2006-11-13
AUTHOR:
    brid
DESCRIPTION:
    delete node for queue and free it
ARGUMENTS PASSED:
    hSecHandle[IO]: Global data handle.
    hItype [IN]: I interface type
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID Sec_DpHandleQueueDelById(WE_HANDLE hSecHandle)
{
    St_SecDpParameter*        pstPara = NULL;
    St_SecDpParameter*        pstPrev = NULL;
    
    if(!hSecHandle)
    {
        return ;
    }

    pstPara = SEC_QUEUE_HEAD.pstNext;
    while (pstPara != &SEC_QUEUE_TAIL)
    {
        if(hSecHandle == pstPara->hItype)
        {
            pstPrev = pstPara->pstPrev;
            Sec_DpHandleQueueNodeRemove(pstPara);
            Sec_TlHandleDpStructFree((void*)pstPara);
            pstPara = pstPrev;

        }
        pstPara = pstPara->pstNext;
        
    }

    return;
}
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueCleanUp
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    cleanup dp queue
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_DpHandleQueueCleanUp(WE_HANDLE hSecHandle)
{
    St_SecDpParameter*        pstPara = NULL;
    
    if(!hSecHandle)
    {
        return ;
    }

    while (!Sec_DpHandleQueueIsEmpty(hSecHandle))
    {
        pstPara = SEC_QUEUE_HEAD.pstNext;
        Sec_DpHandleQueueNodeRemove(pstPara);
        /*add by bird for free memory 061113*/
        Sec_TlHandleDpStructFree((void*)pstPara);
    }

    return;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueNextNodeGet
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    get a node from dp queue
ARGUMENTS PASSED:
    WE_HANDLE           hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    St_SecDpParameter*
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static St_SecDpParameter* Sec_DpHandleQueueNextNodeGet(WE_HANDLE hSecHandle)
{
    St_SecDpParameter*        pstLastNode = NULL;

    if(!hSecHandle)
    {
        return NULL;
    }

    if (Sec_DpHandleQueueIsEmpty(hSecHandle))
    {
        return NULL;
    }

    pstLastNode = SEC_QUEUE_TAIL.pstPrev;
    Sec_DpHandleQueueNodeRemove(pstLastNode); 
    return pstLastNode;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleGlobalVarialbeInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    init global variable
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_DpHandleGlobalVarialbeInit(WE_HANDLE hSecHandle)
{
    if(!hSecHandle)
    {
        return;
    }
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE    
    SEC_CUR_UE_RESP = NULL;
#endif
    SEC_STATES = SEC_STATES_STOP;
    SEC_WANTS_TO_RUN = FALSE;
    SEC_CUR_FUNCTION = NULL;
    return ;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleTerminate
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    terminate sec module
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_DpHandleTerminate (WE_HANDLE hSecHandle)
{  
    WE_INT32 iRes = 0;
    
    if(!hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    Sec_MsgTerminate(hSecHandle);  
    iRes = Sec_DpHandleInterVariableCleanUp(hSecHandle);
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleInterVariableInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    init sec inter variable
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 Sec_DpHandleInterVariableInit (WE_HANDLE hSecHandle)
{
    WE_INT32 iRes = 0;
    
    if(!hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    iRes = Sec_LibCryptInitialise(hSecHandle);    
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    Sec_DpHandleGlobalVarialbeInit(hSecHandle);
    /*for use new cert parser in c.tang 070205*/
    /* 
    iRes = Sec_X509ChainInit(hSecHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }*/
    Sec_DpHandleQueueInit(hSecHandle);
    return M_SEC_ERR_OK;
}
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleInterVariableCleanUp
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    cleanup sec inter variable
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_INT32 Sec_DpHandleInterVariableCleanUp (WE_HANDLE hSecHandle)
{
    WE_UCHAR*   pucData     = NULL;
    WE_INT32    iLen        = 0;
    WE_INT32    iRes        = M_SEC_ERR_OK;

    if(!hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    /*
    iRes = Sec_X509ChainTerminate(hSecHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }*/         /*by tang 070205 for new cert parser.*/
    /*move from sec_iwim.c by bird 060808*/
#ifndef M_SEC_CFG_NO_PLATFORM
        Sec_WimSessionStop (hSecHandle);
#endif
    /* store sec info and clear it. */
    iRes += Sec_WimGetDataInfo(hSecHandle, &pucData, &iLen);
#ifndef M_SEC_CFG_NO_PLATFORM
    Sec_iWimStoreView(hSecHandle, pucData, iLen);  
#endif
    M_SEC_SELF_FREE(pucData);
    iRes += Sec_WimStopInfo(hSecHandle);

    Sec_WimRemoveElement(hSecHandle);
    /* clean dp queue. */
    Sec_DpHandleCleanUp(hSecHandle);
    Sec_DpHandleQueueCleanUp(hSecHandle);
    SEC_STATES = SEC_STATES_STOP;
    SEC_WANTS_TO_RUN = FALSE;    
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
    SEC_CUR_UE_RESP = NULL;
#endif    
    if (iRes != M_SEC_ERR_OK)
    {
        return M_SEC_ERR_GENERAL_ERROR;
    }
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleSecInfoLog
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    log sec info
ARGUMENTS PASSED:
    NONE
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_DpHandleSecInfoLog (WE_VOID)
{
    WE_LOG_MSG((0, (WE_UINT8)0,"Sec_DpHandleSecInfoLog() n"));
#ifdef M_SEC_CFG_WTLS_CLASS_2
    WE_LOG_MSG((0, (WE_UINT8)0,"WTLS class 2 is used. n"));
#endif
#ifdef M_SEC_CFG_WTLS_CLASS_3
    WE_LOG_MSG((0, (WE_UINT8)0,"WTLS class 3 is used. n"));
#endif
    WE_LOG_MSG((0, (WE_UINT8)0,"RSA is used with keysize = %d. n", 1024));

    WE_LOG_MSG((0, (WE_UINT8)0,"RC5 is used. n"));

    WE_LOG_MSG((0, (WE_UINT8)0,"3DES is used. n"));
    return ;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleCurrentCertGet
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    get current cert
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
    WE_INT32        iTargetID[IN]:TARGET ID
    WE_INT32        iSecurityId[IN]:SECURITY ID
    WE_INT32        iOptions[IN]:OPTIONS
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef M_SEC_CFG_WTLS
static WE_VOID Sec_DpHandleCurrentCertGet(WE_HANDLE hSecHandle,
                                          WE_INT32 iTargetID, WE_INT32 iSecurityId,
                                          WE_INT32 iOptions)
{
    WE_UINT32  iValidNotBefore     = 0;
    WE_UINT32  iValidNotAfter      = 0;
    WE_INT16    sIssuercharSet      = 0;
    WE_VOID*   pvIssuer            = NULL;
    WE_INT32    iIssuerLen          = 0;
    WE_INT16    sSubjectcharSet     = 0;
    WE_VOID*   pvSubject           = NULL;
    WE_INT32    iSubjectLen         = 0;
    St_SecSessionInfo* pstInfo = NULL;
    WE_INT32    iResult = 0;

    if (NULL == hSecHandle)
    {
        return;
    }   
    
    iResult = Sec_WimGetInfo(hSecHandle, iSecurityId, &pstInfo);
    if (1 == iOptions) 
    {
        WE_UCHAR* pucSerialNumber = NULL;/*serial number*/
        WE_UCHAR* pucAlg = NULL;                 /*signature algorthm*/
        WE_UCHAR* pucPubKey = NULL;          /*public key*/   
        WE_UINT16 usSerialNumberLen = 0;
        WE_UINT16 usPubKeyLen = 0;  
    
        if ((M_SEC_ERR_OK == iResult) && (pstInfo->ucNumCerts != 0)) 
        {
            iResult = Sec_CertGetItemsForUe(hSecHandle,  
                                            pstInfo->pstCerts[0].pucCert, 
                                            &sIssuercharSet,  
                                            &pvIssuer,  
                                            &iIssuerLen,  
                                            &sSubjectcharSet,  
                                            &pvSubject,  
                                            &iSubjectLen,  
                                            &iValidNotBefore,  
                                            &iValidNotAfter, 
                                            &pucSerialNumber,  
                                            &usSerialNumberLen,
                                            &pucAlg,
                                            &pucPubKey,
                                            &usPubKeyLen);
            if (M_SEC_ERR_OK == iResult) 
            { 
                Sec_iUeShowCertContent(hSecHandle,iTargetID, sIssuercharSet,  
                                pvIssuer, iIssuerLen,  
                                iValidNotBefore, iValidNotAfter,  
                                sSubjectcharSet, pvSubject,  
                                iSubjectLen, pucSerialNumber, usSerialNumberLen, 
                                pucAlg, pucPubKey,usPubKeyLen,M_SEC_ID_NONE);
            }
            if(NULL != pucSerialNumber)
            {
                WE_FREE(pucSerialNumber);
            }
            if(NULL != pucAlg)
            {
                WE_FREE(pucAlg);
            }
                                
            if(NULL != pucPubKey)
            {
                WE_FREE(pucPubKey);
            }
        }
        else
        {
            iResult = M_SEC_ERR_GENERAL_ERROR;
        }
        Sec_DpHandleCurrentCertDeliver(hSecHandle, iTargetID, iResult, iOptions, NULL);
    }
    else
    {
        Sec_DpHandleCurrentCertDeliver(hSecHandle, iTargetID, iResult, iOptions,  
                                     pstInfo->pstCerts[0].pucCert);
    }
    Sec_DpHandleCleanUp(hSecHandle); 
    return ;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleSessionInfoView
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    get view of session info
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]:sec global data
    WE_INT32        iTargetID[IN]:TARGET ID
    WE_INT32        iSecurityId[IN]:SECURITY ID
    WE_INT32        iOptions[IN]:OPTIONS
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_DpHandleSessionInfoView(WE_HANDLE hSecHandle, 
                                           WE_INT32 iTargetID, WE_INT32 iSecurityId ,
                                           WE_INT32 iOptions)
{
    St_SecSessionInfo  *pstInfo = NULL;
    WE_INT32              iResult = 0;

    iResult = Sec_WimGetInfo(hSecHandle, iSecurityId, &pstInfo);
    if ((1 == iOptions) && (M_SEC_ERR_OK == iResult))
    {
        Sec_iUeShowSessionContent(hSecHandle,iTargetID, *pstInfo);
    }
    Sec_DpHandleSessionInfoDeliver(hSecHandle, iTargetID, iResult, iOptions, pstInfo);            
    Sec_DpHandleCleanUp(hSecHandle);
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleCurrentSecClassGet
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    get current security class
ARGUMENTS PASSED:
    WE_HANDLE       hSecHandle[IN/OUT]: sec global data
    WE_INT32        iTargetID[IN]:TARGET ID
    WE_INT32        iSecurityId[IN]:SECURITY ID
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_DpHandleCurrentSecClassGet(WE_HANDLE hSecHandle, 
                                              WE_INT32 iTargetID, WE_INT32 iSecurityId)
{    
    St_SecSessionInfo*  pstInfo         = NULL;
    WE_INT32            iSecClass          = 0;
    WE_INT32            iInfoAvailable  = 0;
    WE_INT32            iCertAvailable  = 0;
    WE_INT32            iResult         = M_SEC_ERR_OK;

    if (NULL == hSecHandle)
    {
        return ;
    }
    
    iResult = Sec_WimGetInfo(hSecHandle, iSecurityId, &pstInfo);
    if (M_SEC_ERR_OK == iResult) 
    {
        iSecClass = pstInfo->ucSecurityClass;
        iInfoAvailable = 1;
        iCertAvailable = (pstInfo->pstCerts != NULL);
    }
    else 
    {
        iSecClass = (iSecurityId >= 0);
        iInfoAvailable = 0;
        iCertAvailable = 0;
    }
    
    Sec_WtlsCurClassResp(hSecHandle, iTargetID, iSecClass, 
                                iInfoAvailable, iCertAvailable);
    Sec_DpHandleCleanUp(hSecHandle);
}
#endif


/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueNodeAdd
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    add a node to dp queue
ARGUMENTS PASSED:
    WE_HANDLE                 hSecHandle[IN/OUT]:sec global data
    St_SecDpParameter*        pstNode[IN]:NODE STRUCT OF DP QUEUE
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_DpHandleQueueNodeAdd(WE_HANDLE hSecHandle, St_SecDpParameter* pstNode)
{
    if (!hSecHandle || !pstNode)
    {
        return;
    }
    pstNode->pstNext = SEC_QUEUE_HEAD.pstNext;
    pstNode->pstPrev = &SEC_QUEUE_HEAD;
    SEC_QUEUE_HEAD.pstNext = pstNode;
    pstNode->pstNext->pstPrev = pstNode;

    return;
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleQueueIsEmpty
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    get dp queue state
ARGUMENTS PASSED:
    WE_HANDLE           hSecHandle[IN/OUT]:sec global data
RETURN VALUE:
    RESULT
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_INT32 Sec_DpHandleQueueIsEmpty(WE_HANDLE hSecHandle)
{ 
    if(!hSecHandle)
    {
        return 0;
    }
    return (SEC_QUEUE_HEAD.pstNext == &SEC_QUEUE_TAIL);
}

/*==================================================================================================
FUNCTION: 
    Sec_DpHandleCurrentCertDeliver
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    deliver current cert
ARGUMENTS PASSED:
    WE_HANDLE           hSecHandle[IN/OUT]:sec global data
    WE_INT32            iTargetID[IN]:TARGET ID
    WE_INT32            iResult[IN]:RESULT
    WE_INT32            iOptions[IN]:OPTION
    WE_UCHAR*           pucCert[IN]:DER ENCODED CERT STRING
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef M_SEC_CFG_WTLS
static WE_VOID Sec_DpHandleCurrentCertDeliver (WE_HANDLE hSecHandle,
                                               WE_INT32 iTargetID, WE_INT32 iResult, 
                                               WE_INT32 iOptions, WE_UCHAR* pucCert)
{ 
    if(!hSecHandle)
    {
        return;
    }
    pucCert = pucCert;
    if (1 == iOptions) 
    {
        Sec_GetCurSvrCertResp (hSecHandle, iTargetID, iResult);
    } 
    
    return;
}
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleSessionInfoDeliver
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    deliver session info
ARGUMENTS PASSED:
    WE_HANDLE               hSecHandle[IN/OUT]:sec global data
    WE_INT32                iTargetID[IN]:TARGET ID 
    WE_INT32                iResult[IN]: RESULT
    WE_INT32                iOptions[IN]:OPTION
    St_SecSessionInfo*      pstSessInf[IN]:SESSION INFO
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_DpHandleSessionInfoDeliver (WE_HANDLE hSecHandle,
                                               WE_INT32 iTargetID, WE_INT32 iResult, 
                                               WE_INT32 iOptions, 
                                               St_SecSessionInfo* pstSessInf)
{ 
    if(!hSecHandle)
    {
        return;
    }
    pstSessInf = pstSessInf;
    if (1 == iOptions) 
    {
        Sec_GetSessionInfoResp(hSecHandle, iTargetID, iResult);
    } 

    return;
}
#endif
/*==================================================================================================
FUNCTION: 
    Sec_DpHandleMain
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    dp main handler
ARGUMENTS PASSED:
    WE_HANDLE           hSecHandle[IN/OUT]:sec global data
    St_SecMessage*      pstSig[IN]:MESSAGE
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_DpHandleMain (WE_HANDLE hSecHandle, St_SecMessage* pstMsg)
{
    WE_VOID*        pvMsg = NULL;

    if(!hSecHandle || !pstMsg || !pstMsg->pvParam)
    {
        return;
    }
    switch (pstMsg->iType)
    {
#ifdef M_SEC_CFG_WTLS
        case M_SEC_DP_MSG_GET_CIPHERSUITE:
        {
            St_SecDpGetWtlsCipherSuite*                 pstData = NULL;
            
            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_GET_CIPHERSUITE \n"));
            pstData = \
                (St_SecDpGetWtlsCipherSuite* )WE_MALLOC((WE_ULONG)sizeof(St_SecDpGetWtlsCipherSuite));
            if(!pstData)
            {
               Sec_WtlsGetCipherSuiteResp(pstMsg->hItype, ((St_SecWtlsGetCipherSuite *)(pstMsg->pvParam))->iTargetID,  
                                      M_SEC_ERR_INSUFFICIENT_MEMORY,NULL,0,NULL,0,NULL,0);
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpGetWtlsCipherSuite));
            pstData->eType = E_SEC_DP_GET_CIPHERSUITE;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_EXCHANGE_WTLS_KEY:
        {
            St_SecDpWtlsKeyXch*                    pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_EXCHANGE_WTLS_KEY \n"));
            pstData = \
                (St_SecDpWtlsKeyXch*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpWtlsKeyXch));
            if(!pstData)
            {
                Sec_KeyExchangeResp(pstMsg->hItype, ((St_SecWtlsKeyExchange *)(pstMsg->pvParam))->iTargetID,  
                                      M_SEC_ERR_INSUFFICIENT_MEMORY,0,NULL,0);
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpWtlsKeyXch));
            pstData->eType = E_SEC_DP_XCH_WTLS_KEY;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;    
        }
        
        case M_SEC_DP_MSG_WTLS_PRF:
        {
            St_SecDpWtlsPrf*                        pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal SEC_MAIN_SIG_E_SEC_DP_WTLS_PRF \n"));
            pstData = \
                (St_SecDpWtlsPrf*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpWtlsPrf));
            if(!pstData)
            {
                Sec_GetPrfResultResp(pstMsg->hItype, ((St_SecGetPrfResult *)(pstMsg->pvParam))->iTargetID,  
                                      M_SEC_ERR_INSUFFICIENT_MEMORY,NULL,0);
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpWtlsPrf));
            pstData->eType = E_SEC_DP_WTLS_PRF;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_VERIFY_WTLS_CERT_CHAIN:
        {
            St_SecDpVerifyWtlsCertChain*            pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal SEC_MAIN_SIG_E_SEC_DP_WTLS_VERIFY_CERT_CHAIN \n"));
            pstData = \
                (St_SecDpVerifyWtlsCertChain*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpVerifyWtlsCertChain));
            if(!pstData)
            {
                Sec_VerifySvrCertResp(pstMsg->hItype, ((St_SecWtlsVerifySvrCert *)(pstMsg->pvParam))->iTargetID,  
                               M_SEC_ERR_INSUFFICIENT_MEMORY);
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpVerifyWtlsCertChain));
            pstData->eType = E_SEC_DP_VERIFY_WTLS_CERT_CHAIN;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
        case M_SEC_DP_MSG_COMPUTE_SIGN:
        {                
                St_SecDpComputeTlsSign           *pstData = NULL;
                
                WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0, 
                    "SEC: received signal SEC_MAIN_SIG_E_SEC_DP_COMPUTE_TLS_SIGN \n"));
                pstData = \
                    (St_SecDpComputeTlsSign*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpComputeTlsSign));
                if(!pstData)
                {
                    break;
                }
                pstData->eType = E_SEC_DP_COMPUTE_SIGN;
                pstData->pstStrct = pstMsg->pvParam;
                pvMsg = pstData;
                break;
        }
#endif
#ifdef M_SEC_CFG_TLS
        case  M_SEC_DP_MSG_GET_TLS_CIPHERSUITE:
        {
                St_SecDpGetTlsCipherSuite*                 pstData = NULL;
                
                WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                             "SEC: received signal M_SEC_DP_MSG_GET_CIPHERSUITE \n"));
                pstData = \
                    (St_SecDpGetTlsCipherSuite* )WE_MALLOC((WE_ULONG)sizeof(St_SecDpGetTlsCipherSuite));
                if(!pstData)
                {
                    break;
                }
                (void)WE_MEMSET(pstData,0,sizeof(St_SecDpGetTlsCipherSuite));
                pstData->eType = E_SEC_DP_GET_TLS_CIPHERSUITE;
                pstData->pstStrct = pstMsg->pvParam;
                pvMsg = pstData;
        break;
        }
        case M_SEC_DP_MSG_VERIFY_TLS_CERT_CHAIN:
        {
            St_SecDpVerifyTlsCertChain*            pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal SEC_MAIN_SIG_E_SEC_DP_TLS_VERIFY_CERT_CHAIN \n"));
            pstData = \
                (St_SecDpVerifyTlsCertChain*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpVerifyTlsCertChain));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpVerifyTlsCertChain));
            pstData->eType = E_SEC_DP_VERIFY_TLS_CERT_CHAIN;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        case M_SEC_DP_MSG_EXCHANGE_TLS_KEY:
            {
                St_SecDpTlsKeyXch*                    pstData = NULL;
                
                WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                    "SEC: received signal M_SEC_DP_MSG_EXCHANGE_TLS_KEY \n"));
                pstData = \
                    (St_SecDpTlsKeyXch*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpTlsKeyXch));
                if(!pstData)
                {
                    break;
                }
                (void)WE_MEMSET(pstData,0,sizeof(St_SecDpTlsKeyXch));
                pstData->eType = E_SEC_DP_XCH_TLS_KEY;
                pstData->pstStrct = pstMsg->pvParam;
                pvMsg = pstData;
                break;
            }
        case M_SEC_DP_MSG_TLS_PRF:
        {
            St_SecDpTlsPrf*                        pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal SEC_MAIN_SIG_M_SEC_DP_MSG_TLS_PRF\n"));
            pstData = \
                (St_SecDpTlsPrf*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpTlsPrf));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpTlsPrf));
            pstData->eType = E_SEC_DP_TLS_PRF;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
#endif    
    
        case M_SEC_DP_MSG_ESTABLISH_CONN:
        {
            St_SecDpConnEst*                pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal SEC_MAIN_SIG_E_SEC_DP_ESTABLISH_CONN\n"));
            pstData = \
                (St_SecDpConnEst*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpConnEst));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpConnEst));
            pstData->eType = E_SEC_DP_ESTABLISHED_CONN;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_TERMINATE_CONN:
        {
            St_SecDpConnTmt*                 pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_TERMINATE_CONN\n"));
            pstData = \
                (St_SecDpConnTmt*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpConnTmt));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpConnTmt));
            pstData->eType = E_SEC_DP_TERMINATED_CONN;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;    
        }
        
        case M_SEC_DP_MSG_RM_PEER_LINKS:
        {
            St_SecDpRmPeerLinks*                pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_RM_PEER_LINKS\n"));
            pstData = \
                (St_SecDpRmPeerLinks*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpRmPeerLinks));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpRmPeerLinks));
            pstData->eType = E_SEC_DP_RM_PEER_LINKS;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;  
        }
        
        case M_SEC_DP_MSG_LINK_PEER_SESSION:
        {
            St_SecDpPeerLinkSession*              pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_LINK_PEER_SESSION \n"));
            pstData = \
                (St_SecDpPeerLinkSession*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpPeerLinkSession));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpPeerLinkSession));
            pstData->eType = E_SEC_DP_LINK_PEER_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;  
        }
        
        case M_SEC_DP_MSG_LOOKUP_PEER:
        {
            St_SecDpLookupPeer*                     pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_LOOKUP_PEER \n"));
            pstData = \
                (St_SecDpLookupPeer*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpLookupPeer));
            if(!pstData)
            {
                if(pstMsg->pvParam)
                {
                    Sec_SearchPeerResp(pstMsg->hItype, ((St_SecSearchPeer *)(pstMsg->pvParam))->iTargetID,
                                 M_SEC_ERR_INSUFFICIENT_MEMORY, 0,0,0);
                }
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpLookupPeer));
            pstData->eType = E_SEC_DP_LOOKUP_PEER;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_ACTIVE_SESSION:
        {
            St_SecDpActiveSession*                  pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_ACTIVE_SESSION \n"));
            pstData = \
                (St_SecDpActiveSession*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpActiveSession));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpActiveSession));
            pstData->eType = E_SEC_DP_ACTIVE_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_INVALID_SESSION:
        {
            St_SecDpInvalidSession*              pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                        "SEC: received signal M_SEC_DP_MSG_INVALID_SESSION \n"));
            pstData = \
                (St_SecDpInvalidSession*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpInvalidSession));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpInvalidSession));
            pstData->eType = E_SEC_DP_INVALID_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_FETCH_SESSION:
        {
            St_SecDpfetchSession*                   pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_FETCH_SESSION \n"));
            pstData = \
                (St_SecDpfetchSession*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpfetchSession));
            if(!pstData)
            {               
                Sec_SessionGetResp(pstMsg->hItype, ((St_SecFetchSessionInfo *)(pstMsg->pvParam))->iTargetID,
                                  M_SEC_ERR_INSUFFICIENT_MEMORY, 0,NULL,0,NULL,0,NULL,0);
                    
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpfetchSession));
            pstData->eType = E_SEC_DP_FETCH_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_UPDATE_SESSION:
        {
            St_SecDpUpdateSession*                  pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_UPDATE_SESSION \n"));
            pstData = \
                (St_SecDpUpdateSession*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpUpdateSession));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpUpdateSession));
            pstData->eType = E_SEC_DP_UPDATE_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
#ifdef M_SEC_CFG_WTLS
        case M_SEC_DP_MSG_VIEW_CUR_CERT:
        {
            St_SecDpViewCurCert*            pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_VIEW_CUR_CERT \n"));
            pstData = \
                (St_SecDpViewCurCert*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpViewCurCert));
            if(!pstData)
            {               
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpViewCurCert));
            pstData->eType = E_SEC_DP_VIEW_CUR_CERT;
            pstData->pstStrct = pstMsg->pvParam;
            pstData->iOptions = pstMsg->iParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_VIEW_SESSION:
        {
            St_SecDpViewSessionInfo*            pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_VIEW_SESSION \n"));
            pstData = \
                (St_SecDpViewSessionInfo*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpViewSessionInfo));
            if(!pstData)
            {
                break;
            } 
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpViewSessionInfo));
            pstData->eType = E_SEC_DP_VIEW_SESSION;
            pstData->pstStrct = pstMsg->pvParam;
            pstData->iOptions = pstMsg->iParam;
            pvMsg = pstData;
            break;
        }
        
        case M_SEC_DP_MSG_GET_CUR_SEC_CLASS:
        {
            St_SecDpGetCurSecClass*         pstData = NULL;

            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received signal M_SEC_DP_MSG_GET_CUR_SEC_CLASS\n"));
            pstData = \
                (St_SecDpGetCurSecClass*)WE_MALLOC((WE_ULONG)sizeof(St_SecDpGetCurSecClass));
            if(!pstData)
            {
                break;
            }
            (void)WE_MEMSET(pstData,0,sizeof(St_SecDpGetCurSecClass));
            pstData->eType = E_SEC_DP_GET_CUR_SEC_CLASS;
            pstData->pstStrct = pstMsg->pvParam;
            pvMsg = pstData;
            break;
        }
#endif
        default: 
        {
            WE_LOG_MSG ((WE_LOG_DETAIL_LOW, (WE_UINT8)0,  
                         "SEC: received unknown signal: %d n", pstMsg->iType));
            break;
        }    
    }
    
    if (pvMsg)
    {
        if (NULL == SEC_CUR_FUNCTION)
        {
            SEC_WANTS_TO_RUN = TRUE;
        }
        ((St_SecDpParameter*)pvMsg)->hItype = pstMsg->hItype; 
        Sec_DpHandleQueueNodeAdd(hSecHandle, (St_SecDpParameter*)pvMsg);
        if(SEC_STATES_STOP == SEC_STATES)    
        {           
            SEC_STATES = SEC_STATES_START;               
        }
    }
    
    pstMsg->pvParam = NULL;
    Sec_MsgRemove (hSecHandle, pstMsg);
    return;
}
#define SEC_PKCOUT       (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pstPKCOut)
#define SEC_MSG_QUEUE_HEAD                  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pstMsgQueueHead)
#define SEC_BREW_IRSA               (((St_PublicData *)(((ISecB*)hSecHandle)->hPrivateData))->m_pIRsa) 
#define SEC_UECALLBACK       (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_SecUeCB)
#define SEC_RESPCALLBACK       (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_SecRespCB)
#define SEC_UECALLBACKDATA  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvUeCBData)
#define SEC_RESPCALLBACKDATA  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvRespCBData)

WE_VOID Sec_DpResetVariable(WE_HANDLE hSecHandle)
{
    /*modify by Bird 061205 for each interface can run independently ,and do not influence other interfaces*/
#if 0   
    ISec * pISec = NULL;
    
    /*if current function is not the release interface*/
    if(NULL != SEC_CUR_FUNCTION)
    {
        if(hSecHandle != (((St_SecDpParameter*)SEC_CUR_FUNCTION)->hItype))
        {
            /*added by bird 061117*/
            pISec = (ISec*)(((St_SecDpParameter*)SEC_CUR_FUNCTION)->hItype);
            SEC_REGCALLBACK(pISec->m_pIShell,((St_PublicData*)(pISec->hPrivateData))->m_cb,
                Sec_RunMainFlow,pISec);
            return;
        }
    }
 #endif
    /*1.Free global pointer*/
    /*certificates list*/
    if(NULL != SEC_UECALLBACKDATA)
    {
        Sec_ReleaseUeCB(hSecHandle);
        SEC_UECALLBACKDATA = NULL;
    }
    if(NULL != SEC_RESPCALLBACKDATA)
    {
        Sec_ReleaseRespCB(hSecHandle);
        SEC_RESPCALLBACKDATA = NULL;
    }
/*

    if(NULL != SEC_CERTIDS)
    {
        WE_FREE (SEC_CERTIDS);
        SEC_CERTIDS = NULL;
    }
    
    if(NULL != SEC_USER_CERT)
    {
        WE_FREE(SEC_USER_CERT);
        SEC_USER_CERT = NULL;
    }
*/ /*tang 070508 for simple linux*/
}






