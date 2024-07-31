/*
 * Copyright 2011-2015 Myriad Group AG. All Rights Reserved.
 * $Id:
 */

#include "mmi_app_myriad_java_trc.h"
#include "jbed_exported_defines.h"
//#include <mmijava_export.h>
#include "mmijava_common.h"
//#include <audio_config.h>
#include "mmijava_id.h"
#include "mmi_appmsg.h"
//#include <mmk_app.h>
//#include <mmi_default.h>
#include <mmifmm_export.h>
#include "mmipub.h"
//#include <window_parse.h>
#include "mmijava_nv.h"
#include "mmijava_permissionsetwin.h"
#include "mmiidle_statusbar.h"
#include "mmijava_backgroundwin.h"
#include "mmijava_netsetwin.h"
#include "mmijava_installprocesswin.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmijava_dynamicmenufun.h"
#include "mmi_mainmenu_matrix.h"
#include "mmijava_suitefun.h"
#include "mmijava_runwin.h"
#include "mmijava_imwin.h"
#include "mmijava_taskmgrwin.h"
#include "mmi_common.h"
#include "mmijava_suitefun.h"
//#include <jbed_global.h>
//#include <jbed_jbni.h>
#include <jbed_main.h>
//#include <javainstance.h>
//#include <jbed_upcall.h>
#include "jbed_file_events.h"
#include "cpl_file.h"
#include <cpl_midletinfo.h>
#include <opl_global.h>
#include <cpl_tpal.h>
#include <ms_msa.h>
#include <jbed_props.h>
//#include <cpl_tm.h>
//#include <cpl_net.h>

rstFactoryCb resetJbedFactoryCB;
#define MMIJAVA_FLASH_BACKLIGHT_DELAY   50
JAVA_FLASH_BACKLIGHT_T  s_flash_backnight_info = {0};
LOCAL void HandleFlashBacklightTimerOut(uint32 praram);

PUBLIC void MMIAPIJAVA_Init(void)
{
    MMIJAVA_OTAInstallInit();
    MMIJAVA_DeleteTempFile();
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIJAVA_InstalledSuiteNV_Init();
#endif
}

PUBLIC void MMIAPIJAVA_OpenMainMenu(void)
{
    MMIJAVA_OpenMainMenu();
}

PUBLIC void MMIAPIJAVA_InitModule(void)
{
#ifdef NATIVE_AMS
    MMIJAVA_RegMenuGroup();
    MMIJAVA_RegWinIdNameArray();
    MMIJAVA_RegNv();
#ifdef MMI_TASK_MANAGER
    MMIJAVA_RegAppletInfo();
#endif
#endif
}

extern bool_t Jbed_Getbackgroundstate(void);

/*****************************************************************************/
//  Description : To see if the java app is running on foreground.
//      1) The midlet is running on foreground.
//      2) It also regards as running on foreground when installing midlet.
//  Global resource dependence:
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsForegroundRuning(void)
{
    if(MMIAPIJAVA_IsJavaRuning())
    {
#ifdef SUPPORT_BACKGROUND_RUN
        if(JBED_MAIN_ACTIVE_FOREGROUND == Jbed_getJbedState() || MMIJAVA_IsInstalling())
#endif
        {
            return TRUE;
        }
    }

    return FALSE;
}

PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void)
{
    return MMK_IsFocusWin(JBED_RUNNING_APP_WIN_ID);
}

PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning(void)
{
    return (Jbed_getJbedState() > 0);
}

PUBLIC const char* MMIAPIJAVA_GetVersion(void)
{
    return Jbed_getVersion();
}


PUBLIC void MMIAPIJAVA_HandSDCardChanged(int sd_type,BOOLEAN is_rootadded)
{
    if (!(MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID) || Jbed_getJbedState() > 0))
        return;

    SCI_TRACE_LOW("=====myriad log: MMIAPIJAVA_HandSDCardChanged enter, sd_type = %d is_rootadded = %d", sd_type, is_rootadded);

    if(Jbed_getJbedState() > 0)
    {
        char *root_name = NULL;
        char *root_path = Jbed_getRootByType(sd_type);

        switch(sd_type)
        {
#ifdef DUAL_TCARD_SUPPORT
        case MMI_DEVICE_SDCARD:
             root_name = JAVA_COMMON_TFCARD1;
             break;

        case MMI_DEVICE_SDCARD_1:
             root_name = JAVA_COMMON_TFCARD2;
             break;
#else
        case MMI_DEVICE_SDCARD:
             root_name = JAVA_COMMON_TFCARD;
             break;
#endif
        default:
             return;
        }

#ifdef PDAP_FC_SUPPORT
        Jbed_file_events_notifyRootChanged(is_rootadded, root_name, root_path);
#endif

        Jbed_ClearFileInitSignal();
        CPL_file_shutdown();
        CPL_file_startup();
    }
    return;
}

