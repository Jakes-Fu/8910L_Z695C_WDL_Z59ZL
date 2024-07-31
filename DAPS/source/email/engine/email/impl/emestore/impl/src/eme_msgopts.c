/*==============================================================================
    FILE NAME   : eme_msgopts.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        This file contain the implementation for the HMsgOpts.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-24   Wenkai Xu        01         Create

==============================================================================*/
/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "eme_msgopts_priv.h"
#include "we_msg.h"

/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/
//#define DECLARE_ME   (IWEMsgValue) *me = ((IWEMsgValue *)hMsgOpts)

/*==============================================================================
 *
 *    Static Function Prototype Declare Section
 *
 *============================================================================*/


/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_GetOpt
 *
 *    COMMRNT:    Get the option by option id
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Find the right option
 *          3. If find, return this option
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_GetOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex, MsgOpt *pmo)
{
   if (NULL == hMsgOpts || NULL == pmo || nIndex < 0)
   {
      return EME_BADPARM;
   }
   
   return IWEMSGVALUE_GetOptByIndex(hMsgOpts, nOptId, nIndex, (WEMsgOpt*)pmo);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_UpdateOpt
 *
 *    COMMRNT:    Update the right option
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Find the right option
 *          3. update the option
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_UpdateOpt(HMsgOpts hMsgOpts, WE_INT32 nIndex, MsgOpt *pmo)
{
   if (NULL == hMsgOpts || NULL == pmo || nIndex < 0)
   {
      return EME_BADPARM;
   }
   
   return IWEMSGVALUE_UpdateOptByIndex(hMsgOpts, nIndex, (WEMsgOpt*)pmo);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_RemoveOpt
 *
 *    COMMRNT:    Remove one option from the MsgOpts
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Find the right option
 *          3. Remove the option
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_RemoveOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex)
{
   if (NULL == hMsgOpts || nIndex < 0)
   {
      return EME_BADPARM;
   }
   
   return IWEMSGVALUE_RemoveOpt(hMsgOpts, nOptId, nIndex);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_GetOptCount
 *
 *    COMMRNT:    Return the options count in the MsgOpts
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Return the options number
 * 
 *----------------------------------------------------------------------------*/
WE_UINT32 HMSGOPTS_GetOptCount(HMsgOpts hMsgOpts)
{
   WE_UINT32 uiCount = 0;
   if (NULL == hMsgOpts)
   {
      return 0;
   }

   IWEMSGVALUE_GetOptNum(hMsgOpts, &uiCount);

   return uiCount;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_AddOpt
 *
 *    COMMRNT:    Add options into the OptNode List in MsgOpts handler
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. For each option
 *          3. Create new OptNode
 *          4. Copy the option to the node
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_AddOpt(HMsgOpts hMsgOpts, MsgOpt *amo)
{
   if (NULL == hMsgOpts || NULL == amo)
   {
      return EME_BADPARM;
   }
   
   return IWEMSGVALUE_AddOpt(hMsgOpts, (WEMsgOpt*)amo);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_Delete
 *
 *    COMMRNT:    Destroy the MsgOpts handler return by HMSGOPTS_New
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Free option list
 *          3. Free the handler memory
 * 
 *----------------------------------------------------------------------------*/
WE_VOID HMSGOPTS_Delete(HMsgOpts hMsgOpts)
{
   IWEMSGVALUE_Release(hMsgOpts);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_AddRef
 *
 *    COMMRNT:    Add a reference for the handler
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Increase the reference count
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_AddRef(HMsgOpts hMsgOpts)
{
   return IWEMSGVALUE_AddRef(hMsgOpts);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMSGOPTS_New
 *
 *    COMMRNT:    Create a new HMsgOpts handler.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Allocate memory for the handler
 *          3. Construct the handler
 * 
 *----------------------------------------------------------------------------*/
HMsgOpts HMSGOPTS_New(WE_VOID *pvMemMgr)
{
   HMsgOpts hMsgOpts = NULL;

   IWEMSGVALUE_New(&hMsgOpts);

   return hMsgOpts;
}