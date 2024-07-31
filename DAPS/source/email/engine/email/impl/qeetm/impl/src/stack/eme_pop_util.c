/*=====================================================================================
    FILE: eme_pop_util.c
    MODULE NAME : POP

  
    GENERAL DESCRIPTION:
    Implement of Pop3 Stack utils
 
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
               EDIT HISTORY FOR FILE
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
#include "eme_pop_api.h"
//#include "AEESecurity.h"		//	AEE security Services

#define MAX_INT_STR_LEN       11

static WE_INT32  Get_FileBreaker(WE_INT32 iFileNum);

WE_INT32 EME_POP_DoRecv(IPOPHANDLER  pPopClient)
{
	//ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	return EME_SUCCESS;
	
}

WE_INT32 EME_POP_DetectWelcome(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	POP_NULL_ASSERT(pcx);			//If Null,return error
	if(PopNone != pcx->Pop_CMD)
		return iResult;
	iResult = EME_POP_Getting_Response(pcx);
	if(EME_SOCKET_RECV_NODATA == iResult)              //Avoid response not a whole line
		return EME_SUCCESS;      

	if(iResult&EME_ERR_SOCKET_START)
		return EME_POP_Notify(pcx,iResult,EME_FAILED);
	//detetct welcome message(note:pop3 will give only one single line less than
   //512 char as welcome message
	if('+' != pcx->Response[0])					//Not the pass response
		return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);   
	if('\n' != pcx->Response[EME_UTILS_STRLEN(pcx->Response)-1])  //not welcomed
		return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
	else		
	{
		if(TRUE == pcx->bAuth)
      {
         WE_INT32 StampLen = 0;
         WE_INT32 PassLen = 0;
         WE_CHAR * pBegin = EME_UTILS_STRCHR(pcx->Response,'<');
         WE_CHAR * pEnd = EME_UTILS_STRCHR(pcx->Response,'>');
         if(NULL == pBegin || NULL == pEnd)
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         PassLen = EME_UTILS_STRLEN(pcx->Password);
         StampLen =(WE_INT32)(pEnd-pBegin+1);
         if(0 >= PassLen)
            return EME_POP_Notify(pcx,EME_ACCOUNT_INVAL,EME_FAILED);
         if(0 >= StampLen)
            return EME_POP_Notify(pcx,EME_SERVER_RESPONSE_ERR,EME_FAILED);
         pcx->pTimeStamp = EME_UTILS_MALLOC(pcx->pMemHdr,StampLen+PassLen+1);
         POP_NULL_NOTIFY(pcx,pcx->pTimeStamp);
         EME_UTILS_MEMSET(pcx->pTimeStamp,0,StampLen+PassLen+1);
         EME_UTILS_STRNCPY(pcx->pTimeStamp,pBegin,StampLen);
         EME_UTILS_STRNCPY(pcx->pTimeStamp+StampLen,(const char *)pcx->Password,PassLen);
      }
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DummyCallback);
		return EME_POP_SuccessNotify(pcx,iResult);
	}
}





WE_INT32 EME_POP_Retrive(IPOPHANDLER pPopClient,WE_BOOL bHeader,WE_SINT32 sMailID)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_INT32    iMailId = 0;
   POP_NULL_ASSERT(pcx);									//If Null,return error
   pcx->bSizeLimitExceed = FALSE;
   pcx->bHeader = bHeader;
   if(sMailID > 0)  //Fetch special,whole or header
   {
      if(TRUE == pcx->bHeader)
         pcx->PopResult.iEvCode = POP_EVT_FETCHSPHEADER;
      else
         pcx->PopResult.iEvCode = POP_EVT_FETCHASP;
   }
   else           //Fetch all,whole or header
   {
      if(TRUE == pcx->bHeader)
      {
         pcx->bSizeLimitExceed = FALSE;
         pcx->PopResult.iEvCode = POP_EVT_FETCHHEADERS;				//Proc Fetch all mails
      }
      else
         pcx->PopResult.iEvCode = POP_EVT_FETACHALL;				//Proc Fetch all mails
      
      if(0 == pcx->iLatest)
      {
         pcx->uMailAuto_id++;/*start 1*/
      }
      else
      {
         pcx->uMailAuto_id--;
      }
   }
   
   //Change to CMD"RETR",Status "Sending",Register the new CMD callback
   if(TRUE == pcx->bHeader)
      pcx->Pop_CMD = PopTop;								
   else
      pcx->Pop_CMD = PopRetr;
   pcx->Pop_Status = ClientSending_CMD;	
   
   //determine the true mail id
   if(sMailID > 0)			//special mail fetching
   {
      pcx->bSpecial = TRUE;
      if((WE_UINT32)sMailID > pcx->uMaillist_len)
         return EME_POP_Notify(pcx,EME_MAIL_NOTEXSITED,EME_FAILED);
      POP_NULL_ASSERT(pcx->ppUidl_List);
      if(TRUE != pcx->ppUidl_List[sMailID-1].bExisted)
         return EME_POP_Notify(pcx,EME_MAIL_NOTEXSITED,EME_FAILED);
      //if(TRUE != pcx->ppUidl_List[sMailID].bFresh) //If the mail is already processed
      pcx->uMailSpec_id = (WE_UINT32)sMailID;
      iMailId = sMailID;
   }
   else					//mails fetching
   {
      pcx->bSpecial = FALSE;
      //Limit the num of mails fetching
      if(pcx->iMailFetched >= pcx->iMailListLimit)
      {
         //All mails are processed,reset all vars
         EME_POP_ResetJobVar(pcx);
         return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
      }
      
      if(0 == pcx->iLatest)
      {
         for(;pcx->uMailAuto_id <= pcx->uMaillist_len;pcx->uMailAuto_id ++)
         {
            POP_NULL_ASSERT(pcx->ppUidl_List);	
            if(TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bExisted &&
               TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bFresh)
               break;
         }
         if(pcx->uMailAuto_id > pcx->uMaillist_len)		//No new mails failure/Mails were all fetched
         {
            //All mails are processed,reset all vars
            EME_POP_ResetJobVar(pcx);
            return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
         }
      }
      else
      {
         for(;pcx->uMailAuto_id > 0;pcx->uMailAuto_id --)
         {
            POP_NULL_ASSERT(pcx->ppUidl_List);	
            if(TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bExisted &&
               TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bFresh)
               break;
         }
         if(0 == pcx->uMailAuto_id)		//No new mails failure/Mails were all fetched
         {
            //All mails are processed,reset all vars
            EME_POP_ResetJobVar(pcx);
            return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
         }
      }
      iMailId = pcx->uMailAuto_id;
   }
   
   if(NULL != pcx->pMail_Buffer)						//If buffer existed
   {
      EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
      pcx->pMail_Buffer = NULL;
   }
   
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);
   pcx->pcTmpFileName = NULL;
   
   POP_NULL_ASSERT(pcx->ppUidl_List);
   if(pcx->ppUidl_List[iMailId-1].uSize > (WE_UINT32)(pcx->iMailSizeLimit*1024))
   {
      pcx->bHeader = TRUE;
      pcx->Pop_CMD = PopTop;
      if(sMailID > 0)
         return EME_SIZE_LIMIT_EXCEED;
      else
      {
         pcx->bSizeLimitExceed = TRUE;
         pcx->PopResult.iEvCode = POP_EVT_FETCHHEADERS;
      }
   }
   else
   {
      pcx->bSizeLimitExceed = FALSE;
   }
   
   if(sMailID <= 0)
   {//notify user number of new mails and fetched number of new mails
      WE_INT32 iEvtCodeTemp = 0;
      iEvtCodeTemp = pcx->PopResult.iEvCode;
      pcx->PopResult.iEvCode = POP_EVT_NUMBER_NOTIFY;
      pcx->PopResult.detail.stNumNotifyRet.uiCurrNum = pcx->iMailFetched + 1;
      pcx->PopResult.detail.stNumNotifyRet.uiTotalNum = pcx->iTotalNum;
      EME_POP_Notify(pcx,EME_SUCCESS,EME_SUCCESS);
      pcx->PopResult.iEvCode = iEvtCodeTemp;
   }

   pcx->PopResult.bHasNewMail = TRUE;

   if(TRUE == pcx->bHeader)
   {
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_TOP_CMD);	
      return EME_POP_TOP_CMD(pcx);			//First,Send the CMD"TOP"		
   }
   else
   {
      EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_RETR_CMD);	
      return EME_POP_RETR_CMD(pcx);			//First,Send the CMD"RETR"
   }
   return EME_SUCCESS;
}


