/*************************************************************************
 ** File Name:      mmisrv_cameraroll_export.h                                 *
 ** Author:                                       *
 ** Date:                                                    *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                                *
*************************************************************************/

#ifndef MMISRV_CAMERAROLL_EXPORT_H_
#define MMISRV_CAMERAROLL_EXPORT_H_

#include "mmk_type.h"
#include "mmk_app.h"
#include "http_api.h"
#include "json_parser.h"
#include "mmi_filemgr.h"
#include "mmifilearray_export.h"
#include "mn_type.h"

#define CAMERA_ROLL_MAX_STRING_LEN  1024
#define CAMERA_ROLL_MAX_ID_LEN  128
#define CONTENT_BUFF_SIZE 50*1024
#define MAX_UPLOAD_ARRAY_SIZE 5
#define MAX_REQUEST_NUM 2

typedef enum
{
	CAMERA_ROLL_TYPE_NONE,
	CAMERA_ROLL_TYPE_DOWNLOAD_THUMBNAIL,
	CAMERA_ROLL_TYPE_DOWNLOAD_PHOTO,
	CAMERA_ROLL_TYPE_MANUAL_UPLOAD_SINGLE_PHOTO,
	CAMERA_ROLL_TYPE_MANUAL_UPLOAD_MULTI_PHOTO,
	CAMERA_ROLL_TYPE_SNAPSHOT_UPLOAD_MULTI_PHOTO,
	CAMERA_ROLL_TYPE_GET_SHARELINK,
	CAMERA_ROLL_TYPE_MAX
}CAMERA_ROLL_TYPE_E;

typedef enum
{
    CAMERA_ROLL_SIG_NONE,
    CAMERA_ROLL_DOWNLOAD_THUMBNAIL,
    CAMERA_ROLL_UPLOAD_PHOTO,
    CAMERA_ROLL_CANCEL,
    CAMERA_ROLL_SIG_MAX
}CAMERA_ROLL_SIG_TYPE_E;

typedef enum
{
    CAMERA_ROLL_OPER_NOT_INIT,
    CAMERA_ROLL_OPER_NO_OPERATION,
    CAMERA_ROLL_OPER_GET_THUMBNAIL_INFO,
    CAMERA_ROLL_OPER_GET_THUMBNAIL_URL,
    CAMERA_ROLL_OPER_DOWNLOAD_THUMBNAIL,
    CAMERA_ROLL_OPER_CREATE_SESSION,
    CAMERA_ROLL_OPER_UPLOAD_SESSION,
    CAMERA_ROLL_OPER_CLOSE_SESSION,
    CAMERA_ROLL_OPER_DOWNLOAD_PHOTO_LOCATION,
    CAMERA_ROLL_OPER_DOWNLOAD_PHOTO,
    CAMERA_ROLL_OPER_AUTO_UPLOAD_CREATE_SESSION,
    CAMERA_ROLL_OPER_AUTO_UPLOAD_SESSION,
    CAMERA_ROLL_OPER_AUTO_CLOSE_SESSION,    
    CAMERA_ROLL_OPER_CREATE_SHARE_LINK,    
    CAMERA_ROLL_OPER_GET_CAMERAROLL_INFO,    
    CAMERA_ROLL_OPER_GET_DELTA_TOKEN_INFO,    
    CMAERA_ROLL_OPERAION_MAX
}CAMERA_ROLL_OPERATION_E;

typedef enum 
{
    JSON_TAG_NONE = 0,
    JSON_TAG_ID,		
    JSON_TAG_NAME,
    JSON_TAG_SIZE,
    JSON_TAG_WEB_URL,
    JSON_TAG_DELTA_LINK,
    JSON_TAG_VALUE,
    JSON_TAG_USER,
    JSON_TAG_DELETED,
    JSON_TAG_APPLICATION,
    JSON_TAG_MAX
}CAMERA_ROLL_JSON_TAG_E;


typedef enum
{
    CAMERA_ROLL_AUTO_SYNC,
    CAMERA_ROLL_MANUAL_SYNC,
    CAMERA_ROLL_SYNC_TYPE_MAX
}ONE_DRIVE_SYNC_STATE;

