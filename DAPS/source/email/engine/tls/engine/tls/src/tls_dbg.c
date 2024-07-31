/*==================================================================================================
    FILE NAME   : we_dbg.c
    MODULE NAME : WEA
    
    GENERAL DESCRIPTION
        This file implements log management.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
    Modification Tracking
    Date               Author               Number      Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-11-07         Jack.Ru              none         Initialization
    
==================================================================================================*/


/*==================================================================================================
    Include Files
==================================================================================================*/
#include "we_def.h"
#include "tls_dbg.h"

#ifdef WE_DEBUG_INFO
/*==================================================================================================
* Macro Define Section
==================================================================================================*/
/*==================================================================================================
    FUNCTION: 
            WapLogPrintf
    DESCRIPTION:
            Manage Log ouput
    RETURN VALUE:
            None
    REMARK:
==================================================================================================*/
void WapLogPrintfNone(const WE_CHAR *format,...)
{
    return;
}

WE_INT32 WapIsPrintLog(WE_INT32 iModule,WE_INT32 iLevel)
{
//    if( (iModule&WAP_DBG_ON)&&(iLevel>=WAP_DBG_LEVEL) )
        return 1;
 //   else
  //      return 0;
}

#endif