/*
 * use the func to handle exit event: MMI_NAMS_EVENT_JBED_THREAD_EXIT
 * called in MMIJAVA_HandleJVMEvent.
 */
PUBLIC void MMIJAVA_HandleJbedExitEvent(void)
{
#if defined(SUPPORT_JBED_BROWSER_NOT_RUN_SAME_TIME) && defined(BROWSER_SUPPORT)
    int flag = 0;
    MMIBROWSER_ENTRY_PARAM * entry_param = NULL;
#endif
    SCI_TRACE_LOW("=====myriad log:MMIAPIJAVA_HandleJbedMMIEvent MMI_NAMS_EVENT_JBED_THREAD_EXIT coming in.");

    Jbed_exitVM();
    if (!Jbed_is_javalist_open())
    {
        Jbed_nAms_finalize();
    }

    MMIJAVA_CloseRunningWin();
    MMIJAVA_util_closeWaitingWin(3);

#ifdef MMI_TASK_MANAGER
    MMIJAVA_CloseJavaAppletWin();
    if (MMIJAVA_GetIsDestroyAllMIDlets()) // reset related flag when all midlets has been destroyed.
    {
        MMIJAVA_SetIsDestroyAllMIDlets(FALSE);
    }
#endif

    MMIJAVA_util_deallocAllMMIEvent();

    /*
     * Notice Here:when jbed lifecycle is end,allow backlight & auto-lock.
     * delete midlet,set FALSE when starting deleting,set TRUE here
     * why: if java is running in forground ,spd allows not to lock screen.
     * but if mvm's demand is changed in future,this way may meet some problems
     */
    //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    MAIN_UpdateIdleJavaState(FALSE);
#if defined(SUPPORT_JBED_BROWSER_NOT_RUN_SAME_TIME) && defined(BROWSER_SUPPORT)
    entry_param = Jpl_getOrset_BrowserStartParam(PARAM_GET,&flag);
    if(flag)
    {
        MMIAPIBROWSER_Entry(entry_param);
        SCI_FREEIF(entry_param->url_ptr);
        Jpl_getOrset_BrowserStartParam(PARAM_INIT_ALL,&flag);
    }
#endif
#if defined(JBED_PREINSTALL) && defined(DYNAMIC_MAINMENU_SUPPORT)
    Jbed_preinstall_CreateShotcutToMainmenu();
#endif
    MMIAPIJAVA_StopJbedFlashBacklight();
    //keep this call in the end of the case:
    if(resetJbedFactoryCB != NULL)
    {
        resetJbedFactoryCB();
    }

    MMIJAVA_HandleInstallPendingEvent();
}

