/*=====================================================================================
    FILE NAME : sec_ue.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This file provides a set of functions, which is for dialogue.        
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author          Number             Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-04          Kevin                                        create sec_ue.c
    2007-05-17     tang                          Delete cert manage and signtext
=====================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include"sec_comm.h"
#include "oem_seccsc.h"


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define M_SEC_HASH_MD5               0x01 
#define M_SEC_HASH_SHA               0x02 

#define M_SEC_CFG_SHOW_COUNTRY_NAME             0x00000001
#define M_SEC_CFG_SHOW_ORGANISATION_NAME        0x00000002
#define M_SEC_CFG_SHOW_ORGANISATIONAL_UNIT_NAME 0x00000004
#define M_SEC_CFG_SHOW_STATE_OR_PROVINCE_NAME   0x00000008
#define M_SEC_CFG_SHOW_COMMON_NAME              0x00000010
#define M_SEC_CFG_SHOW_DOMAIN_COMPONENT         0x00000020
#define M_SEC_CFG_SHOW_SERIAL_NUMBER            0x00000040
#define M_SEC_CFG_SHOW_DN_QUALIFIER             0x00000080
#define M_SEC_CFG_SHOW_LOCALITY_NAME            0x00000100
#define M_SEC_CFG_SHOW_TITLE                    0x00000200
#define M_SEC_CFG_SHOW_SURNAME                  0x00000400
#define M_SEC_CFG_SHOW_GIVENNAME                0x00000800
#define M_SEC_CFG_SHOW_INITIALS                 0x00001000
#define M_SEC_CFG_SHOW_GENERATION_QUALIFIER     0x00002000
#define M_SEC_CFG_SHOW_NAME                     0x00004000
#define M_SEC_CFG_SHOW_EMAIL_ADDRESS            0x00008000
#define M_SEC_CFG_SHOW_UNKNOWN_ATTRIBUTE_TYPE   0x00010000

/* Global variables*/
#define SEC_CBBRS_EVTFUNC     (((ISec*)hSecHandle)-> hcbSecEvtFunc)
#define SEC_CBWAP_EVTFUNC   (((ISec*)hSecHandle)-> hcbSecEvtFunc)
#define SEC_BRS_PRIVDATA       (((ISec*)hSecHandle)-> hSecPrivDataCB)
#define SEC_WAP_PRIVDATA      (((ISec*)hSecHandle)-> hSecPrivDataCB)
#define SEC_CSC_SHELL              (((ISec*)hSecHandle)-> m_pIShell)
#define SEC_IFTYPE                     (((ISec*)hSecHandle)->iIFType)
#define M_SEC_UECALLBACK       (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->m_SecUeCB)
#define M_SEC_UECALLBACKDATA  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->pvUeCBData)
#define M_SEC_UECALLBACKEVENT  (((St_PublicData *)(((ISec*)hSecHandle)->hPrivateData))->iUeEvent)

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
static WE_BOOL Sec_UeSendEvent(WE_HANDLE hSecHandle,WE_INT32 iTargetId, WE_INT32 eSecEvt,void *pvData);

#ifdef M_SEC_CFG_WTLS
static WE_INT32 Sec_UeTimeToStr(WE_UINT32 uiTime, WE_CHAR **ppcTimeStr);
static WE_INT32 Sec_UeCsToUtf8(WE_HANDLE hSecHandle,WE_UCHAR *pucStrToCvt, WE_LONG lStrToCvtLen, 
                               WE_CHAR *pcDst, WE_LONG *plDstLen, WE_INT16 sCharSet);
static WE_CHAR* Sec_UeGetIssuerStr(WE_HANDLE hSecHandle,WE_INT16 sCharSet, 
                                   void *pvIssuer, WE_INT32 iIssuerLen);
static WE_CHAR* Sec_UeCreateFingerpr(WE_UCHAR *pucFingerPrint,WE_INT32 iFingerPrintLen);
#endif
static WE_VOID Sec_RunUeCallBackFun(WE_VOID* pvData);
static WE_VOID Sec_UeFreeMem(WE_INT32 eSecEvt, void* pvData);

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/*==================================================================================================
FUNCTION: 
    Sec_UeConfirm
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    displays a dialog to the end user with a given message    
ARGUMENTS PASSED:
    WE_INT32 iMsg[IN]:An index to the message displayed to the end user.
    WE_INT32 iYesMsg[IN]:An index to the message displayed on the "yes" button
    WE_INT32 iNoMsg[IN]:An index to the message displayed on the "no" button
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_BOOL Sec_UeConfirm(WE_HANDLE hSecHandle,WE_INT32 iTargetId,
                            WE_INT32 iMsg, WE_INT32 iYesMsg, WE_INT32 iNoMsg)
{
    St_Confirm *pstConfirm = NULL;
    iYesMsg = iYesMsg;
    iNoMsg = iNoMsg;
    if(NULL == (pstConfirm = (St_Confirm*)WE_MALLOC(sizeof(St_Confirm))))
    {
        WE_LOG_MSG((0,0, "SEC:malloc1 error in Sec_UeConfirm!\n"));
        return FALSE;
    }
    (void)WE_MEMSET((void*)pstConfirm,0,sizeof(St_Confirm));
    pstConfirm->iConfirmStrId = iMsg;
    /*send event to stk*/
    return Sec_UeSendEvent(hSecHandle,iTargetId, M_UE_CONFIRM_EVENT, (void *)pstConfirm);
}

