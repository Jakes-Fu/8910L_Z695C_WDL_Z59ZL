/*=====================================================================================
    FILE: eme_pop_cmdlist.c
    MODULE NAME : POP 
    

    GENERAL DESCRIPTION:
    Implement of Pop3 Stack cmdlist

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================   
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/08/2007        Hubin                            creat this file
    04/29/2007        Hubin                            add apop cmd
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#include "eme_file_api.h"
#include "eme_pop_prv.h"
#include "eme_dataaccess_priv.h"
//#include "AEESecurity.h"		//	AEE security Services

#define   EME_TEMP_MAIL_FILE_DIR          EMEFS_TEMP_ROOT_DIR /*BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_PATH)//"\\hz\\email\\"*/
#define   EME_BUFFER_CATCH_SIZE            (10 * 1024)
#define   EME_MAX_EMAIL_FILE_NAME_LEN      (30)
#define   EME_MAX_EMAIL_BODY_LEN      200000

WE_INT32 EME_POP_RETR_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_SINT32 sResult = -1;								//String Compare result,if 0 means the end of response
   WE_UINT32 uMail_ID = 0;                      //init it to "0"zero code
   POP_NULL_ASSERT(pcx);	
   if(PopRetr != pcx->Pop_CMD)					//Must be CMD"RETR"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(pcx->bSpecial)									//If fetching special mail
      uMail_ID = pcx->uMailSpec_id;
   else
      uMail_ID = pcx->uMailAuto_id;
   POP_LESS_NOTIFY(pcx,uMail_ID,1);
   POP_MORE_NOTIFY(pcx,uMail_ID,pcx->uMaillist_len+1);
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, RETR_CMD,uMail_ID);		//in the mail list "1" means "0" in the program list
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         WE_CHAR *pcTempFilePath = NULL;
         
         if('+' != pcx->Response[0])							//Not the list response
         {
            if(pcx->bSpecial)
            {
               pcx->PopResult.detail.stFetchSpRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
            }
            else
            {
               pcx->PopResult.detail.stFetchAllRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
            }
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         }
         pcx->Pop_Status = ClientAwaiting_Data;
         if(NULL == pcx->pMail_Buffer)
            pcx->pMail_Buffer = EME_UTILS_MALLOC(pcx->pMemHdr, EME_BUFFER_CATCH_SIZE);
         POP_NOMEM_NOTIFY(pcx,pcx->pMail_Buffer);             //Malloc error
         
         EME_UTILS_LOG_INFO(("EME_POP_RETR_CMD--: pMail_Buffer = 0x%08x", pcx->pMail_Buffer));
         
         EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
         pcx->pAppend_Header = pcx->pMail_Buffer;			//init the append point to the buffer begining
         pcx->uMailBuff_Size = EME_BUFFER_CATCH_SIZE;    //init the mail size
         pcx->uRetred_Size = 0;

/*         
         if(BRWENG10_GETACTIVEPATHTYPE()==E_ACTIVE_PATH_IS_CARD)
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_CARD_PATH);
         }
         else
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_PATH);
         }
*/
         pcTempFilePath = EMEFS_TEMP_ROOT_DIR;

         if(NULL == pcx->pcTmpFileName)
            pcx->pcTmpFileName = EME_UTILS_MALLOC(pcx->pMemHdr, EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         POP_NOMEM_NOTIFY(pcx,pcx->pcTmpFileName);
         EME_UTILS_MEMSET(pcx->pcTmpFileName, 0, EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         EME_UTILS_SPRINTF(pcx->pcTmpFileName, "%s\\eml%x.eml", pcTempFilePath, EME_UTILS_GETTIMEMS);
         if(NULL == pcx->pvTmpEmailFile)
         {
            pcx->pvTmpEmailFile = EME_FileOpen(pcx->pvFileMgr, pcx->pcTmpFileName, EME_OFM_CREATE);
            POP_NOMEM_NOTIFY(pcx,pcx->pvTmpEmailFile);
         }
      }
   }
   
   if(ClientAwaiting_Data == pcx->Pop_Status)
   {
      while(0 != sResult)
      {
         iResult = EME_POP_Getting_Response(pcx);
         if(EME_SOCKET_RECV_NODATA == iResult)
            return EME_SUCCESS;
         else if(iResult&EME_ERR_SOCKET_START)
         {
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            pcx->pMail_Buffer = NULL;
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            return EME_POP_Notify(pcx,iResult,EME_FAILED);	
         }
         
         sResult = EME_UTILS_STRCMP(pcx->Response,".\r\n");		//If goes end
         if(0 == sResult)
         {
            EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            
            EME_UTILS_LOG_INFO(("EME_POP_RETR_CMD--: free pMail_Buffer = 0x%08x", pcx->pMail_Buffer));
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            pcx->pMail_Buffer = NULL;
            break;
         }
         //			if('+' != pcx->Response[0])					//If not the "+ok" response,store it in the Uidl list buffer
         {
            WE_UINT32 uStrlen = EME_UTILS_STRLEN(pcx->Response);
            if(0 == uStrlen)						//not blank word
               return EME_SUCCESS;
            pcx->uRetred_Size += uStrlen;
            
            if((EME_UTILS_STRLEN(pcx->pMail_Buffer) + uStrlen) > EME_BUFFER_CATCH_SIZE)
            {
               EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
               EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
               pcx->pAppend_Header = pcx->pMail_Buffer;
            }
            ///////////////////////////////////////////////////
            EME_UTILS_SPRINTF(pcx->pAppend_Header,"%s",pcx->Response);
            pcx->pAppend_Header += uStrlen;			//let the append point to the new point position
            {
               WE_INT32 iEvtCodeTemp = 0;
               iEvtCodeTemp = pcx->PopResult.iEvCode;
               pcx->PopResult.iEvCode = POP_EVT_SIZE_NOTIFY;
               pcx->PopResult.detail.stSizeNotifyRet.uiTotalSize = pcx->ppUidl_List[uMail_ID-1].uSize;
               pcx->PopResult.detail.stSizeNotifyRet.uiCurrSize = pcx->uRetred_Size;              
               EME_POP_Notify(pcx,EME_SUCCESS,EME_SUCCESS);
               pcx->PopResult.iEvCode = iEvtCodeTemp;
            }
         }			
      }
      //The Response goes end.
      //Change to no CMD,Status "None",Register no callback
#if 0
      *(pcx->pAppend_Header-2) = '\0';						//Server will end the response with"\r\n.\r\n"
#endif																//so delete the first "\r\n" in the mail buffer
      //(the last "\r\n" is not in the mail buffer)
      pcx->pAppend_Header = NULL;
      pcx->Pop_CMD = PopNone;											
      pcx->Pop_Status = Client_Idle;
      //		pcx->ppUidl_List[uMail_ID-1].bFresh = FALSE;//If the mails is fresh is depond on mgr
      if(pcx->bSpecial)
      {
         pcx->PopResult.detail.stFetchSpRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stFetchSpRet.iFetchedSize = pcx->ppUidl_List[uMail_ID-1].uSize;    //not including '\0'
         pcx->PopResult.detail.stFetchSpRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
      }
      else
      {
         pcx->PopResult.detail.stFetchAllRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stFetchAllRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
         pcx->PopResult.detail.stFetchAllRet.iFetchedSize = pcx->ppUidl_List[uMail_ID-1].uSize;   //not including '\0'
      }
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
      pcx->iMailFetched ++;
      return EME_POP_SuccessNotify(pcx,iResult);
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}



