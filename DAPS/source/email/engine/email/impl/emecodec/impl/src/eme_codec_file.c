
#include "eme_codec_file.h"
#include "eme_utils_api.h"

typedef struct _St_PartIndexList
{
   WE_UINT32 uiOffset;
   WE_UINT32 uiSize;
   struct _St_PartIndexList *next;
}St_PartIndexList;

typedef struct _St_PartIndexTable
{
   WE_CHAR           *pcBoundary;
   WE_UINT32         uiCount;
   St_PartIndexList  *pstPartIndexList;
}St_PartIndexTable;

FILE_HANDLE EMEFile_GetHeadData(FILE_HANDLE pIFile, WE_VOID *pvMemHdl, WE_UINT32 uiBlockSize, WE_INT32 *piHeadSize)
{
   WE_CHAR     *pcBuffer = NULL;
   WE_INT32    iResult = EME_SUCCESS;
   WE_CHAR     *pcTmp = 0;
   WE_INT32    iCount = 0;
   WE_BOOL     bIsEnd = FALSE;
   WE_INT32    iOffset = 0;
   WE_BOOL     bFound = FALSE;
   
   /*file info*/
   WE_UINT32   uiFileSize = 0;
   WE_UINT32   uiHadRead = 0;
   WE_UINT32   uiRead = 0;
   WE_INT32    iReadFileSizeOneTime = 0;
   WE_UINT32   uiStartPoint = 0;
   WE_UINT32   uiFieldSize = 0;//"4: 0D0A0D0A; 2: 0A0A"

   WE_CHAR     *pcLF[2] = {0};
   WE_UINT32   uiIndex = 0;
   
   EME_UTILS_LOG_INFO(("EMEFile_GetHeadData: START pIFile = 0x%x, piHeadSize= 0x%x ", pIFile, piHeadSize));
   
   if(NULL == pIFile || NULL == piHeadSize)
   {
      EME_UTILS_LOG_INFO(("EMEFile_GetHeadData: badparm pIFile = 0x%x, piHeadSize= 0x%x ", pIFile, piHeadSize));
      return NULL;
   }
   
   do 
   {
      iReadFileSizeOneTime = EME_FILE_READ_ONE_TIME;
      if(iReadFileSizeOneTime < 1024)
      {
         iReadFileSizeOneTime = 1024;
      }
      
      uiStartPoint = EME_FileTell(pIFile);
      EME_UTILS_LOG_INFO(("EMEFile_GetHeadData: EME_FileTell  uiStartPoint= %d ", uiStartPoint ));
      /*get file size*/
      uiFileSize = EME_FileLength(pIFile);
      EME_UTILS_LOG_INFO(("EMEFile_GetHeadData: EME_FileTell  uiFileSize= %d ", uiFileSize ));
      iResult = (uiFileSize == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "get file size failed");
      
      if(uiBlockSize == 0)
      {
         uiBlockSize = uiFileSize;
      }
      
      *piHeadSize = 0;
      pcBuffer = EME_UTILS_MALLOC(pvMemHdl, (iReadFileSizeOneTime * 2 + 1));
      EME_UTILS_BREAKNULL(pcBuffer, iResult, EME_NOMEMORY, "no memory for buffer");
      EME_UTILS_MEMSET(pcBuffer, 0, (iReadFileSizeOneTime * 2) + 1);
      
      /*Read data from file to fine the <CRLF>*/
      uiRead = EME_FileRead(pIFile, pcBuffer, iReadFileSizeOneTime * 2);
      
      EME_UTILS_LOG_INFO(("EMEFile_GetHeadData: EME_FileRead  uiRead= %d ", uiRead ));	  
      iResult = (uiRead == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "Read file failed");
      
      uiHadRead += uiRead;
      iCount += 2;
      
      bIsEnd = ((uiStartPoint + uiHadRead) >= uiFileSize) ? TRUE : FALSE;
      
      EME_UTILS_LOG_INFO(("EMEFile_GetHeadData:  pcBuffer= %s", pcBuffer));	
      
      /*find the <CRLF>*/
      pcLF[0] = EME_UTILS_STRSTR(pcBuffer, "\n\r\n");
      pcLF[1] = EME_UTILS_STRSTR(pcBuffer, "\n\n");

      if(pcLF[0] == NULL && pcLF[1] == NULL)
      {
         uiFieldSize = 0;
         pcTmp = NULL;
      }
      else if(pcLF[0] == NULL)
      {
         uiFieldSize = 2;
         pcTmp = pcLF[1];
      }
      else if(pcLF[1] == NULL)
      {
         uiFieldSize = 3;
         pcTmp = pcLF[0];
      }
      else
      {
         pcTmp = pcLF[0] > pcLF[1] ? pcLF[1] : pcLF[0];
         uiFieldSize = (pcLF[0] > pcLF[1] ? 2 : 3);
      }

//       pcTmp = EME_UTILS_STRSTR(pcBuffer, "\r\n\r\n");
//       if(pcTmp == NULL)
//       {
//          pcTmp = EME_UTILS_STRSTR(pcBuffer, "\n\n");
//          uiFieldSize = 2;
//       }
//       else
//       {
//          uiFieldSize = 4;
//       }
      
      if(pcTmp != NULL)
      {
         iOffset = (WE_INT32)uiRead;
         /*seek the file pointer to the first*/
         iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
         EME_UTILS_BREAKIF(iResult, "seek file failed");
         
         if((WE_UINT32)(pcTmp - pcBuffer + uiFieldSize) > uiBlockSize)
         {
            *piHeadSize = uiBlockSize;
         }
         else
         {
            *piHeadSize = pcTmp - pcBuffer + uiFieldSize;
         }
         break;
      }
      else if(uiHadRead >= uiBlockSize)
      {
         iOffset = (WE_INT32)uiRead;
         /*seek the file pointer to the first*/
         iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
         EME_UTILS_BREAKIF(iResult, "seek file failed");
         *piHeadSize = uiBlockSize;
         break;
      }
      
      while(!bIsEnd)
      {
         EME_UTILS_MEMCPY(pcBuffer, (pcBuffer + iReadFileSizeOneTime), iReadFileSizeOneTime);
         EME_UTILS_MEMSET((pcBuffer + iReadFileSizeOneTime), 0, iReadFileSizeOneTime);
         pcTmp = NULL;
         
         uiRead = EME_FileRead(pIFile, (pcBuffer + iReadFileSizeOneTime), iReadFileSizeOneTime);
         iResult = (uiRead == 0) ? EME_FAILED : EME_SUCCESS;
         EME_UTILS_BREAKIF(iResult, "Read file failed");
         
         uiHadRead += uiRead;
         iCount++;
         
         bIsEnd = ((uiStartPoint + uiHadRead) >= uiFileSize) ? TRUE : FALSE;
         
         pcLF[0] = EME_UTILS_STRSTR(pcBuffer, "\n\r\n");
         pcLF[1] = EME_UTILS_STRSTR(pcBuffer, "\n\n");
         
         if(pcLF[0] == NULL && pcLF[1] == NULL)
         {
            uiFieldSize = 0;
            pcTmp = NULL;
         }
         else if(pcLF[0] == NULL)
         {
            uiFieldSize = 2;
            pcTmp = pcLF[1];
         }
         else if(pcLF[1] == NULL)
         {
            uiFieldSize = 3;
            pcTmp = pcLF[0];
         }
         else
         {
            pcTmp = pcLF[0] > pcLF[1] ? pcLF[1] : pcLF[0];
            uiFieldSize = (pcLF[0] > pcLF[1] ? 2 : 3);
         }
         
//          pcTmp = EME_UTILS_STRSTR(pcBuffer, "\r\n\r\n");
//          if(pcTmp == NULL)
//          {
//             pcTmp = EME_UTILS_STRSTR(pcBuffer, "\n\n");
//             uiFieldSize = 2;
//          }
//          else
//          {
//             uiFieldSize = 4;
//          }
         
         if(pcTmp != NULL)
         {
            iOffset = iReadFileSizeOneTime * (iCount - 1) + (WE_INT32)uiRead;
            iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
            EME_UTILS_BREAKIF(iResult, "seek file failed");
            *piHeadSize =  iReadFileSizeOneTime * (iCount - 2) + (pcTmp - pcBuffer + uiFieldSize);
            bFound = TRUE;
            break;
         }
         else if(uiHadRead >= uiBlockSize)
         {
            iOffset = iReadFileSizeOneTime * (iCount - 1) + (WE_INT32)uiRead;
            iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
            EME_UTILS_BREAKIF(iResult, "seek file failed");
            *piHeadSize =  uiBlockSize;
            bFound = TRUE;
            break;
         }
      }
      EME_UTILS_BREAKIF(iResult, "Find the <CRLF> failed");
      
      if(!bFound)
      {
         /*The whole file is message head*/
         iOffset = iReadFileSizeOneTime * (iCount - 1) + (WE_INT32)uiRead;
         iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
         EME_UTILS_BREAKIF(iResult, "seek file failed");
         *piHeadSize =  iOffset;
      }
      
   } while(0);
   
   EME_UTILS_FREEIF(pvMemHdl, pcBuffer);
   
   if(EME_SUCCESS != iResult)
   {
      return NULL;
   }
   
   return pIFile;
}


