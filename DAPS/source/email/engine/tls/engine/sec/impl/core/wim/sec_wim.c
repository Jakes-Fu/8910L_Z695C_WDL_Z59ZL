/*==================================================================================================
    FILE NAME : sec_wim.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        The functions in this file carry out the operations as WIM.
  
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================================
    Revision History
    
    Modification                            Tracking
    Date                 Author            Number                         Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-12      Tang                                                      create sec_wim.c

==================================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include "sec_comm.h"
#include "oem_sechandle.h"
#ifdef TLS_TEST_PRF
#include "stdio.h"
#include "string.h"
#endif

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
#define SEC_CREATEFILE_ERROR_NUM                10

/*padded value*/
#define SEC_FIRST_PAD_VALUE                     1
#define SEC_SECOND_PAD_VALUE                    0
#define SEC_PIN_PADDING_CHARACTER               0

#define SEC_HMAC_B                              64
#define SEC_HMAC_MD5_L                          16
#define SEC_HMAC_SHA1_L                         20


typedef St_PublicData   St_SecGlobalData;

#define M_SEC_WIM_ASTUSERCERTKEYPAIR      (((ISec*)hSecHandle)->pstUserCertKeyPair)
#define M_SEC_WIM_HAVELOAD                   (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->pcHaveLoad)
#define M_SEC_WIM_HWEHANDLE               (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->hWeHandle)
#define M_SEC_WIM_CACERT                        (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->hCACert)

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
#ifndef M_SEC_CFG_NO_PLATFORM 
WE_INT32 Sec_WimGenCipherKey(WE_UINT8 *pucKey, WE_INT32 iKeyLen ,WE_UINT8 *pucIv ,WE_INT32 iIvLen );
WE_INT32 Sec_WimEncryptData(WE_HANDLE hSecHandle,
                            WE_UINT8 *pucData, 
                            WE_INT32 iDataLen, 
                            WE_UINT8 *pucEncryptedData,
                            WE_INT32 *piEncryptedDataLen);
WE_INT32 Sec_WimDecryptData(WE_HANDLE hSecHandle,
                            WE_UINT8 *pucData, 
                            WE_INT32 iDataLen,
                            WE_UINT8 *pucDecryptedData,
                            WE_INT32 *piDecDataLen);
#endif

#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
void Sec_WimPMD5(WE_HANDLE hSecHandle,WE_UCHAR *pucSecret, WE_INT32 iSecretLen, 
                  WE_UCHAR *pucSeed, WE_INT32 iSeedLen, WE_INT32 *iResult, 
                  WE_UCHAR *pucBuf, WE_INT32 iDesiredLen);
void Sec_WimHMAC(WE_HANDLE hSecHandle,WE_UINT8 *pucK, 
                 WE_INT32 iKLen, WE_UINT8 *pcData, 
                 WE_INT32 iDataLen, WE_INT32 *piResult,WE_UINT8 *pucBuf);
void Sec_WimPHash(WE_HANDLE hSecHandle,WE_UINT8 *pucSecret, WE_INT32 iSecretLen, 
                  WE_UINT8 *pucSeed, WE_INT32 iSeedLen, WE_INT32 *piResult, 
                  WE_UINT8 *pucBuf, WE_INT32 iDesiredLen);
#endif


/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/

/*====================================================================================
FUNCTION:
    Sec_WimCheckFileExist    
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    tests whether the specified file is exist.      
ARGUMENTS PASSED:  
    hSecHandle[IN/OUT]: Global data handle.
    pcFileName[IN]:     filename.       
RETURN VALUE:
    If file is exist, return Success.Else return EFAILED.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#ifndef M_SEC_CFG_NO_PLATFORM 
WE_INT32 Sec_WimCheckFileExist(WE_HANDLE hSecHandle,WE_CHAR *pcFileName)
{    
    return (WE_INT32)WE_FILE_CHECK_EXIST(M_SEC_WIM_HWEHANDLE,pcFileName);
}
#endif
/*====================================================================================
FUNCTION: 
    Sec_WimChkWTLSCert  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Verify WTLS Certificate .
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    iWid[IN]: The value of the wid.
    stKey[IN]:      stkey        
    pucCert[IN]:  The Certificate which wanted to be verified.             
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimChkWTLSCert (WE_HANDLE hSecHandle,WE_INT32 iTargetID, St_SecPubKeyRsa stKey, 
                                const WE_UINT8 *pucCert)
{
   WE_UINT8    *pucWtlSCert=NULL;
   WE_UINT16   usCertLen=0;
   WE_INT32    iResult=0;
   WE_UINT32   uiNow=0;
   St_SecCertificate stCert={0};
   St_SecCrptPubKey stPubKey={0};
   if (!pucCert)
   {
       return M_SEC_ERR_INVALID_PARAMETER;
   }
   pucWtlSCert = (WE_UINT8*)pucCert;
   iResult = Sec_WtlsCertParse(hSecHandle,pucWtlSCert, &usCertLen, &stCert);
   if (iResult != M_SEC_ERR_OK)
   {
       return iResult;
   }
   uiNow = SEC_CURRENT_TIME();
   if ((uiNow < stCert.iValidNotBefore) || (stCert.iValidNotAfter < uiNow)) 
   {
       return M_SEC_ERR_CERTIFICATE_EXPIRED;
   }
   stPubKey = stKey;
   return Sec_WimVerifyHashedSign (hSecHandle,(WE_INT32)E_SEC_ALG_HASH_SHA1, iTargetID, stPubKey, pucWtlSCert, \
                                  (usCertLen - (stCert.usSignatureLen) - 2),\
                                  stCert.pucSignature, stCert.usSignatureLen);
}
#endif
/*====================================================================================
FUNCTION: 
    Sec_WimChkX509Cert  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Verify X509 Certificate .
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    iTargetID[IN]:   The value of the iTargetID.            
    pucPublicKeyVal[IN]:   public key. 
    pucCert[IN]:           certificate.
    iConnectionType[IN]:   Connection type.
    iChainDepth[IN]:       chain depth.
    iCertUsage[IN]:        the usage of cert.   
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimChkX509Cert (WE_HANDLE hSecHandle,WE_INT32 iTargetID, WE_UINT8 *pucPublicKeyVal,
                             const WE_UINT8 *pucCert, WE_INT32 iConnectionType,
                             WE_INT32 iChainDepth, WE_INT32 iCertUsage)
{
    WE_UINT8        *pucX509cert=NULL;
    WE_UINT16       usCertLen=0;
    WE_INT32        iResult = M_SEC_ERR_OK;
    WE_UINT32       uiNow=0;
    St_SecCertificate stSpCert={0};
    St_SecPubKeyRsa  stKey={0};
    WE_UINT16       usTBSCertLen = 0;
    WE_UINT16       usJumpStep = 0; 
    WE_INT32        iTmpCertUsage=0;
    WE_INT32        iExtnId = 0;
    WE_INT32        iCritical = 0;
    WE_UINT8        *pucExtnValue = 0;
    WE_UINT16       usExtnValueLen = 0;
    WE_UINT8        ucKeyUsageBit0To7 = 0; 
    WE_UINT8        ucKeyUsageBit8To15 = 0;
    WE_INT32        iAnyExtendedKeyUsage = 0;
    WE_INT32        iServerAuth = 0;
    WE_INT32        iClientAuth = 0;
    WE_INT32        iCodeSigning = 0;
    WE_INT32        iEmailProtection = 0;
    WE_INT32        iTimeStamping = 0;
    WE_INT32        iOCSPSigning = 0; 
    WE_INT32        iCA=0;
    WE_INT32        iHasPathLenConstraint=0;
    WE_UINT8        ucPathLenConstraint=0; 
    WE_INT32        iBasicConstraintCa = 0;
    St_SecCrptPubKey stPubKey={0};
    WE_INT32        iHashAlg=0;
    
    iConnectionType=iConnectionType; 
    if (pucCert == NULL)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    pucX509cert = (WE_UINT8*)pucCert;
    usCertLen = 0;
    
    iResult = Sec_X509CertParse(hSecHandle,pucX509cert, &usCertLen, &stSpCert); 
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    uiNow = SEC_CURRENT_TIME();
    if ((uiNow < stSpCert.iValidNotBefore) || (stSpCert.iValidNotAfter < uiNow)) 
    {
        return M_SEC_ERR_CERTIFICATE_EXPIRED;
    }
    usTBSCertLen = (WE_UINT16)stSpCert.stCertUnion.stX509.uiTbsCertLen;
    
    if (stSpCert.ucCertificateVersion == 3) 
    { 
        if (iChainDepth>1)
        {         
            iTmpCertUsage = M_SEC_CERT_USAGE_CA;    
        }     
        else
        {
            iTmpCertUsage = iCertUsage;
        }
        while (stSpCert.stCertUnion.stX509.usExtensionsLen > 0)
        {
            iResult = Sec_X509GetCertExtension(hSecHandle,stSpCert.stCertUnion.stX509.pucExtensions, \
                &iExtnId, &iCritical,&pucExtnValue,&usExtnValueLen, &usJumpStep);
            if (iResult != M_SEC_ERR_OK)
            {
                return iResult;
            }
            switch (iExtnId)
            {  
                case M_SEC_CERT_EXT_KEY_USAGE:
                    {  
                        iResult = Sec_X509GetKeyUsage(hSecHandle,pucExtnValue, &ucKeyUsageBit0To7, 
                            &ucKeyUsageBit8To15);                 
                        if (iResult != M_SEC_ERR_OK)              
                        {  
                            return iResult;  
                        }  
                        if (iTmpCertUsage == M_SEC_CERT_USAGE_CA ||iTmpCertUsage == M_SEC_CERT_USAGE_ROOT) 
                        {  
                            if ( !(ucKeyUsageBit0To7 & 0x4) )   
                            {     
                                return M_SEC_ERR_BAD_CERTIFICATE;   
                            } 
                        }
                    }       
                    break;    
                case M_SEC_CERT_EXT_EXTENDED_KEY_USAGE:    
                    if (M_SEC_CERT_USAGE_SERVER_AUTH == iTmpCertUsage) 
                    {    
                        iResult = Sec_X509GetKeyUsageEx(hSecHandle,pucExtnValue, &iAnyExtendedKeyUsage,\
                                            &iServerAuth, &iClientAuth, &iCodeSigning,&iEmailProtection, \
                                            &iTimeStamping,&iOCSPSigning);   
                        if (iResult != M_SEC_ERR_OK)  
                        {   
                            return iResult;   
                        }             
                        if (!(iServerAuth || iAnyExtendedKeyUsage)) 
                        {  
                            return M_SEC_ERR_BAD_CERTIFICATE; 
                        }    
                    }    
                    break;
                case M_SEC_CERT_EXT_BASIC_CONSTRAINTS: 
                    if (M_SEC_CERT_USAGE_CA == iTmpCertUsage||M_SEC_CERT_USAGE_ROOT == iTmpCertUsage) 
                    {  
                        iResult = Sec_X509GetBasicConstraint(hSecHandle,pucExtnValue, &iCA, 
                                                          &iHasPathLenConstraint,&ucPathLenConstraint); 
                        if (iResult != M_SEC_ERR_OK)  
                        {             
                            return iResult;  
                        }          
                        if (!iCA|| (iHasPathLenConstraint && (ucPathLenConstraint + 2 < iChainDepth)))  
                        {       
                            return M_SEC_ERR_BAD_CERTIFICATE;  
                        } 
                        else   
                        {     
                            iBasicConstraintCa = 1; 
                        }    
                    }      
                    break;
                case M_SEC_CERT_EXT_DOMAIN_INFORMATION:
                case M_SEC_CERT_EXT_CERTIFICATE_POLICIES:   
                case M_SEC_CERT_EXT_SUBJECT_ALT_NAME:        
                case M_SEC_CERT_EXT_NAME_CONSTRAINTS:        
                case M_SEC_CERT_EXT_POLICY_CONSTRAINTS:      
                case M_SEC_CERT_EXT_AUTHORITY_KEY_IDENTIFIER:
                case M_SEC_CERT_EXT_SUBJECT_KEY_IDENTIFIER:  
                    break;    
                default: 
                    if (iCritical)    
                    { 
                        return M_SEC_ERR_BAD_CERTIFICATE;  
                    }   
            }     
            stSpCert.stCertUnion.stX509.pucExtensions += usJumpStep;
            stSpCert.stCertUnion.stX509.usExtensionsLen = 
                (WE_UINT16)(stSpCert.stCertUnion.stX509.usExtensionsLen - usJumpStep);    
        }
        if ((M_SEC_CERT_USAGE_CA == iTmpCertUsage || M_SEC_CERT_USAGE_ROOT == iTmpCertUsage) && !iBasicConstraintCa) 
        {  
            return M_SEC_ERR_BAD_CERTIFICATE;  
        }  
    }   
    
    if ((stSpCert.stCertUnion.stX509.pucSignatureAlgId[stSpCert.stCertUnion.stX509.usSignatureAlgIdLen-1] == 5) || \
        (stSpCert.stCertUnion.stX509.pucSignatureAlgId[stSpCert.stCertUnion.stX509.usSignatureAlgIdLen-1] == 4) || \
        (stSpCert.stCertUnion.stX509.pucSignatureAlgId[stSpCert.stCertUnion.stX509.usSignatureAlgIdLen-1] == 2)  )     
    { 
        iResult = Sec_X509GetRsaModAndExp(hSecHandle,(const WE_UINT8 *)pucPublicKeyVal,
            &(stKey.pucModulus), &(stKey.usModLen), 
            &(stKey.pucExponent), &(stKey.usExpLen));
        if (M_SEC_ERR_OK == iResult)
        {     
            stPubKey = stKey;
            if (stSpCert.stCertUnion.stX509.pucSignatureAlgId[stSpCert.stCertUnion.stX509.usSignatureAlgIdLen-1] == 5)
            {
                iHashAlg = (WE_INT32)E_SEC_ALG_HASH_SHA1;
            }
            /*#ifdef M_SEC_CFG_MD2*/
            else if (stSpCert.stCertUnion.stX509.pucSignatureAlgId[stSpCert.stCertUnion.stX509.usSignatureAlgIdLen-1] == 2)
            {
                iHashAlg = (WE_INT32)E_SEC_ALG_HASH_MD2;
            }
            /* #endif*/
            else
            {
                iHashAlg = (WE_INT32)E_SEC_ALG_HASH_MD5;
            }            
            iResult = Sec_WimVerifyHashedSign (hSecHandle,iHashAlg, iTargetID, stPubKey, \
                     pucX509cert+4, usTBSCertLen, stSpCert.pucSignature, stSpCert.usSignatureLen);    
        }     
    }  
    else     
    {         
        return M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE;     
    }
    if (iResult != M_SEC_ERR_OK)
    {
        return M_SEC_ERR_BAD_CERTIFICATE;
    }
    else
    {
        return M_SEC_ERR_OK;
    }   
}

