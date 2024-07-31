/*****************************************************************************
** File Name:      mmiwns_export.h                                         *
** Author:         grant.peng                                                    *
** Date:           07/07/2015                                                *
** Copyright:      2017 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the stucture and interface of WNS               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 7/2015       grant.peng       Create
******************************************************************************/

#ifndef _MMIWNS_EXPORT_H_
#define _MMIWNS_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_appmsg.h"
#include "mmisrv_wns.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


#define CHANNEL_ID_MAX_LEN 64
#define TIME_MAX_LEN 64



/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    WNS_CHANNEL_STATUS_NULL,
    WNS_CHANNEL_STATUS_WAIT_TO_CREATED,
    WNS_CHANNEL_STATUS_CREATE_ONGOING,
    WNS_CHANNEL_STATUS_ALREADY_CREATED, 
    WNS_CHANNEL_STATUS_FAILED,
}WNS_CHANNEL_STATUS_E;


typedef enum
{
    WNS_DISCONNECTED,
    WNS_CONNECTTING,
    WNS_CONNECTED
}WNS_CONNECT_STATUS_E;

typedef enum
{
    NET_DISCONNECTED,
    NET_CONNECTTING,
    NET_CONNECTED
}NET_CONNECT_STATUS_E;

typedef struct  push_notification_channel  
{
    int channel_uri_length;
    char channel_id[CHANNEL_ID_MAX_LEN];
    char* channel_uri;        
    char expiry_time[TIME_MAX_LEN];        
}PUSH_NOTIFICATION_CHANNEL_T;
 
typedef struct  wns_push_notification
{
    char channel_id[CHANNEL_ID_MAX_LEN];
    char time[TIME_MAX_LEN];
    MSG_WNS_NOTIFICATION_TYPE_E notification_type;        
    int content_length;
    char* contents;
}WNS_PUSH_NOTIFICATION_T;


typedef void (*get_push_notification_channel_cb)(char *appname, char *appkey, PUSH_NOTIFICATION_CHANNEL_T *channel);




/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*create and get channel api, 
**input paramters: appkey, Microsoft will provide it for every application. We need apply it.
**appname, application name used by bind appkey and application. 
**return paramters: struct, include uri and expiry_time*/
PUBLIC MSG_WNS_STATUS_E create_push_notification_channel_for_applicationasync(
                char* appname, 
                char * appkey,
                get_push_notification_channel_cb func_ptr);

 /*close channel api, 
**input paramters: appkey, Microsoft will provide it for every application. We need apply it.*/
PUBLIC BOOLEAN  close_push_notification_channel_for_applicationasync(char * appkey);
 
 /*get global settings api,  will return true or false. True is push notification service is on, otherwise is off.
**it can get push notification service state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_setting(void);

 /*get global settings api,  will return true or false. True is push notification alert is on, otherwise is off.
**it can get push notification alert setting state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_alert_setting(void);
 
/*get notification message detail api, 
**input paramters: appkey, index
**return paramters: struct, include message detail information*/
PUBLIC WNS_PUSH_NOTIFICATION_T *get_notifcation_message_information (char * appkey, int index);
 
/*get notification message  counts
**input paramters: appkey
**return:  application notification message counts.*/
PUBLIC int get_notification_message_count(char * appkey);

PUBLIC void remove_app_event_handler(char *appname);


/*****************************************************************************/
//     Description : init wns module
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIWNS_InitModule(void);

/*****************************************************************************/
//     Description : init wns api
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWNS_Init(void);


/*****************************************************************************/
//     Description : stop wns service at poweroff
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWNS_Stop(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MMIWNS_EXPORT_H_
