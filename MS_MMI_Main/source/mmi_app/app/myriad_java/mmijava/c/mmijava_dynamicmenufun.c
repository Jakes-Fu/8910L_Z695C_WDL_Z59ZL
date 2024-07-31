/****************************************************************************
** File Name:      mmijava_dynamicmenu.c
** Author:
** Date:           2012/8/15
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.
** Description:    This file is used to describe dynmic to mainmenu function
****************************************************************************/

#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "jbed_exported_defines.h"
#include "mmijava_dynamicmenufun.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmijava_common.h"
#include "mmijava_nv.h"
#include "cpl_file.h"
#include "cpl_tpal.h"
#include "cpl_midletinfo.h"
#include "cpl_getpreinstalljar.h"
#include "mmiudisk_export.h"
#include "mmijava_backgroundwin.h"
#include "mmi_nv.h"
#include "mmijava_suitefun.h"
#include "opl_global.h"
#include "mmicc_export.h"
#include "mmi_mainmenu_matrix.h"
#include "mmi_appmsg.h"

#define INVALID_SID -2
#define PREINSTALL_FLAG_SID 0xffffffff
#define MAX_MIDLET_NAME_LENGTH 128
#define JAVA_MAINMENU_ID_LENGTH 5

typedef struct
{
    int32 suite_id;
    uint32 menu_id;
    uint32 res_id;
    char midlet_name[MAX_MIDLET_NAME_LENGTH];
} JAVA_PREINSTALL_MENU_PARAMS;

LOCAL JAVA_PREINSTALL_MENU_PARAMS update_preinstall_menu_params = {0};
LOCAL MMIJAVA_INSTALLED_ITEM_T s_installed_list[MAX_INSTALLED_MIDLETS_NUM] = {0};

