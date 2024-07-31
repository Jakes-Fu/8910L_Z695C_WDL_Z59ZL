/*=====================================================================================
FILE NAME   : eme_tm_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for managing transaction.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_tm_api.h
          
=====================================================================================*/

#ifndef _EME_TM_API_H
#define _EME_TM_API_H

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#include "we_def.h"

/*==================================================================================================
 * Macro Define Section
==================================================================================================*/
#define EVT_EMAIL (0x9000)

/*========================================Email option start==========================================*/
#define EME_OPTID_END                            0x00
#define EME_OPTID_INT_START                      0x01000000
#define EME_OPTID_STR_START                      0x02000000
#define EME_OPTID_TRANS_PARAM                    0x00001000
#define EME_OPTID_ACCOUNT_INFO                   0x00002000

/************** transaction inform ******************/
/*int*/
#define EME_OPTID_TRANS_TYPE                     (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x01)     /* value type: int32 */
#define EME_OPTID_REPLY_CLSID                    (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x02)
#define EME_OPTID_REPLY_USERDATA                 (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x03)     /* value type: int32 */
#define EME_OPTID_EMN_BODYDATALEN                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x04) 
#define EME_OPTID_EMN_CONTENTTYPE                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x05) 
#define EME_OPTID_EMN_CHARSETTYPE                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x06)
#define EME_OPTID_CHECK_PASSWORD                 (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x07)
#define EME_OPTID_EPOP_SET_OBJ                   (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x08)
#define EME_OPTID_CONN_NETID                     (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x09)

/*str*/
#define EME_OPTID_PART_ID                        (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x01)     /* value type: char* , string Length less thans 10 */
#define EME_OPTID_MESSAGE_ID                     (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x02)     /* value type: char*                    */
#define EME_OPTID_LOGIN_MAILBOXNAME              (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x03)     /* value type: char*                    */
#define EME_OPTID_HANDLE_MAILBOXNAME             (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x04)
#define EME_OPTID_EMN_BODYDATA                   (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x05)
#define EME_OPTID_MESSAGE_UID                    (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x06)
#define EME_OPTID_EPOP_SET_STRING                (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x07)   /* value type: char*          */ 
#define EME_OPTID_EPOP_SET_FILTER                (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x08)   /* value type: char*          */ 

/************** account inform ***************/
/*int*/
#define EME_OPTID_ACCOUNT_ID                     (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x01)   /* value type: uint32         */    
#define EME_OPTID_DEFAULT_FOLDER_ID              (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x02)   /* value type: uint32         */
#define EME_OPTID_POP_KEEP_MAIL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x03)   /* value type: int32          */
#define EME_OPTID_SMTP_NEED_SSL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x04)   /* value type: int32         */  
#define EME_OPTID_POP_NEED_SSL                   (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x05)   /* value type: int32         */   
#define EME_OPTID_IMAP_NEED_SSL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x06)   /* value type: int32         */    
#define EME_OPTID_SIZE_LIMIT                     (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x07)   /* value type: uint32         */
#define EME_OPTID_NUM_LIMIT                      (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x08)   /* value type: uint32         */
#define EME_OPTID_RECEIVE_SERVER_PORT            (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x09)   /* value type: uint32         */
#define EME_OPTID_SEND_SERVER_PORT               (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0A)   /* value type: uint32         */
#define EME_OPTID_NETWORK_UMTS_PDP               (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0B)
#define EME_OPTID_SMTP_AUTHENTICATE              (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0C)
#define EME_OPTID_RECEIVE_LATEST                 (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0D)   /* value type: uint32         */


