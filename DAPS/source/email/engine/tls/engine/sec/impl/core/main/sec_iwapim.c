/*=====================================================================================
    FILE NAME :
        Sec_iwapim.c
    MODULE NAME :
        sec
    GENERAL DESCRIPTION
        this file is for running wtls, implementation of main support for wtls.
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    Modification              Tracking
    Date       Author         Number        Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-04 Kevin          none          Init
    2007-05-17     tang                          Delete cert manage and signtext
    2007-06-28    bluefire                       after malloc,memset it
=====================================================================================*/

/**************************************************************************************
*   Include File Section
**************************************************************************************/
#include "sec_comm.h"


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/


#define M_SEC_CFG_TITLE_MUST_EXIST

#define SEC_STATES                              (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iStates)
#define SEC_WANTS_TO_RUN                (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iWantsToRun)
#define SEC_USER_ASKED                      (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iUserReq)
#define SEC_NO_CERT_DIALOG_SHOWN        (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iNocertUeShown)
#define SEC_CUR_UE_RESP                     (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvCurUeResp)
#define SEC_DIALOG_SHOWN                 (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iUeShown)
#define SEC_TLS_SIGN_ALG                (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iSignAlg) 
#define SEC_NO_TITLE_IN_CERT 200


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_ExamineTitleGetWtlsCN( WE_HANDLE hSecHandle, const WE_UCHAR *pucBuf, WE_INT32 iBufLen,
                                    WE_UCHAR **ppucCommonName, WE_INT32 *piCommonNameLen);

WE_INT32 Sec_ExamineTitleOfCertOrRoot(WE_HANDLE hSecHandle, St_SecCertificate stSpCert);
#endif

WE_INT32 Sec_GetX509ComName( WE_HANDLE hSecHandle, const WE_UCHAR *pucBuf, WE_UCHAR **ppucCommonName, 
                             WE_INT32 *piCommonNameLen,
                             WE_UCHAR **ppucDNSName,WE_UINT16 *pusDNSNameLen,
                             WE_UCHAR **ppucIPAddress,WE_UINT16 *pusIPAddressLen);
WE_INT32 Sec_ExamineComName(const WE_UCHAR *pucAddr, WE_INT32 iAddrLen, 
                            const WE_UCHAR *pucDomain,WE_INT32 iDomainLen, 
                            WE_UCHAR *pucCommonName, WE_INT32 iCommonNameLen);
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_GetWtlsComName( WE_HANDLE hSecHandle, const WE_UCHAR *pucBuf, WE_UCHAR **ppucCommonName, 
                             WE_INT32 *piCommonNameLen);
#endif

