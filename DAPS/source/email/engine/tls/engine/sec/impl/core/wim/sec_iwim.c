/*=====================================================================================
    FILE NAME : sec_iwim.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will handle the operations such as WIM, which support WAP and BROSWER application.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
      
    Modification                                 Tracking
    Date                      Author           Number                Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-07-18        Stone An                                        create sec_iwim.c
    2007-05-17     tang                          Delete cert manage and signtext
=====================================================================================*/

/*******************************************************************************
*   Include File Section
*******************************************************************************/
#include "sec_comm.h"
#include "oem_sechandle.h"
#ifdef TLS_TEST_PRF
#include "stdio.h"
#include "string.h"
#endif
#include "sec_dhlib.h" 

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define    MASTER_SECRET                  "master secret"

#define    SEC_WE_HANDLE                 (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->hWeHandle) 
#define    SEC_CALLER_TYPE                          (((ISec*)hSecHandle)->iIFType) 
#define    M_SEC_WIM_HAVELOAD                 (((St_PublicData *)(((ISec *)hSecHandle)->hPrivateData))->pcHaveLoad)

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

typedef struct tagSt_SecWimWtlsKeyExRsa
{
    WE_INT32    iTargetID;
    WE_UCHAR    *pucMasterSecret;
} St_SecWimWtlsKeyExRsa;

typedef struct tagSt_SecWimTlsKeyExRsa
{
    WE_INT32    iTargetID;
    WE_UCHAR    *pucMasterSecret;
} St_SecWimTlsKeyExRsa;
typedef struct tagSt_SecWimVerifyCertChain
{
    WE_INT32     iTargetID;
    WE_INT32     iConnType;
    WE_INT32     iUsage; 
    WE_INT32     iNumCerts;    
    WE_UINT8     ucAlg;
    WE_UINT16   usIssuerLen;
    WE_INT32     iIndex;
    St_SecAsn1Certificate   *pstCerts;
    St_SecCertificate       stCert;
} St_SecWimVerifyCertChain;

#ifdef M_SEC_CFG_TLS
typedef struct tagSt_SecWimTlskeyexchangeRsa
{
   WE_INT32       iTargetID;
   WE_UCHAR       *pucMasterSecret;
} St_SecWimTlskeyexchangeRsa;
#endif

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
static WE_INT32 Sec_WimCheckDirExist(WE_HANDLE hSecHandle,const WE_INT8 *pcDirName);
#ifndef M_SEC_CFG_NO_PLATFORM 
static WE_VOID Sec_WimCheckDir(WE_HANDLE hSecHandle);
#endif

#ifdef M_SEC_CFG_WTLS
static WE_VOID Sec_WimWtlsKeyExRsa(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                St_SecPubKeyRsa stPubKey, const WE_UCHAR *pucRandVal,WE_UINT8 ucAlg);
static WE_VOID  Sec_WimWtlsKeyExNULL(WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                                            const WE_UCHAR *pucRandVal,WE_UINT8 ucHashAlg);
#endif
static WE_VOID Sec_WimVerifyCertChainStart(WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                        WE_INT32 iNumCerts, St_SecAsn1Certificate *pstCerts, 
                                        const WE_UCHAR *pucBuf, WE_INT32 iBufLen,
                                        WE_INT32 iConnType, WE_INT32 iUsage);
static WE_VOID Sec_WimVerifyCertChainInter(WE_HANDLE hSecHandle, 
                                        St_SecWimVerifyCertChain *pstP);
static WE_VOID Sec_WimVerifyCertChainRoot(WE_HANDLE hSecHandle, 
                                       St_SecWimVerifyCertChain *pstP);
static WE_VOID Sec_WimVerifyCertChainDeliverResp(WE_HANDLE hSecHandle,
                                              St_SecWimVerifyCertChain *pstP,
                                              WE_INT32 iResult);
/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
        Sec_WimCheckDirExist
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Check whether directories exists or not.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.    
        pcDirName[IN]: name of directory.  
RETURN VALUE:
        M_SEC_ERR_OK: exist.
        other: not exist.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
#ifndef M_SEC_CFG_NO_PLATFORM 
static WE_INT32 Sec_WimCheckDirExist(WE_HANDLE hSecHandle,const WE_INT8 *pcDirName)
{ 
    if((NULL == hSecHandle)||(NULL == pcDirName))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    return Sec_WimCheckFileExist(hSecHandle, (WE_CHAR*)pcDirName);
}
#endif
/*=====================================================================================
FUNCTION: 
        Sec_WimCheckDir
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Check whether all directories exist or not.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle. 
RETURN VALUE:
        none.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
#ifndef M_SEC_CFG_NO_PLATFORM 
static WE_VOID Sec_WimCheckDir(WE_HANDLE hSecHandle)
{
    WE_INT32       iResult = 0; 
    E_WE_ERROR     eRes = E_WE_OK; 
    
    if (NULL == hSecHandle)
    {
        return;
    }    

    iResult = Sec_WimCheckDirExist(hSecHandle, (const WE_INT8 *)M_SEC_FILE_PATH);
    if (iResult != M_SEC_ERR_OK)
    {
        eRes = WE_FILE_MKDIR(SEC_WE_HANDLE, M_SEC_FILE_PATH);
        return;
    }
    eRes = eRes;
}
#endif
/*=====================================================================================
FUNCTION: 
        Sec_iWimInit
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:        
        Initialse WIM.        
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.    
        iTargetID[IN/OUT]: ID of the object.   
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
WE_VOID Sec_iWimInit(WE_HANDLE hSecHandle)
{   
    if (NULL == hSecHandle)
    {
        return;
    }   
    
#ifndef M_SEC_CFG_NO_PLATFORM 
    Sec_WimCheckDir(hSecHandle);

    if (((WE_INT32)E_SEC_WAP == SEC_CALLER_TYPE) &&
        (M_SEC_HAVEREAD_SESSIONPEER &  *(M_SEC_WIM_HAVELOAD)) != M_SEC_HAVEREAD_SESSIONPEER) 
    {
        Sec_WimSessionStart(hSecHandle);
        *(M_SEC_WIM_HAVELOAD) |= M_SEC_HAVEREAD_SESSIONPEER;
    }
#endif
    Sec_WimInit(hSecHandle);
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimEnd
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Terminate WIM.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        omit.
=====================================================================================*/
WE_VOID Sec_iWimEnd(WE_HANDLE hSecHandle)
{
    if (NULL == hSecHandle)
    {
        return;
    }   
#ifndef M_SEC_CFG_NO_PLATFORM
    Sec_WimSessionStop(hSecHandle);
#endif
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimWtlsGetCipherSuite
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Initialise parameters.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        ucServerAuth[IN]: indication of server authentication or not.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        omit.
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_VOID Sec_iWimWtlsGetCipherSuite(WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                                            WE_UINT8 ucServerAuth, WE_HANDLE hTrustKeyIds)
{ 
#if (defined M_SEC_CFG_RC5_CBC) && (!(defined M_SEC_CFG_3DES_CBC_EDE))
    WE_UCHAR       aucCipherMethods[16] = {0};    
#endif
#if (defined M_SEC_CFG_RC5_CBC) && (defined M_SEC_CFG_3DES_CBC_EDE)
    WE_UCHAR       aucCipherMethods[20] = {0};    
#endif
#if (!(defined M_SEC_CFG_RC5_CBC)) && (defined M_SEC_CFG_3DES_CBC_EDE)
    WE_UCHAR       aucCipherMethods[8] = {0};    
#endif    
    WE_INT32         iCipherMethodsLen = 0;    
    WE_INT32         iKeyExIdLen = 0;         /* for user certificate.*/
    WE_UCHAR       *pucKeyExIds = NULL;
    St_SecTrustedKeyIds *pstTrustKeyIds = NULL;
    WE_INT32         iTooManyCerts = 0;

    if (NULL == hSecHandle)
    {
        Sec_WimWtlsGetCipherSuiteResponse(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER,
            NULL, 0, NULL, 0, NULL, 0, 0);
        return;
    }
    
    if (NULL == hTrustKeyIds)
    {
        pstTrustKeyIds = NULL;
        iTooManyCerts = 0;
    }
    else
    {
        pstTrustKeyIds = (St_SecTrustedKeyIds *)hTrustKeyIds;
        iTooManyCerts = pstTrustKeyIds->iTooManyCerts;
    }
    
#if defined(M_SEC_CFG_WTLS_CLASS_2)
    pucKeyExIds = (WE_UCHAR *)WE_MALLOC(21 * sizeof(WE_UCHAR));
#elif defined(M_SEC_CFG_WTLS_CLASS_3) 
    pucKeyExIds = (WE_UCHAR *)WE_MALLOC(800 * sizeof(WE_UCHAR)); 
#endif    
    if (NULL == pucKeyExIds)
    {
        Sec_WimWtlsGetCipherSuiteResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 
            NULL, 0, NULL, 0, NULL, 0, iTooManyCerts);
        return;
    }

#ifdef M_SEC_CFG_RC5_CBC
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_SHA1;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC_56;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_SHA1;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC_40;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_SHA1;

    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_MD5;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC_56;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_MD5;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_RC5_CBC_40;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_MD5;
#endif
#ifdef M_SEC_CFG_3DES_CBC_EDE
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_3DES_CBC_EDE;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_SHA1;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_3DES_CBC_EDE;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_MD5;
#endif
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_NULL;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_SHA1;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_BULK_NULL;
    aucCipherMethods[iCipherMethodsLen++] = (WE_UCHAR)E_SEC_ALG_HASH_MD5;
    
#if defined(M_SEC_CFG_WTLS_CLASS_3)    
    Sec_WimGetKeyExgIds(hSecHandle, pucKeyExIds, &iKeyExIdLen);
#endif
    if (iKeyExIdLen == 0)
    {
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_NULL;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_768;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_512;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
        pucKeyExIds[iKeyExIdLen++] = (WE_UCHAR)0;
    }

#ifdef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    if ((NULL == pstTrustKeyIds) || (0 == pstTrustKeyIds->iTrustKeyIdsLen))
    {
        ucServerAuth = 1;
    }
#endif
    if (ucServerAuth == 1)
    {
        WE_UINT8  iTemp = 0;   
        pucKeyExIds[iTemp++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_NULL;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        pucKeyExIds[iTemp++] = (WE_UCHAR)0;
        Sec_WimWtlsGetCipherSuiteResponse(hSecHandle, iTargetID, 
            M_SEC_ERR_OK, (WE_UCHAR *)aucCipherMethods, 
            iCipherMethodsLen, pucKeyExIds, iTemp,
            NULL, 0, iTooManyCerts);
        WE_FREE(pucKeyExIds);
        return;
    }  
    
    if (NULL == pstTrustKeyIds)
    {
        Sec_WimWtlsGetCipherSuiteResponse(hSecHandle, iTargetID, M_SEC_ERR_OK, (WE_UCHAR *)aucCipherMethods,
                                    iCipherMethodsLen, pucKeyExIds, iKeyExIdLen,
                                    NULL, 0, iTooManyCerts);
    }
    else
    {
        Sec_WimWtlsGetCipherSuiteResponse(hSecHandle, iTargetID, M_SEC_ERR_OK, (WE_UCHAR *)aucCipherMethods,
                                    iCipherMethodsLen, pucKeyExIds, iKeyExIdLen,
                                    pstTrustKeyIds->pucTrustedKeyIds, pstTrustKeyIds->iTrustKeyIdsLen, iTooManyCerts);
    }
    WE_FREE(pucKeyExIds);
}
#endif

/*=====================================================================================
FUNCTION: 
        Sec_iWimGetPrfResult
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        compute master secret.
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]:  ID of the object.
        iMasterSecretId[IN]: ID of the master secret.
        pucSecret[IN]:  pointer to the secret.
        iSecretLen[IN]: the length of the secret.
        pcLabel[IN]:  pointer to the label.
        pucSeed[IN]:  pointer to the seed.
        iSeedLen[IN]: the length of the seed.
        iOutputLen[IN]: the length of the output data.
        ucConnectionType[IN]: M_SEC_WTLS_CONNECTION_TYPE.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
WE_VOID Sec_iWimGetPrfResult(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                              WE_INT32 iMasterSecretId, const WE_UCHAR *pucSecret, 
                              WE_INT32 iSecretLen, const WE_CHAR *pcLabel,
                              const WE_UCHAR *pucSeed, WE_INT32 iSeedLen,
                              WE_INT32 iOutputLen, WE_UINT8 ucConnectionType,WE_UINT8 ucAlg)
{
    WE_INT32     iRes = 0;
    WE_UCHAR     *pucBuf = NULL;
    
    if (NULL == hSecHandle)
    {
        return;
    }

    /* ioutputLen maybe 0. */
    if (0 == iOutputLen)
    {
        Sec_WimGetPrfResultResponse(hSecHandle, iTargetID, M_SEC_ERR_OK, NULL, 0);
        return;
    }
    pucBuf = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iOutputLen * sizeof(WE_UCHAR));
    if (NULL == pucBuf) 
    {
        Sec_WimGetPrfResultResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, NULL, 0);
        return;
    }
