#include "eme_actindex_api.h"
#include "eme_utils_api.h"
#include "eme_dataaccess_api.h"
#include "eme_dataaccess_priv.h"
#include "eme_boxindex_api.h"
#include "eme_msgindex_api.h"
#include "EMS_Store_API.h"
#include "EMS_Store_Priv.h"
#include "we_msg.h"

static WE_BOOL EME_ActIndex_SynActiveActid(IWEMsg *pIMsg);

static WE_INT32 EME_ActIndex_GetNextAvailableActId(FILEMGR_HANDLE hMgr, WE_UINT32 *pdwActId)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     *pcBuf = NULL;
   WE_UINT32   auiActId[EMS_ACTINFO_MAX_NUMBER] = {0};
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get imsg failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed");
      
      if(0 == uiCnt)
      {
         *pdwActId = 1;
      }
      else if(uiCnt == EMS_ACTINFO_MAX_NUMBER)
      {
         *pdwActId = 0;
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
               
               nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt);
               if(nErr == EME_SUCCESS)
               {
                  auiActId[(WE_UINT32)stOpt.pVal - 1] = 1;
               }
               IWEMSGVALUE_Release(pIMsgValue);
               pIMsgValue = NULL;
            }
            uiIndex++;
         }
         
         uiIndex = 0;
         while(uiIndex < EMS_ACTINFO_MAX_NUMBER)
         {
            if(auiActId[uiIndex] == 0)
            {
               *pdwActId = uiIndex + 1;
               break;
            }
            uiIndex++;
         }
      }
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   EME_UTILS_FREEIF(NULL, pcBuf);
   
   return nErr;
}

