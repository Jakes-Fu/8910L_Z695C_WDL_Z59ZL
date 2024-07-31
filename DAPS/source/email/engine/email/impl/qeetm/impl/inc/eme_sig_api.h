/*=====================================================================================
FILE NAME   : eme_sig_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for managing sigs.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_sig_api.h
          
=====================================================================================*/

#ifndef _EME_TM_SIG_H
#define _EME_TM_SIG_H

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
// list implementation common definitions
#include "eme_tm_api.h"
#include "we_def.h"
#include "eme_list_api.h"
#include "eme_tm_def.h"
//#include "eme_tm_api.h"


/*==================================================================================================
* Macro Define Section
==================================================================================================*/
/*Define Constant Macro start*/
#define EME_INVALID_SIGNAL_ID          0x7fffffff

#define EME_EVT_PARAM_VALID_ID         0x1F2F3F4F

#define EME_MAX_FREE_SIGNAL_LIST_LENGTH 0 
// #define EM_EVT_COMMON_BASE               EVT_USER + 0x7fffff
// #define EM_EVT_COMMON_TIMER_EXPIRED      (EM_EVT_COMMON_BASE + 0x01)
// 
#define EME_INVALID_FSM  0x00000000
#define EME_FSM_END      0xffffffff
typedef WE_UINT32 EMEFsmId;

/*Define Constant Macro end*/

/*Define Macro Function start*/
#define EME_EVT_SENDTO_SELF(pIMms,d, sig) \
WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (WE_ULONG)0L, (WE_ULONG)0L, NULL)

#define EME_EVT_SENDTO_SELF_I(pIMms,d, sig, i) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (WE_ULONG)0L, (WE_ULONG)0L, NULL)

#define EME_EVT_SENDTO_SELF_IU(pIMms,d, sig, i, u) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (u), 0L, NULL)

#define EME_EVT_SENDTO_SELF_IUU(pIMms,d, sig, i, u1, u2) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (u1), (u2), NULL)

#define EME_EVT_SENDTO_SELF_IUP(pIMms,d, sig, i, u, p) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (u), (WE_ULONG)0L, (p))

#define EME_EVT_SENDTO_SELF_IUUP(pIMms,d, sig, i, u1, u2, p) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (u1), (u2), (p))

#define EME_EVT_SENDTO_SELF_IP(pIMms,d, sig, i, p) \
WeEME_PostEventToEngine((pIMms),(d), (sig), (i), (WE_ULONG)0L, (WE_ULONG)0L, (p))

#define EME_EVT_SENDTO_SELF_U(pIMms,d, sig, u) \
WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (u), (WE_ULONG)0L, NULL)

#define EME_EVT_SENDTO_SELF_UU(pIMms,d, sig, u1, u2) \
WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (u1), (u2), NULL)

#define EME_EVT_SENDTO_SELF_UUP(pIMms,d, sig, u1, u2, p) \
WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (u1), (u2), (p))

#define EME_EVT_SENDTO_SELF_UP(pIMms,d, sig, u, p) \
WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (u), 0L, (p))

#define EME_EVT_SENDTO_SELF_P(pIMms,d, sig, p) \
    WeEME_PostEventToEngine((pIMms),(d), (sig), 0L, (WE_ULONG)0L, (WE_ULONG)0L, (p))



typedef struct tagSt_EMEFsmSigData
{
  stListHead        stSigList;

  EMEFsmId          uiEMEFsmId;       /*EME FsmExt Handler*/

  WE_INT32          iParam;          /*subEvent long type Param*/
  WE_ULONG          ulParam1;        /*subEvent unsigned long type Param1*/
  WE_ULONG          ulParam2;        /*subEvent unsigned long type Param2*/
  WE_UINT32         uiParamValidate; /* == EME_EVT_PARAM_VALID_ID*/
  void              *pvParam;        /*subEvent void* type Param */
  /*suggest it is pointer to Extension Instance*/
  /*Before Event,the instance must be AddRef*/   
  void              *pvInstance;     /*TmHandle instance pointer */
  /*Can support one client creating multi-eme*/
  /*in send event function,it must be Added Ref*/
  
  WE_UINT32       uiSignalId;

} St_EMEFsmSigData,*P_St_EMEFsmSigData;
/*Define struct &  union type end*/

/*---------------------------------------------------------------------
 *             Function Declarations
 * ------------------------------------------------------------------*/
WE_INT32 EME_SignalProcess(St_TMHandle* pstHandle);
WE_INT32 EME_SignalFlush(St_TMHandle* pstHandle, St_EMEFsmSigData* pstSig);
WE_INT32 EME_SignalDelete(St_TMHandle* pstHandle, St_EMEFsmSigData* pstSig);
WE_INT32 EME_SignalInit(St_TMHandle* pstHandle);
WE_INT32 EME_SeekExistingSignal(St_TMHandle* pstHandle);
WE_INT32 EME_SignalTerminate(St_TMHandle* pstHandle);

St_EMEFsmSigData *WeEME_PostEventToEngine
(
  void        *pvClientIMms,
  EMEFsmId    uiMmsFsmId,
  WE_UINT32   uiSignalId,
  WE_INT32    iParam,
  WE_ULONG    ulParam1, 
  WE_ULONG    ulParam2,
  void        *pvParam
);

/*=============================================================================
Function: EME_SignalProcess

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SignalProcess(ITMHandle* pHandle);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: EME_SignalFlush

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SignalFlush(ITMHandle pHandle, St_MmsFsmSigData* pstSig);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: EME_SignalDelete

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SignalDelete(ITMHandle* pHandle, St_MmsFsmSigData* pstSig);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: EME_SignalInit

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SignalInit(ITMHandle* pHandle);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: EME_SeekExistingSignal

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SeekExistingSignal(ITMHandle* pHandle);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: EME_SignalTerminate

Description:

    Process sig in signal queue.

Prototype:

   WE_INT32 EME_SignalTerminate(ITMHandle* pHandle);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/

/*=============================================================================
Function: WeEME_PostEventToEngine

Description:

    Process sig in signal queue.

Prototype:

   St_MmsFsmSigData *WeEME_PostEventToEngine
(
  void       *pvClientIMms,
   WE_UINT16   usEventId,
  WE_INT32    iParam,
  WE_ULONG    ulParam1, 
  WE_ULONG    ulParam2,
  void        *pvParam
);
                                         
Parameters:
  
    hTMHandle              [in]: The pointer to the MMSTM handle.
    
Return Value:

    General Error codes
  
Comments:

See Also:

=============================================================================*/


#endif /*End define _EME_TM_SIG_H*/