/*****************************************************************************
** File Name:      mmirecord_common_wintab.c                                 *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_COMMON_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmk_msg.h"
#include "mmi_default.h"
#include "mmipub.h"
#include "mmicom_panel.h"
#include "mmk_timer.h"
#include "mmi_image.h"
#include "mmi_pubmenuwin.h"
#include "mmitheme_update.h"

#include "mmirecord_wintablel.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_main.h"
#include "mmirecord_image.h"
#include "mmirecord_menutable.h"
#include "mmirecord_id.h"
#include "mmirecord_text.h"
#include "mmi_common.h"
#include "guitext.h"
#include "mmicom_banner.h"
#include "mmifmm_export.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef WIN32
		LOCAL uint8  g_timer_id = 0;  //record timer
		LOCAL uint32 g_record_cur_timer_elapsed = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef WIN32
LOCAL void MMIRECORD_SimulatorTimerCallBack(void)
{
        g_record_cur_timer_elapsed++;
}

LOCAL void MMIRECORD_SimulatorStartTimer(void)
{
    uint32  time_out = 1000;

    if(0 != g_timer_id)
    {
        MMK_StopTimer(g_timer_id);
        g_timer_id = 0;
    }
    g_timer_id = MMK_CreateWinTimer(MMIRECORD_MAINPLAY_WIN_ID, time_out, TRUE);
}
LOCAL void MMIRECORD_SimulatorStopTimer(void)
{
    if(0 != g_timer_id)
    {
        MMK_StopTimer(g_timer_id);
        g_timer_id = 0;
    }
}
#endif


LOCAL uint32 s_record_update_animation =0; 
LOCAL BOOLEAN is_pop_name_dialog = FALSE;
BOOLEAN is_delete_record = FALSE;
uint32 total_record_time = 0;

/*****************************************************************************/
//  Description :是否插入录音询问窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordWorkQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);//recordwork

/*****************************************************************************/
//  Description : HandleRecordCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordCallbackMsg(MMI_HANDLE_T win_handle,
                                   DPARAM     param);

/*****************************************************************************/
//  Description : HandlePlayCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCallbackMsg(MMI_HANDLE_T win_handle,
                                 DPARAM     param);

/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

/*****************************************************************************/
//  Description : OpenOptionMenuWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void OpenOptionMenuWin(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : start record
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : PauseRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PauseRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : ResumeRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumeRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : ResumePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumePlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : PausePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PausePlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopPlay(MMI_HANDLE_T win_handlen);

/*****************************************************************************/
//  Description : StartPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartPlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayProgressTimer(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayProgressTimer(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopRecordOrPlay
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopRecordOrPlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : HandleTimerMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerMsg(MMI_HANDLE_T win_handle,
                                  DPARAM     param);

/*****************************************************************************/
//  Description : handle get focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordGetFocus(MMI_HANDLE_T win_handle);
/*****************************************************************************/
//  Description : handle lose focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordLoseFocus(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : HandleOKKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOKKeyMsg(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMainWinUserData(MMIRECORD_WORK_PARAM_T *work_param_ptr,
                                    MMI_HANDLE_T      win_handle);

/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeMainWinUserData(MMI_HANDLE_T      win_handle);

/*****************************************************************************/
//  Description : set recordfile pah and name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRecordFilePath(RECORD_WIN_DATA_T * record_win_data_ptr);

/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_MAINPLAY_WIN_TAB) =
{
#ifndef MMI_RECORD_MINI_SUPPORT
    WIN_STATUSBAR,
#else
    WIN_TITLE(TXT_RECORDER),
#endif
    WIN_FUNC((uint32)HandleRecordMainPlayWinMsg),
    WIN_ID(MMIRECORD_MAINPLAY_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};


/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_HANDLE_T win_handle)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return result;
    }
    
    SCI_TRACE_LOW("[RECORD] HandleWebKeyMsg, entry!");
            
#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
        //MMK_PostMsg(win_handle, MSG_RECORD_START_PLAY, PNULL, PNULL);
        MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]
        && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
