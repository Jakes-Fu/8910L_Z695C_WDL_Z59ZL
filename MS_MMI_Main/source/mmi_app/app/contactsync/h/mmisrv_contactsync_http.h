/****************************************************************************
** File Name:      mmibt_export.h                                         *
** Author:         Wenming feng                                            *
** Date:           Dec/22/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Dec/22/2005    Wenming feng     Create                                  *
**                                                                         *
****************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _PBSYNC_HTTP_H_    
#define  _PBSYNC_HTTP_H_        

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "http_api.h"
#include "mmipb_adapt.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
/************************************************************************/
/* PbSync异常类型                                                    */
/************************************************************************/
#define PBSYNC_ERROR_NOP                     0               // done, no operation
#define PBSYNC_ERROR_SUCCESS                 1               // MMI req handle success
// #define PBSYNC_ERROR_CACHEHIT                1
// #define PBSYNC_ERROR_CACAHEMISS              2
// #define PBSYNC_ERROR_JSON_SUCCESS            3   
#define PBSYNC_ERROR_JSON_TOKENINVALID       4               // profile token invalid
#define PBSYNC_ERROR_JSON_ERROR              5               // json parse fail
// #define PBSYNC_ERROR_DATAPROC_SUCCESS        6
#define PBSYNC_ERROR_PARAM_ERROR             7
#define PBSYNC_ERROR_NOMEMRY                   8
#define PBSYNC_ERROR_NODATA                  9
#define PBSYNC_ERROR_FILE_ERROR              10
#define PBSYNC_ERROR_NET_ERROR               11
#define PBSYNC_ERROR_TIMEOUT                 12
#define PBSYNC_ERROR_OPERATION_ERROR         13   //操作失败的错误码
#define PBSYNC_ERROR_PICSIZE_OVERMAX         14   //
#define PBSYNC_ERROR_PATCH_DELETED_ITEM         15   //


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    RESPONSE_SUCCESS = 0, 
    RESPONSE_NULL,
    RESPONSE_PARSER_ERROR,
    RESPONSE_PDP_DEACTIVE_1_ERROR,
    RESPONSE_PDP_DEACTIVE_2_ERROR,
    RESPONSE_NO_CHANCE_TO_RETRY,
    RESPONSE_NO_CHANCE_RETRY_ERROR,
    RESPONSE_NOT_AUTHENTICATION = 32,//Your call could not be completed as dialed
    RESPONSE_PAGE_NOT_EXIST = 34,//Corresponds with an HTTP 404 - the specified resource was not found.
    RESPONSE_API_ERROR = 68,//Corresponds to a HTTP request to a retired v1-era URL.
    RESPONSE_LIMIT_EXCEEDED = 88,//The request limit for this resource has been reached for the current rate limit window.
    RESPONSE_TOKEN_INVAILD = 89,//The access token used in the request is incorrect or has expired. Used in API v1.1
    RESPONSE_NOT_PERMITTED = 64,//Corresponds with an HTTP 403 - the access token being used belongs to a suspended user and they can't complete the action you're trying to take
    SERVER_OVER_CAPACITY = 130,//Corresponds with an HTTP 503 - Twitter is temporarily over capacity.
    SERVER_INTERNAL_ERROR = 131,//Corresponds with an HTTP 500 - An unknown internal error occurred.
    RESPONSE_TIMESTAMP_ERROR = 135,//Corresponds with a HTTP 401 - it means that your oauth_timestamp is either ahead or behind our acceptable range
    RESPONSE_DUPLICATION_STATUS = 187,//The status text has been Tweeted already by the authenticated account.
    SERVER_BAD_AUTHENTICATION = 215,//Typically sent with 1.1 responses with HTTP code 400. The method requires authentication but it was not presented or was wholly invalid.
    RESPONSE_LOGIN_VERIFICATION = 231,//Returned as a challenge in xAuth when the user has login verification enabled on their account and needs to be directed to twitter.com to generate a temporary password
    RESPONSE_MAX,
}PBSYNC_RESPONSE_CODE_E;

typedef struct _PBSYNC_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}PBSYNC_HTTP_CONTEXT_T;

//保存http请求的参数，用于所有带参数的请求，http conform时再发出请求
typedef struct _PBSYNC_HTTP_REQUEST_PARAM_T_
{
    //char O365_id[MMIPB_SYNC_O365_ID_LEN]; keep it in contact_info.
    //uint32 uiTime; keep it in contact_info o365_modifiedTime
    MMIPB_CONTACT_T contact_info;
    uint32  http_request_fail_retry_time;
    uint32  http_request_response_code;
}PBSYNC_HTTP_REQUEST_PARAM_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN PBSYNC_PdpActive(void);

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN PBSYNC_PdpDeactive(void);
/*****************************************************************************/
//  Description : 获取access token
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void PBSYNC_AccessTokenGet(void);
/*****************************************************************************/
//  Description : 保存access token
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void PBSYNC_AccesstokenSave(char* access_token);
/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 HandlePBSyncHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : HandlePBSyncAppMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePBSyncAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif

#endif //MMIPB_SYNC_WITH_O365
