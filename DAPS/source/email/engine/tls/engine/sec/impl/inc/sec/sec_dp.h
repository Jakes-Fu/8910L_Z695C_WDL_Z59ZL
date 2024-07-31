/*==================================================================================================
    FILE NAME : sec_dp.h
    MODULE NAME : SEC
        
    
    GENERAL DESCRIPTION
        In this file,define the initial function prototype ,and will be update later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date             Author                Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   steven ding                           create sec_dp.h
    2007-05-17     tang                                 Delete cert manage and signtext
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef SEC_DP_H
#define SEC_DP_H

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define M_SEC_SELF_FREE(p)\
{\
    if(NULL != p)\
    {\
        WE_FREE(p);\
        (p) = NULL;\
    }\
}

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

typedef enum tagE_SecDpMsgType
{             
    M_SEC_DP_MSG_GET_CIPHERSUITE = M_SEC_MSG_DST_HANDLE_MAIN + 2,  
    M_SEC_DP_MSG_EXCHANGE_WTLS_KEY,                            
    M_SEC_DP_MSG_WTLS_PRF,                                     
    M_SEC_DP_MSG_VERIFY_WTLS_CERT_CHAIN, 
    M_SEC_DP_MSG_COMPUTE_SIGN,
    M_SEC_DP_MSG_ESTABLISH_CONN,                            
    M_SEC_DP_MSG_TERMINATE_CONN,                           
    M_SEC_DP_MSG_RM_PEER_LINKS,                               
    M_SEC_DP_MSG_LINK_PEER_SESSION,                          
    M_SEC_DP_MSG_LOOKUP_PEER,                                  
    M_SEC_DP_MSG_ACTIVE_SESSION,                               
    M_SEC_DP_MSG_INVALID_SESSION,                             
    M_SEC_DP_MSG_FETCH_SESSION,                                
    M_SEC_DP_MSG_UPDATE_SESSION,                    
    M_SEC_DP_MSG_VIEW_CUR_CERT,                                
    M_SEC_DP_MSG_VIEW_SESSION,                                 
    M_SEC_DP_MSG_GET_CUR_SEC_CLASS,                             
    M_SEC_DP_MSG_ENCRYPT_PKC_RESP,/*E_SEC_MSG_ENCRYPT_PKC_RESP*/
    M_SEC_DP_MSG_VERIFY_SIGN_RESP,/*E_SEC_MSG_VERIFY_SIGN_RESP*/   
    M_SEC_DP_MSG_COMPUTE_SIGN_RESP,  /*E_SEC_MSG_COMPUTE_SIGN_RESP*/
#ifdef M_SEC_CFG_TLS
    M_SEC_DP_MSG_GET_TLS_CIPHERSUITE,
    M_SEC_DP_MSG_VERIFY_TLS_CERT_CHAIN,
    M_SEC_DP_MSG_EXCHANGE_TLS_KEY,
    M_SEC_DP_MSG_TLS_PRF,
#endif
    M_SEC_DP_DEFAULT = 0xFFFFFFFF /* four bytes align */

}E_SecDpMsgType;

typedef enum tagE_SecDpFncId
{
    E_SEC_DP_NONE,                            
    E_SEC_DP_ESTABLISHED_CONN,                   
    E_SEC_DP_TERMINATED_CONN,                    
    E_SEC_DP_GET_CIPHERSUITE,                     
    E_SEC_DP_XCH_WTLS_KEY,                         
    E_SEC_DP_WTLS_PRF,                            
    E_SEC_DP_VERIFY_WTLS_CERT_CHAIN,     
    E_SEC_DP_COMPUTE_SIGN,
    E_SEC_DP_RM_PEER_LINKS,                       
    E_SEC_DP_LINK_PEER_SESSION,                   
    E_SEC_DP_LOOKUP_PEER,                          
    E_SEC_DP_ACTIVE_SESSION,                      
    E_SEC_DP_INVALID_SESSION,                   
    E_SEC_DP_FETCH_SESSION,                  
    E_SEC_DP_UPDATE_SESSION,     
    E_SEC_DP_VIEW_CUR_CERT,                        
    E_SEC_DP_VIEW_SESSION,  
    E_SEC_DP_GET_CUR_SEC_CLASS ,    
#ifdef M_SEC_CFG_TLS
    E_SEC_DP_GET_TLS_CIPHERSUITE,
    E_SEC_DP_VERIFY_TLS_CERT_CHAIN,
    E_SEC_DP_XCH_TLS_KEY,
    E_SEC_DP_TLS_PRF,
#endif
    E_SEC_DP_DEFAULT = 0xFFFFFFFF /* four bytes align */
}E_SecDpFncId;

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
typedef enum tagE_SecTlUeFncId
{
    E_SEC_DP_CONFIRM_DLG,
    E_SEC_DP_DEFAULT2 = 0xFFFFFFFF /* four bytes align */
}E_SecTlUeFncId;
#endif

