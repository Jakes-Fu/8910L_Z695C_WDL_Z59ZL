/*==============================================================================
    FILE NAME   : eme_file.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        Implementations of file operation.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-03-02   Daniel           01         Create
    2007-08-29   Wenkai Xu        02         Add and Modify

==============================================================================*/
/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
/* Include System head file */

/* Include Program Global head file */

/* Include Module head file */

/* Include Private head file */
#include "eme_file_api.h"
#include "eme_utils_api.h"
#include "EMS_Store_API.h"
#include "eme_dataaccess_priv.h"

typedef struct _EMEFileMgr
{
   void *pvReadDir;

}EMEFileMgr;
/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/


/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
/************************************************************************/
/* Open File                                                            */
/************************************************************************/
FILE_HANDLE EME_FileOpen(FILEMGR_HANDLE hMgr, 
                         const WE_CHAR *pPath, 
                         WE_INT32 nMode)
{
   FILE_HANDLE *piFile = NULL;
   
   if (NULL == pPath)
   {
      return NULL;
   }

   EME_UTILS_GETMUTEX

   piFile = TF3RD_FOPEN(pPath, nMode);

   EME_UTILS_PUTMUTEX
   
   if (NULL == piFile)
   {
      return NULL;
   }

   return piFile;
}


/************************************************************************/
/* Create File                                                          */
/************************************************************************/
WE_INT32 EME_CreateFile(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   FILE_HANDLE *piFile = NULL;
   
   if (NULL == pPath)
   {
      return EME_FAILED;
   }

   EME_UTILS_GETMUTEX
      
   piFile = TF3RD_FOPEN(pPath, TF3RD_OFM_CREATE);
      
   if (NULL == piFile)
   {
      EME_UTILS_PUTMUTEX

      return EME_FAILED;
   }

   TF3RD_FCLOSE(piFile);

   EME_UTILS_PUTMUTEX
   
   return EME_SUCCESS;
}


/************************************************************************/
/* Close File                                                           */
/************************************************************************/
WE_INT32 EME_FileClose(FILE_HANDLE hFile)
{
   if (NULL == hFile)
   {
      return EME_FAILED;
   }

   EME_UTILS_GETMUTEX
      
   TF3RD_FCLOSE(hFile);

   EME_UTILS_PUTMUTEX
   
   return EME_SUCCESS;
}


/************************************************************************/
/* Read File                                                            */
/************************************************************************/
WE_UINT32 EME_FileRead(FILE_HANDLE hFile, WE_VOID *pBuf, WE_UINT32 dwSize)
{
   WE_UINT32 uiRet = 0;
   
   if (NULL == hFile || NULL == pBuf)
   {
      return 0;
   }

   EME_UTILS_GETMUTEX
      
   uiRet = TF3RD_FREAD(hFile, pBuf, dwSize);

   EME_UTILS_PUTMUTEX

   return uiRet;
}


/************************************************************************/
/* Write File                                                           */
/************************************************************************/
WE_UINT32 EME_FileWrite(FILE_HANDLE hFile, WE_VOID *pBuf, WE_UINT32 dwSize)
{ 
   WE_UINT32 uiRet = 0;
   
   if (NULL == hFile || NULL == pBuf)
   {
      return 0;
   }

   EME_UTILS_GETMUTEX
      
   uiRet = TF3RD_FWRITE(hFile, pBuf, dwSize);

   EME_UTILS_PUTMUTEX

   return uiRet;
}


/************************************************************************/
/* Seek File                                                            */
/************************************************************************/
WE_INT32 EME_FileSeek(FILE_HANDLE hFile, WE_INT32 offset, WE_INT32 nOrigin)
{
   int nErr = EME_SUCCESS;
   
   if (NULL == hFile)
   {
      return EME_FAILED;
   }

   EME_UTILS_GETMUTEX
      
   nErr = TF3RD_FSEEK(hFile, offset, nOrigin);
   if ((EME_SEEK_CURRENT == nOrigin && 0 == offset) || SUCCESS == nErr)
   {
      EME_UTILS_PUTMUTEX
         
      return EME_SUCCESS;
   }

   EME_UTILS_PUTMUTEX
      
   return EME_FAILED;
}