#ifdef M_SEC_CFG_WTLS
    if (ucConnectionType == M_SEC_WTLS_CONNECTION_TYPE)
    {
        iRes = Sec_WimGetWTLSMasterSecret(hSecHandle, iMasterSecretId, pucSecret, iSecretLen, 
            pcLabel, pucSeed, iSeedLen, iOutputLen, pucBuf,ucAlg);
    }
    else
#endif
#ifdef M_SEC_CFG_TLS   
    if(ucConnectionType == M_SEC_TLS_CONNECTION_TYPE)
    {
        iRes=Sec_WimTLSPRF(hSecHandle,iMasterSecretId, pucSecret, iSecretLen,pcLabel, pucSeed, iSeedLen, iOutputLen, pucBuf);
    }
    else
#endif
    {
        Sec_WimGetPrfResultResponse(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER, NULL, 0);
    }
    
    Sec_WimGetPrfResultResponse(hSecHandle, iTargetID, iRes, pucBuf, iOutputLen);
    WE_FREE(pucBuf);
}
 
/*=====================================================================================
FUNCTION: 
        Sec_iWimWtlsKeyExchange
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Generate master secret by calling Sec_WimWtlsKeyExRsa.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        iTargetID[IN]: ID of the object.
        stParam[IN] : The parameter value.
        pucRandVal[IN] : Pointer to the rand value.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        omit.
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_VOID Sec_iWimWtlsKeyExchange(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                             St_SecWtlsKeyExchParams stParam, const WE_UCHAR *pucRandVal,WE_UINT8 ucHashAlg)
{ 
    St_SecCertificate stCert = {0};
    St_SecPubKeyRsa   stPubKey = {0};
    WE_UCHAR          *pucCert = NULL;
    WE_UINT16         usCertLen = 0;
    WE_INT32          iRes = 0;  
    
    if (NULL == hSecHandle)
    {
        return;
    }    
    switch (stParam.ucKeyExchangeSuite)
    {
        case E_SEC_WTLS_KEYEXCHANGE_NULL          : 
            Sec_WimWtlsKeyExNULL(hSecHandle, iTargetID, pucRandVal,ucHashAlg);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON      : 
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512  : 
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768  : 
            Sec_WimWtlsKeyExRsa(hSecHandle, iTargetID,
                    stParam.stParams.stKeyParam.stPubKey.stPubKey.stRsaKey, pucRandVal,ucHashAlg);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA           : 
        case E_SEC_WTLS_KEYEXCHANGE_RSA_512       : 
        case E_SEC_WTLS_KEYEXCHANGE_RSA_768       : 
            pucCert = stParam.stParams.stCertificates.pucBuf; 
            if (NULL == pucCert)/* add by Sam [070126] */
            {
                Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER, 0, NULL, 0);
                break;
            }
            if ((*pucCert) == M_SEC_CERTIFICATE_WTLS_TYPE)       
            {            
                iRes = Sec_WtlsCertParse(hSecHandle, pucCert+1, &usCertLen, &stCert);    
                if (iRes != M_SEC_ERR_OK)
                {
                    Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
                        break;
                }
                stPubKey.usExpLen = stCert.stCertUnion.stWtls.usExpLen;
                stPubKey.pucExponent = stCert.stCertUnion.stWtls.pucRsaExponent;
                stPubKey.usModLen = stCert.stCertUnion.stWtls.usModLen;
                stPubKey.pucModulus = stCert.stCertUnion.stWtls.pucRsaModulus;
            }
            else if ((*pucCert) == M_SEC_CERTIFICATE_X509_TYPE)     
            {
                iRes = Sec_WimGetX509PubKey(hSecHandle, pucCert + 1, &(stPubKey.usExpLen),
                    &(stPubKey.pucExponent), &(stPubKey.usModLen), &(stPubKey.pucModulus));
                if (iRes != M_SEC_ERR_OK)
                {
                    Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
                    break;
                }
            }
            else      
            { 
                Sec_WimKeyExchangeResponse(hSecHandle, iTargetID,
                M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE, 0, NULL, 0);
                break;
            }    
            
            Sec_WimWtlsKeyExRsa(hSecHandle, iTargetID, stPubKey, pucRandVal,ucHashAlg);  
            break;
        case E_SEC_WTLS_KEYEXCHANGE_SHARED_SECRET : 
        case E_SEC_WTLS_KEYEXCHANGE_DH_ANON       : 
        case E_SEC_WTLS_KEYEXCHANGE_DH_ANON_512   : 
        case E_SEC_WTLS_KEYEXCHANGE_DH_ANON_768   : 
        case E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON     : 
        case E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON_113 : 
        case E_SEC_WTLS_KEYEXCHANGE_ECDH_ANON_131 : 
        case E_SEC_WTLS_KEYEXCHANGE_ECDH_ECDSA    :
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
            break;
        default   : 
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER, 0, NULL, 0);
            break;
    }
}
/*=====================================================================================
FUNCTION: 
        Sec_WimWtlsKeyExNULL
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Get master secret and store it when KeyExchangeSuite is NULL.
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]: ID of the object.
        pucRandVal[IN]: Pointer to the rand value.
        ucHashAlg[IN]: algorithm of hash.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        Omit.        
=====================================================================================*/
static WE_VOID  Sec_WimWtlsKeyExNULL(WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                                const WE_UCHAR *pucRandVal, WE_UINT8 ucHashAlg)
{
    WE_UCHAR            *pucMasterSecret = NULL;
    WE_INT32            iMasterSecretId = 0;
    WE_INT32            iRes = 0;
    WE_INT32            iIndexI = 0;
    
    if (NULL == hSecHandle)
    {
        return;
    }
    
    pucMasterSecret = (WE_UCHAR *)WE_MALLOC(20 * sizeof(WE_UCHAR));  
    if (NULL == pucMasterSecret)
    {
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
        return;
    }
    /* get the master secret. */
    iRes = Sec_WimGetWTLSMasterSecret(hSecHandle, 0, NULL,  0,
        MASTER_SECRET, pucRandVal, 32, 20, pucMasterSecret,ucHashAlg);
    if (iRes != M_SEC_ERR_OK)
    {
        for (iIndexI=0; iIndexI<20; iIndexI++)
        {
            pucMasterSecret[iIndexI] = 0;
        }
        WE_FREE(pucMasterSecret);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
        return;
    }  
    /* store master secret in session cache . iMasterSecretId equals session number.*/
    Sec_WimSaveMastSecret(hSecHandle, pucMasterSecret, M_SEC_WTLS_MASTER_SECRET_LEN, 
        &iMasterSecretId, &iRes);
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0); 
    }  
    else
    {
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_OK, iMasterSecretId, NULL, 0);
    }
    
    /*clear master secret and free it. */
    for (iIndexI=0; iIndexI<20; iIndexI++)
    {
        pucMasterSecret[iIndexI] = 0;
    }
    WE_FREE(pucMasterSecret);
}
/*=====================================================================================
FUNCTION: 
        Sec_WimWtlsKeyExRsa
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Get master secret.
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]: ID of the object.
        stPubKey[IN]: public key.
        pucRandVal[IN]: Pointer to the rand value.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        Omit.        
=====================================================================================*/
static WE_VOID  Sec_WimWtlsKeyExRsa (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                  St_SecPubKeyRsa stPubKey, const WE_UCHAR *pucRandVal,WE_UINT8 ucAlg)
{
    WE_UCHAR            *pucPreMasterSecret = NULL;
    WE_INT32            iPreMasterSecretLen = 0;
    WE_UCHAR            *pucMasterSecret = NULL;
    WE_UCHAR            *pucTemp = NULL;
    WE_UINT16           usExpLen = stPubKey.usExpLen;
    WE_UINT16           usModLen = stPubKey.usModLen;
    St_SecPubKeyRsa     stKey = {0};
    St_SecWimWtlsKeyExRsa  *pstP = NULL;
    WE_INT32            iRes = 0;
    WE_INT32            iIndexI = 0;
    
    if (NULL == hSecHandle)
    {
        return;
    }
    iPreMasterSecretLen = 20 + usExpLen + usModLen + 4;
    pucPreMasterSecret = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iPreMasterSecretLen * sizeof(WE_UCHAR));
    if (NULL == pucPreMasterSecret)
    {
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
        return;
    }
    
    iRes = Sec_LibGenerateRandom(hSecHandle, (WE_CHAR *)pucPreMasterSecret, 20);
    pucPreMasterSecret[0] = 0x01;
    /* store public key in pre-master secret. */
    pucTemp = pucPreMasterSecret + 20;
    Sec_ExportStrUint16to8(&usExpLen, pucTemp);
    pucTemp += 2;
    for (iIndexI=0; iIndexI<usExpLen; iIndexI++) 
    {
        *pucTemp++ = stPubKey.pucExponent[iIndexI];
    }
    Sec_ExportStrUint16to8(&usModLen, pucTemp);
    pucTemp += 2;
    for (iIndexI=0; iIndexI<usModLen; iIndexI++) 
    {
        *pucTemp++ = stPubKey.pucModulus[iIndexI];   
    }   
    
    pucMasterSecret = (WE_UCHAR *)WE_MALLOC(20 * sizeof(WE_UCHAR));  
    if (NULL == pucMasterSecret)
    {
        WE_FREE(pucPreMasterSecret);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
        return;
    }
    /* get the master secret. */
    iRes = Sec_WimGetWTLSMasterSecret(hSecHandle, 0, pucPreMasterSecret,  iPreMasterSecretLen,
        MASTER_SECRET, pucRandVal, 32, 20, pucMasterSecret,ucAlg);
    if (iRes != M_SEC_ERR_OK)
    {
        for (iIndexI=0; iIndexI<20; iIndexI++)
        {
            pucMasterSecret[iIndexI] = 0;
        }
        WE_FREE(pucMasterSecret);
        WE_FREE(pucPreMasterSecret);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
        return;
    }  
    stKey = stPubKey;
    /* encrypt the random value(20 byte) used public key.*/
    iRes = Sec_PKCRsaPublicKeyEncryption(hSecHandle, iTargetID, stKey, pucPreMasterSecret, 20);  
    /* clear pre-master secret and free it. */
    for (iIndexI=0; iIndexI<iPreMasterSecretLen; iIndexI++)
    {
        pucPreMasterSecret[iIndexI] = 0;
    }
    WE_FREE(pucPreMasterSecret);     
    if (iRes != M_SEC_ERR_OK)
    {
        for (iIndexI=0; iIndexI<20; iIndexI++)
        {
            pucMasterSecret[iIndexI] = 0;
        }
        WE_FREE(pucMasterSecret);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
        return;
    } 
    pstP = (St_SecWimWtlsKeyExRsa *)WE_MALLOC(sizeof(St_SecWimWtlsKeyExRsa));
    if (NULL == pstP)
    {
        for (iIndexI=0; iIndexI<20; iIndexI++)
        {
            pucMasterSecret[iIndexI] = 0;
        }
        WE_FREE(pucMasterSecret);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
        return;
    } 
    (void)WE_MEMSET( pstP,0,sizeof(St_SecWimWtlsKeyExRsa) );
    pstP->iTargetID = iTargetID;
    pstP->pucMasterSecret = pucMasterSecret;    
    iRes = Sec_WimAddNewElement(hSecHandle, iTargetID, M_SEC_WIM_WTLS_KEYEXCHANGE_RSA, pstP);
    if(iRes!= M_SEC_ERR_OK)
    {
        for (iIndexI=0; iIndexI<20; iIndexI++)
        {
            pucMasterSecret[iIndexI] = 0;
        }
        WE_FREE(pucMasterSecret);
        WE_FREE(pstP);
        Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
        return;
    }
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimWtlsKeyExRsaResp
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        store master secret in session cache.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        pstElement[IN] : Global queue.
        pstParam[IN] : The parameter value.  
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        Omit.           
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
WE_VOID Sec_iWimWtlsKeyExRsaResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement, 
                              St_SecCrptEncryptPkcResp *pstParam)
{
    St_SecWimWtlsKeyExRsa *pstP = NULL;
    WE_INT32    iRes = 0;
    WE_INT32    iMasterSecretId = 0;
    WE_INT32    iIndexI = 0; 
    
    if ((NULL==hSecHandle) || (NULL==pstElement)) 
    {
        return;
    }
    pstP = pstElement->pvPointer;
    if(NULL==pstParam)
    {
        Sec_WimKeyExchangeResponse(hSecHandle, pstP->iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY,
                                            0, NULL, 0);
    }
    else
    {
        iRes = pstParam->sResult;
        if (iRes != M_SEC_ERR_OK)
        {
            Sec_WimKeyExchangeResponse(hSecHandle, pstP->iTargetID, iRes, 0, NULL, 0);
        }
        else
        {
            /* store master secret in session cache . iMasterSecretId equals session number.*/
            Sec_WimSaveMastSecret(hSecHandle, pstP->pucMasterSecret, M_SEC_WTLS_MASTER_SECRET_LEN, 
                &iMasterSecretId, &iRes);
            if (iRes != M_SEC_ERR_OK)
            {
                Sec_WimKeyExchangeResponse(hSecHandle, pstP->iTargetID, iRes, 0, NULL, 0); 
            }  
            else
            {
                Sec_WimKeyExchangeResponse(hSecHandle, pstP->iTargetID, M_SEC_ERR_OK, iMasterSecretId, 
                    pstParam->pucBuf, pstParam->sBufLen);
            }
        }
    }
    /*clear master secret and free it. */
    for (iIndexI=0; iIndexI<20; iIndexI++)
    {
        pstP->pucMasterSecret[iIndexI] = 0;
    }
    WE_FREE(pstP->pucMasterSecret);
    WE_FREE(pstP);
    WE_FREE(pstElement);
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimWtlsVerifySvrCertChain
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Interface of verifying server certificate chain. 
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]:  ID of the object.
        pucBuf[IN]:  pointer to the data buf.
        iBufLen[IN]: the length of the data buf.
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
WE_VOID  Sec_iWimWtlsVerifySvrCertChain(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                     const WE_UCHAR *pucBuf, WE_INT32 iBufLen)
{
    if (NULL == hSecHandle)
    {
        return;
    }
    
    Sec_WimVerifyCertChainStart(hSecHandle, iTargetID, 0, NULL, pucBuf, iBufLen,
        M_SEC_WTLS_CONNECTION_TYPE, M_SEC_CERT_USAGE_SERVER_AUTH);
}
#endif
/*=====================================================================================
FUNCTION: 
        Sec_WimVerifyCertChainStart
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Get out certificate from 'certs' or 'buffer'.
        Parse the first certificate of the certificate chain, and decide what to do next .    
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]:  ID of the object.
        iNumCerts[IN]: the number of certificates in certificate chain.
        pstCerts[IN]: certificates maybe stores certificate chain.
        pucBuf[IN]: pointer to the data buf.
        iBufLen[IN]: the length of the data buf.
        iConnType[IN]: type of connect.
        iUsage[IN]: usage of certificate: server or client.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        omit.           
IMPORTANT NOTES:
        Next work is verifying the next certificate in certificate chain, which maybe is root certificate.
=====================================================================================*/
static WE_VOID Sec_WimVerifyCertChainStart (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                                         WE_INT32 iNumCerts,  St_SecAsn1Certificate *pstCerts, 
                                         const WE_UCHAR *pucBuf, WE_INT32 iBufLen, WE_INT32 iConnType, 
                                         WE_INT32 iUsage)
{
    WE_INT32          iResult = 0;
    WE_INT32          iIndexI = 0;
    St_SecWimVerifyCertChain *pstP = NULL;
    WE_UCHAR        *pucCert = NULL;
    St_SecPubKeyRsa  stKey = {0};
    WE_UINT16        usFirstCertLen = 0;
    WE_UCHAR        ucSignatureAlgId = 0;
    WE_UCHAR        *pucTmpCert = NULL;
    WE_UINT8          ucTmpNumCerts = 0;
    
    if (NULL == hSecHandle)
    {
        return;
    }
    
    pstP = (St_SecWimVerifyCertChain *)WE_MALLOC(sizeof(St_SecWimVerifyCertChain));
    if (NULL == pstP)
    {
        Sec_WimVerifyCertChainResponse(hSecHandle, iTargetID,
                                            M_SEC_ERR_INSUFFICIENT_MEMORY, iConnType);
        return;  
    }        
    (void)WE_MEMSET( pstP,0,sizeof(St_SecWimVerifyCertChain) );
    pstP->iTargetID = iTargetID;
    pstP->pstCerts = NULL;
    pstP->iNumCerts = 0;
    pstP->iConnType = iConnType;
    pstP->iUsage = iUsage;
    pstP->iIndex = 0;
     
    /* copy out the certificates in certificate chain.*/
    if (pstCerts != NULL)
    {
        pstP->pstCerts = (St_SecAsn1Certificate *)WE_MALLOC((WE_UINT32)iNumCerts * sizeof(St_SecAsn1Certificate));
        if ((NULL == pstP->pstCerts) && (iNumCerts != 0))
        {
            Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
                                          M_SEC_ERR_INSUFFICIENT_MEMORY); 
            return;
        }  
        (void)WE_MEMSET( pstP->pstCerts,0,sizeof(St_SecAsn1Certificate) );
        for (iIndexI=0; iIndexI<iNumCerts; iIndexI++)
        {
            pstP->pstCerts[iIndexI].ucFormat = pstCerts[iIndexI].ucFormat;
            pstP->pstCerts[iIndexI].pucCert = (WE_UCHAR * )WE_MALLOC \
                (pstCerts[iIndexI].uiCertLen * sizeof(WE_UCHAR));
            if ((NULL == (pstP->pstCerts[iIndexI].pucCert)) && (pstCerts[iIndexI].uiCertLen != 0))
            {
                Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
                    M_SEC_ERR_INSUFFICIENT_MEMORY);
                return;
            }    
            (WE_VOID)WE_MEMCPY (pstP->pstCerts[iIndexI].pucCert, pstCerts[iIndexI].pucCert,
                pstCerts[iIndexI].uiCertLen);
            pstP->pstCerts[iIndexI].uiCertLen = pstCerts[iIndexI].uiCertLen;           
            (pstP->iNumCerts)++;
        }
    } 
    else /* certificates stored in buffer haven't been picked out.*/
    {
        /* get the number of certificates.*/
        iResult = Sec_WimVerifiCertChain(hSecHandle, pucBuf, iBufLen, NULL, &ucTmpNumCerts);         
        if (iResult != M_SEC_ERR_OK)
        {
            Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iResult);
            return;      
        }
        
        pstP->pstCerts = (St_SecAsn1Certificate *)WE_MALLOC \
            (ucTmpNumCerts * sizeof(St_SecAsn1Certificate));
        if ((NULL == (pstP->pstCerts)) && (ucTmpNumCerts != 0))
        {
            Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
                M_SEC_ERR_INSUFFICIENT_MEMORY);
            return;                    
        }
        if((pstP->pstCerts)&&(!ucTmpNumCerts))
        {
            (WE_VOID)WE_MEMSET(pstP->pstCerts,0,sizeof(St_SecAsn1Certificate));
        }
        /*let 'pstP->pstCerts' points to the certificates stored in buffer.*/
        iResult = Sec_WimVerifiCertChain(hSecHandle, pucBuf, iBufLen, 
            pstP->pstCerts, &ucTmpNumCerts);         
        if (iResult != M_SEC_ERR_OK) 
        {
            Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iResult);  
            return;
        }        
        /* copy out the certificates in certificate chain.*/
        for (iIndexI=0; iIndexI<ucTmpNumCerts; iIndexI++) 
        {
            pucTmpCert = pstP->pstCerts[iIndexI].pucCert;
            pstP->pstCerts[iIndexI].pucCert =
                (WE_UCHAR * )WE_MALLOC(pstP->pstCerts[iIndexI].uiCertLen * sizeof(WE_UCHAR));
            if ((NULL == (pstP->pstCerts[iIndexI].pucCert))
                && (pstP->pstCerts[iIndexI].uiCertLen != 0))
            {
                Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
                    M_SEC_ERR_INSUFFICIENT_MEMORY);
                return;
            }       
            (WE_VOID)WE_MEMCPY(pstP->pstCerts[iIndexI].pucCert, pucTmpCert,
                pstP->pstCerts[iIndexI].uiCertLen);                     
            (pstP->iNumCerts)++;
        }
        pstCerts = pstP->pstCerts;
        iNumCerts = pstP->iNumCerts;
    }
    
    /* parse the first certificate .*/
    pucCert = pstCerts[0].pucCert;
