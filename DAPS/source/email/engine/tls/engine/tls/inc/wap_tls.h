/*==================================================================================================
    FILE NAME   : wap_tls.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        Provide tls interface.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/


#ifndef WAP_TLS_H
#define WAP_TLS_H

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
typedef struct _ITLS ITLS; 
#if 0
typedef enum
{
    E_TLS_OPT_CANCEL = 1,
    E_TLS_OPT_USER_CERT,
    E_TLS_OPT_USER_CERTS,
    E_TLS_OPT_ERR_RESP,
    E_TLS_OPT_PRIVATE_KEY,
    E_TLS_OPT_ROOT_CERT,
    E_TLS_OPT_VERSION,
    E_TLS_OPT_END
}E_TLS_OPT;
typedef enum
{
    E_TLS_DATA_TO_APP = 1,
    E_TLS_DATA_TO_SVR,
    E_TLS_INFO,
    
    E_TLS_DATA_FROM_APP,
    E_TLS_DATA_FROM_SVR
}E_TLS_DATA_TYPE;

typedef struct tagSt_TlsResumeOpt
{
    E_TLS_OPT eOptId;
    WE_VOID *pvOpt;
}St_TlsOpt;

typedef void (*Fn_ITlsRespCb)(E_TLS_DATA_TYPE eDataType, void* pvResp,
                                WE_HANDLE hPrivData);

typedef struct tagSt_TlsRespData
{
    WE_INT32 iConnId;
    WE_VOID *pvRespData;
    WE_INT32 iRespLen;
    WE_INT32 iErrCode;// only use by E_TLS_INFO
}St_TlsRespData;

#define G_WAP_TLS_PRO_TLS_VERSION               0x00000031
#define G_WAP_TLS_PRO_SSL_VERSION               0x00000030

#define G_ERR_TLS                             0x4000
      
#define G_WAP_ERR_TLS_BAD_RECORD                    (G_ERR_TLS + 1)
#define G_WAP_ERR_TLS_DECRYPTION_FAILED             (G_ERR_TLS + 2)
#define G_WAP_ERR_TLS_RECEIVED_ALERT                (G_ERR_TLS + 3)
#define G_WAP_ERR_TLS_HANDSHAKE_FAILURE             (G_ERR_TLS + 4)
#define G_WAP_ERR_TLS_UNSUPPORTED_CERT              (G_ERR_TLS + 5)
#define G_WAP_ERR_TLS_UNKNOWN_CA                    (G_ERR_TLS + 6)
#define G_WAP_ERR_TLS_BAD_CERTIFICATE               (G_ERR_TLS + 7)
#define G_WAP_ERR_TLS_CERTIFICATE_EXPIRED           (G_ERR_TLS + 8)
#define G_WAP_ERR_TLS_INTERNAL_ERROR                (G_ERR_TLS + 9)
#define G_WAP_ERR_TLS_CANCEL                        (G_ERR_TLS + 10)
#define G_WAP_ERR_TLS_CRYPTOLIB                     (G_ERR_TLS + 11 )
#define G_WAP_ERR_TLS_MALLOC_FAIL                   (G_ERR_TLS + 12 )
#define G_WAP_ERR_TLS_NEED_USER_CERT                (G_ERR_TLS + 13 )
#define G_WAP_ERR_TLS_FINISH_HANDSHAKE_OK           (G_ERR_TLS + 14)
#define G_WAP_ERR_TLS_OTHER                         (G_ERR_TLS + 15)
#endif



/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
WE_INT32 ITls_Create(ITLS **ppITls);
WE_INT32 ITls_ReleaseHandle(ITLS *pITls);
WE_INT32 ITls_Close(ITLS *pITls,WE_INT32 iConnId);
WE_INT32 ITls_StartHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    WE_UCHAR* pucHost,
    WE_BOOL bUseProxy,
    WE_UCHAR *pucAddr,
    WE_UINT16 usPort
);
WE_INT32 ITls_RegRespCB
(
    ITLS *pITls,
    Fn_ITlsRespCb pcb,
    WE_HANDLE hPrivData
);
WE_INT32 ITls_ProcData
(
    ITLS *pITls,
    WE_INT32 iConnId,
    E_TLS_DATA_TYPE eDataType,
    WE_VOID* pvData,
    WE_INT32 iDataLen
);
WE_INT32 ITls_ResumeHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    St_TlsOpt *pstOpt
);
WE_INT32 ITls_SetPara
(
    ITLS *pITls,
    St_TlsOpt *pstOpt
);
WE_INT32 ITls_ViewSvrCert
(
    ITLS *pITls,
    WE_CHAR* pcHost,
    WE_VOID **ppvContent
);


#endif
