/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_commonfun.c#12 $
 */

#include "jbed_exported_defines.h"
#include <window_parse.h>
#include <mmk_type.h>
#include <mmijava_id.h>
#include <mmi_appmsg.h>
#include <mmi_common.h>
#include <java_mmi_text.h>
#include <mmidisplay_data.h>
#include <mmipub.h>
//#include <mmiset.h>
#include <mmiset_export.h>
#include <cpl_tpal.h>
#include <cpl_tm.h>
#include <cpl_midletinfo.h>
#include "mmijava_common.h"
#include <jbed_main.h>
#include <mmicom_string.h>
#include <gui_ucs2b_converter.h>
#include "mmiidle_statusbar.h"
#include <mmi_default.h>
#include <opl_global.h>
#include <mmiudisk_export.h>
#include "cpl_file.h"
#include <java_mmi_image.h>
#include "mmijava_menutable.h"
#include "mmijava_midletinfowin.h"
#include "mmijava_backgroundwin.h"
#include "cpl_net.h"

#if defined(DYNAMIC_MAINMENU_SUPPORT)
#include "mmijava_dynamicmenufun.h"
#endif

#if defined(BROWSER_SUPPORT) && !defined(PLATFORM_SC8800G)
#include <mmibrowser_export.h>
#endif

#if defined(MICROSOFT_WNS_SUPPORT)
#include <mmiwns_export.h>
#endif

#include "mmijava_installprocesswin.h"
#include "mmijava_export.h"

#include "mmi_mainmenu_matrix.h"
#include "mmicom_banner.h"

/******************************************************************************/
/**                  inputmethod framework relevant                          **/
/******************************************************************************/
LOCAL MMIEvent * s_callback_mmi_event_head_ptr = NULL;

#ifdef CHECK_MEMORY_ALLOC
LOCAL int alloc_times = 0; //don't remove, used for debug
#endif

SCI_MUTEX_PTR g_mutex_post_eventlist_ptr = NULL;
BOOLEAN g_jbed_mutex_flag = FALSE;
/*---------------------------------------------------------------------------*/
/*                          EXTERN FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
int g_delete_suiteid = -1;
int g_destroy_suiteid = -1;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : waiting popup window messaging hanlder
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_handleWaitingWinMsg(
                                                MMI_WIN_ID_T     win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM           param
                                            );


/**************************************************************************************************
  * Description: copy permission
  * Input Paras:
      > mEvent :
  * Output Paras:
      > return : void
  * note :
  *************************************************************************************************/
LOCAL void CopyPermissionEvent(MMIEvent * mEvent)
{
    int i;
    int result = TRUE;
    MMIEvent * mEvt = NULL;
    NAMSPermssions_t * permissions_src = NULL;
    NAMSPermssions_t * permissions_dst = NULL;

    if (mEvent->data == NULL)
    {
        SCI_TRACE_LOW("CopyPermissionEvent - ERROR: Wrong parameters.");
        return ;
    }

    permissions_src = (NAMSPermssions_t *)(mEvent->data);

    do
    {
        permissions_dst = (NAMSPermssions_t *)SCI_ALLOCA(sizeof(NAMSPermssions_t));

        if (permissions_dst == NULL)
        {
            return;
        }
        permissions_dst->permissions_num = permissions_src->permissions_num;

        permissions_dst->permssions_body =
            (NAMSPermissionsBody_t *)SCI_ALLOCA(sizeof(NAMSPermissionsBody_t) * (size_t)permissions_dst->permissions_num);

        if (permissions_dst->permssions_body == NULL)
        {
            SCI_FREEIF(permissions_dst);
            return;
        }
        SCI_MEMSET(permissions_dst->permssions_body, 0x0, sizeof(NAMSPermissionsBody_t) * (size_t)permissions_dst->permissions_num);

        for (i = 0; i < permissions_dst->permissions_num; i++)
        {
            int wstrlen = MMIAPICOM_Wstrlen(permissions_src->permssions_body[i].permission_fg_name);
            permissions_dst->permssions_body[i].permission_fg_name = SCI_ALLOCA((wstrlen+1)<<1);
            if (permissions_dst->permssions_body[i].permission_fg_name == NULL)
            {
                result = FALSE;
                break;
            }

            SCI_MEMSET(permissions_dst->permssions_body[i].permission_fg_name, 0x0, (wstrlen+1)<<1);
            SCI_MEMCPY(permissions_dst->permssions_body[i].permission_fg_name,
                       permissions_src->permssions_body[i].permission_fg_name, wstrlen<<1);

            permissions_dst->permssions_body[i].allowed = permissions_src->permssions_body[i].allowed;
            permissions_dst->permssions_body[i].current = permissions_src->permssions_body[i].current;
            permissions_dst->permssions_body[i].changeable = permissions_src->permssions_body[i].changeable;
        }

        if (!result) break;//case failure in for loop.

        mEvt = MMIJAVA_allocMMIEvent();
        if (mEvt == NULL)
        {
            SCI_TRACE_LOW("CopyPermissionEvent - ERROR: alloc mEvt failure.");
            result = FALSE;
            break;
        }

        mEvt->event = mEvent->event;
        mEvt->result = mEvent->result;
        mEvt->event_id = mEvent->event_id;
        mEvt->data_bytes = mEvent->data_bytes;
        mEvt->data = (void *)permissions_dst;
    }
    while(0);

    if (!result)
    {
        for (i = 0; i < permissions_dst->permissions_num; i++)
        {
            SCI_FREEIF(permissions_dst->permssions_body[i].permission_fg_name);
        }
        SCI_FREEIF(permissions_dst->permssions_body);
        SCI_FREEIF(permissions_dst);
        return ;
    }
}

