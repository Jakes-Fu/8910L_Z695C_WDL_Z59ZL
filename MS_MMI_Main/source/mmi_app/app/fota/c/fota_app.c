
#ifdef  FOTA_SUPPORT

#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "mmisrvfmm_export.h"
#include "mmifmm_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmi_resource.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmicom_banner.h"
#include "fota_api.h"
#include "fota_header.h"
#include "fota_nv.h"
#include "mmifota_text.h"

#define CHECK_VERSION_TIMER (60 * 1000)

LOCAL uint8 s_fota_check_ver_timer = 0;
LOCAL BOOLEAN s_b_checking = FALSE;
LOCAL uint8 s_fota_update_starter = 0;
LOCAL uint8 s_fota_starter_retry = 0;
LOCAL uint8 s_download_retry = 0;

#define FOTA_DL_RETRY_MAX 	(5)
#define FOTA_ST_RETRY_MAX 	(5)

const LOCAL uint16 g_tfload_dir_wstr[] = { 'b', 'l', '_','u', 'p','d', 'a' ,'t', 'e', 0 };
const LOCAL uint16 g_tfload_apn_wstr[] = { MMIFILE_SLASH, 'a', 'p', 'n', '.' ,'b', 'i', 'n',  0 };
const LOCAL uint16 g_tfload_cnv_wstr[] = { MMIFILE_SLASH, 'c', 'n', 'v', '.' ,'b', 'i', 'n',  0 };
const LOCAL uint16 g_tfload_ims_wstr[] = { MMIFILE_SLASH, 'i', 'm', 's', '.' ,'b', 'i', 'n',  0 };
const LOCAL uint16 g_tfload_tog_wstr[] = { MMIFILE_SLASH, 't', 'o', 'g', '.' ,'b', 'i', 'n',  0 };

LOCAL void MMIFOTA_StartVerCheck(void);
LOCAL void MMIFOTA_StopVerCheck(void);

PUBLIC wchar *Fota_GetApnFileName(void) {return g_tfload_apn_wstr;}

PUBLIC wchar *Fota_GetCnvFileName(void) {return g_tfload_cnv_wstr;}

PUBLIC wchar *Fota_GetImsFileName(void) {return g_tfload_ims_wstr;}

PUBLIC wchar *Fota_GetToggleFileName(void) {return g_tfload_tog_wstr;}


PUBLIC wchar *Fota_GetUpdatePacPath(void)
{
    static wchar s_update_path[32];
    wchar *sd_path = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD); 

    s_update_path[0] = *sd_path;
    s_update_path[1] = MMIFILE_COLON;
    s_update_path[2] = MMIFILE_SLASH;
    s_update_path[3] = 0;

    MMIAPICOM_Wstrcat(s_update_path, g_tfload_dir_wstr);

    return s_update_path;
}

LOCAL void ShowBannerTip(MMI_TEXT_ID_T tid)
{
    MMICOM_BANNER_INFO_T banner_info = {0};

    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point.x = 0;
    banner_info.start_point.y = 0;
    banner_info.text1_id = tid;

    MMIAPICOM_OpenBannerWin(MAIN_IDLE_WIN_ID, banner_info);
}

LOCAL void Fota_SaveNV(void)
{
    uint32 tm;
    uint32 v;
    uint32 io;
    uint8 flag = 1;

    v = Fota_GetVersion();
    io = Fota_GetOperatorIndex();
    tm = TM_GetTotalSeconds();

    //Last Time
    SCI_TraceLow("[FOTA Update] finish -- write time [%d]", tm);
    MMI_WriteNVItem(MMINV_FOTA_LAST_TIME, &tm);
    //Operator index
    SCI_TraceLow("[FOTA Update] finish -- write operator [%d]", io);
    MMI_WriteNVItem(MMINV_FOTA_OPERATOR_IDX, &io);
    //Version
    SCI_TraceLow("[FOTA Update] finish -- write version [%d]", v);
    MMI_WriteNVItem(MMINV_FOTA_VERSION, &v);
    //imsi
    MMIFOTA_NVSaveIMSI();
    EFS_WriteItemAlways( FOTA_UPDATE_FLAG, sizeof(uint8), (uint8 *)&flag);
}

