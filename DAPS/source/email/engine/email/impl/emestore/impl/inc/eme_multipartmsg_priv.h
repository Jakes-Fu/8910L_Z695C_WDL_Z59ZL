/*==============================================================================
    FILE NAME   : eme_multipartmsg_priv.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        The private definition for MultiPartMsg implementation.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-25   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_MULTIPARTMSG_PRIV_H
#define EME_MULTIPARTMSG_PRIV_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_multipartmsg_api.h"
#include "eme_msgopts_priv.h"
#include "eme_utils_api.h"
#include "listbase.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef struct PartNode
{
   ListHead    node;
   HMsgPart    hMsgPart;
} PartNode;


typedef struct MultiPartMsg
{
   /*
    *	The MultiPartMsg handler 
    * is inherit from MsgOpts handler
    */
   MsgOpts        base;
   
   ListHead       head; /* Message part list head */
   WE_UINT32      dwPartNum;  /* Part number */
} MultiPartMsg;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/

#endif /* EME_MULTIPARTMSG_PRIV_H */
