/*=====================================================================================
    FILE NAME : oem_secmgr.c
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

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include "we_def.h"
#include "we_scl.h"
#include "we_mem.h"
#include "oem_secerr.h"
#include "oem_secfile.h"
#include "oem_secmgr.h"
#include "oem_sechandle.h"

/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/

E_WE_ERROR SecMgr_Initialize
( 
    WE_HANDLE *phWeHandle
)
{
    St_WeHandle *pstWeHandleData = NULL;

    /* Check the parameter first */
    if(NULL==phWeHandle)
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    /* Create memory for WeaHandle */
    pstWeHandleData = (St_WeHandle *)WE_MALLOC(sizeof(St_WeHandle));
    if(NULL==pstWeHandleData)
    {
        return E_WE_ERROR_RESOURCE_LIMIT;
    }
    /* Initialize WeaHandle */
    WE_MEMSET(pstWeHandleData, 0 ,sizeof(St_WeHandle));
    
    /*printf(":::::::::::::::::::::::: 0x%x ::::::::::::::::::::::::::::::::::::::\n", 
    pstWeHandleData->aphWeSocketInfo[0]);*/

    pstWeHandleData->iMagicNum = WE_HANDLE_MAGIC_NUM;
    
    /* init timer */
    
    *phWeHandle = pstWeHandleData;


#ifdef Sec_LOG_WRITE_TO_FILE
    printf("*********start initial dbg*****************\n");
    Sec_InitializeDbg();
    printf("*********end initial dbg*****************\n");
#endif

    return E_WE_OK;
}



E_WE_ERROR SecMgr_Terminate
(
    WE_HANDLE hWeHandle
)
{
    St_WeHandle *pstWeHandleData = (St_WeHandle *)hWeHandle;
    E_WE_ERROR  eResult = E_WE_OK;
    /* Check the parameter first */
    if(NULL==pstWeHandleData)
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM!=pstWeHandleData->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

#ifdef Sec_LOG_WRITE_TO_FILE
    Sec_TerminateDbg();
#endif
    
    /*modify linux by Bird 070313*/
 //   eResult = SecFile_CloseAll(hWeHandle);

    pstWeHandleData->iMagicNum = 0;
    /* Free Data */
    WE_FREE(pstWeHandleData);
    pstWeHandleData = NULL;

    return eResult;
}



E_WE_ERROR SecMgr_RegMsgProcFunc
(
     WE_HANDLE hWeHandle, 
     Fn_WeCallback Fn_cb, 
#ifdef WE_ALG_SEC
     Fn_ISecEventHandle Fn_SecCb,
#endif
     void *pPrivData
)
{
    St_WeHandle *pstWeHandleData = (St_WeHandle *)hWeHandle;

    /* Check the parameter first */
    if(NULL==pstWeHandleData)
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM!=pstWeHandleData->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    /* Register Handle */
    pstWeHandleData->cbWeCallback = Fn_cb;
#ifdef WE_ALG_SEC
    pstWeHandleData->cbSecCallback = Fn_SecCb;
#endif
    pstWeHandleData->pvPrivData = pPrivData;

    return E_WE_OK;
}


