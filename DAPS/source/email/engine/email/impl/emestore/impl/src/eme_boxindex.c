#include "eme_boxindex_api.h"
#include "eme_msgstore_api.h"
#include "eme_dataaccess_priv.h"
#include "EMS_Store_API.h"
#include "EMS_Store_Priv.h"
#include "we_msg.h"

/*Initer*/
#define EME_BOXOPT_ID                    (WEMSG_OPT_32BIT+0x01)
#define EME_BOXOPT_TOTAL                 (WEMSG_OPT_32BIT+0x02)
#define EME_BOXOPT_UNREAD                (WEMSG_OPT_32BIT+0x03)
#define EME_BOXOPT_STATE                 (WEMSG_OPT_32BIT+0x04)
#define EME_BOXOPT_ASY_CRC               (WEMSG_OPT_32BIT+0x05) /*与msgidex.tmf中相对应的crc值，用来判断是否需要同步文件*/
#define EME_BOXOPT_UD_SIZE               (WEMSG_OPT_32BIT+0x06) /*udisk size*/
#define EME_BOXOPT_SD1_SIZE              (WEMSG_OPT_32BIT+0x07) /*sdcard1 size*/
#define EME_BOXOPT_SD2_SIZE              (WEMSG_OPT_32BIT+0x08) /*sdcard2 size*/

/*Wstr*/
#define EME_BOXOPT_NAME                  (WEMSG_OPT_WSZ+0x01)

static WE_INT32 EME_BoxIndex_GetNextAvailableBoxId(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwBoxId)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     *pcBuf = NULL;
   WE_UINT32   auiBoxId[EMS_BOXINFO_MAX_NUMBER] = {0};
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   
   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get imsg failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed");
      
      if(0 == uiCnt)
      {
         *pdwBoxId = 1;
      }
      else if(uiCnt == EMS_BOXINFO_MAX_NUMBER)
      {
         *pdwBoxId = 0;
         nErr = EME_MEETMAX;
      }
      else
      {
         while(uiIndex < uiCnt)
         {
            IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex);
            if(NULL != pIMsgValue)
            {
               WEMsgOpt stOpt = {0};
               
               nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_ID, &stOpt);
               if(nErr == EME_SUCCESS)
               {
                  auiBoxId[(WE_UINT32)stOpt.pVal - 1] = 1;
               }
               IWEMSGVALUE_Release(pIMsgValue);
            }
            uiIndex++;
         }
         
         uiIndex = 0;
         while(uiIndex < EMS_BOXINFO_MAX_NUMBER)
         {
            if(auiBoxId[uiIndex] == 0)
            {
               *pdwBoxId = uiIndex + 1;
               break;
            }
            uiIndex++;
         }
      }
      
   } while (0);
   
   if(NULL != pIMsg)
   {
      IWEMSG_Release(pIMsg);
   }
   
   EME_UTILS_FREEIF(NULL, pcBuf);
   
   return nErr;
}

WE_INT32 EME_BoxIndex_CreateEmptyBoxIndex(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsg       *pIMsg = NULL;
   IWEMsgValue  *pIMsgValue = 0;
   WEMsgOpt     stOpt[2] = {0};
   WE_CHAR      acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR      *pcBoxIndexFileBuf = NULL;
   WE_UINT32    uiBoxIndexFileSize = 0;

   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);

      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create msg failed!!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value failed!!");

      stOpt[0].iId  = EME_BOXOPT_ASY_CRC;
      stOpt[0].pVal = (WE_VOID*)0;
      stOpt[1].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
      EME_UTILS_BREAKIF(nErr, "add opt failed!!!");

      nErr = IWEMSG_AddHdrValue(pIMsg, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add part failed!!");
      
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcBoxIndexFileBuf, &uiBoxIndexFileSize);
      EME_UTILS_BREAKIF(nErr, "encode msg failed!!!");

      nErr = EME_DataAccess_CreateFileByBuffer(hMgr, acBoxIndexFile, pcBoxIndexFileBuf, uiBoxIndexFileSize);
      EME_UTILS_BREAKIF(nErr, "create box index file failed!!");

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}


