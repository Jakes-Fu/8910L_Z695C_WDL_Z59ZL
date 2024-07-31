/*==============================================================================
    FILE NAME   : eme_msgstore.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        This file contain implementation for message store mechanism.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-26   Wenkai Xu        01         Create

==============================================================================*/

//pclint
/*lint -e663 */

/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "eme_msgstore_api.h"
#include "eme_dataaccess_priv.h"
#include "eme_utils_api.h"
#include "we_msg.h"
#include "eme_actindex_api.h"
#include "EMS_Store_API.h"
#include "EMS_Store_Priv.h"

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
WE_UINT32 EME_MsgStore_DetermMsgId(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 uiMsgId)
{
   WE_UINT32 uiActRnd = 0;

   uiMsgId = uiMsgId & 0xFFFF;

   if(0 == dwActId)
   {
      return uiMsgId;
   }

   EME_ActIndex_GetActCfgItem(hMgr, dwActId, EMS_PRFITEM_ACCOUNT_RAND, (WE_VOID**)&uiActRnd);
   uiMsgId = EME_SET_ACTID_TO_MSGID(uiMsgId, dwActId);
   uiMsgId = EME_SET_RAND_TO_MSGID(uiMsgId, uiActRnd);

   return uiMsgId;
}

WE_UINT32 EME_MsgStore_DetermSavePlace(FILE_HANDLE hMgr)
{
   WE_UINT32 uiSD1High = 0;
   WE_UINT32 uiSD1Low  = 0;
   WE_UINT32 uiSD2High = 0;
   WE_UINT32 uiSD2Low  = 0;
   WE_UINT32 uiUDHigh = 0;
   WE_UINT32 uiUDLow = 0;
   WE_UINT32 uiMax = 0;
   WE_UINT32 uiActSaveAs = EMS_SAVE_PLACE_UDISK;

   TF3RD_SD1FREESPACE(&uiSD1High, &uiSD1Low);
   TF3RD_SD2FREESPACE(&uiSD2High, &uiSD2Low);
   TF3RD_UDFREESPACE(&uiUDHigh, &uiUDLow);

   EME_ActIndex_GetActGlobalCfgItem(hMgr, EMS_PRFITEM_SAVE_PLACE, (WE_VOID**)&uiActSaveAs);

   if(uiSD1High > 0 || uiSD1Low > EMS_LEAST_EML_SPACE)
   {
      EME_UTILS_LOG_INFO(("EME_MsgStore_DetermSavePlace-- SAVE TO SD1"));
      return EMS_SAVE_PLACE_SDCARD1;
   }
   else if(uiSD2High > 0 || uiSD2Low > EMS_LEAST_EML_SPACE)
   {
      EME_UTILS_LOG_INFO(("EME_MsgStore_DetermSavePlace-- SAVE TO SD2"));
      return EMS_SAVE_PLACE_SDCARD2;
   }
   else
   {
      EME_UTILS_LOG_INFO(("EME_MsgStore_DetermSavePlace-- SAVE TO UD"));
      return EMS_SAVE_PLACE_UDISK;
   }
}