/************************************************************************/
/* Get file pointer position                                            */
/************************************************************************/
WE_UINT32 EME_FileTell(FILE_HANDLE hFile)
{  
   WE_UINT32 uiRet = 0;

   if (NULL == hFile)
   {
      return 0;
   }
   
   EME_UTILS_GETMUTEX

   uiRet = TF3RD_FTELL(hFile);

   EME_UTILS_PUTMUTEX

   return uiRet;
}


/************************************************************************/
/* Get file length                                                      */
/************************************************************************/
WE_UINT32 EME_FileLength(FILE_HANDLE hFile)
{
   WE_UINT32 dwSize = 0;
   
   EME_UTILS_GETMUTEX

   TF3RD_STAT(hFile, &dwSize);

   EME_UTILS_PUTMUTEX

   return dwSize;
}


/************************************************************************/
/* Truncate file                                                        */
/************************************************************************/
WE_INT32 EME_FileTruncate(FILE_HANDLE hFile, WE_UINT32 dwPos)
{
   int nErr = EME_SUCCESS;
  
   if (NULL == hFile)
   {
      return EME_FAILED;
   }
   
   EME_UTILS_GETMUTEX

   nErr = TF3RD_FTRUNCATE(hFile, dwPos);
   if (SUCCESS != nErr)
   {
      EME_UTILS_PUTMUTEX

      return EME_FAILED;
   }

   EME_UTILS_PUTMUTEX

   return EME_SUCCESS;
}


/************************************************************************/
/* Create directory                                                     */
/************************************************************************/
WE_INT32 EME_CreateDir(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   if (NULL == pPath)
   {
      return EME_FAILED;
   }
   
   EME_UTILS_GETMUTEX

   if(TF3RD_MAKEDIR(pPath) != EME_SUCCESS)
   {
      EME_UTILS_PUTMUTEX

      return EME_FAILED;
   }
   else
   {
      EME_UTILS_PUTMUTEX

      return EME_SUCCESS;
   }
}


/************************************************************************/
/* Remove directory                                                     */
/************************************************************************/
WE_INT32 EME_RemoveDir(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   WE_INT32 iRet = 0;
   WE_CHAR *pcDirPath = NULL;

   if(!pPath)
   {
      return EME_FAILED;
   }

   pcDirPath = TF3RD_STRDUP(pPath);
   if(NULL == pcDirPath)
   {
      return EME_NOMEMORY;
   }

   hMgr = (FILEMGR_HANDLE)EME_EnumInit(pcDirPath, TRUE);
   if(NULL != hMgr)
   {
      WE_CHAR *pcDir = NULL;

      while(NULL != (pcDir = EME_EnumNext(hMgr)))
      {
         EME_RemoveDir(hMgr, pcDir);
      }

      EME_EnumClose(hMgr);
      hMgr = NULL;
   }

   hMgr = (FILEMGR_HANDLE)EME_EnumInit(pcDirPath, FALSE);
   if(NULL != hMgr)
   {
      WE_CHAR *pcFile = NULL;
      
      while(NULL != (pcFile = EME_EnumNext(hMgr)))
      {
         EME_DeleteFile(hMgr, pcFile);
      }
      
      EME_EnumClose(hMgr);
      hMgr = NULL;
   }

   TF3RD_REMOVEDIR(pcDirPath);
   TF3RD_MEMFREE(pcDirPath);

   return iRet;
}


/************************************************************************/
/* Delete File                                                          */
/************************************************************************/
WE_INT32 EME_DeleteFile(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   int nErr = EME_SUCCESS;
   
   if (NULL == pPath)
   {
      return EME_FAILED;
   }
   
   EME_UTILS_GETMUTEX

   nErr = TF3RD_REMOVE(pPath);
   if (SUCCESS != nErr)
   {
      EME_UTILS_PUTMUTEX

      return EME_FAILED;
   }

   EME_UTILS_PUTMUTEX

   return EME_SUCCESS;
}


