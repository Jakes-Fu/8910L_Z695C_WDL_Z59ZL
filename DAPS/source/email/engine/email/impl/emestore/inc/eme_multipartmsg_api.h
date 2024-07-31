/*==============================================================================
    FILE NAME   : eme_multipartmsg_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        A set of interface functions define for email messaging mechanism. 

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-25   Wenkai Xu        01         Create
    2007-10-10   Wenkai Xu        02         Add some comment

==============================================================================*/
#ifndef EME_MULTIPARTMSG_API_H
#define EME_MULTIPARTMSG_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_msgopts_api.h"
#include "eme_msgpart_api.h"
#include "eme_constant_api.h"
#include "we_msg.h"
/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef IWEMsg*  HMultiPartMsg;

/*=========================Msg Opts=========================*/
/* Data Type: Integer */
#define EME_MSGOPT_ACTID                  EMAIL_OPT_ACCOUNTID
#define EME_MSGOPT_BOXID                  EMAIL_OPT_BOXID
#define EME_MSGOPT_MSGID                  EMAIL_OPT_MSGID

#define EME_MSGOPT_DATE                   EMAIL_OPT_DATE
#define EME_MSGOPT_TIME_ZONE              EMAIL_OPT_TIME_ZONE
#define EME_MSGOPT_STATUS                 EMAIL_OPT_STATUS
#define EME_MSGOPT_SIZE                   EMAIL_OPT_SIZE
#define EME_MSGOPT_TYPE                   EMAIL_OPT_MSGTYPE
#define EME_MSGOPT_PRIO                   EMAIL_OPT_PRIORITY
#define EME_MSGOPT_BEDOWN                 EMAIL_OPT_BEDOWN
/* For EMN */
#define EME_MSGOPT_EMN_AUTHTYPE           EMAIL_OPT_EMN_AUTHTYPE
#define EME_MSGOPT_EMN_PORT               EMAIL_OPT_EMN_PORT    
#define EME_MSGOPT_EMN_URITYPE            EMAIL_OPT_EMN_URITYPE 
#define EME_MSGOPT_EMN_UTCTIME            EMAIL_OPT_EMN_UTCTIME 

#define EME_MSGOPT_MSGHIDE                EMAIL_OPT_MSGHIDE
#define EME_MSGOPT_BOXFROM                EMAIL_OPT_BOXFROM

#define EME_MSGOPT_RECEIVE_DATE           EMAIL_OPT_RECEIVE_DATE
#define EME_MSGOPT_STORESIZE              EMAIL_OPT_STORESIZE

/* Data Type: str */
#define EME_MSGOPT_TO                     EMAIL_OPT_TO
#define EME_MSGOPT_CC                     EMAIL_OPT_CC
#define EME_MSGOPT_BCC                    EMAIL_OPT_BCC
#define EME_MSGOPT_FROM                   EMAIL_OPT_FROM
#define EME_MSGOPT_REPLY_TO               EMAIL_OPT_REPLY_TO
#define EME_MSGOPT_SUBJECT                EMAIL_OPT_SUBJECT
#define EME_MSGOPT_UIDL                   EMAIL_OPT_UIDL
#define EME_MSGOPT_BOXNAME                EMAIL_OPT_BOXNAME
#define EME_MSGOPT_DISP_NOTIFY_TO         EMAIL_OPT_DISP_NOTIFICATION_TO
#define EME_MSGOPT_CONTENT_TYPE           EMAIL_OPT_CONTENT_TYPE
/* For EMN */
#define EME_MSGOPT_EMN_USERNAME           EMAIL_OPT_EMN_USERNAME   
#define EME_MSGOPT_EMN_HOST               EMAIL_OPT_EMN_HOST       
#define EME_MSGOPT_EMN_MAILBOXNAME        EMAIL_OPT_EMN_MAILBOXNAME


/*=========================Opts Vals=========================*/
/*
 *	Email message type
 */
#define EMEMSG_TYPE_EMAIL                 0x01
#define EMEMSG_TYPE_EMN                   0x02

typedef WE_UINT32 EMEMSG_TYPE;


/*
 *	Email message status
 */
#define EMEMSG_STATUS_READ                0x00000001
#define EMEMSG_STATUS_HEAD_READ           0x00000002
#define EMEMSG_STATUS_WITH_ATTACH         0x00000004
#define EMEMSG_STATUS_PROTECTED           0x00000008
#define EMEMSG_STATUS_FORWARDED           0x00000010
#define EMEMSG_STATUS_REPLIED             0x00000020
#define EMEMSG_STATUS_SENT                0x00000040
#define EMEMSG_STATUS_SEND_FAILED         0x00000080
#define EMEMSG_STATUS_DRAFT               0x00000100
#define EMEMSG_STATUS_RETRIEVED           0x00000200
#define EMEMSG_STATUS_IN_SERVER           0x00000400
#define EMEMSG_STATUS_BE_DOWN             0x00000800
#define EMEMSG_STATUS_MAX                 0xffffffff

