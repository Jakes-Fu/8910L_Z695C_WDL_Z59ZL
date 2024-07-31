/*==================================================================================================
    FILE NAME   : wap_tlsrec.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       Record operation
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/

#include "tls_config.h"
#ifdef WAP_CONFIG_TLS
#include "tls_def.h"

#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h"
#include "ISecW.h"
#include "we_libalg.h"

#include "tls_ast.h"
#include "tls_ecdr.h" /*before wap_tlspub.h*/
#include "tls_msg.h"

#include "wap_tlsdef.h"
#include "wap_tlsst.h" /*must before wap_tlspub.h*/

#include "wap_tlspub.h" 
#include "wap_sslcore.h"

/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/

/*==================================================================================================
FUNCTION: 
    WapTls_DeleRecList
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Delete the record list
ARGUMENTS PASSED:
    ppstRecList: list header
    pstRec: record
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/

void WapTls_DeleRecList(St_WapTlsRecord **ppstRecList)
{
    St_WapTlsRecord *pstRec = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleRecList begin.\n" );)

    while (*ppstRecList != NULL)
    {
        pstRec = *ppstRecList;
        *ppstRecList = pstRec->pstNext;
        WAP_FREE( pstRec->pucFragment);
        WAP_FREE (pstRec);
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleRecList end.\n" );)
}
/*==================================================================================================
FUNCTION: 
    WapTls_AddRecToList
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Add Record to the list tail
ARGUMENTS PASSED:
    ppstRecList: list header
    pstRec: record
RETURN VALUE:
    if create successfully, return connection address
    else return NULL 
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void WapTls_AddRecToList( St_WapTlsRecord **ppstRecList, St_WapTlsRecord* pstRec)
{
    St_WapTlsRecord *pstRecP = NULL;
    St_WapTlsRecord *pstRecQ = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_AddRecToList begin.\n" );)

    /*Find the last one*/
    for (pstRecQ = NULL, pstRecP = *ppstRecList; pstRecP != NULL; pstRecQ = pstRecP,pstRecP = pstRecP->pstNext);

    if (pstRecQ == NULL)/*no node in this list*/
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_AddRecToList one node.\n" );)
        *ppstRecList = pstRec;
    }
    else /*add to the last node*/
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_AddRecToList add to the last.\n" );)
        pstRecQ->pstNext = pstRec;
    }
    pstRec->pstNext = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_AddRecToList end.\n" );)
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetRecFromList
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Get First Record from record list
ARGUMENTS PASSED:
    ppstRecList : record list
RETURN VALUE:
    NULL or list header
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsRecord* WapTls_GetRecFromList(St_WapTlsRecord **ppstRecList)
{
    St_WapTlsRecord *pstRec = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_GetRecFromList begin.\n" );)

    if (NULL == *ppstRecList)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_GetRecFromList No node,return NULL.\n" );)
        return NULL;
    }
    
    pstRec = *ppstRecList;
    *ppstRecList = pstRec->pstNext;
    pstRec->pstNext = NULL;
    return pstRec;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_NewRecord
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    create a new Record
ARGUMENTS PASSED:
    ucContentType : content type we want to create a record
    uiSeqNum      : sequence number
    stProVer      : protocol version
RETURN VALUE:
    Record created or null when error
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsRecord *WapTls_NewRecord
(
    WE_UINT8 ucContentType,
    WE_UINT32 uiSeqNum,
    St_WapTlsProVer stProVer
)
{
    St_WapTlsRecord *pstRecord = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_NewRecord begin.\n" );)

    pstRecord = (St_WapTlsRecord*)WAP_MALLOC(sizeof(St_WapTlsRecord));
    if (NULL == pstRecord)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_NewRecord Malloc error.\n" );)
        return NULL;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_NewRecord Record seqnum is %d\n"),uiSeqNum);)
    pstRecord->ucContentType = ucContentType;
    pstRecord->uiSeqnum = uiSeqNum;
    pstRecord->stProVer = stProVer;
    pstRecord->pucFragment = NULL;
    pstRecord->pstNext = NULL;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_NewRecord end.\n" );)
    return pstRecord;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DeleRecord
CREATE DATE:
   2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Delete the record passed in
ARGUMENTS PASSED:
    pstRecord: record will be deleted
RETURN VALUE:
    -1: error
    0: success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DeleRecord(St_WapTlsRecord *pstRecord)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleRecord begin.\n" );)
    if (NULL == pstRecord)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DeleRecord record null.\n" );)
        return -1;
    }
    WAP_FREE(pstRecord->pucFragment);
    WAP_FREE(pstRecord);
    pstRecord = NULL;
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ParseRecordHeader
CREATE DATE:
    2007-01-08
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    parse the buffer passed in to the record structure
ARGUMENTS PASSED:
    pstRecord[out] : pointer of record
    pucHeader[in]  : buffer contain the record information
