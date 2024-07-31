/*==================================================================================================
    FILE NAME : sec_ibrs.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the function prototype , to support the browser engine and
        browser application.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                             Tracking
    Date          Author                   Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-06-29   Kevin Yang                            create sec_ibrs.h
    2007-05-17     tang                                 Delete cert manage and signtext
    2007-06-19    bluefire                         four bytes align for dragonfly
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef _SEC_IBRS_H
#define _SEC_IBRS_H

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#define M_SEC_CERTIFICATE_WTLS_TYPE       0x01
#define M_SEC_CERTIFICATE_X509_TYPE       0x02


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef St_SecCrptPubKeyRsa St_SecCrptPubKey;
typedef struct tagSt_SecCrptPrivKey
{    
    WE_UCHAR    * pucBuf;
    WE_UINT16    usBufLen;
    WE_UINT16    usReserved; /* four bytes align */
}St_SecCrptPrivKey;

typedef struct tagSt_SecCrptEncryptPkcResp
{
    WE_INT32    iTargetID;      
    WE_UCHAR    * pucBuf;
    WE_INT16    sBufLen;
    WE_INT16    sResult;
}St_SecCrptEncryptPkcResp;

typedef struct tagSt_SecCrptVerifySignatureResp
{
    WE_INT32    iTargetID;
    WE_INT16    sResult;
    WE_UINT16    usReserved; /* four bytes align */
}St_SecCrptVerifySignatureResp;
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
typedef struct tagSt_SecCrptComputeSignatureResp
{
    WE_INT32    iTargetID;    
    WE_UCHAR    * pucSig;
    WE_INT16    sSigLen;
    WE_INT16    sResult;/* four bytes align */
}St_SecCrptComputeSignatureResp;
#endif
#ifdef M_SEC_CFG_WTLS
typedef struct tagSt_SecViewCurCert
{
    WE_INT32 iTargetID;
    WE_INT32 iSecurityId;
}St_SecViewCurCert;

typedef struct tagSt_SecViewSessionInfo
{
    WE_INT32 iTargetID;
    WE_INT32 iSecurityId;
}St_SecViewSessionInfo;

typedef struct tagSt_SecGetSecCurClass
{
    WE_INT32 iTargetID; 
    WE_INT32 iSecurityId;
}St_SecGetSecCurClass;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 Sec_MsgViewCurCert(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId);

WE_INT32 Sec_MsgViewInfoSession(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId);

WE_INT32 Sec_MsgGetSecCurClass(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId); 
#endif /*M_SEC_CFG_WTLS*/
#endif