#ifdef M_SEC_CFG_WTLS
    if (pstCerts[0].ucFormat == M_SEC_CERTIFICATE_WTLS_TYPE)  /* wtls */
    {
        iResult = Sec_WtlsCertParse(hSecHandle, pucCert, &usFirstCertLen, &(pstP->stCert)); 
        stKey.usExpLen = pstP->stCert.stCertUnion.stWtls.usExpLen;
        stKey.pucExponent = pstP->stCert.stCertUnion.stWtls.pucRsaExponent;
        stKey.usModLen = pstP->stCert.stCertUnion.stWtls.usModLen;
        stKey.pucModulus = pstP->stCert.stCertUnion.stWtls.pucRsaModulus;
        pstP->usIssuerLen = pstP->stCert.stCertUnion.stWtls.ucIssuerLen;
        pstP->ucAlg = M_SEC_SP_RSA;
    }
    else 
#endif
    if (pstCerts[0].ucFormat == M_SEC_CERTIFICATE_X509_TYPE)   /* x509 */
    {
        iResult = Sec_X509CertParse(hSecHandle, pucCert, &usFirstCertLen, &pstP->stCert); 
        pstP->usIssuerLen = pstP->stCert.stCertUnion.stX509.usIssuerLen;
        ucSignatureAlgId = pstP->stCert.stCertUnion.stX509.pucSignatureAlgId \
            [pstP->stCert.stCertUnion.stX509.usSignatureAlgIdLen - 1];
        if (iResult == M_SEC_ERR_OK)
        {
            if ((ucSignatureAlgId == 5) || (ucSignatureAlgId == 4)
#ifdef M_SEC_CFG_MD2
                || (ucSignatureAlgId == 2)
#endif 
                ) 
            {
                pstP->ucAlg = M_SEC_SP_RSA;
                iResult = Sec_X509GetRsaModAndExp(hSecHandle, 
                    pstP->stCert.stCertUnion.stX509.pucPublicKeyVal,
                    &(stKey.pucModulus), &(stKey.usModLen), 
                    &(stKey.pucExponent), &(stKey.usExpLen));
            }
            else
            {
                Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP,
                    M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE);
                return;
            }
        }
    }
    else
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP,
            M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE);
        return;
    }
    
    if (iResult != M_SEC_ERR_OK) 
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iResult);  
        return;
    }    
    
    if (iNumCerts > 1) 
    {
        Sec_WimVerifyCertChainInter(hSecHandle, pstP);
    }
    else
    {
        Sec_WimVerifyCertChainRoot(hSecHandle, pstP);
    }
}

/*=====================================================================================
FUNCTION: 
        Sec_WimVerifyCertInter
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Use public key which is taken out from the next certificate, to verify the former certificate.
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        pstP[IN]: Variable stored parameters about certificate chain.
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
static WE_VOID Sec_WimVerifyCertChainInter(WE_HANDLE hSecHandle, 
                                        St_SecWimVerifyCertChain *pstP)
{
    WE_INT32         iRes = 0;
    WE_INT32         iIndexI = 0;
    WE_INT32         iTargetID = 0;
    WE_UCHAR         *pucPrevCert = NULL;
    WE_UCHAR         *pucCert = NULL;   
    WE_INT32         iPrevFormat = 0;
    WE_INT32         iFormat = 0;
    WE_UINT16        usCertLen = 0;
    St_SecPubKeyRsa  stKey = {0};
    
    if ((NULL == hSecHandle) || (NULL == pstP))
    {
        return;
    }
    iIndexI = pstP->iIndex;
    iTargetID = pstP->iTargetID;
    pucPrevCert = pstP->pstCerts[iIndexI].pucCert;
    iPrevFormat = pstP->pstCerts[iIndexI].ucFormat;
    pucCert = pstP->pstCerts[iIndexI+1].pucCert;
    iFormat = pstP->pstCerts[iIndexI+1].ucFormat;     
    /* Parse the next certificate and get the public key. pstP->stCert point to the next certificate.*/