/*==================================================================================================
FUNCTION: 
    Sec_ExamineTitleOfCertOrRoot
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    examine the title in the root certificate or common certificate.
ARGUMENTS PASSED:
    WE_UCHAR *pucAddr[IN]:Pointer to the address.  
    WE_INT32 iAddrLen[IN]:The length of the address.
    WE_UCHAR *pucDomain[IN]:Pointer to the domain.
    WE_INT32 iDomainLen[IN]:The length of the domain.
    WE_UCHAR *pucCommonName[IN]:Pointer to the common name.
    WE_INT32 iCommonNameLen[IN]:The length of the common name.
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
WE_INT32 Sec_ExamineTitleOfCertOrRoot(WE_HANDLE hSecHandle,St_SecCertificate stSpCert)
{
    WE_UINT8              ucNbrIssuerAtts = 0;
    WE_INT32              iSubcript = 0;
    WE_INT32              iSelfSigned = 1;    
    WE_INT32              iTCaExists = 0;   
    WE_INT32              iRes = 0;
    St_SecNameAttribute * pstIssuerA = NULL;
  
    if (stSpCert.stCertUnion.stWtls.ucIssuerLen == stSpCert.stCertUnion.stWtls.ucSubjectLen)
    {
        for (iSubcript=0; iSubcript<stSpCert.stCertUnion.stWtls.ucIssuerLen; iSubcript++)
        {
            if (stSpCert.pucIssuer[iSubcript] != stSpCert.pucSubject[iSubcript])
            {
                iSelfSigned = 0;
            }
        }
    }
    else
    {
        iSelfSigned = 0;
    }
    
    if (iSelfSigned)
    {
        return M_SEC_ERR_OK;
    }

    iRes = Sec_WtlsGetIssuerInfo(hSecHandle,(const WE_UCHAR *)stSpCert.pucIssuer, 
                                    (WE_UINT16)stSpCert.stCertUnion.stWtls.ucIssuerLen,
                                    &ucNbrIssuerAtts, NULL);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    pstIssuerA = (St_SecNameAttribute *)WE_MALLOC((WE_ULONG)(ucNbrIssuerAtts*(WE_INT32)sizeof(St_SecNameAttribute)));
    if(NULL == pstIssuerA)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET( pstIssuerA,0,(WE_INT32)(ucNbrIssuerAtts*(WE_INT32)sizeof(St_SecNameAttribute)) );
    if(Sec_WtlsGetIssuerInfo(hSecHandle,(const WE_UCHAR *)stSpCert.pucIssuer, 
                                    (WE_UINT16)stSpCert.stCertUnion.stWtls.ucIssuerLen,
                                    &ucNbrIssuerAtts, pstIssuerA) != M_SEC_ERR_OK)
    {
        for (iSubcript=0; iSubcript<ucNbrIssuerAtts; iSubcript++)
        {
            WE_FREE(pstIssuerA[iSubcript].pucAttributeValue);
        }
    
        WE_FREE(pstIssuerA);

        return M_SEC_ERR_INVALID_PARAMETER;
    }
            
    for (iSubcript=0; iSubcript<ucNbrIssuerAtts; iSubcript++)
    {
        if (pstIssuerA[iSubcript].uiAttributeType == (WE_UINT32)M_SEC_TITLE)
        {
            if (!SEC_STRCMP((WE_CHAR *)pstIssuerA[iSubcript].pucAttributeValue, (WE_CHAR *)"T=ca"))
            {
                iTCaExists = 1;
            }
        }
    }
              
    for (iSubcript=0; iSubcript<ucNbrIssuerAtts; iSubcript++)
    {
        WE_FREE(pstIssuerA[iSubcript].pucAttributeValue);
    }
                
    WE_FREE(pstIssuerA);
                  
    if (!iTCaExists)
    {
        return SEC_NO_TITLE_IN_CERT;
    }
    
    return M_SEC_ERR_OK;
}
/*==================================================================================================
FUNCTION: 
    Sec_ExamineTitleGetWtlsCN
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    examint title and common name.
ARGUMENTS PASSED:
    WE_UCHAR *pucAddr[IN]:Pointer to the address.  
    WE_INT32 iAddrLen[IN]:The length of the address.
    WE_UCHAR *pucDomain[IN]:Pointer to the domain.
    WE_INT32 iDomainLen[IN]:The length of the domain.
    WE_UCHAR *pucCommonName[IN]:Pointer to the common name.
    WE_INT32 iCommonNameLen[IN]:The length of the common name.
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
#define SEC_IWAPIM_TMPAUC_LEN 70
WE_INT32 Sec_ExamineTitleGetWtlsCN(WE_HANDLE hSecHandle,const WE_UCHAR *pucBuf, WE_INT32 iBufLen,
                                    WE_UCHAR **ppucCommonName, WE_INT32 *piCommonNameLen)
{    
    WE_UINT16           usCertLen = 0;    
    WE_UINT8            ucIssuerLen = 0;
    WE_UINT8            ucNbrS = 0;
    WE_UCHAR *          pucCert = NULL;
    WE_INT32            iLenLeft = 0;
    WE_INT32            iResult = 0;
    WE_INT32            iSubcript = 0;
    WE_INT32            iStrSize = 0;
    WE_UCHAR            aucTmpStr[SEC_IWAPIM_TMPAUC_LEN] = {0};
    St_SecCertificate   stSpCert = {0};
    St_SecCertificate   stSpCaCert = {0};

    if ((NULL == pucBuf) || (NULL == ppucCommonName) || (NULL == piCommonNameLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
  
    iResult = Sec_WtlsCertParse(hSecHandle,pucBuf+1, &usCertLen, &stSpCert); 
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    ucIssuerLen = stSpCert.stCertUnion.stWtls.ucIssuerLen;
          
    for (iSubcript=0; iSubcript<ucIssuerLen; iSubcript++)
    {
        aucTmpStr[iStrSize] = stSpCert.pucIssuer[iSubcript];

        if(((stSpCert.pucIssuer[iSubcript] == ';') && (stSpCert.pucIssuer[iSubcript+1] == ' ')) || 
        ((iSubcript == (ucIssuerLen-1)) && (ucNbrS == 3)))
        { 
            ucNbrS++;
            if (ucNbrS == 4)
            {  
                if (iStrSize>0) 
                {
                    *ppucCommonName = WE_MALLOC((WE_ULONG)iStrSize);
                    if (NULL == *ppucCommonName)
                    {
                        return M_SEC_ERR_INSUFFICIENT_MEMORY;
                    }
                    (void)WE_MEMCPY(*ppucCommonName,aucTmpStr,(WE_UINT32)iStrSize);
                    *piCommonNameLen = iStrSize;
                }
                else
                {
                    ppucCommonName[0] = NULL;
                }
    
                iSubcript = ucIssuerLen;
            } 
            iStrSize = 0;
            iSubcript += 1;
        }
        else
        {
            iStrSize++;
        }
    }
              
    if(ucNbrS <= 3)
    {
        ppucCommonName[0] = NULL;
    }
                
    iResult = Sec_ExamineTitleOfCertOrRoot(hSecHandle,stSpCert);
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
                  
    pucCert = (WE_UCHAR *)pucBuf;
    iLenLeft = iBufLen - usCertLen - 1;
    while (iLenLeft > 0)
    {
        if ((*pucCert) == 1)
        {
            pucCert += 1 + usCertLen;
        }
        else if ((*pucCert) == 2)
        {
            pucCert += 5 + usCertLen;
        }
        else
        {
            return M_SEC_ERR_INVALID_PARAMETER;
        }

        iResult = Sec_WtlsCertParse(hSecHandle,pucBuf+1, &usCertLen,    &stSpCaCert); 
        if (iResult != M_SEC_ERR_OK)
        {
            return iResult;
        }

        iResult = Sec_ExamineTitleOfCertOrRoot(hSecHandle,stSpCaCert);

        if (iResult != M_SEC_ERR_OK)
        {
            return iResult;
        }
        if ((*pucCert) == 1)
        {
            iLenLeft -= (usCertLen + 1);
        }
        else if ((*pucCert) == 2)
        {
            iLenLeft -= (usCertLen + 5);
        }
    }
    return iResult;
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_ExamineComName
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    examine the common name
ARGUMENTS PASSED:
    WE_UCHAR *pucAddr[IN]:Pointer to the address.  
    WE_INT32 iAddrLen[IN]:The length of the address.
    WE_UCHAR *pucDomain[IN]:Pointer to the domain.
    WE_INT32 iDomainLen[IN]:The length of the domain.
    WE_UCHAR *pucCommonName[IN]:Pointer to the common name.
    WE_INT32 iCommonNameLen[IN]:The length of the common name.
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
#define  SEC_IWAPIM_HOST_LEN 16
WE_INT32 Sec_ExamineComName(const WE_UCHAR *pucAddr, WE_INT32 iAddrLen, 
                            const WE_UCHAR *pucDomain, WE_INT32 iDomainLen, 
                            WE_UCHAR *pucCommonName, WE_INT32 iCommonNameLen)
{
    WE_INT32  iSubcript = 0;
    WE_CHAR * pcTemp = NULL;
    WE_CHAR   acHost[SEC_IWAPIM_HOST_LEN] = {0};
    
    if (pucCommonName == NULL)
    {
        return M_SEC_ERR_OK; 
    }
    
    if ((ct_isdigit (pucCommonName[0])) && (pucAddr != NULL) && (iAddrLen != 0) && (iCommonNameLen < 16)) 
    {
        (void)WE_MEMCPY(acHost, (WE_CHAR*) pucCommonName, (WE_UINT32)iCommonNameLen);  
        for (iSubcript = 0, pcTemp = acHost; iSubcript < 4; iSubcript++) 
        {
            if (SEC_ATOI((const WE_CHAR *)pcTemp) != pucAddr[iSubcript+2]) 
            {
                return M_SEC_ERR_INVALID_COMMON_NAME;
            }
            
            pcTemp = SEC_STRCHR(pcTemp, '.');
            
            if (pcTemp == NULL) 
            {
                if (iSubcript == 3)
                {
                    return M_SEC_ERR_OK;
                }
                else
                {
                    return M_SEC_ERR_INVALID_COMMON_NAME;
                }
            }
            pcTemp++;
        }
    }
    if (pucDomain == NULL) 
    {        
        return M_SEC_ERR_OK;
    }
    if ((iDomainLen !=0) && (iDomainLen == iCommonNameLen) && 
        !WE_MEMCMP (pucDomain, pucCommonName, (WE_UINT32)iCommonNameLen))
    {
        return M_SEC_ERR_OK;
    }
    return M_SEC_ERR_INVALID_COMMON_NAME;
}

/*==================================================================================================
FUNCTION: 
    Sec_GetWtlsComName
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get wtls common name
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle. 
    WE_UCHAR *pucBuf[IN]:Pointer to the data buffer.
    WE_UCHAR **ppucCommonName[OT]:Pointer to the buffer of the common name.
    WE_INT32 *piCommonNameLen[OT]:Pointer to the common name length.
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
WE_INT32 Sec_GetWtlsComName(WE_HANDLE hSecHandle,const WE_UCHAR *pucBuf, WE_UCHAR **ppucCommonName, 
                           WE_INT32 *piCommonNameLen)
{    
    WE_UINT16 usCertLen = 0;
    WE_INT32 iResult = 0;
    WE_INT32 iSubcript = 0;
    WE_INT32 iStrSize = 0;
    WE_UINT8 ucIssuerLen = 0;
    WE_UINT8 ucNbrS = 0;
    WE_UCHAR aucTmpStr[SEC_IWAPIM_TMPAUC_LEN] = {0};
    St_SecCertificate stSpCert = {0};

    if ((NULL == pucBuf) || (NULL == ppucCommonName) || (NULL == piCommonNameLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    iResult = Sec_WtlsCertParse(hSecHandle,pucBuf+1, &usCertLen, &stSpCert); 
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    ucIssuerLen = stSpCert.stCertUnion.stWtls.ucIssuerLen;
          
    for (iSubcript=0; iSubcript<ucIssuerLen; iSubcript++)
    {
        aucTmpStr[iStrSize] = stSpCert.pucIssuer[iSubcript];
        
        if(((stSpCert.pucIssuer[iSubcript] == ';') && (stSpCert.pucIssuer[iSubcript+1] == ' ')) || 
            ((iSubcript == (ucIssuerLen-1)) && (ucNbrS == 3)))
        { 
            ucNbrS++;
            if (ucNbrS == 4)
            {  
                if (iStrSize>0)
                {
                    *ppucCommonName = WE_MALLOC((WE_ULONG)iStrSize);
                    if (NULL == *ppucCommonName)
                    {                        
                        return M_SEC_ERR_INSUFFICIENT_MEMORY;
                    }
                    (void)WE_MEMCPY(*ppucCommonName,aucTmpStr,(WE_UINT32)iStrSize);
                    *piCommonNameLen = iStrSize;
                }
                else
                {
                    ppucCommonName[0] = NULL;
                }
                iSubcript = ucIssuerLen;
            } 
            iStrSize = 0;
            iSubcript += 1;
        }
        else
        {
            iStrSize++;
        }
    }
    if(ucNbrS <= 3)
    {
        ppucCommonName[0] = NULL;
    }
    return iResult;
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_GetX509ComName
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get x509 common name
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.  
    WE_UCHAR *pucBuf[IN]:Pointer to the data.
    WE_UCHAR **ppucCommonName[OT]:Pointer to the buffer of the common name.
    WE_INT32 *piCommonNameLen[OT]:Pointer to the common name length.
    WE_UCHAR **ppucDNSName[OT]:Pointet to the buffer of the DNS name.
    WE_UINT16 *pusDNSNameLen[OT]:Pointer to the DNS name length.
    WE_UCHAR **ppucIPAddress[IO]:Pointer to the buffer of the ip address.
    WE_UINT16 *pusIPAddressLen[OT]:Pointer to the length of the ip address.
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
WE_INT32 Sec_GetX509ComName( WE_HANDLE hSecHandle, const WE_UCHAR *pucBuf, WE_UCHAR **ppucCommonName, 
                           WE_INT32 *piCommonNameLen,
                           WE_UCHAR **ppucDNSName,WE_UINT16 *pusDNSNameLen,
                           WE_UCHAR **ppucIPAddress,WE_UINT16 *pusIPAddressLen)
{
    WE_INT32            iResult = 0;
    WE_INT32            iSubcript = 0;
    WE_UINT16           usRealCertLen = 0;
    WE_UCHAR *          pucInSubject = NULL;
    WE_UINT16           usSubjectLen = 0;
    WE_UINT8            ucNbrSubjectStrings = 0;
    St_SecCertificate   stSpCert = {0};
    St_SecNameAttribute *pstIssuerAtt= NULL;

    if ((NULL == ppucCommonName) || (NULL == piCommonNameLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    ppucDNSName = ppucDNSName;
    pusDNSNameLen = pusDNSNameLen;
    ppucIPAddress = ppucIPAddress;
    pusIPAddressLen = pusIPAddressLen;
    
    iResult = Sec_X509CertParse(hSecHandle,pucBuf, &usRealCertLen, &stSpCert);    
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    pucInSubject = stSpCert.pucSubject;
    usSubjectLen = stSpCert.stCertUnion.stX509.usSubjectLen;
    
    iResult = Sec_X509GetNbrIssuerStr(hSecHandle,pucInSubject, usSubjectLen, &ucNbrSubjectStrings);
    if(iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    
    
    pstIssuerAtt = (St_SecNameAttribute *)WE_MALLOC((WE_ULONG)(ucNbrSubjectStrings*(WE_INT32)sizeof(St_SecNameAttribute)));
    if(NULL == pstIssuerAtt)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }

    (void)WE_MEMSET( pstIssuerAtt,0,(WE_INT32)(ucNbrSubjectStrings*(WE_INT32)sizeof(St_SecNameAttribute)) );
    iResult = Sec_X509GetIssuerInfo(hSecHandle, (const WE_UCHAR *)pucInSubject, 
                                (WE_UINT16)usSubjectLen, &ucNbrSubjectStrings, 
                                pstIssuerAtt);
    for (iSubcript=0; iSubcript<ucNbrSubjectStrings; iSubcript++)
    {
        if ((pstIssuerAtt[iSubcript].uiAttributeType) == (WE_UINT32)M_SEC_COMMON_NAME)
        {
            if (*ppucCommonName != NULL)
            {
                WE_FREE (*ppucCommonName); 
            }
            *ppucCommonName = pstIssuerAtt[iSubcript].pucAttributeValue;
            *piCommonNameLen = pstIssuerAtt[iSubcript].usAttributeValueLen; 
        } 
        else
        {
            WE_FREE(pstIssuerAtt[iSubcript].pucAttributeValue);
        }
    }
    
    WE_FREE(pstIssuerAtt);
    
    return M_SEC_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    Sec_KeyExchangeIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    exchange key in wtls
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    St_SecWtlsKeyExchParams stParam[IN]:The structure of the param.
    WE_UINT8 ucHashAlg[IN]:The value of the hash alg.
    WE_UCHAR * pucRandval[IN]]:Pointer to the randval.
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
void Sec_KeyExchangeIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, 
                        St_SecWtlsKeyExchParams *pstParam,  WE_UINT8 ucHashAlg, 
                        const WE_UCHAR * pucRandval)
{
    SEC_WANTS_TO_RUN = 0;
    Sec_iWimWtlsKeyExchange(hSecHandle,iTargetID, *pstParam,  pucRandval,ucHashAlg);
}
/*==================================================================================================
FUNCTION: 
    Sec_GetPrfResultIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get prf function result.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_UINT8 ucAlg[IN]:Value of the alg.
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
    WE_UCHAR * pucSecret[IN]:Pointer to the secret.
    WE_INT32 iSecretLen[IN]:The length of the secret.
    WE_CHAR * pcLabel[IN]:The pointer to the lable.
    WE_UCHAR * pucSeed[IN]:Pointer to the seed.
    WE_INT32 iSeedLen[IN]:The length of the seed.
    WE_INT32 iOutputLen[IN]:The length of the output data.
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
void Sec_GetPrfResultIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucAlg, WE_INT32 iMasterSecretId,
                        const WE_UCHAR * pucSecret, WE_INT32 iSecretLen, const WE_CHAR * pcLabel,
                        const WE_UCHAR * pucSeed, WE_INT32 iSeedLen, WE_INT32 iOutputLen)
{
      Sec_iWimGetPrfResult(hSecHandle, iTargetID,  iMasterSecretId, 
                                    pucSecret, iSecretLen,  pcLabel, pucSeed, iSeedLen, 
                                    iOutputLen, (WE_UINT8)M_SEC_WTLS_CONNECTION_TYPE,ucAlg);
}
/*==================================================================================================
FUNCTION: 
    Sec_GetCipherSuiteIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get cipher suite supporting wtls
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
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
void Sec_GetCipherSuiteIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_HANDLE hTrustKeyIds)
{
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    St_SecTlConfirmDlgResp *pstUeStConfirmEvtResp = NULL;
#endif    
    if (NULL == hSecHandle)
    {
        return ;
    }
    
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
    switch (SEC_STATES)
    {
    case E_STATE_CIPHER_SUITE_DIRECT_RETURN: 
        return;
    case E_STATE_WTLS_GET_CIPHER_SUITE:
        SEC_USER_ASKED = 0;
#endif
    Sec_iWimWtlsGetCipherSuite(hSecHandle,iTargetID, (WE_UINT8)0, hTrustKeyIds);
        
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        break;
    case E_STATE_UE_CONFIRM:    
        SEC_WANTS_TO_RUN = 0;
        Sec_iUeConfirm(hSecHandle,iTargetID, M_SEC_ID_CONFIRM_1, M_SEC_ID_OK, M_SEC_ID_CANCEL);
        SEC_STATES++;                                                    
        break;
    case E_STATE_UE_CONFIRM_HANDLE:
        if (!SEC_CUR_UE_RESP)
        {
            Sec_WtlsGetCipherSuiteResp(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY, 
                NULL, 0, NULL, 0, NULL, 0);
            Sec_DpHandleCleanUp( hSecHandle );
            return;                         
        }
        pstUeStConfirmEvtResp = (St_SecTlConfirmDlgResp *)SEC_CUR_UE_RESP;
        if ((pstUeStConfirmEvtResp->eType != E_SEC_DP_CONFIRM_DLG) || (!(pstUeStConfirmEvtResp->ucAnswer)))
        {
            if (!(pstUeStConfirmEvtResp->ucAnswer))
            {
                Sec_WtlsGetCipherSuiteResp(hSecHandle, iTargetID, M_SEC_ERR_NO_MATCHING_ROOT_CERTIFICATE, 
                    NULL, 0, NULL, 0, NULL, 0);
            }
            else
            {                
                Sec_WtlsGetCipherSuiteResp(hSecHandle, iTargetID, M_SEC_ERR_GENERAL_ERROR, 
                    NULL, 0, NULL, 0, NULL, 0);
            }
            Sec_DpHandleCleanUp( hSecHandle );
            Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
            SEC_CUR_UE_RESP = NULL;
            return;
        }
        
        Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
        SEC_CUR_UE_RESP = NULL;
        SEC_USER_ASKED = 1;
        Sec_iWimWtlsGetCipherSuite(hSecHandle, iTargetID, (WE_UINT8)1, hTrustKeyIds);
        break;
    default : 
        return;
    }
#endif
}
/*==================================================================================================
FUNCTION: 
    Sec_VerifySvrCertIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    verify the server certificate chain.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_UCHAR * pucBuf[IN]:Pointer to the data.
    WE_INT32 iBufLen[IN]:The length of the buffer.
    WE_UCHAR * pucAddr[IN]:Pointer to the address.
    WE_INT32 iAddrLen[IN]:The length of the addr.
    WE_CHAR * pcLabel[IN]:Pointer to the label.
    WE_CHAR * pucDomain[IN]:Pointer to the domain.
    WE_INT32 iDomainLen[IN]:The length of the domain.
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
void Sec_VerifySvrCertIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, const WE_UCHAR * pucBuf, 
                          WE_INT32 iBufLen,  const WE_UCHAR * pucAddr, WE_INT32 iAddrLen,
                          const WE_CHAR * pucDomain, WE_INT32 iDomainLen)
{
    
    WE_UCHAR *pucCommonName = NULL;
    WE_INT32   iCommonNameLen = 0;
    WE_INT32   iResult = 0;
    WE_UCHAR *pucDNSName = NULL; 
    WE_UINT16 usDNSNameLen = 0;
    WE_UCHAR *pucIPAddress = NULL; 
    WE_UINT16 usIPAddressLen = 0;
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    St_SecTlConfirmDlgResp *pstUeStConfirmEvtResp = NULL;
#endif
    
    if (NULL == hSecHandle)
    {
        return ;
    }
    
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    switch (SEC_STATES)
    {    
    case E_STATE_VERIFY_SVR_CERT_DIRECT_RETURN: 
        return;    
    case E_STATE_WTLS_VERIFY_SVR_CERT_CHAIN:             
        SEC_USER_ASKED =0;
        SEC_DIALOG_SHOWN = 0;        
        if ((iBufLen ==0) && (SEC_NO_CERT_DIALOG_SHOWN == 1)) /* no CA cert and no gateway cert. */
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_DOWNGRADED_TO_ANON);
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }        
        if (iBufLen == 0) /* only no gateway cert. */
        { 
            SEC_WANTS_TO_RUN = 0;
            Sec_iUeConfirm(hSecHandle, iTargetID, M_SEC_ID_CONFIRM_12, M_SEC_ID_OK, M_SEC_ID_CANCEL);
            SEC_STATES = (WE_INT32)E_STATE_HANDLE_UE_CONFIRM;
            break;
        }
