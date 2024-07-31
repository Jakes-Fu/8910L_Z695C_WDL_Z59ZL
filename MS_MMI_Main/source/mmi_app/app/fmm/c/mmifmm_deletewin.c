/*****************************************************************************
** File Name:      mmifmm_deletewin.c                                        *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMM_DELETEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_deletewin.h"
#include "mmifmm_mainwin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmi_filetask.h"
#include "mmifmm_comwin.h"
#include "mmipub.h"
#include "mmifmm_comfunc.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmi_appmsg.h"
#include "window_parse.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#endif
#include "mmicom_banner.h"
#ifdef RECORD_SUPPORT
#include "mmirecord_id.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_FMM_DELETE_ALLFILES_FLAG    (MMIFMM_FILE_MAX_NUM + 1)

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMIFMM_DATA_INFO_T* list_data_ptr;
    MMIFMM_PATH_INFO_T* current_path_ptr;
    uint16              mark_num;
    wchar               cur_pathname [MMIFMM_PATHNAME_LEN +1];
} MMIFMM_DELQUERY_DATA_T;


typedef struct
{
    MMIFMM_DATA_INFO_T* list_data_ptr;
    MMIFMM_PATH_INFO_T* current_path_ptr;
    MMIFMM_COPY_DEL_PATH_DATA_INFO_T *s_copy_del_path_data_ptr;
    APP_DELETE_HANDLE_T   *s_delete_handle_ptr;
    uint32                  s_resoved_num;
    BOOLEAN                 s_need_update;
    BOOLEAN                 s_user_stop_del;
    BOOLEAN                 s_user_stop_copy;
    uint32                  s_copydel_opt_state;
} MMIFMM_DELWIN_DATA_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 timer_id = 0;
uint32 deleted_files_num = 0;
uint32 actual_deleted_files_num = 0;
uint32 total_files_num = 0;
uint16 record_current_delete_num = 0;
uint16 del_current_item_index = 0;
BOOLEAN is_multi_deleted = FALSE;
BOOLEAN is_del_one_file = FALSE;
extern BOOLEAN is_quick_delete;
extern uint32 multi_select_mode;
extern uint32 file_mark_num;
extern uint32 folder_mark_num;
extern uint32 actual_file_mark_num;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle fmm file del window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelFileFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);

/*****************************************************************************/
//  Description : wait deleting window message
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitDeleteWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);
/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeleteDrmFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    uint16 mark_num,
    wchar *cur_pathname
);
#endif
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_DELQUERY_DATA_T *query_data
);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryCloseCtrlFunc (MMI_WIN_ID_T win_id);


/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       );

/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id);