/*====================================================================================
FUNCTION: 
    Sec_WimVerifiCertChain  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    check certificate:include type,version,length,Algorithm.....   
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    pucChain[IN]:    certificate.
    iChainLen[IN] :  The length of  pucChain.
    pstCerts[IN]:    cert .
    pucNumCerts[OUT]:    number of cert. 
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimVerifiCertChain(WE_HANDLE hSecHandle,
                                const WE_UINT8 *pucChain, 
                                WE_INT32 iChainLen, 
                                St_SecAsn1Certificate *pstCerts, 
                                WE_UINT8 *pucNumCerts)
{
    WE_UINT8  *pucCert=NULL;
    WE_UINT16 usCertLen=0;
    WE_UINT16 usAccLen=0;
    WE_UINT8  ucFormat=0;
    WE_UINT8  ucLocalNumCerts = 0;
    WE_INT32  iResult=0;
    WE_UINT16 usJumpStep = 0;
    
    if ((NULL==pucChain )||(NULL==pucNumCerts))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    if (iChainLen < 3)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    pucCert = (WE_UINT8*)pucChain;
    do {     
        usCertLen = 0;
        ucFormat = (*pucCert);
        pucCert++;
        usAccLen++;
#ifdef M_SEC_CFG_WTLS       
        if (M_SEC_CERTIFICATE_WTLS_TYPE==ucFormat)                    /*wtls*/
        {  
            /*check wtls certificate length and format */                                         
            iResult = Sec_WtlsGetCertFieldLength(hSecHandle,pucCert, &usCertLen);
        }
        else 
#endif           
        if (M_SEC_CERTIFICATE_X509_TYPE==ucFormat)             /*x509*/
        {            
            iResult = Sec_X509GetCertFieldLength(hSecHandle,pucCert, &usCertLen, &usJumpStep);
            usCertLen += usJumpStep;                
        }        
        else                                        
        {              
            return M_SEC_ERR_INVALID_PARAMETER;
        } 
        if (iResult != M_SEC_ERR_OK)         
        {                  
            return iResult;          
        }
        
        if (pstCerts != NULL) 
        {       
            if (ucLocalNumCerts >= *pucNumCerts)  
            {
                return M_SEC_ERR_INVALID_PARAMETER;
            }
            pstCerts[ucLocalNumCerts].ucFormat = ucFormat;
            pstCerts[ucLocalNumCerts].pucCert =  pucCert;
            pstCerts[ucLocalNumCerts].uiCertLen =  usCertLen;
        }
        ucLocalNumCerts++;
        usAccLen += usCertLen;
        pucCert += usCertLen;    
    }    
    while (usAccLen <iChainLen); 
    
    if (usAccLen != iChainLen)      
    {          
        return M_SEC_ERR_INVALID_PARAMETER;      
    }      
    *pucNumCerts = ucLocalNumCerts;      
    return M_SEC_ERR_OK;
}