/************************************************************************/
/* Test directory                                                       */
/************************************************************************/
WE_BOOL EME_DirectoryExists(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   WE_BOOL bRet = FALSE;
   if(!pPath)
   {
      return FALSE;
   }
   
   EME_UTILS_GETMUTEX
      
   bRet = (WE_BOOL)(SUCCESS == TF3RD_FACCESS((char*)pPath));

   EME_UTILS_PUTMUTEX

   return bRet;
}


/************************************************************************/
/* Test file                                                            */
/************************************************************************/
WE_BOOL EME_FileExists(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath)
{
   WE_BOOL bRet = FALSE;
   if(!pPath)
   {
      return FALSE;
   }

   EME_UTILS_GETMUTEX

   bRet = (WE_BOOL)(SUCCESS == TF3RD_FACCESS((char*)pPath));

   EME_UTILS_PUTMUTEX

   return bRet;
}


/************************************************************************/
/* Get free space                                                       */
/************************************************************************/
WE_BOOL EME_CheckFitFreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize)
{
   if(EME_CheckSDCard1FreeSpace(hMgr, uiSpaceSize))
   {
      return TRUE;
   }

   if(EME_CheckSDCard2FreeSpace(hMgr, uiSpaceSize))
   {
      return TRUE;
   }

   if(EME_CheckUDiskFreeSpace(hMgr, uiSpaceSize))
   {
      return TRUE;
   }
   
   return FALSE;
}

WE_BOOL EME_CheckFitFreeSpaceEx(FILEMGR_HANDLE hMgr, WE_UINT32 uiMsgId, WE_UINT32 uiSpaceSize)
{
   WE_UINT32 uiActSaveAs = EME_GET_PLACE_FROM_MSGID(uiMsgId);

   if(EMS_SAVE_PLACE_SDCARD1 == uiActSaveAs)
   {
      return EME_CheckSDCard1FreeSpace(hMgr, uiSpaceSize);
   }

   if(EMS_SAVE_PLACE_SDCARD2 == uiActSaveAs)
   {
      return EME_CheckSDCard2FreeSpace(hMgr, uiSpaceSize);
   }

   if(EMS_SAVE_PLACE_UDISK == uiActSaveAs)
   {
      return EME_CheckUDiskFreeSpace(hMgr, uiSpaceSize);
   }
   
   return FALSE;
}

WE_BOOL EME_CheckUDiskFreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize)
{
   WE_UINT32 dwHighSize = 0;
   WE_UINT32 dwLowSize = 0;
   WE_CHAR   *pcUdisk = EMEFS_UDISK_ROOT;
   
   if(NULL == pcUdisk)
   {
      return FALSE;
   }
   
   while((*pcUdisk != 0x00) &&(*pcUdisk == ' ' || *pcUdisk == '	'))pcUdisk++;
   
   if(*pcUdisk != 0x00 && (*pcUdisk == 'd' || *pcUdisk == 'D'))
   {
      TF3RD_UDFREESPACE(&dwHighSize, &dwLowSize);
   }
   else
   {
      TF3RD_SD1FREESPACE(&dwHighSize, &dwLowSize);
   }
   
   if(dwHighSize == 0 && dwLowSize < uiSpaceSize)
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}

WE_BOOL EME_CheckSDCard1FreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize)
{
   WE_UINT32 dwHighSize = 0;
   WE_UINT32 dwLowSize = 0;

   TF3RD_SD1FREESPACE(&dwHighSize, &dwLowSize);

   if(dwHighSize == 0 && dwLowSize < uiSpaceSize)
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}

