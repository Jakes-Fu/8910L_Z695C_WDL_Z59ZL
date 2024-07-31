/*****************************************************************************
 **                         Important Edit History                            *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/01/2019     sky.wang         Create
 ******************************************************************************/

#define _MMISET_UPDATE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmi_resource.h"
#include "mmi_appmsg.h"
#include "mmiset_id.h"
#include "mmiset_text.h"
#include "mmiset_about.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_func.h"
#include "mmiset_export.h"
#include "mmiset_internal.h"
#include "mmiset_update.h"
#include "mmisrvfmm_export.h"
#include "mmicom_banner.h"
#include "mmimp3_text.h"

#define SCAN_TIMER_SEC 1000
#define UPDATE_TIMER_SEC 1000

typedef struct _FILE_T
{
    DWORD  dwSize;              // size of this struct itself
    wchar  szFileID[256];        // file ID,such as FDL,Fdl2,NV and etc.
    wchar  szFileName[256];     // file name,in the packet bin file,it only stores file name
                                // but after unpacketing, it stores the full path of bin file
    wchar  szFileVersion[256];  // Reserved now
    int    nFileSize;           // file size
    int    nFileFlag;           // if "0", means that it need not a file, and
                                			// it is only an operation or a list of operations, such as file ID is "FLASH"
                                			// if "1", means that it need a file
    DWORD  nCheckFlag;          // if "1", this file must be downloaded;
                                			// if "0", this file can not be downloaded;
    DWORD  dwDataOffset;        // the offset from the packet file header to this file data
    DWORD  dwCanOmitFlag;       // if "1", this file can not be downloaded and not check it as "All files"
                                //   in download and spupgrade tool.
    DWORD  dwReserved[255];     // Reserved for future,not used now
}FILE_T, *PFILE_T;

typedef struct _BIN_PACKET_HEADER_T
{
    wchar  szVersion[24];        // packet struct version
    DWORD  dwSize;              // the whole packet size;
    wchar  szPrdName[256];       // product name
    wchar  szPrdVersion[256];   // product version
    int    nFileCount;          // the number of files that will be downloaded, the file may be an operation
    DWORD  dwFileOffset;        // the offset from the packet file header to the array of FILE_T struct buffer
    DWORD  dwMode;
    DWORD  dwFlashType;
    DWORD  dwNandStrategy;
    DWORD  dwIsNvBackup;
    DWORD  dwNandPageType;
    wchar  szPrdAlias[100];    // product alias
    DWORD  dwOmaDmProductFlag;
    DWORD  dwIsOmaDM;
    DWORD  dwIsPreload;
    DWORD  dwReserved[202];
}BIN_PACKET_HEADER_T,*PBIN_PACKET_HEADER_T;

typedef enum {
    NO_SCAN = 0,
    SCANNING,
    SCAN_FINISHED
} SCAN_STATUS;

LOCAL uint8 s_timer_id = 0; 
LOCAL SCAN_STATUS s_scan_status = NO_SCAN; 
LOCAL uint8 s_pac_selected = 0;
LOCAL FILEARRAY s_locallist_array_ptr = PNULL;

LOCAL MMI_RESULT_E HandleNoPacDlgWin(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            SCI_TraceLow("[SKY-MMISET] HandleNoPacDlgWin <MSG_OPEN_WINDOW>");
            break;
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(MMISET_UPDATE_SCAN_SD_WIN_ID);
            MMK_CloseWin(MMISET_UPDATE_PAC_LIST_WIN_ID);
            s_scan_status = SCAN_FINISHED;
            break;
        default :
            break;
    }
    return MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
}

LOCAL MMI_RESULT_E ScanUpdatePackage(MMI_WIN_ID_T  win_id,  MMI_MESSAGE_ID_E msg_id)
{
    MMIFMM_FILTER_T filter = {0};
    wchar scan_path[8] = {0};

    if(PNULL == s_locallist_array_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    strcpy(&filter.filter_str, "*.pac ");

    scan_path[0] = *MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
    scan_path[1] = MMIFILE_COLON;
    scan_path[2] = '\0';

    MMIAPIFMM_SearchFileInPath(
            scan_path, 
            2,
            &filter,
            FALSE,
            FUNC_FIND_FILE,
            s_locallist_array_ptr,
            win_id,
            msg_id );

    return MMI_RESULT_TRUE;
}

LOCAL BOOLEAN IsPacFileValidate(int index);

LOCAL int PackageFilter(void)
{
    int i;
    int count = MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr);

    if (count <= 0)
        return 0;

    SCI_TraceLow("[SKY-MMISET] PacFile count = %d", count);

    for (i = 0; i < count; )
    {
        if (!IsPacFileValidate(i))
        {
            MMIAPIFILEARRAY_Delete(s_locallist_array_ptr, i);
            count--;
        }
	 else 
	 {
	     i++;
	 }
    }

    SCI_TraceLow("[SKY-MMISET] After filter PacFile count = %d", MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr));

    return count;
}