/*====================================================================================
FUNCTION: 
    Sec_WimEncryptDataToFile  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    after Encrypt File Data,then write Encrypted data to file.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:       Global data handle.
    hFileHandle[IN]:          filehandle
    pvData[IN]:               the  data need to be Encrypted 
    iPos[IN]:                 offset
    iDataLen[IN]:             length of pvdata
    piWriteLength[OUT]:       the returned length writed to file
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#ifndef M_SEC_CFG_NO_PLATFORM 
WE_INT32 Sec_WimEncryptDataToFile(WE_HANDLE hSecHandle,WE_HANDLE hFileHandle, 
                                         void *pvData, WE_INT32 iPos, WE_INT32 iDataLen,
                                         WE_INT32 *piWriteLength)
{
    WE_INT32 iResult=0;
    E_WE_ERROR eRes = E_WE_OK;
    WE_UINT8 *pucEncryptedData=NULL;
    WE_INT32 iEncryptedDataLen = 0;
    WE_INT32 iNewPos = 0;
    if ((NULL == hSecHandle) || (NULL == piWriteLength))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }        
    pucEncryptedData = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(iDataLen+10)*sizeof(WE_UINT8));
    if (NULL == pucEncryptedData)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    iEncryptedDataLen = iDataLen+10;
    /*Encrypt data*/
    iResult = Sec_WimEncryptData(hSecHandle,pvData, iDataLen, pucEncryptedData, &iEncryptedDataLen);
    if (iResult != M_SEC_ERR_OK)
    {
        WE_FREE(pucEncryptedData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    eRes = WE_FILE_SEEK(hFileHandle, iPos, E_WE_FILE_SEEK_SET,&iNewPos);
    /*write the Encrypted Data to file */
    eRes = WE_FILE_WRITE(hFileHandle, (void*)pucEncryptedData, iEncryptedDataLen,piWriteLength);
    if (((*piWriteLength) < (WE_INT32)iEncryptedDataLen) || ( eRes != E_WE_OK ))
    {
        WE_FREE(pucEncryptedData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    WE_FREE(pucEncryptedData);
    return (WE_INT32)eRes;
}
/*====================================================================================
FUNCTION: 
    Sec_WimDecryptDataFromFile 
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Read data from file from the positon of offset ,then Decrypted the data.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:       Global data handle.
    hFileHandle[IN]:          filehandle.
    pucDecryptedData[OUT]:    the decrypted data with padded data.
    iPos[IN]:                 the offset position in hFileHandle.
    iDataLen[IN]:             length of data wanted to read from the file. 
    piReaderLength[OUT]:      length of  pucDecryptedData.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimDecryptDataFromFile(WE_HANDLE hSecHandle,WE_HANDLE hFileHandle, 
                                        WE_UINT8 *pucDecryptedData, WE_INT32 iPos, 
                                        WE_INT32 iDataLen,WE_INT32 *piReaderLength)
{
    WE_INT32     iResult=0;
    E_WE_ERROR   eRes = E_WE_OK;
    WE_UINT8     *pucData=NULL;
    WE_INT32     iNewPos=0;
    WE_INT32     iDecDataLen=0;
    WE_INT32     iReadNum=0;
      
    if ((0 == iDataLen)||(NULL==piReaderLength))
    { 
        return M_SEC_ERR_INVALID_PARAMETER;
    }    
    pucData = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(iDataLen+1)*sizeof(WE_UINT8));    
    if(!pucData)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    eRes = WE_FILE_SEEK(hFileHandle, iPos, E_WE_FILE_SEEK_SET,&iNewPos);
    eRes = WE_FILE_READ(hFileHandle, pucData, iDataLen,&iReadNum);
    if((iReadNum<(WE_INT32)iDataLen)||(eRes !=E_WE_OK))
    {
        WE_FREE(pucData);
        return (WE_INT32)eRes;
    }
    pucData[iDataLen] = '\0';
    iDecDataLen=iReadNum;
    /*Decrypt Data which from file*/
    iResult = Sec_WimDecryptData(hSecHandle,pucData, iDataLen, pucDecryptedData,&iDecDataLen);
    *piReaderLength=iDecDataLen;
    if (iResult != M_SEC_ERR_OK)
    {
        WE_FREE(pucData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    WE_FREE(pucData);
    return iResult;
}
/*====================================================================================
FUNCTION: 
    Sec_WimGenCipherKey  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    generate Cipher Key
ARGUMENTS PASSED:
     pucKey[OUT]:    Generated key used to  encrypt private key and pin.
     iKeyLen[IN]:    length of pucKey.
     pucIv[OUT]:     IV    
     iIvLen[IN]:     length of IV
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimGenCipherKey(WE_UINT8 *pucKey, WE_INT32 iKeyLen ,WE_UINT8 *pucIv ,WE_INT32 iIvLen )
{
    WE_INT32    iLoop = 0;

    if(!pucKey || (iKeyLen < 2) || !pucIv || (iIvLen < 2))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    if(iKeyLen >= iIvLen)
    {
        pucKey[0] = (WE_UCHAR)((iKeyLen - iIvLen)%256);
    }
    else
    {
        pucKey[0] = (WE_UCHAR)((iKeyLen - iIvLen)%256);
    }
    
    pucKey[1] = (WE_UCHAR)((iKeyLen + iIvLen)%256);

    for(iLoop = 2; iLoop < iKeyLen; iLoop++)
    {
        pucKey[iLoop] = (pucKey[iLoop-1] + pucKey[iLoop-2])%256;
    }
    pucIv[0] = pucKey[iKeyLen-2];
    pucIv[1] = pucKey[iKeyLen-1];
    for(iLoop = 2; iLoop < iIvLen; iLoop++)
    {
        pucIv[iLoop] = (pucIv[iLoop-1] + pucIv[iLoop-2])%256;
    }

    return M_SEC_ERR_OK;

}
/*====================================================================================
FUNCTION: 
    Sec_WimEncryptData  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Encrypt File Data
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:     Global data handle.
    pucData[IN]:            the data to be  encrypted.     
    iDataLen[IN]:           length of pucData.    
    pucEncryptedData[OUT]:  encrypted data  
    piEncryptedDataLen[OUT]:length of pucEncryptedData
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimEncryptData(WE_HANDLE hSecHandle,WE_UINT8 *pucData, WE_INT32 iDataLen,
                            WE_UINT8 *pucEncryptedData, WE_INT32 *piEncryptedDataLen)
{
    WE_INT32            iResult=0;
    St_SecCipherCrptKey stkeyObj={0};
    WE_UINT8            *pucKey=NULL;
    WE_INT32            iLoop=0;
    WE_INT32            iLenWithPad = 0;
    WE_UINT8            *pucPaddedData=NULL;

    stkeyObj.iKeyLen = 24;
    stkeyObj.iIvLen  = 8;
 
    if ((!pucData) || (!pucEncryptedData) || (!piEncryptedDataLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    if (0==iDataLen)
    {
        *piEncryptedDataLen = 0;
        return M_SEC_ERR_OK;
    }

    pucKey = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(stkeyObj.iKeyLen+stkeyObj.iIvLen)*sizeof(WE_UINT8));
    if(!pucKey)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }

    iResult=Sec_WimGenCipherKey(pucKey,stkeyObj.iKeyLen,&pucKey[stkeyObj.iKeyLen],stkeyObj.iIvLen);
    if (iResult != M_SEC_ERR_OK)
    {
        WE_FREE(pucKey);
        return iResult;
    }
    stkeyObj.pucKey = pucKey;                
    stkeyObj.pucIv = &pucKey[stkeyObj.iKeyLen]; 

    iLenWithPad = iDataLen +2;            
    if( (iLenWithPad %8) != 0 )       
    {
        iLenWithPad += 8 -(iLenWithPad %8);
    }
    *piEncryptedDataLen = iLenWithPad;
  
    pucPaddedData = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(iLenWithPad)*sizeof(WE_UINT8));
    if (NULL == pucPaddedData)
    {       
        WE_FREE(pucKey);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }

    for(iLoop=0; iLoop<iDataLen; iLoop++)
    {
        pucPaddedData[iLoop] = pucData[iLoop];
    }
    /*Padded with 1 or 0*/
    pucPaddedData[iDataLen] = SEC_FIRST_PAD_VALUE;
    pucPaddedData[iDataLen +1] = SEC_SECOND_PAD_VALUE;

    for(iLoop=(iDataLen +2); iLoop<iLenWithPad; iLoop++)
    {
        pucPaddedData[iLoop] = SEC_SECOND_PAD_VALUE;
    }

    iResult = Sec_LibCipherEncrypt(hSecHandle, E_WE_ALG_CIPHER_3DES_CBC/*E_SEC_BULK_3DES_CBC_EDE*/, 
                                   stkeyObj, pucPaddedData, (WE_UINT32)iLenWithPad,
                                   pucEncryptedData,(WE_UINT32*)piEncryptedDataLen);

    WE_FREE(pucKey);
    WE_FREE(pucPaddedData);
    return iResult;
}
/*====================================================================================
FUNCTION: 
    Sec_WimDecryptData  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Decrypt File Data 
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    pucData[IN]:          Encrypted data
    iDataLen[IN]:         length of Encrypted data
    pucDecryptedData[OUT]:Decrypted data
    piDecDataLen[OUT]:    length of Decryped data
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimDecryptData(WE_HANDLE hSecHandle,WE_UINT8 *pucData, 
                            WE_INT32 iDataLen, WE_UINT8 *pucDecryptedData,WE_INT32 *piDecDataLen)
{
    WE_INT32            iResult=0;
    St_SecCipherCrptKey stKeyObj={0};
    WE_UINT8            *pucKey=NULL;

    stKeyObj.iKeyLen = 24;
    stKeyObj.iIvLen  = 8;

    if(iDataLen == 0)
    {
        return M_SEC_ERR_OK;
    }
    if(( NULL==pucData) || (NULL==pucDecryptedData ) )
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    pucKey = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(stKeyObj.iKeyLen+stKeyObj.iIvLen)*sizeof(WE_UINT8));
    if(!pucKey )
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
 
    iResult=Sec_WimGenCipherKey(pucKey,stKeyObj.iKeyLen,&pucKey[stKeyObj.iKeyLen],stKeyObj.iIvLen);
    if (iResult != M_SEC_ERR_OK)
    {  
        WE_FREE(pucKey);
        return iResult;
    }
    stKeyObj.pucKey = pucKey;        
    stKeyObj.pucIv = &pucKey[stKeyObj.iKeyLen]; 

    /*Decrypt data*/
    iResult = Sec_LibCipherDecrypt(hSecHandle,E_WE_ALG_CIPHER_3DES_CBC/*E_SEC_BULK_3DES_CBC_EDE*/,
        stKeyObj, pucData, (WE_UINT32)iDataLen, pucDecryptedData,(WE_UINT32 *)piDecDataLen);

    WE_FREE(pucKey);
    return iResult;  
}
#endif
/*====================================================================================
FUNCTION: 
    Sec_WimGetX509Sub  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Parse X509 certificate,get Subject.  
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    pucX509Cert[IN]:            X509 certificate.
    ppucSubject[OUT]:            Subject
    pusSubjectLen[OUT]:         the length of ppucSubject.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code. 
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimGetX509Sub(WE_HANDLE hSecHandle,WE_UINT8 *pucX509Cert, 
                               WE_UINT8 **ppucSubject,WE_UINT16 *pusSubjectLen)
{
    WE_INT32 iResult=0;
    St_SecCertificate stCert={0};
    WE_UINT16 usCertLen = 0;
    if((NULL==hSecHandle)||(NULL==pucX509Cert)||(NULL==ppucSubject)||(NULL==pusSubjectLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }

    iResult = Sec_X509CertParse(hSecHandle,pucX509Cert, &usCertLen, &stCert);    
    *ppucSubject = stCert.pucSubject;
    *pusSubjectLen = stCert.stCertUnion.stX509.usSubjectLen;
    return iResult;
}
#endif

/*====================================================================================
FUNCTION: 
    Sec_WimGetX509PubKey  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Get X509 RSA publicKey
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    pucX509Cert[IN]:            certificate
    pusExpLen[OUT]:            the length of Exponent.
    ppucExponent[OUT}:        Exponent.
    pusModLen[OUT]:            the length of Modulus.
    ppucModulus[OUT]:        Modulus.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code. 
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimGetX509PubKey(WE_HANDLE hSecHandle,WE_UINT8 *pucX509Cert, 
                              WE_UINT16 *pusExpLen, WE_UINT8** ppucExponent,
                              WE_UINT16 *pusModLen, WE_UINT8** ppucModulus)
{
    WE_INT32 iResult=0;
    WE_UINT16 usCertLen = 0;
    St_SecCertificate stCert={0};
    iResult = Sec_X509CertParse(hSecHandle,pucX509Cert, &usCertLen, &stCert);    
    if (iResult != M_SEC_ERR_OK)
    {
        return iResult;
    }
    if (stCert.stCertUnion.stX509.pucPublicKeyAlgId[stCert.stCertUnion.stX509.usPublicKeyAlgIdLen-2] == 1)
    {
        iResult = Sec_X509GetRsaModAndExp (hSecHandle,stCert.stCertUnion.stX509.pucPublicKeyVal,
                                      ppucModulus, pusModLen, ppucExponent, pusExpLen);
    }
    else 
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    return iResult;
  
}

/*====================================================================================
FUNCTION: 
    Sec_WimVerifyHashedSign  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Verify Hashed Signature .  
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    iHashAlg[IN]:         Algorithm 
    iTargetID[IN]:        iTargetID     
    stPubKey[IN]:         public key.
    pucMsg[IN]:           certificate.
    iMsgLen[IN]:          the lenght of pucMsg.
    pucSig[IN]:           Signature
    iSigLen[IN]:          the lenght of Sig.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimVerifyHashedSign (WE_HANDLE  hSecHandle,WE_INT32 iHashAlg,
                                  WE_INT32 iTargetID,  St_SecCrptPubKey stPubKey, 
                                  const WE_UINT8 *pucMsg, WE_INT32 iMsgLen, 
                                  const WE_UINT8 *pucSig, WE_INT32 iSigLen)
{
   WE_INT32 iResult=0;
   WE_UINT8 aucDigest[20]={0};
   WE_INT32 iDigestLen=20;

   if ((WE_INT32)E_SEC_ALG_HASH_SHA1==iHashAlg )
   {
       iDigestLen = 20;
   }
   else    
   {
       iDigestLen = 16;
   }
   
   iResult = Sec_LibHash(hSecHandle, (E_SecHashAlgType)iHashAlg, pucMsg, iMsgLen, aucDigest,&iDigestLen);
   if (iResult != M_SEC_ERR_OK)
   {
       return iResult;
   }
   return Sec_PKCRsaVerifySignature (hSecHandle,iTargetID, stPubKey,\
                aucDigest, iDigestLen, E_SEC_RSA_PKCS1_NULL,pucSig, iSigLen);
}
/*====================================================================================
FUNCTION: 
    Sec_WimConvPubKey  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Convert public Key   
ARGUMENTS PASSED:
    stKey[IN]:            
    ppucPubKey[OUT]: public key .          
    pusOutPubKeyLen[OUT]:   the length of public key.        
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimConvPubKey(St_SecPubKeyRsa stKey, WE_UINT8 **ppucPubKey, 
                                    WE_UINT16 *pusOutPubKeyLen)
{
    WE_UINT16   usMsgLen=0;
    WE_UINT16   usHdrLen=0;
    WE_UINT16   usMLen=0;
    WE_UINT16   usELen=0;
    WE_UINT8    ucPrePendMod = 0;
    WE_UINT8    ucPrependExp = 0;
    WE_UINT16   usM = stKey.usModLen;
    WE_UINT16   usE = stKey.usExpLen;
    WE_UINT8    *pucP=NULL;
    WE_INT32    iLoop=0;
    WE_UINT8    ucBigEndian=0;   
    WE_UINT16   usU = 0x0102;
 
    if (0x01 == *((WE_INT8*)&(usU))) 
    {
        ucBigEndian = 1;
    }
    else
    {
        ucBigEndian = 0;
    }

    if (*(stKey.pucModulus) > 127)
    {
        ucPrePendMod = 1;
        usM++;
    }
    usMLen = 2;
    if (usM > 127)
    {
        usMLen++;
    }
    if (usM > 255)
    {
        usMLen++;
    } 
    if (*(stKey.pucExponent) > 127)
    {
        ucPrependExp = 1;
        usE++;
    }
    usELen = 2;
    if (usE > 127)
    {
        usELen++;
    }
    if (usE > 255)
    {
        usELen++;
    }

    usMsgLen = (WE_UINT16)(usMLen + usM + usELen + usE);

    usHdrLen = 2;
    if (usMsgLen > 127)
    {
        usHdrLen++;
    }
    if (usMsgLen > 255)
    {
        usHdrLen++;
    }
    if((!ppucPubKey)||(!pusOutPubKeyLen))
    {
        return -1;
    }
    *ppucPubKey = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(usHdrLen + usMsgLen)*sizeof(WE_UINT8)); 
    if (!(*ppucPubKey))
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    pucP = (*ppucPubKey);
    *pucP++ = 0x30; 
   
    switch (usHdrLen)
    {
        case 2: 
            if (ucBigEndian)
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usMsgLen))+1));
            } 
            else
            {
                *pucP++ =(WE_UINT8)( *((WE_INT8*)&(usMsgLen)));
            }   
            break; 
        case 3:
            *pucP++ = 0x81; 
            if (ucBigEndian)
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usMsgLen))+1)); 
            }
            else
            {
                *pucP++ =(WE_UINT8)( *((WE_INT8*)&(usMsgLen))); 
            }
            break;  
        case 4:
            *pucP++ = 0x82; 
            if (ucBigEndian)
            {
                *pucP++ = (WE_UINT8)(*((WE_INT8*)&(usMsgLen)));
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usMsgLen))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usMsgLen))+1)); 
                *pucP++ =(WE_UINT8)( *((WE_INT8*)&(usMsgLen)));
            } 
            break;
        default:
            break;
    } 
   
    *pucP++ = '\x02'; 

    switch (usMLen)   
    {  
        case 2:      
            if (ucBigEndian)
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usM))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*((WE_INT8*)&(usM)));
            }
            break;
        case 3:
            *pucP++ = 0x81; 
            if (ucBigEndian)
            {
                *pucP++ =(WE_UINT8)( *(((WE_INT8*)&(usM))+1)); 
            }
            else
            {
                *pucP++ =(WE_UINT8)(*((WE_INT8*)&(usM)));
            }
            break;
        case 4:
            *pucP++ = 0x82; 
            if (ucBigEndian)
            {
                *pucP++ =(WE_UINT8)(*((WE_INT8*)&(usM)));
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usM))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usM))+1)); 
                *pucP++ = (WE_UINT8)(*((WE_INT8*)&(usM)));
            }
            break;
        default:
            break;
    }

    if (ucPrePendMod)
    {
        *pucP++ = '\x00';
    }
    for (iLoop=0; iLoop<stKey.usModLen; iLoop++) 
    {
        *pucP++ = stKey.pucModulus[iLoop];
    }
    *pucP++ = '\x02'; 
   
    switch (usELen)
    {
        case 2:
            if (ucBigEndian)
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usE))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*((WE_INT8*)&(usE)));
            }
            break;
        case 3:
            *pucP++ = 0x81; 
            if (ucBigEndian)
            {
                *pucP++ =(WE_UINT8)(*(((WE_INT8*)&(usE))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*((WE_INT8*)&(usE)));
            }
            break;
        case 4:
            *pucP++ = 0x82; 
            if (ucBigEndian)
            {
                *pucP++ =(WE_UINT8)( *((WE_INT8*)&(usE)));
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usE))+1)); 
            }
            else
            {
                *pucP++ = (WE_UINT8)(*(((WE_INT8*)&(usE))+1)); 
                *pucP++ =(WE_UINT8)(*((WE_INT8*)&(usE)));
            }
            break;
        default:
            break;
    }

    if (ucPrependExp)
    {
        *pucP++ = '\x00';
    }
    for (iLoop=0; iLoop<stKey.usExpLen; iLoop++) 
    {
        *pucP++ = stKey.pucExponent[iLoop];
    }

    (*pusOutPubKeyLen) = (WE_UINT16) (usHdrLen + usMsgLen);
    return M_SEC_ERR_OK;
}

/*====================================================================================
FUNCTION: 
    Sec_WimGetCACert  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    get root cert
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:     Global data handle.
    iIndex[IN]:             the offset positon in indexfile.    
    ppucCert[OUT]:          the content of certificate:(type+Certification)
    pusCertLen[OUT]:        the length of pucCert. 
    puiFileId[OUT]:         fileId   
    pucCertType[OUT]        certificate type
    pucAble[OUT]            able attribute of certificate
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimGetCACert(WE_HANDLE hSecHandle,WE_INT32 iIndex, WE_UINT8 **ppucCert,
             WE_UINT16 *pusCertLen, WE_INT32 *piCertType, WE_CHAR cFlag, WE_HANDLE hCACerts)
{
    WE_UCHAR     ucCANum = 0;
    St_SecCACert *pstCACert = NULL;
    
    if ((NULL==hSecHandle) || (NULL==ppucCert) || (NULL==pusCertLen) || (NULL == piCertType))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    } 
    
    if (M_SEC_OUTER == cFlag)
    {
        pstCACert = (St_SecCACert *)hCACerts;
    }
    else if (M_SEC_LOCAL == cFlag)
    {
        pstCACert = (St_SecCACert *)M_SEC_WIM_CACERT;
    }
    else
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    while (pstCACert != NULL)
    {
        ucCANum++;
        if (iIndex == ucCANum)
        {
            *ppucCert = pstCACert->pucCert;
            *pusCertLen = (WE_UINT16)(pstCACert->iCertLen);
            *piCertType= pstCACert->iCertType;  
            return M_SEC_ERR_OK;          
        }
        else
        {
            pstCACert = pstCACert->pstNext;
        }
    }      

    return M_SEC_ERR_NOT_FOUND;              
}  

/*====================================================================================
FUNCTION: 
    Sec_WimGetKeyExgIds  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Get Key Exchange Ids .
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    ppucKeyExchangeIds[OUT]:    the value of Id.
    piKeyExIdLen[OUT]:          The length of ppucKeyExchangeIds. 
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
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
void Sec_WimGetKeyExgIds(WE_HANDLE hSecHandle,
                              WE_UINT8 *pucKeyExchangeIds, WE_INT32 *piKeyExIdLen)
{
    WE_INT32    iIdIndex=0;
    
    if ((NULL == pucKeyExchangeIds)||(NULL == piKeyExIdLen)||(NULL == hSecHandle))
    {
        return;
    }
   
    pucKeyExchangeIds[iIdIndex++]=(WE_UINT8)E_SEC_WTLS_KEYEXCHANGE_RSA;
    pucKeyExchangeIds[iIdIndex++]=(WE_UINT8)0;
    pucKeyExchangeIds[iIdIndex++]=(WE_UINT8)0;   
    
    *piKeyExIdLen=iIdIndex;

}          
/*====================================================================================
FUNCTION: 
    Sec_WimGetWTLSMasterSecret  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    get Master secret  
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    iMasterSecretId[IN]:        The id of the corresponding session slot.
    pucSecret[IN]:              PreMasterSecret.
    iSecretLen[IN]:             length of PreMasterSecret.
    pcLabel[IN]:                lable.
    pucSeed[IN]:                seed.
    iSeedLen[IN]:               length of seed.
    iOutputLen[IN]:             output length.
    pucBuf [OUT]:               MasterSecret
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimGetWTLSMasterSecret(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId,const WE_UINT8 *pucSecret, 
                                    WE_INT32 iSecretLen, const WE_CHAR *pcLabel,const WE_UINT8 *pucSeed, 
                                    WE_INT32 iSeedLen, WE_INT32 iOutputLen, WE_UINT8 *pucBuf,WE_UINT8 ucHashAlg)
{
    WE_INT32    iLabelLen=0;
    WE_UINT8    *pucLabAndSeed=NULL;
    WE_UINT8    aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN]={0};
    WE_INT32    iResult=0;
    WE_INT32    iLoop=0;
    WE_CHAR     *pcTmpLabel="master secret";   
  
    if ((NULL==pcLabel) || (NULL==pucSeed))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    iLabelLen =(WE_INT32)SEC_STRLEN(pcLabel);
    pucLabAndSeed = (WE_UINT8 *)WE_MALLOC(((WE_UINT32)(iLabelLen + iSeedLen))*sizeof(WE_UINT8));
    if (NULL == pucLabAndSeed)
    { 
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }    
    /*get pucLabelAndSeed*/
    for (iLoop=0; iLoop<iLabelLen; iLoop++)
    {
        pucLabAndSeed[iLoop] = (WE_UINT8)(pcLabel[iLoop]);
    }
    for (iLoop=0; iLoop<iSeedLen; iLoop++)
    {
        pucLabAndSeed[iLabelLen + iLoop] = pucSeed[iLoop];
    }
  
    if ( NULL==pucSecret )
    {
        if ((0 == WE_MEMCMP(pcLabel, "client write IV",(WE_UINT32)SEC_STRLEN(pcLabel)))
            || (0 == WE_MEMCMP(pcLabel, "server write IV",(WE_UINT32)SEC_STRLEN(pcLabel))))
        { 
            if((WE_UINT8)E_SEC_ALG_HASH_SHA1 == ucHashAlg)
            {
                Sec_WimPHash(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                             pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                             &iResult, pucBuf, iOutputLen);
            }
            else if((WE_UINT8)E_SEC_ALG_HASH_MD5 == ucHashAlg)
            {
                Sec_WimPMD5(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                         pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                         &iResult, pucBuf, iOutputLen);
            }
        }
        else if(WE_MEMCMP(pcLabel,pcTmpLabel,(WE_UINT32)SEC_STRLEN(pcLabel)))
        { 
            /*if pre master secret is NULL,get aucMasterSecret by computer iMasterSecretId */
            Sec_WimGetMastSecret(hSecHandle,iMasterSecretId, aucMasterSecret, M_SEC_WTLS_MASTER_SECRET_LEN, &iResult);
            if (iResult != M_SEC_ERR_OK)
            {
                WE_FREE(pucLabAndSeed);
                return iResult;
            }
            if((WE_UINT8)E_SEC_ALG_HASH_SHA1 == ucHashAlg)
            {
                Sec_WimPHash(hSecHandle,aucMasterSecret, M_SEC_WTLS_MASTER_SECRET_LEN, 
                             pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                             &iResult, pucBuf, iOutputLen);
            }
            else if((WE_UINT8)E_SEC_ALG_HASH_MD5 == ucHashAlg)
            {
                Sec_WimPMD5(hSecHandle,aucMasterSecret, M_SEC_WTLS_MASTER_SECRET_LEN, 
                         pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                         &iResult, pucBuf, iOutputLen);
            }
        }
        else   /*master secret*/
        {
           if((WE_UINT8)E_SEC_ALG_HASH_SHA1 == ucHashAlg)
           {
                Sec_WimPHash(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                             pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                             &iResult, pucBuf, iOutputLen);
           }
            else if((WE_UINT8)E_SEC_ALG_HASH_MD5 == ucHashAlg)
            {
                Sec_WimPMD5(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                    pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                             &iResult, pucBuf, iOutputLen);
            }
        }   
    }
    else
    {
        if((WE_UINT8)E_SEC_ALG_HASH_SHA1 == ucHashAlg)
        {
            Sec_WimPHash(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                         pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                         &iResult, pucBuf, iOutputLen);
        }
        else if((WE_UINT8)E_SEC_ALG_HASH_MD5 == ucHashAlg)
        {
            Sec_WimPMD5(hSecHandle,(WE_UINT8 *)pucSecret, iSecretLen, 
                pucLabAndSeed, (WE_INT32)(iLabelLen + iSeedLen), 
                         &iResult, pucBuf, iOutputLen);
        }
    }

    WE_FREE(pucLabAndSeed);
    return iResult;
} 
#endif

#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
void Sec_WimHMACMD5(WE_HANDLE hSecHandle,WE_UCHAR *pucK, WE_INT32 iKLen, 
                     WE_UCHAR *pucData, WE_INT32 iDataLen, WE_INT32 *piResult, 
                     WE_UCHAR *pucBuf)
{
    WE_UINT8 *pucInner = NULL;
    WE_UINT8 *pucOuter = NULL;
    WE_UCHAR *pucBptr = NULL;
    WE_INT32  iIndex = 0;
    WE_INT32 iDigLen = 16;
    WE_INT32 iResult=0;

    pucInner = (WE_UINT8*)WE_MALLOC((WE_UINT32)(SEC_HMAC_B + iDataLen)*sizeof(WE_UCHAR));
    if (pucInner == NULL)
    {
        *piResult = M_SEC_ERR_INVALID_PARAMETER;
        return;
    }
    pucOuter = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(SEC_HMAC_B + SEC_HMAC_MD5_L)*sizeof(WE_UCHAR));
    if (pucOuter == NULL)
    {
        *piResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
         WE_FREE(pucInner);
        return;
    }
    if (iKLen > SEC_HMAC_B)
    {
        iResult = Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_MD5, pucK, iKLen, pucInner,&iDigLen); 
        if(iResult !=M_SEC_ERR_OK)
        { 
            WE_FREE(pucOuter);
            WE_FREE(pucInner);
            *piResult =iResult;
            return;
        }

        iKLen = SEC_HMAC_MD5_L;
    }
    else 
    {
        for (iIndex = 0; iIndex < iKLen; iIndex++)
        {
            if ( pucK != NULL )
            {
                pucInner[iIndex] = pucK[iIndex];
            }
        }
    }
    
    for (iIndex = iKLen; iIndex < SEC_HMAC_B; iIndex++)
        pucInner[iIndex] = 0;
    for (iIndex = 0; iIndex < SEC_HMAC_B; iIndex++)  
        pucOuter[iIndex] = pucInner[iIndex];
    
    for (iIndex = 0; iIndex < SEC_HMAC_B; iIndex++)
        pucInner[iIndex] ^= 0x36;
    
    for (iIndex = 0; iIndex < SEC_HMAC_B; iIndex++)
        pucOuter[iIndex] ^= 0x5c;
    
    for (iIndex = 0; iIndex < iDataLen; iIndex++)
        pucInner[SEC_HMAC_B + iIndex] = pucData[iIndex];
    pucBptr = pucOuter;
    pucBptr += SEC_HMAC_B; 
    iResult = Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_MD5, pucInner, (SEC_HMAC_B+iDataLen), pucBptr,&iDigLen); 
    if(iResult!=M_SEC_ERR_OK)
    {
        if (pucOuter != NULL)
        {
            WE_FREE(pucOuter);
        }
        if (pucInner != NULL)
        {
            WE_FREE(pucInner);    
        }
        *piResult =iResult;
        return;
    }
    iResult = Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_MD5, pucOuter, (SEC_HMAC_B+SEC_HMAC_MD5_L), pucBuf,&iDigLen); 
    if(iResult !=M_SEC_ERR_OK)
    {          
        if (pucOuter != NULL)
        {
            WE_FREE(pucOuter);
        }
        if (pucInner != NULL)
        {
            WE_FREE(pucInner);    
        }
        *piResult =iResult;
        return;
    }
    *piResult = M_SEC_ERR_OK;
    
    if (pucOuter != NULL)
        WE_FREE(pucOuter);
    if (pucInner != NULL)
        WE_FREE(pucInner);
}