typedef enum
{
    ONE_DRIVE_AUTO_UPLOAD_NONE,
    ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK,		
    ONE_DRIVE_AUTO_UPLOAD_IDLE,
    ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING,    
    ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING,        
    ONE_DRIVE_AUTO_UPLOAD_MAX
}ONE_DRIVE_AUTO_UPLOAD_STATE_E;

typedef enum
{
	ONE_DRIVE_ERROR_NONE,
	ONE_DRIVE_ERROR_NO_MEMORY,
	ONE_DRIVE_ERROR_NO_NETWORK,
	ONE_DRIVE_ERROR_HTTP_ERROR,
	ONE_DRIVE_ERROR_JSON_ERROR,
	ONE_DRIVE_ERROR_UNKNOWN
}ONE_DRIVE_ERROR_E;

typedef struct _MMISRV_CAMERAROLL_ONEDRIVE_METADATA
{
	uint8 * id;
	uint8 * name;
	uint64 size;
	uint8 * thumbnail_url;	
	struct _MMISRV_CAMERAROLL_ONEDRIVE_METADATA * next_ptr;
}MMISRV_CAMERAROLL_ONEDRIVE_METADATA;

typedef struct _MMISRV_CAMERAROLL_UPLOAD_INFO
{
       uint8 upload_url[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8  session_id[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8  file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
	uint16  file_name_len;
	uint16  file_path[MMIFILE_FULL_PATH_MAX_LEN+1];
	uint16  file_path_len;	
	DWORD total_file_size ; 
	DWORD remain_file_size ;
	DWORD sended_file_size ; 
	DWORD uploaded_file_size ; 
	MMIFILE_HANDLE file_handle ; 
	struct _MMISRV_CAMERAROLL_UPLOAD_INFO * next_ptr;
}MMISRV_CAMERAROLL_UPLOAD_INFO_T;

typedef struct _MMISRV_CAMERAROLL_DOWNLOAD_INFO
{
	BOOLEAN bReceive_cnf;
	uint8  file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
	uint16  file_name_len;
	uint16  file_path[MMIFILE_FULL_PATH_MAX_LEN+1];
	uint16  file_path_len;	
	uint8 id[CAMERA_ROLL_MAX_STRING_LEN+1];
	DWORD total_file_size ; 
	DWORD saved_file_size ;
	MMIFILE_HANDLE file_handle ; 
}MMISRV_CAMERAROLL_DOWNLOAD_INFO_T;

typedef struct _MMISRV_CAMERAROLL_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //：??：?：2???∴?2?：a?|━?HTTP???：???┷：a?：o?┐??：?D：＜???HTTP???：?：o?┐?：|：＜??a：：?：＜a?|━??┷
}MMISRV_CAMERAROLL_HTTP_CONTEXT_T;

typedef struct _MMISRV_CAMERAROLL_JSON_PARSE_DATA
{
    CAMERA_ROLL_JSON_TAG_E           cur_tag;            // ?????y?：2?a??|━? xml tag
    int     Object_depth;
	uint8 id[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8 name[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint64 size;	
	BOOLEAN bFileExist;
	BOOLEAN bDone;
}MMISRV_CAMERAROLL_JSON_PARSE_DATA;

typedef struct _MMISRV_CAMERAROLL_JSON_PARSE_DELTA_INFO
{
    	CAMERA_ROLL_JSON_TAG_E           cur_tag;            // ?????y?：2?a??|━? xml tag
	uint8 deltaLink[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8 name[CAMERA_ROLL_MAX_STRING_LEN+1];
	BOOLEAN bNeed_update_meta;
	FILEARRAY delete_photo_ary;
}MMISRV_CAMERAROLL_JSON_PARSE_DELTA_INFO;
	
typedef struct _MMISRV_CAMERAROLL_JSON_PARSE_FOLDER_ID_INFO
{
	CAMERA_ROLL_JSON_TAG_E           cur_tag;            // ?????y?：2?a??|━? xml tag
	BOOLEAN id_from_user;
	BOOLEAN id_from_application;
	uint8 user_id[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8 folder_id[CAMERA_ROLL_MAX_STRING_LEN+1];
}MMISRV_CAMERAROLL_JSON_PARSE_FOLDER_ID_INFO;

typedef struct _MMISRV_CAMERAROLL_OPERATE_INFO_T_
{
    CAMERA_ROLL_OPERATION_E operation;
    char info_string[CAMERA_ROLL_MAX_STRING_LEN];
    uint16 info_strlen;
    char id_string[CAMERA_ROLL_MAX_ID_LEN];
    uint16 id_strlen;
    uint32 upload_buf_size;
    HTTP_REQUEST_ID_T    request_id;
}MMISRV_CAMERAROLL_OPERATE_INFO_T;



typedef struct _MMISRV_CAMERAROLL_REQUEST_ARRAY_T_
{
//    CAMERA_ROLL_OPERATION_E operation;
    MMISRV_CAMERAROLL_OPERATE_INFO_T operation_ary[MAX_REQUEST_NUM];
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //：??：?：2???∴?2?：a?|━?HTTP???：???┷：a?：o?┐??：?D：＜???HTTP???：?：o?┐?：|：＜??a：：?：＜a?|━?
}MMISRV_CAMERAROLL_REQUEST_ARRAY_T;

typedef struct
{
    xSignalHeaderRec     sig;
    void * data_ptr;
}CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T;

typedef struct
{
    SIGNAL_VARS
    char * filename; 
    uint8 file_len;
}CAMERA_ROLL_THUMBNAIL_INFO_SIG_T;

typedef struct
{
    SIGNAL_VARS
    uint8 success_num ; 
    uint8 fail_num;
}CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF_SIG_T;

typedef struct
{
    SIGNAL_VARS
    uint8 success_num ; 
    uint8 fail_num;
}CAMERA_ROLL_UPLOAD_PHOTO_CNF_SIG_T;

typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}CAMERA_ROLL_UPLOAD_PHOTO_SIG_T;

typedef struct _MMISRV_CAMERAROLL_MSG_T_
{
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]; 
    uint8 url[CAMERA_ROLL_MAX_STRING_LEN+1]; 	
    uint8 file_len;
    uint16 url_len;
    uint8 success_num ; 
    uint8 fail_num;
    uint8 total_num;	
    uint8 curr_download_num;
    uint64 total_file_size;
    uint64 uploaded_file_size;	
    uint64 downloaded_file_size;	
    BOOLEAN bDone;
	ONE_DRIVE_ERROR_E error_type;
    CAMERA_ROLL_TYPE_E init_type;
}MMISRV_CAMERAROLL_MSG_T;

typedef struct _MMISRV_CAMERAROLL_ONEDRIVEINFO_T_
{
	uint8 delta_link[CAMERA_ROLL_MAX_STRING_LEN+1]; 	
	uint8 folder_id[CAMERA_ROLL_MAX_STRING_LEN+1];
	uint8 user_id[CAMERA_ROLL_MAX_STRING_LEN+1];
}MMISRV_CAMERAROLL_ONEDRIVEINFO_T;

typedef struct
{
    SIGNAL_VARS
    MMISRV_CAMERAROLL_MSG_T camera_roll_msg_data;
}CAMERA_ROLL_SIG_T;


typedef struct _CAMERA_ROLL_ONEDRIVE_UPLOAD
{
	uint8 *buf_ptr;
	uint64 buf_size;
}CAMERA_ROLL_ONEDRIVE_UPLOAD_T;

typedef uint8 (*CAMERA_ROLL_HTTP_REQ_FUNC)(uint8 app_param , void *);


/*****************************************************************************/
//  FUNCTION:      MMISRV_CAMERAROLL_CreateTask
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_CreateTask(uint32 module_id , CAMERA_ROLL_TYPE_E type);

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_InitHttpRequest(uint32 module_id);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/

PUBLIC void MMISRV_CAMERAROLL_start_get_thumbnail_info(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_start_upload_file(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISRV_CAMERAROLL_start_download_thumbnails(void);

/*****************************************************************************/
//  Description : get json string 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_GetJsonString(void * user_data,const JSON_value* j_value);


/*****************************************************************************/
//  Description : get json integer 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_GetJsonInteger(void * user_data,const JSON_value* j_value);


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_CAMERAROLL_Download_Thumbnail(void);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRV_CAMERAROLL_HandleHTTPMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Download_thumbnail_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Download_photo_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Onedrive_photo_Num_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_Metadata_FileAry_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_ShareLink_CallBack(void* ctx, int type, const JSON_value* value);

/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int MMISRV_CAMERAROLL_JsonParse_DeltaInfo_CallBack(void* ctx, int type, const JSON_value* value);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Upload_single_photo(uint16 * file_patch , uint16 file_path_len);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Download_Original_photo(uint16 * file_patch , uint16 file_path_len);


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Upload_multiple_photos(void);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Set_Upload_multiple_photos(FILEARRAY file_array);


/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_upload_session(uint8 context_id , uint8 http_req_ary_idx , MMISRV_CAMERAROLL_UPLOAD_INFO_T * src_ptr);

/*****************************************************************************/
//  Description : HandleHttpErrorInd
//  Global resource dependence : 
//  Author: wenny.cheng
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_close_session(uint8 context_id , uint8 http_idx , MMISRV_CAMERAROLL_UPLOAD_INFO_T * src_ptr);

/**************************************************************************************/
// Description : send signal to MMI Task  
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC  void MMISRV_CAMERAROLL_SendSignalToMMI(uint32 sig_id,DPARAM param);

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void  MMISRV_CAMERAROLL_Get_Token(int result, char*  buf, uint16 buflen);

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Download_photo(void);

/**************************************************************************************/
// Description : Set_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_SYNC_STATE MMISRV_CAMERAROLL_Set_Sync_State(uint8 state);


/**************************************************************************************/
// Description : Get_Sync_State
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_SYNC_STATE MMISRV_CAMERAROLL_Get_Sync_State(void);

/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(wchar* filename , uint16 file_len);          

/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_Uploading(wchar* filename , uint16 file_len);   

/**************************************************************************************/
// Description : Is_Pic_On_OneDrive
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Pic_Auto_Uploading(wchar* filename , uint16 file_len);

/**************************************************************************************/
// Description : Get_OneDrive_Pic_Url
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC char* MMISRV_CAMERAROLL_Get_OneDrive_Pic_Url(char* filename , uint16 file_len);  

/**************************************************************************************/
// Description : Cancel Download
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Cancel_Download(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Cancel_Upload(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY  MMISRV_CAMERAROLL_Create_upload_file_ary(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_append_upload_file(wchar *  , uint8 );  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Close_upload_file_ary(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Check_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_exec_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Enter_Gallery(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_In_Gallery(void);  


/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Leave_Gallery(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC ONE_DRIVE_AUTO_UPLOAD_STATE_E MMISRV_CAMERAROLL_Get_Auto_upload_status(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Need_manual_auto_upload_photo(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Destroy(void);  

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Load_Upload_File_array(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Get_OneDrive_Photo_Count( const uint8* data_ptr,uint32 data_len );

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint16 MMISRV_CAMERAROLL_Check_Network_status(BOOLEAN);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(wchar* filename , uint16 file_len);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Get_Connect_State(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Set_Connect_State(BOOLEAN bConnect);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Is_Network_init_Done(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC CAMERA_ROLL_OPERATION_E MMISRV_CAMERAROLL_Check_Current_auto_upload_request(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_Share_Link(wchar * filepath , uint16 file_len);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC FILEARRAY MMISRV_CAMERAROLL_Get_Delete_photo_FileAry(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Send_Create_Share_Link_Req(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_cameraroll_info_req(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_delta_token_info_req(void);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Get_OneDrive_Info(MMISRV_CAMERAROLL_ONEDRIVEINFO_T *);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMISRV_CAMERAROLL_Set_OneDrive_Info(MMISRV_CAMERAROLL_ONEDRIVEINFO_T *);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Parse_delta_info( const uint8* data_ptr,uint32 data_len , BOOLEAN * bphoto_delete);

/**************************************************************************************/
// Description : Cancel_Upload
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_CAMERAROLL_Clean_manual_upload_list(void);

#endif //MMISRV_CAMERAROLL_EXPORT_H_