/*str*/
#define EME_OPTID_RECEIVE_SERVER_ADDR            (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x01)   /* value type: char*          */
#define EME_OPTID_RECEIVE_USERNAME               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x02)   /* value type: char*          */
#define EME_OPTID_RECEIVE_PASSWORD               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x03)   /* value type: char*          */  
#define EME_OPTID_SEND_SERVER_ADDR               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x04)   /* value type: char*          */   
#define EME_OPTID_SEND_EMAIL_ADDR                (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x05)   /* value type: char*          */  
#define EME_OPTID_SEND_USERNAME                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x06)   /* value type: char*          */
#define EME_OPTID_SEND_PASSWORD                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x07)   /* value type: char*          */  
#define EME_OPTID_UIDL_FILE_POP                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x08)   /* value type: char*          */  
#define EME_OPTID_UID_FILE_IMAP                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x09)   /* value type: char*          */ 

#define EME_OPTID_EPOP_CONVERTCHARSET            (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0A)   /* value type: char*          */ 
#define EME_OPTID_EPOP_USECOMPRESS               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0B)   /* value type: char*          */ 
//#define EME_OPTID_EPOP_CONTENTID                 (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0C)   /* value type: char*          */ 
#define EME_OPTID_EPOP_CONVERTTYPE               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0D)   /* value type: char*          */ 

/*=========================================Email option end==============================================*/
/*************** TRANS TYPE *********************/
/* SMTP */
#define EME_TRANS_SMTP_SEND_MAIL                         0x01
#define EME_TRANS_SMTP_SEND_RECEIPT                      0x02
#define EME_TRANS_SMTP_FORWARD_WHOLEMAIL                 0x03
#define EME_TRANS_SMTP_FORWARD_ATTACHMENT                0x04
   
/* POP3 */
#define EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER            0x11
#define EME_TRANS_POP3_FETCH_NEW_MAILS                   0x12
#define EME_TRANS_POP3_FETCH_SPEC_MAIL                   0x13
#define EME_TRANS_POP3_DELETE_SPEC_MAIL                  0x14
#define EME_TRANS_POP3_DELETE_ALL_MAIL                   0x15

/*EPOP*/
#define EME_TRANS_EPOP_FETCH_NEW_MAILS_HEADER            0x16
#define EME_TRANS_EPOP_FETCH_NEW_MAILS                   0x17
#define EME_TRANS_EPOP_FETCH_SPEC_MAIL                   0x18
#define EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART              0x19
#define EME_TRANS_EPOP_SET_PUSH_MAIL    	             0x1a
#define EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER              0x1b
#define EME_TRANS_CHECK_PASSWORD			             0x20

/* IMAP4 */
#define EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER           0x21
#define EME_TRANS_IMAP4_FETCH_NEW_MAILS                  0x22
#define EME_TRANS_IMAP4_FETCH_SPEC_MAIL                  0x23
#define EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART             0x24
#define EME_TRANS_IMAP4_MARK_DEL_ON_SERVER               0x25
#define EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER             0x26
#define EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER              0x27
#define EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER            0x28
#define EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX             0x29
#define EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER            0x2A
#define EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX      0x2B
#define EME_TRANS_IMAP4_CREAT_MAILBOX                    0x2C
#define EME_TRANS_IMAP4_RENAME_MAILBOX                   0x2D
#define EME_TRANS_IMAP4_DELETE_MAILBOX                   0x2E
#define EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX                0x2F
#define EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX              0x30
#define EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES        0x31
#define EME_TRANS_IMAP4_LIST_ALL_MAILBOXES               0x32
 
/*************** Trans Status TYPE *********************/
#define EME_TRANS_STATUS_PENDING         1     //Transaction was attempted to complete by TM but could not complete because of some reasons.              
#define EME_TRANS_STATUS_CANCELED        2
#define EME_TRANS_STATUS_DONE            3
#define EME_TRANS_STATUS_INPROGRESS      4     //Transaction is in progress
#define EME_TRANS_STATUS_MAX             0x7FFFFFFF

/*************** Trans Id *********************/
#define EME_ALL_TRANS 0xffffffff
#define EME_PROGRESS_INDI_MASK 0x80000001

/*************** EMN Type8*********************/
#define EME_CONTENTTEXT_EMN           0x01
#define EME_CONTENTBINARY_EMN         0x02

