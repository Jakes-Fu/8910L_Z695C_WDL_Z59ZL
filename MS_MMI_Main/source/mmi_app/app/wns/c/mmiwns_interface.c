/****************************************************************************
** File Name:      mmiwns_interface.c                                            *
** Author:         grant.peng                                              *
** Date:           24/07/2012                                               *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the wns interface for java application.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 24/07/2012    grant.peng      Create
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
    PNS_CREATE_CHANNEL_SIGNAL_T *sendSignal = PNULL;
    SCI_TRACE_LOW("[wns]:create_push_notification_channel_for_applicationasync(), enter.");

    SCI_TRACE_LOW("[wns]:create pns channel, appname(%s), appkey(%s).func_ptr(0x%04x)", 
            appname, appkey, func_ptr);


    if(PNULL != appname && PNULL != appkey && PNULL != func_ptr)
    {
        int appname_len = strlen(appname);
        int appkey_len = strlen(appkey);


        appname_len = MIN(appname_len, WNS_APP_NAME_MAX_LEN); 
        appkey_len = MIN(appkey_len, APP_KEY_MAX_LEN); 

        SCI_TRACE_LOW("[wns]:create pns channel, appname_len(%d), appkey_len(%d).", 
                                        appname_len, appkey_len);

        if(!get_push_notification_setting())
        {
            SCI_TRACE_LOW("[wns]: User have closed the push notification switch. Create channel Failed");
            func_ptr(appname, appkey, PNULL);
            return WNS_STATUS_FAILED;
        }

        //creat signal
        MMI_CREATE_SIGNAL(sendSignal, 
                                                (uint16)MSG_PNS_CREATE_CHANNEL_REQ, 
                                                sizeof(PNS_CREATE_CHANNEL_SIGNAL_T), 
                                                P_APP);    
        strncpy(sendSignal->appname, appname, appname_len);
        strncpy(sendSignal->app_key, appkey, appkey_len);
        sendSignal->get_channel_info_func_cb = func_ptr;

        //send signal
        MMI_SEND_SIGNAL(sendSignal, P_APP);

        return WNS_STATUS_INPROGRESS;
    }

 
    return WNS_STATUS_BADPARAM;
    
}

 /*close channel api, 
**input paramters: appkey, Microsoft will provide it for every application. We need apply it.*/
PUBLIC BOOLEAN  close_push_notification_channel_for_applicationasync(char * appkey)
{
    PNS_CLOSE_CHANNEL_SIGNAL_T *sendSignal = PNULL;
    int appkey_len = 0;

    SCI_TRACE_LOW("[wns]:close_push_notification_channel_for_applicationasync(), enter.");

    SCI_TRACE_LOW("[wns]:close pns channel, appkey(%s).", appkey);

    if(PNULL == appkey)
    {
        SCI_TRACE_LOW("[wns]:close_push_notification_channel_for_applicationasync(), para error.");
        return FALSE;
    }
    
    appkey_len = strlen(appkey);
    appkey_len = MIN(appkey_len, APP_KEY_MAX_LEN); 
   
    //creat signal
    MMI_CREATE_SIGNAL(sendSignal, 
                                            (uint16)MSG_PNS_CLOSE_CHANNEL_REQ, 
                                            sizeof(PNS_CLOSE_CHANNEL_SIGNAL_T), 
                                            P_APP);    
    strncpy(sendSignal->app_key, appkey, appkey_len);

    //send signal
    MMI_SEND_SIGNAL(sendSignal, P_APP);
 
    return TRUE;
    
}
 
 /*get global settings api,  will return true or false. True is push notification service is on, otherwise is off.
**it can get push notification service state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_setting(void)
{
//#ifdef UNISOC_VERSION

    BOOLEAN b_ret = FALSE;
    SCI_TRACE_LOW("[wns]:get_push_notification_setting(), enter.");
    b_ret = MMISet_GetNotification(MMISET_NOTIFICATION_OTHERS/*MMINV_SET_NOTIFICATION_OTHERS*/);
    return b_ret;
    
//#else //#ifdef UNISOC_VERSION

//    return TRUE;

//#endif //#ifdef UNISOC_VERSION

}


 /*get global settings api,  will return true or false. True is push notification alert is on, otherwise is off.
**it can get push notification alert setting state at the current phone*/ 
PUBLIC BOOLEAN get_push_notification_alert_setting(void)
{
    BOOLEAN b_ret = FALSE;

    b_ret = MMISet_GetNotificationAlert();
    SCI_TRACE_LOW("[wns]:get_push_notification_alert_ring_setting(): b_ret(%d)", b_ret);
    return b_ret;
}
 