/**************************************************************************************************/
// Description: handle all java MMI Events
// Date:2011-08-22
// Author:Alex.Li
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_HandleJbedMMIEvent(void* event_ptr)
{
    MMIEvent * mmi_evt = NULL;
    int mmi_event = 0;
    int * pevt_id = NULL;
    int event_id = *(int*)event_ptr;
    if(MMIJAVA_HandleJVMEvent(event_ptr))
    {
        return;
    }

    mmi_evt = MMIJAVA_find_MMIEvent(event_id);
    if(NULL == mmi_evt)
    {
        return;
    }
    mmi_event = mmi_evt->event;

    SCI_TRACE_LOW("=====MMIAPIJAVA_HandleJbedMMIEvent mmi_event =%d",mmi_event);

    switch (mmi_event)
    {
    case MMI_NAMS_EVENT_INSTALL:
    case MMI_NAMS_EVENT_INSTALL_QUERY:
    case MMI_NAMS_EVENT_INSTALL_QUERY_YES_NO:
    case MMI_NAMS_EVENT_INSTALL_PREVIEW_JAD:
    case MMI_NAMS_EVENT_INSTALL_CANCEL:
    case MMI_NAMS_EVENT_INSTALL_UNSTOPPABLE:
    case MMI_NAMS_EVENT_INSTALL_PROGRESS:
    case MMI_NAMS_EVENT_INSTALL_STATUS:
    case MMI_NAMS_EVENT_SELECT_STORAGE:
#ifdef CMCC_41_SUPPORT
    case MMI_NAMS_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS:
#endif //CMCC_41_SUPPORT
        //add all install events handler here
        MMIJAVA_nAms_handleMMIInstallEvent(event_id);
        break;

    case MMI_NAMS_EVENT_RUNTIME_ERROR:
    case MMI_NAMS_EVENT_RUN:
    case MMI_NAMS_EVENT_INFO:
    case MMI_NAMS_EVENT_REMOVE:
 #ifdef SUPPORT_DELETE_ALL_FUNCTION
    case MMI_NAMS_EVENT_REMOVE_ALL:
 #endif
        // add comm information window handler here, winID is JBED_COMM_INFO_WIN_ID
        MMIJAVA_nAms_HandleCommInfoEvent(event_id);
        break;

    case MMI_NAMS_EVENT_DESTROYED:
        // add destroyed event handle function here
        MMIJAVA_nAms_handleDestroyed(event_id);
        break;

    case MMI_NAMS_EVENT_PERMISSIONS:
        {
            // add permission event handle function here
            //MMK_CreateWin((uint32*)JBED_CONNECT_PERMISSION_SETTING_WIN_TAB, (ADD_DATA)pevt_id);
            MMIJAVA_util_closeWaitingWin(1);
            MMIJAVA_OpenPermissionWin(event_id);
        }
        break;

    case MMI_NAMS_EVENT_PERMISSIONS_ANSWERS:
        // add permission answer event handle function here
        {
            MMIJAVA_HandlePermissionEvent();
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        }
        break;

    case MMI_NAMS_EVENT_RESUME_SUSPEND_PLAYER:
        {

        }
        break;

    case MMI_NAMS_EVENT_NOTIFY_UDISK_JBED_ISRUN:
        {
            SCI_TRACE_LOW("=== handle MMI_NAMS_EVENT_NOTIFY_UDISK_JBED_ISRUN ==");
#ifndef MMI_GUI_STYLE_MINISCREEN
            MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_BACKGROUND_RUNNING, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
            MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_BACKGROUND_RUNNING, NULL, IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        }
        break;

#ifndef SUPPORT_JBED_BROWSER_NOT_RUN_SAME_TIME
    case MMI_NAMS_EVENT_REQUEST_BROWSER_FROM_MMI:
        {
            int flag = 0;
            MMIBROWSER_ENTRY_PARAM * entry_param = Jpl_getOrset_BrowserStartParam(PARAM_GET,&flag);

        #ifdef CMCC_41_SUPPORT
            int result = MMIAPIBROWSER_Entry(entry_param);
            if ((int32_t)MMIBROWSER_SUCC == result)
            {
            #ifdef MMI_PAUSE_IN_BACKGROUND_AS_DEFAULT
                //NOTE: According to CMCC41 spec, only the keepBackground is specified in CMCC or Manufacturer trusted midlet jad as yes,
                // we need to let the midlet running on background, otherwise, suspend the midlet when platform request browser.
                // FIXME: see from java code, JBED_ams_getCurrentSuiteTrusted actually not only includes CMCC or Manufacturer trusted
                // but also includes TRUSTED_3RD_PARTY_GROUP trusted, so we also regard as background running when keepBackground is specified
                // as yes in 3RD party trusted midlet, which cmcc spec doesn't define the situation clearly.
                if (JBED_ams_getCurrentSuiteTrusted() && JBED_ams_checkCurrentSuiteKeepBackground())
                {
                    SCI_TRACE_LOW("=== myriad log: MMI_NAMS_EVENT_REQUEST_BROWSER_FROM_MMI no pause ==");
                    Jbed_nAms_noPauseInBackground(g_active_launching_app->sid, g_active_launching_app->mid);
                    Jbed_vmWantForeground(FALSE);
                }
                else
            #endif //MMI_PAUSE_IN_BACKGROUND_AS_DEFAULT
                {
                    SCI_TRACE_LOW("=== myriad log: MMI_NAMS_EVENT_REQUEST_BROWSER_FROM_MMI to pause ==");

                    #ifdef JAVA_SUPPORT_TYPE_MVM
                        Jbed_nAms_Pause(g_active_launching_app->sid, g_active_launching_app->mid);
                        Jbed_vmWantForeground(FALSE);
                    #else  // currently for svm, we must request idle state to pause midlet.
                        Jbed_nAms_requestVmToState(STATE_IDLE);
                    #endif
                }
            }
        #else
            MMIAPIBROWSER_Entry(entry_param);
        #endif
            SCI_FREEIF(entry_param->url_ptr);
            Jpl_getOrset_BrowserStartParam(PARAM_INIT_ALL,&flag);
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        }
        break;
#endif

    case MMI_NAMS_EVENT_REQUEST_CALL_FROM_MMI:
        {
            char * url = mmi_evt->data;
            SCI_TRACE_LOW(("=== handle platform request make call url(%s) ==", url));

            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, (uint8 *)url, CRTL_strlen(url), PNULL, PNULL, MMIAPICC_GetCallSimType(MN_DUAL_SYS_MAX), CC_CALL_NORMAL_CALL, PNULL);/*lint !e64*/
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        }
        break;

    case MMI_NAMS_EVENT_CREATE_TEXTEDIT_WIN_FROM_MMI:
    case MMI_NAMS_EVENT_CLOSE_TEXTEDIT_WIN_FROM_MMI:
    case MMI_NAMS_EVENT_CHECK_AND_FORCE_CLOSE_TEXTEDIT_WIN_FROM_MMI:
    {
        SCI_TRACE_LOW(("===handle TEXT EDIT event =="));
        HandleMMIJavaTextEditEvent(mmi_event);
        MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
    }
    break;

#ifdef MICROSOFT_EXTRA_FUNCTION
    case MMI_NAMS_EVENT_CREATE_THUMBNAIL:
        SCI_TRACE_LOW(("===handle CREATE THUMBNAIL event =="));
        Jbed_createThumbnail(mmi_evt->result);
        break;
#endif

    default:
            MMIJAVA_util_deallocMMIEvent_byEventID(mmi_evt->event_id);
        break;
    }

}