static WE_INT32 EME_MsgStore_GetNextAvailableMsgId(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwMsgId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiMaxMsgId = 0;
   WE_UINT8    *pucMsgIdList = NULL;
   WE_UINT32   uiActSaveAs = 0;
   WE_UINT32   uiActRnd = 0;

   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      pucMsgIdList = EME_UTILS_MALLOC(NULL, EME_MSGID_MAX_NUM * sizeof(WE_UINT8));
      EME_UTILS_BREAKNULL(pucMsgIdList, nErr, EME_NOMEMORY, "no memory!!");
      EME_UTILS_MEMSET(pucMsgIdList, 0x00, EME_MSGID_MAX_NUM * sizeof(WE_UINT8));

      uiActSaveAs = EME_MsgStore_DetermSavePlace(hMgr);

      nErr = EME_ActIndex_GetActCfgItem(hMgr, dwActId, EMS_PRFITEM_ACCOUNT_RAND, (WE_VOID**)&uiActRnd);
      EME_UTILS_BREAKIF(nErr, "get act rand vailed!!");

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file handle failed!!");

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");

      if(EME_MSGID_MAX_NUM == uiCnt)
      {
         nErr = EME_FAILED;
         EME_UTILS_BREAKIF(nErr, "msg id arrived max size!!!");
      }

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt = {0};

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_ACTID, &stOpt);
            if((WE_UINT32)stOpt.pVal == dwActId)
            {
               WE_UINT32   uiMsgId = 0;
               IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt);
               
               uiMsgId = (WE_UINT32)stOpt.pVal & 0xFFF;

               if(uiMsgId < EME_MSGID_MAX_NUM)
               {
                  pucMsgIdList[uiMsgId - 1] = 1;
               }
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      /*msg id base on 1*/
      uiIndex = 0;
      while(uiIndex < EME_MSGID_MAX_NUM)
      {
         if(0 == pucMsgIdList[uiIndex])
         {
            uiMaxMsgId = uiIndex + 1;
            break;
         }
         uiIndex++;
      }

      uiMaxMsgId = EME_SET_PLACE_TO_MSGID(uiMaxMsgId, uiActSaveAs);
      uiMaxMsgId = EME_SET_ACTID_TO_MSGID(uiMaxMsgId, dwActId);
      uiMaxMsgId = EME_SET_RAND_TO_MSGID(uiMaxMsgId, uiActRnd);

      *pdwMsgId = uiMaxMsgId;

   } while (0);

   EME_UTILS_FREEIF(NULL, pucMsgIdList);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_MsgStore_GenerateMsgPath(WE_UINT32 dwMsgId, WE_CHAR *pcMsgPath, WE_CHAR *pcMsgFile)
{
   if(0 == dwMsgId)
   {
      return EME_BADPARM;
   }

   if(EME_GET_PLACE_FROM_MSGID(dwMsgId) == EMS_SAVE_PLACE_SDCARD1)
   {
      if(pcMsgPath != NULL)
         EME_UTILS_SNPRINTF(pcMsgPath, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FOLDER_SDCARD1, dwMsgId);

      if(pcMsgFile != NULL)
         EME_UTILS_SNPRINTF(pcMsgFile, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FILE_SDCARD1, dwMsgId, dwMsgId);
   }
   else if(EME_GET_PLACE_FROM_MSGID(dwMsgId) == EMS_SAVE_PLACE_SDCARD2)
   {
      if(pcMsgPath != NULL)
         EME_UTILS_SNPRINTF(pcMsgPath, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FOLDER_SDCARD2, dwMsgId);

      if(pcMsgFile != NULL)
         EME_UTILS_SNPRINTF(pcMsgFile, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FILE_SDCARD2, dwMsgId, dwMsgId);
   }
   else
   {
      if(pcMsgPath != NULL)
         EME_UTILS_SNPRINTF(pcMsgPath, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FOLDER_UDISK, dwMsgId);

      if(pcMsgFile != NULL)
         EME_UTILS_SNPRINTF(pcMsgFile, EMEFS_FILE_MAX_LEN, EMEFS_MSG_DATA_FILE_UDISK, dwMsgId, dwMsgId);
   }

   return EME_SUCCESS;
}

// static WE_INT32 EME_MsgStore_GenerateFileName(WE_UINT32 dwMsgId, WE_CHAR *pszExt, WE_CHAR acMsgFileName[EMEFS_FILE_MAX_LEN])
// {
//    WE_CHAR   acMsgPath[EMEFS_FILE_MAX_LEN] = {0};
//    WE_CHAR   acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
// 
//    if(0 == dwMsgId)
//    {
//       return EME_BADPARM;
//    }
// 
//    if(EME_MsgStore_GenerateMsgPath(dwMsgId, acMsgPath, acMsgFile) == EME_SUCCESS)
//    {
//       if(NULL != pszExt)
//       {
//          EME_UTILS_SPRINTF(acMsgFileName, "%s/eml%08x.%s", acMsgPath, EME_UTILS_GETTIMEMS, pszExt);
//       }
//       else
//       {
//          EME_UTILS_SPRINTF(acMsgFileName, "%s/eml%08x", acMsgPath, EME_UTILS_GETTIMEMS);
//       }
//    }
//    else
//    {
//       return EME_FAILED;
//    }
// 
//    return EME_SUCCESS;
// }