// the del file window for FMM
WINDOW_TABLE (MMIFMM_DEL_SELECTFILE_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleDelFileFolderWinMsg),
    WIN_ID (MMIFMM_DEL_SELECT_FILE_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    WIN_MOVE_STYLE (MOVE_FORBIDDEN),
    WIN_SOFTKEY (TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

/*****************************************************************************/
//  Description : Combine MMI_TEXT_ID_T and Number to Str
//  Global resource dependence :
//  Author: WEN.DONG
//  Note:
/*****************************************************************************/
void TextAndNumToStr(MMI_TEXT_ID_T text, int16 num, MMI_STRING_T *banner_str);


/*****************************************************************************/
//  Description : handle fmm file del window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelFileFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint32                  full_path_len = 0;
    uint16                  i = 0;
    uint16                  file_ret = SFS_ERROR_NONE;
    uint16                total_num = 0;
    uint16              deleted_num = 0;
    uint16             selected_num = 0;
    MMI_WIN_ID_T        wait_win_id = MMIFMM_FILE_WAITING_WIN_ID;
    MMIFILE_DEVICE_E        filedev = MMI_DEVICE_UDISK;
    MMIFMM_DELWIN_DATA_T    *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM: HandleDelFileFolderWinMsg, win_id =%d, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8260_112_2_18_2_20_52_475, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == DelWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);
        win_data_p->s_resoved_num = 0;
        win_data_p->s_need_update = FALSE;
        APPFILE_CreateTask();
        /* the reason of that MMK_SendMsg instead of MMK_PostMsg is avoid of screen pattern glint.
        certainly, the mmk_postmsg is OK, too. */
        MMK_SendMsg (win_id, MSG_FMM_OPEN_WIN_CHOOSE_PATH, PNULL);
        break;

    case MSG_FMM_OPEN_WIN_CHOOSE_PATH:
        //MMIFMM_OpenComWaitWin (HandleWaitDeleteWinMsg);
        if (win_data_p->list_data_ptr->mark_num > 0)
        {
            total_num = (file_mark_num > 0 && 0 == folder_mark_num) ? file_mark_num : 100;
            total_files_num = total_num;
            if (file_mark_num > 0 && 0 == folder_mark_num)
            {
                is_multi_deleted = TRUE;
            }
        }
        else if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID))
        {
            total_files_num = GUILIST_GetTotalItemNum(MMIFMM_RECORD_LIST_CTRL_ID);
        }
        else
        {
            //total_num = (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num);
            total_num = 100;
        }
        MMIPUB_OpenProgressWinByTextId(STR_GAL_WAIT_REMOVING_FOLDER, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
        MMIPUB_SetProgressTotalSize(&wait_win_id, total_num);
        if (!is_multi_deleted)
        {
            MMI_STRING_T text_str_ptr = {0};
            text_str_ptr.wstr_ptr = L" ";
            text_str_ptr.wstr_len = 1;
            MMIPUB_SetProgressContentByTextPtr(&wait_win_id,&text_str_ptr,TRUE);
        }
        timer_id = MMK_CreateWinTimer(win_id, 200, TRUE);

        if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID) && !is_quick_delete)
        {
            MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
        }
        else if (win_data_p->s_copy_del_path_data_ptr->mark_flag > 0)
        {
            //get a mask file or folder to delete
            if (MMI_FMM_DELETE_ALLFILES_FLAG == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                //delete current file or folder
                MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
            }
            else
            {
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
            }
        }
        else
        {
            //delete current file or folder
            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_SELECT_MARK_FILE:

        //get a mask file or folder to delete
        if (win_data_p->s_user_stop_del)
        {
            MMIPUB_CloseProgressWin(&wait_win_id);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (win_id);
            break;
        }

        if (!MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID))
        {
            for (i = win_data_p->s_copy_del_path_data_ptr->cur_mark_num; i < (uint16) (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num); i++)
            {
                if (MMIFMM_FILE_STATE_UNCHECKED != win_data_p->list_data_ptr->data[i]->state)
                {
                    break;
                }
            }
        }

        if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID))
        {
            if(!is_del_one_file)
            {
                i = record_current_delete_num;
            }
            else
            {
                i = del_current_item_index;
            }
        }

        if (i >= (uint16) (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num))
        {
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_CloseProgressWin(&wait_win_id);

            if (win_data_p->s_resoved_num > 0)
            {
                MMIPUB_OpenAlertWarningWin (TXT_FMM_RESERVE_FOLDER);
            }

            //fixed by liyan.zhu for bugzilla 2760
            MMK_CloseWin (win_id);
        }
        else
        {
            win_data_p->s_copy_del_path_data_ptr->cur_mark_num = (uint16) (i + 1);
            SCI_MEMSET (win_data_p->s_copy_del_path_data_ptr->cur_pathname, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));
            SCI_MEMSET (win_data_p->s_copy_del_path_data_ptr->copyfile_name, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));

            if (win_data_p->list_data_ptr->path_is_valid > 0)
            {
                MMIAPICOM_Wstrncpy (win_data_p->s_copy_del_path_data_ptr->cur_pathname, win_data_p->list_data_ptr->data[i]->filename_ptr,
                                    win_data_p->list_data_ptr->data[i]->filename_len);
            }
            else
            {
                MMIFMM_CombineToFileName (win_data_p->current_path_ptr->pathname, win_data_p->current_path_ptr->pathname_len,
                                          win_data_p->list_data_ptr->data[i]->filename_ptr, win_data_p->list_data_ptr->data[i]->filename_len,
                                          win_data_p->s_copy_del_path_data_ptr->cur_pathname, MMIFMM_PATHNAME_LEN);

            }

            if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID))
            {
                if(!is_del_one_file)
                {
                    record_current_delete_num++;
                }
                else
                {
                    del_current_item_index = 0;
                }
            }

            //send begin delete msg
            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_OPERATE_FILE_BEGIN:
        //begin to delete
        full_path_len = MMIAPICOM_Wstrlen (win_data_p->s_copy_del_path_data_ptr->cur_pathname);
        win_data_p->s_delete_handle_ptr = MMIFILE_DeleteOpen (win_data_p->s_copy_del_path_data_ptr->cur_pathname, full_path_len);

        if (PNULL == win_data_p->s_delete_handle_ptr)
        {
            //move fail
            SCI_TRACE_LOW ("[mmifmm]:deletefilefolder win_data_p->s_delete_handle_ptr == PNULL");
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_CloseProgressWin(&wait_win_id);
            MMIPUB_OpenAlertFailWin (TXT_ERROR);
            MMK_CloseWin (win_id);
        }
