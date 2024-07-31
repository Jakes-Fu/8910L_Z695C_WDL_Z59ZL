/*=====================================================================================
    FILE NAME : oem_seccsc.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will gather the prototypes of functions that convert char set.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                  Author           Number                     Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-10-19  Zhanghuanqing                                     create oem_seccsc.h
    
=====================================================================================*/

#ifndef _OEM_SECCSC_H
#define _OEM_SECCSC_H

#include "we_csc.h"
/*modify linux by Bird 070313*/
WE_BOOL
Sec_Csc(void* pIShell,
        WE_INT32 iFromCharset,
        WE_INT32 iToCharset,
        WE_CHAR* pcSrc,
        WE_LONG lSrcLen,
        WE_CHAR* pcDst,
        WE_LONG* plDstLen);
#endif /*_OEM_SECCSC_H*/