WE_INT32 EMEFile_ReadDataFromFile(FILE_HANDLE pIFile, WE_VOID *pvData, WE_UINT32 uiSize)
{
   WE_INT32 iResult = EME_SUCCESS;
   WE_UINT32 uiRead = 0;

   if(NULL == pIFile || NULL == pvData || uiSize == 0)
   {
      return EME_INVALID_DATA;
   }

   do 
   {
      uiRead = EME_FileRead(pIFile, pvData, uiSize);
      iResult = (uiRead == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "read data from file failed");

      //iResult = EME_FileSeek(pIFile,  -(WE_INT32)uiRead, EME_SEEK_CURRENT);
      //EME_UTILS_BREAKIF(iResult, "seek file pointer failed");
   } while(0);

   return iResult;
}


WE_VOID  EMFile_FreePartIndexList(St_PartIndexList *pstIndexTableFirst, WE_VOID *pvMemHdl)
{
   St_PartIndexList *pstIndexTableTmp = NULL;
   
   while(NULL != pstIndexTableFirst)
   {
      pstIndexTableTmp = pstIndexTableFirst;
      pstIndexTableFirst = pstIndexTableFirst->next;
      EME_UTILS_FREEIF(pvMemHdl, pstIndexTableTmp);
   }
}

WE_VOID  EMFile_FreePartIndexTable(PARTINDEXTB pvPartIndexTable, WE_VOID *pvMemHdl)
{
   St_PartIndexTable *pstPartIndexTable = (St_PartIndexTable *)pvPartIndexTable;

   if(NULL != pstPartIndexTable)
   {
      EME_UTILS_FREEIF(pvMemHdl, pstPartIndexTable->pcBoundary);
      EMFile_FreePartIndexList(pstPartIndexTable->pstPartIndexList, pvMemHdl);
      EME_UTILS_FREEIF(pvMemHdl,pstPartIndexTable);
   }
}

