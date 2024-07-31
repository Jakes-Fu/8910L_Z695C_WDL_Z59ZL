/*=====================================================================================
FILE NAME   : eme_tm_def.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide definition of transaction manager.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                 create eme_tm_def.h
          
=====================================================================================*/
#ifndef _EME_TM_DEF_H
#define _EME_TM_DEF_H

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/

/*Include Program Global head file*/
#include "we_def.h"
#include "eme_tm_api.h"
#include "eme_list_api.h"
/*Include module head file*/



/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
/************************************************************************/
/*definitions for internal signals                                      */
/************************************************************************/

/*Define Constant Macro end*/
#define EME_MAX_TRANSACTION_FREE_LIST_LENGTH 0
#define EME_MAX_SIG_FREE_LIST_LENGHT    0
#define EME_MAX_TRANSACTION_IN_PROGRESS 1


/*********************** Cancel Protect ****************************/
#define EME_CANCEL_ENABLED             0x00000000
#define EME_CANCEL_DISABLED            0x00000001

/*********************** Protocol state ****************************/
#define EME_STACK_STATE_LOGIN          0x00000001
#define EME_STACK_STATE_LOGOUT         0x00000002


/*********************** fsm define ********************************/
#define EME_FSM_SMTP                   0x00000001
#define EME_FSM_POP                    0x00000002
#define EME_FSM_IMAP                   0x00000004
#define EME_FSM_ASYNC                  0x00000008

/*********************** signals define ******************************/
#define EME_SIG_NULL                   0x00000000
#define EME_SIG_START                  0x00010000
/* signals for SMTP */
#define EME_SIG_SMTP_START             (EME_SIG_START + 0x00000100)

#define EME_SIG_SMTP_TRANS_START       (EME_SIG_SMTP_START + 0x01)
#define EME_SIG_SMTP_OPEN              (EME_SIG_SMTP_START + 0x02)
#define EME_SIG_SMTP_SEND              (EME_SIG_SMTP_START + 0x03)
#define EME_SIG_SMTP_SEND_RECEIPT      (EME_SIG_SMTP_START + 0x04)
#define EME_SIG_SMTP_FORWARD           (EME_SIG_SMTP_START + 0x05)
#define EME_SIG_SMTP_CLOSE             (EME_SIG_SMTP_START + 0x06)
#define EME_SIG_SMTP_TRANS_END         (EME_SIG_SMTP_START + 0x07)

#define EME_SIG_SMTP_CANCEL            (EME_SIG_SMTP_START + 0x08)


/* signals for POP */
#define EME_SIG_POP_START              (EME_SIG_START + 0x00000200)

#define EME_SIG_POP_TRANS_START        (EME_SIG_POP_START + 0x01)
#define EME_SIG_POP_CONNECTION         (EME_SIG_POP_START + 0x02)
#define EME_SIG_POP_LOGIN              (EME_SIG_POP_START + 0x03)
#define EME_SIG_POP_GET_MAILINFO       (EME_SIG_POP_START + 0x04)
#define EME_SIG_POP_FETCH_NEWMAILS     (EME_SIG_POP_START + 0x05)
#define EME_SIG_POP_FETCH_SPECMAIL     (EME_SIG_POP_START + 0x06)
#define EME_SIG_POP_FETCH_NEWHDRS      (EME_SIG_POP_START + 0x07)
#define EME_SIG_POP_FETCH_SPECHDR      (EME_SIG_POP_START + 0x08)
#define EME_SIG_POP_DELETE_ALLMAIL     (EME_SIG_POP_START + 0x09)
#define EME_SIG_POP_DELETE_SPECMAIL    (EME_SIG_POP_START + 0x0A)
#define EME_SIG_POP_UNKEEP_MAIL        (EME_SIG_POP_START + 0x0B)
#define EME_SIG_POP_QUIT               (EME_SIG_POP_START + 0x0C)
#define EME_SIG_POP_TRANS_END          (EME_SIG_POP_START + 0x0D)
                                       
