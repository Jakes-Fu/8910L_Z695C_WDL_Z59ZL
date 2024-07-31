#ifndef __OEM_PPP_API_H__
#define __OEM_PPP_API_H__

/*==============================================================================
    FILE NAME   : oem_ppp_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        Provide basic error definition for email engine.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2008-05-11       rachel                         create the file 
==============================================================================*/
#include "we_def.h"
typedef enum
{
    E_NET_GPRS_SETTING = -1,
    E_NET_ACTIVATING, //0
    E_NET_ACTIVATED, // 1
    E_NET_DEACTIVATED, // 2
    E_NET_DEACTIVATING,// 3
    E_NET_AUTO_DISC_TIMEOUT,// 4
    E_NET_MAX = 0x7fffffff
}E_WE_NET_STATUS;

typedef enum{
    E_PPP_SETUP_SUCCESS = 0x00000000,
    E_PPP_SETUP_FAIL,
    E_PPP_LINE_ASYNC
}E_PPP_LINK_MSG_TYPE;

typedef WE_VOID(*FnCallBackPn)(WE_VOID *user);

WE_INT32 OEM_DialByProfile(WE_VOID *pvMemMgr, WE_INT32 iProfileId, FnCallBackPn pFn, WE_VOID *pvUser);

WE_VOID  EME_GetDialId(WE_INT32 *pnDialId);
WE_VOID  EME_DisconnectPPP(WE_INT32 nDialId);
WE_VOID  EME_FreePPPData(WE_VOID);


#endif /* __OEM_PPP_API_H__ */

