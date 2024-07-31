/*****************************************************************************
** File Name:      mmisrv_contactsync_api.c                                               *
** Author:                                                             *
** Date:             07/01/2015                                                *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2015       Yanyan.An         Create
** 
****************************************************************************/

#define _MMISRV_CONTACTSYNC_API_C_

/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmipdp_export.h"
#include "mn_type.h"
#include "mmiconnection_export.h"
#include "mmi_module.h"
#include "mmi_appmsg.h"
#include "http_api.h"
#include "in_message.h"
#include "mmi_signal.h"
#include "mmk_type.h"
#include "xml_sap.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmisrv_contactsync_main.h"
#include "mmisrv_contactsync_id.h"
#include "mmisrv_contactsync_export.h"
#include "mmisrv_contactsync_http.h"
#include "mmi_common.h"
#include "cjson.h"
#include "mmipb_export.h"
#include "mmipb_adapt.h"
#include "stdio.h"
#include "mmipb_task.h"

#ifdef MMIPB_SYNC_WITH_O365
//#include "contactsync_text.h"
#include "mmipb_sync_interface.h"
#include "mmimsa_export.h"
#include "mmisrv_contactsync_datalist.h"
//#define MMIPB_SYNC_USE_PC_TOKEN //shiwei add test


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//网络响应状态码
#define PBSYNC_HTTP_STATUS_CODE_200                  		(200)
#define PBSYNC_HTTP_STATUS_CODE_201			          	(201)
#define PBSYNC_HTTP_STATUS_CODE_204     				 (204)
#define PBSYNC_HTTP_STATUS_CODE_400     				 (400)
#define PBSYNC_HTTP_STATUS_CODE_401     				 (401)  //token invalid
#define PBSYNC_HTTP_STATUS_CODE_403     				 (403)  //no permission
#define PBSYNC_HTTP_STATUS_CODE_404     				 (404)  //delete a not exist contact
#define PBSYNC_HTTP_STATUS_CODE_500     				 (500)


#define PBSYNC_PDP_ACTIVE_TIMEOUT       20000

#define MMIPB_SYNC_MODIFIEDTIME_LEN     25
#define MMIPB_SYNC_REQUEST_RETRY_TIME     3



//此部分的宏定义主要是参考browser设计的
#define ACCEPT_STRING           "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml, image/vnd.wap.wbmp, image/gif, image/jpg,image/jpeg, image/png, image/bmp, text/html,*/*;q=0.1"
#define ACCEPT_LANGUAGE_STRING  "en"
#define USER_AGENT_STRING       "Dorado WAP-Browser/1.0.0"
#define ACCEPT_CHARSET_STRING   "UTF-8,ISO-8859-1,US-ASCII,windows-1252"


#define MS_PBSYNC_HTTP_GET_HEADER_NUM  4

/* the size of buffer for reading list data*/
#define PBSYNC_READ_BUFFER_SIZE         1024*30
/* 内存申请大小的步长*/
#define PBSYNC_BUFFER_STEP_SIZE         1024*20

#define PBSYNC_NEXT_LINK_MAX_LEN    200

//Get
#define MS_PBSYNC_URL_PREBASE "https://outlook.office.com/api/v1.0/me/contacts"
//设置返回数量
//#define MS_PBSYNC_URL_GET_MORE_CONTACTS "https://outlook.office365.com/api/v1.0/me/contacts?$top=30"
#define MS_PBSYNC_URL_GET_MORE_CONTACTS "https://outlook.office.com/api/v1.0/me/contacts?$top=50"
//#define MS_PBSYNC_URL_PREBASE "https://132.245.42.242/api/v1.0/me/contacts"
#define MS_PBSYNC_SLASH_KEYWORD  "/"
//#define MS_PBSYNC_ID_KEYWORD		"?$select=Id"
#define MS_PBSYNC_MODITIME_KEYWORD	"?$select=DateTimeLastModified"
#define MS_PBSYNC_AND_MODITIME_KEYWORD	"&$select=DateTimeLastModified"
#define MS_PBSYNC_GET_ADDED_CONTACT_URI "https://outlook.office.com/api/v1.0/me/contacts?$count=true&$filter=DateTimeCreated%20gt%20"
#define MS_PBSYNC_COUNT_KEYWORD "?$count=true"
#define MS_PBSYNC_AND_COUNT_KEYWORD "&$count=true"
#define MS_PBSYNC_AUTHORIZATION "Authorization"
#define MS_PBSYNC_TOKEN_FRONT_STRING "MSAuth1.0 usertoken=\""
#define MS_PBSYNC_TOKEN_BACK_STRING "\", type=\"MSACT\""

#define MS_PBSYNC_USER_AGENT_NAME "User-Agent"
#define MS_PBSYNC_USER_AGENT_VALUE "MSOAuthPlayground/1.0"
#define MS_PBSYNC_CLIENT_ID_NAME "client-request-id"
#define MS_PBSYNC_CLIENT_ID_VALUE "cbb637c7-818c-4b58-9caa-5d3ed247fdd5" //"000000004C12FF0E"
#define MS_PBSYNC_ACCEPT_NAME "Accept"
#define MS_PBSYNC_ACCEPT_VALUE "application/json; odata.metadata=none"

//Post
#define MS_PBSYNC_CONTENT_TYPE_VALUE "application/json"
#define MS_PBSYNC_ADD_CONTENT  "{\"GivenName\": \"Shiwei9999\", \"Surname\": \"Zhang9999\"}"  //for test

//Delete
#define MS_PBSYNC_DELETE_URI MS_PBSYNC_URL_PREBASE

//Patch
#define MS_PBSYNC_PATCH_URI MS_PBSYNC_URL_PREBASE
#ifdef MMIPB_SYNC_USE_PC_TOKEN
char MS_PBSYNC_ACCESSTOKEN[MAX_TOKEN_STR_SIZE] = "";
char MS_PBSYNC_PCTOKEN[MAX_TOKEN_STR_SIZE] = "";
typedef void (* PC_TOKEN_CALLBACK_FUNC)( int result, char*  buf, uint16 buflen); 
#else
char MS_PBSYNC_ACCESSTOKEN[MAX_TOKEN_STR_SIZE] = "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6Ik1uQ19WWmNBVGZNNXBPWWlKSE1iYTlnb0VLWSIsImtpZCI6Ik1uQ19WWmNBVGZNNXBPWWlKSE1iYTlnb0VLWSJ9.eyJhdWQiOiJodHRwczovL291dGxvb2sub2ZmaWNlMzY1LmNvbS8iLCJpc3MiOiJodHRwczovL3N0cy53aW5kb3dzLm5ldC84NzNjNzE3NS1lODJhLTRmNmMtYmI4NS0xOTlhYmYwMTk1OWIvIiwiaWF0IjoxNDQzMDAwNzkzLCJuYmYiOjE0NDMwMDA3OTMsImV4cCI6MTQ0MzAwNDY5MywidmVyIjoiMS4wIiwidGlkIjoiODczYzcxNzUtZTgyYS00ZjZjLWJiODUtMTk5YWJmMDE5NTliIiwib2lkIjoiZTFmYWEzOGUtZTI1ZS00YzExLWJjYmQtMGU4Y2M1N2Y2YzQ3IiwidXBuIjoiS2F0aWVKQHNwcmQwOTE2Lm9ubWljcm9zb2Z0LmNvbSIsInB1aWQiOiIxMDAzMDAwMDkzQ0ZEMURBIiwic3ViIjoiOHZqTzdKSF94Y2lnSFNiTUdfd2QtNUhkVTVfWG13LTRkRjR5QlBrYlUtSSIsImdpdmVuX25hbWUiOiJLYXRpZSIsImZhbWlseV9uYW1lIjoiSm9yZGFuIiwibmFtZSI6IkthdGllIEpvcmRhbiIsImFtciI6WyJwd2QiXSwidW5pcXVlX25hbWUiOiJLYXRpZUpAc3ByZDA5MTYub25taWNyb3NvZnQuY29tIiwiYXBwaWQiOiJjYmI2MzdjNy04MThjLTRiNTgtOWNhYS01ZDNlZDI0N2ZkZDUiLCJhcHBpZGFjciI6IjAiLCJzY3AiOiJDb250YWN0cy5SZWFkIENvbnRhY3RzLlJlYWRXcml0ZSIsImFjciI6IjEiLCJpcGFkZHIiOiIxMTEuMjA1LjE2MS41MCJ9.mn_JPEYtaOg4o6JAaCyYh5xfF60leHVCUwXmZyO2xHeSQJuo2d87nTTVEyh4s4a1LLZjqDVM4-NZ-nzOSJPuELEEOm-rr543zHCvm6l24lN-qJ8AJYZTo4khm5s1ZR92IOuylC_cxTyh9XncBv6uirU0xNYG38XFugxamRZ4f-A-PZWoCSXRXEHI3OhiVPk6nCG5_1GgLyRdVo54bQFGO5hy52N2qc29siGxywHugJyqkdapeYa_F4hVVSFgC35YHyvtydgS4Zjo3oeLPiaZ0tywxSY-Vbt09mgLMypjQEO-dtXZAUAVH2YFanC5d5qJiehSVPJPHEf55E4jljeSRw";
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//此部分的定义主要是参考vcard设计的
LOCAL uint8 s_num_TelephoneTypes[]=
{
    	MMIPB_ADN_NUMBER,      //移动号码  
	MMIPB_HOME_NUMBER,     //家庭号码
	MMIPB_OFFICE_NUMBER,   //办公号码
	MMIPB_UNKNOWN_NUMBER  //未知号码
};


/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/************************************************************************/
/*       PbSync线程 data proc 驱动的类型定义                                */
/************************************************************************/
typedef  uint32 (*DATAPROC_FUNC)(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);

typedef struct _PBSYNC_DATAPROC_DRIVE
{
    uint32			    proc_type;          //处理类型
    DATAPROC_FUNC       proc_func;          //该请求下的消息处理函数
}PBSYNC_DATAPROC_DRIVE, *PBSYNC_DATAPROC_DRIVE_TBL;

typedef struct _PBSYNC_CUR_REQUEST_PARA
{
    HTTP_CONTEXT_ID_T    s_context_id;//一次http_initrequest 对应一个context_id，而且这个值不能修改，发送请求时要用到
    BOOLEAN    s_need_retry;//需要重试，close -> init
    BOOLEAN    s_need_refresh_token;
}PBSYNC_CUR_REQUEST_PARA;

/************************************************************************/
/*       PBSYNC线程 data proc 驱动的类型定义                                */
/************************************************************************/
#define PBSYNC_DATAPROC_MAP_END_FLAG	0xFFFC

#define PBSYNC_DATAPROC_MAP_BEGIN(thePointer)			\
	if (thePointer) {									\
	static const PBSYNC_DATAPROC_DRIVE _ProcDriveTbl[] =	\
{													
#define PBSYNC_DATAPROC_MAP(proc_type, proc_func)		\
{ (uint32)proc_type, (DATAPROC_FUNC)proc_func },

#define PBSYNC_DATAPROC_MAP_END(thePointer)				\
{PBSYNC_DATAPROC_MAP_END_FLAG, PNULL}				\
};													\
    *thePointer = &_ProcDriveTbl[0];				\
}

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8  s_contacts_sync_account_index[MN_DUAL_SYS_MAX]={0}; 
LOCAL uint32 s_contacts_sync_net_id = 0;
LOCAL uint8  s_contacts_sync_pdp_timer_id      = 0;
LOCAL uint8  s_contacts_sync_pdp_retry_timer_id = 0;
LOCAL MN_DUAL_SYS_E s_contacts_sync_cur_sys_num   = MN_DUAL_SYS_1;

LOCAL PBSYNC_CUR_REQUEST_PARA cur_request_para = {0};
//LOCAL HTTP_CONTEXT_ID_T    s_context_id = 0;
//LOCAL BOOLEAN    s_need_retry = FALSE;

//网络返回的数据处理驱动表
LOCAL PBSYNC_DATAPROC_DRIVE_TBL     s_dataproc_tbl = {0};

LOCAL char*						s_http_data_ptr = PNULL;
LOCAL uint32					s_http_data_cur_size = 0;
LOCAL uint32					s_http_data_last_size = 0;

LOCAL char					s_http_data_next_link[PBSYNC_NEXT_LINK_MAX_LEN] = {0};
LOCAL BOOLEAN				s_cur_chunk_end_flag = TRUE;//表示当前查询的数据块结束
LOCAL BOOLEAN				s_all_end_flag = TRUE; // 表示所有联系人信息都已查询完毕



/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : PbSync data proc 驱动表初始化
//  Global resource dependence : 
//  Author: 
//  Note: PbSync 线程初始化的时候一定要初始化
/*****************************************************************************/
PUBLIC void PBSYNC_DataProcDriveTblInit(void);

/*****************************************************************************/
//  Description : PbSync data proc 驱动表初始化
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PBSYNC_GetDataProcDriveTbl(PBSYNC_DATAPROC_DRIVE_TBL* tbl_ptr);
/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: yanxian.zhou
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
								uint8 timer_id,
								uint32 param
								);

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void);

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void);

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void);

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpInitCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpRequestIdInd(DPARAM param);

/*****************************************************************************/
//  Description : HandleHttpDataInd
//  Global resource dependence : 
//  Author: Glen Li
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpDataInd(
									 xSignalHeaderRec * http_context_ptr, 
									 DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpGetCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Post Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpPostCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Delete Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpDeleteCnf(DPARAM param);


/*****************************************************************************/
//  Description : Handle Http Patch Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpPatchCnf(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpHeaderInd(DPARAM param);
/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpErrorInd(DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpCloseCnf(DPARAM param);
/*****************************************************************************/
//  Description : 处理网络返回的数据
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : exception handle
//  Global resource dependence : 
//  Author: shiwei.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HTTP_ProcError(HTTP_APP_INSTANCE_T  app_instance, uint32 err_code);
/*****************************************************************************/
//  Description : APP容错处理
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E APP_ProcError(HTTP_APP_INSTANCE_T  app_instance, uint16  msg_id);
/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);
/*****************************************************************************/
//  Description : PBSYNC
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL void PBSYNC_SendMsgToApp(uint32 sig_id);
/*****************************************************************************/
//  Description : 清空标志位  
//  Global resource dependence: 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ClearFlag(void);
/*****************************************************************************/
//  Description : SetAllEndFlag
//  Global resource dependence : s_all_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetAllEndFlag(BOOLEAN is_all_end);
/*****************************************************************************/
//  Description : SetCurChunkEndFlag
//  Global resource dependence : s_cur_chunk_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetCurChunkEndFlag(BOOLEAN is_chunk_end);
/*****************************************************************************/
//  Description : GetAllEndFlag
//  Global resource dependence : s_all_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAllEndFlag(void);
/*****************************************************************************/
//  Description : GetCurChunkEndFlag
//  Global resource dependence : s_cur_chunk_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCurChunkEndFlag(void);
/*****************************************************************************/
//  Description : is token expired
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E JsonParser_IsTokenExpired(cJSON * json_object);
/*****************************************************************************/
//  Description : create cjson, get char data from json
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL char * makeJson(MMIPB_CONTACT_T	*contact_ptr);
/*****************************************************************************/
//  Description : 请求获取联系人列表数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactsListData(HTTP_APP_INSTANCE_T  app_instance);
/*****************************************************************************/
//  Description : 请求获取指定联系人数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactInfoData(HTTP_APP_INSTANCE_T  app_instance, char* contactId);
/*****************************************************************************/
//  Description : 请求获取联系人的最近一次修改时间
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactModifyTimeData(HTTP_APP_INSTANCE_T  app_instance);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加的联系人
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllAddedContacts (HTTP_APP_INSTANCE_T  app_instance, uint32 uiTime);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改的联系人
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllUpdatedContacts(HTTP_APP_INSTANCE_T  app_instance, uint32 uiTime);
/*****************************************************************************/
//  Description : 查询Office365当前联系人总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetTotalCount(HTTP_APP_INSTANCE_T  app_instance);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllAddedContactsCount(HTTP_APP_INSTANCE_T  app_instance, uint32 uiTime);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改联系人的总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllUpdatedContactsCount(HTTP_APP_INSTANCE_T  app_instance, uint32 uiTime);
/*****************************************************************************/
//  Description : Handle Post Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PostRequest(HTTP_APP_INSTANCE_T  app_instance, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : Handle Delete Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DeleteRequest(HTTP_APP_INSTANCE_T  app_instance, char* O365_id);
/*****************************************************************************/
//  Description : Handle Patch Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PatchRequest(HTTP_APP_INSTANCE_T  app_instance, char* O365_id, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_LISTGET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactListDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_INFO_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactInfoDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的最近一次修改时间的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_MODIFYTIME_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactModifyTimeDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_ADD的网络返回的数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络响应 CONTACTSYNC_CONTACT_DELETE 消息返回的数据
//  Global resource dependence: 
//  Author: Hongjun.Jia
//  Note: 
/*****************************************************************************/
LOCAL uint32 OnO365ContactDeleteDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_MODIFY的网络返回的数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactModifyDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddedDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactUpdatedDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAllCountDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddedCountDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactUpdatedCountDataProc(HTTP_APP_INSTANCE_T app_instance, const char* str_ptr);
/*****************************************************************************/
//  Description : parser list data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseListData(HTTP_APP_INSTANCE_T app_instance, const char *json_string);
/*****************************************************************************/
//  Description : parser info data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseInfoData(HTTP_APP_INSTANCE_T app_instance, const char *json_string);
/*****************************************************************************/
//  Description : parser all modify time data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseAllModifyTimeData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string);
/*****************************************************************************/
//  Description : parser count data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseCountData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string);
/*****************************************************************************/
//  Description : ParseContactListJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E JsonParser_ParseContactListJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_object);
/*****************************************************************************/
//  Description : ParseAContactJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAContactJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj);
/*****************************************************************************/
//  Description : JsonParser_ParseAllModifyTimeJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAllModifyTimeJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj);
/*****************************************************************************/
//  Description : JsonParser_ParseAModifyTimeJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAModifyTimeJsonObject( HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj);
/*****************************************************************************/
//  Description : JsonParser_ParseCountJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseCountJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj) ;
/*****************************************************************************/
//  Description : is response error
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 JsonParser_ParseResponse(char * response_str);

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : PBSYNC data proc 驱动表初始化
//  Global resource dependence : 
//  Author: 
//  Note: PBSYNC 线程初始化的时候一定要初始化
/*****************************************************************************/
PUBLIC void PBSYNC_DataProcDriveTblInit(void)
{
    PBSYNC_GetDataProcDriveTbl(&s_dataproc_tbl);
}
/*****************************************************************************/
//  Description : PbSync data proc 驱动表初始化
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PBSYNC_GetDataProcDriveTbl(PBSYNC_DATAPROC_DRIVE_TBL* tbl_ptr)
{
    PBSYNC_DATAPROC_MAP_BEGIN(tbl_ptr)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_LISTGET,             OnO365ContactListDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_INFO_GET,            OnO365ContactInfoDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_MODIFYTIME_GET,    	OnO365ContactModifyTimeDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_ADD,               	OnO365ContactAddDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_DELETE,              OnO365ContactDeleteDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_MODIFY,         		OnO365ContactModifyDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME,    	OnO365ContactAddedDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME,    OnO365ContactUpdatedDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_COUNT_GET,    			OnO365ContactAllCountDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET, OnO365ContactAddedCountDataProc)
        PBSYNC_DATAPROC_MAP(PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET, OnO365ContactUpdatedCountDataProc)
	PBSYNC_DATAPROC_MAP_END(tbl_ptr)
        
}

PUBLIC void MMISRV_PBSYNC_OpenWaitingWindow(void)
{
#if 0 //wk
    MMI_STRING_T            wait_text = {0};//move here
    //move here
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PBSYNC_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
    //move here
#endif	
}

PUBLIC void MMISRV_PBSYNC_CloseWaitingWindow(void)
{
#if 0 //wk
    MMK_CloseWin(PBSYNC_WAIT_WIN_ID);
#endif 
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetApnStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    uint8   index   = 0;

    index = s_contacts_sync_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }

    return apn_str;
    
}
/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetUserNameStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   user_name_str = PNULL;
    uint8   index   = 0;

    index = s_contacts_sync_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->username_len)
    {
        user_name_str = (char*)linksetting->username;
    }
    
    return user_name_str;
    
}
/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetPasswordStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   password_str = PNULL;
    uint8   index   = 0;

    index = s_contacts_sync_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    
    return password_str;
}

#ifdef IPVERSION_SUPPORT_V4_V6

LOCAL int GetIpType(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    uint8   index   = 0;

    index = s_contacts_sync_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting)
    {
        return linksetting->ip_type;
    }
    
    return 0;
}

#endif

