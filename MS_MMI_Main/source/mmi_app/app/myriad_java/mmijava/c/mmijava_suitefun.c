/****************************************************************************************
** File Name:      mmijava_app.c                                                        *
** Author:         murphy.xie                                                           *
** Date:           08/09/2012                                                           *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.                   *
** Description:    This file is used to describe the java application function control. *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        murphy.xie       Create
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "jbed_exported_defines.h"
#include "window_parse.h"
#include "sci_types.h"
#include "mmipub.h"
#include "java_mmi_text.h"
#include "mmijava_suitefun.h"
#include "mmi_pubmenuwin.h"
#include "cpl_midletinfo.h"
#include "opl_global.h"
#include "mmijava_amswin.h"
#include "mmiudisk_export.h"
#include "mmijava_common.h"
#include "mmijava_backgroundwin.h"
#include "mmicc_export.h"
#if defined(DYNAMIC_MAINMENU_SUPPORT)
#include "mmi_mainmenu_matrix.h"
#endif
#include "mmi_appmsg.h"

/*****************************************************************************/
//  Description :Check whether this menu_id should display on dynamic popup menu, return
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsPreinstallSuite(int suite_id)
{
    BOOLEAN is_preinstall = FALSE;
    CPL_MI_SUITE_TYPE type = DUMMYTYPE;

    CPL_midletInfo_getSuiteInfoItem(suite_id, MI_PREINSTALLED, (void *)&type);
    if(PREINSTALLED == type)
    {
        is_preinstall = TRUE;
    }

    return  is_preinstall;
}

/* Check conflict situation before running a midlet suite.  */
LOCAL BOOLEAN MMIJAVA_CheckConflictBeforeRunning(void)
{
    BOOLEAN is_install_state = MMIMAINMENU_GetState(MMIMAINMENU_INSTALL_STATE) ||
                               MMIMAINMENU_GetState(MMIMAINMENU_PRE_INSTALL_STATE) ||
                               MMIMAINMENU_GetState(MMIMAINMENU_UPDATE_STATE);

    if(MMIMAINMENU_GetState(MMIMAINMENU_UNINSTALL_STATE))
    {
        MAINMENU_BANNER_TYPE_E banner_type = MAINMENU_BANNER_TYPE_UNINSTALLING;
        MMK_SendMsg(MAIN_MAINMENU_WIN_ID, MSG_MAINMENU_SHOW_BANNER, &banner_type);
        return TRUE;
    }
    else if(is_install_state)
    {
        MAINMENU_BANNER_TYPE_E banner_type = MAINMENU_BANNER_TYPE_INSTALLING;
        MMK_SendMsg(MAIN_MAINMENU_WIN_ID, MSG_MAINMENU_SHOW_BANNER, &banner_type);
        return TRUE;
    }

    //check whether u disk is running
    if (MMIAPIUDISK_UdiskIsRun())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_CheckConflictBeforeRunning, check udisk is run ==");
        MMIPUB_OpenAlertWarningWin(STR_JAVA_UD_IN_USE);
        return TRUE;
    }

    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_IN_CALL);
        return TRUE;
    }

    return FALSE;
}

/* Description: Start a MIDlet suite with conflict checking.
 * Please refer mmijava_suitefun.h for details.
 */
PUBLIC BOOLEAN MMIJAVA_StartSuiteInternal(int suite_id, int midlet_num)
{
    BOOLEAN ret = TRUE;

    SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuiteInternal(%d, %d) enter.", suite_id, midlet_num);

    if (MMIJAVA_CheckConflictBeforeRunning())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuiteInternal conflict return directly.");
        ret = FALSE;
    }
    else
    {
        /* We already check INSTALL or UNINSTALL conflict in MMIJAVA_CheckConflictBeforeRunning,
         * if the jbed thread still exists at this time, it means that the midlet is running
         * on background or vm is starting or closing.
         */
        if (isJbedThreadExist())
        {
            SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuiteInternal Jbed thread exist.");

            if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
            {
                if (g_active_launching_app->sid == suite_id) //if the midlet is running in background, put it on foreground directly.
                {
                    MMIJAVA_CreateRunningWin();
                    Jbed_vmWantForeground(TRUE);
                }
                else
                {
                    MMIJAVA_EnterResumeWin();
                    ret = FALSE;
                }
            }
            else // vm is starting or closing.
            {
                MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
                ret = FALSE;
            }
        }
        else // Start midlet suite normally.
        {
            MMIJAVA_EnvironmentInit();
            ret = MMIJAVA_StartSuite(suite_id, midlet_num);
        }
    }

    return ret;
}

/* Description: Start a MIDlet suite without conflict checking.
 * Please refer mmijava_suitefun.h for details.
 */
