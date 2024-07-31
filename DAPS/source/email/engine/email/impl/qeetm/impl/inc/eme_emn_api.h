/*=====================================================================================
FILE NAME   : eme_emn_api.h
MODULE NAME : Email engine

  GENERAL DESCRIPTION
    supply api for Decoding the EMN body for push email.
    
  TechFaith Software Confidential Proprietary
  Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
  =======================================================================================
  Revision History
  
  Modification                   Tracking
  Date              Author       Number       Description of changes
  ----------   --------------   ---------   --------------------------------------
  2007-04-20        Hu bin                      create eme_emn_api.h
  2007-04-25        Wang fengtu                 modify eme_emn_api.h    
=====================================================================================*/

#ifndef EME_EMN_API_H
#define EME_EMN_API_H

#include "we_def.h"
#include "eme_utils_api.h"
#include "eme_error.h"

typedef WE_VOID* IEMNHandle;

#define EME_EMN_URITYPE_MAILAT          0x0001
#define EME_EMN_URITYPE_POP             0x0002
#define EME_EMN_URITYPE_IMAP            0x0003
#define EME_EMN_URITYPE_ANY             0x0004

#define EME_EMN_AUTHTYPE_ANY            0x0100
#define EME_EMN_AUTHTYPE_POP_PLAIN      0x0101
#define EME_EMN_AUTHTYPE_POP_APOP       0x0102


typedef struct
{
   WE_INT32        iId;            
   WE_VOID*        pvVal;
}St_EMNOpt;


typedef struct
{
   WE_CHAR* MailBox;
   WE_CHAR* TimeStamp;
   WE_BOOL  bTextMode;
   WE_CHAR  acRes[3];
}St_EmnSemIn;


typedef struct
{
   WE_INT32       iAuthType;
   WE_CHAR*       pcUser;
   WE_CHAR*       pcHost;
   WE_UINT16      usPort;
   WE_CHAR        acRes[2];
}St_EmnServer;


typedef struct
{
   WE_INT32       iURIType;
   WE_UINT32      uiUTCTime;
   WE_CHAR*       pcMailboxName;
   St_EmnServer   stIServer;
}St_EmnRet;

WE_INT32 EME_EMN_InitHandle(IEMNHandle* hEMNHandle, WE_VOID* pvIShell, WE_VOID* pvMemMgr, WE_VOID* pvCSC);


WE_INT32 EME_EMN_DecodeEMN(IEMNHandle hEMNHandle, 
                           WE_VOID* pcBody,
                           WE_INT32 iContentType,
                           WE_INT32 iCharsetType,
                           WE_UINT32 uiDataLen,
                           St_EmnRet** ppRet);


WE_INT32 EME_EMN_DeleteHandle(IEMNHandle* hEMNHandle);


#endif /* EME_EMN_API_H */