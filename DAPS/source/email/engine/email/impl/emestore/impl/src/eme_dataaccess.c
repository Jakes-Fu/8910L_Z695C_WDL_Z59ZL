/*==============================================================================
    FILE NAME   : eme_dataaccess.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        This file contains implementations for access data.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-08-30   Wenkai Xu        01         Create
    2007-10-10   Wenkai Xu        02         Add some code for error handle

==============================================================================*/

//pclint
/*lint -e720 */
/*lint -e18 */
/*lint -e787 */

/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
/* Include System head file */

/* Include Program Global head file */

/* Include Module head file */

/* Include Private head file */
#include "eme_dataaccess_priv.h"
#include "ems_store_api.h"
#include "we_msg.h"

#define EME_DATAACCESS_ACT_CASHE

/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/

#ifdef EME_DATAACCESS_ACT_CASHE

#define EME_ACT_CASHE_MAX_NUM          10

typedef struct tagSt_ActCashe
{
   struct tagSt_ActCashe *next;
   struct tagSt_ActCashe *prev;
   WE_CHAR file[128];
   WE_CHAR *buffer;
   WE_UINT32 size;
   WE_BOOL bEmlFile;
}St_ActCashe;

typedef struct tagSt_ActCasheMgr
{
   WE_INT32     iRef;
   St_ActCashe stCasheAct;
}St_ActCasheMgr;

static St_ActCasheMgr gs_stActCashMgr = {0};
#endif

/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
#ifdef EME_DATAACCESS_ACT_CASHE
static WE_VOID act_cache_del(St_ActCashe *pstdel)
{
   if(NULL == pstdel)
      return;

   if(pstdel == &gs_stActCashMgr.stCasheAct)
   {
      return;
   }

   pstdel->next->prev = pstdel->prev;
   pstdel->prev->next = pstdel->next;
   
   EME_UTILS_FREEIF(NULL, pstdel->buffer);
   EME_UTILS_FREEIF(NULL, pstdel);
}

static WE_BOOL act_cache_empty(void)
{
   return (&gs_stActCashMgr.stCasheAct == gs_stActCashMgr.stCasheAct.prev);
}

static WE_UINT32 act_cache_num(void)
{
   St_ActCashe *pstActTmp = gs_stActCashMgr.stCasheAct.prev;
   WE_UINT32 uiNum = 0;;

   while(pstActTmp != &gs_stActCashMgr.stCasheAct)
   {
      uiNum++;
      pstActTmp = pstActTmp->prev;
   }

   return uiNum;
}

static St_ActCashe* act_cache_find(WE_CHAR* pcFile)
{
   St_ActCashe *pstFind = gs_stActCashMgr.stCasheAct.prev;
   
   if(NULL == pcFile)
      return NULL;

   if(act_cache_empty())
      return NULL;
   
   while(pstFind != &gs_stActCashMgr.stCasheAct)
   {
      if(EME_UTILS_STRCMP(pstFind->file, pcFile) == 0)
      {
         return pstFind;
      }
      pstFind = pstFind->prev;
   }
   
   return NULL;
}

static St_ActCashe* act_cache_find_oldest_eml_cache(void)
{
   St_ActCashe *pstFind = gs_stActCashMgr.stCasheAct.prev;
   
   if(act_cache_empty())
      return NULL;
   
   while(pstFind != &gs_stActCashMgr.stCasheAct)
   {
      if(pstFind->bEmlFile)
      {
         return pstFind;
      }
      pstFind = pstFind->prev;
   }
   
   return NULL;
}

