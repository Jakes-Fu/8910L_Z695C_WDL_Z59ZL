/*=====================================================================================
    FILE NAME : oem_Seccsc.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will gather functions that convert char set.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                 Author            Number                   Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-10-19 Zhanghuanqing     none                        create oem_seccsc.c
    
=====================================================================================*/

#include "we_def.h"
#include "we_scl.h"
#include "we_mem.h"
#include "we_csc.h"

#include "oem_seccsc.h"

/*==================================================================================================
FUNCTION: 
    Sec_Csc
CREATE DATE:
    2006-10-19
AUTHOR:
    Bird Zhang
DESCRIPTION: 
    charset convert
ARGUMENTS PASSED:
    WE_INT32 iFromCharset[IN]:souce charset
    WE_INT32 iToCharset[IN]: dst charset
    WE_CHAR *pcSrc[IN]:source string.
    WE_LONG lSrcLen[IN]:source string length
    WE_CHAR *pcDst[OUT]:dst buffer
    WE_LONG *plDstLen[OUT]:dst length

RETURN VALUE:
    TRUE  : sucess
    FALSE : fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_BOOL
Sec_Csc(void* pIShell,
      WE_INT32 iFromCharset,
      WE_INT32 iToCharset,
      WE_CHAR* pcSrc,
      WE_LONG lSrcLen,
      WE_CHAR* pcDst,
      WE_LONG* plDstLen)
{
    Fn_We_Charset_Convert *pFn = NULL;     
    HCSC hCSC = NULL;
    E_WECSCERR eRes = E_WECSC_FAILED;

    eRes = WE_CSC_Create(&hCSC);
    if (eRes != E_WECSC_SUCCESS)
    {
        return FALSE;
    }        
    
    pFn = WE_FINDFUNCTIONOFCSC(pIShell,iFromCharset,iToCharset);
    if(NULL == pFn)
    {
        return FALSE;
    }
    
    if(TRUE != pFn(hCSC, pcSrc,&lSrcLen, pcDst, plDstLen))
    {
        return FALSE;
    }
    WE_CSC_Delete(&hCSC);
    return TRUE;

}

