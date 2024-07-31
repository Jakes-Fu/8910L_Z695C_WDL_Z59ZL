/*==============================================================================
    FILE NAME   : eme_multipartmsg.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        The implementation for multipartmsg.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-25   Wenkai Xu        01         Create

==============================================================================*/
/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "eme_multipartmsg_priv.h"
#include "we_msg.h"


/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/
//#define DECLARE_ME   MultiPartMsg *me = ((MultiPartMsg *)hMsg)


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
 *    FUNCTION:   HMULTIPARTMSG_GetPartCount
 *
 *    COMMRNT:    Return the message parts count in the message
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Return the parts number
 * 
 *----------------------------------------------------------------------------*/
WE_UINT32 HMULTIPARTMSG_GetPartCount(HMultiPartMsg hMsg)
{
   WE_UINT32 uiCount = 0;

   if (NULL == hMsg)
   {
      return 0;
   }
   
   IWEMSG_GetPartCount(hMsg, &uiCount);

   return uiCount;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_CompareOption
 *
 *    COMMRNT:    To check if the part is the one we wan't to search.
 *
 *    Pseudo Code:    
 *          1. Check input parameters.
 *          2. Compare the value depend on the id.
 * 
 *----------------------------------------------------------------------------*/
static WE_BOOL HMULTIPARTMSG_CompareOption(EME_MSGPART_ID nId, 
                                           WE_VOID *pVal, 
                                           HMsgPart hMsgPart, 
                                           WE_INT32 nPos)
{
   WE_INT32 nErr = EME_FAILED;
   MsgOpt stOpt = {0};
   
   switch(nId)
   {
   case EME_MSGPART_ID_CONTENTID:
      {
         WE_UINT32 dwSize = 0;
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_CONTENTID, 0, &stOpt);
         if (EME_SUCCESS == nErr)
         {
            dwSize = (EME_UTILS_WSTRLEN((WE_UINT16 *)stOpt.pVal) + 1) * sizeof(WE_UINT16);
            if (!EME_UTILS_MEMCMP(pVal, stOpt.pVal, dwSize))
            {
               return TRUE;
            }
         }
         
         return FALSE;
      }
   case EME_MSGPART_ID_PART_TYPE:
      {
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_PARTTYPE, 0, &stOpt);
         if (EME_SUCCESS == nErr && (WE_UINT32)stOpt.pVal == (WE_UINT32)pVal)
         {
            return TRUE;
         }
         
         return FALSE;
      }
   case EME_MSGPART_ID_ATTACHMENT_NAME:
      {
         WE_UINT32 dwSize = 0;
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_ATTACHNAME, 0, &stOpt);
         if (EME_SUCCESS == nErr)
         {
            dwSize = (EME_UTILS_WSTRLEN((WE_UINT16 *)stOpt.pVal) + 1) * sizeof(WE_UINT16);
            if (!EME_UTILS_MEMCMP(pVal, stOpt.pVal, dwSize))
            {
               return TRUE;
            }
         }
         
         return FALSE;
      }
   case EME_MSGPART_ID_POSITION:
      {
         if ((WE_INT32)pVal == nPos)
         {
            return  TRUE;
         }
         
         return FALSE;
      }
   case EME_MSGPART_ID_SECTIONID:
      {
         WE_UINT32 dwSize = 0;
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_SECTIONID, 0, &stOpt);
         if (EME_SUCCESS == nErr)
         {
            dwSize = (EME_UTILS_WSTRLEN((WE_UINT16 *)stOpt.pVal) + 1) * sizeof(WE_UINT16);
            if (!EME_UTILS_MEMCMP(pVal, stOpt.pVal, dwSize))
            {
               return TRUE;
            }
         }
         
         return FALSE;
      }
   default:
      return FALSE;
   }
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_FreePartNode
 *
 *    COMMRNT:    Remove the part node from the list and free the content in the node
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Remove the node from the list
 *          3. Free the content
 *          4. Free the node memory
 * 
 *----------------------------------------------------------------------------*/
