/*==================================================================================================
    FILE NAME : oem_secpkc.c
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION:
        This File will gather functions about RSA operation.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                   Tracking
    Date          Author            Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-12-5     tang            None         Draft
    2007-05-17     tang                        Delete cert manage and signtext
    2007-06-28              bluefire           after malloc,memset it
==================================================================================================*/
/***************************************************************************************
    *   Include File Section
****************************************************************************************/

#include "sec_comm.h"
/*#include "we_rsacomm.h"*/



#ifdef WE_RSA_FOR_SEC
/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define    SEC_BREW_ISHELL_HANDLE        (((ISec*)hSecHandle)->m_pIShell)
#define    SEC_BREW_CALLBACK_FNC         (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_cb)
/*==================================================================================================
FUNCTION: 
    Sec_PKCRsaComputeHashedSignature
CREATE DATE:
    2006-12-05
AUTHOR:
    tang
DESCRIPTION:
    USE A RSA PRIVATE KEY STRUCT TO ENCRYPT(SIGN) HASHED DTAA
ARGUMENTS PASSED:
    WE_HANDLE               hSecHandle[IN/OUT]:sec global data
    WE_INT32                iTargetID[IN]:TARGET ID                        
    St_SecCrptPrivKey       stKey[IN]:PRIVATE KEY
    const WE_UCHAR*         pucData[IN]:SRC DATA NEED TO BE SIGNED
    WE_INT32                iDataLen[IN]:SRC DATA LENGTH
RETURN VALUE:
    ERROR CODE
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
WE_INT32 Sec_PKCRsaComputeHashedSignature(WE_HANDLE hSecHandle,
                                          WE_INT32 iTargetID,                                       
                                          St_SecCrptPrivKey stKey,
                                          const WE_UCHAR* pucData, 
                                          WE_INT32 iDataLen)
{
    WE_INT32 iRes = 0;
    St_SecCrptComputeSignatureResp* pstResp = NULL;
    WE_UINT8 aucSign[128]={0};
    WE_INT32 iSignLen=128;

    iRes=SEClib_ComputeRSAHashedSignature( hSecHandle, stKey,pucData,  iDataLen, aucSign,  &iSignLen);

    pstResp = (St_SecCrptComputeSignatureResp*)WE_MALLOC(sizeof(St_SecCrptComputeSignatureResp));
    if(!pstResp)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(pstResp,0,sizeof(St_SecCrptComputeSignatureResp));
    pstResp->iTargetID = iTargetID;
    if(SUCCESS ==iRes)
    { 
        pstResp->sSigLen = (WE_INT16)iSignLen;
        pstResp->pucSig = (WE_UCHAR*)WE_MALLOC((WE_UINT16)pstResp->sSigLen*sizeof(WE_UCHAR));
        if(!pstResp->pucSig)
        {
           pstResp->sResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
        }
        else
        {
           (void)WE_MEMCPY(pstResp->pucSig,aucSign,(WE_UINT16)pstResp->sSigLen);
           pstResp->sResult = M_SEC_ERR_OK;
        }
    }
    else
    {
        pstResp->sResult = (WE_INT16)iRes;
    }
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_COMPUTE_SIGN_RESP, (WE_VOID*)pstResp);
    if(iRes !=M_SEC_ERR_OK)
    {
        M_SEC_SELF_FREE(pstResp->pucSig);
        WE_FREE(pstResp);
        pstResp = NULL;
        return iRes;
    }
    /*delete linux by Bird 070327*/
 //   SEC_REGCALLBACK(NULL,NULL,Sec_RunMainFlow,hSecHandle);
    return iRes;     
}
#endif
/*==================================================================================================
FUNCTION: 
    Sec_PKCRsaVerifySignature
CREATE DATE:
    2006-12-05
AUTHOR:
    tang
DESCRIPTION:
    USE A RSA PUBLIC KEY STRUCT TO DECRYPT DATA
ARGUMENTS PASSED:
    WE_HANDLE               hSecHandle[IN/OUT]:sec global data
    WE_INT32                iTargetID[IN/OUT]:TARGET ID
    St_SecRsaPublicKey      stKey[IN]:RSA PUBLIC KEY STRUCT
    WE_UCHAR*               pucMsg[IN]:MSG NEED TO VERIFIED
    WE_INT32                iMsgLen[IN]:MSG LENGTH
    E_SecRsaSigType         eType[IN]:SIGN TYPE
    const WE_UCHAR*         pucSig[IN]:SIG USED TO VERIFY
    WE_INT32                iSigLen[IN]:SIG LENGTH
RETURN VALUE:
ERROR CODE
USED GLOBAL VARIABLES:
None
USED STATIC VARIABLES:
None
CALL BY:
Omit
IMPORTANT NOTES:
None
==================================================================================================*/
WE_INT32 Sec_PKCRsaVerifySignature(WE_HANDLE hSecHandle,
                                   WE_INT32 iTargetID,
                                   St_SecRsaPublicKey stKey,
                                   WE_UCHAR* pucMsg, 
                                   WE_INT32 iMsgLen, 
                                   E_SecRsaSigType eType,
                                   const WE_UCHAR* pucSig, 
                                   WE_INT32 iSigLen)
{
    St_SecCrptVerifySignatureResp* pstResp = NULL;
    WE_INT32 iRes=0;    

    iRes= SEClib_VerifyRSASignature( hSecHandle, stKey,  pucMsg,  iMsgLen, (E_WeRsaSigType)eType, 	pucSig,  iSigLen);
    pstResp = (St_SecCrptVerifySignatureResp*)WE_MALLOC(sizeof(St_SecCrptVerifySignatureResp));
    if(!pstResp)
    {
       return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(pstResp,0,sizeof(St_SecCrptVerifySignatureResp));
    pstResp->iTargetID = iTargetID;
    if(SUCCESS == iRes)
    {       
       pstResp->sResult =(WE_INT16)iRes;
    }
    else
    {
        pstResp->sResult = (WE_INT16)iRes;
    }
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_VERIFY_SIGN_RESP, (WE_VOID*)pstResp);
    if(iRes !=M_SEC_ERR_OK)
    {
        WE_FREE(pstResp);
        pstResp = NULL;
        return iRes;
    }
    /*call back*/
    /*delete  linux by Bird 070323*/
  //  SEC_REGCALLBACK(NULL,NULL,Sec_RunMainFlow,hSecHandle);
    return iRes;    
}
/*==================================================================================================
FUNCTION: 
    Sec_PKCRsaPublicKeyEncryption
CREATE DATE:
    2006-12-05
AUTHOR:
    tang
DESCRIPTION:
    USE A RSA PUBLIC KEY STRUCT TO ENCRYPT DATA
    ARGUMENTS PASSED:
    WE_HANDLE               hSecHandle[IN/OUT]:sec global data
    WE_INT32                iTargetID[IN]:TARGET ID
    St_SecRsaPublicKey      stKey[IN]:RSA PUBLIC KEY
    const WE_UCHAR*         pucData[IN]:SRC DATA NEED TO BE ENCRYPTED
    WE_INT32                iDataLen[IN]:SRC DATA
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_PKCRsaPublicKeyEncryption(WE_HANDLE hSecHandle,
                                       WE_INT32 iTargetID,
                                       St_SecRsaPublicKey stKey, 
                                       const WE_UCHAR* pucData, 
                                       WE_INT32 iDataLen)
{
    St_SecCrptEncryptPkcResp* pstResp = NULL;
    WE_INT32 iBufLen=0;
    WE_UINT8 *pucBuf=NULL;
    WE_INT32 iRes=0;
    if(stKey.usModLen>128)
    {
        iBufLen=(2+iDataLen/128)*128;
    }
    else
    {
        iBufLen=(1+iDataLen/128)*128;
        
    }
    pucBuf=(WE_UINT8 *)WE_MALLOC((WE_ULONG)iBufLen*sizeof(WE_UINT8));

    iRes= SEClib_RsaPublicKeyEncryption(hSecHandle, stKey, pucData, iDataLen, pucBuf, &iBufLen);

    pstResp = (St_SecCrptEncryptPkcResp*)WE_MALLOC(sizeof(St_SecCrptEncryptPkcResp));
    if(!pstResp)
    {
        WE_FREE(pucBuf);
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    pstResp->iTargetID = iTargetID;
    if(SUCCESS == iRes)
    {
        pstResp->sBufLen = (WE_INT16)iBufLen;
        pstResp->pucBuf = (WE_UCHAR*)WE_MALLOC((WE_UINT16)pstResp->sBufLen*sizeof(WE_UCHAR));
        if(!pstResp->pucBuf)
        {
            pstResp->sResult = M_SEC_ERR_INSUFFICIENT_MEMORY;
        }
        else
        {
            (void)WE_MEMCPY(pstResp->pucBuf,pucBuf, (WE_UINT16)pstResp->sBufLen);
            pstResp->sResult = M_SEC_ERR_OK;
        }        
    }
    else
    {
        pstResp->sResult = (WE_INT16)iRes;
    }
    WE_FREE(pucBuf);
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_ENCRYPT_PKC_RESP, (WE_VOID*)pstResp);
    if(iRes !=M_SEC_ERR_OK)
    {
        WE_FREE(pucBuf);
        M_SEC_SELF_FREE(pstResp->pucBuf );
        WE_FREE(pstResp);
        pstResp =NULL;
        return iRes;
    }
        /*call back*/
    /*modify linux by Bird 070313*/
    //SEC_REGCALLBACK(NULL,NULL,Sec_RunMainFlow,hSecHandle);
    return iRes;    
}

#endif /*WE_RSA_FOR_SEC*/