/*****************************************************************************/
//  Description : copy select storage event
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void CopySelectStorageEvent(MMIEvent * mEvent)
{
    int32      i;
    uint16  ** str_arr;
    uint16  ** str_arr_copyed = NULL;
    MMIEvent * mEvt = NULL;

    mEvt = MMIJAVA_allocMMIEvent();
    if (mEvt == NULL)
    {
        SCI_TRACE_LOW("CopySelectStorageEvent - ERROR: alloc mEvt failure.");
        return ;
    }

    if (mEvent->data_bytes != 0 && mEvent->data != NULL)
    {
        str_arr = (uint16 **)mEvent->data;
        str_arr_copyed = SCI_ALLOCA(sizeof(uint16 *) * (mEvent->data_bytes << 1));

        if (str_arr_copyed == NULL)
        {
            SCI_TRACE_LOW("CopySelectStorageEvent - ERROR: no memory!");
            SCI_FREEIF(mEvt);
            return ;
        }
        for (i = 0; i < (mEvent->data_bytes<<1); i++)
        {
            size_t storage_name_len = 0;
            if (str_arr[i] != NULL)
            {
                storage_name_len = MMIAPICOM_Wstrlen(str_arr[i]);
                str_arr_copyed[i] = SCI_ALLOCA((storage_name_len + 1) * sizeof(uint16));
                if (str_arr_copyed[i] == NULL)
                {
                    SCI_TRACE_LOW("CopySelectStorageEvent - ERROR: no memory, how to handle?");
                }
                SCI_MEMSET(str_arr_copyed[i], 0x0, (storage_name_len + 1) * sizeof(uint16));
                MMIAPICOM_Wstrcpy(str_arr_copyed[i], str_arr[i]);
            }
        }
    }

    mEvt->event = mEvent->event;
    mEvt->result = mEvent->result;
    mEvt->data_bytes = mEvent->data_bytes;
    mEvt->event_id = mEvent->event_id;
    mEvt->data = (void *)str_arr_copyed;

}


/*****************************************************************************/
//  Description : copy event
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void CopyMMIEvent(
                                            MMIEvent * mEvent
                                        )
{
    MMIEvent * mEvt = NULL;

    mEvt = MMIJAVA_allocMMIEvent();
    if (mEvt == NULL)
    {
        SCI_TRACE_LOW("CopyMMIEvent - ERROR: alloc mEvt failure.");
        return ;
    }
    mEvt->event = mEvent->event;
    mEvt->result = mEvent->result;
    mEvt->event_id = mEvent->event_id;
    mEvt->data_bytes = mEvent->data_bytes;
    if (mEvent->data_bytes != 0 && mEvent->data != NULL)
    {
        mEvt->data = SCI_ALLOCA(mEvent->data_bytes + 1);
        if (mEvt->data == NULL)
        {
            SCI_TRACE_LOW("CopyMMIEvent - ERROR: alloc data of mEvt failure.");
            MMIJAVA_util_deallocMMIEvent_byEventID(mEvt->event_id);
            return ;
        }
        SCI_MEMSET(mEvt->data, 0x0, (mEvent->data_bytes + 1));
        SCI_MEMCPY(mEvt->data, mEvent->data, mEvent->data_bytes);
    }

}

/*****************************************************************************/
//  Description :find MMIEvent by mmi event ID
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC MMIEvent * MMIJAVA_find_MMIEvent(int mmi_event_id)
{
    MMIEvent * mmi_evt =  NULL;
    //SCI_ASSERT(NULL != g_mutex_post_eventlist_ptr);/*assert verified*/
    if(NULL == g_mutex_post_eventlist_ptr)
        return PNULL;
    SCI_GetMutex(g_mutex_post_eventlist_ptr,SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    mmi_evt = s_callback_mmi_event_head_ptr;
    if(mmi_evt == NULL)
    {
        SCI_TRACE_LOW("==== myriad log: maybe dealloc error! not find MMIEvent(event_id:%d) ===",mmi_event_id);
        SCI_PutMutex(g_mutex_post_eventlist_ptr);
        return NULL;
    }
    while((mmi_evt->event_id != mmi_event_id) && (mmi_evt->next != NULL))
    {
        mmi_evt = mmi_evt->next;
    }
    if(mmi_evt->event_id == mmi_event_id)
    {
        SCI_TRACE_LOW("MMIJAVA_find_MMIEvent, Found the MMIEvent, mmiEvt->event=%d, mmiEvt->event_id=%d", mmi_evt->event, mmi_evt->event_id);
        SCI_PutMutex(g_mutex_post_eventlist_ptr);
        return mmi_evt;
    }
    else
    {
        SCI_TRACE_LOW("==== myriad log: maybe dealloc error! not find MMIEvent(event_id:%d) ===",mmi_event_id);
        SCI_PutMutex(g_mutex_post_eventlist_ptr);
        return NULL;
    }
}

/*****************************************************************************/
//  Description : MMIJAVA_NotifyJVMExit
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_NotifyJVMExit(void)
{
    if(!MMIJAVA_IsInstalling()
#if defined(SUPPORT_BACKGROUND_RUN)
    && Jbed_tm_getRunningAppNum() == 0
#endif
    )
    {
        if (Jbed_getJbedState() > JBED_MAIN_DOWN)
        {
            SCI_TRACE_LOW("==== myriad log: MMIJAVA_NotifyJVMExit request for vm shutdown.");
            //exit jbed lifecycle if no MIDlets are running.
            Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
            /*create timer, when jbed cannot change it's state to down,
             we cloud change the state to let jvm thread to exit though timer out callback.*/
            Jbed_wakeupVM();
#ifdef MMI_TASK_MANAGER
            MMIAPIJAVA_StopApplet(FALSE);
#endif
        }

    }
    else
    {
        SCI_TRACE_LOW("==== myriad log: MMIJAVA_NotifyJVMExit request for vm active.");
        g_jbed_mutex_flag = FALSE;

        Jbed_requestState(JBED_MAIN_ACTIVE, NULL, NULL);
        Jbed_wakeupVM();
    }
}


/*****************************************************************************/
//  Description : waiting popup window messaging hanlder
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_handleWaitingWinMsg(
                                         MMI_WIN_ID_T     win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM           param
                                     )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIDEFAULT_AllowOpenKeylocWin(FALSE);
            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
        case MSG_KEYDOWN_RED:
        case MSG_KEYUP_RED:
        case MSG_KEYLONG_POWER:
        case MSG_KEYUP_POWER:
        case MSG_KEYDOWN_POWER:
        //case MSG_KEYLONG_CAMREA: //just for test
        //case MSG_KEYUP_CAMERA:
        //case MSG_KEYDOWN_CAMERA:
            //ignore
            SCI_TRACE_LOW("MMIJAVA_handleWaitingWinMsg...... redkey");
            return MMI_RESULT_TRUE;
        case MSG_CLOSE_WINDOW:
            MMIDEFAULT_AllowOpenKeylocWin(TRUE);
            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;

        default:
            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
    }

    return result;
}

#ifdef MICROSOFT_WNS_SUPPORT
LOCAL void MMIJAVA_NotifyDeleteWnsdata(int suite_id)
{
    int length = 0;
    uint16 *identifier = NULL;
    char utf8_identifier[MAX_WNS_IDENTIFIER_LEN] = {0};

    if (suite_id < 0)
    {
        return;
    }

    CPL_midletInfo_getSuiteInfoItem(suite_id, MI_WNS_IDENTIFIER, (void *)&identifier);
    if ((identifier != NULL) && ((length = MMIAPICOM_Wstrlen(identifier)) > 0))
    {
        Jbni_convertUcs2ToUtf8(identifier, length, utf8_identifier, MAX_WNS_IDENTIFIER_LEN);
        remove_app_event_handler(utf8_identifier);
    }
    SCI_FREEIF(identifier);
}
#endif

