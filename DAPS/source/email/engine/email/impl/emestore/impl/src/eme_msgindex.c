/*==============================================================================
    FILE NAME   : eme_msgindex.c
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        This file contain implementation for message index mechanism.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-26   Wenkai Xu        01         Create

==============================================================================*/
/*==============================================================================
 * 
 *    Include Files
 * 
 *============================================================================*/
#include "eme_msgindex_api.h"
#include "eme_boxindex_api.h"
#include "eme_utils_api.h"
#include "eme_dataaccess_priv.h"
#include "eme_msgstore_api.h"
#include "we_msg.h"

/*==============================================================================
 * 
 *    DEFINITIONS AND CONSTANTS(It is not recommended)
 *    It is recommended that this section is defined to private head file.
 * 
 *============================================================================*/

/*==============================================================================
 *
 *    Static Function Prototype Declare Section
 *
 *============================================================================*/


/*==============================================================================
 * 
 *    Functions
 * 
 *============================================================================*/
/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgIndex_IndexMsg
 *
 *    COMMRNT:    Add index for one message
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Get the options to fill the info structure
 * 
 *----------------------------------------------------------------------------*/
static WE_UINT32 EME_MsgIndex_AddrNum(WE_UCHAR *pszAddr)
{
   WE_UINT32 dwNum = 0;
   WE_BOOL bSkip = FALSE;

   if (NULL == pszAddr)
   {
      return 0;
   }
   
   if(*pszAddr == L'\0')
   {
      return 0;
   }

   while (*pszAddr != L'\0')
   {
	   if(*pszAddr == L'\"')
	   {
		   bSkip = !bSkip;
	   }

      if (*pszAddr == ADDR_SPACER_WCHAR && !bSkip)
      {
         ++dwNum;
      }

      ++pszAddr;
   }
   
   return ++dwNum;
}


WE_INT32 EME_MsgIndex_IndexMsg(WE_VOID *pvMemMgr, 
                               FILEMGR_HANDLE hMgr, 
                               HMultiPartMsg hMsg, 
                               WE_BOOL bIsNew)
{
   WE_INT32 nErr = EME_FAILED;
   MsgOpt stMsgOpt = {0};
   
   WE_UINT8  *pszFrom = NULL;
   WE_UINT8  *pszSubject = NULL;

   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   MsgOpt      astMsgIndexOpt[20] = {0};
   WE_INT32    iIndex = 0;
   WE_UINT32   uiMsgId = 0;
   WE_CHAR     acTempStr[EMS_EMAIL_ADDRESS_MAX_LENGTH] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32    uiMsgIndexSize = 0;
   WE_UINT32    dwActId = 0;
   
   if (NULL == hMsg)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      /* Get needed info, if lost then return failed */
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_ACTID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_IndexMsg, no account id ");

      dwActId = (WE_UINT32)stMsgOpt.pVal;
      EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
      iIndex++;
      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_BOXID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_IndexMsg, no box id ");

      EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
      iIndex++;
      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_MSGID, 0, &stMsgOpt);
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_IndexMsg, no message id ");

      uiMsgId = (WE_UINT32)stMsgOpt.pVal;
      EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
      iIndex++;
      
      /* Get To address */
      if(EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_BOXFROM, 0, &stMsgOpt))
      {
          EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
          iIndex++;
      }
      else
      {
          astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_BOXFROM;
          astMsgIndexOpt[iIndex].pVal = (WE_VOID*)0;
          iIndex++;
      }

      /* Get To address */
      if(EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_TO, 0, &stMsgOpt))
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_TO;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)stMsgOpt.pVal;
         iIndex++;
      }
      else
      {
          astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_TO;
          astMsgIndexOpt[iIndex].pVal = (WE_VOID*)"\0";
         iIndex++;
      }

      /* Get CC address */
      if(EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_CC, 0, &stMsgOpt))
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_CC;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)stMsgOpt.pVal;
         iIndex++;
      }
      else
      {
          astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_CC;
          astMsgIndexOpt[iIndex].pVal = (WE_VOID*)"\0";
         iIndex++;
      }

      /* Get BCC address */
      if(EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_BCC, 0, &stMsgOpt))
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_BCC;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)stMsgOpt.pVal;
         iIndex++;
      }
      else
      {
          astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_BCC;
          astMsgIndexOpt[iIndex].pVal = (WE_VOID*)"\0";
          iIndex++;
      }

      /* Get optional options */
      if (EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_FROM, 0, &stMsgOpt))
      {
         WE_UINT32 dwFormLen = 0;

         pszFrom = stMsgOpt.pVal;            
         dwFormLen = EME_UTILS_STRLEN((WE_CHAR*)pszFrom) + 1;
         if (dwFormLen > EMS_EMAIL_ADDRESS_MAX_LENGTH + 1) 
         {
            nErr = EME_FAILED;
            EME_UTILS_BREAK(nErr, "EME_MsgIndex_IndexMsg, from string too long");
         }

         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_FROM;
         astMsgIndexOpt[iIndex].pVal = pszFrom;
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_FROM;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)"\0";
         iIndex++;
      }

      /*Get subject*/
      if (EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_SUBJECT, 0, &stMsgOpt))
      {
         WE_UINT32 dwSubjectLen = 0;

         pszSubject = stMsgOpt.pVal;
         
         dwSubjectLen = EME_UTILS_STRLEN((WE_CHAR*)pszSubject) + 1;
         if (dwSubjectLen > EMS_SUBJECT_MAX_LENGTH + 1) 
         {
            dwSubjectLen = EMS_SUBJECT_MAX_LENGTH + 1;
         }

         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_SUBJECT;
         astMsgIndexOpt[iIndex].pVal = pszSubject;
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_SUBJECT;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)"\0";
         iIndex++;
      }

      if (EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_DATE, 0, &stMsgOpt))
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_DATE;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)EME_UTILS_GETSECONDS;
         iIndex++;
      }

      if (EME_SUCCESS == HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_STATUS, 0, &stMsgOpt))
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_STATUS;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)0;
         iIndex++;
      }

      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_TYPE, 0, &stMsgOpt);
      if (EME_SUCCESS == nErr)
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_TYPE;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)EMEMSG_TYPE_EMAIL;
         iIndex++;
      }

      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_EMN_UTCTIME, 0, &stMsgOpt);
      if (EME_SUCCESS == nErr)
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_EMN_UTCTIME;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)0;
         iIndex++;
      }

      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_BEDOWN, 0, &stMsgOpt);
      if (EME_SUCCESS == nErr)
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_BEDOWN;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)0;
         iIndex++;
      }
      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_PRIO, 0, &stMsgOpt);
      if (EME_SUCCESS == nErr)
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_PRIO;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)EME_PRIORITY_NORMAL;
         iIndex++;
      }
      