#endif
 
    return result;
}
//dong.chunguang add 
LOCAL MMI_RESULT_E HandleNoRecordWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
	MMI_RESULT_E  result = MMI_RESULT_TRUE;
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
		break;
	case MSG_APP_WEB:
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : restore confirm
//  Global resource dependence : none
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordNameWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_TM_T tm = {0};
    uint16   i = 0;
    uint16   j = 0;
    uint8  init_num[5] = {0};
    uint16  num_len = 0;
    uint16  full_path_len = 0;
    uint8  temp_name_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    wchar  record_name_ptr[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    wchar  new_record_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    wchar  new_record_full_path_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    wchar  text[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    MMI_STRING_T    	temp_str = {0};
    MMI_STRING_T    	record_str = {0};
    MMI_HANDLE_T      	ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPUB_TEXT_CTRL_ID );
    RECORD_WIN_DATA_T   *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(MMIRECORD_MAINPLAY_WIN_ID);
    MMIRECORD_WORK_FORMAT_E workFormat = MMIRECORD_GetRecordFileFormatType();

    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
            //GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
            MMIPUB_SetDialogBgLayerStyle(win_id,DIALOG_MASK_BG_SCREEN);
            for (i = 0; i <= MMIFMM_FILE_NUM; i++)
            {
                SCI_MEMSET (temp_name_arr, 0, sizeof (temp_name_arr));
                SCI_MEMSET (new_record_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
                tm = MMIAPICOM_Second2Tm (MMIAPICOM_GetCurTime());
                MMIAPISET_FormatDateStrByDateDisplyType (tm.tm_year, tm.tm_mon, tm.tm_mday, '-', temp_name_arr, MMIFMM_FULL_FILENAME_LEN,MMISET_DATE_DMY);
                MMIAPICOM_StrToWstr (temp_name_arr, new_record_name);
    
                if (0 != i)
                {
                    sprintf ( (char*) init_num, "%d", i);
                    num_len = (uint16) strlen ( (char*) init_num);
    
                    SCI_MEMSET (record_name_ptr, 0, sizeof (record_name_ptr));
                    MMIAPICOM_StrToWstr (init_num, record_name_ptr);
                    MMIAPICOM_Wstrcat(new_record_name,L"(");
                    MMIAPICOM_Wstrcat(new_record_name,record_name_ptr);
                    MMIAPICOM_Wstrcat(new_record_name,L")");
                }
    
                SCI_TRACE_LOW("[RECORD] line[%d] file[%s] func[%s] workFormat=%d", __LINE__, __FILE__, __FUNCTION__, workFormat);
                if(MMIRECORD_WORK_FORMAT_AMR == workFormat){
                    MMIAPICOM_Wstrcat(new_record_name,L".amr");
                }else if(MMIRECORD_WORK_FORMAT_MP3 == workFormat){
                    MMIAPICOM_Wstrcat(new_record_name,L".mp3");
                }else{
                    MMIAPICOM_Wstrcat(new_record_name,L".wav");
                }
    
                SCI_MEMSET (new_record_full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
                for (j = (uint16) MMIAPICOM_Wstrlen (record_win_data_ptr->record_file_full_path) - 1; MMIFILE_SLASH != record_win_data_ptr->record_file_full_path[j] && j > 0; j--)
                {
                    NULL;
                }
    
                full_path_len = (uint16) MMIFMM_CombineToFileName (record_win_data_ptr->record_file_full_path,j,
                                         new_record_name,(uint16) MMIAPICOM_Wstrlen (new_record_name),
                                         new_record_full_path_name,(uint16) MMIFMM_FULL_FILENAME_LEN);
    
                if (!MMIAPIFMM_IsFileExist (new_record_full_path_name, (uint16) full_path_len))
                {
                    break;
                }
            }
		GUIEDIT_SetFont(MMIPUB_INPUTFIELD_CTRL_ID,SONG_FONT_24);	
            GUIEDIT_SetString (MMIPUB_INPUTFIELD_CTRL_ID, new_record_name, (uint16) MMIAPICOM_Wstrlen (new_record_name) - 4);
            //End
            break;
	//case MSG_KEYUP_OK:
	case MSG_CTL_OK:
	case MSG_APP_OK:
            MMIRECORD_DeleteRecordFile();
            MMK_CloseWin(win_id);
            break;
	case MSG_CTL_MIDSK:
		//GUITEXT_GetString(ctrl_handle,&temp_str);
		GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID,&temp_str);
		if (0 == temp_str.wstr_len)
		{
		    break;
		}

		if (MMIAPICOM_IsIncludeInvalidChar(temp_str.wstr_ptr,temp_str.wstr_len))
		{
		    MMIPUB_OpenAlertWinByTextId (PNULL, STR_FILES_INVALID_CHARACTER_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
		    break;
		}

        if (!MMIAPICOM_GetValidFileName(FALSE, temp_str.wstr_ptr, temp_str.wstr_len, text, MMIFILE_FILE_NAME_MAX_LEN))
        {
            MMIPUB_OpenAlertWinByTextId (PNULL, STR_FILE_NAME_INVALID, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            break;
        }

        record_str.wstr_ptr = text;
        record_str.wstr_len = MMIAPICOM_Wstrlen(text);

#if 0
		//MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_STOP_RECORD, PNULL, PNULL);
		record_str.wstr_ptr = text;
		MMIAPICOM_Wstrcat(record_str.wstr_ptr,temp_str.wstr_ptr);
		record_str.wstr_len = temp_str.wstr_len;
#endif

		SCI_TRACE_LOW("[RECORD] line[%d] file[%s] func[%s] workFormat=%d", __LINE__, __FILE__, __FUNCTION__, workFormat);
		if(MMIRECORD_WORK_FORMAT_AMR == workFormat){
		    MMIAPICOM_Wstrcat(record_str.wstr_ptr,L".amr");
		}else if(MMIRECORD_WORK_FORMAT_MP3 == workFormat){
		    MMIAPICOM_Wstrcat(record_str.wstr_ptr,L".mp3");
		}else{
		    MMIAPICOM_Wstrcat(record_str.wstr_ptr,L".wav");
		}
		record_str.wstr_len = record_str.wstr_len + 4;

		SCI_MEMSET (new_record_full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
		for (j = (uint16) MMIAPICOM_Wstrlen (record_win_data_ptr->record_file_full_path) - 1; MMIFILE_SLASH != record_win_data_ptr->record_file_full_path[j] && j > 0; j--)
		{
		    NULL;
		}
		full_path_len = (uint16) MMIFMM_CombineToFileName (record_win_data_ptr->record_file_full_path,j,record_str.wstr_ptr,
		                 record_str.wstr_len,new_record_full_path_name,(uint16) MMIFMM_FULL_FILENAME_LEN);
		if (MMIAPIFMM_IsFileExist (new_record_full_path_name, (uint16) full_path_len))
		{
		    MMIPUB_OpenAlertWinByTextId (PNULL, STR_FILES_NAME_IN_USE_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
		    break;
		}

		if(MMIRECORDSRV_IsRecordFileExist(MMIRECORD_GetRecordHandle())){
			MMIRECORDSRV_RenameRecord(MMIRECORD_GetRecordHandle(),&record_str);
		}
		else{
			MMIRECORD_DeleteRecordFile();
		}
		UpdateRecordFilePath(record_win_data_ptr);
		MMK_CloseWin(win_id);

		{
		    MMICOM_BANNER_INFO_T banner_info = {0};
		    banner_info.banner_type = MMI_BANNER_TEXT;
		    banner_info.text1_id = STR_RECORDING_SAVED_EXT01;
		    MMIAPICOM_OpenBannerWin(MMIRECORD_MAINPLAY_WIN_ID, banner_info);
		}

		break;
	case MSG_NOTIFY_CANCEL:
	case MSG_APP_CANCEL:
		GUIEDIT_GetString(MMIPUB_INPUTFIELD_CTRL_ID,&temp_str);
		if (0 == temp_str.wstr_len)
		{
		    break;
		}
		MMIRECORD_DeleteRecordFile();
		MMK_CloseWin(win_id);
		break;
	case MSG_CLOSE_WINDOW:
		is_pop_name_dialog = FALSE;
		total_record_time = 0;
		break;
	default:
		result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
		break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : restore confirm
//  Global resource dependence : none
//  Author: dong.chunguang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQureyDeleteRecordWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	MMI_STRING_T    	temp_str = {0};
	MMI_HANDLE_T      	ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPUB_TEXT_CTRL_ID );
	RECORD_WIN_DATA_T   *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(MMIRECORD_MAINPLAY_WIN_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		//GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
    	break;
	case MSG_KEYUP_OK:
		//MMK_CloseWin(win_id);
		break;
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_KEYUP_WEB:
	case MSG_KEYDOWN_WEB:
		//MMK_CloseWin(win_id);
		//MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_STOP_RECORD_PLAY, PNULL, PNULL);
		//StopRecordOrPlay(record_win_data_ptr->win_handle);
		total_record_time = 0;
		is_delete_record = TRUE;
		StopRecord(record_win_data_ptr->win_handle);
		MMK_CloseWin(win_id);
        if(MMIRECORD_RESULT_NO_EXIST == MMIRECORD_DeleteRecordFile())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                        MMIPUB_SOFTKEY_ONE, PNULL);
        }
		MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_STOP_RECORD_PLAY, PNULL, PNULL);

		{
		    MMICOM_BANNER_INFO_T banner_info = {0};
		    banner_info.banner_type = MMI_BANNER_TEXT;
		    banner_info.text1_id = STR_RECORDING_DELETED_EXT01;
		    MMIAPICOM_OpenBannerWin(MMIRECORD_MAINPLAY_WIN_ID, banner_info);
		}
		break;
	case MSG_NOTIFY_CANCEL:
    case MSG_APP_CANCEL:
        MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_RESUME_RECORD, PNULL, PNULL);
        MMK_CloseWin(win_id);
        break;
	case MSG_CLOSE_WINDOW:
		is_delete_record = FALSE;
		break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : HandleOKKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOKKeyMsg(MMI_HANDLE_T win_handle)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return result;
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            && MMIRECORD_BUTTON_STATE_FOCUS != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            && MMIRECORD_BUTTON_STATE_FOCUS != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        //MMK_PostMsg(win_handle, MSG_OPEN_OPTION_WIN, PNULL, PNULL);
	    //if(PNULL != record_win_data_ptr)
        //{
            MMIAPICOM_CloseBanner();
            MMIRECORD_OpenRecordFileList();
        //}else{	
        //    MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_RECORD_MS_NO_RECORD,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleNoRecordWin);
		//}
    }else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    //else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
		// Bug 1349242, 如果为外部应用启动录音，则直接保存，不弹出命名弹框，否则会丢失信息
		if (record_win_data_ptr->is_insert_record == FALSE)
		{
			is_pop_name_dialog = TRUE;
		}
		MMK_PostMsg(win_handle, MSG_STOP_RECORD, PNULL, PNULL);
        //MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
/*
#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_PAUSE_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_RESUME_PLAY, PNULL, PNULL);
    }

#endif
*/
    return result;
}

