/*==============================================================================
    FILE NAME : sec_ueresp.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        In this file, define the Dialog response.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author         Number               Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-13          Tang                                       create sec_ueresp.c

============================================================================== */

 
/************************************************************************/
/*      Include File Section                                                                     */
/************************************************************************/

#include "sec_comm.h"

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
 
/*****************************************************************************************
        Macro Define Section
******************************************************************************************/
typedef St_PublicData                       St_SecGlobalData;

#define  M_SEC_CURRENT_TI_RESPONSE_VALUES    (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->pvCurUeResp)
#define  M_SEC_WANTS_TO_RUN                  (((St_SecGlobalData *)(((ISec*)hSecHandle)->hPrivateData))->iWantsToRun)

/************************************************************************/
/*      Function Define Section                                                                     */
/************************************************************************/

/*====================================================================================
FUNCTION: 
    Sec_UeConfirmDialogResp  
CREATE DATE: 
    2006-7-21
AUTHOR: 
    Tang  
DESCRIPTION:
     response of Confirm Dialog . 
ARGUMENTS PASSED:
    hSecHandle[IN/OUT]:   Global data handle
    ucAnswer[in]:         answer.       
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
=====================================================================================*/
void Sec_UeConfirmDialogResp(WE_HANDLE hSecHandle,WE_UINT8 ucAnswer)
{      
    St_SecTlConfirmDlgResp *pstP={0};
    WE_LOG_MSG ((WE_LOG_DETAIL_LOW, 0, 
                    "SEC: Sec_UeConfirmDialogResp(answer = %u)\n", ucAnswer));
    if(!hSecHandle)
    {
        return ;
    }

    pstP= ( St_SecTlConfirmDlgResp *)WE_MALLOC(sizeof( St_SecTlConfirmDlgResp));
    if (!pstP)
    {
        M_SEC_CURRENT_TI_RESPONSE_VALUES = NULL;
        return;
    }
    pstP->eType = E_SEC_DP_CONFIRM_DLG;
    pstP->ucAnswer = ucAnswer;
    M_SEC_CURRENT_TI_RESPONSE_VALUES = ( St_SecTlUeFunctionId *)pstP;
    M_SEC_WANTS_TO_RUN = 1;
}

#endif

