/*****************************************************************************
** File Name:      mmisrv_wns.h                                                                  *
** Author:                                                                                                *
** Date:           06/2015                                                                            *
** Version:           v0.2                                                                              *
** Copyright:      2015 Microsoft. All Rights Reserved.                                    *
** Description:    This file describes the Windows Notification Service interface  *
******************************************************************************
**                         Important Edit History                                                                     *
** --------------------------------------------------------------------------*
** Ver 0.1 Create by Microsoft    05/2015                                                                      *
** Ver 0.2 Change to dynamic length for some struct field, update some field according to
               new protocol, add transaction id in message structure             06/2015              *
******************************************************************************/
#ifndef _MMISRV_WNS_H_
#define _MMISRV_WNS_H_
#ifdef __cplusplus
extern   "C" {
#endif
/*-------------------------------------INCLUDE ------------------------------*/
#include "os_api.h"
#include "tcpip_types.h"
#include <time.h>
/*-------------------------------------MACRO---------------------------------*/
/*Note: the array fields use these macros are null-terminated string*/
#define APP_KEY_MAX_LEN (161+1)
#define AGENT_OS_MAX_LEN (32+1)
#define AGENT_OS_VERSION_MAX_LEN (20+1)
#define AGENT_PROCESSOR_MAX_LEN (32+1)
#define AGENT_LOCALE_ID_MAX_LEN (6+1)

/*--------------------------------------ENUM --------------------------------*/
/*Time structure used in this message interface*/
typedef struct tm MSG_WNS_TM;
/*Indicate status or error code of current operation*/
typedef enum
{
    WNS_STATUS_SUCCESS,
    WNS_STATUS_NOMEMORY,
    WNS_STATUS_BADPARAM, 
    WNS_STATUS_NOTREGISTERED,
    WNS_STATUS_NOTCONNECTED,
    WNS_STATUS_ALREADYCONNECTED,
    WNS_STATUS_INPROGRESS,
    WNS_STATUS_FAILED,
}MSG_WNS_STATUS_E;
/*Indicate the push notification type*/
typedef enum
{
    WNS_TYPE_TOAST,
    WNS_TYPE_TILE,
    WNS_TYPE_BADGE,
    WNS_TYPE_RAW
}MSG_WNS_NOTIFICATION_TYPE_E;

#ifndef MMIWNS_TEST_SUPPORT
//Message Direction:  MMI --> WNS
/*Indicate the message type sent from mmi to wns*/
typedef enum
{
    MSG_WNS_NONE,
    MSG_WNS_STOP_WNS,
    MSG_WNS_REG_WNS, 
    MSG_WNS_CONNECT_WNS,
    MSG_WNS_DISCONNECT_WNS,
    MSG_WNS_CREATE_CHANNEL,
    MSG_WNS_REVOKE_CHANNEL,
    MSG_WNS_MAX
}MSG_WNS_TYPE_E;
#endif
/*----------------------------------STRUCTURE--------------------------------*/
//Message Structure Direction:  MMI --> WNS


/*Note: MSG_WNS_STOP_WNS don't have any message structure body*/

/*Note: MSG_WNS_DISCONNECT_WNS don't have any message structure body*/

/*Indicate request message structure of MSG_WNS_REG_WNS*/
typedef struct
{
    SIGNAL_VARS
    BLOCK_ID task_id;//the task_id indicate which task the wns will communicate with
}WNS_REG_SIGNAL_T;

/*Platform relevant information*/
typedef struct
{
    char agent_os[AGENT_OS_MAX_LEN];
    char agent_os_version[AGENT_OS_VERSION_MAX_LEN];
    char agent_processor[AGENT_PROCESSOR_MAX_LEN];
    char agent_locale_id[AGENT_LOCALE_ID_MAX_LEN];
}WNS_AGENT_INFO_T;

/*Indicate request message structure of MSG_WNS_CONNECT_WNS*/
typedef struct
{
    SIGNAL_VARS
    WNS_AGENT_INFO_T  agent_info;
    TCPIP_NETID_T net_id;
}WNS_CONNECT_SIGNAL_T;

/*Indicate request message structure of MSG_WNS_CREATE_CHANNEL*/
typedef struct
{
    SIGNAL_VARS
    char app_key[APP_KEY_MAX_LEN];
    uint32 channel_id_length;//Note: If channel_id is null or channel_id_length is 0, the server will response a new channel id. If channel_id and channel_id_lengthhas value, that means renew the channel uri using current channel id 
    char* channel_id;//wns need to free this memory.
}WNS_CREATE_CHANNEL_SIGNAL_T;

/*Indicate request message structure of MSG_WNS_REVOKE_CHANNEL*/
typedef struct  
{
    SIGNAL_VARS
    uint32 channel_id_length;
    char* channel_id;//wns need to free this memory
}WNS_REVOKE_CHANNEL_SIGNAL_T;

//Message Structure Direction:  WNS --> MMI

#ifndef MMIWNS_TEST_SUPPORT
/*Common message response field definition*/
#define WNS_RESPONSE_VARS \
    SIGNAL_VARS \
    MSG_WNS_STATUS_E status; \
    MSG_WNS_TYPE_E response_which_message;
#else
/*Common message response field definition*/
#define WNS_RESPONSE_VARS \
    SIGNAL_VARS \
    MSG_WNS_STATUS_E status; \
    uint32 response_which_message;
#endif

/*Common message response structure: Response for MSG_WNS_STOP_WNS, MSG_WNS_REG_WNS, MSG_WNS_CONNECT_WNS
    MSG_WNS_DISCONNECT_WNS*/
typedef struct {
WNS_RESPONSE_VARS
} WNS_STATUS_RESPONSE_SIGNAL_T;

/*Create channel message response structure: Response for MSG_WNS_CREATE_CHANNEL*/
typedef struct
{
    WNS_RESPONSE_VARS
    uint32 channel_id_length;
    char* channel_id;//mmi need to free this memory
    uint32 channel_uri_length;
    char* channel_uri;//mmi need to free this memory
    MSG_WNS_TM expiry_time;
}WNS_CREATE_CHANNEL_RESPONSE_SIGNAL_T;

/*Push notification structure for message MSG_WNS_PUSH_NOTIFICATION*/
typedef struct  
{
    SIGNAL_VARS
    uint32 channel_id_length;
    char* channel_id;//mmi need to free this memory
    MSG_WNS_TM time;
    MSG_WNS_NOTIFICATION_TYPE_E notification_type;
    uint32 content_length;
    char* contents;//mmi need to free this memory
}WNS_PUSH_NOTIFICATION_SIGNAL_T;

/*Remote side disconnected structure for message MSG_WNS_CONNECTION_CLOSED*/
typedef struct  
{
    SIGNAL_VARS
    MSG_WNS_STATUS_E status;
}WNS_DISCONNECTED_SIGNAL_T;

/*Indicate request message structure of MSG_WNS_DISCONNECT_WNS*/
typedef struct
{
    SIGNAL_VARS
    int32 reason;  /* 0 : other reason,   1 : PDP DEACTIVE */
}WNS_DISCONNECT_SIGNAL_T;

typedef struct  
{
    SIGNAL_VARS
}WNS_STOP_SIGNAL_T;

/*Common error structure for message MSG_WNS_COMMON_ERROR*/
typedef struct  
{
    SIGNAL_VARS
    MSG_WNS_STATUS_E status;
}WNS_COMMON_ERROR_SIGNAL_T;

/*-----------------------INTERFACE FUNCTION----------------------------------*/
PUBLIC BLOCK_ID MMIWNS_Init(void);

PUBLIC BLOCK_ID MMIWNS_CUSTOMIZE_Init(uint32 priority);

#ifdef __cplusplus
}
#endif
#endif // _MMISRV_WNS_H_