/*****************************************************************************/
//  Description : HandleRecordCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordCallbackMsg(MMI_HANDLE_T win_handle,
                                   DPARAM     param)
{
    MMIRECORD_RESULT_E result = FALSE;

    if(PNULL == param)
    {
        return;
    }

    result = *(MMIRECORD_RESULT_E *)param;
    SCI_TRACE_LOW("[RECORD] HandleRecordCallbackMsg result = %d", result);
    MMIDEFAULT_TurnOnBackLight();
    StopDisplayProgressTimer(win_handle);

    if(MMIRECORD_RESULT_ERROR == result)
    {
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL,
                                    MMIPUB_SOFTKEY_ONE, PNULL);
        MMIRECORD_UpdateButtonStatusReady(win_handle);
    }
    else
    {
        if(MMIRECORD_RESULT_NO_SPACE == result)
        {
            MMIRECORD_ShowErrorMsg(SFS_ERROR_NO_SPACE);
        }

        MMIRECORD_UpdateButtonStatusStopped(win_handle);
    }

    MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
}

/*****************************************************************************/
//  Description : HandlePlayCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCallbackMsg(MMI_HANDLE_T win_handle,
                                 DPARAM     param)
{
    BOOLEAN is_succ = FALSE;

    if(PNULL == param)
    {
        return;
    }

    is_succ = *(BOOLEAN *)param;

    SCI_TRACE_LOW("[RECORD] HandlePlayCallbackMsg is_succ = %d", is_succ);
    //playing end
    MMIDEFAULT_TurnOnBackLight();

    if(!is_succ)
    {
        MMIPUB_OpenAlertFailWin(STR_FILE_NOT_SUPPORT);
    }
    else
    {
        MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
        MMITHEME_UpdateRect();
    }

    StopDisplayProgressTimer(win_handle);
    MMIRECORD_UpdateButtonStatusStopped(win_handle);
    MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);

}