WE_INT32 EME_POP_TOP_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_SINT32 sResult = -1;				//String Compare result,if 0 means the end of response
   WE_UINT32 uMail_ID = 0;
   POP_NULL_ASSERT(pcx);	
   EME_UTILS_LOG_INFO(("EME_POP_TOP_CMD : EME_POP_Sending_CMD "));
   
   if(PopTop != pcx->Pop_CMD)					//Must be CMD"Top"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(pcx->bSpecial)									//If fetching special mail
      uMail_ID = pcx->uMailSpec_id;
   else
      uMail_ID = pcx->uMailAuto_id;
   POP_LESS_NOTIFY(pcx,uMail_ID,1);
   POP_MORE_NOTIFY(pcx,uMail_ID,pcx->uMaillist_len+1);
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, TOP_CMD,uMail_ID);			//in the mail list "1" means "0" in the program list
      iResult = EME_POP_Sending_CMD(pcx);
      EME_UTILS_LOG_INFO(("EME_POP_TOP_CMD : EME_POP_Sending_CMD return :%s", pcx->Command));
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      EME_UTILS_LOG_INFO(("EME_POP_TOP_CMD : EME_POP_Getting_Response return :%s", pcx->Response));
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);	
      else
      {
         WE_CHAR *pcTempFilePath = NULL;
         
         if('+' != pcx->Response[0])							//Not the list response
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         pcx->Pop_Status = ClientAwaiting_Data;
         if(NULL == pcx->pMail_Buffer)
            pcx->pMail_Buffer = EME_UTILS_MALLOC(pcx->pMemHdr, EME_BUFFER_CATCH_SIZE);
         POP_NOMEM_NOTIFY(pcx, pcx->pMail_Buffer);
         
         EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
         pcx->pAppend_Header = pcx->pMail_Buffer;
         pcx->uMailBuff_Size = EME_BUFFER_CATCH_SIZE;
         pcx->uRetred_Size = 0;
         EME_UTILS_LOG_INFO(("EME_POP_TOP_CMD :   pcx->pAppend_Header =%s\n",   pcx->pAppend_Header));

/*         
         if(E_ACTIVE_PATH_IS_CARD == BRWENG10_GETACTIVEPATHTYPE())
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_CARD_PATH);
         }
         else
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_PATH);
         }
*/

         pcTempFilePath = EMEFS_TEMP_ROOT_DIR;

         if(NULL == pcx->pcTmpFileName)
            pcx->pcTmpFileName = EME_UTILS_MALLOC(pcx->pMemHdr, EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         POP_NOMEM_NOTIFY(pcx,pcx->pcTmpFileName);
         EME_UTILS_MEMSET(pcx->pcTmpFileName, 0, EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         EME_UTILS_SPRINTF(pcx->pcTmpFileName, "%s\\eml%x.eml", pcTempFilePath,  EME_UTILS_GETTIMEMS);
         if(NULL == pcx->pvTmpEmailFile)
         {
            pcx->pvTmpEmailFile = EME_FileOpen(pcx->pvFileMgr, pcx->pcTmpFileName, EME_OFM_CREATE);
            POP_NOMEM_NOTIFY(pcx,pcx->pvTmpEmailFile);
         }
         EME_UTILS_LOG_INFO(("EME_POP_TOP_CMD :   pcx->pcTmpFileName =%s\n",   pcx->pcTmpFileName));
         
      }
   }
   
   if(ClientAwaiting_Data == pcx->Pop_Status)
   {
      while(0 != sResult)
      {
         iResult = EME_POP_Getting_Response(pcx);
         if(EME_SOCKET_RECV_NODATA == iResult)
            return EME_SUCCESS;
         else if(iResult&EME_ERR_SOCKET_START)
         {
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            pcx->pMail_Buffer = NULL;
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            return EME_POP_Notify(pcx,iResult,EME_FAILED);
         }
         
         sResult = EME_UTILS_STRCMP(pcx->Response,".\r\n");		//If goes end
         if(0 == sResult)
         {
            EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            pcx->pMail_Buffer = NULL;
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            break;
         }
         //			if('+' != pcx->Response[0])					//If not the "+ok" response,store it in the Uidl list buffer
         {
            WE_UINT32 uStrlen = EME_UTILS_STRLEN(pcx->Response);
            if(0 == uStrlen)						//not blank word
               return EME_SUCCESS;
            pcx->uRetred_Size += uStrlen;
            
            if((EME_UTILS_STRLEN(pcx->pMail_Buffer) + uStrlen) > EME_BUFFER_CATCH_SIZE)
            {
               EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
               EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
               pcx->pAppend_Header = pcx->pMail_Buffer;
            }
            
            EME_UTILS_SPRINTF(pcx->pAppend_Header,"%s",pcx->Response);
            pcx->pAppend_Header += uStrlen;			//let the append point to the new point position
         }			
      }
      //The Response goes end.
      //Change to no CMD,Status "None",Register no callback
#if 0
      *(pcx->pAppend_Header-2) = '\0';						//Server will end the response with"\r\n.\r\n"
#endif																//so delete the first "\r\n" in the mail buffer
      //(the last "\r\n" is not in the mail buffer)
      //sResult = EME_UTILS_STRLEN(pcx->pMail_Buffer);
      pcx->pAppend_Header = NULL;
      pcx->Pop_CMD = PopNone;											
      pcx->Pop_Status = Client_Idle;
      //      pcx->ppUidl_List[uMail_ID-1].bFresh = FALSE;  //If the mail is fresh is depend on mgr
      if(pcx->bSpecial)
      {
         pcx->PopResult.detail.stSpHeaderRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stSpHeaderRet.iFetchedSize = pcx->uRetred_Size-3;    //not including '\0'
         pcx->PopResult.detail.stSpHeaderRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
      }
      else
      {
         pcx->PopResult.detail.stAllHeaderRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stSpHeaderRet.iFetchedSize = pcx->uRetred_Size-3;    //not including '\0'
         pcx->PopResult.detail.stAllHeaderRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
      }
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
      pcx->iMailFetched ++;
      return EME_POP_SuccessNotify(pcx,iResult);
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}




WE_INT32 EME_POP_QUIT_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   
   POP_NULL_ASSERT(pcx);						//If Null,return error		
   if(PopQuit != pcx->Pop_CMD)					//Must be CMD"DELE"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, "%s", QUIT_CMD);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         if('+' != pcx->Response[0])					//Bad response
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         //Now CMD"QUIT" is successfully passed
         //Change to no CMD,Status "idle",Register no the CMD callback
         pcx->Pop_CMD = PopNone;											
         pcx->Pop_Status = Client_Idle;
         EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
         do
         {
            //Close Socket
            iResult = EME_Socket_Close(pcx->pvSocket);
            POP_FALSE_BREAK(iResult);
            pcx->pvSocket = NULL;         //After closed,the point must be set to null
            //Before Quit,Update the local records
            iResult = EME_POP_UpdateUidlLocal(pcx);
            //Reset ALL first
            (void)EME_POP_ResetALL(pcx);
            POP_FALSE_BREAK(iResult);
            return EME_POP_SuccessNotify(pcx,iResult);
         }while(0);
         //If any of the process failed
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      }
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 EME_POP_DELE_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_UINT32 uMail_ID = 0;
   POP_NULL_ASSERT(pcx);					   	//If Null,return error	
   if(PopDele != pcx->Pop_CMD)					//Must be CMD"DELE"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(pcx->bSpecial)									//If deletinging special mail
      uMail_ID = pcx->uMailSpec_id;
   else
      uMail_ID = pcx->uMailAuto_id;
   POP_LESS_NOTIFY(pcx,uMail_ID,1);
   POP_MORE_NOTIFY(pcx,uMail_ID,pcx->uMaillist_len+1);
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, DELE_CMD,uMail_ID);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         if('+' != pcx->Response[0])					//Bad response
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         //Now CMD"DELE" is successfully passed
         //Change to no CMD,Status "idle",Register no the CMD callback
         pcx->Pop_CMD = PopNone;											
         pcx->Pop_Status = Client_Idle;
         pcx->ppUidl_List[uMail_ID-1].bExisted = FALSE;
         EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
         return EME_POP_SuccessNotify(pcx,iResult);
      }
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 EME_POP_STAT_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pcx);	
   if(PopStat != pcx->Pop_CMD)					//Must be CMD"STAT"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD	
   {
      EME_UTILS_SPRINTF(pcx->Command, "%s", STAT_CMD);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      char * pNum = NULL;
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      if('+' != pcx->Response[0])							//Not the stat response
         return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);	
      pNum = pcx->Response + 4;                   //Leave "+ok" behind
      pcx->uMaillist_len = EME_UTILS_STRTOUL(pNum,NULL,10);//Get the total message Num
      pNum = EME_UTILS_STRCHR(pNum,' ');                    //Find the second ' '
      if(NULL == pNum)
          return EME_FAILED;
      pNum ++;                                     //Behind the seond is the first point to the total size strings
      pcx->uMailTotal_Size = EME_UTILS_STRTOUL(pNum,NULL,10);//Get the total message size
      if(0 == pcx->uMaillist_len)					//Total num is zero
         return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
      
      if(0 == pcx->iLatest)
      {
         pcx->uMailAuto_id = 0;
      }
      else
      {
         pcx->uMailAuto_id = pcx->uMaillist_len + 1;
      }
      if(NULL == pcx->ppUidl_List)           //First should be a null point
         pcx->ppUidl_List = (St_EME_PopUidl *)EME_UTILS_MALLOC(pcx->pMemHdr,sizeof(St_EME_PopUidl)*(pcx->uMaillist_len + 1));
      if(NULL == pcx->ppUidl_List)           //Memory error
         return EME_POP_Notify(pcx,EME_NOMEMORY,EME_FAILED);
      //Change to UIDL CMD,Status "Sending CMD",Register Uidl callback
      EME_UTILS_MEMSET(pcx->ppUidl_List,0,sizeof(St_EME_PopUidl)*(pcx->uMaillist_len + 1));
      pcx->Pop_CMD = PopUidl;											
      pcx->Pop_Status = ClientSending_CMD;
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_UIDL_CMD);
      return EME_POP_UIDL_CMD(pcx);	
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 EME_POP_UIDL_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_SINT32 sResult = -1;				//String Compare result,if 0 means the end of response
   POP_NULL_ASSERT(pcx);	
   if(PopUidl != pcx->Pop_CMD)					//Must be CMD"Pass"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, "%s", UIDL_CMD);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);			//Get Response"+ok"
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      if('+' != pcx->Response[0])							//Not the uidl response
         return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
      pcx->Pop_Status = ClientAwaiting_Data;
   }
   if(ClientAwaiting_Data == pcx->Pop_Status)
   {
      while(0 != sResult)
      {
         iResult = EME_POP_Getting_Response(pcx);
         if(EME_SOCKET_RECV_NODATA == iResult)
            return EME_SUCCESS;
         else if(iResult&EME_ERR_SOCKET_START)
            return EME_POP_Notify(pcx,iResult,EME_FAILED);		 
         sResult = EME_UTILS_STRCMP(pcx->Response,".\r\n");		//If goes end
         if(0 == sResult)
            break;
         //			if('+' != pcx->Response[0])					//If not the "+ok" response,store it in the Uidl list buffer
         {
            WE_UINT32 uMailId = EME_UTILS_STRTOUL(pcx->Response,NULL,10);
            WE_CHAR * pHash	 = NULL;				//The uidl list begin
            WE_UINT32 uStrlen = EME_UTILS_STRLEN(pcx->Response);
            if(0 == uStrlen)						//not blank word
               return EME_SUCCESS;
            POP_LESS_NOTIFY(pcx,uMailId,1);
            POP_MORE_NOTIFY(pcx,uMailId,pcx->uMaillist_len+1);
            if(NULL == pcx->ppUidl_List[uMailId-1].pData)
               pcx->ppUidl_List[uMailId-1].pData = (WE_CHAR *)EME_UTILS_MALLOC(pcx->pMemHdr,sizeof(WE_CHAR)*(EME_MAX_UIDL_LEN+1));
            POP_NOMEM_NOTIFY(pcx,pcx->ppUidl_List[uMailId-1].pData);             //Malloc error
            EME_UTILS_MEMSET(pcx->ppUidl_List[uMailId-1].pData, 0, sizeof(WE_CHAR)*(EME_MAX_UIDL_LEN+1));
            pHash = EME_UTILS_STRCHR(pcx->Response,' ');
            POP_NULL_NOTIFY(pcx,pHash);
            EME_UTILS_SPRINTF(pcx->ppUidl_List[uMailId-1].pData,"%s",pHash+1);
            pcx->ppUidl_List[uMailId-1].bFresh = TRUE;		//Treat every mails as new before they are not compared with local uidl file
            pcx->ppUidl_List[uMailId-1].bExisted = TRUE;	//In the beginning,the mail of course exists
         }			
      }
      //The Response goes end.
      //Change to List CMD,Status "LIST",Register list callback
      pcx->Pop_CMD = PopList;											
      pcx->Pop_Status = ClientSending_CMD;
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_LIST_CMD);
      return EME_POP_LIST_CMD(pcx);
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 EME_POP_LIST_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_SINT32 sResult = -1;				//String Compare result,if 0 means the end of response
   POP_NULL_ASSERT(pcx);	
   if(PopList != pcx->Pop_CMD)					//Must be CMD"LIST"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, "%s", LIST_CMD);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         if('+' != pcx->Response[0])							//Not the list response
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         pcx->Pop_Status = ClientAwaiting_Data;
      }
   }
   if(ClientAwaiting_Data == pcx->Pop_Status)
   {
      while(0 != sResult)
      {
         iResult = EME_POP_Getting_Response(pcx);
         if(EME_SOCKET_RECV_NODATA == iResult)
            return EME_SUCCESS;
         else if(iResult&EME_ERR_SOCKET_START)
            return EME_POP_Notify(pcx,iResult,EME_FAILED);		 
         sResult = EME_UTILS_STRCMP(pcx->Response,".\r\n");		//If goes end
         if(0 == sResult)
            break;
         if('+' != pcx->Response[0])					//If not the "+OK" response,store it in the Uidl list buffer
         {
            WE_UINT32 uMailId = EME_UTILS_STRTOUL(pcx->Response,NULL,10);
            WE_CHAR * pHash	 = NULL;				//The Uidl list begin
            WE_UINT32 uStrlen = EME_UTILS_STRLEN(pcx->Response);
            if(0 == uStrlen)						//not blank word
               return EME_SUCCESS;
            POP_LESS_NOTIFY(pcx,uMailId,1);
            POP_MORE_NOTIFY(pcx,uMailId,pcx->uMaillist_len+1);
            pHash = EME_UTILS_STRCHR(pcx->Response,' ');
            if(NULL == pHash)
                return EME_FAILED;
            pHash ++;
            if(NULL != pcx->ppUidl_List)
               pcx->ppUidl_List[uMailId-1].uSize = EME_UTILS_STRTOUL(pHash,NULL,10);
         }			
      }
      //The Response goes end.
      //Change to no CMD,Status "None",Register no callback
      pcx->Pop_CMD = PopNone;											
      pcx->Pop_Status = Client_Idle;
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
      //Check New Mail Status
      iResult = EME_POP_CheckNewMails(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      pcx->PopResult.detail.stGetInfoRet.iTotalSize = pcx->uMailTotal_Size;
      pcx->PopResult.detail.stGetInfoRet.iTotalNum = pcx->uMaillist_len;
      //Is it useful?
      pcx->PopResult.detail.stGetInfoRet.piSizeList = NULL;
      return EME_POP_SuccessNotify(pcx,iResult);
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 EME_POP_USER_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pcx);						//If Null,return error
   if(PopUser != pcx->Pop_CMD)					//Must be CMD"USER"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      if(TRUE == pcx->bAuth)
      {
         //         WE_CHAR pcMd5digest[MD5_LENGTH*2+1];
         //         EME_UTILS_MEMSET(pcMd5digest,0,MD5_LENGTH*2+1);
         //         iResult = EME_POP_MD5Hash(pcx,pcMd5digest);
         //         if(EME_SUCCESS != iResult)
         //            return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);
         //         EME_UTILS_SPRINTF(pcx->Command, APOP_CMD,pcx->UserName,pcMd5digest);   
      }
      else
         EME_UTILS_SPRINTF(pcx->Command, USER_CMD,pcx->UserName);    	
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         if('+' != pcx->Response[0])					//Not the pass response
            return EME_POP_Notify(pcx,EME_LOGIN_ERROR,EME_FAILED);
         else										//Now CMD"USER" is successfully passed
         {
            //Change to CMD"PASS",Status "Sending",Register new the CMD callback
            if(TRUE == pcx->bAuth)
            {
               //Change to no CMD,Status "Idle",Register no callback
               pcx->Pop_CMD = PopNone;											
               pcx->Pop_Status = Client_Idle;
               EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
               return EME_POP_SuccessNotify(pcx,iResult);
            }
            else
            {
               pcx->Pop_CMD = PopPass;											
               pcx->Pop_Status = ClientSending_CMD;
               EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_PASS_CMD);
               return EME_POP_PASS_CMD(pcx);				//First,Send the CMD"PASS" 
            }
         }
      }
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}


