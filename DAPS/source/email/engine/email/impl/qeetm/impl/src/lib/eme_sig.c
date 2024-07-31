/*=====================================================================================
FILE NAME   : eme_sig.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some function for managing sigs.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_sig.c
          
=====================================================================================*/

//pclint
/*lint -e413 */

/*===================================================================
 * 
 *                     Include Files
 * 
====================================================================*/
/*Include System head file*/

/*Include Program Global head file*/
#include "eme_utils_api.h"
#include "eme_error.h"
/*Include Module head file*/
#include "eme_sig_api.h"
#include "eme_tm_def.h"
#include "eme_list_api.h"
#include "eme_tm_api.h"
#include "oem_tmlib_api.h"
#include "eme_fsm_imap_api.h"
#include "eme_fsm_pop_api.h"
#include "eme_fsm_smtp_api.h"
#include "oem_asyncproc_api.h"
/*Include Private head file*/


   
/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/

/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/


/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/
/*signal free list max length*/


/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/




/*===================================================================
* 
*                    Functions
* 
* ==================================================================*/

/*============================================================================ 
 *    FUNCTION        EME_SignalProcess()
 * 
 *    DESCRIPTION     Get a signal from signal queue and process it,
                      If signal free list is not full, this signal will add to it;
 * 
 *    DEPENDENCIES    None
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    WE_INT32:   EME_SUCCESS if process signal successful
 *                              EME_FAILED if process signal failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
WE_INT32 EME_SignalProcess(St_TMHandle* pstTMHandle)
{
   St_EMEFsmSigData  *pstSignal = NULL;
   WE_UINT32         uiSignal = EME_INVALID_SIGNAL_ID;/*4 bytes is fast*/
   WE_INT32          iResult = EME_SUCCESS;
   St_TMHandle       *pMe = pstTMHandle;
   WE_INT32          iIdx = 0;
   
   if(NULL == pMe)
   {
      return EME_FAILED;
   }
   
   do 
   {
      EME_UTILS_LOG_INFO(("EME_SignalProcess : START!!"));
      /************************************************************************/
      /* get signal to be processed from signal queue, and add it to free list*/                                                                  
      /************************************************************************/
      if(EME_list_empty(&pMe->stSignalQueueFirst))
      {
         iResult = EME_FAILED;
         EME_UTILS_BREAKIF(iResult,"EME_SignalProcess, empty signal queue");
      }
      /* delete from one list and add as another's head */ 
      pstSignal = list_entry(pMe->stSignalQueueFirst.next, St_EMEFsmSigData, stSigList);
      EME_list_del(&pstSignal->stSigList);
      
      /* dispatch signal to proper FSM */
      iIdx = pstSignal->uiEMEFsmId;
      uiSignal = pstSignal->uiSignalId;
      
      EME_UTILS_LOG_INFO(("EME_SignalProcess, add signal to queue, FSM = %d, Signal = %x", iIdx, uiSignal));
      switch(iIdx)
      {
      case EME_FSM_SMTP:
         iResult = EME_HandleSMTPSig(pMe, uiSignal, 0, (WE_UINT32)pstSignal);
         break;
      case EME_FSM_POP:
         iResult = EME_HandlePOPSig(pMe, uiSignal, 0, (WE_UINT32)pstSignal);
         break;
      case EME_FSM_IMAP:
         iResult = EME_HandleIMAPSig(pMe, uiSignal, 0, (WE_UINT32)pstSignal);
         break;
      case EME_FSM_ASYNC:
         iResult = OEM_AsyncProc_ProcHandle(pstSignal->pvParam);
         EME_SignalDelete(pstTMHandle, pstSignal);
         OEMTMLib_EmailTriggerDispatcher(pstTMHandle);
         break;
      default:
         iResult = EME_FAILED;
         break;
      }
      EME_UTILS_BREAKIF(iResult, "EME_SignalProcess, handle sig failed");
            
   } while(0);
   
//    if(EME_SUCCESS != iResult)
//    {
//       EME_SignalDelete(pstTMHandle, pstSignal);
//    }
   
   return iResult;
}



