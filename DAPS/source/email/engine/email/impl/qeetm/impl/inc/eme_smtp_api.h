/*=====================================================================================
    FILE: eme_smtp_api.h
    MODULE NAME : SMTP


    GENERAL DESCRIPTION:
    Declaration of Smtp Stack APIs
 
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================     
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/08/2007        EchoChen                         creat this file
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/


/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_SMTP_API_H_
#define _EME_SMTP_API_H_


#include "we_def.h"
/*==================================================================================================
* Macro Define Section
==================================================================================================*/
/*action type*/
#define SMTP_ACTTYPE_OPEN           0x00000001
#define SMTP_ACTTYPE_SENDREADY      0x00000002
#define SMTP_ACTTYPE_SENDING        0x00000003    
#define SMTP_ACTTYPE_SENDOVER       0x00000004   
#define SMTP_ACTTYPE_CLOSE          0x00000005
#define SMTP_ACTTYPE_NOTIFY         0x00000006
#define SMTP_ACTTYPE_FORWARDING     0x00000007
#define SMTP_ACTTYPE_FORWARDOVER    0x00000008
#define SMTP_ACTTYPE_FORWARDREADY   0x00000009  
#define SMTP_ACTTYPE_NONE           0x7fffffff
typedef WE_UINT32 SMTP_ACTION_TYPE;

/*source type*/
#define SMTP_RAMMAIL_SRCTYPE_BUFFER 0x00000001
#define SMTP_RAWMAIL_SRCTYPE_FILE   0x00000002
#define SMTP_RAWMAIL_SRCTYPE_NONE   0x7fffffff
typedef WE_UINT32 SMTP_RAWMAIL_SRCTYPE;
/*==================================================================================================
* Type Define Section
==================================================================================================*/
/**/
typedef struct St_RawMailInfo
{
   WE_UINT32 uiSrcType;
   WE_CHAR *pcUID;            /*The email UID, it's should be provided when forward email from server*/
   WE_CHAR *pcRawMail;        /*The email body, if send new email, it's should be provided, if forward the email, it maybe null*/
   WE_CHAR *pcFrom;
   WE_CHAR *pcTo; 
   WE_CHAR *pcCc;
   WE_CHAR *pcBcc;
   WE_UINT32 uiTranType;
}St_RawMailInfo;

typedef struct St_SMTPConfigInfo
{
   WE_CHAR* pcUserName;
   WE_CHAR* pcPassword;
   WE_CHAR* pcSmtpSerName;
   WE_INT32 iIsNeedSSL;
   WE_INT32 iIsAuth;
   WE_UINT16 usPort;
   WE_CHAR acRes[2];
}St_SMTPConfigInfo;

typedef struct St_DataSent
{
   WE_UINT32 uiSentData;
   WE_UINT32 uiTotalData;
}St_DataSent;

typedef struct St_SMTPActRet
{
   SMTP_ACTION_TYPE uiActType;
   WE_INT32 iRet;
   WE_CHAR  acRespDesc[512];
   St_DataSent stDataSent;     /*only for sending action*/
}St_SMTPActRet;

typedef WE_VOID (* ActionCB)(WE_VOID*, St_SMTPActRet*);
typedef WE_VOID* ISMTPHandle;

/***************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/

WE_INT32 EME_SMTP_InitHandle
(
 ISMTPHandle* ppHandle,
 WE_VOID* pvMemMgr,
 WE_VOID* pvIShell,
 WE_VOID* pvAsyncMgr,
 WE_VOID* pvFileMgr,
 WE_VOID* pvNetMgr
 );

WE_INT32 EME_SMTP_SetCallBack(ISMTPHandle pHandle,ActionCB pActCB,void* pvPrivData);

WE_INT32 EME_SMTP_Open(ISMTPHandle pHandle,St_SMTPConfigInfo* pstConfigInfo);

WE_INT32 EME_SMTP_Send(ISMTPHandle pHandle,St_RawMailInfo* pstRawInfo);

WE_INT32 EME_SMTP_Forward(ISMTPHandle pHandle,St_RawMailInfo* pstRawInfo);

WE_INT32 EME_SMTP_Close(ISMTPHandle pHandle);

WE_INT32 EME_SMTP_Cancel(ISMTPHandle pHandle);
WE_INT32 EME_SMTP_DeleteHandle(ISMTPHandle *pHandle);

/*=============================Description of interface===========================================*/

/* Description for Macros*/


/* Comment for Interfaces */

#endif  /*End define _EME_SMTP_API_H_*/