LOCAL MMI_RESULT_E  HandleSDScanningWin( SET_WINDOW_ID_E win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            SCI_TraceLow("[SKY-MMISET] HandleSDScanningWin <MSG_OPEN_WINDOW>");

            s_timer_id = MMK_CreateWinTimer(win_id, SCAN_TIMER_SEC, TRUE);
            MMK_StartWinTimer(win_id, s_timer_id, SCAN_TIMER_SEC, TRUE);

            if (PNULL != s_locallist_array_ptr)
            {
                MMIAPIFILEARRAY_Destroy(s_locallist_array_ptr);
            }
            s_locallist_array_ptr = MMIAPIFILEARRAY_Create();  

            s_scan_status = SCANNING;
            ScanUpdatePackage(win_id, MSG_FMM_SEARCH_CNF);
            break;

        case MSG_TIMER:
            SCI_TraceLow("[SKY-MMISET] HandleSDScanningWin <MSG_TIMER> s_scan_status = %d", s_scan_status);
            if (s_scan_status == SCAN_FINISHED)
            {
                MMK_StopTimer(s_timer_id);
                s_timer_id = 0;
                MMIPUB_CloseWaitWin(win_id);
            }
            break;

        case MSG_FMM_SEARCH_CNF:
            SCI_TraceLow("[SKY-MMISET] HandleSDScanningWin :: scan finish ... [package = %d]", MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr));
            if (MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr) == 0)
            {
                MMIPUB_CloseWaitWin(win_id);
                MMIPUB_OpenTextDialogByTextId(PNULL, STR_TOAST_NO_UPD_AVAIL, PNULL, PNULL, NULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK,HandleNoPacDlgWin);
            }
            else
            {
                if (PackageFilter() == 0)
                {
                    MMIPUB_CloseWaitWin(win_id);
                    MMIPUB_OpenTextDialogByTextId(PNULL, STR_TOAST_NO_UPD_AVAIL, PNULL, PNULL, NULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK,HandleNoPacDlgWin);
                    break;
                }
                s_scan_status = SCAN_FINISHED;
            }
            break;
        case MSG_CLOSE_WINDOW:
            SCI_TraceLow("[SKY-MMISET] HandleSDScanningWin <MSG_CLOSE_WINDOW>");
            if (s_timer_id != 0)
            {
                MMK_StopTimer(s_timer_id);
                s_timer_id = 0;
            }
            break;
        case MSG_APP_CANCEL:
            MMIAPIFMM_SearchFileStop();
            if (MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr) == 0)
            {
                MMK_SendMsg(MMISET_UPDATE_PAC_LIST_WIN_ID,MSG_APP_CANCEL,NULL);
            }
            else
            {
                
                PackageFilter();
                s_scan_status = SCAN_FINISHED;
            }
            
            MMIPUB_CloseWaitWin(win_id);
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
            return MMI_RESULT_TRUE;
        default :
            break;
    }
    return MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
}

PUBLIC void MMISET_CreateScanWaitingWin(void)
{
    MMI_WIN_ID_T  waiting_win_id = MMISET_UPDATE_SCAN_SD_WIN_ID;

    SCI_TraceLow("[SKY-MMISET] MMISET_CreateScanWaitingWin");

    MMIPUB_OpenProgressWaitingIndWinByTextId(STR_NOTE_SCAN_SD, &waiting_win_id, PNULL, MMIPUB_SOFTKEY_ONE, HandleSDScanningWin);
}