#else
    /* downgraded to anonymous if gateway certificate is NULL. */
    if ((0 == iBufLen) || (NULL == pucBuf))
    {
        Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_DOWNGRADED_TO_ANON);
        Sec_DpHandleCleanUp( hSecHandle );
        return;
    }            
#endif
        if ((*pucBuf) == M_SEC_CERTIFICATE_WTLS_TYPE)
        {            
#ifdef M_SEC_CFG_TITLE_MUST_EXIST
            iResult = Sec_ExamineTitleGetWtlsCN(hSecHandle,pucBuf, iBufLen, &pucCommonName, &iCommonNameLen);
#else
            iResult = Sec_GetWtlsComName(hSecHandle, pucBuf, &pucCommonName, &iCommonNameLen);
#endif
        }
        else if ((*pucBuf) == M_SEC_CERTIFICATE_X509_TYPE)
        {
            iResult = Sec_GetX509ComName(hSecHandle, pucBuf+1, &pucCommonName,&iCommonNameLen,
                &pucDNSName, &usDNSNameLen, &pucIPAddress, &usIPAddressLen);
        }
        else 
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_INVALID_PARAMETER);
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        
        if (iResult == SEC_NO_TITLE_IN_CERT)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_BAD_CERTIFICATE);
            if (pucCommonName)
            {
                WE_FREE(pucCommonName);
            }
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        else if (iResult != M_SEC_ERR_OK)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, iResult);
            if (pucCommonName)
            {
                WE_FREE(pucCommonName);
            }
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        
        if (pucCommonName)
        {
            iResult= Sec_ExamineComName(pucAddr, iAddrLen,(const WE_UCHAR *)pucDomain, 
                iDomainLen, pucCommonName, iCommonNameLen);         
            if (iResult != 0)
            {
                Sec_VerifySvrCertResp(hSecHandle, iTargetID,M_SEC_ERR_INVALID_COMMON_NAME);
                WE_FREE(pucCommonName);
                Sec_DpHandleCleanUp( hSecHandle );
                return;
            }
        }
        
        if (pucCommonName)
        {
            WE_FREE(pucCommonName);
        }
        SEC_WANTS_TO_RUN = 0;
        Sec_iWimWtlsVerifySvrCertChain(hSecHandle, iTargetID, pucBuf, iBufLen);
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

        break;
    case E_STATE_HANDLE_UE_CONFIRM :
        if (!SEC_CUR_UE_RESP)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY);
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        pstUeStConfirmEvtResp = (St_SecTlConfirmDlgResp *)SEC_CUR_UE_RESP;        
        if (pstUeStConfirmEvtResp->eType != E_SEC_DP_CONFIRM_DLG)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_GENERAL_ERROR);
            Sec_DpHandleCleanUp( hSecHandle );
            Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
            SEC_CUR_UE_RESP = NULL;
            return;
        }
        
        if (!(pstUeStConfirmEvtResp->ucAnswer))                                    
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_BAD_CERTIFICATE);            
        }
        else
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_DOWNGRADED_TO_ANON);
        }
        SEC_DIALOG_SHOWN = 1;
        Sec_DpHandleCleanUp( hSecHandle );
        Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
        SEC_CUR_UE_RESP = NULL;
        break;
    default :
        return;
    }