/*****************************************************************************/
//  Description : 初始化互联网账户设置。如果有预设值，则初始化为预设值
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRV_PBSYNC_InitNetworkAccount(void)
{
    uint32  dual_sys    = 0;
    BOOLEAN is_changed  = FALSE;
    SCI_TRACE_LOW("[PBSYNC]:MMISRV_PBSYNC_InitNetworkAccount");
    
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        char*   apn_str     = PNULL;
        uint32  index       = 0;
        if(MMIAPIPHONE_IsSimCardOk(dual_sys))
        {
            {
                uint8   linkset_num = MMIAPICONNECTION_GetLinkSettingNum(dual_sys);
                for(index = 0; index < linkset_num; index++)
                {
                    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
                    if(PNULL != linksetting_ptr && 0 != strlen(linksetting_ptr->apn))
                    {
                        SCI_TRACE_LOW("[PBSYNC]:MMISRV_PBSYNC_InitNetworkAccount----sim%d  index = %d" , index);                        
                        s_contacts_sync_account_index[dual_sys] = index;
                        is_changed = TRUE;
                        break;
                    }
                }
            }
        }
    }

    if(!is_changed)
    {
        SCI_TRACE_LOW("[PBSYNC]:MMISRV_PBSYNC_InitNetworkAccount----find no SIM !!!");               
    }    
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void) 
{
   SCI_TRACE_LOW("[PBSYNC]:##StopPdpActiveTimer");

    if(0 != s_contacts_sync_pdp_timer_id)
    {
        MMK_StopTimer(s_contacts_sync_pdp_timer_id);
        s_contacts_sync_pdp_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : pdp retry if it can't active
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ContactsSyncPDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;

    active_info.app_handler         = MMI_MODULE_CONTACTSYNC;//g_current_module_id;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = GetApnStr(sim_sys);
    active_info.user_name_ptr       = GetUserNameStr(sim_sys);
    active_info.psw_ptr             = GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
    active_info.ip_type = GetIpType(sim_sys);
#endif
    if(MMIAPIPDP_Active(&active_info))
    {
        MMISRV_PBSYNC_OpenWaitingWindow();
        return_val = TRUE;
    }
    SCI_TRACE_LOW("[PBSYNC]:ContactsSyncPDPTry return_val = %d",return_val);
    return return_val;
}

/*****************************************************************************/
//  Description :HandlePdpRetryTimer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    SCI_TRACE_LOW("[PBSYNC]:HandlePdpRetryTimer begin");
    if(timer_id == s_contacts_sync_pdp_retry_timer_id && 0 != s_contacts_sync_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_contacts_sync_pdp_retry_timer_id);
        s_contacts_sync_pdp_retry_timer_id = 0;
    }
    s_contacts_sync_cur_sys_num ++;
    if(s_contacts_sync_cur_sys_num < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = s_contacts_sync_cur_sys_num ; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                s_contacts_sync_cur_sys_num = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!ContactsSyncPDPTry(s_contacts_sync_cur_sys_num ))
            {
        	    s_contacts_sync_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                            3000, 
                                            		        HandlePdpRetryTimer, 
                                            		        PNULL, 
                                                		    FALSE
                                                		    );
            }
        }
        else
        {
            SCI_TRACE_LOW("[PBSYNC]:HandlePdpRetryTimer Deactive pdp 1");
            MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
        }
    }
    else//找不到卡了
    {
        SCI_TRACE_LOW("[PBSYNC]:HandlePdpRetryTimer Deactive pdp 2");
        MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
    }
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    SCI_TRACE_LOW("[PBSYNC]:HandlePdpActiveTimer---Time out error!---");
    SCI_TRACE_LOW("[PBSYNC]:HandlePdpActiveTimer Deactive pdp");
    MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void)
{
    if(0 != s_contacts_sync_pdp_timer_id)
    {
        MMK_StopTimer(s_contacts_sync_pdp_timer_id);
        s_contacts_sync_pdp_timer_id = 0;
    }
    s_contacts_sync_pdp_timer_id = MMK_CreateTimerCallback(20000, 
                            HandlePdpActiveTimer, 
                            PNULL, 
                            FALSE);
    SCI_TRACE_LOW("[PBSYNC]:StartPdpActiveTimer---s_contacts_sync_pdp_timer_id = %d", s_contacts_sync_pdp_timer_id);
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg--- msg_id=%d",msg_ptr->msg_id);
    SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg--- result=%d" , msg_ptr->result);

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg--- MMIPDP_ACTIVE_CNF");
        StopPdpActiveTimer();
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
	{
            s_contacts_sync_net_id = msg_ptr->nsapi;
            PBSYNC_SendMsgToApp(MSG_PBSYNC_PDP_ACTIVE_CNF);
	     SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg  MMIPDP_RESULT_SUCC s_contacts_sync_net_id %d " , s_contacts_sync_net_id); 
	}
	else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
	{
	    MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
            SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg Deactive fail pdp 1");
	    if(0 != s_contacts_sync_pdp_retry_timer_id)
	    {
                MMK_StopTimer(s_contacts_sync_pdp_retry_timer_id);
                s_contacts_sync_pdp_retry_timer_id = 0;
	    }
	    s_contacts_sync_pdp_retry_timer_id = MMK_CreateTimerCallback(
	                                                    2000, 
                                        		        HandlePdpRetryTimer, 
                                        		        PNULL, 
                                            		    FALSE
                                            		    );
	}
        else 
        {
            SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg Deactive fail pdp 2");
            MMIAPIPBSYNC_PB_RequestFailHandler(0, RESPONSE_PDP_DEACTIVE_1_ERROR);
            MMIPB_SYNC_HTTP_Request_List_Destroy();
            HTTP_CloseRequest(cur_request_para.s_context_id);
            MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
        SCI_TRACE_LOW("[PBSYNC]:MMIRECORD_HandlePdpMsg--- MMIPDP_DEACTIVE_CNF 2");        
        s_contacts_sync_net_id = 0;
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        SCI_TRACE_LOW("[PBSYNC]:MMIRECORD_HandlePdpMsg--- MMIPDP_DEACTIVE_IND 3");
        s_contacts_sync_net_id = 0;
        SCI_TRACE_LOW("[PBSYNC]:HandlePdpMsg MMIPDP_DEACTIVE_IND Deactive pdp 3");
        //bug502132 最好判断一下是不是syncing状态。
        MMIAPIPBSYNC_PB_RequestFailHandler(0, RESPONSE_PDP_DEACTIVE_2_ERROR);
        MMIPB_SYNC_HTTP_Request_List_Destroy();
        HTTP_CloseRequest(cur_request_para.s_context_id);
        MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_PBSYNC_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    SCI_TRACE_LOW("[PBSYNC]:Enter MMISRV_PBSYNC_PdpActive");


#ifdef WIFI_SUPPORT    
    //优先wifi
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_CONTACTSYNC;
        active_info.handle_msg_callback = HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    else
#endif //WIFI_SUPPORT
    {
        //uint16 sim_ok = MN_DUAL_SYS_1;
        MN_DUAL_SYS_E sim_ok = MN_DUAL_SYS_1;
        s_contacts_sync_cur_sys_num   = MN_DUAL_SYS_1;
        if(TRUE == MMIAPIPHONE_GetDataServiceSIM(&sim_ok))
        {
                s_contacts_sync_cur_sys_num = (MN_DUAL_SYS_E)sim_ok;
        }
        else
        {
                MMIAPIPHONE_AlertSimNorOKStatus();
                return FALSE;
        }			
        SCI_TRACE_LOW("[PBSYNC]:MMISRV_PBSYNC_PdpActive s_contacts_sync_cur_sys_num = %d",s_contacts_sync_cur_sys_num);
        return_val = ContactsSyncPDPTry(s_contacts_sync_cur_sys_num );//第一张有效卡
        if(FALSE == return_val) 
        {
		    if(0 != s_contacts_sync_pdp_retry_timer_id)
		    {
                MMK_StopTimer(s_contacts_sync_pdp_retry_timer_id);
                s_contacts_sync_pdp_retry_timer_id = 0;
		    }
		    s_contacts_sync_pdp_retry_timer_id = MMK_CreateTimerCallback(
	                                                    3000, 
			                              		        HandlePdpRetryTimer, 
                                        		        PNULL, 
                                            		    FALSE
                                            		    );
        }
    }
    
    return return_val;
}

#ifdef MMIPB_SYNC_USE_PC_TOKEN
/*****************************************************************************/
//  FUNCTION:      MMIAPIPC_get_token
//  Description :
//  Global resource dependence :
//  Author:        wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIPC_get_token(int login, PC_TOKEN_CALLBACK_FUNC func)
{
    MMIFILE_HANDLE sfs_handle = 0;
    uint32 file_size = 0;
    uint32 readed_size = 0;
    uint8 *data_info_prt = PNULL;
    wchar *token_file_path = L"D:\\access_token.txt";

    SCI_MEMSET(MS_PBSYNC_PCTOKEN, 0x0, sizeof(MS_PBSYNC_PCTOKEN));
    sfs_handle = MMIAPIFMM_CreateFile(token_file_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    SCI_TRACE_LOW("[PBSYNC] USE_PC_TOKEN MMIAPIFMM_CreateFile");
    if(SFS_INVALID_HANDLE != sfs_handle)
    {
        SCI_TRACE_LOW("[PBSYNC] USE_PC_TOKEN MMIAPIPBSYNC_StartInitHttp handle valid");
        file_size = MMIAPIFMM_GetFileSize(sfs_handle);
        if(0 < file_size)
        {SCI_TRACE_LOW("[PBSYNC] USE_PC_TOKEN MMIAPIPBSYNC_StartInitHttp file_size >0");
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(sfs_handle, MS_PBSYNC_PCTOKEN, file_size, &readed_size, NULL))
            {
                //access_token_len = file_size;
                SCI_TRACE_LOW("[PBSYNC] USE_PC_TOKEN MMIAPIPBSYNC_StartInitHttp get token success");
                MMIAPIFMM_CloseFile(sfs_handle);
                SCI_TRACE_LOW("[PBSYNC]:Enter MMIAPIPC_get_token ------");
                if(PNULL == func)
                {
                	SCI_TRACE_LOW("[PBSYNC]:MMIAPIPC_get_token  func = PNULL !!!");
                	return 0;
                }
                func(1 , MS_PBSYNC_PCTOKEN , readed_size);
            }
        }
    }

    return 1;
}
#endif


/*****************************************************************************/
//  Description : active pdp and start init http
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_StartInitHttp(void)
{
    MMISRV_PBSYNC_InitNetworkAccount();
    MMISRV_PBSYNC_PdpActive();
    //20150906 move here
    SCI_TRACE_LOW("[PBSYNC] StartHttpRequest s_http_data_next_link memset 0");
    SCI_MEMSET(s_http_data_next_link, 0x00, PBSYNC_NEXT_LINK_MAX_LEN);
    //20150906 move here
}

/*****************************************************************************/
//  Description : disconnected http and deactive pdp
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_StopHttp(void)
{
	//wk test
	SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_StopHttp HTTP_CloseRequest %d", cur_request_para.s_context_id);
	HTTP_CloseRequest(cur_request_para.s_context_id);//shiwei add for bug500029
	/* disconnected http and deactive pdp */
	MMIAPIPDP_Deactive(MMI_MODULE_CONTACTSYNC);
	
	//send confirm msg to PB MMI 
	//SCI_TRACE_LOW("[PBSYNC] Send MMIPBSYNC_SIG_LOGOUT_CNF");
	//MMIPB_SendSignalToPBTask(MMIPBSYNC_SIG_LOGOUT_CNF, 0);
	SCI_TRACE_LOW("[PBSYNC] call MMIAPIPBSYNC_PB_LogoutHandler");
	MMIAPIPBSYNC_PB_LogoutHandler(0);
}

/*****************************************************************************/
//  Description : 请求获取联系人列表数据
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllContacts(void)
{
    uint32 get_contact_id = 1;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_LISTGET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O send MMIAPIPBSYNC_SVR_GetAllContacts request");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllContacts HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 请求获取指定联系人数据
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetContactById(char *O365_id)
{
    uint32 get_contact_id = 2;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;

    if (PNULL == O365_id)
    {
        SCI_TRACE_LOW("[PBSYNC] error MMIAPIPBSYNC_SVR_GetContactById PNULL == O365_id");
        return MMIPB_ERROR_ERROR;
    }
    strcpy(request_para.contact_info.o365_id,O365_id);

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_INFO_GET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O send MMIAPIPBSYNC_SVR_GetContactById request");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetContactById HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 请求获取联系人的最近一次修改时间
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllIdAndModiTime(void)
{
    uint32 get_contact_id = 3;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_MODIFYTIME_GET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_GetAllIdAndModiTime request");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllIdAndModiTime HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加的联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContacts(uint32 uiTime)
{
    uint32 get_contact_id = 4;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    request_para.contact_info.o365_modifiedTime = uiTime;

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_GetAllAddedContacts request uiTime = %d", uiTime);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllAddedContacts HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改的联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllUpdatedContacts(uint32 uiTime)
{
    uint32 get_contact_id = 5;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    request_para.contact_info.o365_modifiedTime = uiTime;

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_GetAllUpdatedContacts request uiTime = %d", uiTime);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllUpdatedContacts HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 查询Office365当前联系人总数
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetTotalCount(void)
{
    uint32 get_contact_id = 6;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_COUNT_GET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O send MMIAPIPBSYNC_SVR_GetTotalCount request");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetTotalCount HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContactsCount(uint32 uiTime)
{
    uint32 get_contact_id = 7;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    request_para.contact_info.o365_modifiedTime = uiTime;

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_GetAllAddedContactsCount request uiTime = %d", uiTime);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllAddedContactsCount HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改联系人的总数
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount(uint32 uiTime)
{
    uint32 get_contact_id = 8;
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;
    request_para.contact_info.o365_modifiedTime = uiTime;

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(get_contact_id, PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount request uiTime = %d", uiTime);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 请求添加联系人到O365服务器
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_AddContact(uint32 request_id, MMIPB_CONTACT_T *contact_ptr)
{
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;

	SCI_TRACE_LOW("[PBSYNC]MMIAPIPBSYNC_SVR_AddContact,request_id=0X%X", request_id);

    if (PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("[PBSYNC] error MMIAPIPBSYNC_SVR_AddContact PNULL == contact_ptr");
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMCPY(&request_para.contact_info, contact_ptr, sizeof(request_para.contact_info));

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(request_id, PBSYNC_CONTACT_ADD, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_AddContact contact_ptr->name.wstr_len %d", contact_ptr->name.wstr_len);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_AddContact contact_ptr->name.wstr 0 %d", contact_ptr->name.wstr[0]);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_AddContact contact_ptr->name.wstr 1 %d", contact_ptr->name.wstr[1]);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_AddContact contact_ptr->name.wstr 2 %d", contact_ptr->name.wstr[2]);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_AddContact HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 请求从O365服务器上删除一条联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_DeteteContact(uint32 request_id, char* O365_id)
{
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;

	SCI_TRACE_LOW("[PBSYNC]MMIAPIPBSYNC_SVR_DeteteContact,request_id=0X%X", request_id);
	
    if (PNULL == O365_id)
    {
        SCI_TRACE_LOW("[PBSYNC] error MMIAPIPBSYNC_SVR_DeteteContact PNULL == O365_id");
        return MMIPB_ERROR_ERROR;
    }
    strcpy(request_para.contact_info.o365_id,O365_id);

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(request_id, PBSYNC_CONTACT_DELETE, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_DeteteContact request");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_DeteteContact HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 请求从O365服务器上修改联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_ModifyContact(uint32 request_id, char* O365_id, MMIPB_CONTACT_T *contact_ptr)
{
    PBSYNC_HTTP_REQUEST_PARAM_T request_para = {0};
    SCI_MEMSET(&request_para, 0x00, sizeof(PBSYNC_HTTP_REQUEST_PARAM_T));
    request_para.http_request_fail_retry_time = MMIPB_SYNC_REQUEST_RETRY_TIME;

	SCI_TRACE_LOW("[PBSYNC]MMIAPIPBSYNC_SVR_ModifyContact,request_id=0X%X", request_id);
	
    if(PNULL == O365_id || PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("[PBSYNC] error MMIAPIPBSYNC_SVR_ModifyContact PNULL == O365_id || PNULL == contact_ptr");
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMCPY(&request_para.contact_info, contact_ptr, sizeof(request_para.contact_info));
    //第一个参数冗余
    SCI_MEMSET(request_para.contact_info.o365_id, 0x00, sizeof(request_para.contact_info.o365_id));
    strcpy(request_para.contact_info.o365_id,O365_id);
    //暂时先以第一个参数为准

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsFull())
    {
        MMIPB_SYNC_HTTP_Request_List_Append_Item(request_id, PBSYNC_CONTACT_MODIFY, &request_para);
        PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_INIT_CNF);
        SCI_TRACE_LOW("[PBSYNC] ## P--->O Send MMIAPIPBSYNC_SVR_ModifyContact  %s request", contact_ptr->name.wstr);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] ## MMIAPIPBSYNC_SVR_ModifyContact HTTP_Request_List is full, send it later.");
        return MMIPB_ERROR_BUSY;
    }

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : callback function
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note: 
//		result :	
//			MSA_OK 		- 操作成功 		
//			MSA_FAILED 	- 操作失败
//        	MSA_NO_TOKEN	- 使用者尚未登入
//		buf:  token data
//		buflen: token length
/*****************************************************************************/
PUBLIC void contact_sync_get_token_cb( int result, char*  buf, int buflen)
{

    SCI_TRACE_LOW("[PBSYNC] contact_sync_get_token_cb result = %d, buf(token) = %s, buflen = %d", result, buf, buflen);

    if(MSA_OK == result)
    {
        PBSYNC_AccesstokenSave(buf);
        SCI_TRACE_LOW("[PBSYNC] contact_sync_get_token_cb get token success.");
        //send msg to MMI 
        //PBSYNC_SendMsgToApp(MSG_PBSYNC_LOGIN_CNF);
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] contact_sync_get_token_cb get token !!! fail.");
    }
	
}

/*****************************************************************************/
//  Description : callback function
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note: 
//		result :	
//			MSA_OK 		- 操作成功 		
//			MSA_FAILED 	- 操作失败
//        	MSA_NO_TOKEN	- 使用者尚未登入
//		buf:  token data
//		buflen: token length
/*****************************************************************************/
LOCAL void contact_sync_refresh_token_cb( int result, char*  buf, int buflen)
{

    SCI_TRACE_LOW("[PBSYNC] contact_sync_refresh_token_cb result = %d, buf(token) = %x, buflen = %d", result, buf, buflen);

    if(MSA_OK == result)
    {
        PBSYNC_AccesstokenSave(buf);
        SCI_TRACE_LOW("[PBSYNC] contact_sync_refresh_token_cb get token success.");
        PBSYNC_SendMsgToApp(MSG_PBSYNC_CONN_FAILED_CNF);
        SCI_TRACE_LOW("[PBSYNC] RETRY STEP ---2--- send MSG_PBSYNC_CONN_FAILED_CNF.");
    }
    else
    {
        SCI_TRACE_LOW("[PBSYNC] contact_synccontact_sync_refresh_token_cb_get_token_cb get token !!! fail.");
    }
	
}

/*****************************************************************************/
//  Description : 获取access token
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void PBSYNC_AccessTokenGet(void)
{
    SCI_TRACE_LOW("[PBSYNC] call MMIAPIMSA_get_token id = %d", MSA_APP_TYPE_CONTACT_SYNC);
    MMIAPIMSA_get_token(TRUE, MSA_APP_TYPE_CONTACT_SYNC, contact_sync_get_token_cb);
}
/*****************************************************************************/
//  Description : 保存access token
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void PBSYNC_AccesstokenSave(char* access_token)
{
    if (access_token)
    {
        SCI_MEMSET(MS_PBSYNC_ACCESSTOKEN, 0X00, sizeof(MS_PBSYNC_ACCESSTOKEN));
        sprintf(MS_PBSYNC_ACCESSTOKEN , "%s" , access_token);
        //SCI_MEMCPY(MS_PBSYNC_ACCESSTOKEN, access_token, sizeof(MS_PBSYNC_ACCESSTOKEN));
    }
}
/*****************************************************************************/
//  Description : Free Header
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader, uint32 header_num)
{
    uint32 i = 0;
    if (pHttpExternHeader == NULL)
        return ;
	
    for (; i < header_num; i++)
    {
        if (pHttpExternHeader->other_header_ptr[i].header_name_ptr != NULL)
        {
			SCI_FREE(pHttpExternHeader->other_header_ptr[i].header_name_ptr);
			pHttpExternHeader->other_header_ptr[i].header_name_ptr = NULL;
        }
        if (pHttpExternHeader->other_header_ptr[i].header_value_ptr != NULL)
        {
			SCI_FREE(pHttpExternHeader->other_header_ptr[i].header_value_ptr);
			pHttpExternHeader->other_header_ptr[i].header_value_ptr = NULL;
        }
    }
}

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 HandlePBSyncHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    uint32 result = PBSYNC_ERROR_NOP;
    void *original_param = (void*)((uint32)param);

    switch (msg_id)
    {
        case HTTP_SIG_INIT_CNF:
        {
            HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
            if(cnf->module_id != MMI_MODULE_CONTACTSYNC || 1 != cnf->app_param)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_INIT_CNF app_param = %d,cnf->module_id = %d, context_id = %d",cnf->app_param,cnf->module_id, cnf->context_id);
            result = HandleHttpInitCnf(param);
        }
        break;
		
        case HTTP_SIG_REQUEST_ID_IND:
        {
            HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_REQUEST_ID_IND context_id = %d",cnf->context_id);
            result = HandleHttpRequestIdInd(param);
        }
        break;
		
       case HTTP_SIG_DATA_IND:
	{
            HTTP_DATA_IND_SIG_T 	*cnf = (HTTP_DATA_IND_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_DATA_IND context_id = %d",cnf->context_id);
            result = HandleHttpDataInd(cnf, param);
	}
	break;
		
       case HTTP_SIG_GET_CNF:
    	{
            HTTP_GET_CNF_SIG_T 	*cnf = (HTTP_GET_CNF_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_DATA_IND context_id = %d",cnf->context_id);
            result = HandleHttpGetCnf(param);
    	}
        break;
		
        case HTTP_SIG_HEADER_IND:
        {
            HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_HEADER_IND context_id = %d",cnf->context_id);
            result = HandleHttpHeaderInd(param);
        }
        break;
		
        case HTTP_SIG_ERROR_IND:
        {
            HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_ERROR_IND context_id = %d",cnf->context_id);
            result = HandleHttpErrorInd(param);
        }
        break;
	case HTTP_SIG_DELETE_CNF:
	{
            HTTP_DELETE_CNF_SIG_T   *cnf = (HTTP_DELETE_CNF_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_DELETE_CNF context_id = %d",cnf->context_id);
            result = HandleHttpDeleteCnf(param);
	}
	break;

    case HTTP_SIG_CLOSE_CNF:
        {
            HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_CLOSE_CNF context_id = %d",cnf->context_id);
            result = HandleHttpCloseCnf(param);
        }
        break;
		
    case HTTP_SIG_CANCEL_CNF:
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpMsg receive HTTP_SIG_CANCEL_CNF");
        break;
		
    case HTTP_SIG_NET_PROG_IND:
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpMsg receive HTTP_SIG_NET_PROG_IND");
        break;
		
    case HTTP_SIG_REDIRECT_IND:
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpMsg receive HTTP_SIG_REDIRECT_IND");        
        break;
		
    case HTTP_SIG_POST_CNF:
	{
            HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_POST_CNF context_id = %d",cnf->context_id);
            result = HandleHttpPostCnf(param);
	}
       break;

    case HTTP_SIG_PATCH_CNF:
	{
            HTTP_PATCH_CNF_SIG_T *cnf = (HTTP_PATCH_CNF_SIG_T*)original_param;
            if(cnf->context_id != cur_request_para.s_context_id)
            {
                return PBSYNC_ERROR_NOP;
            }
            SCI_TRACE_LOW("[PBSYNC] HTTP_SIG_PATCH_CNF context_id = %d",cnf->context_id);
            result = HandleHttpPatchCnf(param);
	}
       break;
	
#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND");
        break;
#endif
		
    default:
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpMsg Not handle msg_id = %x");
        result = PBSYNC_ERROR_NOP;
        break;
    }
    
	
    return result;
}

/*****************************************************************************/
//  Description : HandlePBSyncAppMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePBSyncAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T   *request_data = PNULL;
    HTTP_APP_INSTANCE_T app_instance_part = 0;
    uint32 remain_count = 0;
    SCI_TRACE_LOW("[PBSYNC] HandlePBSyncAppMsg msg_id = %x", msg_id);
    switch(msg_id)
	{
        //pdp激活
        case MSG_PBSYNC_PDP_ACTIVE_CNF: 
        {
             MMISRV_PBSYNC_CloseWaitingWindow();
             StartHttpRequest();
        }
        break;
        //http会话建立，pb task 有请求发送到等待队列
        case MSG_PBSYNC_HTTP_INIT_CNF:	
        {
            MMIPB_SYNC_HTTP_Request_List_DealOneRequest(&request_data);
            if (PNULL == request_data)
            {
                SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_HTTP_INIT_CNF HTTP_Request_List all request are sent!!!");
                break;
            }
            SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_HTTP_INIT_CNF Do %d opt.", request_data->app_instance);
            app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(request_data->app_instance);
            switch(app_instance_part)
            {
                case PBSYNC_CONTACT_LISTGET:
                    //MMIAPIPBSYNC_SVR_GetAllContacts()
                    result =  ContactSync_GetO365ContactsListData(request_data->app_instance);
                    break;
                case PBSYNC_CONTACT_INFO_GET:
                    //MMIAPIPBSYNC_SVR_GetContactById
                    result =  ContactSync_GetO365ContactInfoData(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id);
                    break;
                case PBSYNC_CONTACT_MODIFYTIME_GET:
                    //MMIAPIPBSYNC_SVR_GetAllIdAndModiTime
                    result =  ContactSync_GetO365ContactModifyTimeData(request_data->app_instance);
                    break;
                case PBSYNC_CONTACT_ADD:
                    //MMIAPIPBSYNC_SVR_AddContact
                    result = PostRequest(request_data->app_instance, &request_data->pbsync_http_param.contact_info);
                    break;
                case PBSYNC_CONTACT_DELETE:
                    //MMIAPIPBSYNC_SVR_DeteteContact
                    result = DeleteRequest(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id);
                    break;
                case PBSYNC_CONTACT_MODIFY:
                    //MMIAPIPBSYNC_SVR_ModifyContact
                    result = PatchRequest(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id, &request_data->pbsync_http_param.contact_info);
                    break;
                case PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME:
                    //MMIAPIPBSYNC_SVR_GetAllAddedContacts
                    result = ContactSync_GetAllAddedContacts(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                    break;
                case PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME:
                    //MMIAPIPBSYNC_SVR_GetAllUpdatedContacts
                    result = ContactSync_GetAllUpdatedContacts (request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                    break;
                case PBSYNC_CONTACT_COUNT_GET:
                    //MMIAPIPBSYNC_SVR_GetTotalCount
                    result = ContactSync_GetTotalCount(request_data->app_instance);
                    break;
                case PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET:
                    //MMIAPIPBSYNC_SVR_GetAllAddedContactsCount
                    result = ContactSync_GetAllAddedContactsCount(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                    break;
                case PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET:
                    //MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount
                    result = ContactSync_GetAllUpdatedContactsCount(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                    break;
                default:
                    break;
            }            
            }
            break;
        case MSG_PBSYNC_CONN_FAILED_CNF:
            MMIPB_SYNC_HTTP_Request_List_DealUnCompletedRequest(&remain_count);
            SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_CONN_FAILED_CNF deal with %d fail request", remain_count);
            while(remain_count > 0)
            {
                if (MMIPB_ERROR_SUCCESS != MMIPB_SYNC_HTTP_Request_List_DealOneFailRequest(&request_data))
                {
                    if (PNULL != request_data)
                    {
                        MMIAPIPBSYNC_PB_RequestFailHandler(request_data->request_id, RESPONSE_NO_CHANCE_RETRY_ERROR);
                        MMIPB_SYNC_HTTP_Request_List_Destroy();
                        HTTP_CloseRequest(cur_request_para.s_context_id);
                        SCI_TRACE_LOW("[PBSYNC] !!! MSG_PBSYNC_CONN_FAILED_CNF call MMIAPIPBSYNC_PB_RequestFailHandler ---1");
                    }
                    else
                    {
                        SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_CONN_FAILED_CNF unknown error");
                    }
                    break;
                }
                if (PNULL == request_data)
                {
                    SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_CONN_FAILED_CNF HTTP_Request_List is empty!!!");
                }
                else
                {
                    SCI_TRACE_LOW("[PBSYNC] MMIPB_SYNC_HTTP_Request_List_DealOneFailRequest requestId %d retry time == %d", request_data->request_id, request_data->pbsync_http_param.http_request_fail_retry_time);
                    if( (request_data->pbsync_http_param.http_request_fail_retry_time == 0) ||(request_data->pbsync_http_param.http_request_response_code == PBSYNC_HTTP_STATUS_CODE_500))
                    {
                        SCI_TRACE_LOW("[PBSYNC] !!! MSG_PBSYNC_CONN_FAILED_CNF call MMIAPIPBSYNC_PB_RequestFailHandler  ---2");
                        MMIAPIPBSYNC_PB_RequestFailHandler(request_data->request_id, RESPONSE_NO_CHANCE_TO_RETRY);
                        MMIPB_SYNC_HTTP_Request_List_Destroy();
                        HTTP_CloseRequest(cur_request_para.s_context_id);
                        break;
                    }
                    MMIPB_SYNC_HTTP_Request_List_Decrease_RetryTimes(request_data->app_instance);
                    SCI_TRACE_LOW("[PBSYNC] MSG_PBSYNC_CONN_FAILED_CNF Do %d opt.", request_data->app_instance);
                    app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(request_data->app_instance);
                    switch(app_instance_part)
                    {
                        case PBSYNC_CONTACT_LISTGET:
                            //MMIAPIPBSYNC_SVR_GetAllContacts()
                            result =  ContactSync_GetO365ContactsListData(request_data->app_instance);
                            break;
                        case PBSYNC_CONTACT_INFO_GET:
                            //MMIAPIPBSYNC_SVR_GetContactById
                            result =  ContactSync_GetO365ContactInfoData(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id);
                            break;
                        case PBSYNC_CONTACT_MODIFYTIME_GET:
                            //MMIAPIPBSYNC_SVR_GetAllIdAndModiTime
                            result =  ContactSync_GetO365ContactModifyTimeData(request_data->app_instance);
                            break;
                        case PBSYNC_CONTACT_ADD:
                            //MMIAPIPBSYNC_SVR_AddContact
                            result = PostRequest(request_data->app_instance, &request_data->pbsync_http_param.contact_info);
                            break;
                        case PBSYNC_CONTACT_DELETE:
                            //MMIAPIPBSYNC_SVR_DeteteContact
                            result = DeleteRequest(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id);
                            break;
                        case PBSYNC_CONTACT_MODIFY:
                            //MMIAPIPBSYNC_SVR_ModifyContact
                            result = PatchRequest(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_id, &request_data->pbsync_http_param.contact_info);
                            break;
                        case PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME:
                            //MMIAPIPBSYNC_SVR_GetAllAddedContacts
                            result = ContactSync_GetAllAddedContacts(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                            break;
                        case PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME:
                            //MMIAPIPBSYNC_SVR_GetAllUpdatedContacts
                            result = ContactSync_GetAllUpdatedContacts (request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                            break;
                        case PBSYNC_CONTACT_COUNT_GET:
                            //MMIAPIPBSYNC_SVR_GetTotalCount
                            result = ContactSync_GetTotalCount(request_data->app_instance);
                            break;
                        case PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET:
                            //MMIAPIPBSYNC_SVR_GetAllAddedContactsCount
                            result = ContactSync_GetAllAddedContactsCount(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                            break;
                        case PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET:
                            //MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount
                            result = ContactSync_GetAllUpdatedContactsCount(request_data->app_instance, request_data->pbsync_http_param.contact_info.o365_modifiedTime);
                            break;
                        default:
                            break;
                    }            
                }
                remain_count--;
            }
            break;

	    	case MSG_PBSYNC_LOGIN_CNF:
		{
			MMIPB_SendSignalToPBTask(MMIPBSYNC_SIG_LOGIN_CNF, 0);
		}
		break;	
			
		case MSG_PBSYNC_PDP_ACTIVE_FAILED_CNF:
		case MSG_PBSYNC_PARSE_FAILED_CNF:
		{
			MMISRV_PBSYNC_CloseWaitingWindow();
			SCI_TRACE_LOW("[PBSYNC] !!! MSG_PBSYNC_PARSE_FAILED_CNF call MMIAPIPBSYNC_PB_RequestFailHandler");
			MMIAPIPBSYNC_PB_RequestFailHandler(0,RESPONSE_PARSER_ERROR);
                    MMIPB_SYNC_HTTP_Request_List_Destroy();
                    HTTP_CloseRequest(cur_request_para.s_context_id);
		}    		   	
		break; 
			
		case MSG_PBSYNC_PARSE_START_IND:
		{
                //StartHttpRequest();
		}
		break;
			
		case MSG_PBSYNC_PARSE_SUCCESS_CNF:
		{
			MMISRV_PBSYNC_CloseWaitingWindow();
		}
		break;
			
		case MSG_PBSYNC_HTTP_CLOSE_CNF:
		{
			MMISRV_PBSYNC_CloseWaitingWindow();
			//PBSYNC_PdpDeactive();shiwei
			s_cur_chunk_end_flag = TRUE;
			s_all_end_flag = TRUE;
		}
	       break;
			
		case MSG_PBSYNC_ALL_CONTACT_CNF://need to imporved
			break;

		case MSG_PBSYNC_CONTACT_DETAIL_CNF://need to imporved
			break;

		case MSG_PBSYNC_ALL_MODITIEM_CNF://need to imporved
			break;

		case MSG_PBSYNC_ADD_CONTACT_CNF://need to imporved
			//通知应用模块
			break;

		case MSG_PBSYNC_DELELE_CONTACT_CNF://need to imporved
			//通知应用模块
			break;	

		case MSG_PBSYNC_MODIFY_CONTACT_CNF://need to imporved
			//通知应用模块
			break;
			
		default:
		{
			SCI_TRACE_LOW("[PBSYNC] HandlePBSyncAppMsg error message is %x", msg_id);
		        //result = APP_ProcError(msg_id);
		        break;			
		}	
		break;
	}
    
	return MMI_RESULT_TRUE;
	
}
/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    SCI_TRACE_LOW("[PBSYNC] ##StartHttpRequest s_contacts_sync_net_id = %d ", s_contacts_sync_net_id);
    if(0 != s_contacts_sync_net_id)
    {
        init_param_ptr->net_id = s_contacts_sync_net_id;
    }
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
    HTTP_InitRequest(MMI_MODULE_CONTACTSYNC, 1, init_param_ptr);
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpInitCnf(DPARAM param)
{
    uint32        result = PBSYNC_ERROR_SUCCESS;
    HTTP_INIT_CNF_SIG_T signal = {0};

    //---1--- 
    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));

    //---2---
    if (MMI_MODULE_CONTACTSYNC != signal.module_id)
    {
    	SCI_TRACE_LOW("[PBSYNC] HandleHttpInitCnf MMI_MODULE_CONTACTSYNC != http_context_ptr->module_id return false");
    	return PBSYNC_ERROR_NOP;
    }

    cur_request_para.s_context_id = signal.context_id;

    if (FALSE == cur_request_para.s_need_retry)
    {
        //wk test
        //send confirm msg to PB MMI 
        //SCI_TRACE_LOW("[PBSYNC] HandleHttpInitCnf send MMIPBSYNC_SIG_LOGIN_CNF");
        //MMIPB_SendSignalToPBTask(MMIPBSYNC_SIG_LOGIN_CNF, 0);
        SCI_TRACE_LOW("[PBSYNC] call MMIAPIPBSYNC_PB_LoginHandler");
        MMIAPIPBSYNC_PB_LoginHandler(0);
    }
    else
    {
        
        if (TRUE == cur_request_para.s_need_refresh_token)
        {
            SCI_TRACE_LOW("[PBSYNC]  RETRY STEP ---1--- HandleHttpInitCnf Refresh token.");
            MMIAPIMSA_refresh_token(MSA_APP_TYPE_CONTACT_SYNC, contact_sync_refresh_token_cb);
            cur_request_para.s_need_refresh_token = FALSE;
        }
        else
        {   
            PBSYNC_SendMsgToApp(MSG_PBSYNC_CONN_FAILED_CNF);//retry current request
            SCI_TRACE_LOW("[PBSYNC] HandleHttpInitCnf send MSG_PBSYNC_CONN_FAILED_CNF MSG");
        }
        cur_request_para.s_need_retry = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpRequestIdInd(DPARAM param)
{
    uint32        error_type = PBSYNC_ERROR_SUCCESS;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};

    GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));

    SCI_TRACE_LOW("[PBSYNC]HandleHttpRequestIdInd: context_id = %d, app_instance = %d, request_id = %d\n", \
                    signal.context_id, signal.app_instance, signal.request_id);
    return error_type;
}

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpDataInd(xSignalHeaderRec* recv_sig_ptr, DPARAM param)
{
    uint32  err_code = PBSYNC_ERROR_NOP;

    HTTP_DATA_IND_SIG_T *signal_ptr = (HTTP_DATA_IND_SIG_T *)recv_sig_ptr;
    HTTP_APP_INSTANCE_T  app_instance = signal_ptr->app_instance;

    char*   new_http_data_ptr = PNULL;
	
    SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: app_instance = %d\n", app_instance);
    SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: incoming datalen = %d\n", signal_ptr->data_len);
	
    s_http_data_cur_size += signal_ptr->data_len;
    SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: s_http_data_cur_size = %d\n", s_http_data_cur_size);
    /*分配buffer，用来读取数据*/
    if (s_http_data_ptr == PNULL)
    {
        s_http_data_ptr = SCI_ALLOC_APPZ(s_http_data_cur_size);
        SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: SCI_ALLOC_APPZ len1 = %d\n", s_http_data_cur_size);
        if (PNULL != s_http_data_ptr)
        {
            SCI_MEMCPY(s_http_data_ptr, signal_ptr->data_ptr, s_http_data_cur_size);
            s_http_data_last_size = s_http_data_cur_size;
        }
        else
        {
            SCI_TRACE_LOW("[PBSYNC] ERROR !! HandleHttpDataInd(): mem alloc 1 failed\n");
            s_http_data_cur_size = 0;
            s_http_data_last_size = 0;
        }
    }
    else
    {
    	//重新分配buffer
    	new_http_data_ptr = SCI_ALLOC_APPZ(s_http_data_cur_size);//概率性assert
    	SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: SCI_ALLOC_APPZ len2 = %d\n", s_http_data_cur_size);
    	if (s_http_data_ptr != PNULL)
    	{
            //将原有的数据保存到新的buffer中
            SCI_MEMCPY(new_http_data_ptr, s_http_data_ptr, s_http_data_last_size);
            //释放old buffer
            SCI_FREE(s_http_data_ptr);
            SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: SCI_FREE old s_http_data_ptr\n");
            //拷贝新收数据到新的buffer中
            SCI_MEMCPY(new_http_data_ptr + s_http_data_last_size, signal_ptr->data_ptr, signal_ptr->data_len);
            s_http_data_last_size = s_http_data_cur_size;
            //当前数据指向新buffer
            s_http_data_ptr = new_http_data_ptr;
    	}
    	else
    	{
            SCI_TRACE_LOW("[PBSYNC] ERROR !! HandleHttpDataInd(): mem alloc 2 failed\n");
            //释放old buffer
            SCI_FREE(s_http_data_ptr);
            s_http_data_cur_size = 0;
            s_http_data_last_size = 0;
    	}
    }	
    return err_code;
}
#if 0
/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpDataInd(xSignalHeaderRec* recv_sig_ptr, DPARAM param)
{
	uint32          err_code = PBSYNC_ERROR_NOP;
	uint32			bytes_to_read = PBSYNC_READ_BUFFER_SIZE;
	HTTP_DATA_IND_SIG_T *signal_ptr = (HTTP_DATA_IND_SIG_T *)recv_sig_ptr;
	HTTP_APP_INSTANCE_T  app_instance = signal_ptr->app_instance;
	char*			old_http_data_ptr = PNULL;
	
	SCI_TRACE_LOW("[PBSYNC]HandleHttpDataInd: app_instance = %d\n", app_instance);
	
	s_http_data_cur_size += signal_ptr->data_len;
	
    while (1)
    {   
		/*分配buffer，用来读取数据*/
		if (s_http_data_ptr == PNULL)
		{
			s_http_data_ptr = SCI_ALLOC_APPZ(bytes_to_read);
			if (!s_http_data_ptr)
			{
				SCI_TRACE_LOW("[PBSYNC] HandleHttpDataInd(): mem alloc 1 failed\n");
				s_http_data_cur_size = 0;
				s_http_data_last_size = 0;
				break;
			}
		}
		//old_http_data_ptr 不等于PNULL，代表需要分配更大的buffer保存网络返回的数据了 
		if (old_http_data_ptr != PNULL)
		{
			//重新分配buffer
			s_http_data_ptr = SCI_ALLOC_APPZ(bytes_to_read);//概率性assert
			if (s_http_data_ptr != PNULL)
			{
				//将原有的数据保存到新的buffer中
				SCI_MEMCPY(s_http_data_ptr, old_http_data_ptr, s_http_data_last_size);
				//释放old buffer
				SCI_FREE(old_http_data_ptr);
			}
			else
			{
				SCI_TRACE_LOW("[PBSYNC] HandleHttpDataInd(): mem alloc 2 failed\n");
				//释放old buffer
				SCI_FREE(old_http_data_ptr);
				s_http_data_cur_size = 0;
				s_http_data_last_size = 0;
				break;
			}
		}
		
		//如果当前要保存的数据大小小于我们分配的buffer，直接累加copy
		if (s_http_data_cur_size < bytes_to_read)
		{
			SCI_MEMCPY(s_http_data_ptr + s_http_data_last_size, signal_ptr->data_ptr, signal_ptr->data_len);
			s_http_data_last_size += signal_ptr->data_len;
			break;
		}
		else //需要重新分配大buffer，保存海量数据 
		{
			old_http_data_ptr = s_http_data_ptr;
			bytes_to_read += PBSYNC_BUFFER_STEP_SIZE;
		}
		
    }	
    return err_code;
}
#endif
/*****************************************************************************/
//  Description : 处理网络返回的数据
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32 err_code = PBSYNC_ERROR_NOP;
    HTTP_APP_INSTANCE_T app_instance_part = 0;
    int i = 0;
    app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(app_instance);
    SCI_TRACE_LOW("[PBSYNC] HandleDataProc: app_instance = %d\n", app_instance);
	
    //保证app_instance 与 保存的MMI的请求相匹配，才做处理
    for (i=0; ;i++)
    {
        if (app_instance_part == s_dataproc_tbl[i].proc_type)
        {
            if (s_dataproc_tbl[i].proc_func && str_ptr)
            {
                err_code = s_dataproc_tbl[i].proc_func(app_instance, str_ptr);
                break;
            }
        }
        else if (PBSYNC_DATAPROC_MAP_END_FLAG == s_dataproc_tbl[i].proc_type)
        {
            break;
        }
    }
	
    return err_code;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpGetCnf(DPARAM param)
{
    uint32 err_code = PBSYNC_ERROR_SUCCESS;
    uint32 responseCode = 0;
    HTTP_GET_CNF_SIG_T signal = {0};
	
    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));
    //http_context_ptr->context_id = signal.context_id;

    SCI_TRACE_LOW("[PBSYNC] HandleHttpGetCnf:app_instance=%d, context_id=%d, result=%d",
    	signal.app_instance, signal.context_id, signal.result);

    if (signal.app_instance == PBSYNC_CONTACT_NO_OPERATION)
    {
        return PBSYNC_ERROR_NOP;
    }

    if(HTTP_SUCCESS != signal.result)
    {
        /* need to be improved */
        err_code = PBSYNC_ERROR_NET_ERROR;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpGetCnf: error! result = %d\n", signal.result);
    }

    MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode(signal.app_instance, &responseCode);
    if ((responseCode >= PBSYNC_HTTP_STATUS_CODE_400) ||(TRUE== MMIPB_SYNC_HTTP_Request_List_IsEmpty()))
    {
        SCI_FREE(s_http_data_ptr);
        s_http_data_ptr = PNULL;
        s_http_data_cur_size = 0;
        s_http_data_last_size = 0;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpGetCnf: app_instance = %d, responseCode = %d\n", signal.app_instance, responseCode);
        if (responseCode == PBSYNC_HTTP_STATUS_CODE_400)
        {
            HTTP_ProcError(signal.app_instance, PBSYNC_ERROR_JSON_ERROR);
        }
        return PBSYNC_ERROR_NOP;
    }
    //也可以通过另外的id
    err_code = HandleDataProc(signal.app_instance, s_http_data_ptr);
    //clear the data
    SCI_FREE(s_http_data_ptr);
    s_http_data_ptr = PNULL;
    s_http_data_cur_size = 0;
    s_http_data_last_size = 0;

    HTTP_ProcError(signal.app_instance, err_code);  
    return PBSYNC_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Handle Http Post Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpPostCnf(DPARAM param)
{
    uint32 err_code = PBSYNC_ERROR_SUCCESS;
    uint32 responseCode = 0;
    HTTP_GET_CNF_SIG_T signal = {0};

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));
    //http_context_ptr->context_id = signal.context_id;

    SCI_TRACE_LOW("[PBSYNC] HandleHttpPostCnf:app_instance=%d, context_id=%d, result=%d",
    	signal.app_instance, signal.context_id, signal.result);

    if (signal.app_instance == PBSYNC_CONTACT_NO_OPERATION)
    {
        return PBSYNC_ERROR_NOP;
    }

    if(HTTP_SUCCESS != signal.result)
    {
        /* need to be improved */
        err_code = PBSYNC_ERROR_NET_ERROR;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpPostCnf: error! result = %d\n", signal.result);
    }

    MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode(signal.app_instance, &responseCode);
    if ((responseCode >= PBSYNC_HTTP_STATUS_CODE_400) ||(TRUE== MMIPB_SYNC_HTTP_Request_List_IsEmpty()))
    {
        SCI_FREE(s_http_data_ptr);
        s_http_data_ptr = PNULL;
        s_http_data_cur_size = 0;
        s_http_data_last_size = 0;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpPostCnf: app_instance = %d, responseCode = %d\n", signal.app_instance, responseCode);
        if (responseCode == PBSYNC_HTTP_STATUS_CODE_400)//bug
        {
            HTTP_ProcError(signal.app_instance, PBSYNC_ERROR_JSON_ERROR);
        }
        return PBSYNC_ERROR_NOP;
    }
    //也可以通过另外的id
    err_code = HandleDataProc(signal.app_instance, s_http_data_ptr);
    //clear the data
    SCI_FREE(s_http_data_ptr);
    s_http_data_ptr = PNULL;
    s_http_data_cur_size = 0;
    s_http_data_last_size = 0;
    HTTP_ProcError(signal.app_instance, err_code);  
    return PBSYNC_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Handle Http Patch Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpPatchCnf(DPARAM param)
{
    uint32 err_code = PBSYNC_ERROR_SUCCESS;
    uint32 responseCode = 0;
    HTTP_GET_CNF_SIG_T signal = {0};
    HTTP_REQUEST_ID_T   requestId = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));
    //http_context_ptr->context_id = signal.context_id;

    SCI_TRACE_LOW("[PBSYNC] HandleHttpPatchCnf:app_instance=%d, request_id=%d, result=%d",
    	signal.app_instance, signal.request_id, signal.result);

    if (signal.app_instance == PBSYNC_CONTACT_NO_OPERATION)
    {
        return PBSYNC_ERROR_NOP;
    }

    if(HTTP_SUCCESS != signal.result)
    {
        /* need to be improved */
        err_code = PBSYNC_ERROR_NET_ERROR;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpPatchCnf: error! result = %d\n", signal.result);
    }

    MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode(signal.app_instance, &responseCode);
    SCI_TRACE_LOW("[PBSYNC] HandleHttpPatchCnf: responseCode = %d\n", responseCode);
    if (((responseCode >= PBSYNC_HTTP_STATUS_CODE_400) && (responseCode != PBSYNC_HTTP_STATUS_CODE_404)) ||(TRUE== MMIPB_SYNC_HTTP_Request_List_IsEmpty()))
    {
        SCI_FREE(s_http_data_ptr);
        s_http_data_ptr = PNULL;
        s_http_data_cur_size = 0;
        s_http_data_last_size = 0;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpPatchCnf: app_instance = %d, responseCode = %d\n", signal.app_instance, responseCode);
        if (responseCode == PBSYNC_HTTP_STATUS_CODE_400)//bug
        {
            HTTP_ProcError(signal.app_instance, PBSYNC_ERROR_JSON_ERROR);
        }
        return PBSYNC_ERROR_NOP;
    }
    //也可以通过另外的id
    err_code = HandleDataProc(signal.app_instance, s_http_data_ptr);
    //clear the data
    SCI_FREE(s_http_data_ptr);
    s_http_data_ptr = PNULL;
    s_http_data_cur_size = 0;
    s_http_data_last_size = 0;
    if (PBSYNC_ERROR_PATCH_DELETED_ITEM == err_code)
    {
        MMIPB_SYNC_HTTP_Request_List_Get_RequestId(signal.app_instance, &requestId);
        MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(signal.app_instance);
        MMIAPIPBSYNC_PB_ModifyContactHandler(requestId, PNULL, 0, TRUE);
        SCI_TRACE_LOW("[PBSYNC] HandleHttpPatchCnf PBSYNC_ERROR_PATCH_DELETED_ITEM");
    }
	
    return err_code;
}
/*****************************************************************************/
//  Description : Handle Http Delete Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpDeleteCnf(DPARAM param)
{
    uint32 responseCode = 0;
    HTTP_GET_CNF_SIG_T signal = {0};
    HTTP_REQUEST_ID_T   requestId = 0;

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));

    SCI_TRACE_LOW("[PBSYNC] HandleHttpDeleteCnf:app_instance=%d, request_id=%d, result=%d",
    	signal.app_instance, signal.request_id, signal.result);

    if (signal.app_instance == PBSYNC_CONTACT_NO_OPERATION)
    {
        return PBSYNC_ERROR_NOP;
    }

    if(HTTP_SUCCESS != signal.result)
    {
        /* need to be improved */
        //err_code = PBSYNC_ERROR_NET_ERROR;
        SCI_TRACE_LOW("[PBSYNC] HandleHttpDeleteCnf: error! result = %d\n", signal.result);
    }

    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsEmpty())
    {
        MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode(signal.app_instance, &responseCode);
        //也可以通过另外的id
        if (PBSYNC_HTTP_STATUS_CODE_204 == responseCode)
        {
            MMIPB_SYNC_HTTP_Request_List_Get_RequestId(signal.app_instance, &requestId);
            MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(signal.app_instance);
            MMIAPIPBSYNC_PB_DeleteContactHandler(requestId, TRUE);
        }
        else if (PBSYNC_HTTP_STATUS_CODE_404 == responseCode)
        {
            MMIPB_SYNC_HTTP_Request_List_Get_RequestId(signal.app_instance, &requestId);
            MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(signal.app_instance);
            MMIAPIPBSYNC_PB_DeleteContactHandler(requestId, TRUE);//bug481489
            //MMIAPIPBSYNC_PB_DeleteContactHandler(requestId, FALSE);
        }
    }
    //clear the data
    SCI_FREE(s_http_data_ptr);
    s_http_data_ptr = PNULL;
    s_http_data_cur_size = 0;
    s_http_data_last_size = 0;
	
    return PBSYNC_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpHeaderInd(DPARAM param)
{
    uint32        result = PBSYNC_ERROR_NOP;
    HTTP_HEADER_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    HTTP_APP_INSTANCE_T app_instance_part = 0;
    uint32 response_code = 0;
    int32 content_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC] Enter HandleHttpHeaderInd()\n");	

    GetSignalStruct(param, &signal, sizeof(HTTP_HEADER_IND_SIG_T));

    request_id = signal.request_id;
    context_id = signal.context_id;
    app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(signal.app_instance);
    response_code = signal.rsp_header_info.response_code;
    content_len = signal.rsp_header_info.content_length;

    SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd response_code=%d, content_len=%d, app_instance=%d", response_code, content_len, signal.app_instance);
    MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode(signal.app_instance, response_code);

    if (PBSYNC_HTTP_STATUS_CODE_200 == response_code
    || PBSYNC_HTTP_STATUS_CODE_201 == response_code)
    {
        SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd: response code 200/201");
    }

    if(PBSYNC_HTTP_STATUS_CODE_400 == response_code)
    {//bug482109
        SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd: para error, response code 400");
        return PBSYNC_ERROR_NOP;
    }

    if((PBSYNC_HTTP_STATUS_CODE_204 == response_code) && (PBSYNC_CONTACT_DELETE == app_instance_part))
    {
        SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd: delete success, response code 204");
    }

    if((PBSYNC_HTTP_STATUS_CODE_404 == response_code) && (PBSYNC_CONTACT_DELETE == app_instance_part))
    {//bug481489
        SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd: delete an inexistent contact, response code 404");
        return PBSYNC_ERROR_NOP;
    }

    if((PBSYNC_HTTP_STATUS_CODE_404 == response_code) && (PBSYNC_CONTACT_MODIFY == app_instance_part))
    {//bug482109
        SCI_TRACE_LOW("[PBSYNC] HandleHttpHeaderInd: modify an inexistent contact, response code 404");
        return PBSYNC_ERROR_NOP;
    }

    if(PBSYNC_HTTP_STATUS_CODE_400 < response_code)
    {
        SCI_TRACE_LOW("[PBSYNC] !!!!! HandleHttpHeaderInd response_code = %d", response_code);
        if (PBSYNC_HTTP_STATUS_CODE_401 == response_code)
        {
            SCI_TRACE_LOW("[PBSYNC]  !!! we need refresh token.");
            cur_request_para.s_need_refresh_token = TRUE;
            //must close current request.
        }
        if (PBSYNC_HTTP_STATUS_CODE_403 == response_code)
        {
            MMIPB_SYNC_HTTP_Request_List_Destroy();
            MMIAPIPBSYNC_PB_RequestFailHandler(request_id, RESPONSE_NOT_PERMITTED);
        }
        SCI_TRACE_LOW("[PBSYNC]  !!! Close request contextid = %d.", cur_request_para.s_context_id);
        HTTP_CloseRequest(cur_request_para.s_context_id);
    }

    return PBSYNC_ERROR_NOP;//return result;
}
/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpErrorInd(DPARAM param)
{
    uint32        err_code = PBSYNC_ERROR_NOP;
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    HTTP_CONTEXT_ID_T       context_id  = 0;
    HTTP_REQUEST_ID_T   requestId = 0;
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    context_id  = signal.context_id;

    SCI_TRACE_LOW("[PBSYNC]HandleHttpErrorInd: app_instance = %d, request_id = %d, result = %d\n",\
                        signal.app_instance, signal.request_id, signal.result);

	
    /* error type classify */
    switch (signal.result)
    {
		case HTTP_SUCCESS:
		{
			/* 不做处理 */
			break;
		}
	
		case HTTP_ERROR_INVALID_INPUT:
		case HTTP_ERROR_FAILED:
		{
		    /* 入参有错误 */
		    err_code = PBSYNC_ERROR_PARAM_ERROR;
		    break;
		}
	
		case HTTP_ERROR_NO_MEMORY:
		{
		    /* 内存不足 */
		    err_code = PBSYNC_ERROR_NOMEMRY;
		    break;
		}

		case HTTP_ERROR_FILE_OPEN:
		case HTTP_ERROR_FILE_WRITE:
		case HTTP_ERROR_FILE_NOT_FOUND:
		case HTTP_ERROR_FILE_DELETE:
		case HTTP_ERROR_FILE_GET_SIZE:
		case HTTP_ERROR_FILE_READ:
		case HTTP_ERROR_FILE_NO_SPACE:
		{
		    /* 文件出错 */
		    err_code = PBSYNC_ERROR_FILE_ERROR;
		    break;
		}

		case HTTP_ERROR_NET_DATA_ERROR:
		case HTTP_ERROR_PROXY_CONNECT:
		case HTTP_ERROR_SERVER_CLOSE:
		case HTTP_ERROR_UNKNOWN:
		case HTTP_ERROR_CONNECT:
		case HTTP_ERROR_HANDSHAKE:
		case HTTP_ERROR_PROXY_HANDSHAKE:
		case HTTP_ERROR_RECV:
		case HTTP_ERROR_SEND:
		{
		    /* 网络连接异常 */
		    err_code = PBSYNC_ERROR_NET_ERROR;
		    break;
		}

		case HTTP_ERROR_TIMEOUT:
		{
		    /* 连接超时，请重试 */
		    err_code = PBSYNC_ERROR_TIMEOUT;
		    break;
		}
		default:
		{
			break;
		}
    }
	
    SCI_TRACE_LOW("[PBSYNC]HandleHttpErrorInd err_code = %x", err_code);
	
	/*出现网络错误，尝试重发，最多三次*/
    if (PBSYNC_ERROR_TIMEOUT == err_code) {
        SCI_TRACE_LOW("[PBSYNC] !!!!! HandleHttpErrorInd PBSYNC_ERROR_TIMEOUT Send request again.");
        //MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode(signal.app_instance, err_code);
        //PBSYNC_SendMsgToApp(MSG_PBSYNC_CONN_FAILED_CNF);bug472323 phone call maybe cause time out.
    }
    if (PBSYNC_ERROR_NET_ERROR == err_code || PBSYNC_ERROR_PARAM_ERROR == err_code ||PBSYNC_ERROR_TIMEOUT == err_code)
    {
        //MMIPB_SYNC_HTTP_Request_List_Decrease_RetryTimes(signal.app_instance);
        if (0 == s_contacts_sync_net_id)
        {//pdp is deactived, reactive it
            SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpErrorInd maybe a phone call in stop sync.");
            //MMISRV_PBSYNC_PdpActive();
            if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_HTTP_Request_List_Get_RequestId(signal.app_instance, &requestId))
            {//bug494139 phone assert when turn off data connection.
                MMIAPIPBSYNC_PB_RequestFailHandler(requestId, RESPONSE_PDP_DEACTIVE_2_ERROR);
                MMIPB_SYNC_HTTP_Request_List_Destroy();
            }
        }
        else
        {
            SCI_TRACE_LOW("[PBSYNC] !!!!! HandleHttpErrorInd HTTP or net work problem, restart HTTP and send again.");
            MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode(signal.app_instance, err_code);
        }
        HTTP_CloseRequest(cur_request_para.s_context_id);
    }

    MMISRV_PBSYNC_CloseWaitingWindow();
	
    return err_code;
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 HandleHttpCloseCnf(DPARAM param)
{
    uint32  result = PBSYNC_ERROR_SUCCESS;
    uint32  retryTimes = 0;
    uint32  responseCode = 0;
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    SCI_TRACE_LOW("[PBSYNC]Enter HandleHttpCloseCnf()\n");	
    //PBSYNC_SendMsgToApp(MSG_PBSYNC_HTTP_CLOSE_CNF);
    SCI_TRACE_LOW("[PBSYNC] HandleHttpCloseCnf s_http_data_next_link memset 0");
    SCI_MEMSET(s_http_data_next_link, 0x00, PBSYNC_NEXT_LINK_MAX_LEN);

    if (FALSE== MMIPB_SYNC_HTTP_Request_List_IsEmpty())
    {
        //HTTP restart.
        SCI_TRACE_LOW("[PBSYNC]SOME request is not success,  restart http. \n");
        cur_request_para.s_need_retry = TRUE;
        StartHttpRequest();
    }
#if 0
    if((0 != responseCode) && (0 < retryTimes))//sync continue
    {
        SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf retry current request. responseCode= %d, retryTimes = %d.", responseCode,  retryTimes);
        switch(responseCode)
        {
            case PBSYNC_ERROR_TIMEOUT:
                //retry http
                SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf PBSYNC_ERROR_TIMEOUT StartHttpRequest.");
                s_need_retry = TRUE;
                StartHttpRequest();
                break;
            case PBSYNC_ERROR_NET_ERROR:
                if (0 == s_contacts_sync_net_id)
                {//pdp is deactived, reactive it
                    SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf start reactive pdp.");
                    MMISRV_PBSYNC_PdpActive();
                }
                else
                {//normal net work problem
                    SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf PBSYNC_ERROR_NET_ERROR StartHttpRequest.");
                    s_need_retry = TRUE;
                    StartHttpRequest();
                }
                break;
            case PBSYNC_HTTP_STATUS_CODE_401:
                SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf refresh token.");
                //token invalid, get new one
                //token refresh.
                //MMIAPIMSA_refresh_token(MSA_APP_TYPE_CONTACT_SYNC ,MMISRV_CAMERAROLL_Get_Token_Func);
                break;
            case PBSYNC_ERROR_PARAM_ERROR:
                SCI_TRACE_LOW("[PBSYNC]  !!! HandleHttpCloseCnf PBSYNC_ERROR_PARAM_ERROR maybe url is Null, please check.");
                break;
        }
    }
#endif
    //成功关闭HTTP
    //clear the http request list here
    return result;
}
/*****************************************************************************/
//  Description : APP容错处理
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E APP_ProcError(HTTP_APP_INSTANCE_T  app_instance,  uint16  msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPB_ERROR_E pbError = MMIPB_ERROR_ERROR;
    HTTP_REQUEST_ID_T   requestId = 0;
    HTTP_APP_INSTANCE_T app_instance_part = 0;
    SCI_TRACE_LOW("[PBSYNC] APP_ProcError app_instance = %d\n", app_instance);
    MMIPB_SYNC_HTTP_Request_List_Get_RequestId(app_instance, &requestId);
    app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(app_instance);

    switch(msg_id)
    {
    	 case MSG_PBSYNC_DATA_ERROR: //need to imporved
        case MSG_PBSYNC_NODATA:
        {//需要考虑加入数量查询的空数据处理。
            MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
            switch(app_instance_part)
            {
                case PBSYNC_CONTACT_LISTGET:
                    pbError = MMIAPIPBSYNC_PB_GetAllContactsHandler(requestId, PNULL, TRUE, TRUE);
                    break;
                case PBSYNC_CONTACT_INFO_GET:
                    //pbError = MMIAPIPBSYNC_PB_GetContactByIdHandler( PNULL);
                    //MMIPB_SYNC_HTTP_Request_List_DeleteByContextId(context_id);
                    break;
                case PBSYNC_CONTACT_MODIFYTIME_GET:
                    pbError = MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler (requestId, PNULL, PNULL, TRUE, TRUE);
                    break;
                case PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME:
                    pbError = MMIAPIPBSYNC_PB_GetAllAddedContactsHandler(requestId, PNULL, TRUE, TRUE);
                    break;
                case PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME:
                    pbError = MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler(requestId, PNULL, TRUE, TRUE);
                    break;
                case PBSYNC_CONTACT_DELETE:
                    MMIAPIPBSYNC_PB_DeleteContactHandler(requestId, FALSE);
                    break;
                case PBSYNC_CONTACT_ADD:
                    MMIAPIPBSYNC_PB_AddContactHandler(requestId, PNULL, 0);
                    break;
                case PBSYNC_CONTACT_MODIFY:
                    MMIAPIPBSYNC_PB_ModifyContactHandler(requestId, PNULL, 0, FALSE);
                    break;
                 default:
                    break;
            }
            break;
        }

        case MSG_PBSYNC_TOKEN_INVALID_ERROR://need to imporved
        {
		//提示token过期
		//MMIPUB_OpenAlertWarningWin(TXT_FBTW_TOKENINVALID);
		//给PB 发送MSG_PBSYNC_TOKEN_INVALID_ERROR 消息
             //MMIFB_SendMsgToWin(MSG_PBSYNC_TOKEN_INVALID_ERROR,  PNULL);
		MMISRV_PBSYNC_CloseWaitingWindow();	
		SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_TOKEN_INVALID_ERROR");
            break;
        }
        case MSG_PBSYNC_INPUT_PARAM_ERROR:
        {
            SCI_TRACE_LOW("[PBSYNC] !!! MSG_PBSYNC_INPUT_PARAM_ERROR call MMIAPIPBSYNC_PB_RequestFailHandler");
            MMIAPIPBSYNC_PB_RequestFailHandler(requestId, RESPONSE_PARSER_ERROR);
            MMIPB_SYNC_HTTP_Request_List_Destroy();
            HTTP_CloseRequest(cur_request_para.s_context_id);
            MMISRV_PBSYNC_CloseWaitingWindow();
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_INPUT_PARAM_ERROR");
            break;
        }
        case MSG_PBSYNC_NOMEMORY://need to imporved
        {  
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_NOMEMORY");
            break;
        }
        case MSG_PBSYNC_FILE_ERROR://need to imporved
        {
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_FILE_ERROR");
            break;
        }
        case MSG_PBSYNC_NET_ERROR://need to imporved
        {
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_NET_ERROR");
            break;
        }
        case MSG_PBSYNC_TIMEOUT_ERROR://need to imporved
        {
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_TIMEOUT_ERROR");
            break;
        }
        case MSG_PBSYNC_TASKBUSY_ERROR://need to imporved
        {
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_TASKBUSY_ERROR");
            break;
        }
        case MSG_PBSYNC_OPERATE_ERROR://need to imporved
        {
            SCI_TRACE_LOW("[PBSYNC] APP_ProcError msg_id MSG_PBSYNC_OPERATE_ERROR");
            break;
        }
        default:
        {
            SCI_TRACE_LOW("[MMIFB] MMIFB_FacebookProcError msg_id UNKNOW_ERROR");
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return (result);
}
/*****************************************************************************/
//  Description : exception handle
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HTTP_ProcError(HTTP_APP_INSTANCE_T  app_instance,  uint32 err_code)
{
#ifdef WIN32
    TCHAR szOutPut[1024] = {0};
    sprintf(szOutPut, "[PBSYNC]Enter HTTP_ProcError(), err_code = %d\n",err_code);
    OutputDebugStr(szOutPut);
#else 
    SCI_TRACE_LOW("[PBSYNC]HTTP_ProcError(), err_code = %d, app_instance = %d\n",\
                    err_code, app_instance);
#endif

    switch(err_code)
    {
        case PBSYNC_ERROR_NOP:
        case PBSYNC_ERROR_SUCCESS:
        {
		/*do nothing*/
            break;
        }
        case PBSYNC_ERROR_JSON_ERROR:
        {
		//ClearFlag();20150821
		APP_ProcError(app_instance, MSG_PBSYNC_DATA_ERROR);
            /* send ERROR code to MMI */
		PBSYNC_SendMsgToApp(MSG_PBSYNC_DATA_ERROR);
            break;
        }
        case PBSYNC_ERROR_JSON_TOKENINVALID:
	{
		//OnTokenUpdate();
		//ClearFlag();20150821
		/* send msg to MMI */           	
		PBSYNC_SendMsgToApp(MSG_PBSYNC_TOKEN_INVALID_ERROR);
		break;
	}
        case PBSYNC_ERROR_PARAM_ERROR:
        {
            //ClearFlag();20150821
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_INPUT_PARAM_ERROR);
            break;
        }
        case PBSYNC_ERROR_NOMEMRY:
        {
            SCI_TRACE_LOW("[PBSYNC]HTTP_ProcError()  PBSYNC_ERROR_NOMEMRY \n");
            //ClearFlag();
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_NOMEMORY);
            break;
        }
        case PBSYNC_ERROR_FILE_ERROR:
        {
            SCI_TRACE_LOW("[PBSYNC]HTTP_ProcError()  PBSYNC_ERROR_FILE_ERROR \n");
            //ClearFlag();
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_FILE_ERROR);
            break;
        }
        case PBSYNC_ERROR_NET_ERROR:
        {
            //ClearFlag();20150821
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_NET_ERROR);
            break;
        }
        case PBSYNC_ERROR_TIMEOUT:
        {
            //ClearFlag();20150821
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_TIMEOUT_ERROR);
            break;
        }
        case PBSYNC_ERROR_NODATA:
        {
            //ClearFlag();
            SCI_TRACE_LOW("[PBSYNC]HTTP_ProcError()  PBSYNC_ERROR_NODATA \n");
            APP_ProcError(app_instance, MSG_PBSYNC_NODATA);
            /* send msg to MMI *///PBSYNC_SendMsgToApp(MSG_PBSYNC_NODATA);
            break;
        }
        case PBSYNC_ERROR_OPERATION_ERROR:
        {
            //ClearFlag();
            SCI_TRACE_LOW("[PBSYNC]HTTP_ProcError()  PBSYNC_ERROR_OPERATION_ERROR \n");
            /* send msg to MMI */
            PBSYNC_SendMsgToApp(MSG_PBSYNC_OPERATE_ERROR);
            break;
        }    

        default:
        {
            //ClearFlag();20150821	
            break;
        }
	}
}
/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}
/*****************************************************************************/
//  Description : PBSYNC_SendMsgToApp
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL void PBSYNC_SendMsgToApp(uint32 sig_id)
{
    MmiSignalS *signal = PNULL;
    
    //creat signal
    MMI_CREATE_SIGNAL(signal,(uint16)sig_id, sizeof(MmiSignalS), P_APP);
	
    //send signal
    MMI_SEND_SIGNAL(signal, P_APP);
}
/*****************************************************************************/
//  Description : 清空标志位  
//  Global resource dependence: 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ClearFlag(void)
{
	SetCurChunkEndFlag(TRUE);
	SetAllEndFlag(TRUE);
}
/*****************************************************************************/
//  Description : SetAllEndFlag
//  Global resource dependence : s_all_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetAllEndFlag(BOOLEAN is_all_end)
{
	s_all_end_flag = is_all_end;
}
/*****************************************************************************/
//  Description : SetCurChunkEndFlag
//  Global resource dependence : s_cur_chunk_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetCurChunkEndFlag(BOOLEAN is_chunk_end)
{
	s_cur_chunk_end_flag = is_chunk_end;
}
/*****************************************************************************/
//  Description : GetAllEndFlag
//  Global resource dependence : s_all_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAllEndFlag(void)
{
	return s_all_end_flag;
}
/*****************************************************************************/
//  Description : GetCurChunkEndFlag
//  Global resource dependence : s_cur_chunk_end_flag
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCurChunkEndFlag(void)
{
	return s_cur_chunk_end_flag;
}
/*****************************************************************************/
//  Description : is token expired
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E JsonParser_IsTokenExpired(cJSON * json_object)
{
	cJSON* error_Object_ptr = PNULL;
	int error_code=0;
	int error_subcode = 0;
	char* error_string = PNULL;
	if (json_object == PNULL) 
	{
		return JSON_STRING_NULL;//null
	}
	
	error_Object_ptr = cJSON_GetObjectItem(json_object,"error");
	
	if(error_Object_ptr != PNULL)
	{
		error_code= cJSON_GetObjectInt(cJSON_GetObjectItem(error_Object_ptr,"code"));
		error_string = cJSON_GetObjectString(cJSON_GetObjectItem(error_Object_ptr, "code"));
		error_subcode=cJSON_GetObjectInt(cJSON_GetObjectItem(error_Object_ptr,"error_subcode"));
		SCI_TRACE_LOW("[PBSYNC]parse error_code=%d,error_subcode=%d",error_code,error_subcode);
		if(0 == MMIAPICOM_StrCmp("ErrorItemNotFound", SCI_STRLEN("ErrorItemNotFound"), (uint8 *)error_string, SCI_STRLEN(error_string)))
		{
			SCI_TRACE_LOW("[PBSYNC]parse error_string=%s", error_string);
			return JSON_ITEM_NOT_FOUND_ERROR;
		}
		else if(error_code==190&&error_subcode==463)
		{
			return JSON_TOKEN_EXPIRED;
		}
		else
		{
			return JSON_REQUEST_ERROR;
		}
	}
	return JSON_TOKEN_VALID;
}

MMI_RESULT_E ContactSync_WSTRNTOSTR(uint8 *pOutput, wchar *unic, size_t maxLen, size_t inputLen)
{
    uint16 pareLen = 0;
    uint16 i = 0;
    if (pOutput == NULL)
    {
    	return -1;
    }
    while(i < inputLen)
    {
        if (pareLen > maxLen)
        {
        	break;
        }
        if ( *unic <= 0x007F )
        {
        	// * U-00000000 - U-0000007F:  0xxxxxxx
        	*(pOutput + pareLen) = (*unic & 0x7F);
        	pareLen += 1;
        }
        else if ( *unic >= 0x0080 && *unic <= 0x07FF )
        {
        	// * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        	*(pOutput + pareLen +1) = (*unic & 0x3F) | 0x80;
        	*(pOutput + pareLen)     = ((*unic >> 6) & 0x1F) | 0xC0;
        	pareLen += 2;
        }
        else if ( *unic >= 0x0800 && *unic <= 0xFFFF )
        {
        	// * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        	*(pOutput + pareLen +2) = (*unic & 0x3F) | 0x80;
        	*(pOutput + pareLen +1) = ((*unic >>  6) & 0x3F) | 0x80;
        	*(pOutput + pareLen)     = ((*unic >> 12) & 0x0F) | 0xE0;
        	pareLen += 3;
        }
        unic++;
        i++;
    }
    *(pOutput + pareLen) = '\0';
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : create cjson, get char data from json
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL char * makeJson(MMIPB_CONTACT_T	*contact_ptr)
{
	cJSON 	*pJsonRoot = NULL;
	uint8   cname_str[MMIPB_NAME_MAX_LEN + 1] = {0};
	uint8  	csurname_str[MMIPB_NAME_MAX_LEN + 1] = {0};
	uint8   cdisplayname_str[MMIPB_NAME_MAX_LEN<<2 + 2] = {0}; 
	#ifndef PB_SUPPORT_LOW_MEMORY
	uint8	cbirth_str[MMIPB_SNS_BIRTH_LEN + 10] = {0};
	#endif
	uint8	cmail_str[MMIPB_MAX_MAIL_LEN + 1] = {0};
	uint8   personal_tel[MMIPB_NUMBER_MAX_STRING_LEN +1] = "\0";
	uint8	num_type  = 0;
	char 	*p = PNULL;
	uint16	i = 0;
	uint16	cur_index = 0;
	cJSON 	*emailArr = PNULL;
	cJSON 	*email1 = PNULL;
	cJSON 	*email2 = PNULL;
	cJSON 	*email3 = PNULL;
	cJSON 	*homeTelArr = PNULL;
	cJSON   *workTelArr = PNULL;
	cJSON 	*number1 = PNULL;
	cJSON 	*number2 = PNULL;
	cJSON 	*number3 = PNULL;
	BOOLEAN is_adnTel_full = FALSE;
	
	if(PNULL == contact_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC] makeJson PNULL == contact_ptr");
		return PNULL;
	}
	
	pJsonRoot = cJSON_CreateObject();
	
	if(NULL == pJsonRoot)
	{
		SCI_TRACE_LOW("[PBSYNC] makeJson NULL == pJsonRoot");
		return PNULL;
	}
	
	//添加字符串
	//GivenName
	if(PNULL !=contact_ptr->name.wstr && 0 !=contact_ptr->name.wstr_len)
	{
            SCI_TRACE_LOW("[PBSYNC] ## makeJson contact_ptr->name.wstr_len %d", contact_ptr->name.wstr_len);
            SCI_TRACE_LOW("[PBSYNC] ## makeJson contact_ptr->name.wstr 0 %d", contact_ptr->name.wstr[0]);
            SCI_TRACE_LOW("[PBSYNC] ## makeJson contact_ptr->name.wstr 1 %d", contact_ptr->name.wstr[1]);
            SCI_TRACE_LOW("[PBSYNC] ## makeJson contact_ptr->name.wstr 2 %d", contact_ptr->name.wstr[2]);
		ContactSync_WSTRNTOSTR(cname_str,
			contact_ptr->name.wstr,
			MMIPB_NAME_MAX_LEN,
			contact_ptr->name.wstr_len
			);
		SCI_TRACE_LOW("[PBSYNC] ## makeJson cname_str %s", cname_str);
		cJSON_AddStringToObject(pJsonRoot, "GivenName", cname_str);
	}
	else if (0 == contact_ptr->name.wstr_len)
	{//bug493814 make "GivenName": null
		SCI_TRACE_LOW("[PBSYNC] ## makeJson contact_ptr->name.wstr_len ==0, add null to GivenName");
		cJSON_AddNullToObject(pJsonRoot, "GivenName");
	}
	
	//Surname
	if(PNULL !=contact_ptr->surname.wstr && 0 !=contact_ptr->surname.wstr_len)
	{
		ContactSync_WSTRNTOSTR(csurname_str,
			contact_ptr->surname.wstr,
			MMIPB_NAME_MAX_LEN,
			contact_ptr->surname.wstr_len
			);
		
		cJSON_AddStringToObject(pJsonRoot, "Surname", csurname_str);
	}
	
	//DisplayName
	if((PNULL !=contact_ptr->name.wstr && 0 !=contact_ptr->name.wstr_len)
		&&(PNULL !=contact_ptr->surname.wstr && 0 !=contact_ptr->surname.wstr_len))
	{
		strcpy(cdisplayname_str,cname_str);//GivenName
		strcat(cdisplayname_str, " ");//空格
		strcat(cdisplayname_str, csurname_str);//Surname
	}
	else if(PNULL !=contact_ptr->surname.wstr && 0 !=contact_ptr->surname.wstr_len)
	{
		strcpy(cdisplayname_str,csurname_str);//Surname
	}
	else if(PNULL !=contact_ptr->name.wstr && 0 !=contact_ptr->name.wstr_len)
	{
		strcpy(cdisplayname_str,cname_str);//GivenName
	}

	if(0 < strlen(cdisplayname_str))
	{
		cJSON_AddStringToObject(pJsonRoot, "DisplayName", cdisplayname_str);
	}
			
#ifndef PB_SUPPORT_LOW_MEMORY
		//ForExample: Birthday=1974-07-22T00:00:00Z
		if(PNULL != contact_ptr->birth.wstr && 0 != contact_ptr->birth.wstr_len)
		{
			MMI_WSTRNTOSTR(cbirth_str,
				MMIPB_SNS_BIRTH_LEN,
				contact_ptr->birth.wstr,
				MMIPB_SNS_BIRTH_LEN,
				contact_ptr->birth.wstr_len
				);
			strcat(cbirth_str, "T00:00:00Z");
			SCI_TRACE_LOW("[PBSYNC] makeJson birth day = %s", cbirth_str);
			cJSON_AddStringToObject(pJsonRoot, "Birthday", cbirth_str);
		}
#endif
		
		//EmailAddress
		//ForExample: "EmailAddresses": [{"Address": "email111@sina.com","Name": "SW (email111@sina.com)"},{"Address": "email222@sina.com","Name": "SW (email222@sina.com)"},null]
		// or : "EmailAddresses":[{"Address":"Mfyx@sina.Com","Name":" "},
#ifdef MMIPB_MAIL_SUPPORT
		//>>> build the property Email
		if(PNULL != contact_ptr->mail.wstr && 0 != contact_ptr->mail.wstr_len)
		{
			MMI_WSTRNTOSTR(cmail_str,
				MMIPB_MAX_MAIL_LEN,
				contact_ptr->mail.wstr,
				MMIPB_MAX_MAIL_LEN,
				contact_ptr->mail.wstr_len
				);		
			cJSON_AddItemToObject(pJsonRoot,"EmailAddresses",emailArr = cJSON_CreateArray());
                    email1=cJSON_CreateObject();
                    if(NULL == email1)
                	{
                		SCI_TRACE_LOW("[PBSYNC] makeJson NULL == email1");
                		return PNULL;
                	}
			cJSON_AddItemToArray(emailArr, email1);
			
			cJSON_AddStringToObject(email1,"Address",cmail_str);
			
			cJSON_AddStringToObject(email1,"Name"," ");

			cur_index++;
		}	
#endif
		
		//follow code used to test EmailAddress
		/*	
		{
			cJSON *emailArr = PNULL;
			cJSON *email1 = PNULL;
			cJSON *email2 = PNULL;
			cJSON *email3 = PNULL;
			
			cJSON_AddItemToObject(pJsonRoot,"EmailAddresses",emailArr = cJSON_CreateArray());
			  
			cJSON_AddItemToArray(emailArr, email1=cJSON_CreateObject());
			
			cJSON_AddStringToObject(email1,"Address","1111");
			  
			cJSON_AddStringToObject(email1,"Name","111@111");
			
			cJSON_AddItemToArray(emailArr, email2=cJSON_CreateObject());
			  
			cJSON_AddStringToObject(email2,"Address","222");
			
			cJSON_AddStringToObject(email2,"Name","222@222");
			  
			cJSON_AddItemToArray(emailArr, email3=cJSON_CreateObject());
				
			cJSON_AddStringToObject(email3,"Address","333");
				  
			cJSON_AddStringToObject(email3,"Name","333@333");							
		}
		*/
		//Number
		//ForExample: "HomePhones": ["33333333333","3"],"BusinessPhones": ["11111111111","1"],"MobilePhone1": "22222222222"
		
		cJSON_AddItemToObject(pJsonRoot,"HomePhones",homeTelArr = cJSON_CreateArray());
		cJSON_AddItemToObject(pJsonRoot,"BusinessPhones",workTelArr = cJSON_CreateArray());
		
		while(i < PBSYNC_MAX_PHONE_NUM)
		{
			if(MMIAPIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton, contact_ptr->number[i].number_len))
			{
				//convert the personal BCD number to number string
				MMIAPICOM_GenDispNumber(((MN_NUMBER_TYPE_E)(contact_ptr->number[i].npi_ton >> 4)),
					(uint8)(contact_ptr->number[i].number_len),
					contact_ptr->number[i].number,
					personal_tel,
					MMIPB_NUMBER_MAX_STRING_LEN);
				//build the property TEL
				num_type = contact_ptr->num_type[i];
				switch(num_type)
				{
				case MMIPB_ADN_NUMBER:
					{	
						//因为o365上MobilePhone1支持1个
						if(is_adnTel_full)
						{
							break;
						}
						
						cJSON_AddStringToObject(pJsonRoot, "MobilePhone1", personal_tel);
						is_adnTel_full = TRUE;
					}
					break;
				case MMIPB_HOME_NUMBER:
					{	
						cJSON  *homeTel = PNULL; 
						//因为o365上HomePhones支持2个
						if(2 <= cJSON_GetArraySize(homeTelArr))
						{
							break;
						}
						
						homeTel = cJSON_CreateString(personal_tel);
						cJSON_AddItemToArray(homeTelArr, homeTel);		
					}
					
					break;
				case MMIPB_OFFICE_NUMBER:
					{
						cJSON  *workTel = PNULL; 
						
						//因为o365上BusinessPhones支持2个
						if(2 <= cJSON_GetArraySize(workTelArr))
						{
							break;
						}
						
						workTel = cJSON_CreateString(personal_tel);
						cJSON_AddItemToArray(workTelArr, workTel);					
					}
					break;
				}
			}
			
			i++;
			
		}
		
		//follow code used to test Number
		/*
		{
			cJSON  *tel = PNULL; 
			cJSON	*numArr = PNULL;
			uint8	j = 0;

			cJSON_AddItemToObject(pJsonRoot,"HomePhones",numArr = cJSON_CreateArray());
			for(j; j<2; j++)
			{		
				//o365上BusinessPhones支持2个
				if(2 <= cJSON_GetArraySize(numArr))
				{
				break;
				}
		
				tel = cJSON_CreateString("123");
				cJSON_AddItemToArray(numArr, tel);									
				cJSON_GetObjectString(numArr);
			}
		}*/

            p = cJSON_PrintUnformatted(pJsonRoot);

            if(NULL == p)
            {
                SCI_TRACE_LOW("[PBSYNC] makeJson convert json list to string faild, exit");
                cJSON_Delete(pJsonRoot);
                return PNULL;
            }
            cJSON_Delete(pJsonRoot);
            return p;
		
}
/*****************************************************************************/
//  Description : Handle Post Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PostRequest(HTTP_APP_INSTANCE_T  app_instance,  MMIPB_CONTACT_T *contact_ptr)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_POST_PARAM_T		*post_param_ptr = PNULL;
	uint32 					len =0;
	char						content_buffer[100] = "";
	char						*p = PNULL;

	SCI_TRACE_LOW("[PBSYNC]PostRequest Entry");
	
	//create json, to generate char string 
	//千万不要忘记释放内存呀，cJSON_Print()函数或者cJSON_PrintUnformatted（）产生的内存，使用free(char *)进行释放
	p = makeJson(contact_ptr);//change shiwei
	
	//---1---
	post_param_ptr = SCI_ALLOCAZ(sizeof(HTTP_POST_PARAM_T));    
	if(PNULL == post_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]PostRequest no memeory to alloca");  
		return MMI_RESULT_FALSE;
	}	
	
	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  
	
	//---2---
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;
	
	//accept
	len = strlen(ACCEPT_STRING);
	post_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(post_param_ptr->accept.accept_ptr,ACCEPT_STRING);
	//accept language
	len = strlen(ACCEPT_LANGUAGE_STRING);
	post_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(post_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
	//accept charset
	len = strlen(ACCEPT_CHARSET_STRING);
	post_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
	strcpy(post_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);
	
	//extern_header
	post_param_ptr->extern_header.header_num = 2;
	post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
	memset(post_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	post_param_ptr->post_body.is_use_file = FALSE;
	post_param_ptr->need_net_prog_ind = TRUE;
	post_param_ptr->is_use_relative_url = TRUE;
	
	//content_type 
	len = strlen(MS_PBSYNC_CONTENT_TYPE_VALUE);
	post_param_ptr->post_body.body_type_ptr = SCI_ALLOC(len + 1);
	memset(post_param_ptr->post_body.body_type_ptr, 0, len + 1);
	strcpy(post_param_ptr->post_body.body_type_ptr, MS_PBSYNC_CONTENT_TYPE_VALUE);
	
	//uri: "https://outlook.office365.com/api/v1.0/me/contacts"
	len=strlen(MS_PBSYNC_URL_PREBASE);
	post_param_ptr->uri.uri_ptr= SCI_ALLOCAZ(len+1);
	strcpy(post_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	
	//post buffer
	//len = strlen(MS_PBSYNC_ADD_CONTENT);
	//strcpy(content_buffer, MS_PBSYNC_ADD_CONTENT, len);
	
    if(PNULL != p)
    {
        post_param_ptr->post_body.u.post_buffer.buffer_ptr = p;//content_buffer;
        post_param_ptr->post_body.u.post_buffer.buffer_len = strlen(p);//strlen(MS_PBSYNC_ADD_CONTENT);
        post_param_ptr->post_body.u.post_buffer.is_copied_by_http = TRUE;
    }
    else
    {
        post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;
    }
	
	//---3---
	//SCI_TRACE_LOW("[PBSYNC]PostRequest entry HTTP_PostRequest post_param_ptr = 0x%x", post_param_ptr);
	HTTP_PostRequest(cur_request_para.s_context_id, post_param_ptr, app_instance);
    	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send PostRequest (Add phone contact to O365) request url = %s", post_param_ptr->uri.uri_ptr);
	
	//---4---
	//---5---

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
	SCI_FREE(post_param_ptr->accept.accept_ptr);
	SCI_FREE(post_param_ptr->accept_language.accept_language_ptr);
	SCI_FREE(post_param_ptr->accept_charset.accept_charset_ptr);
	SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
	SCI_FREE(post_param_ptr->post_body.body_type_ptr);
	post_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(post_param_ptr);
	if(PNULL != p)
	{
		SCI_FREE(p);
	}
	
	return result;
}
/*****************************************************************************/
//  Description : Handle Delete Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DeleteRequest(HTTP_APP_INSTANCE_T  app_instance,  char* O365_id)
{

	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_DELETE_PARAM_T*   	delete_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	
	SCI_TRACE_LOW("[PBSYNC]DeleteRequest Entry");
	
	//---1---
	delete_param_ptr = SCI_ALLOCAZ(sizeof(HTTP_DELETE_PARAM_T));    
	if(PNULL == delete_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]DeleteRequest no memeory to alloca");  
		return MMI_RESULT_FALSE;
	}	
	
	memset(delete_param_ptr,0,sizeof(HTTP_DELETE_PARAM_T));
	//---2---	
	//accept
	len = strlen(ACCEPT_STRING);
	delete_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(delete_param_ptr->accept.accept_ptr,ACCEPT_STRING);
	//accept language
	len = strlen(ACCEPT_LANGUAGE_STRING);
	delete_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(delete_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
	//UA
	len = strlen(USER_AGENT_STRING);
	delete_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(delete_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
	//accept_charset
	len = strlen(ACCEPT_CHARSET_STRING);
	delete_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
	strcpy(delete_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);
		
	//get request get data
	delete_param_ptr->get_data.is_header_use_file = FALSE;
	delete_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;

	//connection
	delete_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	delete_param_ptr->need_net_prog_ind = TRUE;
	delete_param_ptr->is_use_relative_url = TRUE;
	
	//extern_header
	delete_param_ptr->extern_header.header_num = 2;
	delete_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*delete_param_ptr->extern_header.header_num);
	memset(delete_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*delete_param_ptr->extern_header.header_num);
	
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	delete_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(delete_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	delete_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(delete_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[0].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);

	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	delete_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(delete_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	delete_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(delete_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(delete_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);

	//url:"https://outlook.office365.com/api/v1.0/me/contacts/AAMk..dhAAA="
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_SLASH_KEYWORD) + strlen(O365_id);      
	delete_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(delete_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(delete_param_ptr->uri.uri_ptr, MS_PBSYNC_SLASH_KEYWORD);
	strcat(delete_param_ptr->uri.uri_ptr, O365_id);
	
	//content_type 
//	len = strlen(MS_PBSYNC_CONTENT_TYPE_VALUE);
//	delete_param_ptr->delete_body.body_type_ptr = SCI_ALLOC(len + 1);
//	memset(delete_param_ptr->delete_body.body_type_ptr, 0, len + 1);
//	strcpy(delete_param_ptr->delete_body.body_type_ptr, MS_PBSYNC_CONTENT_TYPE_VALUE);
	//---3---
	//SCI_TRACE_LOW("[PBSYNC] ## P--->O DeleteRequest entry HTTP_DeleteRequest delete_param_ptr = 0x%x", delete_param_ptr);
	HTTP_DeleteRequest(cur_request_para.s_context_id, delete_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send DeleteRequest (Delete contact in O365) request url = %s", delete_param_ptr->uri.uri_ptr);
	
	//---4---
	SCI_FREE(delete_param_ptr->accept.accept_ptr);
	SCI_FREE(delete_param_ptr->accept_charset.accept_charset_ptr);
	SCI_FREE(delete_param_ptr->accept_language.accept_language_ptr);
	SCI_FREE(delete_param_ptr->user_agent.user_agent_ptr);
	SCI_FREE(delete_param_ptr->uri.uri_ptr);

	FreeHeader(&(delete_param_ptr->extern_header), delete_param_ptr->extern_header.header_num);
	SCI_FREE(delete_param_ptr->extern_header.other_header_ptr);
	delete_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(delete_param_ptr);
	
	return result;

}
/*****************************************************************************/
//  Description : Handle Patch Request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PatchRequest(HTTP_APP_INSTANCE_T  app_instance,  char* O365_id, MMIPB_CONTACT_T *contact_ptr)
{
	
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_PATCH_PARAM_T		*patch_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	char						*p = PNULL;

	SCI_TRACE_LOW("[PBSYNC]PatchRequest Entry");
	
	//create json, to generate char string 
	//千万不要忘记释放内存呀，cJSON_Print()函数或者cJSON_PrintUnformatted（）产生的内存，使用free(char *)进行释放
	p = makeJson(contact_ptr);
	
	//---1---
	patch_param_ptr = SCI_ALLOCAZ(sizeof(HTTP_PATCH_PARAM_T));    
	if(PNULL == patch_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]PatchRequest no memeory to alloca");  
		return MMI_RESULT_FALSE;
	}	
	
	memset(patch_param_ptr,0,sizeof(HTTP_PATCH_PARAM_T));  

	//---2---
	patch_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	patch_param_ptr->need_net_prog_ind = FALSE;
	
	//accept
	len = strlen(ACCEPT_STRING);
	patch_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(patch_param_ptr->accept.accept_ptr,ACCEPT_STRING);
	//accept language
	len = strlen(ACCEPT_LANGUAGE_STRING);
	patch_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
	strcpy(patch_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
	//accept charset
	len = strlen(ACCEPT_CHARSET_STRING);
	patch_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
	strcpy(patch_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);
	
	//extern_header
	patch_param_ptr->extern_header.header_num = 2;
	patch_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*patch_param_ptr->extern_header.header_num);
	memset(patch_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*patch_param_ptr->extern_header.header_num);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	patch_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(patch_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	patch_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(patch_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[0].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[0].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);

	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	patch_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(patch_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	patch_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(patch_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(patch_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	patch_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	patch_param_ptr->patch_body.is_use_file = FALSE;
	patch_param_ptr->need_net_prog_ind = TRUE;
	patch_param_ptr->is_use_relative_url = TRUE;
	
	//content_type 
	len = strlen(MS_PBSYNC_CONTENT_TYPE_VALUE);
	patch_param_ptr->patch_body.body_type_ptr = SCI_ALLOC(len + 1);
	memset(patch_param_ptr->patch_body.body_type_ptr, 0, len + 1);
	strcpy(patch_param_ptr->patch_body.body_type_ptr, MS_PBSYNC_CONTENT_TYPE_VALUE);
	
	//url:"https://outlook.office365.com/api/v1.0/me/contacts/AAMkA..dhAAA="
	url_length += strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_SLASH_KEYWORD) + strlen(O365_id);      
	patch_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(patch_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(patch_param_ptr->uri.uri_ptr, MS_PBSYNC_SLASH_KEYWORD);
	strcat(patch_param_ptr->uri.uri_ptr, O365_id);
	
	//patch buffer
	//len = strlen(MS_PBSYNC_ADD_CONTENT);
	//strcpy(content_buffer, MS_PBSYNC_ADD_CONTENT, len);
    if(PNULL != p)
    {
        patch_param_ptr->patch_body.u.patch_buffer.buffer_ptr = p;//content_buffer;
        patch_param_ptr->patch_body.u.patch_buffer.buffer_len = strlen(p);//strlen(MS_PBSYNC_ADD_CONTENT);
        patch_param_ptr->patch_body.u.patch_buffer.is_copied_by_http = TRUE;
    }
    else
    {
        patch_param_ptr->patch_body.u.patch_buffer.is_copied_by_http = FALSE;
    }

	//---3---
	//SCI_TRACE_LOW("[PBSYNC]PatchRequest entry HTTP_PatchRequest patch_param_ptr = 0x%x", patch_param_ptr);
	HTTP_PatchRequest(cur_request_para.s_context_id, patch_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send PatchRequest (Patch contact in O365) request url = %s", patch_param_ptr->uri.uri_ptr);

	//---4---
	//---5---	
	SCI_FREE(patch_param_ptr->uri.uri_ptr);
	FreeHeader(&(patch_param_ptr->extern_header), patch_param_ptr->extern_header.header_num);
	SCI_FREE(patch_param_ptr->accept.accept_ptr);
	SCI_FREE(patch_param_ptr->accept_language.accept_language_ptr);
	SCI_FREE(patch_param_ptr->accept_charset.accept_charset_ptr);
	SCI_FREE(patch_param_ptr->patch_body.body_type_ptr);
	SCI_FREE(patch_param_ptr->extern_header.other_header_ptr);
	patch_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(patch_param_ptr);
	if(PNULL != p)
	{
		SCI_FREE(p);
	}
	
	return result;

}
/*****************************************************************************/
//  Description : 请求获取联系人列表数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactsListData(HTTP_APP_INSTANCE_T  app_instance)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint16 					len =0;

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetO365ContactsListData PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	if(0 != SCI_STRLEN(s_http_data_next_link))
	{
		len = SCI_STRLEN(s_http_data_next_link);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		strcpy(get_param_ptr->uri.uri_ptr,s_http_data_next_link);
             SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetO365ContactsListData (Get ALL) s_http_data_next_link = %s", s_http_data_next_link);
	}
	else
	{//uri: "https://outlook.office365.com/api/v1.0/me/contacts"
		//len=strlen(MS_PBSYNC_URL_PREBASE);
		//get more
		len=strlen(MS_PBSYNC_URL_GET_MORE_CONTACTS);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
             //get more
		//strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
		strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_GET_MORE_CONTACTS);
	}
	   
	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetO365ContactsListData (Get ALL) request url = %s", get_param_ptr->uri.uri_ptr);
	
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 请求获取指定联系人数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactInfoData(HTTP_APP_INSTANCE_T  app_instance,  char* contactId)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint16 					len =0;
	uint16					url_length = 0;
	
	if(PNULL == contactId)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetO365ContactInfoData PNULL == contactId");
		return MMI_RESULT_FALSE;
	}
	
	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetO365ContactInfoData PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	//uri:"https://outlook.office365.com/api/v1.0/me/contacts/AAM..AA="
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_SLASH_KEYWORD) + strlen(contactId);
	get_param_ptr->uri.uri_ptr= SCI_ALLOC(url_length+1);
	memset(get_param_ptr->uri.uri_ptr,0,url_length+1);
	strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_SLASH_KEYWORD);
	strcat(get_param_ptr->uri.uri_ptr, contactId);
	   
	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);

	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	
	//---5---
	SCI_TRACE_LOW("[PBSYNC] ## P--->O ContactSync_GetO365ContactInfoData request");
	SCI_TRACE_LOW("[PBSYNC] ## P--->O url = [%s]", get_param_ptr->uri.uri_ptr);

	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 请求获取联系人的最近一次修改时间
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetO365ContactModifyTimeData(HTTP_APP_INSTANCE_T  app_instance)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint16 					len =0;
	uint32					url_length = 0;

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetO365ContactModifyTimeData PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	if(0 != SCI_STRLEN(s_http_data_next_link))
	{
		url_length = SCI_STRLEN(s_http_data_next_link);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(url_length+1);
		memset(get_param_ptr->uri.uri_ptr,0,url_length+1);
		strcpy(get_param_ptr->uri.uri_ptr,s_http_data_next_link);
             SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetO365ContactModifyTimeData(Get ALL id and modiTime) s_http_data_next_link = %s", s_http_data_next_link);
	}
	else
	{//uri: "https://outlook.office365.com/api/v1.0/me/contacts/?$select=DateTimeLastModified"
		//url_length =strlen(MS_PBSYNC_URL_PREBASE);
		//get more
		url_length =strlen(MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//url_length += strlen(MS_PBSYNC_SLASH_KEYWORD) + strlen(MS_PBSYNC_MODITIME_KEYWORD);
		//get more
		url_length += strlen(MS_PBSYNC_AND_MODITIME_KEYWORD);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(url_length+1);
		memset(get_param_ptr->uri.uri_ptr,0,url_length+1);
		//strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
		//strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_SLASH_KEYWORD);
		//get more
		strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_MODITIME_KEYWORD);
		//get more
		strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_AND_MODITIME_KEYWORD);
	}
	   
	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetO365ContactModifyTimeData(Get ALL id and modiTime) request url = %s", get_param_ptr->uri.uri_ptr);
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加的联系人
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllAddedContacts(HTTP_APP_INSTANCE_T  app_instance,  uint32 uiTime)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	//char 					*ctime = "2015-07-21T06:46:40Z";//for test
	char					ctime[MMIPB_SYNC_MODIFIEDTIME_LEN] = "";

	//时间格式转换
	//MS work around suggestion 20151023
	//MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime);
	MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime + 1);

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetAllAddedContacts PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	//url: "https://outlook.office365.com/api/v1.0/me/contacts?$count=true&$filter=DateTimeCreated%20ge%202015-07-21T06:46:40Z"
	if(0 != SCI_STRLEN(s_http_data_next_link))
	{
		url_length = SCI_STRLEN(s_http_data_next_link);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(url_length+1);
		memset(get_param_ptr->uri.uri_ptr,0,url_length+1);
		strcpy(get_param_ptr->uri.uri_ptr,s_http_data_next_link);
	}
	else
	{
		//url_length = strlen(MS_PBSYNC_URL_PREBASE);
		//get more
		url_length = strlen(MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//url_length += strlen(MS_PBSYNC_COUNT_KEYWORD) + strlen("&$filter=DateTimeCreated%20ge%20") + strlen(ctime);
		//get more
		//MS work around suggestion 20151023
		//url_length += strlen(MS_PBSYNC_AND_COUNT_KEYWORD) + strlen("&$filter=DateTimeCreated%20gt%20") + strlen(ctime);
		url_length += strlen(MS_PBSYNC_AND_COUNT_KEYWORD) + strlen("&$filter=DateTimeCreated%20ge%20") + strlen(ctime);
		get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
		//strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
		//get more
		strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);
		//get more
		strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_AND_COUNT_KEYWORD);
		//MS work around suggestion 20151023
		//strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeCreated%20gt%20");
		strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeCreated%20ge%20");
		strcat(get_param_ptr->uri.uri_ptr, ctime);
	}

	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetAllAddedContacts (Get ALL Added) request url = %s", get_param_ptr->uri.uri_ptr);
	
	//---5---		
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改的联系人
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllUpdatedContacts(HTTP_APP_INSTANCE_T  app_instance,  uint32 uiTime)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	//char 					*ctime = "2015-07-21T06:50:41Z";//for test
	char					ctime[MMIPB_SYNC_MODIFIEDTIME_LEN] = "";

	//时间格式转换
	//MS work around suggestion 20151023
	//MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime);
	MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime + 1);

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetAllUpdatedContacts PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	//url:"https://outlook.office365.com/api/v1.0/me/contacts?$count=true&$filter=DateTimeLastModified%20ge%202015-07-21T06:50:41Z%20and%20DateTimeCreated%20lt%202015-07-21T06:50:41Z"
	if(0 != SCI_STRLEN(s_http_data_next_link))
	{
		url_length = SCI_STRLEN(s_http_data_next_link);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(url_length+1);
		memset(get_param_ptr->uri.uri_ptr,0,url_length+1);
		strcpy(get_param_ptr->uri.uri_ptr,s_http_data_next_link);
	}
	else
	{
		//url_length = strlen(MS_PBSYNC_URL_PREBASE);
             //get more
             url_length = strlen(MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//url_length += strlen(MS_PBSYNC_COUNT_KEYWORD);
		//get more
             url_length += strlen(MS_PBSYNC_AND_COUNT_KEYWORD);
		//MS work around suggestion 20151023
		//url_length += strlen("&$filter=DateTimeLastModified%20gt%20") + strlen(ctime) + strlen("%20and%20DateTimeCreated%20le%20") + strlen(ctime);
		url_length += strlen("&$filter=DateTimeLastModified%20ge%20") + strlen(ctime) + strlen("%20and%20DateTimeCreated%20lt%20") + strlen(ctime);       
		get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
		//strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
		//get more
		strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_GET_MORE_CONTACTS);
		//strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);
		//get more
		strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_AND_COUNT_KEYWORD);
		//MS work around suggestion 20151023
		//strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20gt%20");
		strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20ge%20");
		strcat(get_param_ptr->uri.uri_ptr, ctime);
		//MS work around suggestion 20151023
		//strcat(get_param_ptr->uri.uri_ptr, "%20and%20DateTimeCreated%20le%20");
		strcat(get_param_ptr->uri.uri_ptr, "%20and%20DateTimeCreated%20lt%20");
		strcat(get_param_ptr->uri.uri_ptr, ctime);
	}

	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetAllUpdatedContacts (Get ALL Updated) request url = %s", get_param_ptr->uri.uri_ptr);
	
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 查询Office365当前联系人总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetTotalCount(HTTP_APP_INSTANCE_T  app_instance)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;	

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetTotalCount PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	//url:"https://outlook.office365.com/api/v1.0/me/contacts?$count=true"
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_COUNT_KEYWORD);
	get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);

	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetTotalCount (Get ALL Total Count) request url = %s", get_param_ptr->uri.uri_ptr);
	
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllAddedContactsCount(HTTP_APP_INSTANCE_T  app_instance,  uint32 uiTime)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	//char 					*ctime = "2015-07-21T06:46:40Z";//for test
	char					ctime[MMIPB_SYNC_MODIFIEDTIME_LEN] = "";

	//时间格式转换
	//MS work around suggestion 20151023
	//MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime);
	MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime + 1);

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetAllAddedContactsCount PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	
	//url:"https://outlook.office365.com/api/v1.0/me/contacts?$count=true&$filter=DateTimeCreated%20ge%202015-07-21T06:46:40Z"
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//url_length += strlen("&$filter=DateTimeCreated%20gt%20") + strlen(ctime);
	url_length += strlen("&$filter=DateTimeCreated%20ge%20") + strlen(ctime); 
	get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeCreated%20gt%20");
	strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeCreated%20ge%20");
	strcat(get_param_ptr->uri.uri_ptr, ctime);

	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetAllAddedContactsCount (Get ALL Added Count) request url = %s", get_param_ptr->uri.uri_ptr);	
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改联系人的总数
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:uiTime 指定的时间
/*****************************************************************************/
LOCAL MMI_RESULT_E ContactSync_GetAllUpdatedContactsCount(HTTP_APP_INSTANCE_T  app_instance,  uint32 uiTime)
{
	MMI_RESULT_E        		result = MMI_RESULT_TRUE;
	HTTP_GET_PARAM_T		*get_param_ptr = PNULL;
	uint32 					len =0;
	uint32					url_length = 0;
	//char 					*ctime = "2015-07-21T06:46:40Z";//for test
	char					ctime[MMIPB_SYNC_MODIFIEDTIME_LEN] = "";

	//时间格式转换
	//MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime);
	//MS work around suggestion 20151023
	MMIPB_SYNC_UintTimeToO365Str(ctime, uiTime + 1);

	//---3---
	get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
	if(PNULL == get_param_ptr)
	{
		SCI_TRACE_LOW("[PBSYNC]ContactSync_GetAllUpdatedContactsCount PNULL == get_param_ptr return false");
		return MMI_RESULT_FALSE;
	}
	SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
#if 0	
	//url:"https://outlook.office365.com/api/v1.0/me/contacts?$count=true&$filter=DateTimeCreated%20ge%202015-07-21T06:46:40Z"
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//url_length += strlen("&$filter=DateTimeLastModified%20gt%20") + strlen(ctime);
	url_length += strlen("&$filter=DateTimeLastModified%20ge%20") + strlen(ctime);
	get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20gt%20");
	strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20ge%20");
	strcat(get_param_ptr->uri.uri_ptr, ctime);
#endif
	url_length = strlen(MS_PBSYNC_URL_PREBASE);
	url_length += strlen(MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//url_length += strlen("&$filter=DateTimeLastModified%20gt%20") + strlen(ctime) + strlen("%20and%20DateTimeCreated%20le%20") + strlen(ctime);
	url_length += strlen("&$filter=DateTimeLastModified%20ge%20") + strlen(ctime) + strlen("%20and%20DateTimeCreated%20lt%20") + strlen(ctime);       
	get_param_ptr->uri.uri_ptr = (char *) SCI_ALLOC_APPZ(url_length+1);
	strcpy(get_param_ptr->uri.uri_ptr,MS_PBSYNC_URL_PREBASE);
	strcat(get_param_ptr->uri.uri_ptr, MS_PBSYNC_COUNT_KEYWORD);
	//MS work around suggestion 20151023
	//strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20gt%20");
	strcat(get_param_ptr->uri.uri_ptr, "&$filter=DateTimeLastModified%20ge%20");
	strcat(get_param_ptr->uri.uri_ptr, ctime);
	//MS work around suggestion 20151023
	//strcat(get_param_ptr->uri.uri_ptr, "%20and%20DateTimeCreated%20le%20");
	strcat(get_param_ptr->uri.uri_ptr, "%20and%20DateTimeCreated%20lt%20");
	strcat(get_param_ptr->uri.uri_ptr, ctime);

	//extern_header
	get_param_ptr->extern_header.header_num = MS_PBSYNC_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	//accept
	len=strlen(MS_PBSYNC_ACCEPT_NAME);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_PBSYNC_ACCEPT_NAME);
	
	len=strlen(MS_PBSYNC_ACCEPT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,MS_PBSYNC_ACCEPT_VALUE);
	//user_agent
	len=strlen(MS_PBSYNC_USER_AGENT_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_PBSYNC_USER_AGENT_NAME);
	
	len=strlen(MS_PBSYNC_USER_AGENT_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_PBSYNC_USER_AGENT_VALUE);
	//authorization
	len=strlen(MS_PBSYNC_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_PBSYNC_AUTHORIZATION);
	
	len=strlen(MS_PBSYNC_ACCESSTOKEN) + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_TOKEN_BACK_STRING);
	get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr, MS_PBSYNC_TOKEN_FRONT_STRING);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING), MS_PBSYNC_ACCESSTOKEN);
	strcpy(get_param_ptr->extern_header.other_header_ptr[2].header_value_ptr + strlen(MS_PBSYNC_TOKEN_FRONT_STRING) + strlen(MS_PBSYNC_ACCESSTOKEN), MS_PBSYNC_TOKEN_BACK_STRING);
	//client_id
	len=strlen(MS_PBSYNC_CLIENT_ID_NAME);
	get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_PBSYNC_CLIENT_ID_NAME);
	
	len=strlen(MS_PBSYNC_CLIENT_ID_VALUE);
	get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_PBSYNC_CLIENT_ID_VALUE);
	
	//---4---
	HTTP_GetRequest(cur_request_para.s_context_id, get_param_ptr, app_instance);
	SCI_TRACE_LOW("[PBSYNC] ## P--->O Send ContactSync_GetAllUpdatedContactsCount (Get ALL Updated Count) request url = %s", get_param_ptr->uri.uri_ptr);	
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);
	
	return result;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_LISTGET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactListDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactListDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseListData(app_instance, str_ptr);	

    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }	

    SCI_TRACE_LOW("[PBSYNC] OnO365ContactListDataProc json_result = %d, err_code = %d", json_result, err_code);
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_INFO_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactInfoDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactInfoDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseInfoData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的最近一次修改时间的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_MODIFYTIME_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactModifyTimeDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactModifyTimeDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseAllModifyTimeData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_ITEM_NOT_FOUND_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_PATCH_DELETED_ITEM;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_ADD的网络返回的数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactAddDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseInfoData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络响应 CONTACTSYNC_CONTACT_DELETE 消息返回的数据
//  Global resource dependence: 
//  Author: Hongjun.Jia
//  Note: 
/*****************************************************************************/
LOCAL uint32 OnO365ContactDeleteDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32      err_code = PBSYNC_ERROR_NOP;
    
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactDeleteDataProc()\n");    
    
    if (RESPONSE_SUCCESS != JsonParser_ParseResponse(str_ptr))
    {
        err_code = PBSYNC_ERROR_OPERATION_ERROR;
        SCI_TRACE_LOW("[PBSYNC]OnO365ContactDeleteDataProc:unknown error!\n");
        return err_code;
    }
	
    //PBSYNC_SendMsgToApp(MSG_PBSYNC_DELELE_CONTACT_CNF);
    //MMIAPIPBSYNC_PB_DeleteContactHandler();//keep going
    err_code = PBSYNC_ERROR_NODATA;
    return err_code;  
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为CONTACTSYNC_CONTACT_MODIFY的网络返回的数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactModifyDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactModifyDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseInfoData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_ITEM_NOT_FOUND_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_PATCH_DELETED_ITEM;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddedDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactAddedDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseListData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactUpdatedDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactUpdatedDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseListData(app_instance, str_ptr);
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAllCountDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactAllCountDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseCountData(app_instance,  str_ptr);	
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}	
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactAddedCountDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactAddedCountDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseCountData(app_instance,  str_ptr);	
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}	
/*****************************************************************************/
//  Description : 处理网络返回的信息列表的数据
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note:处理app_instance 为PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET的网络数据
/*****************************************************************************/
LOCAL uint32 OnO365ContactUpdatedCountDataProc(HTTP_APP_INSTANCE_T app_instance,  const char* str_ptr)
{
    uint32          err_code = PBSYNC_ERROR_NOP;
    uint32          json_result = JSON_PARSER_SUCCESS;
    int16           i =0;
    uint32          limit = 0;
    uint32          offset = 0;
    uint32          start_index = 0;
    uint32*         buffer = PNULL;
    uint32          first_slot = 0;
    uint32          cache_len = 0;
	
    SCI_TRACE_LOW("[PBSYNC]Enter OnO365ContactUpdatedCountDataProc()\n");
	
    json_result = ContactSync_JsonParser_ParseCountData(app_instance, str_ptr);	
    if (JSON_PARSER_EMPTY == json_result)
    {
        err_code = PBSYNC_ERROR_NODATA;
    }
    else if (JSON_TOKEN_EXPIRED == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_TOKENINVALID;
    }
    else if (JSON_REQUEST_ERROR == json_result || JSON_STRING_NULL == json_result || JSON_PARSER_ERROR == json_result || JSON_PARSER_OTHER_ERROR == json_result)
    {
        err_code = PBSYNC_ERROR_JSON_ERROR;
    }
	
    return err_code;
	
}	
/*****************************************************************************/
//  Description : parser list data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseListData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string)
{
    JSON_PARSER_CODE_E result = JSON_PARSER_MAX;
    cJSON *root_ptr = PNULL;
	
    if (json_string == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseListData response is NULL"); 
        return JSON_STRING_NULL;//null
    }
	
    root_ptr = cJSON_Parse(json_string);
    
    if (root_ptr == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseListData parse error"); 
        return JSON_PARSER_ERROR;//empty string, not complete
    }	
    result = JsonParser_IsTokenExpired(root_ptr);
    
    if(result == JSON_TOKEN_VALID)
    {
        result = JsonParser_ParseContactListJsonObject(app_instance, root_ptr);
    }
    
    SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseListData parse result=%d",result); 
    
    cJSON_Delete(root_ptr);
    return result;
}
/*****************************************************************************/
//  Description : parser info data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseInfoData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string)
{
    JSON_PARSER_CODE_E result = JSON_PARSER_MAX;
    cJSON *root_ptr = PNULL;
	
    if (json_string == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseInfoData response is NULL"); 
        return JSON_STRING_NULL;//null
    }
	
    root_ptr = cJSON_Parse(json_string);
    
    if (root_ptr == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseInfoData parse error"); 
        return JSON_PARSER_ERROR;//empty string, not complete
    }
    
    result = JsonParser_IsTokenExpired(root_ptr);

    if(result == JSON_ITEM_NOT_FOUND_ERROR)
    {
        cJSON_Delete(root_ptr);
        return result;
    }
    
    if(result == JSON_TOKEN_VALID)
    {
        result = JsonParser_ParseAContactJsonObject(app_instance, root_ptr);
    }
    
    SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseInfoData parse result=%d",result); 
    
    cJSON_Delete(root_ptr);
    return result;
}
/*****************************************************************************/
//  Description : parser all modify time data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseAllModifyTimeData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string)
{
    JSON_PARSER_CODE_E result = JSON_PARSER_MAX;
    cJSON *root_ptr = PNULL;
	
    if (json_string == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseAllModifyTimeData response is NULL"); 
        return JSON_STRING_NULL;//null
    }
	
    root_ptr = cJSON_Parse(json_string);
    
    if (root_ptr == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseAllModifyTimeData parse error"); 
        return JSON_PARSER_ERROR;//empty string, not complete
    }	
    result = JsonParser_IsTokenExpired(root_ptr);
    
    if(result == JSON_TOKEN_VALID)
    {
        result = JsonParser_ParseAllModifyTimeJsonObject(app_instance, root_ptr);
    }
    
    SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseAllModifyTimeData parse result=%d",result); 
    
    cJSON_Delete(root_ptr);
    return result;
}
/*****************************************************************************/
//  Description : parser count data
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E ContactSync_JsonParser_ParseCountData(HTTP_APP_INSTANCE_T app_instance,  const char *json_string)
{
    JSON_PARSER_CODE_E result = JSON_PARSER_MAX;
    cJSON *root_ptr = PNULL;
	
    if (json_string == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseCountData response is NULL"); 
        return JSON_STRING_NULL;//null
    }
	
    root_ptr = cJSON_Parse(json_string);
    
    if (root_ptr == PNULL) 
    {
        SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseCountData parse error"); 
        return JSON_PARSER_ERROR;//empty string, not complete
    }	
    result = JsonParser_IsTokenExpired(root_ptr);
    
    if(result == JSON_TOKEN_VALID)
    {
        result = JsonParser_ParseCountJsonObject(app_instance,  root_ptr);
    }
    
    SCI_TRACE_LOW("[PBSYNC]ContactSync_JsonParser_ParseCountData parse result=%d",result); 
    
    cJSON_Delete(root_ptr);
    return result;
}