LOCAL void UpdatePacList (MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    int i;
    int count = MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr);

    SCI_TraceLow("[SKY-MMISET] UpdatePacList count = %d", count);

    GUILIST_SetTextListMarkable (ctrl_id, FALSE);
    GUILIST_RemoveAllItems (ctrl_id);

    if (count > 0)
    {
        GUILIST_SetMaxItem (ctrl_id, count, FALSE);

        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_PRGBAR, FALSE);

        for (i = 0; i < count; i++)
        {
            int file_name_i, list_name_len;
            GUILIST_ITEM_T       item_t    =  {0};
            GUILIST_ITEM_DATA_T  item_data = {0};
            FILEARRAY_DATA_T          file_array_data = {0};
            uint8    date_tmp[GUILIST_STRING_MAX_NUM] = {0};
            wchar  wchar_date_tmp[GUILIST_STRING_MAX_NUM + 1] = {0};
            wchar size_tmp[GUILIST_STRING_MAX_NUM + 1] = {0};
            wchar name_tmp[GUILIST_STRING_MAX_NUM + 1] = {0};

            SCI_MEMSET(date_tmp, 0, GUILIST_STRING_MAX_NUM);
            SCI_MEMSET(wchar_date_tmp, 0, (GUILIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(size_tmp, 0, (GUILIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(name_tmp, 0, (GUILIST_STRING_MAX_NUM + 1) * sizeof(wchar));

            MMIAPIFILEARRAY_Read(s_locallist_array_ptr, i, &file_array_data );

            item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS;
            item_t.left_item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_WITH_LEFT_FOCUS_MS;
            item_t.item_data_ptr = &item_data;

            file_name_i = MMIFMM_GetFinallySlashIndex (file_array_data.filename, (file_array_data.name_len - 1));
            list_name_len = (uint16) (file_array_data.name_len - 1 - file_name_i);
            list_name_len = MIN(list_name_len, GUILIST_STRING_MAX_NUM);
            MMIAPICOM_Wstrncpy (name_tmp,  &file_array_data.filename[file_name_i + 1], list_name_len);
            name_tmp[list_name_len] = 0;

            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = list_name_len + 1;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = name_tmp;

            MMIAPISET_FormatDateStrByDateDisplyType (file_array_data.create_date.year, 
                    file_array_data.create_date.mon, file_array_data.create_date.mday, 
                    '-', date_tmp, GUILIST_STRING_MAX_NUM, MMISET_DATE_YMD);
            MMIAPICOM_StrToWstr (date_tmp, wchar_date_tmp);

            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = wchar_date_tmp;
            item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (wchar_date_tmp) + 1;

            MMIAPIFMM_GetFileSizeString (file_array_data.size, size_tmp, MMIFMM_SIZE_STR_LEN, TRUE);
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = size_tmp;
            item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (size_tmp) + 1;

            GUILIST_AppendItem (ctrl_id, &item_t);
        }
    }
}

LOCAL void ShowBannerTip(MMI_TEXT_ID_T tid)
{
    MMICOM_BANNER_INFO_T banner_info = {0};

    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point.x = 0;
    banner_info.start_point.y = 0;
    banner_info.text1_id = tid;

    MMIAPICOM_OpenBannerWin(MMISET_UPDATE_PAC_LIST_WIN_ID, banner_info);
}

const static uint16 g_tfload_dir_wstr[] = { 'b', 'l', '_','u', 'p','d', 'a' ,'t', 'e',0 };
const static uint16 g_tfload_file_wstr[] = { 't', 'f', 'l','o', 'a','d', '.' ,'p', 'a', 'c', 0 };

LOCAL wchar *GetUpdatePacPath(void)
{
    static wchar s_update_path[32];

    s_update_path[0] = *MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
    s_update_path[1] = MMIFILE_COLON;
    s_update_path[2] = '\\';
    s_update_path[3] = 0;

    MMIAPICOM_Wstrcat(s_update_path, g_tfload_dir_wstr);

    return s_update_path;
}

LOCAL wchar *GetUpdatePacFileName(void)
{
    return g_tfload_file_wstr;
}

LOCAL wchar *GetUpdatePacFullName(void)
{
    static wchar s_update_full_path[64];

    MMIAPICOM_Wstrcpy(s_update_full_path, GetUpdatePacPath());
    MMIAPICOM_Wstrcat(s_update_full_path, L"\\");
    MMIAPICOM_Wstrcat(s_update_full_path, GetUpdatePacFileName());

    return s_update_full_path;
}
#if 0
LOCAL void EraseNVItemByModule(uint16 module_id)
{
    uint16 begin_nv_id;
    uint16 nv_count = 0;

    begin_nv_id = MMI_GetTrueNVItemId(module_id << 16);
    nv_count = MMI_GetModuleNVItemCount(module_id);

    EFS_NvitemDelete(begin_nv_id, begin_nv_id + nv_count);
}
#endif

LOCAL void SDUpdateEraseNVItem(uint16 nv_id)
{
    EFS_FreeItem(MMI_GetTrueNVItemId(nv_id));
}

LOCAL MMI_RESULT_E  HandleRestartWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                MMK_CloseWin(win_id);
                //EraseNVItemByModule(MMI_MODULE_COMMON);
                //EraseNVItemByModule(MMI_MODULE_SET);
                SDUpdateEraseNVItem(MMINV_MAINMENU_MENU_ID_ARRAY);
                MNPHONE_ResetImsDelta();
                POWER_UpdateReset();
            }
            break;
        case MSG_APP_RED:
        case MSG_KEYUP_RED:
	    break;
        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return recode;
}