LOCAL MMI_RESULT_E  MMIJAVA_HandleCommonAlertWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetTextDialogBlankTitle(win_id, TRUE);
        break;
    case MSG_NOTIFY_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle "delete" and "delete all" events from JVM
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_HandleDeleteEvent(int event_id)
{
    MMIEvent * p_mmi_evt = NULL;
    p_mmi_evt = MMIJAVA_find_MMIEvent(event_id);
    if(NULL == p_mmi_evt)
    {
        return;
    }
    switch (p_mmi_evt->result)
    {
        case RES_RUNNING_MIDLETS:
#ifdef SUPPORT_BACKGROUND_RUN
            {
                MMI_STRING_T alert_note = {0};

                if (MMIJAVA_GetReplaceText(STR_JAVA_BACKGROUND, g_active_launching_app->sid, &alert_note))
                {
                    MMIPUB_OpenAlertWinByTextPtr(NULL,
                                                 &alert_note,
                                                 NULL,
                                                 IMAGE_PUBWIN_WARNING,
                                                 NULL,
                                                 NULL,
                                                 MMIPUB_SOFTKEY_ONE,
                                                 NULL);
                    SCI_FREEIF(alert_note.wstr_ptr);
                }
                else
                {
                    MMIJAVA_OpenAlertByTextId(STR_JAVA_RUN_ERROR);
                }
            }
#endif
#ifdef SUPPORT_DELETE_ALL_FUNCTION
            if(MMI_NAMS_EVENT_REMOVE_ALL == p_mmi_evt->event)
            {
                //Jbed_nAms_intialize(FALSE);
                //Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);
                MMIJAVA_EnvironmentInit();
            }
#endif
            break;

        case RES_NOT_FOUND:
            if(MMI_NAMS_EVENT_REMOVE == p_mmi_evt->event)
            {
#ifdef MICROSOFT_WNS_SUPPORT
                MMIJAVA_NotifyDeleteWnsdata(g_delete_suiteid);
#endif
#ifdef DYNAMIC_MAINMENU_SUPPORT
                MMIJAVA_DeleteSuiteRecordInMainmenu(g_delete_suiteid);
#endif
                CPL_midletInfo_removeSuite(g_delete_suiteid);
            }
            Jbed_nAms_updateMIDletInfo();
            break;

        default:
        case RES_ERROR:
#ifndef MMI_GUI_STYLE_MINISCREEN
            MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_OPERATE_ERROR, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
            MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_OPERATE_ERROR, NULL, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif

#ifdef SUPPORT_DELETE_ALL_FUNCTION
            if(MMI_NAMS_EVENT_REMOVE_ALL == p_mmi_evt->event)
            {
                //Jbed_nAms_intialize(FALSE);
                //Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);
                MMIJAVA_EnvironmentInit();
            }
#endif
            break;

        case RES_OK:
            if(MMI_NAMS_EVENT_REMOVE == p_mmi_evt->event)
            {
#ifdef MICROSOFT_WNS_SUPPORT
                MMIJAVA_NotifyDeleteWnsdata(g_delete_suiteid);
#endif
#ifdef DYNAMIC_MAINMENU_SUPPORT
                MMIJAVA_DeleteSuiteRecordInMainmenu(g_delete_suiteid);
#endif
            }
            else
            {
#ifdef MICROSOFT_WNS_SUPPORT
                int  i, suite_count;
                int* list_ids = NULL;

                suite_count = Jbed_nAms_getSuitesCount(NULL, 0);
                if (suite_count > 0)
                {
                    list_ids = SCI_ALLOCA((size_t)suite_count * sizeof(int));
                    if (list_ids != NULL)
                    {
                        Jbed_nAms_getSuitesCount(list_ids, suite_count);
                        for (i = 0; i < suite_count; i++)
                        {
                            MMIJAVA_NotifyDeleteWnsdata(list_ids[i]);
                        }
                        SCI_FREEIF(list_ids);
                    }
                }
#endif
#ifdef DYNAMIC_MAINMENU_SUPPORT
                MMIJAVA_DeleteAllSuitesRecorInMainmenu();
#endif
            }
            Jbed_nAms_updateMIDletInfo();
            break;
    }
    g_delete_suiteid = -1;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMAINMENU_SetState(MMIMAINMENU_UNINSTALL_STATE, FALSE, PNULL);
#endif
    MMIJAVA_NotifyJVMExit();
    MMIJAVA_util_deallocMMIEvent_byEventID(p_mmi_evt->event_id);

}