/*****************************************************************************/
//  Description : HandleTimerMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerMsg(MMI_HANDLE_T win_handle,
                                  DPARAM     param)
{
    MMI_RESULT_E            recode  = MMI_RESULT_FALSE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr
            && record_win_data_ptr->timer_id == *((uint8 *) param))
    {
        //StopDisplayProgressTimer(win_handle);

        if(MMK_IsFocusWin(win_handle))
        {
            /*
            if(MMIRECORD_IsReachMax(record_win_data_ptr->max_file_time, record_win_data_ptr->max_file_size))
            {
                StopRecord(win_handle);
            }
            */
            //bug 721347. begin 2018.01.12
            if(MMIRECORD_IsReachMax(record_win_data_ptr->max_file_time, record_win_data_ptr->max_file_size)
                || !MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, MMIRECORD_GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
            {
                MMIRECORD_RESULT_E record_result = MMIRECORD_RESULT_NO_SPACE;
                StopRecord(win_handle);
                SCI_TRACE_LOW("lzk log. HandleTimerMsg MMIAPIFMM_IsEnoughSpace().");
                MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_RECORD_CALL_BACK, &record_result, sizeof(MMIRECORD_RESULT_E));
            }
            //bug 721347. end 2018.01.12
            else
            {
                s_record_update_animation++;
                if(0 == (s_record_update_animation % 10))
                {
                    MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
                }else{
                    MMIRECORD_UpdateScreen(win_handle, FALSE, FALSE);
                }
                //StartDisplayProgressTimer(win_handle);
				/*
            	if(is_pop_name_dialog==TRUE&&((MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])||
								(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])))
            	{
					MMI_STRING_T    			temp_str = {0};
					MMI_WIN_ID_T                dialog_win_id = MMIRECORD_NAME_DIALOG_WIN_ID;
					
					MMI_GetLabelTextByLang(TXT_RECORD_MS_RECORDING, &temp_str);
					if(!MMK_IsOpenWin(dialog_win_id)){//no open name dialog
						MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleRecordNameWin);
					}
					is_pop_name_dialog = FALSE;
				}else if(is_delete_record == TRUE&&((MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])||
								(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]))){
            		MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_RECORD_MS_DELETE_RECORD,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleQureyDeleteRecordWin);
					is_delete_record = FALSE;
				}*/
            }
        }

        recode  = MMI_RESULT_TRUE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : set recordfile pah and name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRecordFilePath(RECORD_WIN_DATA_T * record_win_data_ptr)
{
    wchar *record_file_path_ptr = MMIRECORD_GetRecordFilePath();
    wchar *record_file_name_ptr = MMIRECORD_GetRecordFileName();
    uint16  str_len = 0;

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    SCI_MEMSET(record_win_data_ptr->record_file_full_path, 0, MMIRECORD_SRV_MAX_FILE_NAME_LEN);
    record_win_data_ptr->record_file_full_path_len = 0;

    SCI_MEMSET(record_win_data_ptr->record_file_name, 0, MMIRECORD_SRV_MAX_FILE_NAME_LEN);
    record_win_data_ptr->record_file_name_len = 0;

    str_len = MMIAPICOM_Wstrlen(record_file_path_ptr);

    if(PNULL != record_file_path_ptr)
    {
        record_win_data_ptr->record_file_full_path_len = MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN, str_len);
        MMIAPICOM_Wstrncpy(record_win_data_ptr->record_file_full_path, record_file_path_ptr, record_win_data_ptr->record_file_full_path_len);

    }

    if(PNULL != record_file_name_ptr)
    {
        record_win_data_ptr->record_file_name_len = MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN, str_len);
        MMIAPICOM_Wstrncpy(record_win_data_ptr->record_file_name, record_file_name_ptr, record_win_data_ptr->record_file_name_len);
    }

}

