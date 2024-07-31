/*==================================================================================================
    FILE NAME : sec_wimresp.c
    MODULE NAME : SEC

    GENERAL DESCRIPTION:
        In this file, define the response function for WIM module, and will be updated later.
          
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
        
    Modification                              Tracking
    Date                   Author            Number                     Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-19      Stone An                                            create sec_wimresp.c
    
==================================================================================================*/

/*******************************************************************************
*   Include File Section
*******************************************************************************/
#include "sec_comm.h"


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Define Macro Function start*/
#define   M_SEC_WANTS_TO_RUN      (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iWantsToRun)
#define   M_SEC_USER_ASKED            (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iUserReq)
#define   M_SEC_NO_CERT_DIALOG_SHOWN        (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iNocertUeShown)
#define   M_SEC_STATES                     (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iStates)

/*Define Macro Function end*/

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
        Sec_WimWtlsGetCipherSuiteResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        omit.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_VOID Sec_WimWtlsGetCipherSuiteResponse (WE_HANDLE hSecHandle, 
                                WE_INT32 iTargetID, WE_INT32 iResult,
                                const WE_UCHAR * pucCipherMethods, WE_INT32 iCipherMethodsLen,
                                const WE_UCHAR *pucKeyExchangeIds, WE_INT32 iKeyExchangeIdsLen,
                                const WE_UCHAR *pucTrustedKeyIds, WE_INT32 iTrustedKeyIdsLen, 
                                WE_INT32  iTooManyCerts)
{   
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
        "Sec_WimWtlsGetCipherSuiteResponse(iTargetID = %d, iResult = %d, cipherMethodsLen = %d, keyExchangeIdsLen = %d, trustedKeyIdsLen = %d, too_many_certs = %d)\n",\
        iTargetID, iResult,  iCipherMethodsLen, iKeyExchangeIdsLen, iTrustedKeyIdsLen, iTooManyCerts));
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"cipherMethods =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucCipherMethods, iCipherMethodsLen);
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"keyExchangeIds =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucKeyExchangeIds, iKeyExchangeIdsLen);
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"trustedKeyIds =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucTrustedKeyIds, iTrustedKeyIdsLen);

    
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    if (iResult == M_SEC_ERR_OK)
    {            
        if ((iTrustedKeyIdsLen == 0) && (M_SEC_USER_ASKED == 0)
            && (iTooManyCerts == 0))
        {
            M_SEC_NO_CERT_DIALOG_SHOWN = 1;
            M_SEC_STATES = (WE_INT32)E_STATE_UE_CONFIRM;
        }
        else
        {
            if (M_SEC_USER_ASKED == 0)
            {                  
                M_SEC_NO_CERT_DIALOG_SHOWN = 0;
            }
            Sec_WtlsGetCipherSuiteResp(hSecHandle, iTargetID, 
                                         iResult, pucCipherMethods, iCipherMethodsLen, pucKeyExchangeIds, 
                                         iKeyExchangeIdsLen, pucTrustedKeyIds, iTrustedKeyIdsLen);
            Sec_DpHandleCleanUp( hSecHandle );
        }
    }
    else
#endif
    {
        Sec_WtlsGetCipherSuiteResp(hSecHandle, iTargetID, 
                                         iResult, pucCipherMethods, iCipherMethodsLen, pucKeyExchangeIds, 
                                         iKeyExchangeIdsLen, pucTrustedKeyIds, iTrustedKeyIdsLen);
        Sec_DpHandleCleanUp( hSecHandle );
    }
    iTooManyCerts = iTooManyCerts;
}
#endif