#define EME_SIG_POP_CANCEL             (EME_SIG_POP_START + 0x0E)

/*signals for EPOP*/
#define EME_SIG_EPOP_FETCH_NEWMAILS	   (EME_SIG_POP_START + 0x0F)
#define EME_SIG_EPOP_FETCH_NEWHDRS	   (EME_SIG_POP_START + 0x10)
#define EME_SIG_EPOP_FETCH_SPECMAIL	   (EME_SIG_POP_START + 0x11)
#define EME_SIG_EPOP_FETCH_SPECPART	   (EME_SIG_POP_START + 0x12)
#define EME_SIG_EPOP_FETCH_PROF		   (EME_SIG_POP_START + 0x13)

/* signals for IMAP */
#define EME_SIG_IMAP_START             (EME_SIG_START + 0x00000400)

#define EME_SIG_IMAP_TRANS_START       (EME_SIG_IMAP_START + 0x01)
#define EME_SIG_IMAP_OPEN              (EME_SIG_IMAP_START + 0x02)
#define EME_SIG_IMAP_SELECT_MAILBOX    (EME_SIG_IMAP_START + 0x03)
#define EME_SIG_IMAP_GET_NEWMAILINFO   (EME_SIG_IMAP_START + 0x04)
#define EME_SIG_IMAP_FETCH_NEWMAILS    (EME_SIG_IMAP_START + 0x05)
#define EME_SIG_IMAP_FETCH_NEWHDRS     (EME_SIG_IMAP_START + 0x06)
#define EME_SIG_IMAP_FETCH_SPEC_MAIL   (EME_SIG_IMAP_START + 0x07)
#define EME_SIG_IMAP_FETCH_SPEC_PART   (EME_SIG_IMAP_START + 0x08)
#define EME_SIG_IMAP_HANDLE_MAIL       (EME_SIG_IMAP_START + 0x09)
#define EME_SIG_IMAP_HANDLE_MAILBOX    (EME_SIG_IMAP_START + 0x0A)
#define EME_SIG_IMAP_CLOSE             (EME_SIG_IMAP_START + 0x0B)
#define EME_SIG_IMAP_TRANS_END         (EME_SIG_IMAP_START + 0x0C)

#define EME_SIG_IMAP_CANCEL            (EME_SIG_IMAP_START + 0x0D)


/* signals for Inner async */
#define EME_SIG_ASYNC_START            (EME_SIG_START + 0x00000800)

#define EME_SIG_ASYNC_RESUME           (EME_SIG_ASYNC_START + 0x01)



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
typedef struct St_TMHandle
{
   /************************************************************************/
   /* signal queue definitions go here                                     */                               
   /************************************************************************/
   stListHead stSignalQueueFirst;
   stListHead stSignalFreeList;
   WE_INT32 iSignalFreeListLength;

   /************************************************************************/
   /* transaction list include(pending list, in progress list and free list)*/                                                                   
   /************************************************************************/
   stListHead stTransQueueFirst;

   //current trans in progress
   WE_INT32 iTransQueueLength;
   
   //pending queue
   stListHead stTransPendingQueueFirst;
   
   // free list 
   stListHead stTransFreeList;
   WE_INT32 iTransFreeListLength;

   WE_INT32 iCanBeCancel;
   
   EMEOpt   *pstAccountOpts;
 
   /* for smtp fsm */
   WE_VOID  *pvSMPT;
   WE_INT32 iSMPTState;

   /* for imap fsm */
   WE_VOID  *pvIMAP;
   WE_INT32 iIMAPState;

   /* for pop fsm */
   WE_VOID  *pvPOP;
   WE_INT32 iPOPState;

   /* for emn */
   WE_VOID  *pvEMN;
   WE_VOID  *pvAsyncMgr;

   WE_VOID  *pvCbkUserData;

   // pointer to global struct outside
   void* pvParent;

   /*response detail*/
   WE_CHAR    acRespDesc[512];

   WE_CHAR*    pcFilter;
}St_TMHandle;


