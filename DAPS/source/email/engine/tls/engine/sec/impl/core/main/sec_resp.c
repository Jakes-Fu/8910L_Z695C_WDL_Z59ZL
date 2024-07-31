/*==================================================================================================
    FILE NAME : sec_resp.c
    MODULE NAME : SEC
          
    GENERAL DESCRIPTION:
        In this file, define the response function prototype. and all the response 
        function will invoke the callback registered by the client.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date                   Author            Number              Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07     Kevin Yang                                    create sec_resp.c
    2007-05-17     tang                          Delete cert manage and signtext
==================================================================================================*/

/*******************************************************************************
*   Include File Section
*******************************************************************************/

#include "sec_comm.h" 
//#include "sec_eds.h"
//#include "sec_sicmsg.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define M_SEC_WAP_CALLBACK_FUN              (((ISec *)hSecHandle)->hcbSecEvtFunc)
#define M_SEC_BRSA_CALLBACK_FUN             (((ISec *)hSecHandle)->hcbSecEvtFunc)

#define M_SEC_WAP_CB_PRIVATE_DATA           (((ISec *)hSecHandle)->hSecPrivDataCB)
#define M_SEC_BRSA_CB_PRIVATE_DATA          (((ISec *)hSecHandle)->hSecPrivDataCB)

#define M_SEC_AEECALLBACKK_RESP       (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_cb)
#define M_SEC_ISHELL_RESP                   0

#define M_SEC_RESPCALLBACK       0
#define M_SEC_RESPCALLBACKDATA  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvRespCBData)
#define M_SEC_RESPCALLBACKEVENT  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iRespEvent)

