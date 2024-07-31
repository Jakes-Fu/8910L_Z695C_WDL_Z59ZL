/*==================================================================================================
    FILE NAME   : wap_ssl.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
    
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

#ifdef WAP_CONFIG_SSL
#include "tls_def.h"
#include "sec_cfg.h"  /*must before isecw.h*/
#include "sec.h" /*before isecw.h*/
#include "ISecW.h"
#include "we_libalg.h"

#include "itls.h"

#include "tls_ast.h"
#include "tls_ecdr.h" /*before wap_tlspub.h*/
#include "tls_msg.h"

#include "wap_tlsdef.h"  /*marco define*/
#include "wap_tlsst.h"  /*structer*/
#include "wap_tlspub.h"
#include "wap_tlshmcore.h"
//#include "wap_tls.h"
#include "wap_sslcore.h"

/***************************************************************************************************
* Macro Define Section
***************************************************************************************************/


/*==================================================================================================
FUNCTION: 
    WapSSL_GetHashParaPad1
CREATE DATE: 
    2006-01-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    The parameter pad1 to hash.
ARGUMENTS PASSED:    
    WE_UCHAR *pucPad1 : pointer to store the pad1 array
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
void WapSSL_GetHashParaPad1
(
    WE_UCHAR    *pucPad1
)
{
    WE_UCHAR aucHashParaPad1[M_WAP_SSL_MD5_PAD_LEN] = 
    {
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36
    };
    if( NULL == pucPad1 )
    {
        return;
    }
    WAP_MEMCPY( pucPad1, aucHashParaPad1,M_WAP_SSL_MD5_PAD_LEN );
}


/*==================================================================================================
FUNCTION: 
    WapSSL_GetHashParaPad2
CREATE DATE: 
    2007-01-22
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    The parameter pad2 to hash.
ARGUMENTS PASSED:    
    WE_UCHAR *pucPad2 : pointer to store the pad2 array
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
void WapSSL_GetHashParaPad2
(
    WE_UCHAR    *pucPad2
)
{
    WE_UCHAR aucHashParaPad2[M_WAP_SSL_MD5_PAD_LEN] = 
    {
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c,
        0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c
    };
    if( NULL == pucPad2 )
    {
        return;
    }

    WAP_MEMCPY( pucPad2, aucHashParaPad2,M_WAP_SSL_MD5_PAD_LEN );
}


/*==================================================================================================
FUNCTION: 
    WapSsl_ConvertAlertDesc
CREATE DATE: 
    2006-12-31
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Convert the Tls' alerts that aren't declared in SSL3.0 protocol to SSL's alerts.
ARGUMENTS PASSED:    
    WE_INT32 iAlertDesc : the alert to convert
RETURN VALUE:
   WE_INT32 : the alert converted
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSL_ConvertAlertDesc (WE_INT32 iAlertDesc)
{
    switch( iAlertDesc )
    {
        case M_WAP_TLS_ALERT_DESC_DECOMPRESSION_FAILURE:
            return M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC;
            
        case M_WAP_TLS_ALERT_DESC_RECORD_OVERFLOW:
            return M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC;
            
        case M_WAP_TLS_ALERT_DESC_UNKNOWN_CA:
            return M_WAP_TLS_ALERT_DESC_BAD_CERTIFICATE;
            
        case M_WAP_TLS_ALERT_DESC_ACCESS_DENIED:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_DECODE_ERROR:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_EXPORT_RESTRICTION:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_PROTOCOL_VERSION:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_INSUFFICIENT_SECURITY:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_USER_CANCELED:
            return M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE;
            
        case M_WAP_TLS_ALERT_DESC_NO_RENEGOTIATION:
            return -1;
        default:
            return iAlertDesc;
    }
}

/*==================================================================================================
FUNCTION: 
    WapSSL_ComputeMac
CREATE DATE: 
    2001-01-04
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    calculate MAC
    The MAC is generated as:
    hash(MAC_write_secret + pad_2 +
              hash(MAC_write_secret + pad_1 + seq_num +
                   SSLCompressed.type + SSLCompressed.length +
                   SSLCompressed.fragment)
        );
ARGUMENTS PASSED:    
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstState,
    WE_UCHAR           *pucMacSecret,
    St_WapTlsRecord    *pstRec,
    WE_UCHAR           *pucMac
RETURN VALUE:
    -1  :   Error
    0   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSL_ComputeMAC
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConnState *pstState,
    WE_UCHAR           *pucMacSecret,
    St_WapTlsRecord    *pstRec,
    WE_UCHAR           *pucMac
)
{

    WE_HANDLE   hHandle = NULL;
    WE_UCHAR    aucTmpBuf[11] = {0};
    WE_UCHAR    aucMd[20] = {0};
    WE_INT32    iPadLen = 0;
    St_WapTlsHashInfo   *pstHashInfo = NULL;
    WE_UCHAR    *pucPad1 = NULL;
    WE_UCHAR    *pucPad2 = NULL;

    WE_INT32    iResult = 0;/*for check the function's result*/

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac begin\n");)
    if( NULL == pstTlsHandle || NULL == pstState || NULL == pucMac 
        || NULL == pucMacSecret || NULL == pstRec )    
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac parameter error\n");)
        return -1;
    }
    
    pstHashInfo = WapTls_FindHash(pstTlsHandle,pstState->ucHashAlg );
    if( NULL == pstHashInfo )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac FindHash error\n");)
        return -1;
    }

    aucTmpBuf[0] = 0;
    aucTmpBuf[1] = 0;
    aucTmpBuf[2] = 0;
    aucTmpBuf[3] = 0;
    aucTmpBuf[4] = (WE_UCHAR)( (pstRec->uiSeqnum >> 24) & 0xff );
    aucTmpBuf[5] = (WE_UCHAR)( (pstRec->uiSeqnum >> 16) & 0xff );
    aucTmpBuf[6] = (WE_UCHAR)( (pstRec->uiSeqnum >> 8) & 0xff );
    aucTmpBuf[7] = (WE_UCHAR)( pstRec->uiSeqnum & 0xff );
    aucTmpBuf[8] = pstRec->ucContentType;
    aucTmpBuf[9] = (WE_UCHAR)( (pstRec->usLength >> 8) & 0xff );
    aucTmpBuf[10] = (WE_UCHAR)( pstRec->usLength & 0xff );

    if( (WE_UINT8)G_SEC_HASH_MD5 == pstState->ucHashAlg )
    {
        iPadLen = M_WAP_SSL_MD5_PAD_LEN;
    }
    else
    {
        iPadLen = M_WAP_SSL_SHA_PAD_LEN;
    }
    pucPad1 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad1 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac MALLOC fail\n");)
        return -1;
    }
    WapSSL_GetHashParaPad1( pucPad1 );
    
    pucPad2 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad2 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac MALLOC fail\n");)
        return -1;
    }
    WapSSL_GetHashParaPad2( pucPad2 );

    iResult = WapTls_HashInit( pstTlsHandle, (E_WeHashAlgType)pstState->ucHashAlg, &hHandle );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashInit step1 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pucMacSecret, pstHashInfo->ucHashSize );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashMacSecret step1 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pucPad1, iPadLen );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac Hash Pad1 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucTmpBuf, 11 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac Hash Seq_num error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstRec->pucFragment, pstRec->usLength );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashFragment error\n");)
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashEnd step1 error\n");)
        return -1;
    }


    iResult = WapTls_HashInit( pstTlsHandle, (E_WeHashAlgType)pstState->ucHashAlg, &hHandle );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashInit step2 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pucMacSecret, pstHashInfo->ucHashSize );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashMacSecret step2 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pucPad2, iPadLen );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac Hash Pad2 error\n");)
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd, pstHashInfo->ucHashSize );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac Hash the result of step1 error\n");)
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, pucMac );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac HashEnd error\n");)
        return -1;
    }

    
    if( NULL != pucPad1 )
    {
        WAP_FREE( pucPad1 );
    }
    if( NULL != pucPad2 )
    {
        WAP_FREE( pucPad2 );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSL_ComputeMac end\n");)
    return 0;
}


#endif