#endif 
}
#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
/*==================================================================================================
FUNCTION: 
    Sec_CompSignatureIm
CREATE DATE:
    2007-01-19
AUTHOR:
    tang
DESCRIPTION:
    compute signature implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    const WE_UCHAR * pucPubKeyHash[IN]:public key hash.
    WE_UINT16  usPubKeyHashLen[IN]:length of public key hash.
    const WE_UCHAR *pucData[IN]:pointer to the data to be signature.
    WE_UINT16 usDataLen[IN]:length of data.
    WE_UCHAR  ucAlg[IN]:rsa.
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
void Sec_CompSignatureIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                            const WE_UCHAR * pucPubKeyHash,WE_UINT16  usPubKeyHashLen,
                            const WE_UCHAR *pucData,WE_UINT16 usDataLen,WE_UCHAR  ucAlg)
{

    SEC_WANTS_TO_RUN = 0;
    ucAlg = ucAlg ;
    Sec_iWimCalSign(hSecHandle,iTargetID, pucPubKeyHash, usPubKeyHashLen, pucData, usDataLen, (WE_UINT8)SEC_TLS_SIGN_ALG);
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_CompSignatureIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    compute signature implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_UCHAR * pucKeyId[IN]:Pointer to the key id.
    WE_INT32 iKeyIdLen[IN]:The length of the key id.
    WE_UCHAR * pucBuf[IN]:Pointer to the data buffer.
    WE_INT32 iBufLen[IN]:the length of the data buffer. 
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

#ifdef M_SEC_CFG_TLS
/*==================================================================================================
FUNCTION: 
    Sec_GetTlsCipherSuiteIm
CREATE DATE:
    2007-01-19
AUTHOR:
    tang
DESCRIPTION:
    get cipher suite supporting tls
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
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
void Sec_GetTlsCipherSuiteIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID)
{
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    St_SecTlConfirmDlgResp *pstUeStConfirmEvtResp = NULL;
#endif

    if (NULL == hSecHandle)
    {
        return ;
    }
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
    switch (SEC_STATES)
    {
    case E_STATE_TLS_CIPHER_SUITE_DIRECT_RETURN: /*0*/
        return;        
    case E_STATE_TLS_GET_CIPHER_SUITE:/*1*/
         SEC_USER_ASKED = 0;
