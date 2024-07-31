/*=====================================================================================
    FILE: eme_smtp_priv.h
    MODULE NAME : SMTP

  
    GENERAL DESCRIPTION:
    Declaration of Smtp Stack Intern Fucntions
 
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
#ifndef EME_SMTP_PRIV_H
#define EME_SMTP_PRIV_H


/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/

/*Include Program Global head file*/
/*Include module head file*/
#include "eme_file_api.h"
#include "eme_smtp_api.h"
#include "eme_socket_api.h"

/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
#define EM_RESPLEN 512
#define EM_MAXSTRLEN    512

/*cmd response*/
#define SERVER_READY "220"
#define EHLO_DOING "250-"
#define EHLO_DONE "250 "
#define CMD_DONE "250"
#define USER_NOT_LOCAL "251"
#define CMD_UNDONE "502"
#define AUTH_DOING "334"
#define AUTH_DONE "235"
#define SERVER_CLOSE "221"
#define DATA_READY "354"
#define REQUEST_ABORT "552"
#define SERVER_NOT_AVAILABLE "421"

#define COMMA_MARK ','
#define END_STR '\0'
/*cmd str*/
#define COLON_MARK ':'
#define SP_MARK ' '
#define LEFT_BRACKET '<'
#define RIGHT_BRACKET '>'
#define END_MARK '.'
#define CMD_END "\r\n"
#define EHLO  "ehlo"
#define HELO  "helo"
#define AUTH_LOGIN "auth login"
#define MAIL_FROM "mail from"
#define RCPT_TO "rcpt to"
#define DATA "data"
#define EFWD "efwd"
#define RSET "rset"
#define QUIT "quit"
#define STARTTLS "STARTTLS"

#define TFMAIL " tfmail"
/**/
#define ENDOFMAIL_TAG "\r\n.\r\n"
#define ENDOFMAIL_TAG_SIZE 5

#define ENDOFFWD_TAG ".\r\n"
#define ENDOFFWD_TAG_SIZE 3

#define SEND_BUFFER_SIZE EME_MAX_DATAWRITE_LEN

/*error msg to compare*/

/*Define Constant Macro end*/


/*Define Macro Function start*/
/*Define Macro Function end*/

/*Define ENUM type start*/
/*Define ENUM type end*/
/*---------------------------------------------------------------------
 *  Here you may want to include extra documentation on data structures
 * ------------------------------------------------------------------*/
/*Define base type start*/

/*Define base type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
typedef struct St_SMTPHandle
{
   St_SMTPConfigInfo stConfigInfo;
   St_RawMailInfo stRawMailInfo;
   WE_CHAR* pcToStrPos;  /* the pos in to list */
   WE_CHAR acToStr[EM_MAXSTRLEN];
   WE_CHAR acResponse[EM_RESPLEN];
   WE_CHAR acCMD[EM_MAXSTRLEN] ;
   WE_UINT32 uiTotalSize;
   WE_UINT32 uiSentSize;
   WE_VOID* pvSendFile;  /*file handle*/
   WE_CHAR* pcSendBuf;
   SMTP_ACTION_TYPE uiActType; 
   ActionCB pCB;     /*action callback function*/
   WE_VOID* pCBCx;      /*callback context*/
   WE_VOID* pvMemMgr; /*memory check handle*/ 
   WE_VOID* pvFileMgr;
   WE_VOID* pvIShell;
   WE_VOID* pvAsyncMgr;
   WE_VOID* pvNetMgr;
   HSocket   pstSock;    /*socket relate*/
   WE_BOOL bCc;//
   WE_BOOL bBcc;
   WE_BOOL bAuth;
   WE_CHAR acRes[1];
}St_SMTPHandle;

/*Define struct &  union type end*/

/*---------------------------------------------------------------------
 *             Function Declarations
 * ------------------------------------------------------------------*/
WE_VOID EME_SMTP_SockErrorHandle(ISMTPHandle pHandle,WE_INT32 iErrcode);
WE_VOID EME_SMTP_connectEstablish(ISMTPHandle pHandle);
WE_VOID EME_SMTP_ConnectSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_EhloSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_HeloSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_AuthSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_AuthNameSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_AuthPwdSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_MailFromSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_RcptToSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_DataSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_SendDataSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_QuitSocket(ISMTPHandle pHandle);
WE_VOID EME_SMTP_SendDataToSer(ISMTPHandle pHandle);

WE_INT32 EME_SMTP_Ehlo(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_Helo(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_Auth(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_AuthNameOrPwd(ISMTPHandle pHandle,void* pvSockBack,char* pcStr);
WE_INT32 EME_SMTP_MailFrom(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_RcptTo(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_Data(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_Efwd(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_SendData(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_Quit(ISMTPHandle pHandle,void* pvSockBack);
WE_INT32 EME_SMTP_NotifyToMGR(ISMTPHandle pHandle,WE_UINT32 uiActType,WE_INT32 iRet);
WE_BOOL EME_SMTP_GetToStr(ISMTPHandle pHandle,WE_CHAR* pcOutStr);
WE_VOID EME_SMTP_StartTlsSocket(ISMTPHandle pHandle);
WE_INT32 EME_SMTP_StartTls(ISMTPHandle pHandle,void* pvSockBack);
/*---------------------------------------------------------------------
 *             Function Documentation
 * --------------------------------------------------------------------*/
/* Remember header files should contain all the information a user
 * would need to use this service */

#endif             /* #ifndef QCT_H */