static WE_VOID Sec_RunRespCallBackFun(WE_VOID* pvData);
#if 1
static WE_VOID Sec_FreeRespCBStruc(WE_INT32 iEvent,WE_VOID* pvData);
#endif

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*==================================================================================================
FUNCTION: 
    Sec_GetPrfResultResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get prf result and return the result to the stk.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]: Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_UCHAR * pucBuf[IN]:Pointer to the data buffer of prf result.
    WE_INT32 iBufLen[IN]:The length of the buffer.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_GetPrfResultResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                              const WE_UCHAR * pucBuf, WE_INT32 iBufLen)
{
    St_SecGetPrfResp*   pstPrf    = NULL;
    
    if(NULL == hSecHandle)
    {    
        return ;
    }
    pstPrf = (St_SecGetPrfResp*)WE_MALLOC(sizeof(St_SecGetPrfResp));
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "Sec_GetPrfResultResp(iTargetID = %d, iResult = %d, iBufLen = %u )\n",\
              iTargetID, iResult, iBufLen ));
    if (NULL == pstPrf) 
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    (void)WE_MEMSET( pstPrf,0,sizeof(St_SecGetPrfResp) );
    pstPrf->iTargetID = iTargetID;
    pstPrf->usResult = (WE_UINT16)iResult;    
    pstPrf->pucBuf = (WE_UCHAR*)WE_MALLOC((WE_ULONG)iBufLen);
    if((NULL == pstPrf->pucBuf) && (0 != iBufLen))
    {
        WE_FREE(pstPrf);        
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    (WE_VOID)WE_MEMCPY(pstPrf->pucBuf,pucBuf,(WE_UINT32)iBufLen);
    pstPrf->usBufLen = (WE_UINT16)iBufLen;
    
    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_WtlsGetPrfResultResp\n"));
    
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_GETPRF_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstPrf;
    
    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);   

}
/*==================================================================================================
FUNCTION: 
    Sec_WtlsVerifySveCertResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    verify gateway certificate chain and return the result.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_VerifySvrCertResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult)
{     
    St_SecVerifySvrCertResp*  pstVerSvrCert = NULL;
    
    if(NULL == hSecHandle)
    {        
        return ;
    }
    pstVerSvrCert = (St_SecVerifySvrCertResp*)WE_MALLOC(sizeof(St_SecVerifySvrCertResp));
    if((NULL == pstVerSvrCert) )
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }   
    pstVerSvrCert->iTargetID = iTargetID;
    pstVerSvrCert->usResult = (WE_UINT16)iResult;
    
    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_WtlsVerifySvrCertResp\n"));
    
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_VERIFYSVRCERT_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstVerSvrCert;

    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);   
    
}
#ifdef M_SEC_CFG_TLS
/*==================================================================================================
FUNCTION: 
    Sec_TlsGetCipherSuiteResp
CREATE DATE:
    2006-07-07
AUTHOR:
    tang
DESCRIPTION:
    Get cipher suite, and return the result to the caller
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    const WE_UCHAR * pucCipherSuites[IN]:a sequence of elements of cipher suite.
    WE_INT32 iCipherSuitesLen[IN]:The length of the cipher suite.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_VOID Sec_TlsGetCipherSuiteResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                                const WE_UCHAR * pucCipherSuites, WE_INT32 iCipherSuitesLen)
{    
    St_SecTlsGetCipherSuitesResp  *pstGetCipherSuite = NULL ;
    
    if (NULL == hSecHandle)
    {
        return ;
    }
    pstGetCipherSuite = (St_SecTlsGetCipherSuitesResp *)WE_MALLOC(sizeof(St_SecTlsGetCipherSuitesResp));
    if (!pstGetCipherSuite)
    {        
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    pstGetCipherSuite->pucCipherSuites = (WE_UCHAR *)WE_MALLOC( iCipherSuitesLen);
    if ((!pstGetCipherSuite->pucCipherSuites)&& (0 != iCipherSuitesLen) )
    {
        M_SEC_SELF_FREE(pstGetCipherSuite);
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    
    pstGetCipherSuite->iTargetID = iTargetID;
    pstGetCipherSuite->usResult = (WE_UINT16)iResult;
    pstGetCipherSuite->usCipherSuitesLen = (WE_UINT16)iCipherSuitesLen;
    (WE_VOID)WE_MEMCPY(pstGetCipherSuite->pucCipherSuites,pucCipherSuites,iCipherSuitesLen);

    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_TlsGetCipherSuiteResp:iResult=%d,iCipherSuitesLen=%d",
                iResult,iCipherSuitesLen));

    M_SEC_RESPCALLBACKEVENT= G_TLS_GETCIPHERSUITE_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstGetCipherSuite;
    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);   

}
/*==================================================================================================
FUNCTION: 
    Sec_SSLGetMasterSecretResp
CREATE DATE:
    2006-07-07
AUTHOR:
    tang
DESCRIPTION:
    compute the signature and give the result.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_UCHAR *pucMasterSecret[IN]:pointer to the master secret.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_SSLGetMasterSecretResp(WE_HANDLE hSecHandle,  WE_INT32 iResult,
                         WE_INT32 iTargetID,WE_UCHAR *pucMasterSecret)
{    
    St_SecSSLGetMasterSecretResp  *pstSSLGetMasterSecretResp = NULL ;
    
    if (NULL == hSecHandle)
    {
        return ;
    }
    pstSSLGetMasterSecretResp = (St_SecSSLGetMasterSecretResp *)WE_MALLOC(sizeof(St_SecSSLGetMasterSecretResp));
    if (NULL == pstSSLGetMasterSecretResp)
    {        
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    (void)WE_MEMSET( pstSSLGetMasterSecretResp,0,sizeof(St_SecSSLGetMasterSecretResp) );
    pstSSLGetMasterSecretResp->pucMasterSecret = (WE_UCHAR *)WE_MALLOC(SEC_STRLEN((const WE_CHAR *)pucMasterSecret) + 1);
    if ((!pstSSLGetMasterSecretResp->pucMasterSecret) && (0 != (SEC_STRLEN((const WE_CHAR *)pucMasterSecret) + 1)))
    {
        M_SEC_SELF_FREE(pstSSLGetMasterSecretResp);
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    
    pstSSLGetMasterSecretResp->iTargetID = iTargetID;
    pstSSLGetMasterSecretResp->usResult = (WE_UINT16)iResult;
    pstSSLGetMasterSecretResp->iMasterSecretLen = (WE_UINT16)(SEC_STRLEN((const WE_CHAR *)pucMasterSecret) + 1);
    (WE_VOID)WE_MEMCPY(pstSSLGetMasterSecretResp->pucMasterSecret, pucMasterSecret, SEC_STRLEN((const WE_CHAR *)pucMasterSecret));
    pstSSLGetMasterSecretResp->pucMasterSecret[SEC_STRLEN((const WE_CHAR *)pucMasterSecret)] = '\0';
    
    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_TlsGetCipherSuiteResp:iResult=%d,iCipherSuitesLen=%d",
                iResult, SEC_STRLEN((const WE_CHAR *)pucMasterSecret) + 1));

    M_SEC_RESPCALLBACKEVENT= G_TLS_GETMASTERSECRET_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstSSLGetMasterSecretResp;
    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 

}

#endif
/*==================================================================================================
FUNCTION: 
    Sec_CompSigResp
CREATE DATE:
    2006-07-07
AUTHOR:
    tang
DESCRIPTION:
    compute the signature and give the result.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_UCHAR * pucSignature[IN]:Pointer to the digital signature.
    WE_INT32 iSignatureLen[IN]:The length of the sign nature.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
void Sec_CompSigResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                         const WE_UCHAR * pucSignature, WE_INT32 iSignatureLen)
{    
    St_SecCompSignResp*  pstCompSig = NULL;	
    if(NULL == hSecHandle)
    {
        return ;
    }
    pstCompSig = (St_SecCompSignResp*)WE_MALLOC(sizeof(St_SecCompSignResp));
    if((NULL == pstCompSig) )
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }   
    (void)WE_MEMSET( pstCompSig,0,sizeof(St_SecCompSignResp) );
    pstCompSig->iTargetID = iTargetID;
    pstCompSig->usResult = (WE_UINT16)iResult;
    
    pstCompSig->pucSignature = (WE_UCHAR*)WE_MALLOC((WE_ULONG)iSignatureLen);
    if((NULL == pstCompSig->pucSignature) && (0 != iSignatureLen))
    {
        M_SEC_SELF_FREE(pstCompSig);
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    (WE_VOID)WE_MEMCPY(pstCompSig->pucSignature,pucSignature,(WE_UINT32)iSignatureLen);
    pstCompSig->usSignatureLen = (WE_UINT16)iSignatureLen;
    
    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_WtlsCompSigResp\n"));
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_COMPUTESIGN_RESP;

    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstCompSig;
    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);    

}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_WtlsGetCipherSuiteResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Get cipher suite, and return the result to the caller
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_UCHAR * pucCipherMethods[IN]:a sequence of elements of cipher suite.
    WE_INT32 iCipherMethodsLen[IN]:The length of the cipher method.
    WE_UCHAR * pucKeyExchangeIds[IN]:pointer including key exchange methods.
    WE_INT32 iKeyExchangeIdsLen[IN]:The length of the key exchange id.
    WE_UCHAR * pucTrustedKeyIds[IN]:Pointer to the trusted CA DN.
    WE_INT32 iTrustedKeyIdsLen[IN]:The length of the trusted key id.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef M_SEC_CFG_WTLS
void Sec_WtlsGetCipherSuiteResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                                const WE_UCHAR * pucCipherMethods, WE_INT32 iCipherMethodsLen,
                                const WE_UCHAR * pucKeyExchangeIds, WE_INT32 iKeyExchangeIdsLen,
                                const WE_UCHAR * pucTrustedKeyIds, WE_INT32 iTrustedKeyIdsLen)
{    
        St_SecGetCipherSuiteResp*  pstGetCipherSuite = NULL;
        if(NULL == hSecHandle)
        {        
            return ;
        }
        pstGetCipherSuite = (St_SecGetCipherSuiteResp*)WE_MALLOC(sizeof(St_SecGetCipherSuiteResp));
        if((NULL == pstGetCipherSuite) )
        {
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        }    
        (void)WE_MEMSET( pstGetCipherSuite,0,sizeof(St_SecGetCipherSuiteResp) );
        pstGetCipherSuite->iTargetID = iTargetID;
        pstGetCipherSuite->usResult = (WE_UINT16)iResult;
        
        pstGetCipherSuite->pucCipherMethods = (WE_UCHAR *)WE_MALLOC((WE_ULONG)iCipherMethodsLen);
        if((NULL == pstGetCipherSuite->pucCipherMethods) && (0 != iCipherMethodsLen))
        {
            M_SEC_SELF_FREE(pstGetCipherSuite);
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        }
        (WE_VOID)WE_MEMCPY( pstGetCipherSuite->pucCipherMethods,pucCipherMethods,(WE_UINT32)
                                iCipherMethodsLen );
        pstGetCipherSuite->usCipherMethodsLen = (WE_UINT16)iCipherMethodsLen;
        
        pstGetCipherSuite->pucKeyExchangeIds = (WE_UCHAR *)WE_MALLOC((WE_ULONG)iKeyExchangeIdsLen);
        if((NULL == pstGetCipherSuite->pucKeyExchangeIds) && (0 != iKeyExchangeIdsLen ))
        {
            M_SEC_SELF_FREE(pstGetCipherSuite->pucCipherMethods);
            M_SEC_SELF_FREE(pstGetCipherSuite);
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        }
        (WE_VOID)WE_MEMCPY(pstGetCipherSuite->pucKeyExchangeIds,pucKeyExchangeIds,(WE_UINT32)
    iKeyExchangeIdsLen);
        pstGetCipherSuite->usKeyExchangeIdsLen = (WE_UINT16)iKeyExchangeIdsLen;
        
        pstGetCipherSuite->pucTrustedKeyIds = (WE_UCHAR *)WE_MALLOC((WE_ULONG)iTrustedKeyIdsLen);
        if((NULL == pstGetCipherSuite->pucTrustedKeyIds) && (0 != iTrustedKeyIdsLen))
        {
            M_SEC_SELF_FREE(pstGetCipherSuite->pucKeyExchangeIds);
            M_SEC_SELF_FREE(pstGetCipherSuite->pucCipherMethods);
            M_SEC_SELF_FREE(pstGetCipherSuite);
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        }
        pstGetCipherSuite->usTrustedKeyIdsLen = (WE_UINT16)iTrustedKeyIdsLen;
        (WE_VOID)WE_MEMCPY( pstGetCipherSuite->pucTrustedKeyIds,pucTrustedKeyIds,(WE_UINT32)
    iTrustedKeyIdsLen );
    
        WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_WtlsGetCipherSuiteResp\n"));
        /*fill the struct*/
        M_SEC_RESPCALLBACKEVENT= G_GETCIPHERSUITE_RESP;
        M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstGetCipherSuite;
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);   

}
#endif
/*==================================================================================================
FUNCTION: 
    Sec_KeyExchangeResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get key exchange, ang return the result to the stk
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
    WE_CHAR * pucPublicValue[IN]:A public value computed by the key exchange 
                                method to be sent to the server side.
    WE_INT32 iPublicValueLen[IN]:The length of the public value.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_KeyExchangeResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                             WE_INT32 iMasterSecretId, const WE_UCHAR * pucPublicValue,
                             WE_INT32 iPublicValueLen)
{    
    St_SecKeyExchResp*   pstKeyExch = NULL;
    if(NULL == hSecHandle)
    {
        return ;
    }
    pstKeyExch = (St_SecKeyExchResp*)WE_MALLOC(sizeof(St_SecKeyExchResp));
    if((NULL == pstKeyExch) )
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }   
    (void)WE_MEMSET( pstKeyExch,0,sizeof(St_SecKeyExchResp) );
    pstKeyExch->iTargetID = iTargetID;
    pstKeyExch->usResult = (WE_UINT16)iResult;
    pstKeyExch->ucMasterSecretId = (WE_UINT8)iMasterSecretId;
    
    pstKeyExch->pucPreMsKey = (WE_UCHAR *)WE_MALLOC((WE_ULONG)iPublicValueLen);
    if((NULL == pstKeyExch->pucPreMsKey) && (0 != iPublicValueLen))
    {
        M_SEC_SELF_FREE(pstKeyExch);
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    (WE_VOID)WE_MEMCPY(pstKeyExch->pucPreMsKey,pucPublicValue,(WE_UINT32)iPublicValueLen);
    pstKeyExch->usPreMsKeyLen = (WE_UINT16)iPublicValueLen;
    
    WE_LOG_MSG((0,(WE_UINT8)0,"SEC:....Sec_WtlsKeyExchangeResp\n"));
    
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_KEYEXCH_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstKeyExch;
    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);    


}

/*==================================================================================================
FUNCTION: 
    Sec_SearchPeerResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    response for search peer operation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]:The value of the result.
    WE_UINT8 ucConnection_type[IN]:the connection type.
    WE_INT32 iMasterSecretID[IN]:ID of the master secret.
    WE_INT32 iSecurityID[IN]:ID of the security.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_SearchPeerResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                        WE_UINT8 ucConnectionType,WE_INT32 iMasterSecretID,
                        WE_INT32 iSecurityID)
{
    St_SecSearchPeerResp*  pstSearchPeer = NULL;
    if(NULL == hSecHandle)
    {
        return ;
    }
    pstSearchPeer = (St_SecSearchPeerResp*)WE_MALLOC(sizeof(St_SecSearchPeerResp));
    if((NULL == pstSearchPeer) )
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    pstSearchPeer->iTargetID = iTargetID;
    pstSearchPeer->usResult = (WE_UINT16)iResult;
    pstSearchPeer->ucConnectionType = ucConnectionType;
    pstSearchPeer->ucMasterSecretId = (WE_UINT8)iMasterSecretID;
    pstSearchPeer->iSecId = iSecurityID;

    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_SEARCHPEER_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstSearchPeer;

    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);
}
/*==================================================================================================
FUNCTION: 
    Sec_SessionGetResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    response for get session information operation.
ARGUMENTS PASSED:
   WE_HANDLE hSecHandle[IN]:Global data handle.
   WE_INT32 iTargetID[IN]:The identity of the invoker.
   WE_INT32 iResult[IN]:The value of the result.
   WE_UINT8 ucSessionOptions[IN]:The value of the session option.
   WE_UCHAR * pucSessionId[IN]:Pointer to the ID of the session.
   WE_UINT8 ucSessionIdLen[IN]: Length of the session id.
   WE_UINT8 ucCipherSuite[2][IN]:array for cipher suite.
   WE_UINT8 ucCompressionAlg[IN]: The value of th alg.
   WE_UCHAR * pucPrivateKeyId[IN]:Pointer to the Id of the private key.
   WE_UINT32 uiCreationTime[IN]:The value of the creation time.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_SessionGetResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult,
                        WE_UINT8 ucSessionOptions, const WE_UCHAR * pucSessionId,
                        WE_UINT8 ucSessionIdLen, WE_UINT8 *ucCipherSuite,
                        WE_UINT8 ucCompressionAlg, const WE_UCHAR * pucPrivateKeyId,
                        WE_UINT32 uiCreationTime)
{
        St_SecSessionGetResp*   pstGetSession = NULL;
        if(NULL == hSecHandle)
        {
            return ;
        }
        pstGetSession = (St_SecSessionGetResp*)WE_MALLOC(sizeof(St_SecSessionGetResp));
        if((NULL == pstGetSession) )
        {
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        } 
        (WE_VOID)WE_MEMSET(pstGetSession, 0x00, sizeof(St_SecSessionGetResp));
        pstGetSession->iTargetID = iTargetID;
        pstGetSession->usResult = (WE_UINT16)iResult;
        if (iResult == M_SEC_ERR_OK)
        {    
            pstGetSession->ucSessionOptions = ucSessionOptions;
            
            pstGetSession->pucSessionId = (WE_UCHAR*)WE_MALLOC((WE_ULONG)ucSessionIdLen);
            if((NULL == pstGetSession->pucSessionId) && (0 != ucSessionIdLen))
            {
                WE_LOG_MSG((0,0,"SEC:Sec_SessionGetResp Session ID Malloc Error!\n"));
                M_SEC_SELF_FREE(pstGetSession);
                SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle
                                        ); 
                return;
            }
            (WE_VOID)WE_MEMCPY(pstGetSession->pucSessionId,pucSessionId,ucSessionIdLen);
            pstGetSession->usSessionIdLen = (WE_UINT16)ucSessionIdLen;
            if (NULL == ucCipherSuite)
            {
                pstGetSession->aucCipherSuite[0] = 0xff;
                pstGetSession->aucCipherSuite[1] = 0xff;
            }
            else
            {
                pstGetSession->aucCipherSuite[0] = ucCipherSuite[0];
                pstGetSession->aucCipherSuite[1] = ucCipherSuite[1];
            }
            pstGetSession->ucCompressionAlg = ucCompressionAlg;
            
            pstGetSession->pucPrivateKeyId = (WE_UCHAR*)SEC_STRDUP((WE_CHAR*)pucPrivateKeyId);
            if(NULL == pstGetSession->pucPrivateKeyId)
            {
                WE_LOG_MSG((0,0,"SEC:Sec_SessionGetResp PrivateKeyId Malloc Error!\n"));
                M_SEC_SELF_FREE(pstGetSession->pucSessionId);
                M_SEC_SELF_FREE(pstGetSession);
                SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle
    ); 
                return;
            }
            pstGetSession->uiCreationTime = uiCreationTime;
        }
        /*fill the struct*/
        M_SEC_RESPCALLBACKEVENT = G_SESSIONGET_RESP;
    
        M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstGetSession;
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);
}