/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
    LOCAL uint8 delay_time_id = 0;
    LOCAL BOOLEAN is_dalayed = FALSE;
    MMI_STRING_T   title_str = {0};

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    if(MSG_TIMER != msg_id)
    {
        SCI_TRACE_LOW("[RECORD] HandleRecordMainPlayWinMsg msg_id = 0x%x", msg_id);
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIRECORD_RequestVirtualHandle();

        if(!MMIRECORD_Constructor())
        {
            MMK_CloseWin(win_id);
            break;
        }

        UpdateRecordFilePath(record_win_data_ptr);
        break;
	case MSG_FULL_PAINT:
        MMI_GetLabelTextByLang(STXT_HEADER_VOICE_REC, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
		break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        HandleOKKeyMsg(record_win_data_ptr->win_handle);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        HandleWebKeyMsg(record_win_data_ptr->win_handle);
        break;
        
    case MSG_GET_FOCUS:
        MMI_GetLabelTextByLang(STXT_HEADER_VOICE_REC, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
        HandleRecordGetFocus(record_win_data_ptr->win_handle);
        break;

    case MSG_APP_UPSIDE:
    case MSG_APP_UP:  //增大音量，显示音量调节杆
    	break;//don't change volume
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_INC);
        }

        break;

    case MSG_APP_DOWNSIDE:
    case MSG_APP_DOWN:  //减小音量，显示音量调节杆
    	break;//don't change volume
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_DEC);
        }

        break;

    case MSG_APP_HASH:
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_SetMute();
        }

        break;

    case MSG_LOSE_FOCUS:
        HandleRecordLoseFocus(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_CALL_BACK:
        HandleRecordCallbackMsg(record_win_data_ptr->win_handle, param);
        break;

    case MSG_RECORD_PLAY_CALL_BACK:
        HandlePlayCallbackMsg(record_win_data_ptr->win_handle, param);
        break;

    case MSG_STOP_RECORD_PLAY:
        StopRecordOrPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_START_PLAY:
        StartPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_START_RECORD:
        if(PNULL != param)
        {
            StartRecord(*(BOOLEAN *)param, record_win_data_ptr->win_handle);
        }
        else
        {
            StartRecord(FALSE, record_win_data_ptr->win_handle);
        }
        MMI_GetLabelTextByLang(STR_HDR_RECORDING_EXT01, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
#ifdef WIN32
		MMIRECORD_SimulatorStartTimer();
#endif
        break;

    case MSG_STOP_RECORD:
        {
            uint32 cur_time = MMIRECORD_GetRecordSecondTime();
            if(!is_dalayed && cur_time <= 1)
            {
                is_dalayed = TRUE;
                delay_time_id = MMK_CreateWinTimer(record_win_data_ptr->win_handle, 300, FALSE);
            }
            else
            {
                is_dalayed = FALSE;
                StopRecord(record_win_data_ptr->win_handle);
            }
#ifdef MMI_ETWS_SUPPORT
          if(TRUE  == MMIAPISMSCB_IsETWSAlert())
           {
                is_pop_name_dialog = FALSE;
           }
#endif            
            if(is_pop_name_dialog==TRUE)
            {
                MMI_STRING_T    			temp_str = {0};
                MMI_WIN_ID_T                dialog_win_id = MMIRECORD_NAME_DIALOG_WIN_ID;
                MMI_WIN_PRIORITY_E 			win_priority = WIN_ONE_LEVEL;
                //PauseRecord(record_win_data_ptr->win_handle);
                MMI_GetLabelTextByLang(STR_RADIO_PRMPT_NAME_REC_EXT01, &temp_str);
                MMIPUB_OpenDialogInputFieldWinByTextPtr(&temp_str,&dialog_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,HandleRecordNameWin);
                is_pop_name_dialog = FALSE;
            }
        }
        break;

    case MSG_RECORD_STOP_PLAY:
        StopPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_PAUSE_PLAY:
        PausePlay(record_win_data_ptr->win_handle);
        break;

    case MSG_PAUSE_RECORD:
        MMI_GetLabelTextByLang(STR_HDR_PAUSED_EXT01, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
		PauseRecord(record_win_data_ptr->win_handle);
		if(is_delete_record==TRUE)
		{
		    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_RECORDING_CANCEL_EXT01,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleQureyDeleteRecordWin);
		    is_delete_record = FALSE;
		}
        break;

    case MSG_RECORD_RESUME_PLAY:
        ResumePlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RESUME_RECORD:
        MMI_GetLabelTextByLang(STR_HDR_RECORDING_EXT01, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
        ResumeRecord(record_win_data_ptr->win_handle);
        break;

    case MSG_INSERT_RECORD:
        if(0 != record_win_data_ptr->record_file_name_len
                && MMIRECORD_InsertRecord(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len))
        {
            MMK_CloseWin(win_id);
        }

        break;

    case MSG_OPEN_OPTION_WIN:
        OpenOptionMenuWin(win_id);
        break;

    case MSG_TIMER:
        if(PNULL != param)
        {
            if(delay_time_id == *((uint8*)param))
            {
                MMK_StopTimer(delay_time_id);
                delay_time_id = 0;
                MMK_PostMsg((record_win_data_ptr->win_handle), MSG_STOP_RECORD, PNULL, PNULL);
            }
#ifdef WIN32
		  if(g_timer_id == *((uint8*)param))
		  {
			  MMIRECORD_SimulatorTimerCallBack();
		  }
#endif
            (void)HandleTimerMsg(record_win_data_ptr->win_handle, param);
        }
        break;

    /*case MSG_CLOSE_WINDOW:
		record_win_data_ptr->is_insert_record = FALSE;
        StopRecordOrPlay(record_win_data_ptr->win_handle);
        FreeMainWinUserData(record_win_data_ptr->win_handle);
        MMIRECORD_Destructor();
        MMIRECORD_FreeVirtualHandle();
        MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
        break;*/ //bug-72892;
#if 0 //UNISOC_MMI_Delete
    case MSG_PROMPTWIN_OK:
        if(PNULL != param)
        {
            MMI_WIN_ID_T            query_win_id = *(MMI_WIN_ID_T*)param;
            MMK_CloseWin(query_win_id);

            if(MMIRECORD_RESULT_NO_EXIST == MMIRECORD_DeleteRecordFile())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                            MMIPUB_SOFTKEY_ONE, PNULL);
            }

            UpdateRecordFilePath(record_win_data_ptr);

        }

        break;
#endif
    case MSG_PROMPTWIN_OK:
    case MSG_PROMPTWIN_CANCEL:
        if(PNULL != param)
        {
            MMI_WIN_ID_T            query_win_id = *(MMI_WIN_ID_T*)param;
            MMK_CloseWin(query_win_id);
        }

        break;

    default:
        //recode = MMI_RESULT_FALSE;
        break;
    }
    
    recode = MMIRECORD_HandleRecordMainPlayWinMsg(win_id,msg_id,param);

    //bug-72892;
    if(MSG_CLOSE_WINDOW == msg_id)
    {
        is_dalayed = FALSE;
        if(delay_time_id)
        {
            MMK_StopTimer(delay_time_id);
            delay_time_id = 0;
        }
        record_win_data_ptr->is_insert_record = FALSE;
        StopRecordOrPlay(record_win_data_ptr->win_handle);  
        FreeMainWinUserData(record_win_data_ptr->win_handle);
        MMIRECORD_Destructor();
        MMIRECORD_FreeVirtualHandle();   
        MMIAPICOM_CloseBanner();
        MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
    }

    return recode;
}

