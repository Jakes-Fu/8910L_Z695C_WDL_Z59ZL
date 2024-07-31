/*=====================================================================================
FILE NAME   : eme_constant_api.h
MODULE NAME : Email engine

  GENERAL DESCRIPTION
    provide constant api define of email engine.
    
  TechFaith Software Confidential Proprietary
  Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
  =======================================================================================
  Revision History
  
  Modification                   Tracking
  Date              Author       Number       Description of changes
  ----------   --------------   ---------   --------------------------------------
  2007-03-02     Jiang Dingqing                  create eme_constant_api.h
        
=====================================================================================*/

#ifndef _EME_CONSTAN_API_H_
#define _EME_CONSTAN_API_H_

#include "we_def.h"
#include "we_msg.h"


typedef struct EmailOpt
{
   WE_INT32          iId;            /* from below defines */
   WE_INT32          type;
   WE_INT32          size;
   WE_VOID           *pVal;          /* depends on opt name */
}EmailOpt;


/*opt*/
#define EMAILOPT_STR_START            WEMSG_OPT_SZ
#define EMAILOPT_STR_END              WEMSG_OPT_SZ_LAST
#define EMAILOPT_INT_START            WEMSG_OPT_32BIT
#define EMAILOPT_INT_END              WEMSG_OPT_32BIT_LAST
#define EMAILOPT_WSTR_START           WEMSG_OPT_WSZ
#define EMAILOPT_WSTR_END             WEMSG_OPT_WSZ_LAST

#define EMAILOPT_HEADER_START         0x00001000
#define EMAILOPT_PART_START           0x00002000
#define EMAILOPT_PARAM_START          0x00004000 
#define EMAILOPT_MESSAGE_START        0x00008000

#define EMAILOPT_END                  0x00000000

/*****************************************************************************************************/
/************************************** Header Opts **************************************************/
/*****************************************************************************************************/

/************** int ****************/
#define EMAIL_OPT_DATE                       (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x01) 
#define EMAIL_OPT_TIME_ZONE                  (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x02)
#define EMAIL_OPT_PRIORITY                   (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x03)
/* for store only */
#define EMAIL_OPT_STATUS                     (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x04)
#define EMAIL_OPT_SIZE                       (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x05)
#define EMAIL_OPT_ATTACH_NUM                 (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x06)
#define EMAIL_OPT_BEDOWN                     (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x07)
#define EMAIL_OPT_ACCOUNTID                  (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x08)
#define EMAIL_OPT_BOXID                      (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x09)
#define EMAIL_OPT_MSGID                      (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0A)
#define EMAIL_OPT_MSGTYPE                    (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0B)

/* For EMN */
#define EMAIL_OPT_EMN_AUTHTYPE               (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0C)
#define EMAIL_OPT_EMN_PORT                   (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0D)
#define EMAIL_OPT_EMN_URITYPE                (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0E)
#define EMAIL_OPT_EMN_UTCTIME                (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x0F)

/*hide*/
#define EMAIL_OPT_MSGHIDE                    (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x10)
#define EMAIL_OPT_BOXFROM                    (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x11)

#define EMAIL_OPT_RECEIVE_DATE               (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x12)
#define EMAIL_OPT_STORESIZE                  (EMAILOPT_INT_START+EMAILOPT_HEADER_START+0x13)

/************** str ****************/
#define EMAIL_OPT_TO                         (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x01)
#define EMAIL_OPT_CC                         (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x02)      
#define EMAIL_OPT_BCC                        (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x03)      
#define EMAIL_OPT_FROM                       (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x04) 
#define EMAIL_OPT_REPLY_TO                   (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x05)                 
#define EMAIL_OPT_SUBJECT                    (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x06)
#define EMAIL_OPT_DISP_NOTIFICATION_TO       (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x07)
#define EMAIL_OPT_X_MAILER                   (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x08)
#define EMAIL_OPT_CONTENT_TYPE               (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x09)
/*for mmail*/
#define EMAIL_OPT_X_CUMAIL_ACCOUNT           (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0A)
#define EMAIL_OPT_X_CUMAIL_FETCHCONTRLO      (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0B)
#define EMAIL_OPT_X_CUMAIL_FLAGIT            (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0C)
/* for store only */
#define EMAIL_OPT_UIDL                       (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0D)
#define EMAIL_OPT_BOXNAME                    (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0E)
/* For EMN */
#define EMAIL_OPT_EMN_USERNAME               (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x0F)
#define EMAIL_OPT_EMN_HOST                   (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x10)
#define EMAIL_OPT_EMN_MAILBOXNAME            (EMAILOPT_STR_START+EMAILOPT_HEADER_START+0x11)


/*****************************************************************************************************/
/************************************** Part Opt *****************************************************/
/*****************************************************************************************************/
/************** int ****************/
#define EMAILPART_OPT_SRC_TYPE               (EMAILOPT_INT_START+EMAILOPT_PART_START+0x01)
#define EMAILPART_OPT_DATA_SIZE              (EMAILOPT_INT_START+EMAILOPT_PART_START+0x02)
#define EMAILPART_OPT_PART_TYPE              (EMAILOPT_INT_START+EMAILOPT_PART_START+0x03)
/* for store only */
#define EMAILPART_OPT_PARTID                 (EMAILOPT_INT_START+EMAILOPT_PART_START+0x04)
/*push mail field*/
#define EMAILPART_OPT_X_ATTACHMENT           (EMAILOPT_INT_START+EMAILOPT_PART_START+0x05)
#define EMAILPART_OPT_X_ATTACHMENT_SIZE      (EMAILOPT_INT_START+EMAILOPT_PART_START+0x06)
#define EMAILPART_OPT_CONTENT_SIZE           (EMAILOPT_INT_START+EMAILOPT_PART_START+0x07)