/*=====================================================================================
FUNCTION: 
        Sec_WimComputeSignatureResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.
        pucSig[IN]: signature.
        iSigLen[IN]: length of signature.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_WimComputeSignatureResponse(WE_HANDLE hSecHandle,
                                                    WE_INT32 iTargetID, WE_INT32 iResult,
                                                    const WE_UCHAR *pucSig, WE_INT32 iSigLen)
{
    if (NULL == hSecHandle)
    {
        return;
    }
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimComputeSignatureResponse(iTargetID = %d, iResult = %d, sigLen = %d)\n",\
              iTargetID, iResult, iSigLen));
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"sig =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucSig, iSigLen);

    M_SEC_WANTS_TO_RUN = 1;
    
    Sec_CompSigResp(hSecHandle, iTargetID, iResult,
                             pucSig,iSigLen ) ;
    Sec_DpHandleCleanUp( hSecHandle );

}
#endif

/*=====================================================================================
FUNCTION: 
        Sec_WimKeyExchangeResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.        
        iMasterSecretId[IN]: ID of the master secret.
        pucPublicValue[IN]: public value.
        iPublicValueLen[IN]: length of public value.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimKeyExchangeResponse (WE_HANDLE   hSecHandle, WE_INT32 iTargetID, 
                                 WE_INT32 iResult, WE_INT32 iMasterSecretId,
                                 const WE_UCHAR* pucPublicValue, WE_INT32 iPublicValueLen)
{
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
      "Sec_WimWtlsKeyExchangeResponse(iTargetID = %d, iResult = %d, masterSecretId = %d, publicValueLen = %d)\n",\
      iTargetID, iResult, iMasterSecretId, iPublicValueLen));
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"publicValue =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucPublicValue, iPublicValueLen);
    
    M_SEC_WANTS_TO_RUN = 1;
    
    Sec_KeyExchangeResp(hSecHandle, iTargetID, iResult, 
                                        iMasterSecretId, pucPublicValue, iPublicValueLen);
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimGetPrfResultResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.    
        pucBuf[IN]: pointer to the data buffer.
        iBufLen[IN]: The length of the buffer.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimGetPrfResultResponse (WE_HANDLE   hSecHandle, 
                         WE_INT32 iTargetID, WE_INT32 iResult,
                         const WE_UCHAR *pucBuf, WE_INT32 iBufLen)
{
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
      "Sec_WimGetPrfResultResponse (iTargetID =%d, iResult = %d, bufLen = %d)\n",\
      iTargetID, iResult, iBufLen));
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"buf =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucBuf, iBufLen);
    
    Sec_GetPrfResultResp(hSecHandle, iTargetID, iResult, pucBuf, iBufLen);
    /* set  M_SEC_WANTS_TO_RUN in Sec_DpHandleCleanUp.  */
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimVerifyCertChainResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.    
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimVerifyCertChainResponse(WE_HANDLE   hSecHandle, WE_INT32 iTargetID, 
                                           WE_INT32 iResult, WE_INT32 iConnType)
{
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimVerifyCertChainResponse(iTargetID = %d, iResult = %d)\n", iTargetID, iResult));

    M_SEC_WANTS_TO_RUN = 1;
    
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE          
    if (iResult != M_SEC_ERR_OK)
    {
        M_SEC_WANTS_TO_RUN = 0;
        if (iResult == M_SEC_ERR_NO_MATCHING_ROOT_CERTIFICATE)
        {            
            Sec_iUeConfirm(hSecHandle,iTargetID, M_SEC_ID_CONFIRM_8, M_SEC_ID_OK, M_SEC_ID_CANCEL);
        }
        else if (iResult == M_SEC_ERR_CERTIFICATE_EXPIRED)
        {
            Sec_iUeConfirm(hSecHandle,iTargetID, M_SEC_ID_CONFIRM_11, M_SEC_ID_OK, M_SEC_ID_CANCEL);
        }
        else
        {
            Sec_iUeConfirm(hSecHandle,iTargetID, M_SEC_ID_CONFIRM_10, M_SEC_ID_OK, M_SEC_ID_CANCEL);
        }
        
        if (M_SEC_WTLS_CONNECTION_TYPE == iConnType)
        {
            M_SEC_STATES = (WE_INT32)E_STATE_HANDLE_UE_CONFIRM;           
        }
#ifdef M_SEC_CFG_TLS
        else
        {
            M_SEC_STATES = (WE_INT32)E_STATE_TLS_HANDLE_UE_CONFIRM;        
        }
#endif
    }
    else
#endif 
    {
        Sec_VerifySvrCertResp(hSecHandle, iTargetID, iResult);
        Sec_DpHandleCleanUp( hSecHandle );
    }

}