#ifdef M_SEC_CFG_SHOW_WARNINGS
/*==================================================================================================
FUNCTION: 
    Sec_UeWarning
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    Create a warning dialog with the supplied message

ARGUMENTS PASSED:
    WE_INT32 iMsg[IN]:An index to the message displayed to the end user.
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_BOOL Sec_UeWarning(WE_HANDLE hSecHandle, WE_INT32 iTargetId,WE_INT32 iMsg)
{
    St_Warning *pstWaring = NULL;

    if (NULL == hSecHandle)
    {
        return FALSE;
    }
    if(NULL == (pstWaring = (St_Warning*)WE_MALLOC(sizeof(St_Warning))))
    {
        WE_LOG_MSG((0,0, "SEC:malloc1 error in Sec_UeWarning!\n"));
        return FALSE;
    }
    (void)WE_MEMSET(pstWaring,0,sizeof(St_Warning));
    
    /* Assign the warning message */
    switch (iMsg)
    {
        case M_SEC_ERR_INSUFFICIENT_MEMORY:
            pstWaring->iWarningContent = M_SEC_ID_WARNING_INSUFFICIENT_MEMORY;
            break;
        case M_SEC_ERR_GENERAL_ERROR:
            pstWaring->iWarningContent = M_SEC_ID_WARNING_GENERAL_ERROR;
            break;
        case M_SEC_ERR_INVALID_PARAMETER:
            pstWaring->iWarningContent = M_SEC_ID_WARNING_INVALID_PARAMETER;
            break;
        default:
            WE_FREE(pstWaring);
            return FALSE;
    }    
    /*send event to STK*/
    return Sec_UeSendEvent(hSecHandle,iTargetId, M_UE_WARNING_EVENT, (void *)pstWaring);
}
#endif
#endif
/*==================================================================================================
FUNCTION: 
    Sec_UeSessionInfo
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    shows the session info of the current session
 ARGUMENTS PASSED:
    St_SecSessionInfo stSessInf[IN]:The session information.
RETURN VALUE:
    TRUE: success
    FALSE: fail
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
WE_BOOL Sec_UeSessionInfo(WE_HANDLE hSecHandle,WE_INT32 iTargetId,St_SecSessionInfo stSessInf)
{
    St_ShowSessionContent *pstSessionInfo = NULL;
    WE_CHAR* pcText = NULL;
    if((pstSessionInfo = (St_ShowSessionContent*)WE_MALLOC(sizeof(St_ShowSessionContent))) == NULL)
    {
        WE_LOG_MSG((0,0, "SEC:malloc error1 in Sec_UeSessionInfo!\n"));
        return FALSE;
    }
    (void)WE_MEMSET(pstSessionInfo,0,sizeof(St_ShowSessionContent));
    
    switch (stSessInf.ucConnectionType)
    {        
        case M_SEC_WTLS_CONNECTION_TYPE:
            pstSessionInfo->iConnTypeId = (M_SEC_ID_SESSION_WTLS);
            break;
            
        default:
            WE_LOG_MSG((0,0, "SEC:connectionType is not know in Sec_UeSessionInfo!\n"));
            WE_FREE(pstSessionInfo);
            return FALSE;
    }
    
    switch (stSessInf.ucHmacAlg)
    {
        case M_SEC_HASH_MD5:
            pstSessionInfo->iHmacId = (M_SEC_ID_SESSION_MD5);
            break;
        case M_SEC_HASH_SHA:
            pstSessionInfo->iHmacId = (M_SEC_ID_SESSION_SHA1);
            break;
        default:
            WE_LOG_MSG((0,0, "SEC:HMAC alg is not know in Sec_UeSessionInfo!\n"));
            WE_FREE(pstSessionInfo);
            return FALSE;
    }
    /*HMAC KEY LEN*/
    if(NULL == (pcText = (WE_CHAR *)WE_MALLOC(4)))
    {
        WE_LOG_MSG((0,0, "SEC:malloc error2 in Sec_UeSessionInfo!\n"));
        WE_FREE(pstSessionInfo);
        return FALSE;
    }
    (void)WE_MEMSET(pcText,0,4);
    (void)SEC_SPRINTF(pcText, "%u", stSessInf.usHmacLen);
    pstSessionInfo->pcHmacKeyLenStr = pcText;
    
    /*ENCRYPTION ALG*/
    switch (stSessInf.ucEncryptionAlg)
    {
        case E_SEC_BULK_NULL:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_NULL);
            break;
        case E_SEC_BULK_RC5_CBC_40:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_RC5_40);
            break;
        case E_SEC_BULK_RC5_CBC_56:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_RC5_56);
            break;
        case E_SEC_BULK_RC5_CBC:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_RC5);
            break;
        case E_SEC_BULK_3DES_CBC_EDE:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_3DES);
            break;
        case E_SEC_BULK_RC5_CBC_64:
            pstSessionInfo->iEncralgId = (M_SEC_ID_SESSION_BULK_RC5_64);
            break;
        default:
            WE_LOG_MSG((0,0, "SEC:Encry alg is not know in Sec_UeSessionInfo!\n"));
            WE_FREE(pstSessionInfo);
            return FALSE;
    }
    /*ENCRYPTION KEY LEN*/
    if(NULL == (pcText = (WE_CHAR *)WE_MALLOC(4)))
    {
        WE_LOG_MSG(( 0,0,"SEC:malloc error3 in Sec_UeSessionInfo!\n"));
        WE_FREE(pstSessionInfo->pcHmacKeyLenStr);
        WE_FREE(pstSessionInfo);
        return FALSE;
    }
    (void)WE_MEMSET(pcText,0,4);
    (void)SEC_SPRINTF(pcText, "%d", stSessInf.usEncryptionKeyLen);
    pstSessionInfo->pcEncrKeyLenStr = pcText;
    
    /* Create key exchange alg  */
    
    switch (stSessInf.ucKeyExchangeAlg)
    {  
        case E_SEC_WTLS_KEYEXCHANGE_NULL:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_NULL);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA_ANON);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_512:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA_ANON_512);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_ANON_768:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA_ANON_768);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_512:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA_512);
            break;
        case E_SEC_WTLS_KEYEXCHANGE_RSA_768:
            pstSessionInfo->iKeyExchId = (M_SEC_ID_SESSION_KEYEXCH_RSA_768);
            break;
        default:
            WE_LOG_MSG((0,0, "SEC:WTLS key exchange alg is not know in Sec_UeSessionInfo!\n"));
            WE_FREE(pstSessionInfo->pcHmacKeyLenStr);
            WE_FREE(pstSessionInfo->pcEncrKeyLenStr);
            WE_FREE(pstSessionInfo);
            return FALSE;
    }
    /*Key exchange len*/
    pcText = (WE_CHAR *)WE_MALLOC(6);
    if(NULL == pcText)
    {
        WE_LOG_MSG((0,0, "SEC:malloc error4 in Sec_UeSessionInfo!\n"));
        WE_FREE(pstSessionInfo->pcHmacKeyLenStr);
        WE_FREE(pstSessionInfo->pcEncrKeyLenStr);
        WE_FREE(pstSessionInfo);
        return FALSE;
    }
    (void)WE_MEMSET(pcText,0,6);
    (void)SEC_SPRINTF(pcText, "%d", stSessInf.usKeyExchangeKeyLen);
    pstSessionInfo->pcKeyExchKeyLenStr = pcText;    
    /*send event to browser*/
    return Sec_UeSendEvent(hSecHandle,iTargetId, M_UE_SHOW_SESSION_CONTENT_EVENT, (void *)pstSessionInfo);
}

