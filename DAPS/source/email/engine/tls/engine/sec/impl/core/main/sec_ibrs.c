/*=====================================================================================
    FILE NAME : sec_ibrs.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This file provides a set of functions, which is for browser engine and app.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                   Author          Number                 Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-08        Kevin                                            create sec_ibrs.c
    2007-05-17     tang                                 Delete cert manage and signtext
=====================================================================================*/

/**************************************************************************************
*   Include File Section
**************************************************************************************/

#include "sec_comm.h"

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
      Sec_MsgViewCurCert
CREATE DATE: 
      2006-07-04
AUTHOR: 
      Kevin
DESCRIPTION:
        view the current certificate information and return the content of the certificate.
ARGUMENTS PASSED:
       WE_HANDLE    hSecHandle[IN]:Global data handle.
       WE_INT32     iTargetID[IN]:The indentify of the invoker.
       WE_INT32     iSecurityId[IN]:The ID of the security.
RETURN VALUE:
    M_SEC_ERR_INSUFFICIENT_MEMORY:
        The result is error.
    M_SEC_ERR_OK:
        The result is OK.
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None.
CALL BY:
    Omit.
IMPORTANT NOTES:
      Describe anything that help understanding the function and important aspect of
      using the function i.e side effect..etc
=====================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_INT32 Sec_MsgViewCurCert(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId)
{
    WE_INT32 iRes = 0;
    St_SecViewCurCert *pstViewCurrentCert = (St_SecViewCurCert*)WE_MALLOC((WE_ULONG)sizeof(St_SecViewCurCert));

    if(NULL == pstViewCurrentCert)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(pstViewCurrentCert,0,sizeof(St_SecViewCurCert));
    pstViewCurrentCert->iTargetID = iTargetID;
    pstViewCurrentCert->iSecurityId = iSecurityId;
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_VIEW_CURCERT, (void*)pstViewCurrentCert);
    if (iRes != M_SEC_ERR_OK)
    {
        WE_FREE(pstViewCurrentCert);
        return iRes;
    }

    return M_SEC_ERR_OK;
}
/*=====================================================================================
FUNCTION: 
      Sec_MsgViewInfoSession
CREATE DATE: 
      2006-07-04
AUTHOR: 
      Kevin
DESCRIPTION:
      view session information when the security channel is working,get the session
      information.
ARGUMENTS PASSED:
      WE_HANDLE hSecHandle[IN]:Global data handle.
      WE_INT32  iTargetID[IN]:The indentify of the invoker.
      WE_INT32  iSecurityId[IN]:ID of the security.
RETURN VALUE:
    M_SEC_ERR_INSUFFICIENT_MEMORY:
        The result is error.
    M_SEC_ERR_OK:
        The result is OK.
USED GLOBAL VARIABLES:
    None.
USED STATIC VARIABLES:
    None.
CALL BY:
    Omit.
IMPORTANT NOTES:
      Describe anything that help understanding the function and important aspect of
      using the function i.e side effect..etc
=====================================================================================*/
WE_INT32 Sec_MsgViewInfoSession(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId)
{
    WE_INT32 iRes = 0;
    St_SecViewSessionInfo *pstViewSessionInfo = (St_SecViewSessionInfo*)WE_MALLOC((WE_ULONG)sizeof(St_SecViewSessionInfo));

    if(NULL == pstViewSessionInfo)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(pstViewSessionInfo,0,sizeof(St_SecViewSessionInfo));
    pstViewSessionInfo->iTargetID = iTargetID;
    pstViewSessionInfo->iSecurityId = iSecurityId;
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_VIEW_INFOSESSION, (void*)pstViewSessionInfo);
    if (iRes != M_SEC_ERR_OK)
    {
        WE_FREE(pstViewSessionInfo);
        return iRes;
    }

    return M_SEC_ERR_OK;
}

/*=====================================================================================
FUNCTION: 
      Sec_MsgGetSecCurClass
CREATE DATE: 
      2006-07-04
AUTHOR: 
      Kevin
DESCRIPTION:
      get current sec class level.
ARGUMENTS PASSED:
      WE_HANDLE     hSecHandle[IN]:Global data handle.
      WE_INT32      iTargetID[IN]:The indentify of the invoker.
      WE_INT32      iSecurityId[IN]:ID of the security.
RETURN VALUE:
    M_SEC_ERR_INSUFFICIENT_MEMORY:
        The result is error.
    M_SEC_ERR_OK:
        The result is OK.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit.
IMPORTANT NOTES:
      Describe anything that help understanding the function and important aspect of
      using the function i.e side effect..etc
=====================================================================================*/
WE_INT32 Sec_MsgGetSecCurClass(WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iSecurityId)
{
    WE_INT32 iRes = 0;
    St_SecGetSecCurClass *pstGetCurrentSecClass = 
                    (St_SecGetSecCurClass*)WE_MALLOC((WE_ULONG)sizeof(St_SecGetSecCurClass));

    if(NULL == pstGetCurrentSecClass)
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET(pstGetCurrentSecClass,0,sizeof(St_SecGetSecCurClass));
    pstGetCurrentSecClass->iTargetID = iTargetID;
    pstGetCurrentSecClass->iSecurityId = iSecurityId;
    iRes = Sec_PostMsg(hSecHandle, (WE_INT32)E_SEC_MSG_GET_CURCLASS, (void*)pstGetCurrentSecClass);
    if (iRes != M_SEC_ERR_OK)
    {
        WE_FREE(pstGetCurrentSecClass);
        return iRes;
    }

    return M_SEC_ERR_OK;
}
#endif