#if 0      
      nErr = HMULTIPARTMSG_GetOpt(hMsg, EME_MSGOPT_SIZE, 0, &stMsgOpt);
      if(EME_SUCCESS == nErr)
      {
         EME_UTILS_MEMCPY(&astMsgIndexOpt[iIndex], &stMsgOpt, sizeof(MsgOpt));
         iIndex++;
      }
      else
      {
         astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_SIZE;
         astMsgIndexOpt[iIndex].pVal = (WE_VOID*)EME_DataAccess_GetMsgSize(hMgr, uiMsgId);
         iIndex++;
      }
#endif

      astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_STORESIZE;
      astMsgIndexOpt[iIndex].pVal = (WE_VOID*)EME_DataAccess_GetMsgSize(hMgr, uiMsgId);
      iIndex++;
      
      astMsgIndexOpt[iIndex].nId  = EME_MSGOPT_MSGHIDE;
      astMsgIndexOpt[iIndex].pVal = (WE_VOID*)FALSE;
      iIndex++;

      astMsgIndexOpt[iIndex].nId = WEMSG_OPT_END;
      
      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, (WE_UINT32)dwActId);

      /*get msg index file msghandle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index failed");

      if(bIsNew)
      {
         WE_INT32 nPos = WEMSG_END_POSTION;

         IWEMSGVALUE_New(&pIMsgValue);
         EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "create msg value failed!!");

         nErr = IWEMSGVALUE_AddOpt(pIMsgValue, (WEMsgOpt*)astMsgIndexOpt);
         EME_UTILS_BREAKIF(nErr, "add opt to msg value failed!!");

         astMsgIndexOpt[0].nId  = EME_MSGOPT_RECEIVE_DATE;
         astMsgIndexOpt[0].pVal = EME_UTILS_GETSECONDS;
         astMsgIndexOpt[1].nId  = WEMSG_OPT_END;

         nErr = IWEMSGVALUE_AddOpt(pIMsgValue, (WEMsgOpt*)astMsgIndexOpt);
         EME_UTILS_BREAKIF(nErr, "add opt to msg value failed!!");

         nErr = IWEMSG_AddPartValue(pIMsg, pIMsgValue, &nPos);
         EME_UTILS_BREAKIF(nErr, "add part value failed!!");
      }
      else
      {
         WE_UINT32   uiCnt   = 0;
         WE_UINT32   uiIndex = 0;
         WEMsgOpt    stOpt = {0};
         
         IWEMSG_GetPartCount(pIMsg, &uiCnt);
         if(0 == uiCnt)
         {
            nErr = EME_FAILED;
            EME_UTILS_BREAKIF(nErr, "msg info count is 0");
         }

         while(uiIndex < uiCnt)
         {
            if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
            {
               if((IWEMSGVALUE_GetOptByIndex(pIMsgValue, EME_MSGOPT_MSGID, 0, &stOpt) == EME_SUCCESS) && ((WE_UINT32)stOpt.pVal == uiMsgId))
               {
                  break;
               }
               
               IWEMSGVALUE_Release(pIMsgValue);
               pIMsgValue = NULL;
            }

            uiIndex++;
         }

         EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "get msg index failed!!");

         nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)astMsgIndexOpt);
         EME_UTILS_BREAKIF(nErr, "update opt failed!!");
         
      }

      /*encode msg index buffer*/
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgIndexBuf, &uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode msg index buffer failed!!");

      /*update msg index file*/
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgIndexFile, pcMsgIndexBuf, uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "update msg index file failed!!");

      /*Syn  box index file*/
      EME_BoxIndex_SynBoxInfo(hMgr, dwActId);

   } while(0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgIndex_GetMsgIndexInfo
 *
 *    COMMRNT:    Get add message index options into the multipartmsg handler 
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgIndex_GetMsgIndexInfo(FILEMGR_HANDLE hMgr, 
                                      WE_UINT32 dwMsgId, 
                                      HMultiPartMsg hMsg)
{
   WE_INT32 nErr = EME_FAILED;
   MsgOpt aMsgOpt[2] = {0};

   WE_UINT32 uiActId = EME_GET_ACTID_FROM_MSGID(dwMsgId);
   WE_CHAR   acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   IWEMsg    *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   
   if (NULL == hMsg)
   {
      return EME_BADPARM;
   }

   do 
   {
      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, uiActId);

      /*get msg index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg info handle failed!!!");

      /*get msg index count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");

      /*find msg value by msg id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            if((IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&aMsgOpt[0]) == EME_SUCCESS) && (WE_UINT32)aMsgOpt[0].pVal == dwMsgId)
            {
               break;
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex++;
      }

      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "get msg value failed!!1");

      aMsgOpt[1].nId = MSGOPT_END;
      aMsgOpt[1].pVal = NULL;
      
      /*act id*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_ACTID, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");

      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add account id failed");
      
      /*box id*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");

      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add box id failed");
      
      /*msg id*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");

      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add message id failed");
      
      /*be down*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BEDOWN, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");

      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add be down failed");
      
      /*status*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STATUS, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");

      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add message status failed");

      /*size*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STORESIZE, (WEMsgOpt*)&aMsgOpt[0]);
      EME_UTILS_BREAKIF(nErr, "get actid failed");
      
      if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
      {
         nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
      }
      EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add message status failed");
      
      /*eml type*/
      nErr = IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_TYPE, (WEMsgOpt*)&aMsgOpt[0]);
      if(nErr == EME_SUCCESS)
      {
         if(EME_SUCCESS != HMULTIPARTMSG_UpdateOpt(hMsg, 0, aMsgOpt))
         {
            nErr = HMULTIPARTMSG_AddOpt(hMsg, aMsgOpt);
         }
         EME_UTILS_BREAKIF(nErr, "EME_MsgIndex_GetMsgIndexInfo, add message status failed");
      }
      
      nErr = EME_SUCCESS;
   } while(0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}


