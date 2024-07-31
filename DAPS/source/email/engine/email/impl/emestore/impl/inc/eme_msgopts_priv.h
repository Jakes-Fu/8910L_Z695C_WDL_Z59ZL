/*==============================================================================
    FILE NAME   : eme_msgopts_priv.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        The private definition for msgopts implementation.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-24   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_MSGOPTS_PRIV_H
#define EME_MSGOPTS_PRIV_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_msgopts_api.h"
#include "eme_utils_api.h"
#include "listbase.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
/*
 *	Define the option list node structure
 */
typedef struct OptNode
{
   ListHead    node;
   MsgOpt      stOpt;
} OptNode;


typedef struct MsgOpts
{
   WE_INT32    nRefs;      /* Reference counter */
   
   ListHead    head;       /* Option list head */
   WE_UINT32   dwOptNum;   /* Options number */
   
   WE_VOID     *pvMemMgr;  /* Memory manager handler */
} MsgOpts;


/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/

#endif /* EME_MSGOPTS_PRIV_H */