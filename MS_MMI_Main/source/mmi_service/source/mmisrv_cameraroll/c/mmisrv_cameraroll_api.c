/*****************************************************************************
** File Name:      mmisrv_cameraroll_api.c                                               *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMISRV_CAMERAROLL_API_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "priority_app.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "in_message.h"
#include "mmisrv_cameraroll_export.h"
#include "mmi_filemgr.h"
#include "mn_type.h" 
#include "mmiphone_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmimsa_export.h"
#include "mmi_nv.h"
#include "mmisrvfmm_export.h"
 /**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#define ONEDRIVE_METADATA_STRING_MAX_LEN 1024 

#define MAX_HTTP_CONTEXT_NUM 8
#define MAX_THUMBNAIL_DOWNLOAD_NUM 12

#define HTTP_RESPONSE_CODE_CREATED                              201
#define HTTP_RESPONSE_CODE_OK                                   200
#define HTTP_RESPONSE_CODE_UNAUTHORIZED                         401
#define HTTP_RESPONSE_CODE_NOT_FOUND                            404
#define HTTP_RESPONSE_CODE_FOUND                            302


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL  BOOLEAN s_cameraroll_task_init_done = FALSE;

LOCAL SCI_MUTEX_PTR         s_camera_roll_mutex_ptr    = SCI_NULL;

static uint8 s_content_buff[CAMERA_ROLL_MAX_STRING_LEN] = {0};

static CAMERA_ROLL_ONEDRIVE_UPLOAD_T s_upload_photo_ary = {0} ; 

LOCAL uint8  s_cameraroll_account_index[MN_DUAL_SYS_MAX]={0};    

LOCAL uint8  s_cameraroll_pdp_timer_id      = 0;
LOCAL uint8  s_cameraroll_pdp_retry_timer_id = 0;
LOCAL uint8 s_cameraroll_pdp_retry_count =0;

LOCAL uint8 s_camera_roll_access_token_str[MAX_TOKEN_STR_SIZE + 1]={0};

LOCAL wchar *s_meta_info_file_path = L"D:\\cameraroll_meta_info.txt";
LOCAL wchar *s_new_meta_info_file_path = L"D:\\cameraroll_new_meta_info.txt";
LOCAL wchar *s_meta_info_file_name = L"cameraroll_meta_info.txt";
LOCAL wchar *s_onedrive_upload_success_file_path = L"D:\\onedrive_upload_success.txt";
LOCAL wchar *s_onedrive_snapshot_upload_list_file_path = L"D:\\onedrive_upload_list.txt";
LOCAL wchar *s_onedrive_manual_upload_list_file_path = L"D:\\onedrive_manual_upload_list.txt";

LOCAL FILEARRAY s_onedrive_manual_upload_file_array = PNULL;
LOCAL FILEARRAY s_onedrive_upload_success_file_array = PNULL;
LOCAL FILEARRAY s_onedrive_snapshot_upload_file_array = PNULL;
LOCAL FILEARRAY s_onedrive_snapshot_list_file_array = PNULL;
LOCAL FILEARRAY s_onedrive_deleted_list_file_array = PNULL;
LOCAL FILEARRAY s_current_upload_file_array = PNULL;
LOCAL wchar * s_current_upload_file_path = PNULL;

LOCAL BOOLEAN s_is_process_download_thumbnail = FALSE;
LOCAL BOOLEAN s_is_need_to_refresh_thumbnail = FALSE;
LOCAL BOOLEAN s_is_process_get_delta_link = FALSE;
LOCAL BOOLEAN s_is_in_gallery = FALSE;

LOCAL CAMERA_ROLL_OPERATION_E s_cameraroll_resume_operation = CMAERA_ROLL_OPERAION_MAX;
LOCAL BLOCK_ID g_delete_onedrive_file_task_id = SCI_INVALID_BLOCK_ID;

static MMISRV_CAMERAROLL_REQUEST_ARRAY_T http_request_operation_array[MAX_HTTP_CONTEXT_NUM] = {0};
LOCAL uint8 s_current_download_thumbnail_count = 0;
LOCAL BOOLEAN  s_is_process_pdp_activate=FALSE;

LOCAL const char* const s_camera_roll_json_tag_arr[] = 
{
    "",
    "id",		
    "name",
    "size",
    "webUrl",
    "@odata.deltaLink",
    "value",
    "user",
    "deleted",
    "application"
};

MMIFILE_HANDLE s_meta_data_file_handle = 0;

static MMISRV_CAMERAROLL_ONEDRIVE_METADATA * s_onedrive_metadata_ptr = PNULL;
static MMISRV_CAMERAROLL_UPLOAD_INFO_T * s_onedrive_upload_photo_ptr = PNULL;
static MMISRV_CAMERAROLL_DOWNLOAD_INFO_T *s_current_download_ptr = PNULL;
static MMISRV_CAMERAROLL_JSON_PARSE_DATA s_onedrive_photo_json_parse_info = {0};
static uint8 s_onedrive_photo_info_src_name[CAMERA_ROLL_MAX_STRING_LEN+1] = {0};
static uint8 s_download_thumbnail_total_count =0;
static uint8 s_download_thumbnail_fail_count =0;
static uint8 s_download_thumbnail_success_count =0;
LOCAL uint8 s_download_thumbnail_timer = 0;
LOCAL uint32 s_download_thumbnail_system_time = 0;

MMI_APPLICATION_T g_camera_roll = {0};
extern uint32 g_current_module_id;
LOCAL uint32 s_cameraroll_net_id = 0;
LOCAL MN_DUAL_SYS_E s_cameraroll_cur_sys_num   = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_b_is_cancel_upload_photo = FALSE;
LOCAL BOOLEAN s_b_is_cancel_download_photo = FALSE;
LOCAL ONE_DRIVE_AUTO_UPLOAD_STATE_E s_auto_upload_status =ONE_DRIVE_AUTO_UPLOAD_NONE;

//#define DEBUG_REFRESH 1

#ifdef DEBUG_REFRESH
LOCAL BOOLEAN s_set_refresh_flag = TRUE;
#endif

extern CAMERA_ROLL_TYPE_E s_cameraroll_init_from ; 
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//Get Drive
#define MS_ONEDRIVE_GET_DRIVE_URI  "https://api.onedrive.com/v1.0/drive/special/cameraroll/children?$select=name,id,size&orderby=lastModifiedDateTime%20desc"
#define MS_ONEDRIVE_AUTHORIZATION "Authorization"
#define MS_ONEDRIVE_ACCEPT_STRING "*/*"
#define MS_ONEDRIVE_ACCEPT_LANGUAGE_STRING  "zh-cn,zh-TW, en"
#define MS_ONEDRIVE_USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define MS_ONEDRIVE_HTTP_GET_HEADER_NUM 2
#define MS_ONEDRIVE_ACCEPT_CHARSET_STRING   "utf-8, utf-16, iso-8859-1, iso-8859-5, iso-10646-ucs-2, GB2312, windows-1252, us-ascii"
#define MS_ONEDRIVE_X_TARGET_NAME "X-Target-URI"
#define MS_ONEDRIVE_X_TARGET_VALUE "https://api.onedrive.com"
#define MS_ONEDRIVE_HTTP_GET_ONEDRIVE_INFO_HEADER_NUM 1
#define MS_ONEDRIVE_GET_PHOTO_ITEMS_URL " https://api.onedrive.com/v1.0/drive/items/"
//#define MS_ONEDRIVE_GET_CAMERAROLL_ID "https://api.onedrive.com/v1.0/drive/special/cameraroll/"
#define MS_ONEDRIVE_GET_CAMERAROLL_ID "https://apis.live.net/v5.0/me?access_token="
#define MS_ONEDRIVE_GET_DELTA_TOKEN_URL "https://api.onedrive.com/v1.0/drive/special/cameraroll/view.delta?token="
#define MS_ONEDRIVE_GET_THUMBNAIL_URL "https://api.onedrive.com/v1.0/drive/items/"
//Post 
#define MS_ONEDRIVE_HTTP_POST_CREATE_SESSION_HEADER_NUM 4
#define MS_ONEDRIVE_HTTP_POST_UPLOAD_SESSION_HEADER_NUM 5
#define MS_ONEDRIVE_HTTP_POST_CLOSE_SESSION_HEADER_NUM 5
#define MS_ONEDRIVE_HTTP_POST_CREATE_SHARE_LINK_HEADER_NUM 1
#define MS_ONEDRIVE_X_HTTP_METHOD_NAME "X-Http-Method-Override"
#define MS_ONEDRIVE_X_HTTP_METHOD_VALUE "BITS_POST"
#define MS_ONEDRIVE_BITS_PACKET_TYPE_NAME "BITS-Packet-Type"
#define MS_ONEDRIVE_BITS_PACKET_TYPE_CREATE_SESSION "Create-Session"
#define MS_ONEDRIVE_BITS_PACKET_TYPE_FRAGMENT "Fragment"
#define MS_ONEDRIVE_BITS_PACKET_TYPE_CLOSE_SESSION "Close-Session"
#define MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_NAME "BITS-Supported-Protocols"
#define MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_VALUE "{7df0354d-249b-430f-820d-3d2a9bef4931}"
#define MS_ONEDRIVE_BITS_CONTENT_LENGTH_NAME "Content-Length"
#define MS_ONEDRIVE_BITS_CONTENT_RANGE_NAME "Content-Range"
#define MS_ONEDRIVE_BITS_CONTENT_RANGE_VALUE "bytes 0-4980/4981"
#define MS_ONEDRIVE_BITS_SESSION_ID_NAME "BITS-Session-Id"
#define MS_ONEDRIVE_BITS_SESSION_ID_VALUE "2mK4VhCdhsJU7yUZ7lMJRdQ3Gjp0qlKiCIXWth_dM1KZUSugi5VNNwFxolYnhAYB8p"
#define MS_ONEDRIVE_BITS_CONTENT_TYPE  "application/octet-stream"

#define MS_ONEDRIVE_CONTENT_TYPE_JSON_VALUE "application/json"
#define MS_ONEDRIVE_SHARE_LINK_JSON_DATA "{\"type\": \"view\"}" 


/*****************************************************************************/
//  Description : GetSignalStruct
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void HandleHttpErrorInd(MMISRV_CAMERAROLL_HTTP_CONTEXT_T *http_context_ptr, DPARAM param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_thumbnail_info_req_func(uint8 app_param , void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_download_thumbnail_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_create_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_upload_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_close_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_create_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_close_session_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_download_photo_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_create_share_link_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_cameraroll_info_req_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_delta_token_info_req_func(uint8 app_param, void * param);


/*****************************************************************************/
//  Description : init json config
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void camera_roll_InitjsonConfig(JSON_config* config_ptr,
                                    JSON_parser_callback callback,
                                    MMISRV_CAMERAROLL_JSON_PARSE_DATA *src_data_ptr);

/*****************************************************************************/
//  Description : parse json char one by one
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int camera_roll_ParseJsonChar(JSON_parser jp_ptr , uint8* data_ptr , uint32 data_len);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 camera_roll_get_avaliable_req_index( uint8* oper_idx);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void camera_roll_download_thumbnail_req(uint8 , uint8);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_thumbnail_info_cnf_func(uint8 app_param, void * param);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_download_photo_cnf_func(uint8 app_param, void * param);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_Http_all_request(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_meta_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_upload_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_download_data_info(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Destroy_Http_all_request(void);

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_all_data(void);

/*****************************************************************************/
//  Description :HandlePdpRetryTimer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpRetryTimer(   uint8 timer_id,  uint32 param   );

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Handle_JSON_Is_Tag_Equal(uint8 index , const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint8 Generate_Download_thumbnail_node_by_JSON(const uint8* data ,uint32 data_len );

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleDownloadThumbnail_timer(uint8  timer_id, uint32 param);

/*****************************************************************************/
//  Description : Free Header
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeHeader(HTTP_EXTERN_HEADER_T *pHttpExternHeader, uint32 header_num);

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Remove_File_From_UploadFileAry(uint16 * file_path , uint16 file_path_len);

CAMERA_ROLL_HTTP_REQ_FUNC http_data_ind_function[] = 
{
	PNULL,											//CAMERA_ROLL_OPER_NOT_INIT
	PNULL ,											//CAMERA_ROLL_OPER_NO_OPERATION
    camera_roll_get_thumbnail_info_req_func,		//CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO
    PNULL,			//CAMERA_ROLL_OPER_GET_THUMBNAIL_URL
    camera_roll_download_thumbnail_req_func,		//CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL
	PNULL,											//CAMERA_ROLL_OPER_CREATE_SESSION
	PNULL,											//CAMERA_ROLL_OPER_UPLOAD_SESSION
	PNULL,											//CAMERA_ROLL_OPER_CLOSE_SESSION
    PNULL ,     //CAMERA_ROLL_OPER_DOWNLOAD_PHOTO_LOCATION
    camera_roll_download_photo_req_func, //    CAMERA_ROLL_OPER_DOWNLOAD_PHOTO
	PNULL ,											//CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION
	PNULL,											//CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION
	PNULL,											//CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION  
    camera_roll_create_share_link_req_func , 			//CAMERA_ROLL_OPER_CREATE_SHARE_LINK
	camera_roll_get_cameraroll_info_req_func ,		//CAMERA_ROLL_OPER_GET_CAMERAROLL_INFO
	camera_roll_get_delta_token_info_req_func,		//CAMERA_ROLL_OPER_GET_DELTA_TOKEN_INFO
	PNULL											//CMAERA_ROLL_OPERAION_MAX
};  

CAMERA_ROLL_HTTP_REQ_FUNC http_cnf_function[] = 
{
	PNULL,											//CAMERA_ROLL_OPER_NOT_INIT
	PNULL ,											//CAMERA_ROLL_OPER_NO_OPERATION
	camera_roll_get_thumbnail_info_cnf_func ,		//CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO
	PNULL,			//CAMERA_ROLL_OPER_GET_THUMBNAIL_URL
	PNULL,		//CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL
	camera_roll_create_session_req_func,			//CAMERA_ROLL_OPER_CREATE_SESSION
	camera_roll_upload_session_req_func,			//CAMERA_ROLL_OPER_UPLOAD_SESSION
	camera_roll_close_session_req_func,				//CAMERA_ROLL_OPER_CLOSE_SESSION
	PNULL ,     //CAMERA_ROLL_OPER_DOWNLOAD_PHOTO_LOCATION
	camera_roll_download_photo_cnf_func, //    CAMERA_ROLL_OPER_DOWNLOAD_PHOTO
	camera_roll_auto_upload_create_session_req_func,	//CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION
	camera_roll_auto_upload_session_req_func,			//CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION
	camera_roll_auto_upload_close_session_req_func,	//CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION    
       PNULL , 											//CAMERA_ROLL_OPER_CREATE_SHARE_LINK	
        PNULL , 										//CAMERA_ROLL_OPER_GET_CAMERAROLL_INFO
        PNULL , 										//CAMERA_ROLL_OPER_GET_DELTA_TOKEN_INFO
	PNULL											//CMAERA_ROLL_OPERAION_MAX
};  


/*****************************************************************************/
//  FUNCTION:      MMISRV_CAMERARoll_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_InitModule(void)
{
	g_camera_roll.ProcessMsg = MMISRV_CAMERAROLL_HandleHTTPMsg;
	s_camera_roll_mutex_ptr = SCI_CreateMutex("Mutex for camera roll callback protection", SCI_INHERIT);
}

/*****************************************************************************/
//  FUNCTION:      MMISRV_CAMERAROll_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_InitHttpRequest(uint32 module_id)
{
       uint8 i=0;
	BOOLEAN bHas_not_init = FALSE;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	MMISRV_CAMERAROLL_MSG_T sig_data={0};	
	
	//init HTTP !!!
	SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_InitHttpRequest-----");
	
	init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
	init_param_ptr->is_cookie_enable = FALSE;
	SCI_TRACE_LOW("[cookie] MMISRV_CAMERAROLL_InitHttpRequest init_param_ptr->is_cookie_enable = FALSE;");
	init_param_ptr->is_cache_enable = FALSE;
	s_cameraroll_task_init_done = FALSE;

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_InitHttpRequest----- i = %d" , i);

		if( 0 == http_request_operation_array[i].context_id)
		{
			HTTP_InitRequest(module_id, i, init_param_ptr);
			bHas_not_init = TRUE;
		}
	}
		
	if(FALSE == bHas_not_init) //means all http context are requested before
	{
		SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_InitHttpRequest all http context are requested before");

		if(CAMERA_ROLL_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO != s_cameraroll_init_from)
		{
			sig_data.init_type = s_cameraroll_init_from;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_INIT_CNF , &sig_data);		
			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF SEND MSG_CAMERA_ROLL_INIT_CNF !!!");			
		}
		else
		{
			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF MMISRV_CAMERAROLL_exec_auto_upload_photo !!!");					  
			MMISRV_CAMERAROLL_exec_auto_upload_photo();
		}		
		s_cameraroll_task_init_done = TRUE;
	}

	SCI_FREE(init_param_ptr);
}

/*****************************************************************************/
//  Description: Get Mutex
//  Global resource dependence:
//  Author: 
/*****************************************************************************/
LOCAL void Camera_Roll_GetMutex(SCI_MUTEX_PTR mutex)
{
    SCI_TRACE_LOW("[cameraroll]Camera_Roll_GetMutex");

    if( NULL == mutex )
    {
	SCI_TRACE_LOW("[cameraroll]Camera_Roll_GetMutex : Mutex is NULL!");

        return; 
    }
    
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
}
/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author: 
/*****************************************************************************/
LOCAL void Camera_Roll_PutMutex(SCI_MUTEX_PTR mutex)
{
    SCI_TRACE_LOW("[cameraroll]Camera_Roll_PutMutex");
    if( NULL == mutex )
    {
	 SCI_TRACE_LOW("[cameraroll]Camera_Roll_PutMutex : Mutex is NULL!");
        return; 
    }
    SCI_PutMutex( mutex );  
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_start_get_thumbnail_info(void)
{
    uint8 avaliable_request_idx,context_id ;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 file_size = 0 , read_size = 0 , onedrive_total_photo_size = 0;
	uint8 * buff = PNULL ; 
	
	if(TRUE == s_is_process_download_thumbnail)
        {
		SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_start_get_thumbnail_info previous download operation still ongoing  SKIP !!!");                
                return ;
        }			

	if(TRUE == s_is_need_to_refresh_thumbnail  || 
          //(MMIAPICOM_GetCurTime() - s_download_thumbnail_system_time >=3600) ||
    	  (!MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        )
	{
    Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
    context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
    Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

    if(0xFF != context_id)
    {
	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_start_get_thumbnail_info start get meta info from onedrive");
	get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

	if(get_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_start_get_thumbnail_info error code MSG_CAMERA_ROLL_ERROR_NOTIFY get_param_ptr == NULL" );
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return;
	}	
	
   	http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO;
	   
	memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  //modify for coverity
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->need_net_prog_ind = FALSE;

	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;

	get_param_ptr->recv_timeout = 5*60*1000; //10 min 

	len=strlen(MS_ONEDRIVE_GET_DRIVE_URI);
	get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(get_param_ptr->uri.uri_ptr,0,len+1);
       strcpy(get_param_ptr->uri.uri_ptr,MS_ONEDRIVE_GET_DRIVE_URI);

	get_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);

	len=strlen(MS_ONEDRIVE_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

	len=strlen(s_camera_roll_access_token_str);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);

	len=strlen(MS_ONEDRIVE_X_TARGET_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_TARGET_NAME);

	len=strlen(MS_ONEDRIVE_X_TARGET_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_TARGET_VALUE);
	
	if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
	{
		SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_GetRequest OK !!!!");
	}

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
    }

	}
	else
	{
		s_is_process_get_delta_link = FALSE;
	
		SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_start_get_thumbnail_info use cached meta data !!!");                
		if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
		{
			s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
			if(SFS_INVALID_HANDLE != s_meta_data_file_handle)
			{
				file_size = MMIAPIFMM_GetFileSize(s_meta_data_file_handle);
				buff = SCI_ALLOC(file_size);
				if(PNULL == buff)
				{
					SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_start_get_thumbnail_info ---(PNULL == buff) "); 
					return;
		      		}
			}
		}
	
		file_ret =  MMIAPIFMM_ReadFile(
		    s_meta_data_file_handle, 
		    buff, 
		    file_size, 
		    &read_size, 
		    NULL
		    );

		if(SFS_ERROR_NONE == file_ret)
		{
			SCI_TRACE_LOW("[cameraroll] camera_roll_get_thumbnail_info_cnf_func read meta success !!");
		}
		
		MMIAPIFMM_CloseFile(s_meta_data_file_handle);
		s_meta_data_file_handle = SFS_INVALID_HANDLE ;

		Generate_Download_thumbnail_node_by_JSON(buff , file_size);

		if(PNULL != s_onedrive_metadata_ptr)
		{     
			//check auto upload status , if YES , pause first 
			if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
		           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
			{
				SCI_TRACE_LOW("[cameraroll]: camera_roll_get_thumbnail_info_cnf_func need download thumbnail !! Pause auto upload !!!");
				Reset_Http_all_request();
			}				
			s_download_thumbnail_fail_count =0;
			s_download_thumbnail_success_count =0;
			s_is_process_download_thumbnail = TRUE;		
			MMISRV_CAMERAROLL_start_download_thumbnails();		
		}
		else
		{
			if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()||
			MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo())
			{
				MMISRV_CAMERAROLL_exec_auto_upload_photo();
			}
		}

		s_download_thumbnail_total_count = MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count(buff , file_size);

		SCI_FREE(buff);
		
		camera_roll_msg_data.curr_download_num = s_current_download_thumbnail_count;
		camera_roll_msg_data.total_num = s_download_thumbnail_total_count ;
		
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY , &camera_roll_msg_data);

	    	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_thumbnail_info_cnf_func");   

		s_download_thumbnail_system_time = MMIAPICOM_GetCurTime();

		if (0 != s_download_thumbnail_timer)
		{
		       MMK_StopTimer(s_download_thumbnail_timer);
			s_download_thumbnail_timer = 0;
	    	}
	}
	
    return;	
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_start_upload_file(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id ;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};

    Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
    context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
    Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);
	
    if(0xFF != context_id)
    {
		SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_start_upload_file !!!!");
		post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));
		
		if(post_param_ptr == NULL) 
		{
			SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_start_upload_file error code MSG_CAMERA_ROLL_ERROR_NOTIFY post_param_ptr == NULL" );
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			return;
		}	
		
		if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
		   ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION;
		}
		else
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_CREATE_SESSION;
		}

		memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
		
		post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		post_param_ptr->need_net_prog_ind = FALSE;
		
		//extra header
		post_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_POST_CREATE_SESSION_HEADER_NUM;
		post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
		memset(post_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
		
		len=strlen(MS_ONEDRIVE_AUTHORIZATION);
		post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);
		
		len=strlen(s_camera_roll_access_token_str);
		post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);
		
		
		len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_NAME);
		post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_HTTP_METHOD_NAME);
		
		len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_VALUE);
		post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_HTTP_METHOD_VALUE);
		
		len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);
		post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);
		
		len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_CREATE_SESSION);
		post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_CREATE_SESSION);
		
		len=strlen(MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_NAME);
		post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_NAME);
		
		len=strlen(MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_VALUE);
		post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
		memset(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
		strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,MS_ONEDRIVE_BITS_SUPPORTED_PROTOCOLS_VALUE);

		post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		post_param_ptr->post_body.is_use_file = FALSE;
		
		memset(&s_content_buff,0, CAMERA_ROLL_MAX_STRING_LEN);
		
		post_param_ptr->post_body.u.post_buffer.buffer_ptr = &s_content_buff;//param_ptr->body_data.data_src.addr_ptr;
		post_param_ptr->post_body.u.post_buffer.buffer_len = CAMERA_ROLL_MAX_STRING_LEN;
		post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;
		
		post_param_ptr->need_net_prog_ind = FALSE;
		post_param_ptr->is_use_relative_url = TRUE;

		post_param_ptr->recv_timeout = 5*60*1000; //10 min 

		len=strlen(s_onedrive_upload_photo_ptr->upload_url);
		post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(post_param_ptr->uri.uri_ptr,0,len+1);
		strcpy(post_param_ptr->uri.uri_ptr,s_onedrive_upload_photo_ptr->upload_url);
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id,post_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_PostRequest success !!!!");
		}