/**************************************************************************************************/
// Description: Interface for enter taksmanager from status bar.
// Date:2011-08-30
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_OpenTaskMgr(void)
{
#ifdef  SUPPORT_TASK_MANAGER
    #ifdef  MMI_PDA_SUPPORT
        int enter_taskmgr_flag = 1;
        MMK_CreateParentWin((uint32*)JBED_APP_WIN_TAB, (void*)enter_taskmgr_flag);
    #else
        MMK_CreateWin((uint32*)JBED_TM_LIST_MENU_WIN_TAB, PNULL);
    #endif
#else
    MMIAPIJAVA_OpenMainMenu();
#endif

}


BOOLEAN MMIAPIJAVA_CheckSmsIsForJava(int port)
{
    //SCI_TRACE_LOW(("=== mmijava_export.c: Call MMIAPIJAVA_CheckSmsIsForJava,port(%d)",port));
    //TODO: Check and start JVM when it is down. Otherwise, we need to
    //      check register sms port form file.
    return CPL_sms_CheckSmsIsForJava(port); /*lint !e718 !e746*/
}

BOOLEAN MMIAPIJAVA_RetrieveSmsContent(void* msg)
{
    SCI_TRACE_LOW(("=== mmijava_export.c: Call MMIAPIJAVA_RetrieveSmsContent"));
    return CPL_sms_RetrieveSmsContent(msg); /*lint !e718 !e746*/
}
/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname,
                                             int pathname_len,
                                             wchar *filename,
                                             int filename_len)
{

}

PUBLIC void MMIAPIJAVA_PauseBackgroundJVM(void)
{

#ifdef SUPPORT_BACKGROUND_RUN
    Jbed_vmWantToIdle();
    //SCI_TRACE_LOW:"========Myriad LOG=====: in MMIAPIJAVA_PauseBackgroundJVM()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_EXPORT_220_112_2_18_2_41_44_0,(uint8*)"");
#endif
}

PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM(void)
{

#ifdef SUPPORT_BACKGROUND_RUN
    Jbed_vmWantForeground(TRUE);
    //SCI_TRACE_LOW:"========Myriad LOG=====: in MMIAPIJAVA_ResumeBackgroundJVM()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_EXPORT_227_112_2_18_2_41_44_1,(uint8*)"");

#endif
}

PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void)
{
    //workaround for pclint
    return FALSE;
}

PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void)
{
    RunningApp* raInfo = NULL;
    int runCount = 0;
    BOOLEAN mExit = FALSE;
    int i = 0;
    uint16* buffer = NULL;
    MMIFILE_DEVICE_E rootType = MMI_DEVICE_NUM;

    /*
     *first check java root device ,
     *if root device is TFCARD, notify exit,
     *java will write media tmp file to java root.
     */
    rootType = Jbed_getJavaRootType();
    if(rootType > 1) //MMIFILE_DEVICE_E.MMI_DEVICE_UDISK = 1
    {
        mExit = TRUE;
    }

    //if midlet installed into TFcard , notify exit
    if(mExit == FALSE)
    {
        runCount = Jbed_tm_queryRunningAppInfo(&raInfo);
        SCI_TRACE_LOW("MMIAPIJAVA_StopOperaSDPlug() - runCount=%d",runCount);
        if(runCount != 0 && raInfo != NULL)
        {
            SCI_TRACE_LOW("MMIAPIJAVA_StopOperaSDPlug() - start loop");
            for(i=0; i<runCount; i++)
            {
                CPL_midletInfo_getSuiteInfoItem(raInfo[i].sid, MI_FILE_ROOT, &buffer);
                SCI_TRACE_LOW("CPL_midletInfo_getSuiteInfoItem() - buffer=%s",buffer);
                if (MMIAPICOM_Wstrncmp(buffer, L"TFCard", 6) == 0)
                {
                    SCI_TRACE_LOW("CPL_midletInfo_getSuiteInfoItem() - mExit = TRUE");
                    mExit = TRUE;
                }
                SCI_FREEIF(buffer);
            }
        }
    }

    if(mExit == TRUE)
    {
        if (Jbed_getJbedState() > JBED_MAIN_DOWN)
        {
            //force exit jbed lifecycle
            Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
            Jbed_wakeupVM();
#ifdef MMI_TASK_MANAGER
            MMIAPIJAVA_StopApplet(FALSE);
#endif
        }
    }
}

static BOOLEAN g_jbed_is_reseting = FALSE;