RETURN VALUE:
    None
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_INT32 WapTls_ParseRecordHeader( St_WapTlsRecord *pstRecord, WE_UCHAR *pucHeader)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ParseRecordHeader begin.\n" );)
    if (NULL == pstRecord)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ParseRecordHeader parameter error.\n" );)
        return -1;
    }
    if (NULL == pucHeader)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_ParseRecordHeader header NULL.\n" );)
        return -1;
    }
    pstRecord->ucContentType = pucHeader[0];
    pstRecord->stProVer.ucMajor = pucHeader[1];
    pstRecord->stProVer.ucMinor = pucHeader[2];
    pstRecord->usLength = (WE_UINT16)(((pucHeader[3]) << 8) | pucHeader[4]);
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecryptRecord
CREATE DATE:
    2006-12-31
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Decrypt record from network data
ARGUMENTS PASSED:
    pstTlsHandle: TLS module handle
    pstConn : connection
    pstRecord : pointer of record we want to decrypt
RETURN VALUE:
    -1: parameter err
    0 : success
    else: alter description
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DecryptRecord 
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn, 
    St_WapTlsRecord* pstRecord
)
{
    St_WapTlsConnState *pstConnState = NULL;
    St_WapTlsHashInfo *pstHashInfo = NULL;
    St_WapTlsCipherInfo *pstCipherInfo = NULL;
    WE_UCHAR aucMD[M_WAP_TLS_MAX_HASH_SIZE] = {0};
    WE_UCHAR aucIV[8] = {0};
    WE_INT32 iLen = 0;
    WE_INT32 i = 0;
    WE_INT32 iPaddingLen = 0;
    WE_INT32 iRet = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:begin\n");)         
    if ((NULL == pstTlsHandle) || (NULL == pstRecord) || (NULL == pstConn))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:parameter error\n");)
        return -1;
    }
    pstConnState = &(pstConn->stRead);
    if (NULL == pstConnState)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:state null\n");)
        return -1;
    }
    pstHashInfo = WapTls_FindHash(pstTlsHandle,pstConnState->ucHashAlg);
    if (NULL == pstHashInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:hashinfo null\n");)
        return -1;
    }    
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg);
    if (NULL == pstCipherInfo)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:cipher info null\n");)       
        return -1;
    }
    if (M_WAP_TLS_CIPHER_TYPE_BLOCK == pstCipherInfo->ucType)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:block decrypt\n");)
        iLen = (WE_INT32)(pstRecord->usLength);
        WAP_MEMCPY(aucIV, pstRecord->pucFragment + pstRecord->usLength - 8, 8);
        
        iRet = WapTls_DecryptBlock(pstTlsHandle,
                                    pstConnState,
                                    pstRecord->pucFragment, 
                                    pstRecord->usLength,
                                    pstRecord->pucFragment);
        if (iRet < 0)
        {
            return -(M_WAP_TLS_ALERT_DESC_DECRYPTION_FAILED);
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:Decrypt success\n");)

        WAP_MEMCPY(pstConnState->aucIV,aucIV,8);

        iPaddingLen = pstRecord->pucFragment[iLen - 1];
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_DecryptRecord:padding length %d\n"),iPaddingLen);)
        if (iLen < iPaddingLen + 1)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:length error\n");)
            return -(M_WAP_TLS_ALERT_DESC_DECRYPTION_FAILED);
        }

        pstRecord->usLength = (WE_UINT16)(iLen - (iPaddingLen + 1));
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_DecryptRecord:length is %d\n"),pstRecord->usLength);)
#ifdef WAP_CONFIG_SSL
       if (pstConn->stProtocolVersion.ucMinor == M_WAP_TLS_PROTOCOL_VERSION_MINOR)