#endif
        Sec_iWimTlsGetCipherSuites(hSecHandle,iTargetID);
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        break;
    case E_STATE_TLS_UE_CONFIRM:    
        SEC_WANTS_TO_RUN = 0;
        Sec_iUeConfirm(hSecHandle,iTargetID, M_SEC_ID_CONFIRM_1, M_SEC_ID_OK, M_SEC_ID_CANCEL);
        SEC_STATES++;                                                    
        break;
    case E_STATE_TLS_UE_CONFIRM_HANDLE:
        if (!SEC_CUR_UE_RESP)
        {
            Sec_TlsGetCipherSuiteResp(hSecHandle, iTargetID, 
                M_SEC_ERR_INSUFFICIENT_MEMORY, NULL, 0);
            Sec_DpHandleCleanUp( hSecHandle );
            return;                         
        }
        pstUeStConfirmEvtResp = (St_SecTlConfirmDlgResp *)SEC_CUR_UE_RESP;
        if ((pstUeStConfirmEvtResp->eType != E_SEC_DP_CONFIRM_DLG) || (!(pstUeStConfirmEvtResp->ucAnswer)))
        {
            if (!(pstUeStConfirmEvtResp->ucAnswer))
            {
                Sec_TlsGetCipherSuiteResp(hSecHandle, iTargetID, M_SEC_ERR_NO_MATCHING_ROOT_CERTIFICATE, 
                    NULL, 0);
            }
            else
            {                
                Sec_TlsGetCipherSuiteResp(hSecHandle, iTargetID, M_SEC_ERR_GENERAL_ERROR, 
                    NULL, 0);
            }
            Sec_DpHandleCleanUp( hSecHandle );
            Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
            SEC_CUR_UE_RESP = NULL;
            return;
        }
        
        Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
        SEC_CUR_UE_RESP = NULL;
        SEC_USER_ASKED = 1;
        Sec_iWimTlsGetCipherSuites(hSecHandle,iTargetID);
        break;
    default : 
        return;
    }