/*************** EMN Body charset *************/
#define EME_CHARSETUTF8_EMN           106
#define EME_CHARSETUTF16_EMN          1015

/*************** EMN URI Type *****************/
#define EME_URI_MAILAT                 0x01
#define EME_URI_POP                    0x02
#define EME_URI_IMAP                   0x03
#define EME_URI_OTHER                  0x04

/*************** EMN authentication Type *****************/
#define EME_AUTHTYPE_ANY            0x0100
#define EME_AUTHTYPE_POP_PLAIN      0x0101
#define EME_AUTHTYPE_POP_APOP       0x0102


/*********************NOTIFY TYPE ******************/
#define EVT_EME_TRANS_RESULT             (EVT_EMAIL + 0x01) /*EMETransResult*/

#define EVT_EME_NEW_MESSAGE              (EVT_EMAIL + 0x02) /*EMEMsg*/
#define EVT_EME_MAILBOX_GETED            (EVT_EMAIL + 0x03) /*EMEMailbox*/
#define EVT_EME_MESSAGE_SIZE_NOTIFY      (EVT_EMAIL + 0x04) /*EMESizeNotify*/
#define EVT_EME_MESSAGE_NUMBER_NOTIFY    (EVT_EMAIL + 0x05) /*EMENumberNotify*/
#define EVT_EME_INFO_NOTIFY              (EVT_EMAIL + 0x06) /*ascii-string*/      

/* any transaction will receive this */

/*==================================================================================================
* Type Define Section
==================================================================================================*/
typedef WE_UINT32 TransID;


typedef struct _EMEOpt
{
   WE_INT32    iId;
   WE_UINT32  type;
   WE_UINT32 size;
   WE_VOID     *pVal;
}EMEOpt;


typedef struct
{
   WE_UINT32   uiTransId;
   WE_UINT32   uiTransType;
   WE_UINT32   uiMsgId;
   WE_INT32    iResult;
   WE_CHAR     acRespDesc[512];
}St_EMETransResult;


typedef struct  
{
   WE_UINT32   uiTransId;
   WE_UINT32   uiTransType;
   WE_CHAR     *pcUidl;
   WE_CHAR     *pcMsgId;
}St_EMEMsg;

typedef struct  
{
   TransID     uiTransId; 
   WE_UINT32   uiTransType;
   WE_CHAR     *pcMailboxName;
   WE_CHAR     *pcDelimiter;
}St_EMEMailbox;

typedef struct
{
   TransID    uiTransId;
   WE_UINT32  uiTransType;
   WE_UINT32  uiTransferedSize;
   WE_UINT32  uiTotalSize;
}St_EMESizeNotify;

typedef struct
{
   TransID    uiTransId;
   WE_UINT32   uiTransType;
   WE_UINT32  uiCurrMsg;
   WE_UINT32  uiTotalMsg;
}St_EMENumNotify;

typedef struct
{
   WE_INT32       iAuthType;
   WE_CHAR*       pcUser;
   WE_CHAR*       pcHost;
   WE_UINT16      usPort;
   WE_CHAR        acRes[2];
}St_EMEServer;


typedef struct
{
   WE_INT32       iURIType;
   WE_UINT32      uiUTCTime;
   WE_CHAR*       pcMailboxName;
   St_EMEServer   stIServer;
}St_EMEEmnRet;