WE_INT32 EME_POP_Delete(IPOPHANDLER pPopClient,WE_SINT32 sMailID)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);	
	if(sMailID > 0)
		pcx->PopResult.iEvCode = POP_EVT_DELETESP;
	else
		pcx->PopResult.iEvCode = POP_EVT_DELETEALL;						//Proc Delete all mails
	//Set the CMD to DELE,Status to Sending_CMD
	pcx->Pop_CMD = PopDele;								
	pcx->Pop_Status = ClientSending_CMD;	

	//determine the true mail id
	if(sMailID > 0)			//special mail fetching
	{
		pcx->bSpecial = TRUE;
		if((WE_UINT32)sMailID > pcx->uMaillist_len)
			return EME_POP_Notify(pcx,EME_MAIL_NOTEXSITED,EME_FAILED);
		POP_NULL_ASSERT(pcx->ppUidl_List);
		if(TRUE != pcx->ppUidl_List[sMailID-1].bExisted)
			return EME_POP_Notify(pcx,EME_MAIL_NOTEXSITED,EME_FAILED);
		pcx->uMailSpec_id = (WE_UINT32)sMailID;
	}
	else					//mails fetching
	{
		pcx->bSpecial = FALSE;
      POP_LESS_ASSERT(pcx->uMailAuto_id,1);     //Ensure that id begin from 1

      if(0 == pcx->iLatest)
      {
         for(;pcx->uMailAuto_id <= pcx->uMaillist_len;pcx->uMailAuto_id ++)
         {
            POP_NULL_ASSERT(pcx->ppUidl_List);	
            if(TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bExisted)
               break;
         }
         if(pcx->uMailAuto_id > pcx->uMaillist_len)		//No new mails failure/Mails were all deleted
         {
            //All mails are processed,reset all vars
            EME_POP_ResetJobVar(pcx);
            return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
         }
      }
      else
      {
         for(;pcx->uMailAuto_id > 0;pcx->uMailAuto_id --)
         {
            POP_NULL_ASSERT(pcx->ppUidl_List);	
            if(TRUE == pcx->ppUidl_List[pcx->uMailAuto_id-1].bExisted)
               break;
         }
         if(0 == pcx->uMailAuto_id)		//No new mails failure/Mails were all deleted
         {
            //All mails are processed,reset all vars
            EME_POP_ResetJobVar(pcx);
            return EME_POP_Notify(pcx,EME_NO_NEWMAIL,EME_SUCCESS);
         }
      }
	}
	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DELE_CMD);	
	return EME_POP_DELE_CMD(pcx);			//First,Send the CMD"DELE"	
}



