/*=====================================================================================
FILE NAME   : eme_codec_decode_bodystruct.h
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
    This File is the header file of eme_codec_decode_bodystruct.c.
    These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070306     wxs                         create
        
                 
=====================================================================================*/
/*==================================================================================================
* Multi-Include-Prevent Section
==================================================================================================*/
#ifndef _EME_CODEC_DECODE_BODYSTRUCT_H_
#define _EME_CODEC_DECODE_BODYSTRUCT_H_


/*---------------------------------------------------------------
 *       Include Files                                       *
 * ------------------------------------------------------------*/
#include "eme_codec_def.h"
#include "eme_utils_api.h"
#include "we_def.h"

/*==================================================================================================
* Type Define Section
==================================================================================================*/  
/*body structure part info*/
typedef enum _E_BodyStructType
{
      E_BODYS_TEXTPLAIN          = 0  ,                       /* plain txt part */
      E_BODYS_TEXTHTML           = 1  ,                       /* html part */
      E_BODYS_RELATEDATTACH      = 2  ,                       /* embeded att(for html) */
      E_BODYS_ATTACHMENT         = 3  ,                          /* normal att */
      
      E_BODYS_MAX_FOURBYTES      = 0x7FFFFFFF  
      
}E_BodyStructType;

typedef struct _St_BodyStructPart
{
   E_BodyStructType           eBodyStructType;
   WE_UINT32                  uiBSPartSize;
   WE_CHAR*                   pcAttachName;              /* attachment file name */
   WE_CHAR*                   pcCharset;           /* WE_CHAR set type */
   WE_CHAR*                   pcTransEncoding;      /* transfer encodeing type */
   WE_CHAR*                   pcContentId;          /* content id for att */
   WE_CHAR                    acSection[10];       /* section in body, refer to rfc2060 */
   WE_CHAR                    acRes[2];
} St_BodyStructPart;
   
/*
* sMarkCounter record the hierarchical part in body,
* when sMarkCounter is 1 the part is main part in body,
* if sMarkCounter > 1 the part is relative or related part in body.
*/
typedef struct _St_BodyStruct
{
//    WE_CHAR                    *_mixedBndary;      /* reserved for rebuild email file */
//    WE_CHAR                    *_relatedBndary;    /* reserved for rebuild email file */
   WE_CHAR                    *pcPosition;
   St_BodyStructPart          **ppstBSPartList;        /* part list. */
   WE_UINT32                  uiBSPartNum;           /* part list number. */
   WE_UINT16                  usMainPtNum;         /* main part number in body. */
   WE_UINT16                  usSecPtNum;          /* seconde level part number. */
   WE_INT16                   sMarkCounter;
   WE_INT16                   sTextHierarch;      /* record text hierarchical level in body */   
}St_BodyStruct;

WE_INT32  EME_BodyStruct_Create(stEMSGHandle *pstEMsgHandle,void** pvBodyStruct);

WE_INT32  EME_BodyStruct_Destroy(stEMSGHandle *pstEMsgHandle,void** pvBodyStruct);

WE_INT32  EME_BodyStruct_Parse(stEMSGHandle *pstEMsgHandle,
                               void* pvBodyStruct, 
                               WE_CHAR* pcBodystructBuff);

WE_UINT32 EME_BodyStruct_GetBdNum(void* pvBodyStruct);

void*     EME_BodyStruct_GetBdPart(void* pvBodyStruct, WE_UINT32 id);
   
   

#endif /*_EM_BODYSTRUCTURE_H*/