#ifdef M_SEC_CFG_WTLS
    if (iFormat == M_SEC_CERTIFICATE_WTLS_TYPE)
    {
        iRes = Sec_WtlsCertParse(hSecHandle, pucCert, &usCertLen, &pstP->stCert); 
        stKey.usExpLen = pstP->stCert.stCertUnion.stWtls.usExpLen;
        stKey.pucExponent = pstP->stCert.stCertUnion.stWtls.pucRsaExponent;
        stKey.usModLen = pstP->stCert.stCertUnion.stWtls.usModLen;
        stKey.pucModulus = pstP->stCert.stCertUnion.stWtls.pucRsaModulus;
        pstP->usIssuerLen = pstP->stCert.stCertUnion.stWtls.ucIssuerLen;
    }
    else 
 #endif
    if (iFormat == M_SEC_CERTIFICATE_X509_TYPE)  
    {
        iRes = Sec_X509CertParse(hSecHandle, pucCert, &usCertLen, &pstP->stCert); 
        stKey.usExpLen = 0; 
        pstP->usIssuerLen = pstP->stCert.stCertUnion.stX509.usIssuerLen;
    }
    else
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
            M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE); 
        return;
    }
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes); 
        return;
    }
    /* use the public key to verify the former certificate.*/
#ifdef M_SEC_CFG_WTLS
    if (iPrevFormat == M_SEC_CERTIFICATE_WTLS_TYPE)            
    {    
        iRes = Sec_WimChkWTLSCert(hSecHandle, iTargetID, stKey, pucPrevCert); 
    }
    else 
#endif
    if (iPrevFormat == M_SEC_CERTIFICATE_X509_TYPE)   
    {    
        iRes = Sec_WimChkX509Cert(hSecHandle, iTargetID, 
            pstP->stCert.stCertUnion.stX509.pucPublicKeyVal, 
            pucPrevCert, pstP->iConnType, (iIndexI+1), pstP->iUsage);
    }
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
        return;
    }  

    iRes = Sec_WimAddNewElement(hSecHandle, iTargetID, M_SEC_WIM_VERIFY_CERT_CHAIN_INTERMEDIATE, pstP);
    if(iRes != M_SEC_ERR_OK)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
        return;
    }
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimVerifyCertInterResp
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Response of verify intermediate certificate.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        pstElement[IN] : Global queue.
        pstParam[IN] : The parameter value.  
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
WE_VOID Sec_iWimVerifyCertInterResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement, 
                                 St_SecCrptVerifySignatureResp *pstParam)
{
    St_SecWimVerifyCertChain    *pstP = NULL;
    WE_INT32    iResult = 0;  
    
    if ((NULL==hSecHandle) || (NULL==pstElement))
    {
        return;
    }
    pstP = pstElement->pvPointer;
    if (NULL==pstParam)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, M_SEC_ERR_INSUFFICIENT_MEMORY);
        WE_FREE(pstElement); 
        return;
    }
    iResult = pstParam->sResult;
    WE_FREE(pstElement); 
    
    if (iResult != M_SEC_ERR_OK)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iResult);
        return;
    }
    
    pstP->iIndex++;
    if (pstP->iIndex < (pstP->iNumCerts - 1)) 
    {
        Sec_WimVerifyCertChainInter(hSecHandle, pstP);
    }
    else  /* for root certificate: the 'index' equals to 'NumCerts-1'. */
    {
        Sec_WimVerifyCertChainRoot(hSecHandle, pstP);
    }
}

/*=====================================================================================
FUNCTION: 
        Sec_WimVerifyCertChainRoot
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Get root certificate and parse it. If it's valid, judge whether it is self-signed.
        If root certificate is not self-signed, verify it by its extension.
        At last, verify the former certificate use the root certificate. 
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        pstP[IN]: Variable stored parameters about certificate chain.
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
static WE_VOID Sec_WimVerifyCertChainRoot (WE_HANDLE hSecHandle,
                                        St_SecWimVerifyCertChain *pstP)
{ 
    WE_INT32         iIndexI = 0;
    WE_INT32         iTargetID = 0;
    WE_INT32         iRes = 0;
    WE_INT32         iNumRootCerts = 0;
    WE_UCHAR         *pucPrevCert = NULL;  
    WE_INT32         iPrevFormat = 0;
    WE_INT32         iCertIndex = 0;
    WE_UCHAR         *pucRootCert = NULL;
    WE_UINT16        usRootCertLen = 0;
    WE_INT32         iMatch = 0;
    WE_UINT32        uiNow = 0;
    St_SecCertificate    stCert = {0};
    St_SecPubKeyRsa      stKey = {0};
    WE_UINT16        usSubjectLen = 0;
    WE_UCHAR         *pucKeyIdentifier = NULL;
    WE_UINT16        usKeyIdentifierLen = 0;  
    WE_UINT16        usLen = 0; 
    WE_INT32          iCertType = 0; 
    
    if ((NULL == hSecHandle) || (NULL == pstP))
    {
        return;
    }
    iIndexI = pstP->iIndex;
    iTargetID = pstP->iTargetID;
    pucPrevCert = pstP->pstCerts[iIndexI].pucCert;
    iPrevFormat = pstP->pstCerts[iIndexI].ucFormat;
    iRes = Sec_WimGetNbrOfCACerts(hSecHandle, &iNumRootCerts, M_SEC_LOCAL, NULL);
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
        return;
    }
    for (iCertIndex=0; iCertIndex<iNumRootCerts; iCertIndex++)
    { 
        usLen = 0;
        /* if it is root certificate, get out of it.*/
        iRes = Sec_WimGetCACert(hSecHandle, iCertIndex+1, &pucRootCert,
                                     &usRootCertLen, &iCertType, M_SEC_LOCAL, NULL);
        if (iRes == M_SEC_ERR_OK)
        {
#ifdef M_SEC_CFG_WTLS
            if (M_SEC_IWAPOPT_WTLS_ROOT_CERTS == iCertType) /* wtls format */
            {
                iRes = Sec_WtlsCertParse(hSecHandle, pucRootCert, &usLen, &stCert);
                usSubjectLen = stCert.stCertUnion.stWtls.ucSubjectLen;
                stKey.usExpLen = stCert.stCertUnion.stWtls.usExpLen;
                stKey.pucExponent = stCert.stCertUnion.stWtls.pucRsaExponent;
                stKey.usModLen = stCert.stCertUnion.stWtls.usModLen;
                stKey.pucModulus = stCert.stCertUnion.stWtls.pucRsaModulus;
            }
            else 
#endif
            if (M_SEC_IWAPOPT_X509_ROOT_CERTS == iCertType)  /* x509 formate */
            {
                iRes = Sec_X509CertParse(hSecHandle, pucRootCert, &usLen, &stCert);
                stKey.usExpLen = 0;
                usSubjectLen = stCert.stCertUnion.stX509.usSubjectLen;
            }
            else
            {
                Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP,
                    M_SEC_ERR_GENERAL_ERROR);
                return;
            }            
            if (iRes != M_SEC_ERR_OK)
            {
                Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
                return;
            }            
            
            /*judge if the certificate is valid. */
            uiNow = SEC_CURRENT_TIME();
            if ((uiNow < stCert.iValidNotBefore) ||(stCert.iValidNotAfter < uiNow)) 
            {  
                /* if it's old, search next. */
                continue;
            }            

            /* judge if the subject of root certificate equals the issuer of last certificate.*/
            if (pstP->usIssuerLen == usSubjectLen &&
                !WE_MEMCMP(pstP->stCert.pucIssuer, stCert.pucSubject, usSubjectLen))  
            {                   
                iMatch = TRUE;
            }                
            /* if it not matched, find the subject from extension of root certificate.*/
            if (!iMatch && (iCertType == M_SEC_IWAPOPT_X509_ROOT_CERTS) && 
                ((iPrevFormat) == M_SEC_CERTIFICATE_WTLS_TYPE) &&   
                (pstP->stCert.stCertUnion.stWtls.ucIssuerIdentifierType == 254) &&
                (stCert.ucCertificateVersion == 3))   
            { 
                usKeyIdentifierLen = (WE_UINT16)(stCert.stCertUnion.stX509.uiExtSubjectKeyIdLen);
                pucKeyIdentifier = stCert.stCertUnion.stX509.pucExtSubjectKeyId;
                if ((pstP->usIssuerLen == usKeyIdentifierLen) && 
                    !WE_MEMCMP(pstP->stCert.pucIssuer, pucKeyIdentifier, usKeyIdentifierLen)) 
                {
                    iMatch = TRUE;
                }
            }                
            
            /* verify the former certificate using public key of root certificate. */
            if (TRUE == iMatch)
            {
#ifdef M_SEC_CFG_WTLS
                if (iPrevFormat == M_SEC_CERTIFICATE_WTLS_TYPE)
                {
                    iRes = Sec_WimChkWTLSCert(hSecHandle, iTargetID, stKey, pucPrevCert); 
                }
                else 
#endif
                if (iPrevFormat == M_SEC_CERTIFICATE_X509_TYPE) 
                {
                    iRes = Sec_WimChkX509Cert(hSecHandle, iTargetID, 
                        stCert.stCertUnion.stX509.pucPublicKeyVal, 
                        pucPrevCert, pstP->iConnType, (iIndexI+1), pstP->iUsage);
                }      
                if (iRes == M_SEC_ERR_OK) 
                {          
                    iRes = Sec_WimAddNewElement (hSecHandle, pstP->iTargetID, 
                        M_SEC_WIM_VERIFY_CERT_CHAIN_ROOT,pstP);
                    if(iRes != M_SEC_ERR_OK)
                    {
                        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
                    }
                } 
                else 
                {
                    Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
                }
                return;
            } 
            else 
            {        
                continue;
            }
        } 
        else /* failure of getting the CA certificate.*/
        {
            Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, iRes);
            return;
        }
    } 
    /* check if the former certificate is expired. */
    uiNow = SEC_CURRENT_TIME();
    if ((uiNow < pstP->stCert.iValidNotBefore) ||(pstP->stCert.iValidNotAfter < uiNow)) 
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
            M_SEC_ERR_CERTIFICATE_EXPIRED);
        return;
    }  
    
    Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, 
        M_SEC_ERR_NO_MATCHING_ROOT_CERTIFICATE);
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimVerifyCertRootResp
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Response of verify root certificate.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        pstElement[IN] : Global queue.
        pstParam[IN] : The parameter value.  
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
WE_VOID Sec_iWimVerifyCertRootResp(WE_HANDLE hSecHandle, St_SecWimElement *pstElement,
                                St_SecCrptVerifySignatureResp *pstParam)
{
    St_SecWimVerifyCertChain *pstP = NULL;
    
    if ((NULL==hSecHandle) || (NULL==pstElement))
    {
        return;
    }
    pstP = pstElement->pvPointer;
    WE_FREE(pstElement);
    if(NULL==pstParam)
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, M_SEC_ERR_INSUFFICIENT_MEMORY);
    }
    else
    {
        Sec_WimVerifyCertChainDeliverResp(hSecHandle, pstP, pstParam->sResult);
    }
}

