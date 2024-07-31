/*=====================================================================================
    FILE NAME :Sec_ue.h
    MODULE NAME :sec
    GENERAL DESCRIPTION
    define ue function
    

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    Modification                             Tracking
    Date            Author                  Number    Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-20 Bird                                     create sec_ue.h
    2007-05-17     tang                          Delete cert manage and signtext
=====================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef SEC_UE_H
#define SEC_UE_H
/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
#ifndef M_SEC_CFG_USE_CERT
#define M_SEC_CFG_USE_CERT
#endif


#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/*******************************************************************************
*   Structure Define Section
*******************************************************************************/
typedef struct tagSt_ShowDlg
{
    WE_INT32 iTargetId;
    WE_VOID* pvBuf;
    WE_UINT32 uiLength;
}St_ShowDlg;

#endif

/* Reasons to call dialog again */
#define M_SEC_UE_INPUT_TOO_SHORT 100
#define M_SEC_UE_INPUT_TOO_LONG  101

#define M_SEC_UE_INPUT_FALSE     102

#define M_SEC_UE_INPUT_MISIATCH  103

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

WE_BOOL 
Sec_UeConfirm(WE_HANDLE   hSecHandle, WE_INT32 iTargetId,WE_INT32 iMsg, WE_INT32 iYesMsg, WE_INT32 iNoMsg);

/*
* Create a warning dialog with the supplied message.
*/
WE_BOOL
Sec_UeWarning(WE_HANDLE   hSecHandle,WE_INT32 iTargetId, WE_INT32 iMsg);

#endif

/*
* Create a form with the supplied session info.
*/
#ifdef M_SEC_CFG_WTLS
WE_BOOL
Sec_UeSessionInfo(WE_HANDLE   hSecHandle,WE_INT32 iTargetId,St_SecSessionInfo stSessInf);
#endif
#ifdef M_SEC_CFG_USE_CERT
WE_BOOL
Sec_UeShowCertContent(WE_HANDLE hSecHandle,WE_INT32 iTargetId, WE_INT16 sIssuerCharSet, 
                              void *pvIssuer, WE_INT32 iIssuerLen,
                              WE_UINT32 iValidNotBefore, WE_UINT32 iValidNotAfter, 
                              WE_INT16 sSubjectCharSet, void *pvSubject, 
                              WE_INT32 iSubjectLen, WE_UCHAR *pucSerialNumber , 
                              WE_UINT16 usSerialNumberLen,WE_UCHAR* pucAlg,
                              WE_UCHAR* pucPubKey,WE_UINT16 usPubKeyLen,
                              WE_INT32 iCancelStr);
#endif /*M_SEC_CFG_USE_CERT*/


WE_INT32 
Sec_DecodeEvtAction(WE_VOID* pvSrcData,WE_UINT32 uiLen,WE_INT32 *piEvent,WE_VOID** pvDesData);

WE_VOID
Sec_ReleaseUeCB(WE_VOID* pvData);



#endif