/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgIndex_MoveMsg
 *
 *    COMMRNT:    Move one message from one box to another box
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. Get the msgextinfo, found out the src msginfo
 *          3. Delete the msginfo from the src box
 *          4. Add this msginfo to dest box
 *          5. Change the msgextinfo
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgIndex_MoveMsg(WE_VOID *pvMemMgr, 
                              FILEMGR_HANDLE hMgr, 
                              WE_UINT32 dwActId, 
                              WE_UINT32 dwSrcBoxId, 
                              WE_UINT32 dwDestBoxId, 
                              WE_UINT32 dwMsgId)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   MsgOpt      astOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;
   
   do 
   {
      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      /*get msg index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file handle failed!!!");

      /*get msg index count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get msg index part count failed!!");

      /*find msg value by msg id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            if((IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&astOpt[0]) == EME_SUCCESS) && ((WE_UINT32)astOpt[0].pVal == dwMsgId))
            {
               break;
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }
         uiIndex++;
      }

      EME_UTILS_BREAKNULL(pIMsgValue, nErr, EME_FAILED, "get msg index part failed!!");

      /*update box id in msg value handle*/
      astOpt[0].nId  = EME_MSGOPT_BOXID;
      astOpt[0].pVal = (WE_VOID*)dwDestBoxId;
      astOpt[1].nId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)astOpt);
      EME_UTILS_BREAKIF(nErr, "update box id failed!");

      /*update box id in msg value handle*/
      astOpt[0].nId  = EME_MSGOPT_BOXFROM;
      astOpt[0].pVal = (WE_VOID*)dwSrcBoxId;
      astOpt[1].nId  = WEMSG_OPT_END;

      nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)astOpt);
      if(WE_NOTFOUND == nErr)
      {
         nErr = IWEMSGVALUE_AddOpt(pIMsgValue, (WEMsgOpt*)astOpt);
      }
      EME_UTILS_BREAKIF(nErr, "update box from failed!");
      
      /*encod msg index file buffer*/
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgIndexBuf, &uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode msg index buffer failed!!!");

      /*update msg index file*/
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgIndexFile, pcMsgIndexBuf, uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "update msg index file failed!");

      EME_BoxIndex_SynBoxInfo(hMgr, dwActId);

   } while(0);
   
   /*free all handle*/
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