typedef WE_UINT32 EMEMSG_STATUS;

#define EME_PRIORITY_HIGH                1
#define EME_PRIORITY_NORMAL              3
#define EME_PRIORITY_LOW                 5

typedef WE_UINT32 EMEMSG_PRIORITY;

#define ADDR_SPACER_WCHAR  L','


/*
 * EMN URI Type
 */
#define EMEMSG_EMN_URITYPE_MAILAT         0x01
#define EMEMSG_EMN_URITYPE_POP            0x02
#define EMEMSG_EMN_URITYPE_IMAP           0x03
#define EMEMSG_EMN_URITYPE_OTHER          0x04

typedef WE_UINT32 EMEMSG_EMN_URITYPE;


/*
 * EMN Authentication Type
 */
#define EMEMSG_EMN_AUTHTYPE_ANY           0x01
#define EMEMSG_EMN_AUTHTYPE_POP_PLAIN     0x02
#define EMEMSG_EMN_AUTHTYPE_POP_APOP      0x03

typedef WE_UINT32 EMEMSG_EMN_AUTHTYPE;


#define EME_MSGPART_ID_CONTENTID          0x00000001        //EME_PARTOPT_CONTENTID
#define EME_MSGPART_ID_PART_TYPE          0x00000002        //EME_PARTOPT_PARTTYPE
#define EME_MSGPART_ID_ATTACHMENT_NAME    0x00000003        //EME_PARTOPT_ATTACHNAME
#define EME_MSGPART_ID_POSITION           0x00000004        //position
#define EME_MSGPART_ID_SECTIONID          0x00000005        //EME_PARTOPT_SECTIONID
#define EME_MSGPART_ID_MAX                0xffffffff

typedef WE_UINT32 EME_MSGPART_ID;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
static __inline WE_INT32 HMULTIPARTMSG_AddOpt(HMultiPartMsg hMsg, MsgOpt *amo)
{
   HMsgPart hMsgPart = NULL;
   WE_INT32 nErr = EME_SUCCESS;

   nErr = IWEMSG_GetHdrValue(hMsg, &hMsgPart);
   if(hMsgPart != NULL)
   {
      nErr = HMSGOPTS_AddOpt((HMsgOpts)hMsgPart, amo);
      IWEMSGVALUE_Release(hMsgPart);
   }

   return nErr;
}

static __inline WE_INT32 HMULTIPARTMSG_GetOpt(HMultiPartMsg hMsg, WE_INT32 nOptId, WE_INT32 nIndex, MsgOpt *pmo)
{
   HMsgPart hMsgPart = NULL;
   WE_INT32 nErr = EME_SUCCESS;
   
   nErr = IWEMSG_GetHdrValue(hMsg, &hMsgPart);
   if(hMsgPart != NULL)
   {
      nErr = HMSGOPTS_GetOpt((HMsgOpts)hMsgPart, nOptId, nIndex, pmo);
      IWEMSGVALUE_Release(hMsgPart);
   }

   return nErr;
}

static __inline WE_INT32 HMULTIPARTMSG_UpdateOpt(HMultiPartMsg hMsg, WE_INT32 nIndex, MsgOpt *pmo)
{
   HMsgPart hMsgPart = NULL;
   WE_INT32 nErr = EME_SUCCESS;
   
   nErr = IWEMSG_GetHdrValue(hMsg, &hMsgPart);
   if(hMsgPart != NULL)
   {
      nErr = HMSGOPTS_UpdateOpt((HMsgOpts)hMsgPart, nIndex, pmo);
      IWEMSGVALUE_Release(hMsgPart);
   }
   
   return nErr;
}

static __inline WE_INT32 HMULTIPARTMSG_RemoveOpt(HMultiPartMsg hMsg, WE_INT32 nOptId, WE_INT32 nIndex)
{
   HMsgPart hMsgPart = NULL;
   WE_INT32 nErr = EME_SUCCESS;
   
   nErr = IWEMSG_GetHdrValue(hMsg, &hMsgPart);
   if(hMsgPart != NULL)
   {
      nErr = HMSGOPTS_RemoveOpt((HMsgOpts)hMsgPart, nOptId, nIndex);
      IWEMSGVALUE_Release(hMsgPart);
   }
   
   return nErr;
}