#endif
}
/*==================================================================================================
FUNCTION: 
    Sec_VerifyTlsCertChainIm
CREATE DATE:
    2007-01-19
AUTHOR:
    tang
DESCRIPTION:
    verify the server certificate chain.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_INT32 iBufLen[IN]:The length of the buffer.
    WE_UCHAR * pucAddr[IN]:Pointer to the address.
    WE_INT32 iAddrLen[IN]:The length of the addr.
    WE_CHAR * pcLabel[IN]:Pointer to the label.
    WE_CHAR * pucDomain[IN]:Pointer to the domain.
    WE_INT32 iDomainLen[IN]:The length of the domain.
    const St_SecTlsAsn1Certificate *pstCerts[IN]:pointer to the certificate.
    WE_UINT8 ucNumCerts[IN}:number of cert.
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
void Sec_VerifyTlsCertChainIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, const WE_UCHAR * pucAddr, 
                               WE_UINT16 usAddrLen,  const WE_UCHAR * pucDomain, WE_UINT16 usDomainLen,
                               const St_SecTlsAsn1Certificate *pstCerts,WE_UINT8 ucNumCerts)
{
    
    WE_UCHAR              *pucCommonName = NULL;
    WE_INT32              iCommonNameLen = 0;
    WE_INT32              iRes = 0;
    WE_UCHAR              *pucDnsName = NULL; 
    WE_UINT16             usDnsNameLen = 0;
    WE_UCHAR              *pucIpAddress = NULL; 
    WE_UINT16             usIpAddressLen = 0;
    St_SecTlsAsn1Certificate         **ppstWimCertificates = NULL;
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    St_SecTlConfirmDlgResp           *pstUeStConfirmEvtResp = NULL; 
#endif
    if (NULL == hSecHandle||NULL==pstCerts)
    {
        return ;
    }

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        

    switch (SEC_STATES)
    {    
    case   E_STATE_TLS_VERIFY_SVR_CERT_DIRECT_RETURN: /*0*/
        return;
    case   E_STATE_TLS_VERIFY_SVR_CERT_CHAIN_COMNAME: /*1*/