WE_INT32 EME_MsgIndex_MoveMsgEx(WE_VOID *pvMemMgr, 
                                FILEMGR_HANDLE hMgr, 
                                WE_UINT32 dwActId, 
                                WE_UINT32 dwSrcBoxId, 
                                WE_UINT32 dwDestBoxId, 
                                WE_UINT32 *pdwMsgId,
                                WE_UINT32 dwMsgNum)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   MsgOpt      astOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;
   WE_UINT32   uiProcessNum = 0;
   
   do 
   {
      if(NULL == pdwMsgId || 0 == dwMsgNum)
      {
         EME_UTILS_LOG_INFO(("bad parameter!!"));
         return EME_BADPARM;
      }

      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      /*get msg index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file handle failed!!!");
      
      /*get msg index count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get msg index part count failed!!");
      
      /*find msg value by msg id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            if((IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&astOpt[0]) == EME_SUCCESS))
            {
               WE_UINT32 uiNum = 0;

               while(uiNum < dwMsgNum)
               {
                  if(((WE_UINT32)astOpt[0].pVal == pdwMsgId[uiNum]))
                  {
                     /*update box id in msg value handle*/
                     astOpt[0].nId  = EME_MSGOPT_BOXID;
                     astOpt[0].pVal = (WE_VOID*)dwDestBoxId;
                     astOpt[1].nId  = WEMSG_OPT_END;
                     
                     nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)astOpt);
                     EME_UTILS_BREAKIF(nErr, "update box id failed!");
                     
                     /*update box id in msg value handle*/
                     astOpt[0].nId  = EME_MSGOPT_BOXFROM;
                     astOpt[0].pVal = (WE_VOID*)dwSrcBoxId;
                     astOpt[1].nId  = WEMSG_OPT_END;
                     
                     nErr = IWEMSGVALUE_UpdateOpt(pIMsgValue, (WEMsgOpt*)astOpt);
                     if(WE_NOTFOUND == nErr)
                     {
                        nErr = IWEMSGVALUE_AddOpt(pIMsgValue, (WEMsgOpt*)astOpt);
                     }
                     EME_UTILS_BREAKIF(nErr, "update box from failed!");

                     uiProcessNum++;
                     break;
                  }
                  uiNum++;
               }
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         if(uiProcessNum == dwMsgNum)
         {
            /*process msg id finished, and break*/
            break;
         }

         uiIndex++;
      }

      /*encod msg index file buffer*/
      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgIndexBuf, &uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode msg index buffer failed!!!");
      
      /*update msg index file*/
      nErr = EME_DataAccess_UpdateFileByBuffer(hMgr, acMsgIndexFile, pcMsgIndexBuf, uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "update msg index file failed!");
      
      EME_BoxIndex_SynBoxInfo(hMgr, dwActId);
      
   } while(0);
   
   /*free all handle*/
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgIndex_RemoveMsg
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgIndex_RemoveMsg(WE_VOID *pvMemMgr, 
                                FILEMGR_HANDLE hMgr, 
                                WE_UINT32 dwMsgId)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   MsgOpt      astOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;
   WE_UINT32   dwActId = EME_GET_ACTID_FROM_MSGID(dwMsgId);
   
   do 
   {
      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      /*get msg index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file handle failed!!!");
      
      /*get msg index file part count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get msg index part count failed!!");
      
      /*find part by msg id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            if((IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&astOpt[0]) == EME_SUCCESS) && ((WE_UINT32)astOpt[0].pVal == dwMsgId))
            {
               /*remove msg part in msg handle*/
               IWEMSG_RemovePartValue(pIMsg, uiIndex);

               IWEMSGVALUE_Release(pIMsgValue);
               pIMsgValue = NULL;

               break;
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
      
   } while(0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_MsgIndex_RemoveMsgEx(WE_VOID *pvMemMgr, 
                                  FILEMGR_HANDLE hMgr, 
                                  WE_UINT32 *pdwMsgId,
                                  WE_UINT32 dwMsgNum)
{
   WE_INT32    nErr = EME_FAILED;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiCnt = 0;
   WE_UINT32   uiIndex = 0;
   MsgOpt      astOpt[2] = {0};
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;
   WE_UINT32   dwActId = 0;
   WE_UINT32   uiProcessNum = 0;
   
   do 
   {
      if(NULL == pdwMsgId || 0 == dwMsgNum)
      {
         EME_UTILS_LOG_INFO(("bad parameter!!!"));
         return EME_BADPARM;
      }

      dwActId = EME_GET_ACTID_FROM_MSGID(pdwMsgId[0]);

      /*get msg index file path*/
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      /*get msg index file handle*/
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file handle failed!!!");
      
      /*get msg index file part count*/
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get msg index part count failed!!");
      
      /*find part by msg id*/
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            if((IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, (WEMsgOpt*)&astOpt[0]) == EME_SUCCESS))
            {
               WE_UINT32 uiNum = 0;
               WE_BOOL   bMatched = FALSE;
               
               while(uiNum < dwMsgNum)
               {
                  if((WE_UINT32)astOpt[0].pVal == pdwMsgId[uiNum])
                  {
                     bMatched = TRUE;
                     break;
                  }
                  uiNum++;
               }
               
               if(bMatched)
               {
                  /*remove msg part in msg handle*/
                  IWEMSG_RemovePartValue(pIMsg, uiIndex);
                  
                  IWEMSGVALUE_Release(pIMsgValue);
                  pIMsgValue = NULL;
                  
                  IWEMSG_GetPartCount(pIMsg, &uiCnt);
                  uiProcessNum++;
                  continue;
               }
            }
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         if(uiProcessNum == dwMsgNum)
         {
            /*process msg id finished, and break*/
            break;
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
      
   } while(0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

/*------------------------------------------------------------------------------
 * 
 *    FUNCTION:   EME_MsgIndex_BoxNameToId
 *
 *    COMMRNT:    
 *
 *    Pseudo Code:    
 *          1. Check input parameters
 *          2. 
 * 
 *----------------------------------------------------------------------------*/
WE_INT32 EME_MsgIndex_BoxNameToId(FILEMGR_HANDLE hMgr, 
                                  WE_UINT16 *pwszBoxName, 
                                  WE_UINT32 dwActId, 
                                  WE_UINT32 *pdwBoxId)
{
   return EME_BoxIndex_GetBoxId(hMgr, dwActId, pwszBoxName, pdwBoxId);
}

WE_INT32 EME_MsgIndex_GetMsgInfoNum(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId, WE_UINT32 *pdwNum)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiNum = 0;
   WE_UINT32   uiMis = EME_UTILS_GETTIMEMS;

   if(0 == dwActId || NULL == pdwNum)
   {
      return EME_BADPARM;
   }

   do 
   {
      *pdwNum = 0;

      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");

      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            WE_UINT32 uiBoxId = 0;
            WE_UINT32 uiMsgId = 0;
            WE_BOOL   bHide = FALSE;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, &stOpt);
            uiBoxId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt);
            uiMsgId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGHIDE, &stOpt);
            bHide = (WE_BOOL)(WE_UINT32)stOpt.pVal;

            if(!bHide && (uiBoxId == dwBoxId || EMS_BOX_ID_ALL == dwBoxId))
            {
               uiNum++;
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex++;
      }

      *pdwNum = uiNum;

   } while (0);

   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
  
   return nErr;
}