void MMIJAVA_setJbedResetState(BOOLEAN state)
{
    g_jbed_is_reseting = state;
}

BOOLEAN MMIJAVA_getJbedResetState(void)
{
    return g_jbed_is_reseting;
}

void MMIJAVA_resetJbedFactoryCB(void)
{
    BOOLEAN flag = FALSE;
    int path_len = 0;
    int file_size = 0;
    int real_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    char * policy_buf = NULL;
    char  path_buff[MMIFMM_PATHNAME_LEN] = {0};
    wchar wpath_buff[MMIFMM_PATHNAME_LEN + 1] = {0};

    SCI_TRACE_LOW("===MMIJAVA_resetJbedFactoryCB==");
    MMIJAVA_setJbedResetState(TRUE);
     /**********************************************************************************
     * not check if the midlet(s) is(are) pre-installed because it doesnot need to,   *
     * the midlet(s) could be re-inited when triggering java operation again          *
     **********************************************************************************/

    //1:delete all shortcut icons in main menu
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIJAVA_DeleteAllSuitesRecorInMainmenu();
#endif

    //2: save policy.utf
    sprintf(path_buff,"%spolicy.utf\0",Jbed_getRootPath());
    SCI_TRACE_LOW("===policy.utf path:%s==",path_buff);
    Jbni_convertUtf8ToUcs2((char*)path_buff, CRTL_strlen(path_buff), (wchar*)wpath_buff, strlen(path_buff)<<1);
    file_handle = MMIAPIFMM_CreateFile(wpath_buff,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0);
    if(file_handle !=0)
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        policy_buf = SCI_ALLOCA(file_size + 1);
        if(policy_buf != NULL)
        {
            CRTL_memset(policy_buf,0,file_size + 1);
            if(SFS_NO_ERROR == MMIAPIFMM_ReadFile(file_handle,(void*)policy_buf,file_size,&real_size,NULL))
            {
                SCI_TRACE_LOW("===file_size(%d),real size(%d)==",file_size,real_size);
                flag = TRUE;
            }
        }

        MMIAPIFMM_CloseFile(file_handle);
    }

    //3:delete jbed path(including all files in the dir)
    CRTL_memset(path_buff,0,MMIFMM_PATHNAME_LEN);
    CRTL_memset(wpath_buff,0,2*MMIFMM_PATHNAME_LEN);
    sprintf(path_buff,"%s\0",Jbed_getRootPath());   //path is as: X:/Java/Installed/
    path_len = CRTL_strlen(path_buff);
    if(path_buff[path_len-1] == '/')
    {
        path_buff[path_len-1] = '\0';
    }
    SCI_TRACE_LOW("===jbed root path:%s==",path_buff);
    Jbni_convertUtf8ToUcs2((char*)path_buff, CRTL_strlen(path_buff), (wchar*)wpath_buff, strlen(path_buff)<<1);
    MMIAPICOM_DelDirAll(wpath_buff,MMIFMM_PATHNAME_LEN);

    //4: delete obj files and dir/no obj files in C disk
    //simply handle here,not to check whether each of D\E\F disk exists

    path_len = CRTL_wstrlen(L"X:\\Jbed");   //'X' stands for D/E/F

    CRTL_memset(wpath_buff,0,2*MMIFMM_PATHNAME_LEN);
    CRTL_wstrncpy(wpath_buff,L"D:\\Jbed",path_len);
    MMIAPICOM_DelDirAll(wpath_buff, MMIFMM_PATHNAME_LEN);

    CRTL_memset(wpath_buff,0,2*MMIFMM_PATHNAME_LEN);
    CRTL_wstrncpy(wpath_buff,L"E:\\Jbed",path_len);
    MMIAPICOM_DelDirAll(wpath_buff,MMIFMM_PATHNAME_LEN);

    CRTL_memset(wpath_buff,0,2*MMIFMM_PATHNAME_LEN);
    CRTL_wstrncpy(wpath_buff,L"F:\\Jbed",path_len);
    MMIAPICOM_DelDirAll(wpath_buff,MMIFMM_PATHNAME_LEN);

    //5: re-write policy.utf
    if(flag)
    {
        CRTL_memset(path_buff,0,MMIFMM_PATHNAME_LEN);
        CRTL_memset(wpath_buff,0,2*MMIFMM_PATHNAME_LEN);
        sprintf(path_buff,"%spolicy.utf\0",Jbed_getRootPath());
        SCI_TRACE_LOW("===re write policy.utf path:%s==",path_buff);
        Jbni_convertUtf8ToUcs2((char*)path_buff, CRTL_strlen(path_buff), (wchar*)wpath_buff, strlen(path_buff)<<1);
        file_handle = MMIAPIFMM_CreateFile(wpath_buff,SFS_MODE_WRITE | SFS_MODE_CREATE_NEW,0,0);
        if(file_handle !=0)
        {
            MMIAPIFMM_WriteFile(file_handle,policy_buf,real_size,&file_size,NULL);
            MMIAPIFMM_CloseFile(file_handle);
        }
    }

    SCI_FREEIF(policy_buf);
    MMIJAVA_regJbedFactoryCB(NULL);
    MMIJAVA_setJbedResetState(FALSE);
    return;
}

