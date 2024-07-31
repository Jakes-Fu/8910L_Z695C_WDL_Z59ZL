/*=====================================================================================
    FILE NAME : isecw.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:       
        In this file,define the interface function prototype of sec module.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author         Number                  Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-07              Kevin Yang        None         Draft
    2007-05-17              tang                           Delete cert manage and signtext
    2007-06-28              bluefire                       after malloc,memset it
=====================================================================================*/

/*=====================================================================================
*   Include File Section
*=====================================================================================*/
#include "sec_comm.h" 
#include "oem_secmgr.h"
#include "isecw.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#define SEC_IAEECALLBACK_VAR        (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->m_cb)
#define SEC_IISHELL_POINTER         (((ISec*)pMe)->m_pIShell)
#define SEC_PRIVATE_DATA            (((ISec*)pMe)->hPrivateData)
#define SEC_SIINFO                  (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstInfo)
#define SEC_PEER_CACHE              (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstPeerCache)
#define SEC_SESSION_CACHE           (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstSessionCache)
#define SEC_WIM_HAVELOAD        (((St_PublicData *)(((ISec *)pMe)->hPrivateData))->pcHaveLoad)
#define SEC_WIM_CACERT             (((St_PublicData *)(((ISec *)pMe)->hPrivateData))->hCACert)

/*******************************************************************************
*   Function Define Section
*******************************************************************************/