#endif

		SCI_FREE(post_param_ptr->uri.uri_ptr);
		FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
		SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
		post_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(post_param_ptr);
	}
	return;	

}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_upload_session(uint8 context_id , uint8 http_req_ary_idx , MMISRV_CAMERAROLL_UPLOAD_INFO_T * src_ptr)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	uint32 read_bytes = 0;
	uint8 content_range_str[CAMERA_ROLL_MAX_STRING_LEN] = {0};
	uint8 upload_url_str[CAMERA_ROLL_MAX_STRING_LEN]={0};
	uint16 upload_url_str_len =0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	
	SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_upload_session !!!!");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_upload_session error code post_MSG_CAMERA_ROLL_ERROR_NOTIFY param_ptr == NULL" );
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return;
	}	

	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;

	//extra header
	post_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_POST_UPLOAD_SESSION_HEADER_NUM;
	post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
	memset(post_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);

	len=strlen(MS_ONEDRIVE_AUTHORIZATION);
	post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

	len=strlen(s_camera_roll_access_token_str);
	post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);


	len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_NAME);
	post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_HTTP_METHOD_NAME);

	len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_VALUE);
	post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_HTTP_METHOD_VALUE);

	len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);
	post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);

	len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_FRAGMENT);
	post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_FRAGMENT);

	len=strlen(MS_ONEDRIVE_BITS_SESSION_ID_NAME);
	post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_ONEDRIVE_BITS_SESSION_ID_NAME);

	len=strlen(src_ptr->session_id);
	post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,src_ptr->session_id);


	len=strlen(MS_ONEDRIVE_BITS_CONTENT_RANGE_NAME);
	post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr,MS_ONEDRIVE_BITS_CONTENT_RANGE_NAME);

    //handle range value 
	read_bytes = MIN(s_upload_photo_ary.buf_size, src_ptr->remain_file_size);
    MMIAPIFMM_SetFilePointer(src_ptr->file_handle ,src_ptr->sended_file_size ,MMIFILE_SEEK_BEGIN);
	MMIAPIFMM_ReadFile(src_ptr->file_handle, s_upload_photo_ary.buf_ptr , read_bytes,&read_bytes, PNULL);

	sprintf(content_range_str , "bytes %d-%d/%d" , src_ptr->sended_file_size , src_ptr->sended_file_size + read_bytes - 1 , src_ptr->total_file_size);
	   
	len=strlen(content_range_str);
	post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr,content_range_str);

	post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	post_param_ptr->post_body.is_use_file = FALSE;

	//memset(&s_content_buff,0, CONTENT_BUFF_SIZE);
	
	post_param_ptr->post_body.u.post_buffer.buffer_ptr = s_upload_photo_ary.buf_ptr;
	post_param_ptr->post_body.u.post_buffer.buffer_len = read_bytes;

	post_param_ptr->post_body.u.post_buffer.is_copied_by_http = FALSE;
    post_param_ptr->need_net_prog_ind = FALSE;
    post_param_ptr->is_use_relative_url = TRUE;

    post_param_ptr->recv_timeout = 5*60*1000; //10 min 

	// handle upload url 
	len=strlen(src_ptr->upload_url);
	post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(post_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(post_param_ptr->uri.uri_ptr,src_ptr->upload_url);


#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id,post_param_ptr, http_req_ary_idx))
	{
		SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_PostRequest success !!!!");

		src_ptr->remain_file_size -= read_bytes;
		src_ptr->sended_file_size += read_bytes;
		http_request_operation_array[context_id].operation_ary[http_req_ary_idx].upload_buf_size = read_bytes;
	}
#endif

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
	SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
	post_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(post_param_ptr);

	return;	
	
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_close_session(uint8 context_id , uint8 http_index, MMISRV_CAMERAROLL_UPLOAD_INFO_T * src_ptr)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	
	SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_close_session !!!!");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_close_session error code MSG_CAMERA_ROLL_ERROR_NOTIFY post_param_ptr == NULL" );
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return;
	}	

	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;

	//extra header
	post_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_POST_CLOSE_SESSION_HEADER_NUM;
	post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
	memset(post_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);

	len=strlen(MS_ONEDRIVE_AUTHORIZATION);
	post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

	len=strlen(s_camera_roll_access_token_str);
	post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);


	len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_NAME);
	post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_HTTP_METHOD_NAME);

	len=strlen(MS_ONEDRIVE_X_HTTP_METHOD_VALUE);
	post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_HTTP_METHOD_VALUE);

	len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);
	post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_name_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_NAME);

	len=strlen(MS_ONEDRIVE_BITS_PACKET_TYPE_CLOSE_SESSION);
	post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[2].header_value_ptr,MS_ONEDRIVE_BITS_PACKET_TYPE_CLOSE_SESSION);

	len=strlen(MS_ONEDRIVE_BITS_SESSION_ID_NAME);
	post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_name_ptr,MS_ONEDRIVE_BITS_SESSION_ID_NAME);

	len=strlen(s_onedrive_upload_photo_ptr->session_id);
	post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[3].header_value_ptr,s_onedrive_upload_photo_ptr->session_id);
	
	len=strlen(MS_ONEDRIVE_BITS_CONTENT_LENGTH_NAME);
	post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[4].header_name_ptr,MS_ONEDRIVE_BITS_CONTENT_RANGE_NAME);

	len=strlen("0");
	post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[4].header_value_ptr,"0");

	post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	post_param_ptr->post_body.is_use_file = FALSE;

	//memset(&s_content_buff,0, CONTENT_BUFF_SIZE);
	
    post_param_ptr->need_net_prog_ind = TRUE;
    post_param_ptr->is_use_relative_url = TRUE;

    post_param_ptr->recv_timeout = 5*60*1000; //5 min 	

	len=strlen(s_onedrive_upload_photo_ptr->upload_url);
	post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(post_param_ptr->uri.uri_ptr,0,len+1);
    strcpy(post_param_ptr->uri.uri_ptr,s_onedrive_upload_photo_ptr->upload_url);
	
#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id ,post_param_ptr, http_index))
	{
		SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_PostRequest success !!!!");
	}
#endif

	SCI_FREE(post_param_ptr->uri.uri_ptr);
	FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
	SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
	post_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(post_param_ptr);

	return;	
}


/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr + sizeof(xSignalHeaderRec)), param, (struct_size - sizeof(xSignalHeaderRec)));
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandleHttpErrorInd(MMISRV_CAMERAROLL_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    HTTP_CONTEXT_ID_T       context_id  = 0;
    HTTP_ERROR_E            http_result = HTTP_SUCCESS;         //http error
    
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    context_id  = signal.context_id;
    http_result = signal.result;
    SCI_TRACE_LOW("[cameraroll]:HandleHttpErrorInd error = %d",http_result);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1620_112_2_18_3_6_35_25,(uint8*)"d",http_result);
        
    //HTTP_CloseRequest(context_id); 
    return ;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 camera_roll_get_avaliable_req_index(uint8 * oper_idx)
{
    uint8 context_id=0 , j = 0;
	

    for(context_id=0 ; context_id<MAX_HTTP_CONTEXT_NUM ; context_id++)
    {
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{    
			if(CAMERA_ROLL_OPER_NO_OPERATION == http_request_operation_array[context_id].operation_ary[j].operation)		
			{
				*oper_idx = j;
				return context_id;
			}
		}
    }

    return 0xFF;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_thumbnail_info_req_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    	MMISRV_CAMERAROLL_JSON_PARSE_DATA input_data = {0};
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	//uint8 total_download_count = 1;
	uint16 onedrive_total_photo_size = 1;
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA *tmp_ptr = PNULL;
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;

#ifdef DEBUG_REFRESH

	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_get_thumbnail_info_req_func refresh token !!!");
		Reset_Http_all_request();
		MMIAPIMSA_refresh_token(MSA_APP_TYPE_CAMERA_ROLL ,MMISRV_CAMERAROLL_Get_Token);	
		s_cameraroll_resume_operation = CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif
    	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_thumbnail_info_req_func");   

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			Reset_all_data();
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			SCI_TRACE_LOW("[cameraroll]camera_roll_get_thumbnail_info_req_func MSG_CAMERA_ROLL_ERROR_NOTIFY error code %d" , cnf->response_code);
		}
		return FALSE;
	}

	data_len = cnf->data_len;
	data_ptr = cnf->data_ptr;

	//delete meta data first 

	if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
	{
		Clean_meta_data_info();

		if(MMIAPIFMM_IsFileExist(s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path)))
		{
			MMIAPIFMM_DeleteFile(s_new_meta_info_file_path , PNULL);
		}
		
		s_meta_data_file_handle = MMIAPIFMM_CreateFile(s_new_meta_info_file_path, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE == s_meta_data_file_handle)
		{
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_UNKNOWN;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			return FALSE;		
		}
	}
	else
	{
		MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_CUR);
	}
	
	//if(SFS_INVALID_HANDLE != s_meta_data_file_handle)
	{
		file_ret = MMIAPIFMM_WriteFile(
			s_meta_data_file_handle, 
			cnf->data_ptr, 
			cnf->data_len, 
			&write_size, 
			NULL
			);

		if(SFS_ERROR_NONE == file_ret)
		{
			SCI_TRACE_LOW("[cameraroll]camera_roll_get_thumbnail_info_req_func meta update success !!");
		}
		//MMIAPIFMM_CloseFile(file_handle);
	}

    SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_thumbnail_info_req_func");   
    return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_thumbnail_info_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    	MMISRV_CAMERAROLL_JSON_PARSE_DATA input_data = {0};
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA *tmp_ptr = PNULL;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint32 write_size = 0;
	uint32 file_size = 0 , read_size = 0 ;
	uint8 * buff = PNULL ;
	BOOLEAN retVal = FALSE;

    	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_thumbnail_info_cnf_func");   
	
	file_size = MMIAPIFMM_GetFileSize(s_meta_data_file_handle);
	buff = SCI_ALLOC(file_size);
	
	if(PNULL == buff)
	{
		SCI_TRACE_LOW("[cameraroll]:camera_roll_get_thumbnail_info_cnf_func --- (PNULL == buff)"); 
		return 0;
	}

	MMIAPIFMM_SetFilePointer(s_meta_data_file_handle, 0 , SFS_SEEK_BEGIN);

	file_ret =  MMIAPIFMM_ReadFile(
	    s_meta_data_file_handle, 
	    buff, 
	    file_size, 
	    &read_size, 
	    NULL
	    );

	if(SFS_ERROR_NONE == file_ret)
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_get_thumbnail_info_cnf_func read meta success !!");
	}
	
	MMIAPIFMM_CloseFile(s_meta_data_file_handle);
	s_meta_data_file_handle = SFS_INVALID_HANDLE ;

	if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_get_thumbnail_info_cnf_func delete meta file !!");
		MMIAPIFMM_DeleteFile(s_meta_info_file_path , PNULL);
	}

	retVal = MMIAPIFMM_RenameFile((const wchar *)s_new_meta_info_file_path, MMIAPICOM_Wstrlen(s_new_meta_info_file_path), s_meta_info_file_name , MMIAPICOM_Wstrlen(s_meta_info_file_name));

	if(FALSE == retVal)
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_get_thumbnail_info_cnf_func MMIAPIFMM_RenameFile FAIL !!!!");
	}

	s_is_process_get_delta_link = FALSE;

	Generate_Download_thumbnail_node_by_JSON(buff , file_size);

	if(PNULL != s_onedrive_metadata_ptr)
	{     
		if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
	           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
		{
			SCI_TRACE_LOW("[cameraroll]: camera_roll_get_thumbnail_info_cnf_func need download thumbnail !! Pause auto upload !!!");
			Reset_Http_all_request();			
		}				

		s_download_thumbnail_fail_count =0;
		s_download_thumbnail_success_count =0;
		s_is_process_download_thumbnail = TRUE;				
		
		MMISRV_CAMERAROLL_start_download_thumbnails();			
	}
	else if(ONE_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status)
	{
		if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()||
		   MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo())
		{
			MMISRV_CAMERAROLL_exec_auto_upload_photo();
		}
	}

	s_download_thumbnail_total_count = MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count(buff , file_size);

	SCI_FREE(buff);
	
	camera_roll_msg_data.total_num = s_download_thumbnail_total_count ;
	camera_roll_msg_data.curr_download_num = s_current_download_thumbnail_count;
	
	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY , &camera_roll_msg_data);

	s_download_thumbnail_system_time = MMIAPICOM_GetCurTime();
	s_is_need_to_refresh_thumbnail = FALSE;

	return TRUE;
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8 camera_roll_download_thumbnail_req_func(uint8 app_param, void * param)
{
#ifdef MMI_SHOW_MSA
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;	
	uint32 write_size = 0;
	uint8 ret=0 , i;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	MMISRV_CAMERAROLL_MSG_T sig_data={0};
	BOOLEAN bIsFound = FALSE;

#ifdef DEBUG_REFRESH

	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[cameraroll]camera_roll_download_thumbnail_req_func refresh token !!!");
		Reset_Http_all_request();
		MMIAPIMSA_refresh_token(MSA_APP_TYPE_CAMERA_ROLL ,MMISRV_CAMERAROLL_Get_Token);	
		s_cameraroll_resume_operation = CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif


	if (0 != s_download_thumbnail_timer)
	{
	       MMK_StopTimer(s_download_thumbnail_timer);
		s_download_thumbnail_timer = 0;
    	}

	if(HTTP_RESPONSE_CODE_OK != cnf->response_code && 
	    HTTP_RESPONSE_CODE_FOUND !=cnf->response_code &&
	    HTTP_RESPONSE_CODE_NOT_FOUND !=cnf->response_code &&
	    HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code
	    )
		{
			Reset_all_data();
			sig_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &sig_data);
			SCI_TRACE_LOW("[cameraroll]camera_roll_download_thumbnail_req_func error code MSG_CAMERA_ROLL_ERROR_NOTIFY %d" , cnf->response_code);
        	
		return FALSE;
	}

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_download_thumbnail_req_func");     
		
	if(302 ==cnf->response_code)
	{
		SCI_TRACE_LOW("[cameraroll]camera_roll_download_thumbnail_req_func  302 , start get files ");
		return TRUE;
	}
		
	if(HTTP_RESPONSE_CODE_NOT_FOUND !=cnf->response_code)
	{
		file_dev = MMI_DEVICE_SDCARD;
		file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
		if (file_dev < MMI_DEVICE_NUM)
		{
			default_path = MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS ;//MMIDC_GetPhotoDefaultPathName();        
		}	

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(FALSE == bIsFound)
		{
			SCI_TRACE_LOW("[cameraroll] Enter camera_roll_download_thumbnail_req_func context not found !!!");     
			return FALSE;
		}

		MMIAPICOM_StrToWstr((uint8*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , img_file_name);

		MMIAPIFMM_CombineFullPathEx(save_path, MMIFILE_FULL_PATH_MAX_LEN,file_dev,default_path, img_file_name);  

		file_handle = MMIAPIFMM_CreateFile(save_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_ret = MMIAPIFMM_WriteFile(
				file_handle, 
				cnf->data_ptr, 
				cnf->data_len, 
				&write_size, 
				NULL
				);

			if(SFS_ERROR_NONE == file_ret)
			{
				ret = TRUE;
			}
			MMIAPIFMM_CloseFile(file_handle);

			s_download_thumbnail_success_count++;
			sig_data.success_num = s_download_thumbnail_success_count;
			sig_data.total_num = s_download_thumbnail_total_count;
			sig_data.fail_num = s_download_thumbnail_fail_count;
			sig_data.bDone = TRUE;
				
			SCI_MEMCPY((void*)sig_data.filename , save_path , MMIAPICOM_Wstrlen(save_path) * sizeof(wchar));
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF , &sig_data);
		}
	}
	else
	{
		SCI_TRACE_LOW("[cameraroll]camera_roll_download_thumbnail_req_func  404 get !!!");
		s_download_thumbnail_fail_count++;
		sig_data.success_num = s_download_thumbnail_success_count;
		sig_data.total_num = s_download_thumbnail_total_count;
		sig_data.fail_num = s_download_thumbnail_fail_count;
		sig_data.bDone = FALSE;
		SCI_MEMCPY((void*)sig_data.filename , save_path , MMIAPICOM_Wstrlen(save_path) * sizeof(wchar));
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF , &sig_data);
	}

	if(s_current_download_thumbnail_count == (s_download_thumbnail_success_count + s_download_thumbnail_fail_count))
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_download_thumbnail_req_func  download thumbnail done !!!");
		s_current_download_thumbnail_count =0;
		s_is_process_download_thumbnail = FALSE;

		
		if((ONE_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) && 
		    (MMISRV_CAMERAROLL_Need_manual_auto_upload_photo() ||
		     MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()))
		{
			MMISRV_CAMERAROLL_exec_auto_upload_photo();
		}
	}
	else
	{
        	s_download_thumbnail_timer =  MMK_CreateTimerCallback(1000 * 40, HandleDownloadThumbnail_timer, PNULL, FALSE);
	}

	return TRUE;