/*
        if (win_data_p->list_data_ptr->mark_num > 0)
        {
            selected_num = GUILIST_GetSelectedItemNum(MMIFMM_UDISK_LISTBOX_CTRL_ID);
            deleted_num = ((total_num > selected_num) ? (total_num - selected_num) : 0);
        }
        else
        {
            selected_num = (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num);
            deleted_num = total_num - selected_num;
        }
        MMIPUB_UpdateProgressBarEx(&wait_win_id,deleted_num,TRUE);
*/
        break;
    case MSG_FMM_DELETE_END_CNF:
        //delete one circle ok
        win_data_p->s_need_update = TRUE;//cr237690
        win_data_p->s_delete_handle_ptr = PNULL;
        file_ret  = * ( (uint16 *) param);

        if (SFS_ERROR_NONE == file_ret || SFS_ERROR_NOT_EXIST == file_ret)  //MS00194050
        {
#ifdef  VIDEOTHUMBNAIL_SUPPORT//cr130686
			wchar           dir_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
			uint16          dir_name_len = 0;
			if(MMIAPIFMM_SplitFullPath(win_data_p->s_copy_del_path_data_ptr->cur_pathname, 
				full_path_len,
				PNULL, PNULL,
				dir_name, &dir_name_len,
				NULL, NULL))
			{
				//if (MMIFMM_IsDefaultVideoFolder(dir_name,dir_name_len))
				{
					MMIFMM_VideoThumbnailDeleteRelatedFile(win_data_p->s_copy_del_path_data_ptr->cur_pathname);
				}
			}
#endif

            if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID) && !is_quick_delete)
            {
                if(!is_del_one_file)
                {
                     MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
                }
                else
                {
                     MMK_CloseWin(win_id);
                }
            }
            //win_data_p->s_need_update = TRUE;
            else if (0 == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                //not file or folder to delete

                if (!win_data_p->s_user_stop_del)
                {
                    if (win_data_p->s_resoved_num > 0)
                    {
                        MMIPUB_OpenAlertWarningWin (TXT_FMM_RESERVE_FOLDER);
                    }

//arvin zhang fixed for cr221394---start---
#ifdef MMI_FMM_MINI_SUPPORT
                    MMIPUB_OpenAlertSuccessWin (TXT_DELETED);
#endif
                }

#ifdef MMI_FMM_MINI_SUPPORT
                else//user stop deleting file operation
                {
                    MMIPUB_OpenAlertSuccessWin (TXT_USER_CANCEL);
                }

#endif
//arvin zhang fixed for cr221394---end---
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                MMIPUB_CloseProgressWin(&wait_win_id);
                MMK_CloseWin (win_id);
            }
            else if (MMI_FMM_DELETE_ALLFILES_FLAG == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                MMIAPIFMM_CreateDirectory (win_data_p->s_copy_del_path_data_ptr->cur_pathname);
                //fixed by liyan.zhu for bugzilla 2760
#if 0
                FmmAlertCopyDelResult (MMIFMM_OPT_DEL);
#endif
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                MMIPUB_CloseProgressWin(&wait_win_id);
                MMK_CloseWin (win_id);
            }
            else
            {
                //delete next masked file or folder
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
            }
        }
        else
        {
            //delete error, stop delete
            //SCI_TRACE_LOW:"FMM_DEL: MSG_FMM_DELETE_END_CNF file_ret = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8379_112_2_18_2_20_52_476, (uint8*) "d", file_ret);

            if (SFS_ERROR_ABORT != file_ret)
            {
                filedev = MMIAPIFMM_GetDeviceTypeByPath (win_data_p->current_path_ptr->pathname, MMIFILE_DEVICE_LEN);

                MMIAPIFMM_PromptFileErrorType (PNULL, (uint32) file_ret, filedev);
                is_quick_delete = TRUE;//just a flag: don't prompt banner text
                //MMIPUB_OpenAlertFailWin(TXT_ERROR);
                /*if (MMK_IsOpenWin (MMIFMM_RECORD_LIST_WIN_ID))
                {
                    is_quick_delete = TRUE;//just a flag: don't prompt banner text
                    //MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
                    break;
                }*/
            }

            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_CloseProgressWin(&wait_win_id);
            MMK_CloseWin (win_id);
        }

        break;
    case MSG_CLOSE_WINDOW:
        FmmCopyDelExit (win_id);
        DelWinCloseCtrlFunc (win_id);
        if(MMK_IsOpenWin(MMIFMM_RECORD_LIST_WIN_ID))
        {
            //MMK_CloseWin(MMIFMM_RECORD_LIST_WIN_ID);
            record_current_delete_num = 0;
            MMK_PostMsg (MMIFMM_RECORD_LIST_WIN_ID, MSG_FMM_DELETE_END_CNF, PNULL, 0);
        }

        MMIPUB_CloseProgressWin(&wait_win_id);
        MMK_PostMsg (MMIFMM_UDISK_CHILD_WIN_ID, MSG_FMM_DELETE_END_CNF, PNULL, 0);
        {
            MMICOM_BANNER_INFO_T banner_info = {0};
            MMI_STRING_T          banner_str = {0};
            wchar                   text[64] = {0};
            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.start_point.x = 0;
            banner_info.start_point.y = 0;
#ifdef RECORD_SUPPORT
            if(MMK_IsOpenWin(MMIFMM_RECORD_LIST_WIN_ID))
            {
                if (!is_quick_delete)
                {
                    banner_info.text1_id = STR_RECORDINGS_DELETED_EXT01;
                    MMIAPICOM_OpenBannerWin(MMIRECORD_MAINPLAY_WIN_ID, banner_info);
                }
            }
            else
#endif
            {
                if (1 == file_mark_num)
                {
                    banner_info.text1_id = STR_NOTE_FILE_DELETED_EXT01;
                }
                
                if (1 == folder_mark_num)
                {
                    banner_info.text1_id = STR_FOLDER_DELETED;
                }
                
                if (1 == folder_mark_num && 1 == file_mark_num)
                {
                    banner_info.text1_id = STR_FILE_FOLDER_DELETED;
                }
                
                if(TRUE == win_data_p->s_user_stop_del)
                {
                    banner_info.banner_type = MMI_BANNER_STR_TEXT;
                    banner_str.wstr_ptr =text;
                    //TextAndNumToStr(STR_NOTE_FILES_DELETED_EXT01,100,&banner_str);
                    MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_NOTE_FILES_DELETED_EXT01, L"%N",deleted_files_num,64,&banner_str);
                    banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                    banner_info.text1_str.wstr_len = banner_str.wstr_len;
                }
                else
                {
                    
                    if (0 == folder_mark_num && 1 < file_mark_num)
                    {
                        banner_info.banner_type = MMI_BANNER_STR_TEXT;
                        banner_str.wstr_ptr =text;
                        //TextAndNumToStr(STR_NOTE_FILES_DELETED_EXT01,100,&banner_str);
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_NOTE_FILES_DELETED_EXT01, L"%N",file_mark_num,64,&banner_str);
                        banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                        banner_info.text1_str.wstr_len = banner_str.wstr_len;
                    }
                    
                    if (1 < folder_mark_num && 0 == file_mark_num)
                    {
                        banner_info.banner_type = MMI_BANNER_STR_TEXT;
                        banner_str.wstr_ptr =text;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_FOLDERS_DELETED, L"%N",folder_mark_num,64,&banner_str);
                        banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                        banner_info.text1_str.wstr_len = banner_str.wstr_len;
                    }
                    
                    if (1 == folder_mark_num && 1 < file_mark_num)
                    {
                        banner_info.banner_type = MMI_BANNER_STR_TEXT;
                        banner_str.wstr_ptr =text;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_FILES_FOLDER_DELETED, L"%N",file_mark_num,64,&banner_str);
                        banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                        banner_info.text1_str.wstr_len = banner_str.wstr_len;
                    }
                    
                    if (1 < folder_mark_num && 1 == file_mark_num)
                    {
                        banner_info.banner_type = MMI_BANNER_STR_TEXT;
                        banner_str.wstr_ptr =text;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_FILE_FOLDERS_DELETED, L"%N",folder_mark_num,64,&banner_str);
                        banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                        banner_info.text1_str.wstr_len = banner_str.wstr_len;
                    }
                    
                    if (1 < folder_mark_num && 1 < file_mark_num)
                    {
                        MMI_STRING_T    temp_str = {0};
                        wchar   temp_text[64] = {0};
                        char num_str[10+1] = {0};
                        wchar num_wstr[10+1] = {0};
                        banner_info.banner_type = MMI_BANNER_STR_TEXT;
                        banner_str.wstr_ptr =temp_text;
                        temp_str.wstr_ptr =text;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_FILES_FOLDERS_DELETED, L"%0N",file_mark_num,64,&temp_str);
                        
                        sprintf((char *)num_str,"%d",folder_mark_num);
                        MMIAPICOM_StrToWstr(num_str, num_wstr);
                        MMIAPICOM_CustomGenerateReplaceStr(&temp_str, L"%1N", num_wstr, 128,&banner_str);
                        banner_info.text1_str.wstr_ptr = banner_str.wstr_ptr;
                        banner_info.text1_str.wstr_len = banner_str.wstr_len;
                    }
                }
                
                
                
                if (!is_quick_delete)
                {
                    MMIAPICOM_OpenBannerWin(MMIFMM_MAIN_WIN_ID, banner_info);
                }
            }
       }

       if(timer_id != 0)
       {
           MMK_StopTimer(timer_id);
           timer_id = 0;
       }

        deleted_files_num = 0;
        actual_deleted_files_num = 0;
        total_files_num = 0;
        file_mark_num = 0;
        folder_mark_num = 0;
        is_multi_deleted = FALSE;
        is_quick_delete = FALSE;
        is_del_one_file = FALSE;
       //End
        break;

    case MSG_TIMER:
        if (timer_id == *((uint8 *) param))
        {
            if (is_multi_deleted  && deleted_files_num < total_files_num)
            {
                MMIPUB_UpdateProgressBarEx(&wait_win_id,deleted_files_num,TRUE);
            }
            else if (!is_multi_deleted && deleted_files_num < 100)
            {
                MMIPUB_UpdateProgressBarEx(&wait_win_id,deleted_files_num,TRUE);
            }
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : wait deleting window message
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitDeleteWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_DELWIN_DATA_T       *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8195_112_2_18_2_20_52_473, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_p->s_user_stop_del = FALSE;
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        // 不处理
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        if (!win_data_p->s_user_stop_del)
        {
            win_data_p->s_user_stop_del = TRUE;
            MMIFILE_AbortDelete (win_data_p->s_delete_handle_ptr);
        }

        break;
    case MSG_APP_RED:
        if (!win_data_p->s_user_stop_del)//CR200474
        {
            win_data_p->s_user_stop_del = TRUE;
            MMIFILE_AbortDelete (win_data_p->s_delete_handle_ptr);
        }
        //result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    case MSG_CLOSE_WINDOW:
        win_data_p->s_user_stop_del = TRUE;

        //SCI_TRACE_LOW:"[FMM]HandleWaitDeleteWinMsg abort!! Delete"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8230_112_2_18_2_20_52_474, (uint8*) "");
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : to handle the message of progress window
//  Global resource dependence :
//  Author: wen.dong
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E               result  = MMI_RESULT_TRUE;
    MMIFMM_DELWIN_DATA_T   *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_p->s_user_stop_del = FALSE;
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    case MSG_APP_WEB:
    case MSG_APP_OK:
        break;
    case MSG_APP_CANCEL:
        if (!win_data_p->s_user_stop_del)
        {
            win_data_p->s_user_stop_del = TRUE;
            MMIFILE_AbortDelete (win_data_p->s_delete_handle_ptr);
        }
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    return (result);
}