LOCAL void Fota_Local_Update(void)
{
    MN_PLMN_T carrier_plmn[MMI_DUAL_SYS_MAX] = {0};

    if (MMIFOTA_CheckApn())
    {
        MMIFOTA_UpdateApn();
        SCI_MEMSET(carrier_plmn, 0x00, sizeof(carrier_plmn));
        SCI_TRACE_LOW("[szp]memsetAutoAdapting.");
        MMINV_WRITE(MMINV_CARRIER_PLMN, carrier_plmn);
    }

    if (MMIFOTA_CheckConfigNV())
    {
        MMIFOTA_UpdateConfigNV();
    }

    if (MMIFOTA_CheckIMS())
    {
        MMIFOTA_UpdateIMS();
        MNPHONE_ResetImsDelta();
    }

    if (MMIFOTA_CheckToggle())
    {
        MMIFOTA_UpdateToggle();
    }
}

LOCAL MMI_RESULT_E  Fota_HandleConfirmRebootWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
            break;
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                MMK_CloseWin(win_id);
                // reboot to update the pac
                POWER_Reset();
            }
            break;

        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            //MMIFILE_DeleteDirectory(Fota_GetUpdatePacPath());
            //MMIFOTA_StopVerCheck();
            //s_b_checking = FALSE;
            return TRUE;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return recode;
}

LOCAL void download_rel(FOTA_SEVER_RESULT result);

LOCAL  void dl_retryCB(uint8 timer_id, uint32 param)
{
    MMIFOTA_DownLoad_Package(download_rel);
    SCI_TraceLow("[FOTA Update]  -- retry down load <%d> times", s_download_retry);
    MMK_StopTimer(timer_id);
}

LOCAL void download_rel(FOTA_SEVER_RESULT result)
{
    if (result == FOTA_RETURN_OK)
    {
        Fota_SaveNV();
        // .. start ..
        MMIAPISET_SetFlyMode(TRUE);
        MMIFOTA_Set_Upate_Begin();
        // .. update .. 
        Fota_Local_Update();
        // .. end ..
        MMIFOTA_Set_Upate_Finish();
        MMIAPISET_SetFlyMode(FALSE);

        MMIFOTA_Update_Finish();

        MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_OTA_SETTINGS_UPD, STR_OTA_REBOOT_DEV,
            IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
            Fota_HandleConfirmRebootWinMsg);
        ShowBannerTip(STR_OTA_UPD_LATEST_SETTINGS);
        s_b_checking = FALSE;
    }
    else if (result == FOTA_GET_URL_REQ_ERR)
    {
        if (s_download_retry >= FOTA_DL_RETRY_MAX)
        {
            MMIFOTA_Update_Finish();
            ShowBannerTip(STR_OTA_FAIL_UPD);
            s_b_checking = FALSE;
	     s_download_retry = 0;
        }
        else
        {
	     s_download_retry++;
	     MMK_CreateTimerCallback(1000, dl_retryCB, PNULL, FALSE);
        }
    }
    else
    {
        MMIFOTA_Update_Finish();
        s_b_checking = FALSE;

        ShowBannerTip(STR_OTA_FAIL_UPD);
    }
}