/*****************************************************************************/
//  Description : ParseContactListJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E JsonParser_ParseContactListJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_object)
{
	int i = 0;
	cJSON *json_array_ptr = PNULL;
	uint16  json_array_size = 0;
	cJSON *json_item_ptr = PNULL;
	char * next_link = PNULL;      
	
	if(json_object == PNULL)
	{
		return JSON_STRING_NULL;
	}
	
	json_array_ptr = cJSON_GetObjectItem(json_object, "value");//解析出json数组
	
	if(json_array_ptr == PNULL )
	{	
		return JSON_PARSER_ERROR;
	}
	
	json_array_size = (uint16)cJSON_GetArraySize(json_array_ptr);
	
	if(json_array_size == 0 )
	{	
		SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseContactListJsonObject contact list is empty"); 
		return JSON_PARSER_EMPTY;
		
	}
		
	next_link = cJSON_GetObjectString(cJSON_GetObjectItem(json_object, "@odata.nextLink"));
	SCI_MEMSET(s_http_data_next_link, 0x00, PBSYNC_NEXT_LINK_MAX_LEN);
	if ((NULL != next_link) && (0 != SCI_STRLEN(next_link)))
	{
	    if (PBSYNC_NEXT_LINK_MAX_LEN < SCI_STRLEN(next_link))
	    {
	        SCI_TRACE_LOW("[PBSYNC] ERROR !!! JsonParser_ParseContactListJsonObject next_link maybe too long");
	    }
	    else
	    {
	        SCI_MEMCPY(s_http_data_next_link, next_link, SCI_STRLEN(next_link));
	    }
	}
        
	SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseContactListJsonObject s_http_data_next_link =%s", s_http_data_next_link);
		
	for (i = 0; i < json_array_size; i++)
	{
		json_item_ptr = cJSON_GetArrayItem(json_array_ptr, i);
		
		if(json_item_ptr != PNULL)
		{
			//设置当前数据包处理状态
			//cur chunk has finished
			if(i == (json_array_size - 1))
			{
				//set cur chunk end flag
				SetCurChunkEndFlag(TRUE);
			}
			//cur chunk has not finished
			else
			{
				SetCurChunkEndFlag(FALSE);
			}

			//设置所有数据包处理状态
			// 处理到最后一个数据块的最后一条信息时置为TRUE
			if((0 == SCI_STRLEN(s_http_data_next_link)) && GetCurChunkEndFlag())
			{
				ClearFlag();//clear flag will SetAllEndFlag(TRUE);
				SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseContactListJsonObject s_http_data_next_link = NULL, Clear flag");
			}
			else
			{
				SetAllEndFlag(FALSE);
			}

			SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseContactListJsonObject s_cur_chunk_end_flag=%d, s_all_end_flag=%d", s_cur_chunk_end_flag, s_all_end_flag);
			
			JsonParser_ParseAContactJsonObject(app_instance, json_item_ptr);		
		}		
	}
	
	return JSON_PARSER_SUCCESS;
}