/*=====================================================================================
FUNCTION: 
        Sec_WimDeletePeerLinkSessionResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iResult[IN]: result of the calling function.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
          omit.
=====================================================================================*/
WE_VOID Sec_WimDeletePeerLinkSessionResponse (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult)
{    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,"Sec_WimDeletePeerLinkSessionResponse(iResult = %d)\n", iResult));
    
#ifdef M_SEC_CFG_SHOW_WARNINGS
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_iUeWarning(hSecHandle,iTargetID, iResult);
    }
#endif
    iTargetID = iTargetID;
    iResult = iResult;
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimPeerLinkSessionResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iResult[IN]: result of the calling function.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimPeerLinkSessionResponse (WE_HANDLE hSecHandle, WE_INT32 iResult)
{
    #ifdef M_SEC_CFG_SHOW_WARNINGS
    WE_INT32 iTargetID = 0;
    #endif
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
                "Sec_WimPeerLinkSessionResponse(iResult = %d)\n", iResult));    
    
#ifdef M_SEC_CFG_SHOW_WARNINGS
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_iUeWarning(hSecHandle, iTargetID, iResult);
    }                   
#endif
    iResult = iResult;
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimPeerSearchResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.
        ucConnectionType[IN]: type of connection.
        iMasterSecretId[IN]: Id of master secret.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimPeerSearchResponse (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                WE_INT32 iResult, WE_UINT8 ucConnectionType, WE_INT32 iMasterSecretId)
{   
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimPeerSearchResponse(iTargetID = %d, iResult = %d, connectionType = %u, masterSecretId = %d)\n",
              iTargetID, iResult, ucConnectionType, iMasterSecretId));

    Sec_SearchPeerResp(hSecHandle, iTargetID, iResult, ucConnectionType,
                                 iMasterSecretId, Sec_WimGetNewSecIdInfo( hSecHandle));
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimSessionActiveResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iResult[IN]: result of the calling function.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimSessionActiveResponse (WE_HANDLE hSecHandle, WE_INT32 iResult)
{
    #ifdef M_SEC_CFG_SHOW_WARNINGS    
    WE_INT32 iTargetID = 0;
    #endif
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimSessionActiveResponse(iResult = %d)\n", iResult));    
#ifdef M_SEC_CFG_SHOW_WARNINGS
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_iUeWarning(hSecHandle, iTargetID, iResult);
    }                
#endif
    iResult = iResult;
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimSessionInvalidateResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iResult[IN]: result of the calling function.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimSessionInvalidateResponse (WE_HANDLE hSecHandle, WE_INT32 iResult)
{
    #ifdef M_SEC_CFG_SHOW_WARNINGS    
    WE_INT32 iTargetID = 0;
    #endif
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimSessionInvalidateResponse(iResult = %d)\n", iResult));    
#ifdef M_SEC_CFG_SHOW_WARNINGS
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_iUeWarning(hSecHandle, iTargetID, iResult);
    }                
#endif
    iResult = iResult;
    Sec_DpHandleCleanUp( hSecHandle );
}

