/*=====================================================================================
    FILE NAME : sec_wimsi.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This file defined several functions to manage and deal with the session information.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                              Tracking
    Date                    Author          Number            Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-22          Bird                                      create sec_wimsi.c
    
=====================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include"sec_comm.h"

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
/*if save server's certficates or not ,0 is not save ,other wise is save*/
#define M_SEC_SAVE_SERVERCERT 0

/*GLOBAL VARIABLE*/
#define SEC_SIINFO (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pstInfo)

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/

static WE_BOOL 
Sec_WimInfoFreeInfo(St_SecInfo *pstSecInfo);


/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/
/*==================================================================================================
FUNCTION: 
    Sec_WimInfoGetMasterSecretId
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    get master secret ID according to security ID
    
ARGUMENTS PASSED:
    WE_INT32 iSecurityId[IN]:The security id number
    WE_UINT8 *ucMasterSecretId[out]:master secret ID
RETURN VALUE:
    NULL
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
static WE_INT32 Sec_WimInfoGetMasterSecretId(WE_HANDLE hSecHandle,WE_INT32 iSecurityId, 
                                             WE_UINT8 *ucMasterSecretId)
{
    if((NULL == hSecHandle) || (NULL == ucMasterSecretId))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    if (iSecurityId >= 0) 
    {
        WE_INT32 iSlot = M_SEC_IDTOSLOT(iSecurityId);
        
        if (SEC_SIINFO->astIds[iSlot].iSecurityId == iSecurityId) 
        {
            *ucMasterSecretId = SEC_SIINFO->astIds[iSlot].ucMasterSecretId;
            return M_SEC_ERR_OK;
        }
    }
    
    return M_SEC_ERR_NOT_FOUND;
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_WimStartInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    fill St_SecInfo strucure ,using the input data from file
    
ARGUMENTS PASSED:
    WE_UCHAR *pucData[IN]:input data
    WE_INT32 iLen[IN]:length
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_WimStartInfo(WE_HANDLE hSecHandle, WE_UCHAR *pucData, WE_INT32 iLen)
{
    WE_INT32 iIndex = 0;
    
    if(NULL == hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    /* add by Sam [070203] */
    if (pucData == NULL || iLen <= 0)
    {
        return M_SEC_ERR_OK;
    }
    
    for (iIndex=0; iIndex<M_SEC_SECURITYID; ++iIndex) 
    {
        SEC_SIINFO->astIds[iIndex].iSecurityId = -1;
        SEC_SIINFO->astIds[iIndex].ucMasterSecretId = 0;
    }
    
    SEC_SIINFO->iCurSecurityId = 0;
    (void)WE_MEMCPY(SEC_SIINFO,pucData,(WE_UINT32)iLen);    
    
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_WimGetDataInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    get data from St_SecInfo 
ARGUMENTS PASSED:
    WE_UCHAR **ppucData[OUT]:data get from St_SecInfo
    WE_INT32 *piLen[out]:data length
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_WimGetDataInfo(WE_HANDLE hSecHandle, WE_UCHAR **ppucData, WE_INT32 *piLen)
{
    St_SecInfo* pstSecInfo = NULL; 
    WE_BOOL bValue = TRUE;
    if((NULL == hSecHandle) ||(NULL == ppucData) || (NULL == piLen))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    pstSecInfo = SEC_SIINFO;
    
    *piLen = sizeof(St_SecInfo);
    *ppucData = WE_MALLOC((WE_UINT32)(*piLen));
    if (NULL == *ppucData )
    {
        *piLen  = 0;
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(*ppucData,0,(WE_UINT32)*piLen);
    
    /*copy data*/
    /*set certificate number is 0,because it does not store is file by bird 061109*/
    bValue = Sec_WimInfoFreeInfo(SEC_SIINFO);
    if(!bValue)
    {
        WE_FREE(*ppucData);
        *ppucData = NULL;
        *piLen  = 0;
        return M_SEC_ERR_GENERAL_ERROR;
    }

    (void)WE_MEMCPY(*ppucData,pstSecInfo,(WE_UINT32)*piLen);
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_WimStopInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    FREE St_SecInfo
    
ARGUMENTS PASSED:
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_WimStopInfo(WE_HANDLE hSecHandle)
{  
    WE_BOOL bResult = TRUE;
    if(NULL == hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    bResult = Sec_WimInfoFreeInfo(SEC_SIINFO);
    if (bResult != TRUE)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_WimGetNewSecIdInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    get secret ID
    
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle[IO]: handle of data.
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_WimGetNewSecIdInfo(WE_HANDLE hSecHandle)
{
    if(NULL == hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    return SEC_SIINFO->iCurSecurityId;
}

/*==================================================================================================
FUNCTION: 
    Sec_WimAddInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
   add new entry to St_SecInfo
    
ARGUMENTS PASSED:
    WE_INT32 iMasterSecretId[IN]:master secret ID
    WE_INT32 iSecurityId[IN]:The security id number
    WE_INT32 iFullHandhake[IN]:if fullhandhake or not
    St_SecSessionInfo *pstFromInfo[IN]:session info data
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_WimAddInfo(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId, WE_INT32 iSecurityId,
                        WE_INT32 iFullHandhake, St_SecSessionInfo *pstFromInfo)
{
    WE_INT32 iSlot = 0;
    St_SecSessionInfo *pstToInfo = NULL;    
    WE_INT32 iIndex = 0;
    WE_LOG_MSG((0,0,"SEC:Sec_WimAddInfo\n"));
    if((NULL == hSecHandle ) || (NULL == pstFromInfo)
        ||(iMasterSecretId >= M_SEC_SESSION_CACHE_SIZE))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    if (iFullHandhake) 
    {
        for (iIndex=0; iIndex<M_SEC_SECURITYID; ++iIndex) 
        {
            if (SEC_SIINFO->astIds[iIndex].ucMasterSecretId == iMasterSecretId) 
            {
                SEC_SIINFO->astIds[iIndex].ucMasterSecretId = 0;
                SEC_SIINFO->astIds[iIndex].iSecurityId = -1;
            }
        }
    }
    
    
    iSlot = M_SEC_IDTOSLOT(iSecurityId);
    SEC_SIINFO->astIds[iSlot].iSecurityId = iSecurityId;
    SEC_SIINFO->astIds[iSlot].ucMasterSecretId = (WE_UINT8) iMasterSecretId;
    
    
    if (iFullHandhake) 
    {
        pstToInfo = &SEC_SIINFO->astInfo[iMasterSecretId];
        
        if (pstToInfo->ucNumCerts > 0 || pstToInfo->pstCerts) 
        {
            for (iIndex=0; iIndex<pstToInfo->ucNumCerts; iIndex++)
            {
                M_SEC_SELF_FREE(pstToInfo->pstCerts[iIndex].pucCert);
            }
            WE_FREE(pstToInfo->pstCerts);
        }
        
        *pstToInfo = *pstFromInfo;
        /* add by Sam [070130] */
        
        /*if (pstFromInfo->ucNumCerts > 0)*/  /*the number of cert is 1;modified by tang 070409*/
        if (pstFromInfo->ucNumCerts == 1)
        {
            pstToInfo->ucNumCerts = pstFromInfo->ucNumCerts;
            pstToInfo->pstCerts = (St_SecAsn1Certificate*)WE_MALLOC(pstToInfo->ucNumCerts*sizeof(St_SecAsn1Certificate));
            if (!pstToInfo->pstCerts)
            {
                return M_SEC_ERR_INSUFFICIENT_MEMORY;
            }
            (WE_VOID)WE_MEMSET(pstToInfo->pstCerts, 0x00, pstToInfo->ucNumCerts*sizeof(St_SecAsn1Certificate));
            for (iIndex = 0; iIndex < pstToInfo->ucNumCerts; iIndex++)
            {
                pstToInfo->pstCerts[iIndex].ucFormat = pstFromInfo->pstCerts[iIndex].ucFormat;
                if (pstFromInfo->pstCerts[iIndex].uiCertLen)
                {
                    pstToInfo->pstCerts[iIndex].uiCertLen = pstFromInfo->pstCerts[iIndex].uiCertLen;
                    pstToInfo->pstCerts[iIndex].pucCert = (WE_UINT8 *)WE_MALLOC(pstToInfo->pstCerts[iIndex].uiCertLen * sizeof(WE_UINT8));
                    if (!pstToInfo->pstCerts[iIndex].pucCert)
                    {
                        WE_INT32 iIndexJ = 0;
                        for (iIndexJ = 0; iIndexJ < iIndex; iIndexJ++)
                        {
                            WE_FREE(pstToInfo->pstCerts[iIndexJ].pucCert);
                        }
                        WE_FREE(pstToInfo->pstCerts);
                        return M_SEC_ERR_INSUFFICIENT_MEMORY;
                    }
                    (WE_VOID)WE_MEMSET(pstToInfo->pstCerts[iIndex].pucCert, 0x00, pstToInfo->pstCerts[iIndex].uiCertLen);
                    (WE_VOID)WE_MEMCPY(pstToInfo->pstCerts[iIndex].pucCert, 
                                       pstFromInfo->pstCerts[iIndex].pucCert, 
                                       pstToInfo->pstCerts[iIndex].uiCertLen);
                }
            }
        }
        pstToInfo->usEncryptionKeyLen *= 8;
        pstToInfo->usKeyExchangeKeyLen *= 8;

        /*delete by birdzhang 061017 for memory leak*/
     /*   
        if (pstFromInfo->ucNumCerts > 0) 
        {
            pstToInfo->pstCerts = WE_MALLOC(sizeof(St_SecAsn1Certificate) *
                pstFromInfo->ucNumCerts);
            if(NULL == pstToInfo->pstCerts)
            {
                return M_SEC_ERR_INSUFFICIENT_MEMORY;
            }
            for (iIndex=0; iIndex<pstFromInfo->ucNumCerts; ++iIndex) 
            {
                pstToInfo->pstCerts[iIndex].ucFormat = pstFromInfo->pstCerts[iIndex].ucFormat;
                pstToInfo->pstCerts[iIndex].uiCertLen = pstFromInfo->pstCerts[iIndex].uiCertLen;
                pstToInfo->pstCerts[iIndex].pucCert = WE_MALLOC(pstFromInfo->pstCerts[iIndex].uiCertLen);
                if (!pstToInfo->pstCerts[iIndex].pucCert)
                {
                    WE_FREE(pstToInfo->pstCerts);
                    return M_SEC_ERR_INSUFFICIENT_MEMORY;
                }
                (void)WE_MEMCPY(pstToInfo->pstCerts[iIndex].pucCert, pstFromInfo->pstCerts[iIndex].pucCert, pstFromInfo->pstCerts[iIndex].uiCertLen);
            }
        }
     */
    }
    
    
    ++SEC_SIINFO->iCurSecurityId;
    if (SEC_SIINFO->iCurSecurityId < 0)
    {
        SEC_SIINFO->iCurSecurityId = 0;
    }    
    return M_SEC_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    Sec_WimGetInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    get session info data ,according security ID
    
ARGUMENTS PASSED:
    WE_INT32 iSecurityId[IN]:The security id number
    St_SecSessionInfo **ppstInfo[out]:session info data
RETURN VALUE:
    NULL
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
WE_INT32 Sec_WimGetInfo(WE_HANDLE hSecHandle,WE_INT32 iSecurityId, St_SecSessionInfo **ppstInfo)
{
    WE_UINT8 ucMasterSecretId = 0;
    WE_INT32 iResult = 0;
    if((NULL == hSecHandle) || (NULL == ppstInfo))
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    iResult = Sec_WimInfoGetMasterSecretId(hSecHandle,iSecurityId, &ucMasterSecretId);
    
    if (iResult == M_SEC_ERR_OK)
    {
        *ppstInfo = &(SEC_SIINFO->astInfo[ucMasterSecretId]);
    }
    else
    {
        *ppstInfo = NULL;
    }    
    return iResult;
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_WimInfoFreeInfo
CREATE DATE:
    2006-07-22
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    free memroy
    
ARGUMENTS PASSED:
    St_SecInfo *pstSecInfo[IN]:St_SecInfo structure
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_BOOL Sec_WimInfoFreeInfo(St_SecInfo *pstSecInfo)
{
    WE_INT32 iIndexI = 0;
    WE_INT8  cNumCerts = 0;
    if(NULL == pstSecInfo)
    {
        return FALSE;
    }
    /*2*/
    /*astInfo*/
    /*modify by Bird 061109*/
    for(iIndexI = 0; iIndexI < M_SEC_SESSION_CACHE_SIZE; iIndexI++)
    {
        /*have certificate or not?*/
        cNumCerts = (WE_INT8)(pstSecInfo->astInfo[iIndexI].ucNumCerts);
        if(cNumCerts > 0)
        {
            /*delete by bird 070126*/
            /* add by Sam [070130] */
            WE_INT iIndexJ = 0;
            for(iIndexJ = 0; iIndexJ < cNumCerts; iIndexJ++)
            {
                if(NULL != pstSecInfo->astInfo[iIndexI].pstCerts[iIndexJ].pucCert)
                {
                    WE_FREE(pstSecInfo->astInfo[iIndexI].pstCerts[iIndexJ].pucCert);
                }
                pstSecInfo->astInfo[iIndexI].pstCerts[iIndexJ].pucCert = NULL;
            }
            WE_FREE(pstSecInfo->astInfo[iIndexI].pstCerts);
            pstSecInfo->astInfo[iIndexI].pstCerts = NULL;
        }
        pstSecInfo->astInfo[iIndexI].ucNumCerts = 0;     
    }
    return TRUE;
  
}