LOCAL MMI_RESULT_E ContactSync_NumberFilter(char *inputNum, char *outNum)
{
    uint32 	i = 0;
    if (PNULL == outNum)
    {
        SCI_TRACE_LOW("[PBSYNC] !!!ContactSync_NumberFilter outNum == NULL.");
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW("[PBSYNC] ContactSync_NumberFilter inputNum len is %d.", SCI_STRLEN(inputNum));
    //SCI_TRACE_LOW("[PBSYNC] ContactSync_NumberFilter inputNum is %s.", inputNum);
    while (*inputNum != '\0')
    {
        if((*inputNum >= '0' && *inputNum <= '9')||*inputNum == '*'||
            *inputNum == '#'||(*inputNum == 'P'||*inputNum == 'p')||
            *inputNum == 'W'||*inputNum == 'w'||*inputNum=='+')
        {
            *(outNum + i) = *inputNum;
            //SCI_TRACE_LOW("[PBSYNC] ContactSync_NumberFilter outNum is %c.", *outNum);
            i++;
            if (i > MMIPB_BCD_NUMBER_MAX_LEN - 1)
            {
                break;
            }
        }
        inputNum++;
    }
    *(outNum + i) = '\0';
    //SCI_TRACE_LOW("[PBSYNC] ContactSync_NumberFilter SCI_STRLEN outNum is %d.", SCI_STRLEN(outNum));
    SCI_TRACE_LOW("[PBSYNC] ContactSync_NumberFilter outNum is %s.", outNum);
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E ContactSync_EmailFilter(char *inputString, wchar *outString)
{
    uint32 	i = 0;
    if (PNULL == outString)
    {
        SCI_TRACE_LOW("[PBSYNC] !!!ContactSync_EmailFilter outNum == NULL.");
        return MMI_RESULT_FALSE;
    }

    while(*inputString != '\0')   /* copy string */
    {
        *(outString + i) = *inputString;
        i++;
        if (i > MMIPB_MAX_MAIL_LEN - 1)
        {
            break;
        }
        inputString++;
    }
    *(outString + i) = '\0';
    SCI_TRACE_LOW("[PBSYNC] ContactSync_EmailFilter outString is %s.", outString);
    return MMI_RESULT_TRUE;
}

int32 ContactSync_enc_get_utf8_size(uint8 pInput)
{
	uint8 c = pInput;
	// 0xxxxxxx 返回0
	// 10xxxxxx 不存在
	// 110xxxxx 返回2
	// 1110xxxx 返回3
	// 11110xxx 返回4
	// 111110xx 返回5
	// 1111110x 返回6
	if(c< 0x80) return 0;
	if(c>=0x80 && c<0xC0) return -1;
	if(c>=0xC0 && c<0xE0) return 2;
	if(c>=0xE0 && c<0xF0) return 3;
	if(c>=0xF0 && c<0xF8) return 4;
	if(c>=0xF8 && c<0xFC) return 5;
	if(c>=0xFC) return 6;
}

/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int32 ContactSync_enc_utf8_to_unicode(uint8* pInput, uint16 inputLen, wchar *Unic, uint16 *outputlen)
{
    int16 utfbytes = 0;
    int16 pareLen = 0;
    wchar onewstr = 0;
    uint8 *pOutput = PNULL;
    int16 i = 0;
    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    uint8 b1, b2, b3, b4, b5, b6;
    if (pInput == NULL || Unic == NULL)
    {
    	return -1;
    }

    while(pareLen < inputLen)
    {
        utfbytes = ContactSync_enc_get_utf8_size(*(pInput + pareLen));
        onewstr = 0;
        pOutput = (uint8 *) &onewstr;
        switch ( utfbytes )
        {
        	case 0:
        		*pOutput     = *(pInput + pareLen);
        		utfbytes    += 1;
        		break;
        	case 2:
        		b1 = *(pInput + pareLen);
        		b2 = *(pInput + pareLen + 1);
        		if ( (b2 & 0xE0) != 0x80 )
        			return 0;
        		*pOutput     = (b1 << 6) + (b2 & 0x3F);
        		*(pOutput+1) = (b1 >> 2) & 0x07;
        		break;
        	case 3:
        		b1 = *(pInput + pareLen);
        		b2 = *(pInput + pareLen + 1);
        		b3 = *(pInput + pareLen + 2);
        		if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
        			return 0;
        		*pOutput     = (b2 << 6) + (b3 & 0x3F);
        		*(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
        		break;
        	/*case 4:
        		b1 = *(pInput + pareLen);
        		b2 = *(pInput + pareLen + 1);
        		b3 = *(pInput + pareLen + 2);
        		b4 = *(pInput + pareLen + 3);
        		if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
        			|| ((b4 & 0xC0) != 0x80) )
        			return 0;
        		*pOutput     = (b3 << 6) + (b4 & 0x3F);
        		*(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
        		*(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
        		break;
        	case 5:
        		b1 = *(pInput + pareLen);
        		b2 = *(pInput + pareLen + 1);
        		b3 = *(pInput + pareLen + 2);
        		b4 = *(pInput + pareLen + 3);
        		b5 = *(pInput + pareLen + 4);
        		if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
        			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
        			return 0;
        		*pOutput     = (b4 << 6) + (b5 & 0x3F);
        		*(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
        		*(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
        		*(pOutput+3) = (b1 << 6);
        		break;
        	case 6:
        		b1 = *(pInput + pareLen);
        		b2 = *(pInput + pareLen + 1);
        		b3 = *(pInput + pareLen + 2);
        		b4 = *(pInput + pareLen + 3);
        		b5 = *(pInput + pareLen + 4);
        		b6 = *(pInput + pareLen + 5);
        		if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
        			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
        			|| ((b6 & 0xC0) != 0x80) )
        			return 0;
        		*pOutput     = (b5 << 6) + (b6 & 0x3F);
        		*(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
        		*(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
        		*(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
        		break;*/
        	default:
        		return 0;
        		break;
        }
        pareLen += utfbytes;
        *(Unic + i) = onewstr;
        SCI_TRACE_LOW("[PBSYNC] ContactSync_enc_utf8_to_unicode unicode = %x\n", onewstr);
        i++;
        if (i > MMIPB_NAME_MAX_LEN-1)
        {
            SCI_TRACE_LOW("[PBSYNC] ContactSync_enc_utf8_to_unicode MMIPB_NAME_MAX_LEN");
            break;
        }
    }
    *outputlen = i;

    return utfbytes;
}

/*****************************************************************************/
//  Description : ParseAContactJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAContactJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj) 
{
	
	MMISRV_CONTACT_T contact_t = {0};
	MMIPB_ERROR_E pbError = MMIPB_ERROR_ERROR;
	uint32 	i = 0;
	cJSON 	*json_array_ptr = PNULL;
	uint16  json_array_size = 0;
	cJSON 	*json_item_ptr = PNULL;
	char 	*id = PNULL;
	char 	*email_name = PNULL;
	char 	*email_address = PNULL;
	char	*dateTimeCreated = PNULL;
	char    *dateTimeLastModified = PNULL;
	char    *birthday = PNULL;
	char	*displayName = PNULL;
	char 	*givenname = PNULL;
	char 	*surname = PNULL;
	char	*im = PNULL;
	char	*jobTitle = PNULL;
	char	*companyName = PNULL;
	char	*department = PNULL;
	char	*officeLocation = PNULL;
	char	*profession = PNULL;
	char	*businessHomePage = PNULL;
	char	*assistantName = PNULL;
	char	*manager = PNULL;
	char	*homePhone = PNULL;
	char	*homePhone2 = PNULL;
	char	*businessPhone = PNULL;
	char	*businessPhone2 = PNULL;
	char	*mobilePhone = PNULL;
	char FilteredNum[MMIPB_BCD_NUMBER_MAX_LEN] = "";
	uint8	cur_index = 0;
	HTTP_REQUEST_ID_T   requestId = 0;
	HTTP_APP_INSTANCE_T app_instance_part = 0;
    
	if(json_obj == PNULL)
	{
		return JSON_STRING_NULL;
	}	
	
	SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAContactJsonObject app_instance %d", app_instance);

	SCI_MEMSET(&contact_t, 0x00, sizeof(MMISRV_CONTACT_T));
	
	//---1--- 
	//id
	id = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "Id"));
	
	//DateTimeCreated
	dateTimeCreated = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "DateTimeCreated"));
	
	//DateTimeLastModified
	dateTimeLastModified = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "DateTimeLastModified"));
	
	//Birthday
	birthday = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "Birthday"));
	
	//DisplayName
	displayName = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "DisplayName"));
	
	//GivenName
	givenname = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "GivenName"));
	
	//Surname
	surname = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "Surname"));
	
	//EmailAddresses
	json_array_ptr = cJSON_GetObjectItem(json_obj, "EmailAddresses");
	json_array_size = (uint16)cJSON_GetArraySize(json_array_ptr);
	
	for (i = 0; i < json_array_size; i++)
	{
		json_item_ptr = cJSON_GetArrayItem(json_array_ptr, i);
		email_address = cJSON_GetObjectString(cJSON_GetObjectItem(json_item_ptr, "Address"));
		email_name = cJSON_GetObjectString(cJSON_GetObjectItem(json_item_ptr, "Name"));
		//因为Contacts Spec仅支持一个email，故找到第一个有效的email结束
		if(PNULL != email_address)
		{
			break;
		}
	}
	
	//MobilePhone1
	mobilePhone = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "MobilePhone1"));
	
	//HomePhones
	json_array_ptr = cJSON_GetObjectItem(json_obj, "HomePhones");
	//homePhone = cJSON_GetObjectString(json_array_ptr->child);
	homePhone = cJSON_GetObjectString(cJSON_GetArrayItem(json_array_ptr, 0));
	homePhone2 = cJSON_GetObjectString(cJSON_GetArrayItem(json_array_ptr, 1));
	
	
	//BusinessPhones
	json_array_ptr = cJSON_GetObjectItem(json_obj, "BusinessPhones");
	//businessPhone = cJSON_GetObjectString(json_array_ptr->child);
	businessPhone = cJSON_GetObjectString(cJSON_GetArrayItem(json_array_ptr, 0));
	businessPhone2 = cJSON_GetObjectString(cJSON_GetArrayItem(json_array_ptr, 1));

	//---2--- 
	//服务器存储转化为手机存储
	//id
	if (PNULL != id)
	{
		strncpy(contact_t.o365_id, id, MIN(MMIPB_SYNC_O365_ID_LEN, strlen(id)));
	}
	
	//DateTimeLastModified
	if (PNULL != dateTimeLastModified)
	{
		MMIPB_SYNC_O365StrTimeToUint(&(contact_t.o365_modifiedTime), dateTimeLastModified);
	}
	
	//name
	if (PNULL != givenname)
	{       
            SCI_MEMSET(&contact_t.name, 0,sizeof(MMIPB_NAME_T));
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr givenname 0 : %d ", givenname[0]);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr givenname 1 : %d ", givenname[1]);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr givenname 2 : %d ", givenname[2]);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject strlen givenname  : %d ", SCI_STRLEN(givenname));
            ContactSync_enc_utf8_to_unicode(givenname, SCI_STRLEN(givenname), contact_t.name.wstr, &(contact_t.name.wstr_len));
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject givenname  : %s ", givenname);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr name len : %d ", contact_t.name.wstr_len);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr name 0 : %d ", contact_t.name.wstr[0]);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr name 1 : %d ", contact_t.name.wstr[1]);
            SCI_TRACE_LOW("[MMIPBSYNC] JsonParser_ParseAContactJsonObject contact_ptr name 2 : %d ", contact_t.name.wstr[2]);
            //contact_t.name.wstr_len = MIN(MMIAPICOM_Wstrlen(contact_t.name.wstr), MMIPB_NAME_MAX_LEN);  
	}
	if (PNULL != surname)
	{
		SCI_MEMSET(&contact_t.surname, 0,sizeof(MMIPB_NAME_T));
		ContactSync_enc_utf8_to_unicode(surname, SCI_STRLEN(surname), contact_t.surname.wstr, &(contact_t.surname.wstr_len));
		//contact_t.surname.wstr_len = MIN(MMIAPICOM_Wstrlen(contact_t.surname.wstr), MMIPB_NAME_MAX_LEN);  
	}
	
	//number
	//按照mobile, home, business来check
	if (PNULL != mobilePhone)
	{
            SCI_MEMSET(FilteredNum, 0x00, sizeof(FilteredNum));
            ContactSync_NumberFilter(mobilePhone, FilteredNum);
            cur_index = 0;
            contact_t.num_type[cur_index] = MMIPB_ADN_NUMBER;
            MMIAPIPB_StringToBCDNumber(FilteredNum, &contact_t.number[cur_index]);
	}
	
	if ((PNULL != homePhone)
		&& (cur_index < (MMIPB_MAX_NV_PHONE_NUM-1)))
	{
            SCI_MEMSET(FilteredNum, 0x00, sizeof(FilteredNum));
            ContactSync_NumberFilter(homePhone, FilteredNum);
            cur_index++;
            contact_t.num_type[cur_index] = MMIPB_HOME_NUMBER;
            MMIAPIPB_StringToBCDNumber(FilteredNum, &contact_t.number[cur_index]);
	}
	
	if ((PNULL != homePhone2) 
		&& (cur_index < (MMIPB_MAX_NV_PHONE_NUM-1)))
	{
            SCI_MEMSET(FilteredNum, 0x00, sizeof(FilteredNum));
            ContactSync_NumberFilter(homePhone2, FilteredNum);
            cur_index++;
            contact_t.num_type[cur_index] = MMIPB_HOME_NUMBER;
            MMIAPIPB_StringToBCDNumber(FilteredNum, &contact_t.number[cur_index]);
	}
	
	if ((PNULL != businessPhone)
		&& (cur_index < (MMIPB_MAX_NV_PHONE_NUM-1)))
	{
            SCI_MEMSET(FilteredNum, 0x00, sizeof(FilteredNum));
            ContactSync_NumberFilter(businessPhone, FilteredNum);
            cur_index++;
            contact_t.num_type[cur_index] = MMIPB_OFFICE_NUMBER;
            MMIAPIPB_StringToBCDNumber(FilteredNum, &contact_t.number[cur_index]);
	}
	
	if ((PNULL != businessPhone2)
		&& (cur_index < (MMIPB_MAX_NV_PHONE_NUM-1)))
	{
            SCI_MEMSET(FilteredNum, 0x00, sizeof(FilteredNum));
            ContactSync_NumberFilter(businessPhone2, FilteredNum);
            cur_index++;
            contact_t.num_type[cur_index] = MMIPB_OFFICE_NUMBER;
            MMIAPIPB_StringToBCDNumber(FilteredNum, &contact_t.number[cur_index]);
	}	