/*============================================================================ 
 *    FUNCTION        EME_SignalDelete()
 * 
 *    DESCRIPTION     Delete a specified signal from signal queue.   
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    WE_INT32:    EME_SUCCESS if delete specified signal was successful
 *                               EME_FAILED if delete specified signal failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
WE_INT32 EME_SignalDelete(St_TMHandle* pstTMHandle, St_EMEFsmSigData* pstSig)
{
   St_TMHandle *pMe = pstTMHandle;
   void        *pvMemHdr = NULL;
   
   if(NULL == pstSig || NULL == pMe)
   {
      return EME_FAILED;
   }
   
    pvMemHdr = EME_OEMTMLib_GetMemHdr(pMe);
   
   do 
   {
      /************************************************************************/
      /* add deleted signal to the front node of free list                    */
      /************************************************************************/
      if(EME_MAX_FREE_SIGNAL_LIST_LENGTH > pMe->iSignalFreeListLength)
      {
         /************************************************************************/
         /* initialize free node                                                 */                 
         /************************************************************************/
         EME_UTILS_MEMSET(pstSig, 0, sizeof(St_EMEFsmSigData));
         EME_list_add_tail(&pstSig->stSigList, &pMe->stSignalFreeList);
         pMe->iSignalFreeListLength++;
         
         EME_UTILS_LOG_INFO(("EME_SignalDelete, add delete signal to free list, %x, free list size = %d", 
            pstSig, pMe->iSignalFreeListLength));
      }
      else
      {
         /************************************************************************/
         /*   no free node available ,free  the signal                           */                                       
         /************************************************************************/
         EME_UTILS_FREEIF(pvMemHdr, pstSig);

      }   
      
   } while(0);
   
   return EME_SUCCESS;
}

/*============================================================================ 
 *    FUNCTION        EME_SignalInit()
 * 
 *    DESCRIPTION     Initializes the signal queue.    
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    WE_INT32:    EME_SUCCESS if initialization was successful
 *                               EME_FAILED if initialization failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
WE_INT32 EME_SignalInit(St_TMHandle* pstTMHandle)
{
   St_TMHandle  *pMe = pstTMHandle;
   if(NULL == pMe)
   {
      return EME_FAILED;
   }
   
   do 
   {
      /*Init list head*/
      EME_INIT_LIST_HEAD(&pMe->stSignalQueueFirst);
      EME_INIT_LIST_HEAD(&pMe->stSignalFreeList);
      pMe->iSignalFreeListLength = 0;
      
   } while(0);
   
   return EME_SUCCESS;
}

/*============================================================================ 
 *    FUNCTION        EME_SeekExistingSignal()
 * 
 *    DESCRIPTION     To check the whether have some signal in signal queue;   
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    WE_INT32:   EME_SUCCESS if signal queue have some signals
 *                              EME_FAILED if signal queue is empty   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
WE_INT32 EME_SeekExistingSignal(St_TMHandle* pstTMHandle)
{
   St_TMHandle *pMe = pstTMHandle;

   if(NULL == pMe)
   {
      return EME_FAILED;
   }
   
   if(EME_list_empty(&pMe->stSignalQueueFirst))
   {
      return EME_FAILED;
   }
   
   return EME_SUCCESS;
}

/*============================================================================ 
 *    FUNCTION        EME_SignalTerminate()
 * 
 *    DESCRIPTION     Terminate signal queue contain signal queue and free list    
 *                    when termination happen.
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    WE_INT32:    EME_SUCCESS if terminate signal queue successful
 *                               EME_FAILED if terminate signal queue failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
WE_INT32 EME_SignalTerminate(St_TMHandle* pstTMHandlee)
{
   St_EMEFsmSigData* pstSignal = NULL;
   St_TMHandle *pMe = pstTMHandlee;
   void        *pvMemHdr = NULL;
   //   WE_INT32      iResult = EME_SUCCESS;

   EME_UTILS_LOG_INFO(("EME_SignalTerminate : START!"));
   EME_UTILS_LOG_INFO(("EME_SignalTerminate : START!"));
   if(NULL == pMe)
   {
      return EME_FAILED;
   }
   /************************************************************************/
   /*  delete all signals in signal queue and free list                    */                                                
   /************************************************************************/
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pMe);
   do 
   {
      
      /************************************************************************/
      /*delete signals one by one                                             */                         
      /************************************************************************/
      while(!EME_list_empty( &pMe->stSignalQueueFirst))
      {
         pstSignal = list_entry(pMe->stSignalQueueFirst.next,St_EMEFsmSigData, stSigList);
         EME_list_del(&pstSignal->stSigList);
         
         EME_UTILS_FREEIF(pvMemHdr, pstSignal);
      }
      
      /************************************************************************/
      /* delete all signals in free list                                      */                                 
      /************************************************************************/
      while(!EME_list_empty( &pMe->stSignalFreeList))
      {
         pstSignal = list_entry(pMe->stSignalFreeList.next,St_EMEFsmSigData, stSigList);
         EME_list_del(&pstSignal->stSigList);
         EME_UTILS_FREEIF(pvMemHdr, pstSignal);
      }

   } while(0);
   
   return EME_SUCCESS;
}