void MMIJAVA_regJbedFactoryCB(rstFactoryCb cb)
{
    resetJbedFactoryCB = cb;
}

/************************************
 * call the API to delete all jbed files
 ************************************/
PUBLIC void MMIAPIJAVA_resetJbedFactorySetting(void)
{
    int timeout_count = 0;
    MMIJAVA_regJbedFactoryCB(NULL);
    SCI_TRACE_LOW("===MMIAPIJAVA_resetJbedFactorySetting==");
    //first:if jbed is running,exit it
#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    if(Jbed_isInRunState())
    {
        MMIJAVA_Destroy_AllRunningMIDlets();

        do
        {
            timeout_count++;
            SCI_SLEEP(20);
            SCI_TRACE_LOW("===here-sleep 20ms over==");
        }while((Jbed_getJbedState() > JBED_MAIN_DOWN) && (timeout_count <200));  //4s time out

        MMIJAVA_regJbedFactoryCB(MMIJAVA_resetJbedFactoryCB);
        return;
    }
#endif

    MMIJAVA_resetJbedFactoryCB();

    return;
}

/*****************************************************************************/
//  Description : to start flash backlight
//  Global resource dependence : none
//  Author:minghui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartJbedFlashBacklight(uint32 duration)
{
    uint32 count = 0;
    uint32 tick_count = (uint32)SCI_GetTickCount();

    SCI_TRACE_LOW("mmijava_export.c: MMIAPIJAVA_StartJbedFlashBacklight Enter duration =%d", duration);

    SCI_TRACE_LOW("mmijava_export.c MMIAPIJAVA_StartJbedFlashBacklight  tick_count=%d ", tick_count);

    if (duration <= MMIJAVA_FLASH_BACKLIGHT_DELAY)
    {
        SCI_TRACE_LOW("mmijava_export.c: MMIAPIJAVA_HandleJbedFlashBacklight duration < default delay");
        return;
    }


    if(!MMIDEFAULT_IsBacklightOn())
    {
        MMIDEFAULT_TurnOnBackLight();
    }

    s_flash_backnight_info.flash_count = (uint32)(duration / MMIJAVA_FLASH_BACKLIGHT_DELAY);
    s_flash_backnight_info.is_backlighton = TRUE;

    if(NULL == s_flash_backnight_info.flash_timer_id_ptr)
    {
        s_flash_backnight_info.flash_timer_id_ptr = SCI_CreatePeriodTimer("FLASH_LCD",
                                   HandleFlashBacklightTimerOut,
                                   0,
                                   MMIJAVA_FLASH_BACKLIGHT_DELAY,
                                   SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_flash_backnight_info.flash_timer_id_ptr,
                                HandleFlashBacklightTimerOut,
                                MMIJAVA_FLASH_BACKLIGHT_DELAY);
        SCI_ActiveTimer(s_flash_backnight_info.flash_timer_id_ptr);
    }

    if(NULL == s_flash_backnight_info.flash_timer_id_ptr)
    {
        SCI_TRACE_LOW("mmijava_export.c MMIAPIJAVA_StartJbedFlashBacklight:create timer failed");
        return;
    }

    MMK_PauseTimer(MMI_LCD_LIGHT_TIMER_ID);
    MMK_PauseTimer(MMI_KEYBAD_LIGHT_TIMER_ID);

}
/*****************************************************************************/
//  Description : backlight flash timer callback function
//  Global resource dependence : none
//  Author:minghui.wang
//  Note:
/*****************************************************************************/
LOCAL void HandleFlashBacklightTimerOut( uint32 praram )
{
    SCI_TRACE_LOW("mmijava_export.c HandleFlashBacklightTimerOut: count =%d", s_flash_backnight_info.flash_count);

    SCI_DeactiveTimer(s_flash_backnight_info.flash_timer_id_ptr);

    if (0 < s_flash_backnight_info.flash_count)
    {
#ifndef WIN32
        uint32 brightness = 0;
        s_flash_backnight_info.is_backlighton = !s_flash_backnight_info.is_backlighton;
        s_flash_backnight_info.flash_count--;
        brightness = s_flash_backnight_info.is_backlighton ? 50 :0;
        LCD_SetBackLightBrightness(brightness);
        GPIO_SetKeyPadBackLight(s_flash_backnight_info.is_backlighton);
#endif

        SCI_ChangeTimer(s_flash_backnight_info.flash_timer_id_ptr,
                                    HandleFlashBacklightTimerOut,
                                    MMIJAVA_FLASH_BACKLIGHT_DELAY);
        SCI_ActiveTimer(s_flash_backnight_info.flash_timer_id_ptr);
        SCI_TRACE_LOW("mmijava_export.c HandleFlashBacklightTimerOut: count >0");

    }
    else if (0 == s_flash_backnight_info.flash_count)
    {
        EVENT_TASKTOMMI_T stop_backlight_event = {0};
        s_flash_backnight_info.flash_count--;
        stop_backlight_event.eventId = MMI_NAMS_EVENT_STOP_LCDBACKLIGHT;
        MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID, MSG_JAVA_MMI_EVENT, (DPARAM)&stop_backlight_event, sizeof(EVENT_TASKTOMMI_T));
        SCI_TRACE_LOW("mmijava_export.c HandleFlashBacklightTimerOut: count = 0");
    }
}
/*****************************************************************************/
//  Description : to stop flash backlight
//  Global resource dependence : none
//  Author:minghui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopJbedFlashBacklight(void)
{
    SCI_TRACE_LOW("mmijava_export.c MMIAPIJAVA_StopJbedFlashBacklight Enter");

    if (s_flash_backnight_info.flash_timer_id_ptr != NULL)
    {
        uint32 tick_count = (uint32)SCI_GetTickCount();
        SCI_TRACE_LOW("mmijava_export.c MMIAPIJAVA_StopJbedFlashBacklight stop tick_count=%d ", tick_count);

        if (SCI_IsTimerActive(s_flash_backnight_info.flash_timer_id_ptr))
        {
            SCI_DeactiveTimer(s_flash_backnight_info.flash_timer_id_ptr);
        }

        if(!s_flash_backnight_info.is_backlighton)
        {
#ifndef WIN32
          LCD_SetBackLightBrightness(50);
          GPIO_SetKeyPadBackLight(!s_flash_backnight_info.is_backlighton);
#endif
        }
        SCI_DeleteTimer(s_flash_backnight_info.flash_timer_id_ptr);
        s_flash_backnight_info.flash_timer_id_ptr = NULL;
        s_flash_backnight_info.flash_count = 0;
        s_flash_backnight_info.is_backlighton = FALSE;

        MMK_ResumeTimer(MMI_LCD_LIGHT_TIMER_ID);
        MMK_ResumeTimer(MMI_KEYBAD_LIGHT_TIMER_ID);
    }

}
/*****************************************************************************/
//  Description : check whether the MIDlet suite is available or not.
//  Global resource dependence : none
//  Author:kula
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_isMIDletSuiteAvailable(int32 suiteId)
{
    return Jbed_nAms_isMIDletSuiteAvailable(suiteId);
}