#endif        
        if ((pucAddr != NULL) || (pucDomain != NULL))     
        { 
            if ((((*pstCerts).pucCert) == NULL) || (((*pstCerts).uiCertLen) == 0))
            {
                Sec_VerifySvrCertResp(hSecHandle, iTargetID,M_SEC_ERR_INVALID_PARAMETER);
                Sec_DpHandleCleanUp( hSecHandle );
                return; 
            }
            /*tls only support x509 cert in this Pro*/
            iRes = Sec_GetX509ComName(hSecHandle,(const WE_UCHAR *)(pstCerts[0].pucCert), \
                                     &pucCommonName,&iCommonNameLen,\
                                     &pucDnsName,&usDnsNameLen,\
                                     &pucIpAddress,&usIpAddressLen);
            if (iRes != M_SEC_ERR_OK)
            {
                Sec_VerifySvrCertResp(hSecHandle, iTargetID, iRes);
                if (pucCommonName)
                {
                    WE_FREE(pucCommonName);
                }
                Sec_DpHandleCleanUp( hSecHandle );
                return;
            }
    
            if ((pucCommonName != NULL))
            {
                iRes= Sec_ExamineComName(pucAddr, usAddrLen,  pucDomain, usDomainLen, \
                    pucCommonName,iCommonNameLen);
                WE_FREE(pucCommonName);
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
                if (iRes != 0)
                {                  
                    SEC_WANTS_TO_RUN = 0;                    
                    Sec_iUeConfirm(hSecHandle,iTargetID,M_SEC_ID_CONFIRM_1, M_SEC_ID_OK, M_SEC_ID_CANCEL);
                    SEC_STATES = E_STATE_TLS_HANDLE_UE_CONFIRM ;                                                    
                    return;
                }
                else 
                {
                    SEC_STATES = E_STATE_TLS_VERIFY_SVR_CERT_CHAIN ; 
                }
#else
                if (iRes != 0)
                {
                    Sec_VerifySvrCertResp(hSecHandle, iTargetID,M_SEC_ERR_INVALID_COMMON_NAME);
                    Sec_DpHandleCleanUp( hSecHandle );
                    return;
                }
#endif
            }
            
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
            else
            {
                SEC_STATES = E_STATE_TLS_VERIFY_SVR_CERT_CHAIN ; 
            }
#endif

        } 
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
        else
        {
            SEC_STATES = E_STATE_TLS_VERIFY_SVR_CERT_CHAIN; 
        }
        break;
#else
        if ( 0 == ucNumCerts ) 
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID,  M_SEC_ERR_INSUFFICIENT_MEMORY);
            Sec_DpHandleCleanUp( hSecHandle );
            return;            
        }
#endif
  
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE        
    case   E_STATE_TLS_VERIFY_SVR_CERT_CHAIN: /*3*/
#endif
        SEC_WANTS_TO_RUN = 0;
        ppstWimCertificates = ( St_SecTlsAsn1Certificate **)&pstCerts; 
        Sec_iWimVerifyTLSCertificateChain(hSecHandle,iTargetID,  *ppstWimCertificates, ucNumCerts);
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE      
        break;
    case   E_STATE_TLS_HANDLE_UE_CONFIRM:/*7*/
        if (!SEC_CUR_UE_RESP)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_INSUFFICIENT_MEMORY);
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        pstUeStConfirmEvtResp = ( St_SecTlConfirmDlgResp *)SEC_CUR_UE_RESP;
        if(pstUeStConfirmEvtResp->eType != E_SEC_DP_CONFIRM_DLG)
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID,  M_SEC_ERR_GENERAL_ERROR);
            Sec_DpHandleCleanUp( hSecHandle );
            return;
        }
        if (!(pstUeStConfirmEvtResp->ucAnswer))                                    
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_BAD_CERTIFICATE);
        }
        else
        {
            Sec_VerifySvrCertResp(hSecHandle, iTargetID, M_SEC_ERR_OK);
        }
        SEC_DIALOG_SHOWN = 1;
        Sec_DpHandleCleanUp( hSecHandle );
        Sec_TlHandleUeStructFree(SEC_CUR_UE_RESP);
        SEC_CUR_UE_RESP = NULL;
        break;
    default :  
        return;
        }
    #endif
} 

