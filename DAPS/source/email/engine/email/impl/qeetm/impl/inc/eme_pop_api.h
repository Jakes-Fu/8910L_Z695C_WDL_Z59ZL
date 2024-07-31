/*=====================================================================================
    FILE: eme_pop_api.h
    MODULE NAME : POP


    GENERAL DESCRIPTION:
    Declaration of Pop3 Stack API

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================     
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    03/05/2007        Hubin                            creat this file
    03/12/2007        Hubin                            modified(var and function declaration)
    03/14/2007        Hubin                            modified(mail id start from 1)
    04/28/2007        Hubin                            Add Apop CMD
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/

#ifndef EME_POP_API_H
#define EME_POP_API_H

#include "we_def.h"
#include "eme_socket_api.h"
#include "eme_utils_api.h"   
            
#define POP_EVT_START     0x30000        //Event code start
#define POP_EVT_LEN       0x2000         //Event code length
//Event code end
#define POP_EVT_RESERVE   POP_EVT_BEGIN + POP_EVT_LEN


//Pop3 Event code
#define POP_EVT_BEGIN            (POP_EVT_START)
#define POP_EVT_INIT             (POP_EVT_START + 0x01)
#define POP_EVT_LOGIN            (POP_EVT_START + 0x02)
#define POP_EVT_GETINFO          (POP_EVT_START + 0x03)
#define POP_EVT_FETACHALL        (POP_EVT_START + 0x04)
#define POP_EVT_FETCHASP         (POP_EVT_START + 0x05)
#define POP_EVT_FETCHHEADERS     (POP_EVT_START + 0x06)
#define POP_EVT_FETCHSPHEADER    (POP_EVT_START + 0x07)
#define POP_EVT_DELETEALL        (POP_EVT_START + 0x08)
#define POP_EVT_DELETESP         (POP_EVT_START + 0x09)
#define POP_EVT_QUIT             (POP_EVT_START + 0x0A)
#define POP_EVT_CANCEL           (POP_EVT_START + 0x0B)
#define POP_EVT_DISCONNECT       (POP_EVT_START + 0x0C)
#define POP_EVT_SIZE_NOTIFY      (POP_EVT_START + 0x0D)
#define POP_EVT_NUMBER_NOTIFY    (POP_EVT_START + 0x0E)

#define EPOP_EVT_FETCHALL		 (POP_EVT_START + 0x0F)
#define EPOP_EVT_FETCHSPPART     (POP_EVT_START + 0x10)
#define EPOP_EVT_FETCHPROF		 (POP_EVT_START + 0x11)
#define EPOP_EVT_FETCHHEADERS    (POP_EVT_START + 0x12)		
#define EPOP_EVT_FETCHSPMAIL	 (POP_EVT_START + 0x13)
#define EPOP_EVT_SET        	 (POP_EVT_START + 0x14)
#define EPOP_EVT_GET_FILTER   	 (POP_EVT_START + 0x15)

#define POP_EVT_RESERVED         (POP_EVT_RESERVE)

//Is it useful?
#define MAX_POPFETCH_LIMIT          20
//Fetch mails type
/*
typedef enum _Pop_FetchType
{
   FetchType_Normal = 0,         //Normal fetch type,if number <= limit,fetching from the early to the latest
                                 //if number > limit,fetch the latest limit number of mails,also from early to the latest
} Pop_FetchType;
*/
typedef struct st_pop_init
{
   WE_CHAR     *pcServerAdd;
   WE_CHAR     *pcUserName;
   WE_CHAR     *pcPassWord;
   WE_CHAR     *pcUidlPath;
   WE_INT32    iIsNeedSSL;
   WE_INT32    iNumLimit;        //fetch num limit,if zero means no limit
   WE_INT32    iSizeLimit;
   WE_INT32    iLatest;
   WE_UINT16   usPort;
   WE_BOOL     bAuth;            //If use Apop security         
   WE_CHAR     acRes[1];

} St_POP_InitConn;

/////////////////Result Structure Of Each Operations Begin///////////////////////////
//Init connection result
typedef struct st_pop_initret
{
   WE_INT32   iInitResult;         //In fact,no actual result needed.
} St_POP_InitRet;               //Dummy ready to be redfined in further use

//Login result
typedef struct st_pop_loginret
{
   WE_INT32   iLoginResult;      //In fact,no actual result needed.
} St_POP_LoginRet;               //Dummy ready to be redfined in further use