#ifdef M_SEC_CFG_USE_CERT
/*==================================================================================================
FUNCTION: 
    Sec_UeShowCertContent
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    displays a dialog to the end user with the given certificate information
ARGUMENTS PASSED:
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
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_BOOL Sec_UeShowCertContent(WE_HANDLE hSecHandle,WE_INT32 iTargetId,
                                WE_INT16 sIssuerCharSet, 
                                void *pvIssuer, WE_INT32 iIssuerLen,
                                  WE_UINT32 iValidNotBefore, WE_UINT32 iValidNotAfter, 
                                  WE_INT16 sSubjectCharSet, void *pvSubject, 
                                  WE_INT32 iSubjectLen, WE_UCHAR *pucSerialNumber , 
                                  WE_UINT16 usSerialNumberLen,WE_UCHAR* pucAlg,
                                  WE_UCHAR* pucPubKey,WE_UINT16 usPubKeyLen,
                                  WE_INT32 iCancelStr)
{
    St_ShowCertContent *pstShowCert = NULL;
    WE_CHAR* pcTextIssuer = NULL;
    WE_CHAR* pcTextSub = NULL;
    WE_CHAR* pcTimeAft = NULL;
    WE_CHAR* pcTimeBef = NULL;
    WE_BOOL   bFlag = FALSE;

    if (M_SEC_ID_NONE != iCancelStr )
    {
        return  FALSE;
    }
    
    if(NULL == (pstShowCert = (St_ShowCertContent*)WE_MALLOC(sizeof(St_ShowCertContent))))
    {
        WE_LOG_MSG((0,0, "SEC:malloc error1 in Sec_UeShowCertContent!\n"));
        return FALSE;
    }
    (void)WE_MEMSET(pstShowCert,0,sizeof(St_ShowCertContent));
    
    if ( NULL == (pcTextIssuer = Sec_UeGetIssuerStr(hSecHandle,sIssuerCharSet, 
                                                    pvIssuer, iIssuerLen)))
    {
        WE_LOG_MSG((0,0, "SEC:malloc error2 in Sec_UeShowCertContent!\n"));
        WE_FREE(pstShowCert);
        return FALSE;
    }
    pstShowCert->pcIssuerStr = pcTextIssuer;
    
    if (NULL == (pcTextSub = Sec_UeGetIssuerStr(hSecHandle,sSubjectCharSet, 
                                                pvSubject, iSubjectLen )))
    {
        WE_LOG_MSG((0,0, "SEC:malloc error3 in Sec_UeShowCertContent!\n"));
        WE_FREE(pstShowCert);
        WE_FREE(pcTextIssuer);
        return FALSE;
     }
    pstShowCert->pcSubjectStr = pcTextSub;
    
    /*NotBefore*/
    pcTimeBef = (WE_CHAR*)WE_MALLOC(30);
    if(NULL == pcTimeBef)
    {
        WE_LOG_MSG((0,0, "SEC:malloc error4 in Sec_UeShowCertContent!\n"));
        WE_FREE(pstShowCert);
        WE_FREE(pcTextSub);
        return FALSE;
    }
    
    pstShowCert->iNotBefMonth = Sec_UeTimeToStr(iValidNotBefore,&pcTimeBef);
    pstShowCert->pcNotBefStr = pcTimeBef;
    
    /*Not After*/
    pcTimeAft = (WE_CHAR*)WE_MALLOC(30);
    if(NULL == pcTimeAft)
    {
        WE_LOG_MSG(( 0,0,"SEC:malloc error5 in Sec_UeShowCertContent!\n"));
        WE_FREE(pstShowCert);
        WE_FREE(pcTextSub);
        WE_FREE(pcTimeBef);
        return FALSE;
    }
    
    pstShowCert->iNotAftMonth = Sec_UeTimeToStr(iValidNotAfter,&pcTimeAft);
    pstShowCert->pcNotAftStr = pcTimeAft;
    
    /*serial number*/
    pstShowCert->pucSerialNumber = (WE_UCHAR*)Sec_UeCreateFingerpr(pucSerialNumber,usSerialNumberLen);

    /*alg*/
    pstShowCert->pucAlg = (WE_UCHAR *)SEC_STRDUP((WE_CHAR *)pucAlg);

    /*pub key*/
    pstShowCert->pucPubKey = (WE_UCHAR*)Sec_UeCreateFingerpr(pucPubKey,usPubKeyLen);
        
    /*send event to browser*/
    bFlag =  Sec_UeSendEvent(hSecHandle,iTargetId, M_UE_CURSVRCERT, (void *)pstShowCert);

    return bFlag;

}
#endif /*M_SEC_CFG_USE_CERT*/
#endif /*#M_SEC_CFG_WTLS*/