#ifdef MMIPB_MAIL_SUPPORT
	//email
	if (PNULL != email_address)
	{
            SCI_MEMSET(&contact_t.mail, 0, sizeof(MMIPB_MAIL_T));
            ContactSync_EmailFilter(email_address, contact_t.mail.wstr);
            contact_t.mail.wstr_len = MIN(MMIAPICOM_Wstrlen(contact_t.mail.wstr), MMIPB_MAX_MAIL_LEN);
            SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAContactJsonObject contact_t.mail.wstr %s", contact_t.mail.wstr);
            SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAContactJsonObject contact_t.mail.wstr_len %d", contact_t.mail.wstr_len);
	}
#endif		
#ifndef PB_SUPPORT_LOW_MEMORY
	//birth
	if(PNULL != birthday)
	{
		
            SCI_MEMSET(&contact_t.birth,0,sizeof(MMIPB_BIRTH_T));
            MMI_STRNTOWSTR(contact_t.birth.wstr, MMIPB_SNS_BIRTH_LEN, birthday, strlen(birthday), MIN(MMIPB_SNS_BIRTH_LEN, strlen(birthday)));
            SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAContactJsonObject birthday %s", birthday);
            contact_t.birth.wstr_len = MIN(MMIAPICOM_Wstrlen(contact_t.birth.wstr), MMIPB_SNS_BIRTH_LEN);
	}