//GetMailinfo result
typedef struct st_pop_getinforet
{
   WE_INT32   iTotalSize;         //total size of the mails
   WE_INT32   iTotalNum;          //how many in server
   WE_INT32 * piSizeList;         //the size list
} St_POP_GetinfoRet;

//Fetch All mails result
typedef struct st_pop_fetchallret
{
   WE_CHAR    *   pcFetchedID;         //the uidl value of this fetched mail
   WE_INT32   iFetchedSize;      //the size of this fetched mail
   WE_CHAR    *   pcDataBuffer;      //fetched mail's data buffer(including header and body)
} St_POP_FetchallRet;

//Fetch special mail result
typedef struct st_pop_fetchspret
{
   WE_CHAR     *  pcFetchedID;
   WE_INT32   iFetchedSize;      //the size of this fetched mail
   WE_CHAR    *   pcDataBuffer;      //fetched mail's data buffer(including header and body)
} St_POP_FetchspRet;

//Fetch ALl mails' header result
typedef struct st_pop_allheadersret
{
   WE_CHAR    *   pcFetchedID;         //the uidl value of this fetched mail
   WE_INT32   iFetchedSize;      //the size of this fetched mail
   WE_CHAR    *   pcDataBuffer;      //fetched mail's data buffer(just including header)
} St_POP_AllheaderRet;

//Fetch special mail header result
typedef struct st_pop_spheaderret
{
   WE_CHAR     *  pcFetchedID;
   WE_INT32   iFetchedSize;      //the size of this fetched mail
   WE_CHAR    *   pcDataBuffer;      //fetched mail's data buffer(including header)
} St_POP_SpheaderRet;

//Delete all mail result
typedef struct st_pop_delallret
{
   WE_INT32   iDelAllResult;      //In fact,no actual result needed.
} St_POP_DelallRet;               //Dummy ready to be redefined in further use

//Delete special mail result
typedef struct st_pop_delspret
{
   WE_INT32   iDelSpRet;         //In fact,no actual result needed.
} St_POP_DelspRet;               //Dummy ready to be redefined in further use

//Quit Result
typedef struct st_pop_quitret
{
   WE_INT32   iQuitRet;         //In fact,no actual result needed.
} St_POP_QuitRet;               //Dummy ready to be redefined in further use

//Cancle Result
typedef struct st_pop_cancleret
{
   WE_INT32   iCancleRet;         //In fact,no actual result needed.
} St_POP_CancleRet;               //Dummy ready to be redefined in further use

typedef struct st_pop_sizenotifyret
{
   WE_UINT32   uiCurrSize;
   WE_UINT32   uiTotalSize;
} St_POP_SizeNotifyRet;               

typedef struct st_pop_numnotifyret
{
   WE_UINT32   uiCurrNum;
   WE_UINT32   uiTotalNum;
} St_POP_NumNotifyRet; 

typedef struct
{
    WE_INT32       iObj;
    WE_CHAR*       pcValue;
    WE_CHAR*       pcFilter;
}St_EPOP_PushMailSet;

/////////////////Result Structure Of Each Operations End///////////////////////////

//PopClient Return Structure
typedef struct st_pop_popret
{
   WE_INT32 iResult;            //If Success,return EME_SUCCESS,else return EME_FAILED
   WE_INT32 iEvCode;            //Event code
   WE_BOOL  bHasNewMail;
   WE_CHAR  acRespDesc[512];
   struct
   {
      //this union changes depends on var operations
      St_POP_InitRet         stInitRet;
      St_POP_LoginRet        stLogRet;
      St_POP_GetinfoRet      stGetInfoRet;
      St_POP_FetchallRet     stFetchAllRet;
      St_POP_FetchspRet      stFetchSpRet;
      St_POP_AllheaderRet    stAllHeaderRet;
      St_POP_SpheaderRet     stSpHeaderRet;
      St_POP_DelallRet       stDelAllRet;
      St_POP_DelspRet        stDelSpRet;
      St_POP_QuitRet         stQuitRet;
      St_POP_CancleRet       stCancleRet;
      St_POP_SizeNotifyRet   stSizeNotifyRet;
      St_POP_NumNotifyRet    stNumNotifyRet;
   } detail;
} St_POP_Ret;