/*****************************************************************************/
//  Description: HandleJavaAlertNoMidletWinMsg
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaAlertNoMidletWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    int32 suite_id = -1;
    uint32 menu_id = 0;
    BOOLEAN is_get = FALSE;
    DIALOG_TYPE dialog_type = DIALOG_WITH_CONFIRMATION;
    MMIPUB_DIALOG_SOFTKEY_T dialog_softkey = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        dialog_softkey.index = MIDDLE_BUTTON;
        dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
        dialog_softkey.content = STXT_SOFTKEY_REMOVE_EXT01;
        MMIPUB_SetDialogSoftkey(&win_id, dialog_type, &dialog_softkey);
        break;

    case MSG_APP_WEB:
        is_get = MMIMAINMENU_GetCurItemInfo(&suite_id, &menu_id);
        if(is_get)
        {
            MMIMENU_DyDeleteItemByMenuId(menu_id);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Preinstall a midlet from main menu.
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
LOCAL void JavaMainMenuPreinstall(JAVA_PREINSTALL_MENU_PARAMS *preinstall_prams_ptr)
{
    char *insPath = NULL;
    JavaUnzipOperationResult unzipRes = Jbed_getPreinstallFile(preinstall_prams_ptr->res_id, (char *)preinstall_prams_ptr->midlet_name, &insPath);
    if (unzipRes == JAVA_UNZIP_OPERATION_SUCCESS)
    {
        SCI_TRACE_LOW("=== myriad Java install test : insPath = %s.", insPath);
    }
    else
    {
        SCI_TRACE_LOW("=== myriad Java install test : getPreinstallFile failed, unzipRes = %d.", unzipRes);
    }

    // Record the preinstall menu parameter and update it after installation is over.
    CRTL_memset(&update_preinstall_menu_params, 0, sizeof(JAVA_PREINSTALL_MENU_PARAMS));
    CRTL_memcpy(&update_preinstall_menu_params, preinstall_prams_ptr, sizeof(JAVA_PREINSTALL_MENU_PARAMS));

    MMIAPIJAVA_CommandInstall(insPath);

    if (insPath != NULL)
    {
        CRTL_free(insPath);
    }
}

/*****************************************************************************/
//  Description : JavaMainMenuOKHandler
//  Global resource dependence : none
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL void JavaMainMenuOKHandler(
                                     void* param1_ptr,
                                     void* param2_ptr
                                 )
{
    int32 suite_id = INVALID_SID;
    BOOLEAN *is_preinstall_ptr = PNULL;
    BOOLEAN is_preinstall = FALSE;
    JAVA_PREINSTALL_MENU_PARAMS *preinstall_prams_ptr = NULL;
    BOOLEAN is_install_state = MMIMAINMENU_GetState(MMIMAINMENU_INSTALL_STATE) ||
                               MMIMAINMENU_GetState(MMIMAINMENU_PRE_INSTALL_STATE) ||
                               MMIMAINMENU_GetState(MMIMAINMENU_UPDATE_STATE);


    SCI_TRACE_LOW("=== Myriad JavaMainMenuOKHandler enter ==");
    if(param1_ptr == PNULL || param2_ptr == PNULL)
    {
        return;
    }
    is_preinstall_ptr = (BOOLEAN*)param1_ptr;
    is_preinstall = *is_preinstall_ptr;

    if(MMIMAINMENU_GetState(MMIMAINMENU_UNINSTALL_STATE))
    {
        MAINMENU_BANNER_TYPE_E banner_type = MAINMENU_BANNER_TYPE_UNINSTALLING;
        MMK_SendMsg(MAIN_MAINMENU_WIN_ID, MSG_MAINMENU_SHOW_BANNER, &banner_type);
        return;
    }
    else if(is_install_state)
    {
        MAINMENU_BANNER_TYPE_E banner_type = MAINMENU_BANNER_TYPE_INSTALLING;
        MMK_SendMsg(MAIN_MAINMENU_WIN_ID, MSG_MAINMENU_SHOW_BANNER, &banner_type);
        return;
    }

    //check whether u disk is running
    if (MMIAPIUDISK_UdiskIsRun())
    {
        SCI_TRACE_LOW("=== MMIJAVA_OpenJAVAFromMainMenu,check udisk is run ==");
        MMIPUB_OpenAlertWarningWin(STR_JAVA_UD_IN_USE);
        return;
    }

    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(STR_JAVA_IN_CALL);
        return;
    }

    //get suite id first.
    if (is_preinstall)
    {
        preinstall_prams_ptr = (JAVA_PREINSTALL_MENU_PARAMS *)param2_ptr;
        suite_id = preinstall_prams_ptr->suite_id;
    }
    else
    {
        int32 *suite_id_ptr = (int32 *)param2_ptr;
        suite_id = *suite_id_ptr;
    }

    /* We already check INSTALL or UNINSTALL conflict before, if the jbed thread
     * still exists at this time, it means that the midlet is running on background
     * or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_OpenJAVAFromMainMenu Jbed thread exist.");

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
            }
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
        }
    }
    else
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_OpenJAVAFromMainMenu is_preinstall = %d, suite_id = %d", is_preinstall, suite_id);

        if (is_preinstall && suite_id == PREINSTALL_FLAG_SID)
        {
            JavaMainMenuPreinstall(preinstall_prams_ptr);
            return;
        }

        if (suite_id >= 0)
        {
            MMIJAVA_EnvironmentInit();
            if(is_preinstall || MMIAPIJAVA_isMIDletSuiteAvailable(suite_id))
            {
                MMIJAVA_StartSuite(suite_id, -1);
            }
            else
            {
                MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_MAIN_APP_NOT_FIND_EXT01, TXT_NULL,
                                                      IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
                                                      HandleJavaAlertNoMidletWinMsg);
            }
        }
        else
        {
            MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_MAIN_APP_NOT_FIND_EXT01, TXT_NULL,
                                                  IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER,
                                                  HandleJavaAlertNoMidletWinMsg);
        }
    }
}

/*****************************************************************************/
//  Description : MMIJAVA_DeleteJAVAFromMainMenu
//  Global resource dependence : none
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN JavaMainMenuDeleteHandler(
                                     void* param1_ptr,
                                     void* param2_ptr
                                 )
{
    /**
     * Actually JavaMainMenuDeleteHandler is never called in this project,
     * since MMIJAVA_DeleteSuite is called when midlet is deleted.
     */
    int suite_id = INVALID_SID;
    MMI_STRING_T prompt = {0};
    BOOLEAN *is_preinstall_ptr = PNULL;
    BOOLEAN is_preinstall = FALSE;

    if(param1_ptr == PNULL || param2_ptr == PNULL)
    {
        return FALSE;
    }
    is_preinstall_ptr = (BOOLEAN*)param1_ptr;
    is_preinstall = *is_preinstall_ptr;

    if (is_preinstall)
    {
        SCI_TRACE_LOW("JavaMainMenuDeleteHandler: The preinstall MIDlet suite cannot be deleted!");
        return FALSE;
    }
    else
    {
        int32 *suite_id_ptr = (int32 *)param2_ptr;
        suite_id = *suite_id_ptr;
        if (suite_id >= 0)
        {
            MMIJAVA_DeleteSuite(suite_id);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, STR_JAVA_FILE_MISSING, TXT_NULL, \
                                        IMAGE_PUBWIN_WARNING,PNULL, PNULL,MMIPUB_SOFTKEY_ONE,
                                        PNULL);
        }

        return TRUE;
    }
}