/*==================================================================================================
FUNCTION: 
    Sec_UeTimeToStr
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    Converts the supported time to a string  format . 
ARGUMENTS PASSED:
    WE_UINT32 uiTime[IN]:the time wanted to convert.
    WE_CHAR **ppcTimeStr[OUT]:string format as "day year".
RETURN VALUE:
    month index 
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
static WE_INT32 Sec_UeTimeToStr(WE_UINT32 uiTime, WE_CHAR **ppcTimeStr)
{
    WE_UINT32 uiTmpTime = uiTime;
    WE_UINT32 uiSeconds = 0;
    WE_UINT32 uiMinutes = 0;
    WE_UINT32 uiHours = 0;
    WE_UINT32 uiDays = 0;
    WE_UINT32 uiDaysIn31month = 31;
    WE_UINT32 uiDaysIn30month = 30;
    WE_UINT32 uiDaysIn29month = 29;
    WE_UINT32 uiDaysIn28month = 28;
    WE_INT32  iNbrYears = 0; 
    WE_INT32  iLeapYear = 0;
    WE_UINT32 uiYear = 0;
    WE_INT32  iTmpStrID = 0;

    if(NULL == ppcTimeStr)
    {
        return 0;
    }
    /* modify by Sam [070316] */
    /* add time to display with date in the period of validity */
    /* Get days since 1/1 1970 */
    uiDays = uiTmpTime / (60*60*24);
    /* Get seconds current day */
    uiTmpTime = uiTmpTime % (60*60*24);
    /* Get current hours */
    uiHours = uiTmpTime / (60*60);
    /* get minutes current day */
    uiTmpTime = uiTmpTime % (60*60);
    /* Get current minutes */
    uiMinutes = uiTmpTime / 60;
    /* get current seconds */
    uiSeconds = uiTmpTime % 60;
    /* Get years since 1/1 1970 */
    uiYear = 1970 + uiDays / 365;
    
    /* 1972 is the first leap year after 1970 */
    iLeapYear = 1;
    /* For each year since 1970 subtract the days in each month */
    while ((uiDays > 0) && (iNbrYears >= 0))
    {
        iLeapYear++;
        if (uiDays <= 30)   /* January */
        {
            iTmpStrID = (M_SEC_ID_JAN);
            break;
        }
        uiDays -= uiDaysIn31month;    
        if (iLeapYear == 4)
        {
            if (uiDays <= 28) /* February */
            {
                iTmpStrID = (M_SEC_ID_FEB);
                break;
            }
            uiDays -= uiDaysIn29month;  
        }
        else
        {
            if (uiDays <= 27) /* February */
            {
                iTmpStrID = (M_SEC_ID_FEB);
                break;
            }
            uiDays -= uiDaysIn28month;  
        }
        if (uiDays <= 30)   /* March */
        {
            iTmpStrID = (M_SEC_ID_MARCH);
            break;
        }
        uiDays -= uiDaysIn31month;    
        if (uiDays <= 29)   /* April */
        {
            iTmpStrID = (M_SEC_ID_APR);
            break;
        }
        uiDays -= uiDaysIn30month;    
        if (uiDays <= 30)   /* May */
        {
            iTmpStrID = (M_SEC_ID_MAY);
            break;
        }
        uiDays -= uiDaysIn31month;    
        if (uiDays <= 29)   /* June */
        {
            iTmpStrID= (M_SEC_ID_JUNE);
            break;
        }
        uiDays -= uiDaysIn30month;    
        if (uiDays <= 30)   /* July */
        {
            iTmpStrID = (M_SEC_ID_JULY);
            break;
        }
        uiDays -= uiDaysIn31month;    
        if (uiDays <= 30)   /* August */
        {
            iTmpStrID = (M_SEC_ID_AUG);
            break;
        }
        uiDays -= uiDaysIn31month;    
        if (uiDays <= 29)   /* September */
        {
            iTmpStrID = (M_SEC_ID_SEP);
            break;
        }
        uiDays -= uiDaysIn30month;    
        if (uiDays <= 30)   /* October */
        {
            iTmpStrID = (M_SEC_ID_OCT);
            break;
        }
        uiDays -= uiDaysIn31month;   
        if (uiDays <= 29)   /* November */
        {
            iTmpStrID = (M_SEC_ID_NOV);
            break;
        }
        uiDays -= uiDaysIn30month;    
        if (uiDays <= 30)   /* December */
        {
            iTmpStrID = (M_SEC_ID_DEC);
            break;
        }
        uiDays -= uiDaysIn31month;    
        iNbrYears++;
        if (iLeapYear == 4)
        {
            iLeapYear = 0;
        }
    } /* while */
    if ((uiDays == 0) && (iTmpStrID == 0))
    {
        iTmpStrID = (M_SEC_ID_JAN);    
    }
    uiDays++;   /* To get the correct date, since 0 is no a date */
    (void)SEC_SPRINTF(*ppcTimeStr, "%02u/%02d/%04u  %02u:%02u:%02u", uiDays, iTmpStrID, uiYear, uiHours, uiMinutes, uiSeconds);
    
    return iTmpStrID; 
}