static WE_VOID act_cache_add(WE_HANDLE hMgr, WE_CHAR *pcFile)
{
   WE_UINT32 uiFileSize = 0;
   WE_CHAR   *pcFilebuf = NULL;
   St_ActCashe *pstCatch = NULL;
   
   if(NULL == pcFile)
      return;

   if(!EME_UTILS_STRSTR(pcFile, ".tmf"))
   {
      return;
   }

   while(pstCatch = act_cache_find(pcFile))
   {
      act_cache_del(pstCatch);
   }

   if(EME_ACT_CASHE_MAX_NUM == act_cache_num())
   {
      pstCatch = act_cache_find_oldest_eml_cache();
      if(NULL != pstCatch)
      {
         act_cache_del(pstCatch);
      }
      else
      {
         act_cache_del(gs_stActCashMgr.stCasheAct.prev);
      }
   }

   EME_DataAccess_GetFileBuffer(hMgr, pcFile, &pcFilebuf, &uiFileSize);
   if(NULL != pcFile && uiFileSize > 0)
   {  
      pstCatch = EME_UTILS_MALLOC(NULL, sizeof(St_ActCashe));
      if(NULL == pstCatch)
      {
         EME_UTILS_FREEIF(NULL, pcFilebuf);
         return;
      }
      EME_UTILS_MEMSET(pstCatch, 0x00, sizeof(St_ActCashe));
      
      pstCatch->buffer = pcFilebuf;
      pstCatch->size   = uiFileSize;
      
      EME_UTILS_MEMCPY(pstCatch->file, pcFile, EME_UTILS_STRLEN(pcFile));

      if(EME_UTILS_STRSTR(pcFile, EMEFS_FILE_ACTINDEX) ||
         EME_UTILS_STRSTR(pcFile, EMEFS_FILE_BOXINDEX) ||
         EME_UTILS_STRSTR(pcFile, EMEFS_FILE_MSGINDEX) ||
         EME_UTILS_STRSTR(pcFile, EMEFS_FILE_ACTDATA))
      {
         pstCatch->bEmlFile = FALSE;
      }
      else
      {
         pstCatch->bEmlFile = TRUE;
      }

      gs_stActCashMgr.stCasheAct.next->prev = pstCatch;
      pstCatch->next = gs_stActCashMgr.stCasheAct.next;
      pstCatch->prev = &gs_stActCashMgr.stCasheAct;
      gs_stActCashMgr.stCasheAct.next = pstCatch;
   }
}
#endif

WE_VOID EME_DataAccess_InitActCashe(FILE_HANDLE hMgr)
{
#ifdef EME_DATAACCESS_ACT_CASHE
   WE_CHAR *pcDirPath = NULL;
   WE_CHAR *pPath = EMEFS_ACT_ROOT_DIR;
   FILE_HANDLE hFolderMgr = NULL;
   FILE_HANDLE hFileMgr = NULL;

   if(gs_stActCashMgr.iRef > 0)
   {
      gs_stActCashMgr.iRef++;
      return;
   }

   gs_stActCashMgr.iRef = 1;
   gs_stActCashMgr.stCasheAct.next = gs_stActCashMgr.stCasheAct.prev = &gs_stActCashMgr.stCasheAct;
   
   hFolderMgr = EME_EnumInit(EMEFS_ACT_ROOT_DIR, TRUE);
   if(NULL != hFolderMgr)
   {
      WE_CHAR *pcDir = NULL;
      
      while(NULL != (pcDir = EME_EnumNext(hFolderMgr)))
      {
         hFileMgr = EME_EnumInit(pcDir, FALSE);
         if(hFileMgr != NULL)
         {
            WE_CHAR *pcFile = NULL;

            while(NULL != (pcFile = EME_EnumNext(hFileMgr)))
            {
               act_cache_add(hMgr, pcFile);
            }

            EME_EnumClose(hFileMgr);
            hFileMgr = NULL;
         }
      }
      
      EME_EnumClose(hFolderMgr);
      hFolderMgr = NULL;
   }

   hFolderMgr = EME_EnumInit(EMEFS_ACT_ROOT_DIR, FALSE);
   if(NULL != hFolderMgr)
   {
      WE_CHAR *pcFile = NULL;

      while(NULL != (pcFile = EME_EnumNext(hFolderMgr)))
      {
         act_cache_add(hMgr, pcFile);
      }

      EME_EnumClose(hFolderMgr);
      hFolderMgr = NULL;
   }
#endif
}

