/*==================================================================================================
    FILE NAME   : wap_tlscvt.c
    MODULE NAME : TLS

    GENERAL DESCRIPTION
       The file offers the public functions for TLS module in order to conversion datas. 
       It provides all type of encode,decode and size functions which are nothing with protocol.   
       
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/

/*==================================================================================================
    Include File Section
==================================================================================================*/
#include "tls_config.h"

#ifdef WAP_CONFIG_TLS
#include "tls_def.h"
#include "tls_dbg.h"

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
* Typedef Declare Section
***************************************************************************************************/
#define WAP_TLS_CVT_MALLOC    1
#define WAP_TLS_CVT_UNMALLOC   0

/**************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
static WE_INT32 WapTls_EncodeSessionId
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsSessionId *pstSessId
);
static WE_INT32 WapTls_DecodeSessionId
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsSessionId *pstSessId
);

static WE_INT32 WapTls_DecodeUcVector
(
    St_WapEcdrDecoder    *pstDecoder,
    WE_INT32             iLength,
    WE_UCHAR              **ppucPointer,
    WE_UINT32              uiFlag
);

static WE_INT32 WapTls_GetUcVectorSize
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_UINT32          uiLength
);

static WE_INT32 WapTls_EncodeStaticUcVector
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32           iLength,
    WE_UCHAR           *pucPointer
);
static WE_INT32 WapTls_DecodeStaticUcVector
(
    St_WapEcdrDecoder  *pstDecoder,
    WE_INT32           iLength,
    WE_UCHAR           *pucPointer
);

static WE_INT32 WapTls_GetStaticUcVectorSize
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32           iLength
);
static WE_INT32 WapTls_EncodeASN1Cert
(
    St_WapEcdrEncoder *pstEncoder,
    St_SecTlsAsn1Certificate  *pstCert
);
static WE_INT32 WapTls_FreeASN1Cert
(
    St_SecTlsAsn1Certificate *pstCert
);

static WE_INT32 WapTls_DecodeASN1Cert
(
    St_WapEcdrDecoder *pstDecoder,
    St_SecTlsAsn1Certificate *pstCert,
    WE_UINT32          uiFlag       
);
static WE_INT32 WapTls_GetASN1CertSize
(
    St_WapEcdrEncoder *pstEncoder,
    St_SecTlsAsn1Certificate *pstCert
);    
static WE_INT32 WapTls_DecodeDN
(
      St_WapEcdrDecoder *pstDecoder,
      St_SecTlsDistingubshedName *pstDN,
      WE_UINT32         uiFlag      
);

static WE_INT32 WapTls_FreeUcVector( WE_UCHAR *pucPointer);


/*==================================================================================================
FUNCTION: 
    WapTls_EncodeInit
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Init a encoder.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder   *pstEncoder : encoder will be initialized
    WE_UCHAR    *pucData    :   data will store in the encoder
    WE_INT32     iLength    :   the length of the data
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
void WapTls_EncodeInit
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UCHAR    *pucData,
    WE_INT32     iLength
)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeInit:Begin\n");)
    if ( NULL == pstEncoder )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeInit:parameter error\n");)
        return ;
    }
    pstEncoder->iLength   = iLength;
    pstEncoder->iPosition = 0;
    pstEncoder->pucData   = pucData;
    return ;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncodeRecord
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode a TLS record layer data.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeRecord(St_WapEcdrEncoder *pstEncoder, St_WapTlsRecord  *pstRecord)
{
    WE_INT32 iRet = -1;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:Begin\n");)
    if (NULL == pstEncoder || NULL  == pstRecord)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:parameter error\n");)
        return -1;
    }

    iRet = TlsEcdr_EncodeUint8(pstEncoder, &(pstRecord->ucContentType));
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:EncodeUint8 Error\n");)
        return -1;
    }
    iRet = TlsEcdr_EncodeUint8(pstEncoder, &(pstRecord->stProVer.ucMajor));
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:EncodeUint8 Error\n");)    
        return -1;
    }
    iRet = TlsEcdr_EncodeUint8(pstEncoder, &(pstRecord->stProVer.ucMinor));
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:EncodeUint8 Error\n");)
        return -1;
    }
    iRet = TlsEcdr_EncodeUint16(pstEncoder, &(pstRecord->usLength));
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:EncodeUint16 Error\n");)
        return -1;
    }
    iRet = WapTls_EncodeUcVector(pstEncoder,pstRecord->usLength, &(pstRecord->pucFragment));
    if (iRet == -1)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:EncodeUcVerctor Error\n");)
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeRecord:End,return 1\n");)
    return 1;
    
}


/*==================================================================================================
FUNCTION: 
    WapTls_EncodeUcVector
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode a unsigned char vector. 
ARGUMENTS PASSED:    
    St_WapEcdrEncoder   *pstEncoder :   pointer to a encoder structure
    WE_INT32            iLength     :   the length of the vector
    WE_CHAR             **ppPointer :   pointer to  store the data encoded    
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeUcVector
(
    St_WapEcdrEncoder *pstEncoder,
    WE_INT32          iLength,
    WE_UCHAR          **ppPointer
)
{
    WE_INT32    iTmp = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeUcVector:Begin\n");)
    if( NULL == pstEncoder || NULL == ppPointer)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeUcVector:parameter error\n");)
        return -1;
    }
    iTmp = TlsEcdr_EncodeOctets(pstEncoder, (WE_CHAR *)*ppPointer, iLength);
    if( iTmp < 0 )
    {
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeUcVector:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeUcVector
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode an unsigned char vector.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder   *pstDecoder :   pointer to a decoder structure
    WE_INT32            iLength     :   the length of the vector
    WE_CHAR             **ppPointer :   pointer to the data to decode
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    Free *ppucPointer
==================================================================================================*/
static WE_INT32 WapTls_DecodeUcVector
(
    St_WapEcdrDecoder    *pstDecoder,
    WE_INT32             iLength,
    WE_UCHAR             **ppucPointer,
    WE_UINT32            uiFlag
)
{
    WE_INT32 iTmp = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:Begin\n");)
    if (NULL == pstDecoder || NULL == ppucPointer)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:parameter error\n");)
        return -1;
    }
    if (0 == iLength)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:RemainLength == 0\n");)
        *ppucPointer = NULL;
        return 1;
    }

    if (uiFlag == WAP_TLS_CVT_UNMALLOC)
    {
        iTmp = pstDecoder->iPosition + iLength;
        if (iTmp > pstDecoder->iLength)
        {
            *ppucPointer = NULL;
            return -1;
        }
        *ppucPointer = pstDecoder->pucData + pstDecoder->iPosition;
        pstDecoder->iPosition += iLength;
        return 1;
    }
    
    /*rember malloc */
    *ppucPointer = (WE_UCHAR*)WAP_MALLOC(iLength);
    if (NULL == *ppucPointer)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:malloc error\n");)
        return -1;
    }
    iTmp = TlsEcdr_DecodeOctets( pstDecoder, (WE_CHAR *)*ppucPointer, iLength);
    if( iTmp < 0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:error,return -1\n");)
        WAP_FREE(*ppucPointer);
        return -1;
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUcVector:return 1\n");)

    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetUcVectorSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Change the Decoder's position as the char vector's length.
ARGUMENTS PASSED:
    St_WapEcdrDecoder   *pstDecoder :
    WE_INT32            iLength     :
    WE_CHAR             **ppPointer :
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_GetUcVectorSize
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_UINT32           uiLength
)
{
    if( NULL == pstEncoder )
    {
        return -1;
    }
    pstEncoder->iPosition += uiLength;
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeUcVector
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Free an unsigned char.
ARGUMENTS PASSED:
    WE_UCHAR     *pucPointer : unsigned char vector to be freed
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_FreeUcVector
(
    WE_UCHAR     *pucPointer
)
{
    if (NULL == pucPointer)
    {
        return -1;
    }
    WAP_FREE(pucPointer);
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncodeStaticUcVector
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode an unsigned char array.
ARGUMENTS PASSED:
    St_WapEcdrEncoder  *pstEncoder  : encoder
    WE_INT32           iLength      : the length of the array
    WE_UCHAR           *pucPointer  : pointer of the array
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_EncodeStaticUcVector
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32           iLength,
    WE_UCHAR           *pucPointer
)
{
    WE_INT32 iRemLen = 0;
    
    if( NULL == pstEncoder || NULL == pucPointer )
    {
        return -1;
    }
    iRemLen = TlsEcdr_GetRemainLength(pstEncoder);
    if( iRemLen < 0 || iRemLen < iLength )
    {
        return -1;
    }
    WAP_MEMCPY( (pstEncoder->pucData + pstEncoder->iPosition), pucPointer, iLength);
    pstEncoder->iPosition += iLength;
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeStaticUcVector
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode the data to an unsigned char array.
ARGUMENTS PASSED:
    St_WapEcdrDecoder  *pstDecoder  : decode that store the data
    WE_INT32           iLength      : the length to decode
    WE_UCHAR           *pucPointer  : to store the decode result
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_DecodeStaticUcVector
(
    St_WapEcdrDecoder  *pstDecoder,
    WE_INT32           iLength,
    WE_UCHAR           *pucPointer
)
{
    WE_INT32 iRemLen = 0;
    
    if( NULL == pstDecoder || NULL == pucPointer )
    {
        return -1;
    }
    iRemLen = pstDecoder->iLength - pstDecoder->iPosition;;
    if( iRemLen < 0 || iRemLen < iLength )
    {
        return -1;
    }
    WAP_MEMCPY(  pucPointer, (pstDecoder->pucData + pstDecoder->iPosition), iLength);
    pstDecoder->iPosition += iLength;
    return 1;
}

/*==================================================================================================
FUNCTION: 
    WapTls_GetStaticUcVectorSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    change the encoder's position as the length passed in.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder  *pstEncoder  : encoder
    WE_INT32           iLength      : the length to move the position
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_GetStaticUcVectorSize
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32           iLength
)
{
    if( NULL == pstEncoder )
    {
        return -1;
    }

    pstEncoder->iPosition += iLength;
    return 1;
}

/*==================================================================================================
FUNCTION: 
    WapTls_EncodeUint24
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode a 24 bit unsigned int
ARGUMENTS PASSED:    
    St_WapEcdrEncoder   *pstEncoder :
    WE_UINT32           *puiPointer :
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeUint24
(
    St_WapEcdrEncoder  *pstEncoder,
    M_WAP_TLS_UINT24   *puiPointer
)
{
    WE_INT32 iRemLen = 0;
    
    if( NULL == pstEncoder || NULL == puiPointer)
    {
        return -1;
    }
    iRemLen = TlsEcdr_GetRemainLength(pstEncoder);
    if( iRemLen < 3)
    {
        return -1;
    }
    
    pstEncoder->pucData[pstEncoder->iPosition] = (WE_UCHAR)((*puiPointer>>16) & 0xFF);
    pstEncoder->pucData[pstEncoder->iPosition + 1] = (WE_UCHAR)((*puiPointer>>8 ) & 0xFF);
    pstEncoder->pucData[pstEncoder->iPosition + 2] = (WE_UCHAR)(*puiPointer & 0xFF);
    pstEncoder->iPosition += 3;

    return 1;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeUint24
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode a 24 bit unsigned int
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder   :   Decoder
    M_WAP_TLS_UINT24  *ptPointer    :   pointer to an unsigned 24bit int.
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_DecodeUint24
(
    St_WapEcdrDecoder *pstDecoder,
    M_WAP_TLS_UINT24  *ptPointer
)
{
    WE_UINT8 ucTmp1 = '\0';
    WE_UINT8 ucTmp2 = '\0';
    WE_UINT8 ucTmp3 = '\0';

    WE_INT32 iRemLen = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUint24:Begin\n");)
    if( NULL == pstDecoder || NULL == ptPointer )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUint24:Parameter error\n");)
        return -1;
    }
    iRemLen = pstDecoder->iLength - pstDecoder->iPosition;
    if( iRemLen < 3)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUint24:Parameter error\n");)
        return -1;
    }
    ucTmp1 = pstDecoder->pucData[pstDecoder->iPosition];
    ucTmp2 = pstDecoder->pucData[pstDecoder->iPosition + 1];
    ucTmp3 = pstDecoder->pucData[pstDecoder->iPosition + 2];

    pstDecoder->iPosition += 3;

    /*0x00 add for avoid a expected value of first byte of the puiPointer*/
    *ptPointer =( 0x00 | ((M_WAP_TLS_UINT24)ucTmp1 << 16) \
                        | ((M_WAP_TLS_UINT24)ucTmp2 << 8)\
                        | (M_WAP_TLS_UINT24)ucTmp3 );
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeUint24:End\n");)
    return 1;

}

/*==================================================================================================
FUNCTION: 
    WapTls_EncodeSessionId
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode TLS record
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsSessionId *pstSessId
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_EncodeSessionId
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsSessionId *pstSessId
)
{
    WE_INT32 iTmp = 0;
    
    if( NULL == pstEncoder || NULL == pstSessId )
    {
        return -1;
    }
    iTmp = TlsEcdr_EncodeUint8(pstEncoder,&pstSessId->ucLength);
    if (iTmp == -1)
    {
        return -1;
    }
    
    iTmp = WapTls_EncodeStaticUcVector(pstEncoder, pstSessId->ucLength, (WE_UCHAR *)pstSessId->aucId);
    if (iTmp == -1)
    {
        return -1;
    }
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeSessionId
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode TLS record
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsSessionId *pstSessId
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_DecodeSessionId
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsSessionId *pstSessId
)
{
    WE_INT32 iTmp = -1;
    
    if( NULL == pstDecoder || NULL == pstSessId )
    {
        return -1;
    }
    iTmp = TlsEcdr_DecodeUint8(pstDecoder,&pstSessId->ucLength);
    if (-1 == iTmp)
    {
        return -1;
    }
    iTmp = WapTls_DecodeStaticUcVector(pstDecoder,(WE_INT32)pstSessId->ucLength , pstSessId->aucId);
    if (-1 == iTmp)
    {
        return -1;
    }
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetSessionIdSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Change the encoder's position as the size of session id.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder  *pstEncoder,
    St_WapTlsSessionId *pstSessId
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_GetSessionIdSize
(
    St_WapEcdrEncoder  *pstEncoder,
    St_WapTlsSessionId *pstSessId
)
{
    WE_INT32 iTmp = 0;
    if( NULL == pstEncoder || NULL == pstSessId )
    {
        return -1;
    }

    pstEncoder->iPosition += 1;
    iTmp = WapTls_GetStaticUcVectorSize(pstEncoder,pstSessId->ucLength);
    if( iTmp < 0 )
    {
        return -1;
    }
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncodeProVer
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:

ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsProVer   *pstProVer
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_EncodeProVer
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsProVer   *pstProVer
)
{
    WE_INT32 iTmp = 0;
    if( NULL == pstEncoder || NULL  == pstProVer)
    {
        return -1;
    }
    iTmp = TlsEcdr_EncodeUint8( pstEncoder, &pstProVer->ucMajor );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = TlsEcdr_EncodeUint8( pstEncoder, &pstProVer->ucMinor );
    if( iTmp < 0 )
    {
        return -1;
    }
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeProVer
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode TLS record
ARGUMENTS PASSED:    

RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_DecodeProVer(St_WapEcdrDecoder *pstDecoder,St_WapTlsProVer *pstProVer)
{
    WE_INT32 iTmp = 0;
    if( NULL == pstDecoder || NULL  == pstProVer)
    {
        return -1;
    }
    iTmp = TlsEcdr_DecodeUint8( pstDecoder, &pstProVer->ucMajor );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = TlsEcdr_DecodeUint8( pstDecoder, &pstProVer->ucMinor );
    if( iTmp < 0 )
    {
        return -1;
    }
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetProVerSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:

ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_GetProVerSize(St_WapEcdrEncoder *pstEncoder,St_WapTlsProVer *pstProVer)
{
    if( NULL == pstEncoder || NULL  == pstProVer)
    {
        return -1;
    }
    pstEncoder->iPosition += 2;/*encode pstProVer->ucMajor += 1,encode pstProVer->ucMinor += 1*/
    return 1;

}
/*==================================================================================================
FUNCTION: 
    
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode a handshake layer clientHello PDU.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeClientHello
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientHello *pstClientHello
)
{
    WE_INT32 iTmp = 0 ;
    if( NULL == pstEncoder || NULL == pstClientHello )
    {
        return -1;
    }
    iTmp = WapTls_EncodeProVer( pstEncoder, &(pstClientHello->stClientVer) );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = WapTls_EncodeStaticUcVector( pstEncoder, 32, (WE_UCHAR *)pstClientHello->taucRandom);
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = WapTls_EncodeSessionId( pstEncoder, &(pstClientHello->stSessionId) );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = TlsEcdr_EncodeUint16( pstEncoder, &(pstClientHello->usCipherSuitLen) );
    if( iTmp < 0 )
    {
        return -1;
    }
    
    iTmp = WapTls_EncodeUcVector(pstEncoder, pstClientHello->usCipherSuitLen, 
                                    &(pstClientHello->pucCipherSuit));
    if( iTmp < 0 )
    {
        return -1;
    }
    
    iTmp = TlsEcdr_EncodeUint8(pstEncoder, &(pstClientHello->ucNumCompressMethods) );
    if( iTmp < 0 )
    {
        return -1;
    }

    iTmp = WapTls_EncodeStaticUcVector(pstEncoder, pstClientHello->ucNumCompressMethods, \
                                        (WE_UCHAR *)pstClientHello->aucCompressMethods );
    if( iTmp < 0 )
    {
        return -1;
    }
    
    return 1;
    
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetClientHelloSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
     Change the encoder's position as the size of clientHello to get the size of it.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_GetClientHelloSize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientHello *pstClientHello
)
{
    WE_INT32 iTmp = 0 ;
    if( NULL == pstEncoder || NULL == pstClientHello )
    {
        return -1;
    }
    iTmp = WapTls_GetProVerSize( pstEncoder, &(pstClientHello->stClientVer) );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = WapTls_GetStaticUcVectorSize( pstEncoder, 32 );
    if( iTmp < 0 )
    {
        return -1;
    }
    iTmp = WapTls_GetSessionIdSize( pstEncoder, &(pstClientHello->stSessionId) );
    if( iTmp < 0 )
    {
        return -1;
    }
    
    pstEncoder->iPosition += 2;/*size of pstClientHello->usCipherSuitLen*/

    iTmp = WapTls_GetUcVectorSize(pstEncoder, (WE_UINT32)pstClientHello->usCipherSuitLen );
    if( iTmp < 0 )
    {
        return -1;
    }
    
    pstEncoder->iPosition += 1;/*size of pstClientHello->ucNumCompressMethods*/
    
    iTmp = WapTls_GetStaticUcVectorSize(pstEncoder, pstClientHello->ucNumCompressMethods );
    if( iTmp < 0 )
    {
        return -1;
    }
    
    return 1;

}