/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        break;
    //case MSG_APP_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMIFMM_DELQUERY_DATA_T *win_data_p = (MMIFMM_DELQUERY_DATA_T*) MMK_GetWinUserData (win_id);

            if (PNULL != win_data_p)
            {
                MMIFMM_EnterDeleteWin (win_data_p->list_data_ptr, win_data_p->current_path_ptr, win_data_p->cur_pathname);
            }

            multi_select_mode = 0;
            MMK_CloseWin (win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //enter fmm explorer
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        DelQueryCloseCtrlFunc (win_id);
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_DELQUERY_DATA_T *query_data
)
{
    MMIFMM_DELQUERY_DATA_T *del_query_ptr = PNULL;

    if (PNULL == query_data)
    {
        return MMI_RESULT_FALSE;
    }

    del_query_ptr = (MMIFMM_DELQUERY_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_DELQUERY_DATA_T));
    del_query_ptr->current_path_ptr = query_data->current_path_ptr;
    del_query_ptr->list_data_ptr = query_data->list_data_ptr;
    del_query_ptr->mark_num = query_data->mark_num;
    MMIAPICOM_Wstrncpy (del_query_ptr->cur_pathname, query_data->cur_pathname, MMIAPICOM_Wstrlen (query_data->cur_pathname));

    MMK_SetWinUserData (win_id, (void *) del_query_ptr);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELQUERY_DATA_T *win_data_p = (MMIFMM_DELQUERY_DATA_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : del file menu function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_DeleteSelectFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMI_CTRL_ID_T       ctrl_id,
    MMIFMM_PATH_INFO_T  *current_path_ptr
)
{
    uint16  current_index = 0;
    uint32  file_len = 0;
    MMI_WIN_ID_T            query_win_id = MMIFMM_QUERY_WIN_ID;
    MMI_TEXT_ID_T  prompt_text = PNULL;
    uint16  mark_flag = 0;
    wchar cur_pathname [MMIFMM_PATHNAME_LEN +1] = {0};

    //SCI_TRACE_LOW:"MMIFMM: DeleteSelectFile"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7786_112_2_18_2_20_51_467, (uint8*) "");

    if (0 == list_data_ptr->mark_num)
    {
        current_index = GUILIST_GetCurItemIndex (ctrl_id);
        file_len = MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, current_index, cur_pathname, MMIFMM_PATHNAME_LEN);
    }

    //prompt_text = (list_data_ptr->checked != MMIFMM_FILE_STATE_UNCHECKED) ? STR_QUERY_DELETE_FILES_EXT01 : STR_QUERY_DELETE_FOLDER_EXT01;
    if (file_mark_num == 1 && 0 == folder_mark_num)
    {
        prompt_text = STR_QUERY_DELETE_FILES_EXT02;
    }
    else if(file_mark_num > 1 && 0 == folder_mark_num)
    {
        prompt_text = STR_QUERY_DELETE_FILES_EXT01;
    }
    else if (folder_mark_num > 0 && 0 == file_mark_num)
    {
        prompt_text = STR_QUERY_DELETE_FOLDERS;
    }
    else if (file_mark_num > 0 && 0 < folder_mark_num)
    {
        prompt_text = STR_QUERY_DELETE_FILES_FOLDERS;
    }

	//CR161515
	// 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
	if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
	{
		mark_flag = list_data_ptr->mark_num;
	}
	else
	{
		if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
		{
			mark_flag = MMI_FMM_DELETE_ALLFILES_FLAG;
		}
		else
		{
			mark_flag = list_data_ptr->mark_num;
		}
	}