/*****************************************************************************/
//  Description : Add installed suite record into related NV list.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
LOCAL void AddInstalledSuiteRecordToNV(int suiteId)
{
    uint32    i = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    SCI_TRACE_LOW("AddInstalledSuiteRecordToNV suiteId = %d", suiteId);

    MMINV_READ(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list, return_value);
    if (return_value != MN_RETURN_SUCCESS)
    {
        SCI_TRACE_LOW("warning: AddInstalledSuiteRecordToNV MMINV_READ failed!");
        return;
    }

    for (i = 0; i < MAX_INSTALLED_MIDLETS_NUM; i++)
    {
        if (s_installed_list[i].used == 0)
        {
            s_installed_list[i].used = 1;
            s_installed_list[i].sid = suiteId;
            break;
        }
    }

    if (i >= MAX_INSTALLED_MIDLETS_NUM)
    {
        SCI_TRACE_LOW("error: AddInstalledSuiteRecordToNV installed too much midlets which has exceeded MAX_INSTALLED_MIDLETS_NUM!");
        return;
    }

    MMINV_WRITE(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list);
}

/*****************************************************************************/
//  Description : Delete installed suite record from related NV list.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
LOCAL void DelInstalledSuiteRecordInNV(int suiteId)
{
    uint32    i = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    SCI_TRACE_LOW("DelInstalledSuiteRecordInNV suiteId = %d", suiteId);

    MMINV_READ(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list, return_value);
    if (return_value != MN_RETURN_SUCCESS)
    {
        SCI_TRACE_LOW("warning: DelInstalledSuiteRecordInNV MMINV_READ failed!");
        return;
    }

    for (i = 0; i < MAX_INSTALLED_MIDLETS_NUM; i++)
    {
        if (s_installed_list[i].used == 1 && s_installed_list[i].sid == suiteId)
        {
            s_installed_list[i].used = 0;
            s_installed_list[i].sid = -1;
            break;
        }
    }

    MMINV_WRITE(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list);
}

/* Parse icon name to get menu id and midlet name. According to the rule,
 * the icon name is comprised of menu id(the first 5 bytes) and midlet name.*/