// (4) notification opts in p
typedef void (*PFNFREESIGDATA)(St_TMHandle *pstTMHandle, void* pvTransEvt);
// trans event structure used as node element of transaction list
typedef struct
{
   stListHead     stTransList; // indicate that St_TransEvt is a node
   
   EMEOpt*        pstTransOpts;
   EMEOpt*        pstAccountOpts;

   TransID        uiTransID;// unique indication for a  transaction.
   WE_UINT32      uiTransType;
   WE_UINT32      uiTransState;  //Transaction state
          
   //only for temp use
   WE_CHAR*       pcUIDL;
   //we need to know in handling which signal the error occurs
   WE_INT32       iCurrSignalID;
   //the error occurs while handling signals.
   WE_INT32       iErrCode;
   //used to check whether the current transaction node is a valid one.
   WE_UINT32      uiParamValidate;
   
   //PFNFREESIGDATA to delete memory in p data, case Dequeued
   PFNFREESIGDATA pfnFreeSigData;

   //WE_VOID*       pvProtocol;

   //used to transmit memory related objects between signals. 
   WE_VOID*       pvTransData;
   
   WE_UINT32      uiTickCount;
   WE_BOOL        bFirstPackage;
   WE_BOOL        bLastPackage;

}St_TransEvt;
/*Define struct &  union type end*/

/*---------------------------------------------------------------------
 *             Function Declarations
 * ------------------------------------------------------------------*/

/*---------------------------------------------------------------------
 *             Function Documentation
 * --------------------------------------------------------------------*/
/*===========================================================================
 *    FUNCTION        Mms_GetOpt
 * 
 *    DESCRIPTION     Mms_GetOpt
 *                      Find the proper config opt or mms opt from set of opts.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      MmsOpt * pOpts in       config opts or mms opts 
 *    PARAMETERS      WE_UINT32 uiOptID
 * 
 *    RETURN VALUE    MmsOpt*     indicated config opt or mms opt
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        Mms_CreateNotifyRespBuffer
 * 
 *    DESCRIPTION     Mms_CreateNotifyRespBuffer   
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      IMSGHandle hMsgHandle
 *    PARAMETERS      MmsOpt* pNotificationOpts
 *    PARAMETERS      MMSOPTVAL_MMS_STATUS uiStatus
 *    PARAMETERS      MMSOPTVAL_MMS_VER uiVersion
 *    PARAMETERS      WE_UCHAR** ppucBuffer
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        Mms_CreateAcknowledgeIndBuffer
 * 
 *    DESCRIPTION     Mms_CreateAcknowledgeIndBuffer   
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      IMSGHandle hMsgHandle
 *    PARAMETERS      MmsOpt* pNotificationOpts
 *    PARAMETERS      MMSOPTVAL_MMS_VER uiVersion
 *    PARAMETERS      WE_UCHAR** ppucBuffer
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/
/*===========================================================================
 *    FUNCTION        Mms_CreateReadReport
 * 
 *    DESCRIPTION     Mms_CreateReadReport   
 *                       create v1.0 or v1.1 read report according to version.
 * 
 *    DEPENDENCIES      
 * 
 *    PARAMETERS      IMSGHandle hMsgHandle
 *    PARAMETERS      MmsOpt* pRetrieveConfOpts
 *    PARAMETERS      MMSOPTVAL_MMS_VER uiVersion
 *    PARAMETERS      WE_UCHAR** ppucBuffer
 *    PARAMETERS      WE_UINT* puiSize
 * 
 *    RETURN VALUE    WE_INT32 AEEQMEError.h  
 *     
 *    SIDE EFFECTS    None   
 * 
 *===========================================================================*/

/* Remember header files should contain all the information a user
 * would need to use this service */


#endif             /* #ifndef _EME_TM_DEF_H */