/*==================================================================================================
FUNCTION: 
    Sec_GetCurSvrCertResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    view current used certificate information 
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]: The value of the reslut.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef M_SEC_CFG_WTLS
void Sec_GetCurSvrCertResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult)
{    
        St_SecGetCurSvrCertResp* pstGetCurCert = NULL;
        if(NULL == hSecHandle)
        {
            return ;
        }
        pstGetCurCert = (St_SecGetCurSvrCertResp*)WE_MALLOC(sizeof(St_SecGetCurSvrCertResp));
        if((NULL == pstGetCurCert) )
        {
            SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
            return;
        }
        pstGetCurCert->iTargetID = iTargetID;
        pstGetCurCert->usResult = (WE_UINT16)iResult;
        
        /*fill the struct*/
        M_SEC_RESPCALLBACKEVENT= G_GETCURSVRCERT_RESP;
        M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstGetCurCert;
    
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);  
}
/*==================================================================================================
FUNCTION: 
    Sec_GetSessionInfoResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    view the current session information
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iResult[IN]: The value of the reslut.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_GetSessionInfoResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iResult)
{       
    St_SecGetSessionInfoResp* pstGetSessionInfo = NULL;
    if(NULL == hSecHandle)
    {
        return ;
    }
    pstGetSessionInfo = (St_SecGetSessionInfoResp*)WE_MALLOC(sizeof(St_SecGetSessionInfoResp));
    if((NULL == pstGetSessionInfo) )
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    pstGetSessionInfo->iTargetID = iTargetID;
    pstGetSessionInfo->usResult = (WE_UINT16)iResult;
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_GETSESSIONINFO_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstGetSessionInfo;

    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);

}
/*==================================================================================================
FUNCTION: 
    Sec_WtlsCurClassResp
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get current sec class response
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.
    WE_INT32 iTargetID[IN]:The identity of the invoker.
    WE_INT32 iSecClass[IN]:The value of the sec class.
    WE_INT32 iInfoAvailable[IN]:The flg of the info available.
    WE_INT32 iCertAvailable[IN]:The flg of the cert available.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_WtlsCurClassResp(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecClass,
                          WE_INT32 iInfoAvailable, WE_INT32 iCertAvailable)
{    
    St_SecGetWtlsCurClassResp*   pstCurSecCls = NULL;
    
    if (NULL == hSecHandle)
    {
        return ;
    }
    pstCurSecCls = (St_SecGetWtlsCurClassResp*)WE_MALLOC(sizeof(St_SecGetWtlsCurClassResp));
    if (NULL == pstCurSecCls)
    {
        SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle); 
        return;
    }
    pstCurSecCls->iTargetID = iTargetID;
    pstCurSecCls->ucCertReady = (WE_UINT8)iCertAvailable;
    pstCurSecCls->ucSessionInfoReady = (WE_UINT8)iInfoAvailable;
    pstCurSecCls->ucSecClass = (WE_UINT8)iSecClass;    
    /*fill the struct*/
    M_SEC_RESPCALLBACKEVENT = G_WTLSCURCLASS_RESP;
    M_SEC_RESPCALLBACKDATA = (WE_VOID*)pstCurSecCls;

    SEC_REGCALLBACK(M_SEC_ISHELL_RESP,M_SEC_RESPCALLBACK,Sec_RunRespCallBackFun,hSecHandle);

}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_RunRespCallBackFun
CREATE DATE:
    2006-12-09