WE_BOOL EME_CheckSDCard2FreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize)
{
   WE_UINT32 dwHighSize = 0;
   WE_UINT32 dwLowSize = 0;

   TF3RD_SD2FREESPACE(&dwHighSize, &dwLowSize);

   if(dwHighSize == 0 && dwLowSize < uiSpaceSize)
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}
/************************************************************************/
/* Enum init                                                            */
/************************************************************************/
WE_INT32 EME_EnumInit(const WE_CHAR *pDir, WE_BOOL bDirs)
{
   EMEFileMgr *pstFileMgr = NULL;
   
   if (NULL == pDir)
   {
      return 0;
   }
   
   pstFileMgr = EME_UTILS_MALLOC(NULL, sizeof(EMEFileMgr));
   if(NULL == pstFileMgr)
   {
      return 0;
   }
   EME_UTILS_MEMSET(pstFileMgr, 0x00, sizeof(EMEFileMgr));

   EME_UTILS_GETMUTEX

   pstFileMgr->pvReadDir = TF3RD_OPENDIR(pDir, bDirs);
   if(NULL == pstFileMgr->pvReadDir)
   {
      EME_UTILS_PUTMUTEX
      EME_UTILS_FREEIF(NULL, pstFileMgr);
      return 0;
   }
   
   EME_UTILS_PUTMUTEX

   return (WE_INT32)pstFileMgr;
}


/************************************************************************/
/* Enum next                                                            */
/************************************************************************/
WE_CHAR* EME_EnumNext(FILEMGR_HANDLE hMgr)
{
   EMEFileMgr *piFileMgr = (EMEFileMgr*)hMgr;
   char       *pcFind = NULL;
   WE_CHAR    *pcRet = NULL;
   
   if (NULL == piFileMgr)
   {
      return FALSE;
   }
   
   EME_UTILS_GETMUTEX

   pcRet = TF3RD_READDIR(piFileMgr->pvReadDir);

   EME_UTILS_PUTMUTEX

   return pcRet;
}


/************************************************************************/
/* Enum close: Some platform may need this api                          */
/************************************************************************/
WE_VOID EME_EnumClose(FILEMGR_HANDLE hMgr)
{
   EMEFileMgr *pstFileMgr = (EMEFileMgr*)hMgr;
   
   if(!hMgr)
      return;

   EME_UTILS_GETMUTEX

   if(pstFileMgr->pvReadDir)
   {
      TF3RD_CLOSEDIR(pstFileMgr->pvReadDir);
   }

   TF3RD_MEMFREE(hMgr);
   EME_UTILS_PUTMUTEX
}

FILEMGR_HANDLE EME_CreateFileMgr(void)
{
   EMEFileMgr *pstFileMgr = NULL;

   pstFileMgr = EME_UTILS_MALLOC(NULL, sizeof(EMEFileMgr));
   if(NULL == pstFileMgr)
      return NULL;

   return (FILEMGR_HANDLE)pstFileMgr;
}

void EME_DeleteFileMgr(FILEMGR_HANDLE *hFileMgr)
{
   EMEFileMgr *pstFileMgr = (EMEFileMgr*)hFileMgr;

   if(!hFileMgr)
      return;
   
   EME_UTILS_GETMUTEX

   if(pstFileMgr->pvReadDir)
   {
      TF3RD_CLOSEDIR(pstFileMgr->pvReadDir);
   }

   EME_UTILS_PUTMUTEX

   EME_UTILS_FREEIF(NULL, hFileMgr);
}

WE_BOOL EME_SDCard1Vailed(FILEMGR_HANDLE *hFileMgr)
{
   if(EME_SUCCESS == TF3RD_SD1VAILED())
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

WE_BOOL EME_SDCard2Vailed(FILEMGR_HANDLE *hFileMgr)
{
   if(EME_SUCCESS == TF3RD_SD2VAILED())
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

WE_BOOL EME_UdiskVailed(FILEMGR_HANDLE *hFileMgr)
{
   if(EME_SUCCESS == TF3RD_UDVAILED())
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}