#endif
	
	//---3--- 
	//指定存储设备
	contact_t.storage_id = MMIPB_STORAGE_PHONE;
	MMIPB_SYNC_HTTP_Request_List_Get_RequestId(app_instance, &requestId);
	app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(app_instance);
	//---4---
        switch(app_instance_part)
        {
            case PBSYNC_CONTACT_LISTGET:
                //此处调pb接口
                if(GetCurChunkEndFlag())
                {
                    MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                }
                pbError = MMIAPIPBSYNC_PB_GetAllContactsHandler(requestId, (MMIPB_CONTACT_T *)(&contact_t), GetCurChunkEndFlag() ,GetAllEndFlag());
                break;
            case PBSYNC_CONTACT_INFO_GET:
                //此处调pb接口
                //pbError = MMIAPIPBSYNC_PB_GetContactByIdHandler((MMIPB_CONTACT_T *)(&contact_t));
                break;
            case PBSYNC_CONTACT_ADD:
                //此处调pb接口
                MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                pbError = MMIAPIPBSYNC_PB_AddContactHandler(requestId, contact_t.o365_id, contact_t.o365_modifiedTime);
                break;
            case PBSYNC_CONTACT_DELETE:
                MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                pbError = MMIAPIPBSYNC_PB_DeleteContactHandler(requestId, TRUE);
                break;
            case PBSYNC_CONTACT_MODIFY:
                //此处调pb接口
                MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                pbError = MMIAPIPBSYNC_PB_ModifyContactHandler(requestId, contact_t.o365_id, contact_t.o365_modifiedTime, FALSE);
                break;
            case PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME:
                //此处调pb接口
                if(GetCurChunkEndFlag())
                {
                    MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                }
                pbError = MMIAPIPBSYNC_PB_GetAllAddedContactsHandler(requestId, (MMIPB_CONTACT_T *)(&contact_t), GetCurChunkEndFlag() ,GetAllEndFlag());
                break;
            case PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME:
                //此处调pb接口
                if(GetCurChunkEndFlag())
                {
                    MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
                }
                pbError = MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler(requestId, (MMIPB_CONTACT_T *)(&contact_t), GetCurChunkEndFlag() ,GetAllEndFlag());
                break;
            default:
                SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAContactJsonObject requestId error");
                break;		
	}
	
	if(MMIPB_ERROR_SUCCESS == pbError)
	{
		return JSON_PARSER_SUCCESS;
	}
	
	return JSON_PARSER_SUCCESS;
}
/*****************************************************************************/
//  Description : JsonParser_ParseAllModifyTimeJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAllModifyTimeJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj) 
{
	int i = 0;
	cJSON *json_array_ptr = PNULL;
	uint16  json_array_size = 0;
	cJSON *json_item_ptr = PNULL;
	char* next_link = PNULL;
	
	
	if(json_obj == PNULL)
	{
		return JSON_STRING_NULL;
	}	
	
	
	json_array_ptr = cJSON_GetObjectItem(json_obj, "value");//解析出json数组
	
	if(json_array_ptr == PNULL )
	{	
		return JSON_PARSER_ERROR;
	}
	
	json_array_size = (uint16)cJSON_GetArraySize(json_array_ptr);
	
	if(json_array_size == 0 )
	{	
		SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseAllModifyTimeJsonObject ModifyTime list is empty"); 
		
		return JSON_PARSER_EMPTY;		
	}
		
	next_link = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "@odata.nextLink"));
	SCI_MEMSET(s_http_data_next_link, 0x00, PBSYNC_NEXT_LINK_MAX_LEN);
	if ((NULL != next_link) && (0 != SCI_STRLEN(next_link)))
	{
	    if (PBSYNC_NEXT_LINK_MAX_LEN < SCI_STRLEN(next_link))
	    {
	        SCI_TRACE_LOW("[PBSYNC] ERROR !!! JsonParser_ParseAllModifyTimeJsonObject next_link maybe too long");
	    }
	    else
	    {
	        SCI_MEMCPY(s_http_data_next_link, next_link, SCI_STRLEN(next_link));
	    }
	}
	SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseAllModifyTimeJsonObject s_http_data_next_link =%s", s_http_data_next_link);
	
	for (i = 0; i < json_array_size; i++)
	{
		json_item_ptr = cJSON_GetArrayItem(json_array_ptr, i);
		
		if(json_item_ptr != PNULL)
		{
			//设置当前数据包处理状态
			//cur chunk has finished
			if(i == (json_array_size - 1))
			{
				//set cur chunk end flag
				SetCurChunkEndFlag(TRUE);
			}
			//cur chunk has not finished
			else
			{
				SetCurChunkEndFlag(FALSE);
			}

			//设置所有数据包处理状态
			// 处理到最后一个数据块的最后一条信息时置为TRUE
			if((0 == SCI_STRLEN(s_http_data_next_link)) && GetCurChunkEndFlag())
			{
				ClearFlag();//SetAllEndFlag(TRUE);
			}
			else
			{
				SetAllEndFlag(FALSE);
			}
				
			SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseAllModifyTimeJsonObject s_cur_chunk_end_flag=%d, s_all_end_flag=%d", s_cur_chunk_end_flag, s_all_end_flag);

			JsonParser_ParseAModifyTimeJsonObject(app_instance, json_item_ptr);		
		}		
	}
	
	return JSON_PARSER_SUCCESS;
}