AUTHOR:
    Bird 
DESCRIPTION:
    callback function of response 
ARGUMENTS PASSED:
    WE_VOID* pvData:data to resolve
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_RunRespCallBackFun(WE_VOID* hSecHandle)
{
    WE_INT32 iEvent = 0;
    WE_VOID* pvCBData = NULL;
    
    if (NULL == hSecHandle)
    {
        return;
    }
    iEvent = M_SEC_RESPCALLBACKEVENT;
    pvCBData = M_SEC_RESPCALLBACKDATA;
    /*set to null before call back*/
    M_SEC_RESPCALLBACKDATA = NULL;
    if (NULL != M_SEC_BRSA_CALLBACK_FUN)
    {
        ((Fn_ISecEventHandle)(M_SEC_BRSA_CALLBACK_FUN))(iEvent,
            (void *)(pvCBData),M_SEC_BRSA_CB_PRIVATE_DATA);
    }
    Sec_FreeRespCBStruc(iEvent,pvCBData);
    
}
/*==================================================================================================
FUNCTION: 
    Sec_ReleaseRespCB
CREATE DATE:
    2006-12-19
AUTHOR:
    Bird
DESCRIPTION:
    free response callback data when release function is called, if the callback does not excute
ARGUMENTS PASSED:
    WE_VOID* pvData:data to resolve
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID Sec_ReleaseRespCB(WE_VOID* hSecHandle)
{
    WE_INT32 iEvent = 0;
    WE_VOID* pvCBData = NULL;
    if( NULL == hSecHandle)
    {
        return;
    }
    iEvent = M_SEC_RESPCALLBACKEVENT;
    pvCBData = M_SEC_RESPCALLBACKDATA;
    Sec_FreeRespCBStruc(iEvent,pvCBData);
}

#if 1
/*==================================================================================================
FUNCTION: 
    Sec_FreeRespCBStruc
CREATE DATE:
    2006-12-19
AUTHOR:
    Bird
DESCRIPTION:
    free response callback data when the callback is called
ARGUMENTS PASSED:
    WE_INT32 iEvent:event
    WE_VOID* pvData:data
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_VOID Sec_FreeRespCBStruc(WE_INT32 iEvent,WE_VOID* pvData)
{
    WE_LOG_MSG((0,0,"SEC:Sec_FreeCBStruc\n"));
    if(NULL == pvData)
    {
        return;
    }
    switch(iEvent)
    {
        case G_GETPRF_RESP:
            {
                St_SecGetPrfResp* pstData = (St_SecGetPrfResp*)pvData;
                M_SEC_SELF_FREE(pstData->pucBuf);
                
            }
            break;
        case G_VERIFYSVRCERT_RESP:
            /*
            St_SecVerifySvrCertResp;
            */
            break;
        case G_GETCIPHERSUITE_RESP:
            {
                St_SecGetCipherSuiteResp* pstData = (St_SecGetCipherSuiteResp*)pvData;
                M_SEC_SELF_FREE(pstData->pucCipherMethods);
                M_SEC_SELF_FREE(pstData->pucKeyExchangeIds);
                M_SEC_SELF_FREE(pstData->pucTrustedKeyIds);
            }
            break;
