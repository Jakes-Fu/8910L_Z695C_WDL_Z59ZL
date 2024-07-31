/*=====================================================================================
    FILE: eme_pop_api.c
    MODULE NAME : POP


    GENERAL DESCRIPTION:
    Implement of Pop3 Stack APIs

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================  
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/05/2007        Hubin                            creat this file
    04/29/2007        Hubin                            add apop cmd
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#include "eme_file_api.h"
#include "eme_pop_api.h"
#include "eme_pop_prv.h"


WE_INT32 EME_POP_InitHandle(IPOPHANDLER * ppPopClient,
                            WE_VOID * pstMemMgr,
                            WE_VOID * pvIShell,
                            WE_VOID * pvAsyncMgr,
                            WE_VOID * pvFileMgr, 
                            WE_VOID* pvNetMgr)
{
	ST_EME_POPCLIENT * pcx = NULL;
  
	if(NULL == *ppPopClient)
   {
      *ppPopClient = (ST_EME_POPCLIENT *)EME_UTILS_MALLOC(pstMemMgr,sizeof(ST_EME_POPCLIENT));
      if(*ppPopClient == NULL)
         return EME_FAILED;
      else
         EME_UTILS_MEMSET(*ppPopClient, 0, sizeof(ST_EME_POPCLIENT));
   }
	else
		return EME_FAILED;		//inited twice or bad pointer received
	pcx = (ST_EME_POPCLIENT *)(*ppPopClient);
   if(NULL == pcx)
      return EME_NOMEMORY;
	EME_UTILS_MEMSET(pcx,0,sizeof(ST_EME_POPCLIENT));
	pcx->pMemHdr = pstMemMgr;
	pcx->pvIShell = pvIShell;
   pcx->pvNetMgr = pvNetMgr;
	pcx->pvFileMgr = pvFileMgr;
   pcx->pvAsyncMgr = pvAsyncMgr;
	pcx->PopResult.iEvCode = POP_EVT_BEGIN;
	pcx->PopResult.iResult = EME_FAILED;
	pcx->pvGlobe = NULL;								//No Upper Callback registered
	pcx->ppUidl_List	= NULL;						//Init Uidl list to none
   pcx->ppUidl_Local = NULL;
   pcx->uMailTotal_Size = 0;
	pcx->uMaillist_len = 0;		
	pcx->uMailAuto_id = 0;
	pcx->uMailSpec_id = 1;
	pcx->pMail_Buffer = NULL;
	pcx->pAppend_Header = NULL;
   pcx->pTimeStamp = NULL;
	pcx->Pop_CMD = PopNone;					//Init the cmd status to none
	pcx->Pop_Status = Client_Idle;			//Nothing to be done
	pcx->Accnt.uPort = 110;
	pcx->bHeader = FALSE;
	pcx->bSpecial = FALSE;
	pcx->uMailBuff_Size = 0;
   pcx->iMailListLimit = 0;
   pcx->iMailFetched = 0;
   pcx->iMailSizeLimit = 0;
   pcx->bAuth = FALSE;
	return EME_SUCCESS;
}


WE_INT32 EME_POP_InitConnection(IPOPHANDLER  pPopClient, St_POP_InitConn* pAccnt)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	POP_NULL_ASSERT(pcx);			//If Null,return error
 //  EME_NULL_ASSERT(pAccnt);			//If Null,return error
   //Initing the accnt infomation
   EME_UTILS_LOG_INFO(("EME_POP_InitConnection : START!!"));
   EME_UTILS_LOG_INFO(("EME_POP_InitConnection : START!!"));
   
   {
      WE_UINT32 uStrlen = 0;
      POP_NULL_ASSERT(pAccnt->pcPassWord);
      POP_NULL_ASSERT(pAccnt->pcUserName);
      POP_NULL_ASSERT(pAccnt->pcServerAdd);
      POP_NULL_ASSERT(pAccnt->pcUidlPath);
      uStrlen = EME_UTILS_STRLEN(pAccnt->pcPassWord);
      if(uStrlen >= EME_POP_PASSWORD_LEN || 0 == uStrlen)
         return EME_LOGIN_ERROR;
      uStrlen = EME_UTILS_STRLEN(pAccnt->pcUserName);
      if(uStrlen >= EME_POP_USERNAME_LEN || 0 == uStrlen)
         return EME_LOGIN_ERROR;
      uStrlen = EME_UTILS_STRLEN(pAccnt->pcServerAdd);
      if(uStrlen >= EME_GENERIC_TEXT_LEN || 0 == uStrlen)
         return EME_FAILED;
      uStrlen = EME_UTILS_STRLEN(pAccnt->pcUidlPath);
      if(uStrlen >= EME_POP_UIDLFILE_LEN || 0 == uStrlen)
         return EME_FAILED;
      EME_UTILS_SPRINTF(pcx->Password,"%s",pAccnt->pcPassWord);
      EME_UTILS_SPRINTF(pcx->UserName,"%s",pAccnt->pcUserName);
      EME_UTILS_SPRINTF(pcx->Accnt.MailServer,"%s",pAccnt->pcServerAdd);
      EME_UTILS_SPRINTF(pcx->Uidl_FileName,"%s",pAccnt->pcUidlPath);
      pcx->Accnt.uPort = pAccnt->usPort;
      pcx->iMailListLimit = pAccnt->iNumLimit;
      pcx->iLatest = pAccnt->iLatest;

//      pcx->iMailListLimit = 50;
      pcx->iMailSizeLimit = pAccnt->iSizeLimit;
      pcx->bAuth = pAccnt->bAuth;
      
   }
	pcx->PopResult.iEvCode = POP_EVT_INIT;	//Proc InitConnection
	pcx->Pop_CMD = PopNone;			//No CMD
	pcx->pvSocket =  EME_Socket_Create(pcx->pvIShell, pcx->pvAsyncMgr, pcx->pvNetMgr, pcx,pcx->pMemHdr, pAccnt->iIsNeedSSL);
	if(NULL == pcx->pvSocket)				//Failure Notify Process
		return EME_POP_Notify(pcx,EME_SOCKET_NET_ERR,EME_FAILED);
	EME_UTILS_LOG_INFO(("EME_POP_InitConnection :CALL EME_Socket_Create success!"));
	EME_UTILS_LOG_INFO(("EME_POP_InitConnection :CALL EME_Socket_Create success!"));
	
	iResult = EME_Socket_Connection(pcx->pvSocket,pcx->Accnt.MailServer,pcx->Accnt.uPort);
	EME_UTILS_LOG_INFO(("EME_POP_InitConnection :CALL EME_Socket_Connection iRet = %d!!", iResult));
	EME_UTILS_LOG_INFO(("EME_POP_InitConnection :CALL EME_Socket_Connection iRet = %d!!", iResult));
	EME_UTILS_LOG_INFO(("EME_POP_InitConnection :CALL EME_Socket_Connection iRet = %d!!", iResult));
	 
	if(EME_SUCCESS != iResult)			//Failure Notify Process
		return EME_POP_Notify(pcx,iResult,EME_FAILED);
	EME_Socket_RegCbConnectionOk(pcx->pvSocket,(WE_VOID*)EME_POP_DoRecv);//Register DoRecv() to NetworkReady signal
	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_DetectWelcome);
	EME_Socket_RegCbErrHandler(pcx->pvSocket,(WE_VOID*)EME_POP_SocketErrorHandling);
	return iResult;
}

WE_INT32 EME_POP_Login(IPOPHANDLER pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);						//If Null,return error
	pcx->PopResult.iEvCode = POP_EVT_LOGIN;						//Proc Login
	//Change to CMD"USER",Status "Sending",Register the new CMD callback
	pcx->Pop_CMD = PopUser;								
	pcx->Pop_Status = ClientSending_CMD;					
	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_USER_CMD);	
	return EME_POP_USER_CMD(pcx);			//First,Send the CMD"USER"
}



WE_INT32 EME_POP_GetMailInfo(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);									//If Null,return error
	pcx->PopResult.iEvCode = POP_EVT_GETINFO;						//Proc Getinfo
	//Change to CMD"UIDL",Status "Sending",Register the new CMD callback
	pcx->Pop_CMD = PopStat;								
	pcx->Pop_Status = ClientSending_CMD;
	////Get client local records
	EME_POP_InitUidFile(pcx);
   //if(EME_SUCCESS != iResult)          //Init Uid local file failed
      //return EME_POP_FailureNotify(pcx,iResult);
	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_STAT_CMD);	
	return EME_POP_STAT_CMD(pcx);			//First,Send the CMD"STAT"
}




WE_INT32 EME_POP_FetchAllMails(IPOPHANDLER  pPopClient)
{
	POP_NULL_ASSERT(pPopClient);			//If Null,return error	
	return EME_POP_Retrive(pPopClient,FALSE,-1);
}

WE_INT32 EME_POP_FetchAllMailHeaders(IPOPHANDLER  pPopClient)
{
	POP_NULL_ASSERT(pPopClient);			//If Null,return error	
	return EME_POP_Retrive(pPopClient,TRUE,-1);
}

WE_INT32 EME_POP_FetchSpecifiedMail(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl)
{
	WE_SINT32 sMailId = 0;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pPopClient);			//If Null,return error	
   POP_NULL_ASSERT(pcUidl);
   iResult = EME_POP_GetMailId(pPopClient,pcUidl,&sMailId);
   if(EME_SUCCESS != iResult)
      return iResult;
   POP_LESS_ASSERT(sMailId,1);
	return EME_POP_Retrive(pPopClient,FALSE,sMailId);
}
WE_INT32 EME_POP_FetchSpecifiedMailHeader(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl)
{
	WE_SINT32 sMailId = 0;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pPopClient);			//If Null,return error	
   
   iResult = EME_POP_GetMailId(pPopClient,pcUidl,&sMailId);
   if(EME_SUCCESS != iResult)
      return iResult;
   POP_LESS_ASSERT(sMailId,1);
	return EME_POP_Retrive(pPopClient,TRUE,sMailId);
}

WE_INT32 EME_POP_DeleteAllMails(IPOPHANDLER  pPopClient)
{
	POP_NULL_ASSERT(pPopClient);			//If Null,return error
	return EME_POP_Delete(pPopClient,-1);
}

WE_INT32 EME_POP_DeleteSepcifiedMail(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl)
{
	WE_SINT32 sMailId = 0;
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pPopClient);			//If Null,return error	
   
   iResult = EME_POP_GetMailId(pPopClient,pcUidl,&sMailId);
   if(EME_SUCCESS != iResult)
      return iResult;
   POP_LESS_ASSERT(sMailId,1);
	return EME_POP_Delete(pPopClient,sMailId);
}

WE_INT32 EME_POP_Quit(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);									//If Null,return error
	pcx->PopResult.iEvCode = POP_EVT_QUIT;						//Proc Quit
	//Change to CMD"Quit",Status "Sending",Register the new CMD callback
	pcx->Pop_CMD = PopQuit;								
	pcx->Pop_Status = ClientSending_CMD;					
	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_QUIT_CMD);	
	return EME_POP_QUIT_CMD(pcx);			//First,Send the CMD"STAT"
}

WE_INT32 EME_POP_CancelOperation(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	POP_NULL_ASSERT(pcx);			//If Null,return error
   pcx->PopResult.iEvCode = POP_EVT_CANCEL;						//Proc Cancel

    EME_UTILS_LOG_INFO(("EME_POP_CancelOperation   START!!"));
    EME_UTILS_LOG_INFO(("EME_POP_CancelOperation   START!!"));
   do 
   {
      //Close socket
      iResult = EME_Socket_Close(pcx->pvSocket);
      EME_UTILS_LOG_INFO(("EME_POP_CancelOperation   EME_Socket_Close return iret = %d!!", iResult));
       EME_UTILS_LOG_INFO(("EME_POP_CancelOperation   EME_Socket_Close return iret = %d!!", iResult));
	   
      POP_FALSE_BREAK(iResult);
      pcx->pvSocket = NULL;         //After closed,the point must be set to null
      //Before Quit,Update the local records
      iResult = EME_POP_UpdateUidlLocal(pcx);
      POP_FALSE_BREAK(iResult);
      //Reset ALL
      iResult = EME_POP_ResetALL(pcx);  
      POP_FALSE_BREAK(iResult);
//      return EME_POP_SuccessNotify(pcx,iResult);
      return iResult;
   } while(0);
   //If above process find some error
   return iResult;
//   return EME_POP_Notify(pcx,iResult,EME_FAILED);
	
}

WE_INT32 EME_POP_DeleteHandle(IPOPHANDLER *ppPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)(*ppPopClient);
   WE_INT32 iResult =  EME_SUCCESS;
   POP_NULL_ASSERT(pcx);
   if(NULL != pcx->pvSocket)
   {
      //Close socket
      iResult = EME_Socket_Close(pcx->pvSocket);
      POP_FALSE_ASSERT(iResult);
      pcx->pvSocket = NULL;         //After closed,the point must be set to null
   }
   
   iResult = EME_POP_UpdateUidlLocal(pcx);      //If runs the normal cancel or quit process,it will do nothing
                                                //If abnormal terminate(server error,for example),it will save the work
   POP_FALSE_ASSERT(iResult);
	iResult = EME_POP_ResetALL(pcx);
   POP_FALSE_ASSERT(iResult);
	EME_UTILS_FREEIF(pcx->pMemHdr,pcx);
   *ppPopClient = NULL;
	return EME_SUCCESS;
		
}

WE_INT32	 EME_POP_RegCallBack(IPOPHANDLER  pPopClient,POPREGNOTIFY pcb,void * pClientData)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx)
	POP_NULL_ASSERT(pcb);
	POP_NULL_ASSERT(pClientData);
	pcx->PopClientNotify = (POPREGNOTIFY)pcb;
	pcx->pvGlobe = pClientData;
	return EME_SUCCESS;
}

WE_INT32 EME_POP_ResetJobVar(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult =  EME_SUCCESS;
	POP_NULL_ASSERT(pcx);			//If Null,return error
	pcx->pAppend_Header = NULL;
	if(NULL != pcx->pMail_Buffer)
	{
		EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
		pcx->pMail_Buffer = NULL;
	}

   EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);
   pcx->pcTmpFileName = NULL;

	pcx->uRetred_Size = 0;
	pcx->uMailAuto_id = 0;
	pcx->uMailSpec_id = 0;
	pcx->uMailBuff_Size = 0;
	pcx->Pop_CMD = PopIdle;
	pcx->Pop_Status = Client_Idle;
	pcx->bHeader = FALSE;
	pcx->bSpecial = FALSE;
	return iResult;
}


WE_INT32 EME_POP_TAG_MAIL(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl,WE_BOOL bFlag)
{
	WE_SINT32 sMailId = 0;
   WE_INT32 iResult =  EME_SUCCESS;
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   POP_NULL_ASSERT(pcx);			//If Null,return error	 
   iResult = EME_POP_GetMailId(pPopClient,pcUidl,&sMailId);
   if(EME_SUCCESS != iResult)
      return iResult;
   POP_LESS_ASSERT(sMailId,1);   
   POP_NULL_ASSERT(pcx->ppUidl_List)
      pcx->ppUidl_List[sMailId-1].bFresh = !bFlag;
   return EME_SUCCESS;
}

WE_INT32 EME_POP_GetUidl(IPOPHANDLER  pPopClient,WE_INT32 iNum,WE_CHAR * pcUid,WE_INT32 iStrlen)
{
   WE_INT32 iResult =  EME_SUCCESS;
   WE_INT32 iLen = 0;
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   POP_NULL_ASSERT(pcx);			//If Null,return error	 
   POP_NULL_ASSERT(pcx->ppUidl_List);
   POP_NULL_ASSERT(pcUid);
   POP_LESS_ASSERT(iNum,1);
   POP_MORE_ASSERT(iNum,(WE_INT32)pcx->uMaillist_len+1);
   iLen = EME_UTILS_STRLEN(pcx->ppUidl_List->pData);
   POP_LESS_ASSERT(iStrlen,iLen);
   EME_UTILS_SPRINTF(pcUid,"%s",pcx->ppUidl_List->pData);
   return iResult;

}

WE_INT32  EME_POP_GetSizeLimit(IPOPHANDLER  pPopClient,WE_BOOL * pbSizeLimit)
{
   ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
   POP_NULL_ASSERT(pcx);			//If Null,return error
   *pbSizeLimit = pcx->bSizeLimitExceed;
   return EME_SUCCESS;
}


WE_INT32 
EME_EPOP_Prof(IPOPHANDLER pPopClient, const WE_CHAR *pcConvertCharset, const WE_CHAR *pcUseCompress)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	POP_NULL_ASSERT(pcx);

	pcx->PopResult.iEvCode = EPOP_EVT_FETCHPROF;

	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcConvertCharset);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcUseCompress);

	if (NULL != pcConvertCharset){

		pcx->pcConvertCharset = EME_UTILS_STRDUP(pcx->pMemHdr, (WE_CHAR *)pcConvertCharset);

		if (NULL != pcUseCompress){

			pcx->pcUseCompress = EME_UTILS_STRDUP(pcx->pMemHdr, (WE_CHAR *)pcUseCompress);
		}

		pcx->Pop_CMD = EPopProf;								
		pcx->Pop_Status = ClientSending_CMD;

		EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_EPOP_PROF_CMD);
		
		return EME_EPOP_PROF_CMD(pcx);	
	}

	return EME_FAILED;
}

WE_INT32 
EME_EPOP_FetchAllMails(IPOPHANDLER pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32    iMailId = 0;
	WE_BOOL bHeader = FALSE;

	POP_NULL_ASSERT(pcx);

	pcx->bSizeLimitExceed = FALSE;
	pcx->PopResult.iEvCode = EPOP_EVT_FETCHALL;
	pcx->Pop_CMD = EPopEret;
	pcx->Pop_Status = ClientSending_CMD;
	pcx->bSpecial = FALSE;

	if (pcx->iMailFetched >= pcx->iMailListLimit){

		EME_POP_ResetJobVar(pcx);

		return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
	}

   if(0 == pcx->iLatest)
   {
      for (; pcx->uMailAuto_id <= pcx->uMaillist_len; pcx->uMailAuto_id++){
         
         POP_NULL_ASSERT(pcx->ppUidl_List);
         
         if (pcx->ppUidl_List[pcx->uMailAuto_id - 1].bExisted &&
            pcx->ppUidl_List[pcx->uMailAuto_id - 1].bFresh){
            
            break;
         }
      }

      if (pcx->uMailAuto_id > pcx->uMaillist_len){
         
         EME_POP_ResetJobVar(pcx);
         
         return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
      }
   }
   else
   {
      for (; pcx->uMailAuto_id > 0; pcx->uMailAuto_id--){
         
         POP_NULL_ASSERT(pcx->ppUidl_List);
         
         if (pcx->ppUidl_List[pcx->uMailAuto_id - 1].bExisted &&
            pcx->ppUidl_List[pcx->uMailAuto_id - 1].bFresh){
            
            break;
         }
      }

      if (0 == pcx->uMailAuto_id){
         
         EME_POP_ResetJobVar(pcx);
         
         return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
      }
   }


	iMailId = pcx->uMailAuto_id;

	
	EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);
	
	POP_NULL_ASSERT(pcx->ppUidl_List);

#if 0
	if(pcx->ppUidl_List[iMailId - 1].uSize > (WE_UINT32)(pcx->iMailSizeLimit * 1024)){

		bHeader = TRUE;
		pcx->Pop_CMD = PopTop;
		pcx->bSizeLimitExceed = TRUE;
		pcx->PopResult.iEvCode = POP_EVT_FETCHHEADERS;
	}
	else{

		pcx->bSizeLimitExceed = FALSE;
	}
#endif	
	{
		WE_INT32 iEvtCodeTemp = 0;

		iEvtCodeTemp = pcx->PopResult.iEvCode;
		pcx->PopResult.iEvCode = POP_EVT_NUMBER_NOTIFY;
		pcx->PopResult.detail.stNumNotifyRet.uiCurrNum = pcx->iMailFetched + 1;
		pcx->PopResult.detail.stNumNotifyRet.uiTotalNum = pcx->iTotalNum;

		EME_POP_Notify(pcx,EME_SUCCESS,EME_SUCCESS);

		pcx->PopResult.iEvCode = iEvtCodeTemp;
	}
	
   pcx->PopResult.bHasNewMail = TRUE;

	if(TRUE == bHeader){

		EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_POP_TOP_CMD);

		return EME_POP_TOP_CMD(pcx);				
	}
	else{

		EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_EPOP_ERET_CMD);	

		return EME_EPOP_ERET_CMD(pcx);
	}

	return EME_SUCCESS;
}

WE_INT32 
EME_EPOP_FetchAllMailHeaders(IPOPHANDLER pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32    iMailId = 0;

	POP_NULL_ASSERT(pcx);

	pcx->bSizeLimitExceed = FALSE;
	pcx->PopResult.iEvCode = EPOP_EVT_FETCHHEADERS;
	pcx->Pop_CMD = PopTop;
	pcx->Pop_Status = ClientSending_CMD;

	if (pcx->iMailFetched >= pcx->iMailListLimit){

		EME_POP_ResetJobVar(pcx);

		return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
	}

   if(0 == pcx->iLatest)
   {
      for (; pcx->uMailAuto_id <= pcx->uMaillist_len; pcx->uMailAuto_id++){
         
         POP_NULL_ASSERT(pcx->ppUidl_List);
         
         if (pcx->ppUidl_List[pcx->uMailAuto_id - 1].bExisted &&
            pcx->ppUidl_List[pcx->uMailAuto_id - 1].bFresh){
            
            break;
         }
      }

      if (pcx->uMailAuto_id > pcx->uMaillist_len){
         
         EME_POP_ResetJobVar(pcx);
         
         return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
      }
   }
   else
   {
      for (; pcx->uMailAuto_id > 0; pcx->uMailAuto_id--){
         
         POP_NULL_ASSERT(pcx->ppUidl_List);
         
         if (pcx->ppUidl_List[pcx->uMailAuto_id - 1].bExisted &&
            pcx->ppUidl_List[pcx->uMailAuto_id - 1].bFresh){
            
            break;
         }
      }

      if (0 == pcx->uMailAuto_id){
         
         EME_POP_ResetJobVar(pcx);
         
         return EME_POP_Notify(pcx, EME_NO_NEWMAIL, EME_SUCCESS);
      }
   }

	iMailId = pcx->uMailAuto_id;

	EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);

	POP_NULL_ASSERT(pcx->ppUidl_List);
	
	{
		WE_INT32 iEvtCodeTemp = 0;
		iEvtCodeTemp = pcx->PopResult.iEvCode;
		pcx->PopResult.iEvCode = POP_EVT_NUMBER_NOTIFY;
		pcx->PopResult.detail.stNumNotifyRet.uiCurrNum = pcx->iMailFetched + 1;
		pcx->PopResult.detail.stNumNotifyRet.uiTotalNum = pcx->iTotalNum;
		EME_POP_Notify(pcx,EME_SUCCESS,EME_SUCCESS);
		pcx->PopResult.iEvCode = iEvtCodeTemp;
	}
	
   pcx->PopResult.bHasNewMail = TRUE;

	EME_Socket_RegCbReadable(pcx->pvSocket,(WE_VOID*)EME_POP_TOP_CMD);	
	return EME_POP_TOP_CMD(pcx);
}

WE_INT32 
EME_EPOP_FetchSepcialMailPart(IPOPHANDLER pPopClient, 
							  const WE_CHAR *pcUidl, 
							  const WE_CHAR *pcContentID,
							  const WE_CHAR *pcConvertType,
							  WE_INT32   iAttachSize)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult = EME_SUCCESS;
	WE_UINT32 uiMailID = 0;

	POP_NULL_ASSERT(pcx);
	POP_NULL_ASSERT(pcUidl);

	pcx->bSizeLimitExceed = FALSE;
	pcx->PopResult.iEvCode = EPOP_EVT_FETCHSPPART;
	pcx->Pop_CMD = EPopEret;
	pcx->Pop_Status = ClientSending_CMD;
	pcx->bSpecial = TRUE;

	iResult = EME_POP_GetMailId(pPopClient, pcUidl, (WE_SINT32*)&uiMailID);

	if (uiMailID > pcx->uMaillist_len){

		return EME_POP_Notify(pcx, EME_MAIL_NOTEXSITED, EME_FAILED);
	}

	POP_NULL_ASSERT(pcx->ppUidl_List);

	if (FALSE == pcx->ppUidl_List[uiMailID - 1].bExisted){

		return EME_POP_Notify(pcx, EME_MAIL_NOTEXSITED, EME_FAILED);
	}

	if (iAttachSize > (WE_UINT32)(pcx->iMailSizeLimit * 1024)){

		pcx->bSizeLimitExceed = TRUE;
		return EME_SIZE_LIMIT_EXCEED;
	}

	pcx->uMailSpec_id = uiMailID;
	
	EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcContentID);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcConvertType);

	if (NULL != pcContentID){

		pcx->pcContentID = EME_UTILS_STRDUP(pcx->pMemHdr, (WE_CHAR *)pcContentID);
	}

	if (NULL != pcConvertType){

		pcx->pcConvertType = EME_UTILS_STRDUP(pcx->pMemHdr, (WE_CHAR *)pcConvertType);
	}

	EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_EPOP_ERET_CMD);	
	return EME_EPOP_ERET_CMD(pcx);
}

WE_INT32 
EME_EPOP_FetchSepcialMail(IPOPHANDLER pPopClient, const WE_CHAR *pcUidl)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult = EME_SUCCESS;
	WE_UINT32 uiMailID = 0;

	POP_NULL_ASSERT(pcx);
	POP_NULL_ASSERT(pcUidl);

	pcx->bSizeLimitExceed = FALSE;
	pcx->PopResult.iEvCode = EPOP_EVT_FETCHSPMAIL;
	pcx->Pop_CMD = EPopEret;
	pcx->Pop_Status = ClientSending_CMD;
	pcx->bSpecial = TRUE;

	iResult = EME_POP_GetMailId(pPopClient, pcUidl, (WE_INT32*)&uiMailID);

	if (uiMailID > pcx->uMaillist_len || uiMailID == 0){

		return EME_POP_Notify(pcx, EME_MAIL_NOTEXSITED, EME_FAILED);
	}

	POP_NULL_ASSERT(pcx->ppUidl_List);

	if (FALSE == pcx->ppUidl_List[uiMailID - 1].bExisted){

		return EME_POP_Notify(pcx, EME_MAIL_NOTEXSITED, EME_FAILED);
	}

	if (pcx->ppUidl_List[uiMailID - 1].uSize > (WE_UINT32)(pcx->iMailSizeLimit * 1024)){

		pcx->bSizeLimitExceed = TRUE;
		return EME_SIZE_LIMIT_EXCEED;
	}

	pcx->uMailSpec_id = uiMailID;
	
	EME_UTILS_FREEIF(pcx->pMemHdr,pcx->pMail_Buffer);
	EME_UTILS_FREEIF(pcx->pMemHdr, pcx->pcTmpFileName);

	EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_EPOP_ERET_CMD);	
	return EME_EPOP_ERET_CMD(pcx);
}

WE_INT32 EME_EPOP_LogOut(IPOPHANDLER  pPopClient)
{
	ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;
	WE_INT32 iResult = EME_SUCCESS;
	
	if (NULL == pcx || NULL == pcx->pvFileMgr)
	{
		return EME_BADPARM;
	}
	if (NULL != pcx->pcTmpFileName && EME_FileExists(pcx->pvFileMgr, pcx->pcTmpFileName))
	{
		EME_DeleteFile(pcx->pvFileMgr, pcx->pcTmpFileName);
	}
	if (pcx->pvSocket)
	{
		iResult = EME_Socket_Close(pcx->pvSocket);
		pcx->pvSocket = NULL; 
	}
	 
    
	return iResult;
}

WE_INT32 EME_EPOP_Set(IPOPHANDLER  pPopClient, St_EPOP_PushMailSet* pstSet)
{
    ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;

    POP_NULL_ASSERT(pcx);

    pcx->PopResult.iEvCode = EPOP_EVT_SET;
    pcx->Pop_CMD = EPopEset;
    pcx->Pop_Status = ClientSending_CMD;

    pcx->pstEpopSet = pstSet;

    EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_EPOP_ESET_CMD);	

    return EME_EPOP_ESET_CMD(pcx);
}

WE_INT32 EME_EPOP_GetFilter(IPOPHANDLER  pPopClient)
{
    ST_EME_POPCLIENT * pcx = (ST_EME_POPCLIENT*)pPopClient;

    POP_NULL_ASSERT(pcx);

    pcx->PopResult.iEvCode = EPOP_EVT_GET_FILTER;
    pcx->Pop_CMD = EPopEget;
    pcx->Pop_Status = ClientSending_CMD;

    EME_Socket_RegCbReadable(pcx->pvSocket, (WE_VOID*)EME_EPOP_EGET_CMD);

    return EME_EPOP_EGET_CMD(pcx);
}