#endif
        {
            for (i = 0; i< iPaddingLen; i++)
            {
                if (pstRecord->pucFragment[iLen - i -2] != iPaddingLen)
                {
                    return -(M_WAP_TLS_ALERT_DESC_DECRYPTION_FAILED);
                }                    
            } /*end for*/
        }
    } /*end cipher block*/
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:stream decrypt\n");)
        iRet = WapTls_DecryptStreamUpdate(pstTlsHandle,
                                          pstConnState, 
                                          pstRecord->pucFragment,
                                          pstRecord->usLength, 
                                          pstRecord->pucFragment);
        if (iRet < 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:stream error\n");)
            return -(M_WAP_TLS_ALERT_DESC_DECRYPTION_FAILED);            
        }
    }/*end cipher stream*/

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_DecryptRecord:Hash size is %d\n"),pstHashInfo->ucHashSize);)
    if (pstHashInfo->ucHashSize > 0)
    {
        if (pstRecord->usLength < pstHashInfo->ucHashSize)
        {
             WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:incorrect record length\n");)
             return -(M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC);
        }

        pstRecord->usLength = (WE_UINT16)(pstRecord->usLength - pstHashInfo->ucHashSize);
#ifdef WAP_CONFIG_SSL
        if (pstConn->stProtocolVersion.ucMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
        {
            iRet = WapSSL_ComputeMAC(pstTlsHandle,pstConnState,pstConnState->aucMacSecret, pstRecord,aucMD);
        }
        else
#endif
        {
            iRet = WapTls_ComputeMAC(pstTlsHandle,pstConnState,pstConnState->aucMacSecret,pstRecord, aucMD);
        }

        if (iRet < 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:MAC computation faild\n");)
            return -(M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC);
        }

       for (i = 0; i < pstHashInfo->ucHashSize; i++)
       {
           if (pstRecord->pucFragment[pstRecord->usLength + i] != aucMD[i])
           {
               WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:bad mac\n");)
               return -(M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC);
           }
       }
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecryptRecord:end return 0\n");)

    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_EncryptRecord
CREATE DATE:
    2006-12-31
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Encrypt record at client
ARGUMENTS PASSED:
    pstTlsHandle: TLS module handle
    pstConn : connection
    pstRecord : pointer of record we want to decrypt
RETURN VALUE:
    -1: parameter err
    0 : success
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_EncryptRecord
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    St_WapTlsRecord *pstRecord
)
{
    St_WapTlsConnState *pstConnState = NULL;
    St_WapTlsHashInfo *pstHashInfo = NULL;
    St_WapTlsCipherInfo *pstCipherInfo = NULL;
    WE_UCHAR aucMD[M_WAP_TLS_MAX_HASH_SIZE] = {0};
    WE_INT32 iLen = 0;
    WE_INT32 i = 0;
    WE_INT32 iPaddingLen = 0;
    WE_UCHAR aucPadding[8] = {0};
    WE_INT32 iBlockSize = 0;  
    WE_INT32 iBufferLen = 0;
    WE_INT32   iRet = -1;
    WE_UCHAR *pucBuffer = NULL;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:begin\n");)           
    if (NULL == pstTlsHandle)
    {
        return -1;
    }
    pstConnState = &(pstConn->stWrite);
    if (NULL == pstConnState)
    {
        return -1;
    }
    pstHashInfo = WapTls_FindHash(pstTlsHandle,pstConnState->ucHashAlg);
    if (NULL == pstHashInfo)
    {
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord:hash size is %d\n"),pstHashInfo->ucHashSize);)           
    if (pstHashInfo->ucHashSize > 0)
    {
#ifdef WAP_CONFIG_SSL
        if (pstConn->stProtocolVersion.ucMinor == M_WAP_SSL_PROTOCOL_VERSION_MINOR)
        {
            iRet = WapSSL_ComputeMAC(pstTlsHandle,pstConnState,pstConnState->aucMacSecret, pstRecord,aucMD);
        }
        else
#endif
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:compute tls mac\n");)
            iRet = WapTls_ComputeMAC(pstTlsHandle,pstConnState,pstConnState->aucMacSecret, pstRecord,aucMD);
        }
        if (iRet < 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:mac compute error\n");)
            return -1;
        }
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg);
    if (M_WAP_TLS_CIPHER_TYPE_BLOCK == pstCipherInfo->ucType)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:M_WAP_TLS_CIPHER_TYPE_BLOCK\n");)       
        iBlockSize = pstCipherInfo->ucBlockSize;        
        iLen = pstRecord->usLength + pstHashInfo->ucHashSize + 1;
        iPaddingLen = iBlockSize - (iLen % iBlockSize);        
        if (iPaddingLen == iBlockSize)
        {
            iPaddingLen = 0;
        }
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord:iPaddingLen is %d\n"),iPaddingLen);)
        for (i = 0;i <= iPaddingLen; i++)
        {
            aucPadding[i] = (WE_UINT8)iPaddingLen;
        }

        iBufferLen = pstRecord->usLength + pstHashInfo->ucHashSize + 1 + iPaddingLen;
        pucBuffer = WAP_MALLOC(iBufferLen);
        if (NULL == pucBuffer)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:Malloc error\n");)
            return -1;
        }
        WAP_MEMCPY(pucBuffer, pstRecord->pucFragment, pstRecord->usLength);
        WAP_MEMCPY(pucBuffer + pstRecord->usLength, aucMD, pstHashInfo->ucHashSize);
        WAP_MEMCPY(pucBuffer + pstRecord->usLength + pstHashInfo->ucHashSize, aucPadding, iPaddingLen + 1);            
        iRet = WapTls_EncryptBlock(pstTlsHandle, pstConnState,pucBuffer,iBufferLen,pucBuffer);
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord:encrypt result is %d\n"),iRet);)       
        if (iRet < 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:Encrypt error\n");)
            WAP_FREE(pucBuffer);
            return -1;
        }
        WAP_MEMCPY(pstConnState->aucIV, pucBuffer + iBufferLen - iBlockSize, iBlockSize);
        WAP_FREE(pstRecord->pucFragment);
        pstRecord->pucFragment = pucBuffer;
        pstRecord->usLength = (WE_UINT16)iBufferLen;
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord:after Encrypt record length is %d\n"),iBufferLen);)
    }
    else /*stream cipher*/
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:stream cipher\n");)
        if (pstHashInfo->ucHashSize > 0)
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord:hash size >0, reocrd length is %d\n"),pstRecord->usLength);)
            iBufferLen = pstRecord->usLength + pstHashInfo->ucHashSize;
            pucBuffer = WAP_MALLOC(iBufferLen);
            if (NULL == pucBuffer)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:Malloc error\n");)
                return -1;
            }
            iRet =WapTls_EncryptStreamUpdate(pstTlsHandle,pstConnState,pstRecord->pucFragment,pstRecord->usLength,pucBuffer);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:encrypt stram error\n");)
                WAP_FREE(pucBuffer);
                return -1;
            }  
            iRet =WapTls_EncryptStreamUpdate(pstTlsHandle,pstConnState,aucMD,pstHashInfo->ucHashSize,pucBuffer + pstRecord->usLength);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:encrypt stram error---\n");)
                WAP_FREE(pucBuffer);                
                return -1;
            }
            WAP_FREE(pstRecord->pucFragment);
            pstRecord->pucFragment = pucBuffer;
            pstRecord->usLength = (WE_UINT16)iBufferLen;
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncryptRecord: after encrypt length is %d\n"),iBufferLen);)
        }
        else /*NULL*/
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:hash size <0\n");)
            iRet =WapTls_EncryptStreamUpdate(pstTlsHandle,pstConnState,pstRecord->pucFragment,pstRecord->usLength,pstRecord->pucFragment);
            if (iRet < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncryptRecord:Encrypt error\n");)
                return -1;
            }             
        }  
    }  /*end stream cipher*/
    return 0;
}

#endif