#ifdef M_SEC_CFG_TLS
        case G_TLS_GETCIPHERSUITE_RESP:
            {          
                St_SecTlsGetCipherSuitesResp* pstData = (St_SecTlsGetCipherSuitesResp*)pvData;
                M_SEC_SELF_FREE(pstData->pucCipherSuites);           
            }
            break;
        case G_TLS_GETMASTERSECRET_RESP:
            {          
                St_SecSSLGetMasterSecretResp* pstData = (St_SecSSLGetMasterSecretResp *)pvData;
                M_SEC_SELF_FREE(pstData->pucMasterSecret);           
            }
            break;
#endif
        case G_KEYEXCH_RESP:
            {
                St_SecKeyExchResp* pstData = (St_SecKeyExchResp*)pvData;
                M_SEC_SELF_FREE(pstData->pucPreMsKey);
            }
            break;
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
        case G_COMPUTESIGN_RESP:
            {
                St_SecCompSignResp* pstData =(St_SecCompSignResp*)pvData ;
                M_SEC_SELF_FREE(pstData->pucSignature);
            }
            break;
#endif
        case G_SEARCHPEER_RESP:
            /*
            St_SecSearchPeerResp;
            */
            break;
        case G_SESSIONGET_RESP:
            {
                St_SecSessionGetResp* pstData = (St_SecSessionGetResp*)pvData ;
                M_SEC_SELF_FREE(pstData->pucPrivateKeyId);
                M_SEC_SELF_FREE(pstData->pucSessionId);
            }
            break;
#ifdef M_SEC_CFG_WTLS
        case G_GETCURSVRCERT_RESP:
            /*
            St_SecGetCurSvrCertResp;
            */
            break;
        case G_GETSESSIONINFO_RESP:
            /*
            St_SecGetSessionInfoResp;
            */
            break;
        case G_WTLSCURCLASS_RESP:
            /*
            St_SecGetWtlsCurClassResp;
            */
            break;
#endif
        default:
            WE_LOG_MSG((0,0,"SEC:Sec_FreeCBStruc, Unknown Event\n"));
            break;
                
    }
    M_SEC_SELF_FREE(pvData);
}
#endif