WE_INT32 EME_BoxIndex_GetBoxId(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT16 *pwszBoxName, WE_UINT32 *pdwBoxId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   
   if (NULL == pwszBoxName || NULL == pdwBoxId)
   {
      return EME_BADPARM;
   }
   
   
   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get box index part count failed");
      
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            MsgOpt stOpt[2] = {0};
            
            if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_NAME, (WEMsgOpt*)&stOpt[0]) == EME_SUCCESS)
            {
               if(EME_UTILS_WSTRCMP(stOpt[0].pVal, pwszBoxName) == 0)
               {
                  nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_ID, (WEMsgOpt*)&stOpt[0]);
                  EME_UTILS_BREAKIF(nErr, "get box id failed");
                  
                  *pdwBoxId = (WE_UINT32)stOpt[0].pVal;
                  break;
               }
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         
         uiIndex++;
      }
      
   } while (0);
   
   if(pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_BoxIndex_UpdateBox(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId, MsgOpt *pstOpts)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WE_CHAR     *pcBoxIndexBuf = NULL;
   WE_UINT32   uiBoxIndexSize = 0;
   WE_BOOL     bUpdate = FALSE;
   
   if (0 == dwActId || 0 == dwBoxId)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /*get box index file path*/
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);
      
      /*get box index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");
      
      /*get box count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get box index part count failed");
      
      /*find box by id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt[2] = {0};
            
            if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_ID, (WEMsgOpt*)&stOpt[0]) == EME_SUCCESS)
            {
               if(dwBoxId == (WE_UINT32)stOpt[0].pVal)
               {
                  /*update box info*/
                  nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)pstOpts);
                  EME_UTILS_BREAKIF(nErr, "update box index opt failed!!");

                  IWEMSGVALUE_Release(pIMsgValue);
                  pIMsgValue = NULL;
                  bUpdate = TRUE;
                  break;
               }
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      if(bUpdate)
      {
         nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcBoxIndexBuf, &uiBoxIndexSize);
         EME_UTILS_BREAKIF(nErr, "encode box index file buff failed");

         nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acBoxIndexFile, pcBoxIndexBuf, uiBoxIndexSize);
         EME_UTILS_BREAKIF(nErr, "update box index file failed!!");
      }
      
   } while (0);
   
   if(pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_BoxIndex_CreateBox(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT16 *pwcBoxName, WE_UINT32 dwBoxState, WE_UINT32 *pdwBoxId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcBoxIndexBuf = NULL;
   WE_UINT32   uiBoxIndexSize = 0;
   WE_UINT32   uiBoxId = 0;
   WEMsgOpt    stOpt[10] = {0};
   WE_INT32    nPos = WEMSG_END_POSTION;

   if(0 == dwActId || NULL == pwcBoxName || 0 == dwBoxState || NULL == pdwBoxId)
   {
      return EME_BADPARM;
   }

   /*box info save at udisk only!!*/
//   if(!EME_CheckUDiskFreeSpace(hMgr, EMS_LEAST_BOX_SPACE))
//   {
//      return EME_NOSTORAGE;
//   }
   
   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);

      nErr = EME_BoxIndex_GetNextAvailableBoxId(hMgr, dwActId, &uiBoxId);
      EME_UTILS_BREAKIF(nErr, "get next box id failed");

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value handle failed!!");

      stOpt[0].iId  = EME_BOXOPT_ID;
      stOpt[0].pVal = (WE_VOID*)uiBoxId;
      stOpt[1].iId  = EME_BOXOPT_NAME;
      stOpt[1].pVal = (WE_VOID*)pwcBoxName;
      stOpt[2].iId  = EME_BOXOPT_STATE;
      stOpt[2].pVal = (WE_VOID*)dwBoxState;
      stOpt[3].iId  = EME_BOXOPT_UNREAD;
      stOpt[3].pVal = (WE_VOID*)0;
      stOpt[4].iId  = EME_BOXOPT_TOTAL;
      stOpt[4].pVal = (WE_VOID*)0;
      stOpt[5].iId  = EME_BOXOPT_UD_SIZE;
      stOpt[5].pVal = (WE_VOID*)0;
      stOpt[6].iId  = EME_BOXOPT_SD1_SIZE;
      stOpt[6].pVal = (WE_VOID*)0;
      stOpt[7].iId  = EME_BOXOPT_SD2_SIZE;
      stOpt[7].pVal = (WE_VOID*)0;
      stOpt[8].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
      EME_UTILS_BREAKIF(nErr, "add opt failed!!");

      nErr = IWEMSG_AddPartValue(pIMsg, pIMsgValue, &nPos);
      EME_UTILS_BREAKIF(nErr, "add part value failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcBoxIndexBuf, &uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode box index info failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acBoxIndexFile, pcBoxIndexBuf, uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "update box index file failed!!");

      *pdwBoxId = uiBoxId;

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

static WE_VOID EME_BoxIndex_DeleteAllMailByBoxId(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WEMsgOpt    stOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;

   if(0 == dwActId)
   {
      return;
   }

   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "open msg index file failed!!");

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get msg count failed!!");

      while(uiIndex < uiCnt)
      {
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt[0]))
            {
               WE_UINT32 uiEmlId = (WE_UINT32)stOpt[0].pVal;

               if((EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, &stOpt[0])) && (WE_UINT32)stOpt[0].pVal == dwBoxId)
               {
                  EME_MsgStore_RemoveMsg(hMgr, (WE_UINT32)stOpt[0].pVal);
                  IWEMSG_RemovePartValue(pIMsg, uiIndex);
                  IWEMSG_GetPartCount(pIMsg, &uiCnt);
                  IWEMSGVALUE_Release(pIMsgValue);
                  pIMsgValue = NULL;
                  continue;
               }
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      /*encode msg index file buffer*/
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgIndexBuf, &uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode msg index buffer failed!!!");
      
      /*update msg index file*/
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgIndexFile, pcMsgIndexBuf, uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "update msg index file failed!");
      
      EME_BoxIndex_SynBoxInfo(hMgr, dwActId);
      
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return;
}