/*get notification message detail api, 
**input paramters: appkey, index
**return paramters: struct pionter, include message detail information*/
PUBLIC WNS_PUSH_NOTIFICATION_T *get_notifcation_message_information(char *appkey, int index)
{
    BOOLEAN b_ret = FALSE;
    WNS_PUSH_NOTIFICATION_T *wns_push_ptr = PNULL;
    int push_count = MMIWNS_GetNotificationCount(appkey);
    PNS_MGR_APP_INFO_T *app_info = MMIWNS_GetAppInfoByAppKey(appkey);

    SCI_TRACE_LOW("[wns]:get_notifcation_message_information(), enter.");
    
    if(PNULL != app_info && 0 != push_count && index < push_count)
    {
        SCI_TRACE_LOW("[wns]:get_notifcation_message_information(), appkey(%s).", appkey);
        
        do
        {
            wns_push_ptr = SCI_ALLOC_APP(sizeof(WNS_PUSH_NOTIFICATION_T));
            if(PNULL == wns_push_ptr)
            {
                SCI_TRACE_LOW("[wns]:get_notifcation_message_information(), SCI_ALLOC_APP failed.");
                b_ret = FALSE;
                break;
            }
            SCI_MEMSET(wns_push_ptr, 0, sizeof(WNS_PUSH_NOTIFICATION_T));
            wns_push_ptr->notification_type = app_info->push_notification_list[index].notification_type;
            SCI_MEMCPY(wns_push_ptr->time, app_info->push_notification_list[index].time, TIME_MAX_LEN);
            SCI_MEMCPY(wns_push_ptr->channel_id, app_info->channel_id, CHANNEL_ID_MAX_LEN);
            wns_push_ptr->content_length = strlen(app_info->push_notification_list[index].contents);

            wns_push_ptr->contents = SCI_ALLOC_APP(wns_push_ptr->content_length + 1);
            if(PNULL == wns_push_ptr->contents)
            {
                SCI_TRACE_LOW("[wns]:get_notifcation_message_information(), SCI_ALLOC_APP failed.");
                b_ret = FALSE;
                break;
            }
            SCI_MEMSET(wns_push_ptr->contents, 0, wns_push_ptr->content_length + 1);

            SCI_MEMCPY(wns_push_ptr->contents, 
                        app_info->push_notification_list[index].contents, 
                        wns_push_ptr->content_length + 1);
            b_ret = TRUE;
            
        }while(0);

        if(!b_ret)
        {
            if(PNULL != wns_push_ptr)
            {
                if(PNULL != wns_push_ptr->contents)
                {
                    SCI_FREE(wns_push_ptr->contents);
                    wns_push_ptr->contents = PNULL;
                }
                SCI_FREE(wns_push_ptr);
                wns_push_ptr = PNULL;
            }
        }
    }

    return wns_push_ptr;
}

/*get notification message  counts
**input paramters: appkey
**return:  application notification message counts.*/
PUBLIC int get_notification_message_count(char *appkey)
{
    SCI_TRACE_LOW("[wns]:get_notification_message_count(), enter.");
    return MMIWNS_GetNotificationCount(appkey);
}

/*clear notification in idle api, 
**input paramters: 
**appname, application name used by bind appkey and application. 
**return paramters: void*/
PUBLIC void clear_push_notification_in_idle_screen(char* appname)
{
    //给PNS manager发送清除消息的通知，消息里带上appname
    PNS_CLEAR_NOTIFICATION_SIGNAL_T *sendSignal = PNULL;
    int appname_len = 0;

    if(PNULL == appname)
    {
        SCI_TRACE_LOW("[wns]:clear_push_notification_in_idle_screen(), para error.");
        return;
    }
    
    SCI_TRACE_LOW("[wns]: clear_push_notification_in_idle_screen(), appname(%s)", appname);
    
    appname_len = strlen(appname);
    appname_len = MIN(appname_len, WNS_APP_NAME_MAX_LEN); 
   
    //creat signal
    MMI_CREATE_SIGNAL(sendSignal, 
                                            (uint16)MSG_PNS_CLEAR_NOTIFICATION, 
                                            sizeof(PNS_CLEAR_NOTIFICATION_SIGNAL_T), 
                                            P_APP);    
    strncpy(sendSignal->appname, appname, appname_len);

    //send signal
    MMI_SEND_SIGNAL(sendSignal, P_APP);
 
    return;
    
}



/*remove app api
**input paramters: appname, 
**return:  void.*/
PUBLIC void remove_app_event_handler(char *appname)
{
    PNS_APP_REMOVED_SIGNAL_T *sendSignal = PNULL;
    int appname_len = 0;

    SCI_TRACE_LOW("[wns]:remove_app_event_handler(), enter.");

    SCI_TRACE_LOW("[wns]:remove_app_event_handler(), appname(%s).", appname);

    if(PNULL == appname)
    {
        SCI_TRACE_LOW("[wns]:remove_app_event_handler(), para error.");
        return;
    }
    
    appname_len = strlen(appname);
    appname_len = MIN(appname_len, WNS_APP_NAME_MAX_LEN); 
   
    //creat signal
    MMI_CREATE_SIGNAL(sendSignal, 
                                            (uint16)MSG_PNS_APP_IS_REMOVED, 
                                            sizeof(PNS_APP_REMOVED_SIGNAL_T), 
                                            P_APP);    
    strncpy(sendSignal->appname, appname, appname_len);

    //send signal
    MMI_SEND_SIGNAL(sendSignal, P_APP);
 
    return;
    
}

