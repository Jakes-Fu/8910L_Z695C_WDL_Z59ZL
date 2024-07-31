/*==================================================================================================
    FILE NAME : iTls.h
    MODULE NAME : TLS
    
    GENERAL DESCRIPTION
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date             Author               Number         Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-09-04    wangshuhua                          create itls.h
==================================================================================================*/

/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/

#ifndef _ITLS_H_
#define _ITLS_H_

//#define BREW_PLATFORM
#ifdef BREW_PLATFORM
#include "AEEHeap.h"    // Memory allocation Routines
#include "AEEStdLib.h"  // AEE Standard C library functions
#include "AEEModGen.h"  // Contains Sample AEEModule declarations
#include "AEEAppGen.h"
#endif
#include "we_def.h"
/*******************************************************************************
*   macro Define Section
*******************************************************************************/
#ifdef BREW_PLATFORM
#ifndef AEECLSID_ITLS
#define AEECLSID_ITLS        0x12358964
#endif
#endif

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

typedef enum
{
    E_ITLS_ERROR_OK = 0,                /* no error occurs */
    E_ITLS_ERROR_BAD_HANDLE,            /* handle has been invalid*/
    E_ITLS_ERROR_INVALID_PARAM,         /* invalid parameter*/          
    E_ITLS_ERROR_MEM_FAIL,              /* memory limit */
    E_ITLS_ERROR_NO_SUPPORT,            /* not support */
    E_ITLS_ERROR_WAP_TLS                 /* besides upstairs*/
    
}E_ITLS_ERROR;

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

#define G_WAP_ERR_TLS_SEND_DATA_SUCC                 (G_ERR_TLS + 1)
#define G_WAP_ERR_TLS_SEND_DATA_FAIL                  (G_ERR_TLS + 21)
#define G_WAP_ERR_TLS_RECV_DATA_SUCC                  (G_ERR_TLS + 3)
#define G_WAP_ERR_TLS_RECV_DATA_FAIL                  (G_ERR_TLS + 4)
#define G_WAP_ERR_TLS_SEND_DATA_END                   (G_ERR_TLS + 5)
#define G_WAP_ERR_TLS_FINISH_HANDSHAKE_OK           (G_ERR_TLS + 6)
#define G_WAP_ERR_TLS_HANDSHAKE_FAILURE             (G_ERR_TLS + 7)
#define G_WAP_ERR_TLS_SERVER_CLOSE                  (G_ERR_TLS + 8)
#define G_WAP_ERR_TLS_CANCEL                        (G_ERR_TLS + 9)
#define G_WAP_ERR_TLS_VERIFY_CERT_ERR                (G_ERR_TLS + 10)
typedef struct tagSt_TlsDNAttribute
{
   WE_UINT8* pucCountry;  // 国家
   WE_UINT8* pucState;    // 省份，州
   WE_UINT8* pucLocality; // 城镇
   WE_UINT8* pucOrg;   // 组织
   WE_UINT8* pucOrgUnit; //部门
   WE_UINT8* pucCommonname; //名称
   WE_UINT8* pucHash;  //hash值
}St_TlsDNAttribute;
typedef struct tagSt_CertInfo
{
	WE_INT32 iVersion;
	WE_UINT8 uiSerialnumber;
	WE_UINT32 uiSerialnumberlen;
	St_TlsDNAttribute  stIssuer;
	St_TlsDNAttribute  stSubject;
	WE_UINT8* pucNotBefore;
	WE_UINT8* pucNotAfter;
	WE_UINT8  aucSigAlg[10];
	WE_UINT8  aucSigHash[20];
	WE_UINT8*  pucUserId;
	WE_UINT32 uiUserIdLen;
	WE_UINT8* pucSubjectId;
	WE_UINT32 uiSubjectIdLen;	
}St_CertInfo;
/************************************************
*   ITls interface definition                  *
************************************************/
/*The following is ITls interface and struct definition. */
#ifdef BREW_PLATFORM
typedef struct ITls ITls;