#ifdef DRM_SUPPORT

    if (IsDeleteDrmFile (list_data_ptr, current_path_ptr, mark_flag, cur_pathname))
    {
        //if (1 < s_copy_del_path_data_ptr->mark_flag)
           if (1 < mark_flag)
           {
            //如果是多个文件
            //for bug 1004379, use MMIPUB_OpenQueryWinByTextId func ,the query win just do not show. so use MMIPUB_OpenConfirmationDialogByTextId instread
            //MMIPUB_OpenQueryWinByTextId (TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
            MMIPUB_OpenConfirmationDialogByTextId(
                PNULL,
                TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,
                PNULL,
                IMAGE_PUBWIN_QUERY,
                &query_win_id,
                PNULL,
                MMIPUB_SOFTKEY_OKCANCEL,
                HandleDelQueryWin
                );
           }
        else
        {
            //如果就是单个文件
            //for bug 1004379, use MMIPUB_OpenQueryWinByTextId func ,the query win just do not show. so use MMIPUB_OpenConfirmationDialogByTextId instread
            //MMIPUB_OpenQueryWinByTextId (TXT_DRM_RIGHTS_VALID_WHETHER_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
            MMIPUB_OpenConfirmationDialogByTextId(
                PNULL,
                TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,
                PNULL,
                IMAGE_PUBWIN_QUERY,
                &query_win_id,
                PNULL,
                MMIPUB_SOFTKEY_OKCANCEL,
                HandleDelQueryWin
                );
        }
    }
    else