WE_INT32 EME_MsgStore_NewMsg(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwMsgId)
{
   WE_INT32   nErr = EME_SUCCESS;
   WE_CHAR    acMsgDataFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR    acMsgDataFolder[EMEFS_FILE_MAX_LEN] = {0};
//   MsgOpt     stMsgOpt[10] = {0};
   IWEMsg     *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     *pcMsgDataFile = NULL;
   WE_UINT32   uiMsgDataSize = 0;

   if(0 == dwActId || NULL == pdwMsgId)
   {
      return EME_BADPARM;
   }

   do 
   {
      nErr = EME_MsgStore_GetNextAvailableMsgId(hMgr, dwActId, &uiMsgId);
      EME_UTILS_BREAKIF(nErr, "get next msg id failed!!");

      nErr = EME_MsgStore_GenerateMsgPath(uiMsgId, acMsgDataFolder, acMsgDataFile);
      EME_UTILS_BREAKIF(nErr, "generate msg path failed");

      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create msg handle failed!!");

      IWEMSGVALUE_New(&pIMsgValue);
      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value handle failed!!");

//       stMsgOpt[0].nId  = EME_MSGOPT_ACTID;
//       stMsgOpt[0].pVal = (WE_VOID*)dwActId;
// 
//       stMsgOpt[1].nId  = EME_MSGOPT_MSGID;
//       stMsgOpt[1].pVal = (WE_VOID*)uiMsgId;
// 
//       stMsgOpt[2].nId  = EME_MSGOPT_BOXID;
//       stMsgOpt[2].pVal = (WE_VOID*)0;
// 
//       stMsgOpt[3].nId  = WEMSG_OPT_END;
// 
//       nErr = IWEMSGVALUE_AddOpt(pIMsgValue, (WEMsgOpt*)stMsgOpt);
//       EME_UTILS_BREAKIF(nErr, "add opt failed");

      nErr = IWEMSG_AddHdrValue(pIMsg, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add msg value faild!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgDataFile, &uiMsgDataSize);
      EME_UTILS_BREAKIF(nErr, "encode failed!!");
      
      EME_RemoveDir(hMgr, acMsgDataFolder);

      nErr = EME_CreateDir(hMgr, acMsgDataFolder);
      EME_UTILS_BREAKIF(nErr, "create msg folder failed!!");

      nErr = EME_DataAccess_CreateFileByBuffer(hMgr, acMsgDataFile, pcMsgDataFile, uiMsgDataSize);
      EME_UTILS_BREAKIF(nErr, "create file failed!!");

      *pdwMsgId = uiMsgId;

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   if(EME_SUCCESS != nErr && EME_UTILS_STRLEN(acMsgDataFolder) > 0)
   {
      EME_RemoveDir(hMgr, acMsgDataFolder);
   }

   return nErr;
}

static WE_INT32 EME_MsgStore_RemoveTrashyFiles(FILEMGR_HANDLE hMgr, WE_UINT32 dwMsgId, WE_CHAR **ppFileList, WE_UINT32 dwCount)
{ 
   WE_CHAR aMsgPath[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR *pcEnumName= NULL;
   WE_UINT32 dwTmp = 0;
   
   do 
   {
      /* Generate the message folder path */
      EME_MsgStore_GenerateMsgPath(dwMsgId, aMsgPath, acMsgFile);
      
      hMgr = EME_EnumInit( aMsgPath, FALSE);      
      while (pcEnumName = EME_EnumNext(hMgr))
      {
         /* Exclude message file */
         if (!EME_UTILS_STRCMP(pcEnumName, acMsgFile))
         {
            continue;
         }
         
         for (dwTmp = 0; NULL != ppFileList && dwTmp < dwCount; ++dwTmp)
         {
            if (NULL != ppFileList[dwTmp] && !EME_UTILS_STRCMP(pcEnumName, ppFileList[dwTmp]))
            {
               /* Is in the file list */
               break;
            }
         }

         if (NULL == ppFileList || dwTmp == dwCount)
         {
            /* not in the file list , then delete file!!*/
            EME_UTILS_LOG_INFO(("delete trash file = %s", pcEnumName));
            EME_DeleteFile(hMgr, pcEnumName);
         }
      }
      
      EME_EnumClose(hMgr);
      
   } while(0);
   
   return EME_SUCCESS;
}

static WE_INT32 EME_MsgStore_UpdateMsgPartPriv(WE_VOID *pvMemMgr, 
                                               FILEMGR_HANDLE hMgr, 
                                               HMsgPart hMsgPart, 
                                               WE_UINT32 dwMsgId, 
                                               WE_CHAR *pPath)
{
   WE_INT32 nErr = EME_FAILED;
   
   MsgPartData stPartData = {0};
   MsgOpt      stMsgOpt[2] = {0};
   
   WE_CHAR   acMsgPath[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR   acBodyFile[EMEFS_FILE_MAX_LEN] = {0};

   if (NULL == hMsgPart)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /*generate msg path and msg file*/
      nErr = EME_MsgStore_GenerateMsgPath(dwMsgId, acMsgPath, NULL);
      EME_UTILS_BREAKIF(nErr, "get msg path failed!!");
         
      /* Check if the part has content */
      if (EME_SUCCESS == HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_DATABUF, 0, &stMsgOpt[0]))
      {
         stPartData.pData = (WE_CHAR *)stMsgOpt[0].pVal;
         stPartData.dwSize = EME_UTILS_STRLEN((WE_CHAR *)stMsgOpt[0].pVal);
         
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_SRCTYPE, 0, &stMsgOpt[0]);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsgPartPriv, get source type option failed");

         if (EMEPART_SRCTYPE_BUFFER == (WE_UINT32)stMsgOpt[0].pVal)
         {
            stPartData.nType = PART_DATA_TYPE_BUFFER;
         }
         else if (EMEPART_SRCTYPE_FILE == (WE_UINT32)stMsgOpt[0].pVal)
         {
            stPartData.nType = PART_DATA_TYPE_FILE;
         }
         
         nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_PARTTYPE, 0, &stMsgOpt[0]);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsgPartPriv, get part type option failed");
         
         if (EMEPART_TYPE_TEXT_PLAIN == (WE_UINT32)stMsgOpt[0].pVal)
         {
            stPartData.pExt = TEXTPLAIN_EXT;
         } 
         else if (EMEPART_TYPE_TEXT_HTML == (WE_UINT32)stMsgOpt[0].pVal)
         {
            stPartData.pExt = TEXTHTML_EXT;
         } 
         else if (EMEPART_TYPE_ATTACHMENT == (WE_UINT32)stMsgOpt[0].pVal)
         {
            MsgOpt stOptAttName = {0};
            WE_CHAR *pAttName = NULL;
            
            nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_ATTACHNAME, 0, &stOptAttName);
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsgPartPriv, get attachment name failed");
            
            pAttName = EME_UTILS_STRRCHR((WE_CHAR *)stOptAttName.pVal, '\\');
            if(NULL == pAttName)
               pAttName = EME_UTILS_STRRCHR((WE_CHAR *)stOptAttName.pVal, '/');

            if (pAttName == NULL)
            {
               pAttName = (WE_CHAR *)stOptAttName.pVal;
            }
            else
            {
               ++pAttName;
            }
            
            stPartData.pExt = EME_UTILS_STRRCHR(pAttName, '.');
            if (NULL != stPartData.pExt)
            {
                ++stPartData.pExt;  //clear '.'
            }
         }

         /*generate msg body file name*/
         if(NULL != stPartData.pExt)
         {
            EME_UTILS_SPRINTF(acBodyFile, "%s\\eml%08x.%s", acMsgPath, EME_UTILS_GETTIMEMS, stPartData.pExt);
         }
         else
         {
            EME_UTILS_SPRINTF(acBodyFile, "%s\\eml%08x", acMsgPath, EME_UTILS_GETTIMEMS);
         }

         /*save message body part*/
         if(PART_DATA_TYPE_BUFFER == stPartData.nType)
         {
            nErr = EME_DataAccess_CreateFile(hMgr, acBodyFile, stPartData.pData, stPartData.dwSize);
         }
         else
         {
            if(EME_UTILS_STRNCMP(acMsgPath, (WE_CHAR*)stPartData.pData, EME_UTILS_STRLEN(acMsgPath)) != 0)
            {
               nErr = EME_DataAccess_CopyFile(pvMemMgr, hMgr, acBodyFile, stPartData.pData);
            }
            else
            {
               EME_UTILS_MEMSET(acBodyFile, 0x00, EMEFS_FILE_MAX_LEN);
               EME_UTILS_STRCPY(acBodyFile, stPartData.pData);
            }
         }
         EME_UTILS_BREAKIF(nErr, "copy body file failed!!");
         
         if (NULL != pPath)
         {
            EME_UTILS_MEMCPY(pPath, acBodyFile, EME_UTILS_STRLEN(acBodyFile) + 1);
         }

         stMsgOpt[0].nId = EME_PARTOPT_SRCTYPE;
         stMsgOpt[0].pVal = (WE_VOID *)EMEPART_SRCTYPE_FILE;
         nErr = HMSGPART_UpdateOpt(hMsgPart, 0, &stMsgOpt[0]);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsgPartPriv, update source type option failed");
         
         stMsgOpt[0].nId = EME_PARTOPT_DATABUF;
         stMsgOpt[0].pVal = acBodyFile;
         nErr = HMSGPART_UpdateOpt(hMsgPart, 0, &stMsgOpt[0]);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsgPartPriv, update data buffer failed");
      }

   } while(0);

   return nErr;
}
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgStore_SaveMsgPartPriv
 *
 *    COMMRNT:    Save one message part to the storage
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
static WE_INT32 EME_MsgStore_SaveMsgPartPriv(WE_VOID *pvMemMgr, 
                                             FILEMGR_HANDLE hMgr, 
                                             HMsgPart hMsgPart, 
                                             WE_UINT32 dwMsgId, 
                                             WE_CHAR *pPath)
{
   WE_INT32  nErr = EME_FAILED;
   WE_CHAR   acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
   IWEMsg    *pIMsg = NULL;
   WE_CHAR   *pcMsgFileBuff = NULL;
   WE_UINT32 uiMsgFileSize = 0;

   if (NULL == hMsgPart)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /*generate msg path and msg file*/
      nErr = EME_MsgStore_GenerateMsgPath(dwMsgId, NULL, acMsgFile);
      EME_UTILS_BREAKIF(nErr, "get msg path failed!!");
      
      nErr = EME_MsgStore_UpdateMsgPartPriv(pvMemMgr, hMgr, hMsgPart, dwMsgId, pPath);
      EME_UTILS_BREAKIF(nErr, "update msg part failed!!");

      /*get msg handle from msg value*/
      nErr = IWEMSGVALUE_GetIWEMsg(hMsgPart, &pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg handle failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgFileBuff, &uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "encode msg file buffer failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgFile, pcMsgFileBuff, uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "update msg file failed!!");

   } while(0);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_MsgStore_SaveMsgPart(WE_VOID *pvMemMgr, 
                                  FILEMGR_HANDLE hMgr, 
                                  HMsgPart hMsgPart, 
                                  WE_UINT32 dwMsgId)
{
   return EME_MsgStore_SaveMsgPartPriv(pvMemMgr, hMgr, hMsgPart, dwMsgId, NULL);
}

WE_INT32 EME_MsgStore_SaveMsgHeader(WE_VOID *pvMemMgr, 
                                    FILEMGR_HANDLE hMgr, 
                                    HMultiPartMsg hMsg, 
                                    WE_UINT32 dwMsgId)
{
   WE_INT32    nErr = EME_SUCCESS;
   WE_CHAR     acMsgDataFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     acMsgDataFolder[EMEFS_FILE_MAX_LEN] = {0};
   IWEMsg      *pIMsgOld = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiMsgFileSize = 0;
   WE_CHAR     *pcMsgFileBuff = NULL;

   do 
   {
      nErr = EME_MsgStore_GenerateMsgPath(dwMsgId, acMsgDataFolder, acMsgDataFile);
      EME_UTILS_BREAKIF(nErr, "generate msg path failed!!");

      pIMsgOld = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgDataFile);
      EME_UTILS_BREAKNULL(pIMsgOld, nErr, EME_FAILED, "get old file handle failed!!");

      nErr = IWEMSG_GetHdrValue(hMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get msg value faild!!");

      nErr = IWEMSG_AddHdrValue(pIMsgOld, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add header opt failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsgOld, (WE_UCHAR**)&pcMsgFileBuff, &uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "encode msg failed!!");

      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgDataFile, pcMsgFileBuff, uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "update msg file failed!!");
      
   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsgOld)
      IWEMSG_Release(pIMsgOld);

   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgStore_SaveMsg
 *
 *    COMMRNT:    Save one message to storage
 *
 *    Pseudo Code:    
 *          1. Check input parameters.
 *          2. If is new message create a new message and record the message id
 *             Else check the message id.
 *          3. Save the message header.
 *          4. For each message part, save to storage.
 *          5. Remove the trashy files.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgStore_SaveMsg(WE_VOID *pvMemMgr, 
                              FILEMGR_HANDLE hMgr, 
                              HMultiPartMsg hMsg, 
                              WE_BOOL bIsNew, 
                              WE_BOOL bClean)
{
   WE_INT32  nErr = EME_FAILED;
   WE_UINT32 dwMsgId = 0;
   MsgOpt    aMsgOpt[2] = {0};
   
   WE_UINT32 dwPartNum = 0;
   WE_CHAR   **ppcFileList = NULL;
   WE_CHAR   *pNameBuf = NULL;
   WE_UINT32 dwPartPos = 0;
   HMsgPart  hMsgPart = NULL;
   WE_UINT32 dwActId = 0;
   WE_CHAR   acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR   *pcMsgFileBuff = NULL;
   WE_UINT32 uiMsgFileSize = 0;
   
   if (NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_ACTID, 0, aMsgOpt);
      EME_UTILS_BREAKIF(nErr, "get actid failed!!");
      dwActId = (WE_UINT32)aMsgOpt->pVal;

      if (bIsNew)
      {
         nErr = EME_MsgStore_NewMsg(hMgr, dwActId, &dwMsgId);
         EME_UTILS_BREAKIF(nErr, "create new msg failed!!!");
         
         aMsgOpt[1].nId = MSGOPT_END;
         aMsgOpt[1].pVal = NULL;
         
         nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, aMsgOpt);
         if (EME_SUCCESS == nErr)
         {
            aMsgOpt->pVal = (WE_VOID *)dwMsgId;
            nErr = HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt);
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, update message id option failed");
         }
         else 
         {
            aMsgOpt->nId = EME_MSGOPT_MSGID;
            aMsgOpt->pVal = (WE_VOID *)dwMsgId;
            nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, record message id option failed");
         }
      }
      else 
      {
         nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, aMsgOpt);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, update message no message id exist");

         dwMsgId = (WE_UINT32)aMsgOpt->pVal;
      }
      
      /*generate msg path and msg file*/
      nErr = EME_MsgStore_GenerateMsgPath(dwMsgId, NULL, acMsgFile);
      EME_UTILS_BREAKIF(nErr, "get msg path failed!!");
      
      /*
       *	Then save all message part
       */
      dwPartNum = HMULTIPARTMSG_GetPartCount(hMsg);
      if (0 != dwPartNum)
      {
         /* Allocate memory for file list */
         ppcFileList = (WE_CHAR **)EME_UTILS_MALLOC(pvMemMgr, sizeof(WE_CHAR *) * dwPartNum);
         EME_UTILS_BREAKNULL(ppcFileList, nErr, EME_NOMEMORY, "EME_MsgStore_SaveMsg, no memory for file list");
         EME_UTILS_MEMSET(ppcFileList, 0x00, sizeof(WE_CHAR *) * dwPartNum);
         
         pNameBuf = (WE_CHAR *)EME_UTILS_MALLOC(pvMemMgr, sizeof(WE_CHAR) * EME_MAXPATH * dwPartNum);
         EME_UTILS_BREAKNULL(pNameBuf, nErr, EME_NOMEMORY, "EME_MsgStore_SaveMsg, no memory for name buffer");
         EME_UTILS_MEMSET(pNameBuf, 0x00, sizeof(WE_CHAR) * EME_MAXPATH * dwPartNum);
         
         for (dwPartPos = 0; dwPartPos < dwPartNum; ++dwPartPos)
         {
            ppcFileList[dwPartPos] = pNameBuf + (dwPartPos * sizeof(WE_CHAR) * EME_MAXPATH);
            nErr = HMULTIPARTMSG_GetMessagePart(hMsg, EME_MSGPART_ID_POSITION, (WE_VOID *)dwPartPos, &hMsgPart);
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, get message part failed");
            
            nErr = EME_MsgStore_UpdateMsgPartPriv(pvMemMgr, hMgr, hMsgPart, dwMsgId, ppcFileList[dwPartPos]);
            HMSGPART_Delete(hMsgPart);
            hMsgPart = NULL;
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, save one message part failed");
         }
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, save message parts failed");
      }
      
      nErr = IWEMSG_EncodeMsg(hMsg, (WE_UCHAR**)&pcMsgFileBuff, &uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "encode msg file buffer failed!!");
      
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgFile, pcMsgFileBuff, uiMsgFileSize);
      EME_UTILS_BREAKIF(nErr, "update msg file failed!!");

      if (bClean)
      {
         /* There the third parameter can be null, that means delete all file */
         nErr = EME_MsgStore_RemoveTrashyFiles(hMgr, dwMsgId, ppcFileList, dwPartNum);
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, remove trashy files failed");
      }

      EME_UTILS_FREEIF(pvMemMgr, pNameBuf);
      EME_UTILS_FREEIF(pvMemMgr, ppcFileList);
      
      nErr = EME_SUCCESS;
   } while(0);
   
   if (EME_SUCCESS != nErr)
   {
      EME_UTILS_FREEIF(pvMemMgr, pNameBuf);
      EME_UTILS_FREEIF(pvMemMgr, ppcFileList);
   }
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgStore_WindUp
 *
 *    COMMRNT:    Delete the file in the message folder which is not referenced 
 *                by the message.
 *
 *    Pseudo Code:    
 *          1. Check input parameters.
 *          2. Get the message referenced file list.
 *          3. Remove files not in the list.
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgStore_WindUp(WE_VOID *pvMemMgr, FILEMGR_HANDLE hMgr, HMultiPartMsg hMsg)
{
   WE_INT32 nErr = EME_FAILED;
   WE_UINT32 dwMsgId = 0;
   
   WE_UINT32 dwPartNum = 0;
   WE_CHAR **ppcFileList = NULL;
   WE_UINT32 dwPartPos = 0;
   HMsgPart hMsgPart = NULL;
   MsgOpt stMsgOpt = {0};
   
   if (NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "EME_MsgStore_SaveMsg, update message no message id exist");
      
      dwMsgId = (WE_UINT32)stMsgOpt.pVal;
      
      dwPartNum = HMULTIPARTMSG_GetPartCount(hMsg);
      if (0 != dwPartNum)
      {      
         /* Allocate memory for file list */
         ppcFileList = (WE_CHAR **)EME_UTILS_MALLOC(pvMemMgr, sizeof(WE_CHAR *) * dwPartNum);
         EME_UTILS_BREAKNULL(ppcFileList, nErr, EME_NOMEMORY, "EME_MsgStore_WindUp, no memory for file list");
         EME_UTILS_MEMSET(ppcFileList, 0x00, sizeof(WE_CHAR *) * dwPartNum);
         
         for (dwPartPos = 0; dwPartPos < dwPartNum; ++dwPartPos)
         {
            nErr = HMULTIPARTMSG_GetMessagePart(hMsg, EME_MSGPART_ID_POSITION, (WE_VOID *)dwPartPos, &hMsgPart);
            EME_UTILS_BREAKIF(nErr, "EME_MsgStore_WindUp, get message part failed");
            
            nErr = HMSGPART_GetOpt(hMsgPart, EME_PARTOPT_DATABUF, 0, &stMsgOpt);
            HMSGPART_Delete(hMsgPart);
            hMsgPart = NULL;
            EME_UTILS_BREAKIF(nErr,  "EME_MsgStore_WindUp, get file path failed");
            
            ppcFileList[dwPartPos] = (WE_CHAR *)stMsgOpt.pVal;
         }
         EME_UTILS_BREAKIF(nErr, "EME_MsgStore_WindUp, Get part file list failed");
      }
      
      nErr = EME_MsgStore_RemoveTrashyFiles(hMgr, dwMsgId, ppcFileList, dwPartNum);
      EME_UTILS_BREAKIF(nErr, "EME_MsgStore_WindUp, remove trashy files failed");
      
      nErr = EME_SUCCESS;
   } while (0);
   
   EME_UTILS_FREEIF(pvMemMgr, ppcFileList);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgStore_GetMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgStore_GetMsg(WE_VOID *pvMemMgr, 
                             FILEMGR_HANDLE hMgr, 
                             WE_UINT32 dwMsgId, 
                             HMultiPartMsg hMsg)
{
   WE_INT32 nErr = EME_FAILED;
   IWEMsg   *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   WE_CHAR     acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
   
   if (NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      nErr = EME_MsgStore_GenerateMsgPath(dwMsgId, NULL, acMsgFile);
      EME_UTILS_BREAKIF(nErr, "generate msg path failed!!");

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg file handle failed!!");

      nErr = IWEMSG_GetHdrValue(pIMsg, &pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "get msg header value failed!!");

      nErr = IWEMSG_AddHdrValue(hMsg, pIMsgValue);
      EME_UTILS_BREAKIF(nErr, "add msg header value failed!!");

      IWEMSGVALUE_Release(pIMsgValue);
      pIMsgValue = NULL;

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");

      while(uiIndex < uiCnt)
      {
         WE_INT32    nPos = WEMSG_END_POSTION;
         if(EME_SUCCESS == IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex))
         {
            IWEMSG_AddPartValue(hMsg, pIMsgValue, &nPos);
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex++;
      }

   } while(0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgStore_RemoveMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgStore_RemoveMsg(FILEMGR_HANDLE hMgr, WE_UINT32 dwMsgId)
{
   WE_CHAR  acMsgPath[EMEFS_FILE_MAX_LEN] = {0};

   EME_MsgStore_GenerateMsgPath(dwMsgId, acMsgPath, NULL);

   EME_RemoveDir(hMgr, acMsgPath);

   return EME_SUCCESS;
}