#else
    return FALSE;
#endif
}


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_send_session_http_req(void)
{
	uint8 i=0 , avaliable_request_idx =0 , http_idx=0 , context_id;
	BOOLEAN bIs_found = FALSE;
	uint64 buff_size_ary[4] = {150*1024 , 100*1024 , 50*1024 , 10*1024};

	SCI_TRACE_LOW("[cameraroll] ENTER camera_roll_send_session_http_req error code");

	Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);	
	
	   
		context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);

		if(0xFF == context_id)
		{
			SCI_TRACE_LOW("[cameraroll] ENTER camera_roll_send_session_http_req (0xFF == avaliable_request_idx)");
			return 0;
		}

		if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
		   ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION;
		}
		else
		{
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_UPLOAD_SESSION;
		}
		
		bIs_found = FALSE;
		
		if(PNULL ==s_upload_photo_ary.buf_ptr)
			{
			for(i=0;i < 4; i++)
				{

				s_upload_photo_ary.buf_ptr = SCI_ALLOC_APPZ(buff_size_ary[i]);
				if(PNULL !=s_upload_photo_ary.buf_ptr)
					{
						bIs_found  = TRUE;
					s_upload_photo_ary.buf_size= buff_size_ary[i];
					SCI_TRACE_LOW("[cameraroll] ENTER camera_roll_send_session_http_req s_upload_photo_ary.buf_size %d" , s_upload_photo_ary.buf_size);
					break;
					}
					else
						continue;
				}
		}
				else
				{
					//compose post request 
			SCI_MEMSET(s_upload_photo_ary.buf_ptr , 0x00 , s_upload_photo_ary.buf_size);
					bIs_found  = TRUE;
				}


		if(bIs_found)
		{
			MMISRV_CAMERAROLL_upload_session(context_id , avaliable_request_idx , s_onedrive_upload_photo_ptr);
	}
	//}

	Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_close_session_http_req(void)
{
	uint8 avaliable_request_idx =0 , context_id;
	//send close session !!!!!
	Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
	context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
	Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	if(0xFF == context_id)
		return FALSE;

	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status ||
	   ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status )
	{
		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION;
	}
	else
	{
		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = CAMERA_ROLL_OPER_CLOSE_SESSION;
	}
	
	MMISRV_CAMERAROLL_close_session(context_id , avaliable_request_idx , s_onedrive_upload_photo_ptr);
	SCI_TRACE_LOW("[cameraroll] camera_roll_upload_session_req_func send close session !!!!");

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_create_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint8 i=0 , avaliable_request_idx =0 , http_idx=0;
	BOOLEAN bIs_found = FALSE;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};

#ifdef DEBUG_REFRESH
	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[cameraroll]camera_roll_create_session_req_func refresh token !!!");
		Reset_Http_all_request();
		MMIAPIMSA_refresh_token(MSA_APP_TYPE_CAMERA_ROLL ,MMISRV_CAMERAROLL_Get_Token);	
		s_cameraroll_resume_operation = CAMERA_ROLL_OPER_CREATE_SESSION ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_create_session_req_func"); 

	if(HTTP_RESPONSE_CODE_CREATED != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{		
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
	              SCI_TRACE_LOW("[cameraroll]camera_roll_create_session_req_func error code MSG_CAMERA_ROLL_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
			Reset_all_data();	  
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		}
		return FALSE;
	}

	if(s_b_is_cancel_upload_photo)
	{
              SCI_TRACE_LOW("[cameraroll]camera_roll_create_session_req_func upload has been canceled !!");
		s_b_is_cancel_upload_photo = FALSE;
		return FALSE;
	}

	// get session id
	if(PNULL != cnf->rsp_header_info.session_id_ptr)
		strncpy(s_onedrive_upload_photo_ptr->session_id , cnf->rsp_header_info.session_id_ptr , strlen(cnf->rsp_header_info.session_id_ptr));

	//prepare file info 
	if (s_onedrive_upload_photo_ptr->file_handle != 0)
	{
            MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
            s_onedrive_upload_photo_ptr->file_handle = 0;
	}

	s_onedrive_upload_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_onedrive_upload_photo_ptr->file_path , SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/
	if (s_onedrive_upload_photo_ptr->file_handle != 0)
	{
		//get file size
		s_onedrive_upload_photo_ptr->total_file_size =  s_onedrive_upload_photo_ptr->remain_file_size =MMIAPIFMM_GetFileSize(s_onedrive_upload_photo_ptr->file_handle);
		s_onedrive_upload_photo_ptr->uploaded_file_size = s_onedrive_upload_photo_ptr->sended_file_size= 0;

		//start upload processing ...
		camera_roll_send_session_http_req();
	}
	else
	{
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_UNKNOWN;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		SCI_TRACE_LOW("[cameraroll]camera_roll_create_session_req_func ERROR s_onedrive_upload_photo_ptr->file_handle == 0 !!!!");
		Remove_File_From_UploadFileAry(s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len);
	}	

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_upload_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	BOOLEAN bIsFound = FALSE;

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_upload_session_req_func"); 

	SCI_MEMSET((void*)s_upload_photo_ary.buf_ptr, 0x00 , s_upload_photo_ary.buf_size);

	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			SCI_TRACE_LOW("[cameraroll] camera_roll_upload_session_req_func error code MSG_CAMERA_ROLL_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
			MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
			Reset_all_data();
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);		
		}
		return FALSE;
	}

	if(s_b_is_cancel_upload_photo)
	{
              SCI_TRACE_LOW("[cameraroll]camera_roll_upload_session_req_func upload has been canceled !!");
		s_b_is_cancel_upload_photo = FALSE;
		return FALSE;
	}

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		if(cnf->context_id == http_request_operation_array[i].context_id)
		{
			bIsFound = TRUE;
			break;
		}			
	}

	if(FALSE == bIsFound)
	{
		SCI_TRACE_LOW("[cameraroll] Enter camera_roll_upload_session_req_func context not found !!!");     
		return FALSE;
	}

	s_onedrive_upload_photo_ptr->uploaded_file_size += http_request_operation_array[i].operation_ary[cnf->app_instance].upload_buf_size;
	if(0 < s_onedrive_upload_photo_ptr->remain_file_size)
	{
		camera_roll_send_session_http_req();
	}
	else
	{
		camera_roll_close_session_http_req();
	}

	camera_roll_msg_data.total_file_size = s_onedrive_upload_photo_ptr->total_file_size; 
	camera_roll_msg_data.uploaded_file_size = s_onedrive_upload_photo_ptr->uploaded_file_size ;

	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY , &camera_roll_msg_data);


	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_close_session_req_func(uint8 app_param, void * param)
{
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	MMISRV_CAMERAROLL_UPLOAD_INFO_T * tmp_ptr = PNULL;
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	FILEARRAY_DATA_T file_data = {0};
	MMIFILE_HANDLE      file_handle =   0;    

	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			SCI_TRACE_LOW("[cameraroll]camera_roll_close_session_req_func error code MSG_CAMERA_ROLL_ERROR_NOTIFY %d" , cnf->rsp_header_info.response_code);
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
			Reset_all_data();
		}
		return FALSE;
	}	
	
	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_close_session_req_func"); 
	
	MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);	

	SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
	file_data.type = FILEARRAY_TYPE_FILE;
	MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len, s_onedrive_upload_photo_ptr->file_path_len);/*lint !e670 */
	file_data.name_len = s_onedrive_upload_photo_ptr->file_path_len;
	MMIAPIFILEARRAY_Add (s_onedrive_upload_success_file_array, &file_data);
	MMIAPIFILEARRAY_SaveArrayToFileEx(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));

	camera_roll_msg_data.bDone = TRUE;
	MMI_WSTRNCPY (camera_roll_msg_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len, s_onedrive_upload_photo_ptr->file_path_len);/*lint !e670 */
	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF , &camera_roll_msg_data);

	//if(s_is_in_gallery)
	{
		if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()||
		   MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo())
		{
			SCI_TRACE_LOW("[cameraroll] camera_roll_auto_upload_close_session_req_func start auto upload !!!"); 
			MMISRV_CAMERAROLL_exec_auto_upload_photo();
		}
		else
		{
			s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_IDLE ;
			Clean_upload_data_info();
		}
	}

	s_is_need_to_refresh_thumbnail = TRUE;

	SCI_FREE(s_onedrive_upload_photo_ptr);
	s_onedrive_upload_photo_ptr = PNULL;

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_create_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint32 data_len = 0;
	uint8 *data_ptr = PNULL;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	uint8 i=0 , avaliable_request_idx =0 , http_idx=0;
	BOOLEAN bIs_found = FALSE;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_auto_upload_create_session_req_func"); 

	if(HTTP_RESPONSE_CODE_CREATED != cnf->rsp_header_info.response_code)
	{

		SCI_TRACE_LOW("[cameraroll]camera_roll_auto_upload_create_session_req_func ERROR code %d" , cnf->rsp_header_info.response_code);

		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{		
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			Reset_all_data();	  
		}
		return FALSE;
	}

	// get session id

	if(PNULL != cnf->rsp_header_info.session_id_ptr)
	{
		strncpy(s_onedrive_upload_photo_ptr->session_id , cnf->rsp_header_info.session_id_ptr , strlen(cnf->rsp_header_info.session_id_ptr));
		SCI_TRACE_LOW("[cameraroll]camera_roll_auto_upload_create_session_req_func session_id %s" , s_onedrive_upload_photo_ptr->session_id);
	}
	else

	{
		SCI_TRACE_LOW("[cameraroll]camera_roll_auto_upload_create_session_req_func session_id not exist !!!");
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		Reset_all_data();	  
		return FALSE;
	}

	//prepare file info 
	if (s_onedrive_upload_photo_ptr->file_handle != 0)
	{
            MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
            s_onedrive_upload_photo_ptr->file_handle = 0;
	}
    
	s_onedrive_upload_photo_ptr->file_handle = MMIAPIFMM_CreateFile(s_onedrive_upload_photo_ptr->file_path , SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL, NULL);/*lint !e655*/
	if (s_onedrive_upload_photo_ptr->file_handle != 0)
	{
		//get file size
		s_onedrive_upload_photo_ptr->total_file_size =  s_onedrive_upload_photo_ptr->remain_file_size =MMIAPIFMM_GetFileSize(s_onedrive_upload_photo_ptr->file_handle);
		s_onedrive_upload_photo_ptr->uploaded_file_size = s_onedrive_upload_photo_ptr->sended_file_size= 0;

		//start upload processing ...
		camera_roll_send_session_http_req();
	}
	else
	{
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_UNKNOWN;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		SCI_TRACE_LOW("[cameraroll]camera_roll_auto_upload_create_session_req_func ERROR s_onedrive_upload_photo_ptr->file_handle == 0 !!!!");
		Remove_File_From_UploadFileAry(s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len);
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_session_req_func(uint8 app_param, void * param)
{
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	BOOLEAN bIsFound = FALSE;

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_auto_upload_session_req_func"); 

	SCI_MEMSET((void*)s_upload_photo_ary.buf_ptr, 0x00 , s_upload_photo_ary.buf_size);


	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);		
       	 	SCI_TRACE_LOW("[cameraroll]camera_roll_auto_upload_session_req_func error code %d" , cnf->rsp_header_info.response_code);
			MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
			Reset_all_data();
		}
		return FALSE;
	}

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		if(cnf->context_id == http_request_operation_array[i].context_id)
		{
			bIsFound = TRUE;
			break;
		}			
	}

	if(FALSE == bIsFound)
	{
		SCI_TRACE_LOW("[cameraroll] Enter camera_roll_upload_session_req_func context not found !!!");     
		return FALSE;
	}

	s_onedrive_upload_photo_ptr->uploaded_file_size += http_request_operation_array[i].operation_ary[cnf->app_instance].upload_buf_size;
	if(0 < s_onedrive_upload_photo_ptr->remain_file_size)
	{
		camera_roll_send_session_http_req();
	}
	else
	{
		camera_roll_close_session_http_req();
	}

	camera_roll_msg_data.total_file_size = s_onedrive_upload_photo_ptr->total_file_size; 
	camera_roll_msg_data.uploaded_file_size = s_onedrive_upload_photo_ptr->uploaded_file_size ;

	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY , &camera_roll_msg_data);
	

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_auto_upload_close_session_req_func(uint8 app_param, void * param)
{
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	MMISRV_CAMERAROLL_UPLOAD_INFO_T * tmp_ptr = PNULL;
	HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)param;	
	uint8 i=0 , avaliable_request_idx =0;
	//FILEARRAY upload_success_file_array = PNULL;
	FILEARRAY_DATA_T file_data = {0};
	MMIFILE_HANDLE      file_handle =   0;    
	uint32 upload_file_size = 0;
	
	if(HTTP_RESPONSE_CODE_OK != cnf->rsp_header_info.response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->rsp_header_info.response_code)
		{
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);		
			SCI_TRACE_LOW("camera_roll_close_session_req_func error code %d" , cnf->rsp_header_info.response_code);
			MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
			Reset_all_data();
		}
		return FALSE;
	}	
	
	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_auto_upload_close_session_req_func"); 

	MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);	
	s_onedrive_upload_photo_ptr->file_handle = 0;

	SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
	file_data.type = FILEARRAY_TYPE_FILE;
	MMI_WSTRNCPY (file_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len, s_onedrive_upload_photo_ptr->file_path_len);/*lint !e670 */
	file_data.name_len = s_onedrive_upload_photo_ptr->file_path_len;
	MMIAPIFILEARRAY_Add (s_onedrive_upload_success_file_array, &file_data);
	MMIAPIFILEARRAY_SaveArrayToFileEx(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));

	MMIAPIFILEARRAY_Delete(s_current_upload_file_array , 0);
	MMIAPIFILEARRAY_SaveArrayToFileEx(s_current_upload_file_array , s_current_upload_file_path ,MMIAPICOM_Wstrlen(s_current_upload_file_path));
	
	camera_roll_msg_data.bDone = TRUE;
	MMI_WSTRNCPY (camera_roll_msg_data.filename, MMIFILE_FULL_PATH_MAX_LEN,
	  				s_onedrive_upload_photo_ptr->file_path, s_onedrive_upload_photo_ptr->file_path_len, s_onedrive_upload_photo_ptr->file_path_len);/*lint !e670 */
	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF , &camera_roll_msg_data);
	

	SCI_FREE(s_onedrive_upload_photo_ptr);
	s_onedrive_upload_photo_ptr = PNULL;

	if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo() || 
		MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo())
        {
            MMISRV_CAMERAROLL_exec_auto_upload_photo();
        }
        else
        {
		SCI_TRACE_LOW("[cameraroll] camera_roll_auto_upload_close_session_req_func No photp need to be uploaded !!!"); 
		s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_IDLE;
		if(s_is_in_gallery)
		{
			Clean_upload_data_info();
		}
		else
		{
			MMISRV_DeleteCameraRollTaskThread	();
		}
        }

	s_is_need_to_refresh_thumbnail = TRUE;
	
        return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_create_share_link_req_func(uint8 app_param, void * param)
{
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_CAMERAROLL_JSON_PARSE_DATA input_data = {0};

	if(HTTP_RESPONSE_CODE_CREATED != cnf->response_code &&  HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			SCI_TRACE_LOW("camera_roll_create_share_link_req_func error code %d" , cnf->response_code);
			Reset_all_data();
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		}
		return FALSE;
	}	
	
	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_create_share_link_req_func"); 

	SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func cnf->data_len %d" , cnf->data_len);
	
	Clean_meta_data_info();	
	if(PNULL == cnf->data_ptr || 0 ==  cnf->data_len)
	{
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
				return 0;
	}

	camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_ShareLink_CallBack,&input_data);
	jp_ptr = new_JSON_parser(&jc); 
	if(PNULL == jp_ptr)
	{
	    SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func PNULL == jp_ptr");
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_JSON_ERROR;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
	    return FALSE;
	}
	if(0 == camera_roll_ParseJsonChar(jp_ptr , cnf->data_ptr , cnf->data_len) && FALSE == input_data.bDone)
	{
	    SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func parse error!");
	}
	delete_JSON_parser(jp_ptr); 

	if(TRUE == input_data.bDone && (0 < strlen(input_data.name)))
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func share link success");
		
		camera_roll_msg_data.url_len =  strlen(input_data.name);
		
		if(CAMERA_ROLL_MAX_STRING_LEN < camera_roll_msg_data.url_len)
		{
			SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func share link size overflow !!!!!");
		}
		else
		{
		camera_roll_msg_data.bDone = TRUE;
			strncpy(camera_roll_msg_data.url , input_data.name , camera_roll_msg_data.url_len);
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_SHARE_LINK_CNF , &camera_roll_msg_data);
			SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func share link = %s" , camera_roll_msg_data.url);
		}
	}

	if((ONE_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) &&
	   (MMISRV_CAMERAROLL_Need_manual_auto_upload_photo() ||
	    MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()))
	{
		MMISRV_CAMERAROLL_exec_auto_upload_photo();
	}


        return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_cameraroll_info_req_func(uint8 app_param, void * param)
{

	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_CAMERAROLL_JSON_PARSE_FOLDER_ID_INFO input_data = {0};
	MMISRV_CAMERAROLL_ONEDRIVEINFO_T onedrive_info = {0};
	int retVal = 0;
		
	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			SCI_TRACE_LOW("camera_roll_get_cameraroll_info_req_func error code %d" , cnf->response_code);
			Reset_all_data();
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		}
		return FALSE;
	}

	if(PNULL == cnf->data_ptr || 0 ==  cnf->data_len)
	{
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return 0;
	}

	retVal = MMIAPIMSA_Update_UserInfo(cnf->data_ptr , cnf->data_len);

	if(TRUE == retVal)
		MMISRV_CAMERAROLL_exec_auto_upload_photo();
	else
	{
		SCI_TRACE_LOW("camera_roll_get_cameraroll_info_req_func get user info FAIL !!!");
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_JSON_ERROR;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return 0;		
	}

	return TRUE;

}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_get_delta_token_info_req_func(uint8 app_param, void * param)
{
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)param;			
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
	   
	BOOLEAN bHas_photo_delete = FALSE;

	//s_is_process_get_delta_link = FALSE;
	
	if(HTTP_RESPONSE_CODE_OK != cnf->response_code)
	{
		if(HTTP_RESPONSE_CODE_NOT_FOUND == cnf->response_code)
		{
			camera_roll_msg_data.curr_download_num = 0;
			camera_roll_msg_data.total_num = 0 ;
			
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY , &camera_roll_msg_data);
			s_is_process_get_delta_link = FALSE;
		}
		else if(HTTP_RESPONSE_CODE_UNAUTHORIZED != cnf->response_code)
		{
			SCI_TRACE_LOW("camera_roll_get_delta_token_info_req_func error code %d" , cnf->response_code);
			Reset_all_data();
			camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		}
		return FALSE;
	}	
	
	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_delta_token_info_req_func"); 

	SCI_TRACE_LOW("[cameraroll] camera_roll_get_delta_token_info_req_func cnf->data_len %d" , cnf->data_len);
	
	if(PNULL == cnf->data_ptr || 0 ==  cnf->data_len)
	{
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return 0;
	}

	s_is_need_to_refresh_thumbnail  |= MMISRV_CAMERAROLL_Parse_delta_info(cnf->data_ptr  , cnf->data_len , &bHas_photo_delete);

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_get_delta_token_info_req_func s_is_need_to_refresh_thumbnail = %d" , s_is_need_to_refresh_thumbnail); 

	MMISRV_CAMERAROLL_start_get_thumbnail_info();

        return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_download_photo_req_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};	


	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_download_photo_req_func"); 

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[cameraroll]: Enter camera_roll_download_photo_req_func has been canceled !!");	
		Clean_download_data_info();
			return FALSE;
	}