/*==================================================================================================
FUNCTION: 
    WapTls_DecodeServerHello
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    decode a handshake layer server hello PDU.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsServerHello *pstSerHello
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_DecodeServerHello
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsServerHello *pstSerHello
)
{
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:Begin\n");)
    if( NULL == pstDecoder || NULL == pstSerHello )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:Parameter error\n");)
        return -1;
    }
    
    iResult = WapTls_DecodeProVer( pstDecoder, &pstSerHello->stServerVer );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:Decode proVer error\n");)
        return -1;
    }
    iResult = WapTls_DecodeStaticUcVector( pstDecoder, 32, pstSerHello->taucRandom );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:TlsEcdr_DecodeOctets error\n");)
        return -1;
    }
    iResult = WapTls_DecodeSessionId( pstDecoder, &pstSerHello->stSessionId );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:Decode session id error\n");)
        return -1;
    }
    iResult = WapTls_DecodeStaticUcVector( pstDecoder,2, pstSerHello->taucCipherSuite);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:Decode ciphersuite error\n");)
        return -1;
    }
    iResult = TlsEcdr_DecodeUint8( pstDecoder, &pstSerHello->ucCompressMethod );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:DecodeUint8 error\n");)
        return -1;
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerHello:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    
CREATE DATE: 
    2007-01-10
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:

ARGUMENTS PASSED:    

RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_EncodeASN1Cert
(
    St_WapEcdrEncoder *pstEncoder,
    St_SecTlsAsn1Certificate  *pstCert
)
{
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Begin\n");)
    if( NULL == pstEncoder || NULL == pstCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Parameter error\n");)
        return -1;
    }
    
    iResult = WapTls_EncodeUint24( pstEncoder, &pstCert->uiCertLen );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Decode uint24 error\n");)
        return -1;
    }
    iResult = WapTls_EncodeUcVector(pstEncoder,pstCert->uiCertLen,&(pstCert->pucCert));    
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:TlsEcdr_DecodeOctets error\n");)
        return -1;
    }
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_GetASN1CertSize
CREATE DATE: 
    2007-01-10
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Change the encoder's position as the size of ASN1 cert.
ARGUMENTS PASSED:
    St_WapEcdrEncoder        *pstEncoder : Encoder
    St_SecTlsAsn1Certificate *pstCert    : pointer to the ASN1 cert
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_GetASN1CertSize
(
    St_WapEcdrEncoder        *pstEncoder,
    St_SecTlsAsn1Certificate *pstCert
)
{
    WE_INT32    iTmp = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetASN1CertSize:Begin\n");)
    if( NULL == pstEncoder || NULL == pstCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetASN1CertSize:Parameter error\n");)
        return -1;
    }

    pstEncoder->iPosition += 3;/*the size of cert's length which is uint24 */

    iTmp = WapTls_GetUcVectorSize( pstEncoder, pstCert->uiCertLen );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetASN1CertSize:encode cert size error\n");)
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetASN1CertSize:End\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_DecodeASN1Cert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Convert an ASN1 cert as the net's order.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder        *pstDecoder : store the decode result 
    St_SecTlsAsn1Certificate *pstCert    : ASN1 cert
    WE_UINT32                uiFlag      : whether malloc flag
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_DecodeASN1Cert
(
    St_WapEcdrDecoder        *pstDecoder,
    St_SecTlsAsn1Certificate *pstCert,
    WE_UINT32                uiFlag       
) 
{
    WE_INT32    iResult = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Begin\n");)
    if( NULL == pstDecoder || NULL == pstCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Parameter error\n");)
        return -1;
    }
    
    iResult = WapTls_DecodeUint24( pstDecoder, &pstCert->uiCertLen );
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:Decode uint24 error\n");)
        return -1;
    }
    /*pstCert->pucCert is MALLOC in this function if uiFlag is M_WAP_TLS_CVT_MALLOC*/
    iResult = WapTls_DecodeUcVector(pstDecoder,pstCert->uiCertLen,&(pstCert->pucCert),uiFlag);
    if( iResult < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:TlsEcdr_DecodeOctets error\n");)
        return -1;
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeASN1Cert:End\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_FreeASN1Cert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Free the ASN1 certificate
ARGUMENTS PASSED:    
    St_SecTlsAsn1Certificate *pstCert : pointer to the ASN1 cert to be freed
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_FreeASN1Cert
(
    St_SecTlsAsn1Certificate *pstCert
)
{
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeASN1Cert:Begin\n");)
    if( NULL == pstCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeASN1Cert:Parameter error\n");)
        return -1;
    }
    if( NULL != pstCert->pucCert )
    {
        WAP_FREE( pstCert->pucCert );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeASN1Cert:End\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeServerCert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode the handshake layer serverCert PDU.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder   :   decoder
    St_WapTlsServerCert *pstCert    :   server's cert
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_DecodeServerCert
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsServerCert *pstCert
)    
{
    M_WAP_TLS_UINT24      tCertListLen = 0;
    WE_UINT32             iSum = 0;
    WE_UINT32             i = 0;
    WE_INT32              iTmp = 0;
    St_SecTlsAsn1Certificate     stTmpCert = {0};
    St_WapEcdrDecoder  stDecoder = {0};

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:Begin\n");)
    if( NULL == pstDecoder || NULL == pstCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:parameter error\n");)
        return -1;
    }
    iTmp = WapTls_DecodeUint24( pstDecoder, &tCertListLen );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:decode cert list length error\n");)
        return -1;
    }
    /*get certs' number*/
    stDecoder = *pstDecoder;
    for( i = 0; iSum < tCertListLen; i++ )
    {
        iTmp = WapTls_DecodeASN1Cert( &stDecoder, &stTmpCert,WAP_TLS_CVT_UNMALLOC);
        if( iTmp < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:get cert number error\n");)
            return -1;
        }
        iSum += ( 3 + stTmpCert.uiCertLen );/*'3' is the byte of uiCertLen that is uint24*/
    }
    pstCert->ucCertNum = (WE_UINT8)i;
    
    if( 0 == pstCert->ucCertNum )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:decode cert number is 0\n");)
        pstCert->pstCert = NULL;
    }
    else
    {
        iTmp = (pstCert->ucCertNum) * sizeof(St_SecTlsAsn1Certificate);
        pstCert->pstCert = (St_SecTlsAsn1Certificate *)WAP_MALLOC( iTmp );
        if( NULL == pstCert->pstCert )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:malloc error\n");)
            return -1;
        }
        WAP_MEMSET(pstCert->pstCert, 0, iTmp);
        for( i = 0; i < pstCert->ucCertNum; i++ )
        {
            iTmp = WapTls_DecodeASN1Cert(pstDecoder, &(pstCert->pstCert[i]),WAP_TLS_CVT_MALLOC );
            if( iTmp < 0)
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:decode ASN1 cert error\n");)
                return -1;
            }
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerCert:End\n");)    
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeServerCert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Free the member of a serverCert structure and itself.
ARGUMENTS PASSED:    
    St_WapTlsServerCert *pstCert    :     the serverCert will be freed
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_FreeServerCert(St_WapTlsServerCert *pstCert)  
{
    WE_INT32 i = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerCert:Begin\n");)
    if( NULL == pstCert )
    {
        return -1;
    }
    for( i = 0; i < pstCert->ucCertNum; i++)
    {
        WAP_FREE(pstCert->pstCert[i].pucCert);
    }
    WAP_FREE(pstCert->pstCert);
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerCert:end\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeServerKeyExch
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode the handshake layer serverKeyExch PDU.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder,
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_DecodeServerKeyExch
(
    St_WapEcdrDecoder *pstDecoder,
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc
)
{
    WE_INT32   iTmp = 0 ;
    WE_UINT32              uiFlag = WAP_TLS_CVT_MALLOC;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:Begin\n");)
    if( NULL == pstDecoder || NULL == pstKeyExc )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:parameter error\n");)
        return -1;
    }
    switch( iKeyExcAlg )
    {
        case G_SEC_TLS_KEYEXCH_RSA_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:case RSA export\n");)
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.usModLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                            pstKeyExc->stParams.stRSAParam.stTlsRSAParam.usModLen,\
                                            &(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.pucModulus),
                                            uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.usExpLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                            pstKeyExc->stParams.stRSAParam.stTlsRSAParam.usExpLen,\
                                            &(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.pucExponent),
                                            uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stRSAParam.stTlsSignature.usSignLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stRSAParam.stTlsSignature.usSignLen,\
                                         &(pstKeyExc->stParams.stRSAParam.stTlsSignature.pucSignature),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            break;
        case G_SEC_TLS_KEYEXCH_DHE_DSS:
        case G_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT:
        case G_SEC_TLS_KEYEXCH_DHE_RSA:
        case G_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:case DHE\n");)
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHPLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHPLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHP),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHGLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHGLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHG),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHYsLen));
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHYsLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHYs),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
             iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsSignature.usSignLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsSignature.usSignLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsSignature.pucSignature),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            break;
        case G_SEC_TLS_KEYEXCH_DH_ANON:
        case G_SEC_TLS_KEYEXCH_DH_ANON_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:case DH ANON\n");)
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHPLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHPLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHP),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHGLen) );
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHGLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHG),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = TlsEcdr_DecodeUint16(pstDecoder, \
                                        &(pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHYsLen));
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_DecodeUcVector(pstDecoder, \
                                         pstKeyExc->stParams.stDHParam.stTlsDHParam.usDHYsLen,\
                                         &(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHYs),
                                         uiFlag);
            if( iTmp < 0 )
            {
                return -1;
            }
            pstKeyExc->stParams.stDHParam.stTlsSignature.pucSignature = NULL;
            pstKeyExc->stParams.stDHParam.stTlsSignature.usSignLen = 0;
            break;
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:default\n");)
            return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeServerKeyExch:End, return 0\n");)
    return 0;
}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeServerKeyExch
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
     Free the member of a serverKeyExchange structure and itself.
