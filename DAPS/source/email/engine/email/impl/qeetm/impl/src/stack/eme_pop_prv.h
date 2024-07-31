/*=====================================================================================
    FILE: eme_pop_prv.h
    MODULE NAME : POP

  
    GENERAL DESCRIPTION:
    Declaration of Pop3 Stack Intern functions and data structures
 
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================     
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/08/2007        Hubin                            creat this file
    04/28/2007        Hubin                            Add Apop CMD
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#ifndef EME_POP_PRV_H
#define EME_POP_PRV_H

#include "eme_pop_api.h"

#define EME_POP_CMDBUFF_LEN			256	//command buffer length,max is 255 described in rfc2449
#define EME_POP_RESPBUFF_LEN		5120	//response buffer length
#define EME_POP_USERNAME_LEN		48	//Pop user name max length - 1 
#define EME_POP_PASSWORD_LEN		48	//Pop password	max length - 1
#define EME_POP_UIDLFILE_LEN		64	//Pop uidl file name length - 1
#define EME_GENERIC_TEXT_LEN		100
#define EME_MAX_UIDL_LEN		    255

//Commmand List
#ifdef CMCC_PUSHMAIL_2_0_0
#define USER_CMD_2_0_0	"USER %s 2.0.0\r\n"	//%s = user name
#endif
#define USER_CMD	"USER %s\r\n"	//%s = user name
#define PASS_CMD	"PASS %s\r\n"	//%s = password
#define APOP_CMD  "APOP %s %s\r\n"//%s = name %s = digest
#define RETR_CMD	"RETR %d\r\n"	//%d = msg id
#ifdef CMCC_PUSHMAIL_2_0_0
//ERET <uid> [<content-id> [<convert-type> [<offset> [<length>]]]]
#define ERET_CMD	"ERET %s\r\n"	//%d = msg id
#endif

#define TOP_CMD		"TOP %d 0\r\n"	//%d = msg id,0 means no body
#define DELE_CMD	"DELE %d\r\n" //%d = msg id
#define UIDL_CMD	"UIDL\r\n"
#define STAT_CMD	"STAT\r\n"
#define LIST_CMD	"LIST\r\n"
#define QUIT_CMD	"QUIT\r\n"

//#define TEST_USE

//PopClient CMD
typedef enum _EME_Pop_CMD
{
      PopBegin = 0,		//the very beginning
      PopNone,			//No CMD
      //////////Basic Pop Cmd//////////////////////////
      PopUser,
      PopPass,
      PopRetr,
      PopTop,
      PopUidl,
      PopStat,
      PopList,
      PopDele,
      PopQuit,
      EPopProf,
      EPopEret,
      EPopEset,
      EPopEget,
      ///////////////////////////////////////////////////////
      PopIdle,		//client is idle,any cmd can be given
      PopStatusMax,
      PopStatus_MAX_FOURBYTES = 0x7fffffff
} EME_Pop_CMD;

//PopClient Status
typedef enum _EME_Pop_Status
{
	Client_Idle = 0,
	ClientSending_CMD,
	ClientSending_Data,
	ClientAwaiting_Response,
	ClientAwaiting_Data,
	ClientStatus_Max,
   ClientStatus_MAX_FOURBYTES = 0x7fffffff
} EME_Pop_Status;

//UIDL Data Struct
typedef struct st_eme_popuidl
{
   WE_CHAR *	pData;		//uidl content
   WE_UINT32	uSize;		//The mail's size
   WE_BOOL		bFresh;		//If the mail is fresh(download before)
   WE_BOOL		bExisted;	//If the mail is still there
   //   WE_BOOL     bFetchedSuccess;     //If the mails is fetched successfully(download,decode,store & etc. all success)
   //Can only be changed from outside
   WE_CHAR     acRes[2];     
} St_EME_PopUidl;

//Account Info Struct
typedef struct st_popaccnt
{
   WE_CHAR			MailServer[EME_GENERIC_TEXT_LEN];
   WE_UINT16		uPort;
   WE_CHAR        acRes[2];
} St_PopAccnt;

//PopClient Data Struct
typedef struct st_eme_popclient
{
   //Chars used in pop	
   WE_CHAR				Command[EME_POP_CMDBUFF_LEN];		//command buffer
   WE_CHAR				Response[EME_POP_RESPBUFF_LEN];		//response buffer
   WE_CHAR				UserName[EME_POP_USERNAME_LEN];		//User Name
   WE_CHAR				Password[EME_POP_PASSWORD_LEN];		//PassWord	
   WE_CHAR				Uidl_FileName[EME_POP_UIDLFILE_LEN];	//Uidl's file name
   WE_CHAR*				pcFilter;
   
   St_POP_Ret		PopResult;							//Pop Return Result
   EME_Pop_CMD	   Pop_CMD;						      //Client CMD
   EME_Pop_Status	Pop_Status;					      //Client Status
   //The UIDL CMD infomation
   //(servers's infomation)
   St_EME_PopUidl  *	ppUidl_List;						//Uidl List
   
   WE_UINT32			uMaillist_len;						//Uidl List length
   WE_UINT32         uMailTotal_Size;              //Mails total size
   WE_UINT32			uMailAuto_id;						//the current processing mail(useful in fetch all mails) start from 1
   WE_UINT32			uMailSpec_id;							//The current process mail(useful in get special mail) start from 1	
   WE_INT32          iMailListLimit;
   WE_INT32          iMailSizeLimit;
   WE_INT32          iMailFetched;  //current fetch number of new mails
   WE_INT32          iTotalNum;  //total number of new mails
   WE_INT32          iLatest;    //receive by lastest
   
   //(client infomation)
   St_EME_PopUidl	*	ppUidl_Local;						//The local uidl records
   WE_UINT32			uLocal_Uidlnum;						//Local records number
   //The mail content
   WE_VOID        *pvTmpEmailFile;               //The file pointer of email temp file
   WE_CHAR        *pcTmpFileName;                //The temp email file name
   
   WE_CHAR			*	pMail_Buffer;						//MailContentBuffer
   WE_CHAR			*	pAppend_Header;						//MailAppendHeader
   WE_CHAR        *  pTimeStamp;                     //Get the time stamp needed by apop cmd in the welcome message
   WE_UINT32			uMailBuff_Size;               //The actual mail buffer size
   WE_UINT32			uRetred_Size;						//Retrived Mail Size
   
   POPREGNOTIFY		PopClientNotify;					//Callback
   WE_VOID *			pvSocket;								//socket instance
   WE_VOID *			pvGlobe;							//The MGR point,if NULL means no callback registerd
   WE_VOID *			pvIShell;							//The Net Mgr
   WE_VOID *         pvAsyncMgr;
   WE_VOID *         pvNetMgr;
   //Files
   WE_VOID			*	pvFile;
   WE_VOID			*	pvFileMgr;
   St_EME_MemHandle *	pMemHdr;							//Mem handler
   St_PopAccnt			Accnt;								//Account Info
   
   WE_BOOL				bHeader;							//If fetching header infomation or whole mail
   WE_BOOL				bSpecial;							//If fetching special
   WE_BOOL           bSizeLimitExceed;
   WE_BOOL           bAuth;
   
   WE_CHAR			*pcConvertCharset;								//Retrieved mail character set.
   WE_CHAR			*pcUseCompress;							//Retrieved mail compress.
   WE_CHAR         *pcContentID;
   WE_CHAR			*pcConvertType;
   St_EPOP_PushMailSet*    pstEpopSet;
} ST_EME_POPCLIENT;

#define EME_MAILHEADER_PRESIZE		2048			//Pre-malloced mail header buffer size
#define EME_MAIL_TRAILER			48				//The response is always larget than the mail itself
//Declaration
//////////Basic Pop Cmd Functions///////////////
WE_INT32 EME_POP_USER_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_PASS_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_RETR_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_TOP_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_UIDL_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_STAT_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_LIST_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_QUIT_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_DELE_CMD(IPOPHANDLER  pPopClient);

/*EPOP CMD*/
WE_INT32 EME_EPOP_PROF_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_EPOP_ERET_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_EPOP_ESET_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_EPOP_EGET_CMD(IPOPHANDLER  pPopClient);