static WE_VOID HMULTIPARTMSG_FreePartNode(WE_VOID *pvMemMgr, PartNode *pNode)
{
//    /* Invalidate parameter */
//    if (NULL == pNode)
//    {
//       return;
//    }
//    
//    /* Remove this node */
//    List_Remove(&pNode->node);
//    
//    /* Delete this part */
//    HMSGPART_Delete(pNode->hMsgPart);
//    
//    /* Free this node */
//    EME_UTILS_FREE(pvMemMgr, pNode);
   return;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_RemoveMessagePart
 *
 *    COMMRNT:    Remove one message part from multipart message.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *				2. find out the right part
 *          3. remove it
 *          4. decrease part number
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMULTIPARTMSG_RemoveMessagePart(HMultiPartMsg hMsg, EME_MSGPART_ID nId, WE_VOID *pVal)
{
   HMsgPart  hMsgPart = NULL;
   WE_UINT32 uiCnt = 0;
   WE_UINT32 uiIndex = 0;
   WE_BOOL   bFound = FALSE;
   
   IWEMSG_GetPartCount(hMsg, &uiCnt);
   
   while(uiIndex < uiCnt)
   {
      IWEMSG_GetPartValue(hMsg, &hMsgPart, uiIndex);
      if(hMsgPart != NULL)
      {
         if(HMULTIPARTMSG_CompareOption(nId, pVal, hMsgPart, uiIndex))
         {
            bFound = TRUE;
            IWEMSGVALUE_Release(hMsgPart);
            hMsgPart = NULL;
            break;
         }
         
         IWEMSGVALUE_Release(hMsgPart);
         hMsgPart = NULL;
      }
      
      uiIndex++;
   }
   
   if(bFound)
   {
      return IWEMSG_RemovePartValue(hMsg, uiIndex);
   }
   
   return EME_FAILED;

}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_GetMessagePart
 *
 *    COMMRNT:    Get one message part depend on part id and val
 *
 *    Pseudo Code:    
 *          1. Check input parameters.
 *          2. Enumerate all message part to find out the message part 
 *             which want to get.
 *          3. return it to caller and add it's reference.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMULTIPARTMSG_GetMessagePart(HMultiPartMsg hMsg, 
                                      EME_MSGPART_ID nId, 
                                      WE_VOID *pVal, 
                                      HMsgPart *phMsgPart)
{  
   HMsgPart hMsgPart = NULL;
   WE_UINT32 uiCnt = 0;
   WE_UINT32 uiIndex = 0;
   WE_BOOL   bFound = FALSE;

   IWEMSG_GetPartCount(hMsg, &uiCnt);

   while(uiIndex < uiCnt)
   {
      IWEMSG_GetPartValue(hMsg, &hMsgPart, uiIndex);
      if(hMsgPart != NULL)
      {
         if(HMULTIPARTMSG_CompareOption(nId, pVal, hMsgPart, uiIndex))
         {
            bFound = TRUE;
            break;
         }

         IWEMSGVALUE_Release(hMsgPart);
         hMsgPart = NULL;
      }

      uiIndex++;
   }

   if(bFound)
   {
      *phMsgPart = hMsgPart;
      return EME_SUCCESS;
   }

   return EME_FAILED;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_AddMessagePart
 *
 *    COMMRNT:    Add one message part into the message
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *				2. Create a new part node
 *          3. Copy the HMsgPart to the new node
 *          4. Insert it into the part list
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 HMULTIPARTMSG_AddMessagePart(HMultiPartMsg hMsg, HMsgPart hMsgPart, WE_INT32 nPos)
{
   /* nPos == -1 means the last */
   if (NULL == hMsg || NULL == hMsgPart || nPos < -1)
   {
      return EME_BADPARM;
   }
   
   return IWEMSG_AddPartValue(hMsg, hMsgPart, &nPos);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_Delete
 *
 *    COMMRNT:    Destroy the MultiPartMsg handler return by HMULTIPARTMSG_New
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Free option list
 *          3. Free the handler memory
 * 
 *----------------------------------------------------------------------------*/
static WE_VOID HMULTIPARTMSG_FreeAllPartNodes(MultiPartMsg *me)
{
//    PartNode *pNode = NULL;
//    PartNode *pTmp = NULL;
//    
//    if (NULL == me)
//    {
//       return;
//    }
//    
//    ForEachSafeEntryFWD(&me->head, pNode, pTmp, PartNode, node) {
//       HMULTIPARTMSG_FreePartNode(me->base.pvMemMgr, pNode);
//    }
//    
//    me->dwPartNum = 0;

   return;
}


WE_VOID HMULTIPARTMSG_Delete(HMultiPartMsg hMsg)
{
//    DECLARE_ME;
//    if (NULL == me)
//    {
//       return;
//    }
//    
//    HMULTIPARTMSG_FreeAllPartNodes(me);
//    HMSGOPTS_Dtor((MsgOpts *)me);
//    
//    EME_UTILS_FREEIF(me->base.pvMemMgr, me);
   IWEMSG_Release(hMsg);
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   HMULTIPARTMSG_New
 *
 *    COMMRNT:    Create a new HMultiPartMsg handler.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Allocate memory for the handler
 *          3. Construct the handler
 * 
 *----------------------------------------------------------------------------*/
static WE_VOID HMULTIPARTMSG_Ctor(MultiPartMsg *me, WE_VOID *pvMemMgr)
{
//    /* Construct the base handler */
//    HMSGOPTS_Ctor((MsgOpts *)me, pvMemMgr);
//    
//    List_Init(&me->head);
//    me->dwPartNum = 0;
   return;
}

HMultiPartMsg HMULTIPARTMSG_New(WE_VOID *pvMemMgr)
{
   HMultiPartMsg pMe = NULL;
   HMsgOpts      hMsgOpt = NULL;
   
   IWEMSG_New(&pMe);
   if(NULL == pMe)
   {
      return NULL;
   }

   IWEMSGVALUE_New(&hMsgOpt);
   if(NULL == hMsgOpt)
   {
      IWEMSG_Release(pMe);
      return NULL;
   }

   IWEMSG_AddHdrValue(pMe, hMsgOpt);

   IWEMSGVALUE_Release(hMsgOpt);

   return pMe;
}