WE_VOID EME_DataAccess_DestroyActCashe(FILE_HANDLE hMgr)
{
#ifdef EME_DATAACCESS_ACT_CASHE
   St_ActCashe *cashTmp = gs_stActCashMgr.stCasheAct.prev;

   if(--gs_stActCashMgr.iRef > 0)
      return;

   while(cashTmp != &gs_stActCashMgr.stCasheAct)
   {
      act_cache_del(cashTmp);
      cashTmp = gs_stActCashMgr.stCasheAct.prev;
   }
#endif
}


WE_VOID EME_DataAccess_GetFileBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR **pcData, WE_UINT32 *puiSize)
{
   FILE_HANDLE hFile = NULL;
   WE_UINT32   uiSize = 0;
   WE_CHAR     *pcBuffer = NULL;
   WE_INT      nErr = EME_SUCCESS;

   do
   {
      hFile = EME_FileOpen(hMgr, filename, EME_OFM_READ);
      EME_UTILS_BREAKNULL(hFile, nErr, EME_FAILED, "open file failed");

      uiSize = EME_FileLength(hFile);

      pcBuffer = EME_UTILS_MALLOC(NULL, uiSize);
      EME_UTILS_BREAKNULL(pcBuffer, nErr, EME_NOMEMORY, "no memory");

      EME_UTILS_MEMSET(pcBuffer, 0x00, uiSize);

      EME_FileRead(hFile, pcBuffer, uiSize);
      *puiSize = uiSize;
      *pcData = pcBuffer;

   }while(0);

   if(hFile != NULL)
   {
      EME_FileClose(hFile);
   }
}

WE_INT EME_DataAccess_CreateFileByBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR *pcData, WE_UINT32 uiSize)
{
   FILE_HANDLE hFile = NULL;
   WE_INT      nErr = EME_SUCCESS;
   WE_UINT32   uiWriteSize = 0;
   
#ifdef EME_DATAACCESS_ACT_CASHE
   St_ActCashe *pstCache = NULL;
#endif

   do
   {
      if(EME_CheckUDiskFreeSpace(hMgr, uiSize + 1024))
      {
         hFile = EME_FileOpen(hMgr, filename, EME_OFM_CREATE);
         EME_UTILS_BREAKNULL(hFile, nErr, EME_FAILED, "open file failed");
         
         uiWriteSize = EME_FileWrite(hFile, pcData, uiSize);
         if(uiWriteSize != uiSize)
         {
            EME_UTILS_LOG_ERROR(("write data to file failed!! file = %s", filename));
            EME_UTILS_LOG_ERROR(("write data to file failed!! size = %d, write = %d", uiSize, uiWriteSize));
         }

         EME_FileClose(hFile);
      }
      else
      {
         EME_UTILS_LOG_ERROR(("no storeage to write: %d", uiSize));
         nErr = EME_NOSTORAGE;
      }

#ifdef EME_DATAACCESS_ACT_CASHE
      act_cache_add(hMgr, filename);
#endif

   }while(0);

   return nErr;
}