LOCAL BOOLEAN ParsePreinstallIconName(const char *icon_name, char *midlet_name, int *menu_id)
{
    int id = 0;
    BOOLEAN ret = FALSE;
    char *suffix, c = '.';
    char menu_id_str[JAVA_MAINMENU_ID_LENGTH+1] = {0};

    do
    {
        if (icon_name == NULL || CRTL_strlen(icon_name) <= JAVA_MAINMENU_ID_LENGTH)
        {
            SCI_TRACE_LOW("=== Myriad ParsePreinstallIconName error: icon name is invalid. \n");
            break;
        }

        // parse menu id.
        CRTL_strncpy(menu_id_str, icon_name, JAVA_MAINMENU_ID_LENGTH);
        id = CRTL_atoi(menu_id_str);
        if (id == 0)
        {
            SCI_TRACE_LOW("=== Myriad ParsePreinstallIconName error: wrong menu id! \n");
            break;
        }
        else if (id < 10000 || id > 99999)
        {
            SCI_TRACE_LOW("=== Myriad ParsePreinstallIconName warning: id = %d is not in specified range, right? \n", id);
        }

        // parse midlet name.
        suffix = CRTL_strrchr(icon_name, c);
        if (suffix == NULL || suffix == icon_name ||
            suffix - icon_name <= JAVA_MAINMENU_ID_LENGTH  ||
            suffix - icon_name > (MAX_MIDLET_NAME_LENGTH + JAVA_MAINMENU_ID_LENGTH))
        {
            SCI_TRACE_LOW("=== Myriad ParsePreinstallIconName error: get the icon name failed. \n");
            break;
        }
        CRTL_strncpy(midlet_name, icon_name + JAVA_MAINMENU_ID_LENGTH, suffix - icon_name - JAVA_MAINMENU_ID_LENGTH);

        *menu_id = id;
        ret = TRUE;
    }while(FALSE);

    return ret;
}

/*****************************************************************************/
//  Description : add suite to mainmenu
//  Global resource dependence : none
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AddSuiteRecordToMainmenu(
                                  int32 suite_id,
                                  GUIMENU_FILE_DEVICE_E device_type
                              )
{
    GUIMAINMENU_DYNAMIC_INFO_T dynamic_item_info = {0};
    uint16 * name_content = NULL;
    uint16 * icon_path    = NULL;
    int32_t handle;
    uint8  *buf = NULL;
    int    file_size = 0;
    int    name_size = 0;
    int    icon_size = 0;
    int    tmp_size = 0;
    int    i = 0;
    int32  suite_id_param = suite_id;
    BOOLEAN is_preinstall = FALSE;

    SCI_TRACE_LOW("MMIJAVA_AddSuiteRecordToMainmenu suite_id is %d",suite_id);
    name_size = Jbed_nAms_querySuitesInfo(suite_id, NAMS_SI_NAME, (void *)&name_content);
    if(PNULL == name_content)
    {
        return;
    }
    tmp_size = (name_size << 1) + sizeof(uint16) ;
    dynamic_item_info.dynamic_menu_info.text_len = MIN(tmp_size, GUIMENU_DYMAINMENU_STR_MAX_NUM);
    CRTL_memcpy(dynamic_item_info.dynamic_menu_info.text,name_content,
                        dynamic_item_info.dynamic_menu_info.text_len);

    for(i=0;i<GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM;i++)
    {
       dynamic_item_info.dynamic_menu_info.str_lang_info[i].language_type = MMIMENU_LANGUAGE_ENGLISH + i;
       CRTL_memcpy(dynamic_item_info.dynamic_menu_info.str_lang_info[i].text,
                   dynamic_item_info.dynamic_menu_info.text,
                   dynamic_item_info.dynamic_menu_info.text_len);
    }

    icon_size = Jbed_nAms_querySuitesInfo(suite_id, NAMS_SI_ICON, (void *)&icon_path);
    if(icon_size > 0)
    {
        //use plat file comon interface
        if (CPL_FILE_SUCCESS !=CPL_file_open(icon_path,
            icon_size,
            CPL_FILE_MODE_READ,
            JBED_APP_JVM,
            &handle,
            NULL))
        {
            SCI_FREEIF(name_content);
            SCI_FREEIF(icon_path);
            return;
        }
        file_size = CPL_file_getFileLength(icon_path, icon_size, JBED_APP_JVM, NULL);
        buf = (uint8*)SCI_ALLOCA(file_size + 1);
        CRTL_memset(buf,0,file_size + 1);
        CPL_file_read(handle,buf,file_size,NULL);

        dynamic_item_info.dynamic_menu_info.icon_ptr = buf;
        dynamic_item_info.dynamic_menu_info.icon_datasize = file_size;
        dynamic_item_info.dynamic_menu_info.has_icon = TRUE;
        CPL_file_close(handle,NULL);
    }
    else
    {
          dynamic_item_info.dynamic_menu_info.has_icon = 0;
    }
    dynamic_item_info.dynamic_menu_info.link_function_ptr = JavaMainMenuOKHandler;
    dynamic_item_info.dynamic_menu_info.delete_function_ptr = JavaMainMenuDeleteHandler;
    dynamic_item_info.dynamic_menu_info.param1ptr = (void *)(&is_preinstall);
    dynamic_item_info.dynamic_menu_info.parm1_size = sizeof(&is_preinstall);
    dynamic_item_info.dynamic_menu_info.param2ptr = (void *)(&suite_id_param);
    dynamic_item_info.dynamic_menu_info.parm2_size = sizeof(&suite_id_param);
    dynamic_item_info.suite_id = suite_id;
    dynamic_item_info.is_uninstallable = TRUE;
    MMIMENU_DyAppendItem(&dynamic_item_info, GUIMENU_DEVICE_SYSTEM/*device_type*/, TRUE);
    SCI_FREEIF(name_content);
    SCI_FREEIF(icon_path);
    SCI_FREEIF(buf);
}