/*****************************************************************************/
//  Description : StopRecordOrPlay
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopRecordOrPlay(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        StopRecord(win_handle);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        StopPlay(win_handle);
    }
}

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_GROUP_ID_T     group_id    = MENU_RECORD_START_OPTION;
    MMI_MENU_ID_T           menu_id     = ID_RECORD_STORIGE_POS;
    BOOLEAN                 is_Rerecord  = FALSE;
    MMI_WIN_ID_T            query_win_id = MMIRECORD_QUERY_WIN_ID;


    if(PNULL != param)
    {
        RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(param->dst_handle);
        GUIMENU_GetId(param->ctrl_id, &group_id, &menu_id);

        switch(menu_id)
        {
        case ID_RECORD_START:
            MMK_PostMsg(param->dst_handle, MSG_START_RECORD, PNULL, PNULL);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_LIST:
            if(PNULL != record_win_data_ptr)
            {
                MMIRECORD_OpenRecordFileList();
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STOPPED_PLAY:
            MMK_CloseWin(param->win_id);
            MMK_PostMsg(param->dst_handle , MSG_RECORD_START_PLAY, PNULL, PNULL);
            break;

        case ID_RECORD_AGAIN:
            is_Rerecord = TRUE;
            MMK_PostMsg(param->dst_handle, MSG_START_RECORD, &is_Rerecord, sizeof(BOOLEAN));
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            break;

        case ID_RECORD_INSERT://recordwork
            MMK_PostMsg(param->dst_handle, MSG_INSERT_RECORD, PNULL, PNULL);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_SEND_BY_MMS:
            if(PNULL != record_win_data_ptr
                    && 0 != record_win_data_ptr->record_file_name_len)
            {
                MMIRECORD_SendFileViaMMS(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len);
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_SEND_BY_BT:
            if(PNULL != record_win_data_ptr
                    && 0 != record_win_data_ptr->record_file_name_len)
            {
                MMIRECORD_SendFileViaBT(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len);
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_WAV:
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_WAV);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_MP3://andy.he_record
            //set storage file format type  menu
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_MP3);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_AMR://andy.he_record
            //set storage file format type  menu
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_AMR);
            MMK_CloseWin(param->win_id);
            break;

        default:
            if(menu_id >= ID_RECORD_UDISK && menu_id <= ID_RECORD_UDISK_MAX)
            {
                //set storage menu
                MMIRECORD_SetRecordStorage(MMI_DEVICE_UDISK + menu_id - ID_RECORD_UDISK);
                MMK_CloseWin(param->win_id);
            }

            break;
        }
    }
}


/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    if(PNULL != param)
    {
        MMIRECORD_LoadRecordMenu(param->dst_handle);
        MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
    }

}


/*****************************************************************************/
//  Description : OpenOptionMenuWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void OpenOptionMenuWin(MMI_HANDLE_T win_handle)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    MMIAPICOM_ClosePanelChildWin();

    menuparam.group_id = (MMI_MENU_GROUP_ID_T)MENU_RECORD_START_OPTION;
    menuparam.win_id = (MMI_WIN_ID_T)MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID;
    menuparam.applet_handle = SPRD_RECORD_APPLET_ID;

    menuparam.dst_handle = win_handle;
    menuparam.func.OnMenuOk = HandlePubMenuOKHandler;
    menuparam.func.OnMenuInit = HandlePubMenuInitHandler;
    MMIPUB_OpenPubMenuWin(&menuparam);



    //MMK_CreateWin((uint32 *)MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_TAB,(ADD_DATA)win_handle);
}
/*****************************************************************************/
//  Description : PauseRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PauseRecord(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_PauseRecord())
    {
        MMIRECORD_UpdateButtonStatusRecordPaused(win_handle);
        StopDisplayProgressTimer(win_handle);
        //MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
        GUIWIN_SeSoftkeytButtonIconId(win_handle, IMAGE_RECORD_RECORD, MIDDLE_BUTTON, TRUE);
#ifdef WIN32
	MMIRECORD_SimulatorStopTimer();
#endif
    }
}