#endif
    {
        if (!is_quick_delete)
        {
            //MMIPUB_OpenQueryWinByTextId (TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
            MMIPUB_OpenConfirmationDialogByTextId(
                PNULL,
                prompt_text,
                PNULL,
                PNULL,
                &query_win_id,
                PNULL,
                MMIPUB_SOFTKEY_OKCANCEL,
                HandleDelQueryWin
                );
        }
    }

    if (!is_quick_delete)
    {
        MMIFMM_DELQUERY_DATA_T del_query_data = {0};
        del_query_data.current_path_ptr = current_path_ptr;
        del_query_data.list_data_ptr = list_data_ptr;
        del_query_data.mark_num = mark_flag;
        MMIAPICOM_Wstrncpy (del_query_data.cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
        DelQueryOpenCtrlFunc (query_win_id, &del_query_data);
    }
    else
    {
        MMIFMM_EnterDeleteWin (list_data_ptr, current_path_ptr, cur_pathname);
    }
}
/*****************************************************************************/
//  Description : del all files menu function
//  Global resource dependence :
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_DeleteAllFiles (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMI_CTRL_ID_T       ctrl_id,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    BOOLEAN			isDeleteOne
)
{
    uint16  current_index = 0;
    uint32  file_len = 0;
    MMI_WIN_ID_T            query_win_id = MMIFMM_QUERY_WIN_ID;
    MMI_TEXT_ID_T  prompt_text = PNULL;
    uint16  mark_flag = 0;
    wchar cur_pathname [MMIFMM_PATHNAME_LEN +1] = {0};

    //SCI_TRACE_LOW:"MMIFMM: DeleteSelectFile"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7786_112_2_18_2_20_51_467, (uint8*) "");

    if (0 == list_data_ptr->mark_num)
    {
        if(isDeleteOne)
        {
        	current_index = GUILIST_GetCurItemIndex (ctrl_id);
             del_current_item_index = current_index;
        }
        else
        {
        	current_index = 0;
        }
        file_len = MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, current_index, cur_pathname, MMIFMM_PATHNAME_LEN);
    }

    if(isDeleteOne)
    {
        prompt_text = STR_QUERY_DELETE_RECORDING_EXT01;//STR_GAL_LIST_DELETE_FILE_EXT01;
        mark_flag = list_data_ptr->mark_num;
    }
    else
    {
	//change by your define
        prompt_text = STR_QUERY_DELETE_ALL_REC_EXT01;
        mark_flag = GUILIST_GetTotalItemNum(MMIFMM_RECORD_LIST_CTRL_ID);
    }
    is_del_one_file = isDeleteOne;

    MMIPUB_OpenConfirmationDialogByTextId(
        PNULL,
        prompt_text,
        PNULL,
        PNULL,
        &query_win_id,
        PNULL,
        MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
        HandleDelQueryWin
        );

    {
        MMIFMM_DELQUERY_DATA_T del_query_data = {0};
        del_query_data.current_path_ptr = current_path_ptr;
        del_query_data.list_data_ptr = list_data_ptr;
        del_query_data.mark_num = mark_flag;
        MMIAPICOM_Wstrncpy (del_query_data.cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
        DelQueryOpenCtrlFunc (query_win_id, &del_query_data);
    }
}
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeleteDrmFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    uint16 mark_num,
    wchar *cur_pathname
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_one_drmfile = FALSE;
    uint16 i = 0;
    wchar full_path_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};

    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return result;
    }

    if (0 < mark_num)
    {
        //如果只选中了一个，且为文件，则需要检查一下，是否是DRM，若是，直接退出
        for (i = 0; i < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[i]->state)
            {
                if (MMIFMM_FILE_TYPE_DRM == list_data_ptr->data[i]->type)
                {
                    SCI_MEMSET (full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
                    MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                                              list_data_ptr->data[i]->filename_ptr, list_data_ptr->data[i]->filename_len,
                                              full_path_name, MMIFMM_PATHNAME_LEN);
                    drm_permission = MMIAPIDRM_GetFilePermission (SFS_INVALID_HANDLE, full_path_name);
                    is_one_drmfile = MMIAPIDRM_IsRightsValid (SFS_INVALID_HANDLE, full_path_name, TRUE, drm_permission);

                    if (is_one_drmfile)
                    {
                        break;
                    }
                }
            }
        }

        result = is_one_drmfile;
    }
    else
    {
        //如果当前无标记，选择高亮文件
        MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
        file_handle = SFS_CreateFile (cur_pathname, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

        if (SFS_INVALID_HANDLE != file_handle)
        {
            if (MMIAPIDRM_IsDRMFile (file_handle, PNULL))
            {
                drm_permission = MMIAPIDRM_GetFilePermission (file_handle, PNULL);
                result = MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, drm_permission);
            }
        }

        SFS_CloseFile (file_handle);

    }

    return result;
}
#endif
/*****************************************************************************/
// Description : enter delete file win
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterDeleteWin (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    wchar* cur_pathname
)
{

    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;
    MMIFMM_DELWIN_DATA_T        *del_win_data_ptr = PNULL;

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_DEL_SELECT_FILE_WIN_ID);
    }

    del_win_data_ptr = (MMIFMM_DELWIN_DATA_T*) SCI_ALLOC_APPZ (sizeof (MMIFMM_DELWIN_DATA_T));

    del_win_data_ptr->s_copy_del_path_data_ptr = (MMIFMM_COPY_DEL_PATH_DATA_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_COPY_DEL_PATH_DATA_INFO_T));

    if (0 == list_data_ptr->mark_num)
    {
        //current_index = GUILIST_GetCurItemIndex(ctrl_id);
        MMIAPICOM_Wstrncpy (del_win_data_ptr->s_copy_del_path_data_ptr->cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
    }

	//CR161515
	// 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
	if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
	{
		del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = list_data_ptr->mark_num ;
	}
	else
	{
		if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
		{
			MMIAPICOM_Wstrncpy (del_win_data_ptr->s_copy_del_path_data_ptr->cur_pathname, current_path_ptr->pathname, current_path_ptr->pathname_len);
			del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = MMI_FMM_DELETE_ALLFILES_FLAG;
		}
		else
		{
			del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = list_data_ptr->mark_num ;
		}
	}


    del_win_data_ptr->current_path_ptr = current_path_ptr;
    del_win_data_ptr->list_data_ptr = list_data_ptr;
    del_win_data_ptr->s_delete_handle_ptr = PNULL;
    del_win_data_ptr->s_need_update = FALSE;
    del_win_data_ptr->s_user_stop_copy = FALSE;
    del_win_data_ptr->s_user_stop_del = FALSE;
    del_win_data_ptr->s_resoved_num = 0;
    del_win_data_ptr->s_copydel_opt_state = 0;

    win_handle = MMK_CreateWin ( (uint32*) MMIFMM_DEL_SELECTFILE_WIN_TAB, (ADD_DATA) del_win_data_ptr);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (del_win_data_ptr->s_copy_del_path_data_ptr);
        SCI_FREE (del_win_data_ptr);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T *del_win_ptr = (MMIFMM_DELWIN_DATA_T *) MMK_GetWinAddDataPtr (win_id);

    if (PNULL == del_win_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    MMK_SetWinUserData (win_id, (void *) del_win_ptr);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p->s_copy_del_path_data_ptr);
        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != delwin_data_p)
    {
        SFS_FindClose (delwin_data_p->s_copy_del_path_data_ptr->sfs_find_handle);
        delwin_data_p->s_copy_del_path_data_ptr->sfs_find_handle = 0;

        if (delwin_data_p->s_need_update)
        {
#ifdef MMI_PDA_SUPPORT
            GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_DELETE_END_CNF, PNULL, 0);
        }
    }
}