ARGUMENTS PASSED:    
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc    
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_FreeServerKeyExch
(
    WE_INT32 iKeyExcAlg,
    St_WapTlsServerKeyExch *pstKeyExc    
)
{
    WE_INT32   iTmp = 0 ;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:Begin\n");)
    if( NULL == pstKeyExc )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:parameter error\n");)
        return -1;
    }
    switch( iKeyExcAlg )
    {
        case G_SEC_TLS_KEYEXCH_RSA_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:case RSA export\n");)
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.pucModulus);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stRSAParam.stTlsRSAParam.pucExponent);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stRSAParam.stTlsSignature.pucSignature);
            if( iTmp < 0 )
            {
                return -1;
            }
            break;
        case G_SEC_TLS_KEYEXCH_DHE_DSS:
        case G_SEC_TLS_KEYEXCH_DHE_DSS_EXPORT:
        case G_SEC_TLS_KEYEXCH_DHE_RSA:
        case G_SEC_TLS_KEYEXCH_DHE_RSA_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:case DHE\n");)
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHP);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHG);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHYs);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsSignature.pucSignature);
            if( iTmp < 0 )
            {
                return -1;
            }
            break;
        case G_SEC_TLS_KEYEXCH_DH_ANON:
        case G_SEC_TLS_KEYEXCH_DH_ANON_EXPORT:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:case DH ANON\n");)
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHP);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHG);
            if( iTmp < 0 )
            {
                return -1;
            }
            iTmp = WapTls_FreeUcVector(pstKeyExc->stParams.stDHParam.stTlsDHParam.pucDHYs);
            if( iTmp < 0 )
            {
                return -1;
            }
            break;
        default:
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:default\n");)
            return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeServerKeyExch:End, return 0\n");)
    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapTls_DecodeCertReq
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode the handshake layer serverCertRequest PDU.
    Convert the data from the net to fill the cert request structure.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsCertReq *pstCertReq  
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_DecodeCertReq
(
    St_WapEcdrDecoder *pstDecoder,
    St_WapTlsCertReq *pstCertReq
)
{
    WE_INT32 i = 0;
    WE_INT32 iSum = 0;
    WE_INT32 iTmp = 0;
    WE_UINT16 usCertAuthLen = 0;
    St_WapEcdrDecoder  stDecoder = {0};
    St_SecTlsDistingubshedName   stDNTmp = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:Begin\n");)
    if( NULL == pstDecoder || NULL == pstCertReq )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:parameter error\n");)
        return -1;
    }

    iTmp = TlsEcdr_DecodeUint8( pstDecoder, &(pstCertReq->ucCertTypeNum) );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_DecodeCertReq:Num of cert type %d\n"),pstCertReq->ucCertTypeNum);)
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:decode certTypeNum error\n");)
        return -1;
    }
    
    iTmp = WapTls_DecodeUcVector( pstDecoder, pstCertReq->ucCertTypeNum,
                                            &(pstCertReq->pucCertType),
                                            WAP_TLS_CVT_MALLOC);
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:decode certType error\n");)
        return -1;
    }
    iTmp = TlsEcdr_DecodeUint16( pstDecoder, &usCertAuthLen );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:decode DNs length error\n");)
        return -1;
    }

    /*Get the number of the DNs*/
    stDecoder = *pstDecoder;
    for( i = 0; iSum < usCertAuthLen; i++ )
    {
        iTmp = WapTls_DecodeDN(&stDecoder, &stDNTmp,WAP_TLS_CVT_UNMALLOC);
        if( iTmp < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:decode DN error\n");)
            return -1;
        }
        iSum += ( 2+ stDNTmp.usNameLen );
    }
    pstCertReq->ucCertAuthNum = (WE_UINT8) i;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_DecodeCertReq:DN num is %d\n"),i);)
    if( 0 == pstCertReq->ucCertAuthNum )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:Cert nember is 0\n");)
        pstCertReq->pstDN = NULL;
    }
    else
    {
        iTmp = pstCertReq->ucCertAuthNum * sizeof( St_SecTlsDistingubshedName );
        pstCertReq->pstDN = (St_SecTlsDistingubshedName *)WAP_MALLOC( iTmp );
        if (NULL == pstCertReq->pstDN)
        {
            return -1;
        }

        for( i = 0; i < pstCertReq->ucCertAuthNum; i++)
        {
            iTmp = WapTls_DecodeDN( pstDecoder, &(pstCertReq->pstDN[i]),WAP_TLS_CVT_MALLOC);
            if( iTmp < 0 )
            {
                WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:Decode DN error\n");)
                return -1;
            }
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeCertReq:End\n");)
    return 1;
}


/*==================================================================================================
FUNCTION: 
    WapTls_FreeCertReq
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    free the cert request structure and it's member
ARGUMENTS PASSED:    
    St_WapTlsCertReq *pstCertReq    : pointer to the cert request structure
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_FreeCertReq(St_WapTlsCertReq *pstCertReq )
{
    WE_INT32 i = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeCertReq:Begin\n");)
    if( NULL == pstCertReq )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeCertReq:parameter error\n");)
        return -1;
    }
    WAP_FREE( pstCertReq->pucCertType );

    for( i = 0; i < pstCertReq->ucCertAuthNum; i++)
    {
        WAP_FREE( pstCertReq->pstDN[i].pucName);
    }
    WAP_FREE( pstCertReq->pstDN );
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeCertReq:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_DecodeDN
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Decode the Distinguished Name.
ARGUMENTS PASSED:    
    St_WapEcdrDecoder *pstDecoder,
    St_SecTlsDistingubshedName *pstDN   : The pointer that will be filled.
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
static WE_INT32 WapTls_DecodeDN
(
    St_WapEcdrDecoder *pstDecoder,
    St_SecTlsDistingubshedName *pstDN,
    WE_UINT32        uiFlag
)
{
    WE_INT32 iTmp = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeDN:Begin\n");)
    if( NULL == pstDecoder || NULL == pstDN)
    {
        return -1;
    }
    iTmp = TlsEcdr_DecodeUint16( pstDecoder, &pstDN->usNameLen);
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeDN:Decode DN name error\n");)
        return -1;
    }
    /*
         if uiFlag is M_WAP_TLS_CVT_MALLOC, pstDN->pucName Free in WapTls_FreeCertReq
    */
    iTmp = WapTls_DecodeUcVector( pstDecoder, pstDN->usNameLen, &pstDN->pucName,uiFlag);
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeDN:Decode DN \n");)
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_DecodeDN:End\n");)
    return 1;
}