WE_INT EME_DataAccess_UpdateFileByBuffer(FILEMGR_HANDLE hMgr, const WE_CHAR *filename, WE_CHAR *pcData, WE_UINT32 uiSize)
{
   FILE_HANDLE hFile = NULL;
   WE_INT      nErr = EME_SUCCESS;
   WE_UINT32   uiWriteSize = 0;
   WE_BOOL     bWrite = TRUE;

#ifdef EME_DATAACCESS_ACT_CASHE
   St_ActCashe *pstCache = NULL;
#endif
   
   do
   {
#ifdef EME_DATAACCESS_ACT_CASHE
      pstCache = act_cache_find(filename);
      if(NULL != pstCache)
      {
         WE_INT iCmp = 0;
         if(pstCache->size == uiSize && 0 == EME_UTILS_MEMCMP(pstCache->buffer, pcData, uiSize))
         {
            bWrite = FALSE;
         }
      }
#endif

      if(bWrite)
      {
         WE_UINT32 uiOldSize = 0;

         hFile = EME_FileOpen(hMgr, filename, EME_OFM_READWRITE);
         EME_UTILS_BREAKNULL(hFile, nErr, EME_FAILED, "open file failed");
         uiOldSize = EME_FileLength(hFile);

         if(uiOldSize >= uiSize || EME_CheckUDiskFreeSpace(hMgr, uiSize - uiOldSize + 10))
         {
            nErr = EME_FileTruncate(hFile, 0);
            uiWriteSize = EME_FileWrite(hFile, pcData, uiSize);
            if(uiWriteSize != uiSize)
            {
               EME_UTILS_LOG_ERROR(("write data to file failed!! file = %s", filename));
               EME_UTILS_LOG_ERROR(("write data to file failed!! size = %d, write = %d", uiSize, uiWriteSize));
            }
         }
         else
         {
            EME_UTILS_LOG_ERROR(("no storeage %d", uiSize));
            nErr = EME_NOSTORAGE;
         }
         EME_FileClose(hFile);

#ifdef EME_DATAACCESS_ACT_CASHE
         act_cache_add(hMgr, filename);
#endif
      }

   }while(0);
   
   return nErr;
}

