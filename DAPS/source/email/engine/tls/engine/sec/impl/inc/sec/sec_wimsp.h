/*=====================================================================================
    FILE NAME : sec_wimsp.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        In this file, define the structure and functions used in the SEC module.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                           Tracking
    Date           Author                 Number       Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-22 Bird                    none           create sec_wimsp.h
    2007-06-19    bluefire                         four bytes align for dragonfly
    
=====================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_WIMSP_H
#define SEC_WIMSP_H

/***************************************************************************************************
*   macro Define Section
***************************************************************************************************/
#define M_FILE_LENGTH_BITS 4

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
typedef struct tagSt_SecSession 
{
    WE_UINT32 uiCreationTime;    
    WE_UCHAR  aucSessionId[M_SEC_SESSION_ID_LEN];     
    WE_UINT8  aucCipherSuite[2];
    WE_UCHAR  aucPrivateKeyId[M_SEC_PRIVATE_KEY_ID_LEN];
    WE_UINT8  ucSessionOptions;
    WE_UINT8  ucSessionIdLen;
    WE_UINT8  ucCompressionAlg;
    WE_UINT8  ucReserved; /* four bytes align */
}St_SecSession;

typedef struct tagSt_SecSessionRec 
{
    St_SecSession stInfo;
    WE_UCHAR      aucMasterSecret[M_SEC_TLS_MASTER_SECRET_LEN];
    WE_UINT8      ucIsActive;
    WE_UINT8      ucSlotInUse;
    WE_UINT8      aucReserved[2]; /* four bytes align */
}St_SecSessionRec;

typedef struct tagSt_SecPeerRec 
{
    WE_INT32  iConnectionType;
    WE_INT32  iAddrLen;
    WE_INT32  iMasterSecretId;
    WE_UCHAR  aucAddress[M_SEC_ADDRESS_LEN];
    WE_UINT16 usPortNum;
    WE_UINT8  ucSlotInUse;
    WE_UINT8  ucReserved; /* four bytes align */
    
}St_SecPeerRec ;


/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
void Sec_WimGetMastSecret(WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UCHAR *pucMasterSecret, 
                          WE_INT32 iMasterSecretLen, WE_INT32 *piRes);

void Sec_WimSaveMastSecret(WE_HANDLE hSecHandle, WE_UCHAR *pucMasterSecret, WE_INT32 iMasterSecretLen, 
                           WE_INT32 *piMasterSecretId, WE_INT32 *piRes);

void Sec_WimClrSessionInfoRec(St_SecSession *pstInfo);

void Sec_WimClrSessionRec(St_SecSessionRec *pstRec);

void Sec_WimClrPeerRec(St_SecPeerRec *pstrec);

void Sec_WimClrSessionInfo(WE_HANDLE hSecHandle,WE_INT32 iIndex);

void Sec_WimClrPeerInfo(WE_HANDLE hSecHandle, WE_INT32 iIndex);

#ifndef M_SEC_CFG_NO_PLATFORM
void Sec_WimSessionStart(WE_HANDLE hSecHandle);

void Sec_WimSessionStop(WE_HANDLE hSecHandle);
#endif
void Sec_WimPeerSearch(WE_HANDLE hSecHandle, WE_INT32 iWid, WE_UINT8 ucConnectionType,
                       const WE_UCHAR *pucAddress, WE_INT32 iAddrLen, WE_UINT16 usPortNum);

void Sec_WimPeerAttachSession(WE_HANDLE hSecHandle,WE_UINT8 uiConnectionType,
                              const WE_UCHAR *pucAddress, WE_INT32 iAddressLen,
                              WE_UINT16 usPortNum, WE_INT32 iMasterSecretId);

void Sec_WimPeerRemove (WE_HANDLE hSecHandle, WE_INT32 iTargetID, WE_INT32 iMasterSecretId);

void Sec_WimSessionEnable(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId, WE_UINT8 ucIsActive);

void Sec_WimSessionDisable(WE_HANDLE hSecHandle,WE_INT32 iMasterSecretId);

void Sec_WimSessionSlotClr(WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId);

void Sec_WimFetchSession(WE_HANDLE hSecHandle, WE_INT32 iWid, WE_INT32 iMasterSecretId);

void Sec_WimUpdateSession(WE_HANDLE hSecHandle, WE_INT32 iMasterSecretId, WE_UINT8 ucSessionOptions,
                          const WE_UCHAR *pucSessionId, WE_UINT8 ucSessionIdLen,
                          WE_UINT8 ucCipherSuite[2], WE_UINT8 ucCompressionAlg,
                          const WE_UCHAR *pucPrivateKeyId,WE_UINT32 uiCreationTime);

#ifndef M_SEC_CFG_NO_PLATFORM 
WE_INT32 Sec_WimGetSessionLength(WE_HANDLE hSecHandle,WE_HANDLE hFileHandle,
                                 WE_UINT32* puiSessionLength, WE_UINT32* uiPeerLength,
                                 WE_UINT32* uiViewLength,WE_LONG *plReadNum);
#endif
#endif 