/* Convert the supplied strToCvt to UTF-8 and store in the
 * supplied dst. Returns 0 on success and -1 otherwise. */
/*==================================================================================================
FUNCTION: 
    Sec_UeCsToUtf8
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    convert string to UTF8 fromat
ARGUMENTS PASSED:
    WE_UCHAR *pucStrToCvt[IN]:source string.
    WE_LONG lStrToCvtLen[IN]:source string length
    WE_CHAR *pcDst[OUT]:dst buffer
    WE_LONG *plDstLen[OUT]:dst length
    WE_INT16 sCharSet[IN]:charset
RETURN VALUE:
    0: sucess
    -1:fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 Sec_UeCsToUtf8(WE_HANDLE hSecHandle,WE_UCHAR *pucStrToCvt, WE_LONG lStrToCvtLen, 
                               WE_CHAR *pcDst, WE_LONG *plDstLen, WE_INT16 sCharSet)
{
    WE_INT32 iResult = M_SEC_ERR_OK;
    if((NULL == hSecHandle) || (NULL == pcDst) || (NULL == plDstLen))
    {
        return -1;
    }
    
    /* I WTLS: IANA defined character sets */
    /*WE_CSC_UTF_8*/
    switch (sCharSet)
    {
        case WE_CSC_UTF_8:           /* WTLS charSet */ 
        case WE_ASN1_UTF8STRING:        /* X.509 charSet */ 
            (void)WE_MEMCPY(pcDst, pucStrToCvt, (WE_UINT32)lStrToCvtLen);
            *plDstLen = lStrToCvtLen;
            break;        
        case WE_CSC_LATIN_1:         /* WTLS charSet */
        case WE_ASN1_T61STRING:         /* X.509 charSet, TeletexString */
        case WE_ASN1_PRINTABLESTRING:  /* X.509 charSet */
        case WE_ASN1_IA5STRING:   /* X.509 charSet */
            {
                if(!Sec_Csc(NULL,WE_CSC_LATIN_1,WE_CSC_UTF_8,(WE_CHAR*)pucStrToCvt,\
                            lStrToCvtLen,pcDst,plDstLen))
                {
                    WE_LOG_MSG((0,0, "SEC:Sec_UeCsToUtf8 error1 !\n"));
                    return -1;
                }
            }        
            break;
            
        case WE_ASN1_UNIVERSALSTRING:   /* X.509 charSets */
        case WE_ASN1_BMPSTRING:         /* X.509 charSets */
            {
                
                if(!Sec_Csc(NULL,WE_CSC_UCS_2,WE_CSC_UTF_8,(WE_CHAR*)pucStrToCvt,\
                            lStrToCvtLen,pcDst,plDstLen))
                {
                    WE_LOG_MSG((0,0, "SEC:Sec_UeCsToUtf8 error2 !\n"));
                    return -1;
                }            
            }        
            break;        
        default:
            return -1;
    }
    pcDst[*plDstLen] = '\0';
    return iResult;
}
#endif

/*==================================================================================================
FUNCTION: 
    Sec_UeSendEvent
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    registry callback , notify other module to create dialog
ARGUMENTS PASSED:
    WE_INT32 eSecEvt[IN]:dialog event.
    void *pvData[IN]:data of dialog
RETURN VALUE:
    TRUE:sucess
    FALSE:fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_BOOL Sec_UeSendEvent(WE_HANDLE hSecHandle, WE_INT32 iTargetId,WE_INT32 eSecEvt,void *pvData)
{ 
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    WE_INT32     iRes = 0;
    St_ShowDlg* pstShowDlg = NULL;
#endif

    if((NULL == hSecHandle) || (NULL == pvData))
    {
        Sec_UeFreeMem(eSecEvt, pvData);
        return FALSE;
    }

    /*modified by Bird 070123*/
    if((E_SEC_BRS == (E_IFType)SEC_IFTYPE )) /*brs & signtext*/
    {
        /*fill the struct*/
        M_SEC_UECALLBACKEVENT = eSecEvt;
        M_SEC_UECALLBACKDATA = (WE_VOID*)pvData;
        SEC_REGCALLBACK(SEC_CSC_SHELL,M_SEC_UECALLBACK,Sec_RunUeCallBackFun,hSecHandle);
     }
    else /*wtls*/
    {
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
#ifdef M_SEC_WTLS_NO_CONFIRM_UE  /*wtls& signtext no confirm*/
        if(M_UE_CONFIRM_EVENT ==eSecEvt )
        {
            St_ConfirmAction stConfirm = {0};
            stConfirm.bIsOk = TRUE;
            iRes = Sec_UeConfirmAction(hSecHandle,iTargetId,stConfirm);
            Sec_UeFreeMem(eSecEvt, pvData);
            if( 0 != iRes)
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }

        }
        else
        {
            /*1.encode*/
            pstShowDlg = (St_ShowDlg*)WE_MALLOC(sizeof(St_ShowDlg));
            if(NULL == pstShowDlg)
            {
                Sec_UeFreeMem(eSecEvt, pvData);
                return FALSE;
            }
            pstShowDlg->iTargetId = iTargetId;
            iRes = Sec_EncodeEvt(eSecEvt,pvData,&(pstShowDlg->pvBuf),&(pstShowDlg->uiLength));
            if(FALSE == iRes)
            {
                WE_FREE(pstShowDlg);
                Sec_UeFreeMem(eSecEvt, pvData);
                return FALSE;
            }
            /*2.send event*/
            /*fill the struct*/
            M_SEC_UECALLBACKEVENT = G_Evt_SHOW_DLG;
            M_SEC_UECALLBACKDATA = (WE_VOID*)pstShowDlg;
            SEC_REGCALLBACK(SEC_CSC_SHELL,M_SEC_UECALLBACK,Sec_RunUeCallBackFun,hSecHandle);
            Sec_UeFreeMem(eSecEvt, pvData);    
            
        }