/************** str ****************/
#define EMAILPART_OPT_CONTENT_ID             (EMAILOPT_STR_START+EMAILOPT_PART_START+0x01)
#define EMAILPART_OPT_DATA_BUFF              (EMAILOPT_STR_START+EMAILOPT_PART_START+0x02)
#define EMAILPART_OPT_TRANS_ENCODING         (EMAILOPT_STR_START+EMAILOPT_PART_START+0x03)
#define EMAILPART_OPT_CONTENT_DISP           (EMAILOPT_STR_START+EMAILOPT_PART_START+0x04)
#define EMAILPART_OPT_MIME_TYPE              (EMAILOPT_STR_START+EMAILOPT_PART_START+0x05)
/*for body struct*/
#define EMAILPART_OPT_SECTION_ID             (EMAILOPT_STR_START+EMAILOPT_PART_START+0x06)

/*****************************************************************************************************/
/************************************** Param Opts ***************************************************/
/*****************************************************************************************************/
/************** str ****************/
#define EMAILPARAM_OPT_FILE_NAME             (EMAILOPT_STR_START+EMAILOPT_PARAM_START+0x01)
#define EMAILPARAM_OPT_CHARSET               (EMAILOPT_STR_START+EMAILOPT_PARAM_START+0x02)
#define EMAILPARAM_OPT_START                 (EMAILOPT_STR_START+EMAILOPT_PARAM_START+0x03)
#define EMAILPARAM_OPT_TYPE                  (EMAILOPT_STR_START+EMAILOPT_PARAM_START+0x04)

/*****************************************************************************************************/
/************************************** Message Opts *************************************************/
/*****************************************************************************************************/
/************** int ****************/
#define EMAILMESSAGE_OPT_SRC_TYPE            (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x01)
#define EMIALMESSAGE_OPT_DECODE_TYPE         (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x02)
/* for store only */
#define EMIALMESSAGE_OPT_REC_SERTYPE         (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x03)
#define EMIALMESSAGE_OPT_BUFFER_SIZE         (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x04)
#define EMAILMESSAGE_OPT_ACTID               (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x05)
#define EMAILMESSAGE_OPT_DEF_INBOXID         (EMAILOPT_INT_START+EMAILOPT_MESSAGE_START+0x06)

/************** str ****************/
#define EMAILMESSAGE_OPT_BUFF                (EMAILOPT_STR_START+EMAILOPT_MESSAGE_START+0x01)



/*****************************************************************************************************/
/************************************** Defined Values ***********************************************/
/*****************************************************************************************************/
/* EMAIL_OPT_STATUS */
#define EMAIL_STATUS_READ                0x00000001
#define EMAIL_STATUS_WITH_BODY           0x00000002
#define EMAIL_STATUS_WITH_ATTACH         0x00000004
#define EMAIL_STATUS_PROTECTED           0x00000008
#define EMAIL_STATUS_FORWARDED           0x00000010
#define EMAIL_STATUS_REPLIED             0x00000020
#define EMAIL_STATUS_SENT                0x00000040
#define EMAIL_STATUS_SEND_FAILED         0x00000080
#define EMAIL_STATUS_DRAFT               0x00000100
#define EMAIL_STATUS_RETRIEVED           0x00000200
#define EMAIL_STATUS_IN_SERVER           0x00000400
#define EMAIL_STATUS_BE_DOWN             0x00000800
#define EMAIL_STATUS_MAX                 0xffffffff



/* EMIALMESSAGE_OPT_DECODE_TYPE */
#define  EMAIL_DECODETYPE_HEADER          0x00000001
#define  EMAIL_DECODETYPE_BODYSTRUCT      0x00000002
#define  EMAIL_DECODETYPE_MSGPART         0x00000003
#define  EMAIL_DECODETYPE_WHOLEMSG        0x00000004
#define  EMAIL_DECODETYPE_MSGPART_LIST    0x00000005
typedef WE_UINT32 EmailDecodeType;

/* EMAILPART_OPT_SRC_TYPE */
#define EMAILPART_SRCTYPE_BUFF            0x00000001
#define EMAILPART_SRCTYPE_FILE            0x00000002
typedef WE_UINT32 EmailSrcType;

/* EMAILPART_OPT_PART_TYPE */
#define EMAILPART_PARTTYPE_TEXTPLAIN      0x00000001
#define EMAILPART_PARTTYPE_TEXTHTML       0x00000002
#define EMAILPART_PARTTYPE_ATTACHMENT     0x00000003
typedef WE_UINT32 EmailPartType;

/* EMAILMESSAGE_OPT_SRC_TYPE */
#define EMAIL_ENCODE_BYBUFF               0x00000001
#define EMAIL_ENCODE_BYFILE               0x00000002
typedef WE_UINT32 EmailEncodeType;

/* EMIALMESSAGE_OPT_REC_SERTYPE */
#define EMAIL_REC_SER_TYPE_IMAP4          0x00000001
#define EMAIL_REC_SER_TYPE_POP3           0x00000002
typedef WE_UINT32 EmailRecSerType;


#endif  //_EME_CONSTAN_API_H_