/*=====================================================================================
FUNCTION: 
        Sec_WimGetSessionMastResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        omit.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimGetSessionMastResponse (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                WE_INT32 iResult, WE_UINT8 ucSessionOptions, 
                                const WE_UCHAR *pucSessionId, WE_UINT8 ucSessionIdLen,
                                WE_UINT8 aucCipherSuite[2], WE_UINT8 ucCompressionAlg,
                                const WE_UCHAR *pucPrivateKeyId, WE_UINT32 uiCreationTime)
{
    if (aucCipherSuite != NULL) 
    {
        WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimGetSessionMastResponse(iTargetID = %d, iResult = %d, sessionOptions = %u, sessionIdLen = %u, cipherSuite[2] = %u%u, compressionAlg = %u, creationTime = %u)\n",\
              iTargetID, iResult, ucSessionOptions, ucSessionIdLen, \
              aucCipherSuite[0], aucCipherSuite[1], ucCompressionAlg, uiCreationTime));
    }
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"sessionId =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucSessionId, ucSessionIdLen);
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"privateKeyId =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucPrivateKeyId, 4);

    
    Sec_SessionGetResp(hSecHandle, iTargetID, iResult, ucSessionOptions, 
                           pucSessionId, ucSessionIdLen, aucCipherSuite, 
                           ucCompressionAlg, pucPrivateKeyId, uiCreationTime);
    Sec_DpHandleCleanUp( hSecHandle );
}


/*=====================================================================================
FUNCTION: 
        Sec_WimWtlsComputeSignatureResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        iResult[IN]: result of the calling function.
        pucSig[IN]: signature.
        iSigLen[IN]: length of signature.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_WimWtlsComputeSignatureResponse(WE_HANDLE hSecHandle,
                                                    WE_INT32 iTargetID, WE_INT32 iResult,
                                                    const WE_UCHAR *pucSig, WE_INT32 iSigLen)
{
    if (NULL == hSecHandle)
    {
        return;
    }
    
    Sec_CompSigResp( hSecHandle,  iTargetID,  iResult, pucSig, iSigLen) ;                             
    Sec_DpHandleCleanUp( hSecHandle );
}
#endif
/*=====================================================================================
FUNCTION: 
        Sec_WimSessionUpdateResponse
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        omit.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iResult[IN]: result of the calling function.
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        omit.   
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_WimSessionUpdateResponse (WE_HANDLE hSecHandle, WE_INT32 iResult)
{
    #ifdef M_SEC_CFG_SHOW_WARNINGS    
    WE_INT32 iTargetID = 0;
    #endif
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_WimSessionUpdateResponse(iResult = %d)\n", iResult));    
#ifdef M_SEC_CFG_SHOW_WARNINGS
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_iUeWarning(hSecHandle, iTargetID, iResult);
    }                
#endif
    iResult = iResult;
    Sec_DpHandleCleanUp( hSecHandle );
}

#ifdef M_SEC_CFG_TLS
void Sec_WimGetCipherSuitesResponse(WE_HANDLE hSecHandle,WE_INT32 iTargetID, WE_INT32 iResult, 
                                     WE_UCHAR *pucCipherSuites, 
                                     WE_INT32 iCipherSuitesLen)
{
        WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
            "Sec_WimGetCipherSuitesResponse(iTargetID = %d, iResult = %d, iCipherSuitesLen = %d)\n",\
            iTargetID, iResult,  iCipherSuitesLen));
        WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"pucCipherSuites =\n"));
        WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pucCipherSuites, iCipherSuitesLen);
   
        
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        if (iResult == M_SEC_ERR_OK)
        {            
            if ((iCipherSuitesLen == 0) && (M_SEC_USER_ASKED == 0))
            {
                M_SEC_NO_CERT_DIALOG_SHOWN = 1;
                M_SEC_STATES = (WE_INT32)E_STATE_TLS_UE_CONFIRM;
            }
            else
            {
                if (M_SEC_USER_ASKED == 0)
                {                  
                    M_SEC_NO_CERT_DIALOG_SHOWN = 0;
                }
                Sec_TlsGetCipherSuiteResp(hSecHandle, iTargetID, 
                                             iResult, pucCipherSuites, iCipherSuitesLen );
                Sec_DpHandleCleanUp( hSecHandle );
            }
        }
        else
#endif
        {
            Sec_TlsGetCipherSuiteResp(hSecHandle, iTargetID, 
                                         iResult, pucCipherSuites, iCipherSuitesLen );
            Sec_DpHandleCleanUp( hSecHandle );

        }

}
#endif

/*************************************************************************************************
end
*************************************************************************************************/