WE_INT32 EME_ActIndex_CreateAccount(FILE_HANDLE hMgr, WE_UINT16 *pdwActName, WE_UINT32 *pdwActId)
{
   WE_INT32   nErr = EME_SUCCESS;
   WE_UINT32  dwActId = 0;

   WE_CHAR    acActDataFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR    acActFolder[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR    acActFileTmp[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR    *pcActDataBuff = NULL;
   WE_UINT32  uiActDataSize = 0;

   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WEMsgOpt    astOpt[5] = {0};
   WE_INT32    nPos = WEMSG_END_POSTION;

   if(NULL == pdwActName || NULL == pdwActId)
   {
      return EME_BADPARM;
   }

   /*account info save at udisk only!!*/
//   if(!EME_CheckUDiskFreeSpace(hMgr, EMS_LEAST_ACT_SPACE))
//   {
//      return EME_NOSTORAGE;
//   }
   
   do 
   {
      /*get next account id*/
      nErr = EME_ActIndex_GetNextAvailableActId(hMgr, &dwActId);
      EME_UTILS_BREAKIF(nErr, "get next act id failed");

      /*get account data file path*/
      EME_UTILS_SPRINTF(acActDataFile, EMEFS_ACT_DATA_FILE, dwActId);
      EME_UTILS_SPRINTF(acActFolder, EMEFS_ACT_DATA_FOLDER, dwActId);

      EME_RemoveDir(hMgr, acActFolder);
      EME_CreateDir(hMgr, acActFolder);

      nErr = EME_BoxIndex_CreateEmptyBoxIndex(hMgr, dwActId);
      EME_UTILS_BREAKIF(nErr, "create box index file failed!!");

      nErr = EME_MsgIndex_CreateEmptyMsgIndex(hMgr, dwActId);
      EME_UTILS_BREAKIF(nErr, "create msg index file failed!!");

      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create msg handle failed!!!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value failed");

      /*add uidl file opt*/
      EME_UTILS_SPRINTF(acActFileTmp, EMEFS_ACT_UIDL_FILE, dwActId);
      astOpt[0].iId  = EMS_PRFITEM_EMAIL_UIDLFILEDIR;
      astOpt[0].pVal = acActFileTmp;
      astOpt[1].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr,"add uidl opt failed");
      EME_UTILS_MEMSET(acActFileTmp, 0, EMEFS_FILE_MAX_LEN);

      /*add uid file opt*/
      EME_UTILS_SPRINTF(acActFileTmp, EMEFS_ACT_UID_FILE, dwActId);
      astOpt[0].iId  = EMS_PRFITEM_EMAIL_UIDFILEDIR;
      astOpt[0].pVal = acActFileTmp;
      astOpt[1].iId  = WEMSG_OPT_END;
      
      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr,"add uidl opt failed");
      EME_UTILS_MEMSET(acActFileTmp, 0, EMEFS_FILE_MAX_LEN);

      /*add sig file opt*/
      EME_UTILS_SPRINTF(acActFileTmp, EMEFS_ACT_SIG_FILE, dwActId);
      astOpt[0].iId  = EMS_PRFITEM_EMAIL_CREATE_SIGNATURE;
      astOpt[0].pVal = acActFileTmp;
      astOpt[1].iId  = WEMSG_OPT_END;
      
      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr,"add uidl opt failed");
      
      /*account rand number*/
      astOpt[0].iId  = EMS_PRFITEM_SAVE_PLACE;
      astOpt[0].pVal = (WE_VOID*)EMS_SAVE_PLACE_UDISK;
      astOpt[1].iId  = WEMSG_OPT_END;
      
      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr,"add uidl opt failed");

      /*account rand number*/
      astOpt[0].iId  = EMS_PRFITEM_ACCOUNT_RAND;
      astOpt[0].pVal = (WE_VOID*)EME_UTILS_GETRAND();
      astOpt[1].iId  = WEMSG_OPT_END;
      
      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr,"add uidl opt failed");

      nErr = IWEMSG_AddHdrValue(pIMsg, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add header value failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActDataBuff, &uiActDataSize);
      EME_UTILS_BREAKIF(nErr, "encode act data buffer failed!!");

      nErr = EME_DataAccess_CreateFileByBuffer(hMgr, acActDataFile, pcActDataBuff, uiActDataSize);
      EME_UTILS_BREAKIF(nErr, "create file by buffer failed!!");

      IWEMSGVALUE_Release(pIMsgValue);
      pIMsgValue = NULL;
      
      IWEMSG_Release(pIMsg);
      pIMsg = NULL;

      pcActDataBuff = NULL;
      uiActDataSize = 0;

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value failed!!");

      astOpt[0].iId  = EMS_PRFITEM_ACCOUNT_ID;
      astOpt[0].pVal = (WE_VOID*)dwActId;
      astOpt[1].iId  = EMS_PRFITEM_ACCOUNT_NAME;
      astOpt[1].pVal = (WE_VOID*)pdwActName;
      astOpt[2].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, astOpt);
      EME_UTILS_BREAKIF(nErr, "add msg info file failed!!");

      nErr = IWEMSG_AddPartValue(pIMsg, pIMsgValue, &nPos);
      EME_UTILS_BREAKIF(nErr, "add part fai");

      EME_ActIndex_SynActiveActid(pIMsg);

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActDataBuff, &uiActDataSize);
      EME_UTILS_BREAKIF(nErr, "encod msg failed!!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActDataBuff, uiActDataSize);
      EME_UTILS_BREAKIF(nErr, "udpate file buffer failed!!");

      *pdwActId = dwActId;

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

static WE_BOOL EME_ActIndex_SynActiveActid(IWEMsg *pIMsg)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsgValue  *pIMsgValue = NULL;
   IWEMsgValue  *pIMsgHeader = NULL;
   WE_UINT32    uiCnt = 0;
   WE_UINT32    uiIndex = 0;
   WE_UINT32    uiActiveId = 0;
   WE_UINT32    auiActIdList[EMS_ACTINFO_MAX_NUMBER] = {0};
   WEMsgOpt     stOpt[2] = {0};
   WE_BOOL      bUpdate = TRUE;

   if(NULL == pIMsg)
      return FALSE;

   do 
   {
      if(WE_SUCCESS != IWEMSG_GetHdrValue(pIMsg, &pIMsgHeader))
      {
         IWEMSGVALUE_New(&pIMsgHeader);
         EME_UTILS_BREAKNULL(pIMsgHeader, nErr, EME_FAILED, "create msg header failed!!");
         stOpt[0].iId  = EMS_PRFITEM_ACTIVE_ACCOUNTID;
         stOpt[0].pVal = (WE_VOID*)0;
         stOpt[1].iId  = WEMSG_OPT_END;

         IWEMSGVALUE_AddOpt(pIMsgHeader, stOpt);
         IWEMSG_AddHdrValue(pIMsg, pIMsgHeader);
      }

      /*get active id*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgHeader, EMS_PRFITEM_ACTIVE_ACCOUNTID, &stOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get active id failed!!");

      uiActiveId = (WE_UINT32)stOpt[0].pVal;

      /*get account count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act count failed!!");

      if(uiActiveId == 0 && 0 == uiCnt)
      {
         bUpdate = FALSE;
         break;
      }

      while(uiIndex < uiCnt)
      {
         if(WE_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt[0]);
            auiActIdList[uiIndex] = (WE_UINT32*)stOpt[0].pVal;
         }

         IWEMSGVALUE_Release(pIMsgValue);
         pIMsgValue = NULL;

         uiIndex++;
      }

      uiIndex = 0;
      while(uiIndex < EMS_ACTINFO_MAX_NUMBER && uiActiveId > 0)
      {
         if(auiActIdList[uiIndex] == uiActiveId)
         {
            bUpdate = FALSE;
            break;
         }
         uiIndex++;
      }

      if(bUpdate)
      {
         stOpt[0].iId  = EMS_PRFITEM_ACTIVE_ACCOUNTID;
         stOpt[0].pVal = (WE_VOID*)auiActIdList[0];
         stOpt[1].iId  = WEMSG_OPT_END;

         IWEMSGVALUE_UpdateOpt(pIMsgHeader, stOpt);
      }

   } while (0);

   if(NULL != pIMsgHeader)
      IWEMSGVALUE_Release(pIMsgHeader);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   return bUpdate;
}

static WE_VOID EME_ActIndex_DeleteAllMailByActId(FILE_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WEMsgOpt    stOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};

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
               EME_MsgStore_RemoveMsg(hMgr, (WE_UINT32)stOpt[0].pVal);
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex++;
      }
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return;
}

WE_INT32 EME_ActIndex_DeleteAccount(FILE_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;

   if(0 == dwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed");

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act count failed!");

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == WE_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            if(IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt) == EME_SUCCESS && (WE_UINT32)stOpt.pVal == dwActId)
            {
               IWEMSG_RemovePartValue(pIMsg, uiIndex);
               break;
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      if(pIMsgValue != NULL)
      {
         WE_CHAR   *pcActIndexBuf = NULL;
         WE_UINT32 uiActIndexSize = 0;
         WE_CHAR   acActFolder[EMEFS_FILE_MAX_LEN] = {0};

         EME_UTILS_SPRINTF(acActFolder, EMEFS_ACT_DATA_FOLDER, dwActId);

         EME_ActIndex_SynActiveActid(pIMsg);

         nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuf, &uiActIndexSize);
         EME_UTILS_BREAKIF(nErr, "encode act index file failed!!");

         nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActIndexBuf, uiActIndexSize);
         EME_UTILS_BREAKIF(nErr, "update act index file failed!!");

         EME_ActIndex_DeleteAllMailByActId(hMgr, dwActId);
         EME_RemoveDir(hMgr, acActFolder);
      }

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_ActIndex_GetAccountId(FILE_HANDLE hMgr, WE_UINT16 *pdwActName, WE_UINT32 *pdwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   
   if(NULL == pdwActId || NULL == pdwActName)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act count failed!");
      
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == WE_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            if(IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_NAME, &stOpt) == EME_SUCCESS)
            {
               if(EME_UTILS_WSTRCMP((WE_UINT16*)stOpt.pVal, pdwActName) == 0)
               {
                  nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt);
                  EME_UTILS_BREAKIF(nErr, "get account id failed!!");

                  *pdwActId = (WE_UINT32)stOpt.pVal;
                  break;
               }
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
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

WE_INT32 EME_ActIndex_GetAccountNum(FILE_HANDLE hMgr, WE_UINT32 *pdwActNum)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   
   if(NULL == pdwActNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act count failed!");
      
      *pdwActNum = uiCnt;

   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_GetActInfo(FILEMGR_HANDLE hMgr, WE_UINT32 dwNum, EMS_ActInfo *pActInfo)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiPos = 0;
   
   if(0 == dwNum || NULL == pActInfo)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act count failed!");

      while(uiIndex < uiCnt && uiPos < dwNum)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == WE_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            
            IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_NAME, &stOpt);

            EME_UTILS_MEMCPY((WE_CHAR*)pActInfo[uiPos].wazActName, (WE_CHAR*)stOpt.pVal, EME_UTILS_WSTRLEN((WE_UINT16*)stOpt.pVal) * sizeof(WE_UINT16));

            IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt);

            pActInfo[uiPos].uiActId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;

            uiPos++;
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

WE_INT32 EME_ActIndex_UpdateIndex(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WE_UINT32 uiItem,  WE_VOID *pItemVal)
{
   WE_INT32   nErr = EME_SUCCESS;
   IWEMsg     *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WE_CHAR     *pcActIndexFile = NULL;
   WE_UINT32   uiActIndexSize = 0;
   
   if(uiActId == 0)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      if(EMS_PRFITEM_ACCOUNT_NAME != uiItem)
      {
         return EME_SUCCESS;
      }
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file handle failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get account number failed!!");
      
      while(uiIndex < uiCnt)
      {
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            WEMsgOpt stOpt[2] = {0};
            
            IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt[0]);
            if((WE_UINT32)stOpt[0].pVal == uiActId)
            {
               stOpt[0].iId  = uiItem;
               stOpt[0].pVal = pItemVal;
               stOpt[1].iId  = WEMSG_OPT_END;
               
               IWEMSGVALUE_UpdateOpt(pIMsgValue, stOpt);
               break;
            }
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }
      
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "get msg value failed!!");
      
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexFile, &uiActIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode act index file failed!");
      
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActIndexFile, uiActIndexSize);
      EME_UTILS_BREAKIF(nErr, "update act index file failed!!");
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_SetActGlobalCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiItem,  WE_VOID *pItemVal)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WEMsgOpt    stOpt[2] = {0};
   WE_CHAR     *pcActIndexBuff = NULL;
   WE_UINT32   uiActIndexBuff = 0;
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");
      
      stOpt[0].iId  = uiItem;
      stOpt[0].pVal = pItemVal;
      stOpt[1].iId  = WEMSG_OPT_END;
      
      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, stOpt);
      if(nErr == WE_NOTFOUND)
      {
         nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
      }
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");
      
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuff, &uiActIndexBuff);
      EME_UTILS_BREAKIF(nErr, "encode act index buffer failed!!");
      
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActIndexBuff, uiActIndexBuff);
      EME_UTILS_BREAKIF(nErr, "update file failed!!");
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_SetActGlobalCfgItemEx(FILEMGR_HANDLE hMgr, WEMsgOpt *pstOpt)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     *pcActIndexBuff = NULL;
   WE_UINT32   uiActIndexBuff = 0;

   if(pstOpt == NULL)
      return EME_BADPARM;
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");
      
      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, pstOpt);
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");
      
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuff, &uiActIndexBuff);
      EME_UTILS_BREAKIF(nErr, "encode act index buffer failed!!");
      
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActIndexBuff, uiActIndexBuff);
      EME_UTILS_BREAKIF(nErr, "update file failed!!");
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_SetActCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WE_UINT32 uiItem,  WE_VOID *pItemVal)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acActDataFile[EMEFS_FILE_MAX_LEN]= {0};
   WE_UINT32   uiActDataFile = 0;
   WE_CHAR     *pcActDataFile = NULL;
   WEMsgOpt    stOpt[2] = {0};

   if(uiActId == 0 || 0 == uiItem)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acActDataFile, EMEFS_ACT_DATA_FILE, uiActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActDataFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act data file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header value failed");
      
      stOpt[0].iId  = uiItem;
      stOpt[0].pVal = pItemVal;
      stOpt[1].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, stOpt);
      if(nErr == WE_NOTFOUND)
      {
         nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
      }
      EME_UTILS_BREAKIF(nErr, "update opt failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActDataFile, &uiActDataFile);
      EME_UTILS_BREAKIF(nErr, "encode failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acActDataFile, pcActDataFile, uiActDataFile);
      EME_UTILS_BREAKIF(nErr, "update file failed!!");

      EME_ActIndex_UpdateIndex(hMgr, uiActId, uiItem, pItemVal);

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_ActIndex_SetActCfgItemEx(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WEMsgOpt *pstOpt)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acActDataFile[EMEFS_FILE_MAX_LEN]= {0};
   WE_UINT32   uiActDataFile = 0;
   WE_CHAR     *pcActDataFile = NULL;

   if(uiActId == 0 || NULL == pstOpt)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acActDataFile, EMEFS_ACT_DATA_FILE, uiActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActDataFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act data file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header value failed");

      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, pstOpt);
      EME_UTILS_BREAKIF(nErr, "update opt failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActDataFile, &uiActDataFile);
      EME_UTILS_BREAKIF(nErr, "encode failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acActDataFile, pcActDataFile, uiActDataFile);
      EME_UTILS_BREAKIF(nErr, "update file failed!!");

      while(pstOpt->iId != WEMSG_OPT_END)
      {
         if(EMS_PRFITEM_ACCOUNT_NAME == pstOpt->iId)
         {
            EME_ActIndex_UpdateIndex(hMgr, uiActId, pstOpt->iId, pstOpt->pVal);      
         }
         pstOpt++;
      }
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_ActIndex_GetActGlobalCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiItem,  WE_VOID **ppItemVal)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WEMsgOpt    stOpt[2] = {0};
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");
      
      IWEMSGVALUE_GetOpt(pIMsgValue, uiItem, stOpt);

      if(uiItem >= WEMSG_OPT_32BIT && uiItem <= WEMSG_OPT_32BIT_LAST)
      {
         *ppItemVal = stOpt[0].pVal;
      }
      if(uiItem >= WEMSG_OPT_SZ && uiItem <= WEMSG_OPT_SZ_LAST)
      {
         if(NULL != stOpt[0].pVal)
            *ppItemVal = EME_UTILS_STRDUP(NULL, (WE_CHAR*)stOpt[0].pVal);
         else
            *ppItemVal = EME_UTILS_STRDUP(NULL, "");
      }
      else if(uiItem >= WEMSG_OPT_WSZ && uiItem <= WEMSG_OPT_WSZ_LAST)
      {
         if(NULL != stOpt[0].pVal)
            *ppItemVal = EME_UTILS_WSTRDUP(NULL, (WE_UINT16*)stOpt[0].pVal);
         else
            *ppItemVal = EME_UTILS_WSTRDUP(NULL, L"");
      }

   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_GetActCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WE_UINT32 uiItem,  WE_VOID **ppItemVal)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acActDataFile[EMEFS_FILE_MAX_LEN]= {0};
   WEMsgOpt    stOpt[2] = {0};
   
   if(uiActId == 0 || 0 == uiItem || NULL == ppItemVal)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      EME_UTILS_SPRINTF(acActDataFile, EMEFS_ACT_DATA_FILE, uiActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActDataFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act data file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header value failed");
      
      IWEMSGVALUE_GetOpt(pIMsgValue, uiItem, stOpt);

      if(uiItem >= WEMSG_OPT_32BIT && uiItem <= WEMSG_OPT_32BIT_LAST)
      {
         *ppItemVal = stOpt[0].pVal;
      }
      if(uiItem >= WEMSG_OPT_SZ && uiItem <= WEMSG_OPT_SZ_LAST)
      {
         if(NULL != stOpt[0].pVal)
            *ppItemVal = EME_UTILS_STRDUP(NULL, (WE_CHAR*)stOpt[0].pVal);
         else
            *ppItemVal = EME_UTILS_STRDUP(NULL, "");
      }
      else if(uiItem >= WEMSG_OPT_WSZ && uiItem <= WEMSG_OPT_WSZ_LAST)
      {
         if(NULL != stOpt[0].pVal)
            *ppItemVal = EME_UTILS_WSTRDUP(NULL, (WE_UINT16*)stOpt[0].pVal);
         else
            *ppItemVal = EME_UTILS_WSTRDUP(NULL, L"");
      }
            
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_CreateEmptyActIndex(FILE_HANDLE hMgr)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsg       *pIMsg = NULL;
   IWEMsgValue  *pIMsgValue = NULL;
   WE_CHAR      acActIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32    uiActIndexSize = 0;
   WE_CHAR      *pcActIndexBuff = NULL;
   WEMsgOpt     stOpt[3] = {0};
   
   do 
   {
      EME_UTILS_SPRINTF(acActIndexFile, EMEFS_ACT_INDEX_FILE);
      
      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create act handle failed!!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value failed");

      stOpt[0].iId  = EMS_PRFITEM_ACTIVE_ACCOUNTID;
      stOpt[0].pVal = 0;
      stOpt[1].iId  = EMS_PRFITEM_SAVE_PLACE;
      stOpt[1].pVal = EMS_SAVE_PLACE_UDISK;
      stOpt[2].iId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
      EME_UTILS_BREAKIF(nErr, "add opt failed!!");
      
      nErr = IWEMSG_AddHdrValue(pIMsg, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add header failed!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuff, &uiActIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode act failed");
      
      nErr = EME_DataAccess_CreateFileByBuffer(hMgr, acActIndexFile, pcActIndexBuff, uiActIndexSize);
      EME_UTILS_BREAKIF(nErr, "create act index file failed");
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_CheckAndFixedActIndex(FILE_HANDLE hMgr)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsg       *pIMsg = NULL;
   IWEMsgValue  *pIMsgValue = NULL;
   WE_CHAR      acActIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32    uiCnt = 0;
   WE_UINT32    uiIndex = 0;
   WE_BOOL      bChanged = FALSE;
   WE_CHAR      *pcActIndexBuff = NULL;
   WE_UINT32    uiActIndexSize = 0;
   
   do 
   {
      EME_UTILS_SPRINTF(acActIndexFile, EMEFS_ACT_INDEX_FILE);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActIndexFile);
      if(NULL == pIMsg)
      {
         /*actindex.tmf file error, then delete old file and create emtpy file!*/
         EME_DeleteFile(hMgr, acActIndexFile);
         return EME_ActIndex_CreateEmptyActIndex(hMgr);
      }

      IWEMSG_GetPartCount(pIMsg, &uiCnt);
      while(uiIndex < uiCnt)
      {
         WEMsgOpt stOpt[2] = {0};

         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt[0]))
            {
               /*check account data is vailed!!*/
               if(!EME_ActIndex_CheckActountOK(hMgr, (WE_UINT32)stOpt[0].pVal))
               {
                  WE_CHAR  aucActFolder[EMEFS_FILE_MAX_LEN] = {0};
                  /*remove bad part form msg handle*/
                  IWEMSG_RemovePartValue(pIMsg, uiIndex);
                  
                  /*delete all mail by bad account id if we could*/
                  EME_ActIndex_DeleteAllMailByActId(hMgr, (WE_UINT32)stOpt[0].pVal);
                  
                  EME_UTILS_SPRINTF(aucActFolder, EMEFS_ACT_DATA_FOLDER, (WE_UINT32)stOpt[0].pVal);
                  EME_RemoveDir(hMgr, aucActFolder);

                  IWEMSG_GetPartCount(pIMsg, &uiCnt);
                  bChanged = TRUE;
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

      if(EME_ActIndex_SynActiveActid(pIMsg) || bChanged)
      {
         nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuff, &uiActIndexSize);
         EME_UTILS_BREAKIF(nErr, "encode act index file failed!!");

         nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acActIndexFile, pcActIndexBuff, uiActIndexSize);
         EME_UTILS_BREAKIF(nErr, "update act index file failed!!");
      }

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_FactoryReset(FILE_HANDLE hMgr)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsg       *pIMsg = NULL;
   IWEMsgValue  *pIMsgValue = NULL;
   WE_CHAR      acActIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32    uiCnt = 0;
   WE_UINT32    uiIndex = 0;
   
   do 
   {
      EME_UTILS_SPRINTF(acActIndexFile, EMEFS_ACT_INDEX_FILE);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActIndexFile);
      if(NULL == pIMsg)
      {
         /*actindex.tmf file error, then delete old file and create emtpy file!*/
         EME_DeleteFile(hMgr, acActIndexFile);
         return EME_ActIndex_CreateEmptyActIndex(hMgr);
      }
      
      IWEMSG_GetPartCount(pIMsg, &uiCnt);
      while(uiIndex < uiCnt)
      {
         WEMsgOpt stOpt[2] = {0};
         
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stOpt[0]))
            {
               EME_ActIndex_DeleteAccount(hMgr, (WE_UINT32)stOpt[0].pVal);
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }
      
      EME_RemoveDir(hMgr, EMEFS_UDISK_EML_ROOT_DIR);
      EME_DeleteFile(hMgr, acActIndexFile);
      EME_ActIndex_CreateEmptyActIndex(hMgr);
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_ActIndex_GetActiveActId(FILE_HANDLE hMgr, WE_UINT32 *pdwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WEMsgOpt    stOpt = {0};

   if(NULL == pdwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");

      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get msg header failed");

      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACTIVE_ACCOUNTID, &stOpt);
      EME_UTILS_BREAKIF(nErr, "get opt failed!!");

      *pdwActId = (WE_UINT32)stOpt.pVal;

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_BOOL  EME_ActIndex_CheckActountOK(FILE_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32 nErr = EME_SUCCESS;
   WE_CHAR  acActFile[EMEFS_FILE_MAX_LEN] = {0};
   IWEMsg   *pIMsg = NULL;

   if(0 == dwActId)
   {
      return FALSE;
   }

   do 
   {
      /*check actindex.tmf file is vailed!!*/
//       pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
//       EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
//       IWEMSG_Release(pIMsg);
//       pIMsg = NULL;

      /*check actdata.tmf file is vailed*/
      EME_UTILS_MEMSET(acActFile, 0x00, EMEFS_FILE_MAX_LEN);
      EME_UTILS_SPRINTF(acActFile, EMEFS_ACT_DATA_FILE, dwActId);
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      IWEMSG_Release(pIMsg);
      pIMsg = NULL;

      /*check msgindex.tmf file is vailed*/
      EME_UTILS_MEMSET(acActFile, 0x00, EMEFS_FILE_MAX_LEN);
      EME_UTILS_SPRINTF(acActFile, EMEFS_MSG_INDEX_FILE, dwActId);
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      IWEMSG_Release(pIMsg);
      pIMsg = NULL;

      /*check boxindex.tmf file is vailed*/
      EME_UTILS_MEMSET(acActFile, 0x00, EMEFS_FILE_MAX_LEN);
      EME_UTILS_SPRINTF(acActFile, EMEFS_BOX_INDEX_FILE, dwActId);
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acActFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      IWEMSG_Release(pIMsg);
      pIMsg = NULL;

      return TRUE;

   } while (0);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return FALSE;
}

WE_INT32 EME_ActIndex_SetActiveActId(FILE_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WEMsgOpt    stOpt[2] = {0};
   WE_CHAR     *pcActIndexBuff = NULL;
   WE_UINT32   uiActIndexBuff = 0;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WE_BOOL     bFound = FALSE;
   
   if(0 == dwActId)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, EMEFS_ACT_INDEX_FILE);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get act index file failed!!");
      
      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get header opt failed!!");

      IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACTIVE_ACCOUNTID, &stOpt[0]);

      if(dwActId == (WE_UINT32)stOpt[0].pVal)
      {
         break;
      }

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get act number failed!!");

      while(uiIndex < uiCnt)
      {
         if(WE_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            WEMsgOpt stActOpt[2] = {0};

            IWEMSGVALUE_GetOpt(pIMsgValue, EMS_PRFITEM_ACCOUNT_ID, &stActOpt[0]);
            if((WE_UINT32)stActOpt[0].pVal == dwActId)
            {
               IWEMSGVALUE_Release(pIMsgValue);
               pIMsgValue = NULL;

               bFound = TRUE;
               break;
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      if(bFound)
      {
         nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
         EME_UTILS_BREAKIF(nErr, "get msg header failed");
         
         stOpt[0].iId  = EMS_PRFITEM_ACTIVE_ACCOUNTID;
         stOpt[0].pVal = (WE_VOID*)dwActId;
         stOpt[1].iId  = WEMSG_OPT_END;
         
         nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, stOpt);
         if(nErr == WE_NOTFOUND)
         {
            nErr = IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
         }
         EME_UTILS_BREAKIF(nErr, "update opt failed!!");
         
         nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcActIndexBuff, &uiActIndexBuff);
         EME_UTILS_BREAKIF(nErr, "encode act index buffer failed!!");
         
         nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, EMEFS_ACT_INDEX_FILE, pcActIndexBuff, uiActIndexBuff);
         EME_UTILS_BREAKIF(nErr, "update file failed!!");
      }
      else
      {
         nErr = EME_INVALID_DATA;
         EME_UTILS_BREAKIF(nErr, "count find actid in actindex.tmf file");
      }
      
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