/*=====================================================================================
FUNCTION: 
        Sec_WimVerifyCertChainDeliverResp
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        Interface of response of verifying certificate chain. 
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        pstP[IN]: structure stored parameter.
        iResult[IN]: result of last operation.
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
static WE_VOID Sec_WimVerifyCertChainDeliverResp (WE_HANDLE hSecHandle,
                                               St_SecWimVerifyCertChain *pstP, WE_INT32 iResult)
{
    WE_INT32         iIndexI = 0;
    
    if ((NULL == hSecHandle) || (NULL == pstP))
    {
        return;
    }    

    Sec_WimVerifyCertChainResponse(hSecHandle, pstP->iTargetID, iResult, pstP->iConnType);
    
    for (iIndexI=0; iIndexI<pstP->iNumCerts; iIndexI++)
    {
        WE_FREE(pstP->pstCerts[iIndexI].pucCert);
    }
    if (NULL != (pstP->pstCerts))
    {
        WE_FREE(pstP->pstCerts);
    }
    WE_FREE (pstP);
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimStoreView
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        If find a session file, store view in it.            
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]:Global data handle.  
        pucData: data to be stored in View file.
        iDataLen: length of data.
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
#ifndef M_SEC_CFG_NO_PLATFORM 
WE_VOID Sec_iWimStoreView(WE_HANDLE hSecHandle, const WE_UCHAR *pucData, 
                       WE_INT32 iDataLen)
{
    WE_HANDLE         hFileHandle = NULL;
    WE_CHAR           *pcFileName = M_SEC_SH_NAME;
    WE_UINT32         uiPos = 0;
    WE_LONG          lRes = 0;
    WE_INT32          iResult = 0;
    WE_INT32          iNewPos = 0;    
    WE_UINT32         uiSessionLen = 0;
    WE_UINT32         uiPeerLen = 0;
    WE_UINT32         uiViewLen = 0;
    E_WE_ERROR        eRes = E_WE_OK;
    
    if ((NULL == hSecHandle) || (NULL == pucData) || (0 == iDataLen))
    {
        return;
    }    
  
    if ((WE_INT32)E_SEC_WAP != SEC_CALLER_TYPE) /*is not wtls*/
    {
        return;
    }
    
    eRes = WE_FILE_OPEN(SEC_WE_HANDLE, pcFileName, E_WE_FILE_SET_RDWR, &hFileHandle);
    if(eRes != E_WE_OK)
    {
        eRes = WE_FILE_REMOVE(SEC_WE_HANDLE, pcFileName);
        return;
    }
    /*read session ,peer length*/
    iResult = Sec_WimGetSessionLength(hSecHandle, hFileHandle, &uiSessionLen,
        &uiPeerLen, &uiViewLen, &lRes);
    if(iResult != 0)
    {
        eRes = WE_FILE_CLOSE(hFileHandle);
        eRes = WE_FILE_REMOVE(SEC_WE_HANDLE, pcFileName);
        return;
    }    
    /* View file position */        
    uiPos = 3 * M_FILE_LENGTH_BITS + uiSessionLen + uiPeerLen;
    /*store 'view' data*/
    eRes = WE_FILE_SEEK(hFileHandle, (WE_LONG)uiPos, E_WE_FILE_SEEK_SET, &iNewPos);
    eRes = WE_FILE_WRITE(hFileHandle, (WE_VOID *)pucData, iDataLen, &(iResult));           
    if ((iResult < (WE_INT32)iDataLen) ||(eRes != E_WE_OK))
    {
        eRes = WE_FILE_CLOSE(hFileHandle);
        eRes = WE_FILE_REMOVE(SEC_WE_HANDLE, pcFileName);
        return;
    }
    /*write View file length*/
    uiPos = 2 * M_FILE_LENGTH_BITS;
    eRes = WE_FILE_SEEK(hFileHandle, (WE_LONG)uiPos, E_WE_FILE_SEEK_SET, &iNewPos); 
    eRes = WE_FILE_WRITE(hFileHandle, &iDataLen, M_FILE_LENGTH_BITS, &iResult);
    if ((iResult < M_FILE_LENGTH_BITS) ||(eRes != E_WE_OK))
    {
        eRes = WE_FILE_CLOSE(hFileHandle);
        eRes = WE_FILE_REMOVE(SEC_WE_HANDLE, pcFileName);
        return;
    } 
    eRes = WE_FILE_CLOSE(hFileHandle); 
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimReadView
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        If find a session file, take out 'View' from it. Then delete the file.          
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]:Global data handle.  
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
void Sec_iWimReadView(WE_HANDLE hSecHandle, WE_UCHAR **ppucData, WE_INT32 *piDataLen)
{
    WE_CHAR           *pcFileName = M_SEC_SH_NAME;
    WE_HANDLE        hFileHandle = NULL;
    WE_INT32           iNewPos = 0; 
    WE_INT32           iRes = 0;   
    WE_INT32           iResult = 0;
    E_WE_ERROR      eRes = E_WE_OK;
    WE_UINT32         uiSessionLen =0;
    WE_UINT32         uiPeerLen = 0;
    WE_UINT32         uiViewLen = 0;
    WE_UINT32         uiPos = 0;
    WE_LONG           lReadNum = 0;
    
    if ((NULL == hSecHandle) || (NULL == ppucData) || (NULL == piDataLen))
    {
        return;
    }
    *ppucData = NULL;
    *piDataLen = 0;

    if ((M_SEC_HAVEREAD_VIEWINFO & *(M_SEC_WIM_HAVELOAD)) == M_SEC_HAVEREAD_VIEWINFO)
    {
        return;
    }
    
    iResult = Sec_WimCheckFileExist(hSecHandle, pcFileName);
    if(iResult != M_SEC_ERR_OK)
    {  
        /*the session file can not exist when wim run first time.*/
        return;
    }
    eRes = WE_FILE_OPEN(SEC_WE_HANDLE, pcFileName, E_WE_FILE_SET_RDWR, &(hFileHandle));
    if (eRes != E_WE_OK) 
    {
        return;
    }     
    /*get session and peer data length and check length. */
    iRes = Sec_WimGetSessionLength(hSecHandle, hFileHandle, &uiSessionLen,
                                &uiPeerLen, &uiViewLen, &lReadNum);
    if ((iRes != M_SEC_ERR_OK) || (0 == uiViewLen))
    {
        eRes = WE_FILE_CLOSE(hFileHandle);
        return;
    }
 
    *ppucData = (WE_UCHAR *)WE_MALLOC((uiViewLen) * sizeof(WE_UCHAR));
    if (NULL == *ppucData)
    {
        eRes = WE_FILE_CLOSE(hFileHandle);
        return;
    }
    (void)WE_MEMSET( *ppucData,0,(uiViewLen) * sizeof(WE_UCHAR) );
    uiPos = 3 * M_FILE_LENGTH_BITS + uiSessionLen + uiPeerLen;
    eRes = WE_FILE_SEEK(hFileHandle, (WE_LONG)uiPos , E_WE_FILE_SEEK_SET, &iNewPos); 
    /* read out 'View'. */            
    eRes = WE_FILE_READ(hFileHandle, *ppucData, (WE_LONG)uiViewLen, &(iResult));
    if(eRes != E_WE_OK || iResult < (WE_INT32)uiViewLen)
    {
        WE_FREE(*ppucData);
        *ppucData = NULL;
        eRes = WE_FILE_CLOSE(hFileHandle);
        return;        
    }
    eRes = WE_FILE_CLOSE(hFileHandle);
    *piDataLen = (WE_INT32)uiViewLen;
    *(M_SEC_WIM_HAVELOAD) |= M_SEC_HAVEREAD_VIEWINFO;
    return;
}
#endif

/*=====================================================================================
FUNCTION: 
        Sec_WimFreeRsaElement
CREATE DATE: 
        2006-11-14
AUTHOR: 
        Bird
DESCRIPTION:
        build the request message about user certificate using request info and signature.
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        pstElement[IN]: pointer to the element structure.
RETURN VALUE:
        None
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        omit.           
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
void Sec_WimFreeRsaElement(WE_HANDLE hSecHandle, St_SecWimElement* pstElement)
{
    if((NULL == hSecHandle) || (NULL == pstElement))
    {
        return;
    }
    switch(pstElement->iState)
    {
        /*1*/
        case M_SEC_WIM_WTLS_KEYEXCHANGE_RSA:
        {
            St_SecWimWtlsKeyExRsa *pstP = NULL;
            pstP = pstElement->pvPointer;
            WE_FREE(pstP->pucMasterSecret);
            WE_FREE(pstP);
        }
        break;
        /*2*/
        case M_SEC_WIM_VERIFY_CERT_CHAIN_INTERMEDIATE:
        case M_SEC_WIM_VERIFY_CERT_CHAIN_ROOT:

        {
            St_SecWimVerifyCertChain    *pstP = NULL;
            WE_INT32 iIndexI = 0;
            pstP = pstElement->pvPointer;
            for (iIndexI=0; iIndexI<pstP->iNumCerts; iIndexI++)
            {
                WE_FREE(pstP->pstCerts[iIndexI].pucCert);
            }
            if (NULL != (pstP->pstCerts))
            {
                WE_FREE(pstP->pstCerts);
            }
            WE_FREE (pstP);
        }
        break;
        
        default:
            break;
        
    }
    WE_FREE(pstElement);

}

#ifdef M_SEC_CFG_TLS

WE_VOID Sec_iWimTlsKeyExRsaResp (WE_HANDLE hSecHandle,St_SecWimElement *pstElement, 
                                    St_SecCrptEncryptPkcResp *pstParam)
{
    St_SecWimTlsKeyExRsa *pstP = NULL;
    WE_INT32 iRes = 0;
    
    WE_INT32 iMasterSecretID = 0;
    WE_INT32 iIndexI=0;
#ifdef TLS_TEST_PRF
        FILE* fin = NULL;
        int i = 0;
        char tmp[48];
        char *j=NULL;
        char name[14]="save_aucms_";
        int index=0;
#endif
    
    pstP = pstElement->pvPointer;
    iRes = pstParam->sResult;
    
    if (iRes != M_SEC_ERR_OK) 
    {
        Sec_WimKeyExchangeResponse(hSecHandle,pstP->iTargetID, iRes, 0, NULL, 0);
    }
    
    Sec_WimSaveMastSecret(hSecHandle,pstP->pucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN, 
        &iMasterSecretID, &iRes);
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
      "############Sec_iWimTlsKeyExRsaResp (iMasterSecretID =%d, iRes = %d )\n",\
      iMasterSecretID, iRes ));
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"save pstP->pucMasterSecret =\n"));
    WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, pstP->pucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN);    
    WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"\n"));
#ifdef TLS_TEST_PRF
        index=iMasterSecretID+49;
         j=(char *)&index;
         memcpy(name+11,j,1);
         fin = fopen(name,"w+");
         if(!fin)
         {
            return ;
         }
         for(i=0;i<M_SEC_TLS_MASTER_SECRET_LEN;i++)
         {
             sprintf(tmp,"0x%x, ",pstP->pucMasterSecret[i]);
             fwrite(tmp,strlen(tmp),1,fin);
         }
         fclose(fin);
#endif
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimKeyExchangeResponse(hSecHandle,pstP->iTargetID, iRes, 0, NULL, 0);

    }
    
    Sec_WimKeyExchangeResponse(hSecHandle,pstP->iTargetID, M_SEC_ERR_OK, iMasterSecretID, 
        pstParam->pucBuf, pstParam->sBufLen);
    
    /*clear master secret and free it. */ 
    for (iIndexI=0; iIndexI<M_SEC_TLS_MASTER_SECRET_LEN; iIndexI++)
    {
        pstP->pucMasterSecret[iIndexI] = 0;
    }

    WE_FREE(pstP->pucMasterSecret);
    WE_FREE(pstP);
    WE_FREE(pstElement);
}

/*=====================================================================================
FUNCTION: 
        Sec_iWimTlsGetCipherSuites
CREATE DATE: 
        2007-1-19
AUTHOR: 
        tang
DESCRIPTION:
        Get tls ciphersuiter.          
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]:Global data handle.  
        iTargetID[IN]: ID of the object.
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
void Sec_iWimTlsGetCipherSuites(WE_HANDLE hSecHandle,WE_INT32 iTargetID)
{
  WE_UINT16            usCipher=0;
  WE_INT32             iRes=0;
  WE_INT32             iCipherSuitesLen = 0;
  WE_UCHAR             *pucCipherSuites = NULL;
  WE_UCHAR             *pucP=NULL;
  
/*#define M_ALGORITHM_TEST */

#ifdef M_ALGORITHM_TEST 

WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++#define M_ALGORITHM_TEST+++++++++++++ \n"));
{
    IFile  * pFile = NULL ;
    WE_UINT8 ucGetAlg = 0;
    St_WeHandle *pstWeHandleData = NULL;
    ISecW  *pstSecHandle = (ISecW *)hSecHandle;
    if ( NULL == pstSecHandle )
    {
         DBGPRINTF("%s\n","+++error++,NULL == pstSecHandle");
         Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, M_SEC_ERR_WIM_NOT_INITIALISED, NULL, 0);
         return;
    }
    WE_LOG_MSG((0,(WE_UINT8)0,"++++++++open fs:/shared/tls/ciphersuite.txt++++++++ \n"));
    
    pstWeHandleData = (St_WeHandle*)(((St_PublicData*)(pstSecHandle->hPrivateData))->hWeHandle);
    pFile = IFILEMGR_OpenFile( pstWeHandleData->pIFileMgr, "fs:/shared/tls/ciphersuite.txt", _OFM_READ );
    
    
    WE_LOG_MSG((0,(WE_UINT8)0,"++++++++fs:/shared/tls/ciphersuite.txt++++++++ read \n"));
    (WE_VOID)IFILE_Read(pFile, &ucGetAlg, 1);   
    WE_LOG_MSG((0,(WE_UINT8)0,"++++++++fs:/shared/tls/ciphersuite.txt++++++++ read over \n"));

    if( pFile )
    {
        (WE_VOID)IFILE_Release(pFile);
    }
    WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++get cipher suites ++++++++++++\n"));
 
    pucCipherSuites = (WE_UCHAR *)WE_MALLOC((WE_UINT32)20*2*sizeof(WE_UCHAR));
    if(NULL==pucCipherSuites)
    {
         Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, NULL, 0);
         return;
    }
    pucP = pucCipherSuites;
    switch(ucGetAlg)
    {
        case G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5:/*0x03*/
        {
            iCipherSuitesLen++;   
            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 
            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5  0x03++++++++++++\n")); 
            
            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites);                
        }
        break;
        case G_SEC_TLS_RSA_WITH_RC4_128_MD5:/*0x04*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_WITH_RC4_128_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 
            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_WITH_RC4_128_MD5  0x04 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case G_SEC_TLS_RSA_WITH_RC4_128_SHA:/*0x05*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_WITH_RC4_128_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 

            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_WITH_RC4_128_SHA  0x05 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5:/*0x06*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 

            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5  0x06 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA:/*0x08*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 

            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA  0x08 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case G_SEC_TLS_RSA_WITH_DES_CBC_SHA:/*0x09*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_WITH_DES_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;
            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_WITH_DES_CBC_SHA  0x09 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA:/*0x0a*/
        {
            iCipherSuitesLen++;  
            usCipher = G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 
            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA  0x0a ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
        case 0x32: /*all*/
        {
            iCipherSuitesLen++; /*M_SEC_CFG_3DES_CBC_EDE*/
            iCipherSuitesLen++;                       /*TLS_RSA_EXPORT_WITH_RC4_40_MD5*/
            iCipherSuitesLen++;                       /*TLS_RSA_WITH_RC4_128_SHA*/
            iCipherSuitesLen++;                       /*TLS_RSA_WITH_RC4_128_MD5*/ 
            iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5*/
            iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA*/
            iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_WITH_DES_CBC_SHA*/

            usCipher = G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;

            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;  
            usCipher = G_SEC_TLS_RSA_WITH_RC4_128_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;
            usCipher = G_SEC_TLS_RSA_WITH_RC4_128_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;

            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;

            usCipher = G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2;

            usCipher = G_SEC_TLS_RSA_WITH_DES_CBC_SHA;
            Sec_ExportStrUint16to8(&usCipher, pucP);
            pucP+=2; 
            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++all alg  0x32 ++++++++++++\n")); 

            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
        }
        break;
            
        default:            
            WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++M_SEC_ERR_UNSUPPORTED_METHOD ++++++++++++\n")); 
            Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, M_SEC_ERR_UNSUPPORTED_METHOD, pucCipherSuites, (2*iCipherSuitesLen));
            WE_FREE(pucCipherSuites); 
         break;
    }    
    
    WE_LOG_MSG((0,(WE_UINT8)0,"+++++++++++++get cipher suites ++++++++++++\n"));    
} 

#else /*Not M_ALGORITHM_TEST*/
    #ifdef Test_M_SEC_CFG_3DES_CBC_EDE /*M_SEC_CFG_3DES_CBC_EDE*/
      iCipherSuitesLen++;
    #endif  
  
    #ifdef M_SEC_CFG_RC4
      iCipherSuitesLen++;                       /*TLS_RSA_EXPORT_WITH_RC4_40_MD5*/
      iCipherSuitesLen++;                       /*TLS_RSA_WITH_RC4_128_SHA*/
      iCipherSuitesLen++;                       /*TLS_RSA_WITH_RC4_128_MD5*/ 
    #endif
    
    #ifdef M_SEC_CFG_RC2    
      iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5*/
    #endif
  
    #ifdef M_SEC_CFG_IDEA
      iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_WITH_IDEA_CBC_SHA*/
    #endif
    
    #ifdef M_SEC_CFG_DES40
        iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA*/
        iCipherSuitesLen++;                     /*G_SEC_TLS_RSA_WITH_DES_CBC_SHA*/
    #endif
#ifdef M_SEC_CFG_DHE_RSA    
        iCipherSuitesLen++;                     /*G_SEC_TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA*/
        iCipherSuitesLen++;                     /*G_SEC_TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA*/
        iCipherSuitesLen++;                     /*G_SEC_TLS_DHE_RSA_WITH_DES_CBC_SHA*/
        
#endif
  
    if(iCipherSuitesLen > 0)
    {
         pucCipherSuites = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iCipherSuitesLen*2*sizeof(WE_UCHAR));
         if(NULL==pucCipherSuites)
         {
              Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, NULL, 0);
              return;
         }
         pucP = pucCipherSuites;
    }
    else
    {
        Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, M_SEC_ERR_NOT_IMPLEMENTED, NULL, 0);
        return;
    }
   
    #ifdef Test_M_SEC_CFG_3DES_CBC_EDE     /*M_SEC_CFG_3DES_CBC_EDE*/
    usCipher = G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA;
    Sec_ExportStrUint16to8(&usCipher, pucP);
    pucP+=2;
    #endif
    
    #ifdef M_SEC_CFG_RC4   
      
      usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5;
      Sec_ExportStrUint16to8(&usCipher, pucP);
      pucP+=2; 
  
      
      usCipher = G_SEC_TLS_RSA_WITH_RC4_128_SHA;
      Sec_ExportStrUint16to8(&usCipher, pucP);
      pucP+=2;
     
  
      usCipher = G_SEC_TLS_RSA_WITH_RC4_128_MD5;
      Sec_ExportStrUint16to8(&usCipher, pucP);
      pucP+=2;
  
    #endif 
  
    #ifdef M_SEC_CFG_RC2
        usCipher = G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
    #endif
    
    #ifdef M_SEC_CFG_IDEA
        usCipher = G_SEC_TLS_RSA_WITH_IDEA_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
    #endif
  
  #ifdef M_SEC_CFG_DES40
       
  
        usCipher = G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
  
        usCipher = G_SEC_TLS_RSA_WITH_DES_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
  #endif
#ifdef M_SEC_CFG_DHE_RSA    
        usCipher = G_SEC_TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
        usCipher = G_SEC_TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
        usCipher = G_SEC_TLS_DHE_RSA_WITH_DES_CBC_SHA;
        Sec_ExportStrUint16to8(&usCipher, pucP);
        pucP+=2;
  #endif
  
    Sec_WimGetCipherSuitesResponse(hSecHandle,iTargetID, iRes, pucCipherSuites, (2*iCipherSuitesLen));
    WE_FREE(pucCipherSuites);
  
#endif /*M_ALGORITHM_TEST*/
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimVerifyTLSCertificateChain
CREATE DATE: 
        2007-01-19
AUTHOR: 
        tang