/*****************************************************************************/
//  Description : handle "Run" and "Runtime error" events from JVM
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_HandleRunAndRuntimeEvent(int event_id)
{
    MMIEvent * p_mmi_evt = NULL;
    p_mmi_evt = MMIJAVA_find_MMIEvent(event_id);
    if(NULL == p_mmi_evt)
    {
        return;
    }

    if(NULL == p_mmi_evt->data)
    {
        if(p_mmi_evt->result == RES_NOT_FOUND)
        {
#ifndef MMI_GUI_STYLE_MINISCREEN
            MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_FILE_MISSING, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
            MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_FILE_MISSING, NULL, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif
        }
        else
        {
#ifndef MMI_GUI_STYLE_MINISCREEN
            MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_RUN_ERROR, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
            MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_RUN_ERROR, NULL, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif
        }
    }
    else
    {
        MMI_STRING_T text_str = {0};
        int i=0,length =0;
        unsigned char   tmp_buf[400] ;
        unsigned short  data_buf[400] ;

        SCI_MEMSET(tmp_buf, 0,sizeof(tmp_buf ));
        SCI_MEMSET(data_buf,0,sizeof(data_buf));

        SCI_MEMCPY((unsigned char*)data_buf,(unsigned char*)p_mmi_evt->data,p_mmi_evt->data_bytes);
        length = p_mmi_evt->data_bytes>>1;
        for(i=0;i<length;i++)
        {
            data_buf[i] = data_buf[i] & 0x00ff;
        }

        MMIAPICOM_WstrToStr(data_buf,tmp_buf);

        if(MMIAPICOM_Stristr((char *)tmp_buf,"java.lang.OutOfMemoryError") !=NULL)
        {
            MMI_GetLabelTextByLang(STR_JAVA_NO_MEMORY,&text_str);
#ifndef MMI_GUI_STYLE_MINISCREEN
            MMIPUB_OpenAlertWinByTextPtr( NULL,NULL, &text_str, IMAGE_PUBWIN_WARNING, NULL,NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
            MMIPUB_OpenAlertWinByTextPtr( NULL,&text_str, NULL, IMAGE_PUBWIN_WARNING, NULL,NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif

        }
        else
        {
            #ifdef CMCC_41_SUPPORT
            // As cets cases required, display platform string which contains the Chinese format instead of exception message passing by vm.
            if (p_mmi_evt->event == MMI_NAMS_EVENT_RUNTIME_ERROR)
            {
                MMI_GetLabelTextByLang(STR_JAVA_RUN_ERROR, &text_str);
            }
            else
            #endif
            {
                text_str.wstr_ptr = p_mmi_evt->data;
                text_str.wstr_len = p_mmi_evt->data_bytes>>1;
            }
            /*
             * The text passed by vm may exceed one screen,
             * use MMIPUB_OpenTextDialogByTextPtr of scrolling the text to show.
             */
            MMIPUB_OpenTextDialogByTextPtr(PNULL,
                                           &text_str,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           PNULL,
                                           MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                           MMIJAVA_HandleCommonAlertWinMsg);
        }
    }

    if((p_mmi_evt->event == MMI_NAMS_EVENT_RUN && (p_mmi_evt->result == RES_NOT_FOUND || p_mmi_evt->result == RES_ERROR)) || (p_mmi_evt->event == MMI_NAMS_EVENT_RUNTIME_ERROR))
    {
        /* We should use Jbed_tm_setActiveRunningApp instead of g_active_launching_app to judge whether
         * the running app information has been initialized, because the error may happen
         * before event of EVENT_RUNNING and after envent of EVENT_RUN.*/
        if (NULL != Jbed_tm_setActiveRunningApp(g_current_launching_app.sid, g_current_launching_app.mid))
        {
            SCI_TRACE_LOW("==== myriad log: MMIJAVA_HandleRunAndRuntimeEvent invoke Jbed_tm_destroyRunningAppInfo ===");
            Jbed_tm_destroyRunningAppInfo(g_current_launching_app.sid, g_current_launching_app.mid);
            if (NULL != g_active_launching_app)
            {
                g_active_launching_app = NULL;
            }
        }
    }
    g_is_start_running_process = FALSE;
    g_jbed_mutex_flag = TRUE;
    MMIJAVA_NotifyJVMExit();
    MMIJAVA_util_deallocMMIEvent_byEventID(p_mmi_evt->event_id);
}


/*****************************************************************************/
//  Description : allocate MMI Event
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC MMIEvent * MMIJAVA_allocMMIEvent(void)
{
    MMIEvent *aEvt;

    aEvt = (MMIEvent *)SCI_ALLOCA(sizeof(MMIEvent));
    if (aEvt == NULL)
    {
        SCI_TRACE_LOW("MMIJAVA_allocMMIEvent - ERROR: no memory!!!");
        return NULL;
    }
#ifdef CHECK_MEMORY_ALLOC
    SCI_TRACE_LOW("==== myriad log: MMIEvent alloc occurs,alloc_times add 1,total(%d) ===",++alloc_times); //for check memory
#endif
    SCI_TRACE_LOW("MMIJAVA_allocMMIEvent - alloc Success! MMIEvent handle = %ld", aEvt);
    SCI_MEMSET(aEvt, 0x0, sizeof(MMIEvent));
    //SCI_ASSERT(NULL != g_mutex_post_eventlist_ptr);/*assert verified*/
    if(NULL == g_mutex_post_eventlist_ptr)
    {
        SCI_FREEIF(aEvt);
        return PNULL;
    }

    SCI_GetMutex(g_mutex_post_eventlist_ptr,SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    aEvt->next = s_callback_mmi_event_head_ptr;
    s_callback_mmi_event_head_ptr = aEvt;
    SCI_PutMutex(g_mutex_post_eventlist_ptr);
    return aEvt;
}


/*****************************************************************************/
//  Description :dealloc MMIEvent by mmi event ID
//  Global resource dependence :
//  Author: nix
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_deallocMMIEvent_byEventID(int mmi_event_id)
{
    MMIEvent * pre_event = NULL;
    MMIEvent * cur_event = NULL;
    //SCI_ASSERT(NULL != g_mutex_post_eventlist_ptr);/*assert verified*/
    if(NULL == g_mutex_post_eventlist_ptr)
        return;
    SCI_GetMutex(g_mutex_post_eventlist_ptr,SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    cur_event = s_callback_mmi_event_head_ptr;
    pre_event = cur_event;

    for( ; cur_event!=NULL; pre_event = cur_event,cur_event =cur_event->next)
    {
        if(cur_event->event_id == mmi_event_id)
        {
            break;
        }
    }

    if(NULL == cur_event)
    {
        SCI_TRACE_LOW("MMIJAVA_util_deallocMMIEvent_byEventID, can't find MMIEvent with curevent_id=%d", mmi_event_id);
        SCI_PutMutex(g_mutex_post_eventlist_ptr);
        return;
    }
#ifdef CHECK_MEMORY_ALLOC
    SCI_TRACE_LOW("==== myriad log: MMIEvent dealloc occurs,alloc_times dec 1,total(%d) ===",--alloc_times);//for check memory
#endif
    if(cur_event == s_callback_mmi_event_head_ptr)
    {
        s_callback_mmi_event_head_ptr = cur_event->next;
    }
    else
    {
        pre_event->next = cur_event->next;
    }
    SCI_PutMutex(g_mutex_post_eventlist_ptr);
    switch(cur_event->event)
    {
        case MMI_NAMS_EVENT_PERMISSIONS:
        {
            int i =0;
            NAMSPermssions_t * permissions = NULL;

            SCI_TRACE_LOW("==== myriad log: permission dealloc ===");
            permissions = (NAMSPermssions_t *)(cur_event->data);
            if(permissions !=NULL)
            {
                for (i = 0; i < permissions->permissions_num; i++)
                {
                    SCI_FREEIF(permissions->permssions_body[i].permission_fg_name);
                }
                SCI_FREEIF(permissions->permssions_body);
                SCI_FREEIF(permissions);
            }
            SCI_FREEIF(cur_event);
        }
        break;

        case MMI_NAMS_EVENT_SELECT_STORAGE:
        {
            int i =0;
            uint16 ** str_arr = (uint16 **)(cur_event->data);

            SCI_TRACE_LOW("==== myriad log: storage dealloc ===");
            for (i = 0; i < ((cur_event->data_bytes)<<1); i++)
            {
                SCI_FREEIF(str_arr[i]);
            }
            SCI_FREEIF(str_arr);
            SCI_FREEIF(cur_event);
        }
        break;

        default:
        {
            SCI_TRACE_LOW("==== myriad log: normal dealloc ===");
            SCI_FREEIF(cur_event->data);
            SCI_FREEIF(cur_event);
        }
        break;
    }
}

/*****************************************************************************/
//  Description :dealloc all MMIEvent (some may not be dealloc)
//  Global resource dependence :
//  Author: nix.long
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_deallocAllMMIEvent(void)
{
    MMIEvent * temp_event = NULL;
    MMIEvent * cur_event  = NULL;

    //SCI_ASSERT(NULL != g_mutex_post_eventlist_ptr);/*assert verified*/
    if(NULL == g_mutex_post_eventlist_ptr)
        return;

    SCI_GetMutex(g_mutex_post_eventlist_ptr,SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    cur_event = s_callback_mmi_event_head_ptr;
#ifdef CHECK_MEMORY_ALLOC
    SCI_TRACE_LOW("====myriad log: MMIJAVA_util_deallocAllMMIEvent(),alloc_times(%d)===",alloc_times);//for check memory
#endif
    while(cur_event != NULL)
    {
#ifdef CHECK_MEMORY_ALLOC
        SCI_TRACE_LOW("==== myriad log: MMIEvent dealloc occurs,alloc_times dec 1,total(%d) ===",--alloc_times);//for check memory
#endif
        temp_event = cur_event->next;

        switch(cur_event->event)
        {
            case MMI_NAMS_EVENT_PERMISSIONS:
            {
                int i =0;
                NAMSPermssions_t * permissions = NULL;

                SCI_TRACE_LOW("==== myriad log: permission dealloc ===");
                permissions = (NAMSPermssions_t *)(cur_event->data);
                if(permissions !=NULL)
                {
                    for (i = 0; i < permissions->permissions_num; i++)
                    {
                        SCI_FREEIF(permissions->permssions_body[i].permission_fg_name);
                    }
                    SCI_FREEIF(permissions->permssions_body);
                    SCI_FREEIF(permissions);
                }
                SCI_FREEIF(cur_event);
            }
            break;

            case MMI_NAMS_EVENT_SELECT_STORAGE:
            {
                int i =0;
                uint16 ** str_arr = (uint16 **)(cur_event->data);

                SCI_TRACE_LOW("==== myriad log: storage dealloc ===");
                for (i = 0; i < ((cur_event->data_bytes)<<1); i++)
                {
                    SCI_FREEIF(str_arr[i]);
                }

                SCI_FREEIF(cur_event);
            }
            break;

            default:
            {
                SCI_TRACE_LOW("==== myriad log: normal dealloc ===");
                SCI_FREEIF(cur_event->data);
                SCI_FREEIF(cur_event);
            }
            break;
        }

        cur_event  = temp_event;
    }

    s_callback_mmi_event_head_ptr = NULL;
    SCI_PutMutex(g_mutex_post_eventlist_ptr);
}
/*****************************************************************************/
//  Description : handle comm info java MMI event, such as delete, app details, delete all, runtime error
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_HandleCommInfoEvent(
                                                 int event_id
                                              )
{
    MMIEvent * p_mmi_evt = NULL;
    p_mmi_evt = MMIJAVA_find_MMIEvent(event_id);
    if(NULL == p_mmi_evt)
    {
        return;
    }

    //??¨®¨¤¦Ì?waiting¡ä¡ã?¨²¡ê??¨²thread exito¨®¡ê??¨´1?¡À?¡ê?¡¤??¨°?¨¢¨®D?¨²¡ä?2?¡Á?¦Ì??¨º¨¬a
    switch(p_mmi_evt->event)
    {
        case MMI_NAMS_EVENT_RUNTIME_ERROR:
        case MMI_NAMS_EVENT_RUN:
            MMIJAVA_HandleRunAndRuntimeEvent(event_id);
            break;
        case MMI_NAMS_EVENT_INFO:
            MMIJAVA_util_closeWaitingWin(3);
            MMIJAVA_OpenMidletInfoWin(event_id);
            break;
        case MMI_NAMS_EVENT_REMOVE:
 #ifdef SUPPORT_DELETE_ALL_FUNCTION
        case MMI_NAMS_EVENT_REMOVE_ALL:
 #endif
            MMIJAVA_HandleDeleteEvent(event_id);
            break;
        default:
            MMIJAVA_util_closeWaitingWin(3);
            MMIJAVA_util_deallocMMIEvent_byEventID(p_mmi_evt->event_id);
            break;
    }
}

/*****************************************************************************/
//  Description : handle destroyed event for running Midlet
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_handleDestroyed(
                                           int event_id
                                        )
{
    int sid = 0;
    int mid = 0;
    int index = 0;
    char * sdata = NULL;
    char * pOriginal = NULL;

    MMIEvent * p_mmi_evt = MMIJAVA_find_MMIEvent(event_id);
    if(NULL == p_mmi_evt)
    {
        return;
    }

    sdata = SCI_ALLOCA(p_mmi_evt->data_bytes);

    if(sdata == NULL)
    {
        SCI_TRACE_LOW("[mmijava_wintab_common.c] ERROR: Not enough memory to malloc!!");
        return;
    }

    GUI_WstrToUTF8((uint8*)sdata, p_mmi_evt->data_bytes, p_mmi_evt->data, p_mmi_evt->data_bytes);
    pOriginal = sdata;

    while(((*sdata) !='_') && ((*sdata) != '\0'))
    {
        sdata++;
        if(CRTL_isdigit(*sdata)) /*lint !e718 !e48*/
        {
            sid = (sid * 10) + (*sdata - '0');
        }
    }

    SCI_FREE(pOriginal);

    pOriginal = NULL;
    sdata = NULL;
    mid = p_mmi_evt->result;


    Jbed_tm_destroyRunningAppInfo(sid, mid);

#ifdef JAVA_SUPPORT_TYPE_MVM
    #ifdef MMI_TASK_MANAGER
        // when MMIJAVA_GetIsDestroyAllMIDlets() is TRUE, the JBED_COMM_WAITING_WIN_ID windows
        // will not disappear until all midlets has been destroyed.
        if (!MMIJAVA_GetIsDestroyAllMIDlets() && Jbed_tm_getRunningAppNum() > 0)
    #else
        if (Jbed_tm_getRunningAppNum() > 0)
    #endif //MMI_TASK_MANAGER
        {
            MMIJAVA_util_closeWaitingWin(3);
        }
    #ifdef MMI_TASK_MANAGER
        else if (MMIJAVA_GetIsDestroyAllMIDlets() && Jbed_tm_getRunningAppNum() == 0)
        {
            /* all midlets are destroyed. */
            MMIJAVA_SetIsDestroyAllMIDlets(FALSE);// reset related flag when all midlets has been destroyed.
        }
    #endif
#else
    #ifndef AMS_TCK_RUNNER
    if (!MMK_IsOpenWin(JBED_COMM_WAITING_WIN_ID) && !MMK_IsOpenWin(MMIPUB_ALERT_TEXT_WIN_ID) && isJbedThreadExist())
    {
        /* If MIDlet is exited by itself, there is no waiting win for JVM exiting.
         * Create it here for the case.
         */
        MMI_STRING_T prompt = {0};
        MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
        MMIJAVA_util_openWaitingWin(&prompt);
    }
    #endif
#endif

#ifdef SUPPORT_TASK_MANAGER
    if(MMK_IsOpenWin(JBED_TM_LIST_MENU_WIN_ID))
    {
        MMK_SendMsg(JBED_TM_LIST_MENU_WIN_ID, MSG_JAVA_NOTIFY_TASKLIST_UPDATE, PNULL);
    }
#endif
    //MMIJAVA_NotifyJVMExit();
    MMIJAVA_util_deallocMMIEvent_byEventID(p_mmi_evt->event_id);
}

/*****************************************************************************/
//  Description : handle the event of callback
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_amsCB(
                                  MMIEvent * event
                              )
{
    SCI_TRACE_LOW("MMIJAVA_util_amsCB, mEvent->event= %d, mEvent->event_id=%d", event->event, event->event_id);
    switch (event->event)
    {
        case MMI_NAMS_EVENT_PERMISSIONS:
            // Do permission event copy  from java to MMI side
            CopyPermissionEvent(event);
            break;
        case MMI_NAMS_EVENT_SELECT_STORAGE:
            //Do storage event copy  from java to MMI side
            CopySelectStorageEvent(event);
            break;
        default:
            // Do other event copy from java to MMI side
            CopyMMIEvent(event);
            break;

    }
    MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID, MSG_JAVA_MMI_EVENT, (DPARAM)&(event->event_id), sizeof(event->event_id));
}


/*****************************************************************************/
//  Description :judge the win of javalist's status
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC bool_t Jbed_is_javalist_open()
{
#if defined MMI_PDA_SUPPORT && defined SUPPORT_TASK_MANAGER
    return MMK_IsOpenWin(JBED_APP_WIN_ID);
#else
    return MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID) || MMK_IsOpenWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
#endif
}

/*
 * jad_url - push's input; return - pull's output
 */
char * MMIJAVA_JadUrlRepo(REPO_OP_E opt,char * jad_url)
{
    static bool_t repo_full_flag = FALSE;
    static char repo_buffer[REPO_MAX_SIZE] ;

    if(PNULL != jad_url)
    {
    	SCI_TRACE_LOW("===call MMIJAVA_JadUrlRepo(%d,%s) ==",opt,jad_url);
    }
    switch(opt)
    {
        case REPO_TYPE_PUSH:
        {
            if(jad_url != NULL )
            {
                int32_t len = CRTL_strlen(jad_url);
                SCI_TRACE_LOW("===jad url len:%d ==",len);
                CRTL_memset(repo_buffer,0,REPO_MAX_SIZE);
                CRTL_memcpy(repo_buffer,jad_url,MIN(len,REPO_MAX_SIZE-1));
                repo_full_flag = TRUE;
            }
            else
            {
                repo_full_flag = FALSE;
            }
        }
        break;

        case REPO_TYPE_PULL:
        {
            if(repo_full_flag)
            {
                SCI_TRACE_LOW("===return url ptr ==");
                repo_full_flag = FALSE;
                return (char*)repo_buffer;
            }
        }
        break;

        default:
        {
            SCI_TRACE_LOW("===invalid repo operats ==");
            repo_full_flag = FALSE;
        }
        break;
    }

    return NULL;
}

#if defined(BROWSER_SUPPORT) && !defined(PLATFORM_SC8800G)
LOCAL void MMIJAVA_OTAInstallJad_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    int i =0;
    int fileNameLen = 0;
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    char  file_tmp[MMIFILE_FULL_PATH_MAX_LEN]  = {0};

    SCI_TRACE_LOW("===jad callback type  :%d ====",param_ptr->mime_type);
    SCI_TRACE_LOW("===jad callback url   :%s ====",param_ptr->url_ptr);
#ifdef BROWSER_SUPPORT_NETFRONT
    SCI_TRACE_LOW(("===jad callback result:%d ====",param_ptr->result_code));
#endif

    if(param_ptr->mime_type != MMIBROWSER_MIME_TYPE_JAD)
    {
        SCI_TRACE_LOW("===jad callback result:file type is not jad====");
        return;
    }

    if(param_ptr->download_result)
    {
        MMIAPICOM_Wstrcpy((uint16_t*)file_name, (const uint16_t *)param_ptr->file_name);
        fileNameLen = MMIAPICOM_Wstrlen(file_name);

        MMIAPICOM_WstrToStr(param_ptr->file_name,(uint8 *)file_tmp);
        SCI_TRACE_LOW("===jad callback file:%s ====",file_tmp);
        SCI_TRACE_LOW("=====convert====");
        for(i=0;i<fileNameLen;i++)
        {
            if (file_name[i] == '/')
            {
                file_name[i] = '\\';
            }
        }
        SCI_MEMSET(file_tmp,0,MMIFILE_FULL_PATH_MAX_LEN);
        MMIAPICOM_WstrToStr(file_name,(uint8 *)file_tmp);
        SCI_TRACE_LOW("===jad callback file:%s ====",file_tmp);
        MMIJAVA_JadUrlRepo(REPO_TYPE_PUSH,param_ptr->url_ptr);
        #ifdef SUPPORT_JBED_BROWSER_NOT_RUN_SAME_TIME
        MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_JAVA_OTA_INSTALL_START,(DPARAM)file_name,sizeof(file_name));
        #else
        MMIAPIJAVA_Install(file_name, MMIAPICOM_Wstrlen(file_name));
        #endif
    }
    else
    {
        SCI_TRACE_LOW("=====jad download fail!====");
    }
}

LOCAL void MMIJAVA_OTAInstallJar_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    int i =0;
    int fileNameLen = 0;
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    char  file_tmp[MMIFILE_FULL_PATH_MAX_LEN]  = {0};


    SCI_TRACE_LOW("=======================start============================");
    SCI_TRACE_LOW("===jar callback type  :%d ====",param_ptr->mime_type);
    SCI_TRACE_LOW("===jar callback url   :%s ====",param_ptr->url_ptr);
#ifdef BROWSER_SUPPORT_NETFRONT
    SCI_TRACE_LOW("===jar callback result:%d ====",param_ptr->result_code);
#endif

    if(param_ptr->mime_type != MMIBROWSER_MIME_TYPE_JAR)
    {
        SCI_TRACE_LOW("===jar callback result:file type is not jar====");
        return;
    }

    if(param_ptr->download_result)
    {
        MMIAPICOM_Wstrcpy((uint16_t*)file_name, (const uint16_t *)param_ptr->file_name);
        fileNameLen = MMIAPICOM_Wstrlen(file_name);

        MMIAPICOM_WstrToStr(param_ptr->file_name,(uint8 *)file_tmp);
        SCI_TRACE_LOW("===jar callback file:%s ====",file_tmp);
        SCI_TRACE_LOW("=====convert====");
        for(i=0;i<fileNameLen;i++)
        {
            if (file_name[i] == '/')
            {
                file_name[i] = '\\';
            }
        }
        SCI_MEMSET(file_tmp,0,MMIFILE_FULL_PATH_MAX_LEN);
        MMIAPICOM_WstrToStr(file_name,(uint8 *)file_tmp);
        SCI_TRACE_LOW("===jar callback file:%s ====",file_tmp);
        #ifdef SUPPORT_JBED_BROWSER_NOT_RUN_SAME_TIME
        MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_JAVA_OTA_INSTALL_START,(DPARAM)file_name,sizeof(file_name));
        #else
        MMIAPIJAVA_Install(file_name, MMIAPICOM_Wstrlen(file_name));
        #endif
    }
    else
    {
        SCI_TRACE_LOW("=====jar download fail!====");
    }
}
#endif