WE_INT32 EME_MsgIndex_GetMsgInfo(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId, WE_UINT32 dwNum, EMS_MsgInfo *pastMsgInfo)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiPos = 0;
   WE_UINT32   uiMis = EME_UTILS_GETTIMEMS;
   
   if(0 == dwBoxId || 0 == dwActId || 0 == dwNum || NULL == pastMsgInfo)
   {
      return EME_BADPARM;
   }
   
   do 
   {     
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      while(uiIndex < uiCnt && uiPos < dwNum)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt stOpt = {0};
            WE_UINT32 uiBoxId = 0;
            WE_UINT32 uiMsgId = 0;
            WE_BOOL   bHide = FALSE;


            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, &stOpt);
            uiBoxId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt);
            uiMsgId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGHIDE, &stOpt);
            bHide = (WE_BOOL)(WE_UINT32)stOpt.pVal;

            if(!bHide &&  (uiBoxId == dwBoxId))
            {
               /*box id*/
               pastMsgInfo[uiPos].dwBoxId = uiBoxId;
               pastMsgInfo[uiPos].dwMsgId = uiMsgId;

               /*act id*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_ACTID, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwActId = (WE_UINT32)stOpt.pVal;
               }

               /*box from*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXFROM, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwBoxFrom= (WE_UINT32)stOpt.pVal;
               }

               /*msg size*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STORESIZE, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwMsgSize = (WE_UINT32)stOpt.pVal;
               }

               /*msg date*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_DATE, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwDate = (WE_UINT32)stOpt.pVal;
               }

               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_RECEIVE_DATE, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwReceDate = (WE_UINT32)stOpt.pVal;
               }

               /*msg type*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_TYPE, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwEmlType = (WE_UINT32)stOpt.pVal;
               }

               /*msg status*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STATUS, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwEmlStatus = (WE_UINT32)stOpt.pVal;
               }

               /*subject*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_SUBJECT, &stOpt) == EME_SUCCESS)
               {
                  WE_UINT16 *pwszStr = EME_UTILS_UTF8TOWSTR(NULL, (WE_CHAR*)stOpt.pVal);
                  if(NULL != pwszStr)
                  {
                     EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszSubject, pwszStr, 
                         (EME_UTILS_WSTRLEN(pwszStr) > EMS_SUBJECT_MAX_LENGTH ? EMS_SUBJECT_MAX_LENGTH : EME_UTILS_WSTRLEN(pwszStr)) * sizeof(WE_UINT16));
                     EME_UTILS_FREEIF(NULL, pwszStr);
                  }
               }

               /*To Address*/
               {
                  WE_CHAR   *pszTo = NULL;
                  WE_CHAR   *pszCc = NULL;
                  WE_UINT32 dwAddrNum = 0;
                  WE_UINT16 *pwszStr = NULL;

                  /* Get the address */
                  if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_TO, &stOpt))
                  {
                     pszTo = stOpt.pVal;
                     dwAddrNum += EME_MsgIndex_AddrNum(pszTo);
                  }
                  
                  if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_CC, &stOpt))
                  {
                     pszCc = stOpt.pVal;
                     dwAddrNum += EME_MsgIndex_AddrNum(pszCc);
                  }
                                    
                  if (dwAddrNum == 1)
                  {
                     WE_UINT8 *pszTmp = NULL;
                     WE_UINT8 *pszTmp1 = NULL;
                     WE_UINT32 dwToLen = 0;
                     
                     if (NULL != pszTo && EME_UTILS_STRLEN(pszTo) > 0)
                     {
                        pszTmp = pszTo;
                     }
                     else if (NULL != pszCc && EME_UTILS_STRLEN(pszCc) > 0)
                     {
                        pszTmp = pszCc;
                     }
                     pszTmp1 = EME_UTILS_STRCHR(pszTmp, '<');
                     if(NULL != pszTmp1)
                     {
                         pszTmp = pszTmp1;
                     }
                     else
                     {
                         EME_UTILS_LOG_INFO(("EME_MsgIndex_GetMsgInfo to address have no <>"));
                     }
                     
                     dwToLen = EME_UTILS_STRLEN((WE_CHAR*)pszTmp) + 1;
                     if (dwToLen > EMS_EMAIL_ADDRESS_MAX_LENGTH + 1) 
                     {
                        nErr = EME_FAILED;
                        EME_UTILS_BREAK(nErr, "EME_MsgIndex_IndexMsg, to string too long");
                     }
                     pwszStr = EME_UTILS_UTF8TOWSTR(NULL, pszTmp);
                     EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszTo, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                     EME_UTILS_FREEIF(NULL, pwszStr);
                  }
                  else if (dwAddrNum > 1)
                  {
                     WE_CHAR     acTempStr[EMS_EMAIL_ADDRESS_MAX_LENGTH] = {0};

                     EME_UTILS_SPRINTF(acTempStr, "%d %s", dwAddrNum, "recipients");
                     
                     pwszStr =EME_UTILS_UTF8TOWSTR(NULL, acTempStr);
                     EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszTo, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                     EME_UTILS_FREEIF(NULL, pwszStr);
                  }
               }

               /*from address*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_FROM, &stOpt) == EME_SUCCESS)
               {
                  WE_UINT16 *pwszStr = EME_UTILS_UTF8TOWSTR(NULL, (WE_CHAR*)stOpt.pVal);
                  if(NULL != pwszStr)
                  {
                     EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszFrom, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                     EME_UTILS_FREEIF(NULL, pwszStr);
                  }
               }

               /*msg prio*/
               if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_PRIO, &stOpt) == EME_SUCCESS)
               {
                  pastMsgInfo[uiPos].dwMsgPrio = (WE_UINT32)stOpt.pVal;
               }

               uiPos++;
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

