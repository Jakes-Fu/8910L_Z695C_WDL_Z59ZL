/*=====================================================================================
    FILE NAME : sec_iue.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:       
        This file provides a set of functions, which is for dialogue.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author         Number                  Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-20           Bird                                           create sec_iue.c
    2007-05-17           tang                                 Delete cert manage and signtext
=====================================================================================*/

/***************************************************************************************
*   Include File Section
****************************************************************************************/

#include"sec_comm.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#define SEC_IFTYPE              (((ISec*)hSecHandle)->iIFType)

/*******************************************************************************
*   Function Define Section
*******************************************************************************/

#ifdef M_SEC_CFG_USE_CERT
/*==================================================================================================
FUNCTION: 
    Sec_iUeShowCertContent
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    call Sec_UeShowCertContent fuction to create dialog
    
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN/OUT]: Global data handle.
    WE_INT16 sIssuerCharSet[IN]:defined character set for WTLS certificates. For X.509 certificates this value is -1
    void *pvIssuer[IN]:The issuer of the certificate.  
                                X.509: DER encoded issuer beginning with a "SEQUENCE OF" 
                                according to the X.509 specification. [X.509] 
                                WTLS: Name according to the WTLS specification.[WAP-WTLS] 
    WE_INT32 iIssuerLen[IN]:The length of issuer
    WE_UINT32 uiValidNotBefore[IN]:The beginning of the validity period of the certificate
    WE_UINT32 uiValidNotAfter[IN]:The end of the validity period of the certificate
    WE_INT16 sSubjectCharSet[IN]:defined character set for WTLS certificates. For X.509 certificates this value is -1
    void *pvSubject[IN]:The subject of the certificate
    WE_INT32 iSubjectLen[IN]:The length of subject
    WE_UCHAR *pucFingerPrint[IN]:The fingerprint of the certificate
    WE_INT32 iFingerPrintLen[IN]:The length of the fingerprint
    WE_INT32 iCancelStr[IN]:The message to display on the cancel choice
RETURN VALUE:
    NULL
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
void 
Sec_iUeShowCertContent (WE_HANDLE  hSecHandle,WE_INT32 iTargetId,
                        WE_INT16 sIssuerCharSet, 
                        void *pvIssuer, WE_INT32 iIssuerLen,
                        WE_UINT32 uiValidNotBefore, WE_UINT32 uiValidNotAfter, 
                        WE_INT16 sSubjectCharSet, void *pvSubject, WE_INT32 iSubjectLen,
                        WE_UCHAR *pucSerialNumber , WE_UINT16 usSerialNumberLen,
                        WE_UCHAR* pucAlg,WE_UCHAR* pucPubKey,WE_UINT16 usPubKeyLen,
                        WE_INT32 iCancelStr)
{
    WE_BOOL bRet = FALSE;
    if(NULL == hSecHandle)
    {
        #ifdef M_SEC_CFG_SHOW_WARNINGS
        Sec_iUeWarning (hSecHandle,iTargetId,M_SEC_ERR_GENERAL_ERROR); 
        #endif
        return;
    }
    WE_LOG_MSG(( 0,0,"SEC:Sec_iUeShowCertContent\n"));
    bRet = Sec_UeShowCertContent (hSecHandle, iTargetId,sIssuerCharSet, 
        pvIssuer, iIssuerLen,
        uiValidNotBefore, uiValidNotAfter, 
        sSubjectCharSet, pvSubject, iSubjectLen,
        pucSerialNumber, usSerialNumberLen,
        pucAlg,pucPubKey,usPubKeyLen,
        iCancelStr);
    if (!bRet) 
    {
        #ifdef M_SEC_CFG_SHOW_WARNINGS
        Sec_iUeWarning (hSecHandle,iTargetId,M_SEC_ERR_GENERAL_ERROR); 
        #endif
    }
    else 
    {
        /* sec_tiPushWindow (dlg);*/
    }
}
#endif /*#M_SEC_CFG_WTLS*/
#endif 

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/*==================================================================================================
FUNCTION: 
    Sec_iUeConfirm
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    call Sec_UeConfirm fuction to create dialog, see description of   Sec_UeConfirm    
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN/OUT]: Global data handle.
    WE_INT32 iMsg[IN]:An index to the message displayed to the end user.
    WE_INT32 iYesMsg[IN]:An index to the message displayed on the "yes" button
    WE_INT32 iNoMsg[IN]:An index to the message displayed on the "no" button
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void 
Sec_iUeConfirm (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,WE_INT32 iMsg, WE_INT32 iYesMsg, WE_INT32 iNoMsg)
{
    WE_BOOL bRet = FALSE;
    WE_LOG_MSG(( 0,0,"SEC:Sec_iUeConfirm\n"));
    bRet = Sec_UeConfirm (hSecHandle,iTargetId, iMsg, iYesMsg, iNoMsg);
    
    if (!bRet) 
    {
        Sec_UeConfirmDialogResp(hSecHandle,FALSE);
    }
}

#endif

/*==================================================================================================
FUNCTION: 
    Sec_iUeShowSessionContent
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    call Sec_UeSessionInfo fuction to create dialog,see description of   Sec_iUeShowSessionContent    
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN/OUT]: Global data handle.
    St_SecSessionInfo stSessInf[IN]:The session information.
RETURN VALUE:
    NULL
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
void 
Sec_iUeShowSessionContent (WE_HANDLE hSecHandle,WE_INT32 iTargetId,St_SecSessionInfo stSessInf)
{
    WE_BOOL bRet = FALSE;
    
    WE_LOG_MSG(( 0,0,"SEC:Sec_iUeShowSessionContent\n"));
    bRet = Sec_UeSessionInfo (hSecHandle, iTargetId,stSessInf);
    
    if (!bRet) 
    {
        #ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        #ifdef M_SEC_CFG_SHOW_WARNINGS
        Sec_iUeWarning (hSecHandle,iTargetId,M_SEC_ERR_GENERAL_ERROR);
        #endif
        #endif
    }
    else 
    {
        /* sec_tiPushWindow (dlg);*/
    }
}
#endif

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