/*****************************************************************************/
//  Description : JsonParser_ParseAModifyTimeJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseAModifyTimeJsonObject( HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj) 
{
    //shiwei 20150803
    MMIPB_ERROR_E   pbError = MMIPB_ERROR_ERROR;
    char          *id = PNULL;
    char         *o365_modifiedTime = PNULL;
    MMISRV_CONTACT_MODIFYTIME_T   modifytime_t = {0};
    HTTP_REQUEST_ID_T   requestId = 0;

    SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseAModifyTimeJsonObject enter");

    SCI_MEMSET(&modifytime_t, 0x00, sizeof(MMISRV_CONTACT_MODIFYTIME_T));

    //---1--- 
    //Id
    id = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "Id"));
     
    //DateTimeLastModified
    o365_modifiedTime = cJSON_GetObjectString(cJSON_GetObjectItem(json_obj, "DateTimeLastModified"));

    //---2--- 
    //服务器存储转化为手机存储
    //id
    if (PNULL != id)
    {
        strncpy(modifytime_t.o365_id, id, MIN(MMIPB_SYNC_O365_ID_LEN, strlen(id)));
    }
     
     //DateTimeLastModified
    if (PNULL != o365_modifiedTime)
    {
        MMIPB_SYNC_O365StrTimeToUint(&(modifytime_t.o365_modifiedTime), o365_modifiedTime);
    }
     
    //此处调pb接口
    MMIPB_SYNC_HTTP_Request_List_Get_RequestId(app_instance, &requestId);
    if(GetCurChunkEndFlag())
    {
        MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);
    }
    pbError = MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler(requestId, modifytime_t.o365_id, modifytime_t.o365_modifiedTime, GetCurChunkEndFlag(),  GetAllEndFlag());

     
    return JSON_PARSER_SUCCESS;

	
}
/*****************************************************************************/
//  Description : JsonParser_ParseCountJsonObject
//  Global resource dependence : none
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
LOCAL JSON_PARSER_CODE_E  JsonParser_ParseCountJsonObject(HTTP_APP_INSTANCE_T app_instance,  cJSON *json_obj) 
{
	uint16	count = 0;
	MMIPB_ERROR_E	pbError = MMIPB_ERROR_ERROR;
	HTTP_REQUEST_ID_T   requestId = 0;
	HTTP_APP_INSTANCE_T app_instance_part = 0;

	if(json_obj == PNULL)
	{
		return JSON_STRING_NULL;
	}	

	SCI_TRACE_LOW("[PBSYNC]JsonParser_ParseCountJsonObject enter, app_instance %d", app_instance);

	count= cJSON_GetObjectInt(cJSON_GetObjectItem(json_obj, "@odata.count"));
	MMIPB_SYNC_HTTP_Request_List_Get_RequestId(app_instance, &requestId);
	app_instance_part = MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(app_instance);
       MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(app_instance);

	switch(app_instance_part)
	{
	case PBSYNC_CONTACT_COUNT_GET:
		//此处调pb接口
		pbError = MMIAPIPBSYNC_PB_GetTotalCountHandler(requestId, count);
		break;
	case PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET:
		//此处调pb接口
		pbError = MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler(requestId, count);
		break;
	case PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET:
		//此处调pb接口
		pbError = MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler(requestId, count);
		break;
	default:
		SCI_TRACE_LOW("[PBSYNC] JsonParser_ParseCountJsonObject requestId error");
		break;		
	}
	return JSON_PARSER_SUCCESS;
}
/*****************************************************************************/
//  Description : is response error
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 JsonParser_ParseResponse(char * response_str)
{
	cJSON* root_ptr = PNULL;
	cJSON* errors_ptr = PNULL;
	cJSON* error_item = PNULL;
	uint32  err_code = 0;
	
	SCI_TRACE_LOW("[PBSYNC]Parse response = %s", response_str); 
	
	if (response_str == PNULL) 
	{
		SCI_TRACE_LOW("[PBSYNC] response is NULL"); 
		return RESPONSE_NULL;//null
	}
	root_ptr = cJSON_Parse(response_str);
	
	if(root_ptr == PNULL)
	{
		SCI_TRACE_LOW("[PBSYNC] response parse error"); 
		return RESPONSE_PARSER_ERROR;
	}
	
	errors_ptr = cJSON_GetObjectItem(root_ptr,"errors");
	
	if(errors_ptr == PNULL)
	{
		err_code = RESPONSE_SUCCESS;
	}
	else
	{
		if(cJSON_GetArraySize(errors_ptr) != 0)
		{
			error_item = cJSON_GetArrayItem(errors_ptr,0);
			err_code =(PBSYNC_RESPONSE_CODE_E)cJSON_GetObjectInt(cJSON_GetObjectItem(error_item,"code"));
		}
		else
		{
			SCI_TRACE_LOW("[PBSYNC] response  error list is empty"); 
		}
	}
	
	SCI_TRACE_LOW("[PBSYNC] response parse result=%d",err_code); 
	cJSON_Delete(root_ptr);
    return err_code;
}
/*****************************************************************************/
//  Description : 将整型格式的时间转化成Office365字符串格式时间
//  Global resource dependence : none
//  Author: shiwei.zhang
//  Note: 2015-7-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UintTimeToO365Str(char *pO365tm/*[out]*/, 
                                                        uint32 uiTm/*[in]*/)
{
    MMI_TM_T systm = {0};
    if(PNULL == pO365tm)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    systm = MMIAPICOM_Second2Tm(uiTm);
    sprintf(pO365tm, "%04d-%02d-%02dT%02d:%02d:%02dZ",
        systm.tm_year,systm.tm_mon,\
        systm.tm_mday, systm.tm_hour,\
        systm.tm_min, systm.tm_sec);

    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description : 将Office365字符串格式时间转化成整型格式的时间
//  Global resource dependence : none
//  Author: shiwei.zhang
//  Note: O365tm : 2015-07-21T06:50:41Z, uiTm = ?
//  2015-7-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365StrTimeToUint(uint32 *uiTm/*[out]*/, 
                                                        char *pO365tm/*[in]*/)
{
    SCI_TM_T file_time = {0};
    uint32 phoneModifiedTime = 0;
    char fourChar[4] = "";
    char twoChar [2]= "";

    if (0 == SCI_STRLEN(pO365tm))//0 can be the max len of o365 time
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_MEMCPY(fourChar, pO365tm, sizeof(fourChar));//year
    file_time.tm_year = (uint32)atoi(fourChar);
    SCI_MEMCPY(twoChar, pO365tm + 5, sizeof(twoChar));//mon
    file_time.tm_mon = (uint32)atoi(twoChar);
    SCI_MEMCPY(twoChar, pO365tm + 8, sizeof(twoChar));//day
    file_time.tm_mday =(uint32)atoi(twoChar);
    SCI_MEMCPY(twoChar, pO365tm + 11, sizeof(twoChar));//hour
    file_time.tm_hour =(uint32)atoi(twoChar);
    SCI_MEMCPY(twoChar, pO365tm + 14, sizeof(twoChar));//min
    file_time.tm_min =(uint32)atoi(twoChar);
    SCI_MEMCPY(twoChar, pO365tm + 17, sizeof(twoChar));//sec
    file_time.tm_sec =(uint32)atoi(twoChar);

    phoneModifiedTime = MMIAPICOM_Tm2Second( 
                      file_time.tm_sec, 
                      file_time.tm_min, 
                      file_time.tm_hour,
                      file_time.tm_mday,
                      file_time.tm_mon, 
                      file_time.tm_year
                      );
    *uiTm = phoneModifiedTime;

    return MMIPB_ERROR_SUCCESS;
}

#endif //MMIPB_SYNC_WITH_O365