/*==================================================================================================
FUNCTION: 
    WapTls_EncodeClientCert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode the client's cert as the net's order.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeClientCert
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
)
{
    M_WAP_TLS_UINT24 tCertListLen = 0;
    WE_INT32 i = 0;
    WE_INT32 iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeClientCert:Begin\n");)
    if( NULL == pstEncoder || NULL == pstClientCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeClientCert:parameter error\n");)
        return -1;
    }
    for( i = 0; i < pstClientCert->ucCertNum; i++ )
    {
        /*3 is the bytes of cert length*/
        tCertListLen += ( pstClientCert->pstCert[i].uiCertLen + 3 );
    }

    iTmp = WapTls_EncodeUint24(pstEncoder, &tCertListLen);/*tCertListLen is 0 when no cert was found*/
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeClientCert:encode certListLen error\n");)
        return -1;
    }
    for( i = 0; i < pstClientCert->ucCertNum; i++ )
    {
        iTmp = WapTls_EncodeASN1Cert(pstEncoder, (St_SecTlsAsn1Certificate *)&pstClientCert->pstCert[i] );
        if( iTmp < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)(("WapTls_EncodeClientCert:encode cert %d error\n"),i);)
            return -1;
        }
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeClientCert:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetClientCertSize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Change the encoder's position as the size of client cert.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_GetClientCertSize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsClientCert *pstClientCert
)
{
    WE_INT32 i = 0;
    WE_INT32 iTmp = 0;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetClientCertSize:Begin\n");)
    if( NULL == pstEncoder || NULL == pstClientCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetClientCertSize:parameter error\n");)
        return -1;
    }

    pstEncoder->iPosition += 3;/*the size of cert's number which is uint24*/

    for( i = 0; i < pstClientCert->ucCertNum; i++ )
    {
        iTmp = WapTls_GetASN1CertSize(pstEncoder, (St_SecTlsAsn1Certificate *)&pstClientCert->pstCert[i] );
        if( iTmp < 0 )
        {
            return -1;
        }
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetClientCertSize:End\n");)
    return 1;

}
/*==================================================================================================
FUNCTION: 
    WapTls_FreeClientCert
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    free the client cert structure and it's member
ARGUMENTS PASSED:    
    St_WapTlsClientCert *pstClientCert
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_FreeClientCert
(
    St_WapTlsClientCert *pstClientCert
)
{
    WE_INT32 iTmp = 0;
    WE_INT32 i = 0;
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeClientCert:Begin\n");)
    if( NULL == pstClientCert )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeClientCert:Begin\n");)
        return -1;
    }
    for (i = 0; i < pstClientCert->ucCertNum; i++) 
    {
        iTmp = WapTls_FreeASN1Cert( &(pstClientCert->pstCert[i]) );
        if( iTmp < 0 )
        {
            return -1;
        }
    }
    if( NULL != pstClientCert->pstCert )
    {
        WAP_FREE( pstClientCert->pstCert );
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_FreeClientCert:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_EncodeCertVerify
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode a client cert verify PDU.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeCertVerify
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsCertVerify *pstClientCert
)
{
    WE_INT32 iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertVerify:Begin\n");)
    if( NULL == pstEncoder || NULL == pstClientCert)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertVerify:parameter error\n");)
        return -1;
    }

    iTmp = TlsEcdr_EncodeUint16( pstEncoder, &pstClientCert->usSiglen );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertVerify:encode signature length error\n");)
        return -1;
    }
    
    iTmp = WapTls_EncodeUcVector( pstEncoder, pstClientCert->usSiglen,\
                                        &(pstClientCert->pucSignature) );
    
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertVerify:encode signature error\n");)
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertVerify:End\n");)
    return 1;
}
/*==================================================================================================
FUNCTION: 
    WapTls_GetCertVerifySize
CREATE DATE: 
    2006-12-14
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Change the encoder's position as the size of client cert verify.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder *pstEncoder:
    St_WapTlsRecord   *pstRecord:     
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_GetCertVerifySize
(
    St_WapEcdrEncoder *pstEncoder,
    St_WapTlsCertVerify *pstClientCert
)
{
    WE_INT32 iTmp = 0;

    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetCertVerifySize:Begin\n");)
    if( NULL == pstEncoder || NULL == pstClientCert)
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetCertVerifySize:parameter error\n");)
        return -1;
    }

    pstEncoder->iPosition += 2; /*Encode size of pstClientCert->usSiglen that is uint16*/
    iTmp = WapTls_GetUcVectorSize (pstEncoder, (WE_UINT32)pstClientCert->usSiglen );
    
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetCertVerifySize:get signature size error\n");)
        return -1;
    }
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_GetCertVerifySize:End\n");)
    return 1;

}
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE

/*==================================================================================================
FUNCTION: 
    WapTls_EncodeCertReq
CREATE DATE: 
    2007-5-18
AUTHOR: 
    Ma Zhiqiang
DESCRIPTION:
    Encode cert request.
ARGUMENTS PASSED:    
    St_WapEcdrEncoder  *pstEncoder,
    St_WapTlsCertReq   *pstCertReq
RETURN VALUE:
    -1  :   Error
    1   :   Success
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None      
CALL BY:
    Omit.
IMPORTANT NOTES:
    None.
==================================================================================================*/
WE_INT32 WapTls_EncodeCertReq
(
    St_WapTlsCertReq   *pstCertReq,
    WE_CHAR            **ppcBuffer,
    WE_INT32  *piLen
)
{  
    WE_INT32 i = 0;
    WE_INT32 iTmp = 0;
    WE_INT32 iLength = 0;
    St_WapEcdrEncoder stEncoder = {0};
    
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Begin\n");)
    if( NULL == ppcBuffer || NULL == pstCertReq )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:parameter error\n");)
        return -1;
    }
    for( i = 0; i < pstCertReq->ucCertAuthNum; i++)
    {
        iLength += 4 + pstCertReq->pstDN[i].usNameLen;
    }
    iLength += 2 + pstCertReq->ucCertTypeNum + 2;

    stEncoder.iLength = 4 + iLength;
    stEncoder.iPosition = 0;
    stEncoder.pucData = (WE_UCHAR *)WAP_MALLOC(stEncoder.iLength);
    
    WAP_MEMSET(stEncoder.pucData, 0, stEncoder.iLength);
    
    iTmp = TlsEcdr_EncodeInt32( &stEncoder, &(iLength) );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode length error\n");)
        return -1;
    }

    
    iTmp = TlsEcdr_EncodeUint8( &stEncoder, &(pstCertReq->ucCertTypeNum) );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode certTypeNum error\n");)
        return -1;
    }

    iTmp = WapTls_EncodeUcVector( &stEncoder, pstCertReq->ucCertTypeNum,&(pstCertReq->pucCertType));
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode certType error\n");)
        return -1;
    }
    
    iTmp = TlsEcdr_EncodeUint8( &stEncoder, &(pstCertReq->ucCertAuthNum) );
    if( iTmp < 0 )
    {
        WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode DNs length error\n");)
        return -1;
    }

    for( i = 0; i < pstCertReq->ucCertAuthNum; i++)
    {
        iTmp = TlsEcdr_EncodeUint16( &stEncoder, &(pstCertReq->pstDN[i].usNameLen) );
        if( iTmp < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode DN error\n");)
            return -1;
        }
        iTmp = WapTls_EncodeUcVector( &stEncoder, pstCertReq->pstDN[i].usNameLen,&(pstCertReq->pstDN[i].pucName));
        if( iTmp < 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:Encode DN error\n");)
            return -1;
        }
    }
    *ppcBuffer = (WE_CHAR *)stEncoder.pucData;
    *piLen = stEncoder.iLength;
    WAP_LOG_PRINT((E_WAP_DBG_TLS,E_WAP_DBG_LEVEL_LOW)("WapTls_EncodeCertReq:End\n");)
    return 1;

}
#endif
#endif