#ifdef M_SEC_CFG_SHOW_WARNINGS
/*==================================================================================================
FUNCTION: 
    Sec_iUeWarning
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    call Sec_UeShowCertList fuction to create dialog,see description of   Sec_iUeWarning    
ARGUMENTS PASSED:
    WE_HANDLE hSecHandle [IN/OUT]: Global data handle.
    WE_INT32 iMsg[IN]:An index to the message displayed to the end user.
RETURN VALUE:
    NULL
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void 
Sec_iUeWarning (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,WE_INT32 iMsg)
{  
    WE_BOOL bResult = TRUE;
    WE_LOG_MSG((0,0, "SEC:Sec_iUeWarning\n"));
    bResult = Sec_UeWarning (hSecHandle,iTargetId, iMsg);
    if (bResult != TRUE)
    {
        return;
    }
    /* sec_tiPushWindow (dlg);*/
}           
#endif 

/*****************************************************************************
*Interface to STK or Borwser, Called after the customer finish operation on dialog and push
*OK or Cancel button
******************************************************************************/

/*1.confirm*/
/*==================================================================================================
FUNCTION: 
    Sec_UeConfirmAction
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    confirm dialog response
    
ARGUMENTS PASSED:
WE_HANDLE hSecHandle [IN/OUT]: Global data handle.
St_ConfirmAction stConfirm[IN]:user data

RETURN VALUE:
    M_SEC_ERR_OK is success ,otherwise is fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 Sec_UeConfirmAction(WE_HANDLE hSecHandle ,WE_INT32 iTargetId,St_ConfirmAction stConfirm)
{
    iTargetId=iTargetId;
    WE_LOG_MSG( (0,0,"SEC: Sec_UeConfirmAction(bIsOk = %d)", stConfirm.bIsOk));

    if(NULL == hSecHandle)
    {
         Sec_UeConfirmDialogResp (hSecHandle, FALSE);
         return M_SEC_ERR_INVALID_PARAMETER;
    }
    if(!stConfirm.bIsOk)
    {
        Sec_UeConfirmDialogResp (hSecHandle, FALSE);
    }
    else
    {
        Sec_UeConfirmDialogResp (hSecHandle, TRUE);
    }
    return M_SEC_ERR_OK;    
}

#endif