/*****************************************************************************/
//  Description : Read preinstall icons information from rom zip and
//      generate preinstall MIDlets meinmenu.
//  @param res_id, the resource id of preinstall data.
//  NOTE: 1) On your side, you must compress the preinstall
//      resource(including jad, jar and icon) into standard zip format.
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_GeneratePreinstallMainmenu(MMI_DATA_ID_T res_id)
{
    BOOLEAN is_preinstall = TRUE;
    int32_t count = 0, i, j;
    JavaPreinstallIconsInfo *list = NULL;
    JAVA_PREINSTALL_MENU_PARAMS java_preinstall_prams = {0};
    GUIMAINMENU_DYNAMIC_INFO_T dynamic_item_info = {0};

    list = Jbed_getPreinstallIcons(res_id, &count);
    for (i = 0; i < count; i++)
    {
        int menu_name_size = 0;
        uint16 *menu_name_ptr = NULL;

        CRTL_memset(&java_preinstall_prams, 0, sizeof(JAVA_PREINSTALL_MENU_PARAMS));
        CRTL_memset(&dynamic_item_info, 0, sizeof(GUIMAINMENU_DYNAMIC_INFO_T));

        // parse icon name.
        if (! ParsePreinstallIconName((const char *)list[i].name,
            java_preinstall_prams.midlet_name, &(java_preinstall_prams.menu_id)))
        {
            SCI_TRACE_LOW("=== Myriad MMIJAVA_GeneratePreinstallMainmenu warning: ParsePreinstallIconName failed! \n");
            continue;
        }

        // covert midlet name from utf8 to wchar.
        menu_name_size = (CRTL_strlen(java_preinstall_prams.midlet_name)+1) * sizeof(uint16);
        menu_name_ptr = SCI_ALLOCA(menu_name_size);
        if (menu_name_ptr == NULL)
        {
            SCI_TRACE_LOW("=== Myriad MMIJAVA_GeneratePreinstallMainmenu warning: malloc failed! \n");
            continue;
        }
        CRTL_memset((char*)menu_name_ptr, 0x0, menu_name_size);
        Jbni_convertUtf8ToUcs2((char*)java_preinstall_prams.midlet_name,
            CRTL_strlen(java_preinstall_prams.midlet_name), menu_name_ptr, menu_name_size);

        // fill the name of dynamic menu.
        dynamic_item_info.dynamic_menu_info.text_len = MIN(menu_name_size, GUIMENU_DYMAINMENU_STR_MAX_NUM);
        CRTL_memcpy(dynamic_item_info.dynamic_menu_info.text,
            menu_name_ptr, dynamic_item_info.dynamic_menu_info.text_len);
        for(j = 0; j < GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM; j++)
        {
           dynamic_item_info.dynamic_menu_info.str_lang_info[j].language_type = MMIMENU_LANGUAGE_ENGLISH + j;
           CRTL_memcpy(dynamic_item_info.dynamic_menu_info.str_lang_info[j].text,
                       dynamic_item_info.dynamic_menu_info.text,
                       dynamic_item_info.dynamic_menu_info.text_len);
        }

        java_preinstall_prams.suite_id = PREINSTALL_FLAG_SID;
        java_preinstall_prams.res_id = res_id;

        dynamic_item_info.menu_id = java_preinstall_prams.menu_id;
        dynamic_item_info.suite_id = java_preinstall_prams.suite_id;
        dynamic_item_info.is_uninstallable = FALSE;

        dynamic_item_info.dynamic_menu_info.icon_ptr = list[i].buffer;
        dynamic_item_info.dynamic_menu_info.icon_datasize = list[i].bufLen;
        dynamic_item_info.dynamic_menu_info.has_icon = TRUE;

        dynamic_item_info.dynamic_menu_info.link_function_ptr = JavaMainMenuOKHandler;
        dynamic_item_info.dynamic_menu_info.delete_function_ptr = JavaMainMenuDeleteHandler;
        dynamic_item_info.dynamic_menu_info.param1ptr = (void *)(&is_preinstall);
        dynamic_item_info.dynamic_menu_info.parm1_size = sizeof(&is_preinstall);
        dynamic_item_info.dynamic_menu_info.param2ptr = (void *)(&java_preinstall_prams);
        dynamic_item_info.dynamic_menu_info.parm2_size = sizeof(JAVA_PREINSTALL_MENU_PARAMS);
        MMIMENU_DyAppendItem(&dynamic_item_info, GUIMENU_DEVICE_SYSTEM/*device_type*/, FALSE);

        SCI_FREEIF(menu_name_ptr);
    }

    Jbed_freePreinstallIcons(list, count);
}