WE_INT32 EME_POP_Getting_Response(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point 
	return EME_Socket_Read(pcx->pvSocket,pcx->Response,EME_POP_RESPBUFF_LEN,NULL);
//	return em_commToServer(&pcx->socket, GET_RESPONSE, NULL,
//		pcx->Response, EME_POP_RESPBUFF_LEN, 0);
}
WE_INT32 EME_POP_Sending_CMD(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point  
	if(pcx->pcFilter)
	{
            return EME_Socket_Write(pcx->pvSocket,pcx->pcFilter,EME_UTILS_STRLEN(pcx->pcFilter),TRUE);
	}
	return EME_Socket_Write(pcx->pvSocket,pcx->Command,EME_UTILS_STRLEN(pcx->Command),TRUE);
//	return em_commToServer(&pcx->socket, SENT_COMMAND, 
//		pcx->Command, pcx->Response, 
//		EME_POP_RESPBUFF_LEN, 0);
}

//Dummy Callback
WE_INT32 EME_POP_DummyCallback(IPOPHANDLER  pPopClient)
{
	//Always Success
	return EME_SUCCESS;
}

WE_INT32 EME_POP_GetMailId(IPOPHANDLER  pPopClient,const WE_CHAR * puidl,WE_SINT32 * psMailId)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   WE_UINT32 iCnt = 0;
	POP_NULL_ASSERT(pcx);
   POP_NULL_ASSERT(pcx->ppUidl_List);
   for(iCnt = 0;iCnt < pcx->uMaillist_len;iCnt ++)
   {
      if(0 == EME_UTILS_STRCMP(puidl,pcx->ppUidl_List[iCnt].pData))   
      {
         *psMailId = (WE_SINT32)(iCnt + 1);
         return EME_SUCCESS;
      }
   }
   return EME_MAIL_NOTEXSITED;
}