PARTINDEXTB EMEFile_GetPartIndexTable(FILE_HANDLE pIFile, 
                                      WE_VOID *pvMemHdl, 
                                      WE_CHAR *pcBoundary, 
                                      WE_UINT32 uiBodySize, 
                                      WE_INT32 *piPartCount)
{
   WE_INT32    iResult = EME_SUCCESS;

   WE_CHAR     *pcBuffer = NULL;
   WE_CHAR     *pcBufferTmp = NULL;
   WE_CHAR     *pcTmp = 0;
   WE_CHAR     *pcTmpEnd = 0;
   WE_BOOL     bIsEnd = FALSE;
   WE_BOOL     bWantSetTrue = FALSE;
   WE_INT32    iOffset = 0;
   WE_INT32    iIndexTmp = 0;

   /*index table info*/
   St_PartIndexTable *pstPartIndexTable = NULL;

   St_PartIndexList *pstIndexTableNew = NULL;
   St_PartIndexList *pstIndexTableLast = NULL;
   St_PartIndexList *pstIndexTableFirst = NULL;

   /*boundary info*/
   WE_CHAR     *pcBoundaryLast = NULL;
   WE_CHAR     *pcBoundaryEnd = NULL;

   /*file info*/
   WE_UINT32    uiRead = 0;
   WE_UINT32    uiHadRead = 0;
   WE_UINT32    uiFileSize = 0;
   WE_UINT32    uiStartPoint = 0;

   /*read size from file at once*/
   WE_INT32     iReadSizeOneTime = 0;


   if(NULL == pIFile || NULL == piPartCount || NULL == pcBoundary)
   {
      return NULL;
   }

   do 
   {
      uiStartPoint = EME_FileTell(pIFile);
      
      /*determine the read size at once*/
      if(EME_FILE_READ_ONE_TIME < EME_UTILS_STRLEN(pcBoundary))
      {
         iReadSizeOneTime = EME_UTILS_STRLEN(pcBoundary);
      }
      else
      {
         iReadSizeOneTime = EME_FILE_READ_ONE_TIME;
      }

      /*get file size*/
      uiFileSize = EME_FileLength(pIFile);
      iResult = (uiFileSize == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "get file size failed");
      
      if(uiBodySize == 0)
      {
         uiBodySize = uiFileSize;
      }

      /*allocate new memory for index table info*/
      pstPartIndexTable = EME_UTILS_MALLOC(pvMemHdl, sizeof(St_PartIndexTable));
      EME_UTILS_BREAKNULL(pstPartIndexTable, iResult, EME_NOMEMORY, "no memory");
      EME_UTILS_MEMSET(pstPartIndexTable, 0, sizeof(St_PartIndexTable));

      pstPartIndexTable->pcBoundary = EME_UTILS_STRDUP(pvMemHdl, pcBoundary);
      EME_UTILS_BREAKNULL(pstPartIndexTable->pcBoundary, iResult, EME_NOMEMORY, "strdup boundry failed");

      pcBoundaryEnd = EME_UTILS_MALLOC(pvMemHdl, EME_UTILS_STRLEN(pcBoundary) + 3);/*2:--, 1:\0*/
      EME_UTILS_BREAKNULL(pcBoundaryEnd, iResult, EME_NOMEMORY, "no memory for end");
      EME_UTILS_MEMSET(pcBoundaryEnd, 0, EME_UTILS_STRLEN(pcBoundary) + 3);

      EME_UTILS_SPRINTF(pcBoundaryEnd, "%s--", pcBoundary);
      
   	pcBuffer = EME_UTILS_MALLOC(pvMemHdl, iReadSizeOneTime * 2 + 1);
      EME_UTILS_BREAKNULL(pcBuffer, iResult, EME_NOMEMORY, "no memory for buffer");
      EME_UTILS_MEMSET(pcBuffer, 0, (iReadSizeOneTime * 2) + 1);

      /*Read data from file to fine the <CRLF>*/
      uiRead = EME_FileRead(pIFile, pcBuffer, iReadSizeOneTime * 2);
      iResult = (uiRead == 0) ? EME_FAILED : EME_SUCCESS;
      EME_UTILS_BREAKIF(iResult, "Read file failed");

      uiHadRead += uiRead;

      iResult = EME_FileSeek(pIFile, -(WE_INT32)uiRead, EME_SEEK_CURRENT);
      EME_UTILS_BREAKIF(iResult, "seek file pointer failed");

      bIsEnd = ((uiHadRead + uiStartPoint) >= uiFileSize) ? TRUE : FALSE;
      
      do 
      {
         if(bWantSetTrue)
         {
            bIsEnd = TRUE;
         }
         
         if(pcBoundaryLast != NULL && (pcBoundaryLast - iReadSizeOneTime) > pcBuffer)
         {
            pcBufferTmp = pcBoundaryLast - iReadSizeOneTime + EME_UTILS_STRLEN(pcBoundary);
         }
         else
         {
            pcBufferTmp = pcBuffer;
         }

         pcBoundaryLast = NULL;
         pcTmp = pcBuffer;

         while(NULL != pcTmp && (pcTmp - pcBuffer) <= iReadSizeOneTime * 2)
         {
            pcTmp = NULL;
            pcTmpEnd = NULL;

            /*find the <CRLF>*/
            pcTmp = EME_UTILS_STRSTR(pcBufferTmp, pcBoundary);
            pcTmpEnd = EME_UTILS_STRSTR(pcBufferTmp, pcBoundaryEnd);
            if(pcTmp != NULL && pcTmp != pcTmpEnd)
            {
               pstIndexTableNew = EME_UTILS_MALLOC(pvMemHdl, sizeof(St_PartIndexList));
               EME_UTILS_BREAKNULL(pstIndexTableNew, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_MEMSET(pstIndexTableNew, 0, sizeof(St_PartIndexList));

               iOffset = pcTmp - pcBuffer;
               if(iOffset == 0)
               {
                  pstIndexTableNew->uiOffset = EME_FileTell(pIFile);
//                  iResult = (pstIndexTableNew->uiOffset == 0) ? EME_FAILED : EME_SUCCESS;
//                  EME_UTILS_BREAKIF(iResult, "get file pointer failed");
                  pcBoundaryLast = pcTmp;
               }
               else
               {
                  /*seek the file pointer to the first*/
                  iResult = (WE_INT32)EME_FileSeek(pIFile, iOffset, EME_SEEK_CURRENT);
                  EME_UTILS_BREAKIF(iResult, "seek file failed");

                  pstIndexTableNew->uiOffset = EME_FileTell(pIFile);
//                  iResult = (pstIndexTableNew->uiOffset == 0) ? EME_FAILED : EME_SUCCESS;
//                  EME_UTILS_BREAKIF(iResult, "get file pointer failed");

                  /*seek the file pointer to the first*/
                  iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
                  EME_UTILS_BREAKIF(iResult, "seek file failed");
                  pcBoundaryLast = pcTmp;
               }

               if(NULL == pstIndexTableFirst)
               {
                  pstIndexTableFirst = pstIndexTableNew;
                  pstIndexTableLast = pstIndexTableFirst;
                  pstIndexTableNew = NULL;
               }
               else if(pstIndexTableLast != NULL)
               {
                  pstIndexTableLast->next = pstIndexTableNew;
                  pstIndexTableLast = pstIndexTableNew;
                  pstIndexTableNew = NULL;
               }
               else
               {
                  iResult = EME_FAILED;
                  EME_UTILS_BREAKIF(iResult, "create index table failed");
               }
               
               iIndexTmp++;
               pcBufferTmp = pcTmp + EME_UTILS_STRLEN(pcBoundary);
            }
            else if(pcTmpEnd != NULL || bIsEnd || uiHadRead >= uiBodySize)
            {
               pstIndexTableNew = EME_UTILS_MALLOC(pvMemHdl, sizeof(St_PartIndexList));
               EME_UTILS_BREAKNULL(pstIndexTableNew, iResult, EME_NOMEMORY, "no memory");
               EME_UTILS_MEMSET(pstIndexTableNew, 0, sizeof(St_PartIndexList));

               if(pcTmpEnd != NULL)
               {
                  iOffset = pcTmpEnd - pcBuffer;
               }
               else if(bIsEnd)
               {
                  iOffset = EME_UTILS_STRLEN(pcBuffer);
               }
               else
               {
                  iOffset = (2 * iReadSizeOneTime) - (uiHadRead - uiBodySize);
               }
               iResult = (WE_INT32)EME_FileSeek(pIFile, iOffset, EME_SEEK_CURRENT);
               EME_UTILS_BREAKIF(iResult, "seek file failed");
               
               pstIndexTableNew->uiOffset = EME_FileTell(pIFile);
//               iResult = (pstIndexTableNew->uiOffset == 0) ? EME_FAILED : EME_SUCCESS;
//               EME_UTILS_BREAKIF(iResult, "get file pointer failed");
               
               iResult = (WE_INT32)EME_FileSeek(pIFile, -iOffset, EME_SEEK_CURRENT);
               EME_UTILS_BREAKIF(iResult, "seek file failed");
               bIsEnd = TRUE;

               if(pstIndexTableFirst == NULL)
               {
                  pstIndexTableFirst = pstIndexTableNew;
                  pstIndexTableLast = pstIndexTableNew;
                  pstIndexTableNew = NULL;
               }
               else if(pstIndexTableLast != NULL)
               {
                  pstIndexTableLast->next = pstIndexTableNew;
                  pstIndexTableLast = pstIndexTableNew;
                  pstIndexTableNew = NULL;
               }
               else
               {
                  iResult = EME_FAILED;
               }
               
               break;
            }
         }

         EME_UTILS_BREAKIF(iResult, "find boundry failed");
         
         if(!bIsEnd)
         {
            iResult = EME_FileSeek(pIFile, (WE_INT32)uiRead, EME_SEEK_CURRENT);
            EME_UTILS_BREAKIF(iResult, "seek file pointer failed");

            iResult = EME_FileSeek(pIFile, -iReadSizeOneTime, EME_SEEK_CURRENT);
            EME_UTILS_BREAKIF(iResult, "seek file pointer failed");
            
            uiHadRead -= iReadSizeOneTime;

            EME_UTILS_MEMSET(pcBuffer, 0, iReadSizeOneTime * 2 + 1);
            
            uiRead = EME_FileRead(pIFile, pcBuffer, iReadSizeOneTime * 2);
            iResult = (uiRead == 0) ? EME_FAILED : EME_SUCCESS;
            EME_UTILS_BREAKIF(iResult, "Read file failed");

            uiHadRead += uiRead;

            iResult = EME_FileSeek(pIFile, -(WE_INT32)uiRead, EME_SEEK_CURRENT);
            EME_UTILS_BREAKIF(iResult, "seek file pointer failed");

            bWantSetTrue = ((uiHadRead + uiStartPoint) >= uiFileSize) ? TRUE : FALSE;
         }
      } while(!bIsEnd);

      EME_UTILS_BREAKIF(iResult, "find the part failed");
      
      *piPartCount = iIndexTmp;
      pstPartIndexTable->uiCount = (WE_UINT32)iIndexTmp;
      
      pstIndexTableLast = pstIndexTableFirst;
      while(pstIndexTableLast != NULL && pstIndexTableLast->next != NULL)
      {
         pstIndexTableLast->uiSize = pstIndexTableLast->next->uiOffset - pstIndexTableLast->uiOffset;
         pstIndexTableLast = pstIndexTableLast->next;
      }

      pstPartIndexTable->pstPartIndexList = pstIndexTableFirst;

   } while(0);
   

   EME_UTILS_FREEIF(pvMemHdl, pcBuffer);
   EME_UTILS_FREEIF(pvMemHdl, pcBoundaryEnd);

   if(iResult != EME_SUCCESS)
   {
      EME_UTILS_FREEIF(pvMemHdl, pstIndexTableNew);
      EMFile_FreePartIndexTable(pvMemHdl, pstPartIndexTable);
      pstPartIndexTable = NULL;
      EMFile_FreePartIndexTable(pvMemHdl, pstIndexTableFirst);
      pstIndexTableFirst = NULL;
   }
   
   return pstPartIndexTable;
}


FILE_HANDLE EMEFile_GetOnePartByIndex(FILE_HANDLE pIFile, PARTINDEXTB pvPartIndexTable, WE_INT32 iIndex, WE_CHAR *pcBoundary, WE_INT32 *piDataSize)
{
   St_PartIndexTable *pstPartIndexTable = (St_PartIndexTable*)pvPartIndexTable;
   St_PartIndexList  *pstPartIndexList = NULL;

   WE_INT32               iResult = EME_SUCCESS;
   WE_INT32               iIndexTmp = 0;
   WE_BOOL           bFound = FALSE;

   if(NULL == pstPartIndexTable || NULL == pcBoundary || NULL == piDataSize || iIndex < 0 || pIFile == NULL)
   {
      return NULL;
   }

   if(iIndex >= (WE_INT32)pstPartIndexTable->uiCount || pstPartIndexTable->pcBoundary == NULL)
   {
      return NULL;
   }

   do 
   {
      *piDataSize = 0;
      iResult = EME_UTILS_MEMCMP(pcBoundary, pstPartIndexTable->pcBoundary, EME_UTILS_STRLEN(pcBoundary));
      iResult = (iResult == 0) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "the boundry is not match");

      pstPartIndexList = pstPartIndexTable->pstPartIndexList;
      while(pstPartIndexList->next != NULL)
      {
         if(iIndexTmp == iIndex)
         {
            iResult = EME_FileSeek(pIFile, (WE_INT32)pstPartIndexList->uiOffset, EME_SEEK_START);
            EME_UTILS_BREAKIF(iResult, "seek to start failed");
            *piDataSize = (WE_INT32)(pstPartIndexList->uiSize);
            bFound = TRUE;
            break;
         }
         pstPartIndexList = pstPartIndexList->next;
         iIndexTmp++;
      }

      if(!bFound)
      {
         return NULL;
      }

   } while(0);

   return pIFile;
}