AEEINTERFACE(ITls) 
{
     INHERIT_IBase(ITls);

    /*register callback function*/
    E_ITLS_ERROR (*RegClientEvtCB)(ITls * pMe,WE_HANDLE hPrivData,Fn_ITlsRespCb pcbSecEvtFunc);    
    E_ITLS_ERROR (*ProcTlsData)(ITls * pMe, WE_INT32 iConnId,E_TLS_DATA_TYPE eDataType,WE_VOID *pvData,WE_INT32 iDataLen);    
    E_ITLS_ERROR (*ResumeTlsHandShake)(ITls * pMe, WE_INT32 iConnId,St_TlsOpt *pstOpt);
    E_ITLS_ERROR (*SetOptPara)(ITls * pMe, St_TlsOpt *pstOpt);    
    E_ITLS_ERROR (*ViewSvrCert)(ITls * pMe,WE_CHAR * pcHost, WE_VOID **ppvContent);
    E_ITLS_ERROR (*CloseConn)(ITls *pMe,WE_INT32 iConnId);     
    E_ITLS_ERROR (*BeginHandeShake)(ITls * pMe, WE_INT32 iConnId,WE_UCHAR *pucHost,
                                 WE_BOOL bUseProxy,WE_UCHAR *pucAddr,WE_UINT16 usPort);
};

/* Extension structure. All variables in here are reference via "pMe->" */
struct ITls
{
    AEEVTBL(ITls) *pvt;

    WE_UINT32   m_nRefs;           
    IShell      *m_pIShell;        
    IModule     *m_pIModule;
    WE_HANDLE    hTlsHandle; // ITLS
   
};

/* ITls interface opened to external for wap wtls */
#define ITls_AddRef(p)                           AEEGETPVTBL((p),ITls)->AddRef((p))                            
#define ITls_Release(p)                          AEEGETPVTBL((p),ITls)->Release((p))  
#define ITls_RegClientEvtCB(p,v,c)                AEEGETPVTBL((p),ITls)->RegClientEvtCB((p),(v),(c))
#define ITls_ProcTlsData(p,i,e,v,s)                 AEEGETPVTBL((p),ITls)->ProcTlsData((p),(i),(e),(v),(s))
#define ITls_ResumeTlsHandShake(p,i,v)            AEEGETPVTBL((p),ITls)->ResumeTlsHandShake((p),(i),(v))
#define ITls_SetOptPara(p,v)                     AEEGETPVTBL((p),ITls)->SetOptPara((p),(v))
#define ITls_ViewServerCert(p,c,v)                   AEEGETPVTBL((p),ITls)->ViewSvrCert((p),(c),(v))
#define ITls_BeginHandeShake(p,i,c,b,u,s)        AEEGETPVTBL((p),ITls)->BeginHandeShake((p),(i),(c),(b),(u),(s))
#define ITls_CloseConn(p,s)                        AEEGETPVTBL((p),ITls)->CloseConn((p),(s))

WE_INT32 ITls_ClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *pIModule, void **ppMod);

#else
typedef void ITls;

E_ITLS_ERROR ITls_RegClientEvtCB
(
    ITls * pMe,
    WE_HANDLE hPrivData,
    WE_HANDLE pcbSecEvtFunc
);                 
E_ITLS_ERROR ITls_ProcTlsData
(
    ITls * pMe, 
    WE_INT32 iConnId,
    E_TLS_DATA_TYPE eDataType,
    WE_VOID *pvData,
    WE_INT32 iDataLen
);           
E_ITLS_ERROR ITls_ResumeTlsHandShake
(
    ITls * pMe, 
    WE_INT32 iConnId,
    St_TlsOpt *pstOpt
);           
E_ITLS_ERROR ITls_SetOptPara
(
    ITls * pMe,
    St_TlsOpt *pstOpt
);                    
E_ITLS_ERROR ITls_ViewServerCert
(
    ITls * pMe,
    WE_CHAR * pcHost,
    WE_VOID **ppvContent
);              
E_ITLS_ERROR ITls_BeginHandeShake
(
    ITls * pMe, 
    WE_INT32 iConnId,
    WE_UCHAR *pucHost,
    WE_BOOL bUseProxy,
    WE_UCHAR *pucAddr,
    WE_UINT16 usPort
);    
E_ITLS_ERROR ITls_CloseConn
(
    ITls *pMe,
    WE_INT32 iConnId
);               
WE_INT32 ITls_ClsCreateInstance(void **ppMod);
E_ITLS_ERROR ITls_Release(ITls * pMe);

#endif
#endif