static __inline WE_UINT32 HMULTIPARTMSG_GetOptCount(HMultiPartMsg hMsg)
{
   HMsgPart  hMsgPart = NULL;
   WE_UINT32 uiCnt = 0;
   
   IWEMSG_GetHdrValue(hMsg, &hMsgPart);
   if(hMsgPart != NULL)
   {
      uiCnt = HMSGOPTS_GetOptCount((HMsgOpts)hMsgPart);
      IWEMSGVALUE_Release(hMsgPart);
   }
   
   return uiCnt;
}


WE_INT32 HMULTIPARTMSG_AddMessagePart(HMultiPartMsg hMsg, HMsgPart hMsgPart, WE_INT32 nPos);
WE_INT32 HMULTIPARTMSG_GetMessagePart(HMultiPartMsg hMsg, EME_MSGPART_ID nId, WE_VOID *pVal, HMsgPart *phMsgPart);
WE_INT32 HMULTIPARTMSG_RemoveMessagePart(HMultiPartMsg hMsg, EME_MSGPART_ID nId, WE_VOID *pVal);
WE_UINT32 HMULTIPARTMSG_GetPartCount(HMultiPartMsg hMsg);

HMultiPartMsg HMULTIPARTMSG_New(WE_VOID *pvMemMgr);
WE_VOID HMULTIPARTMSG_Delete(HMultiPartMsg hMsg);

/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Function:      HMULTIPARTMSG_AddMessagePart
 * 
 *    Description:   Adds one message part to specified position in the message.
 * 
 *    Prototype:
 *       WE_INT32 HMULTIPARTMSG_AddMessagePart(HMultiPartMsg hMsg, HMsgPart hMsgPart, WE_INT32 nPos);
 * 
 *    Parameters:
 *       hMsg     [in]: The message handler.
 *       hMsgPart [in]: The part handler.
 *       nPos     [in]: The position at which the message part has to added/inserted,
 *                      -1 means the end position.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the part could be added successfully
 *       EME_NOMEMORY:  If enough memory couldn't be allocated
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMULTIPARTMSG_GetMessagePart
 * 
 *    Description:   Retrieves the corresponding message part as specified by 
 *                   the Message part id and value.
 * 
 *    Prototype:
 *       WE_INT32 HMULTIPARTMSG_GetMessagePart(HMultiPartMsg hMsg, EME_MSGPART_ID nId, WE_VOID *pVal, HMsgPart *phMsgPart);
 * 
 *    Parameters:
 *       hMsg        [in]: The message handler.
 *       nId         [in]: The message part id, of type EME_MSGPART_ID.
 *       pVal        [in]: The id value for which we are making a search.
 *       phMsgPart  [out]: Upon return would point to a valid HMsgPart.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If we are able to get the message part.
 *       EME_NOMEMORY:  If we can't allocate enough memory.
 *       EME_FAILED:    Otherwise.
 *  	   
 *    Comments:   
 *       The user may want to query the for a particular message part of type, whose
 *       content-id is "x-Designing-Msg-Parts", he would pass in the nId as,
 *       EME_MSGPART_ID_CONTENTID, and pVal as "x-Designing-Msg-Parts", then we would
 *       return the message part which matches this criteria.
 * 
 *       Once the user is done using the structure he needs to free the memory.
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMULTIPARTMSG_RemoveMessagePart
 * 
 *    Description:   Removes the corresponding message part, specified by the Id and Value.
 * 
 *    Prototype:
 *       WE_INT32 HMULTIPARTMSG_RemoveMessagePart(HMultiPartMsg hMsg, EME_MSGPART_ID nId, WE_VOID *pVal);
 * 
 *    Parameters:
 *       hMsg        [in]: The message handler.
 *       nId         [in]: The message part id, of type EME_MSGPART_ID.
 *       pVal        [in]: The id value for which we are making a search.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If we are able to remove the message part.
 *       EME_FAILED:    Otherwise.
 *  	   
 *    Comments:   
 *       The user may want to delete a particular message part of type, whose
 *       content-id is "x-Designing-Msg-Parts", he would pass in the nId as,
 *       EME_MSGPART_ID_CONTENTID, and pVal as "x-Designing-Msg-Parts", then we would
 *       delete the message part which matches this criteria.
 * 
 *       If more than one is found do we delete all of them?? //@TODO
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMULTIPARTMSG_GetPartCount
 * 
 *    Description:   Gets the message parts count.
 * 
 *    Prototype:
 *       WE_UINT32 HMULTIPARTMSG_GetPartCount(HMultiPartMsg hMsg);
 * 
 *    Parameters:
 *       hMsg        [in]: The message handler.
 * 
 *    Return Value:
 *       The part count.
 *  	   
 *    Comments:   
 *       The user may want to get the message parts count which can be retrieved 
 *       by using this API. 
 * 
*============================================================================*/

#endif /* EME_MULTIPARTMSG_API_H */