IWEMsg* EME_DataAccess_GetIWEMsgByFile(FILE_HANDLE hMgr, const WE_CHAR *filename)
{
   IWEMsg    *pIMsg = NULL;
   WE_UINT32 nErr = EME_SUCCESS;
   WE_CHAR   *pcBuf = NULL;
   WE_UINT32 uiSize = 0;
   WE_BOOL   bFind = FALSE;

#ifdef EME_DATAACCESS_ACT_CASHE
   St_ActCashe *pstCashe = NULL;
#endif

   do 
   {
#ifdef EME_DATAACCESS_ACT_CASHE
      pstCashe = act_cache_find(filename);
      bFind = (NULL == pstCashe) ? FALSE : TRUE;
#endif

      if(!bFind)
      {
         EME_DataAccess_GetFileBuffer(hMgr, filename, &pcBuf, &uiSize);
         EME_UTILS_BREAKNULL(pcBuf, nErr, EME_FAILED, "get file buffer failed!!");

#ifdef EME_DATAACCESS_ACT_CASHE
         act_cache_add(hMgr, filename);
#endif
      }
#ifdef EME_DATAACCESS_ACT_CASHE
      else
      {
         pcBuf = pstCashe->buffer;
         uiSize = pstCashe->size;
      }
#endif

      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create msg failed");
      
      nErr = IWEMSG_DecodeMsg(pIMsg, (WE_UCHAR*)pcBuf, uiSize);
      EME_UTILS_BREAKIF(nErr, "decode msg failed");
   } while (0);

   if(!bFind)
   {
      EME_UTILS_FREEIF(NULL, pcBuf);
   }


   if(nErr != EME_SUCCESS && NULL != pIMsg)
   {
      EME_UTILS_LOG_ERROR(("EME_DataAccess_GetIWEMsgByFile--: error code = %d", nErr));
      IWEMSG_Release(pIMsg);
      return NULL;
   }

   return pIMsg;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_DataAccess_Init
 *
 *    COMMRNT:    Init the file system
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Check the folder...
 *          3. Need not process error
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_DataAccess_Init(FILEMGR_HANDLE hMgr)
{
   WE_INT32 nErr = EME_SUCCESS;
   
   /*
    *	Add by Creaman 11/30/2007
    * CreateDir may is not recursive.
    * That is the function can create only the final directory in a path.
    * So we may first to create the root dir.
    */
   if (!EME_DirectoryExists(hMgr, EMEFS_UDISK_ROOT))
   {
      nErr = EME_CreateDir(hMgr, EMEFS_UDISK_ROOT);
      if (EME_SUCCESS != nErr)
      {
         EME_UTILS_LOG_ERROR(("EME_DataAccess_Init, create email root folder failed"));
         return EME_FAILED;
      }
   }

   if(EME_SDCard1Vailed(hMgr) && !EME_DirectoryExists(hMgr, EMEFS_SDCARD1_ROOT))
   {
      EME_CreateDir(hMgr, EMEFS_SDCARD1_ROOT);
   }

   if(EME_SDCard1Vailed(hMgr) && !EME_DirectoryExists(hMgr, EMEFS_SDCARD2_ROOT))
   {
      EME_CreateDir(hMgr, EMEFS_SDCARD2_ROOT);
   }

   /* Check/Create message folder */
   if (!EME_DirectoryExists(hMgr, EMEFS_UDISK_EML_ROOT_DIR))
   {
      nErr = EME_CreateDir(hMgr, EMEFS_UDISK_EML_ROOT_DIR);
      if (EME_SUCCESS != nErr)
      {
         EME_UTILS_LOG_ERROR(("EME_DataAccess_Init, create message folder failed"));
         return EME_FAILED;
      }
   }
   
    if (EME_SDCard1Vailed(hMgr) && !EME_DirectoryExists(hMgr, EMEFS_SDCARD1_EML_ROOT_DIR))
    {
       EME_CreateDir(hMgr, EMEFS_SDCARD1_EML_ROOT_DIR);
    }

    if (EME_SDCard2Vailed(hMgr) && !EME_DirectoryExists(hMgr, EMEFS_SDCARD2_EML_ROOT_DIR))
    {
       EME_CreateDir(hMgr, EMEFS_SDCARD2_EML_ROOT_DIR);
    }
    
   /* Check/Create account folder */
   if (!EME_DirectoryExists(hMgr, EMEFS_ACT_ROOT_DIR))
   {
      nErr = EME_CreateDir(hMgr, EMEFS_ACT_ROOT_DIR);
      if (EME_SUCCESS != nErr)
      {
         EME_UTILS_LOG_ERROR(("EME_DataAccess_Init, create account folder failed"));
         return EME_FAILED;
      }
   }

   /* Check/Create account.act file */
   if (!EME_FileExists(hMgr, EMEFS_ACT_INDEX_FILE))
   {
      return EME_ActIndex_CreateEmptyActIndex(hMgr);
   }
   else
   {
      return EME_ActIndex_CheckAndFixedActIndex(hMgr);
   }

   return EME_SUCCESS;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_DataAccess_IsEnoughSpace
 *
 *    COMMRNT:    To judge whether sufficient space.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Get the free space
 *          3. Compare with least needed space
 * 
 *----------------------------------------------------------------------------*/
WE_BOOL EME_DataAccess_IsEnoughSpace(FILEMGR_HANDLE hMgr, 
                                     WE_UINT32 dwLeastSpace)
{
   return EME_CheckFitFreeSpace(hMgr, dwLeastSpace);
}
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_DataAccess_CopyFile
 *
 *    COMMRNT:    This function is used to make a copy of an existing file.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Test the two files
 *          3. Open source file and create destination file
 *          4. Allocate memory block for file copy
 *          5. Copy the file contents
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_DataAccess_CopyFile(WE_VOID *pvMemMgr, FILEMGR_HANDLE hMgr, WE_CHAR *pDest, WE_CHAR *pSrc)
{
   WE_INT32 nErr = EME_SUCCESS;
   
   FILE_HANDLE hSrcFile = NULL;
   FILE_HANDLE hDestFile = NULL;
   WE_UINT32 dwRWSize = 0;
   WE_VOID *pBuf = NULL;
   
   if (NULL == pDest || NULL == pSrc)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /*
       *	Test the destination file
       */
      if (EME_FileExists(hMgr, pDest))
      {
         nErr =  EME_ALREADY;
         EME_UTILS_BREAK(nErr, 
            "EME_DataAccess_CopyFile, destination file already exist");
      }
      
      /* Open the two files */
      hSrcFile = EME_FileOpen(hMgr, pSrc, EME_OFM_READ);
      EME_UTILS_BREAKNULL(hSrcFile, nErr, EME_FAILED, 
         "EME_DataAccess_CopyFile, open source file failed");
      
      hDestFile = EME_FileOpen(hMgr, pDest, EME_OFM_CREATE);
      EME_UTILS_BREAKNULL(hDestFile, nErr, EME_FAILED, 
         "EME_DataAccess_CopyFile, create destination file failed");
      
      /* Allocate the memory for I/O buffer */
      pBuf = EME_UTILS_MALLOC(pvMemMgr, EME_IO_BUF_SIZE);
      EME_UTILS_BREAKNULL(pBuf, nErr, EME_NOMEMORY, 
         "EME_DataAccess_CopyFile, no memory for copy");
      EME_UTILS_MEMSET(pBuf, 0x00, EME_IO_BUF_SIZE);
      
      while ((dwRWSize = EME_FileRead(hSrcFile, pBuf, EME_IO_BUF_SIZE)) > 0)
      {
         if (dwRWSize != EME_FileWrite(hDestFile, pBuf, dwRWSize))
         {
            nErr = EME_NOSTORAGE;
            break;
         }
      }
      EME_UTILS_BREAKIF(nErr, 
         "EME_DataAccess_CopyFile, copying file failed");
      
      nErr = EME_SUCCESS;
   } while(0);
   
   EME_UTILS_FREEIF(pvMemMgr, pBuf);
   
   if (NULL != hDestFile)
   {
      EME_FileClose(hDestFile);
   }
   
   if (NULL != hSrcFile)
   {
      EME_FileClose(hSrcFile);
   }
   
   /* Handle error */
   if (EME_SUCCESS != nErr && EME_ALREADY != nErr)
   {
      EME_DeleteFile(hMgr, pDest);
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_DataAccess_CreateFile
 *
 *    COMMRNT:    This function is used to create a new file.
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Test the file
 *          2. Create destination file
 *          3. Write the buffer to the file
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_DataAccess_CreateFile(FILEMGR_HANDLE hMgr, WE_CHAR *pDest, WE_VOID *pData, WE_UINT32 dwSize)
{
   WE_INT32 nErr = EME_FAILED;
   
   FILE_HANDLE hDestFile = NULL;
   
   if (NULL == pDest || NULL == pData)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /* Test the file */
      if (EME_FileExists(hMgr, pDest))
      {
         nErr =  EME_ALREADY;
         EME_UTILS_BREAK(nErr, 
            "EME_DataAccess_CreateFile, destination file already exist");
      }
      
      hDestFile = EME_FileOpen(hMgr, pDest, EME_OFM_CREATE);
      EME_UTILS_BREAKNULL(hDestFile, nErr, EME_FAILED, 
         "EME_DataAccess_CreateFile, create destination file failed");
      
      if (dwSize != EME_FileWrite(hDestFile, pData, dwSize))
      {
         nErr = EME_FAILED;
         EME_UTILS_BREAK(nErr, 
            "EME_DataAccess_CreateFile, write content to file failed");
      }
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (NULL != hDestFile)
   {
      EME_FileClose(hDestFile);
   }
   
   /* Handle error */
   if (EME_SUCCESS != nErr && EME_ALREADY != nErr)
   {
      EME_DeleteFile(hMgr, pDest);
   }
   
   return nErr;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_DataAccess_GetMsgSize
 *
 *    COMMRNT:    This function is used to calculate the message size
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_UINT32 EME_DataAccess_GetMsgSize(FILEMGR_HANDLE hMgr, WE_UINT32 dwMsgId)
{
   WE_UINT32 dwSize = 0;
   WE_CHAR aMsgPath[EME_MAXPATH] = {0};
   WE_CHAR *pcFile = NULL;
   
   WE_UINT32 dwCard0 = EME_GET_PLACE_FROM_MSGID(dwMsgId);
   
   if(EMS_SAVE_PLACE_SDCARD1 == dwCard0)
   {
      EME_UTILS_SPRINTF(aMsgPath, EMEFS_MSG_DATA_FOLDER_SDCARD1, dwMsgId);
   }
   else if(EMS_SAVE_PLACE_SDCARD2 == dwCard0)
   {
      EME_UTILS_SPRINTF(aMsgPath, EMEFS_MSG_DATA_FOLDER_SDCARD2, dwMsgId);
   }
   else if(EMS_SAVE_PLACE_UDISK == dwCard0)
   {
      EME_UTILS_SPRINTF(aMsgPath, EMEFS_MSG_DATA_FOLDER_UDISK, dwMsgId);
   }
   else
   {
      return 0;
   }
   
   hMgr = EME_EnumInit(aMsgPath, FALSE);
   
   while (pcFile = EME_EnumNext(hMgr))
   {
      FILE_HANDLE hFile = NULL;
      
      hFile = EME_FileOpen(hMgr, pcFile, EME_OFM_READ);
      if (NULL == hFile)
      {
         continue;
      }
      
      dwSize += EME_FileLength(hFile);
      
      EME_FileClose(hFile);
   }
   
   EME_EnumClose(hMgr);
   
   return dwSize;
}

WE_CHAR* EME_DataAccess_GetTempDir(FILE_HANDLE hMgr)
{
   WE_UINT32 uiSD1High = 0;
   WE_UINT32 uiSD1Low  = 0;
   WE_UINT32 uiSD2High = 0;
   WE_UINT32 uiSD2Low  = 0;
   WE_UINT32 uiUDHigh = 0;
   WE_UINT32 uiUDLow = 0;
   WE_UINT32 uiMax = 0;
   
   TF3RD_SD1FREESPACE(&uiSD1High, &uiSD1Low);
   TF3RD_SD2FREESPACE(&uiSD2High, &uiSD2Low);
   TF3RD_UDFREESPACE(&uiUDHigh, &uiUDLow);
   
   if(uiSD1High > uiSD2High && uiSD1High > uiUDHigh)
   {
      return EMEFS_TEMP_SDCARD1_DIR;
   }
   else if(uiSD2High > uiSD1High && uiSD2High > uiUDHigh)
   {
      return EMEFS_TEMP_SDCARD2_DIR;
   }
   else if(uiUDHigh > uiSD1High && uiUDHigh > uiSD2High)
   {
      return EMEFS_TEMP_UDISK_DIR;
   }
   else if(uiSD1Low > uiSD2Low && uiSD1Low > uiUDLow)
   {
      return EMEFS_TEMP_SDCARD1_DIR;
   }
   else if(uiSD2Low > uiSD1Low && uiSD2Low > uiUDLow)
   {
      return EMEFS_TEMP_SDCARD2_DIR;
   }
   else
   {
      return EMEFS_TEMP_UDISK_DIR;
   }
}


typedef struct tag_PathMgr
{
   WE_INT32 iRef;
   WE_CHAR  *pcPathMgr;
}St_PathMgr;
static St_PathMgr gs_stPathMgr = {0};
extern PUBLIC void SP_CFG_GetRootPath(char** udisk_path, char** sd1_path, char **sd2_path);

WE_CHAR* EME_DataAccess_GetPath(EFSType eType)
{
   if(gs_stPathMgr.pcPathMgr == NULL)
   {
      return NULL;
   }
   
   return (WE_CHAR*)(gs_stPathMgr.pcPathMgr + (eType * EMEFS_FILE_MAX_LEN));
}

WE_VOID EME_DataAccess_DestroyPathMgr(WE_VOID)
{
   if(--gs_stPathMgr.iRef > 0)
   {
      return;
   }

   EME_UTILS_FREEIF(NULL, gs_stPathMgr.pcPathMgr);
   gs_stPathMgr.iRef = 0;
   return;
}

WE_VOID EME_DataAccess_InitPathMgr(WE_VOID)
{
   EFSType eFSType = EFSTYPE_UDISK_ROOT;
   WE_CHAR *pcudiskPath = NULL;
   WE_CHAR *pcsd1Path = NULL;
   WE_CHAR *pcsd2Path = NULL;
   WE_CHAR *pcDestPath = NULL;

   SP_CFG_GetRootPath(&pcudiskPath, &pcsd1Path, &pcsd2Path);

   if(gs_stPathMgr.iRef > 0)
   {
      gs_stPathMgr.iRef++;
      return;
   }

   gs_stPathMgr.pcPathMgr = EME_UTILS_MALLOC(NULL, EFSTYPE_MAX * EMEFS_FILE_MAX_LEN);
   if(gs_stPathMgr.pcPathMgr == NULL)
   {
      return;
   }
   EME_UTILS_MEMSET(gs_stPathMgr.pcPathMgr, 0x00, EFSTYPE_MAX * EMEFS_FILE_MAX_LEN);
   gs_stPathMgr.iRef = 1;

   while(eFSType < EFSTYPE_MAX)
   {
      pcDestPath = gs_stPathMgr.pcPathMgr + (eFSType * EMEFS_FILE_MAX_LEN);
      switch(eFSType)
      {
      case EFSTYPE_UDISK_ROOT:
         EME_UTILS_STRCAT(pcDestPath, pcudiskPath);
         break;
      case EFSTYPE_SDCARD1_ROOT:
         EME_UTILS_STRCAT(pcDestPath, pcsd1Path);
         break;
      case EFSTYPE_SDCARD2_ROOT:
         EME_UTILS_STRCAT(pcDestPath, pcsd2Path);
         break;
      case EFSTYPE_ACT_ROOT_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_UDISK_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\account");
         break;
      case EFSTYPE_ACT_INDEX_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_FILE_ACTINDEX);
         break;
      case EFSTYPE_TEMP_UDISK_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_UDISK_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\temp");
         break;
      case EFSTYPE_TEMP_SDCARD1_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD1_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\temp");
         break;
      case EFSTYPE_TEMP_SDCARD2_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD2_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\temp");
         break;
      case EFSTYPE_UDISK_EML_ROOT_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_UDISK_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\eml");
         break;
      case EFSTYPE_SDCARD1_EML_ROOT_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD1_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\eml");
         break;
      case EFSTYPE_SDCARD2_EML_ROOT_DIR:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD2_ROOT));
         EME_UTILS_STRCAT(pcDestPath, "\\eml");
         break;
         /*email data file*/
      case EFSTYPE_MSG_DATA_FILE_UDISK:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_UDISK_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x\\eml%08x.tmf");
         break;
      case EFSTYPE_MSG_DATA_FILE_SDCARD1:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD1_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x\\eml%08x.tmf");
         break;
      case EFSTYPE_MSG_DATA_FILE_SDCARD2:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD2_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x\\eml%08x.tmf");
         break;
      case EFSTYPE_MSG_DATA_FOLDER_UDISK:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_UDISK_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x");
         break;
      case EFSTYPE_MSG_DATA_FOLDER_SDCARD1:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD1_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x");
         break;
      case EFSTYPE_MSG_DATA_FOLDER_SDCARD2:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_SDCARD2_EML_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\eml%08x");
         break;
         /*account info file*/
      case EFSTYPE_ACT_DATA_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\"EMEFS_FILE_ACTDATA);
         break;
      case EFSTYPE_BOX_INDEX_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\"EMEFS_FILE_BOXINDEX);
         break;
      case EFSTYPE_MSG_INDEX_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\"EMEFS_FILE_MSGINDEX);
         break;
      case EFSTYPE_ACT_UIDL_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\uidl");
         break;
      case EFSTYPE_ACT_UID_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\uid");
         break;
      case EFSTYPE_ACT_SIG_FILE:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME"\\sig");
         break;
      case EFSTYPE_ACT_DATA_FOLDER:
         EME_UTILS_STRCAT(pcDestPath, EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR));
         EME_UTILS_STRCAT(pcDestPath, "\\"EMEFS_ACCONT_NAME);
         break;
      }
      eFSType++;
   }
}


WE_VOID EME_DataAccess_ClearPathMgr(WE_VOID)
{
    EME_UTILS_MEMSET(&gs_stPathMgr, 0x00, sizeof(St_PathMgr));
    EME_UTILS_MEMSET(&gs_stActCashMgr, 0x00, sizeof(St_ActCasheMgr));
    return;
}