WE_INT32 EME_MsgIndex_CreateEmptyMsgIndex(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32     nErr = EME_SUCCESS;
   IWEMsg       *pIMsg = NULL;
   WE_CHAR      acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32    uiMsgIndexSize = 0;
   WE_CHAR      *pcMsgIndexBuff = NULL;

   if(0 == dwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);

      IWEMSG_New(&pIMsg);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "create msg handle failed!!");

      nErr = IWEMSG_EncodeMsg(pIMsg, (WE_UCHAR**)&pcMsgIndexBuff, &uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "encode msg failed");

      nErr = EME_DataAccess_CreateFileByBuffer(hMgr, acMsgIndexFile, pcMsgIndexBuff, uiMsgIndexSize);
      EME_UTILS_BREAKIF(nErr, "create msg index file failed");

   } while (0);

   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);

   return nErr;
}

static WE_BOOL EME_MsgIndex_MsgIsExist(FILEMGR_HANDLE hMgr, WE_UINT32 uiMsgId)
{
   WE_CHAR  acMsgFile[EMEFS_FILE_MAX_LEN] = {0};
   
   if(0 == uiMsgId)
   {
      return FALSE;
   }

   EME_MsgStore_GenerateMsgPath(uiMsgId, NULL, acMsgFile);
   
   if(EMS_SAVE_PLACE_SDCARD1 == EME_GET_PLACE_FROM_MSGID(uiMsgId))
   {
      if(EME_SDCard1Vailed(hMgr) && EME_FileExists(hMgr, acMsgFile))
      {
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   else if(EMS_SAVE_PLACE_SDCARD2 == EME_GET_PLACE_FROM_MSGID(uiMsgId))
   {
      if(EME_SDCard2Vailed(hMgr) && EME_FileExists(hMgr, acMsgFile))
      {
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   
   return TRUE;
}

WE_INT32 EME_MsgIndex_SyncMsgIndexInfo(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32   uiMsgIndexSize = 0;

   if(0 == dwActId)
   {
      return EME_BADPARM;
   }
   
   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt  stOpt[2] = {0};
            WE_UINT32 uiMsgId = 0;
            WE_CHAR   acMsgDataFile[EMEFS_FILE_MAX_LEN] = {0};
            
            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt[0]);
            uiMsgId = (WE_UINT32)stOpt[0].pVal;
            
            if(EME_MsgIndex_MsgIsExist(hMgr, uiMsgId))
            {
               stOpt[0].iId  = EME_MSGOPT_MSGHIDE;
               stOpt[0].pVal = (WE_VOID*)FALSE;
               stOpt[1].iId  = WEMSG_OPT_END;
            }
            else
            {
               stOpt[0].iId  = EME_MSGOPT_MSGHIDE;
               stOpt[0].pVal = (WE_VOID*)TRUE;
               stOpt[1].iId  = WEMSG_OPT_END;
            }

            if(IWEMSGVALUE_UpdateOpt(pIMsgValue, stOpt) != WE_SUCCESS)
            {
               IWEMSGVALUE_AddOpt(pIMsgValue, stOpt);
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
   
   return nErr;
}

WE_INT32 EME_MsgIndex_SearchMsgInfo(FILEMGR_HANDLE hMgr, 
                                    WE_UINT32      dwActId,
                                    WE_UINT32      dwBoxId, 
                                    WE_CHAR        *pcFileter, 
                                    EMS_MsgInfo    *pstMsgInfo,
                                    WE_UINT32      *pdwNum
                                    )
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_UINT32   uiPos = 0;
   WE_UINT32   uiMis = EME_UTILS_GETTIMEMS;
   EMS_MsgInfo *pastMsgInfo = pstMsgInfo;
   
   if(0 == dwActId || NULL == pdwNum || NULL == pcFileter)
   {
      return EME_BADPARM;
   }
   
   do 
   {     
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      if(0 == uiCnt)
      {
         EME_UTILS_LOG_INFO(("the msg count is 0!!"));
         break;
      }

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt  stOpt = {0};
            WE_UINT32 uiBoxId = 0;
            WE_UINT32 uiMsgId = 0;
            WE_BOOL   bHide = FALSE;
            WE_BOOL   bMatch = FALSE;
            WE_CHAR   *pszTo = NULL;
            WE_CHAR   *pszCc = NULL;
            WE_CHAR   *pszSubject = NULL;
            WE_CHAR   *pszFrom = NULL;
            WE_UINT32 dwAddrNum = 0;


            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, &stOpt);
            uiBoxId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt);
            uiMsgId = (WE_UINT32)stOpt.pVal;

            IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGHIDE, &stOpt);
            bHide = (WE_BOOL)(WE_UINT32)stOpt.pVal;

            if(!bHide &&  (uiBoxId == dwBoxId || EMS_BOX_ID_ALL == dwBoxId))
            {
               /* Get To address */
               if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_TO, &stOpt))
               {
                  pszTo = (WE_CHAR*)stOpt.pVal;

                  dwAddrNum += EME_MsgIndex_AddrNum(pszTo);
                  if(!bMatch && EME_UTILS_STRSTR(pszTo, pcFileter) != NULL)
                  {
                     bMatch = TRUE;
                  }
               }

               /* Get CC address */
               if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_CC, &stOpt))
               {
                  pszCc = (WE_CHAR*)stOpt.pVal;
                  
                  dwAddrNum += EME_MsgIndex_AddrNum(pszCc);
                  if(!bMatch && EME_UTILS_STRSTR(pszCc, pcFileter) != NULL)
                  {
                     bMatch = TRUE;
                  }
               }

               /* Get From address */
               if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_FROM, &stOpt))
               {
                  pszFrom = (WE_CHAR*)stOpt.pVal;
                  
                  if(!bMatch && EME_UTILS_STRSTR(pszFrom, pcFileter) != NULL)
                  {
                     bMatch = TRUE;
                  }
               }

               /* Get Subject address */
               if (EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_SUBJECT, &stOpt))
               {
                  pszSubject = (WE_CHAR*)stOpt.pVal;
                  
                  if(!bMatch && EME_UTILS_STRSTR(pszSubject, pcFileter) != NULL)
                  {
                     bMatch = TRUE;
                  }
               }

               if(bMatch)
               {
                  WE_UINT16 *pwszStr = NULL;

                  if(NULL != pastMsgInfo && uiPos < *pdwNum)
                  {
                     if (dwAddrNum == 1)
                     {
                        WE_UINT8  *pszTmp = NULL;
                        WE_UINT32 dwToLen = 0;
                        
                        if (NULL != pszTo)
                        {
                           pszTmp = pszTo;
                        }
                        else if (NULL != pszCc)
                        {
                           pszTmp = pszCc;
                        }
                        
                        dwToLen = EME_UTILS_STRLEN((WE_CHAR*)pszTmp) + 1;
                        if (dwToLen > EMS_EMAIL_ADDRESS_MAX_LENGTH + 1) 
                        {
                           nErr = EME_FAILED;
                           EME_UTILS_BREAK(nErr, "EME_MsgIndex_IndexMsg, to string too long");
                        }
                        pwszStr = EME_UTILS_UTF8TOWSTR(NULL, pszTmp);
                        EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszTo, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                        EME_UTILS_FREEIF(NULL, pwszStr);
                     }
                     else if (dwAddrNum > 1)
                     {
                        WE_CHAR   acTempStr[EMS_EMAIL_ADDRESS_MAX_LENGTH] = {0};
                        WE_UINT16 *pwszStr = NULL;
                        
                        EME_UTILS_SPRINTF(acTempStr, "%d %s", dwAddrNum, "recipients");
                        
                        pwszStr =EME_UTILS_UTF8TOWSTR(NULL, acTempStr);
                        EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszTo, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                        EME_UTILS_FREEIF(NULL, pwszStr);
                     }
                     
                     if(NULL != pszFrom)
                     {
                        pwszStr = EME_UTILS_UTF8TOWSTR(NULL, pszFrom);
                        EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszFrom, pwszStr, EME_UTILS_WSTRLEN(pwszStr) * sizeof(WE_UINT16));
                        EME_UTILS_FREEIF(NULL, pwszStr);
                     }
                     
                     if(NULL != pszSubject)
                     {
                        pwszStr = EME_UTILS_UTF8TOWSTR(NULL, pszSubject);
                        EME_UTILS_MEMCPY((WE_CHAR*)pastMsgInfo[uiPos].wszSubject, pwszStr, 
                           (EME_UTILS_WSTRLEN(pwszStr) > EMS_SUBJECT_MAX_LENGTH ? EMS_SUBJECT_MAX_LENGTH : EME_UTILS_WSTRLEN(pwszStr)) * sizeof(WE_UINT16));
                        EME_UTILS_FREEIF(NULL, pwszStr);
                     }
                     
                     /*box id*/
                     pastMsgInfo[uiPos].dwBoxId = uiBoxId;
                     pastMsgInfo[uiPos].dwMsgId = uiMsgId;
                     
                     /*msg prio*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_PRIO, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwMsgPrio = (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*act id*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_ACTID, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwActId = (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*box from*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXFROM, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwBoxFrom= (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*msg size*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STORESIZE, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwMsgSize = (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*msg date*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_DATE, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwDate = (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*msg type*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_TYPE, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwEmlType = (WE_UINT32)stOpt.pVal;
                     }
                     
                     /*msg status*/
                     if(IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_STATUS, &stOpt) == EME_SUCCESS)
                     {
                        pastMsgInfo[uiPos].dwEmlStatus = (WE_UINT32)stOpt.pVal;
                     }
                  }

                  uiPos++;
               }//if(bMatch)

            }//if(!bHide &&  (uiBoxId == dwBoxId || EMS_BOX_ID_ALL == dwBoxId)
            
            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;

         }//if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)

         uiIndex++;
      }

      *pdwNum = uiPos;
     
   } while (0);
   
   if(NULL != pIMsgValue)
      IWEMSGVALUE_Release(pIMsgValue);
   
   if(NULL != pIMsg)
      IWEMSG_Release(pIMsg);
   
   return nErr;
}

