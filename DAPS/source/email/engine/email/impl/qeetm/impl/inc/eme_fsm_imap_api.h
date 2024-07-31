/*=====================================================================================
FILE NAME   : eme_fsm_imap_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Supply api for dispatching signals to proper FSM according to signal id.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_fsm_imap_api.h
          
=====================================================================================*/

#ifndef _EME_FSM_IMAP_H
#define _EME_FSM_IMAP_H
/*---------------------------------------------------------------
*       Include Files
* ------------------------------------------------------------*/
/*Include System head file*/
#include "we_def.h"
/*Include Program Global head file*/
#include "eme_tm_def.h"
#include "eme_imap_api.h"
/*Include module head file*/

/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
/*Define Constant Macro end*/


/*Define Macro Function start*/
/*Define Macro Function end*/

/*Define ENUM type start*/
/*Define ENUM type end*/
/*---------------------------------------------------------------------
 *  Here you may want to include extra documentation on data structures
 * ------------------------------------------------------------------*/
/*Define base type start*/
/*Define base type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/

/*---------------------------------------------------------------------
 *             Function Declarations
 * ------------------------------------------------------------------*/
WE_INT32 EME_HandleIMAPSig(St_TMHandle *pstTmHandle, WE_UINT32 uiEvtCode, WE_UINT16 usWParam, WE_UINT32 ulDwParam);
WE_VOID EME_IMAPActionCB(WE_VOID* pvTmHandle, St_IMAPActRet* pstActRet);




#endif             /* #ifndef _EME_FSM_IMAP_H */
