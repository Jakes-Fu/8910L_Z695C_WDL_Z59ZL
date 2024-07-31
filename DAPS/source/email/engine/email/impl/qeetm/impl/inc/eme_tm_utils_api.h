/*=====================================================================================
FILE NAME   : eme_tm_utils_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for tm utils.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                 create eme_tm_utils_api.h
          
=====================================================================================*/
#ifndef _EME_TM_UTILS_API_H
#define _EME_TM_UTILS_API_H

/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
#include "we_def.h"
/*Include Program Global head file*/
#include "eme_tm_api.h"
#include "eme_tm_def.h"
/*Include module head file*/


/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
#define EME_STORE_INIT_OPT_NUM         4
#define EME_CODEC_DECODE_OPT_NUM       8

#define EME_IMAP_OPEN_OPT_NUM          10

#define EME_MAX_UINT32_STR_LEN         11

#define EME_IMAP_HANDLMAIL_OPT_NUM     5

#define EME_SMTP_SEND_RECEIPT_OPT_NUM  7

#define EME_IMAP_UPDATE_PART_NUM       10

#define EME_POP_UPDATE_PART_NUM        10

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
EMEOpt* EME_CopyAllOpts(WE_VOID* pvMemHdr, EMEOpt *pstOrigOpts);
WE_INT32 EME_GetStrOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_CHAR** ppcVal);
WE_INT32 EME_GetIntOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_INT32* piVal);
WE_INT32 EME_ChangeIntOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_INT32 iVal);
WE_INT32 EME_ChangeStrOptValue(EMEOpt *pstOrigOpts, WE_INT32 iOptId, WE_CHAR* pcVal);
WE_INT32 EME_FreeAllOptValue(WE_VOID* pvMemHdr, EMEOpt *pstFreeOpts);
WE_INT32 EME_FreeTransEvt(WE_VOID* pvMemHdr, St_TransEvt *pstFreeTransEvt);
WE_BOOL EME_SMTPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt);
WE_BOOL EME_POPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt);
WE_BOOL EME_IMAPCloseNeed(St_TMHandle *pstTmHandle, St_TransEvt *pstOldTransEvt);
WE_INT32 EME_StackClose(St_TMHandle *pstTmHandle, WE_UINT32 uiTransType);


#endif             /* #ifndef _EME_TM_UTILS_API_H */
