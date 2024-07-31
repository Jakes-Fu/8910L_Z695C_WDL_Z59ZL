/*==================================================================================================
    HEADER NAME : isecb.c
    MODULE NAME : SEC
    
    PRE-INCLUDE FILES DESCRIPTION
        
    GENERAL DESCRIPTION
    In this file,define the interface function prototype of sec module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                   Tracking
    Date          Author            Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang        None         Draft
==================================================================================================*/

/***************************************************************************************************
*   Include File Section
***************************************************************************************************/

#include "sec_comm.h"
#include "oem_secmgr.h"
#include "isecb.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#define SEC_IAEECALLBACK_VAR    (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->m_cb)
#define SEC_IISHELL_POINTER     (((ISec*)pMe)->m_pIShell)

/*==================================================================================================
FUNCTION: 
    ISecB_GetInfoOpts
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    get the detailed information, such as current server cert, session info or current security class.
ARGUMENTS PASSED:
    ISecB *         pMe[IN]:pointer of ISecB instance.
    St_SecInfoOpt      *pstInfoOpt[IN]: info used to get the detailed infomation.
RETURN VALUE:
    G_SEC_INVALID_PARAMETER,
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_OK
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
#ifdef M_SEC_CFG_WTLS
WE_UINT32 ISecB_GetInfoOpts(WE_HANDLE pMe, St_SecInfoOpt *pstInfoOpt)
{
    WE_INT32       iResult = 0;
     
    if ((NULL == pMe) || (NULL == pstInfoOpt))
    {
        return G_SEC_INVALID_PARAMETER;
    }
    WE_LOG_MSG((0,(WE_UCHAR)0,"ISecB_GetInfoOpts\n"));
    switch (pstInfoOpt->eType)
    {
    case G_SEC_CUR_SERVER_CERT:
        {   
            St_SecCurSvrCertPara *pstCurSvrCertPara = (St_SecCurSvrCertPara *)(pstInfoOpt->hParaValue);
            if (NULL == pstCurSvrCertPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            iResult = Sec_MsgViewCurCert((WE_HANDLE)pMe, pstCurSvrCertPara->iTargetID, 
                pstCurSvrCertPara->iSecId);         
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    case G_SEC_SESSION_INFO:
        {   
            St_SecSessionInfoPara *pstSessionInfoPara = (St_SecSessionInfoPara *)(pstInfoOpt->hParaValue);
            if (NULL == pstSessionInfoPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }
            iResult = Sec_MsgViewInfoSession((WE_HANDLE)pMe, pstSessionInfoPara->iTargetID, 
                pstSessionInfoPara->iSecId);     
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    case G_SEC_CUR_CLASS:
        {   
            St_SecCurClassPara *pstCurClassPara = (St_SecCurClassPara *)(pstInfoOpt->hParaValue);
            if (NULL == pstCurClassPara)
            {
                return G_SEC_INVALID_PARAMETER;
            }     
            iResult = Sec_MsgGetSecCurClass((WE_HANDLE)pMe, pstCurClassPara->iTargetID, 
                pstCurClassPara->iSecId);         
            if (M_SEC_ERR_OK != iResult)
            {
                return G_SEC_NOTENOUGH_MEMORY; 
            }
        }
        break;
    default:
        {
            return G_SEC_INVALID_PARAMETER;
        }
    }    
    
    SEC_REGCALLBACK(SEC_IISHELL_POINTER,SEC_IAEECALLBACK_VAR,Sec_RunMainFlow,pMe);
    return G_SEC_OK;
}
#endif




