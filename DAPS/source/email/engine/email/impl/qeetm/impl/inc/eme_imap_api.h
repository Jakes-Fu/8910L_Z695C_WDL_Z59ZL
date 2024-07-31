/*=====================================================================================
    FILE: eme_imap_api.h
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Defination of IMAP Stack APIs

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================  
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/

#ifndef EME_IMAP_API_H
#define EME_IMAP_API_H


#include "we_def.h"
#include "eme_error.h"

#define EME_IMAP_SERVER_ADDR                       0x1000
#define EME_IMAP_SERVER_PORT                       0x1001
#define EME_IMAP_USERNAME                          0x1002
#define EME_IMAP_PASSWORD                          0x1003
#define EME_IMAP_UID_FILE_PATH                     0x1004
#define EME_IMAP_SIZE_LIMIT                        0x1005
#define EME_IMAP_IS_NEED_SSL                       0x1006
#define EME_IMAP_NUMBER_LIMIT                      0x1007
#define EME_IMAP_MAILBOX_NAME                      0x1008
#define EME_IMAP_MAILBOX_RENAME                    0x1009
#define EME_IMAP_MAIL_PATH                         0x100A
#define EME_IMAP_MAIL_BUFFER                       0x100B
#define EME_IMAP_MAIL_BUFFER_SIZE                  0x100C
#define EME_IMAP_UID                               0x100D
#define EME_IMAP_HANDLE_MAIL_TYPE                  0x100E
#define EME_IMAP_HANDLE_MAILBOX_TYPE               0x100F
#define EME_IMAP_OPTEND                            0x1010
#define EME_IMAP_RECEIVE_LAST                      0x1011

typedef enum
{
   E_EME_IMAP_MARK_DELETED         = 0x1200,
   E_EME_IMAP_UNMARK_DELETED       = 0x1201,
   E_EME_IMAP_MARK_SEEN            = 0x1202,
   E_EME_IMAP_UNMARK_SEEN          = 0x1203,
   E_EME_IMAP_APPEND               = 0x1204,
   E_EME_IMAP_COPY                 = 0x1205,
   E_EME_IMAP_HANDLE_MAIL_RESERVE  = 0x7FFFFFFF
}E_IMAPHandleMail;

typedef enum
{
   E_EME_IMAP_CREATE                  =  0x1300,
   E_EME_IMAP_RENAME                  =  0x1301,
   E_EME_IMAP_DELETE                  =  0x1302,
   E_EME_IMAP_SUBSCRIBE               =  0x1303,
   E_EME_IMAP_UNSUBSCRIBE             =  0x1304,
   E_EME_IMAP_LIST_MAILBOXS           =  0x1305,
   E_EME_IMAP_LIST_SUBSCRIBE_MAILBOXS =  0x1306,
   E_EME_IMAP_EXPUNGMSG               =  0X1307,
   E_EME_IMAP_HANDLE_MAILBOX_RESERVE  =  0x7FFFFFFF
}E_IMAPHandleMailBox;

typedef enum
{
   E_EME_IMAP_OPEN_IMAP_END                             =  0x1400,
   E_EME_IMAP_CLOSE_IMAP_END                            =  0x1401,
   E_EME_IMAP_SELECT_MAILBOX_END                        =  0x1402,
   E_EME_IMAP_GET_INFOMATION_OF_NEW_MAILS_END           =  0x1403,
   E_EME_IMAP_FETCH_HEADER_END                          =  0x1404,
   E_EME_IMAP_FETCH_MAIL_END                            =  0x1405,
   E_EME_IMAP_FETCH_SPECIFIC_MAIL_END                   =  0x1406,
   E_EME_IMAP_FETCH_SPECIFIC_PART_END                   =  0x1407,
   E_EME_IMAP_HANDLE_MAIL_END                           =  0x1408,
   E_EME_IMAP_HANDLE_MAILBOX_END                        =  0x1409,
   
   E_EME_IMAP_TRANSFER_SIZE_NOTIFY                      =  0x140A,
   E_EME_IMAP_NUMBER_NOTIFY                             =  0x140B,
   E_EME_IMAP_MAILBOX_NOTIFY                            =  0x140C,

   E_EME_IMAP_EVT_CODE_RESERVE                          =  0x7FFFFFFF
}E_IMAPEvtCode;

typedef WE_VOID* IMAPHandle;

typedef struct
{
   WE_INT32        iId;            
   WE_VOID*        pvVal;
}St_IMAPOpt;

typedef struct  
{ 
   WE_CHAR*   pcMailboxName;
   WE_CHAR    acDelimiter[4];
   WE_BOOL    bSelectable;
   WE_CHAR    acRes[3];
}St_IMAPMailboxGeted;

typedef struct
{
   WE_UINT32 uiTransferedSize;
   WE_UINT32 uiTotalSize;
}St_IMAPTransferNotify;


typedef struct  
{
   WE_CHAR* pcCMD;
   WE_CHAR* pcResponse;
}St_IMAPServerResponse;

typedef struct
{
   WE_VOID* pvHeader;
   WE_VOID* pvBodystructure;
   WE_UINT32 uiUID;
   WE_UINT32 uiMailSize;
}St_IMAPHeader;

typedef struct  
{
   WE_VOID* pvWholeMail; 
   WE_UINT32 uiUID;
   WE_UINT32 uiMailSize;
}St_IMAPMail;

typedef struct
{
   WE_UINT32 uiCurrentNumber;
   WE_UINT32 uiTotalNumber;
}St_IMAPNumberNotify;

typedef struct
{
   WE_VOID* pvMailPart; /* used for EME_IMAP_FetchSpecificPart */
   WE_UINT32 uiUID;
   WE_UINT32 uiPartSize;
}St_IMAPMailPart;