/*****************************************************************************/
//  Description : ResumeRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumeRecord(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_ResumeRecord())
    {
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        StartDisplayProgressTimer(win_handle);
        //MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
        GUIWIN_SeSoftkeytButtonIconId(win_handle, IMAGE_RECORD_PAUSE, MIDDLE_BUTTON, TRUE);
#ifdef WIN32
	MMIRECORD_SimulatorStartTimer();
#endif
    }
}

/*****************************************************************************/
//  Description : ResumePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumePlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_ResumePlay())
    {
        MMIRECORD_UpdateButtonStatusPlaying(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
    }
}

/*****************************************************************************/
//  Description : PausePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PausePlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_PausePlay())
    {
        MMIRECORD_UpdateButtonStatusPlayPause(win_handle);
        StopDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
    }
}

/*****************************************************************************/
//  Description : StopPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopPlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_StopPlay())
    {
        MMIRECORD_UpdateButtonStatusStopped(win_handle);
        StopDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
    }

}

/*****************************************************************************/
//  Description : StartPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartPlay(MMI_HANDLE_T win_handle)
{

    MMIRECORD_RESULT_E  result = MMIRECORD_RESULT_SUCCESS;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);


    result = MMIRECORD_PlayCurrent();

    if(MMIRECORD_RESULT_SUCCESS == result)
    {
        MMIRECORD_UpdateButtonStatusPlaying(win_handle);
        StartDisplayProgressTimer(win_handle);
    }
    else if(MMIRECORD_RESULT_NO_EXIST == result)
    {
        UpdateRecordFilePath(record_win_data_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
        MMIRECORD_UpdateButtonStatusReady(win_handle);
    }
    else
    {
        MMIPUB_OpenAlertFailWin(STR_FILE_NOT_SUPPORT);
    }

    MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
}
/*****************************************************************************/
//  Description : StopRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopRecord(MMI_HANDLE_T win_handle)
{
    MMI_WIN_ID_T    query_win_id = MMIRECORD_WORK_QUERY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_RESULT_SUCCESS == MMIRECORD_StopRecord())
    {
        if(record_win_data_ptr->is_insert_record && !is_delete_record)//recordwork
        {
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_RECORD_APPLET_ID,
                                          TXT_RECORD_SAVEAINSERTRECORD,
                                          IMAGE_PUBWIN_QUERY,
                                          &query_win_id,
                                          HandleRecordWorkQueryWinMsg,
                                          win_handle);
        }
        else
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
        }

        MMIRECORD_UpdateButtonStatusStopped(win_handle);
        StopDisplayProgressTimer(win_handle);
        total_record_time = 0;
        /*if (MMIAPICC_IsInState (CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState (CC_MO_CONNECTING_STATE))
        {
            MMIRECORD_DeleteRecordFile();
        }*/
    }
#ifdef WIN32
		MMIRECORD_SimulatorStopTimer();
		g_record_cur_timer_elapsed = 0;
#endif 
    MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);

}
/*****************************************************************************/
//  Description : start record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle)
{
    BOOLEAN error_code = TRUE;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NO_SPACE;
    uint32 record_id = MMIRECORD_GetRecordId();
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);
    MMIFILE_DEVICE_E file_dev;
    MMI_TEXT_ID_T text_id = TXT_NULL;

    if(PNULL == record_win_data_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[RECORD] StartRecord is_reRecord = %d", is_reRecord);

    if(!is_reRecord)
            //&& !record_win_data_ptr->is_insert_record)
    {
        record_win_data_ptr->record_support_type = MMIRECORD_GetRecordFileFormatType();
    }
	
    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        file_dev = MMIRECORD_GetRecordStorage();
    }
    else
    {
        file_dev = MMIAPIFMM_GetFirstValidSD();
        SCI_TRACE_LOW("[RECORD] StartRecord file_dev = %d", file_dev);
        if((MMI_DEVICE_NUM <= file_dev) || (file_dev <= MMI_DEVICE_UDISK))
        {
            text_id = STR_NOTE_NO_CARD;
            MMIPUB_OpenAlertWinByTextId (PNULL, text_id, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            error_code = FALSE;
            return error_code;
        }
    }
  

    fs_error = MMIRECORD_StartRecordNormal(is_reRecord, record_win_data_ptr->record_support_type, &record_id, file_dev);

    UpdateRecordFilePath(record_win_data_ptr);

    switch(fs_error)
    {
    case SFS_ERROR_NONE:
        MMIAPICOM_ClosePanelChildWin();
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE, TRUE);
        break;

    default:
        MMIRECORD_UpdateButtonStatusReady(win_handle);
        MMIRECORD_ShowErrorMsg(fs_error);
        error_code = FALSE;
        break;
    }

    return error_code;
}