PUBLIC void MMIJAVA_OTAInstallInit(void)
{
#if defined(BROWSER_SUPPORT) && !defined(PLATFORM_SC8800G)
    MMIBROWSER_MIME_DOWNLOAD_PARAM_T jar_otacallback = {0,PNULL};/*lint !e64*/
    MMIBROWSER_MIME_DOWNLOAD_PARAM_T jad_otacallback = {0,PNULL};/*lint !e64*/

    jad_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAD;
    jad_otacallback.call_back = MMIJAVA_OTAInstallJad_CallBackFunction;
    jar_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAR;
    jar_otacallback.call_back = MMIJAVA_OTAInstallJar_CallBackFunction;
    MMIAPIBROWSER_RegMimeFunc(&jar_otacallback);
    MMIAPIBROWSER_RegMimeFunc(&jad_otacallback);
#endif
}

PUBLIC void DeleteFileIfExist(const uint16* name, int32 nameChars)
{
    if(CPL_file_exists(name, nameChars , JBED_APP_JVM, NULL) == CPL_FILE_ISREG)
    {
        CPL_file_delete(name, nameChars , JBED_APP_JVM, NULL);
    }
}

PUBLIC void MMIJAVA_DeleteTempFile(void)
{
    //FIXME: hard code here
    CPL_file_startup();
    DeleteFileIfExist(L"D:/Java/Installed/t.p", 21);
    DeleteFileIfExist(L"D:/Java/Installed/t.p.obj", 25);
    DeleteFileIfExist(L"E:/Java/Installed/t.p", 21);
    DeleteFileIfExist(L"E:/Java/Installed/t.p.obj", 25);
    DeleteFileIfExist(L"F:/Java/Installed/t.p", 21);
    DeleteFileIfExist(L"F:/Java/Installed/t.p.obj", 25);
    DeleteFileIfExist(L"D:/JBed/Installed/t.p", 21);
    DeleteFileIfExist(L"D:/JBed/Installed/t.p.obj", 25);
    DeleteFileIfExist(L"E:/JBed/Installed/t.p", 21);
    DeleteFileIfExist(L"E:/JBed/Installed/t.p.obj", 25);
    DeleteFileIfExist(L"F:/JBed/Installed/t.p", 21);
    DeleteFileIfExist(L"F:/JBed/Installed/t.p.obj", 25);
    CPL_file_shutdown();
}