LOCAL MMI_RESULT_E  HandleSDUpdateProgressWin( SET_WINDOW_ID_E win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    static int s_update_status = 0;
    wchar *p_path;
    FILEARRAY_DATA_T file_array_data = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                // TODO
                // copy the select pac file 
                // to bl_update/tfload.pac
                // and restart the phone to tf update mode
                SCI_TraceLow("[SKY-MMISET] HandleSDUpdateProgressWin <MSG_OPEN_WINDOW>");
                s_update_status = 0;
                s_timer_id = MMK_CreateWinTimer(win_id, UPDATE_TIMER_SEC, TRUE);
                MMK_StartWinTimer(win_id, s_timer_id, UPDATE_TIMER_SEC, TRUE);
                MMIPUB_SetProgressTotalSize(&win_id, 100);
                break;
            }
        case MSG_CLOSE_WINDOW:
            SCI_TraceLow("[SKY-MMISET] HandleSDUpdateProgressWin <MSG_CLOSE_WINDOW>");
            if (s_timer_id != 0)
            {
                MMK_StopTimer(s_timer_id);
                s_timer_id = 0;
            }
            break;
        case MSG_TIMER:
	     SCI_TraceLow("[SKY-MMISET] HandleSDUpdateProgressWin <MSG_TIMER> s_update_status = %d", s_update_status);

            switch(s_update_status)
            {
                case 0: //check dir and remove old file
                    p_path = GetUpdatePacFullName();
                    if (MMIAPIFMM_IsFileExist(p_path, MMIAPICOM_Wstrlen(p_path)))
                    {
                        if (MMIAPIFMM_DeleteFile(p_path, PNULL) != 0)
                        {
                            ShowBannerTip(TXT_UPDATE_FAIL);
                            MMK_CloseWin(win_id);
                            return TRUE;
                        }
                    }
                    MMIPUB_SetProgressWinFinishedSize(25);
                    break;
                case 1: //TO mkdir
                    p_path = GetUpdatePacPath();
                    if (!MMIAPIFMM_IsFolderExist(p_path, MMIAPICOM_Wstrlen(p_path)))
                    {
                        if (MMIAPIFMM_CreateDirectory(p_path) != 0)
                        {
                            ShowBannerTip(TXT_UPDATE_FAIL);
                            MMK_CloseWin(win_id);
                            return TRUE;
                        }
                    }
                    MMIPUB_SetProgressWinFinishedSize(50);
                    break;
                case 2: //copy file 
                    p_path = GetUpdatePacFullName();
  
                    if (FALSE == MMIAPIFILEARRAY_Read(s_locallist_array_ptr, s_pac_selected, &file_array_data ))
                    {
                        ShowBannerTip(TXT_UPDATE_FAIL);
                        MMK_CloseWin(win_id);
                        return TRUE;
                    }
                    if (MMIAPIFMM_CopyFile( file_array_data.filename, p_path) != 0)
                    {
                        ShowBannerTip(TXT_UPDATE_FAIL);
                        MMK_CloseWin(win_id);
                        return TRUE;
                    }
                    MMIPUB_SetProgressWinFinishedSize(75);
                    break;
                case 3: //finish
                    MMIPUB_SetProgressWinFinishedSize(100);
                    break;
                default:
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_TITLE_SOFTWARE_UPT, STR_UPDATE_REBOOT_NOTE,
                            IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK,
                            HandleRestartWinMsg);
                    return MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
            }
            /* since stage 2 is to copy file, set SoftKeyBtn to gray before copying */
            if (s_update_status == 1)
            {
                GUIWIN_SetSoftkeyBtnState(win_id, 2, TRUE, FALSE);
            }
            MMIPUB_UpdateProgressWin(win_id);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            s_update_status++;
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
            /* ignore this event during upgrading, or it will trigger MSG_CLOSE_WINDOW in MMIPUB_HandleProgressWinMsg */
            return MMI_RESULT_TRUE;
            break;
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
        case MSG_KEYUP_RED:
            /* if any MSG_APP_CANCEL keys is received after copying file start, ignore it */
            if (s_update_status >= 2)
            {
                return MMI_RESULT_TRUE;
            }
            s_update_status = 0;
            if (s_timer_id != 0)
            {
                MMK_StopTimer(s_timer_id);
                s_timer_id = 0;
            }

            ShowBannerTip(TXT_UPDATE_CANCELLED);

            p_path = GetUpdatePacPath();
            if (!MMIAPIFMM_IsFolderExist(p_path, MMIAPICOM_Wstrlen(p_path)))
            {
                MMIAPIFMM_DeleteDirectory(p_path);
            }
            break;
        default :
            break;
    }
    return MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
}

