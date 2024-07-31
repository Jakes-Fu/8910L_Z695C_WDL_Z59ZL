/*=====================================================================================
    FILE NAME : oem_secmgr.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:    
        In this file, define the WEA management functions.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                   Author           Number               Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-06-01         Alan                                        create oem_secmgr.c
    
=====================================================================================*/
#ifndef __WE_WE_MGR__
#define __WE_WE_MGR__

#include "we_cfg.h"
#include "we_def.h"
#include "oem_secerr.h"
#include "oem_secevent.h"

#ifdef WE_ALG_SEC
#include "sec.h"
#endif

typedef void (*Fn_WeCallback)
(
    E_WE_EVENT weNotifyType, 
    void* pCbData, 
    WE_HANDLE hWeSubHandle
);  /* Fn_WeCallback prototype */


E_WE_ERROR SecMgr_Initialize
(
     WE_HANDLE *phWeHandle
);

E_WE_ERROR SecMgr_Terminate
(
    WE_HANDLE hWeHandle
);

E_WE_ERROR SecMgr_RegMsgProcFunc
(
     WE_HANDLE hWeHandle, 
     Fn_WeCallback Fn_cb, 
#ifdef WE_ALG_SEC
     Fn_ISecEventHandle Fn_SecCb,
#endif
     void *pPrivData
);




#endif