WE_INT32 EME_POP_PASS_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pcx);						//If Null,return error
   if(PopPass != pcx->Pop_CMD)					//Must be CMD"Pass"
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);  //pcx->Pop_CMD is not a invald data
   if(ClientSending_CMD == pcx->Pop_Status)		//It is sending CMD
   {
      EME_UTILS_SPRINTF(pcx->Command, PASS_CMD,pcx->Password);		
      iResult = EME_POP_Sending_CMD(pcx);
      if(EME_SUCCESS != iResult)
         return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
      else
         pcx->Pop_Status = ClientAwaiting_Response;
      
   }
   else if(ClientAwaiting_Response == pcx->Pop_Status)	//It is receiving response
   {
      iResult = EME_POP_Getting_Response(pcx);
      if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
         return EME_SUCCESS;      
      if(iResult&EME_ERR_SOCKET_START)
         return EME_POP_Notify(pcx,iResult,EME_FAILED);
      else
      {
         if('+' != pcx->Response[0])					//Not the pass response
            return EME_POP_Notify(pcx,EME_LOGIN_ERROR,EME_FAILED);
         else										//Now CMD"Pass" is successfully passed
         {
            //Change to no CMD,Status "Idle",Register no callback
            pcx->Pop_CMD = PopNone;											
            pcx->Pop_Status = Client_Idle;
            EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
            return EME_POP_SuccessNotify(pcx,iResult);
         }
      }
   }
   if(ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status)
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); //pcx->Pop_Status is invalid data
   //////////////////////////////////////////
   return iResult;
}

