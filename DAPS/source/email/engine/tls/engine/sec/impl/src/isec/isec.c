/*=====================================================================================
    FILE NAME : sec.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:       
        In this file,define the commons interface function prototype of sec module.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author         Number                  Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-07              Kevin Yang        None         Draft
    2007-05-17              tang                           Delete cert manage and signtext
    2007-06-28              bluefire                       after malloc,memset it
=====================================================================================*/

/*=====================================================================================
*   Include File Section
*=====================================================================================*/
#include "sec_comm.h" 
#include "oem_secmgr.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define SEC_PRIVATE_DATA            (((ISec*)pMe)->hPrivateData)
#define SEC_SIINFO                  (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstInfo)
#define SEC_PEER_CACHE              (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstPeerCache)
#define SEC_SESSION_CACHE           (((St_PublicData *)(((ISec*)pMe)->hPrivateData))->pstSessionCache)
#define SEC_WIM_HAVELOAD        (((St_PublicData *)(((ISec *)pMe)->hPrivateData))->pcHaveLoad)
#define SEC_WIM_CACERT             (((St_PublicData *)(((ISec *)pMe)->hPrivateData))->hCACert)

/*******************************************************************************
*   Global Variable Declare Section
*******************************************************************************/
/*global data move from sec_mod.h*/
St_SecInfo               stInfo = {0};
St_SecSessionRec         astSessionCache[M_SEC_SESSION_CACHE_SIZE] = {{{0}}};    
St_SecPeerRec            astPeerCache[M_SEC_PEER_CACHE_SIZE] = {{0}}; 
/* add for user_priv file load number */
WE_CHAR                  cHaveLoad = 0;

/*==================================================================================================
FUNCTION: 
    ISec_RegClientEventCB
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    register callback function.
ARGUMENTS PASSED:
    ISec *      pMe[IN]: pointer about an instance of ISec   
    WE_HANDLE   hPrivData[IN]:Private data handle used in callback function.
    Fn_ISecEventHandle  pcbSecEvtFunc[IN]:pointer about The callback function .    
RETURN VALUE:
    referrence counter
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISec_RegClientEventCB
(
    ISEC *pISec,
    WE_HANDLE hPrivData,
    Fn_ISecEventHandle pcbSecEvtFunc
)    
{
    ISec *pITmpSec = (ISec *)pISec;
    
    if ((NULL == pISec) || (NULL == hPrivData) || (NULL == pcbSecEvtFunc))
    {
        return G_SEC_INVALID_PARAMETER;
    }
    pITmpSec->hcbSecEvtFunc = (WE_HANDLE)pcbSecEvtFunc;
    pITmpSec->hSecPrivDataCB = hPrivData;
    return G_SEC_OK;
    
}

/*==================================================================================================
FUNCTION: 
    ISec_Release
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    sec release
ARGUMENTS PASSED:
    WE_HANDLE pMe[IN]: pointer about an instance of ISec 
RETURN VALUE:
    referrence counter
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 ISec_Release (WE_HANDLE pMe)
{
   WE_UINT32 uiRes = 0;
   
   if (NULL == pMe)
   {
      return G_SEC_INVALID_PARAMETER;
   }

   Sec_DpResetVariable(pMe);   
   
   uiRes += Sec_StopEx(pMe);
   if (NULL != (St_PublicData*)(((ISec*)pMe)->hPrivateData))
   {
     uiRes += (WE_UINT32)SecMgr_Terminate((((St_PublicData*)(((ISec*)pMe)->hPrivateData))->hWeHandle));   
   }
   if (SEC_PRIVATE_DATA != NULL)
   {
       WE_FREE(SEC_PRIVATE_DATA);
       SEC_PRIVATE_DATA = NULL;
   }  
   /*Free the object itself*/
   WE_FREE(pMe);

   return uiRes;
}

/*==================================================================================================
FUNCTION:     
    ISec_Create    
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    create one instance of ISec interface.
ARGUMENTS PASSED:
    WE_UINT16         sSize[IN]:size of ISec
    IShell *        pIShell[IN]:Pointer to the IShell interface.
    IModule*        pIModule[IN]:Pointer to the module.
    WE_HANDLE*        ppMod[IN]:Pointer to mod.
RETURN VALUE:
    G_SEC_NOTENOUGH_MEMORY,
    G_SEC_GENERAL_ERROR,
    G_SEC_OK
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 ISec_Create( WE_INT32 iType, WE_HANDLE* phHandle)
{
   ISec* pMe = NULL;
#ifndef M_SEC_CFG_NO_PLATFORM 
   WE_UCHAR        *pucData = NULL; 
   WE_INT32          iDataLen = 0;
   WE_INT32          iRes = 0;
#endif

   if( !phHandle )
   {
      return G_SEC_INVALID_PARAMETER;
   }
   
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,"ISec_Create begin\n"));

   *phHandle = NULL;
   if((pMe = (ISec*)WE_MALLOC((WE_ULONG)(sizeof(ISec)))) == NULL )
   {
      return G_SEC_NOTENOUGH_MEMORY;
   }
   (void)WE_MEMSET(pMe,0,sizeof(ISec));
   /*malloc memory for global variable*/
   if((pMe->hPrivateData = WE_MALLOC((WE_ULONG)(sizeof(St_PublicData)))) == NULL)
   {
      WE_FREE(pMe);
      return G_SEC_NOTENOUGH_MEMORY;
   }   
   (void)WE_MEMSET(pMe->hPrivateData,0,sizeof(St_PublicData));
   
   /* initialize the data members*/
   pMe->iIFType = iType;
   /*socket id*/
   /*((St_PublicData *)(((ISec*)pMe)->hPrivateData))->iFd =  iFd;*/
   /*added by bird 070126*/
   SEC_SIINFO = &stInfo;
   /*added by bird 070202*/
   SEC_PEER_CACHE = astPeerCache;
   SEC_SESSION_CACHE = astSessionCache;
   SEC_WIM_HAVELOAD = &cHaveLoad;
   /*Note: this must be run after the former sentence*/
   if(SUCCESS != (WE_INT32)SecMgr_Initialize(&(((St_PublicData*)(pMe->hPrivateData))->hWeHandle)))
   { 
       WE_FREE(pMe->hPrivateData);
       WE_FREE(pMe);
       return G_SEC_GENERAL_ERROR;
   }
   if(SUCCESS != Sec_StartEx((WE_HANDLE)pMe))
   {
       (void)SecMgr_Terminate(((St_PublicData*)(pMe->hPrivateData))->hWeHandle);
       WE_FREE(pMe->hPrivateData);
       WE_FREE(pMe);
       return G_SEC_GENERAL_ERROR;
   }
    Sec_iWimInit((WE_HANDLE)pMe);
    {          
#ifndef M_SEC_CFG_NO_PLATFORM 
        Sec_iWimReadView((WE_HANDLE)pMe, &pucData, &iDataLen);

        iRes = Sec_WimStartInfo((WE_HANDLE)pMe, pucData, iDataLen);
        if (pucData != NULL)
        {
            WE_FREE(pucData);
        }
        if (iRes != M_SEC_ERR_OK)
        {
            iRes = Sec_WimStopInfo((WE_HANDLE)pMe);
        }
 #endif
    }
   /* Set the pointer in the parameter*/

   *phHandle = (WE_HANDLE)pMe;
 
   WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM, 0,"ISec_Create end\n"));


   return G_SEC_OK;
}