#ifdef DEBUG_REFRESH
	if(s_set_refresh_flag)
	{
		SCI_TRACE_LOW("[cameraroll] camera_roll_download_photo_req_func refresh token !!!");
		Reset_Http_all_request();
		MMIAPIMSA_refresh_token(MSA_APP_TYPE_CAMERA_ROLL ,MMISRV_CAMERAROLL_Get_Token);	
		s_cameraroll_resume_operation = CAMERA_ROLL_OPER_DOWNLOAD_PHOTO ; 
		s_set_refresh_flag = FALSE;
		return FALSE;
	}
#endif

	{
		data_ind_sig = (HTTP_DATA_IND_SIG_T*)param;
		if(HTTP_RESPONSE_CODE_OK != data_ind_sig->response_code && 302 !=data_ind_sig->response_code)
		{
			if(HTTP_RESPONSE_CODE_UNAUTHORIZED != data_ind_sig->response_code)
			{
				SCI_TRACE_LOW("[cameraroll] Enter camera_roll_download_photo_req_func  MSG_CAMERA_ROLL_ERROR_NOTIFY ");
				Reset_all_data();
				camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
				MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
			}
			MMIAPIFMM_CloseFile(s_current_download_ptr->file_handle);
			MMIAPIFMM_DeleteFile(s_current_download_ptr->file_path , PNULL);
			SCI_FREE(s_current_download_ptr);
			s_current_download_ptr = PNULL;
			SCI_TRACE_LOW("[cameraroll]camera_roll_download_photo_req_func error code MSG_CAMERA_ROLL_ERROR_NOTIFY %d" , data_ind_sig->response_code);
			return FALSE;
		}

		if(302 ==data_ind_sig->response_code)
		{
			SCI_TRACE_LOW("[cameraroll]camera_roll_download_photo_req_func  302 , start get files ");
			return TRUE;
		}

		if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(s_current_download_ptr->file_handle,data_ind_sig->data_ptr, data_ind_sig->data_len,&write_size,NULL))
		{
			s_current_download_ptr->saved_file_size +=  data_ind_sig->data_len;
			MMIAPIFMM_SetFilePointer(s_current_download_ptr->file_handle, s_current_download_ptr->saved_file_size, SFS_SEEK_BEGIN);
			SCI_TRACE_LOW("[cameraroll]camera_roll_download_photo_req_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile s_current_download_ptr->saved_file_size = %d " , s_current_download_ptr->saved_file_size);
			SCI_TRACE_LOW("[cameraroll]camera_roll_download_photo_req_func  SFS_ERROR_NONE == MMIAPIFMM_WriteFile s_current_download_ptr->total_file_size = %d " , s_current_download_ptr->total_file_size);						
		}
		camera_roll_msg_data.total_file_size = s_current_download_ptr->total_file_size;
		camera_roll_msg_data.downloaded_file_size = s_current_download_ptr->saved_file_size;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_NOTIFY , &camera_roll_msg_data);
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL uint8    camera_roll_download_photo_cnf_func(uint8 app_param, void * param)
{
	HTTP_DATA_IND_SIG_T *data_ind_sig = PNULL;
       uint32   write_size      = 0;  
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};	

	SCI_TRACE_LOW("[cameraroll] Enter camera_roll_download_photo_cnf_func"); 

	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[cameraroll]: Enter camera_roll_download_photo_cnf_func has been canceled !!");
		MMIAPIFMM_CloseFile(s_current_download_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_ptr);
		s_b_is_cancel_download_photo = FALSE;
			return FALSE;
		}

		MMIAPIFMM_CloseFile(s_current_download_ptr->file_handle);
		SCI_FREE(s_current_download_ptr);
		s_current_download_ptr = PNULL;

		camera_roll_msg_data.bDone = TRUE;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF , &camera_roll_msg_data);

	if((ONE_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) &&
	    (MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()||
	     MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()))
	{
		MMISRV_CAMERAROLL_exec_auto_upload_photo();
	}	

	return TRUE;
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRV_CAMERAROLL_HandleHTTPMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    //void    *original_param = (void*)((uint32)param-sizeof(xSignalHeaderRec));
	void    *original_param = (void*)((uint32)param);	
	uint8 i=0;
	BOOLEAN bIsFound = FALSE;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	MMISRV_CAMERAROLL_MSG_T sig_data={0};	
	uint8 operation = 0;

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
		{
			HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
			if(cnf->module_id != g_current_module_id || cnf->app_param >= MAX_HTTP_CONTEXT_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			if( 0 != http_request_operation_array[cnf->app_param].context_id)
			{
				SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_InitHttpRequest request too much context , idx %d , context_id %d , close it !", cnf->app_param , http_request_operation_array[cnf->app_param].context_id);
				HTTP_CloseRequest(http_request_operation_array[cnf->app_param].context_id);
			}

			SCI_MEMSET((void*)&http_request_operation_array[cnf->app_param] , 0x00 , sizeof(MMISRV_CAMERAROLL_REQUEST_ARRAY_T));
			http_request_operation_array[cnf->app_param].operation_ary[0].operation= CAMERA_ROLL_OPER_NO_OPERATION;
			http_request_operation_array[cnf->app_param].operation_ary[1].operation= CAMERA_ROLL_OPER_NO_OPERATION;
			http_request_operation_array[cnf->app_param].context_id   = cnf->context_id;
			http_request_operation_array[cnf->app_param].app_instance = 0;
			http_request_operation_array[cnf->app_param].module_id    = cnf->module_id;

			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF index %d , context_id %d",cnf->app_param , cnf->context_id);

			if(!s_cameraroll_task_init_done)
			{
				s_cameraroll_task_init_done = TRUE;
				SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF s_auto_upload_status %d ",s_auto_upload_status);			

				if(CAMERA_ROLL_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO != s_cameraroll_init_from)
				{
					sig_data.init_type = s_cameraroll_init_from;
					MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_INIT_CNF , &sig_data);		
					SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF SEND MSG_CAMERA_ROLL_INIT_CNF !!!");			
				}
				else
				{
					SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_INIT_CNF MMISRV_CAMERAROLL_exec_auto_upload_photo !!!");					  
					MMISRV_CAMERAROLL_exec_auto_upload_photo();
				}
			}
			result = MMI_RESULT_TRUE;
		}
        break;

    case HTTP_SIG_NEED_AUTH_IND:
		{
			MMI_RESULT_E result = MMI_RESULT_TRUE;
			HTTP_AUTH_IND_SIG_T *cnf = (HTTP_AUTH_IND_SIG_T *)original_param;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND");        

			s_cameraroll_resume_operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;

			// Cancel first 
			Reset_Http_all_request();
			s_is_process_download_thumbnail = FALSE;	   
			s_is_process_get_delta_link = FALSE;			
			
			MMIAPIMSA_refresh_token(MSA_APP_TYPE_CAMERA_ROLL ,MMISRV_CAMERAROLL_Get_Token);	

			result = MMI_RESULT_TRUE;
		}
        break;

    case HTTP_SIG_REQUEST_ID_IND:
	{
	    MMI_RESULT_E result = MMI_RESULT_TRUE;
	    HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T *)original_param;

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
		return MMI_RESULT_FALSE;
		}

		http_request_operation_array[i].operation_ary[cnf->app_instance].request_id = cnf->request_id;		
	}
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND");
		result = MMI_RESULT_TRUE;
        break;

    case HTTP_SIG_GET_CNF:
		{
			HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)original_param;
			
			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
            {
                return MMI_RESULT_FALSE;
            }

	              SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_GET_CNF");

			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;
				
			if(PNULL != http_cnf_function[operation])
			{
				http_cnf_function[operation](i , cnf);
			}
			
			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_GET_CNF index %d , error_code = %d , response code = %d" , i , result , cnf->rsp_header_info.response_code);        
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);
			http_request_operation_array[i].operation_ary[cnf->app_instance].operation = CAMERA_ROLL_OPER_NO_OPERATION;
		}
        break;

    case HTTP_SIG_HEAD_CNF:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_HEAD_CNF");        
        break;

    case HTTP_SIG_DATA_IND:
		{
			HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;	
			uint32 data_len = 0;
			uint8 *data_ptr = PNULL;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}

			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;

			if(HTTP_RESPONSE_CODE_UNAUTHORIZED == cnf->response_code)
			{
				SCI_TRACE_LOW("[cameraroll]: token expired !!! , need to refresh token"); 
				return MMI_RESULT_TRUE; // need refresh token
			}

			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_DATA_IND index %d" , i); 

			if(PNULL !=http_data_ind_function[operation])
			http_data_ind_function[operation](i , cnf);						       
		}
        break;

    case HTTP_SIG_HEADER_IND:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_HEADER_IND");
        break;

    case HTTP_SIG_ERROR_IND:
	{
		HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;
		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}

		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
			return MMI_RESULT_FALSE;
		}		

		//HTTP_CloseRequest(cnf->context_id);
		SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_ERROR_IND , MSG_CAMERA_ROLL_ERROR_NOTIFY result = %d" , cnf->result);

		if((PNULL != s_current_download_ptr) && (SFS_INVALID_HANDLE != s_current_download_ptr->file_handle))
		{
			MMIAPIFMM_CloseFile(s_current_download_ptr->file_handle);
			MMIAPIFMM_DeleteFile(s_current_download_ptr->file_path , PNULL);
			SCI_FREE(s_current_download_ptr);
			s_current_download_ptr = PNULL;
		}

		Reset_all_data();
		sig_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;	
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &sig_data);
	}
        break;

    case HTTP_SIG_CLOSE_CNF:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_CLOSE_CNF");
        break;

    case HTTP_SIG_CANCEL_CNF:
	{
	        HTTP_CANCEL_CNF_SIG_T *cnf = (HTTP_CANCEL_CNF_SIG_T*)original_param;		
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_CANCEL_CNF");

		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			if(cnf->context_id == http_request_operation_array[i].context_id)
			{
				bIsFound = TRUE;
				break;
			}			
		}
		if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
		{
			return MMI_RESULT_FALSE;
		}	

		SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_CANCEL_CNF index %d " , i );        
		SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
		SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);
                http_request_operation_array[i].operation_ary[cnf->app_instance].operation = CAMERA_ROLL_OPER_NO_OPERATION;
    	}
        break;

    case HTTP_SIG_NET_PROG_IND:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_NET_PROG_IND");
        break;

    case HTTP_SIG_REDIRECT_IND:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_REDIRECT_IND");        
        break;

    case HTTP_SIG_POST_CNF:
		{
			HTTP_POST_CNF_SIG_T *cnf = (HTTP_POST_CNF_SIG_T*)original_param;
			uint8 operation = 0;

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				if(cnf->context_id == http_request_operation_array[i].context_id)
				{
					bIsFound = TRUE;
					break;
				}			
			}
			
			if(!bIsFound || cnf->app_instance >= MAX_REQUEST_NUM)
			{
				return MMI_RESULT_FALSE;
			}

			SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_POST_CNF");   
			
			operation = http_request_operation_array[i].operation_ary[cnf->app_instance].operation;
					
			if(PNULL !=http_cnf_function[operation])
				http_cnf_function[operation](i , cnf);

			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
			SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[cnf->app_instance].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);
                        http_request_operation_array[i].operation_ary[cnf->app_instance].operation = CAMERA_ROLL_OPER_NO_OPERATION;
		}
        break;

#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND");
        break;
#endif

    case MSG_MS_ACCOUNT_SIGNOFF_CNF:
	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_SIGNOFF_CNF , &sig_data);
	 break;

    default:
        SCI_TRACE_LOW("[cameraroll]HandleHttpMsg Not handle msg_id = %x");
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : parse json char one by one
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int camera_roll_ParseJsonChar(JSON_parser jp_ptr , uint8* data_ptr , uint32 data_len)
{
    uint32 offset = 0;
    int  c = -1;
    int parse_res = 0;

    SCI_TRACE_LOW("[cameraroll]camera_roll_ParseJsonChar ENTER");
    
    for(offset = 0; offset < data_len; offset++)
    {
        c = (*(data_ptr+offset))& 0x000000ff;
        parse_res = JSON_parser_char(jp_ptr, c);
        if(0 == parse_res)
        {            
            SCI_TRACE_LOW("[cameraroll]camera_roll_ParseJsonChar break !! jp_ptr->error = %d " , JSON_parser_get_last_error(jp_ptr));
	     break;
        }
    }
    return parse_res;
}

/*****************************************************************************/
//  Description : set weather info flag by json_tag
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Download_thumbnail_CallBack(void* ctx, int type, const JSON_value* value)
{
#ifdef MMI_SHOW_MSA
   	 int res = 1;

	MMISRV_CAMERAROLL_JSON_PARSE_DATA * camera_roll_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)ctx;	
	uint8 i=0;
	BOOLEAN bIs_Found = FALSE;
	uint8 avaliable_request_idx ;
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA *remove_ptr = PNULL;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;	   
	uint8 ret_val = 0;
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA * tmp_ptr = PNULL;
	
    switch(type) 
    {
    case JSON_T_INTEGER:        // 5
	if(!camera_roll_data_ptr->bFileExist)
		MMISRV_CAMERAROLL_GetJsonInteger(ctx,value);
        break;
        
    case JSON_T_FLOAT:   
		break;
    case JSON_T_STRING:         // 10
	if (JSON_TAG_NAME == camera_roll_data_ptr->cur_tag)
	{
		if(MAX_THUMBNAIL_DOWNLOAD_NUM <= s_current_download_thumbnail_count)
		{
			camera_roll_data_ptr->bDone = TRUE;
			return FALSE;
		}

		file_dev = MMI_DEVICE_SDCARD;
		file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
		if (file_dev < MMI_DEVICE_NUM)
		{
			default_path = MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS ;//MMIDC_GetPhotoDefaultPathName();        
		}	

		MMIAPICOM_StrToWstr((uint8*)(((*value).vu).str).value , img_file_name);
		MMIAPIFMM_CombineFullPathEx(save_path, MMIFILE_FULL_PATH_MAX_LEN,file_dev,default_path, img_file_name); 	

		if(TRUE == MMIAPIFMM_IsFileExist(save_path ,MMIAPICOM_Wstrlen(save_path)))
		{		
			// file already exist , skip it
			camera_roll_data_ptr->bFileExist = TRUE;
		}
		else
		{
		//new file need to be downloaded
		camera_roll_data_ptr->bFileExist = FALSE;


		s_current_download_thumbnail_count ++ ;

		if(PNULL == s_onedrive_metadata_ptr)
		{
			s_onedrive_metadata_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_CAMERAROLL_ONEDRIVE_METADATA));
			SCI_ASSERT(PNULL != s_onedrive_metadata_ptr);
		}
		else
		{
			tmp_ptr = s_onedrive_metadata_ptr;
			while(PNULL != tmp_ptr->next_ptr)
			tmp_ptr = tmp_ptr->next_ptr;

			tmp_ptr->next_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_CAMERAROLL_ONEDRIVE_METADATA));
			SCI_ASSERT(PNULL != tmp_ptr->next_ptr);
			}
			MMISRV_CAMERAROLL_GetJsonString(ctx,value);			
		}
	}
	else
	{
		if(!camera_roll_data_ptr->bFileExist && JSON_TAG_NONE != camera_roll_data_ptr->cur_tag)
		{
			SCI_TRACE_LOW("JsonParse_CallBack get string ");
			MMISRV_CAMERAROLL_GetJsonString(ctx,value);
		}
	}
	break;
    case JSON_T_KEY:            // 11
		{
		if(Handle_JSON_Is_Tag_Equal(JSON_TAG_ID , value))
				{
		        camera_roll_data_ptr->cur_tag = JSON_TAG_ID;
		}   
		else if(Handle_JSON_Is_Tag_Equal(JSON_TAG_NAME , value))
					{
			camera_roll_data_ptr->cur_tag = JSON_TAG_NAME;
						camera_roll_data_ptr->bFileExist = FALSE;
		}
		else if(Handle_JSON_Is_Tag_Equal(JSON_TAG_SIZE , value))
		{
			camera_roll_data_ptr->cur_tag = JSON_TAG_SIZE;
				}
		else
		{
			camera_roll_data_ptr->cur_tag = JSON_TAG_NONE;
			}
		}
        break;
    
    default:
        res = -1;
        break;
    }
    return res;
#else
    return -1;
#endif
}
/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Onedrive_photo_Num_CallBack(void* ctx, int type, const JSON_value* value)
{
    int res = 1;
    uint16 str_len = 0;

	MMISRV_CAMERAROLL_JSON_PARSE_DATA * camera_roll_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)ctx;	
    
    switch(type) 
    {
    case JSON_T_KEY:            // 11
        if(Handle_JSON_Is_Tag_Equal(JSON_TAG_ID , value))
        {
	        camera_roll_data_ptr->size ++;
        }   
		break;
    default:
        res = -1;
        break;
    } 
    return res;
    }