#else
            /*1.encode*/
            pstShowDlg = (St_ShowDlg*)WE_MALLOC(sizeof(St_ShowDlg));
            if (NULL == pstShowDlg)
            {
                Sec_UeFreeMem(eSecEvt, pvData);
                return FALSE;
            }
            (void)WE_MEMSET( pstShowDlg ,0,sizeof(St_ShowDlg) );
            pstShowDlg->iTargetId = iTargetId;
            iRes = Sec_EncodeEvt(eSecEvt,pvData,&(pstShowDlg->pvBuf),&(pstShowDlg->uiLength));
            if(FALSE == iRes)
            {
                WE_FREE(pstShowDlg);
                Sec_UeFreeMem(eSecEvt, pvData);
                return FALSE;
            }
            /*2.send event*/
            /*fill the struct*/
            M_SEC_UECALLBACKEVENT = G_Evt_SHOW_DLG;
            M_SEC_UECALLBACKDATA = (WE_VOID*)pstShowDlg;
            SEC_REGCALLBACK(SEC_CSC_SHELL,M_SEC_UECALLBACK,Sec_RunUeCallBackFun,hSecHandle);
            Sec_UeFreeMem(eSecEvt, pvData);

#endif
#else
        /* add for klocwork, actually it's not used. */
        {
            Sec_UeFreeMem(eSecEvt, pvData);
            return FALSE;
        }
#endif        
    }   
    iTargetId = iTargetId;
    return TRUE;
}

/*==================================================================================================
FUNCTION: 
    Sec_UeGetIssuerStr
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    according to charset get issuer string.    
ARGUMENTS PASSED:
    WE_INT16 sCharSet[IN]:certificate charset.
    void *pvIssuer[IN]:issuer of certificate
    WE_INT32 iIssuerLen[IN]:issuer length
RETURN VALUE:
    issuer string
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
static WE_CHAR* Sec_UeGetIssuerStr(WE_HANDLE hSecHandle,WE_INT16 sCharSet, 
                                   void *pvIssuer, WE_INT32 iIssuerLen)
{
    WE_UINT8 ucNbrIssuerAtts = 0;
    St_SecNameAttribute *pstIssuerAtt = NULL;
    WE_INT32 iIndex = 0; 
    WE_INT32 iAttLen = 0; 
    WE_INT32 iAttStrPos = 0;
    WE_CHAR *pcAttStr = NULL; 
    WE_CHAR *pcTmpStr = NULL;
    WE_LONG lTmpStrLen = 0;
    WE_INT32 iResult = 0;
    WE_UINT32 uiTmpType = 0;
    
    /* Use parser to get issuer attributes. */
    if (-1 == sCharSet)  /* X.509 certificate */
    {
        iResult = Sec_X509GetNbrIssuerStr(hSecHandle,pvIssuer, (WE_UINT16)iIssuerLen, &ucNbrIssuerAtts);
        pstIssuerAtt = (St_SecNameAttribute *)WE_MALLOC(ucNbrIssuerAtts*sizeof(St_SecNameAttribute));
        if(!pstIssuerAtt)
        {
            return NULL;
        }
        (void)WE_MEMSET(pstIssuerAtt,0,ucNbrIssuerAtts*sizeof(St_SecNameAttribute));
        iResult = Sec_X509GetIssuerInfo(hSecHandle, (const WE_UCHAR *)pvIssuer, (WE_UINT16)iIssuerLen, \
                             &ucNbrIssuerAtts, pstIssuerAtt);
    }
    else /* WTLS certificate */
    {
        iResult = Sec_WtlsGetIssuerInfo(hSecHandle, pvIssuer, (WE_UINT16)iIssuerLen, &ucNbrIssuerAtts, NULL);
        pstIssuerAtt = (St_SecNameAttribute *)WE_MALLOC(ucNbrIssuerAtts*sizeof(St_SecNameAttribute));
        if(!pstIssuerAtt)
        {
            return NULL;
        }
        (void)WE_MEMSET(pstIssuerAtt,0,ucNbrIssuerAtts*sizeof(St_SecNameAttribute));
        iResult = Sec_WtlsGetIssuerInfo(hSecHandle, (const WE_UCHAR *)pvIssuer, (WE_UINT16)iIssuerLen,\
                             &ucNbrIssuerAtts, pstIssuerAtt);
        if (iResult != M_SEC_ERR_OK)
        {
            WE_FREE(pstIssuerAtt);
            return NULL;
        }
        for (iIndex=0; iIndex<ucNbrIssuerAtts; iIndex++)
        {
            pstIssuerAtt[iIndex].sAttributeCharSet = sCharSet;
        }
    }
    /* Get length of string to show */
    iAttLen = 0;
    for (iIndex=0; iIndex<ucNbrIssuerAtts; iIndex++)
    {
        iAttLen += pstIssuerAtt[iIndex].usAttributeValueLen * 2;
    }
    
    /*malloc memory*/  
    pcAttStr  = (WE_CHAR *)WE_MALLOC((WE_UINT32)iAttLen*2 +  (WE_UINT32)ucNbrIssuerAtts + 5);
    if (!pcAttStr )
    {
        for (iIndex = 0; iIndex < ucNbrIssuerAtts; iIndex++)
        {
            WE_FREE(pstIssuerAtt[iIndex].pucAttributeValue);
        }
        WE_FREE(pstIssuerAtt);
        return NULL;
    }
    
    /*GetData*/ 
