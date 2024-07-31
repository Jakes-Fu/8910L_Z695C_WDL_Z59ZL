/*=====================================================================================
    FILE NAME :Sec_iue.h
    MODULE NAME :sec
    GENERAL DESCRIPTION
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    Modification                     Tracking
    Date           Author          Number      Description of changes
    ---------- ---------- --------- --------------------------------------
    2006-07-20 Bird                              create sec_iue.h
    2007-05-17     tang                          Delete cert manage and signtext
=====================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef SEC_IUE_H
#define SEC_IUE_H

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

void 
Sec_iUeConfirm (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,WE_INT32 iMsg, WE_INT32 iYesMsg, WE_INT32 iNoMsg);


#ifdef M_SEC_CFG_USE_CERT
#ifdef M_SEC_CFG_WTLS
void 
Sec_iUeShowCertContent (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,
   WE_INT16 sIssuerCharSet, 
                        void *pvIssuer, WE_INT32 iIssuerLen,
                        WE_UINT32 uiValidNotBefore, WE_UINT32 uiValidNotAfter, 
                        WE_INT16 sSubjectCharSet, void *pvSubject, WE_INT32 iSubjectLen,
                        WE_UCHAR *pucSerialNumber , WE_UINT16 usSerialNumberLen,
                        WE_UCHAR* pucAlg,WE_UCHAR* pucPubKey,WE_UINT16 usPubKeyLen,
                        WE_INT32 iCancelStr);
#endif /*#M_SEC_CFG_WTLS*/
#endif

#ifdef M_SEC_CFG_WTLS
void 
Sec_iUeShowSessionContent (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,St_SecSessionInfo stSessInf);
#endif
void 
Sec_iUeWarning (WE_HANDLE   hSecHandle,WE_INT32 iTargetId,WE_INT32 iMsg);

#endif