/***************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
typedef WE_VOID* ITMHandle; 


WE_INT32 EMETM_InitHandle(ITMHandle *phTMHandle, WE_VOID* pMemHdr, WE_VOID *pvParent);
WE_INT32 EMETM_DeleteHandle(ITMHandle *phTMHandle);

WE_INT32 EMETM_CreateSession(ITMHandle hTMHandle, EMEOpt *pstAccountOpts);
WE_INT32 EMETM_CloseSession(ITMHandle hTMHandle);

WE_INT32 EMETM_EnqueueTrans(ITMHandle hTMHandle, 
                          EMEOpt* pstTransOpts,
                          TransID *puiTransId);
WE_INT32 EMETM_DequeueTrans(ITMHandle hTMHandle, TransID uiTransId);

WE_INT32 EMETM_HandleTransactions(ITMHandle hTMHandle);

WE_INT32 EMETM_SignalProcess(ITMHandle hTMHandle);

WE_INT32 EMETM_DecodeEMN(ITMHandle hTMHandle, EMEOpt *pOpts, St_EMEEmnRet** ppRet);
WE_INT32 EMETM_GetTransNumber(ITMHandle hTMHandle,
                             TransID   transID,
                             unsigned int *puiNum);

typedef void (*PFNEMETMNOTIFY)(WE_UINT32 uiNotifyType, WE_VOID *pvResult, WE_VOID *pvUserData);

/*=============================Description of interface===========================================*/

/* Description for Macros*/


/* Comment for Interfaces */

/*=============================================================================
Function: EMETM_InitHandle

Description:

    Initializing the inner structure for a MMS

Prototype:

    int EMETM_InitHandle(ITMHandle *phTMHandle)
                                         
Parameters:
  
    phTMHandle   [out]: The pointer to the MMSETM handle. 
    
Return Value:

    General Error codes
  
Comments:

    Attention Please, this interface must be invoked first before using other interface, and Must call
    EMETM_DeleteHandle() at the end of usage.

See Also:

=============================================================================*/
/*=============================================================================
Function: EMETM_DeleteHandle

Description:

    Delete the MMSETM handle.

Prototype:

   int EMETM_DeleteHandle(ITMHandle *phTMHandle)
                                         
Parameters:
  
    phTMHandle: 
    
Return Value:

    General Error codes
  
Comments:

    

See Also:

=============================================================================*/
/*=============================================================================
Function: EMETM_EnqueueTrans

Description:

    This API creates new entry for transaction queue and enqueues new transaction in it. 
    Once the transaction is successfully queued into transaction queue, the transaction will be 
    in TM_TRANS_PENDING state and transaction status are returned to caller. Caller can keep 
    track of his different requests by getting transaction queue using EMETM_GetTransQueue(). 

Prototype:

   int EMETM_EnqueueTrans(ITMHandle hTMHandle, St_TransDetails *pstTransDetails, St_TransStatus *pstTransStatus)
                                         
Parameters:
  
    hTMHandle        [in]:  The pointer to the MMSTM handle.
    pstTransDetails  [in]:  The pointer to the structure of the transaction details.
    pstTransStatus: [out]:  The pointer to the structure of the transaction status. 
    
Return Value:

    General Error codes
  
Comments:

    

See Also:

=============================================================================*/
/*=============================================================================
Function: EMETM_DequeueTrans

Description:

    Cancels the already enqueued transaction. If value of transID equals EMETM_TRANSSTATETYPE_MAX, then 
    all the transactions associated are canceled.

Prototype:

   int EMETM_DequeueTrans(ITMHandle hTMHandle, St_TransStatus *pstTransStatus)
                                         
Parameters:
  
    hTMHandle           [in]: The pointer to the MMSETM handle.
    pstTransStatus      [in]: The pointer to the structure of transaction details.

Return Value:

    General Error codes
  
Comments:

    

See Also:

=============================================================================*/
/*=============================================================================
Function: EMETM_GetTransQueue

Description:

    Gets the list of available currently transactions in TM's queue. 
    The details for each transaction are given using St_TransStatus structure array,
    and output the array number.

Prototype:

   int EMETM_GetTransQueue(ITMHandle hTMHandle, St_TransStatus **ppstTransStatus, unsigned int *puiNum)
                                         
Parameters:
  
    hTMHandle               [in]: The pointer to the MMSTM handle.
    ppstTransStatus          [out]: The structure array of each status of transactions.  
    puiNum                 [out]: The number of the structure in the output structure array.
    
Return Value:

    General Error codes
  
Comments:

    Attention Please, this ppstTransStatus must be free after useage.

See Also:

=============================================================================*/

#endif  /*End define _MMSE_TM_API_H_*/
