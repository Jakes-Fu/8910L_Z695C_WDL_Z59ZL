/*==================================================================================================
    FILE NAME   : wap_sslhm.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        handshake layer functions that are different from TLS.
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
#include "sec.h"
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
#include "wap_sslcore.h"
/**************************************************************************************************
* Macro Declare Section
***************************************************************************************************/
#define WAP_SSL_SENDER_CLIENT (const WE_UCHAR *)"\x43\x4C\x4E\x54"
#define WAP_SSL_SENDER_SERVER (const WE_UCHAR *)"\x53\x52\x56\x52"

#define PROC_INTEERR         WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_INTERNAL_ERROR,\
                                    M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR )


/*==================================================================================================
FUNCTION: 
    WapSSLHm_CreateKeys
CREATE DATE: 
    2007-01-04
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Cryptographic computations.

ARGUMENTS PASSED:    
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
RETURN VALUE:
    0   :   success
    -1  :   error
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSLHm_CreateKeys
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
)
{
    St_WapTlsConnState  *pstConnState = NULL;
    St_WapTlsHashInfo   *pstHashInfo = NULL;
    St_WapTlsCipherInfo *pstCipherInfo = NULL;
    WE_INT32    iKeyBlockSize = 0;
    WE_UCHAR    *pucKeyBlock = NULL;
    WE_UCHAR    *pucTmpKeyBlock = NULL;
    WE_INT32    i = 0;
    WE_INT32    j = 0;
    WE_INT32    iTmp = 0;
    WE_INT32    iBytesLeft = 0;
    WE_CHAR     cTmp = 'A';
    WE_CHAR     acTmp[8] = {0};
    WE_HANDLE   hHandle = NULL;
    WE_UCHAR    aucMd[20] = {0};
    WE_INT32    iResult = 0;/*for check the result of function*/

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys Begin\n" );)
    if( NULL == pstConn || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys parameter error\n" );)
        return -1;
    }
    pstConnState = pstConn->pstPendingWrite;
    pstHashInfo = WapTls_FindHash( pstTlsHandle,pstConnState->ucHashAlg );
    if( NULL == pstHashInfo )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys FindHash error\n" );)
        return -1;
    }
    pstCipherInfo = WapTls_FindCipher( pstTlsHandle,pstConnState->ucCipherAlg );
    if( NULL == pstCipherInfo )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys FindCipher error\n" );)
        return -1;
    }

    
    if( TRUE == pstCipherInfo->ucExportAble )
    {
        iKeyBlockSize = 2 * ( pstHashInfo->ucHashSize 
                                + pstCipherInfo->ucKeyMaterialLen );
    }
    else
    {
        iKeyBlockSize = 2 * ( pstHashInfo->ucHashSize 
                                + pstCipherInfo->ucKeyMaterialLen
                                + pstCipherInfo->ucIvSize );
    }

    pucTmpKeyBlock = pucKeyBlock = (WE_UCHAR *)WAP_MALLOC( iKeyBlockSize );
    if( NULL == pucKeyBlock )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys Malloc fail\n" );)
        return -1;
    }


    /*calculate key_block =
       MD5(master_secret + SHA(`A' + master_secret +ServerHello.random + ClientHello.random)) +
       MD5(master_secret + SHA(`BB' + master_secret + ServerHello.random + ClientHello.random)) +
       MD5(master_secret + SHA(`CCC' + master_secret + ServerHello.random + ClientHello.random))
       + [...];*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys compute key_block begin\n" );)
    for( i = 0, iBytesLeft = iKeyBlockSize; iBytesLeft > 0; i++)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys compute key_block %d time\n",i+1);)
        for( j = 0; j <= i; j++ )
        {
            acTmp[j] = cTmp;/*acTmp='A' || 'BB' || 'CCC' ||...*/
        }
        cTmp++;

        iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_SHA1, &hHandle);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashInit SHA error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, (WE_UCHAR *)acTmp, j );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys SHA const char error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }

        ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                        hHandle, 
                                        pstConnState->iMasterSecretId );
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucServerRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys SHA serverRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }

        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucClientRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys SHA clientRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys SHAEnd error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }


        iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashInit MD5 error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }

        ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                        hHandle, 
                                        pstConnState->iMasterSecretId );
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd, 20 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 SHA's result error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 End error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iTmp = MIN( 16, iBytesLeft );
        WAP_MEMCPY( pucTmpKeyBlock, aucMd, iTmp );
        pucTmpKeyBlock += iTmp;
        iBytesLeft -= iTmp;
    }/*end for: end calc key_block*/

    pucTmpKeyBlock = pucKeyBlock;
    WAP_MEMCPY( pstConn->pstPendingWrite->aucMacSecret, pucTmpKeyBlock, pstHashInfo->ucHashSize );
    pucTmpKeyBlock += pstHashInfo->ucHashSize;

    WAP_MEMCPY( pstConn->pstPendingRead->aucMacSecret, pucTmpKeyBlock, pstHashInfo->ucHashSize );
    pucTmpKeyBlock += pstHashInfo->ucHashSize;

    WAP_MEMCPY( pstConn->pstPendingWrite->aucEncryptionKey, pucTmpKeyBlock, pstCipherInfo->ucKeyMaterialLen );
    pucTmpKeyBlock += pstCipherInfo->ucKeyMaterialLen;
    WAP_MEMCPY( pstConn->pstPendingRead->aucEncryptionKey, pucTmpKeyBlock, pstCipherInfo->ucKeyMaterialLen );
    pucTmpKeyBlock += pstCipherInfo->ucKeyMaterialLen;

    if( FALSE == pstCipherInfo->ucExportAble )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys is Unexportable\n" );)
        WAP_MEMCPY( pstConn->pstPendingWrite->aucIV, pucTmpKeyBlock, pstCipherInfo->ucIvSize );
        pucTmpKeyBlock += pstCipherInfo->ucIvSize;
        WAP_MEMCPY( pstConn->pstPendingRead->aucIV, pucTmpKeyBlock, pstCipherInfo->ucIvSize );
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys is Exportable,compute final_client_write key\n" );)

        /*compute final_client_write_key = MD5(client_write_key 
                                       + ClientHello.random + ServerHello.random);*/
        iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashInit MD5 error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, 
                                     pstConn->pstPendingWrite->aucEncryptionKey, 
                                     pstCipherInfo->ucKeyMaterialLen );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 client_write_key error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }

        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucClientRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 clientRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucServerRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 serverRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashEnd MD5 error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        WAP_MEMCPY(pstConn->pstPendingWrite->aucEncryptionKey, aucMd, pstCipherInfo->ucKeySize );


        /*compute final_server_write_key = MD5(server_write_key 
                                        + ServerHello.random + ClientHello.random);*/
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys compute final_server_write key\n" );)
        iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashInit md5 error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, 
                                     pstConn->pstPendingRead->aucEncryptionKey, 
                                     pstCipherInfo->ucKeyMaterialLen );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 server_write_key  error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucServerRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 serverRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        
        iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucClientRandom, 32 );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys MD5 clientRandom error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys HashEnd error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        WAP_MEMCPY(pstConn->pstPendingRead->aucEncryptionKey, aucMd, pstCipherInfo->ucKeySize );

    
        if( M_WAP_TLS_CIPHER_TYPE_BLOCK == pstCipherInfo->ucType )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys cipher is block,begin compute IV\n" );)
            /*client_write_IV = MD5(ClientHello.random + ServerHello.random);*/
            iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV HashInit MD5 error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }

            iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucClientRandom, 32 );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 clientRandom error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucServerRandom, 32 );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 serverRandom error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 End error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            WAP_MEMCPY( pstConn->pstPendingWrite->aucIV, aucMd, pstCipherInfo->ucIvSize );


            /*server_write_IV = MD5(ServerHello.random + ClientHello.random);*/
            iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 Init error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            
            iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucServerRandom, 32 );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 serverRandom error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            
            iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, pstConn->pstHandshake->taucClientRandom, 32 );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 clientRandom error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }

            iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
            if( iResult < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys calc IV MD5 END error\n" );)
                PROC_INTEERR;
                WAP_FREE( pucKeyBlock );
                return -1;
            }
            WAP_MEMCPY( pstConn->pstPendingRead->aucIV, aucMd, pstCipherInfo->ucIvSize ); 
        }
    }

    
    if( M_WAP_TLS_CIPHER_TYPE_STREAM == pstCipherInfo->ucType )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys cipher is stream\n" );)
        iResult = WapTls_DecryptStreamInit( pstTlsHandle, 
                                            pstCipherInfo->ucCipherAlg, 
                                            pstConn->pstPendingRead->aucEncryptionKey,
                                            pstCipherInfo->ucKeySize,
                                            &(pstConn->pstPendingRead->hCipherHandle)
                                           );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys DecryptStreamInit error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
        
        iResult = WapTls_EncryptStreamInit( pstTlsHandle, 
                                            pstCipherInfo->ucCipherAlg, 
                                            pstConn->pstPendingWrite->aucEncryptionKey,
                                            pstCipherInfo->ucKeySize,
                                            &(pstConn->pstPendingWrite->hCipherHandle)
                                           );
        if( iResult < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys DecryptStreamInit error\n" );)
            PROC_INTEERR;
            WAP_FREE( pucKeyBlock );
            return -1;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateKeys end, return 0\n" );)
    WAP_FREE( pucKeyBlock );
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapSSLHm_CreateClientCertVerify
CREATE DATE: 
    2007-01-04
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Create client cert verify message.
    
ARGUMENTS PASSED:    
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
RETURN VALUE:
    0   :   success
    -1  :   error
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSLHm_CreateClientCertVerify
(
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
)
{
    WE_UCHAR    aucMd[36] = {0};
    WE_HANDLE   hHandle = NULL;
    St_WapTlsHandshake *pstHs = NULL;
    WE_UCHAR    *pucPad1 = NULL;
    WE_UCHAR    *pucPad2 = NULL;
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify Begin\n" );)
    if( NULL == pstConn || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify Handle error\n" );)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if( NULL == pstHs )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify handshake is null\n" );)
        return -1;
    }

    if( !(pstHs->ucFlags & M_WAP_TLS_FLAG_NEED_CERT_VERIFY) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify Don't need certverify\n" );)
        return -1;
    }


    pucPad1 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad1 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad1( pucPad1 );
    pucPad2 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad2)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad2( pucPad2 );

    /*CertificateVerify.signature.md5_hash
                    MD5(master_secret + pad_2 +
                            MD5(handshake_messages + master_secret + pad_1));*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify Calc signature.md5_hash Begin\n" );)
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hVerifyMd5Handle, 
                                    pstConn->pstPendingRead->iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hVerifyMd5Handle, \
                                    pucPad1, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5 pad1 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hVerifyMd5Handle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5 END error\n" );)
        PROC_INTEERR;
        return -1;
    }
    pstHs->hVerifyMd5Handle = NULL;

    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5Init error\n" );)
        PROC_INTEERR;
        return -1;
    }

    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    pstConn->pstPendingRead->iMasterSecretId );
    
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5 Pad2 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd, 16);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5 verifyHandle's result error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MD5End error\n" );)
        PROC_INTEERR;
        return -1;
    }


    /*Certificate.signature.sha_hash
            SHA(master_secret + pad_2 +
                SHA(handshake_messages + master_secret + pad_1));*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify Calc signature.sha_hash Begin\n" );)
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hVerifyShaHandle, 
                                    pstConn->pstPendingRead->iMasterSecretId );
    
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hVerifyShaHandle, \
                                    pucPad1, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify SHA pad1 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hVerifyShaHandle, aucMd + 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify SHAEnd error\n" );)
        PROC_INTEERR;
        return -1;
    }
    pstHs->hVerifyShaHandle = NULL;


    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_SHA1, &hHandle );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify HashInit SHA error\n" );)
        PROC_INTEERR;
        return -1;
    }
    
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    pstConn->pstPendingRead->iMasterSecretId );
    
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify sha Pad2 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd + 16, 20);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify SHA verifyHandle's result error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd + 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify SHAEnd error\n" );)
        PROC_INTEERR;
        return -1;
    }
    
    ISecW_CompSign( pstTlsHandle->hSecHandle, 
                    pstConn->iObjectId,
                    pstHs->pucPubKeyHash,
                    pstHs->usPubKeyHashLen,
                    aucMd,
                    36);

    if( NULL != pucPad1 )
    {
        WAP_FREE( pucPad1 );
    }
    if( NULL != pucPad2 )
    {
        WAP_FREE( pucPad2 );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify END\n" );)
    return 0;
    
}

/*==================================================================================================
FUNCTION: 
    WapSSLHm_CreateClientFinished
CREATE DATE: 
    2007-01-04
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    create client finish PDU
    enum { client(0x434C4E54), server(0x53525652) } Sender;
     struct {
         opaque md5_hash[16];
         opaque sha_hash[20];
     } Finished;

ARGUMENTS PASSED:    
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
RETURN VALUE:
    0   :   success
    -1  :   error
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSLHm_CreateClientFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn
)
{
    WE_UCHAR    aucMd[36] = {0};
    WE_HANDLE   hHandle = NULL;
    WE_INT32    iMasterSecretId = 0;
    St_WapTlsHandshake *pstHs = NULL;
    WE_UCHAR    *pucPad1 = NULL;
    WE_UCHAR    *pucPad2 = NULL;
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished Begin\n" );)
    if( NULL == pstConn || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished parameter error\n" );)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if( NULL == pstHs )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished parameter error\n" );)
        return -1;
    }

    if( NULL != pstConn->pstPendingRead )
    {
        iMasterSecretId = pstConn->pstPendingRead->iMasterSecretId;
    }
    else
    {
        iMasterSecretId = pstConn->stRead.iMasterSecretId;
    }


    pucPad1 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad1 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad1( pucPad1 );
    pucPad2 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad2)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad2( pucPad2 );

    /* md5_hash = MD5(master_secret + pad2 +
                        MD5(handshake_messages + Sender + master_secret + pad1)); */
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hClientMd5Handle, \
                                    (WE_UCHAR*)WAP_SSL_SENDER_CLIENT, 4 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 client const error\n" );)
        PROC_INTEERR;
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hClientMd5Handle, 
                                    iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hClientMd5Handle, \
                                    pucPad1, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 pad1 error\n" );)
        PROC_INTEERR;
        return -1;
    }

    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hClientMd5Handle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 End step1 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    pstHs->hClientMd5Handle = NULL;

   
    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished Init MD5 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 pad2 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd, 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 step1's result error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished MD5 End error\n" );)
        PROC_INTEERR;
        return -1;
    }

    /* calc sha_hash
        sha_hash = SHA(master_secret + pad2 +
                         SHA(handshake_messages + Sender + master_secret + pad1)); */
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hClientShaHandle, \
                                    (WE_UCHAR *)WAP_SSL_SENDER_CLIENT, 4 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA client const error\n" );)
        PROC_INTEERR;
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hClientShaHandle,
                                    iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hClientShaHandle, \
                                    pucPad1, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA pad1 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    
    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hClientShaHandle, aucMd + 16);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA end step1 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    pstHs->hClientShaHandle = NULL;


    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_SHA1, &hHandle);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA init error\n" );)
        PROC_INTEERR;
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA Pad2 error\n" );)
        PROC_INTEERR;
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd + 16, 20 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA step1's result error\n" );)
        PROC_INTEERR;
        return -1;
    }

    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd + 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished SHA Final error\n" );)
        PROC_INTEERR;
        return -1;
    }

    /*create clientFinished success, send it*/
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished: create success\n" );)
    WapTlsHm_ProcSendClientFinished( pstTlsHandle,  pstConn, (WE_UCHAR *)aucMd, 36 );

    
    if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished: full handshake, wait server CCS\n" );)
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS;
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished: TLS opened\n" );)
        WapTlsHm_ProcHandShakeFinished( pstTlsHandle, pstConn);
        TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
            E_WAP_MSG_MODULEID_TLS, 
            M_WAP_TLS_TLSHM_MSG_OPEND, 
            pstConn->iConnId, 
            0, 
            0, 
            0, 
            NULL, 
            NULL);
    }

    if( NULL != pucPad1 )
    {
        WAP_FREE( pucPad1 );
    }
    if( NULL != pucPad2 )
    {
        WAP_FREE( pucPad2 );
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientFinished End,return 0\n" );)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapSSLHm_ProcRecvServerFinished
CREATE DATE: 
    2007-01-04
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Check the serverFinished PDU when receive it.
ARGUMENTS PASSED:    
    St_WapTlsHandle* pstTlsHandle,
    St_WapTlsConn* pstConn
RETURN VALUE:
    0   :   success
    -1  :   error
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapSSLHm_ProcRecvServerFinished
(
    St_WapTlsHandle *pstTlsHandle,
    St_WapTlsConn *pstConn,
    WE_UCHAR *pucMsg,
    WE_UINT32 uiMsgLen    
)
{
    St_WapTlsHandshake  *pstHs = NULL;
    WE_UCHAR    aucMd[36] = {0};
    WE_HANDLE   hHandle = NULL;
    WE_UCHAR    *pucPad1 = NULL;
    WE_UCHAR    *pucPad2 = NULL;
    WE_INT32    iResult = 0;

    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished Begin\n" );)
    if( NULL == pstConn || NULL == pstTlsHandle )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished parameter error\n" );)
        return -1;
    }

    pstHs = pstConn->pstHandshake;
    if( NULL == pstHs )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished parameter error\n" );)
        return -1;
    }

    if( NULL == pucMsg || 36 != uiMsgLen )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished handshake message from server is error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }

    pucPad1 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad1 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad1( pucPad1 );
    pucPad2 = (WE_UCHAR *)WAP_MALLOC( M_WAP_SSL_MD5_PAD_LEN );
    if( NULL == pucPad2)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_CreateClientCertVerify MALLOC fail\n" );)
        return -1;
    }
    WapSSL_GetHashParaPad2( pucPad2 );
    
    /* md5_hash = MD5(master_secret + pad2 +
                        MD5(handshake_messages + Sender + master_secret + pad1)); */
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hServerMd5Handle, \
                                    (WE_UCHAR *)WAP_SSL_SENDER_SERVER, 4 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 server const error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hServerMd5Handle, 
                                    pstConn->stRead.iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hServerMd5Handle, \
                                    pucPad1, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 pad1 error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }

    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hServerMd5Handle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 step1 end error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    pstHs->hServerMd5Handle = NULL;


    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_MD5, &hHandle);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 init step2 error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    pstConn->stRead.iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_MD5_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 pad2 error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd, 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished MD5 step1's result error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }    
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished get MD5's result error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }



    /* sha_hash = SHA(master_secret + pad2 +
                     SHA(handshake_messages + Sender + master_secret + pad1)); */
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hServerShaHandle, \
                                    (WE_UCHAR *)WAP_SSL_SENDER_SERVER, 4 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA server const error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    pstHs->hServerShaHandle, 
                                    pstConn->stRead.iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, pstHs->hServerShaHandle, \
                                    pucPad1, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA pad1 error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }

    iResult = WapTls_HashEnd( pstTlsHandle, pstHs->hServerShaHandle, aucMd + 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA step1 end error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    pstHs->hServerShaHandle = NULL;


    iResult = WapTls_HashInit( pstTlsHandle, E_WE_ALG_HASH_SHA1, &hHandle);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA step2 init error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    ISecW_SslHashUpdateWMasterSec( pstTlsHandle->hSecHandle, 
                                    hHandle, 
                                    pstConn->stRead.iMasterSecretId );
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, \
                                    pucPad2, M_WAP_SSL_SHA_PAD_LEN );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA pad2 error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }
    iResult = WapTls_HashUpdate( pstTlsHandle, hHandle, aucMd + 16, 20 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished SHA step1's result error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }    
    iResult = WapTls_HashEnd( pstTlsHandle, hHandle, aucMd + 16 );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished get SHA's result error\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }

    pstHs->ucFlags |= M_BIT_SERVER_FINISHED;
    pstHs->ucState = M_WAP_TLS_HM_STATE_CHECK_SERVER_FINISHED;

    /*compare the msg returnd by server with the calc result*/
    if( WAP_MEMCMP(aucMd, pucMsg, 36) )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished: compare result isn't same\n" );)
        WapTlsHm_ProcHandShakeErr( pstTlsHandle, pstConn,\
                                    G_WAP_ERR_TLS_HANDSHAKE_FAILURE,\
                                    M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR );
        return -1;
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished:compare result is same\n" );)
    if( pstConn->pstHandshake->ucFlags & M_WAP_TLS_FLAG_FULL_HANDSHAKE )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished:full handshake, tls is open\n" );)
        WapTlsHm_ProcHandShakeFinished( pstTlsHandle, pstConn);
        TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                        E_WAP_MSG_MODULEID_TLS, 
                        M_WAP_TLS_TLSHM_MSG_OPEND, 
                        pstConn->iConnId, 
                        0, 
                        0, 
                        0, 
                        NULL, 
                        NULL);
    }
    else
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished:simple handshake, send client CCS\n" );)
        TlsMsg_PostMsg(pstTlsHandle->hMsgHandle,  
                        E_WAP_MSG_MODULEID_TLS, 
                        M_WAP_TLS_TLSHM_MSG_SEND_CCS, 
                        pstConn->iConnId, 
                        0, 
                        0, 
                        0, 
                        NULL, 
                        NULL);
        pstConn->pstHandshake->ucState = M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED;
    }

    
    if( NULL != pucPad1 )
    {
        WAP_FREE( pucPad1 );
    }
    if( NULL != pucPad2 )
    {
        WAP_FREE( pucPad2 );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapSSLHm_ProcRecvServerFinished:end return 0\n" );)
    return 0;

}

#endif
