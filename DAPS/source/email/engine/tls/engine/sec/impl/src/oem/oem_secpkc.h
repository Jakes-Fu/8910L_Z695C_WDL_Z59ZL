/*==================================================================================================
    FILE NAME : oem_secpkc.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will gather prototypes of function about RSA operation.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                   Tracking
    Date          Author            Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-12-5     tang            None         
    2007-05-17     tang                            Delete cert manage and signtext
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef _OEM_SEC_PKC_H 
#define _OEM_SEC_PKC_H

#ifdef WE_RSA_FOR_SEC
/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef enum tagE_SecRsaSigType
{
    E_SEC_RSA_PKCS1_NULL = 0,
    E_SEC_RSA_PKCS1_MD2,
    E_SEC_RSA_PKCS1_MD5,
    E_SEC_RSA_PKCS1_SHA1,
    E_SEC_RSA_PKCS1_PSS_SHA1_MGF1
}E_SecRsaSigType;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 Sec_PKCRsaGenerateKeyPair(WE_HANDLE hSecHandle,
                                   WE_INT32 iTargetID,  
                                   WE_INT32 iModBits);
WE_INT32 Sec_PKCRsaVerifySignature(WE_HANDLE hSecHandle,
                                   WE_INT32 iTargetID,
                                   St_SecRsaPublicKey stKey,
                                   WE_UCHAR* pucMsg, 
                                   WE_INT32 iMsgLen, 
                                   E_SecRsaSigType eType,
                                   const WE_UCHAR* pucSig, 
                                   WE_INT32 iSigLen);
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
WE_INT32 Sec_PKCRsaComputeHashedSignature(WE_HANDLE hSecHandle,
                                          WE_INT32 iTargetID,                                       
                                          St_SecCrptPrivKey stKey,
                                          const WE_UCHAR* pucData, 
                                          WE_INT32 iDataLen);
#endif

WE_INT32 Sec_PKCRsaPublicKeyEncryption(WE_HANDLE hSecHandle,
                                       WE_INT32 iTargetID,
                                       St_SecRsaPublicKey stKey, 
                                       const WE_UCHAR* pucData, 
                                       WE_INT32 iDataLen);

#endif /*WE_RSA_FOR_SEC*/

#endif /*_OEM_SEC_PKC_H*/