#ifdef M_SEC_CFG_SHOW_COUNTRY_NAME
    uiTmpType |= M_SEC_CFG_SHOW_COUNTRY_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_ORGANISATION_NAME
    uiTmpType |= M_SEC_CFG_SHOW_ORGANISATION_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_ORGANISATIONAL_UNIT_NAME
    uiTmpType |= M_SEC_CFG_SHOW_ORGANISATIONAL_UNIT_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_STATE_OR_PROVINCE_NAME
    uiTmpType |= M_SEC_CFG_SHOW_STATE_OR_PROVINCE_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_COMMON_NAME
    uiTmpType |= M_SEC_CFG_SHOW_COMMON_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_DOMAIN_COMPONENT
    uiTmpType |= M_SEC_CFG_SHOW_DOMAIN_COMPONENT;
#endif
#ifdef M_SEC_CFG_SHOW_SERIAL_NUMBER
    uiTmpType |= M_SEC_CFG_SHOW_SERIAL_NUMBER;
#endif
#ifdef M_SEC_CFG_SHOW_DN_QUALIFIER
    uiTmpType |= M_SEC_CFG_SHOW_DN_QUALIFIER;
#endif
#ifdef M_SEC_CFG_SHOW_LOCALITY_NAME
    uiTmpType |= M_SEC_CFG_SHOW_LOCALITY_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_TITLE
    uiTmpType |= M_SEC_CFG_SHOW_TITLE;
#endif
#ifdef M_SEC_CFG_SHOW_SURNAME
    uiTmpType |= M_SEC_CFG_SHOW_SURNAME;
#endif
#ifdef M_SEC_CFG_SHOW_GIVENNAME
    uiTmpType |= M_SEC_CFG_SHOW_GIVENNAME;
#endif
#ifdef M_SEC_CFG_SHOW_INITIALS
    uiTmpType |= M_SEC_CFG_SHOW_INITIALS;
#endif
#ifdef M_SEC_CFG_SHOW_GENERATION_QUALIFIER
    uiTmpType |= M_SEC_CFG_SHOW_GENERATION_QUALIFIER;
#endif
#ifdef M_SEC_CFG_SHOW_NAME
    uiTmpType |= M_SEC_CFG_SHOW_NAME;
#endif
#ifdef M_SEC_CFG_SHOW_EMAIL_ADDRESS
    uiTmpType |= M_SEC_CFG_SHOW_EMAIL_ADDRESS;
#endif
#ifdef M_SEC_CFG_SHOW_UNKNOWN_ATTRIBUTE_TYPE
    uiTmpType |= M_SEC_CFG_SHOW_UNKNOWN_ATTRIBUTE_TYPE;
