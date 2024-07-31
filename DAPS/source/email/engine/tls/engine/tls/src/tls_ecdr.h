/*=====================================================================================
    FILE NAME   : wap_ecdr.h
    MODULE NAME : WAP_ECDR
    
    PRE-INCLUDE FILES DESCRIPTION
        we_def.h
        
    GENERAL DESCRIPTION
        In this file,define the function prototype 
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    Modification Tracking
    Date       Author       Number    Description of changes
    ---------- ------------ --------- --------------------------------------------
    2006-05-18 ChenZhiFeng  none      Init
    2006-08-07 MaZhiqiang   None      Optimization
=====================================================================================*/

/**************************************************************************************
*   Multi-Include-Prevent Section
**************************************************************************************/
#ifndef WAP_ECDR_H
#define WAP_ECDR_H

/**************************************************************************************
*   include File Section
**************************************************************************************/
#include "tls_def.h"

/**************************************************************************************
*   Marco Define Section
**************************************************************************************/

/**************************************************************************************
*   Type Define Section
**************************************************************************************/
/* Encoder, used for encode. */
typedef struct tagSt_WapEcdrEncoder
{
    WE_INT32        iLength;    /* The length of encoded data */
    WE_INT32        iPosition;  /* The current position */
    WE_UCHAR       *pucData;    /* The encoded data */
} St_WapEcdrEncoder;

/* Decoder, used for decode */
typedef struct tagSt_WapEcdrDecoder
{
    WE_INT32        iLength;    /* The length of decoded data */
    WE_INT32        iPosition;  /* The current position */
    WE_UCHAR       *pucData;    /* The decoded data */
} St_WapEcdrDecoder;

/***************************************************************************************
*   Prototype Declare Section
***************************************************************************************/
St_WapEcdrEncoder *TlsEcdr_CreateEncoder
(
    WE_INT32     iLength
);

/* Destroy the encoder structure */
WE_INT32 TlsEcdr_DestroyEncoder
(
    St_WapEcdrEncoder *pstEncoder
);

/* Change the current buffer position of the conversion object */
WE_INT32 TlsEcdr_ChangeEncodePosition
(
    St_WapEcdrEncoder  *pstEncoder,
    WE_INT32            iOffset
);

/* encode fuctions */
WE_INT32 TlsEcdr_EncodeUint8
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UINT8            *pucSrc
);

WE_INT32 TlsEcdr_EncodeInt8
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_INT8             *pcSrc
);

WE_INT32 TlsEcdr_EncodeUint16
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UINT16           *pusSrc
);

WE_INT32 TlsEcdr_EncodeInt16
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_INT16            *psSrc
);

WE_INT32 TlsEcdr_EncodeUint32
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UINT32           *puiSrc
);

WE_INT32 TlsEcdr_EncodeInt32
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_INT32            *piSrc
);

WE_INT32 TlsEcdr_EncodeUintVar
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_UINT32            *puiSrc
);

WE_INT32 TlsEcdr_EncodeOctets
(
    St_WapEcdrEncoder   *pstEncoder,
    WE_CHAR             *pcSrc,
    WE_INT32             iSrcLength
);

/* Create a new Decoder structure */
St_WapEcdrDecoder *TlsEcdr_CreateDecoder
(
    WE_UCHAR    *pucData,
    WE_UINT32    uiDataLength
);

WE_INT32 TlsEcdr_DestroyDecoder
(
    St_WapEcdrDecoder   *pstDecoder
);

/* Change the current buffer position of the conversion object */
WE_INT32 TlsEcdr_ChangeDecodePosition
(
    St_WapEcdrDecoder  *pstDecoder,
    WE_INT32            iOffset
);


/* decode fuctions */
WE_INT32 TlsEcdr_DecodeUint8
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_UINT8            *pucDst
);

WE_INT32 TlsEcdr_DecodeInt8
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_INT8             *pcDst
);

WE_INT32 TlsEcdr_DecodeUint16
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_UINT16           *pusDst
);

WE_INT32 TlsEcdr_DecodeInt16
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_INT16            *psDst
);

WE_INT32 TlsEcdr_DecodeUint32
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_UINT32           *puiDst
);

WE_INT32 TlsEcdr_DecodeInt32
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_INT32            *piDst
);

WE_INT32 TlsEcdr_DecodeUintVar
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_UINT32            *puiDst
);

/* pcDst is Malloced by Invoker */
WE_INT32 TlsEcdr_DecodeOctets
(
    St_WapEcdrDecoder   *pstDecoder,
    WE_CHAR             *pcDst,
    WE_INT32            iDstLength
);

/* Get encode size */
WE_INT32 TlsEcdr_GetEncodeUint8Size( void );

WE_INT32 TlsEcdr_GetEncodeInt8Size( void );

WE_INT32 TlsEcdr_GetEncodeUint16Size( void );

WE_INT32 TlsEcdr_GetEncodeInt16Size( void );

WE_INT32 TlsEcdr_GetEncodeUint32Size( void );

WE_INT32 TlsEcdr_GetEncodeInt32Size( void );

/* Get uint var type length after encode */
WE_INT32 TlsEcdr_GetEncodeUintVarSize
(
    WE_UINT32 *puiSrc
);

WE_INT32 TlsEcdr_GetEncodeOctetsSize
(
    WE_INT32    iSrcLength
);

/* Return the number of bytes remaining in the Encoder data */
WE_INT32 TlsEcdr_GetRemainLength
(
    St_WapEcdrEncoder *pstEncoder
);

#endif  /* end WAP_ECDR_H */