/*****************************************************************************/
//  Description : Get available suiteIDs.
//        If ids is NULL or idsSize is 0, only return the available ids count;
//      Otherwise, avaialbe suite Id will be stored in param ids.
//        This API can be called twice, first-time with (ids == null, idsSize == 0)
//      to get the availabe suite id count. second-time with ids buffer and the size
//      of ids buffer(idsSize) to get avaialbe suite ids.
//      @param ids, the buffer used to store available suite Id.
//      @param idsSize, the size of buffer ids;
//      @return the available suite Id count;
//  Global resource dependence : none
//  Author:kula
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIJAVA_getSDCardAvailableSuites(int32 *ids, int32 idsSize)
{
    return Jbed_nAms_getSDCardAvailableSuites(ids, idsSize);
}

/*****************************************************************************/
//  Description : Start a MIDlet suite by specifying the suite id.
//  @param suite_id, the suite id of midlet suite which you want to run.
//                   the suite id must be set a meaningful value.
//  @return TRUE if it is successful to start the suite.
//          FASLE if it is failed to start the suite.
//  Global resource dependence:
//  Author: Sean Xu
// CAUTION: 1) We create the MMIJAVA_StartSuiteEx function with conflict check
//             instead of MMIJAVA_StartSuite to be called by external modules.
//             Please do not call MMIJAVA_StartSuite anymore from outside module.
//          2) The caller(SPRD) should specify the parameter of suite id
//             with meaningful value. You've better let Myriad know if
//             you are not sure how to get correct suite id.
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_StartSuiteEx(int suite_id)
{
    SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuiteEx suite_id = %d  enter.", suite_id);
    return MMIJAVA_StartSuiteInternal(suite_id, -1);
}