/*==================================================================================================
FUNCTION: 
    Sec_TlsKeyExchangeIm
CREATE DATE:
    2007-01-19
AUTHOR:
    tang
DESCRIPTION:
    exchange key in tls
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_UINT8 ucKeyExchangeAlgorithm[IN]:the algorithm of key exchange.
    WE_INT32  iOptions[IN]:the option of connection to decide use tls prf or ssl prf.
    St_SecTlsKeyExchangeParams *pstParam[IN]:pointer to the St_SecTlsKeyExchangeParams.
    const WE_UCHAR *pucRandval[IN]:random number.
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
WE_VOID Sec_TlsKeyExchangeIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                              WE_UINT8 ucKeyExchangeAlgorithm, WE_INT32  iOptions,
                              St_SecTlsKeyExchangeParams *pstParam,  
                              const WE_UCHAR *pucRandval)
{
        SEC_WANTS_TO_RUN = 0;
        Sec_iWimTLSkeyExchange(hSecHandle,iTargetID,ucKeyExchangeAlgorithm,iOptions,pstParam, pucRandval);
}
/*==================================================================================================
FUNCTION: 
    Sec_GetTlsPrfResultIm
CREATE DATE:
    2007-01-19
AUTHOR:
    tang
DESCRIPTION:
    get prf function result.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the object.
    WE_UINT8 ucAlg[IN]:Value of the alg.
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
    WE_UCHAR * pucSecret[IN]:Pointer to the secret.
    WE_INT32 iSecretLen[IN]:The length of the secret.
    WE_CHAR * pcLabel[IN]:The pointer to the lable.
    WE_UCHAR * pucSeed[IN]:Pointer to the seed.
    WE_INT32 iSeedLen[IN]:The length of the seed.
    WE_INT32 iOutputLen[IN]:The length of the output data.
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
void Sec_GetTlsPrfResultIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucAlg, WE_INT32 iMasterSecretId,
                        const WE_UCHAR * pucSecret, WE_INT32 iSecretLen, const WE_CHAR * pcLabel,
                        const WE_UCHAR * pucSeed, WE_INT32 iSeedLen, WE_INT32 iOutputLen)
{
    Sec_iWimGetPrfResult(hSecHandle,iTargetID, iMasterSecretId, pucSecret, iSecretLen, 
        pcLabel,pucSeed, iSeedLen, iOutputLen, M_SEC_TLS_CONNECTION_TYPE, ucAlg);
}

#endif

/*==================================================================================================
FUNCTION: 
    Sec_SetupConIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    set up session connection implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    St_SecSetupConnection * pstPara[IN]:Pointer to the pata.
    
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
void Sec_SetupConIm (WE_HANDLE hSecHandle, St_SecSetupConnection * pstPara)
{
    WE_INT32 iRes = 0;
    
    if ((NULL == hSecHandle) || (NULL == pstPara))
    {
        return ;
    }
    
    if (1 == SEC_STATES) 
    {
        if (Sec_WimAddInfo( hSecHandle, 
                            (WE_INT32)pstPara->ucMasterSecretId, 
                            pstPara->iSecurity_id, 
                            (WE_INT32)pstPara->ucFull_handshake, 
                            &pstPara->stSessionInfo) != M_SEC_ERR_OK)
        {
            iRes = Sec_WimStopInfo(hSecHandle);
            if (iRes != M_SEC_ERR_OK)
            {
                return;
            }
        }
        Sec_DpHandleCleanUp( hSecHandle );
    }
    else
    {
        return;
    }
}

/*==================================================================================================
FUNCTION: 
    Sec_StopCon
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    stop the session connection implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.  
    WE_INT32 iTargetID[IN]:ID of the oject.
    WE_INT32 iSecurityId[IN]:ID of the security.
    WE_UINT8 ucConnectionType[IN]: The value of the connection type.
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
void Sec_StopCon (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId, WE_UINT8 ucConnectionType)
{
    if (NULL == hSecHandle)
    {
        return ;
    }
    
    if (1 == SEC_STATES)
    {
        WE_UNUSED_PARAMETER(iTargetID);
        WE_UNUSED_PARAMETER(iSecurityId);
        WE_UNUSED_PARAMETER(ucConnectionType);
        Sec_DpHandleCleanUp( hSecHandle );
    }
    else
    {
        return;
    }
}

/*==================================================================================================
FUNCTION: 
    Sec_RemovePeer
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    remove peer from one session based on mastersecret id.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.  
    WE_INT32 iMasterSecretId[IN]:ID of the master secret
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
void Sec_RemovePeer (WE_HANDLE hSecHandle, WE_INT32 iTargetID,WE_INT32 iMasterSecretId)
{
    Sec_WimPeerRemove(hSecHandle, iTargetID, iMasterSecretId);    
}
/*==================================================================================================
FUNCTION: 
    Sec_AttachPeerToSessionIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    attachment one peer to session implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.  
    WE_UINT8 ucConnectionType[IN]:The value of the connection type.
    WE_UCHAR * pucAddress[IN]:Pointer to the address.
    WE_INT32 iAddressLen[IN]:The length of the address.
    WE_UINT16 usPortnum[IN]:The value of the port number.
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
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
void Sec_AttachPeerToSessionIm (WE_HANDLE hSecHandle, WE_UINT8 ucConnectionType,
                               const WE_UCHAR * pucAddress, WE_INT32 iAddressLen,
                               WE_UINT16 usPortnum,  WE_INT32 iMasterSecretId)
{
    Sec_WimPeerAttachSession(hSecHandle, ucConnectionType, pucAddress, 
                                    iAddressLen, usPortnum, iMasterSecretId); 
}
/*==================================================================================================
FUNCTION: 
    Sec_EnableSessionIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    enable one session implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
    WE_UINT8 ucIsActive[IN]:The flg of the status.
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
void Sec_EnableSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucIsActive)
{
    Sec_WimSessionEnable(hSecHandle, iMasterSecretId, ucIsActive);
}
/*==================================================================================================
FUNCTION: 
    Sec_DisableSessionIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    disable one session implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iMasterSecretId[IN]:ID of the master sceret.
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
void Sec_DisableSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId)
{
    Sec_WimSessionDisable(hSecHandle, iMasterSecretId);
}
/*==================================================================================================
FUNCTION: 
    Sec_UpdateSessionIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    update session implementation
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IN]:Global data handle.  
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
    WE_UINT8 ucSessionOptions[IN]:The value of the session option.
    WE_UCHAR * pucSessionId[IN]:ID of the session.
    WE_UINT8 ucSessionIdLen[IN]:The length of the session id.
    WE_UINT8 aucCipherSuite[IN]:Array to cipher suite.
    WE_UINT8 ucCompressionAlg[IN]:The value of compression alg.
    WE_UCHAR * pucPrivateKeyId[IN]:ID of the private key.
    WE_UINT32 uiCreationTime[IN]:the value of the creation time.
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
void Sec_UpdateSessionIm (WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucSessionOptions,
                          WE_UCHAR * pucSessionId, WE_UINT8 ucSessionIdLen,
                          WE_UINT8 aucCipherSuite[2], WE_UINT8 ucCompressionAlg,
                          WE_UCHAR * pucPrivateKeyId, WE_UINT32 uiCreationTime)
{
    Sec_WimUpdateSession(hSecHandle, iMasterSecretId, ucSessionOptions,
                        pucSessionId, ucSessionIdLen,
                        aucCipherSuite, ucCompressionAlg,
                        pucPrivateKeyId, uiCreationTime);    
}
/*==================================================================================================
FUNCTION: 
    Sec_SearchPeerIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    search peer implementation.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.  
    WE_INT32 iId[IN]: The value of the ID.
    WE_UINT8 ucConnectionType[IN]:The value of the connection type.
    WE_UCHAR * pucAddress[IN]:Pointer to the address.
    WE_INT32 iAddressLen[IN]:The length of the address.
    WE_UINT16 usPortnum[IN]: The value of the port number.
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
void Sec_SearchPeerIm (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_UINT8 ucConnectionType,
                        const WE_UCHAR * pucAddress, WE_INT32 iAddressLen, WE_UINT16 usPortnum)
{
    Sec_WimPeerSearch(hSecHandle, iTargetID, ucConnectionType, pucAddress, 
                            iAddressLen, usPortnum);
}
/*==================================================================================================
FUNCTION: 
    Sec_FetchSessionInfoIm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    fetch sessin information.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]:Global data handle.
    WE_INT32 iId[IN]:The value of the ID.
    WE_INT32 iMasterSecretId[IN]:ID of the master secret.
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_FetchSessionInfoIm (WE_HANDLE hSecHandle, WE_INT32 iId, WE_INT32 iMasterSecretId)
{
    Sec_WimFetchSession(hSecHandle, iId, iMasterSecretId);
}