WE_INT32 EME_BoxIndex_DeleteBox(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcBoxIndexBuf = NULL;
   WE_UINT32   uiBoxIndexSize = 0;

   if(0 == dwBoxId || 0 == dwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt = {0};

            if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_ID, &stOpt) == EME_SUCCESS)
            {
               if(dwBoxId == (WE_UINT32)stOpt.pVal)
               {
                  IWEMSG_RemovePartValue(pIMsg, uiIndex);
                  break;
               }
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex++;
      }

      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "find part failed!!");
      
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcBoxIndexBuf, &uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode box index info failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acBoxIndexFile, pcBoxIndexBuf, uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "update box index file failed!!");

      EME_BoxIndex_DeleteAllMailByBoxId(hMgr, dwActId, dwBoxId);
      
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_BoxIndex_GetBoxNum(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwNum)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   
   if(0 == dwActId || NULL == pdwNum)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      *pdwNum = uiCnt;

   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_BoxIndex_GetBoxInfo(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 uiNum, EMS_BoxInfo *pastBoxInfo)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   nPos = 0;
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};

   if(0 == dwActId || 0 == uiNum || NULL == pastBoxInfo)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get box index file handle failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      while(uiIndex < uiCnt && nPos < uiNum)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            
            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_ID, &stOpt);
            pastBoxInfo[nPos].uiBoxId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_UNREAD, &stOpt);
            pastBoxInfo[nPos].uiUnRead = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_TOTAL, &stOpt);
            pastBoxInfo[nPos].uiTotal = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_STATE, &stOpt);
            pastBoxInfo[nPos].ucState = (WE_UINT8)((WE_UINT32)stOpt.pVal);

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_UD_SIZE, &stOpt);
            pastBoxInfo[nPos].dwUDSize = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_SD1_SIZE, &stOpt);
            pastBoxInfo[nPos].dwSD1Size = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_SD2_SIZE, &stOpt);
            pastBoxInfo[nPos].dwSD2Size = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_BOXOPT_NAME, &stOpt);
            EME_UTILS_MEMCPY(pastBoxInfo[uiIndex].wszFolder, (WE_CHAR*)stOpt.pVal, EME_UTILS_WSTRLEN((WE_UINT16*)stOpt.pVal) * sizeof(WE_UINT16));

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;

            nPos++;
         }
         
         uiIndex++;         
      }
      
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_BoxIndex_SynBoxInfo(FILE_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsgMsg = NULL;
   IWEMsg      *pIMsgBox = NULL;
   IWEMsgValue *pIMsgMsgValue = NULL;
   IWEMsgValue *pIMsgBoxValue = NULL;
   WE_UINT32   uiMsgCnt = 0;
   WE_UINT32   uiBoxCnt = 0;
   WE_UINT32   uiMsgIndex = 0;
   WE_UINT32   uiBoxIndex = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     acBoxIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcBoxIndexBuff = NULL;
   WE_UINT32   uiBoxIndexSize = 0;
   EMS_BoxInfo *pstBoxInfo = NULL;
   WE_UINT32   uiMis = EME_UTILS_GETTIMEMS;
   WE_UINT32   uiMsgIndexCrc = 0;

   do 
   {
      EME_UTILS_SPRINTF(acBoxIndexFile, EMEFS_BOX_INDEX_FILE, dwActId);
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      pIMsgBox = EME_DataAccess_GetIWEMsgByFile(hMgr, acBoxIndexFile);
      EME_UTILS_BREAKNULL(pIMsgBox, nErr, EME_FAILED, "get box index file handle failed!!");

      /*获取box个数，如果不存在box信息，直接返回成功，不需同步*/
      nErr = IWEMSG_GetPartCount(pIMsgBox, &uiBoxCnt);
      EME_UTILS_BREAKIF(nErr, "get box number failed!!");
      
      if(0 == uiBoxCnt)
      {
         break;
      }

      /*解析msgindex.tmf文件,并提取msgindex.tmf文件的crc值*/
      pIMsgMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsgMsg, nErr, EME_FAILED, "get msg index file handle failed!!");
      uiMsgIndexCrc = IWEMSG_GetCrc(pIMsgMsg);

      /*从boxindex.tmf文件中获取msgindex.tmf文件的crc值，用来比较文件是否被修改*/
      if(IWEMSG_GetHdrValue(pIMsgBox, &pIMsgBoxValue) == EME_SUCCESS)
      {
         WEMsgOpt    stOpt[2] = {0};

         /*获取crc值，进行比较，如果获取失败，则添加新的crc值*/
         if(IWEMSGVALUE_GetOpt(pIMsgBoxValue, EME_BOXOPT_ASY_CRC, &stOpt[0]) == EME_SUCCESS)
         {
            /*
              crc相同，不需进行同步，直接返回成功
              crc不同，同步，并更新crc值
            */
            if((WE_UINT32)stOpt->pVal == uiMsgIndexCrc)
            {
               break;
            }
            else
            {
               stOpt[0].iId  = EME_BOXOPT_ASY_CRC;
               stOpt[0].pVal = (WE_VOID*)uiMsgIndexCrc;
               stOpt[1].iId  = WEMSG_OPT_END;
               
               nErr = IWEMSGVALUE_UpdateOpt(pIMsgBoxValue, stOpt);
               EME_UTILS_BREAKIF(nErr, "update crc opt faild!!");
            }
         }
         else
         {
            stOpt[0].iId  = EME_BOXOPT_ASY_CRC;
            stOpt[0].pVal = (WE_VOID*)uiMsgIndexCrc;
            stOpt[1].iId  = WEMSG_OPT_END;
            
            nErr = IWEMSGVALUE_AddOpt(pIMsgBoxValue, stOpt);
            EME_UTILS_BREAKIF(nErr, "update crc opt faild!!");
         }

         IWEMSGVALUE_Release(pIMsgBoxValue);
         pIMsgBoxValue = NULL;
      }
      else
      {
         WEMsgOpt    stOpt[2] = {0};

         /*在boxindex.tmf文件中没有head信息时，添加head信息*/
         IWEMSGVALUE_New(&pIMsgBoxValue);
         EME_UTILS_BREAKNULL(pIMsgBoxValue, nErr, EME_FAILED, "create boxindex header value faild!!");
         
         stOpt[0].iId  = EME_BOXOPT_ASY_CRC;
         stOpt[0].pVal = (WE_VOID*)uiMsgIndexCrc;
         stOpt[1].iId  = WEMSG_OPT_END;
         
         nErr = IWEMSGVALUE_AddOpt(pIMsgBoxValue, stOpt);
         EME_UTILS_BREAKIF(nErr, "update crc opt faild!!");

         nErr = IWEMSG_AddHdrValue(pIMsgBox, pIMsgBoxValue);
         EME_UTILS_BREAKIF(nErr, "add head value faild!");

         IWEMSGVALUE_Release(pIMsgBoxValue);
         pIMsgBoxValue = NULL;
      }

      pstBoxInfo = EME_UTILS_MALLOC(NULL, sizeof(EMS_BoxInfo) * (EMS_BOXINFO_MAX_NUMBER + 1));
      EME_UTILS_BREAKNULL(pstBoxInfo, nErr, EME_NOMEMORY, "no memory!!");
      EME_UTILS_MEMSET(pstBoxInfo, 0x00, sizeof(EMS_BoxInfo) * (EMS_BOXINFO_MAX_NUMBER + 1));

      /*get box info*/
      while(uiBoxIndex < uiBoxCnt)
      {
         WEMsgOpt    stOpt = {0};

         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsgBox, &pIMsgBoxValue, uiBoxIndex))
         {
            WE_UINT32 dwBoxId = 0;
            WE_UINT32 dwBoxStatus = 0;
            WE_UINT16 *pwcBoxName = 0;

            IWEMSGVALUE_GetOpt(pIMsgBoxValue, EME_BOXOPT_ID, &stOpt);
            dwBoxId = (WE_UINT32)stOpt.pVal;

            if(dwBoxId <= EMS_BOXINFO_MAX_NUMBER)
            {
               IWEMSGVALUE_GetOpt(pIMsgBoxValue, EME_BOXOPT_STATE, &stOpt);
               dwBoxStatus = (WE_UINT32)stOpt.pVal;
               
               IWEMSGVALUE_GetOpt(pIMsgBoxValue, EME_BOXOPT_NAME, &stOpt);
               pwcBoxName = (WE_UINT16*)stOpt.pVal;
               
               pstBoxInfo[dwBoxId].uiBoxId = dwBoxId;
               pstBoxInfo[dwBoxId].ucState = dwBoxStatus;
               EME_UTILS_MEMCPY(pstBoxInfo[dwBoxId].wszFolder, pwcBoxName, EME_UTILS_WSTRLEN(pwcBoxName) * sizeof(WE_UINT16));
            }

            IWEMSGVALUE_Release(pIMsgBoxValue);
            pIMsgBoxValue = NULL;
         }
         uiBoxIndex++;
      }

      nErr = IWEMSG_GetPartCount(pIMsgMsg, &uiMsgCnt);
      EME_UTILS_BREAKIF(nErr, "get msg count failed!!");

      while(uiMsgIndex < uiMsgCnt)
      {
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsgMsg, &pIMsgMsgValue, uiMsgIndex))
         {
            WEMsgOpt   stOpt = {0};
            WE_UINT32  dwBoxId = 0;
            WE_UINT32  dwStatus = 0;
            WE_UINT32  dwSize = 0;
            WE_UINT32  dwMsgId = 0;
            WE_BOOL    bHide = FALSE;

            IWEMSGVALUE_GetOpt(pIMsgMsgValue, EME_MSGOPT_BOXID, &stOpt);
            dwBoxId = (WE_UINT32)stOpt.pVal;
            
            IWEMSGVALUE_GetOpt(pIMsgMsgValue, EME_MSGOPT_MSGID, &stOpt);
            dwMsgId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgMsgValue, EME_MSGOPT_MSGHIDE, &stOpt);
            bHide = (WE_BOOL)(WE_UINT32)stOpt.pVal;

            if(dwBoxId <= EMS_BOXINFO_MAX_NUMBER && !bHide)
            {
               IWEMSGVALUE_GetOpt(pIMsgMsgValue, EME_MSGOPT_STATUS, &stOpt);
               dwStatus = (WE_UINT32)stOpt.pVal;
               
               IWEMSGVALUE_GetOpt(pIMsgMsgValue, EME_MSGOPT_STORESIZE, &stOpt);
               dwSize = (WE_UINT32)stOpt.pVal;
               
               if(EMS_SAVE_PLACE_SDCARD1 == EME_GET_PLACE_FROM_MSGID(dwMsgId))
               {
                  pstBoxInfo[dwBoxId].dwSD1Size += dwSize;
               }
               else if(EMS_SAVE_PLACE_SDCARD2 == EME_GET_PLACE_FROM_MSGID(dwMsgId))
               {
                  pstBoxInfo[dwBoxId].dwSD2Size += dwSize;
               }
               else if(EMS_SAVE_PLACE_UDISK == EME_GET_PLACE_FROM_MSGID(dwMsgId))
               {
                  pstBoxInfo[dwBoxId].dwUDSize += dwSize;
               }

               pstBoxInfo[dwBoxId].uiTotal   += 1;
               if(!(EMEMSG_STATUS_READ & dwStatus))
               {
                  pstBoxInfo[dwBoxId].uiUnRead += 1;
               }
            }

            IWEMSGVALUE_Release(pIMsgMsgValue);
            pIMsgMsgValue = NULL;
         }
         uiMsgIndex++;
      }

      IWEMSG_Release(pIMsgMsg);
      pIMsgMsg = NULL;

      uiBoxIndex = 0;
      while(uiBoxIndex < uiBoxCnt)
      {
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsgBox, &pIMsgBoxValue, uiBoxIndex))
         {
            WEMsgOpt   stOpt[6] = {0};
            WE_UINT32  dwBoxId = 0;

            IWEMSGVALUE_GetOpt(pIMsgBoxValue, EME_BOXOPT_ID, &stOpt[0]);
            dwBoxId = (WE_UINT32)stOpt[0].pVal;

            stOpt[0].iId  = EME_BOXOPT_TOTAL;
            stOpt[0].pVal = (WE_VOID*)pstBoxInfo[dwBoxId].uiTotal;
            stOpt[1].iId  = EME_BOXOPT_UNREAD;
            stOpt[1].pVal = (WE_VOID*)pstBoxInfo[dwBoxId].uiUnRead;
            stOpt[2].iId  = EME_BOXOPT_UD_SIZE;
            stOpt[2].pVal = (WE_VOID*)pstBoxInfo[dwBoxId].dwUDSize;
            stOpt[3].iId  = EME_BOXOPT_SD1_SIZE;
            stOpt[3].pVal = (WE_VOID*)pstBoxInfo[dwBoxId].dwSD1Size;
            stOpt[4].iId  = EME_BOXOPT_SD2_SIZE;
            stOpt[4].pVal = (WE_VOID*)pstBoxInfo[dwBoxId].dwSD2Size;
            stOpt[5].iId  = WEMSG_OPT_END;
            
            IWEMSGVALUE_UpdateOpt(pIMsgBoxValue, stOpt);

            IWEMSGVALUE_Release(pIMsgBoxValue);
            pIMsgBoxValue = NULL;
         }
         uiBoxIndex++;
      }

      nErr = IWEMSG_EncodeMsg(pIMsgBox, (WE_UCHAR**)&pcBoxIndexBuff, &uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode box index buffer failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acBoxIndexFile, pcBoxIndexBuff, uiBoxIndexSize);
      EME_UTILS_BREAKIF(nErr, "update box index file failed!!");

   } while (0);

   if(NULL != pIMsgBoxValue)
      IWEMSGVALUE_Release(pIMsgBoxValue);

   if(NULL != pIMsgMsgValue)
      IWEMSGVALUE_Release(pIMsgMsgValue);

   if(NULL != pIMsgBox)
      IWEMSG_Release(pIMsgBox);

   if(NULL != pIMsgMsg)
      IWEMSG_Release(pIMsgMsg);

   EME_UTILS_FREEIF(NULL, pstBoxInfo);

   return nErr;
}