WE_INT32 EME_POP_FailureNotify(IPOPHANDLER  pPopClient,WE_INT32 iResult)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point
	POP_NULL_ASSERT(pcx->pvGlobe);				//No callback registered
	pcx->PopResult.iResult = EME_FAILED;
	if(NULL != pcx->PopClientNotify)
	pcx->PopClientNotify(pcx->pvGlobe,&(pcx->PopResult));
	return iResult;
}

WE_INT32 EME_POP_SuccessNotify(IPOPHANDLER  pPopClient,WE_INT32 iResult)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point
	POP_NULL_ASSERT(pcx->pvGlobe);				//No callback registered
	pcx->PopResult.iResult = EME_SUCCESS;
	if(NULL != pcx->PopClientNotify)
	pcx->PopClientNotify(pcx->pvGlobe,&(pcx->PopResult));
	return iResult;
}

WE_INT32 EME_POP_Notify(IPOPHANDLER  pPopClient,WE_INT32 iResult,WE_INT32 iRet)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point
	POP_NULL_ASSERT(pcx->pvGlobe);				//No callback registered
	pcx->PopResult.iResult = iResult;
   EME_UTILS_MEMSET(pcx->PopResult.acRespDesc, 0, 512);

   /*copy respone description*/
   if(EME_SUCCESS != iRet && EME_UTILS_STRLEN(pcx->Response) > 0)
      EME_UTILS_MEMCPY(pcx->PopResult.acRespDesc, ((WE_CHAR*)pcx->Response), (EME_UTILS_STRLEN(pcx->Response)));
   EME_UTILS_MEMSET(pcx->Response, 0, EME_POP_RESPBUFF_LEN);
	if(NULL != pcx->PopClientNotify)
	pcx->PopClientNotify(pcx->pvGlobe,&(pcx->PopResult));
	return iRet;
}

WE_INT32 EME_POP_SocketErrorHandling(IPOPHANDLER pPopClient,WE_INT32 iSocketRet)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If no point
	POP_NULL_ASSERT(pcx->pvGlobe);				//No callback registered
	if(EME_SOCKET_SHUTDOWN == iSocketRet)		//Server disconnect the socket because of idle out of time
		pcx->PopResult.iEvCode = POP_EVT_DISCONNECT; //Change proc code to disconnect process
	pcx->PopResult.iResult = iSocketRet;
	if(NULL != pcx->PopClientNotify)
	pcx->PopClientNotify(pcx->pvGlobe,&(pcx->PopResult));
	return EME_SUCCESS;
}

