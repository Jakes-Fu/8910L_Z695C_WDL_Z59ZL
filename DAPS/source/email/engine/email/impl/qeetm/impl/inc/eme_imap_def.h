/*=====================================================================================
    FILE: eme_imap_def.h
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Defination of IMAP Stack macros and data structures

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================  
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#ifndef EME_IMAP_DEF_PRV_H
#define EME_IMAP_DEF_PRV_H


#include "we_def.h"
#include "eme_error.h"
#include "eme_utils_api.h"
#include "eme_socket_api.h"
#include "eme_imap_api.h"
#include "eme_dataaccess_priv.h"


#define EME_IMAP_STRING_LENGTH 128
#define EME_IMAP_MAX_STRING_LENGTH 10240
#define EME_IMAP_MAX_CMD_LENGTH 128
#define EME_MAILINFO_ENLARGE_LENGTH 30
#define EME_ENLARGE_SIZE 512
#define EME_UIDFILE_ENLARGE_NUMBER 10
#define EME_IMAP_ADDITIONAL_MEMORY 512
#define EME_UIDFILE_FLAG "TFMAIL_UID_FILE\n"
#define EME_MAX(a,b) (a)>(b)?(a):(b)

/* operation type that may use */
#define E_EME_IMAP_OPTYPE_LOGIN                    0x2000
#define E_EME_IMAP_OPTYPE_LOGOUT                   0x2001
#define E_EME_IMAP_OPTYPE_SELECT_MAILBOX           0x2002
#define E_EME_IMAP_OPTYPE_SEARCH_UID               0x2003
#define E_EME_IMAP_OPTYPE_FETCH_SIZE               0x2004
#define E_EME_IMAP_OPTYPE_FETCH_BODYSTRUCTURE      0x2005   
#define E_EME_IMAP_OPTYPE_FETCH_HEADER             0x2006
#define E_EME_IMAP_OPTYPE_FETCH_MAIL               0x2007
#define E_EME_IMAP_OPTYPE_FETCH_SPECIFIC_MAIL      0x2008
#define E_EME_IMAP_OPTYPE_FETCH_MAILPART           0x2009
#define E_EME_IMAP_OPTYPE_CREATE                   0x200A
#define E_EME_IMAP_OPTYPE_RENAME                   0x200B
#define E_EME_IMAP_OPTYPE_DELETE                   0x200C
#define E_EME_IMAP_OPTYPE_SUBSCRIBE                0x200D
#define E_EME_IMAP_OPTYPE_UNSUBSCRIBE              0x200E
#define E_EME_IMAP_OPTYPE_LIST                     0x200F
#define E_EME_IMAP_OPTYPE_LSUB                     0x2011
#define E_EME_IMAP_OPTYPE_EXPUNGMSG_SELECT         0x2012
#define E_EME_IMAP_OPTYPE_EXPUNGMSG                0x2013
#define E_EME_IMAP_OPTYPE_MARK_DELETED             0x2014
#define E_EME_IMAP_OPTYPE_UNMARK_DELETED           0x2015
#define E_EME_IMAP_OPTYPE_MARK_SEEN                0x2016
#define E_EME_IMAP_OPTYPE_UNMARK_SEEN              0x2017
#define E_EME_IMAP_OPTYPE_APPEND                   0x2018
#define E_EME_IMAP_OPTYPE_COPY                     0x2019
#define E_EME_IMAP_OPTYPE_NO_OPERATION             0x201A


#define EME_IMAP_ADDITIONAL_MEMORY 512

#define   EME_IMAP_TEMP_MAIL_FILE_DIR           EMEFS_TEMP_ROOT_DIR /*BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_PATH)*/
#define   EME_IMAP_BUFFER_CATCH_SIZE            (48 * 1024)
#define   EME_IMAP_MAX_FILE_NAME_LEN            (30)

typedef struct
{
   PFNIMAPNOTIFY pfnNotify;
   WE_VOID* pvClientData;
}St_IMAPCallback;

typedef struct
{
   WE_CHAR*  pcUserName;
   WE_CHAR*  pcPassword;
   WE_CHAR*  pcUIDFilePath;
   WE_UINT32 uiSizeLimit; 
   WE_UINT32 uiNumberLimit;
   WE_INT32  iLast;
}St_IMAPAccountInfo;

typedef struct  
{
   WE_VOID* pvMemMgr; /* for memory check */
   WE_VOID* pvFileMgr; /* for file operate  */
   WE_VOID* pvIShell; 
   WE_VOID* pvNetMgr;
   WE_VOID* pvAsyncMgr;
}St_IMAPMgr;

typedef struct  
{
   WE_BOOL  bConnected;
   WE_BOOL  bAuthenticated;
   WE_BOOL  bSelected;
   WE_BOOL  bInfoGeted; /* whether infomation has geted */
}St_IMAPStatus;

typedef struct
{
   WE_UINT32 uiUID;
   WE_UINT32 uiSize;
}St_MailInfo;

typedef struct
{
   WE_UINT32 uiIndex;  /* index of mails's infomation array that have not downloaded */
   WE_UINT32 uiLength; /* array length */
   St_MailInfo* pastMailInfo; /* point to mails's infomation's array */
   WE_UINT32 uiMaxLength; /* length that has allocated memory */
   WE_VOID*  pMemHdr;
}St_MailInfoArray;