/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_ShareLink_CallBack(void* ctx, int type, const JSON_value* value)
{
    int res = 1;
	uint16 str_len = 0;

	MMISRV_CAMERAROLL_JSON_PARSE_DATA * camera_roll_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)ctx;	
	
    switch(type) 
    {
        case JSON_T_STRING:         // 10
	if (JSON_TAG_WEB_URL == camera_roll_data_ptr->cur_tag)
		{
		camera_roll_data_ptr->bDone = TRUE;
		str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN,(((*value).vu).str).length);
		strncpy(camera_roll_data_ptr->name , (((*value).vu).str).value , str_len);
		SCI_TRACE_LOW("[cameraroll] camera_roll_create_share_link_req_func JSON_TAG_WEB_URL if Found !!");
		res = 0; // break JSON !!!!
	}
	break;
	case JSON_T_KEY:            // 11
	if(Handle_JSON_Is_Tag_Equal(JSON_TAG_WEB_URL , value))
	{
	        camera_roll_data_ptr->cur_tag = JSON_TAG_WEB_URL;
	}   
	else
	{
		camera_roll_data_ptr->cur_tag = JSON_TAG_NONE;
	}
	break;
	default:
	res = -1;
	break;
	} 
    return res;
}

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_DeltaInfo_CallBack(void* ctx, int type, const JSON_value* value)
{
	int res = 1;
	uint16 str_len = 0;

	MMISRV_CAMERAROLL_JSON_PARSE_DELTA_INFO * camera_roll_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DELTA_INFO *)ctx;	
    
    switch(type) 
    {
	case JSON_T_STRING:         // 10
	if (JSON_TAG_DELTA_LINK == camera_roll_data_ptr->cur_tag)
	{
		str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN,(((*value).vu).str).length);
		strncpy(camera_roll_data_ptr->deltaLink , (((*value).vu).str).value , str_len);
		SCI_TRACE_LOW("[cameraroll] MMISRV_CAMERAROLL_JsonParse_DeltaInfo_CallBack JSON_TAG_DELTA_LINK is Found !!");
	}
	else if(JSON_TAG_NAME == camera_roll_data_ptr->cur_tag)
	{
		SCI_MEMSET((void*)camera_roll_data_ptr->name, 0x00 , CAMERA_ROLL_MAX_STRING_LEN+1);
		str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN,(((*value).vu).str).length);
		strncpy(camera_roll_data_ptr->name , (((*value).vu).str).value , str_len);
		}
	break;
	break;
    case JSON_T_KEY:            // 11
	if(Handle_JSON_Is_Tag_Equal(JSON_TAG_DELTA_LINK , value))
	{
	        camera_roll_data_ptr->cur_tag = JSON_TAG_DELTA_LINK;
	}   
	else if(Handle_JSON_Is_Tag_Equal(JSON_TAG_NAME , value))
        {
	        camera_roll_data_ptr->cur_tag = JSON_TAG_NAME;
		camera_roll_data_ptr->bNeed_update_meta = TRUE;
	}	
	else if(Handle_JSON_Is_Tag_Equal(JSON_TAG_DELETED , value))
	{
		FILEARRAY_DATA_T array_item = {0};
		wchar img_file_name[CAMERA_ROLL_MAX_STRING_LEN+1] = {0};
		
		MMIAPICOM_StrToWstr(camera_roll_data_ptr->name , img_file_name);

		MMIAPICOM_Wstrncpy(array_item.filename, img_file_name,  MMIAPICOM_Wstrlen(img_file_name));
		array_item.name_len = MMIAPICOM_Wstrlen(img_file_name);

		array_item.type = FILEARRAY_TYPE_FILE;
		MMIAPIFILEARRAY_Add(camera_roll_data_ptr->delete_photo_ary , &array_item);
		SCI_TRACE_LOW("[cameraroll]:ENTER Delete_old_onedrive_photo_task SOME PHOTO in Onedrive has been deleted  name %s =" , camera_roll_data_ptr->name); 		
        }   
	else
	{
		camera_roll_data_ptr->cur_tag = JSON_TAG_NONE;
	}
		break;
    default:
        res = -1;
        break;
    } 
    return res;

}


/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Handle_JSON_Is_Tag_Equal(uint8 index , const JSON_value* value)
    {
	if(0 == strncmp((char*)s_camera_roll_json_tag_arr[index], (char*)(((*value).vu).str).value, (((*value).vu).str).length))
        {
		return TRUE;
	}

	return FALSE;
        }

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint8 Generate_Download_thumbnail_node_by_JSON( const uint8* data_ptr,uint32 data_len )
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_CAMERAROLL_JSON_PARSE_DATA input_data = {0};

	//delete meta data first 
    Clean_meta_data_info();
    s_current_download_thumbnail_count = 0;
    camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_Download_thumbnail_CallBack,&input_data);

    jp_ptr = new_JSON_parser(&jc); 
	
    if(PNULL == jp_ptr)
    {
        SCI_TRACE_LOW("[cameraroll]  Generate_Download_thumbnail_node_by_JSON PNULL == jp_ptr");
	//	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
        return FALSE;
}
    if(0 == camera_roll_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
    {
        //weather_data.is_bad_data = TRUE;
        SCI_TRACE_LOW("[cameraroll]  Generate_Download_thumbnail_node_by_JSON parse error!");
    }
    delete_JSON_parser(jp_ptr); 

     SCI_TRACE_LOW("[cameraroll]  Generate_Download_thumbnail_node_by_JSON  s_current_download_thumbnail_count = %d" , s_current_download_thumbnail_count);

    return s_current_download_thumbnail_count;
}


/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Fetch_PhotoInfo_CallBack(void* ctx, int type, const JSON_value* value)
{
    int res = 1;
    uint16 str_len = 0;

	MMISRV_CAMERAROLL_JSON_PARSE_DATA * camera_roll_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)ctx;	
        
    switch(type) 
    {
    case JSON_T_INTEGER:        // 5
       if(camera_roll_data_ptr->bDone)
       {
        camera_roll_data_ptr->size = ((*value).vu.integer_value);
		res = 0; // break JSON !!!!
       }
		break;
    case JSON_T_STRING:         // 10
	 if(JSON_TAG_NAME ==camera_roll_data_ptr->cur_tag)
		{
		str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN,(((*value).vu).str).length);

		SCI_MEMSET(camera_roll_data_ptr->name , 0x00 ,CAMERA_ROLL_MAX_STRING_LEN + 1);
		strncpy(camera_roll_data_ptr->name , (((*value).vu).str).value , str_len);
	 }
	 else if (JSON_TAG_ID ==camera_roll_data_ptr->cur_tag)
	 {
		if(0 == MMIAPICOM_StrCmp(camera_roll_data_ptr->name  , strlen(camera_roll_data_ptr->name ) , s_onedrive_photo_info_src_name , strlen(s_onedrive_photo_info_src_name)))
		{
		        SCI_TRACE_LOW("[cameraroll]  MMISRV_CAMERAROLL_JsonParse_Fetch_PhotoInfo_CallBack Find Files !!");
			camera_roll_data_ptr->bDone = TRUE;
			str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN,(((*value).vu).str).length);
			strncpy(camera_roll_data_ptr->id , (((*value).vu).str).value , str_len);
		}
		}
		break;
    case JSON_T_KEY:            // 11
        if(Handle_JSON_Is_Tag_Equal(JSON_TAG_ID , value))
        {
	        camera_roll_data_ptr->cur_tag = JSON_TAG_ID;
        }
	 else if (Handle_JSON_Is_Tag_Equal(JSON_TAG_NAME , value))
        {
        	camera_roll_data_ptr->cur_tag = JSON_TAG_NAME;
        }
	 else if (Handle_JSON_Is_Tag_Equal(JSON_TAG_SIZE , value))
        {
        	camera_roll_data_ptr->cur_tag = JSON_TAG_SIZE;
        }
	 else
	 {
        	camera_roll_data_ptr->cur_tag = JSON_TAG_NONE;
	 }
        break;
    
    default:
        res = -1;
        break;
    } 
    return res;
} 
/*****************************************************************************/
//  Description : init json config
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void camera_roll_InitjsonConfig(JSON_config* config_ptr,
                                    JSON_parser_callback callback,
                                    MMISRV_CAMERAROLL_JSON_PARSE_DATA *record_data_ptr)
{
    if(PNULL == config_ptr)
    {
        SCI_TRACE_LOW("[cameraroll]camera_roll_InitjsonConfig PNULL == config_ptr");
        return;
    }
    init_JSON_config(config_ptr);

    config_ptr->callback = callback;
    config_ptr->allow_comments = TRUE;
    config_ptr->handle_floats_manually = TRUE;
    config_ptr->callback_ctx = record_data_ptr;
}

/*****************************************************************************/
//  Description : get json integer 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_GetJsonInteger(void * user_data,const JSON_value* j_value)
{
    MMISRV_CAMERAROLL_JSON_PARSE_DATA * parse_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)user_data;
    uint16                   str_len = 0;
    MMISRV_CAMERAROLL_ONEDRIVE_METADATA *  tmp_ptr = PNULL;
	uint8 * current_tag_ptr = PNULL;
	
    if(PNULL == j_value || PNULL == parse_data_ptr || PNULL == s_onedrive_metadata_ptr)
    {
        SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_GetJsonString param error!");
        return;
    }
	
	tmp_ptr = s_onedrive_metadata_ptr;
	while(PNULL != tmp_ptr->next_ptr)
		tmp_ptr = tmp_ptr->next_ptr;
	
	SCI_TRACE_LOW("[cameraroll]---oneDrive: need add  = %d---",((*j_value).vu.integer_value));
	
	if(JSON_TAG_SIZE == parse_data_ptr->cur_tag)
	{       
		tmp_ptr->size = ((*j_value).vu.integer_value);
	}
}
/*****************************************************************************/
//  Description : get json string 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_GetJsonString(void * user_data,const JSON_value* j_value)
{
    MMISRV_CAMERAROLL_JSON_PARSE_DATA * parse_data_ptr = (MMISRV_CAMERAROLL_JSON_PARSE_DATA *)user_data;
    uint16                   str_len = 0;
    MMISRV_CAMERAROLL_ONEDRIVE_METADATA *  tmp_ptr = PNULL;
	uint8 * current_tag_ptr = PNULL;

    if(PNULL == j_value || PNULL == parse_data_ptr || PNULL == s_onedrive_metadata_ptr)
    {
        SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_GetJsonString param error!");
        return;
    }
    SCI_TRACE_LOW("[cameraroll]---oneDrive : MMISRV_CAMERAROLL_GetJsonString = %s---",(((*j_value).vu).str).value);

	tmp_ptr = s_onedrive_metadata_ptr;
	while(PNULL != tmp_ptr->next_ptr)
		tmp_ptr = tmp_ptr->next_ptr;

	str_len = MIN(ONEDRIVE_METADATA_STRING_MAX_LEN,(((*j_value).vu).str).length);

	current_tag_ptr = SCI_ALLOC_APPZ(str_len + 1);
	strncpy(current_tag_ptr , (((*j_value).vu).str).value , str_len);

    {
        SCI_TRACE_LOW("[cameraroll]---oneDrive: need add  = %s---",(((*j_value).vu).str).value);
        
        switch(parse_data_ptr->cur_tag)
        {
        case JSON_TAG_ID:
            {       
				tmp_ptr->id = current_tag_ptr;
            }
            break;
		case JSON_TAG_NAME:
			{
				 tmp_ptr->name = current_tag_ptr;
			}
			break;                
        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
LOCAL void camera_roll_download_thumbnail_req(uint8 context_id , uint8  index)
{
    uint8 avaliable_request_idx ;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
		   
	get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));

	memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  //modify for coverity
	
	get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	get_param_ptr->need_net_prog_ind = FALSE;

	get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	get_param_ptr->need_net_prog_ind = FALSE;
	get_param_ptr->is_use_relative_url = TRUE;
	get_param_ptr->recv_timeout = 60*1000; //5 min 

	len=strlen(MS_ONEDRIVE_GET_PHOTO_ITEMS_URL)+strlen(http_request_operation_array[context_id].operation_ary[index].id_string)+strlen("/thumbnails/0/c240x320/content");
	get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(get_param_ptr->uri.uri_ptr,0,len+1);
	sprintf(get_param_ptr->uri.uri_ptr , "%s%s%s" , MS_ONEDRIVE_GET_THUMBNAIL_URL, http_request_operation_array[context_id].operation_ary[index].id_string , "/thumbnails/0/c240x320/content");

	get_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_GET_HEADER_NUM;
	get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
	memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);

	len=strlen(MS_ONEDRIVE_AUTHORIZATION);
	get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

	len=strlen(s_camera_roll_access_token_str);
	get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);

	len=strlen(MS_ONEDRIVE_X_TARGET_NAME);
	get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_TARGET_NAME);

	len=strlen(MS_ONEDRIVE_X_TARGET_VALUE);
	get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
	memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
	strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_TARGET_VALUE);
	
	if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, index))
	{
		SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_GetRequest OK !!!!");
	}

	SCI_FREE(get_param_ptr->uri.uri_ptr);
	FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
	SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
	get_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(get_param_ptr);

    return;	
}



/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRV_CAMERAROLL_start_download_thumbnails(void)
{
#ifdef MMI_SHOW_MSA
	uint i=0;
	BOOLEAN bIs_Found = FALSE;
    uint8 avaliable_request_idx , context_id;
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA *remove_ptr = PNULL;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar img_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	wchar save_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint32 save_path_len = 0;	   
	uint8 ret_val = 0;

	if(PNULL == s_onedrive_metadata_ptr)
		return 0 ;
	
	file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS ;//MMIDC_GetPhotoDefaultPathName();        
	}	

	while(s_onedrive_metadata_ptr)
	{
		Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
		context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
		Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

		if(0xFF != context_id)
		{
			//copy photo name
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation = CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL;
			strncpy((char*)&http_request_operation_array[context_id].operation_ary[avaliable_request_idx].info_string[0] , s_onedrive_metadata_ptr->name , strlen(s_onedrive_metadata_ptr->name));
			
			//copy photo id
			http_request_operation_array[context_id].operation_ary[avaliable_request_idx].id_strlen = MIN(CAMERA_ROLL_MAX_ID_LEN , strlen(s_onedrive_metadata_ptr->id));
			strncpy((char*)&http_request_operation_array[context_id].operation_ary[avaliable_request_idx].id_string[0] , s_onedrive_metadata_ptr->id , http_request_operation_array[context_id].operation_ary[avaliable_request_idx].id_strlen);

                     camera_roll_download_thumbnail_req(context_id , avaliable_request_idx);
			ret_val ++ ; 
			remove_ptr = s_onedrive_metadata_ptr;
			s_onedrive_metadata_ptr = s_onedrive_metadata_ptr->next_ptr;
			
			if(remove_ptr->id) SCI_FREE(remove_ptr->id);
			if(remove_ptr->name)SCI_FREE(remove_ptr->name);
			if(remove_ptr->thumbnail_url)SCI_FREE(remove_ptr->thumbnail_url);
			SCI_FREE(remove_ptr);			
		}
		else
		{
			Clean_meta_data_info();
			SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_start_download_thumbnails -- too much photos , need 2nd round to finish it ");
			break;
		}
	}

	return ret_val;
#else
    return 0;
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

    index = s_cameraroll_account_index[dual_sys]; 
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

    index = s_cameraroll_account_index[dual_sys]; 
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

    index = s_cameraroll_account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    
    return password_str;
}

/*****************************************************************************/
//  Description : pdp retry if it can't active
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CameraRollPDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    uint8                setting_index = 0;

    setting_index = MMIAPICONNECTION_GetAccountDefaultIndex(sim_sys, MMICONNECTION_ACCOUNT_BROWSER);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, setting_index);
    if(setting_item_ptr)
    {
        active_info.app_handler = g_current_module_id;
        active_info.apn_ptr = (char*)setting_item_ptr->apn;
        active_info.user_name_ptr = (char*)setting_item_ptr->username;
        active_info.psw_ptr = (char*)setting_item_ptr->password;
        active_info.dual_sys = sim_sys;
        active_info.priority = setting_index;
        active_info.ps_service_rat = MN_UNSPECIFIED;
        active_info.handle_msg_callback = HandlePdpMsg;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = setting_item_ptr->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
            s_is_process_pdp_activate = TRUE;
        }
        SCI_TRACE_LOW("[cameraroll]:CameraRollPDPTry return_val = %d",return_val);	
    }	
    return return_val;
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void) 
{
   SCI_TRACE_LOW("[cameraroll]:StopPdpActiveTimer");

    if(0 != s_cameraroll_pdp_timer_id)
    {
        MMK_StopTimer(s_cameraroll_pdp_timer_id);
        s_cameraroll_pdp_timer_id = 0;
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    MMISRV_CAMERAROLL_MSG_T sig_data={0};
    
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- msg_id=%d",msg_ptr->msg_id);
    SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- result=%d" , msg_ptr->result);

    s_is_process_pdp_activate = FALSE;

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        {
            SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- MMIPDP_ACTIVE_CNF");
            StopPdpActiveTimer();
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                s_cameraroll_net_id = msg_ptr->nsapi;
                SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg  MMIPDP_RESULT_SUCC s_cameraroll_net_id %d " , s_cameraroll_net_id);
                MMISRV_CAMERAROLL_InitHttpRequest(g_current_module_id);
            }
            else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
            {
                MMIAPIPDP_Deactive(g_current_module_id);
                if(0 != s_cameraroll_pdp_retry_timer_id)
                {
                        MMK_StopTimer(s_cameraroll_pdp_retry_timer_id);
                        s_cameraroll_pdp_retry_timer_id = 0;
                }
                s_cameraroll_pdp_retry_timer_id = MMK_CreateTimerCallback(
                        2000, 
                        HandlePdpRetryTimer, 
                        PNULL, 
                        FALSE
                        );
            }
            else 
            {
                SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- FAIL to ACTIVATE PDP , RESET Cameraroll TASK !!");
                s_cameraroll_net_id = 0;
                MMIAPIPDP_Deactive(g_current_module_id);
                MMISRV_CAMERAROLL_Check_Network_status(FALSE);            
            }
        }        
        break;
        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- MMIPDP_DEACTIVE_CNF");        
            s_cameraroll_net_id = 0;
        break;
        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        {
            SCI_TRACE_LOW("[cameraroll]:HandlePdpMsg--- MMIPDP_DEACTIVE_IND");
            s_cameraroll_net_id = 0;
            MMIAPIPDP_Deactive(g_current_module_id);
            MMISRV_CAMERAROLL_Check_Network_status(FALSE);
        }
        break;

        default:
        break;
    }
    
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
    MMISRV_CAMERAROLL_MSG_T sig_data={0};
	
    SCI_TRACE_LOW("[cameraroll]:HandlePdpRetryTimer begin");

	if( 0 < s_cameraroll_pdp_retry_count)	
		s_cameraroll_pdp_retry_count --;
	else
	{
	       SCI_TRACE_LOW("[cameraroll]:HandlePdpRetryTimer - PDP activate FAIL !!");
		MMIAPIPDP_Deactive(g_current_module_id);
		MMISRV_CAMERAROLL_Check_Network_status(FALSE);
		return ; 
	}	
	
	if(timer_id == s_cameraroll_pdp_retry_timer_id && 0 != s_cameraroll_pdp_retry_timer_id)
	{
	    MMK_StopTimer(s_cameraroll_pdp_retry_timer_id);
	    s_cameraroll_pdp_retry_timer_id = 0;
	}
	
	if(!CameraRollPDPTry(s_cameraroll_cur_sys_num ))
	{
		s_cameraroll_pdp_retry_timer_id = MMK_CreateTimerCallback(
	                                            1500, 
	                            		        HandlePdpRetryTimer, 
	                            		        PNULL, 
	                                		    FALSE
	                                		    );
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
    SCI_TRACE_LOW("[cameraroll]:HandlePdpActiveTimer---Time out error!---");
    MMIAPIPDP_Deactive(g_current_module_id);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void)
{
    if(0 != s_cameraroll_pdp_timer_id)
    {
        MMK_StopTimer(s_cameraroll_pdp_timer_id);
        s_cameraroll_pdp_timer_id = 0;
    }
    
    s_cameraroll_pdp_timer_id = MMK_CreateTimerCallback(20000, 
                            HandlePdpActiveTimer, 
                            PNULL, 
                            FALSE);
}