#ifdef MICROSOFT_WNS_SUPPORT
/*****************************************************************************/
//  Description : launch a MIDlet suite by wns push message identifier
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @return the MIDlet suite id which is equal or greater than zero;
//         -1 means MIDlet suite not found.
//  Global resource dependence :
//  Author: Kula
// CAUTION: before calling this API, please call MMIJAVA_EnvironmentInit()
//                 to initial Java environment.
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_LaunchMIDletSuiteByWnsPushMessage(char *wnsIdentifier)
{
    int suiteId = Jbed_nAms_getSuiteIdByWnsIdentifier(wnsIdentifier);

    if (suiteId < 0)
    {
        SCI_TRACE_LOW("=== mmijava_export.c MMIJAVA_LaunchMIDletSuiteByWnsPushMessage suiteId is not found");
        return -1;
    }

    Jbed_nAms_setLaunchByWnsPushEnvironment();
    if (!MMIJAVA_StartSuiteEx(suiteId))
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_LaunchMIDletSuiteByWnsPushMessage: MMIJAVA_StartSuiteEx failed.");
        Jbed_nAms_isMIDletSuiteLaunchedByWnsPush(); //reset the flag.
        return -1;
    }

    return suiteId;
}

/*****************************************************************************/
//  Description : Get icon by WNS push message identifier which is specified in JAD
//  or MANIFEST file with property: MIDlet-WNS-AppName;
//  Usage: You probably need to call the function twice:
//      The first time, you specify the parameter of buffer as NULL, the function will
//      return the size of icon buffer which you need to malloc. After you mallocated
//      the buffer passed as the parameter and call MMIJAVA_GetIconByWnsIdentifier
//      again, the icon data will be filled in this buffer. You are also responsible
//      for freeing the buffer.
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @param buffer, the buffer is used for filling icon data.
//  @param bufLen, the length of buffer, it is of no effect when buffer is passed as NULL.
//  @return the icon buffer size; return -1 means no responding icon or getting icon failed.
//  Global resource dependence:
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetIconByWnsIdentifier(char *wnsIdentifier, uint8 *buffer, int bufLen)
{
    return Jbed_nAms_getIconByWnsIdentifier(wnsIdentifier, buffer, bufLen);
}

/*****************************************************************************/
//  Description : Get icon path by WNS push message identifier which is specified in JAD
//  or MANIFEST file with property: MIDlet-WNS-AppName;
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @param iconPath, the pass out value of icon path in unicode string format.
//  @return the icon path length caculated by unicode unit;
//          <= 0 means no responding icon in WNS MIDlet or getting icon path failed.
//  Example code:
//      int path_len = 0;
//      uint16 *icon_path = NULL;
//      path_len = MMIJAVA_GetIconPathByWnsIdentifier(identifier, &icon_path);
//      if (path_len > 0)
//      {
//          // Use icon_path value as your wish.
//      }
//      SCI_FREEIF(icon_path);  //remember to free the memory.
//  Global resource dependence:
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetIconPathByWnsIdentifier(char *identifier,  uint16 **iconPath)
{
    int pathLength = 0;
    int suiteId = -1;

    if (identifier == NULL || iconPath == NULL)
    {
        SCI_TRACE_LOW("MMIJAVA_LaunchMIDletSuiteByWnsPushMessage WARNING: invalid argument.");
        return pathLength;
    }

    suiteId = Jbed_nAms_getSuiteIdByWnsIdentifier(identifier);
    if (suiteId >= 0)
    {
        pathLength = Jbed_nAms_querySuitesInfo(suiteId, NAMS_SI_ICON, (void *)iconPath);
    }
    if (pathLength <= 0)
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_GetIconPathByWnsIdentifier, WARNING: %s.",
            (suiteId < 0) ? "couldn't find midlet" : "couldn't find icon path");
    }

    return pathLength;
}

/* refer to mmiava_export.h */
PUBLIC APP_STATE MMIJAVA_QueryWnsAppState(char *wnsIdentifier)
{
    int suiteId = Jbed_nAms_getSuiteIdByWnsIdentifier(wnsIdentifier);
    if (suiteId < 0)
    {
        return STATE_APP_NOT_FOUND;
    }
    else if (Jbed_getJbedState() == JBED_MAIN_DOWN)
    {
        return STATE_NO_APP_RUNNING;
    }
    else
    {
        if (Jbed_tm_queryRunningAppSuite(suiteId))
        {
            if (Jbed_getJbedState() == JBED_MAIN_ACTIVE_FOREGROUND)
            {
                return STATE_RUNNING_IN_FG;
            }
            else
            {
                return STATE_RUNNING_IN_BG;
            }
        }
        else
        {
            return STATE_RUNNING_OTHER_APP;
        }
    }
}

#endif

/**
 * When native application sign off MSA, call this API to notify Java application
 * that MSA is signed off by other apps.
 */
PUBLIC void MMIJAVA_MSASignOffNotify()
{
#ifdef MICROSOFT_MSA_SUPPORT
    Jbed_msa_notifySignOff();
#endif
}


/*****************************************************************************/
//  Description : Set Application running properties. It can set application
//                properties. And application can use System.getPropert to
//                get this value.
//  Global resource dependence:
//  Author:
//  Note: If the property is set before launching an application, it must be called
//        before calling MMIJAVA_StartSuite().
/*****************************************************************************/
PUBLIC void MMIJAVA_SetAppProperty(const char *key, const char *val)
{
    setAppPropertyValue(key, val);
}