/*****************************************************************************/
//  Description : handle del task callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_DelCallBack (uint16     result)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);

    if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
    {
        if (PNULL != delwin_data_p && PNULL != delwin_data_p->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (delwin_data_p->s_delete_handle_ptr);
            delwin_data_p->s_delete_handle_ptr = PNULL;
        }

        MMK_PostMsg (MMIFMM_DEL_SELECT_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (SFS_ERROR_NONE != result && SFS_ERROR_NOT_EXIST != result) //MS00194050
    {
        if (PNULL != delwin_data_p && PNULL != delwin_data_p->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (delwin_data_p->s_delete_handle_ptr);
            delwin_data_p->s_delete_handle_ptr = PNULL;
        }

        MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
    }
}

PUBLIC void MMIAPIFMM_DelProgressCallBack(uint16 result)
{
    if (MMK_IsOpenWin(MMIFMM_FILE_WAITING_WIN_ID))
    {
        if (is_multi_deleted  && deleted_files_num < total_files_num)
        {
             actual_deleted_files_num++;
        }
        else if (!is_multi_deleted && deleted_files_num < 100)
        {
            actual_deleted_files_num++;
        }
    }
    deleted_files_num = actual_deleted_files_num;
    if ((actual_file_mark_num != 0) && (actual_file_mark_num > total_files_num))
    {
         deleted_files_num =  deleted_files_num * total_files_num / actual_file_mark_num;
    }
}

/*****************************************************************************/
//  Description : get del state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetDelState (void)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);

    if (PNULL != delwin_data_p)
    {
        return delwin_data_p->s_copydel_opt_state;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : Combine MMI_TEXT_ID_T and Number to Str
//  Global resource dependence :
//  Author: WEN.DONG
//  Note:
/*****************************************************************************/
void TextAndNumToStr(MMI_TEXT_ID_T text, int16 num, MMI_STRING_T *banner_str)
{
    MMI_STRING_T text_str = {0};
    MMI_STRING_T num_str = {0};
    wchar result_str[10] = {0};
    char temp_str[10] = {0};

    sprintf(temp_str, "%ld %",num);
    MMIAPICOM_StrToWstr((uint8*)temp_str,&result_str);
    MMIAPICOM_Wstrcat(&result_str, L"%");
    num_str.wstr_ptr = result_str;
    num_str.wstr_len = (uint16) MMIAPICOM_Wstrlen (result_str);
    MMI_GetLabelTextByLang(text, &text_str);
    MMIAPICOM_MergeTwoString(&num_str,&text_str,banner_str,30);
}