typedef struct tagSt_SecDpParameter 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    struct tagSt_SecDpParameter*            pstPrev;
    struct tagSt_SecDpParameter*            pstNext;
}St_SecDpParameter;

#ifdef M_SEC_CFG_WTLS
typedef struct tagSt_SecDpGetWtlsCipherSuite
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecWtlsGetCipherSuite*               pstStrct;
    
}St_SecDpGetWtlsCipherSuite;
        
typedef struct tagSt_SecDpWtlsKeyXch 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecWtlsKeyExchange*                  pstStrct;
}St_SecDpWtlsKeyXch ;

typedef struct tagSt_SecDpWtlsPrf 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecGetPrfResult*                 pstStrct;
}St_SecDpWtlsPrf ;

typedef struct tagSt_SecDpVerifyWtlsCertChain 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecWtlsVerifySvrCert*                pstStrct;
}St_SecDpVerifyWtlsCertChain;
#endif

#ifdef M_SEC_CFG_TLS
typedef struct tagSt_SecDpVerifyTlsCertChain 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecTlsVerifySvrCert*               pstStrct;
}St_SecDpVerifyTlsCertChain;

typedef struct tagSt_SecDpGetTlsCipherSuite
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecTlsGetCipherSuite*                pstStrct;    
}St_SecDpGetTlsCipherSuite;

typedef struct tagSt_SecDpTlsKeyXch 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecTlsKeyExchange*                   pstStrct;
}St_SecDpTlsKeyXch ;

typedef struct tagSt_SecDpTlsPrf 
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecGetPrfResult*                 pstStrct;
}St_SecDpTlsPrf ;

#endif

#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
typedef struct tagSt_SecDpComputeTlsSign
{
    E_SecDpFncId                            eType;
    WE_HANDLE hItype;
    St_SecDpParameter*                      pstPrev;
    St_SecDpParameter*                      pstNext;
    St_SecTlsComputeSign*                   pstStrct;
}St_SecDpComputeTlsSign;
#endif


typedef struct tagSt_SecDpConnEst 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecSetupConnection*                    pstStrct;
}St_SecDpConnEst;

typedef struct tagSt_SecDpConnTmt
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecStopConnection*                     pstStrct;
}St_SecDpConnTmt ;

typedef struct tagSt_SecDpRmPeerLinks 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecRemovePeer*                         pstStrct;
}St_SecDpRmPeerLinks ;

typedef struct tagSt_SecDpPeerLinkSession  
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecAttachPeer*                         pstStrct;
}St_SecDpPeerLinkSession;

typedef struct tagSt_SecDpLookupPeer
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecSearchPeer*                         pstStrct;
}St_SecDpLookupPeer;

typedef struct tagSt_SecDpActiveSession 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecEnableSession*                      pstStrct;
}St_SecDpActiveSession;

typedef struct tagSt_SecDpInvalidSession
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecDisableSession*                     pstStrct;
}St_SecDpInvalidSession;

typedef struct tagSt_SecDpfetchSession
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecFetchSessionInfo*                   pstStrct;
}St_SecDpfetchSession;

typedef struct tagSt_SecDpUpdateSession 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecUpdateSession*                      pstStrct;
}St_SecDpUpdateSession;

#ifdef M_SEC_CFG_WTLS
typedef struct tagSt_SecDpViewCurCert 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecViewCurCert*                        pstStrct;
  WE_INT32                                  iOptions;
}St_SecDpViewCurCert;

typedef struct tagSt_SecDpViewSessionInfo 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecViewSessionInfo*                    pstStrct;
  WE_INT32                                  iOptions;
}St_SecDpViewSessionInfo;

typedef struct tagSt_SecDpGetCurSecClass 
{
  E_SecDpFncId                              eType;
  WE_HANDLE hItype;
  St_SecDpParameter*                        pstPrev;
  St_SecDpParameter*                        pstNext;
  St_SecGetSecCurClass*                     pstStrct;
}St_SecDpGetCurSecClass;
#endif
/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

WE_INT32 Sec_DpHandleTerminate (WE_HANDLE hSecHandle);
WE_VOID Sec_DpHandleInternal(WE_HANDLE hSecHandle);
WE_INT32 Sec_DpHandleInit (WE_HANDLE hSecHandle);
WE_VOID Sec_DpHandleCleanUp(WE_HANDLE hSecHandle);
WE_VOID Sec_DpHandleQueueDelById(WE_HANDLE hSecHandle);
WE_VOID Sec_DpResetVariable(WE_HANDLE hSecHandle);

#endif

