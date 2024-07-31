/*==============================================================================
    FILE NAME   : eme_dataaccess_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
         This file contains some abstract definitions/declarations for
         access data.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-08-30   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_DATAACCESS_API_H
#define EME_DATAACCESS_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
/* Include System head file */

/* Include Program Global head file */
#include "we_def.h"

/* Include module head file */
#include "eme_error.h"
#include "eme_file_api.h"
#include "eme_multipartmsg_api.h"
#include "EMS_Store_API.h"
#include "we_msg.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
/*
 *	This is reserved for some information of the email file, 
 * like account id, box id and be down flag, 
 * user can use user-define structure instead, 
 * but do not exceed the space define below.
 */

/*
 *	Part data structure
 */
#define PART_DATA_TYPE_BUFFER    0
#define PART_DATA_TYPE_FILE      1

typedef struct MsgPartData 
{
   WE_INT32    nType;   /* Data type */
   WE_VOID    *pData;   /* Data */
   WE_UINT32   dwSize;  /* Data size */
   WE_CHAR    *pExt;    /* Default extension */
}MsgPartData;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
/*
 *	Common Operation
 */
WE_INT32 EME_DataAccess_Init(FILEMGR_HANDLE hMgr);
WE_BOOL EME_DataAccess_IsEnoughSpace(FILEMGR_HANDLE hMgr, WE_UINT32 dwLeastSpace);

/*
 *	Message files operation
 */
WE_UINT32 EME_DataAccess_GetMsgSize(FILEMGR_HANDLE hMgr, WE_UINT32 dwMsgId);

WE_VOID EME_DataAccess_GetFileBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR **pcData, WE_UINT32 *puiSize);

WE_INT EME_DataAccess_CreateFileByBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR *pcData, WE_UINT32 uiSize);

WE_INT EME_DataAccess_UpdateFileByBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR *pcData, WE_UINT32 uiSize);

IWEMsgValue* EME_DataAccess_GetIWEMsgValueByIWEMsg(IWEMsg *pIMsg, WE_UINT32 uiOptId, WE_INT32 Index);

IWEMsg* EME_DataAccess_GetIWEMsgByFile(FILE_HANDLE hMgr, const WE_CHAR *filename);

WE_INT32 EME_DataAccess_CreateFile(FILEMGR_HANDLE hMgr, WE_CHAR *pDest, WE_VOID *pData, WE_UINT32 dwSize);

WE_INT32 EME_DataAccess_CopyFile(WE_VOID *pvMemMgr, FILEMGR_HANDLE hMgr, WE_CHAR *pDest, WE_CHAR *pSrc);

WE_CHAR* EME_DataAccess_GetTempDir(FILE_HANDLE hMgr);

WE_VOID EME_DataAccess_InitActCashe(FILE_HANDLE hMgr);

WE_VOID EME_DataAccess_DestroyActCashe(FILE_HANDLE hMgr);

WE_VOID EME_DataAccess_DestroyPathMgr(WE_VOID);

WE_VOID EME_DataAccess_InitPathMgr(WE_VOID);

WE_VOID EME_DataAccess_ClearPathMgr(WE_VOID);
/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Function:      EME_DataAccess_Init
 * 
 *    Description:   This function is used to init the email file system.
 * 
 *    Prototype:
 *       WE_INT32 EME_DataAccess_Init(FILEMGR_HANDLE hMgr);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful.  
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_DataAccess_IsEnoughSpace
 * 
 *    Description:   There may have many restricts when access the file system, 
 *                   this function can provide a method to check the restricts.
 * 
 *    Prototype:
 *       WE_BOOL EME_DataAccess_IsEnoughSpace(FILEMGR_HANDLE hMgr, 
 *                                            WE_UINT32 dwLeastSpace);
 * 
 *    Parameters:
 *       hMgr           [in]: Some platform may need file manager handle, 
 *                            this parameter point to the file manager handler.
 *       dwLeastSpace   [in]: At least needed storage.
 *    Return Value:
 *       TRUE:    If is available. 
 *       FALSE:   If otherwise.
 *  	   
 *    Comments:
 * 
 *============================================================================*/

#endif /* EME_DATAACCESS_API_H */