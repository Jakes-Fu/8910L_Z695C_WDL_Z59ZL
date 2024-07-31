/*****************************************************************************
** File Name:      mmiwns_manager.h                                         *
** Author:         grant.peng                                                    *
** Date:           07/18/2015                                                *
** Copyright:      2017 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe push application manager               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 7/2015       grant.peng       Create
******************************************************************************/

#ifndef _MMIWNS_MANAGER_H_
#define _MMIWNS_MANAGER_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define WNS_APP_MAX_NUM 10
#define WNS_NOTIFICATION_MAX_NUM 1
#define CHANNEL_URI_MAX_LEN 256
#define CONTENT_MAX_LEN (5*1024)
#define WNS_APP_NAME_MAX_LEN 64




/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct  wns_push_notification_info
{
    char time[TIME_MAX_LEN];
    MSG_WNS_NOTIFICATION_TYPE_E notification_type;        
    char contents[CONTENT_MAX_LEN];
}WNS_PUSH_NOTIFICATION_INFO_T;

typedef struct  pns_mgr_app_info_tag  
{
    char appname[WNS_APP_NAME_MAX_LEN];
    char app_key[APP_KEY_MAX_LEN];
    char channel_id[CHANNEL_ID_MAX_LEN];
    char channel_uri[CHANNEL_URI_MAX_LEN];
    char expiry_time[TIME_MAX_LEN];
    WNS_CHANNEL_STATUS_E channel_status;
    uint16 app_index;
    uint16 push_notification_count;
    WNS_PUSH_NOTIFICATION_INFO_T push_notification_list[1];
}PNS_MGR_APP_INFO_T;

typedef struct  pns_mgr_app_nv_info_tag  
{
    PNS_MGR_APP_INFO_T pns_mgr_app_info;
    //WNS_NOTIFICATION_MAX_NUM定义为1的时候，这个地方会编译不过，
    //万一以后WNS通知需要再改成存多条，这个地方需要打开。
    //WNS_PUSH_NOTIFICATION_INFO_T push_notification_list[WNS_NOTIFICATION_MAX_NUM - 1];
}PNS_MGR_APP_NV_INFO_T;




/*Indicate request message structure of MSG_PNS_CREATE_CHANNEL_REQ*/
typedef struct
{
    SIGNAL_VARS
    char appname[WNS_APP_NAME_MAX_LEN];
    char app_key[APP_KEY_MAX_LEN];
    get_push_notification_channel_cb get_channel_info_func_cb;
}PNS_CREATE_CHANNEL_SIGNAL_T;


/*Indicate request message structure of MSG_PNS_CLOSE_CHANNEL_REQ*/
typedef struct
{
    SIGNAL_VARS
    char app_key[APP_KEY_MAX_LEN];
}PNS_CLOSE_CHANNEL_SIGNAL_T;


/*Indicate request message structure of MSG_PNS_APP_IS_REMOVED*/
typedef struct
{
    SIGNAL_VARS
    char appname[WNS_APP_NAME_MAX_LEN];
}PNS_APP_REMOVED_SIGNAL_T;

/*Indicate request message structure of MSG_PNS_CLEAR_NOTIFICATION*/
typedef struct
{
    SIGNAL_VARS
    char appname[WNS_APP_NAME_MAX_LEN];
}PNS_CLEAR_NOTIFICATION_SIGNAL_T;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Get channel id by app key
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC int MMIWNS_GetNotificationCount(const char *app_key);

/*****************************************************************************/
//  Discription: Get app info by app key
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC PNS_MGR_APP_INFO_T *MMIWNS_GetAppInfoByAppKey(const char *app_key);

/*****************************************************************************/
//  Discription: Get app info by app name
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC PNS_MGR_APP_INFO_T *MMIWNS_GetAppInfoByAppName(const char *appname);


/*****************************************************************************/
//     Description : handle java close channel request
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC MSG_WNS_STATUS_E MMIWNS_ClearNotificationByAppName(char *appname);


/*****************************************************************************/
//  Discription: Is wns application on front ground state
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWNS_IsAppOnFgState(const char *appname);


/*****************************************************************************/
//     Description : show saved push notification on idle screen
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_ParsePushMsg(PNS_MGR_APP_INFO_T *app_info_ptr, int index);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MMIWNS_MANAGER_H_