PUBLIC BOOLEAN MMIJAVA_StartSuite(int suite_id, int midlet_num)
{
    BOOLEAN ret = TRUE;

    SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuite(%d, %d) enter.", suite_id, midlet_num);

    if (suite_id < 0 || !MMIAPIJAVA_isMIDletSuiteAvailable(suite_id))
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuite invalid suite id or not exist.");
        MMIPUB_OpenAlertWarningWin(STR_JAVA_OPERATE_ERROR);
        ret = FALSE;
    }
    else
    {
        int midlet_count = Jbed_nAms_getMIDletsCount(suite_id);

        if(midlet_num <= 0)
        {
            if (midlet_count > 1)
            {
                MMIJAVA_EnterMidletListWin(suite_id);
            }
            else if (midlet_count == 1)
            {
                Jbed_nAms_launch(suite_id, 1);
            }
            else
            {
                SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuite midlet_count is not right.");
                MMIPUB_OpenAlertWarningWin(STR_JAVA_OPERATE_ERROR);
                ret = FALSE;
            }
        }
        else if (midlet_num <= midlet_count)
        {
            Jbed_nAms_launch(suite_id, midlet_num);
        }
        else // midlet_num > midlet_count
        {
            SCI_TRACE_LOW("=== Myriad MMIJAVA_StartSuite error! (midlet_num = %d) > (midlet_count = %d)", midlet_num, midlet_count);
            MMIPUB_OpenAlertWarningWin(STR_JAVA_OPERATE_ERROR);
            ret = FALSE;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description :return suite_id according to selected index of listbox
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetSuiteID(uint32 index)
{
    int*   list_ids = NULL;
    int    suite_count;
    int    select_suitid;

    suite_count = Jbed_nAms_getSuitesCount(NULL, 0);
    // Check the suite_count and index, the index starts 0.
    if (suite_count <= 0 || index >= suite_count)
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_GetSuiteID failed: index = %d, suite_count = %d", index, suite_count);
        return -1;
    }

    list_ids = SCI_ALLOCA((size_t)suite_count * sizeof(int));
    if (list_ids == NULL)
    {
        return -1;
    }
    Jbed_nAms_getSuitesCount(list_ids, suite_count);

    select_suitid = list_ids[index];

    SCI_FREEIF(list_ids);

    return select_suitid;
}

/*****************************************************************************/
//  Description : handle the event of clicking on java icon
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMainMenu(void)
{
#ifdef NATIVE_AMS
    /**
     * If udisk is being used, prompt and returns
     */
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_UD_IN_USE);
        return;
    }

    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_IN_CALL);
        return;
    }

    if(!Jbed_checkRootAvailable())
    {
#ifndef MMI_GUI_STYLE_MINISCREEN
        MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_NO_STORAGE, \
                                  IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
        MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_NO_STORAGE, NULL, \
                                  IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif
        return;
    }

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    if(MMIJAVA_HandleStateConflict())
    {
        return;
    }

    /* We already check INSTALL or UNINSTALL conflict in MMIJAVA_CheckConflictBeforeRunning,
     * if the jbed thread still exists at this time, it means that the midlet is running
     * on background or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_OpenMainMenu Jbed thread exist.");

        if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
        {
            MMIJAVA_EnterResumeWin();
            return;
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
            return;
        }
    }
#endif

#if defined(SUPPORT_HEAD_FILE_PREINSTALL)
    if(Jbed_headFilePreinstall_fileRestore())
    {
        MMIJAVA_EnvironmentInit();

        #if (defined SUPPORT_TASK_MANAGER)
            MMIJAVA_OpenTaskMangerWin();
        #else
            MMIJAVA_EnterAmsWin();
        #endif
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_DELETE_APPS);
    }
#else//SUPPORT_HEAD_FILE_PREINSTALL
    #if defined JBED_PREINSTALL
        Jbed_preinstall_init();
    #else//JBED_PREINSTALL
        MMIJAVA_EnvironmentInit();
        #if defined (SUPPORT_TASK_MANAGER)
            MMIJAVA_OpenTaskMangerWin();
        #else
            MMIJAVA_EnterAmsWin();
        #endif
    #endif
#endif

#else //NATIVE_AMS
    JavaEntry(0, NULL);
#endif
}

#if defined JBED_PREINSTALL
PUBLIC void MMIJAVA_OpenMainMenuAfterPreinstall(void)
{
#ifdef NATIVE_AMS
    /**
     * If udisk is being used, prompt and returns
     */
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_UD_IN_USE);
        return;
    }

    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_IN_CALL);
        return;
    }

    if(!Jbed_checkRootAvailable())
    {
#ifndef MMI_GUI_STYLE_MINISCREEN
        MMIPUB_OpenAlertWinByTextId(NULL, NULL, STR_JAVA_NO_STORAGE, \
                                  IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#else
        MMIPUB_OpenAlertWinByTextId(NULL, STR_JAVA_NO_STORAGE, NULL, \
                                  IMAGE_PUBWIN_WARNING, NULL, NULL, MMIPUB_SOFTKEY_ONE, NULL);
#endif
        return;
    }

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    if(Jbed_isInRunState())
    {
        //MMK_CreateWin((uint32*)JBED_LAUNCH_CHECK_WIN_TAB, PNULL);
        MMIJAVA_EnterResumeWin();
        return;
    }
#endif

        MMIJAVA_EnvironmentInit();
        #if defined (SUPPORT_TASK_MANAGER)
            MMIJAVA_OpenTaskMangerWin();
        #else
            MMIJAVA_EnterAmsWin();
        #endif

#else //NATIVE_AMS
    JavaEntry(0, NULL);
#endif
}
#endif

