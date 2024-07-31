/*==============================================================================
    FILE NAME   : email_store_priv.h
    MODULE NAME : CoreStore


    GENERAL DESCRIPTION:
        

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-08   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMAIL_STORE_PRIV_H
#define EMAIL_STORE_PRIV_H

/*------------------------------------------------------------------------------
 *    Include Files
*----------------------------------------------------------------------------*/
#include "email_store_api.h"
#include "eme_multipartmsg_api.h"
#include "eme_dataaccess_api.h"
#include "eme_msgstore_api.h"
#include "eme_msgindex_api.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef struct StoreHandle
{
   WE_UINT32      dwActId;
   WE_UINT32      dwBoxId;
   WE_UINT32      dwMsgId;
   WE_CHAR        pcMsgPath[EME_MAXPATH];
   
   FILEMGR_HANDLE hFileMgr;
   WE_VOID *      pvMemMgr;
   
   MsgOpt *       pstHeaderOpt;
   MsgOpt *       pstPartOpt;
   
   HMultiPartMsg  hMsg;
}StoreHandle;

#endif /* EMAIL_STORE_PRIV_H */
