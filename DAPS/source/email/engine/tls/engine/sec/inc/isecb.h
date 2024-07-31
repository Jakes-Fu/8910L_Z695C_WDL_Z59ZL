/*==================================================================================================
    FILE NAME : iSecB.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        In this file,define the interface function prototype ,and the related structure 
        used in these interfaces.This is a public head file of isecb module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date              Author                 Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang                            create isecb.h
==================================================================================================*/


/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/

#ifndef _ISECB_H_
#define _ISECB_H_

#ifdef M_SEC_CFG_WTLS

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
typedef struct _ISecB  ISecB;
/************************************************************************/
/* Response Event                                                                                                   */
/************************************************************************/
#define    G_WTLSCURCLASS_RESP             0x21
#define    G_GETSESSIONINFO_RESP           0x26
#define    G_GETCURSVRCERT_RESP            0x27

#define    G_Evt_CONFIRM                                ( 0x31 )
#define    G_Evt_SHOWSESSIONCONTENT        ( 0x3B )
#define    G_Evt_SHOWCURSVRCERT                 ( 0x3F )

/***************************************************************************
* The result value from response
***************************************************************************/
/***************************************************
* Certificate types used in sec module
****************************************************/
#define G_SEC_USER_CERT                    0x01
#define G_SEC_ROOT_CERT                    0x02  

/************************************************************************/
/* string constants                                                                                                    */
/************************************************************************/

/* Session  */
/*"WTLS connection type"*/
#define G_SEC_ID_SESSION_WTLS                 ( 0x18) 

/*"HMAC_MD5 algorithm"*/
#define G_SEC_ID_SESSION_MD5                  ( 0x19) 
/*"HMAC_SHA-1 algorithm"*/
#define G_SEC_ID_SESSION_SHA1                 ( 0x1a) 

/*"Cipher Method: NULL"*/
#define G_SEC_ID_SESSION_BULK_NULL            ( 0x1b) 
/*"Cipher Method: RC5"*/
#define G_SEC_ID_SESSION_BULK_RC5             ( 0x1c) 
/*"Cipher Method: RC5 40"*/
#define G_SEC_ID_SESSION_BULK_RC5_40          ( 0x1d) 
/*"Cipher Method: RC5 56"*/
#define G_SEC_ID_SESSION_BULK_RC5_56          ( 0x1e) 
/*"Cipher Method: RC5 64"*/
#define G_SEC_ID_SESSION_BULK_RC5_64          ( 0x1f) 
/*"Cipher Method: Triple DES"*/
#define G_SEC_ID_SESSION_BULK_3DES            ( 0x20) 

/*"KeyExchange : NULL"*/
#define G_SEC_ID_SESSION_KEYEXCH_NULL         ( 0x21) 
/*"KeyExchange : RSA ANON"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA_ANON     ( 0x22) 
/*"KeyExchange : RSA ANON 512"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA_ANON_512 ( 0x23) 
/*"KeyExchange : RSA ANON 768"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA_ANON_768 ( 0x24) 
/*"KeyExchange : RSA"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA          ( 0x25) 
/*"KeyExchange : RSA 512"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA_512      ( 0x26) 
/*"KeyExchange : RSA 768"*/
#define G_SEC_ID_SESSION_KEYEXCH_RSA_768      ( 0x27) 

/* "dd/mm/yyyy", the new format contain the month. */
/* open for compile */
/* Month  */
/*"January"*/
#define G_SEC_ID_JANUARY                      ( 0x36) 
/*"February"*/
#define G_SEC_ID_FEBRUARY                     ( 0x37) 
/*"March"*/
#define G_SEC_ID_MARCH                        ( 0x38) 
/*"April"*/
#define G_SEC_ID_APRIL                        ( 0x39) 
/*"May"*/
#define G_SEC_ID_MAY                          ( 0x3a) 
/*"June"*/
#define G_SEC_ID_JUNE                         ( 0x3b) 
/*"July"*/
#define G_SEC_ID_JULY                         ( 0x3c)
/*"August"*/ 
#define G_SEC_ID_AUGUST                       ( 0x3d) 
/*"September"*/
#define G_SEC_ID_SEPTEMBER                    ( 0x3e) 
/*"October"*/
#define G_SEC_ID_OCTOBER                      ( 0x3f) 
/*"November"*/
#define G_SEC_ID_NOVEMBER                     ( 0x40) 
/*"December"*/
#define G_SEC_ID_DECEMBER                     ( 0x41) 


/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/************************************************************************/
/* Response Structure                                                                                               */
/************************************************************************/
typedef struct tagSt_SecGetWtlsCurClassResp
{
   WE_INT32 iTargetID; 
   WE_UINT8 ucSecClass;
   WE_UINT8 ucSessionInfoReady;
   WE_UINT8 ucCertReady;
}St_SecGetWtlsCurClassResp;

typedef struct tagSt_SecGetSessionInfoResp
{
   WE_INT32  iTargetID;
   WE_UINT16 usResult;
}St_SecGetSessionInfoResp;

typedef struct tagSt_SecGetCurSvrCertResp
{
   WE_INT32  iTargetID;
   WE_UINT16 usResult;
}St_SecGetCurSvrCertResp;
/**************************************************
* structure used in i interface for user event
**************************************************/
typedef struct tagSt_ShowCertContent
{
   WE_CHAR* pcIssuerStr;
   WE_CHAR* pcSubjectStr;
   /* the certificate time is the UTC time, not local time. */
   WE_INT32 iNotBefMonth;/* not used */
   WE_CHAR* pcNotBefStr;/* "dd/mm/yyyy", the new format contain the month. */
   WE_INT32 iNotAftMonth;/* not used */
   WE_CHAR* pcNotAftStr;/* same as pcNotBefStr */
   WE_UCHAR* pucSerialNumber;/*serial number*/
   WE_UCHAR* pucAlg;/*signature algorthm*/
   WE_UCHAR* pucPubKey;/*public key*/
       
}St_ShowCertContent;

typedef struct tagSt_ShowSessionContent
{
   WE_INT32 iConnTypeId;
   WE_INT32 iHmacId;
   WE_CHAR* pcHmacKeyLenStr;
   WE_INT32 iEncralgId;
   WE_CHAR* pcEncrKeyLenStr;
   WE_INT32 iKeyExchId;
   WE_CHAR* pcKeyExchKeyLenStr;
}St_ShowSessionContent;


/* type of info operation */
typedef enum tagE_SecInfoType
{             
    G_SEC_CUR_SERVER_CERT, 
    G_SEC_SESSION_INFO,
    G_SEC_CUR_CLASS
}E_SecInfoType;

/* parameters in connection */
typedef struct tagSt_SecCurSvrCertPara
{
    WE_INT32 iTargetID;
    WE_INT32 iSecId;
} St_SecCurSvrCertPara;

typedef struct tagSt_SecSessionInfoPara
{
    WE_INT32 iTargetID;
    WE_INT32 iSecId;
} St_SecSessionInfoPara;

typedef struct tagSt_SecCurClassPara
{
    WE_INT32 iTargetID;
    WE_INT32 iSecId;
} St_SecCurClassPara;

typedef struct tagSt_SecInfoOpt
{
   E_SecInfoType         eType; 
   WE_HANDLE             hParaValue;
} St_SecInfoOpt;


/************************************************
*   ISecB interface definition                  *
************************************************/
WE_UINT32 ISecB_GetInfoOpts(WE_HANDLE pMe, St_SecInfoOpt *pstInfoOpt) ;
#endif
#endif/*_ISECB_H_*/