/*****************************************************************************/
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleDownloadThumbnail_timer(uint8  timer_id, uint32 param)
{
	int i=0 , j=0;
	MMISRV_CAMERAROLL_MSG_T sig_data={0};	

	if (0 != s_download_thumbnail_timer)
	{
		MMK_StopTimer(s_download_thumbnail_timer);
		s_download_thumbnail_timer = 0;

		if(0 != s_current_download_thumbnail_count)
		{
			//some download thumbnail is expired , so reset all settings
                     SCI_TRACE_LOW("[cameraroll]:Enter HandleDownloadThumbnail_timer to reset download request ");
			//Reset_all_data();
			Clean_meta_data_info();

			for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
			{
				for(j=0;j<MAX_REQUEST_NUM;j++)
				{
					if((CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO  == http_request_operation_array[i].operation_ary[j].operation) || 
					    (CAMERA_ROLL_OPER_GET_THUMBNAIL_URL  == http_request_operation_array[i].operation_ary[j].operation)  ||
					    (CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL  == http_request_operation_array[i].operation_ary[j].operation)
				   	)
					{
						HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
						SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
						SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);	
					}
				}			
			}

			s_current_download_thumbnail_count = 0;
			s_is_process_download_thumbnail = FALSE;
			s_is_process_get_delta_link = FALSE;

			sig_data.error_type = ONE_DRIVE_ERROR_HTTP_ERROR;
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &sig_data);
			
			if((ONE_DRIVE_AUTO_UPLOAD_IDLE ==s_auto_upload_status) && 
			    (MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()||
		            MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()))
			{
				MMISRV_CAMERAROLL_exec_auto_upload_photo();
			}
		}
	}	
}

/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    SCI_TRACE_LOW("[cameraroll]:Enter MMISRV_CAMERAROLL_PdpActive");

    if(0 != s_cameraroll_net_id)
    {
        SCI_TRACE_LOW("[cameraroll]:Enter MMISRV_CAMERAROLL_PdpActive s_cameraroll_net_id is exist %d" , s_cameraroll_net_id);
        MMISRV_CAMERAROLL_InitHttpRequest(g_current_module_id);
        return TRUE;
    }
    else if (s_is_process_pdp_activate) // already process pdp activate before !!!!
    {
        SCI_TRACE_LOW("[cameraroll]:Enter MMISRV_CAMERAROLL_PdpActive s_is_process_pdp_activate is TRUE , wait for callback function !");
        return TRUE;
    }  
    
#ifdef WIFI_SUPPORT    
    //优先wifi
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = g_current_module_id;
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
        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        BOOLEAN bIsFound = FALSE;

        bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sim_ok);

        if(FALSE == bIsFound)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
            return FALSE;
        }
		
        s_cameraroll_pdp_retry_count = 2;
        s_cameraroll_cur_sys_num = sim_ok;
		
        return_val = CameraRollPDPTry(s_cameraroll_cur_sys_num);//第一张有效卡
        if(FALSE == return_val) 
        {
		if(0 != s_cameraroll_pdp_retry_timer_id)
		{
		    MMK_StopTimer(s_cameraroll_pdp_retry_timer_id);
		    s_cameraroll_pdp_retry_timer_id = 0;
		}
		
		s_cameraroll_pdp_retry_timer_id = MMK_CreateTimerCallback(
			3000, 
			HandlePdpRetryTimer, 
			PNULL, 
			FALSE
		);
        }
    }
    
    return return_val;
}


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Upload_single_photo(uint16 * file_path , uint16 file_path_len)
{
	MMIAPIMSA_USER_INFO_T *user_info = PNULL;
	FILEARRAY single_upload_ary = PNULL;
       FILEARRAY_DATA_T array_item = {0};
	SFS_DATE_T  modify_date = {0};
	SFS_TIME_T  modify_time = {0};
	
	if(PNULL == file_path || 0 == file_path_len)
		return FALSE;

	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Upload_single_photo");

	single_upload_ary = MMIAPIFILEARRAY_Create();
	
	if(PNULL== single_upload_ary)
		return FALSE;
	   
	//check auto upload status , if YES , pause first 
	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		SCI_TRACE_LOW("[cameraroll]: Pause auto upload !!!");
		Reset_all_data();
	}
	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Upload_single_photo add file to ary");
	MMIAPICOM_Wstrncpy(array_item.filename, file_path, file_path_len);
	array_item.name_len = file_path_len;
	MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, 
		&array_item.size, &modify_date, &modify_time);
       
	array_item.modify_date.mday = modify_date.mday;
	array_item.modify_date.mon = modify_date.mon;
	array_item.modify_date.year = modify_date.year;

	array_item.modify_time.sec = modify_time.sec;
	array_item.modify_time.min = modify_time.min;
	array_item.modify_time.hour = modify_time.hour;

	array_item.type = FILEARRAY_TYPE_FILE;
	MMIAPIFILEARRAY_Add(single_upload_ary, &array_item);

	MMIAPIFILEARRAY_Combine(single_upload_ary , s_onedrive_manual_upload_file_array);	

	MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);
	s_onedrive_manual_upload_file_array = single_upload_ary;
	   
	MMIAPIFILEARRAY_SaveArrayToFile(s_onedrive_manual_upload_file_array, s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));

	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Upload_single_photo current manual upload array size = %d " , MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array));

	s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
	MMISRV_CAMERAROLL_exec_auto_upload_photo();

	return TRUE;
}
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Check_auto_upload_photo(void)
{
	uint8 i = 0 , j=0;

	if(ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK == s_auto_upload_status ||
	   !(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL) && 
	      MMISRV_CAMERAROLL_Get_Connect_State()))
		return;

	if(TRUE == s_is_in_gallery)
	{
		for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
		{
			for(j=0;j<MAX_REQUEST_NUM;j++)
			{
				if(CAMERA_ROLL_OPER_NOT_INIT != http_request_operation_array[i].operation_ary[j].operation&&
				    CAMERA_ROLL_OPER_NO_OPERATION != http_request_operation_array[i].operation_ary[j].operation)
				{
				       SCI_TRACE_LOW("[cameraroll] MMISRV_CAMERAROLL_Check_auto_upload_photo some operation is processing in gallery , don't trigger auto upload !!");
					return;
				}
			}
		}		
	}

	//Can start auto upload !!!!!
	if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo())
        {
            if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING !=s_auto_upload_status ||
            (CAMERA_ROLL_OPER_NO_OPERATION == MMISRV_CAMERAROLL_Check_Current_auto_upload_request())
            )
            {
                SCI_TRACE_LOW("[cameraroll] ========= MMISRV_CAMERAROLL_Check_auto_upload_photo NEED auto upload photos  =================");
                s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
                MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO);
            }
	}else if(MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo())
	{
            if(ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING !=s_auto_upload_status ||
            (CAMERA_ROLL_OPER_NO_OPERATION == MMISRV_CAMERAROLL_Check_Current_auto_upload_request())
            )
            {
                SCI_TRACE_LOW("[cameraroll] ========= MMISRV_CAMERAROLL_Check_auto_upload_photo NEED auto upload photos  =================");
                s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING;
                MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO);
            }
	}
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_exec_auto_upload_photo(void)
{
    	SCI_TIME_T time = {0};
	SCI_DATE_T date = {0};
	uint32 sec = 0;
	wchar *file_path = PNULL;
	uint16 snapshot_upload_file_size = 0 , upload_file_success_size =  0 , manual_upload_file_size =0 ,i=0, j = 0 , current_upload_size=0;
	FILEARRAY_DATA_T upload_file_data = {0};
	FILEARRAY_DATA_T manual_upload_file_data = {0};
	FILEARRAY_DATA_T upload_success_file_data = {0};
	BOOLEAN b_already_uploaded = FALSE;
	FILEARRAY modified_auto_upload_ary = PNULL;
	MMIAPIMSA_USER_INFO_T * user_info =	MMIAPIMSA_get_user_info();
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};	
    
	if( 0 == strlen(user_info->id))
	{
		
            SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_exec_auto_upload_photo camera roll id doesn't exist  !!!");
            //MMISRV_CAMERAROLL_Get_cameraroll_info_req();
            camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_UNKNOWN;	
            MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
            return ;
	}

	snapshot_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_snapshot_upload_file_array);
	upload_file_success_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_upload_success_file_array);
	manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);

	if(0 == manual_upload_file_size && 0 == snapshot_upload_file_size)
	{
		SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_exec_auto_upload_photo camera roll no photo needs to be upload !!");
		return;
	}

	if(manual_upload_file_size > 0) 
	{
		s_current_upload_file_array = s_onedrive_manual_upload_file_array;
		s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING ; 
		s_current_upload_file_path = s_onedrive_manual_upload_list_file_path ; 
		current_upload_size = manual_upload_file_size;
	}
	else
	{
		s_current_upload_file_array = s_onedrive_snapshot_upload_file_array;
		s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING ; 
		s_current_upload_file_path = s_onedrive_snapshot_upload_list_file_path ; 
		current_upload_size = snapshot_upload_file_size ;
	}

	if(manual_upload_file_size > 0 && snapshot_upload_file_size > 0)
	{
		modified_auto_upload_ary = MMIAPIFILEARRAY_Create();
		
		for(i=0; i < snapshot_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_onedrive_snapshot_upload_file_array , i ,&upload_file_data);

			for(j=0; j < manual_upload_file_size ; j++)
			{
				MMIAPIFILEARRAY_Read(s_onedrive_manual_upload_file_array , j ,&manual_upload_file_data);
				
				if(!((manual_upload_file_data.name_len == upload_file_data.name_len) && 
				      ( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
				   )
				{
					MMIAPIFILEARRAY_Add(modified_auto_upload_ary , &upload_file_data);
				}
			}
		}
		
		MMIAPIFILEARRAY_SaveArrayToFileEx(modified_auto_upload_ary , s_onedrive_snapshot_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_snapshot_upload_list_file_path));	
		MMIAPIFILEARRAY_Destroy(&modified_auto_upload_ary);		
	}

		if(!MMIAPIFILEARRAY_Read(s_current_upload_file_array , 0 ,&upload_file_data))
		{
			SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_exec_auto_upload_photo MMIAPIFILEARRAY_Read s_onedrive_snapshot_upload_file_array fail !!!");
			return;
		}

	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_exec_auto_upload_photo start upload photo !!");	
		   
		if(PNULL == s_onedrive_upload_photo_ptr)
		{
			s_onedrive_upload_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_CAMERAROLL_UPLOAD_INFO_T));
			SCI_ASSERT(PNULL != s_onedrive_upload_photo_ptr);
			//tmp_ptr = s_onedrive_upload_photo_ptr;
		}
		else
		{
			SCI_MEMSET((void*)s_onedrive_upload_photo_ptr , 0x00 ,sizeof(MMISRV_CAMERAROLL_UPLOAD_INFO_T) );
		}

	       // save file path 
	       SCI_MEMCPY(s_onedrive_upload_photo_ptr->file_path , upload_file_data.filename , sizeof(uint16)*upload_file_data.name_len);
		s_onedrive_upload_photo_ptr->file_path_len = upload_file_data.name_len;
	       
		//create name 
	       TM_GetSysTime(&time);
		TM_GetSysDate(&date);

		time.hour = (0 == time.hour) ? 12 : time.hour;
		time.min = (0 == time.min) ? 60 : time.min;
		time.sec = (0 == time.sec) ? 60 : time.sec;
		
		sprintf((char*)s_onedrive_upload_photo_ptr->file_name,"%d%d%d%d%d-%d.jpg",date.year, date.mon,date.mday,time.hour,time.min,time.sec);
		s_onedrive_upload_photo_ptr->file_name_len = strlen(s_onedrive_upload_photo_ptr->file_name);

		//sprintf((char*)s_onedrive_upload_photo_ptr->upload_url,"%s%s%s%s%s%s","https://cid-" , onedrive_info.user_id ,  ".users.storage.live.com/items/" , onedrive_info.folder_id, "/" , s_onedrive_upload_photo_ptr->file_name);
		sprintf((char*)s_onedrive_upload_photo_ptr->upload_url,"%s%s%s%s","https://storage.live.com/users/0x" , user_info->id ,  "/jump/wmphotos/", s_onedrive_upload_photo_ptr->file_name);
	       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Upload_single_photo !! tmp_ptr->file_name %s" , s_onedrive_upload_photo_ptr->file_name);	
		   
		MMISRV_CAMERAROLL_start_upload_file();
	}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Enter_Gallery(void)
{
       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Enter_Gallery !!");	
	s_is_in_gallery = TRUE;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_In_Gallery(void)
{
	return s_is_in_gallery ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Leave_Gallery(void)
{
        SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Leave_Gallery !!");
	s_is_in_gallery = FALSE;

	if(ONE_DRIVE_AUTO_UPLOAD_IDLE == s_auto_upload_status && 
	    !(s_is_process_download_thumbnail ||s_is_process_get_delta_link))
	{
	       SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Leave_Gallery MMISRV_DeleteCameraRollTaskThread  !!");
		MMISRV_DeleteCameraRollTaskThread();
	}
}

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Upload_multiple_photos(void)
{
       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Upload_multiple_photos !!");	


	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING != s_auto_upload_status)
	{
		s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING;
		MMISRV_CAMERAROLL_exec_auto_upload_photo();
	}	

	return TRUE;
}
/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Set_Upload_multiple_photos(FILEARRAY file_array)
{
       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Set_Upload_multiple_photos !!");	
	MMIAPIFILEARRAY_Combine(s_onedrive_manual_upload_file_array,file_array);			
	MMIAPIFILEARRAY_SaveArrayToFile(s_onedrive_manual_upload_file_array, s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
	return TRUE;
}


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Download_Original_photo(uint16 * file_path , uint16 file_path_len)
{
    uint16       w_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};          
    uint16      w_file_name_len=0;      
	uint8       file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};          
	uint16      file_name_len=0 , src_file_path , str_len;  
	uint16 w_file_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0} ;
    uint32 read_size = 0 , file_size =0;	
	uint8 * buff = PNULL;
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	BOOLEAN retVal = FALSE;
	
	if(PNULL==file_path || 0==file_path_len  ||s_b_is_cancel_download_photo)
    {
	    SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Download_Original_photo --- (PNULL==file_path || 0==file_path_len ||s_b_is_cancel_download_photo)"); 
	    return FALSE;
    }

    SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Download_Original_photo !!");	

	//check auto upload status , if YES , pause first 
	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status || 
           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status
		)
	{
		Reset_all_data();
	}
	
    src_file_path = file_path_len;
	SCI_MEMCPY((void*)w_file_path , file_path , sizeof(wchar)*file_path_len);

	MMIAPIFMM_SplitLastName(w_file_path, &file_path_len, 
                     w_file_name, &w_file_name_len);  

    MMI_WSTRNTOSTR(file_name,MMIFILE_FULL_PATH_MAX_LEN,w_file_name,MMIFILE_FILE_NAME_MAX_LEN, MMIFILE_FILE_NAME_MAX_LEN);
	
	SCI_MEMSET((void*)&s_onedrive_photo_json_parse_info , 0x00 , sizeof(MMISRV_CAMERAROLL_JSON_PARSE_DATA));
	SCI_MEMSET((void*)s_onedrive_photo_info_src_name , 0x00 , CAMERA_ROLL_MAX_STRING_LEN+1);

	str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN, strlen(file_name));

	strncpy(s_onedrive_photo_info_src_name , file_name , str_len);	
	
	SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Download_Original_photo re-create meta info , filename %s" , s_onedrive_photo_info_src_name);

        if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        {
			file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
			if(SFS_INVALID_HANDLE != file_handle)
			{
				file_size = MMIAPIFMM_GetFileSize(file_handle);
				buff = SCI_ALLOC(file_size);
				if(PNULL == buff)
                {
					SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Download_Original_photo --- (PNULL == buff)"); 
				return FALSE;
                }

                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    buff, 
                    file_size, 
                    &read_size, 
                    NULL
                    );
				
				if(SFS_ERROR_NONE == file_ret)
				{
					SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Download_Original_photo meta update success !!");
				}
				
				MMIAPIFMM_CloseFile(file_handle);

				//start parse JSON
			camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_Fetch_PhotoInfo_CallBack,&s_onedrive_photo_json_parse_info);
				
				jp_ptr = new_JSON_parser(&jc); 
				
				if(PNULL == jp_ptr)
				{
					SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Download_Original_photo MSG_CAMERA_ROLL_ERROR_NOTIFY PNULL == jp_ptr");
					return FALSE;
				}
			if(0 == camera_roll_ParseJsonChar(jp_ptr , buff , file_size) && !s_onedrive_photo_json_parse_info.bDone)
				{
					//weather_data.is_bad_data = TRUE;
					SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Download_Original_photo parse error!");
				}
				delete_JSON_parser(jp_ptr); 
				SCI_FREE(buff);

        }
	}
	else
		return FALSE;


	if(s_onedrive_photo_json_parse_info.bDone)
	{		
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Download_Original_photo input_data.bDone = %d " , s_onedrive_photo_json_parse_info.bDone );

		if(PNULL == s_current_download_ptr)
		{
			s_current_download_ptr = SCI_ALLOC_APPZ(sizeof(MMISRV_CAMERAROLL_DOWNLOAD_INFO_T));
			SCI_ASSERT(PNULL != s_current_download_ptr);
		}
		else
		{
			SCI_MEMSET(s_current_download_ptr , 0x00 , sizeof(MMISRV_CAMERAROLL_DOWNLOAD_INFO_T));
		}

		//store photo info
		SCI_MEMCPY((void*)s_current_download_ptr->file_path , file_path , src_file_path * sizeof(uint16));
		s_current_download_ptr->file_path_len = src_file_path;
		s_current_download_ptr->file_handle = MMIAPIFMM_CreateFile(s_current_download_ptr->file_path, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);
		s_current_download_ptr->total_file_size =  s_onedrive_photo_json_parse_info.size;

		str_len = MIN(ONEDRIVE_METADATA_STRING_MAX_LEN, strlen(s_onedrive_photo_json_parse_info.id));
		strncpy(s_current_download_ptr->id , s_onedrive_photo_json_parse_info.id , str_len);
		
		if(SFS_INVALID_HANDLE == s_current_download_ptr->file_handle)
		{
			SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Download_photo create file handle fail  !!!!");
			return FALSE;
		}

		MMIAPIFMM_SetFilePointer(s_current_download_ptr->file_handle, 0, SFS_SEEK_BEGIN);

		retVal = MMISRV_CAMERAROLL_Download_photo();//(file_path ,src_file_path,s_onedrive_metadata_ptr);
	}

	return retVal;
}