LOCAL MMI_RESULT_E  HandleConfirmStartWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                MMK_CloseWin(win_id);
                MMIPUB_OpenProgressWinByTextId(STR_WAIT_UPDATING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, HandleSDUpdateProgressWin);
            }
            break;

        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            return TRUE;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return recode;
}

LOCAL void ComfirmToStartOffLineUpdate(void)
{
    MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_TITLE_SOFTWARE_UPT, STR_DIAL_ERASE_USER_DATA,
            IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
            HandleConfirmStartWinMsg);
}

LOCAL BOOLEAN IsPacFileValidate(int index)
{
    //check the pac file
    SFS_HANDLE f_pac = 0;
    uint32 f_size, read_size;
    FILEARRAY_DATA_T file_array_data = {0};
    BIN_PACKET_HEADER_T pac_header;
    BOOLEAN b_ret = TRUE;

    const wchar m_szPacVersion[24] = {'B','P','_','R','1','.','0','.','0','\0'};
    const uint16 m_szPacPrdName[] = { 'N', 'O', 'R', '_', 'F', 'L', 'A', 'S', 'H', '_', 'U','I', 'X', '8', '9', '1', '0', 0 };

    if (FALSE == MMIAPIFILEARRAY_Read(s_locallist_array_ptr, index, &file_array_data ))
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <get file name error>");
        b_ret = FALSE;
        goto ret_err;
    }

    f_pac = SFS_CreateFile((uint16 *)file_array_data.filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);   
    if(0 == f_pac)
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file open error>");
        b_ret = FALSE;
        goto ret_err;
    }

    if (SFS_NO_ERROR != SFS_GetFileSize(f_pac, &f_size) || f_size < sizeof(BIN_PACKET_HEADER_T))
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file size error>");
        b_ret = FALSE;
        goto ret_err;
    }

    if (SFS_NO_ERROR !=  SFS_ReadFile(f_pac, &pac_header, sizeof(pac_header), &read_size, NULL) || read_size != sizeof(pac_header))
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <read file error>");
        b_ret = FALSE;
        goto ret_err;
    }

    if (pac_header.dwSize != f_size)
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file package size error>");
        b_ret = FALSE;
        goto ret_err;
    }

    if (MMIAPICOM_Wstrcmp(pac_header.szVersion, m_szPacVersion) != 0)
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file version error >");
        b_ret = FALSE;
        goto ret_err;
    }

    if(MMIAPICOM_Wstrcmp(pac_header.szPrdName, m_szPacPrdName) !=0 )
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file product name error >");
        b_ret = FALSE;
        goto ret_err;
    }

    if (pac_header.nFileCount <= 0)
    {
        SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file package count error>");
        b_ret = FALSE;
        goto ret_err;
    }

    SCI_TraceLow("[SKY-MMISET] IsPacFileValidate <file %d OK>", index);

ret_err:
    if(f_pac != 0)  SFS_CloseFile(f_pac);

    return b_ret;
}