#endif
    pcTmpStr = pcAttStr ;
    iAttStrPos = 0;
    for (iIndex = 0; iIndex < ucNbrIssuerAtts ; iIndex++)
    {
        lTmpStrLen = (pstIssuerAtt[iIndex].usAttributeValueLen * 2) + 2;
        
        if (((pstIssuerAtt[iIndex].uiAttributeType) & uiTmpType) != 0)
        {
            /*TO UTF8*/
            iResult = Sec_UeCsToUtf8(hSecHandle,pstIssuerAtt[iIndex].pucAttributeValue, 
                                    pstIssuerAtt[iIndex].usAttributeValueLen, 
                                    pcTmpStr, 
                                    &lTmpStrLen, 
                                    pstIssuerAtt[iIndex].sAttributeCharSet);
            if (iResult != M_SEC_ERR_OK)
            {
                for (iIndex = 0; iIndex < ucNbrIssuerAtts; iIndex++)
                    WE_FREE(pstIssuerAtt[iIndex].pucAttributeValue);
                WE_FREE(pstIssuerAtt);
                WE_FREE(pcAttStr);
                return NULL;
            }            
            iAttStrPos += lTmpStrLen;
            pcTmpStr += (lTmpStrLen + 1);
            pcAttStr [iAttStrPos++] = ' ';
        }
    } /* for */
    iAttStrPos--;
    pcAttStr [iAttStrPos] = '\0';
    /*free Memory*/
    for (iIndex = 0; iIndex < ucNbrIssuerAtts; iIndex++)
    {
        WE_FREE(pstIssuerAtt[iIndex].pucAttributeValue);
    }
    WE_FREE(pstIssuerAtt);
    
    return pcAttStr ;
}
/*==================================================================================================
FUNCTION: 
    Sec_UeCreateFingerpr
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
   convert fingerprint to hexadecimal
ARGUMENTS PASSED:
    WE_UCHAR *pucFingerPrint[IN]:fingerprint.
    WE_INT32 iFingerPrintLen[IN]:length of fingerprint
RETURN VALUE:
    hexadecimal fingerprint
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_CHAR* Sec_UeCreateFingerpr(WE_UCHAR *pucFingerPrint,WE_INT32 iFingerPrintLen)
{
    WE_CHAR *pcTmpStr = NULL;
    WE_INT32 iIndex = 0;
    WE_UCHAR *pucFP = pucFingerPrint;
    WE_CHAR *pcBuf = NULL;
    
    if((NULL == pucFingerPrint) || (NULL == pucFP))
    {
        return NULL;
    }
    /* Convert fingerprint */
    pcTmpStr = (WE_CHAR *)WE_MALLOC((WE_UINT32)iFingerPrintLen*3+2);
    if(NULL == pcTmpStr)
    {
        return NULL;
    }
    pcBuf = pcTmpStr;
    for (iIndex = 0; iIndex < iFingerPrintLen; iIndex++)
    {
        WE_SCL_BYTE2HEX (*pucFP, pcBuf);
        pucFP++;
        pcBuf += 2;
        *pcBuf = ' ';
        pcBuf++;
    }
    pcBuf--;
    *pcBuf = '\0';
    return pcTmpStr;
}
#endif
/*==================================================================================================
FUNCTION: 
    Sec_UeFreeMem
CREATE DATE:
    2006-07-20
AUTHOR:
    Bird Zhang
DESCRIPTION: 
   free structure according to event type
ARGUMENTS PASSED:
    WE_INT32 eSecEvt,[IN]:event type.
    void* pvData[IN]:buffer for free
RETURN VALUE:
    TRUE:success
    FALSE:fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_VOID Sec_UeFreeMem(WE_INT32 eSecEvt, void* pvData)
{
    if(NULL == pvData)
    {
        return ;
    }
    switch (eSecEvt)
    {
        
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        case M_UE_CONFIRM_EVENT:/*St_Confirm*/
            break;
#endif
#ifdef M_SEC_CFG_WTLS            
        case M_UE_CURSVRCERT:
            {
                St_ShowCertContent *pstShowCert = (St_ShowCertContent*)pvData;
                WE_FREE(pstShowCert->pucSerialNumber);
                WE_FREE(pstShowCert->pucAlg);
                WE_FREE(pstShowCert->pucPubKey);
                WE_FREE(pstShowCert->pcIssuerStr);
                WE_FREE(pstShowCert->pcNotAftStr);
                WE_FREE(pstShowCert->pcNotBefStr);
                WE_FREE(pstShowCert->pcSubjectStr);
            }
            break;

        case M_UE_SHOW_SESSION_CONTENT_EVENT:
            {
                St_ShowSessionContent *pstShowSessionInfo = (St_ShowSessionContent*)pvData;
                WE_FREE(pstShowSessionInfo->pcEncrKeyLenStr);
                WE_FREE(pstShowSessionInfo->pcHmacKeyLenStr);
                WE_FREE(pstShowSessionInfo->pcKeyExchKeyLenStr);
            }
            break;
#endif
            
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
        case M_UE_WARNING_EVENT:/*St_Warning*/
            break;
        case G_Evt_SHOW_DLG:
            {
                St_ShowDlg* pstData = (St_ShowDlg*)pvData;
                M_SEC_SELF_FREE(pstData->pvBuf);
            }
            break;
#endif

        default:
            WE_LOG_MSG((0,0,"SEC:Sec_UeFreeMem, Unknown Event\n"));
            break;                
    }    
    WE_FREE(pvData);
}
/*==================================================================================================
FUNCTION: 
    Sec_RunUeCallBackFun
CREATE DATE:
    2006-12-09
AUTHOR:
    Bird 
DESCRIPTION:
    callback function of ue dialog
ARGUMENTS PASSED:
    WE_VOID* pvData:data to resolve
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

static WE_VOID Sec_RunUeCallBackFun(WE_VOID* hSecHandle)
{
    WE_INT32 iEvent = 0;
    WE_VOID* pvCBData = NULL;
    if( NULL == hSecHandle)
    {
        return;
    }
    iEvent = M_SEC_UECALLBACKEVENT;
    pvCBData = M_SEC_UECALLBACKDATA;
    /*set to null*/
    M_SEC_UECALLBACKDATA = NULL;
    if(NULL != SEC_CBBRS_EVTFUNC)
    {
        ((Fn_ISecEventHandle)(SEC_CBBRS_EVTFUNC))(iEvent,(void *)(pvCBData),SEC_BRS_PRIVDATA);
    }
    Sec_UeFreeMem(iEvent,pvCBData);

    
}
/*==================================================================================================
FUNCTION: 
    Sec_ReleaseUeCB
CREATE DATE:
    2006-12-19
AUTHOR:
    Bird
DESCRIPTION:
    free response callback data when release function is called, if the callback does not excute
ARGUMENTS PASSED:
    WE_VOID* pvData:data to resolve
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

WE_VOID Sec_ReleaseUeCB(WE_VOID* hSecHandle)
{
    WE_INT32 iEvent = 0;
    WE_VOID* pvCBData = NULL;
    if( NULL == hSecHandle)
    {
        return;
    }
    iEvent = M_SEC_UECALLBACKEVENT;
    pvCBData = M_SEC_UECALLBACKDATA;
    Sec_UeFreeMem(iEvent,pvCBData);
}