WE_INT32 EME_POP_ResetALL(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	
	POP_NULL_ASSERT(pcx);			      //If Null,return error
	if(NULL != pcx->ppUidl_List)			//Release the ppUidl_list
	{
		WE_UINT32 uLen = 0;
		for(uLen = 0;uLen < pcx->uMaillist_len;uLen ++)
			EME_UTILS_FREEIF(pcx->pMemHdr,pcx->ppUidl_List[uLen].pData);
		EME_UTILS_FREEIF(pcx->pMemHdr,pcx->ppUidl_List);
	}
   if(NULL != pcx->ppUidl_Local)       //Release the local list
   {
      WE_UINT32 uLen = 0;
      for(uLen = 0;uLen < pcx->uLocal_Uidlnum;uLen ++)
         EME_UTILS_FREEIF(pcx->pMemHdr,pcx->ppUidl_Local[uLen].pData);
      EME_UTILS_FREEIF(pcx->pMemHdr,pcx->ppUidl_Local);
   }

	pcx->pAppend_Header = NULL;
   if(NULL != pcx->pvTmpEmailFile)
   {
      (void)EME_FileClose(pcx->pvTmpEmailFile);
      pcx->pvTmpEmailFile = NULL;
   }
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);
   EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
   EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pTimeStamp);   
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcConvertCharset);
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcUseCompress);
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcContentID);
   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcConvertType);
   pcx->uMaillist_len = 0;
   pcx->uLocal_Uidlnum = 0;
   pcx->uMailTotal_Size = 0;
	pcx->uRetred_Size = 1;
	pcx->uMailAuto_id = 0;
	pcx->uMailSpec_id = 0;
	pcx->uMailBuff_Size = 0;
	pcx->Pop_CMD = PopIdle;
	pcx->Pop_Status = Client_Idle;
	pcx->bHeader = FALSE;
	pcx->bSpecial = FALSE;
   pcx->iMailFetched = 0;
   pcx->PopResult.bHasNewMail = FALSE;
	return iResult;	
}



