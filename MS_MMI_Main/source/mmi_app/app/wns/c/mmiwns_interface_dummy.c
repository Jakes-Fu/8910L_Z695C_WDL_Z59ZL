/****************************************************************************
** File Name:      mmiwns_interface_dummy.c                                 *
** Author:         cheney.wu                                                *
** Date:           07/04/2017                                               *
** Copyright:      2017 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the wns interface for java application.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/04/2017    cheney.wu      Create
****************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiwns_export.h"
#include "mmiwns_manager.h"
#include "mmiset_export.h"

/**-------------------------------------------------------------------------*
**                         MICRO DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*create and get channel api, 
**input paramters: appkey, Microsoft will provide it for every application. We need apply it.
**appname, application name used by bind appkey and application. 
**return paramters: struct, include uri and expiry_time*/
PUBLIC MSG_WNS_STATUS_E create_push_notification_channel_for_applicationasync(
                char* appname, 
                char* appkey,
                get_push_notification_channel_cb func_ptr)
{
    return WNS_STATUS_FAILED;
}

 /*close channel api, 
**input paramters: appkey, Microsoft will provide it for every application. We need apply it.*/
PUBLIC BOOLEAN  close_push_notification_channel_for_applicationasync(char * appkey)
{
    return FALSE;
}
 
 /*get global settings api,  will return true or false. True is push notification service is on, otherwise is off.
**it can get push notification service state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_setting(void)
{
    return TRUE;
}


 /*get global settings api,  will return true or false. True is push notification alert is on, otherwise is off.
**it can get push notification alert setting state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_alert_setting(void)
{
    return FALSE;
}
 
/*get notification message detail api, 
**input paramters: appkey, index
**return paramters: struct pionter, include message detail information*/
PUBLIC WNS_PUSH_NOTIFICATION_T *get_notifcation_message_information(char *appkey, int index)
{
    return NULL;
}

/*get notification message  counts
**input paramters: appkey
**return:  application notification message counts.*/
PUBLIC int get_notification_message_count(char *appkey)
{
    return 0;
}

/*clear notification in idle api, 
**input paramters: 
**appname, application name used by bind appkey and application. 
**return paramters: void*/
PUBLIC void clear_push_notification_in_idle_screen(char* appname)
{
    return;
}



/*remove app api
**input paramters: appname, 
**return:  void.*/
PUBLIC void remove_app_event_handler(char *appname)
{
    return;
}