PUBLIC BOOLEAN MMIJAVA_HandleJVMEvent(void* event_ptr)
{
    BOOLEAN ret = TRUE;
    EVENT_TASKTOMMI_T *event_tasktommi = (EVENT_TASKTOMMI_T*)event_ptr;
    int event_id = event_tasktommi->eventId;

    switch(event_id)
    {
    case MMI_NAMS_EVENT_START_SCHEDULE:
        /*nothing to do*/
        break;
    case MMI_NAMS_EVENT_START_OS_TIMER:
        OPL_os_createTimerCallback(); /*lint !e718 !e746*/
        break;
    case MMI_NAMS_EVENT_PDP_ACTIVE:
        Jbed_conn_startPDPActive();
        break;
    case MMI_NAMS_EVENT_PDP_DEACTIVE:
        Jbed_conn_startPDPDeactive();
        break;
    case MMI_NAMS_EVENT_START_PUSH_TIMER:
        Jbed_startNextTimePush();
        break;
    case MMI_NAMS_EVENT_JBED_THREAD_EXIT:
        MMIJAVA_HandleJbedExitEvent();
        break;
#ifdef SUPPORT_CAMERA_CAPTURE
    case MMI_NAMS_EVENT_MEDIA_SNAPSHOT:
        CPL_media_doSnapshotOperation(event_tasktommi->snapshotOptParam);
        break;
#endif
    case MMI_NAMS_EVENT_START_LCDBACKLIGHT:
        MMIAPIJAVA_StartJbedFlashBacklight(event_tasktommi->blDuration);
        break;

    case MMI_NAMS_EVENT_STOP_LCDBACKLIGHT:
        MMIAPIJAVA_StopJbedFlashBacklight();
        break;
    case MMI_NAMS_EVENT_COMMAND_INSTALL:
    case MMI_NAMS_EVENT_COMMAND_INSTALL_STATUS:
    case MMI_NAMS_EVENT_COMMAND_INSTALL_PROGRESS:
        MMIJAVA_nAms_handleMMICommandInstallEvent(event_tasktommi);
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

#ifndef DRM_SUPPORT_NONE
/* To prevent build error. Remove this after integrate with DRM is done.
 */
PUBLIC void MMIAPIJAVA_OTAInstallDrm(uint16 * name, char * url)
{
    return; //just for pclint
}
#endif

/*****************************************************************************/
//  Description :Close waiting popup window if exist
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_closeWaitingWin(int test)
{
    /* workaround solution to ensure waiting window is totally opened.
     * Otherwise, may causes some un-expected result, such as crash.
     */
    //SCI_TRACE_LOW("=== wait win check close (%d)===",test);
    if (MMK_IsOpenWin(JBED_COMM_WAITING_WIN_ID))
    {
        //SCI_TRACE_LOW("=== wait win is closed by func (%d)===",test);
        MMK_CloseWin(JBED_COMM_WAITING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Create a waiting popup window
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_openWaitingWin(
                                          MMI_STRING_T * user_prompt
                                      )
{
    int prompt_num = 0;
    MMI_STRING_T default_prompt = {0};
    MMI_GetLabelTextByLang(STR_JAVA_WAITING, &default_prompt);

    if(MMK_IsOpenWin(MMIPUB_ALERT_WIN_ID))
    {
        return;
    }
    prompt_num = (user_prompt == NULL ? 1 : 2);

    if (MMK_IsOpenWin(JBED_COMM_WAITING_WIN_ID))
    {

        MMIPUB_UpdateWaitWinText(TRUE,prompt_num,&default_prompt,user_prompt,NULL,JBED_COMM_WAITING_WIN_ID);
    }
    else
    {
#ifndef MMI_GUI_STYLE_MINISCREEN
        MMIPUB_OpenWaitWin(prompt_num, &default_prompt, user_prompt, NULL, JBED_COMM_WAITING_WIN_ID, IMAGE_NULL,
            ANIM_PUBWIN_WAIT, WIN_THREE_LEVEL, MMIPUB_SOFTKEY_NONE, MMIJAVA_handleWaitingWinMsg);

#else
		MMIPUB_OpenAlertWaitingWin(STR_JAVA_WAITING);
#endif
	}
}


PUBLIC BOOLEAN MMIAPIJAVA_IsWaitingWinOpen(void)
{
    return MMK_IsOpenWin(JBED_COMM_WAITING_WIN_ID);
}

PUBLIC void MMIJAVA_EnvironmentInit(void)
{
    Jbed_nAms_intialize(FALSE);
    Jbed_nAms_registerNotify(MMIJAVA_util_amsCB);

}
/*Edit by script, ignore 2 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527

/*****************************************************************************/
//  Description : Handle State Conflict
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_HandleStateConflict(void)
{
    BOOLEAN is_state_conflict = FALSE;
    BOOLEAN is_install_state = FALSE;
    BOOLEAN is_uninstall_state = FALSE;
    GUI_POINT_T start_point = {0, 0};
    MMICOM_BANNER_INFO_T banner_info = {0};

    is_install_state = MMIMAINMENU_GetState(MMIMAINMENU_INSTALL_STATE) ||
                       MMIMAINMENU_GetState(MMIMAINMENU_PRE_INSTALL_STATE) ||
                       MMIMAINMENU_GetState(MMIMAINMENU_UPDATE_STATE);
    is_uninstall_state = MMIMAINMENU_GetState(MMIMAINMENU_UNINSTALL_STATE);

    if(is_install_state)
    {
        is_state_conflict = TRUE;
        banner_info.banner_type = MMI_BANNER_TEXT;
        banner_info.text1_id = STR_JAVA_INSTALLING_EXT01;
        banner_info.start_point = start_point;
        MMIAPICOM_OpenBannerWin(NULL, banner_info);
    }
    else if(is_uninstall_state)
    {
        is_state_conflict = TRUE;
        banner_info.banner_type = MMI_BANNER_TEXT;
        banner_info.text1_id = STR_JAVA_UNINSTALLING_EXT01;
        banner_info.start_point = start_point;
        MMIAPICOM_OpenBannerWin(NULL, banner_info);
    }

    return is_state_conflict;
}

/* Replace the %U in text string with the midlet name by specified suite id. */
BOOLEAN MMIJAVA_GetReplaceText(MMI_TEXT_ID_T text_id, int32 suite_id, MMI_STRING_T *str_ptr)
{
    BOOLEAN ret = FALSE;
    wchar *name_content = PNULL;
    uint16 name_size = 0;
    wchar wstr_old[] = {L"%U"};

    name_size = Jbed_nAms_querySuitesInfo(suite_id, NAMS_SI_NAME, (void *)&name_content);
    if(0 != name_size)
    {
        uint16 replace_str_len = 0;
        MMI_STRING_T original_str = {0};

        MMI_GetLabelTextByLang(text_id, &original_str);
        replace_str_len = (original_str.wstr_len + name_size + 1) * sizeof(wchar);
        str_ptr->wstr_ptr = (wchar*)SCI_ALLOCA(replace_str_len); //should be freed in caller.
        if (PNULL != str_ptr->wstr_ptr)
        {
            SCI_MEMSET(str_ptr->wstr_ptr, 0, replace_str_len);
            MMIAPICOM_Wstrncpy(str_ptr->wstr_ptr, original_str.wstr_ptr, original_str.wstr_len);
            MMIAPICOM_WstrReplace(str_ptr->wstr_ptr, replace_str_len, wstr_old, name_content);  //replace the string.
            str_ptr->wstr_len = (uint16)MMIAPICOM_Wstrlen(str_ptr->wstr_ptr);
            ret = TRUE;
        }

        SCI_FREEIF(name_content);
    }

    return ret;
}

/*****************************************************************************/
//  Description : Post the banner of MIDlet is running background.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_PostMIDletRunningBanner(void)
{
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T start_point = {0, 0};

    banner_info.banner_type = MMI_BANNER_STR_TEXT;
    if (MMIJAVA_GetReplaceText(STR_JAVA_BACKGROUND, g_active_launching_app->sid, &(banner_info.text1_str)))
    {
        banner_info.start_point = start_point;
        MMIAPICOM_OpenBannerWin(NULL, banner_info);
        SCI_FREEIF(banner_info.text1_str.wstr_ptr);
    }
    else
    {
        MMIJAVA_OpenAlertByTextId(STR_JAVA_RUN_ERROR);
    }
}

/*****************************************************************************/
//  Description : update title on status bar with text id.
//  Global resource dependence :
//  Author: sean.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_UpdateTitle(MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T title_str;

    MMI_GetLabelTextByLang(text_id, &title_str);
    if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
    {
        GUIWIN_UpdateStb();
    }
}

/*****************************************************************************/
//  Description : Return max sim card number to jbed porting layer.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetMaxSimNumber(void)
{
    return MMI_DUAL_SYS_MAX;
}