WE_INT32 EME_MsgIndex_RecoverMsg(WE_VOID *pvMemMgr, 
                                 FILEMGR_HANDLE hMgr,
                                 WE_UINT32 dwActId,
                                 WE_UINT32 *pdwMsgId,
                                 WE_UINT32 dwMsgNum)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32    uiMsgIndexSize = 0;

   if(NULL == pdwMsgId || 0 == dwMsgNum || 0 == dwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      if(0 == uiCnt)
      {
         EME_UTILS_LOG_INFO(("the msg count is 0!!"));
         break;
      }

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt  stOpt = {0};
            WE_UINT32 uiMsgIndex = 0;

            if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt))
            {
               WE_BOOL bMatch = FALSE;
               while(uiMsgIndex < dwMsgNum)
               {
                  if(pdwMsgId[uiMsgIndex] == (WE_UINT32)stOpt.pVal)
                  {
                     bMatch = TRUE;
                     break;
                  }

                  uiMsgIndex++;
                  continue;
                  
               }

               if(bMatch)
               {
                  WE_UINT32 uiBoxForm = 0;
                  WEMsgOpt  stUpdateOpt[3] = {0};

                  IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXFROM, &stOpt);
                  uiBoxForm = (WE_UINT32)stOpt.pVal;
                  if(uiBoxForm != 0)
                  {
                     stUpdateOpt[0].iId  = EME_MSGOPT_BOXID;
                     stUpdateOpt[0].pVal = (WE_VOID*)uiBoxForm;
                     stUpdateOpt[1].iId  = EME_MSGOPT_BOXFROM;
                     stUpdateOpt[1].pVal = (WE_VOID*)0;
                     stUpdateOpt[2].iId  = WEMSG_OPT_END;

                     IWEMSGVALUE_UpdateOpt(pIMsgValue, stUpdateOpt);
                  }

               }//end if(bMatch)

            }//end if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt))

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;

         }// end if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)

         uiIndex ++;
      }// end while(uiIndex < uiCnt)

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

   return nErr;
}