void Sec_WimPMD5(WE_HANDLE hSecHandle,WE_UCHAR *pucSecret, WE_INT32 iSecretLen, 
                  WE_UCHAR *pucSeed, WE_INT32 iSeedLen, WE_INT32 *iResult, 
                  WE_UCHAR *pucBuf, WE_INT32 iDesiredLen)
{
    WE_UCHAR *pucSecAndSeed = NULL; 
    WE_UCHAR aucPart[16] = {0};   
    WE_INT32 iRes = 0;
    WE_INT32 iIterations = 0;
    WE_INT32 iIndexI = 0;
    WE_INT32 iIndexJ = 0;
    WE_INT32 iNumCopiedChars = 0;;
    
    pucSecAndSeed = (WE_UCHAR *)WE_MALLOC((WE_UINT32)(SEC_HMAC_MD5_L + iSeedLen)*sizeof(WE_CHAR));
    if (NULL == pucSecAndSeed )
    {
        *iResult = M_SEC_ERR_INVALID_PARAMETER;
        return;
    }
    
    Sec_WimHMACMD5(hSecHandle,pucSecret, iSecretLen, pucSeed, iSeedLen, &iRes, pucSecAndSeed); 
    if (iRes != M_SEC_ERR_OK)
    {
        *iResult = iRes;
        WE_FREE(pucSecAndSeed);
        return;
    }
    for (iIndexI = 0; iIndexI < iSeedLen; iIndexI++)
    {
        pucSecAndSeed[SEC_HMAC_MD5_L + iIndexI] = pucSeed[iIndexI]; 
    }
    
    iIterations = iDesiredLen / SEC_HMAC_MD5_L; 
    if (iDesiredLen % SEC_HMAC_MD5_L != 0)
    {
        iIterations++;
    }
    
    iNumCopiedChars = 0;
    for (iIndexI = 0; iIndexI < iIterations; iIndexI++)
    {
        Sec_WimHMACMD5(hSecHandle,pucSecret, iSecretLen, pucSecAndSeed, (SEC_HMAC_MD5_L + iSeedLen), &iRes, aucPart);
        if (iRes != M_SEC_ERR_OK)
        {
            *iResult = iRes;
            if (NULL != pucSecAndSeed)
            {
                WE_FREE(pucSecAndSeed);
            }
            return;
        }
        
        for (iIndexJ  = 0; iIndexJ < SEC_HMAC_MD5_L; iIndexJ++)
        {
            pucBuf[(SEC_HMAC_MD5_L * iIndexI) + iIndexJ] = aucPart[iIndexJ];
            if (++iNumCopiedChars >= iDesiredLen)
                break;
        }
        if (iNumCopiedChars >= iDesiredLen)
            break;
        Sec_WimHMACMD5(hSecHandle,pucSecret, iSecretLen, pucSecAndSeed, SEC_HMAC_MD5_L, &iRes, pucSecAndSeed); 
        if (iRes != M_SEC_ERR_OK)
        {
            *iResult = iRes;
            if (pucSecAndSeed != NULL)
                WE_FREE(pucSecAndSeed);
            return;
        }
    }
    
    *iResult = M_SEC_ERR_OK;
    if (pucSecAndSeed != NULL)
        WE_FREE(pucSecAndSeed);
    return;
}