WE_INT32 
EME_EPOP_PROF_CMD(IPOPHANDLER  pPopClient)
{
   WE_INT32 iResult = EME_SUCCESS;
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   
   POP_NULL_ASSERT(pcx);	
   
   if (EPopProf != pcx->Pop_CMD){
      
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);
   }
   
   if (ClientSending_CMD == pcx->Pop_Status){
      
      pcx->Pop_Status = ClientAwaiting_Response;
      
      if (NULL == pcx->pcUseCompress){
         
         EME_UTILS_SPRINTF(pcx->Command, "PROF %s\r\n",pcx->pcConvertCharset);
      }
      else{
         
         EME_UTILS_SPRINTF(pcx->Command, "PROF %s %s\r\n",pcx->pcConvertCharset, pcx->pcUseCompress);	
      }
      
      iResult = EME_POP_Sending_CMD(pcx);
      
      if (EME_SUCCESS != iResult){
         
         return EME_POP_Notify(pcx, EME_SOCKET_NET_ERR, EME_FAILED);
      }
   }
   else if (ClientAwaiting_Response == pcx->Pop_Status){
      
      iResult = EME_POP_Getting_Response(pcx);
      
      if (EME_SOCKET_RECV_NODATA == iResult){
         
         return EME_SUCCESS;
      }
      
      if (iResult & EME_ERR_SOCKET_START){
         
         return EME_POP_Notify(pcx, iResult, EME_FAILED);
      }
      else{
         
         if ('+' != pcx->Response[0]){
            
            return EME_POP_Notify(pcx, EME_SERVER_RESPONSE_ERR, EME_FAILED);
         }
         else{
            
            pcx->Pop_CMD = PopNone;											
            pcx->Pop_Status = Client_Idle;
            
            EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_POP_DummyCallback);
            
            return EME_POP_SuccessNotify(pcx, iResult);
         }
      }
   }
   
   if (ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status){
      
      return EME_POP_Notify(pcx, EME_INVALID_DATA, EME_FAILED);
   }
   
   return iResult;
}