/*============================================================================ 
 *    FUNCTION        WeEME_PostEventToEngine()
 * 
 *    DESCRIPTION     This function is used to post signal to engine.    
 *                    Create a new signal and add it to signal queue.
                      then process the signal.
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    St_EMEFsmSigData:  Signal pointer if create new signal successful
 *                                       NULL if create new signal failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
St_EMEFsmSigData *WeEME_PostEventToEngine
(
  void        *pvClientIEME,
  EMEFsmId    uiEMEFsmId,
  WE_UINT32   uiSignalId,
  WE_INT32    iParam,
  WE_ULONG    ulParam1, 
  WE_ULONG    ulParam2,
  void        *pvParam
)
{
   St_EMEFsmSigData  *pstSignal = NULL;
   WE_INT32          iResult = EME_SUCCESS;
   St_TMHandle       *pMe = (St_TMHandle*)pvClientIEME;
   void              *pvMemHdr = NULL;
   
   if(NULL == pMe)
   {
      return NULL;
   }
   
   pvMemHdr = EME_OEMTMLib_GetMemHdr(pMe);
   do 
   {
      /************************************************************************/
      /* get free node and assign it to the new signal                        */                                             
      /************************************************************************/
      if(!EME_list_empty(&pMe->stSignalFreeList) && pMe->iSignalFreeListLength > 0)
      {
         /************************************************************************/
         /*  move the start free node from free list                             */                                       
         /************************************************************************/
         pstSignal = list_entry(pMe->stSignalFreeList.next,St_EMEFsmSigData, stSigList) ;
         EME_list_del(&pstSignal->stSigList);
         pMe->iSignalFreeListLength--;
         EME_UTILS_LOG_INFO(("WeEME_PostEventToEngine, get memory from free list %x, free list length = %d", 
            pstSignal, pMe->iSignalFreeListLength));
      }
      else
      {
         pstSignal = (St_EMEFsmSigData*)EME_UTILS_MALLOC(pvMemHdr, sizeof(St_EMEFsmSigData));
         EME_UTILS_BREAKNULL(
            pstSignal, 
            iResult, 
            EME_NOMEMORY, 
            "WeEME_PostEventToEngine,failed to add signal to queue");
         EME_UTILS_MEMSET(pstSignal, 0, sizeof(St_EMEFsmSigData));
         
         EME_UTILS_LOG_INFO(("WeEME_PostEventToEngine, alloc new memory for signal %x",  pstSignal));
      }
      
      /************************************************************************/
      /* init new signal value                                                */                     
      /************************************************************************/
      pstSignal->uiParamValidate  = EME_EVT_PARAM_VALID_ID;
      pstSignal->pvInstance       = pvClientIEME;
      pstSignal->uiEMEFsmId       = uiEMEFsmId;//FSM in core
      pstSignal->iParam           = iParam;
      pstSignal->ulParam1         = ulParam1;
      pstSignal->ulParam2         = ulParam2;
      pstSignal->pvParam          = pvParam;
      pstSignal->uiSignalId       = uiSignalId;//signal in core
      
      //pstSignal->next = NULL;

      EME_UTILS_LOG_INFO(("WeEME_PostEventToEngine--: FSMId = 0x%08x, Signal = 0x%08x", uiEMEFsmId, uiSignalId));

      /************************************************************************/
      /* add new signal to signal queue                                       */                              
      /************************************************************************/
      EME_list_add_tail(&pstSignal->stSigList, &pMe->stSignalQueueFirst);
      
      
      iResult = OEMTMLib_EmailTriggerDispatcher(pMe);
      
      
   }while(0);
   
   if(EME_SUCCESS != iResult)
   {
      /************************************************************************/
      /*  clean signal data                                                   */                 
      /************************************************************************/
      
   }
   
   return pstSignal;
}