WE_INT32 EME_POP_InitUidFile(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	WE_CHAR		* pTemp = NULL;
	WE_CHAR		* pTempAppend = NULL;
	WE_INT32	  iSize = 0;
	WE_UINT32	  iCnt = 0;
	WE_BOOL		bFileExist = FALSE;
   WE_INT32    iLineCount = 0;
   WE_UINT32    old_Uidlnum = 0; //for test bug114444
	POP_NULL_ASSERT(pcx);	
	bFileExist = EME_FileExists(pcx->pvFileMgr,pcx->Uidl_FileName);
	if(FALSE == bFileExist)				//no file
		return EME_SUCCESS;
	do 
	{
      EME_UTILS_LOG_INFO(("File Name = %s", pcx->Uidl_FileName));
		pcx->pvFile = EME_FileOpen(pcx->pvFileMgr,pcx->Uidl_FileName,EME_OFM_READWRITE);
      EME_UTILS_BREAKNULL(pcx->pvFile, iResult, EME_FAILED, "open file failed");//File pointer error

		iSize = EME_FileLength(pcx->pvFile);
		if(0 == iSize)									//0 size file,no need to worry
		{
			iResult =  EME_SUCCESS;
			EME_UTILS_BREAK(iResult, "file size is 0");
		}
		pTemp = (WE_CHAR *)EME_UTILS_MALLOC(pcx->pMemHdr,sizeof(WE_CHAR)*(iSize+1));
      EME_UTILS_BREAKNULL(pTemp, iResult, EME_NOMEMORY, "no memory for temp buffer");

      EME_UTILS_MEMSET(pTemp, 0x00, sizeof(WE_CHAR)*(iSize+1));
		if(0 == EME_FileRead(pcx->pvFile,pTemp,iSize))	//File read error
		{
			iResult =  EME_FAILED;
			EME_UTILS_BREAKIF(iResult, "Read file failed");
		}

		*(pTemp + iSize) = '\0';
        old_Uidlnum = pcx->uLocal_Uidlnum;
		pcx->uLocal_Uidlnum = EME_UTILS_STRTOUL(pTemp,NULL,10);	//Get total uidl records num,no need to worry
		if(0 == pcx->uLocal_Uidlnum)					//0 uid numbers	
		{
			iResult =  EME_SUCCESS;
			EME_UTILS_BREAK(iResult, "There is no mail remain");
		}	

      if(NULL == pcx->ppUidl_Local)
      {
         pcx->ppUidl_Local = (St_EME_PopUidl *)EME_UTILS_MALLOC(pcx->pMemHdr,sizeof(St_EME_PopUidl)*(pcx->uLocal_Uidlnum + 1));	
         EME_UTILS_BREAKNULL(pcx->ppUidl_Local, iResult, EME_NOMEMORY, "no memory for local uid buffer");
      }
      else
      {
          EME_UTILS_LOG_INFO(("EME_POP_InitUidFile old Uidlnum =%d,Uidlnum=%d",old_Uidlnum,pcx->uLocal_Uidlnum));
      }
      EME_UTILS_MEMSET(pcx->ppUidl_Local, 0, sizeof(St_EME_PopUidl) * (pcx->uLocal_Uidlnum + 1));

      pTempAppend = pTemp + Get_FileBreaker(pcx->uLocal_Uidlnum);

		for(iCnt = 0;iCnt < pcx->uLocal_Uidlnum;iCnt ++)
		{
			if(NULL == pcx->ppUidl_Local[iCnt].pData)
			{
            pcx->ppUidl_Local[iCnt].pData = (WE_CHAR *)EME_UTILS_MALLOC(pcx->pMemHdr,sizeof(WE_CHAR)*(EME_MAX_UIDL_LEN+1));
            EME_UTILS_BREAKNULL(pcx->ppUidl_Local[iCnt].pData, iResult, EME_NOMEMORY, "no memory for uidl data");
            EME_UTILS_MEMSET(pcx->ppUidl_Local[iCnt].pData, 0, sizeof(WE_CHAR)*(EME_MAX_UIDL_LEN+1));

				pcx->ppUidl_Local[iCnt].bExisted = TRUE;
//				pcx->ppUidl_Local[iCnt].bFresh = TRUE;

				iResult = EME_POP_GetLine(pcx,pcx->ppUidl_Local[iCnt].pData,pTempAppend,&iLineCount);
            EME_UTILS_BREAKIF(iResult, "get line failed");

            if(iLineCount >= EME_POP_RESPBUFF_LEN)            //If line count out of uild value length
            {
               EME_UTILS_LOG_INFO(("Line count = %d", iLineCount));
               iResult = EME_FAILED;
               EME_UTILS_BREAKIF(iResult, "Line count lager than 512");
            }

				pTempAppend += iLineCount;
				if((pTempAppend-pTemp) >= iSize)		//end of file
            {
               if(iCnt + 1 < pcx->uLocal_Uidlnum)
                  iResult = EME_FAILED;//the iCnt is less than the number indicated in the file header
               break; 
            }
			}
		}		

      if((pTempAppend-pTemp) < iSize)
      {
         iResult = EME_FAILED;
         EME_UTILS_BREAK(iResult, "search line failed");
      }

	} while(0);

	EME_UTILS_FREEIF(pcx->pMemHdr,pTemp);
	pTempAppend = NULL;

   if(NULL != pcx->pvFile)
	   EME_FileClose(pcx->pvFile);

   if (iResult != EME_SUCCESS)
   {
		for(iCnt = 0;iCnt < pcx->uLocal_Uidlnum;iCnt ++)
		{
         EME_UTILS_FREEIF(pcx->pMemHdr, pcx->ppUidl_Local[iCnt].pData);
      }
      EME_UTILS_FREEIF(pcx->pMemHdr, pcx->ppUidl_Local);
      pcx->uLocal_Uidlnum = 0;
   }

	return iResult;
}


WE_INT32 EME_POP_GetLine(IPOPHANDLER  pPopClient,WE_CHAR * pDest,const WE_CHAR * pSrc,WE_INT32 * piLineCount)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_CHAR * pResult = NULL;
	WE_INT32 iLineCount = 0;
	POP_NULL_ASSERT(pcx);
   POP_NULL_ASSERT(pDest);
   POP_NULL_ASSERT(pSrc);
   POP_NULL_ASSERT(piLineCount);

	pResult = EME_UTILS_STRSTR(pSrc,"\r\n");		//If find line breaker
	if(NULL != pResult)
	{
		iLineCount = pResult - pSrc + 2;
		if(iLineCount >= EME_POP_RESPBUFF_LEN)    //If line count out of uild value length
			return EME_FAILED;
		EME_UTILS_STRNCPY(pDest,pSrc,iLineCount);
		*(pDest + iLineCount) = '\0';
      *piLineCount = iLineCount;
		return EME_SUCCESS;
	}
	return EME_FAILED;
}