WE_INT32 
EME_EPOP_ERET_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult = EME_SUCCESS;
   WE_SINT32 sResult = -1;						
   WE_UINT32 uMail_ID = 0;    
   WE_UINT32 uiResponseLen = 0;
   WE_CHAR *pcUid = NULL;
   WE_UINT32 uiUidLen = 0;
   
   POP_NULL_ASSERT(pcx);	
   
   if (EPopEret != pcx->Pop_CMD){
      
      return EME_POP_Notify(pcx, EME_INVALID_DATA, EME_FAILED);
   }
   
   if (pcx->bSpecial){
      
      uMail_ID = pcx->uMailSpec_id;
   }
   else{
      
      uMail_ID = pcx->uMailAuto_id;
   }
   
   POP_LESS_NOTIFY(pcx,uMail_ID,1);
   POP_MORE_NOTIFY(pcx,uMail_ID,pcx->uMaillist_len+1);
   
   
   
   if (ClientSending_CMD == pcx->Pop_Status){
      
      
      uiUidLen = EME_UTILS_STRLEN(pcx->ppUidl_List[uMail_ID-1].pData);
      pcUid = EME_UTILS_MALLOC(pcx->pMemHdr, uiUidLen);
      if (NULL == pcUid)
      {
         return EME_FAILED;
      }
      EME_UTILS_MEMSET(pcUid, 0, uiUidLen);
      EME_UTILS_MEMCPY(pcUid, pcx->ppUidl_List[uMail_ID-1].pData, uiUidLen-2);
      
      pcx->Pop_Status = ClientAwaiting_Response;
      if (pcx->bSpecial && NULL != pcx->pcContentID){
         
         if (NULL != pcx->pcConvertType){
            
            EME_UTILS_SPRINTF(pcx->Command, 
               "ERET %s %s %s\r\n", 
               pcUid,
               pcx->pcContentID,
               pcx->pcConvertType);
         }
         else{
            
            EME_UTILS_SPRINTF(pcx->Command, 
               "ERET %s %s\r\n", 
               pcUid,
               pcx->pcContentID);
         }
      }
      else{
#if 1
         EME_UTILS_SPRINTF(pcx->Command, 
            "ERET %s 0 txt 0 %d\r\n", 
            pcUid, EME_MAX_EMAIL_BODY_LEN);
#else
         EME_UTILS_SPRINTF(pcx->Command, 
            "ERET %s\r\n", 
            pcUid);
#endif
      }
      
      EME_UTILS_FREEIF(pcx->pMemHdr, pcUid);
      
      iResult = EME_POP_Sending_CMD(pcx);
      
      if (EME_SUCCESS != iResult){
         
         return EME_POP_Notify(pcx, EME_SOCKET_NET_ERR,EME_FAILED);
      }
   }
   else if (ClientAwaiting_Response == pcx->Pop_Status){
      
      iResult = EME_POP_Getting_Response(pcx);
      
      if (EME_SOCKET_RECV_NODATA == iResult){
         
         return EME_SUCCESS;
      }
      
      if (iResult & EME_ERR_SOCKET_START){
         
         return EME_POP_Notify(pcx, iResult, EME_FAILED);
      }
      else
      {
         WE_CHAR *pcTempFilePath = NULL;
         
         if ('+' != pcx->Response[0]) 
         {
            if(pcx->bSpecial)
            {
               pcx->PopResult.detail.stFetchSpRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
            }
            else
            {
               pcx->PopResult.detail.stFetchAllRet.pcFetchedID = pcx->ppUidl_List[uMail_ID-1].pData;
            }
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         }
         
         pcx->Pop_Status = ClientAwaiting_Data;
         
         if (NULL == pcx->pMail_Buffer){
            
            pcx->pMail_Buffer = EME_UTILS_MALLOC(pcx->pMemHdr, EME_BUFFER_CATCH_SIZE);
         }
         
         POP_NOMEM_NOTIFY(pcx, pcx->pMail_Buffer);
         EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
         
         pcx->pAppend_Header = pcx->pMail_Buffer;
         pcx->uMailBuff_Size = EME_BUFFER_CATCH_SIZE;
         pcx->uRetred_Size = 0;

/*
         if(BRWENG10_GETACTIVEPATHTYPE() == E_ACTIVE_PATH_IS_CARD)
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_CARD_PATH);
         }
         else
         {
            pcTempFilePath = BRWENG10_GETDIRPATH(E_PATH_TYPE_EMAIL_TEMP_PATH);
         }
*/

         pcTempFilePath = EMEFS_TEMP_ROOT_DIR;

         if(NULL == pcx->pcTmpFileName){
            pcx->pcTmpFileName = EME_UTILS_MALLOC(pcx->pMemHdr, EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         }
         
         POP_NOMEM_NOTIFY(pcx,pcx->pcTmpFileName);
         
         EME_UTILS_MEMSET(pcx->pcTmpFileName, 
            0, 
            EME_UTILS_STRLEN(pcTempFilePath) + EME_MAX_EMAIL_FILE_NAME_LEN);
         
         EME_UTILS_SPRINTF(pcx->pcTmpFileName, 
            "%s\\eml%x.eml", 
            pcTempFilePath,
            EME_UTILS_GETTIMEMS);
         
         if(NULL == pcx->pvTmpEmailFile){
            
            pcx->pvTmpEmailFile = EME_FileOpen(pcx->pvFileMgr, pcx->pcTmpFileName, EME_OFM_CREATE);
            POP_NOMEM_NOTIFY(pcx,pcx->pvTmpEmailFile);
         }
      }
   }
   
   if (ClientAwaiting_Data == pcx->Pop_Status){
      
      while (0 != sResult){
         
         iResult = EME_POP_Getting_Response(pcx);
         
         if (EME_SOCKET_RECV_NODATA == iResult){
            
            return EME_SUCCESS;
         }
         else if (iResult & EME_ERR_SOCKET_START){
            
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            
            return EME_POP_Notify(pcx, iResult, EME_FAILED);
         }
         
         sResult = EME_UTILS_STRCMP(pcx->Response, ".\r\n");
         
         if (0 == sResult){
            
            EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
            
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            
            EME_FileClose(pcx->pvTmpEmailFile);
            pcx->pvTmpEmailFile = NULL;
            
            break;
         }
         
         uiResponseLen = EME_UTILS_STRLEN(pcx->Response);
         
         if (0 == uiResponseLen){
            
            return EME_SUCCESS;
         }
         
         pcx->uRetred_Size += uiResponseLen;
         
         if (EME_UTILS_STRLEN(pcx->pMail_Buffer) + uiResponseLen > EME_BUFFER_CATCH_SIZE){
            
            EME_FileWrite(pcx->pvTmpEmailFile, pcx->pMail_Buffer, EME_UTILS_STRLEN(pcx->pMail_Buffer));
            
            EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
            
            pcx->pAppend_Header = pcx->pMail_Buffer;
         }
         
         EME_UTILS_SPRINTF(pcx->pAppend_Header, "%s", pcx->Response);
         pcx->pAppend_Header += uiResponseLen;	
         
         {
            WE_INT32 iEvtCodeTemp = 0;
            iEvtCodeTemp = pcx->PopResult.iEvCode;
            pcx->PopResult.iEvCode = POP_EVT_SIZE_NOTIFY;
            pcx->PopResult.detail.stSizeNotifyRet.uiTotalSize = pcx->ppUidl_List[uMail_ID - 1].uSize;
            pcx->PopResult.detail.stSizeNotifyRet.uiCurrSize = pcx->uRetred_Size;     
            
            EME_POP_Notify(pcx, EME_SUCCESS, EME_SUCCESS);
            pcx->PopResult.iEvCode = iEvtCodeTemp;
         }
      }
      
      pcx->pAppend_Header = NULL;
      pcx->Pop_CMD = PopNone;											
      pcx->Pop_Status = Client_Idle;
      
      if (pcx->bSpecial){
         
         pcx->PopResult.detail.stFetchSpRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stFetchSpRet.iFetchedSize = pcx->ppUidl_List[uMail_ID - 1].uSize;
         pcx->PopResult.detail.stFetchSpRet.pcFetchedID = pcx->ppUidl_List[uMail_ID - 1].pData;
      }
      else{
         
         pcx->PopResult.detail.stFetchAllRet.pcDataBuffer = pcx->pcTmpFileName;
         pcx->PopResult.detail.stFetchAllRet.pcFetchedID = pcx->ppUidl_List[uMail_ID - 1].pData;
         pcx->PopResult.detail.stFetchAllRet.iFetchedSize = pcx->ppUidl_List[uMail_ID - 1].uSize;
      }
      
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
      
      pcx->iMailFetched++;
      
      return EME_POP_SuccessNotify(pcx,iResult);
   }
   
   if (ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status){
      
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); 
   }
   
   return iResult;
}

