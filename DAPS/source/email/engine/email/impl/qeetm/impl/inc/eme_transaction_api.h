/*=====================================================================================
FILE NAME   : eme_transaction_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for transaction.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                 create eme_transaction_api.h
          
=====================================================================================*/
#ifndef _EME_TRANSACTION_API_H
#define _EME_TRANSACTION_API_H

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
#include "we_def.h"
/*Include Program Global head file*/
//TM lib public header file
#include "eme_tm_def.h"
#include "eme_tm_api.h"
/*Include module head file*/



/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
//used to check whether the current transaction node is a valid one
#define EME_EVT_PARAM_VALID_ID    0x1F2F3F4F
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
WE_INT32 EME_TransactionInit(St_TMHandle  *pstHandle);
WE_INT32 EME_TransactionTerminate(St_TMHandle *pstHandle);
WE_INT32 EME_FindPendingTrans(St_TMHandle *pstHandle);
WE_INT32 EME_HandleTransactions(St_TMHandle  *pstHandle);
////delete all transactions in the queue
WE_INT32 EME_TransactionFlush(St_TMHandle *pstHandle, TransID uiTransID);
WE_INT32 EME_DequeueTransaction(St_TMHandle *pstHandle, TransID uiTransID);
WE_INT32 EME_DeleteTransaction(St_TMHandle *pstHandle, St_TransEvt* pstTransData);
WE_INT32 EME_AddNewTransaction(St_TMHandle *pstHandle,St_TransEvt* pstTransData);
St_TransEvt* EME_NextTransEvt(St_TMHandle *pstTMHandle);

/*---------------------------------------------------------------------
 *             Function Documentation
 *---------------------------------------------------------------------*/
/*===========================================================================
 *    FUNCTION        EME_TransactionInit
 * 
 *    DESCRIPTION     EME_TransactionInit
 *                    used to initialize transaction queue..
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle * pHandle
 * 
 *    RETURN VALUE    WE_INT32   common error definition
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_TransactionTerminate
 * 
 *    DESCRIPTION     EME_TransactionTerminate   
 *                    Delete all transaction node in transaction queue,
 *                    both in pending and in progress list.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle* pHandle
 * 
 *    RETURN VALUE    WE_INT32   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_FindPendingTrans
 * 
 *    DESCRIPTION     EME_FindPendingTrans   
 *                    try to find if there are pending trans existed in pending queue.
 *                    return success if there are pending trans else return failed;
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle* pHandle
 * 
 *    RETURN VALUE    WE_INT32   
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_HandleTransactions
 * 
 *    DESCRIPTION     EME_HandleTransactions
 *                    handle a transaction that can be processed. convert the transaction to be handled
 *                    into internal signals.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle * pHandle
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_TransactionFlush
 * 
 *    DESCRIPTION     EME_TransactionFlush   
 *                   used to cancel operations of specified msg id and trans id,
 *                   case trans id equal to TRANS_ALL,all the transactions in the 
 *                   transaction queue will be removed.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle* pHandle
 *    PARAMETERS      MMSETM_MSGID tuiMsgID         the msg which starts the transaction
 *    PARAMETERS      MMSETM_TRANSID uiTransID       the unique trans id for a transaction.
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_DeleteTransaction
 * 
 *    DESCRIPTION     EME_DeleteTransaction   
 *                    the time a transaction finished,we need to call this function to remove it from queue.
 *                    also we will do some clean work here,e.g release IWAP.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle* pvPi
 *    PARAMETERS      St_TransEvt* pstTransData : the transaction node in the queue
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        EME_AddNewTransaction
 * 
 *    DESCRIPTION     EME_AddNewTransaction   
 *                    the time a new transaction is started,we need to put it into trans queue.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      ITMHandle* pHandle
 *    PARAMETERS      St_TransEvt* pstTransData : the transaction node of the new transaction
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/* Remember header files should contain all the information a user
 * would need to use this service */



#endif             /* #ifndef QCT_H */
