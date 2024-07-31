/*=====================================================================================
FILE NAME   : eme_codec_api.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File is the header file of eme_codec_api.c.
      These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070302     wxs            None         Initialize   
         20070306     wxs                         modify
         
=====================================================================================*/

/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_API_H_
#define _EME_CODEC_API_H_

/************************************************************************/
/*                  include files                                  */
/************************************************************************/
#include "eme_constant_api.h"


/*==================================================================================================
* Type Define Section
==================================================================================================*/  
typedef void* IEMSGHandle; 
typedef void(*PFNENCODINGCB)(IEMSGHandle hEmailHandle, EmailOpt *ppstOpts, void *pvUserData, int iResult);
typedef void(*PFNDECODINGCB)(IEMSGHandle hEmailHandle, void *pvUserData, int iResult);
typedef void(*PFNNOTIFY)(void * pUser);
/***************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/
int EMECodecLib_InitHandle(IEMSGHandle *phEmailHandle,void* pvMemoryMgr,void *pIFileMgr,void* pICSC);
int EMECodecLib_AddHeader(IEMSGHandle hEmailHandle,EmailOpt *pstHeaderOpts);
int EMECodecLib_AddPart(IEMSGHandle hEmailHandle,EmailOpt *pstOpts); 
int EMECodecLib_Encoding(IEMSGHandle hEmailHandle, EmailOpt **ppstOpts);
int EMECodecLib_Decoding(IEMSGHandle hEmailHandle, EmailOpt *pstOpts);
int EMECodecLib_GetNumOfParts(IEMSGHandle hEmailHandle, WE_UINT32 *pPartNum);
int EMECodecLib_GetHeader(IEMSGHandle hEmailHandle, EmailOpt **ppstOpts);
int EMECodecLib_GetPart(IEMSGHandle hEmailHandle, WE_UINT32 uiPartIndex, EmailOpt **ppstOpts);
int EMECodecLib_DeleteHandle(IEMSGHandle *phEmailHandle);

/*************************************************************************************************
 Encoding & Decoding of Synchronism.
 ************************************************************************************************/
int EMECodecLib_InitHandleEx(IEMSGHandle *phEmailHandle,void* pvMemoryMgr,void *pIFileMgr,void* pICSC, void *pICBMgr);
int EMECodecLib_EncodingEx(IEMSGHandle hEmailHandle, WE_CHAR *pcDir, void *pvUserData, PFNENCODINGCB pfn, EmailOpt **ppstOpts);
int EMECodecLib_AddPartEx(IEMSGHandle hEmailHandle, EmailOpt *pstOpts);
int EMECodecLib_DecodingEx(IEMSGHandle hEmailHandle, WE_CHAR *pcDir, EmailOpt *pstOpts, void *pvUserData, PFNDECODINGCB pfn);

int EMECodecLib_UnZip(IEMSGHandle hEmailHandle, void *pvInFileName, void *pvOutFileName);
/*===========================================================================
 INTERFACES DOCUMENTATION  (Hubin 20070423 added)
===========================================================================

   EMECodecLib_InitHandle()
Description:
   This Function Init the codec lib handler
Prototype:
   int EMECodecLib_InitHandle(IEMSGHandle *phEmailHandle,void* pvMemoryMgr,void *pIFileMgr)
Parameters:
   phEmailHandle:The codec lib handler
   pvMemoryMgr:Memory manager handler
   pIFileMgr:File manager handler
Return Value:
   Codec lib init state and the codec lib handler itself
Side Effects:
   None

   EMECodecLib_AddHeader()
Description:
   This Function Add the message's header part
Prototype:
   int EMECodecLib_AddHeader(IEMSGHandle hEmailHandle,EmailOpt *pstHeaderOpts)
Parameters:
   hEmailHandle:The codec lib handler
   pstHeaderOpts:Header Data Opts
Return Value:
   Add header state
Side Effects:
   None

   EMECodecLib_AddPart()
Description:
   This Function Add the message's body part
Prototype:
   int EMECodecLib_AddPart(IEMSGHandle hEmailHandle,EmailOpt *pstOpts)
Parameters:
   hEmailHandle:The codec lib handler
   pstOpts:Body Data Opts
Return Value:
   Add body state
Side Effects:
   None

   EMECodecLib_Encoding()
Description:
   This Function Encode the message
Prototype:
   int EMECodecLib_Encoding(IEMSGHandle hEmailHandle, EmailOpt **pstOpts)
Parameters:
   hEmailHandle:The codec lib handler
   pstOpts:Encoded Opts returned
Return Value:
   Encode state
Side Effects:
   None

   EMECodecLib_Decoding()
Description:
   This Function Decode the incoming message
Prototype:
   int EMECodecLib_Decoding(IEMSGHandle hEmailHandle, EmailOpt *pstOpts)
Parameters:
   hEmailHandle:The codec lib handler
   pstOpts:Decode Opts
Return Value:
   Decode state
Side Effects:
   None

   EMECodecLib_GetNumOfParts()
Description:
   This Function Get the number of body parts
Prototype:
   int EMECodecLib_GetNumOfParts(IEMSGHandle hEmailHandle, WE_UINT32 *pPartNum)
Parameters:
   hEmailHandle:The codec lib handler
   pPartNum:Numbers returned
Return Value:
   Operation state
Side Effects:
   None

   EMECodecLib_GetHeader()
Description:
   This Function Get the number of body parts
Prototype:
   int EMECodecLib_GetHeader(IEMSGHandle hEmailHandle, EmailOpt **pstOpts)
Parameters:
   hEmailHandle:The codec lib handler
   pstOpts:Headers data opts returned
Return Value:
   Operation state
Side Effects:
   None

   EMECodecLib_GetPart()
Description:
   This Function Get the number of body parts
Prototype:
   int EMECodecLib_GetPart(IEMSGHandle hEmailHandle, 
                                WE_UINT32 uiPartIndex, 
                                EmailOpt **pstOpts)
Parameters:
   hEmailHandle:The codec lib handler
   uiPartIndex:Which body part need to know
   pstOpts:Body data opts returned
Return Value:
   Operation state
Side Effects:
   None

   EMECodecLib_DeleteHandle()
Description:
   This Function delete the codec handler
Prototype:
   int EMECodecLib_DeleteHandle(IEMSGHandle *phEmailHandle)
Parameters:
   phEmailHandle:The codec lib handler
Return Value:
   Operation state
Side Effects:
   None
===========================================================================*/
#endif /* _EME_CODEC_API_H_ */