/*====================================================================================
FUNCTION: 
    Sec_WimPHash  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:    
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    pucSecret[IN]:              secret.
    iSecretLen[IN]:             length of pucSecret.
    pucSeed[IN]:                seed.
    iSeedLen[IN]:               length of pucSeed.
    piResult[OUT]:              result.
    pucBuf[OUT]:                buffer.
    iDesiredLen[IN]:            Desired Lenght.
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
=====================================================================================*/
void Sec_WimPHash(WE_HANDLE hSecHandle,WE_UINT8 *pucSecret, WE_INT32 iSecretLen, 
                  WE_UINT8 *pucSeed, WE_INT32 iSeedLen, WE_INT32 *piResult, 
                  WE_UINT8 *pucBuf, WE_INT32 iDesiredLen)
{
    WE_INT32 iRes=0;
    WE_INT32 iTimes=0;
    WE_INT32 iLoop=0;
    WE_INT32 iJLoop=0;
    WE_UINT8 *pucSecAndSeed=NULL; 
    WE_INT32 iNumCopiedchars=0;
    WE_UINT8 aucPart[20]={0};   
    if((!hSecHandle)||(!pucSeed))
    {
        *piResult = M_SEC_ERR_INVALID_PARAMETER;
         return ;
    }
    
    pucSecAndSeed = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(SEC_HMAC_SHA1_L + iSeedLen)*sizeof(WE_UINT8));
    if (!pucSecAndSeed)
    {
        *piResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
        return;
    }
    
    Sec_WimHMAC(hSecHandle,pucSecret, iSecretLen, pucSeed, iSeedLen,  &iRes, pucSecAndSeed); 
    if (iRes != M_SEC_ERR_OK)
    {
        *piResult = iRes;      
         WE_FREE(pucSecAndSeed);  
         return;
    }
    for (iLoop=0; iLoop<iSeedLen; iLoop++)
    {
        pucSecAndSeed[SEC_HMAC_SHA1_L+iLoop] = pucSeed[iLoop]; 
    }
    
    iTimes = iDesiredLen / SEC_HMAC_SHA1_L; 
    if (iDesiredLen % SEC_HMAC_SHA1_L != 0)
    {
        iTimes++;
    }
    iNumCopiedchars = 0;
    for (iLoop=0; iLoop<iTimes; iLoop++)
    {
        Sec_WimHMAC(hSecHandle,pucSecret, iSecretLen, pucSecAndSeed, (SEC_HMAC_SHA1_L + iSeedLen), &iRes, aucPart);
        if (iRes != M_SEC_ERR_OK)
        {
            *piResult = iRes;
            if (pucSecAndSeed != NULL)
            {
                WE_FREE(pucSecAndSeed);
            }
            return;
        }
        
        for (iJLoop=0; iJLoop<SEC_HMAC_SHA1_L; iJLoop++)
        {
            if(!pucBuf)
            {
                *piResult = M_SEC_ERR_INVALID_PARAMETER;      
                WE_FREE(pucSecAndSeed);  
                return;
            }
            pucBuf[(SEC_HMAC_SHA1_L*iLoop) + iJLoop] = aucPart[iJLoop];
            if (++iNumCopiedchars >= iDesiredLen)
            {
                break;
            }
        }
        if (iNumCopiedchars >= iDesiredLen)
        {
            break;
        }
        Sec_WimHMAC(hSecHandle,pucSecret, iSecretLen, pucSecAndSeed, SEC_HMAC_SHA1_L, &iRes, pucSecAndSeed); 
        if (iRes != M_SEC_ERR_OK)
        {
            *piResult = iRes;
            if (pucSecAndSeed != NULL)
            {
                WE_FREE(pucSecAndSeed);
            }
            return;
        }
    }    
    *piResult = M_SEC_ERR_OK;
    if (pucSecAndSeed != NULL)
    {
        WE_FREE(pucSecAndSeed);
    }
    return;
}
#endif

