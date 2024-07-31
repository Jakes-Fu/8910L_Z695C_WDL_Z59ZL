/*==============================================================================
    FILE NAME   : EMS_Store_Priv.h
    MODULE NAME : AppStore


    GENERAL DESCRIPTION:
        

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-16   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMS_STORE_PRIV_H
#define EMS_STORE_PRIV_H

/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "EMS_Store_API.h"
#include "eme_dataaccess_api.h"
#include "eme_dataaccess_priv.h"
#include "eme_msgindex_api.h"
#include "eme_msgstore_api.h"
#include "eme_callback.h"
#include "eme_actindex_api.h"
#include "eme_boxindex_api.h"
#include "eme_serverconf_api.h"

#ifdef __cplusplus
extern "C"{
#endif

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
#define EMS_LEAST_EML_SPACE                     0x00100000
#define EMS_LEAST_BOX_SPACE                     0x00005000
#define EMS_LEAST_ACT_SPACE                     0x0001e000

#define ACCOUNT_USERINFO_NAME_LENGTH            255
#define ACCOUNT_USERINFO_EMAIL_LENGTH           255
#define ACCOUNT_USERINFO_REPLY2_EMAIL_LENGTH    255
#define ACCOUNT_USERINFO_USER_ID_LENGTH         255
#define ACCOUNT_USERINFO_PWD_LENGTH             255
#define ACCOUNT_USERINFO_SMTP_ID_LENGTH         255
#define ACCOUNT_USERINFO_SMTP_PWD_LENGTH        255
#define ACCOUNT_PROTECT_PASSWORLD_MAX_LEN      255

#define POP3_SERVER_LENGTH                      255
#define POP3_SERVER_PORT_LENGTH                 5
#define IMAP4_SERVER_LENGTH                     255
#define IMAP4_SERVER_PORT_LENGTH                5
#define SMTP_SERVER_LENGTH                      255
#define SMTP_SERVER_PORT_LENGTH                 5

#define SENDER_NAME_LENGTH                      255

#define ACT_MSGINFO_SIZE_LIMIT_LEN              10
#define ACT_MSGINFO_NUM_LIMIT_LEN               5

#define CREATE_SIGNATURE_MAX_SIZE               300

#define RECIPIENTS_WSZ                          L"recipients"

#define RECIPIENTS_DIGIT_MAX_LENGTH             3


typedef struct CfgUserInfo
{
   WE_UINT16 wszName[ACCOUNT_USERINFO_NAME_LENGTH + 1]; 
   WE_UINT16 wszEmail[ACCOUNT_USERINFO_EMAIL_LENGTH + 1];
   WE_UINT16 wszReply2[ACCOUNT_USERINFO_REPLY2_EMAIL_LENGTH + 1];
   WE_UINT16 wszUserId[ACCOUNT_USERINFO_USER_ID_LENGTH + 1];
   WE_UINT16 wszPwd[ACCOUNT_USERINFO_PWD_LENGTH + 1];
   WE_UINT16 wszSmtpid[ACCOUNT_USERINFO_SMTP_ID_LENGTH + 1];
   WE_UINT16 wszSmtppwd[ACCOUNT_USERINFO_SMTP_PWD_LENGTH + 1];
   WE_CHAR   acRes[2];
}CfgUserInfo;

typedef struct CfgConnectInfo
{
   WE_UINT32 uiSerPro;
   WE_UINT32 uiRecSertype;
   WE_UINT16 wszPop3Server[POP3_SERVER_LENGTH +1];
   WE_UINT16 wszPop3Port[POP3_SERVER_PORT_LENGTH +1];
   WE_UINT16 wszImap4Server[IMAP4_SERVER_LENGTH +1];
   WE_UINT16 wszImap4Port[IMAP4_SERVER_PORT_LENGTH +1];
   WE_UINT16 wszSmtpServer[SMTP_SERVER_LENGTH +1];
   WE_UINT16 wszSmtpPort[SMTP_SERVER_PORT_LENGTH +1]; 
   WE_UINT32 uiUser1PrfId;
   WE_UINT32 uiUser2PrfId;
   WE_UINT32 uiUser3PrfId;
   WE_BOOL bImap4Ssl;
   WE_BOOL bPop3Ssl;
   WE_BOOL bSmtpSsl;
   WE_BOOL bSmtpAuth;
}CfgConnectInfo; 


typedef struct CfgMsgInfo  
{
   WE_UINT32 uiDownloadMode;
   WE_UINT16 wszMsgsize[ACT_MSGINFO_SIZE_LIMIT_LEN +1];
   WE_UINT16 wszNumLimit[ACT_MSGINFO_NUM_LIMIT_LEN + 1];
   WE_UINT16 wszSenderName[SENDER_NAME_LENGTH + 1];
   WE_BOOL bNewMsgNotif;
   WE_BOOL bReceiveLeaveCopy;
   WE_BOOL bDeleteLeaveCopy;
   WE_BOOL bSaveSent;
   WE_BOOL bIncludeOriginal;
   WE_BOOL bReadRecipt;
}CfgMsgInfo;


typedef struct ActCfg
{
   CfgUserInfo stUsrInfo;
   CfgConnectInfo stConnectInfo;
   CfgMsgInfo stMessageInfo;
   WE_UINT32 uiAddSignature;  
   WE_UINT32 uiDefaultInBoxId;
   WE_UINT16 wszActName[ACCOUNT_USERINFO_NAME_LENGTH+1];
   WE_CHAR aszCreateSig[EME_MAXPATH];
   WE_CHAR pcUidlFileDir[EME_MAXPATH];
   WE_CHAR pcUidFileDir[EME_MAXPATH];
   WE_BOOL bUnseen;
   WE_BOOL bDownLoadAttach;
}ActCfg;

typedef struct GloabalSetting
{
   WE_UINT16 wszProtectPassward[ACCOUNT_PROTECT_PASSWORLD_MAX_LEN];
}GloabalSetting;

typedef struct _EMSSyncMsgInfo
{
   AEECallback    syncmsginfoCallback;
}EMSSyncMsgInfo;

typedef struct _EMSFactoryReset
{
   AEECallback    factoryCallback;
}EMSFactoryReset;

typedef struct _EMSDelMultiMsg
{
   AEECallback    delmsgCallback;
   unsigned int   *puiMsgList;
   unsigned int   uiMsgCount;
   unsigned int   uiMsgIndex;
}EMSDelMultiMsg;

typedef struct _EMSDelMultitAct
{
   AEECallback    delactCallback;
   unsigned int   *puiActList;
   unsigned int   uiActCount;
   unsigned int   uiActIndex;
}EMSDelMultiAct;

typedef struct _EMSUpdateState
{
   AEECallback      updateCallback;
   StEMSUpdateState *pstMsgState;
   unsigned int     uiMsgStateCount;
}EMSUpdateState;

typedef struct _EMSSearchMsg
{
   AEECallback  searchCallback;
   EMS_MsgInfo  *pstMsgInfo;
   char         *pcFilter;
   unsigned int uiActId;
   unsigned int uiBoxId;
   unsigned int uiMsgNum;
}EMSSearchMsg;

typedef struct _EMSMoveMultiMsg
{
   AEECallback    movemsgCallback;
   unsigned int   *puiMsgList;
   unsigned int   uiActId;
   unsigned int   uiSrcBoxId;
   unsigned int   uiDesBoxId;
   unsigned int   uiMsgCount;
   unsigned int   uiMsgIndex;
}EMSMoveMultiMsg;

typedef struct _EMSRecoverMsg
{
   AEECallback    recoverCallback;
   unsigned int   *puiMsgList;
   unsigned int   uiActId;
   unsigned int   uiMsgCount;
   unsigned int   uiMsgIndex;
}EMSRecoverMsg;

typedef struct _EMSCleanMsg
{
   AEECallback    cleanCallback;
   unsigned int   uiActId;
   unsigned int   uiBoxId;
   unsigned int   uiSaveplace;
}EMSCleanMsg;

typedef struct EMSStore
{   
   FILEMGR_HANDLE hFileMgr;
   WE_VOID *      pvMemMgr;
   ActCfg         stActCfg;
   GloabalSetting stGloabalSetting;

   EMS_ASYNTYPE   asynType;

   struct
   {
      EMSDelMultiMsg  multiMsgDel;
      EMSDelMultiAct  multiActDel;
      EMSMoveMultiMsg multiMsgMove;
      EMSFactoryReset factoryReset;
      EMSUpdateState  stateUpdate;
      EMSSearchMsg    msgSearch;
      EMSRecoverMsg   recoverMsg;
      EMSSyncMsgInfo  syncMsgInfo;
      EMSCleanMsg     cleanMsg;
   }u;

}EMSStore;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* EMS_STORE_PRIV_H */