//Check all mails to get the new mails' id
WE_INT32 EME_POP_CheckNewMails(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_UINT32 uCnt = 0;
	WE_UINT32 uLocalCnt = 0;
   POP_NULL_ASSERT(pcx);
	POP_NULL_ASSERT(pcx->ppUidl_List);

   pcx->iTotalNum = pcx->uMaillist_len;
	for(uCnt = 0;uCnt < pcx->uMaillist_len;uCnt ++)
	{
		if(NULL == pcx->ppUidl_Local)						//If there is no record file,set every file to download stat
         pcx->ppUidl_List[uCnt].bFresh = TRUE;
		else
		{
			for(uLocalCnt = 0;uLocalCnt < pcx->uLocal_Uidlnum;uLocalCnt ++)
			{
				if(0 == EME_UTILS_STRCMP(pcx->ppUidl_List[uCnt].pData,pcx->ppUidl_Local[uLocalCnt].pData))
				{
					pcx->ppUidl_List[uCnt].bFresh = FALSE;	//Find the same uidl in local,so do not need to download it
					pcx->iTotalNum--;
               break;
				}
			}
		}
	}
	return EME_SUCCESS;
}

WE_BOOL EME_POP_CheckMail(IPOPHANDLER  pPopClient,WE_UINT32 id)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_UINT32 uCnt = 0;
	if(NULL == pcx)
		return FALSE;
	if(NULL == pcx->ppUidl_List)	
		return FALSE;
	if(id >= pcx->uMaillist_len)
		return FALSE;
	for(uCnt = 0;uCnt < pcx->uLocal_Uidlnum;uCnt ++)
	{
		if(0 == EME_UTILS_STRCMP(pcx->ppUidl_List[id].pData,pcx->ppUidl_Local[uCnt].pData))
			return TRUE;
	}
	return FALSE;
}

WE_INT32 EME_POP_UpdateUidlLocal(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_UINT32 uCnt = 0;
	WE_UINT32 uUidCnt = 0;
	WE_CHAR	cTemp[10];
	WE_BOOL		bFileExist = FALSE;
	EME_UTILS_MEMSET(cTemp,0,sizeof(WE_CHAR)*10);
	POP_NULL_ASSERT(pcx);
	
	bFileExist = EME_FileExists(pcx->pvFileMgr,pcx->Uidl_FileName);
	if(NULL == pcx->ppUidl_List)					//No mails at all in server
		return EME_SUCCESS;
	for(uCnt = 0;uCnt < pcx->uMaillist_len;uCnt ++)
	{
      if(FALSE == pcx->ppUidl_List[uCnt].bFresh)
			uUidCnt ++;
	}
	if(0 == uUidCnt)								//No mails download
		return EME_SUCCESS;
	if(TRUE == bFileExist)							//If file existed,delete it
	{
		if(EME_FAILED == EME_DeleteFile(pcx->pvFileMgr,pcx->Uidl_FileName))
			return EME_FAILED;
	}
	pcx->pvFile = EME_FileOpen(pcx->pvFileMgr,pcx->Uidl_FileName,EME_OFM_CREATE);
	if(NULL == pcx->pvFile)
		return EME_FAILED;
	EME_UTILS_SPRINTF(cTemp,"%d\r\n",uUidCnt);
	EME_FileWrite(pcx->pvFile,cTemp,EME_UTILS_STRLEN(cTemp));
   
	for(uCnt = 0;uCnt < pcx->uMaillist_len;uCnt ++)    //Let the bFresh flag uidl write into the uidl file
	{
		if(FALSE == pcx->ppUidl_List[uCnt].bFresh && NULL != pcx->ppUidl_List[uCnt].pData)
			EME_FileWrite(pcx->pvFile,pcx->ppUidl_List[uCnt].pData,EME_UTILS_STRLEN(pcx->ppUidl_List[uCnt].pData));
	}	
	EME_FileClose(pcx->pvFile);
	return EME_SUCCESS;
}