//EPOP function

WE_INT32 EME_EPOP_Retrive(IPOPHANDLER pPopClient,WE_BOOL bHeader,WE_SINT32 sMailID);
//Retrive mails function
//bHeader:If get header only || sMailID:if -1,retrive all,else specifies the id
WE_INT32 EME_POP_Retrive(IPOPHANDLER pPopClient,WE_BOOL bHeader,WE_SINT32 sMailID);
//sMailID:if -1,retrive all,else specifies the id
WE_INT32 EME_POP_Delete(IPOPHANDLER pPopClient,WE_SINT32 sMailID);
//Dummy Callback,to avoid unnecessary risk
WE_INT32 EME_POP_DummyCallback(IPOPHANDLER  pPopClient);
//Connection is established
WE_INT32 EME_POP_DoRecv(WE_VOID * pPopClient);
//Detect the welcome message
WE_INT32 EME_POP_DetectWelcome(IPOPHANDLER  pPopClient);
//Reset ALL
WE_INT32 EME_POP_ResetALL(IPOPHANDLER  pPopClient);
/***************************   Notifier************************/
//Failure Notify
WE_INT32 EME_POP_FailureNotify(IPOPHANDLER  pPopClient,WE_INT32 iResult);
//Success Notify
WE_INT32 EME_POP_SuccessNotify(IPOPHANDLER  pPopClient,WE_INT32 iResult);
//Selfdefined Notify
//iResult:Notify the Callback || iRet: Return to Caller
WE_INT32 EME_POP_Notify(IPOPHANDLER  pPopClient,WE_INT32 iResult,WE_INT32 iRet);