/*==================================================================================================
FUNCTION: 
    ISecW_MakeCipherSuite
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    The function provides the information for wtls about the bulk cipher algorithm
    key exchange method and the trusted ca information.
ARGUMENTS PASSED:
    WE_HANDLE      pMe[IN]: pointer of ISec instance
    WE_INT32       iTargetID[IN]: the identity of the invoker ,input from the interface.
    WE_UINT8       ucConnType[IN]:the type of connection:wtls,tls or ssl
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_MakeCipherSuite (WE_HANDLE pMe, WE_INT32 iTargetID,WE_UINT8   ucConnType ,WE_HANDLE hCACerts)
{
   WE_INT32 iResult = 0;
   WE_HANDLE  hTrustKeyIds = NULL;
 
   if (NULL == pMe)
   {
      return G_SEC_INVALID_PARAMETER;
   }
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_MakeCipherSuite()\n"));
#ifdef M_SEC_CFG_WTLS
   if (M_SEC_WTLS_CONNECTION_TYPE == ucConnType)
   {
       iResult = Sec_WimGetTrustedKeyIds((WE_HANDLE)pMe, hCACerts, &hTrustKeyIds);
       if (M_SEC_ERR_OK != iResult)
       {
           return     G_SEC_NOTENOUGH_MEMORY ;
       }
       iResult = Sec_MsgWtlsGetCipherSuite((WE_HANDLE)pMe,iTargetID,hTrustKeyIds);
   }
   else
#endif

#ifdef M_SEC_CFG_TLS
   if (M_SEC_TLS_CONNECTION_TYPE == ucConnType)
   {      
      iResult = Sec_MsgTlsGetCipherSuite((WE_HANDLE)pMe,iTargetID);
   }
   else
#endif
   {
      return G_SEC_INVALID_PARAMETER;
   }
   
   if (M_SEC_ERR_OK != iResult)
   {
      return G_SEC_NOTENOUGH_MEMORY; 
   }

   Sec_RunMainFlow(pMe);
   return G_SEC_OK;
}
/*==================================================================================================
FUNCTION: 
    ISecW_ProcAsyAlgorithm
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    the interface will provide the function about the master secret key.
ARGUMENTS PASSED:
    ISecW *        pMe[IN]: pointer of ISecW instance.
    WE_INT32       iObjectId[IN]: the identity of the invoker ,input from the interface..
    WE_UINT8       ucAlg[IN]:Wtls:The value of the hash Algorithm.
                             Tls:The Algorithm of key Exchange.
    WE_VOID *      pvParam[IN]:if the type of connection is wtls,it is point to the structure St_SecWtlsKeyExchParams,which includs the public key and key exchange method.
                   if the type of connection is tls,which point to the structure of St_SecTlsKeyExchangeParams
    WE_UCHAR *     pucRandval[IN]:random data to create the master secret,if the type of connetion is wtls, 
                   it's length is 32;else if tls ,it is 64(rfc2246).
    WE_UINT8       ucConnType[IN]:the type of connection:wtls,ssl or tls   
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
 WE_UINT32 ISecW_ProcAsyAlgorithm(WE_HANDLE pMe, WE_INT32 iTargetID, WE_VOID *pvParam,
                                 WE_UINT8 ucAlg,WE_UCHAR * pucRandval,WE_UINT8 ucConnType)
{
   WE_INT32 iResult = 0;
    
   if (NULL == pMe)
   {
      return G_SEC_INVALID_PARAMETER;
   } 
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_ProcAsyAlgorithm()\n"));
#ifdef M_SEC_CFG_WTLS
   if (M_SEC_WTLS_CONNECTION_TYPE==ucConnType)
   {
      iResult = Sec_MsgWtlsKeyExchange ((WE_HANDLE)pMe,iTargetID,pvParam,ucAlg,pucRandval);
   }
   else
#endif  

#ifdef M_SEC_CFG_TLS
   if (M_SEC_TLS_CONNECTION_TYPE==ucConnType)
   {      
      iResult = Sec_MsgTlsKeyExchange ((WE_HANDLE)pMe,iTargetID,pvParam,ucAlg,pucRandval);
   }
   else
#endif   
   {
      return G_SEC_INVALID_PARAMETER;
   }
   if (M_SEC_ERR_OK != iResult)
   {
      return G_SEC_NOTENOUGH_MEMORY; 
   }  

   Sec_RunMainFlow(pMe);
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............exit ISecW_ProcAsyAlgorithm()\n"));
   return G_SEC_OK;
}
/*==================================================================================================
FUNCTION: 
    ISecW_ComputePrf
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Get the Pseudo Random Function.If the master secret id is 
    to be used, "pucSecret" is NULL, and "iMasterSecretId" 
    denotes which master secret is to use. Or,"pucSecret" must be provided.
ARGUMENTS PASSED:
    WE_HANDLE     pMe[IN]: pointer of ISec instance.
    WE_INT32    iTargetID[IN]: the identity of the invoker ,input from the interface.
    WE_UINT8    ucAlg[IN]:The value of the Algorithm.
    WE_INT32    iMasterSecretId[IN]:ID of master secret.
    WE_INT32    iSecretLen[IN]:Length of the secret.
    WE_INT32    iSeedLen[IN]:Length of the seed.
    WE_INT32    iOutputLen[IN]:Length of the output.
    WE_UCHAR *  pucSecret[IN]:Pointer to the secret.
    WE_CHAR  *  pcLabel[IN]:Pointer to lable.
    WE_UCHAR *  pucSeed[IN]:Pointer to seed.
    WE_UINT8    ucConnType[IN]:the type of connection:wtls,ssl or tls   
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_ComputePrf(WE_HANDLE pMe, WE_INT32 iTargetID, WE_UINT8 ucAlg, 
                                        WE_INT32 iMasterSecretId,  WE_UCHAR * pucSecret, 
                                        WE_INT32 iSecretLen,  WE_CHAR * pcLabel, 
                                         WE_UCHAR * pucSeed, WE_INT32 iSeedLen,
                                       WE_INT32 iOutputLen,WE_UINT8 ucConnType)
{
   WE_UCHAR * pucSecretMem = NULL;
   WE_CHAR  * pcLableMem = NULL;
   WE_UCHAR * pucSeedMem = NULL;
   WE_INT32   iResult = 0;

   if (NULL == pMe)
   {
      return G_SEC_INVALID_PARAMETER;
   }
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
             "..............into ISecW_PeerOpts()\n"));

#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))

   if (NULL != pucSecret && iSecretLen > 0)
   {
      pucSecretMem = WE_MALLOC((WE_ULONG)iSecretLen);
      if (NULL == pucSecretMem)
      {
         return G_SEC_NOTENOUGH_MEMORY;
      }

      (void)WE_MEMCPY(pucSecretMem,pucSecret,(WE_UINT32)iSecretLen);
   }
   if (NULL != pucSeed && iSeedLen > 0)
   {
      pucSeedMem = WE_MALLOC((WE_ULONG)iSeedLen);
      if (!pucSeedMem)
      {
         if (NULL != pucSecretMem)
         {
            WE_FREE(pucSecretMem);
         }
         return G_SEC_NOTENOUGH_MEMORY;
      }
      (void)WE_MEMCPY(pucSeedMem, pucSeed, (WE_UINT32)iSeedLen);
   }
   if (NULL != pcLabel)
   {
      pcLableMem = (WE_CHAR  * )SEC_STRDUP(pcLabel);
      if (NULL == pcLableMem)
      {
         if (NULL != pucSecretMem)
         {
            WE_FREE(pucSecretMem);
         }
         if (NULL != pucSeedMem)
         {
            WE_FREE(pucSeedMem);
         }
         return G_SEC_NOTENOUGH_MEMORY;
      }
   }
#else
   return G_SEC_INVALID_PARAMETER;
#endif

#ifdef M_SEC_CFG_WTLS
   if (M_SEC_WTLS_CONNECTION_TYPE==ucConnType)
   {
      iResult = Sec_MsgWtlsGetPrfResult ((WE_HANDLE)pMe,iTargetID,ucAlg,iMasterSecretId,pucSecretMem,
                                     iSecretLen,pcLableMem,pucSeedMem,iSeedLen,iOutputLen);
   }
   else
#endif    
#ifdef M_SEC_CFG_TLS
   if (M_SEC_TLS_CONNECTION_TYPE==ucConnType)
   {      
      iResult = Sec_MsgTlsGetPrfResult ((WE_HANDLE)pMe,iTargetID,ucAlg,iMasterSecretId,pucSecretMem,
                                     iSecretLen,pcLableMem,pucSeedMem,iSeedLen,iOutputLen);
   }
   else
#endif   
   {
      if (NULL != pcLableMem)
      {
         WE_FREE(pcLableMem);
      }
      return G_SEC_INVALID_PARAMETER;
   }


   if (M_SEC_ERR_OK != iResult)
   {
      if (NULL != pucSecretMem)
      {
         WE_FREE(pucSecretMem);
      }
      if (NULL != pucSeedMem)
      {
         WE_FREE(pucSeedMem);
      }
      if (NULL != pcLableMem)
      {
         WE_FREE(pcLableMem);
      }
      return G_SEC_NOTENOUGH_MEMORY;  
   }

   Sec_RunMainFlow(pMe);
   return G_SEC_OK;
}
/*==================================================================================================
FUNCTION: 
    ISecW_ValidateSvrCert
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    the interface will provide the function to verify the certificate chain from the gateway.
ARGUMENTS PASSED:
    WE_HANDLE      pMe[IN]: pointer of ISec instance.
    WE_INT32    iTargetID[IN]: the identity of the invoker ,input from the interface.
    WE_UCHAR *  pvBuf[IN]:Pointer to the parameter.
    WE_UCHAR *  iBufLen[IN]:length of the buffer.
    WE_UINT8    ucConnType[IN]:the type of connection:wtls,ssl or tls   
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_ValidateSvrCert(WE_HANDLE pMe, WE_INT32 iTargetID,
                                          WE_VOID *pvBuf,  WE_INT32 iBufLen,
                                          WE_HANDLE hCACerts, WE_CHAR cFlag ,
                                          WE_UINT8 ucConnType)
{
   WE_INT32   iResult = 0;

   if (NULL == pMe||NULL==pvBuf)
   {
      return G_SEC_INVALID_PARAMETER;
   } 
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
             "..............into ISecW_ValidateSvrCert()\n"));  
#if (defined ( M_SEC_CFG_WTLS ) || defined (M_SEC_CFG_TLS))
   iResult = Sec_WimAddCACert((WE_HANDLE)pMe, iTargetID, hCACerts, cFlag);
   if (iResult != M_SEC_ERR_OK)
   {
        return G_SEC_INVALID_PARAMETER;
   }
#else
   return G_SEC_INVALID_PARAMETER;
#endif

#ifdef M_SEC_CFG_WTLS
   if (M_SEC_WTLS_CONNECTION_TYPE == ucConnType)
   {
      iResult = Sec_MsgWtlsVerifySvrCert ((WE_HANDLE)pMe,iTargetID,pvBuf,iBufLen);
   }
   else
#endif
#ifdef M_SEC_CFG_TLS
   if (M_SEC_TLS_CONNECTION_TYPE == ucConnType)
   {      
      iResult = Sec_MsgTlsVerifyCertChain ((WE_HANDLE)pMe,iTargetID,pvBuf,iBufLen);
   }
   else
#endif   
   {
      return G_SEC_INVALID_PARAMETER;
   }

   if (M_SEC_ERR_OK != iResult)
   {
      return G_SEC_NOTENOUGH_MEMORY;
   }

   Sec_RunMainFlow(pMe);
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
             "..............exit ISecW_ValidateSvrCert()\n"));  
   return G_SEC_OK;
}
/*==================================================================================================
FUNCTION: 
    ISecW_CompSign
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    compute the signature for wtls.
ARGUMENTS PASSED:
    WE_HANDLE       pMe[IN]: pointer of ISec instance.
    WE_INT32      iTargetID[IN]: the identity of the invoker ,input from the interface.
    WE_UCHAR *    pucKeyId[IN]:Pointer to the key id .
    WE_INT32      iKeyIdLen[IN]:Length of the key id.
    WE_UCHAR *    pucBuf[IN]:Pointer to the buffer.
    WE_INT32      iBufLen[IN]:Length of the buffer.
    WE_UINT8   ucConnType[IN]:The type of connection:wtls,tls or ssl.   
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK.
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
WE_UINT32 ISecW_CompSign(WE_HANDLE pMe, WE_INT32 iTargetID,
                               WE_UCHAR * pucKeyId, WE_INT32 iKeyIdLen,
                               WE_UCHAR * pucBuf, WE_INT32 iBufLen)
{
   WE_UCHAR * pucKeyIdMem = NULL;
   WE_UCHAR * pucBufMem = NULL;
   WE_INT32   iResult = 0;

   if (NULL == pMe)
   {
      return G_SEC_INVALID_PARAMETER;
   }    
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
             "..............into ISecW_CompSign()\n"));

   if (NULL != pucKeyId && iKeyIdLen > 0)
   {
      pucKeyIdMem = WE_MALLOC((WE_ULONG)iKeyIdLen);
      if (NULL == pucKeyIdMem)
      {
         return G_SEC_NOTENOUGH_MEMORY;
      }
      (void)WE_MEMCPY(pucKeyIdMem,pucKeyId,(WE_UINT32)iKeyIdLen);
   }
   if (NULL != pucBuf && iBufLen > 0)
   {
      pucBufMem = WE_MALLOC((WE_ULONG)iBufLen);
      if (NULL == pucBufMem)
      {
         if (NULL != pucKeyIdMem)
         {
            WE_FREE(pucKeyIdMem);
         }
         return G_SEC_NOTENOUGH_MEMORY;
      }
      (void)WE_MEMCPY(pucBufMem,pucBuf,(WE_UINT32)iBufLen);
   }
   iResult = Sec_MsgCompSign((WE_HANDLE)pMe,iTargetID,pucKeyIdMem,iKeyIdLen,pucBufMem,iBufLen,M_SEC_SP_RSA);
   if (M_SEC_ERR_OK != iResult)
   {
      if (NULL != pucKeyIdMem)
      {
         WE_FREE(pucKeyIdMem);
      }
      if (NULL != pucBufMem)
      {
         WE_FREE(pucBufMem);
      }
      return G_SEC_NOTENOUGH_MEMORY;  
   }

   Sec_RunMainFlow(pMe);
   return G_SEC_OK;
}
#endif
/*==================================================================================================
FUNCTION: 
    ISecW_ConnectionOpts
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    stop session connection.
ARGUMENTS PASSED:
    ISecW *         pMe[IN]: pointer of ISecW instance.
    WE_INT32        iTargetID[IN]: the identity of the invoker ,input from the interface.
    WE_INT32        iSecurityId[IN]:ID of the security.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    AEE_SUCCESS.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_ConnectionOpts(WE_HANDLE pMe, St_SecConnectOpt *pstConnectOpt ,WE_UINT8 ucConnType)
{
    WE_INT32   iResult = 0;
    
    if ((NULL == pMe) || (NULL == pstConnectOpt))
    {
        return G_SEC_INVALID_PARAMETER;
    }
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_ConnectionOpts()\n"));  
    
    switch (pstConnectOpt->eType)
    {
    case G_SEC_SETUP_CONNECTION:
        {   
            St_SecSessionInfo           stInfo = {0};
            St_SecAsn1Certificate      *pstCert = NULL;
            St_SecSetupConnectPara *pstSecSetupConnectPara = (St_SecSetupConnectPara *)(pstConnectOpt->hParaValue);
            if (NULL == pstSecSetupConnectPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            
            if(pstSecSetupConnectPara->pstSessionInfo->ucNumCerts > 0)
            {
                pstCert = WE_MALLOC((WE_ULONG)sizeof(St_SecAsn1Certificate));
                if(NULL == pstCert)
                {
                    return G_SEC_NOTENOUGH_MEMORY;
                }
                (void)WE_MEMSET(pstCert,0,sizeof(St_SecAsn1Certificate));
                pstCert->ucFormat = pstSecSetupConnectPara->pstSessionInfo->ucCertFormat;
                pstCert->uiCertLen = pstSecSetupConnectPara->pstSessionInfo->uiCertLen;
                if(pstSecSetupConnectPara->pstSessionInfo->uiCertLen > (WE_UINT32)0 && NULL != pstSecSetupConnectPara->pstSessionInfo->pucCert)
                {    
                    pstCert->pucCert = WE_MALLOC((WE_ULONG)pstSecSetupConnectPara->pstSessionInfo->uiCertLen);
                    if(NULL == pstCert->pucCert)
                    {          
                        WE_FREE(pstCert);
                        return G_SEC_NOTENOUGH_MEMORY;
                    }
                    (void)WE_MEMCPY(pstCert->pucCert,pstSecSetupConnectPara->pstSessionInfo->pucCert,pstSecSetupConnectPara->pstSessionInfo->uiCertLen);
                }
                else
                {
                    pstCert->pucCert = NULL;
                }
            }
            stInfo.ucConnectionType = ucConnType;
            stInfo.ucSecurityClass = pstSecSetupConnectPara->pstSessionInfo->ucSecurityClass ;
            stInfo.ucKeyExchangeAlg = pstSecSetupConnectPara->pstSessionInfo->ucKeyExchangeAlg;
            stInfo.usKeyExchangeKeyLen = pstSecSetupConnectPara->pstSessionInfo->usKeyExchangeKeyLen;
            stInfo.ucHmacAlg = pstSecSetupConnectPara->pstSessionInfo->ucHmacAlg;
            stInfo.usHmacLen = pstSecSetupConnectPara->pstSessionInfo->usHmacLen;
            stInfo.ucEncryptionAlg = pstSecSetupConnectPara->pstSessionInfo->ucEncryptionAlg;
            stInfo.usEncryptionKeyLen = pstSecSetupConnectPara->pstSessionInfo->usEncryptionKeyLen;
            stInfo.ucNumCerts = pstSecSetupConnectPara->pstSessionInfo->ucNumCerts;
            stInfo.pstCerts = pstCert;
            
            iResult = Sec_MsgSetupCon((WE_HANDLE)pMe, pstSecSetupConnectPara->iTargetID, 
                pstSecSetupConnectPara->iMasterSecretId, pstSecSetupConnectPara->iSecId,
                pstSecSetupConnectPara->iFullHandshake, stInfo); 
            if(M_SEC_ERR_OK != iResult)
            {
                if (NULL != pstCert)
                {
                    if(NULL != pstCert->pucCert)
                    {
                        WE_FREE(pstCert->pucCert);
                    }       
                    WE_FREE(pstCert);
                }
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    case G_SEC_STOP_CONNECTION:
        {   
            St_SecStopConnectPara *pstSecStopPara = (St_SecStopConnectPara *)(pstConnectOpt->hParaValue);
            if (NULL == pstSecStopPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            
            iResult = Sec_MsgStopCon((WE_HANDLE)pMe, pstSecStopPara->iTargetID,
                pstSecStopPara->iSecId, (WE_UCHAR)ucConnType);
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    default:
        return G_SEC_INVALID_PARAMETER;            
    }
    
    Sec_RunMainFlow(pMe);
    return G_SEC_OK;

}

/*==================================================================================================
FUNCTION: 
    SecW_ClearPeer
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    remove peer from a session slot.
ARGUMENTS PASSED:
    ISecW *         pMe[IN]: pointer of ISecW instance.
    WE_INT32        iMasterSecretId[IN]:ID of the master secret.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    AEE_SUCCESS.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_PeerOpts(WE_HANDLE pMe, St_SecPeerOpt *pstPeerOpt ,WE_UINT8   ucConnType)
{
    WE_INT32   iResult = 0;
    
    if ((NULL == pMe) || (NULL == pstPeerOpt))
    {
        return G_SEC_INVALID_PARAMETER;
    }
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_PeerOpts()\n"));
    
    switch (pstPeerOpt->eType)
    {
    case G_SEC_REMOVE_PEER:
        {
            WE_INT32   iTargetId = 0;    
            St_SecRemovePeerPara *pstRemovePeerPara = (St_SecRemovePeerPara *)(pstPeerOpt->hValue);
            
            if (NULL == pstRemovePeerPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            iResult = Sec_MsgRemovePeer((WE_HANDLE)pMe, iTargetId, pstRemovePeerPara->iMasterSecretId);            
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY;
            }
        }
        break;
    case G_SEC_ATTACH_PEER:
        {
            WE_UCHAR * pucAddressMem = NULL;
            St_SecAttachPeerPara *pstAttachPeerPara = (St_SecAttachPeerPara *)(pstPeerOpt->hValue);
            
            if (NULL == pstAttachPeerPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }          
            if ((NULL != pstAttachPeerPara->pucAddress) && (pstAttachPeerPara->iAddressLen > 0))
            {
                pucAddressMem = WE_MALLOC((WE_ULONG)pstAttachPeerPara->iAddressLen);
                if (NULL == pucAddressMem)
                {
                    return G_SEC_NOTENOUGH_MEMORY;
                }
                (void)WE_MEMCPY(pucAddressMem, pstAttachPeerPara->pucAddress, (WE_UINT32)pstAttachPeerPara->iAddressLen);
            }
            
            iResult = Sec_MsgAttachPeerToSession((WE_HANDLE)pMe, 
                (WE_UCHAR)ucConnType, pucAddressMem,
                pstAttachPeerPara->iAddressLen, NULL, 
                pstAttachPeerPara->usPortnum, pstAttachPeerPara->iMasterSecretId);
            
            if (M_SEC_ERR_OK != iResult)
            {
                if (NULL != pucAddressMem)
                {
                    WE_FREE(pucAddressMem);
                }
                return G_SEC_NOTENOUGH_MEMORY;  
            }
        }
        break;
    case G_SEC_SEARCH_PEER:
        {
            WE_UCHAR * pucAddressMem = NULL;
            St_SecSearchPeerPara *pstSearchPeerPara = (St_SecSearchPeerPara *)(pstPeerOpt->hValue);
            
            if (NULL == pstSearchPeerPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            if ((NULL != pstSearchPeerPara->pucAddress) && (pstSearchPeerPara->iAddressLen > 0))
            {
                pucAddressMem = (WE_UCHAR*)WE_MALLOC((WE_ULONG)pstSearchPeerPara->iAddressLen);
                if (NULL == pucAddressMem)
                {
                    return G_SEC_NOTENOUGH_MEMORY;
                }
                (void)WE_MEMCPY(pucAddressMem, pstSearchPeerPara->pucAddress, 
                    (WE_UINT32)(pstSearchPeerPara->iAddressLen));
            } 
            
            iResult = Sec_MsgSearchPeer((WE_HANDLE)pMe, pstSearchPeerPara->iTargetID, 
                (WE_UCHAR)ucConnType, pucAddressMem, 
                pstSearchPeerPara->iAddressLen, NULL, pstSearchPeerPara->usPortNum); 
            if (M_SEC_ERR_OK != iResult)
            {
                WE_FREE(pucAddressMem); 
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    default:
        return G_SEC_INVALID_PARAMETER;
    }
    
    Sec_RunMainFlow(pMe);
    return G_SEC_OK;

}

/*==================================================================================================
FUNCTION: 
    ISecW_SessionOpts
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    enable a session connection for wtls.
ARGUMENTS PASSED:
    ISecW *     pMe[IN]: pointer of ISecW instance.
    WE_INT32    iMasterSecretId[IN]:ID of the master secret.
    WE_UINT8    ucIsActive[IN]:flg of the active.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    AEE_SUCCESS.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_SessionOpts(WE_HANDLE pMe, St_SecSessionOpt *pstSessionOpt )
{
    WE_INT32   iResult = 0;
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_SessionOpts()\n"));  
    
    if ((NULL == pMe) || (NULL == pstSessionOpt))
    {
        return G_SEC_INVALID_PARAMETER;
    }

    switch (pstSessionOpt->eType)
    {
    case G_SEC_ENABLE_SESSION:
        {   
            St_SecEnableSessionPara *pstEnableSessionPara = (St_SecEnableSessionPara *)(pstSessionOpt->hValue);
            
            if (NULL == pstEnableSessionPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            iResult = Sec_MsgEnableSession((WE_HANDLE)pMe, pstEnableSessionPara->iMasterSecretId, 
                pstEnableSessionPara->ucIsActive);                
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            } 
        }
        break;
    case G_SEC_DISABLE_SESSION:
        {   
            St_SecDisableSessionPara *pstDisableSessionPara = (St_SecDisableSessionPara *)(pstSessionOpt->hValue);
            
            if (NULL == pstDisableSessionPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            iResult = Sec_MsgDisableSession((WE_HANDLE)pMe, pstDisableSessionPara->iMasterSecretId);                
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            }                
        }
        break;
    case G_SEC_GET_SESSION:
        {   
            St_SecSessionGetPara *pstSessionGetPara = (St_SecSessionGetPara *)(pstSessionOpt->hValue);
            
            if (NULL == pstSessionGetPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }      
            iResult = Sec_MsgGetSessionInfo((WE_HANDLE)pMe, pstSessionGetPara->iTargetID, 
                pstSessionGetPara->iMasterSecretId);               
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            } 
            
        }
        break;
    case G_SEC_RENEW_SESSION:
        {   
            WE_UCHAR * pucSessionIdMem = NULL;
            WE_UCHAR * pucPrivateKeyIdMem = NULL;
            St_SecSessionRenewPara *pstSessionRenewPara = (St_SecSessionRenewPara *)(pstSessionOpt->hValue);
            
            if (NULL == pstSessionRenewPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }   
            if ((NULL != pstSessionRenewPara->pucSessionId) && (pstSessionRenewPara->ucSessionIdLen > 0))
            {
                pucSessionIdMem = WE_MALLOC((WE_ULONG)pstSessionRenewPara->ucSessionIdLen);
                if (NULL == pucSessionIdMem)
                {
                    return G_SEC_NOTENOUGH_MEMORY;
                }
                (void)WE_MEMCPY(pucSessionIdMem, pstSessionRenewPara->pucSessionId, (WE_UINT32)pstSessionRenewPara->ucSessionIdLen);
            }
            if (NULL != pstSessionRenewPara->pucPrivateKeyId)
            {
                pucPrivateKeyIdMem = WE_MALLOC((WE_ULONG)M_SEC_PRIVATE_KEY_ID_LEN);
                if (NULL == pucPrivateKeyIdMem)
                {
                    if (NULL != pucSessionIdMem)
                    {
                        WE_FREE(pucSessionIdMem);
                    }
                    return G_SEC_NOTENOUGH_MEMORY;
                }
                (void)WE_MEMCPY(pucPrivateKeyIdMem, pstSessionRenewPara->pucPrivateKeyId, (WE_UINT32)M_SEC_PRIVATE_KEY_ID_LEN);
            }
            
            iResult = Sec_MsgRenewSession((WE_HANDLE)pMe, pstSessionRenewPara->iMasterSecretId, 
                pstSessionRenewPara->ucSessionOptions, pucSessionIdMem, 
                pstSessionRenewPara->ucSessionIdLen,
                pstSessionRenewPara->pucCipherSuite, 
                pstSessionRenewPara->ucCompressionAlg, 
                pucPrivateKeyIdMem, pstSessionRenewPara->uiCreationTime);
            
            if (M_SEC_ERR_OK != iResult)
            {
                if (NULL != pucSessionIdMem)
                {
                    WE_FREE(pucSessionIdMem);
                }
                if (NULL != pucPrivateKeyIdMem)
                {
                    WE_FREE(pucPrivateKeyIdMem);
                }
                return G_SEC_NOTENOUGH_MEMORY;  
            }      
        }
        break;
    default:
        {
            return G_SEC_INVALID_PARAMETER;             
        }
    }
   
   Sec_RunMainFlow(pMe);
   return G_SEC_OK;

}

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

#ifndef M_SEC_WTLS_NO_CONFIRM_UE 
/*==================================================================================================
FUNCTION: 
    ISecW_EvtConfirmAction
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    after the invoker receive the confirm event ,this function should be run to give a 
    feedback to sec module.
ARGUMENTS PASSED:
    ISecB *             pMe[IN]:pointer of ISec instance.
    WE_INT32 iTargetID[IN]:the identity of the invoker ,input from the interface.
    St_ConfirmAction    stConfirm[IN]:The value of the confirm.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_OK,
    G_SEC_GENERAL_ERROR  
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_EvtConfirmAction(WE_HANDLE pMe,WE_INT32 iTargetID,St_ConfirmAction stConfirm)
{
   WE_INT32   iResult = 0;
    
   if(NULL == pMe)
   {
       return G_SEC_INVALID_PARAMETER;
   }    
   WE_LOG_MSG((0,(WE_UCHAR)0,"Sec_EvtConfirmAction\n"));
    
   iResult = Sec_UeConfirmAction((WE_HANDLE)pMe,iTargetID, stConfirm);
        
   if(M_SEC_ERR_OK != iResult)
   {
       return G_SEC_GENERAL_ERROR;  
   }    

   Sec_RunMainFlow(pMe);
   return G_SEC_OK;
}
#endif
/*==================================================================================================
FUNCTION: 
    ISecW_EvtShowDlgAction
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    after the invoker receive the confirm event ,this function should be run to give a 
    feedback to sec module.
ARGUMENTS PASSED:
    WE_HANDLE pMe[IN]:pointer of ISec instance.
    WE_INT32 iTargetID[IN]:the identity of the invoker ,input from the interface.
    WE_VOID* pvSrcData[IN]:Pointer to the dada.
    WE_UINT32 uiLength[IN]:Length of data.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_OK,
    G_SEC_GENERAL_ERROR  
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISecW_EvtShowDlgAction(WE_HANDLE pMe,WE_INT32 iTargetId,
                                WE_VOID* pvSrcData,WE_UINT32 uiLength)
{
    WE_INT32 iRes = 0;
    WE_INT32 iEvent = 0;
    WE_VOID* pvData = NULL;
    if((NULL == pMe) || (NULL == pvSrcData))
    {
        return G_SEC_GENERAL_ERROR;
    }
    /*decode*/
    if(!Sec_DecodeEvtAction(pvSrcData,uiLength,&iEvent,&pvData))
    {
        if(NULL != pvData)
        {
            WE_FREE(pvData);
            pvData = NULL;
        }
        return G_SEC_GENERAL_ERROR;
    }
    switch(iEvent)
    {      
#ifndef M_SEC_WTLS_NO_CONFIRM_UE  /*have confirmation dialog*/

        case M_UE_CONFIRM_EVENT:
        {
            St_ConfirmAction* pstConfirm = (St_ConfirmAction*)pvData;
            if(NULL == pstConfirm)
            {
                return G_SEC_NOTENOUGH_MEMORY;
            }
            
            iRes = (WE_INT32)ISecW_EvtConfirmAction(pMe,iTargetId,*pstConfirm);

        }
        break;
#endif        
        default:
            iRes = G_SEC_GENERAL_ERROR;
            break;
    }
    /*Free memory*/
    if(NULL != pvData)
    {
        WE_FREE(pvData);
        pvData = NULL;
    }
    return (WE_UINT32)iRes;
}
#endif
/*==================================================================================================
FUNCTION: 
    ISecW_SslHashUpdateWMasterSec
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    after the invoker receive the confirm event ,this function should be run to give a 
    feedback to sec module.
ARGUMENTS PASSED:
    WE_HANDLE pMe[IN]:pointer of ISec instance.
    WE_INT32 iTargetID[IN]:the identity of the invoker ,input from the interface.
    WE_INT32 iMasterSecretID[IN]:Master Secret ID.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_OK,
    G_SEC_GENERAL_ERROR  
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
WE_INT32 ISecW_SslHashUpdateWMasterSec(WE_HANDLE pMe, WE_HANDLE hHandle,WE_INT32 iMasterSecretID)
{
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,
              "..............into ISecW_SslHashUpdateWMasterSec()\n"));
    
     Sec_iWimGetMasterSecret ((WE_HANDLE)pMe,hHandle,iMasterSecretID);
     return G_SEC_OK ;
} 
#endif