DESCRIPTION:
        Interface of verifying server certificate chain. 
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]:  ID of the object.
        pstCerts[IN]:   pointer to the cert chain.
        iNumCerts[IN]:  number of certificate.
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
void  Sec_iWimVerifyTLSCertificateChain(WE_HANDLE hSecHandle,WE_INT32 iTargetID,
                                        St_SecTlsAsn1Certificate *pstCerts, WE_INT32 iNumCerts)
{
    WE_INT32 iILoop=0;
    
    if(NULL==hSecHandle||NULL==pstCerts)
    {
        return ;
    }
    for (iILoop=0; iILoop<iNumCerts; iILoop++)
    {
        pstCerts[iILoop].ucFormat = M_SEC_CERTIFICATE_X509_TYPE;
    }
    Sec_WimVerifyCertChainStart(hSecHandle,iTargetID, iNumCerts, (St_SecAsn1Certificate*)pstCerts, NULL, 0,  M_SEC_TLS_CONNECTION_TYPE, 
        M_SEC_CERT_USAGE_SERVER_AUTH);
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimTLSkeyExchange
CREATE DATE: 
        2007-01-19
AUTHOR: 
        tang
DESCRIPTION:
        get certificate of certificate chain. 
ARGUMENTS PASSED:
        WE_HANDLE hSecHandle[IN]: Global data handle.
        WE_INT32 iTargetID[IN]:  iTargetID.
        WE_INT32 iIsSsl[IN]:the option of key exchange.
        St_SecTlsKeyExchangeParams *pstParam[IN]:pointer to the structure of St_SecTlsKeyExchangeParams.
        const WE_UCHAR *pucRandval[IN]:random.
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
void Sec_iWimTLSkeyExchange(WE_HANDLE hSecHandle,WE_INT32 iTargetID, 
                            WE_UINT8 ucKeyExchangeAlgorithm,WE_INT32 iIsSsl,
                            St_SecTlsKeyExchangeParams *pstParam, const WE_UCHAR *pucRandval)
{
    WE_UCHAR                    *pucPreMasterSecret=NULL;
    WE_INT32                    iPreMasterSecretLen=48;    
    WE_INT32                    iMasterSecretID = 0;
    WE_UCHAR                    *pucMasterSecret=NULL;    
    WE_INT32                    iRes = M_SEC_ERR_OK;
    WE_UINT16                   usExpLen=0;
    WE_UINT16                   usModLen=0;    
    St_SecPubKeyRsa             stPubKey={0};
    St_SecPubKeyRsa             stTmpCrptKey={0};
    WE_INT32                    iILoop=0;
    WE_UCHAR                    *pucCert=NULL;
    WE_UINT16                    usCertLen=0;
    St_SecWimTlskeyexchangeRsa     *pstP=NULL;
    St_SecCertificate             stCert={0};
    WE_INT32                      usTmpLen=0;
    WE_INT32                      iIsKey=0;
#ifdef M_SEC_CFG_DHE_RSA 
    WE_UCHAR *                   pucPublicValue = NULL ;
    WE_INT32                     iPmsLen = 48 ;
    WE_INT32                     iPublicValueLen = 0;
    WE_INT32                     iLoop =0 ;
#endif
  
    switch (ucKeyExchangeAlgorithm)
    {
    case  E_SEC_TLS_KEYEXCH_NULL            : 
    case  E_SEC_TLS_KEYEXCH_DH_DSS_EXPORT   : 
    case  E_SEC_TLS_KEYEXCH_DH_DSS          : 
    case  E_SEC_TLS_KEYEXCH_DH_RSA_EXPORT   : 
    case  E_SEC_TLS_KEYEXCH_DH_RSA          : 
    case  E_SEC_TLS_KEYEXCH_DH_ANON_EXPORT  : 
    case  E_SEC_TLS_KEYEXCH_DH_ANON         : 
    case  E_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT  : 
        {
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
            return;            
        }
    case  E_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT  : 
    case  E_SEC_TLS_KEYEXCH_DHE_RSA         : 
        {
 #ifdef M_SEC_CFG_DHE_RSA           
            pucMasterSecret = (WE_UCHAR *)WE_MALLOC (M_SEC_TLS_MASTER_SECRET_LEN);
            pucPreMasterSecret = WE_MALLOC((pstParam->stDHParam.stTlsDHParam.usDHPLen) *3* sizeof(unsigned char));
            pucPublicValue = WE_MALLOC((pstParam->stDHParam.stTlsDHParam.usDHPLen)*2 * sizeof(unsigned char));

            if(!pucMasterSecret ||!pucPreMasterSecret || !pucPublicValue)
            {
                if(pucMasterSecret)
                {
                    WE_FREE(pucMasterSecret);
                    pucMasterSecret = NULL ;
                }
                if(pucPreMasterSecret)
                {
                    WE_FREE(pucPreMasterSecret);
                    pucPreMasterSecret = NULL ;
                }
                if(pucPublicValue)
                {
                    WE_FREE(pucPublicValue);
                    pucPublicValue = NULL ;
                }                
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0); 
                return;    
            }
            iRes = SEClib_generateDHKeyPair(hSecHandle,pstParam->stDHParam.stTlsDHParam.pucDHP,pstParam->stDHParam.stTlsDHParam.usDHPLen,
                pstParam->stDHParam.stTlsDHParam.pucDHG, pstParam->stDHParam.stTlsDHParam.usDHGLen, 
                pstParam->stDHParam.stTlsDHParam.pucDHYs,pstParam->stDHParam.stTlsDHParam.usDHYsLen, 
                &pucPublicValue, &iPublicValueLen,
                &pucPreMasterSecret, &iPmsLen );
            
            if (iRes != SEC_ERR_OK)
            {
                WE_FREE (pucMasterSecret);
                WE_FREE (pucPreMasterSecret);
                WE_FREE (pucPublicValue);
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
                return;
            }
            
            if (iIsSsl)
            {
                iRes = Sec_WimSSLPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                    iPmsLen, pucRandval, 64, 48, pucMasterSecret); 
                
            }
            else
            {
                iRes = Sec_WimTLSPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                    iPmsLen,   (const char *)"master secret",\
                    pucRandval, 64, 48, pucMasterSecret); 
                
            }
            if (iRes != SEC_ERR_OK)
            {
                WE_FREE (pucMasterSecret);
                WE_FREE(pucPublicValue);
                WE_FREE(pucPreMasterSecret);
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
                return;
            }
            
            Sec_WimSaveMastSecret(hSecHandle,pucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN, 
                &iMasterSecretID, &iRes);
            
            if (iRes != M_SEC_ERR_OK)
            {
                if (pucPreMasterSecret != NULL)
                {
                    for (iLoop=0; iLoop<iPmsLen; iLoop++)
                    {
                        pucPreMasterSecret[iLoop] = 0;
                    }
                    WE_FREE(pucPreMasterSecret);
                }
                if (pucPublicValue != NULL)
                {
                    WE_FREE(pucPublicValue);
                }
                
                WE_FREE (pucMasterSecret);
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  iRes, 0, NULL, 0); 
                return;
            }
               
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, iMasterSecretID, pucPublicValue, iPublicValueLen);
            
            if (pucPublicValue != NULL)
            {
                WE_FREE(pucPublicValue);
            }
            if (pucPreMasterSecret != NULL)
            {
                for (iLoop=0; iLoop<iPmsLen; iLoop++)
                {
                    pucPreMasterSecret[iLoop] = 0;
                }
                WE_FREE(pucPreMasterSecret);
            }            
            WE_FREE (pucMasterSecret);
            return;
 #else
         Sec_WimKeyExchangeResponse(hSecHandle,iTargetID,  M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
         return;    
 #endif
            
        }
    case  E_SEC_TLS_KEYEXCH_RSA_EXPORT      : 
        {
         if(NULL==pstParam)
         {
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INVALID_PARAMETER, 0, NULL, 0); 
             return ;
         }
         /*get certificate*/
         usCertLen = pstParam->stSecCert.usBufLen;
         pucCert   = pstParam->stSecCert.pucBuf;
         if(usCertLen==0)
         {
            iIsKey = 1;
            usModLen = (*pstParam).stRSAParam.stTlsRSAParam.usModLen;
            usExpLen = (*pstParam).stRSAParam.stTlsRSAParam.usExpLen;
            stPubKey.pucExponent = (WE_UCHAR *)WE_MALLOC(usExpLen*sizeof(WE_UCHAR));
            stPubKey.pucModulus = (WE_UCHAR *)WE_MALLOC(usModLen*sizeof(WE_UCHAR));
            if(!stPubKey.pucExponent || !stPubKey.pucModulus)
            {
                if(stPubKey.pucModulus)
                {
                    WE_FREE(stPubKey.pucModulus);
                    stPubKey.pucModulus=NULL;
                }
                if(stPubKey.pucExponent)
                {
                    WE_FREE(stPubKey.pucExponent);
                    stPubKey.pucExponent=NULL;
                }
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0); 
                return ;
            }
            for (iILoop=0; iILoop<usExpLen; iILoop++) 
            {
                stPubKey.pucExponent[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucExponent[iILoop];
            }
            for (iILoop=0; iILoop<usModLen; iILoop++) 
            {
                stPubKey.pucModulus[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucModulus[iILoop];
            }
         }
         else
         {
            /*
             if ((*pucCert)== 1)  //wtls cert,and get wtls public key                      
             {
                 iRes = Sec_WtlsCertParse(hSecHandle, pucCert, &usCertLen, &stCert);    
                 if (iRes != M_SEC_ERR_OK)
                 {
                     Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
                     return;
                 }
                 stPubKey.usExpLen = stCert.stCertUnion.stWtls.usExpLen;
                 stPubKey.pucExponent = stCert.stCertUnion.stWtls.pucRsaExponent;
                 stPubKey.usModLen = stCert.stCertUnion.stWtls.usModLen;
                 stPubKey.pucModulus = stCert.stCertUnion.stWtls.pucRsaModulus;
             }
             else */ /*tang 20070711*/
             if ((*pstParam->stSecCert.pucBuf) == 48) /*x509 cert,and get certificate public key*/           
             {
                 iRes = Sec_WimGetX509PubKey(hSecHandle, pucCert , &(stPubKey.usExpLen),\
                              &(stPubKey.pucExponent), &(stPubKey.usModLen), &(stPubKey.pucModulus));
             }
             else                                             
             { 
                 Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE,  0, NULL, 0);
                 return;
             }
             if (iRes != M_SEC_ERR_OK)
             {
                 Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
                 return;
             }
             usTmpLen=stPubKey.usModLen;
             if((stPubKey.usModLen >64))
             {
                if(((*pstParam).stRSAParam.stTlsRSAParam.usModLen>0))
                {
                     iIsKey = 1;
                     stPubKey.usModLen = (*pstParam).stRSAParam.stTlsRSAParam.usModLen;
                     stPubKey.usExpLen = (*pstParam).stRSAParam.stTlsRSAParam.usExpLen;
                     stPubKey.pucExponent = (WE_UCHAR *)WE_MALLOC(stPubKey.usExpLen*sizeof(WE_UCHAR));
                     stPubKey.pucModulus = (WE_UCHAR *)WE_MALLOC(stPubKey.usModLen*sizeof(WE_UCHAR));
                     if(!stPubKey.pucExponent || !stPubKey.pucModulus)
                     {
                         if(stPubKey.pucModulus)
                         {
                             WE_FREE(stPubKey.pucModulus);
                             stPubKey.pucModulus=NULL;
                         }
                         if(stPubKey.pucExponent)
                         {
                             WE_FREE(stPubKey.pucExponent);
                             stPubKey.pucExponent=NULL;
                         }
                         Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0); 
                         return ;
                     }
                     for (iILoop=0; iILoop<stPubKey.usExpLen; iILoop++) 
                     {
                         stPubKey.pucExponent[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucExponent[iILoop];
                     }
                     for (iILoop=0; iILoop<stPubKey.usModLen; iILoop++) 
                     {
                         stPubKey.pucModulus[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucModulus[iILoop];
                     }
                }
             }
         }

         pucPreMasterSecret = (WE_UCHAR*) WE_MALLOC((WE_UINT32)iPreMasterSecretLen*sizeof(WE_UCHAR));
         if (pucPreMasterSecret == NULL)
         { 
            if(iIsKey)
            {
                WE_FREE(stPubKey.pucExponent);
                WE_FREE(stPubKey.pucModulus);                
            }
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
            return;
         }
         /*the length of tls random is 48 */
         (WE_VOID)Sec_LibGenerateRandom(hSecHandle,(WE_CHAR*)pucPreMasterSecret,48);
         
         pucPreMasterSecret[0] = M_SEC_TLS_PROTOCOL_VERSION_MAJOR;
         /*the version of tls protocol*/
         if (iIsSsl & G_SEC_TLS_OPTION_SSL_CLIENT_HELLO)
         {
             pucPreMasterSecret[1] = M_SEC_SSL_PROTOCOL_VERSION_MINOR;
         }
         else
         {
             pucPreMasterSecret[1] = M_SEC_TLS_PROTOCOL_VERSION_MINOR;
         }
         
         pucMasterSecret = (WE_UCHAR *)WE_MALLOC (M_SEC_TLS_MASTER_SECRET_LEN);
         if(pucMasterSecret == NULL)
         {
             if(iIsKey)
             {
                 WE_FREE(stPubKey.pucExponent);
                 WE_FREE(stPubKey.pucModulus);                
             }
             WE_FREE(pucPreMasterSecret);
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
             return;
         }
         /*the length of server random data is 64*/
         if (iIsSsl)
         {
             iRes = Sec_WimSSLPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                 iPreMasterSecretLen, pucRandval, 64, 48, pucMasterSecret); 
         }
         else
         {
             iRes = Sec_WimTLSPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                 iPreMasterSecretLen,   (const char *)"master secret",\
                 pucRandval, 64, 48, pucMasterSecret); 
         }
         if (iRes != M_SEC_ERR_OK)
         {
             if(iIsKey)
             {
                 WE_FREE(stPubKey.pucExponent);
                 WE_FREE(stPubKey.pucModulus);                
             }
             for (iILoop=0; iILoop<M_SEC_TLS_MASTER_SECRET_LEN; iILoop++)
             {
                 pucMasterSecret[iILoop] = 0;
             }
             WE_FREE(pucMasterSecret);
             WE_FREE(pucPreMasterSecret);
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
             return;
         }    
         stTmpCrptKey = stPubKey;
         iRes = Sec_PKCRsaPublicKeyEncryption (hSecHandle,iTargetID,stTmpCrptKey, pucPreMasterSecret, iPreMasterSecretLen);       
         for (iILoop=0; iILoop<iPreMasterSecretLen; iILoop++)
         {
             pucPreMasterSecret[iILoop] = 0;
         }
         WE_FREE(pucPreMasterSecret);
        
         if (pstParam->stSecCert.usBufLen == 0)  
         {
             WE_FREE(stPubKey.pucExponent);
             WE_FREE(stPubKey.pucModulus);
         }
         else
         {
             if((usTmpLen >64)&&((*pstParam).stRSAParam.stTlsRSAParam.usModLen>0))
             {
                 WE_FREE(stPubKey.pucExponent);
                 WE_FREE(stPubKey.pucModulus);
             }
         }
         if (iRes != M_SEC_ERR_OK) 
         {
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
         }    
         pstP = (St_SecWimTlskeyexchangeRsa*)WE_MALLOC(sizeof(St_SecWimTlskeyexchangeRsa));
         if(!pstP)
         {
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
             WE_FREE(pucMasterSecret);
             pucMasterSecret =NULL;
             return;
         }
         (void)WE_MEMSET( pstP,0,sizeof(St_SecWimTlskeyexchangeRsa) );
         pstP->iTargetID = iTargetID;
         pstP->pucMasterSecret = pucMasterSecret;        
        
         iRes =Sec_WimAddNewElement(hSecHandle, iTargetID, M_SEC_WIM_TLS_KEYEXCHANGE_RSA, pstP);
         if(iRes !=M_SEC_ERR_OK)
         {
            WE_FREE(pucMasterSecret);
            pucMasterSecret =NULL;
            WE_FREE(pstP);
            pstP =NULL;
         }
         return;
    }    
    case  E_SEC_TLS_KEYEXCH_DHE_DSS         :   
        {
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_UNSUPPORTED_METHOD, 0, NULL, 0); 
            return;
        }
    case  E_SEC_TLS_KEYEXCH_RSA: 
       {
        if(NULL==pstParam)
        {
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INVALID_PARAMETER, 0, NULL, 0); 
            return ;
        }
        /*get certificate*/
        usCertLen = pstParam->stSecCert.usBufLen;
        pucCert   = pstParam->stSecCert.pucBuf;
        if (usCertLen == 0)  
        {
            iIsKey = 1;
            usModLen = (*pstParam).stRSAParam.stTlsRSAParam.usModLen;
            usExpLen = (*pstParam).stRSAParam.stTlsRSAParam.usExpLen;
            stPubKey.pucExponent = (WE_UCHAR *)WE_MALLOC(usExpLen*sizeof(WE_UCHAR));
            stPubKey.pucModulus = (WE_UCHAR *)WE_MALLOC(usModLen*sizeof(WE_UCHAR));
            if(!stPubKey.pucExponent || !stPubKey.pucModulus)
            {
                if(stPubKey.pucModulus)
                {
                    WE_FREE(stPubKey.pucModulus);
                    stPubKey.pucModulus = NULL;
                }
                if(stPubKey.pucExponent)
                {
                    WE_FREE(stPubKey.pucExponent);
                    stPubKey.pucExponent = NULL;
                }
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0); 
                return ;
            }
            for (iILoop=0; iILoop<usExpLen; iILoop++) 
            {
                stPubKey.pucExponent[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucExponent[iILoop];
            }
            for (iILoop=0; iILoop<usModLen; iILoop++) 
            {
                stPubKey.pucModulus[iILoop] = (*pstParam).stRSAParam.stTlsRSAParam.pucModulus[iILoop];
            }
        }
        else  
        { 
            /*
            if ((*pucCert)== 1)  //wtls cert,and get wtls public key                     
            {
                iRes = Sec_WtlsCertParse(hSecHandle, pucCert, &usCertLen, &stCert);    
                if (iRes != M_SEC_ERR_OK)
                {
                    Sec_WimKeyExchangeResponse(hSecHandle, iTargetID, iRes, 0, NULL, 0);
                    return;
                }
                stPubKey.usExpLen = stCert.stCertUnion.stWtls.usExpLen;
                stPubKey.pucExponent = stCert.stCertUnion.stWtls.pucRsaExponent;
                stPubKey.usModLen = stCert.stCertUnion.stWtls.usModLen;
                stPubKey.pucModulus = stCert.stCertUnion.stWtls.pucRsaModulus;
            }
            else 
            */ /*tang 20070711*/
            if ((*pstParam->stSecCert.pucBuf) == 48) /*x509 cert,and get certificate public key*/           
            {
                iRes = Sec_WimGetX509PubKey(hSecHandle, pucCert , &(stPubKey.usExpLen),\
                             &(stPubKey.pucExponent), &(stPubKey.usModLen), &(stPubKey.pucModulus));
            }
            else                                             
            { 
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE,  0, NULL, 0);
                return;
            }
            if (iRes != M_SEC_ERR_OK)
            {
                Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
                return;
            }
        }
        pucPreMasterSecret = (WE_UCHAR*) WE_MALLOC((WE_UINT32)iPreMasterSecretLen*sizeof(WE_UCHAR));
        if (pucPreMasterSecret == NULL)
        { 
            if(iIsKey)
            {                
                WE_FREE(stPubKey.pucExponent);
                WE_FREE(stPubKey.pucModulus);                
            }
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
            return;
        }
        /*the length of tls random is 48 */
        (WE_VOID)Sec_LibGenerateRandom(hSecHandle,(WE_CHAR*)pucPreMasterSecret,48);
        
        pucPreMasterSecret[0] = M_SEC_TLS_PROTOCOL_VERSION_MAJOR;
        /*the version of tls protocol*/
        if (iIsSsl & G_SEC_TLS_OPTION_SSL_CLIENT_HELLO)
        {
            pucPreMasterSecret[1] = M_SEC_SSL_PROTOCOL_VERSION_MINOR;
        }
        else
        {
            pucPreMasterSecret[1] = M_SEC_TLS_PROTOCOL_VERSION_MINOR;
        }
        
        pucMasterSecret = (WE_UCHAR *)WE_MALLOC (M_SEC_TLS_MASTER_SECRET_LEN);
        if(pucMasterSecret == NULL)
        {
             if(iIsKey)
             {
                 WE_FREE(stPubKey.pucExponent);
                 WE_FREE(stPubKey.pucModulus);                
             }
             WE_FREE(pucPreMasterSecret);
             Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
             return;
        }
        /*the length of server random data is 64*/
        if (iIsSsl)
        {
            iRes = Sec_WimSSLPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                iPreMasterSecretLen, pucRandval, 64, 48, pucMasterSecret); 
        }
        else
        {
            iRes = Sec_WimTLSPRF(hSecHandle,iMasterSecretID, pucPreMasterSecret,\
                iPreMasterSecretLen,   (const char *)"master secret",\
                pucRandval, 64, 48, pucMasterSecret); 
        }
        if (iRes != M_SEC_ERR_OK)
        {
            if(iIsKey)
            {
                WE_FREE(stPubKey.pucExponent);
                WE_FREE(stPubKey.pucModulus);                
            }
            for (iILoop=0; iILoop<M_SEC_TLS_MASTER_SECRET_LEN; iILoop++)
            {
                pucMasterSecret[iILoop] = 0;
            }
            WE_FREE(pucMasterSecret);
            WE_FREE(pucPreMasterSecret);
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
            return;
        }    
        stTmpCrptKey = stPubKey;
        iRes = Sec_PKCRsaPublicKeyEncryption (hSecHandle,iTargetID,stTmpCrptKey, pucPreMasterSecret, iPreMasterSecretLen);       
        for (iILoop=0; iILoop<iPreMasterSecretLen; iILoop++)
        {
            pucPreMasterSecret[iILoop] = 0;
        }
        WE_FREE(pucPreMasterSecret);

        if (pstParam->stSecCert.usBufLen == 0)  
        {
            WE_FREE(stPubKey.pucExponent);
            WE_FREE(stPubKey.pucModulus);
        }
        if (iRes != M_SEC_ERR_OK) 
        {
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, iRes, 0, NULL, 0);
        }    
        pstP = (St_SecWimTlskeyexchangeRsa*)WE_MALLOC(sizeof(St_SecWimTlskeyexchangeRsa));
        if(pstP ==NULL)
        {
            WE_FREE(pucMasterSecret);
            pucMasterSecret =NULL;
            Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 0, NULL, 0);
            return ;
        }
        (void)WE_MEMSET( pstP,0,sizeof(St_SecWimTlskeyexchangeRsa) );
        pstP->iTargetID = iTargetID;
        pstP->pucMasterSecret = pucMasterSecret;        
       
        iRes =Sec_WimAddNewElement(hSecHandle, iTargetID, M_SEC_WIM_TLS_KEYEXCHANGE_RSA, pstP);
        if(iRes !=M_SEC_ERR_OK)
        {
            WE_FREE(pucMasterSecret);
            pucMasterSecret =NULL;
            WE_FREE(pstP);
            pstP =NULL;
        }
        return;
    }
    default : 
        Sec_WimKeyExchangeResponse(hSecHandle,iTargetID, M_SEC_ERR_INVALID_PARAMETER, 0, NULL, 0); 
    }
}
/*=====================================================================================
FUNCTION: 
        Sec_iWimGetMasterSecret
CREATE DATE: 
        2007-01-19
AUTHOR: 
        tang
DESCRIPTION:
        hash updata masterSecret. 
ARGUMENTS PASSED:
        WE_HANDLE hSecHandle[IN]: Global data handle.
        WE_HANDLE hHashHandle[IN/OUT]:pointer to the hash handle.
        WE_INT32 iMasterSecretID[IN]:MasterSecretID
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
WE_VOID Sec_iWimGetMasterSecret (WE_HANDLE hSecHandle,WE_HANDLE hHandle, WE_INT32 iMasterSecretID)
{
    WE_UCHAR aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN]={0};
    WE_INT32 iRes=0;
#ifdef TLS_TEST_PRF
    FILE* fin = NULL;
    int i = 0;
    char tmp[48];
    char *j=NULL;
    char name[14]="get_aucms_";
    int index=0;
#endif
    
    Sec_WimGetMastSecret(hSecHandle,iMasterSecretID, aucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN, &iRes);
    if (iRes != M_SEC_ERR_OK)
    {
        //Sec_SSLGetMasterSecretResp( hSecHandle,  iRes, ,NULL);
        return ;
    }

#ifdef TLS_TEST_PRF
    {
        index=iMasterSecretID+49;
        j=(char *)(&index);
        memcpy(name+10,j,1);
        fin = fopen(name,"w+");
        if(!fin)
        {
            return -1;
        }
        for(i=0;i<M_SEC_TLS_MASTER_SECRET_LEN;i++)
        {
            sprintf(tmp,"0x%x, ",aucMasterSecret[i]);
            fwrite(tmp,strlen(tmp),1,fin);
        }
        fclose(fin);
    }
#endif
       We_LibHashUpdate(hSecHandle,aucMasterSecret,M_SEC_TLS_MASTER_SECRET_LEN,hHandle);
    // Sec_SSLGetMasterSecretResp (hSecHandle,iRes, iTargetID,aucMasterSecret);
}

#endif

/*=====================================================================================
FUNCTION: 
        Sec_iWimCalSign
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        If find user certificate matching to KeyExId, that means the private key can be used compute signature.
        Read out the private key and then compute the signature.
ARGUMENTS PASSED:
        hSecHandle[IN]: Global data handle.
        iTargetID[IN]: ID of the object.
        pucKeyId[IN]: Pointer to the key exchange Id.
        iKeyIdLen[IN]: length of the key exchange id.
        pucBuf[IN]: pointer to the data buffer.
        iBufLen[IN]: The length of the buffer.
        ucAlg[IN]: signature algorithm.
RETURN VALUE:
        none.    
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.    
CALL BY:
        omit.           
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_VOID Sec_iWimCalSign(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                  const WE_UCHAR *pucKeyExId, WE_INT32 iKeyExIdLen,
                                  const WE_UCHAR *pucBuf, WE_INT32 iBufLen, WE_UINT8 ucAlg)
{
    St_SecCrptPrivKey  stCrptPrivKey = {0};
    WE_INT32 iRes = 0 ;
    
    if ((NULL == hSecHandle) || (NULL == pucKeyExId)||(NULL == pucKeyExId))
    {
        return;
    }
 
    stCrptPrivKey.pucBuf = (WE_UCHAR *)pucKeyExId;
    stCrptPrivKey.usBufLen = (WE_UINT16)iKeyExIdLen;  
    /* compute signature.*/
    iRes = Sec_PKCRsaComputeHashedSignature(hSecHandle, iTargetID, stCrptPrivKey, pucBuf, iBufLen);    
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimComputeSignatureResponse(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER, NULL, 0);
        return;
    }      
    iRes = Sec_WimAddNewElement (hSecHandle, iTargetID, M_SEC_WIM_COMPUTE_SIGNATURE, NULL);
    if (iRes != M_SEC_ERR_OK)
    {
        Sec_WimComputeSignatureResponse(hSecHandle, iTargetID, iRes, NULL, 0);
        return;
    }   


}