/*****************************************************************************/
//  Description : Update preinstall suite id to the prameter data of main menu.
//  @param suite_id, the suite id.
//  Global resource dependence : none
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_UpdatePreinstallSuiteId(int32 suite_id)
{
    BOOLEAN is_preinstall = TRUE;
    update_preinstall_menu_params.suite_id = suite_id;
    MMIMENU_DySetItemSuiteId(update_preinstall_menu_params.menu_id, suite_id);
    MMIMENU_DySetItemParam(update_preinstall_menu_params.menu_id,
                           &is_preinstall,
                           sizeof(&is_preinstall),
                           &update_preinstall_menu_params,
                           sizeof(JAVA_PREINSTALL_MENU_PARAMS));
    // Record preinstall midlet suite id to NV means the preinstall midlet
    // has been installed by user.
    AddInstalledSuiteRecordToNV(suite_id);
}

/*****************************************************************************/
//  Description : delete the suite entry point from main menu according the suite ID
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteSuiteRecordInMainmenu(int32 suite_id)
{
    BOOLEAN is_get = FALSE;
    uint32 menu_id = 0;

    SCI_TRACE_LOW("MMIJAVA_DeleteSuiteRecordInMainmenu suite_id = %d",suite_id);
    if(suite_id < 0 )
    {
        return;
    }

    is_get = MMIMAINMENU_GetItemInfoBySuiteId(suite_id, &menu_id, PNULL);

    if(is_get)
    {
        MMIMENU_DyDeleteItemByMenuId(menu_id);
    }
}


