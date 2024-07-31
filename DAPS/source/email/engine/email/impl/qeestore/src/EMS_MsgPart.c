/*==============================================================================
    FILE NAME   : EMS_MsgPart.c
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
#include "EMS_MsgPart_API.h"
#include "eme_msgpart_api.h"

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
int HEMSMSGPART_AddOpt(HEMSMsgPart hMsgPart, EMSMsgOpt *amo)
{
   return HMSGPART_AddOpt((HMsgPart)hMsgPart, (MsgOpt *)amo);
}

int HEMSMSGPART_GetOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex, EMSMsgOpt *pmo)
{
   return HMSGPART_GetOpt((HMsgPart)hMsgPart, (WE_INT32)nOptId, (WE_INT32)nIndex, (MsgOpt *)pmo);
}

int HEMSMSGPART_UpdateOpt(HEMSMsgPart hMsgPart, int nIndex, EMSMsgOpt *pmo)
{
   return HMSGPART_UpdateOpt((HMsgPart)hMsgPart, (WE_INT32)nIndex, (MsgOpt *)pmo);
}

int HEMSMSGPART_RemoveOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex)
{
   return HMSGPART_RemoveOpt((HMsgPart)hMsgPart, (WE_INT32)nOptId, (WE_INT32)nIndex);
}

unsigned int HEMSMSGPART_GetOptCount(HEMSMsgPart hMsgPart)
{
   return (unsigned int) HMSGPART_GetOptCount((HMsgPart)hMsgPart);
}

HEMSMsgPart HEMSMSGPART_New(void *pvMemMgr)
{
   return (HEMSMsgPart) HMSGPART_New((WE_VOID *)pvMemMgr);
}

void HEMSMSGPART_Delete(HEMSMsgPart hMsgPart)
{
   HMSGPART_Delete((HMsgPart)hMsgPart);
}