/*=====================================================================================
FUNCTION: 
        Sec_iWimWtlsCalSignResp
CREATE DATE: 
        2006-7-19
AUTHOR: 
        Stone An
DESCRIPTION:
        response of compute signature.        
ARGUMENTS PASSED:
        hSecHandle[IN/OUT]: Global data handle.
        pstElement[IN] : Global queue.
        pstParam[IN] : The parameter value.  
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
WE_VOID Sec_iWimWtlsCalSignResp(WE_HANDLE hSecHandle, 
                St_SecWimElement *pstElement, St_SecCrptComputeSignatureResp *pstParam)
{
    WE_INT32   iResult = 0;
    
    if ((NULL==hSecHandle) || (NULL==pstElement))
    {
        return;
    }
    if(NULL == pstParam)
    {
            Sec_WimWtlsComputeSignatureResponse(hSecHandle, pstElement->iTargetID, 
                M_SEC_ERR_INSUFFICIENT_MEMORY, NULL, 0);
    }
    else
    {
        iResult = pstParam->sResult;
        if (iResult != M_SEC_ERR_OK)
        {
            Sec_WimWtlsComputeSignatureResponse(hSecHandle, pstElement->iTargetID, 
                iResult, NULL, 0);
        } 
        else 
        {
            Sec_WimWtlsComputeSignatureResponse(hSecHandle, pstElement->iTargetID, M_SEC_ERR_OK, 
                pstParam->pucSig, pstParam->sSigLen);
        }
    }
    WE_FREE(pstElement);
}
#endif



/*************************************************************************************************
end
*************************************************************************************************/