/**************************************************************************************/
// Description : send signal to MMI Task  
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC  void MMISRV_CAMERAROLL_SendSignalToMMI(uint32 sig_id,DPARAM param)
{
	CAMERA_ROLL_SIG_T *signal = PNULL;
	
	if(PNULL == param)
	{
	    return;
	}
    
	MmiCreateSignal(sig_id, sizeof(CAMERA_ROLL_SIG_T), (MmiSignalS**)&signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_MEMCPY(&(signal->camera_roll_msg_data), param, sizeof(MMISRV_CAMERAROLL_MSG_T));
	
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void  MMISRV_CAMERAROLL_Get_Token(int result, char*  buf, uint16 buflen)

{
	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Get_Token !!");	
	
	if(result == MSA_OK)
	{
		SCI_MEMSET((void*)s_camera_roll_access_token_str , 0x00 , MAX_TOKEN_STR_SIZE + 1);
//		sprintf(s_camera_roll_access_token_str , "Bearer %s" , buf);	
		sprintf(s_camera_roll_access_token_str , "WLID1.1 t=%s" , buf);			

		//check whether this request is from refresh 
		if(s_cameraroll_resume_operation != CMAERA_ROLL_OPERAION_MAX)
		{
			switch(s_cameraroll_resume_operation)
			{
				case     CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO:
					MMISRV_CAMERAROLL_start_get_thumbnail_info();
					break;
				case     CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL:
					MMISRV_CAMERAROLL_start_download_thumbnails();
					break;
				case     CAMERA_ROLL_OPER_CREATE_SESSION:
				case CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION:
					MMISRV_CAMERAROLL_start_upload_file();
					break;
				case     CAMERA_ROLL_OPER_UPLOAD_SESSION:
				case CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION:
					camera_roll_send_session_http_req();
					break;
				case     CAMERA_ROLL_OPER_CLOSE_SESSION:
				case CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION:
				{
					//need to restart upload process !!!!
					Clean_upload_data_info();
					MMISRV_CAMERAROLL_exec_auto_upload_photo();					
				}
					break;
				case CAMERA_ROLL_OPER_DOWNLOAD_PHOTO:
					MMISRV_CAMERAROLL_Download_photo();
					break;
				case CAMERA_ROLL_OPER_CREATE_SHARE_LINK:
					MMISRV_CAMERAROLL_Send_Create_Share_Link_Req();
					break;
				case CAMERA_ROLL_OPER_GET_DELTA_TOKEN_INFO:
					MMISRV_CAMERAROLL_Get_delta_token_info_req();
					break;
				case CAMERA_ROLL_OPER_GET_CAMERAROLL_INFO:
					MMISRV_CAMERAROLL_Get_cameraroll_info_req();
					break;			
				default:
					break;
			}

			s_cameraroll_resume_operation = CMAERA_ROLL_OPERAION_MAX ; 
		}
	}
	else
	{
		SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Get_Token   result != MSA_OK !!!!");
	}
}

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Download_photo(void)//(uint16 * file_path , uint16 file_path_len, MMISRV_CAMERAROLL_ONEDRIVE_METADATA * src_meta_data)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint8 avaliable_request_idx , context_id;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	
		
	if(s_b_is_cancel_download_photo)
	{
		SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Download_photo has been canceled !!");	
		Clean_download_data_info();
		return FALSE;
	}

	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Download_photo !!");	
	
    Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
    context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
    Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	if(0xFF != context_id)
    {
		SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_Download_photo !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
		
		if(get_param_ptr == NULL)
		{
			SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Download_photo MSG_CAMERA_ROLL_ERROR_NOTIFY !!!!get_param_ptr == NULL");
			return FALSE;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_DOWNLOAD_PHOTO;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		get_param_ptr->recv_timeout = 5*60*1000; //5 min 

		len=strlen(MS_ONEDRIVE_GET_PHOTO_ITEMS_URL)+strlen(s_current_download_ptr->id)+strlen("/content");
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s%s" , MS_ONEDRIVE_GET_PHOTO_ITEMS_URL, s_current_download_ptr->id , "/content");

		get_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_GET_HEADER_NUM;
		get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
		memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
		
		len=strlen(MS_ONEDRIVE_AUTHORIZATION);
		get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);
		
		len=strlen(s_camera_roll_access_token_str);
		get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);
		
		len=strlen(MS_ONEDRIVE_X_TARGET_NAME);
		get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_name_ptr,MS_ONEDRIVE_X_TARGET_NAME);
		
		len=strlen(MS_ONEDRIVE_X_TARGET_VALUE);
		get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[1].header_value_ptr,MS_ONEDRIVE_X_TARGET_VALUE);
		
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_GetRequest OK !!!!");
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}

	return TRUE;	
}

/**************************************************************************************/
// Description : Set_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_SYNC_STATE MMISRV_CAMERAROLL_Set_Sync_State(uint8 state)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	ONE_DRIVE_SYNC_STATE state_data = state;
	
	MMINV_WRITE(MMINV_CAMERAROLL_SYNC_STATE, &state_data);

	return state_data;
}

/**************************************************************************************/
// Description : Get_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_SYNC_STATE MMISRV_CAMERAROLL_Get_Sync_State(void)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	
#ifdef MMI_SHOW_MSA
	ONE_DRIVE_SYNC_STATE state_data = CAMERA_ROLL_AUTO_SYNC;
#else
	ONE_DRIVE_SYNC_STATE state_data = CAMERA_ROLL_MANUAL_SYNC;
#endif	

	MMINV_READ(MMINV_CAMERAROLL_SYNC_STATE, &state_data, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_CAMERAROLL_SYNC_STATE, &state_data);
	}

	return state_data;
}

/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(wchar* filename , uint16 file_len)
{
	uint32 upload_file_num =0;
	uint32 i=0;
	FILEARRAY_DATA_T file_data = {0};
	
	MMISRV_CAMERAROLL_Load_Upload_File_array();
	upload_file_num = MMIAPIFILEARRAY_GetArraySize(s_onedrive_upload_success_file_array);
	for(i=0;i<upload_file_num;i++)
	{
		if(MMIAPIFILEARRAY_Read(s_onedrive_upload_success_file_array , i ,&file_data))
		{
			if(file_len == file_data.name_len)
			{
				if( 0 == MMIAPICOM_Wstrncmp(file_data.filename , filename , file_len))
				{
				       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Is_Pic_On_OneDrive FOUND !!");
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_Uploading(wchar* filename , uint16 file_len)
{
	uint32 upload_file_num =0;
	uint32 i=0;
	FILEARRAY_DATA_T file_data = {0};

	MMISRV_CAMERAROLL_Load_Upload_File_array();

	upload_file_num = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);
	for(i=0;i<upload_file_num;i++)
	{
		if(MMIAPIFILEARRAY_Read(s_onedrive_manual_upload_file_array , i ,&file_data))
		{
			if(file_len == file_data.name_len)
			{
				if( 0 == MMIAPICOM_Wstrncmp(file_data.filename , filename , file_len))
				{
				       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Is_Pic_Uploading FOUND !!");
					return TRUE;
				}
			}
		}
	}



	return FALSE;
}


/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_Auto_Uploading(wchar* filename , uint16 file_len)
{
	uint32 upload_file_num =0;
	uint32 i=0;
	FILEARRAY_DATA_T file_data = {0};

	MMISRV_CAMERAROLL_Load_Upload_File_array();
	
	upload_file_num = MMIAPIFILEARRAY_GetArraySize(s_onedrive_snapshot_upload_file_array);
	for(i=0;i<upload_file_num;i++)
	{
		if(MMIAPIFILEARRAY_Read(s_onedrive_snapshot_upload_file_array , i ,&file_data))
		{
			if(file_len == file_data.name_len)
			{
				if( 0 == MMIAPICOM_Wstrncmp(file_data.filename , filename , file_len))
				{
				       SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Is_Pic_Uploading FOUND !!");
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/**************************************************************************************/
// Description : Get_OneDrive_Pic_Url
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC char* MMISRV_CAMERAROLL_Get_OneDrive_Pic_Url(char* filename , uint16 file_len)
{
	return PNULL;
}
/**************************************************************************************/
// Description : Cancel Download
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Cancel_Download(void)
{
	s_b_is_cancel_download_photo = TRUE;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Cancel_Upload(void)
{
	s_b_is_cancel_upload_photo = TRUE;

	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status)
	{
		MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);
		s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();
		MMIAPIFILEARRAY_SaveArrayToFile(s_onedrive_manual_upload_file_array, s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
		s_auto_upload_status =ONE_DRIVE_AUTO_UPLOAD_IDLE;
	}

	Reset_all_data();
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY  MMISRV_CAMERAROLL_Create_upload_file_ary(void)
{
	s_onedrive_snapshot_list_file_array = MMIAPIFILEARRAY_Create();
	return s_onedrive_snapshot_list_file_array;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_append_upload_file(wchar * file_path , uint8 file_len)
{
	FILEARRAY_DATA_T array_item = {0};
	SFS_DATE_T  modify_date = {0};
	SFS_TIME_T  modify_time = {0};

	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_append_upload_file");
	
	MMIAPICOM_Wstrncpy(array_item.filename, file_path, file_len);
	array_item.name_len = file_len;
	MMIAPIFMM_GetFileInfo(array_item.filename, array_item.name_len, 
		&array_item.size, &modify_date, &modify_time);
    
	array_item.modify_date.mday = modify_date.mday;
	array_item.modify_date.mon = modify_date.mon;
	array_item.modify_date.year = modify_date.year;
	
	array_item.modify_time.sec = modify_time.sec;
	array_item.modify_time.min = modify_time.min;
	array_item.modify_time.hour = modify_time.hour;

	array_item.type = FILEARRAY_TYPE_FILE;
	MMIAPIFILEARRAY_Add(s_onedrive_snapshot_list_file_array, &array_item);
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Close_upload_file_ary(void)
{
	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Close_upload_file_ary");
	
	MMIAPIFILEARRAY_Combine(s_onedrive_snapshot_upload_file_array,s_onedrive_snapshot_list_file_array);			
	MMIAPIFILEARRAY_SaveArrayToFile(s_onedrive_snapshot_upload_file_array, s_onedrive_snapshot_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_snapshot_upload_list_file_path));

	MMIAPIFILEARRAY_Destroy(&s_onedrive_snapshot_list_file_array);
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_AUTO_UPLOAD_STATE_E MMISRV_CAMERAROLL_Get_Auto_upload_status(void)
{
    SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Get_Auto_upload_status status = %d" , s_auto_upload_status);
    SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Get_Auto_upload_status net_id =%d" , s_cameraroll_net_id);	

    if( 0 == s_cameraroll_net_id)
    {
	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Get_Auto_upload_status PDP deactive !!!");	
	return ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK;
    }
	
    return s_auto_upload_status;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo(void)
{
	uint16 upload_file_size = 0 ;
	
	MMISRV_CAMERAROLL_Load_Upload_File_array();

	upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_snapshot_upload_file_array);

	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo upload_file_size = %d" , upload_file_size);

	return (upload_file_size > 0) ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Need_manual_auto_upload_photo(void)
{
	uint16 upload_file_size = 0 ;

	MMISRV_CAMERAROLL_Load_Upload_File_array();

	upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);

	SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Need_manual_auto_upload_photo upload_file_size = %d" , upload_file_size);

	return (upload_file_size > 0) ; 
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Load_Upload_File_array(void)
{
#ifdef MMI_SHOW_MSA
	MMIFILE_HANDLE file_handle = 0;
	
	//upload file array
	if(PNULL == s_onedrive_snapshot_upload_file_array)
	{
		s_onedrive_snapshot_upload_file_array = MMIAPIFILEARRAY_Create();

		if(MMIAPIFMM_IsFileExist(s_onedrive_snapshot_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_snapshot_upload_list_file_path) ))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_snapshot_upload_file_array , s_onedrive_snapshot_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_snapshot_upload_list_file_path));
	}	
	else
	{
			file_handle = MMIAPIFMM_CreateFile(s_onedrive_snapshot_upload_list_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		MMIAPIFMM_CloseFile(file_handle);
		}
	}

	//upload success file array
	if(PNULL == s_onedrive_upload_success_file_array)
	{
		s_onedrive_upload_success_file_array = MMIAPIFILEARRAY_Create();

		//upload success file array
		if(MMIAPIFMM_IsFileExist(s_onedrive_upload_success_file_path , MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path)))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));
		}
		else
			{
			file_handle = MMIAPIFMM_CreateFile(s_onedrive_upload_success_file_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
			MMIAPIFMM_CloseFile(file_handle);
			}
	}

	// manual upload file array
	if(PNULL == s_onedrive_manual_upload_file_array)
	{
		s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();

		//upload success file array
		if(MMIAPIFMM_IsFileExist(s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path)))
		{
			MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_manual_upload_file_array , s_onedrive_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
		}
		else
		{
			file_handle = MMIAPIFMM_CreateFile(s_onedrive_manual_upload_list_file_path , SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
			MMIAPIFMM_CloseFile(file_handle);
}
	}
#endif
}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Destroy(void)
{

    SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Destroy g_current_module_id = %d" , g_current_module_id);

    Reset_all_data();
    Destroy_Http_all_request();
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count( const uint8* data_ptr,uint32 data_len )
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
       MMISRV_CAMERAROLL_JSON_PARSE_DATA input_data = {0};
	   
	if(PNULL == data_ptr || 0 == data_len)
				return 0;

		camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_Onedrive_photo_Num_CallBack,&input_data);

		jp_ptr = new_JSON_parser(&jc); 

		if(PNULL == jp_ptr)
		{
		    SCI_TRACE_LOW("[cameraroll] MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count PNULL == jp_ptr");
		//	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		    return FALSE;
		}
	if(0 == camera_roll_ParseJsonChar(jp_ptr , data_ptr , data_len) && FALSE == input_data.bDone)
		{
		    //weather_data.is_bad_data = TRUE;
		    SCI_TRACE_LOW("[cameraroll] MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count parse error!");
		}
		delete_JSON_parser(jp_ptr); 

		return input_data.size;
	}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count(void)
{
	return MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);
}
/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Check_Network_status(BOOLEAN network_on)
{

        MN_PHONE_PLMN_STATUS_E net_status = PLMN_NO_SERVICE;
        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        MMISRV_CAMERAROLL_MSG_T sig_data={0};	
        MMI_GMMREG_RAT_E current_rat;
        BOOLEAN bIsFound = FALSE;
		
        if (FALSE == network_on)
        {
        	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Check_Network_status and NO NETWORK !!!!");

		MMISRV_DeleteCameraRollTaskThread();

              s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK;	
		if(s_is_in_gallery)
		{
			sig_data.error_type = ONE_DRIVE_ERROR_NO_NETWORK;	
			MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &sig_data);
		}
        }
	 else
	 {        	

#ifndef WIN32
            bIsFound = MMIAPIPHONE_GetDataServiceSIM(&sim_ok);

            if(FALSE == bIsFound)
            {
                return FALSE;
            }

            current_rat = MMIAPIPHONE_GetTDOrGsm(sim_ok);

            if (MMI_GMMREG_RAT_GPRS != current_rat
            &&	MMI_GMMREG_RAT_3G != current_rat
            && MMI_GMMREG_RAT_GPRS_AND_3G != current_rat)
            {
                SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Check_Network_status No Network !! %d" , current_rat);
                MMISRV_DeleteCameraRollTaskThread();
                s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK;	
                if(s_is_in_gallery)
                {
                    sig_data.error_type = ONE_DRIVE_ERROR_NO_NETWORK;	
                    MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &sig_data);
                }				
                return TRUE;
            }
#endif

		switch(s_auto_upload_status)
		{
			case     ONE_DRIVE_AUTO_UPLOAD_NONE:
			case     ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK:
				//b_need_create_task = TRUE;
	              s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_IDLE;
				break;
			case     ONE_DRIVE_AUTO_UPLOAD_IDLE:
			case     ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING:
			case     ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING:
				break;
		}

	       SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Check_Network_status s_auto_upload_status = %d " , s_auto_upload_status);
	 }

	 return 1;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(wchar* filename , uint16 file_len)
{
	uint32 manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);
	FILEARRAY_DATA_T file_data = {0};
	uint32 i=0 ,  count=0;

			count = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);

			for(i=0;i<count;i++)
			{
				MMIAPIFILEARRAY_Read(s_onedrive_manual_upload_file_array , i ,&file_data);
				
		if(((file_data.name_len == file_len) && 
		      ( 0 == MMIAPICOM_Wstrncmp(file_data.filename , filename , file_len)))
				   )
				{
					MMIAPIFILEARRAY_Delete(s_onedrive_manual_upload_file_array , i);
					break;
				}
			}

		MMIAPIFILEARRAY_SaveArrayToFileEx(s_onedrive_manual_upload_file_array , s_onedrive_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));		
	}


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Get_Connect_State(void)
{
#ifdef MSA_SUPPORT
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	BOOLEAN state_data = FALSE;
	
	MMINV_READ(MMINV_CAMERAROLL_IS_CONNECT_ONEDRIVE, &state_data, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_CAMERAROLL_IS_CONNECT_ONEDRIVE, &state_data);
	}
	return state_data;
#else
    return FALSE;
#endif
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Set_Connect_State(BOOLEAN bConnect)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	BOOLEAN state_data = bConnect;
	
	MMINV_WRITE(MMINV_CAMERAROLL_IS_CONNECT_ONEDRIVE, &state_data);
	if(FALSE == bConnect)
	{
		MMISRV_DeleteCameraRollTaskThread();
	}

	return;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Network_init_Done(void)
{
	return s_cameraroll_task_init_done;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC CAMERA_ROLL_OPERATION_E MMISRV_CAMERAROLL_Check_Current_auto_upload_request(void)
{
	uint8 i=0,j=0;
	CAMERA_ROLL_OPERATION_E retval = CAMERA_ROLL_OPER_NO_OPERATION;
	
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			switch(http_request_operation_array[i].operation_ary[j].operation)
		{
			case CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION:
			case CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION:
			case CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION:
					retval =  http_request_operation_array[i].operation_ary[j].operation;
			break;
		}	
	}
	}

	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Check_Current_auto_upload_request %d " , retval);

	return retval;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY MMISRV_CAMERAROLL_Get_Delete_photo_FileAry(void)
{
	return	s_onedrive_deleted_list_file_array;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_Share_Link(wchar * file_path , uint16 file_path_len)
{
	uint16       w_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};          
	uint16      w_file_name_len=0;      
	uint8       file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};          
	uint16      file_name_len=0 , src_file_path , str_len;  
	uint16 w_file_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0} ;
	uint32 read_size = 0 , file_size =0;	
	uint8 * buff = PNULL;
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
	MMIFILE_HANDLE file_handle = 0;
	MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	
	if(PNULL==file_path || 0==file_path_len)
	{
	    SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Get_Share_Link --- (PNULL==file_path || 0==file_path_len)"); 
	    return ;
	}

    	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_CAMERAROLL_Get_Share_Link !!");	

	//check auto upload status , if YES , pause first 
	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status || 
           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status
		)
	{
		Reset_all_data();
	}
	
	src_file_path = file_path_len;
	SCI_MEMCPY((void*)w_file_path , file_path , sizeof(wchar)*file_path_len);

	MMIAPIFMM_SplitLastName(w_file_path, &file_path_len, w_file_name, &w_file_name_len);  

	MMI_WSTRNTOSTR(file_name,MMIFILE_FULL_PATH_MAX_LEN,w_file_name,MMIFILE_FILE_NAME_MAX_LEN, MMIFILE_FILE_NAME_MAX_LEN);
	

	SCI_MEMSET((void*)&s_onedrive_photo_json_parse_info , 0x00 , sizeof(MMISRV_CAMERAROLL_JSON_PARSE_DATA));
	SCI_MEMSET((void*)s_onedrive_photo_info_src_name , 0x00 , CAMERA_ROLL_MAX_STRING_LEN+1);

	str_len = MIN(CAMERA_ROLL_MAX_STRING_LEN, strlen(file_name));

	strncpy(s_onedrive_photo_info_src_name , file_name , str_len);	
	
	SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Get_Share_Link re-create meta info , filename %s" , s_onedrive_photo_info_src_name);

        if(MMIAPIFMM_IsFileExist(s_meta_info_file_path, MMIAPICOM_Wstrlen(s_meta_info_file_path)))
        {
		file_handle = MMIAPIFMM_CreateFile(s_meta_info_file_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
		if(SFS_INVALID_HANDLE != file_handle)
		{
			file_size = MMIAPIFMM_GetFileSize(file_handle);
			buff = SCI_ALLOC(file_size);
			if(PNULL == buff)
			{
				SCI_TRACE_LOW("[cameraroll]:MMISRV_CAMERAROLL_Get_Share_Link --- (PNULL == buff)"); 
				return ;
			}

                	file_ret =  MMIAPIFMM_ReadFile(
				file_handle, 
				buff, 
				file_size, 
				&read_size, 
				NULL
				);

			if(SFS_ERROR_NONE == file_ret)
			{
				SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Get_Share_Link meta update success !!");
			}
				
			MMIAPIFMM_CloseFile(file_handle);

			//start parse JSON
			camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_Fetch_PhotoInfo_CallBack,&s_onedrive_photo_json_parse_info);
			
			jp_ptr = new_JSON_parser(&jc); 
			
			if(PNULL == jp_ptr)
			{
				SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Get_Share_Link MSG_CAMERA_ROLL_ERROR_NOTIFY PNULL == jp_ptr");
				camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_JSON_ERROR;								
				MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
				return ;
			}
			if(0 == camera_roll_ParseJsonChar(jp_ptr , buff , file_size) && !s_onedrive_photo_json_parse_info.bDone)
			{
				//weather_data.is_bad_data = TRUE;
				SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Get_Share_Link parse error!");
			}
			delete_JSON_parser(jp_ptr); 
			SCI_FREE(buff);

		}
        }


	if(s_onedrive_photo_json_parse_info.bDone)
	{		
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Get_Share_Link input_data.bDone = %d " , s_onedrive_photo_json_parse_info.bDone );

		MMISRV_CAMERAROLL_Send_Create_Share_Link_Req();
	}

	return;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Send_Create_Share_Link_Req(void)
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_POST_PARAM_T* post_param_ptr= NULL;
	uint16 len = 0;
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	uint8 avaliable_request_idx = 0 , context_id;
	
	
	SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_Send_Create_Share_Link_Req !!!!");
	post_param_ptr = (HTTP_POST_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_POST_PARAM_T));

	if(post_param_ptr == NULL)
	{
		SCI_TRACE_LOW("[cameraroll]MMISRV_CAMERAROLL_Send_Create_Share_Link_Req error code MSG_CAMERA_ROLL_ERROR_NOTIFY post_param_ptr == NULL" );
		camera_roll_msg_data.error_type = ONE_DRIVE_ERROR_NO_MEMORY;								
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return;
	}	

	Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
	context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
	Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	if(0xFF != context_id)
    	{

	http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_CREATE_SHARE_LINK;

	memset(post_param_ptr,0,sizeof(HTTP_POST_PARAM_T));  //modify for coverity
	
	post_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
	post_param_ptr->need_net_prog_ind = FALSE;

	//extra header
	post_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_POST_CREATE_SHARE_LINK_HEADER_NUM;
	post_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);
	memset(post_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*post_param_ptr->extern_header.header_num);

	len=strlen(MS_ONEDRIVE_AUTHORIZATION);
	post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

	len=strlen(s_camera_roll_access_token_str);
	post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
	memset(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
	strcpy(post_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);

	//content_type 
	len = strlen(MS_ONEDRIVE_CONTENT_TYPE_JSON_VALUE);
	post_param_ptr->post_body.body_type_ptr = SCI_ALLOC(len + 1);
	memset(post_param_ptr->post_body.body_type_ptr, 0, len + 1);
	strcpy(post_param_ptr->post_body.body_type_ptr, MS_ONEDRIVE_CONTENT_TYPE_JSON_VALUE);

	len=strlen(MS_ONEDRIVE_GET_PHOTO_ITEMS_URL)+strlen(s_onedrive_photo_json_parse_info.id)+strlen("/action.createLink");
	post_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
	memset(post_param_ptr->uri.uri_ptr,0,len+1);
	sprintf(post_param_ptr->uri.uri_ptr , "%s%s%s" , MS_ONEDRIVE_GET_PHOTO_ITEMS_URL, s_onedrive_photo_json_parse_info.id , "/action.createLink");

	post_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
	post_param_ptr->post_body.is_use_file = FALSE;
	post_param_ptr->is_use_relative_url = TRUE;
	post_param_ptr->recv_timeout = 5*60*1000; //5 min 

	post_param_ptr->post_body.u.post_buffer.buffer_ptr = MS_ONEDRIVE_SHARE_LINK_JSON_DATA;
	post_param_ptr->post_body.u.post_buffer.buffer_len = strlen(MS_ONEDRIVE_SHARE_LINK_JSON_DATA);
	post_param_ptr->post_body.u.post_buffer.is_copied_by_http = TRUE;

#ifdef HTTP_SUPPORT
	if(HTTP_SUCCESS ==  HTTP_PostRequest(http_request_operation_array[context_id].context_id ,post_param_ptr, avaliable_request_idx))
	{
		SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_PostRequest success !!!!");
	}
#endif
	
	SCI_FREE(post_param_ptr->uri.uri_ptr);
	FreeHeader(&(post_param_ptr->extern_header), post_param_ptr->extern_header.header_num);
	SCI_FREE(post_param_ptr->extern_header.other_header_ptr);
	SCI_FREE(post_param_ptr->post_body.body_type_ptr);
	post_param_ptr->extern_header.other_header_ptr = NULL;
	SCI_FREE(post_param_ptr);

	}
	return;	
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_cameraroll_info_req(void)

