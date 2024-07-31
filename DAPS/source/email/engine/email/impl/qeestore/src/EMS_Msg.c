/*==============================================================================
    FILE NAME   : EMS_Msg.c
    MODULE NAME : AppStore


    GENERAL DESCRIPTION:
        

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-17   Wenkai Xu        01         Create

==============================================================================*/
/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "EMS_Msg_API.h"
#include "eme_multipartmsg_api.h"

/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/


/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
int HEMSMSG_AddOpt(HEMSMsg hMsg, EMSMsgOpt *amo)
{
   return HMULTIPARTMSG_AddOpt((HMultiPartMsg)hMsg, (MsgOpt *)amo);
}

int HEMSMSG_GetOpt(HEMSMsg hMsg, int nOptId, int nIndex, EMSMsgOpt *pmo)
{
   return HMULTIPARTMSG_GetOpt((HMultiPartMsg)hMsg, (WE_INT32)nOptId, (WE_INT32)nIndex, (MsgOpt *)pmo);
}

int HEMSMSG_UpdateOpt(HEMSMsg hMsg, int nIndex, EMSMsgOpt *pmo)
{
   return HMULTIPARTMSG_UpdateOpt((HMultiPartMsg)hMsg, (WE_INT32)nIndex, (MsgOpt *)pmo);
}

int HEMSMSG_RemoveOpt(HEMSMsg hMsg, int nOptId, int nIndex)
{
   return HMULTIPARTMSG_RemoveOpt((HMultiPartMsg)hMsg, (WE_INT32)nOptId, (WE_INT32)nIndex);
}

unsigned int HEMSMSG_GetOptCount(HEMSMsg hMsg)
{
   return (unsigned int) HMULTIPARTMSG_GetOptCount((HMultiPartMsg)hMsg);
}

int HEMSMSG_AddMessagePart(HEMSMsg hMsg, HEMSMsgPart hMsgPart, int nPos)
{
   return HMULTIPARTMSG_AddMessagePart((HMultiPartMsg)hMsg, (HMsgPart)hMsgPart, (WE_INT32)nPos);
}

int HEMSMSG_GetMessagePart(HEMSMsg hMsg, EMS_MSGPART_ID nId, void *pVal, HEMSMsgPart *phMsgPart)
{
   return HMULTIPARTMSG_GetMessagePart((HMultiPartMsg)hMsg, (EME_MSGPART_ID)nId, (WE_VOID *)pVal, (HMsgPart *)phMsgPart);
}

int HEMSMSG_RemoveMessagePart(HEMSMsg hMsg, EMS_MSGPART_ID nId, void *pVal)
{
   return HMULTIPARTMSG_RemoveMessagePart((HMultiPartMsg)hMsg, (EME_MSGPART_ID)nId, (WE_VOID *)pVal);
}

unsigned int HEMSMSG_GetPartCount(HEMSMsg hMsg)
{
   return (unsigned int) HMULTIPARTMSG_GetPartCount((HMultiPartMsg)hMsg);
}

HEMSMsg HEMSMSG_New(void *pvMemMgr)
{
   return (HEMSMsg) HMULTIPARTMSG_New((WE_VOID *)pvMemMgr);
}

void HEMSMSG_Delete(HEMSMsg hMsg)
{
   HMULTIPARTMSG_Delete((HMultiPartMsg)hMsg);
}