/*****************************************************************************/
//  Description : delete all the suite entry points from main menu
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_DeleteAllSuitesRecorInMainmenu(void)
{
    int i =0;
    int menuCount = 0;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    BOOLEAN deleteRet = FALSE;
    //delete dymenu of java
    menuCount = MMIMENU_DyGetItemCount();
    SCI_TRACE_LOW("MMIJAVA_DeleteAllSuitesRecorInMainmenu - before delete MMIMENU_DyGetItemCount() = %d",menuCount);
    deleteRet = MMIMENU_DyDeleteItemByFunc(JavaMainMenuOKHandler);
    SCI_TRACE_LOW("MMIJAVA_DeleteAllSuitesRecorInMainmenu - MMIMENU_DyDeleteItemByFunc() = %d",deleteRet);
    menuCount = MMIMENU_DyGetItemCount();
    SCI_TRACE_LOW("MMIJAVA_DeleteAllSuitesRecorInMainmenu - after delete MMIMENU_DyGetItemCount() = %d",menuCount);
    //retore NV
    MMINV_READ(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list, return_value);
    for (i = 0; i < MAX_INSTALLED_MIDLETS_NUM; i++)
    {
        s_installed_list[i].used = 0;
        s_installed_list[i].sid = INVALID_SID;
    }
    MMINV_WRITE(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list);
}

/*****************************************************************************/
//  Description : To check if given suite id is in record.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInstalledSuiteInRecord(int suiteId)
{
    uint32    i = 0;
    BOOLEAN   ret = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    SCI_TRACE_LOW("MMIJAVA_IsInstalledSuiteInRecord suiteId = %d", suiteId);

    MMINV_READ(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list, return_value);
    if (return_value != MN_RETURN_SUCCESS)
    {
        SCI_TRACE_LOW("warning: MMIJAVA_IsInstalledSuiteInRecord MMINV_READ failed!");
        return ret;
    }

    for (i = 0; i < MAX_INSTALLED_MIDLETS_NUM; i++)
    {
        if (s_installed_list[i].used == 1 && s_installed_list[i].sid == suiteId)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : Init installed suite NV
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_InstalledSuiteNV_Init(void)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(s_installed_list, 0, MAX_INSTALLED_MIDLETS_NUM*sizeof(MMIJAVA_INSTALLED_ITEM_T));

        MMINV_WRITE(JAVANV_INSTALLED_SUITE_INFO, &s_installed_list);
    }
}

PUBLIC void MMIJAVA_AddPreinstallToMainmenu(int32 num_mid, MMIFILE_DEVICE_E root)
{
    int i =0;
    CPL_midletInfo_initialize();
    MMIJAVA_DeleteAllSuitesRecorInMainmenu();
#if defined(JBED_PREINSTALL)
    //if defined JBED_PREINSTALL selector.utf start from 1
    for(i=1; i<num_mid + 1; i++)
#else//JBED_PREINSTALL
    //if undefined JBED_PREINSTALL selector.utf start from 0
    for(i=0; i<num_mid; i++)
#endif//JBED_PREINSTALL
    {
        MMIJAVA_AddSuiteRecordToMainmenu(i,root);
    }
}

/*****************************************************************************/
//  Description : Add SD Card Suites To Mainmenu
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AddSDCardSuitesToMainmenu(void)
{
    int32           i  = 0;
    int32           item_count = 0;
    int             *list_ids = NULL;

    MMIJAVA_EnvironmentInit();

    item_count = MMIAPIJAVA_getSDCardAvailableSuites(NULL, 0);
    SCI_TRACE_LOW("MMIJAVA_AddSDCardSuitesToMainmenu item_count = %d", item_count);
    if(item_count <= 0)
    {
        return;
    }

    list_ids = SCI_ALLOCA((size_t)item_count * sizeof(int));
    if (list_ids == NULL)
    {
        SCI_TRACE_LOW("MMIJAVA_AddSDCardSuitesToMainmenu - ERROR: alloc memory failure");
        return;
    }

    MMIAPIJAVA_getSDCardAvailableSuites(list_ids, item_count);
    for(i = 0; i < item_count; i++)
    {
        SCI_TRACE_LOW("MMIJAVA_AddSDCardSuitesToMainmenu index = %d, suite_id = %d", i, list_ids[i]);
        if(MMIAPIJAVA_isMIDletSuiteAvailable(list_ids[i]))
        {
            MMIJAVA_AddSuiteRecordToMainmenu(list_ids[i], GUIMENU_DEVICE_UDISK);
        }
    }

    SCI_FREEIF(list_ids);
}

#endif