{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint32 tickcount = 0;
	uint8 avaliable_request_idx ,context_id ;
	uint8* token_ptr = PNULL;


	if(0 == strlen(s_camera_roll_access_token_str))
		return;
	
	token_ptr = s_camera_roll_access_token_str + strlen("Bearer ");

	Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
	context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
	Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_send_user_info_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return ;
		}	

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_GET_CAMERAROLL_INFO;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;
		
		len=strlen(MS_ONEDRIVE_GET_CAMERAROLL_ID) + strlen(token_ptr);
		get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
		memset(get_param_ptr->uri.uri_ptr,0,len+1);
		sprintf(get_param_ptr->uri.uri_ptr , "%s%s" , MS_ONEDRIVE_GET_CAMERAROLL_ID , token_ptr);			
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_GetRequest OK !!!!");
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);
	}
	return;	
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_delta_token_info_req(void)

{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
	HTTP_INIT_CNF_SIG_T signal = {0};
	HTTP_APP_PARAM_T app_param = 0;
	HTTP_GET_PARAM_T* get_param_ptr= NULL;
	wchar* tmp_ptr = NULL;
	wchar* ptr = NULL;
	uint16 len = 0;
	uint32 tickcount = 0;
	uint8 avaliable_request_idx , context_id ;
	MMISRV_CAMERAROLL_ONEDRIVEINFO_T delta_info ={0};

	if(TRUE == s_is_process_get_delta_link || TRUE == s_is_process_download_thumbnail)
	{
		SCI_TRACE_LOW("[cameraroll]: MMISRV_CAMERAROLL_Get_delta_token_info_req previous operation still ongoing  SKIP !!!");                
		return ;
	}
		
	Camera_Roll_GetMutex(s_camera_roll_mutex_ptr);
	context_id = camera_roll_get_avaliable_req_index(&avaliable_request_idx);
	Camera_Roll_PutMutex(s_camera_roll_mutex_ptr);

	if(0xFF != context_id)
	{
		SCI_TRACE_LOW("[cameraroll]:START  MMISRV_CAMERAROLL_Get_delta_token_info_req !!!!");
		get_param_ptr = (HTTP_GET_PARAM_T*)SCI_ALLOC_APPZ(sizeof(HTTP_GET_PARAM_T));
	
		if(get_param_ptr == NULL)
		{
			return ;
		}	

		MMISRV_CAMERAROLL_Get_OneDrive_Info(&delta_info);

		http_request_operation_array[context_id].operation_ary[avaliable_request_idx].operation= CAMERA_ROLL_OPER_GET_DELTA_TOKEN_INFO;
			
		memset(get_param_ptr,0,sizeof(HTTP_GET_PARAM_T));  
		
		get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
		get_param_ptr->need_net_prog_ind = FALSE;
		
		get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
		get_param_ptr->need_net_prog_ind = FALSE;
		get_param_ptr->is_use_relative_url = TRUE;

		get_param_ptr->extern_header.header_num = MS_ONEDRIVE_HTTP_GET_ONEDRIVE_INFO_HEADER_NUM;
		get_param_ptr->extern_header.other_header_ptr = SCI_ALLOC(sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);
		memset(get_param_ptr->extern_header.other_header_ptr,0,sizeof(HTTP_OTHER_HEADER_T)*get_param_ptr->extern_header.header_num);

		len=strlen(MS_ONEDRIVE_AUTHORIZATION);
		get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_name_ptr,MS_ONEDRIVE_AUTHORIZATION);

		len=strlen(s_camera_roll_access_token_str);
		get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr = SCI_ALLOC(len+1);	
		memset(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,0,len+1);
		strcpy(get_param_ptr->extern_header.other_header_ptr[0].header_value_ptr,s_camera_roll_access_token_str);

		if( 0 == strlen(delta_info.delta_link))
		{
			len=strlen(MS_ONEDRIVE_GET_DELTA_TOKEN_URL) + strlen("latest");
			get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
			memset(get_param_ptr->uri.uri_ptr,0,len+1);
			sprintf(get_param_ptr->uri.uri_ptr , "%s%s" , MS_ONEDRIVE_GET_DELTA_TOKEN_URL, "latest");	
		}
		else
		{
			len=strlen(delta_info.delta_link);
			get_param_ptr->uri.uri_ptr= SCI_ALLOC(len+1);
			memset(get_param_ptr->uri.uri_ptr,0,len+1);
			strcpy(get_param_ptr->uri.uri_ptr,delta_info.delta_link);
		}	
		
#ifdef HTTP_SUPPORT
		if(HTTP_SUCCESS == HTTP_GetRequest(http_request_operation_array[context_id].context_id ,get_param_ptr, avaliable_request_idx))
		{
			SCI_TRACE_LOW("[cameraroll]:HandleHttpMsg HTTP_GetRequest OK !!!!");
			s_is_process_get_delta_link = TRUE;		
		}
#endif

		SCI_FREE(get_param_ptr->uri.uri_ptr);
		FreeHeader(&(get_param_ptr->extern_header), get_param_ptr->extern_header.header_num);
		SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
		get_param_ptr->extern_header.other_header_ptr = NULL;
		SCI_FREE(get_param_ptr);

	}

	return;	
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_OneDrive_Info(MMISRV_CAMERAROLL_ONEDRIVEINFO_T * info)
{
	MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

	MMISRV_CAMERAROLL_ONEDRIVEINFO_T onedrive_info = {0};
	
	MMINV_READ(MMINV_CAMERAROLL_ONDRIVE_INFO, &onedrive_info, return_value);

	if(MN_RETURN_SUCCESS != return_value)
	{
	    MMINV_WRITE(MMINV_CAMERAROLL_ONDRIVE_INFO, &onedrive_info);
	}

	SCI_MEMCPY((void*)info , (void*)&onedrive_info , sizeof(MMISRV_CAMERAROLL_ONEDRIVEINFO_T));

	return ;	
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Set_OneDrive_Info(MMISRV_CAMERAROLL_ONEDRIVEINFO_T * info)
{
	MMINV_WRITE(MMINV_CAMERAROLL_ONDRIVE_INFO, info);
	return;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Parse_delta_info( const uint8* data_ptr,uint32 data_len  , BOOLEAN * bPhoto_delete)
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
	MMISRV_CAMERAROLL_JSON_PARSE_DELTA_INFO input_data = {0};
	MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data={0};
	MMISRV_CAMERAROLL_ONEDRIVEINFO_T onedrive_info = {0};
	
	if(PNULL == data_ptr || 0 == data_len)
	return 0;

	input_data.delete_photo_ary = MMIAPIFILEARRAY_Create();

	camera_roll_InitjsonConfig(&jc,MMISRV_CAMERAROLL_JsonParse_DeltaInfo_CallBack,&input_data);

	jp_ptr = new_JSON_parser(&jc); 

	if(PNULL == jp_ptr)
	{
		SCI_TRACE_LOW("[cameraroll] MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count PNULL == jp_ptr");
		//	MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_ERROR_NOTIFY , &camera_roll_msg_data);
		return FALSE;
	}

	camera_roll_ParseJsonChar(jp_ptr , data_ptr , data_len);

	delete_JSON_parser(jp_ptr); 

	if( 0 < MMIAPIFILEARRAY_GetArraySize(input_data.delete_photo_ary))
	{
		SCI_TRACE_LOW("[cameraroll]:ENTER Delete_old_onedrive_photo_task SOME PHOTO in Onedrive has been deleted  !!!"); 

		s_onedrive_deleted_list_file_array = input_data.delete_photo_ary;
		MMISRV_CAMERAROLL_SendSignalToMMI(MSG_CAMERA_ROLL_DELETE_PHOTO_NOTIFY , &camera_roll_msg_data);
	}
	else
	{
		SCI_TRACE_LOW("[cameraroll]:ENTER Delete_old_onedrive_photo_task NO PHOTO in Onedrive has been deleted  !!!"); 
		MMIAPIFILEARRAY_Destroy(&input_data.delete_photo_ary);
	}

	MMISRV_CAMERAROLL_Get_OneDrive_Info(&onedrive_info);
	strcpy(onedrive_info.delta_link ,input_data.deltaLink);
	MMISRV_CAMERAROLL_Set_OneDrive_Info(&onedrive_info);
		
	return input_data.bNeed_update_meta;
}

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Clean_manual_upload_list(void)
{

	MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);
	s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();
	MMIAPIFILEARRAY_SaveArrayToFile(s_onedrive_manual_upload_file_array, s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));

	return TRUE;
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
// 	Description : HandleDownloadThumbnail_timer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void Remove_File_From_UploadFileAry(uint16 * file_path , uint16 file_path_len)
{
	uint16 snapshot_upload_file_size = 0 ,  manual_upload_file_size =0 ,i=0;
	FILEARRAY_DATA_T upload_file_data = {0};
	FILEARRAY_DATA_T manual_upload_file_data = {0};

	SCI_TRACE_LOW("[cameraroll]:ENTER Remove_File_From_UploadFileAry !!!"); 

	snapshot_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_snapshot_upload_file_array);
	manual_upload_file_size = MMIAPIFILEARRAY_GetArraySize(s_onedrive_manual_upload_file_array);

	if(manual_upload_file_size > 0) 
	{
		for(i=0; i < manual_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_onedrive_manual_upload_file_array , i , &manual_upload_file_data);

			if(((manual_upload_file_data.name_len == file_path_len) && 
			( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
			)
			{
				MMIAPIFILEARRAY_Delete(s_onedrive_manual_upload_file_array , i);
				SCI_TRACE_LOW("[cameraroll]:ENTER Remove_File_From_UploadFileAry Delete array data !!!"); 
				break;
			}
		}
	}
	
	if(snapshot_upload_file_size > 0)
	{
		for(i=0; i < snapshot_upload_file_size ; i++)
		{
			MMIAPIFILEARRAY_Read(s_onedrive_snapshot_upload_file_array , i , &manual_upload_file_data);

			if(((manual_upload_file_data.name_len == file_path_len) && 
			( 0 == MMIAPICOM_Wstrncmp(manual_upload_file_data.filename , upload_file_data.filename , upload_file_data.name_len)))
			)
			{
				MMIAPIFILEARRAY_Delete(s_onedrive_snapshot_upload_file_array , i);
				SCI_TRACE_LOW("[cameraroll]:ENTER Remove_File_From_UploadFileAry Delete array data !!!"); 
				break;
			}
		}
	}	
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_Http_all_request(void)
{
	uint8 i=0,j=0;
	HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
	
	SCI_TRACE_LOW("[cameraroll]: Enter Reset_Http_all_request");

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if((CAMERA_ROLL_OPER_NO_OPERATION  != http_request_operation_array[i].operation_ary[j].operation) && 
			    (CAMERA_ROLL_OPER_NOT_INIT  != http_request_operation_array[i].operation_ary[j].operation))
			{
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);
			}
		}			
	}

       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;	
	s_b_is_cancel_upload_photo = FALSE;
	s_b_is_cancel_download_photo = FALSE;

	if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status|| 
           ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status)
	{
		s_auto_upload_status = ONE_DRIVE_AUTO_UPLOAD_IDLE;
	}	
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_meta_data_info(void)
{
	MMISRV_CAMERAROLL_ONEDRIVE_METADATA * tmp_ptr = PNULL;

	SCI_TRACE_LOW("[cameraroll]: Enter Clean_meta_data_info");
		
	while(s_onedrive_metadata_ptr)
	{
		tmp_ptr =  s_onedrive_metadata_ptr;
		s_onedrive_metadata_ptr = s_onedrive_metadata_ptr->next_ptr;
		if(tmp_ptr->id) SCI_FREE(tmp_ptr->id);
		if(tmp_ptr->name) SCI_FREE(tmp_ptr->name);
		if(tmp_ptr->thumbnail_url) SCI_FREE(tmp_ptr->thumbnail_url);	
		SCI_FREE(tmp_ptr);
	}
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_upload_data_info(void)
{
	SCI_TRACE_LOW("[cameraroll]: Enter Clean_upload_data_info");
		
	if(s_onedrive_upload_photo_ptr)
	{
		if(0 !=s_onedrive_upload_photo_ptr->file_handle)
		{
			MMIAPIFMM_CloseFile(s_onedrive_upload_photo_ptr->file_handle);
		}
	    	SCI_FREE(s_onedrive_upload_photo_ptr);
	}

	if(s_upload_photo_ary.buf_ptr)
	{
		SCI_FREE(s_upload_photo_ary.buf_ptr);
		s_upload_photo_ary.buf_size =0;
	}
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Clean_download_data_info(void)
{
	uint8 i ,j ;
	if((PNULL != s_current_download_ptr) && (SFS_INVALID_HANDLE != s_current_download_ptr->file_handle))
	{
		MMIAPIFMM_CloseFile(s_current_download_ptr->file_handle);
		MMIAPIFMM_DeleteFile(s_current_download_ptr->file_path , PNULL);
		SCI_FREE(s_current_download_ptr);
		s_current_download_ptr = PNULL;
	}

	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		for(j=0;j<MAX_REQUEST_NUM;j++)
		{
			if(CAMERA_ROLL_OPER_DOWNLOAD_PHOTO== http_request_operation_array[i].operation_ary[j].operation)
			{
				SCI_TRACE_LOW("[cameraroll]: Clean_download_data_info cancel download request !! context id %d , request %d " , i , j );	
				HTTP_CancelRequest(http_request_operation_array[i].context_id , j , http_request_operation_array[i].operation_ary[j].request_id);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].info_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_STRING_LEN);
				SCI_MEMSET((void*)http_request_operation_array[i].operation_ary[j].id_string , 0x00 , sizeof(char) * CAMERA_ROLL_MAX_ID_LEN);
			}
		}			
	}
	s_b_is_cancel_download_photo = FALSE;
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Destroy_Http_all_request(void)
{
	uint8 i=0;

	SCI_TRACE_LOW("[cameraroll]: Enter Clean_Http_all_request");
	
	for(i=0;i<MAX_HTTP_CONTEXT_NUM;i++)
	{
		HTTP_CloseRequest(http_request_operation_array[i].context_id);
		SCI_MEMSET((void*)&http_request_operation_array[i] , 0x00 , sizeof(MMISRV_CAMERAROLL_REQUEST_ARRAY_T));
	}

       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;	
	s_b_is_cancel_upload_photo = FALSE;
	s_b_is_cancel_download_photo = FALSE;	   
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void Reset_all_data(void)
{
	SCI_TRACE_LOW("[cameraroll]: Enter Reset_all_data");
	Reset_Http_all_request();
	Clean_meta_data_info();
	Clean_upload_data_info();
	Clean_download_data_info();

	if(ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == s_auto_upload_status ||
	    ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == s_auto_upload_status)
		s_auto_upload_status =ONE_DRIVE_AUTO_UPLOAD_IDLE;

	if(SFS_INVALID_HANDLE !=s_meta_data_file_handle)
	{
		MMIAPIFMM_CloseFile(s_meta_data_file_handle);
		s_meta_data_file_handle = SFS_INVALID_HANDLE ;
	}	
	
	s_download_thumbnail_total_count =0;
       s_download_thumbnail_fail_count =0;
       s_download_thumbnail_success_count =0;
	s_b_is_cancel_upload_photo = FALSE;
       s_b_is_cancel_download_photo = FALSE;	   
       s_is_process_download_thumbnail = FALSE;	   
       s_is_process_get_delta_link = FALSE;
}