WE_INT32 EME_EPOP_ESET_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult = EME_SUCCESS;
   WE_SINT32 sResult = -1;						
   WE_UINT32 uMail_ID = 0;    
   WE_UINT32 uiResponseLen = 0;
   WE_CHAR *pcUid = NULL;
   WE_UINT32 uiUidLen = 0;
   
   POP_NULL_ASSERT(pcx);
   POP_NULL_ASSERT(pcx->pstEpopSet);
   
   if (EPopEset != pcx->Pop_CMD)
   {
      return EME_POP_Notify(pcx, EME_INVALID_DATA, EME_FAILED);
   }
   
   if (ClientSending_CMD == pcx->Pop_Status)
   {
      pcx->Pop_Status = ClientAwaiting_Response;
      
      if(NULL != pcx->pstEpopSet->pcFilter)
      {
         /* ESET FILTER [<sub-para>] */
         EME_UTILS_SPRINTF(pcx->Command, "XSET FILTER\r\n");
      }
      else
      {
         //            EME_UTILS_SPRINTF(pcx->Command, "ESET %s\r\n", pcx->pstEpopSet->pcValue);
         EME_UTILS_SPRINTF(pcx->Command, "SET %s\r\n", pcx->pstEpopSet->pcValue);
      }
      
      iResult = EME_POP_Sending_CMD(pcx);
      
      if (EME_SUCCESS != iResult)
      {
         return EME_POP_Notify(pcx, EME_SOCKET_NET_ERR,EME_FAILED);
      }
   }
   else if (ClientAwaiting_Response == pcx->Pop_Status)
   {
      iResult = EME_POP_Getting_Response(pcx);
      
      if (EME_SOCKET_RECV_NODATA == iResult)
      {
         return EME_SUCCESS;
      }
      
      if (iResult & EME_ERR_SOCKET_START)
      {
         return EME_POP_Notify(pcx, iResult, EME_FAILED);
      }
      else
      {
         if ('+' != pcx->Response[0])
         {
            return EME_POP_Notify(pcx, EME_SERVER_RESPONSE_ERR, EME_FAILED);
         }
         
         if(NULL != pcx->pstEpopSet->pcFilter)
         {
            //                EME_UTILS_SPRINTF(pcx->Command, "%s", pcx->pstEpopSet->pcFilter);
            pcx->pcFilter = pcx->pstEpopSet->pcFilter;
            iResult = EME_POP_Sending_CMD(pcx);
            pcx->Pop_Status = ClientAwaiting_Response;
            
            if (EME_SUCCESS != iResult)
            {
               return EME_POP_Notify(pcx, EME_SOCKET_NET_ERR,EME_FAILED);
            }
            pcx->pcFilter = NULL;
            pcx->pstEpopSet->pcFilter = NULL;
            return iResult;
         }
         
         pcx->Pop_Status = Client_Idle;
         pcx->Pop_CMD = PopNone;											
         
         free(pcx->pstEpopSet);
         pcx->pstEpopSet = NULL;
         
         return EME_POP_SuccessNotify(pcx,iResult);
      }
   }
   
   if (ClientAwaiting_Data == pcx->Pop_Status)
   {
   }
   
   if (ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
   {
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); 
   }
   
   return iResult;
}