WE_INT32 EME_MsgIndex_CleanMsg(WE_VOID *pvMemMgr, 
                               FILEMGR_HANDLE hMgr,
                               WE_UINT32 dwActId,
                               WE_UINT32 dwBoxId,
                               WE_UINT32 dwSaveplace)
{
   WE_INT32    nErr = EME_SUCCESS;
   IWEMsg      *pIMsg = NULL;
   IWEMsgValue *pIMsgValue = NULL;
   WE_UINT32   uiIndex = 0;
   WE_UINT32   uiCnt = 0;
   WE_CHAR     acMsgIndexFile[EMEFS_FILE_MAX_LEN] = {0};
   WE_CHAR     *pcMsgIndexBuf = NULL;
   WE_UINT32    uiMsgIndexSize = 0;

   if(0 == dwActId)
   {
      return EME_BADPARM;
   }

   do 
   {
      EME_UTILS_SPRINTF(acMsgIndexFile, EMEFS_MSG_INDEX_FILE, dwActId);
      
      pIMsg = EME_DataAccess_GetIWEMsgByFile(hMgr, acMsgIndexFile);
      EME_UTILS_BREAKNULL(pIMsg, nErr, EME_FAILED, "get msg index file failed!!");
      
      nErr = IWEMSG_GetPartCount(pIMsg, &uiCnt);
      EME_UTILS_BREAKIF(nErr, "get part count failed!!");
      
      if(0 == uiCnt)
      {
         EME_UTILS_LOG_INFO(("the msg count is 0!!"));
         break;
      }

      while(uiIndex < uiCnt)
      {
         if(IWEMSG_GetPartValue(pIMsg, &pIMsgValue, uiIndex) == EME_SUCCESS)
         {
            WEMsgOpt  stOpt = {0};

            if(EME_SUCCESS == IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_MSGID, &stOpt))
            {
               WE_UINT32 uiMsgId = (WE_UINT32)stOpt.pVal;

               IWEMSGVALUE_GetOpt(pIMsgValue, EME_MSGOPT_BOXID, &stOpt);

               if(EME_GET_PLACE_FROM_MSGID(uiMsgId) == dwSaveplace 
                  && ((WE_UINT32)stOpt.pVal == dwBoxId || 0 == (WE_UINT32)stOpt.pVal))
               {
                  IWEMSG_RemovePartValue(pIMsg, uiIndex);
                  EME_MsgStore_RemoveMsg(hMgr, uiMsgId);
                  
                  IWEMSGVALUE_Release(pIMsgValue);
                  pIMsgValue = NULL;

                  IWEMSG_GetPartCount(pIMsg, &uiCnt);
                  continue;
               }
            }

            IWEMSGVALUE_Release(pIMsgValue);
            pIMsgValue = NULL;
         }

         uiIndex ++;
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

   return nErr;
}