typedef struct
{
   WE_UINT32   uiMaxLength;     /* actual allocated memory length, if long string length 
                                   is larger than it, allocate more memory */
   WE_UINT32   uiLength;        /* long string length */
   WE_CHAR*    pcLongString;    /* actual long string data */
   WE_VOID*       pMemHdr;         /* for memory check */
   /*operate by file*/
   WE_VOID     *pvFileMgr;
   WE_CHAR     *pcFileName;
   WE_VOID     *pvTmpFile;
   WE_VOID     *pvCatchBuf;
   WE_UINT32   uiHadUsedSize;
}St_LongString;

/* one item of uid file, it indicate uid of one mailbox in server */
typedef struct
{
   WE_UINT32  uiMaxUID; /* record the newest mail's uid */
   WE_CHAR*   pcMailboxName;  /* indicate a mailbox in server */
   WE_INT32   iBeValide; /* is the mailbox exist on server */
   WE_UINT32  nCurrNum;/*Number of mail that have been received*/
   St_MailInfo* nCurrList; /* array that have been received  */
}St_UIDItem;

/* used to store data read from uid file */
typedef struct
{
   WE_UINT32 uiItemNum; /* number of uid item */
   St_UIDItem **ppstUIDItem; /* list of uid item */
   WE_VOID*  pMemHdr; /* for memory check */
   WE_VOID*  pFileMgr;
}St_UIDBuf;

/* this struct contain infomation that may use in transation */
typedef struct  
{
   /*---------------------------mailbox infomation----------------------------*/
   WE_CHAR* pcMailboxName; /* current selected mailbox name */
   WE_UINT32 uiMaxSavedUID; /* max saved UID of current selected mailbox */
   WE_UINT32 uiMaxDownloadUID; /* max UID which need download of current selected mailbox on server */
   WE_UINT32 uiCurrentUID; /* current UID that used to download new mail */
   WE_UINT32 NumOfMails; /* mails's number of current selected mailbox */
   
   /*-----original server response infomation, after form a whole mail data, 
    -------it will be moved to othre place(pointers below)------------------*/
   St_LongString stBodystructure; /* contain bodystructure's data when getting data from server */
   St_LongString stUIDString; /* contain uid string's data when getting data from server */
   St_LongString stHeader; /* contain header's data when getting data from server */
   WE_VOID* pvMail; /* contain mail's data got from server */
   WE_VOID* pvSpecificMail; /* contain specific mail's data when getting data from server */
   WE_VOID* pvMailPart; /* contain mail part's data when getting data from server */
   
   /*save the mail info to temp file*/
   WE_VOID* pvCatchBuf;
   WE_CHAR* pcFileName;
   WE_VOID* pvTmpFile;
   WE_UINT32 uiUsedBufSize;

   /*---useful infomation got from server response, they are ready for return----*/
   St_MailInfoArray stMailInfoArray; /* contain new mail's infomation, include uid and size */
   St_UIDBuf stUIDBuf; /* contain max saved UID of all mailboxs */
   WE_CHAR* pcBodystructure; /* contain bodystructure data ready for return */
   WE_CHAR* pcHeader; /* contain header data ready for return */
   WE_CHAR* pcMail; /* contain mail data ready for return */
   WE_CHAR* pcSpecificMail; /* contain specific mail data ready for return */
   WE_CHAR* pcMailPart; /* contain mail part data ready for return */
   
   /*---------------------------other infomation-------------------------------*/
   WE_CHAR* pcUIDFilePath;
   WE_VOID* pvFile;
   WE_VOID* pvAppendMailBuffer;
   WE_UINT32 uiSpecificUID; /* the specific mail's UID */
   WE_INT32 eOperationType; /* operation type, for internal use */
   WE_INT32 eTransationType; /* transation type, for record caller's operation */
   WE_UINT32 uiTotalSize; /* size of the mail that will be downloaded or will be sent */
   WE_UINT32 uiCurrentSize; /* transfered size of data now */
   WE_INT32  iSizeLimitExceed;
}St_IMAPInfo;

typedef struct  
{
   HSocket hSocket;
   WE_CHAR acCMD[EME_IMAP_MAX_CMD_LENGTH];
   WE_CHAR acResponse[EME_IMAP_MAX_STRING_LENGTH]; /* contain response data from server */
   WE_CHAR *pcSendMailBuffer;
}St_IMAPSocket;

typedef struct St_IMAPHandle
{
   St_IMAPStatus stIMAPStatus; /* record IMAP's status  */
   St_IMAPInfo stIMAPInfo; /* record untagged response's infomation mainly */
   St_IMAPSocket stIMAPSocket; /* store socket related infomation */
   St_IMAPAccountInfo stIMAPAccountInfo; /* store account infomation that may user later */
   St_IMAPCallback stIMAPCallback; /* store callback of MGR */
   St_IMAPActRet stIMAPActRet; /* store data that may return to MGR */
   St_IMAPMgr stIMAPMgr;
}St_IMAP;

#endif /* EME_IMAP_DEF_PRV_H */