LOCAL MMI_RESULT_E HandlePacListWindow (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id = MMISET_UPDATE_PAC_LIST_CTRL_ID;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            SCI_TraceLow("[SKY-MMISET] HandlePacListWindow <MSG_OPEN_WINDOW>");
            MMISET_CreateScanWaitingWin();
            {
                MMI_STRING_T  string= {0};
                GUILIST_EMPTY_INFO_T empty_info = {0};
                const wchar U_en[] = {  0x0020, 0 };

                string.wstr_ptr = U_en;
                string.wstr_len = sizeof(U_en)/sizeof(wchar);
                empty_info.text_buffer = string;

                GUILIST_SetEmptyInfo(list_ctrl_id, &empty_info);
            }
            MMK_SetAtvCtrl (win_id, list_ctrl_id);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
            break;
        case MSG_CLOSE_WINDOW:
            if (s_locallist_array_ptr != PNULL)
            {
                MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr);
                s_locallist_array_ptr = PNULL;
            }
            break;
        case MSG_GET_FOCUS:
            if (s_scan_status == SCAN_FINISHED)
            {
                UpdatePacList (win_id, list_ctrl_id);
            }
            break;
        case MSG_CTL_MIDSK:
            {
                s_pac_selected = GUILIST_GetCurItemIndex(list_ctrl_id);
                SCI_TraceLow("[SKY-MMISET] HandlePacListWindow <MSG_CTL_MIDSK>  -- %d", s_pac_selected);

                if (!IsPacFileValidate(s_pac_selected))
                {
                    MMIPUB_OpenAlertTextWinByTextId(PNULL, STR_DIAL_ILLEGAL_FILE, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, PNULL);
                    MMIPUB_SetWinSoftkeyEx( MMIPUB_ALERT_TEXT_WIN_ID, TXT_COMMON_OK, STXT_EXIT, STXT_CANCEL, FALSE );
                    break;
                }
                if (MMIAPIPHONE_GetBatCapacity() < 2 && FALSE == CHGMNG_IsChargeConnect())
                {
                    /* If battery capacity is less than 21% in uncharging state,  display the warning */
                    wchar wstr_num[10] = {0};
                    MMI_STRING_T current_temp = {0};
                    MMI_STRING_T new_tmp = {0};
                    wchar wstr_old[] = {L"N%"};
                    char    num_temp[5] = "21%";

                    MMIAPICOM_StrToWstr((const uint8*)num_temp, wstr_num);
                        
                    MMI_GetLabelTextByLang(STR_DIAL_CHARGE_PHONE_UPD, &current_temp);

                    new_tmp.wstr_len = current_temp.wstr_len + 10;
                    new_tmp.wstr_ptr = SCI_ALLOC_APP(new_tmp.wstr_len * sizeof(wchar));

                    if (new_tmp.wstr_ptr == NULL)
                    {
                        SCI_TraceLow("[SKY-MMISET] HandlePacListWindow memory allocation fail\n");
                        break;
                    }
                    else
                    {
                        SCI_MEMSET(new_tmp.wstr_ptr, 0, new_tmp.wstr_len * sizeof(wchar));

                        SCI_MEMCPY(new_tmp.wstr_ptr, current_temp.wstr_ptr, current_temp.wstr_len*sizeof(wchar));

                        MMIAPICOM_WstrReplace(new_tmp.wstr_ptr, sizeof(wchar) *new_tmp.wstr_len, wstr_old, wstr_num);

                        current_temp.wstr_ptr = new_tmp.wstr_ptr;
                        current_temp.wstr_len = new_tmp.wstr_len;
                        
                        MMIPUB_OpenAlertTextWinByTextPtr(PNULL, &current_temp, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK, PNULL);

                        SCI_FREE(new_tmp.wstr_ptr);
                        new_tmp.wstr_ptr = PNULL;
                        
                        break;
                    }
                }
                /* Do not upgrade if free space of SD card is less than 20M bytes */
                if (MMI_GetFreeSpace(MMI_DEVICE_SDCARD) < 20*1024)
                {
                    MMI_TEXT_ID_T storage_text_id = MMIAPIFMM_GetDeviceName (MMI_DEVICE_SDCARD);
                    MMIPUB_OpenAlertWinByTextId (PNULL, storage_text_id, STR_NO_SPACE_EXT01, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK, PNULL);
                    break;
                }
                ComfirmToStartOffLineUpdate();

                break;
            }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

WINDOW_TABLE (MMISET_PAC_LIST_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandlePacListWindow),
    WIN_ID (MMISET_UPDATE_PAC_LIST_WIN_ID),
    WIN_TITLE(STR_LIST_SW_UPD_OFFINE),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL (GUILIST_TEXTLIST_E, MMISET_UPDATE_PAC_LIST_CTRL_ID),
    END_WIN
};

PUBLIC BOOLEAN MMISET_EnterSettingUpdateWin(void)
{
    SCI_TraceLow("[SKY-MMISET] MMISET_EnterSettingUpdateWin");

    MMK_CloseWin(MMISET_UPDATE_PAC_LIST_WIN_ID);

    if (MMK_IsOpenWin(MMISET_UPDATE_PAC_LIST_WIN_ID))
    {
        return TRUE;
    }

    if (MMK_CreateWin((uint32 *)MMISET_PAC_LIST_WIN_TAB, PNULL))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