//Get mail's id via the uidl value
WE_SINT32 EME_POP_GetMailId(IPOPHANDLER  pPopClient, const WE_CHAR * puidl, WE_SINT32 * psMailId);

/////////Socket coommuications//////////////////
WE_INT32 EME_POP_Sending_CMD(IPOPHANDLER  pPopClient);
WE_INT32 EME_POP_Getting_Response(IPOPHANDLER  pPopClient);
//Socket error handling
WE_INT32 EME_POP_SocketErrorHandling(IPOPHANDLER pPopClient,WE_INT32 iSocketRet);

//Uid files operations
WE_INT32 EME_POP_InitUidFile(IPOPHANDLER  pPopClient);
//Helper function
WE_INT32 EME_POP_GetLine(IPOPHANDLER  pPopClient,WE_CHAR *,const WE_CHAR *,WE_INT32 *);
//Check out all the new mails
WE_INT32 EME_POP_CheckNewMails(IPOPHANDLER  pPopClient);
//Check if the specified mail is new
WE_BOOL EME_POP_CheckMail(IPOPHANDLER  pPopClient,WE_UINT32);
//Update Uidl files
WE_INT32 EME_POP_UpdateUidlLocal(IPOPHANDLER  pPopClient);
//Get MD5 Hash
WE_INT32 EME_POP_MD5Hash(IPOPHANDLER  pPopClient,WE_CHAR * pcData);
//If want a auth
WE_INT32  EME_POP_SetAuth(IPOPHANDLER  pPopClient,WE_BOOL bAuth);
WE_INT32  EME_POP_GetAuth(IPOPHANDLER  pPopClient,WE_BOOL * pbAuth);
//NULL Assert self defined
//Only here will return EME_INVALID_DATA Error
#ifndef POP_NULL_ASSERT
#define POP_NULL_ASSERT(p)				if(NULL == (p))	{\
																EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
																return	EME_INVALID_DATA;\
														}
#endif

#ifndef POP_FALSE_BREAK
#define POP_FALSE_BREAK(p)				if(EME_SUCCESS != (p))	{\
                                                 EME_UTILS_LOG_INFO(("------FALSE break------- in %s:%dl\n",__FILE__,__LINE__));\
                                                 break;\
                                          }
#endif

#ifndef POP_FALSE_ASSERT
#define POP_FALSE_ASSERT(p)				if(EME_SUCCESS != (p))	{\
                                                 EME_UTILS_LOG_INFO(("------FALSE_ASSERT------- in %s:%dl\n",__FILE__,__LINE__));\
                                                 return p;\
                                          }
#endif


#ifndef POP_LESS_ASSERT
#define POP_LESS_ASSERT(p,m)             if((p) < (m))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_INVALID_DATA;\
                                          }
#endif

#ifndef POP_MORE_ASSERT
#define POP_MORE_ASSERT(p,m)             if((p) >= (m))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_INVALID_DATA;\
                                          }
#endif
			
#ifndef POP_LESS_NOTIFY
#define POP_LESS_NOTIFY(pcx,p,m)         if((p) < (m))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);\
                                          }
#endif

//used to detect array out bounder			
#ifndef POP_MORE_NOTIFY
#define POP_MORE_NOTIFY(pcx,p,m)         if((p) >= (m))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);\
                                          }
#endif


#ifndef POP_NULL_NOTIFY
#define POP_NULL_NOTIFY(pcx,p)         if(NULL == (p))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_INVALID_DATA++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_POP_Notify(pcx,EME_INVALID_DATA,EME_FAILED);\
                                          }
#endif

//especially for malloc error
#ifndef POP_NOMEM_NOTIFY
#define POP_NOMEM_NOTIFY(pcx,p)         if(NULL == (p))  {\
                                                   EME_UTILS_LOG_INFO(("++++++++++EME_NOMEMORY++++++++++ in %s:%dl\n",__FILE__,__LINE__));\
                                                return EME_POP_Notify(pcx,EME_NOMEMORY,EME_FAILED);\
                                          }
#endif

						
#endif