/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayProgressTimer(MMI_HANDLE_T win_handle)
{
    uint32  time_out = MMI_RECORD_TIME_OUT;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    if(0 != record_win_data_ptr->timer_id)
    {
        MMK_StopTimer(record_win_data_ptr->timer_id);
        record_win_data_ptr->timer_id = 0;
        s_record_update_animation = 0;
    }

    record_win_data_ptr->timer_id = MMK_CreateWinTimer(win_handle, time_out, TRUE);

}

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayProgressTimer(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(0 != record_win_data_ptr->timer_id)
    {
        MMK_StopTimer(record_win_data_ptr->timer_id);
        record_win_data_ptr->timer_id = 0;
        s_record_update_animation = 0;
    }
}

/*****************************************************************************/
//  Description :HandleRecordWorkQueryWinMsg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordWorkQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)//recordwork
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *info_ptr     = (MMIPUB_INFO_T*)MMK_GetWinAddDataEx(win_id);
    MMI_HANDLE_T  mainwin_handle  = 0;


    switch(msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if(PNULL != info_ptr)
        {
            mainwin_handle  = info_ptr->user_data;
            MMK_PostMsg(mainwin_handle, MSG_INSERT_RECORD, PNULL, PNULL);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : handle lose focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordLoseFocus(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr
            && record_win_data_ptr->is_insert_record
            && (0 != record_win_data_ptr->max_file_time
                || 0 != record_win_data_ptr->max_file_size))
    {
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]
                || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
        {
            SCI_TRACE_LOW("[MMIRECORD] Main Win Lose Focus, cur is work state, will pause record");
            PauseRecord(win_handle);
            record_win_data_ptr->is_record_work_paused = TRUE;
        }
    }
}

/*****************************************************************************/
//  Description : handle get focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordGetFocus(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(record_win_data_ptr->is_insert_record
            && (0 != record_win_data_ptr->max_file_size
                || 0 != record_win_data_ptr->max_file_time))
    {
        if(record_win_data_ptr->is_record_work_paused)
        {
            SCI_TRACE_LOW("[MMIRECORD] Main Win Get Focus, cur is work state, will resume record");
            ResumeRecord(win_handle);
            record_win_data_ptr->is_record_work_paused = FALSE;
        }
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        StartDisplayProgressTimer(win_handle);
    }
}

/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeMainWinUserData(MMI_HANDLE_T      win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr)
    {
        SCI_FREE(record_win_data_ptr);
        record_win_data_ptr = PNULL;
    }

}
/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMainWinUserData(MMIRECORD_WORK_PARAM_T *work_param_ptr,
                                    MMI_HANDLE_T      win_handle)
{
    RECORD_WIN_DATA_T  *record_win_data_ptr = PNULL;

    record_win_data_ptr = SCI_ALLOCA(sizeof(RECORD_WIN_DATA_T));

    if(PNULL != record_win_data_ptr)
    {
        SCI_MEMSET(record_win_data_ptr, 0, sizeof(RECORD_WIN_DATA_T));
        record_win_data_ptr->timer_id = 0;
        record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD] = MMIRECORD_BUTTON_STATE_FOCUS;
        record_win_data_ptr->is_record_work_paused = FALSE;

        record_win_data_ptr->record_play_anim.cur_frame  = 0;

        record_win_data_ptr->record_play_anim.total_frame = RECORD_ANIMAL_TOTAL_FRAME_NUM;
#ifdef MMI_RECORD_MINI_SUPPORT
        record_win_data_ptr->record_play_anim.bg.bg_type = GUI_BG_IMG;
        record_win_data_ptr->record_play_anim.bg.img_id = IMAGE_NULL;
#endif

        if(PNULL != work_param_ptr)
        {
            record_win_data_ptr->max_file_size = work_param_ptr->max_file_size;
            record_win_data_ptr->max_file_time = work_param_ptr->max_file_time;
            record_win_data_ptr->record_all_support_type = work_param_ptr->record_support_type;
            record_win_data_ptr->record_support_type = MMIRECORD_DecideRecordWorkFormat(work_param_ptr->record_support_type);
            record_win_data_ptr->is_insert_record = TRUE;
            if(record_win_data_ptr->record_support_type != MMIRECORD_GetRecordFileFormatType())
            {
                MMIRECORD_SetRecordFileFormatType(record_win_data_ptr->record_support_type);
            }
        }
        else
        {
            record_win_data_ptr->record_support_type = MMIRECORD_GetRecordFileFormatType();
        }

        record_win_data_ptr->record_play_anim.win_id  = win_handle;
        record_win_data_ptr->win_handle = win_handle;
        MMK_SetWinUserData(win_handle, record_win_data_ptr);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description :MMIRECORD_OpenRecordMainWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  MMI_HANDLE_T  MMIRECORD_OpenRecordMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr)
{
    MMI_HANDLE_T      win_handle = 0;

    win_handle = MMK_CreateWinByApplet(SPRD_RECORD_APPLET_ID, (uint32*)MMIRECORD_MAINPLAY_WIN_TAB, PNULL);

    if(0 != win_handle)
    {
        CreateMainWinUserData(work_param_ptr, win_handle);
    }

    return win_handle;
}

/*****************************************************************************/
//  Description : Close Menu
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_CloseMenu(void)
{
    MMK_CloseWin(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID);
}
/*Edit by script, ignore 2 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527

#ifdef WIN32
/*****************************************************************************/
//  Description : Get simulator record second time
//  Return : record time
//  Para : none
//  Author : jiaxin.sun
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_GetSimulatorRecordSecondTime(void)
{
    return g_record_cur_timer_elapsed;
}
#endif

/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