LOCAL void Fota_PrepareUpdateDir(void)
{
    wchar *p_path, f_buff[128];

    p_path = Fota_GetUpdatePacPath();
    if (!MMIAPIFMM_IsFolderExist(p_path, MMIAPICOM_Wstrlen(p_path)))
    {
        // make dir 'bl_update'
        if (MMIAPIFMM_CreateDirectory(p_path) != 0)
        {
            ShowBannerTip(STR_OTA_FAIL_UPD);
        }
    }
    else
    {
        // remove apn.bin
        MMIAPICOM_Wstrcpy(f_buff, p_path);
        MMIAPICOM_Wstrcat(f_buff, Fota_GetApnFileName());
        if (MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
        {
            MMIAPIFMM_DeleteFileSyn(f_buff, MMIAPICOM_Wstrlen(f_buff));
        }
        // remove cnv.bin
        MMIAPICOM_Wstrcpy(f_buff, p_path);
        MMIAPICOM_Wstrcat(f_buff, Fota_GetCnvFileName());
        if (MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
        {
            MMIAPIFMM_DeleteFileSyn(f_buff, MMIAPICOM_Wstrlen(f_buff));
        }
        // remove ims.bin
        MMIAPICOM_Wstrcpy(f_buff, p_path);
        MMIAPICOM_Wstrcat(f_buff, Fota_GetImsFileName());
        if (MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
        {
            MMIAPIFMM_DeleteFileSyn(f_buff, MMIAPICOM_Wstrlen(f_buff));
        }
        // remove tog.bin
        MMIAPICOM_Wstrcpy(f_buff, p_path);
        MMIAPICOM_Wstrcat(f_buff, Fota_GetToggleFileName());
        if (MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
        {
            MMIAPIFMM_DeleteFileSyn(f_buff, MMIAPICOM_Wstrlen(f_buff));
        }
    }
}

LOCAL MMI_RESULT_E  Fota_HandleConfirmUpdateWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 close_timer = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
            close_timer = MMK_CreateWinTimer(win_id, 30*1000, FALSE);
            SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- create timer [%d]", close_timer);
            break;
	case MSG_CLOSE_WINDOW:
            SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- close window");
            if (close_timer != 0)
            {
                MMK_StopTimer(close_timer);
                close_timer = 0;
            }
            break;
        case MSG_TIMER:
            SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- timer msg [%d]", *(uint8 *)param);
            if (close_timer == *(uint8 *)param)
            {
                MMK_CloseWin(win_id);
                MMIFOTA_Update_Finish();
                s_b_checking = FALSE;
            }
            break;
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            MMK_CloseWin(win_id);

            if(!MMISD_IsSDExistEx())
            {
                SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- no sd card");
                MMIPUB_OpenAlertWarningWin (TXT_PLEASE_INSERT_SD);
                MMIFOTA_Update_Finish();
                s_b_checking = FALSE;
            }
            else if (MMIAPIUDISK_UdiskIsRun()) //U≈Ã π”√÷–
            {
                SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- udisk running");
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                MMIFOTA_Update_Finish();
                s_b_checking = FALSE;
            }
            else
            {
                Fota_PrepareUpdateDir();
                SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- down load");
                s_download_retry = 0;
                MMIFOTA_DownLoad_Package(download_rel);
            }
            break;

        case MSG_APP_CANCEL:
            SCI_TraceLow("[FOTA Update] Fota_HandleConfirmUpdateWinMsg -- user cancel");
            MMK_CloseWin(win_id);
            MMIFOTA_Update_Finish();
            s_b_checking = FALSE;
            return TRUE;

        case MSG_APP_RED:
        case MSG_KEYUP_RED:
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return recode;
}

LOCAL void get_url_rel(FOTA_SEVER_RESULT result)
{
    if (result == FOTA_RETURN_OK)
    {
        // There is new version
        // Do you want to download it ?
        SCI_TraceLow("[FOTA Update] get_url_rel -- result = FOTA_RETURN_OK");
        MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_OTA_SETTINGS_UPD, STR_OTA_UPD_AVL,
            IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
            Fota_HandleConfirmUpdateWinMsg);
    }
    else
    {
        // get error,
        // stop http reques
        SCI_TraceLow("[FOTA Update] get_url_rel -- error ----update finish!");
        MMIFOTA_Update_Finish();
        s_b_checking = FALSE;
    }
}

LOCAL void check_version_rel(FOTA_SEVER_RESULT result)
{
    SCI_TraceLow("[FOTA Update] check_version_rel -- result = %d", result);
    MMIFOTA_StopVerCheck();

    if (result == FOTA_RETURN_OK)
    {
        SCI_TraceLow("[FOTA Update] check version ok  --to get url");
        MMIFOTA_Get_URL(get_url_rel);
    }
    else
    {
        // check error,
        // stop http reques
        MMIFOTA_Update_Finish();
        s_b_checking = FALSE;

	//TO Retry Check ??
	//MMIFOTA_StartVerCheck();
    }
}

LOCAL void MMIFOTA_CheckVerCB(uint8 timer_id,uint32 param)
{
    SCI_TraceLow("[FOTA Update] Timer -- MMIFOTA_CheckVerCB");

    if (!s_b_checking)
    {
        s_b_checking = TRUE;
        MMIFOTA_Check_Version(check_version_rel);
    }
}

LOCAL void MMIFOTA_StopVerCheck(void)
{
    SCI_TraceLow("[FOTA Update] Timer -- MMIFOTA_StopVerCheck");
    MMK_StopTimer(s_fota_check_ver_timer);
    s_fota_check_ver_timer = 0;
}

LOCAL void MMIFOTA_StartVerCheck(void)
{
    SCI_TraceLow("[FOTA Update] Timer -- MMIFOTA_StartVerCheck");
    if (s_fota_check_ver_timer == 0)
        s_fota_check_ver_timer = MMK_CreateTimerCallback(CHECK_VERSION_TIMER, MMIFOTA_CheckVerCB, PNULL,FALSE);
}

LOCAL  BOOLEAN MMIFOTA_OpenPDP(void)
{
    SCI_TraceLow("[FOTA Update] MMIFOTA_OpenPDP");

    if (Fota_SM_Get() == FOTA_SM_NULL)
    {
        if (!MMIFOTA_ConnectPDP())
        {
            SCI_TraceLow("[FOTA Update] MMIFOTA_OpenPDP -- fail");
            return FALSE;
        }
    }
    MMIFOTA_StartVerCheck();
    return TRUE;
}

LOCAL  void MMIFOTA_StarterCB(uint8 timer_id, uint32 param)
{
    SCI_TraceLow("[FOTA Update] Timer -- MMIFOTA_StarterCB");
    MMK_StopTimer(s_fota_update_starter);
    s_fota_update_starter = 0;

    if (!MMIFOTA_OpenPDP())
    {
	 if (s_fota_starter_retry++ < FOTA_ST_RETRY_MAX)
	 {
            SCI_TraceLow("[FOTA Update] Timer -- Retry to Start PDP [retry : %d times]", s_fota_starter_retry);
            s_fota_update_starter = MMK_CreateTimerCallback(30 * 1000, MMIFOTA_StarterCB, PNULL, FALSE);
	 }
	 else
	 {
	     SCI_TraceLow("[FOTA Update] Timer -- Start PDP [final fail !!!]");
	 }
    }
}

PUBLIC void MMIFOTA_Start(void)
{
    uint32 last_chk_tm = 0;
    uint32 cur_tm = 0;
    BOOLEAN is_enable;
    BOOLEAN is_sim_changed;

    if (!MMIAPIPHONE_IsSimAvailable(FOTA_UPDATE_SIM))
    {
        SCI_TraceLow("[FOTA Update] MMIFOTA_Start, --- stop --- [Sim 0 no exist or not ready]");
        return;
    }

    if (MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_FOTA_ENABLE, &is_enable))
    {
#if !defined (RELEASE_INFO) || defined(DEBUG_FOR_USER_SUPPORT) 
        is_enable = FALSE;
#else
	  is_enable = TRUE;
#endif
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            is_enable = FALSE;
        }
        MMI_WriteNVItem(MMINV_FOTA_ENABLE, &is_enable);
    }

    SCI_TraceLow("[FOTA Update] MMIFOTA_Start, %s", is_enable ? "enable" : "disable");

    if (!is_enable)
    {
        return;
    }

    cur_tm = TM_GetTotalSeconds();

    is_sim_changed = MMIFOTA_NVIsIMSIChanged();

    if (0 != MMI_ReadNVItem(MMINV_FOTA_LAST_TIME, &last_chk_tm))
    {
        last_chk_tm = 0;
    }

    SCI_TraceLow("[FOTA Update] MMIFOTA_Start, sim changed = %s", is_sim_changed ? "TRUE" : "FALSE");

    SCI_TraceLow("[FOTA Update] MMIFOTA_Start, cur time : %d , last_chk_tm = %d", cur_tm, last_chk_tm);

    if (is_sim_changed || last_chk_tm == 0 || cur_tm < last_chk_tm)
    {
        // haven't cheked, to check right now.
        SCI_TraceLow("[FOTA Update] MMIFOTA_Start, <Start right now>");
        s_fota_starter_retry = 0;
        s_fota_update_starter = MMK_CreateTimerCallback(30 * 1000, MMIFOTA_StarterCB, PNULL, FALSE);
    }
    else
    {
        uint8 period = 0;
        uint32 nex_chk_tm = 0;

        if (MN_RETURN_SUCCESS !=  MMI_ReadNVItem(MMINV_FOTA_CHECK_PERIOD, &period))
        {
            period = CHECK_PER_MONTH;
            MMI_WriteNVItem(MMINV_FOTA_CHECK_PERIOD, &period);
        }

        switch(period)
        {
            case CHECK_PER_DAY:
                nex_chk_tm = last_chk_tm + (24 * 60 * 60);
                break;
            case CHECK_PER_WEEK:
                nex_chk_tm = last_chk_tm + (7 * 24 * 60 * 60);
                break;
            case CHECK_PER_MONTH:
                nex_chk_tm = last_chk_tm + (30 * 24 * 60 * 60);
                break;
            default:
                nex_chk_tm = last_chk_tm + (24 * 60 * 60);
                break;
        }
        SCI_TraceLow("[FOTA Update] MMIFOTA_Start, period = %d, <Start at %d hours later>", period, (nex_chk_tm - cur_tm) / 60 / 60);

        if (nex_chk_tm != 0)
        {
            s_fota_starter_retry = 0;
            s_fota_update_starter = MMK_CreateTimerCallback((nex_chk_tm - cur_tm) * 1000, MMIFOTA_StarterCB, PNULL, FALSE);
        }
    }

    //MMI_WriteNVItem(MMINV_FOTA_LAST_TIME, &cur_tm);
}

PUBLIC void MMIFOTA_ReStart(void)
{
    if (s_b_checking)
        return;

    MMK_StopTimer(s_fota_update_starter);
    MMIFOTA_Start();
}

PUBLIC void MMIFOTA_Repair(void)
{
    if (!MMIFOTA_Is_Upate_Finish())
    {
        // fota upate not finish, to repair it
        SCI_TraceLow("[FOTA Update] +++++++++ MMIFOTA_Repair ++++++++++");
        // .. start ..
        MMIFOTA_Set_Upate_Begin();
        // .. update .. 
        Fota_Local_Update();
        // .. end ..
        MMIFOTA_Set_Upate_Finish();

        // Fota Update be interrupted , the phone must be fly mode
        // After repair , should be close the fly mode
        MMIAPISET_SetFlyMode(FALSE);
        SCI_TraceLow("[FOTA Update] +++++++++ MMIFOTA_Repair +++++ FINISH ++");
    }
}

PUBLIC void Fota_LongTrace(const char *string, int len)
{
    if (len < 128)
    {
        SCI_TraceLow("[FOTA Update] :%s ",string);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[FOTA Update] :%s ",buff);
        
          p += l;
          len -= l;
        }
    }
}
#endif
