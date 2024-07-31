/*==================================================================================================
    FILE NAME : sec_main.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        this file is the main entry of SEC module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date                   Author            Number                    Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07      Kevin Yang                                          create sec_main.c
    2007-05-17     tang                                              Delete cert manage and signtext
==================================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include "sec_comm.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#define SEC_MAIN_ISHELL_POINTER              (((ISec*)hSecHandle)->m_pIShell)
#define SEC_MAIN_WANTS_TO_RUN                (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iWantsToRun)
#define SEC_MAIN_CALLBACK                    (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_cb)
#define SEC_CIPHER_HANDLE                    (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvAlgHandle)


/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/
/*==================================================================================================
FUNCTION: 
    Sec_IsRun
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Judge whether sec module needs to run.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]: Handle for global data used in sec module.
RETURN VALUE:
    True: the sec module needs to run.
    False: the sec module does not need to run.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_IsRun(WE_HANDLE hSecHandle)
{
    WE_INT32 iResult = 0;
    if(NULL == hSecHandle)
    {
        return FALSE;
    }
    
    iResult = (!Sec_MsgQueueState(hSecHandle));
    
    if (!iResult)
    {
        iResult = SEC_MAIN_WANTS_TO_RUN;
    }
    
    return iResult;
}
/*==================================================================================================
FUNCTION: 
    Sec_Run
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Run the sec module.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]: Handle for global data used in sec module.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static void Sec_Run(WE_HANDLE hSecHandle)
{
    if(NULL == hSecHandle)
    {
        return;
    }
    if(SEC_MAIN_WANTS_TO_RUN)
    {
        Sec_DpHandleInternal(hSecHandle);
    }
    
    Sec_MsgProcess(hSecHandle);
}
/*==================================================================================================
FUNCTION: 
    Sec_StartEx
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Start sec module. and this function will be invoked when the sec module is created.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]: Handle for global data used in sec module.
RETURN VALUE:
    M_SEC_ERR_OK
    M_SEC_ERR_GENERAL_ERROR
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_StartEx(WE_HANDLE hSecHandle)
{
    return Sec_DpHandleInit(hSecHandle);
}
/*==================================================================================================
FUNCTION: 
    Sec_Stop
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Exit the sec module. and this function will be invoked when the sec module is released.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]: Handle for global data used in sec module.
RETURN VALUE:
    M_SEC_ERR_INVALID_PARAMETER
    M_SEC_ERR_GENERAL_ERROR
    M_SEC_ERR_OK
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 Sec_StopEx(WE_HANDLE hSecHandle)
{
    WE_INT32 iResult = 0;

    if(NULL == hSecHandle)
    {
        return M_SEC_ERR_INVALID_PARAMETER;
    }
    
    iResult = Sec_DpHandleTerminate(hSecHandle);

    if(NULL != SEC_CIPHER_HANDLE)
    {
        iResult = Sec_LibCryptTerminate(hSecHandle);
        if(M_SEC_ERR_OK != iResult)
        {
            return M_SEC_ERR_GENERAL_ERROR;
        }
    }
    return M_SEC_ERR_OK;
}
/*==================================================================================================
FUNCTION: 
    Sec_RunMainFlow
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    A callback function, and sec module run in this callback function.
ARGUMENTS PASSED:
    void * pHandle [IN]: Handle for global data used in sec module.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_RunMainFlow(WE_HANDLE hHandle)
{
    ISec * hSecHandle = NULL;
    if(NULL == hHandle)
    {
        return;
    }
    hSecHandle = (ISec*)hHandle;
    while(Sec_IsRun((WE_HANDLE)hSecHandle))
    {
        Sec_Run((WE_HANDLE)hSecHandle);
    } 
}

/*==================================================================================================
FUNCTION: 
    Sec_PostMsg
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    Post message to sec module's message queue.
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN]:Handle for global data used in sec module.
    WE_UINT32 iEvent [IN]: event id.
    void* hData [IN]:event data body.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_PostMsg(WE_HANDLE hSecHandle, WE_INT32 iEvent, WE_HANDLE hData)
{ 
    WE_INT32 iResult = -100;
    switch (iEvent)
    {
        case E_SEC_MSG_SETUPCON:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                                (WE_INT32)M_SEC_DP_MSG_ESTABLISH_CONN,hData );
            break;
        case E_SEC_MSG_STOPCON:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                            (WE_INT32)M_SEC_DP_MSG_TERMINATE_CONN, hData );
            break;
        case E_SEC_MSG_DELPEER:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_RM_PEER_LINKS, hData );
            break;
        case E_SEC_MSG_ATTACHPEER:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                       (WE_INT32)M_SEC_DP_MSG_LINK_PEER_SESSION, hData );
            break;
        case E_SEC_MSG_SEARCHPEER:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_LOOKUP_PEER, hData );
            break;
        case E_SEC_MSG_ENVSESSION:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_ACTIVE_SESSION, hData );
            break;
        case E_SEC_MSG_DISSESSION:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                (WE_INT32)M_SEC_DP_MSG_INVALID_SESSION, hData );
            break;
        case E_SEC_MSG_GETSESINFO:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_FETCH_SESSION, hData );
            break;
        case E_SEC_MSG_UPSESSION:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                (WE_INT32)M_SEC_DP_MSG_UPDATE_SESSION, hData );  
            break;
#ifdef M_SEC_CFG_WTLS
        case E_SEC_MSG_VIEW_CURCERT:
            iResult = SEC_MSG_DISPATCH_IP (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                            (WE_INT32)M_SEC_DP_MSG_VIEW_CUR_CERT, 1, hData );
            break;
        case E_SEC_MSG_VIEW_INFOSESSION:
            iResult = SEC_MSG_DISPATCH_IP (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                    (WE_INT32)M_SEC_DP_MSG_VIEW_SESSION, 1, hData );
            break;
        case E_SEC_MSG_GET_CURCLASS:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                    (WE_INT32)M_SEC_DP_MSG_GET_CUR_SEC_CLASS, hData );
            break;
        case E_SEC_MSG_WTLSGETCIPHERSUITE:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_GET_CIPHERSUITE, hData );
            break;
          
        case E_SEC_MSG_WTLSKEYEXCHANGE:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                        (WE_INT32)M_SEC_DP_MSG_EXCHANGE_WTLS_KEY, hData );
            break;

        case E_SEC_MSG_WTLSGETPRFRESULT: 
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                                (WE_INT32)M_SEC_DP_MSG_WTLS_PRF, hData );
            break;
#endif      
        case E_SEC_MSG_WTLSVERIFYSVRCERT:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                                (WE_INT32)M_SEC_DP_MSG_VERIFY_WTLS_CERT_CHAIN, hData );
            break;
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
        case E_SEC_MSG_COMPSIG: 
            iResult =SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                (WE_INT32)M_SEC_DP_MSG_COMPUTE_SIGN, hData );  
            break;
#endif
#ifdef M_SEC_CFG_TLS
        case E_SEC_MSG_TLSGETCIPHERSUITE:
            iResult =SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                (WE_INT32)M_SEC_DP_MSG_GET_TLS_CIPHERSUITE, hData );
            break;
        case E_SEC_MSG_TLSVERIFYCERTCHAIN:
            iResult =SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                (WE_INT32)M_SEC_DP_MSG_VERIFY_TLS_CERT_CHAIN, hData );
            break;
        case E_SEC_MSG_TLSKEYEXCHANGE:
            iResult =SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                (WE_INT32)M_SEC_DP_MSG_EXCHANGE_TLS_KEY, hData );
            break;
        case E_SEC_MSG_TLSGETPRFRESULT:
            iResult =SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_HANDLE_MAIN,
                (WE_INT32)M_SEC_DP_MSG_TLS_PRF, hData );
            break;
#endif        
        case E_SEC_MSG_ENCRYPT_PKC_RESP:      
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_WIM_MAIN, 
                                (WE_INT32)M_SEC_DP_MSG_ENCRYPT_PKC_RESP, hData );
            break;
#if (defined ( M_SEC_CFG_WTLS_CLASS_3 ) || defined (M_SEC_CFG_TLS))
        case E_SEC_MSG_COMPUTE_SIGN_RESP:   
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_WIM_MAIN, 
                                (WE_INT32)M_SEC_DP_MSG_COMPUTE_SIGN_RESP, hData );
            break;
#endif
        case E_SEC_MSG_VERIFY_SIGN_RESP:
            iResult = SEC_MSG_DISPATCH_P (hSecHandle, M_SEC_MSG_DST_WIM_MAIN, 
                                (WE_INT32)M_SEC_DP_MSG_VERIFY_SIGN_RESP, hData );
            break;
        default:
            WE_LOG_MSG ((0, 0,"SEC: Received unknown msg (%04x)\n", iEvent));
            iResult = M_SEC_ERR_INVALID_PARAMETER;
            break;
    }
    return iResult;
}