WE_INT32 EME_EPOP_EGET_CMD(IPOPHANDLER  pPopClient)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32 iResult = EME_SUCCESS;
   WE_SINT32 sResult = -1;						
   WE_UINT32 uMail_ID = 0;    
   WE_UINT32 uiResponseLen = 0;
   WE_CHAR *pcUid = NULL;
   WE_UINT32 uiUidLen = 0;
   
   POP_NULL_ASSERT(pcx);
   
   if (EPopEget != pcx->Pop_CMD)
   {
      return EME_POP_Notify(pcx, EME_INVALID_DATA, EME_FAILED);
   }
   
   if (ClientSending_CMD == pcx->Pop_Status)
   {
      pcx->Pop_Status = ClientAwaiting_Response;
      
      EME_UTILS_SPRINTF(pcx->Command, "XGET FILTER\r\n");
      
      iResult = EME_POP_Sending_CMD(pcx);
      
      if (EME_SUCCESS != iResult)
      {
         return EME_POP_Notify(pcx, EME_SOCKET_NET_ERR,EME_FAILED);
      }
   }
   else if (ClientAwaiting_Response == pcx->Pop_Status)
   {
      iResult = EME_POP_Getting_Response(pcx);
      
      if (EME_SOCKET_RECV_NODATA == iResult)
      {
         return EME_SUCCESS;
      }
      
      if (iResult & EME_ERR_SOCKET_START)
      {
         return EME_POP_Notify(pcx, iResult, EME_FAILED);
      }
      else
      {
         if ('+' != pcx->Response[0])
         {
            return EME_POP_Notify(pcx, EME_SERVER_RESPONSE_ERR, EME_FAILED);
         }
         pcx->Pop_Status = ClientAwaiting_Data;
         
         if (NULL == pcx->pMail_Buffer)
         {
            pcx->pMail_Buffer = EME_UTILS_MALLOC(pcx->pMemHdr, EME_BUFFER_CATCH_SIZE);
         }
         POP_NOMEM_NOTIFY(pcx, pcx->pMail_Buffer);
         EME_UTILS_MEMSET(pcx->pMail_Buffer, 0, EME_BUFFER_CATCH_SIZE);
         
         pcx->pAppend_Header = pcx->pMail_Buffer;
         pcx->uMailBuff_Size = EME_BUFFER_CATCH_SIZE;
         pcx->uRetred_Size = 0;
         
      }
   }
   
   if (ClientAwaiting_Data == pcx->Pop_Status)
   {
      while (0 != sResult)
      {
         iResult = EME_POP_Getting_Response(pcx);
         
         if (EME_SOCKET_RECV_NODATA == iResult)
         {
            return EME_SUCCESS;
         }
         else if (iResult & EME_ERR_SOCKET_START)
         {
            EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pMail_Buffer);
            return EME_POP_Notify(pcx, iResult, EME_FAILED);
         }
         
         sResult = EME_UTILS_STRCMP(pcx->Response, ".\r\n");
         if (0 == sResult)
         {
            //pcx->pMail_Buffer, notify app
            break;
         }
         
         uiResponseLen = EME_UTILS_STRLEN(pcx->Response);
         if (0 == uiResponseLen)
         {
            return EME_SUCCESS;
         }
         
         pcx->uRetred_Size += uiResponseLen;
         
         EME_UTILS_SPRINTF(pcx->pAppend_Header, "%s", pcx->Response);
         pcx->pAppend_Header += uiResponseLen;	
         
      }
      
      pcx->pAppend_Header = NULL;
      pcx->Pop_CMD = PopNone;											
      pcx->Pop_Status = Client_Idle;
      
      pcx->PopResult.detail.stFetchAllRet.pcDataBuffer = pcx->pMail_Buffer;
      
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
      
      return EME_POP_SuccessNotify(pcx,iResult);
   }
   
   if (ClientSending_CMD != pcx->Pop_Status &&
      ClientAwaiting_Response != pcx->Pop_Status &&
      ClientAwaiting_Data != pcx->Pop_Status)
   {
      return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED); 
   }
   
   return iResult;
}

