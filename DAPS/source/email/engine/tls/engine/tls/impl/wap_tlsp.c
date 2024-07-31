/*==================================================================================================
    FILE NAME   : wap_tlsp.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        encapsulation SEC operation   
       
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
#include "wap_tlsst.h"
#include "wap_tlspub.h" 

/**************************************************************************************************
*  Macro Define Section
***************************************************************************************************/
#define TLS_NUM_CIPHER_SUITES  28
#define TLS_NUM_CIPHER_ALGS  8
#define TLS_NUM_HASH_ALGS 3
/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static WE_INT32 WapTls_HMACInit
(
    St_WapTlsHandle *pstTlsHandle,
    E_WeHmacAlgType eAlgType,
    WE_UCHAR *pucKey,
    WE_INT32  iKeySize,  
    WE_HANDLE *phMacHandle
);
static WE_INT32 WapTls_HMACUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHandle,
    WE_UCHAR *pucBuffer,
    WE_INT32  iBufferLen
);
static WE_INT32 WapTls_HMACEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHmacHandle,
    WE_UCHAR *pucMACResult
);
/*==================================================================================================
FUNCTION: 
    WapTls_GetCipherSuiteTable
CREATE DATE:
    2007-01-16
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    For get const array
ARGUMENTS PASSED:
    None
RETURN VALUE:
    NULL : Err
    Success: pointer of St_WapTlsCipherSuiteInfo
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsCipherSuiteInfo* WapTls_GetCipherSuiteTable(void)
{
    St_WapTlsCipherSuiteInfo *pstCipherSuiteInfoTab = NULL;
    
    St_WapTlsCipherSuiteInfo astTlsCipherSuiteInfo[TLS_NUM_CIPHER_SUITES] =
    {
        {G_SEC_TLS_NULL_WITH_NULL_NULL, TRUE,
        G_SEC_TLS_KEYEXCH_NULL, M_WAP_TLS_CIPHER_NULL, M_WAP_TLS_HASH_NULL},

        {G_SEC_TLS_RSA_WITH_NULL_MD5, TRUE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_NULL, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_RSA_WITH_NULL_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_NULL, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_RSA_EXPORT_WITH_RC4_40_MD5, TRUE,
        G_SEC_TLS_KEYEXCH_RSA_EXPORT, M_WAP_TLS_CIPHER_RC4_40, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_RSA_WITH_RC4_128_MD5, FALSE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_RC4_128, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_RSA_WITH_RC4_128_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_RC4_128, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5, TRUE,
        G_SEC_TLS_KEYEXCH_RSA_EXPORT, M_WAP_TLS_CIPHER_RC2_CBC_40, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_RSA_WITH_IDEA_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_IDEA_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_RSA_EXPORT_WITH_DES40_CBC_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_RSA_EXPORT, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_RSA_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_RSA_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_RSA, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_DH_DSS_EXPORT, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_DSS_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_DSS, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_DSS, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_DH_RSA_EXPORT, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_RSA_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_RSA, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_RSA, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_DSS_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DHE_DSS, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DHE_DSS, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA, TRUE,
        G_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_RSA_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DHE_RSA, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DHE_RSA, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_ANON_EXPORT_WITH_RC4_40_MD5, TRUE,
        G_SEC_TLS_KEYEXCH_DH_ANON_EXPORT, M_WAP_TLS_CIPHER_RC4_40, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_DH_ANON_WITH_RC4_128_MD5, FALSE,
        G_SEC_TLS_KEYEXCH_DH_ANON, M_WAP_TLS_CIPHER_RC4_128, M_WAP_TLS_HASH_MD5},

        {G_SEC_TLS_DH_ANON_EXPORT_WITH_DES40_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_ANON, M_WAP_TLS_CIPHER_DES40_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_ANON_WITH_DES_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_ANON, M_WAP_TLS_CIPHER_DES_CBC, M_WAP_TLS_HASH_SHA},

        {G_SEC_TLS_DH_ANON_WITH_3DES_EDE_CBC_SHA, FALSE,
        G_SEC_TLS_KEYEXCH_DH_ANON, M_WAP_TLS_CIPHER_3DES_EDE_CBC, M_WAP_TLS_HASH_SHA}
    };
    pstCipherSuiteInfoTab = (St_WapTlsCipherSuiteInfo *)WAP_MALLOC(TLS_NUM_CIPHER_SUITES *sizeof(St_WapTlsCipherSuiteInfo));
    if (NULL == pstCipherSuiteInfoTab)
    {
        return NULL;
    }
    WAP_MEMCPY(pstCipherSuiteInfoTab,astTlsCipherSuiteInfo,TLS_NUM_CIPHER_SUITES * sizeof(St_WapTlsCipherSuiteInfo));
    return pstCipherSuiteInfoTab;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeCipherSuiteTable
CREATE DATE:
    2007-01-17
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Free Table
ARGUMENTS PASSED:
    None
RETURN VALUE:
    Err: -1
    Success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
 WE_INT32 WapTls_FreeCipherSuiteTable(St_WapTlsCipherSuiteInfo *pstCipherSuTab)
 {
    if (NULL != pstCipherSuTab)
    {
        WAP_FREE(pstCipherSuTab);
        pstCipherSuTab = NULL;
        return 0;
    }
    return -1;
 }
/*==================================================================================================
FUNCTION: 
    WapTls_GetCipherInfoTable
CREATE DATE:
    2007-01-16
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    For get const array
ARGUMENTS PASSED:
    None
RETURN VALUE:
    NULL : Err
    Success: pointer of St_WapTlsCipherInfo
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsCipherInfo* WapTls_GetCipherInfoTable(void)
{
    St_WapTlsCipherInfo *pstCipherInfoTab = NULL;
     
    St_WapTlsCipherInfo astTlsCipherInfo[TLS_NUM_CIPHER_ALGS] = 
    {
        {M_WAP_TLS_CIPHER_NULL,         TRUE,  M_WAP_TLS_CIPHER_TYPE_STREAM,  0,  0, 0, 0},
        {M_WAP_TLS_CIPHER_IDEA_CBC,     FALSE, M_WAP_TLS_CIPHER_TYPE_BLOCK,  16, 16, 8, 8},
        {M_WAP_TLS_CIPHER_RC2_CBC_40,   TRUE,  M_WAP_TLS_CIPHER_TYPE_BLOCK,   5, 16, 8, 8},
        {M_WAP_TLS_CIPHER_RC4_40,       TRUE,  M_WAP_TLS_CIPHER_TYPE_STREAM,  5, 16, 0, 0},
        {M_WAP_TLS_CIPHER_RC4_128,      FALSE, M_WAP_TLS_CIPHER_TYPE_STREAM, 16, 16, 0, 0},
        {M_WAP_TLS_CIPHER_DES40_CBC,    TRUE,  M_WAP_TLS_CIPHER_TYPE_BLOCK,   5,  8, 8, 8},
        {M_WAP_TLS_CIPHER_DES_CBC,      FALSE, M_WAP_TLS_CIPHER_TYPE_BLOCK,   8,  8, 8, 8},
        {M_WAP_TLS_CIPHER_3DES_EDE_CBC, FALSE, M_WAP_TLS_CIPHER_TYPE_BLOCK,  24, 24, 8, 8}
    };
    pstCipherInfoTab = (St_WapTlsCipherInfo *)WAP_MALLOC(TLS_NUM_CIPHER_ALGS *sizeof(St_WapTlsCipherInfo));
    if (NULL == pstCipherInfoTab)
    {
        return NULL;
    }
    WAP_MEMCPY(pstCipherInfoTab,astTlsCipherInfo,TLS_NUM_CIPHER_ALGS * sizeof(St_WapTlsCipherInfo));
    return pstCipherInfoTab;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeCipherInfoTable
CREATE DATE:
    2007-01-17
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Free Table
ARGUMENTS PASSED:
    None
RETURN VALUE:
    Err: -1
    Success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_FreeCipherInfoTable(St_WapTlsCipherInfo *pstCipherInfoTab)
{
    if (NULL != pstCipherInfoTab)
    {
        WAP_FREE(pstCipherInfoTab);
        pstCipherInfoTab = NULL;
        return 0;
    }
    return -1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetHashInfoTable
CREATE DATE:
    2007-01-16
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    For get const array
ARGUMENTS PASSED:
    None
RETURN VALUE:
    NULL : Err
    Success: pointer of St_WapTlsHashInfo
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
 St_WapTlsHashInfo* WapTls_GetHashInfoTable(void)
{
     St_WapTlsHashInfo *pstHashInfoTab = NULL;
     
    St_WapTlsHashInfo astTlsHashInfo[TLS_NUM_HASH_ALGS] ={
                    {M_WAP_TLS_HASH_NULL, 0, 0},
                    {M_WAP_TLS_HASH_MD5, 16, 48},
                    {M_WAP_TLS_HASH_SHA, 20, 40}
                    };
    pstHashInfoTab = (St_WapTlsHashInfo *)WAP_MALLOC(TLS_NUM_HASH_ALGS * sizeof(St_WapTlsHashInfo));
    if (NULL == pstHashInfoTab)
    {
        return NULL;
    }
    WAP_MEMCPY(pstHashInfoTab,astTlsHashInfo,TLS_NUM_HASH_ALGS * sizeof(St_WapTlsHashInfo));
    return pstHashInfoTab;
   
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeHashInfoTable
CREATE DATE:
    2007-01-17
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Free Table
ARGUMENTS PASSED:
    None
RETURN VALUE:
    Err: -1
    Success: 0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_FreeHashInfoTable(St_WapTlsHashInfo *pstHashInfoTab)
{
    if (NULL != pstHashInfoTab)
    {
        WAP_FREE(pstHashInfoTab);
        pstHashInfoTab = NULL;
        return 0;
    }
    return -1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FindCipherSuite
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Find the cipher suite info according to the ciphersuite passed in
ARGUMENTS PASSED:
    usCipherSuite: Cipher suite 
RETURN VALUE:
    if can find the cipher suite in the array, return its address in the array
    else return NULL
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsCipherSuiteInfo *WapTls_FindCipherSuite
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT16 usCipherSuite
)
{
    WE_INT32 iTmp = 0;
    St_WapTlsCipherSuiteInfo *pstCipherSuiteTab = NULL;

    if (NULL == pstTlsHandle)
    {
        return NULL;
    }
        
    pstCipherSuiteTab = pstTlsHandle->pstCipherSuTab;
    for (iTmp = 0; iTmp < TLS_NUM_CIPHER_SUITES; iTmp++)
    {
        if (pstCipherSuiteTab[iTmp].usCipherSuite == usCipherSuite)
        {
            return &(pstCipherSuiteTab[iTmp]);
        }
    }
    return NULL;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FindCipher
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Find the cipher info in the array according to the cipher passed in
ARGUMENTS PASSED:
    ucCipher: cipher 
RETURN VALUE:
    if can find the cipher in the array, return its address in the array
    else return NULL
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
St_WapTlsCipherInfo* WapTls_FindCipher
(
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT8 ucCipher
)
{
    WE_INT32 iTmp = 0;
    St_WapTlsCipherInfo *pstCipherInfoTab = NULL;

    if (NULL == pstTlsHandle)
    {
        return NULL;
    }

    pstCipherInfoTab = pstTlsHandle->pstCipherInfoTab;
    for (iTmp = 0;iTmp < TLS_NUM_CIPHER_ALGS; iTmp++)
    {
        if (pstCipherInfoTab[iTmp].ucCipherAlg == ucCipher)
        {
            return &(pstCipherInfoTab[iTmp]);
        }
    }
    return NULL;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FindHash
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Find the hash info according to the hash algorithm
ARGUMENTS PASSED:
    ucHashAlg: Hash algorithm
RETURN VALUE:
    if can find the hash algorithm in the array, return its address in the array
    else return NULL
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
 St_WapTlsHashInfo* WapTls_FindHash
 (
    St_WapTlsHandle *pstTlsHandle,
    WE_UINT8 ucHashAlg
 )
{
    WE_INT32 iTmp = 0;
    St_WapTlsHashInfo *pstHashInfoTab = NULL;

    pstHashInfoTab = pstTlsHandle->pstHashInfoTab;

    for (iTmp = 0; iTmp < TLS_NUM_HASH_ALGS; iTmp++)
    {
        if (pstHashInfoTab[iTmp].ucHashAlg == ucHashAlg)
        {
            return &(pstHashInfoTab[iTmp]);
        }
    }
    return NULL;
}
/*==================================================================================================
FUNCTION: 
    WapTls_HashInit
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Init Hash operation
ARGUMENTS PASSED:
    hTlsHandle : TLS Module handle
    eAlgType   : Hash algorithm 
    phHandle[out] : Hash handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32  WapTls_HashInit
(
    St_WapTlsHandle *pstTlsHandle,
    E_WeHashAlgType eAlgType,
    WE_HANDLE *phHandle
)
{
    WE_INT32 iTmp = -1;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashInit begin\n");) 
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashInit TLS handle NULL\n");) 
        return -1;
    }
    iTmp = We_LibHashInit( pstTlsHandle->hLibAlgHandle, eAlgType,phHandle);
    if (iTmp != M_WE_LIB_ERR_OK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashInit SEC error\n");) 
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashInit end\n");) 
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_HashUpdate
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Update Hash algorithm
ARGUMENTS PASSED:
    hTlsHandle: TLS Module handle
    hHandle : Hash handle
    pucBuf : buffer to store data to hash
    iLen   : buffer length
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_HashUpdate
( 
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHandle,
    WE_UCHAR *pucBuf, 
    WE_INT32 iLen
)
{
    WE_INT32 iTmp = -1;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashUpdate begin\n");) 
    if (pstTlsHandle == NULL)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashUpdate TLS Handle NULL\n");) 
        return -1;
    }
    if (NULL == hHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashUpdate Hash handle NULL\n");) 
        return -1;
    }
    iTmp = We_LibHashUpdate( pstTlsHandle->hLibAlgHandle,
                                  pucBuf, 
                                  iLen,
                                  hHandle );
    if (iTmp != M_WE_LIB_ERR_OK)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashUpdate SEC Operation error\n");) 
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashUpdate end\n");) 
    return 0;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_HashEnd
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Get hash result
ARGUMENTS PASSED:
    hTlsHandle: TLS Module handle
    hHandle  : Hash handle
    pucDigest[out]: store hash result
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32  WapTls_HashEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE       hHandle,
    WE_UINT8        *pucDigest
)
{
    WE_INT32 iDigestLen = M_WAP_TLS_MAX_HASH_SIZE;
    WE_INT32 iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashEnd begin.\n" );)

    if(NULL == pstTlsHandle)
    {
        return -1;
    }
    iTmp = We_LibHashFinal( pstTlsHandle->hLibAlgHandle,
                                 pucDigest,
                                 &iDigestLen,
                                 hHandle);

    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashEnd SEC error\n");)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HashEnd ending.\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_HMACInit
CREATE DATE:
    20070108
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Init MAC handle
ARGUMENTS PASSED:
    pstTlsHandle: adapt handle
    eAlgType    : HMAC algorithm
    pucKey      : HMAC Key
    iKeySize    : Key size
    phMacHandle[out] : HMAC Handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_HMACInit
(
    St_WapTlsHandle *pstTlsHandle,
    E_WeHmacAlgType eAlgType,
    WE_UCHAR *pucKey,
    WE_INT32  iKeySize,  
    WE_HANDLE *phMacHandle
)
{
    WE_INT32 iTmp = -1;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACInit begin\n");)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACInit TLS handle NULL\n");)
        return -1;
    }
    iTmp = We_LibHmacInit( pstTlsHandle->hLibAlgHandle, eAlgType,pucKey,iKeySize,phMacHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACInit SEC error\n");)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACInit end.\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_HMACUpdate
CREATE DATE:
    20070108
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    MAC update
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    hHandle  : MAC handle
    pucBuffer: store Data to MAC
    iBufferLen: Buffer len
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_HMACUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHandle,
    WE_UCHAR *pucBuffer,
    WE_INT32  iBufferLen
)
{
    WE_INT32 iTmp = -1;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACUpdate begin.\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACUpdate TLS handle NULL\n" );)
        return -1;
    }
    iTmp = We_LibHmacUpdate( pstTlsHandle->hLibAlgHandle,pucBuffer,iBufferLen,hHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACUpdate SEC error\n");)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACUpdate end.\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_HMACEnd
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    End MAC operation
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    hHmacHandle : MAC handle
    pucMACResult[out]: MAC result
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 WapTls_HMACEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hHmacHandle,
    WE_UCHAR *pucMACResult
)
{
    WE_INT32 iTmp = -1;
    WE_INT32  iDigestLen = M_WAP_TLS_MAX_HASH_SIZE; 

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACEnd begin.\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACEnd TLS handle NULL\n" );)
        return -1;
    }
    iTmp = We_LibHmacFinal( pstTlsHandle->hLibAlgHandle,pucMACResult,&iDigestLen,hHmacHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACEnd SEC HashFinal error\n");)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_HMACEnd end.\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_ComputeMAC
CREATE DATE:
    2006-12-31
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Compute MAC value
ARGUMENTS PASSED:
    pstTlsHandle : Tls module handle
    pstConnState : connection state
    pucMacSecret : MAC secret
    pstRec       : record
    pucMac       : [out] MAC result
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

WE_INT32 WapTls_ComputeMAC
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucMacSecret,
    St_WapTlsRecord  *pstRec,
    WE_UCHAR   *pucMAC
)
{
    St_WapTlsHashInfo *pstHashInfo = NULL;
    WE_HANDLE hHMACHandle = NULL;
    WE_UCHAR aucTmpBuf[13] = {0};
    WE_INT32 iRet = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ComputeMAC:begin\n");)
    pstHashInfo = WapTls_FindHash(pstTlsHandle,pstConnState->ucHashAlg);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_ComputeMAC:SeqNum is %d\n"),pstRec->uiSeqnum);)
    aucTmpBuf[0] = 0;
    aucTmpBuf[1] = 0;
    aucTmpBuf[2] = 0;
    aucTmpBuf[3] = 0;
    aucTmpBuf[4] = (WE_UCHAR)((pstRec->uiSeqnum >> 24) & 0xff);
    aucTmpBuf[5] = (WE_UCHAR)((pstRec->uiSeqnum >> 16) & 0xff);
    aucTmpBuf[6] = (WE_UCHAR)((pstRec->uiSeqnum >> 8) & 0xff);
    aucTmpBuf[7] = (WE_UCHAR)((pstRec->uiSeqnum) & 0xff);
    aucTmpBuf[8] = pstRec->ucContentType;
    aucTmpBuf[9] = pstRec->stProVer.ucMajor;
    aucTmpBuf[10] = pstRec->stProVer.ucMinor;
    aucTmpBuf[11] = (WE_UCHAR)((pstRec->usLength >> 8) & 0xff);
    aucTmpBuf[12] = (WE_UCHAR)(pstRec->usLength & 0xff);   
    
    iRet = WapTls_HMACInit(pstTlsHandle,
                           (E_WeHmacAlgType)pstConnState->ucHashAlg,
                           pucMacSecret,
                           pstHashInfo->ucHashSize,
                           &hHMACHandle);
    if (iRet < 0)
    {
        return -1;
    }
    iRet = WapTls_HMACUpdate(pstTlsHandle,
                             hHMACHandle, 
                             aucTmpBuf, 13);
    if (iRet < 0)
    {
        return -1;
    }
    iRet = WapTls_HMACUpdate(pstTlsHandle,
                         hHMACHandle, 
                         pstRec->pucFragment,
                         pstRec->usLength);
    if (iRet < 0)
    {
        return -1;
    }
    iRet = WapTls_HMACEnd(pstTlsHandle,
                         hHMACHandle, 
                         pucMAC);
    if (iRet < 0)
    {
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_ComputeMAC:end\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncryptBlock
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Encrypt data with block cipher
ARGUMENTS PASSED:
    pstTlsHandle: TLS handle
    pstConnState: Connection state
    pucBufferLen: Data to be encrypted
    iBufferLne  : Data length
    pucOutBuffer: Buffer to store data encrypted
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_EncryptBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
)
{
    St_WeCipherCrptKey stKeyObject = {0};
    St_WapTlsCipherInfo *pstCipherInfo = NULL;    
    WE_UINT32            uiOutLen = uiBufferLen;
    WE_INT32            iTmp = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptBlock begin.\n" );)
    if ((NULL == pstTlsHandle) || (NULL == pstConnState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptBlock parameter error.\n" );)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg);
    stKeyObject.pucKey = pstConnState->aucEncryptionKey;
    stKeyObject.iKeyLen = pstCipherInfo->ucKeySize;
    stKeyObject.pucIv = pstConnState->aucIV;
    stKeyObject.iIvLen = pstCipherInfo->ucIvSize;

    
    iTmp = We_LibCipherEncryptNoPadding( pstTlsHandle->hLibAlgHandle,
                                         (E_WeCipherAlgType)pstConnState->ucCipherAlg, 
                                         stKeyObject, 
                                         pucBufferIn, 
                                         uiBufferLen, 
                                         pucOutBuffer,
                                         &uiOutLen );
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptBlock SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptBlock end.\n" );)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncryptStreamInit
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Init Stream cipher for encrypt
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    ucCipherAlg : Cipher algorithm
    pucEncryKey : Encrypt key
    ucKeySize   : Key size
    hCipherHandle: Cipher handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_EncryptStreamInit
(
   St_WapTlsHandle *pstTlsHandle,
   WE_UCHAR ucCipherAlg,
   WE_UCHAR *pucEncryKey,
   WE_UCHAR ucKeySize,
   WE_HANDLE  *phCipherHandle
)
{
    WE_INT32 iTmp = -1;
    St_WeCipherCrptKey  stKeyObject = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamInit begin.\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamInit TLS handle NULL\n" );)
        return -1;
    }
    stKeyObject.pucKey = pucEncryKey;
    stKeyObject.iKeyLen = ucKeySize;
    stKeyObject.pucIv = NULL;
    stKeyObject.iIvLen = 0;
    
    iTmp = We_LibCipherEncryptInitNoPadding(pstTlsHandle->hLibAlgHandle,(E_WeCipherAlgType)ucCipherAlg,
                                                 stKeyObject,phCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamInit SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamInit ending.\n" );)
    return 0;  
    
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncryptStreamUpdate
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Update stream encrypt
ARGUMENTS PASSED:
    pstTlsHandle: Tls Module handle
    pstConnState: Connection state
    pucBuffer   : Data to be encrypted
    uiBufferLen : Data length
    pucOutBuffer: Buffer to store data encrypted
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_EncryptStreamUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
)
{
    WE_INT32 iTmp = -1;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamUpdate begin.\n" );)
    if ((NULL == pstTlsHandle) || (pstConnState == NULL))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamUpdate parameter error.\n" );)
        return -1;
    }
    iTmp = We_LibCipherEncryptUpdateNoPadding(pstTlsHandle->hLibAlgHandle,pucBufferIn,uiBufferLen,pucOutBuffer,
                                              &(uiBufferLen),pstConnState->hCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamUpdate SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamUpdate ending.\n" );)
    return 0;    
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncryptStreamEnd
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    End stream encrypt
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    hCipherHandle: Stream cipher handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_EncryptStreamEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hCipherHandle
)
{
    WE_INT32 iTmp = -1;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamEnd begin.\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamEnd TLS handle NULL.\n" );)
        return -1;
    }
    iTmp = We_LibCipherEncryptFinalNoPadding(pstTlsHandle->hLibAlgHandle,hCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamEnd SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_EncryptStreamEnd ending.\n" );)
    return 0;  
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecryptBlock
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Decrypt data with block cipher
ARGUMENTS PASSED:
    pstTlsHandle: TLS handle
    pstConnState: Connection state
    pucBufferLen: Data to be decrypted
    iBufferLne  : Data length
    pucOutBuffer: Buffer to store data decrypted
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DecryptBlock
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  iBufferLen,
    WE_UCHAR *pucOutBuffer
)
{
    St_WeCipherCrptKey stKeyObject = {0};
    St_WapTlsCipherInfo *pstCipherInfo = NULL;    
    WE_INT32            iOutLen = iBufferLen;
    WE_INT32            iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptBlock begin.\n" );)
    if ((NULL == pstTlsHandle) || (NULL == pstConnState))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptBlock parameter error.\n");)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher(pstTlsHandle,pstConnState->ucCipherAlg);
    stKeyObject.pucKey = pstConnState->aucEncryptionKey;
    stKeyObject.iKeyLen = pstCipherInfo->ucKeySize;
    stKeyObject.pucIv = pstConnState->aucIV;
    stKeyObject.iIvLen = pstCipherInfo->ucIvSize;

    iTmp = We_LibCipherDecryptNoPadding( pstTlsHandle->hLibAlgHandle,
                                         (E_WeCipherAlgType)pstConnState->ucCipherAlg, 
                                         stKeyObject, 
                                         pucBufferIn, 
                                         (WE_UINT32)iBufferLen, 
                                         pucOutBuffer,
                                         (WE_UINT32*)&iOutLen );
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptBlock SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptBlock ending.\n" );)
    return 0; 
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecryptStreamInit
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Init Stream cipher for decrypt
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    ucCipherAlg : Cipher algorithm
    pucEncryKey : decrypt key
    ucKeySize   : Key size
    hCipherHandle: Cipher handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DecryptStreamInit
(
   St_WapTlsHandle *pstTlsHandle,
   WE_UCHAR ucCipherAlg,
   WE_UCHAR *pucEncryKey,
   WE_UCHAR ucKeySize,
   WE_HANDLE  *phCipherHandle
)
{
    WE_INT32 iTmp = -1;
    St_WeCipherCrptKey  stKeyObject = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamInit begin.\n" );)
    if (NULL == pstTlsHandle)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamInit TLS handle NULL.\n" );)
        return -1;
    }
    stKeyObject.pucKey = pucEncryKey;
    stKeyObject.iKeyLen = ucKeySize;
    stKeyObject.pucIv = NULL;
    stKeyObject.iIvLen = 0;
    iTmp = We_LibCipherDecryptInitNoPadding(pstTlsHandle->hLibAlgHandle,(E_WeCipherAlgType)ucCipherAlg,
                                                 stKeyObject,phCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamInit SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamInit ending.\n" );)
    return 0;  
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecryptStreamUpdate
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    Update stream decrypt
ARGUMENTS PASSED:
    pstTlsHandle: Tls Module handle
    pstConnState: Connection state
    pucBuffer   : Data to be decrypted
    uiBufferLen : Data length
    pucOutBuffer: Buffer to store data decrypted
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 WapTls_DecryptStreamUpdate
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstConnState,
    WE_UCHAR *pucBufferIn,
    WE_UINT32  uiBufferLen,
    WE_UCHAR *pucOutBuffer
)
{
    WE_INT32 iTmp = -1;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamUpdate begin.\n" );)
    if ((NULL == pstTlsHandle) || (pstConnState == NULL))
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamUpdate parameter error.\n" );)
        return -1;
    }
    iTmp = We_LibCipherDecryptUpdateNoPadding(pstTlsHandle->hLibAlgHandle,pucBufferIn,uiBufferLen,pucOutBuffer,
                                              &(uiBufferLen),pstConnState->hCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamUpdate SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamUpdate ending.\n" );)
    return 0;    
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecryptStreamEnd
CREATE DATE:
    2007-01-09
AUTHOR:
    Wang Shuhua
DESCRIPTION:
    End stream decrypt
ARGUMENTS PASSED:
    pstTlsHandle: TLS Module handle
    hCipherHandle: Stream cipher handle
RETURN VALUE:
    Error: -1
    Success:0
USED GLOBAL VARIABLES:   
    None
CALL BY:
    omit
IMPORTANT NOTES:
Release CipherHandle
==================================================================================================*/
WE_INT32 WapTls_DecryptStreamEnd
(
    St_WapTlsHandle *pstTlsHandle,
    WE_HANDLE hCipherHandle
)
{
    WE_INT32 iTmp = -1;
    if (NULL == pstTlsHandle)
    {
        return -1;
    }
    iTmp = We_LibCipherDecryptFinalNoPadding(pstTlsHandle->hLibAlgHandle,hCipherHandle);
    if (iTmp != M_WE_LIB_ERR_OK) 
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamEnd SEC encipher error--.\n" );)
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)( "WapTls_DecryptStreamEnd ending.\n" );)
    return 0;  
}
#endif