/*****************************************************************************/
//  Description : Delete midlet suite query window message handler
//  Global resource dependence :
//  Author:Alex.Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_Delete_CallBack(
                                       MMI_WIN_ID_T win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM param
                                   )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    DIALOG_TYPE             dialog_type = DIALOG_WITH_CONFIRMATION;
    MMIPUB_DIALOG_SOFTKEY_T dialog_softkey = {0};

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            dialog_softkey.index = MIDDLE_BUTTON;
            dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
            dialog_softkey.content = STXT_SOFTKEY_UNINSTALL_EXT01;
            MMIPUB_SetDialogSoftkey(&win_id, dialog_type, &dialog_softkey);
            break;

#if 0 //UNISOC_MMI_Delete
        case MSG_APP_OK:
        case MSG_CTL_OK:
#else
        case MSG_APP_WEB:
#endif
        {
            MMK_CloseWin(win_id);
            SCI_TRACE_LOW("MMIJava_delete_callBack g_delete_suiteid=%d",g_delete_suiteid);
            Jbed_nAms_delete(g_delete_suiteid);
            break;
        }

        case MSG_KEYLONG_POWER:   //IF power off,close win
        case MSG_KEYDOWN_RED:
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            g_delete_suiteid = -1;
#ifdef DYNAMIC_MAINMENU_SUPPORT
            MMIMAINMENU_SetState(MMIMAINMENU_UNINSTALL_STATE, FALSE, PNULL);
#endif
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : Delete Midlet suite by suite ID from option menu
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteSuite(int suite_id)
{
    SCI_TRACE_LOW("MMIJava_DeleteSuite suite_id=%d",suite_id);

    if(suite_id < 0)//if g_delete_suiteid not valid, return.
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, STR_JAVA_REMOVE_FAIL, TXT_NULL, \
                                    IMAGE_PUBWIN_WARNING,PNULL, PNULL,MMIPUB_SOFTKEY_ONE,\
                                    PNULL);
        return;
    }

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    if(MMIJAVA_HandleStateConflict())
    {
        return;
    }

    if(Jbed_isMIDletInRunning())
    {
        SCI_TRACE_LOW("===can't delete when run in svm ==");
        MMIJAVA_PostMIDletRunningBanner();
        return;
    }
#endif

    g_delete_suiteid = suite_id;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMAINMENU_SetState(MMIMAINMENU_UNINSTALL_STATE, TRUE, PNULL);
#endif

    MMIJAVA_EnvironmentInit();

    //MMIDEFAULT_AllowTurnOffBackLight(FALSE);

    MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_MAIN_UNINSTALL_EXT01, PNULL, NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE, MMIJAVA_Delete_CallBack);
}


#ifdef SUPPORT_DELETE_ALL_FUNCTION
/*****************************************************************************/
//  Description : Delete all midlet suites query window message handler
//  Global resource dependence :
//  Author:Alex.Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJava_deleteAll_callBack(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                      )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            MMI_STRING_T prompt = {0};
            MMK_CloseWin(win_id);
            MMK_CloseWin(JBED_MAIN_OPTION_WIN_ID);
            MMI_GetLabelTextByLang(TXT_JAVA_DELETEING, &prompt);
            MMIJAVA_util_openWaitingWin(&prompt);
            Jbed_nAms_deleteAll();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return result;

/*****************************************************************************/
//  Description : Delete all Midlet suites from option menu
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteAll()
{
    MMI_STRING_T prompt_text = {0};
    MMI_GetLabelTextByLang(TXT_DELETEALL_QUERY, &prompt_text);
#ifndef MMI_GUI_STYLE_MINISCREEN
    MMIPUB_OpenAlertWinByTextPtr( NULL,NULL, &prompt_text,IMAGE_PUBWIN_QUERY,NULL,NULL, MMIPUB_SOFTKEY_OKEXIT, MMIJava_deleteAll_callBack);
#else
    MMIPUB_OpenAlertWinByTextPtr( NULL,&prompt_text, NULL,IMAGE_PUBWIN_QUERY,NULL,NULL, MMIPUB_SOFTKEY_OKEXIT, MMIJava_deleteAll_callBack);
#endif
}
#endif


#if defined (NATIVE_AMS) && defined (AMS_TCK_RUNNER)
/*****************************************************************************/
//  Description : Start jvm for tck test
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_StartTCK(void)
{
    Jbed_nAms_launch((int)JBEDAMS_STORAGE_NAME, 0);
    return;
}
#endif
