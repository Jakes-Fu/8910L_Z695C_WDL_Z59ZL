/*=====================================================================================
FILE NAME   : oem_tmlib_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Supply api of transaction manager lib for transaction manager use.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create oem_tmlib_api.h
          
=====================================================================================*/
#ifndef _OEM_TMLIB_API_H
#define _OEM_TMLIB_API_H

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
/*Include Program Global head file*/
#include "we_def.h"
/*Include module head file*/
#include "eme_tm_def.h"


/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
#define EME_NETWORK_DEFAULT   0x00000000
/*Define Constant Macro end*/

#define EME_TM_STATE_FREE 1
#define EME_TM_STATE_BUSY 2
typedef WE_UINT EME_TM_STATE;
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
WE_INT32 OEMTMLib_EmailTriggerDispatcher(St_TMHandle* pstTmHandle);
WE_VOID* EME_OEMTMLib_GetMemHdr(St_TMHandle *pstTmHandle);
WE_VOID* EME_OEMTMLib_GetIFileMgr(St_TMHandle *pstTmHandle);
WE_VOID* OEMTMLib_GetINetMgr(St_TMHandle *pstTmHandle);
WE_VOID* EME_OEMTMLib_GetIShell(St_TMHandle *pstTmHandle);
WE_VOID* OEMTMLib_GetHCSC(St_TMHandle *pstTmHandle);
//WE_INT32 OEMTMLib_SeletNetWork(St_TMHandle *pstTmHandle, WE_INT32 iPDP);

WE_INT32 OEMTMLib_NotifySize(St_TMHandle *pstTMHandle, 
                             St_TransEvt* pstUserData, 
                             WE_UINT32 uiTransferSize, 
                             WE_UINT32 uiTotalSize);
WE_INT32 OEMTMLib_NotifyNumber(St_TMHandle *pstTMHandle, 
                             St_TransEvt* pstUserData, 
                             WE_UINT32 uiCurrMsg, 
                             WE_UINT32 uiTotalNum);
WE_INT32 OEMTMLib_NotifyNewMail(St_TMHandle *pstTMHandle,
                             St_TransEvt* pstUserData,
                             WE_CHAR *pcMsgId,
                             WE_CHAR *pcUidl);
WE_INT32 OEMTMLib_NotifyMailbox(St_TMHandle *pstTMHandle,
                                St_TransEvt* pstUserData,
                                WE_CHAR *pcMailboxName, 
                                WE_CHAR *pcDelimiter);
WE_INT32 OEMTMLib_TransReply(St_TMHandle *pstTMHandle, 
                             WE_UINT32 uiTransID,
                             WE_UINT32 uiTransType,
                             WE_UINT32 uiMsgId,
                             WE_UINT32 uiClsId,
                             WE_INT32 iResult,
                             WE_CHAR  *pcRespDesc);
WE_INT32 OEMTMLib_NotifyInfo(St_TMHandle *pstTMHandle, 
                             St_TransEvt* pstUserData, 
                             WE_CHAR *pcInfo);

WE_VOID OEMTMLib_SetEmlTMState(St_TMHandle *pstTMHandle, EME_TM_STATE tuiState);
EME_TM_STATE OEMTMLib_GetEmlTMState(St_TMHandle *pstTMHandle);
/*---------------------------------------------------------------------
 *             Function Documentation
 * --------------------------------------------------------------------*/
/*===========================================================================
 *    FUNCTION        OEM_TM_Lib_HandleIContentOpts
 * 
 *    DESCRIPTION     OEM_TM_Lib_HandleIContentOpts   
 *                   Handle body part opt contain IContent , replace the IContent opt with 
 *                   actual data buffer opt and then return msg handle through registered tm lib callback.
 *
 *                   handle body part opt contain large data buffer, replace the opt with IContent or IFile opt.
 *                    return back current body part num and IContent opts.
 *
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle tmHandle(in): MMSE tm lib internal structure
 *    PARAMETERS      PFNNOTIFY pfnCallback :  used for tm lib to register  a callback for 
 *                                             returning IContent handling result.
 *    PARAMETERS      MmsOpt* pContentOpts(in): IContent body part data opts get from database
 *    PARAMETERS      St_TransEvt* pstEvt(in): p data pass all through the transaction
 *                      
 * 
 *    RETURN VALUE    int   : AEEQMEError.h
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        OEM_TMLib_GetNotificationOpts
 * 
 *    DESCRIPTION     OEM_TMLib_GetNotificationOpts   
 *                   Get actual notification.ind opts according to TRX_SRC_DATA_TYPE
 *                   case ISQL related, we only need to use StoreLib to get opts from storage and 
 *                   pass the copy of these opts back to the TMLib.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle tmHandle :pointer to MMSE tm lib internal structure
 *    PARAMETERS      MmsOpt** ppOpts(out):    notification.ind opts ,not related to storage handle.
 *    PARAMETERS      St_TransEvt* pstEvt:p data pass all through the transaction
 * 
 *    RETURN VALUE    int   :AEEQMEError.h
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        OEM_TMLib_HandleRetrievedMsg
 * 
 *    DESCRIPTION     OEM_TMLib_HandleRetrievedMsg   
 *                    handle retrieved msg handle according to TRX_SRC_DATA_TYPE.
 *                    case ISQL related, we begin store process by call inside signals.
 *                    MMS_SIG_STORE_MSG_BEGIN..
 * 
 *                    
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle tmHandle : pointer to MMSE tm lib internal structure
 *    PARAMETERS      PFNNOTIFY pfnCallback :
 *                          used to notify tm lib according to different TRX_SRC_DATA_TYPE,
 *                          whatever TRX_SRC_DATA_TYPE, you just need to notify TM lib  through
 *                          pfnCallback with parameter Msg handle.
 *    PARAMETERS      IMsgHandle msgHandle : retrieve.conf msg handle after parsed .
 *    PARAMETERS      void* pstEvt::p data pass all through the transaction
 * 
 *    RETURN VALUE   int   :AEEQMEError.h
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
 /*===========================================================================
 *    FUNCTION        OEMTMLib_TriggerDispatcher
 * 
 *    DESCRIPTION     OEMTMLib_TriggerDispatcher
 *                    used to add internal signals and transactions into BREW callback queue.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle tmHandle
 * 
 *    RETURN VALUE    int AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        OEMTMLib_GetFSMgr
 * 
 *    DESCRIPTION     OEMTMLib_GetFSMgr   
 *                    get ISQL pointer from QMETM struct.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle tmHandle
 * 
 *    RETURN VALUE    int AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/* Remember header files should contain all the information a user
 * would need to use this service */



#endif             /* #ifndef _OEM_TMLIB_API_H */