/*====================================================================================
FUNCTION: 
    Sec_WimHMAC  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    HMAC
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:         Global data handle.
    pucK[IN]:       secret.        
    iKLen[IN]:      the length of pucK.        
    pucData[IN]:    seed.        
    iDataLen[IN]:   the length of pucData.      
    piResult[OUT]:  result.        
    pucBuf[OUT]:      pucbuf.        
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
=====================================================================================*/
#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
void Sec_WimHMAC(WE_HANDLE hSecHandle,WE_UINT8 *pucK, WE_INT32 iKLen, WE_UINT8 *pucData, 
                 WE_INT32 iDataLen, WE_INT32 *piResult,WE_UINT8 *pucBuf)
{
    WE_UINT8 *pucIn=NULL;
    WE_UINT8 *pucOut=NULL;
    WE_UINT8 *pucBptr=NULL;
    WE_INT32 iLoop=0;
    WE_INT32 iDigLen=0;
    WE_INT32 iResult=0;
   
    if((!hSecHandle)||(!pucData))
    {
        *piResult = M_SEC_ERR_INVALID_PARAMETER;
         return ;
    }
    pucIn = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(SEC_HMAC_B + iDataLen)*sizeof(WE_UINT8));        
    if (NULL==pucIn)
    {
        *piResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
        return;
    }
    pucOut = (WE_UINT8 *)WE_MALLOC((WE_UINT32)(SEC_HMAC_B + SEC_HMAC_SHA1_L)*sizeof(WE_UINT8));
    if (!pucOut)
    {
        *piResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
        WE_FREE(pucIn);
        return;
    }
        
    if (iKLen > SEC_HMAC_B)
    {
        iDigLen=SEC_HMAC_B + iDataLen;
        iResult=Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_SHA1, pucK, iKLen, pucIn,&iDigLen); 
        if(iResult!=M_SEC_ERR_OK)
        {          
             WE_FREE(pucOut);
             WE_FREE(pucIn);
             *piResult =iResult;
             return;
        }
        iKLen = SEC_HMAC_SHA1_L;
    }
    else 
    {
        for (iLoop=0; iLoop<iKLen; iLoop++)
        {
            if (pucK != NULL)
            {
                pucIn[iLoop] = pucK[iLoop];
            }
        }
    }
    
    for (iLoop=iKLen; iLoop<SEC_HMAC_B; iLoop++)
    {
        pucIn[iLoop] = 0;
    }
    for (iLoop=0; iLoop<SEC_HMAC_B; iLoop++)  
    {
        pucOut[iLoop] = pucIn[iLoop];
    }    
    for (iLoop=0; iLoop<SEC_HMAC_B; iLoop++)
    {
        pucIn[iLoop] ^= 0x36;
    }    
    for (iLoop=0; iLoop<SEC_HMAC_B; iLoop++)
    {
        pucOut[iLoop] ^= 0x5c;
    }    
    for (iLoop=0; iLoop<iDataLen; iLoop++)
    {
        pucIn[SEC_HMAC_B+iLoop] = pucData[iLoop];
    }
    
    pucBptr = pucOut;
    pucBptr += SEC_HMAC_B; 
    iDigLen=SEC_HMAC_SHA1_L;
    iResult=Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_SHA1, pucIn, (SEC_HMAC_B+iDataLen), pucBptr,&iDigLen); 
    if(iResult!=M_SEC_ERR_OK)
    {
        if (pucOut != NULL)
        {
            WE_FREE(pucOut);
        }
        if (pucIn != NULL)
        {
            WE_FREE(pucIn);    
        }
        *piResult =iResult;
        return;
    }
    
    iDigLen=SEC_HMAC_SHA1_L;
    iResult=Sec_LibHash(hSecHandle,E_SEC_ALG_HASH_SHA1, pucOut, (SEC_HMAC_B+SEC_HMAC_SHA1_L), pucBuf,&iDigLen); 
    if(iResult!=M_SEC_ERR_OK)
    { 
        if (pucOut != NULL)
        {
            WE_FREE(pucOut);
        }
        if (pucIn != NULL)
        {
            WE_FREE(pucIn);
        }
        *piResult =iResult;
        return;
    }
    *piResult = M_SEC_ERR_OK;
    
    if (pucOut != NULL)
    {
        WE_FREE(pucOut);
    }
    if (pucIn != NULL)
    {
        WE_FREE(pucIn);
    }
}
#endif
/*====================================================================================
FUNCTION: 
    Sec_WimGetNbrOfCACerts  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
    Get number of ca certificate .
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:             Global data handle.
    pucCACertNum[OUT]:              Number of ca certificate.
RETURN VALUE:
    the number of ca certificate.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimGetNbrOfCACerts(WE_HANDLE hSecHandle, WE_INT32 *piCACertNum, 
                                WE_CHAR cFlag, WE_HANDLE hCACerts)
{
    WE_UCHAR     ucCANum = 0;
    St_SecCACert *pstCACert = NULL;
    
    if ((NULL == hSecHandle) || (NULL == piCACertNum))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }

    if (M_SEC_OUTER == cFlag)
    {
        pstCACert = (St_SecCACert *)hCACerts;
    }
    else if (M_SEC_LOCAL == cFlag)
    {
        pstCACert = (St_SecCACert *)M_SEC_WIM_CACERT;
    }
    else
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    while (pstCACert != NULL)
    {
        ucCANum++;
        pstCACert = pstCACert->pstNext;
    }   

    *piCACertNum = ucCANum;
    
    return M_SEC_ERR_OK;
}                 
#ifdef M_SEC_CFG_TLS
/*====================================================================================
FUNCTION: 
    Sec_WimSSLPRF  
CREATE DATE: 
    2007-1-18
AUTHOR: 
    Tang  
DESCRIPTION:
    Computer SSL PRF.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    WE_INT32 iMasterSecretId[IN]:
    const WE_UCHAR *pucSecret[IN]:
    WE_INT32 iSecretLen[IN]:
    const WE_UCHAR *pucSeed[IN]:
    WE_INT32 iSeedLen[IN]:
    WE_INT32 iOutputLen[IN]:
    WE_UCHAR *pucBuf[IN]:
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimSSLPRF(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId, 
                       const WE_UCHAR *pucSecret, WE_INT32 iSecretLen, 
                       const WE_UCHAR *pucSeed, WE_INT32 iSeedLen, 
                       WE_INT32 iOutputLen, WE_UCHAR *pucBuf)
{
  WE_UCHAR          aucLabel[] = "ABCDEFGHIJ"; 
  WE_CHAR           cLabelLen = 1;
  WE_UCHAR          aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN]={0};
  WE_UCHAR          *pucPrfSecret=NULL;
  WE_INT32          iPrfSecretLen=0;
  WE_INT32          iGeneratedOutputLen = 0;
  WE_INT32          iCopyLen=0;
  WE_HANDLE         pvMd5Handle=NULL;
  WE_HANDLE         pvShaHandle=NULL;
  WE_UCHAR          aucDigest[20]={0}; 
  WE_INT32          iILoop=0;
  WE_INT32          iRes=0;
  WE_UCHAR          *pucNextBufPos = NULL;
  WE_INT32          iTmpDigLen=0;
#ifdef TLS_TEST_PRF
  FILE* fin = NULL;
  int i = 0;
  char tmp[48];
  char *j=NULL;
  char name[14]="get_aucms_";
  int index=0;
#endif
  
  if (!pucBuf)
  {
      return M_SEC_ERR_INVALID_PARAMETER;
  }
  pucNextBufPos = pucBuf;
  if (pucSecret == NULL)            
  {
     Sec_WimGetMastSecret(hSecHandle,iMasterSecretId, aucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN, &iRes);
     if (iRes != M_SEC_ERR_OK)
     {
        return iRes;
     }
     pucPrfSecret = aucMasterSecret;
     iPrfSecretLen = M_SEC_TLS_MASTER_SECRET_LEN;
#ifdef TLS_TEST_PRF
     {
         index=iMasterSecretId+49;
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
  } 
  else
  {
     pucPrfSecret = (WE_UCHAR*)pucSecret;
     iPrfSecretLen = iSecretLen;
  }

  while (iGeneratedOutputLen < 160)
  {
    iTmpDigLen=20;
    iRes = Sec_LibHashInit (hSecHandle,E_SEC_ALG_HASH_SHA1,&pvShaHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    iRes = Sec_LibHashUpdate (hSecHandle, aucLabel,cLabelLen,pvShaHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    iRes = Sec_LibHashUpdate (hSecHandle, pucPrfSecret, iPrfSecretLen,pvShaHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    iRes = Sec_LibHashUpdate ( hSecHandle, pucSeed, iSeedLen,pvShaHandle);
    if (iRes != M_SEC_ERR_OK)
    {
        return iRes;
    }
    iRes = Sec_LibHashFinal ( hSecHandle, aucDigest,&iTmpDigLen,pvShaHandle);
    if (iRes != M_SEC_ERR_OK)
    {
       return iRes;
    }
    
    iRes = Sec_LibHashInit (hSecHandle,E_SEC_ALG_HASH_MD5,&pvMd5Handle);
    if (iRes != M_SEC_ERR_OK)
    {
       return iRes;
    }
    iRes = Sec_LibHashUpdate ( hSecHandle,pucPrfSecret, iPrfSecretLen,pvMd5Handle);
    if (iRes != M_SEC_ERR_OK)
    {
       return iRes;
    }
    iRes = Sec_LibHashUpdate ( hSecHandle, aucDigest, iTmpDigLen,pvMd5Handle);
    if (iRes != M_SEC_ERR_OK)
    {
       return iRes;
    }
    iRes = Sec_LibHashFinal (hSecHandle, aucDigest,&iTmpDigLen,pvMd5Handle);
    if (iRes != M_SEC_ERR_OK)
    {
       return iRes;
    }
       
    if ((iOutputLen - iGeneratedOutputLen) >= 16)
    {
       iCopyLen = 16;
    }
    else
    {
        iCopyLen = iOutputLen - iGeneratedOutputLen; 
    }
    for (iILoop=0;iILoop<iCopyLen;iILoop++)
    {
       pucNextBufPos[iILoop] = aucDigest[iILoop];
    }
    iGeneratedOutputLen+=16;
    pucNextBufPos+=16;
    if (iGeneratedOutputLen >= iOutputLen)
    {
       break;
    }
    for (iILoop=0;iILoop<cLabelLen;iILoop++)
    {
       aucLabel[iILoop]++;
    }
    cLabelLen++;
  }
  return iRes;
}
/*====================================================================================
FUNCTION: 
    Sec_WimTLSPRF  
CREATE DATE: 
    2007-1-18
AUTHOR: 
    Tang  
DESCRIPTION:
    Computer TLS PRF.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    WE_INT32 iMasterSecretId[IN]:
    const WE_UCHAR *pucSecret[IN]:
    WE_INT32 iSecretLen[IN]:
    const WE_CHAR *pcLabel[IN]:
    const WE_UCHAR *pucSeed[IN]:
    WE_INT32 iSeedLen[IN]:
    WE_INT32 iOutputLen[IN]:
    WE_UCHAR *pucBuf[OUT]:
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
WE_INT32 Sec_WimTLSPRF(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId,
                      const WE_UCHAR *pucSecret, WE_INT32 iSecretLen, const WE_CHAR *pcLabel, 
                      const WE_UCHAR *pucSeed, WE_INT32 iSeedLen, 
                      WE_INT32 iOutputLen, WE_UCHAR *pucBuf)
{
    WE_INT32        iLabelLen=0;
    WE_UCHAR        *pucLabelAndSeed=NULL;
    WE_UCHAR        aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN]={0};
    WE_INT32        iRes=0;
    WE_INT32        iILoop=0;
    WE_INT32        iJLoop=0;
    WE_UCHAR        *pucS1 = NULL;
    WE_UCHAR        *pucS2 = NULL;
    WE_INT32        iLen = 0;
    WE_UCHAR        *pucMd5Buf=NULL; 
    WE_UCHAR        *pucSha1Buf=NULL;
    
#ifdef TLS_TEST_PRF
    FILE* fin = NULL;
    int i = 0;
    char tmp[48];
    char *j=NULL;
    char name[14]="get_aucms_";
    int index=0;
#endif
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
      "...........Sec_WimTLSPRF (pcLabel =%s )\n",pcLabel ));
    if (!pucBuf||!pcLabel)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    if (pucSecret == NULL)            
    {
        Sec_WimGetMastSecret( hSecHandle,iMasterSecretId, aucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN, &iRes);

        WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
          "...........Sec_WimTLSPRF (iMasterSecretID =%d, iRes = %d )\n",\
          iMasterSecretId, iRes ));
        WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"get aucMasterSecret =\n"));
        WE_LOG_DATA (WE_LOG_DETAIL_MEDIUM, 0, aucMasterSecret, M_SEC_TLS_MASTER_SECRET_LEN);
        WE_LOG_MSG ((WE_LOG_DETAIL_MEDIUM, 0,"\n"));

#ifdef TLS_TEST_PRF
        {
            index=iMasterSecretId+49;
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
        
        if (iRes != M_SEC_ERR_OK)
        {
            return iRes;
        }
        iLen = (M_SEC_TLS_MASTER_SECRET_LEN / 2) + (M_SEC_TLS_MASTER_SECRET_LEN % 2);
    }
    else
    {
        iLen = (iSecretLen / 2) + (iSecretLen % 2);    
    }  
    pucS1 = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iLen*sizeof(WE_UCHAR)+1);
    if (!pucS1)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    pucS2 = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iLen*sizeof(WE_UCHAR)+1);
    if (!pucS2)
    {
        WE_FREE(pucS1);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }    

    if (pucSecret == NULL)                 
    {
        for (iJLoop=0; iJLoop<iLen; iJLoop++)        
        {                                                    
            pucS1[iJLoop] = aucMasterSecret[iJLoop];
            pucS2[iLen-iJLoop-1] = aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN-iJLoop-1];
        }
    }
    else
    {
        for (iJLoop=0; iJLoop<iLen; iJLoop++)        
        {                                                    
            pucS1[iJLoop] = pucSecret[iJLoop];
            pucS2[iLen-iJLoop-1] = pucSecret[iSecretLen-iJLoop-1];
        }
    }
    iLabelLen = (WE_INT32)SEC_STRLEN(pcLabel);
    pucLabelAndSeed = (WE_UCHAR *)WE_MALLOC((WE_UINT32)(iLabelLen + iSeedLen)*sizeof(WE_UCHAR));
    if (!pucLabelAndSeed)
    {
        WE_FREE(pucS2);
        WE_FREE(pucS1);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    for (iILoop=0; iILoop<iLabelLen; iILoop++)
    {
        pucLabelAndSeed[iILoop] = (WE_UCHAR)pcLabel[iILoop];
    }
    for (iILoop=0; iILoop<iSeedLen; iILoop++)
    {
        pucLabelAndSeed[iLabelLen + iILoop] = pucSeed[iILoop];
    }
    pucMd5Buf = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iOutputLen*sizeof(WE_UCHAR)+1);
    pucSha1Buf = (WE_UCHAR *)WE_MALLOC((WE_UINT32)iOutputLen*sizeof(WE_UCHAR)+1);
    if ((!pucMd5Buf) || (!pucSha1Buf))
    {
        if (pucMd5Buf)
        {
            WE_FREE(pucMd5Buf);
        }
        if (pucSha1Buf)
        {
            WE_FREE(pucSha1Buf);
        }
        WE_FREE(pucS2);
        WE_FREE(pucS1);
        WE_FREE(pucLabelAndSeed);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }  
    if ((0 == WE_MEMCMP(pcLabel, "IV block",(WE_UINT32)SEC_STRLEN(pcLabel))))
    {
        Sec_WimPMD5(hSecHandle,NULL, 0, pucLabelAndSeed, 
            (WE_INT32)(iLabelLen + iSeedLen), &iRes, pucMd5Buf, iOutputLen);
        if (iRes != M_SEC_ERR_OK)
        {
            WE_FREE(pucMd5Buf);
            WE_FREE(pucSha1Buf);
            WE_FREE(pucS2);
            WE_FREE(pucS1);
            WE_FREE(pucLabelAndSeed);
            return iRes;
        }
    
        Sec_WimPHash(hSecHandle,NULL, 0,  pucLabelAndSeed, (WE_INT32)(iLabelLen + iSeedLen),&iRes,\
            pucSha1Buf, iOutputLen);
        if (iRes == M_SEC_ERR_OK)
        {    
            for (iJLoop=0; iJLoop<iOutputLen; iJLoop++)        
            {
                pucBuf[iJLoop] = (WE_UCHAR) (pucMd5Buf[iJLoop] ^ pucSha1Buf[iJLoop]);
            }
        }
    }
    else
    {
        Sec_WimPMD5(hSecHandle,pucS1, iLen, pucLabelAndSeed, 
            (WE_INT32)(iLabelLen + iSeedLen), &iRes, pucMd5Buf, iOutputLen);
        if (iRes != M_SEC_ERR_OK)
        {
            WE_FREE(pucMd5Buf);
            WE_FREE(pucSha1Buf);
            WE_FREE(pucS2);
            WE_FREE(pucS1);
            WE_FREE(pucLabelAndSeed);
            return iRes;
        }
    
        Sec_WimPHash(hSecHandle,pucS2, iLen,  pucLabelAndSeed, (WE_INT32)(iLabelLen + iSeedLen),&iRes,\
            pucSha1Buf, iOutputLen);
        if (iRes == M_SEC_ERR_OK)
        {    
            for (iJLoop=0; iJLoop<iOutputLen; iJLoop++)        
            {
                pucBuf[iJLoop] = (WE_UCHAR) (pucMd5Buf[iJLoop] ^ pucSha1Buf[iJLoop]);
            }
        } 
    }
    WE_FREE(pucSha1Buf);
    WE_FREE(pucMd5Buf);
    if (pucLabelAndSeed)
    {
        WE_FREE(pucLabelAndSeed);
    }
    WE_FREE(pucS2);
    WE_FREE(pucS1);
    return iRes;
}


#endif

/*====================================================================================
FUNCTION: 
    Sec_WimAddCACert  
CREATE DATE: 
    2007-1-18
AUTHOR: 
    Tang  
DESCRIPTION:
    add ca cert to list.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    WE_INT32 iTargetID[IN]:the identity of the invoker ,input from the interface.
    WE_HANDLE hCACerts[IN]:pointer to the buffer.
    WE_CHAR cFlag[IN]:flag.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
WE_INT32  Sec_WimAddCACert(WE_HANDLE hSecHandle, WE_INT32 iTargetID,
                                    WE_HANDLE hCACerts, WE_CHAR cFlag)
{
    St_SecCACert *pstCACert = NULL;
    St_SecCACert *pstPrevCACert = NULL;
    St_SecCACert *pstTmp = NULL;
    St_SecCACert *pstTmpI = NULL;
    St_SecCACert *pstInTmp = NULL;
    
    if (NULL == hSecHandle)
    {
       return M_SEC_ERR_INVALID_PARAMETER;
    }    
    
    /* free the former certs. */
    pstTmp = (St_SecCACert *)M_SEC_WIM_CACERT;
    if (NULL != pstTmp)
    {
        if (cFlag != G_SEC_CA_NO_CHANGE)
        {
            while (pstTmp != NULL)
            {
                pstTmpI = pstTmp;
                pstTmp = pstTmp->pstNext;
                WE_FREE(pstTmpI->pucCert);
                pstTmpI->pucCert = NULL;
                WE_FREE(pstTmpI);
                pstTmpI = NULL;
            }    
            M_SEC_WIM_CACERT = NULL;
        }
        else
        {
            return M_SEC_ERR_OK;
        }
    }

    pstInTmp = (St_SecCACert *)hCACerts;
    while (pstInTmp != NULL)
    {
        pstCACert = (St_SecCACert *)WE_MALLOC(sizeof(St_SecCACert));
        if (NULL == pstCACert)
        {
            pstTmp = (St_SecCACert *)M_SEC_WIM_CACERT;
            while (pstTmp != NULL)
            {
                pstTmpI = pstTmp;
                pstTmp = pstTmp->pstNext;
                WE_FREE(pstTmpI->pucCert);
                WE_FREE(pstTmpI);
            }
            return M_SEC_ERR_INSUFFICIENT_MEMORY;
        }
        (void)WE_MEMSET(pstCACert, 0, sizeof(St_SecCACert));
        if (pstPrevCACert != NULL)
        {
            pstPrevCACert->pstNext = pstCACert;  /* add next node. */
        }
        pstCACert->iCertType = pstInTmp->iCertType;
        pstCACert->iCertLen = pstInTmp->iCertLen;
        pstCACert->pucCert = (WE_UCHAR *)WE_MALLOC((WE_ULONG)(pstInTmp->iCertLen) * sizeof(WE_UCHAR));
        if (NULL == pstCACert->pucCert)
        {
            pstTmp = (St_SecCACert *)M_SEC_WIM_CACERT;
            while (pstTmp != NULL)
            {
                pstTmpI = pstTmp;
                pstTmp = pstTmp->pstNext;
                WE_FREE(pstTmpI->pucCert);
                WE_FREE(pstTmpI);
            }
            WE_FREE(pstCACert);
            return M_SEC_ERR_INSUFFICIENT_MEMORY;
        }
        (void)WE_MEMCPY(pstCACert->pucCert, pstInTmp->pucCert, (WE_ULONG)(pstInTmp->iCertLen));
        
        if (NULL == M_SEC_WIM_CACERT)
        {
            M_SEC_WIM_CACERT = (WE_HANDLE)pstCACert;
        }
        pstPrevCACert = pstCACert;
        pstInTmp = pstInTmp->pstNext;
    };
    
    iTargetID = iTargetID;
    return M_SEC_ERR_OK;
}
#endif
/*====================================================================================
FUNCTION: 
    Sec_WimGetTrustedKeyIds  
CREATE DATE: 
    2007-1-18
AUTHOR: 
    Tang  
DESCRIPTION:
    get trust key id.
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle.
    WE_HANDLE hCACerts[IN]:pointer to the buffer.
    WE_HANDLE *phTrustKeyIds[OUT]:pinter to the trust key id.
RETURN VALUE:
    If success, return M_SEC_ERR_OK.Else return error code.
USED GLOBAL VARIABLES:
    None      
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit        
IMPORTANT NOTES:  
    None         
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_WimGetTrustedKeyIds(WE_HANDLE hSecHandle, 
                                    WE_HANDLE hCACerts, WE_HANDLE *phTrustKeyIds)
{
    WE_INT32         iRes = 0;
    WE_UCHAR       *pucRootCert = NULL;
    WE_UINT16       usRootCertLen = 0;
    WE_INT32         iNumAbleRootCerts = 0;
    WE_UINT8         *pucSubjectLen = NULL;
    WE_UCHAR        **ppucSubject = NULL;
    WE_UCHAR        *pucTmpSubject = NULL;
    WE_UINT8          *pucSubjectType = NULL;
    const WE_UINT8 ucX509Name = 1;
    const WE_UINT8 ucWtlsName = 2;
    WE_UINT8       iIndexI = 0;
    WE_INT32       iIndexJ = 0;
    WE_INT32       iIndexK = 0;
    WE_UINT16     usAllocLen = 0;
    WE_INT32       iResult = 0;
    WE_INT32       iCertType = 0;
    WE_UINT16     usTemp = 0;
    WE_UINT8       ucSubjectIdType = 0;
    WE_UINT16     usSubjectcharSet = 0;
    WE_UCHAR     *pucTrustedKeyIds = NULL; /* for CA certificate.*/
    WE_INT32       iIndex = 0;
    St_SecTrustedKeyIds *pstTrustKeyIds = NULL;
        
    if ((NULL == hSecHandle) || (NULL == phTrustKeyIds))
    {
       return M_SEC_ERR_INVALID_PARAMETER;
    }  

    *phTrustKeyIds = NULL;
    
    /* get the number of CA certificate. */
    iRes = Sec_WimGetNbrOfCACerts(hSecHandle, &iNumAbleRootCerts, M_SEC_OUTER, hCACerts);
    if (iRes != M_SEC_ERR_OK)
    {    
        return M_SEC_ERR_INVALID_PARAMETER;
    }  

    if (0 == iNumAbleRootCerts) /* should not be executed.*/
    {
        *phTrustKeyIds = NULL;
        return M_SEC_ERR_OK;
    }
    
    pstTrustKeyIds = (St_SecTrustedKeyIds *)WE_MALLOC(sizeof(WE_UCHAR) * sizeof(St_SecTrustedKeyIds));
    if  (NULL == pstTrustKeyIds)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }         
    (WE_VOID)WE_MEMSET(pstTrustKeyIds, 0, sizeof(St_SecTrustedKeyIds));

    pucSubjectLen = (WE_UINT8*)WE_MALLOC(sizeof(WE_UINT8) * (WE_UINT32)iNumAbleRootCerts);
    pucSubjectType = (WE_UINT8*)WE_MALLOC(sizeof(WE_UINT8) * (WE_UINT32)iNumAbleRootCerts);
    ppucSubject = (WE_UCHAR**)WE_MALLOC(sizeof(WE_UCHAR*) * (WE_UINT32)iNumAbleRootCerts);
    if ((NULL==pucSubjectLen) || (NULL==pucSubjectType) ||  (NULL==ppucSubject))
    {
        if (pucSubjectLen != NULL )
        {
            WE_FREE(pucSubjectLen);
        }
        if (pucSubjectType != NULL )
        {
            WE_FREE(pucSubjectType);
        }
        if (NULL != ppucSubject)
        { 
            WE_FREE(ppucSubject);
        }
        WE_FREE(pstTrustKeyIds);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }      
    /* get subjects of all CA certificates.*/    
    for (iIndexK=0; iIndexK<iNumAbleRootCerts; iIndexK++, iIndexI++)
    {
        iResult = Sec_WimGetCACert(hSecHandle, iIndexK+1, &pucRootCert,
                                     &usRootCertLen, &iCertType, M_SEC_OUTER, hCACerts);
        if (iResult == M_SEC_ERR_OK) 
        {
            if (M_SEC_IWAPOPT_WTLS_ROOT_CERTS == iCertType) /* wtls format */
            {
                iResult = Sec_WtlsGetSubjectInfo(hSecHandle, pucRootCert, &ucSubjectIdType,
                    &usSubjectcharSet, &(pucSubjectLen[iIndexI]), &pucTmpSubject);  
                if (iResult != M_SEC_ERR_OK) 
                {
                    pucSubjectLen[iIndexI] = 0;
                    ppucSubject[iIndexI] = NULL;
                    continue;
                }
                pucSubjectType[iIndexI] = ucWtlsName;
                usAllocLen = (WE_UINT16)(usAllocLen + 6 + pucSubjectLen[iIndexI]);   
            }
            else if (M_SEC_IWAPOPT_X509_ROOT_CERTS == iCertType)  /* x509 formate */
            {
                iResult = Sec_WimGetX509Sub(hSecHandle, pucRootCert, 
                                                    &pucTmpSubject, &(usTemp));  
                if (iResult != M_SEC_ERR_OK) 
                {
                    pucSubjectLen[iIndexI] = 0;
                    ppucSubject[iIndexI] = NULL;
                    continue;
                }
                
                if(usTemp > 255)
                {
                    pucSubjectLen[iIndexI] = 255;
                }
                else
                {
                    pucSubjectLen[iIndexI] = (WE_UINT8)usTemp;
                }
                pucSubjectType[iIndexI] = ucX509Name;
                usAllocLen = (WE_UINT16)(usAllocLen + 4 + pucSubjectLen[iIndexI]); 
            } 
            else /* not wtls or x509 format. */
            {
                for (iIndexJ=0; iIndexJ<iIndexI; iIndexJ++)
                {
                    if (NULL != ppucSubject[iIndexJ])
                    {
                        WE_FREE(ppucSubject[iIndexJ]); 
                    }
                }            
                WE_FREE(ppucSubject);
                WE_FREE(pucSubjectLen);
                WE_FREE(pucSubjectType);
                WE_FREE(pstTrustKeyIds);
                return M_SEC_ERR_UNKNOWN_CERTIFICATE_TYPE;
            }              
            
            ppucSubject[iIndexI] = (WE_UCHAR *)WE_MALLOC(pucSubjectLen[iIndexI] * sizeof(WE_UCHAR));
            if ((NULL == ppucSubject[iIndexI]) && (0 != pucSubjectLen[iIndexI]))
            {  
                for (iIndexJ=0; iIndexJ<iIndexI; iIndexJ++)
                {
                    if (NULL != ppucSubject[iIndexJ])
                    {
                        WE_FREE(ppucSubject[iIndexJ]); 
                    }
                }            
                WE_FREE(ppucSubject);
                WE_FREE(pucSubjectLen);
                WE_FREE(pucSubjectType);
                WE_FREE(pstTrustKeyIds);
                return M_SEC_ERR_INSUFFICIENT_MEMORY;
            }
            for(iIndexJ=0; iIndexJ<pucSubjectLen[iIndexI]; iIndexJ++)
            {
                ppucSubject[iIndexI][iIndexJ] = pucTmpSubject[iIndexJ];
            }  
        }
        else  /* failure to get the root certificate. */
        {
            for (iIndexJ=0; iIndexJ<iIndexI; iIndexJ++)
            {
                if (NULL != ppucSubject[iIndexJ])
                {
                    WE_FREE(ppucSubject[iIndexJ]); 
                }
            }    
            WE_FREE(ppucSubject);
            WE_FREE(pucSubjectLen);
            WE_FREE(pucSubjectType);
            WE_FREE(pstTrustKeyIds);
            return M_SEC_ERR_BAD_CERTIFICATE;
        }
    }
    
    pucTrustedKeyIds = (WE_UCHAR *)WE_MALLOC(sizeof(WE_UCHAR) * usAllocLen); 
    if (NULL == pucTrustedKeyIds)
    {
        for (iIndexJ=0; iIndexJ<iNumAbleRootCerts; iIndexJ++) 
        {
            if (NULL != ppucSubject[iIndexJ])
            {
                WE_FREE(ppucSubject[iIndexJ]); 
            }
        }
        WE_FREE(ppucSubject);
        WE_FREE(pucSubjectLen);
        WE_FREE(pucSubjectType);
        WE_FREE(pstTrustKeyIds);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    
    if (iNumAbleRootCerts <= M_SEC_MAX_NBR_OF_CERTS_IN_TRUSTED_KEY_IDS)
    { 
        iIndex = 0;
        for (iIndexI=0; iIndexI<iNumAbleRootCerts; iIndexI++)
        {
            if (ppucSubject[iIndexI] != NULL)
            {
                if (pucSubjectType[iIndexI] == ucWtlsName)
                {
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)0;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)1;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)0;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)4;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)pucSubjectLen[iIndexI];
                    for (iIndexJ=0; iIndexJ<pucSubjectLen[iIndexI]; iIndexJ++)
                    {
                        pucTrustedKeyIds[iIndex++] = (ppucSubject[iIndexI])[iIndexJ];
                    }
                }
                if (pucSubjectType[iIndexI] == ucX509Name)
                {
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)E_SEC_WTLS_KEYEXCHANGE_RSA;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)0;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)255;
                    pucTrustedKeyIds[iIndex++] = (WE_UCHAR)pucSubjectLen[iIndexI];
                    for (iIndexJ=0; iIndexJ<pucSubjectLen[iIndexI]; iIndexJ++)
                    {
                        pucTrustedKeyIds[iIndex++] = (ppucSubject[iIndexI])[iIndexJ];
                    }
                }
            }
        }
    }
    else
    {
        pstTrustKeyIds->iTooManyCerts = 1;
    }

    pstTrustKeyIds->iTrustKeyIdsLen = iIndex;
    pstTrustKeyIds->pucTrustedKeyIds = pucTrustedKeyIds;
    *phTrustKeyIds = (WE_HANDLE)pstTrustKeyIds;
    
    for (iIndexJ=0; iIndexJ<iNumAbleRootCerts; iIndexJ++)
    {
        if (ppucSubject[iIndexJ] != NULL)
        {
            WE_FREE(ppucSubject[iIndexJ]);
        }
    }
    WE_FREE(ppucSubject);
    WE_FREE(pucSubjectLen);
    WE_FREE(pucSubjectType);    
    return M_SEC_ERR_OK;
}
#endif