WE_INT32  EMEFile_GetOnePartInfo(PARTINDEXTB pvPartIndexTable, WE_INT32 iIndex,  WE_CHAR *pcBoundary, WE_INT32 *piOffset, WE_INT32 *piDataSize)
{
   St_PartIndexTable *pstPartIndexTable = (St_PartIndexTable*)pvPartIndexTable;
   St_PartIndexList  *pstPartIndexList = NULL;

   WE_INT32               iResult = EME_SUCCESS;
   WE_INT32               iIndexTmp = 0;
   WE_BOOL           bFound = FALSE;

   if(NULL == pstPartIndexTable || NULL == pcBoundary || NULL == piOffset || iIndex < 0 || NULL == piDataSize)
   {
      return EME_INVALID_DATA;
   }

   if(iIndex >= (WE_INT32)pstPartIndexTable->uiCount || pstPartIndexTable->pcBoundary == NULL)
   {
      return EME_INVALID_DATA;
   }

   do 
   {
      *piOffset = 0;
      iResult = EME_UTILS_MEMCMP(pcBoundary, pstPartIndexTable->pcBoundary, EME_UTILS_STRLEN(pcBoundary));
      iResult = (iResult == 0) ? EME_SUCCESS : EME_FAILED;
      EME_UTILS_BREAKIF(iResult, "the boundry is not match");

      pstPartIndexList = pstPartIndexTable->pstPartIndexList;
      while(pstPartIndexList->next != NULL)
      {
         if(iIndexTmp == iIndex)
         {
            *piOffset = (WE_INT32)(pstPartIndexList->uiOffset);
            *piDataSize = (WE_INT32)(pstPartIndexList->uiSize);
            bFound = TRUE;
            break;
         }
         pstPartIndexList = pstPartIndexList->next;
         iIndexTmp++;
      }

      if(!bFound)
      {
         iResult = EME_FAILED;
      }

   } while(0);

   return iResult;
}
