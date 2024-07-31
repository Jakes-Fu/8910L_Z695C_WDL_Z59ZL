/*=====================================================================================
    FILE NAME : sec_cm.c
    MODULE NAME : SEC

    GENERAL DESCRIPTION
        The functions in this file operate the certificates in x509 or wtls format.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                                  Tracking
    Date                     Author             Number                  Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-07-08              wuxl                                          create sec_cm.c
    2007-05-17              tang                                 Delete cert manage and signtext
=====================================================================================*/

/*=====================================================================================
*   Include File Section
*=====================================================================================*/
#include "sec_comm.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

/*Define Constant Macro start*/
#define MD2_WITH_RSA_ENCRYP        2
#define MD5_WITH_RSA_ENCRYP        4
#define SHA1_WITH_RSA_ENCRYP       5

/*******************************************************************************
*   Function Define Section
*******************************************************************************/

/*==================================================================================================
FUNCTION: 
    Sec_CertGetItemsForUe
CREATE DATE:
    2006-07-08
AUTHOR:
    Jabber Wu
    modify by bird 061118
DESCRIPTION:
    Get the information from the input DER encoded certificate, and return it to Ue.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]: Global data handle.
    WE_UCHAR *pucCert    [IN]: Pointer to the DER encoded certificate in buffer.
    WE_INT16 *psIssuercharSet  [OUT]: Place to issuer character set.
    WE_VOID **ppvInIssuer      [OUT]: Place to put pointer to issuer value.
    WE_INT32 *piInIssuerLen    [OUT]: Place to length of issuer.
    WE_INT16 *psSubjectcharSet [OUT]: Place to subject character set.
    WE_VOID **ppvInSubject     [OUT]: Place to put pointer to subject value.
    WE_INT32 *piInSubjectLen   [OUT]: Place to length of subject.
    WE_UINT32 *puiValidnotBefore[OUT]: Place to the beginning of the validity period of the certificate.
    WE_UINT32 *puiValidnotAfter[OUT]: Place to the end of the validity period of the certificate.
    WE_UCHAR *ppucSerialNumber   [OUT]: Place to serial number.
    WE_INT32 *pusSerialNumberLen [OUT]: Place to length of serial number.
    WE_UCHAR **ppucAlg[OUT]:Place to algorithm
    WE_UCHAR **ppucPubKey[OUT]:place to public key
    WE_UINT16* pusPubKeyLen[OUT]:place to publick key length
RETURN VALUE:
    M_SEC_ERR_OK: If task is successful.
    M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE: Certificate not surpport.
    M_SEC_ERR_INVALID_PARAMETER: Invalid parameter.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_CertGetItemsForUe(WE_HANDLE hSecHandle, WE_UCHAR *pucCert, WE_INT16 *psIssuercharSet, 
                            WE_VOID **ppvInIssuer, WE_INT32 *piInIssuerLen, 
                            WE_INT16 *psSubjectcharSet, 
                            WE_VOID **ppvInSubject, WE_INT32 *piInSubjectLen, 
                            WE_UINT32 *puiValidnotBefore, WE_UINT32 *puiValidnotAfter, 
                            WE_UCHAR **ppucSerialNumber,WE_UINT16* pusSerialNumberLen,
                            WE_UCHAR **ppucAlg, WE_UCHAR **ppucPubKey, 
                            WE_UINT16* pusPubKeyLen)
{
    WE_UCHAR  ucAlgId         = 0;
    WE_UCHAR  *pucPtr          = NULL;
    WE_UINT16 usCertLen       = 0;
    WE_INT32  iRes            = M_SEC_ERR_OK;
    St_SecCertificate stCert = {0};

    if (!pucCert || !psIssuercharSet || !ppvInIssuer || !piInIssuerLen || !psSubjectcharSet 
        || !ppvInSubject || !piInSubjectLen || !puiValidnotBefore || !puiValidnotAfter) 
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }

    pucPtr = pucCert;
    switch (*pucPtr)
    {
#ifdef M_SEC_CFG_WTLS
        case M_SEC_CERTIFICATE_WTLS_TYPE:
            iRes = Sec_WtlsCertParse(hSecHandle, pucPtr + 1, &usCertLen, &stCert);
            if (M_SEC_ERR_OK != iRes)
            {
                return iRes;
            }
            /*add by bird 061107*/
            if((NULL != ppucSerialNumber)&&(NULL != pusSerialNumberLen)&&
                (NULL != ppucAlg)&&
                (NULL != ppucPubKey)&&(NULL != pusPubKeyLen))
            {
                WE_UCHAR aucAlg[3][16] = 
                {
                    "anonymous",/*0*/
                    "ecdsa_sha", /*1*/
                    "rsa_sha"      /*2*/
                };
                St_SecCrptPubKeyRsa stPubKey = {0};
                /*get serial number*/
                *ppucSerialNumber = NULL;
                *pusSerialNumberLen = 0;

                /*get signature algorithm*/
                *ppucAlg = (WE_UCHAR*)WE_SCL_STRNDUP((WE_CHAR*)aucAlg[stCert.stCertUnion.stWtls.ucSignatureAlgorithm],\
                    (WE_INT32)SEC_STRLEN((WE_CHAR*)aucAlg[stCert.stCertUnion.stWtls.ucSignatureAlgorithm]));
                if(NULL == *ppucAlg)
                {
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }                
                /* *pusAlgLen = stCert.stCertUnion.stX509.usSignatureAlgIdLen;*/

                /*get public key RSA*/
                stPubKey.pucExponent = stCert.stCertUnion.stWtls.pucRsaExponent;
                stPubKey.pucModulus = stCert.stCertUnion.stWtls.pucRsaModulus;
                stPubKey.usExpLen = stCert.stCertUnion.stWtls.usExpLen;
                stPubKey.usModLen = stCert.stCertUnion.stWtls.usModLen;
                
                iRes = Sec_WimConvPubKey(stPubKey,ppucPubKey,pusPubKeyLen);  
                if((NULL == *ppucPubKey) || (M_SEC_ERR_OK != iRes))
                {
                    WE_FREE(*ppucAlg);
                    *ppucAlg = NULL;
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
            }
            
            *piInIssuerLen     = stCert.stCertUnion.stWtls.ucIssuerLen;
            *piInSubjectLen    = stCert.stCertUnion.stWtls.ucSubjectLen;
            *psIssuercharSet   = (WE_INT16)stCert.stCertUnion.stWtls.usIssuercharacterSet;
            *psSubjectcharSet  = (WE_INT16)stCert.stCertUnion.stWtls.usSubjectcharacterSet;
            
            break;
#endif
        case M_SEC_CERTIFICATE_X509_TYPE:
            iRes = Sec_X509CertParse(hSecHandle, pucPtr + 1, &usCertLen, &stCert);
            if (M_SEC_ERR_OK != iRes)
            {
                return iRes;
            }
            /*add by bird 061107*/
            if((NULL != ppucSerialNumber) && (NULL != pusSerialNumberLen)&&
                (NULL != ppucAlg)&&
                (NULL != ppucPubKey)&&(NULL != pusPubKeyLen))
            {
                WE_CHAR *pcAlg = NULL;  
                /*get serial number*/
                *ppucSerialNumber = (WE_UCHAR*)WE_MALLOC(stCert.stCertUnion.stX509.usSerialNumberLen);
                if(NULL == *ppucSerialNumber)
                {
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
                (void)WE_MEMCPY(*ppucSerialNumber,stCert.stCertUnion.stX509.pucSerialNumber,
                            stCert.stCertUnion.stX509.usSerialNumberLen);
                *pusSerialNumberLen = stCert.stCertUnion.stX509.usSerialNumberLen;

                /*get signature x509 algorithm???*/
                
                ucAlgId = stCert.stCertUnion.stX509.pucSignatureAlgId[stCert.stCertUnion.stX509.usSignatureAlgIdLen-1];
                switch (ucAlgId)
                {
                    case SHA1_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"sha1WithRSAEncrypt";
                        break;
                    case MD5_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"md5WithRSAEncrypt";
                        break;
                    case MD2_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"md2WithRSAEncrypt";
                        break;
                    default:
                        /* add by sam [070308] */
                        WE_FREE(*ppucSerialNumber);
                        *ppucSerialNumber = NULL;
                        return M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE;
                }
                *ppucAlg  =  (WE_UCHAR*)WE_SCL_STRNDUP(pcAlg,(WE_INT32)SEC_STRLEN(pcAlg));
                if(NULL == *ppucAlg)
                {
                    WE_FREE(*ppucSerialNumber);
                    *ppucSerialNumber = NULL;
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }


                /*get public key RSA*/
                *ppucPubKey = (WE_UCHAR*)WE_MALLOC(stCert.stCertUnion.stX509.usPublicKeyValLen);
                if(NULL == *ppucPubKey)
                {
                    WE_FREE(*ppucSerialNumber);
                    WE_FREE(*ppucAlg);
                    *ppucSerialNumber = NULL;
                    *ppucAlg = NULL;
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
                (void)WE_MEMCPY(*ppucPubKey,stCert.stCertUnion.stX509.pucPublicKeyVal,
                    stCert.stCertUnion.stX509.usPublicKeyValLen);
                *pusPubKeyLen = stCert.stCertUnion.stX509.usPublicKeyValLen;
            }
            
            *psIssuercharSet   = -1;
            *piInIssuerLen     = stCert.stCertUnion.stX509.usIssuerLen;
            *psSubjectcharSet  = -1;
            *piInSubjectLen    = stCert.stCertUnion.stX509.usSubjectLen;
            
            break;
        case WE_ASN1_SEQUENCE_TYPE:
            iRes = Sec_X509CertParse(hSecHandle, pucPtr, &usCertLen, &stCert);
            if (M_SEC_ERR_OK != iRes)
            {
                return iRes;
            }
            /*add by bird 061107*/
            if((NULL != ppucSerialNumber) && (NULL != pusSerialNumberLen)&&
                (NULL != ppucAlg)&&
                (NULL != ppucPubKey)&&(NULL != pusPubKeyLen))
            {
                WE_CHAR *pcAlg = NULL;  
                /*get serial number*/
                *ppucSerialNumber = (WE_UCHAR*)WE_MALLOC(stCert.stCertUnion.stX509.usSerialNumberLen);
                if(NULL == *ppucSerialNumber)
                {
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
                (void)WE_MEMCPY(*ppucSerialNumber,stCert.stCertUnion.stX509.pucSerialNumber,
                            stCert.stCertUnion.stX509.usSerialNumberLen);
                *pusSerialNumberLen = stCert.stCertUnion.stX509.usSerialNumberLen;

                /*get signature x509 algorithm???*/
                
                ucAlgId = stCert.stCertUnion.stX509.pucSignatureAlgId[stCert.stCertUnion.stX509.usSignatureAlgIdLen-1];
                switch (ucAlgId)
                {
                    case SHA1_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"sha1WithRSAEncrypt";
                        break;
                    case MD5_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"md5WithRSAEncrypt";
                        break;
                    case MD2_WITH_RSA_ENCRYP:
                        pcAlg = (WE_CHAR*)"md2WithRSAEncrypt";
                        break;
                    default:
                        /* add by sam [070308] */
                        WE_FREE(*ppucSerialNumber);
                        *ppucSerialNumber = NULL;
                        return M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE;
                }
                *ppucAlg  =  (WE_UCHAR*)WE_SCL_STRNDUP(pcAlg,(WE_LONG)SEC_STRLEN(pcAlg));
                if(NULL == *ppucAlg)
                {
                    WE_FREE(*ppucSerialNumber);
                    *ppucSerialNumber = NULL;
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }


                /*get public key RSA*/
                *ppucPubKey = (WE_UCHAR*)WE_MALLOC(stCert.stCertUnion.stX509.usPublicKeyValLen);
                if(NULL == *ppucPubKey)
                {
                    WE_FREE(*ppucSerialNumber);
                    WE_FREE(*ppucAlg);
                    *ppucSerialNumber = NULL;
                    *ppucAlg = NULL;
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
                (void)WE_MEMCPY(*ppucPubKey,stCert.stCertUnion.stX509.pucPublicKeyVal,
                    stCert.stCertUnion.stX509.usPublicKeyValLen);
                *pusPubKeyLen = stCert.stCertUnion.stX509.usPublicKeyValLen;

            }
            
            *psIssuercharSet   = -1;
            *piInIssuerLen     = stCert.stCertUnion.stX509.usIssuerLen;
            *psSubjectcharSet  = -1;
            *piInSubjectLen    = stCert.stCertUnion.stX509.usSubjectLen;
            
            break;
        default:
            return M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE;
    }
    
    *puiValidnotBefore = stCert.iValidNotBefore;
    *puiValidnotAfter  = stCert.iValidNotAfter;
    *ppvInIssuer       = stCert.pucIssuer;
    *ppvInSubject      = stCert.pucSubject;    
  
     return iRes;
}