//pClientData refers to MGR point especfilly
//pPopRet contains vars return values including the retrieved data from server
typedef WE_VOID (* POPREGNOTIFY) (WE_VOID *,St_POP_Ret *);
typedef WE_VOID * IPOPHANDLER;



//Set The Notify Callback
WE_INT32 EME_POP_RegCallBack(IPOPHANDLER ,POPREGNOTIFY,void *);

//Init the client struct
WE_INT32 EME_POP_InitHandle(IPOPHANDLER * ppPopClient,
                            WE_VOID * pstMemMgr,
                            WE_VOID * pvIShell,
                            WE_VOID * pvAsyncMgr,
                            WE_VOID * pvFileMgr, 
                            WE_VOID* pvNetMgr);

//Destruct pop3 client
WE_INT32 EME_POP_DeleteHandle(IPOPHANDLER * ppPopClient);

//////////Commanly used Interface//////////////////////
//Init the socket connection
WE_INT32 EME_POP_InitConnection(IPOPHANDLER  pPopClient, St_POP_InitConn*);
//Login the mail server
WE_INT32 EME_POP_Login(IPOPHANDLER  pPopClient);
//Return mail information
WE_INT32 EME_POP_GetMailInfo(IPOPHANDLER  pPopClient);
//Fetch all mails in the mails server
WE_INT32 EME_POP_FetchAllMails(IPOPHANDLER  pPopClient);
//Fetch the specified mail in the mail server
WE_INT32 EME_POP_FetchSpecifiedMail(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl);   //id start from 1
//Fetch all mail headers
WE_INT32 EME_POP_FetchAllMailHeaders(IPOPHANDLER  pPopClient);
//Fetch the specified mail header
WE_INT32 EME_POP_FetchSpecifiedMailHeader(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl); //id start from 1
//Delete all mails in the server(while "leave copies on server"option is set to off)
WE_INT32 EME_POP_DeleteAllMails(IPOPHANDLER  pPopClient);
//Delete the specified mail in the server(while "leave copies on server"option is set to off)
WE_INT32 EME_POP_DeleteSepcifiedMail(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl);      //id start from 1
//Quit the current connection and login
WE_INT32 EME_POP_Quit(IPOPHANDLER  pPopClient);
//Cancel the current operation?
WE_INT32 EME_POP_CancelOperation(IPOPHANDLER  pPopClient);
//Reset all vars if want to do a retrieve all/del all job after a retr all/del all job is done
//Called inner already,but for the most safety(sometimes may forget it),may be called
WE_INT32 EME_POP_ResetJobVar(IPOPHANDLER  pPopClient);
//Tag the mail's fetch result flag
WE_INT32 EME_POP_TAG_MAIL(IPOPHANDLER  pPopClient,const WE_CHAR * pcUidl,WE_BOOL bFlag);
//Get one mail's uidl strings
WE_INT32 EME_POP_GetUidl(IPOPHANDLER  pPopClient,WE_INT32 iNum,WE_CHAR * pcUid,WE_INT32 iStrlen);
//Get if size limit
WE_INT32  EME_POP_GetSizeLimit(IPOPHANDLER  pPopClient,WE_BOOL * pbSizeLimit);

WE_INT32 EME_EPOP_Prof(IPOPHANDLER pPopClient, const WE_CHAR *pcConvertCharset, const WE_CHAR *pcUseCompress);
WE_INT32 EME_EPOP_FetchAllMails(IPOPHANDLER pPopClient);
WE_INT32 EME_EPOP_FetchAllMailHeaders(IPOPHANDLER pPopClient);
WE_INT32 EME_EPOP_FetchSepcialMailPart(IPOPHANDLER pPopClient, 
									   const WE_CHAR *pcUidl, 
									   const WE_CHAR *pcContentID,
									   const WE_CHAR *pcConvertType,
									   WE_INT32   iAttachSize);
WE_INT32 EME_EPOP_FetchSepcialMail(IPOPHANDLER pPopClient, const WE_CHAR *pcUidl);

WE_INT32 EME_EPOP_LogOut(IPOPHANDLER  pPopClient);
	
WE_INT32 EME_EPOP_Set(IPOPHANDLER  pPopClient, St_EPOP_PushMailSet* pstSet);
WE_INT32 EME_EPOP_GetFilter(IPOPHANDLER  pPopClient);



#endif