static WE_INT32  Get_FileBreaker(WE_INT32 iFileNum)
{
   WE_CHAR cTemp[MAX_INT_STR_LEN];

   EME_UTILS_MEMSET(cTemp,0,MAX_INT_STR_LEN*sizeof(WE_CHAR));
   EME_UTILS_SPRINTF(cTemp,"%d",iFileNum);

   return EME_UTILS_STRLEN(cTemp)+2;
}


WE_INT32 EME_POP_MD5Change(WE_CHAR * pDest,const WE_CHAR * pSrc)
{

#define MD5_LENGTH 16

   WE_INT32 iResult =  EME_SUCCESS;
   WE_INT32 iCnt = 0;
   WE_CHAR TempIn = 0;
   WE_CHAR TempOut = 0;
   WE_CHAR Dic[17] = {"0123456789abcdef"};
   WE_INT32 iSrcLen = 0;
   if(NULL == pDest || pSrc == NULL)
      return EME_FAILED;
   iSrcLen = EME_UTILS_STRLEN(pSrc);
   if(iSrcLen != MD5_LENGTH)
      return EME_FAILED;
   for(iCnt = 0;iCnt < MD5_LENGTH;iCnt ++)
   {
      TempIn = *(pSrc+iCnt);
      TempOut = TempIn >> 4;
      TempOut = TempOut & 0x0f;
      *(pDest+iCnt*2) = Dic[TempOut];
      TempOut = TempIn & 0x0f;
      *(pDest+iCnt*2+1) = Dic[TempOut];
   }
   return iResult;
}
/*
WE_INT32 EME_POP_MD5Hash(IPOPHANDLER  pPopClient,WE_CHAR * pcData)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   IHash* pIHash = NULL;
   WE_INT32 iResult =  EME_SUCCESS;
   WE_INT32 size = MD5_LENGTH;
   WE_CHAR Temp[MD5_LENGTH+1];
   POP_NULL_ASSERT(pcx);
   EME_UTILS_MEMSET(Temp,0,MD5_LENGTH+1);
   do 
   {
      iResult = ISHELL_CreateInstance(pcx->pvIShell, AEECLSID_MD5, (void **)&pIHash);
      EME_UTILS_BREAKIF(iResult,"EME_POP_MD5Hash,AEECLSID_MD5 create failed");
      EME_UTILS_BREAKNULL(pIHash,iResult,EME_NOMEMORY,"pIHash,NULL");
      IHASH_Update(pIHash, (const byte*)pcx->pTimeStamp, EME_UTILS_STRLEN((const char*)pcx->pTimeStamp));
      iResult = IHASH_GetResult(pIHash, (byte*)Temp, (WE_INT*)&size);
      EME_UTILS_BREAKIF(iResult,"EME_POP_MD5Hash,IHASH_GetResult failed");
      iResult = EME_POP_MD5Change(pcData,Temp);
      EME_UTILS_BREAKIF(iResult,"EME_POP_MD5Hash,EME_POP_MD5Change failed");
   } while(0);
   EME_UTILS_RELEASEIF(pIHash);
   return iResult;
}

*/

/*
WE_INT32  EME_POP_SetAuth(IPOPHANDLER  pPopClient,WE_BOOL bAuth)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   POP_NULL_ASSERT(pcx);			//If Null,return error
   pcx->bAuth = bAuth;
   return EME_SUCCESS;
}

WE_INT32  EME_POP_GetAuth(IPOPHANDLER  pPopClient,WE_BOOL * pbAuth)
{  
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   POP_NULL_ASSERT(pcx);			//If Null,return error
   *pbAuth = pcx->bAuth;
   return EME_SUCCESS;
}
*/