typedef struct  
{
   WE_INT32        iResult;
   E_IMAPEvtCode   eEvtCode;
   WE_CHAR         acRespDesc[512];
   union
   {
      St_IMAPNumberNotify stNumberNotify; /* user for EME_IMAP_FetchMail and EME_IMAP_FetchHeader */
      St_IMAPTransferNotify stTransferNotify; /* used for EME_IMAP_FetchMail, EME_IMAP_FetchSpecificMail, 
                                                  EME_IMAP_FetchSpecificPart or EME_IMAP_HandleMail when it's 
                                                  operation is EME_IMAP_APPEND */
      St_IMAPMailboxGeted stMailboxInfo; /* used for EME_IMAP_HandleMailbox when operation type 
                                             is EME_IMAP_LIST_MAILBOXS or EME_IMAP_LIST_SUBSCRIBE_MAILBOXS*/
      St_IMAPServerResponse stServerResponse; /* used whenever server response error */
      St_IMAPHeader stHeader; /* used for EME_IMAP_FetchHeader */
      St_IMAPMail   stMail; /* used for EME_IMAP_FetchMail or EME_IMAP_FetchSpecificMail */
      St_IMAPMailPart stMailPart; /* used for EME_IMAP_FetchSpecificPart */
   }unRetData;
}St_IMAPActRet;

typedef WE_VOID (* PFNIMAPNOTIFY)(WE_VOID* pvClientData, St_IMAPActRet* stIMAPActRet);

WE_INT32 EME_IMAP_InitHandle(IMAPHandle* phIMAPHandle, 
                             WE_VOID* pvMemMgr, 
                             WE_VOID* pvIShell,
                             WE_VOID* pvAsyncMgr,
                             WE_VOID* pvFileMgr, 
                             WE_VOID* pvINetMgr);

WE_INT32 EME_IMAP_SetCallback(IMAPHandle hIMAPHandle, PFNIMAPNOTIFY pfnNotify, WE_VOID* pvClientData);

/* pOpts: EME_IMAP_SERVER_ADDR, EME_IMAP_SERVER_PORT, EME_IMAP_USERNAME, EME_IMAP_PASSWORD,
          EME_IMAP_UIDFILE */
WE_INT32 EME_IMAP_OpenIMAP(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts);

WE_INT32 EME_IMAP_CloseIMAP(IMAPHandle phIMAPHandle);

WE_INT32 EME_IMAP_SelectMailbox(IMAPHandle hIMAPHandle, WE_CHAR* pcMailboxName);

WE_INT32 EME_IMAP_GetInfoOfNewMails(IMAPHandle hIMAPHandle);

/* fetch one header of new mails (include bodystructure). */
WE_INT32 EME_IMAP_FetchHeader(IMAPHandle hIMAPHandle);

/* fetch one whole mail of new mails. */
WE_INT32 EME_IMAP_FetchMail(IMAPHandle hIMAPHandle);

WE_INT32 EME_IMAP_SaveUID(IMAPHandle hIMAPHandle);

WE_INT32 EME_IMAP_FetchSpecificMail(IMAPHandle hIMAPHandle, WE_UINT32 uiUID, WE_UINT32 uiSize);

WE_INT32 EME_IMAP_FetchSpecificPart(IMAPHandle hIMAPHandle, WE_UINT32 uiUID, WE_CHAR* pcSectionID, WE_UINT32 uiSize);

/* pOpts: EME_IMAP_HANDLE_MAIL_TYPE 
   when value of EME_IMAP_HANDLE_MAIL_TYPE is EME_IMAP_MARK_DELETED, EME_IMAP_UNMARK_DELETED, 
   EME_IMAP_MARK_SEEN, EME_IMAP_UNMARK_SEEN, pOpts will also need option EME_IMAP_UID.
   when value of EME_IMAP_HANDLE_MAIL_TYPE is EME_IMAP_APPEND, EME_IMAP_COPY, pOpts will alse need
   EME_IMAP_MAILBOX_NAME */
WE_INT32 EME_IMAP_HandleMail(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts);

/* pOpts: EME_IMAP_HANDLE_MAILBOX_TYPE 
   when value of EME_IMAP_HANDLE_MAILBOX_TYPE is EME_IMAP_CREATE, EME_IMAP_RENAME, EME_IMAP_DELETE,
   EME_IMAP_SUBSCRIBE, EME_IMAP_UNSUBSCRIBE, EME_IMAP_EXPUNGMSG, pOpts will alse need EME_IMAP_MAILBOX_NAME */
WE_INT32 EME_IMAP_HandleMailbox(IMAPHandle hIMAPHandle, St_IMAPOpt* pOpts);

WE_INT32 EME_IMAP_Cancel(IMAPHandle hIMAPHandle);

WE_INT32 EME_IMAP_DeleteHandle(IMAPHandle* phIMAPHandle);


#endif /* EME_IMAP_API_H */
