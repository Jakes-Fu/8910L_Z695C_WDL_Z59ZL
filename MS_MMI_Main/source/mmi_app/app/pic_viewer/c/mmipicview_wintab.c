/*****************************************************************************
** File Name:      mmipicview_wintab.c                                       *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32 
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "window_parse.h"
#include "mmk_type.h"
#include "guilistbox.h"
#include "guiform.h"
#include "guilabel.h"
#include "guirichtext.h"
#include "guidropdownlist.h"
#include "mmipicview_export.h"
#include "mmipicview_internal.h"
#include "mmipicview_nv.h"
#include "mmipicview_id.h"
#include "mmipicview_menutable.h"
#include "mmipicview_position.h"
#include "mmipicview_text.h"
#include "mmipicview_image.h"
#include "mmidisplay_data.h"
#include "mmifilearray_export.h"
#include "mmi_theme.h"
#include "mmitheme_special_func.h"
#include "mmimms_export.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmifmm_export.h"
#include "mmi_default.h"
#include "mmipic_edit_export.h"
#include "mmidc_export.h"
#include "mmidc_setting.h"
#include "mmidc_save.h"
#include "mmi_mainmenu_export.h"
#include "guires.h"
#include "mmk_timer.h"
#include "mmibt_export.h"
#include "guiedit.h"
#include "guistring.h"
#include "guictrl_api.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_wallpaper_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"

#ifdef CMCC_UI_STYLE
#include "mmidl_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif

#ifdef WRE_SUPPORT
#include "mmiwre.h"
#endif
#include "mmicom_banner.h"
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
#include "mmisrv_cameraroll_export.h"
#include "guiiconlist.h"
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#include "mmiwk.h" 
#endif
#include "tasks_id.h"
#include "priority_app.h"
#include "os_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_FILE_SIZE_MAX_LEN    12  //****.**** K

//file suffix max len
#define MMIPICVIEW_DETAIL_STR_MAX_LEN       20
#define MMI_PICVIEW_MINI_SUPPORT TRUE


#define TIMER_DELAY             5000


#define PROGRESS_LEVEL 10
#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef SNS_SUPPORT
static IMMISns  *pMe = PNULL;
#endif
LOCAL BOOLEAN   s_mmipicview_is_preview_del = FALSE;        //is preview delete
LOCAL BOOLEAN   s_mmipicview_del_file_is_cancelled = 0;     //delete file is canceled
LOCAL uint8     s_mmipicview_slide_play_timer_id = 0;       //slide play timer
LOCAL uint16    s_mmipicview_del_file_index = 0;            //delete the picture index
LOCAL BOOLEAN   s_mmipicview_is_del_err = FALSE;        //is preview delete
LOCAL BOOLEAN   s_mmipicview_is_fat_deleteing = FALSE; //is fat deleteing

LOCAL uint8  s_progress_timer = 0;

LOCAL MMI_RESULT_E s_mmipicview_multi_choice_delete = MMI_RESULT_FALSE;
LOCAL MMI_RESULT_E s_mmipicview_multi_choice_share = MMI_RESULT_FALSE;
LOCAL const uint32  s_mmipicview_slide_time[MMIPICVIEW_TIME_TYPE_MAX] = {2000,3000,6000,10000,16000,20000}; //slide time
#ifdef PIC_EDITOR_SUPPORT
LOCAL uint16 s_editpic_index = 0;//add for cr NEWMS00108303
#endif
#ifdef CMCC_UI_STYLE
LOCAL MMIPICVIEW_PATH_TYPE_E s_mmipicview_path_type = MMIPICVIEW_PHOTO;
#endif
//zhuyongjing add  for gallary ui 20150707
const uint32 menu_picview_show[][2] = 
{
    {ID_PICVIEW_SHOW_ALLLOCAL,STR_GAL_LIST_ALL_EXT01},
    {ID_PICVIEW_SHOW_CAPTURED,STR_GALLERY_CAPTURED_PHOTOS_MK},
    {ID_PICVIEW_SHOW_DOWNLOAD,STR_GALLERY_DOWNLOADED_EXT01},
    {ID_PICVIEW_SHOW_VIDEO,STR_GAL_LIST_VIDEO_ALBUM},
#ifdef MMI_SHOW_MSA
    {ID_PICVIEW_SHOW_ONEDRIVE,STR_GAL_LIST_ONEDRIVE_EXT01},
#endif
};
LOCAL BOOLEAN   s_mmipicview_save_items_onedrive = FALSE;
LOCAL BOOLEAN s_previous_folder_is_subfolder = FALSE;
LOCAL PIC_SOURCE_TYPE s_mmipicview_show_folder = CAMERA_ROLL_SOURCE_NONE;
LOCAL FILEARRAY     s_uploading_file_array = PNULL; //current path all picture files array
LOCAL uint16    s_mmipicview_current_upload_pic_idx = 0 ;
LOCAL uint16    s_mmipicview_current_download_pic_idx = 0 ;
LOCAL uint32       s_total_mark_num = 0;
LOCAL BOOLEAN s_origial_connect_status = FALSE;
LOCAL uint8        s_wait_ready_timer_id = 0;
LOCAL MMIPICVIEW_DEFAULT_PATH_T   download_path = {0};
static BOOLEAN      s_refresh_preview_anim = TRUE;
static BLOCK_ID g_filearray_task_id = SCI_INVALID_BLOCK_ID;
LOCAL GALLERY_TASK_TYPE_E s_task_cancel = GALLERY_TASK_IDLE;
//add end

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMIPICVIEW_DEFAULT_PATH_T g_mmipicview_dc_path = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle list folder option win msg
//  Global resource dependence : 
//  Author: kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );
#endif
/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

//lijinghua add
LOCAL MMI_RESULT_E HandleShareItemsPiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );
//lijinghua end

//zhuyongjing add
LOCAL void SendPicByEmail(
                       uint32  item_index  //from 0
                       );
LOCAL MMI_RESULT_E HandleDownloadWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
PUBLIC void MMIPICVIEW_EntryOneDriveTip(BOOLEAN isforSetting);
LOCAL void MMIAPIPICVIEW_DeleteAfterSignOff(void);
LOCAL BOOLEAN Multi_Upload_Pics_Signal(void);
LOCAL BOOLEAN Markable_Delete_Filearray_signal(void);
LOCAL BOOLEAN Onedrive_Disconnect_Delete_Filearray_Signal(void);

LOCAL MMI_RESULT_E HandleSharelinkWaitingWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
//add end

// /*****************************************************************************/
// //  Description : handle list preview mark option win msg
// //  Global resource dependence : 
// //  Author: Jassmine
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E HandlePiclistMarkWinMsg(
//                                           MMI_WIN_ID_T        win_id,
//                                           MMI_MESSAGE_ID_E    msg_id,
//                                           DPARAM              param
//                                           );
                                          
/*****************************************************************************/
//  Description : set list opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              );

// /*****************************************************************************/
// //  Description : set list MARK opt menu gray
// //  Global resource dependence : 
// //  Author: Jassmine
// //  Note: 
// /*****************************************************************************/
// LOCAL void SetGrayListMarkMenu(
//                               MMI_CTRL_ID_T     list_ctrl_id
//                               );
#endif

/*****************************************************************************/
//  Description : handle picture slide play win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlidePlayWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlidePlayAnim(
                               uint32           item_index, //from 0
                               MMI_CTRL_ID_T    ctrl_id
                               );
/*****************************************************************************/
//  Description : get pic or video info
//  Global resource dependence : 
//  Author: lijinghua
//  Note: 
/*****************************************************************************/

LOCAL uint32 MMIAPIPICVIEW_GetRecentPicFileInfo(uint32 index, uint32 pic_num);

/*****************************************************************************/
//  Description : start slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartSlidePlayTimer(
                               MMI_WIN_ID_T   win_id
                               );

/*****************************************************************************/
//  Description : stop slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopSlidePlayTimer(void);

/*****************************************************************************/
//  Description : get the first mark picture index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetFirstMarkIndex(//is mark
                                uint32          *index_ptr,     //in[current index]/out[the first mark index]
                                uint32          toatal_num,     //in
                                MMI_CTRL_ID_T   list_ctrl_id    //in
                                );

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextSlidePic(//is change
                           uint32            cur_index,          //in:from
                           uint32            total_num,          //in
                           MMI_CTRL_ID_T     ctrl_id,            //in
                           MMI_CTRL_ID_T     list_ctrl_id        //in
                           );

/*****************************************************************************/
//  Description : switch the next marked picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextMarkedPic(//is change
                            uint16          *mark_num_ptr,      //in/out:may PNULL
                            uint32          cur_index,          //in:from 0
                            uint32          toatal_num,         //in
                            MMI_CTRL_ID_T   ctrl_id,            //in
                            MMI_CTRL_ID_T   list_ctrl_id        //in
                            );

/*****************************************************************************/
//  Description : handle preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : set preview opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayPreviewOptMenu(
                                 MMI_CTRL_ID_T      list_ctrl_id
                                 );

/*****************************************************************************/
//  Description : send picture by mms
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByMms(
                        uint32  item_index  //from 0
                        );

/*****************************************************************************/
//  Description : send picture by blue tooth
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByBt(
                       uint32  item_index  //from 0
                       );

//lijinghua add for multiple pic share by BT
LOCAL void SendMultiPicByBt();
LOCAL void SendMultiItemByEmail(void);
//lijinghua end
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : send picture to sns
//  Global resource dependence : 
//  Author: wei.ren
//  Note: 
/*****************************************************************************/
LOCAL void SendPicToSNS(
                       uint32  item_index ,
                       uint32  sns_index  //from 0
                       );
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle slide set win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlideSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           );

/*****************************************************************************/
//  Description : set slide set param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideSettings(void);

/*****************************************************************************/
//  Description : get slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideEffectIndex(void);

/*****************************************************************************/
//  Description : set slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideEffectIndex(
                               uint16   cur_index
                               );
#endif
/*****************************************************************************/
//  Description : get slide time
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideTime(void); //return ms
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void);

/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(
                             uint16   cur_index
                             );

/*****************************************************************************/
//  Description : save slide set param
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SaveSlideSet(void);
#endif
/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaper(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        );
/*****************************************************************************/
//  Description : set power on/off anim
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetPowerAnim(
                        uint32  item_index,  //from 0
                        BOOLEAN is_power_on,
                        MMI_CTRL_ID_T  ctrl_id
                        );
/*****************************************************************************/
//  Description : set picture anim control param
//  Global resource dependence :
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
BOOLEAN MMIPICVIEW_SetSelPicParam (
    const wchar          *full_path_wstr_ptr,//in
    uint16         full_path_wstr_len, //in:×??úêy
    MMI_CTRL_ID_T  ctrl_id
);
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set PB contatct icon
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPhontbook(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        );
#endif
                 
#ifdef PICVIEW_VIEW_ZOOM

/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void ZoomPic(
                   uint32   item_index  //from 0
                   );   //lijinghua modify it for pic zoom
#endif
#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : edit picture
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void EditPic(
                   uint32  item_index  //from 0
                   );
#endif
/*****************************************************************************/
//  Description : handle picture list mark option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListMarked(
//                                MMI_CTRL_ID_T  list_ctrl_id
//                                );

/*****************************************************************************/
//  Description : handle picture list mark cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 );

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListMarkedAll(
//                                   MMI_CTRL_ID_T   list_ctrl_id
//                                   );

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListUnmarkedAll(
//                                     MMI_CTRL_ID_T list_ctrl_id
//                                     );

/*****************************************************************************/
//  Description : handle picture preview delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : to handle the message of progress window
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id,
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       );
/*****************************************************************************/
//  Description : to handle the message of input field window    
//  Global resource dependence : 
//  Author:lijinghua
//  Note: 
/*****************************************************************************/	   
LOCAL MMI_RESULT_E HandleRenameDialogWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : cancel delete    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopDelete(void);

/*****************************************************************************/
//  Description : delete selected pictures
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectedPic(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in
                             );

/*****************************************************************************/
//  Description : handle delete selected pictures confirm message
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePicCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:
                              );
#if 0
/*****************************************************************************/
//  Description : handle delete picture success alert window
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSuccAlertWin(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );
#endif
/*****************************************************************************/
//  Description : handle rename win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicRenameWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : set initial name
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetInitName(
                       MMI_CTRL_ID_T    ctrl_id,    //in
                       MMI_CTRL_ID_T    list_ctrl_id//in
                       );

/*****************************************************************************/
//  Description : rename picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenamePic(
                        MMI_CTRL_ID_T  ctrl_id,    //in
                        MMI_CTRL_ID_T  list_ctrl_id//in
                        );

/*****************************************************************************/
//  Description : update picture list array
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void UpdateListArray(
                           uint16   index,          //in
                           wchar    *new_name_ptr,  //in:full name
                           uint16   new_name_len    //in:full name
                           );

/*****************************************************************************/
//  Description : handle detail win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicDetailWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
// 	Description: handle email entry tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryOneDriveTipWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

LOCAL MMI_RESULT_E  HandleEntryOneDriveTipInfoWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  );
/*****************************************************************************/
//  Description : Handle Pic One Drive Auto Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveConnectDialogWinMsg(
															MMI_WIN_ID_T     win_id, 
															MMI_MESSAGE_ID_E msg_id, 
															DPARAM           param 
															 );

/*****************************************************************************/
//  Description : Handle Pic One Drive Auto Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveStartWinMsg (
													MMI_WIN_ID_T     win_id, 
													MMI_MESSAGE_ID_E msg_id, 
													DPARAM           param 
												 );

/*****************************************************************************/
//  Description : Handle Pic One Drive Auto Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveCheckUpdateWinMsg(
														 MMI_WIN_ID_T     win_id, 
														 MMI_MESSAGE_ID_E msg_id, 
														 DPARAM           param 
													   );

/*****************************************************************************/
//  Description : Handle Pic One Drive Auto Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveAutoUploadDialogWinMsg(
															MMI_WIN_ID_T     win_id, 
															MMI_MESSAGE_ID_E msg_id, 
															DPARAM           param 
															 );

/*****************************************************************************/
//  Description : Handle Pic One Drive Manual Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveManualUploadDialogWinMsg(
															  MMI_WIN_ID_T     win_id, 
															  MMI_MESSAGE_ID_E msg_id, 
															  DPARAM           param 
															  );

/*****************************************************************************/
//  Description : handle onedrive win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicOneDriveWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : Update MS progressbar by timer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateProgressByTimer(void);

/*****************************************************************************/
//  Description : Start MS progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartProgressTimer(void);

/*****************************************************************************/
//  Description : Stop MS progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopProgressTimer(void);

/*****************************************************************************/
//  Description : MS progressbar timer callback function
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ProgressTimerCallBack(void);

/*****************************************************************************/
//  Description : handle onedrive win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicOneDriveUploadOptWinMsg(
										   MMI_WIN_ID_T        win_id,
										   MMI_MESSAGE_ID_E    msg_id,
										   DPARAM              param
                                         );

/*****************************************************************************/
//  Description : set detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDetail(
					 MMI_WIN_ID_T	win_id,
                     MMI_CTRL_ID_T  ctrl_id,
                     MMI_CTRL_ID_T  list_ctrl_id
                     );

/*****************************************************************************/
//  Description : add fix string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddFixStrItem(
                         uint16             *index_ptr, //in/out
                         MMI_TEXT_ID_T      text_id,
                         GUIRICHTEXT_ITEM_T *richtext_item_ptr,
                         MMI_CTRL_ID_T      ctrl_id
                         );

/*****************************************************************************/
//  Description : add string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddStrItem(
                      const uint8           *str_ptr,           //in
                      uint16                str_len,            //in
                      uint16                *index_ptr,         //in/out
                      GUIRICHTEXT_ITEM_T    *richtext_item_ptr, //in
                      MMI_CTRL_ID_T         ctrl_id
                      );

/*****************************************************************************/
//  Description : open set default path win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathWin(void);

/*****************************************************************************/
//  Description : open set default path callback
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle list preview longok menu win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistLongokOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : set list longok opt menu gray
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListLongokOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              );
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle pda style setting win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPdaSettingWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );                                         
/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitPicPdaSettingsListCtrl(void);
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendPicPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //???tID
                                 MMI_TEXT_ID_T  text_1,     //?÷??±?
                                 MMI_TEXT_ID_T  text_2,     //?±??±?
                                 MMI_IMAGE_ID_T image_id,   //í?±êID
                                 uint32         user_data   //ó??§êy?Y
                                 );
#endif                                 
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : get text_id by view type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByViewType(MMIPICVIEW_VIEW_TYPE_E view_type);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get text_id by effect type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEffectType(MMIPICVIEW_EFFECT_TYPE_E effect_type);

/*****************************************************************************/
//  Description : get text_id by slide time type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByTimeType(MMIPICVIEW_TIME_TYPE_E time_type);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle view set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicViewSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendViewListItem(void);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicEffectSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendEffectListItem(void);

/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicTimeSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendTimeListItem(void);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Discription: append split item
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(	
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_TEXT_ID_T    text_id
                               );
#endif

/*****************************************************************************/
//  Description : Start rect progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartOneDriveSyncProgressTimer(void);

/*****************************************************************************/
//  Description : Stop rect progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopOneDriveSyncProgressTimer(void);

/*****************************************************************************/
//  Description : rect progressbar timer callback function
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ProgressTimerCallBack(void);
/*****************************************************************************/
//  Description :gallery  task waiting win handler
//  Global resource dependence : 
//  Author: zhuyongjing
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleTaskWaitSpinnerWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
//picture folder list option
WINDOW_TABLE( MMIPICVIEW_FOLDER_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePicFolderlistOptWinMsg),
    WIN_ID(MMIPICVIEW_PICFOLDER_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_FOLDER_LIST_OPT,MMIPICVIEW_FOLDER_LIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//picture viewer list option
WINDOW_TABLE( MMIPICVIEW_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePiclistOptWinMsg),
    WIN_ID(MMIPICVIEW_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    //zhuyongjing change  for gallary ui 20150707
    //zhuyongjing delete static menu add create dynamic menu 
    //CREATE_POPMENU_CTRL(MENU_PICVIEW_LIST_OPT,MMIPICVIEW_LIST_OPT_MENU_CTRL_ID),
    // WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
   WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK),
   //change end
   // CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL_ID, MMIPICVIEW_LIST_WIN_ID),//zhuyongjing del
    END_WIN
};

//lijinghua add
WINDOW_TABLE( MMIPICVIEW_SHARE_ITEMS_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleShareItemsPiclistOptWinMsg),
    WIN_ID(MMIPICVIEW_SHARE_ITEMS_LIST_OPT_WIN_ID),
    #if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
    CREATE_POPMENU_CTRL(MENU_PICVIEW_SEND_ITEMS,MMIPICVIEW_SHARE_OPT_MENU_CTRL_ID),
    #endif
	WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK),
    END_WIN
};
//lijinghua end

//picture viewer list option
// WINDOW_TABLE( MMIPICVIEW_LIST_MARK_WIN_TAB ) = 
// {
//     WIN_FUNC( (uint32)HandlePiclistMarkWinMsg),
//     WIN_ID(MMIPICVIEW_LIST_MARK_WIN_ID),
//     WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
//     CREATE_POPMENU_CTRL(MENU_PICVIEW_MARK_OPT,MMIPICVIEW_LIST_MARK_MENU_CTRL_ID),
//     WIN_SOFTKEY(STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN),
//     END_WIN
// };
#endif

//picture preview option
WINDOW_TABLE( MMIPICVIEW_PREVIEW_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePreviewOptWinMsg),
    WIN_ID(MMIPICVIEW_PREVIEW_OPT_WIN_ID),
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(STXT_OPTION),
#endif
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    //zhuyongjing instead of dynamic menu
    //CREATE_POPMENU_CTRL(MENU_PICVIEW_PREVIEW_OPT,MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID),
    CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL1_ID, MMIPICVIEW_LIST_WIN_ID),
    //zhuyonjing chaneg for gallayr ui
    //WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),  
    WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_NULL),
    //change end
    END_WIN
};

//slide play
WINDOW_TABLE( MMIPICVIEW_SLIDE_PLAY_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicSlidePlayWinMsg), 
    WIN_ID( MMIPICVIEW_SLIDE_PLAY_WIN_ID ),
    WIN_STYLE( WS_DISABLE_COMMON_BG),
    WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIPICVIEW_SLIDE_PLAY_ANIM_CTRL_ID,MMIPICVIEW_SLIDE_PLAY_WIN_ID),
    END_WIN
};
#ifndef MMI_PICVIEW_MINI_SUPPORT
//slide set
WINDOW_TABLE( MMIPICVIEW_SLIDE_SET_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicSlideSetWinMsg), 
    WIN_ID( MMIPICVIEW_SLIDE_SET_WIN_ID ),
    WIN_TITLE(TXT_PICVIEW_SLIDE_SET),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPICVIEW_SET_FORM_CTRL_ID),
        //effect
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPICVIEW_EFFECT_FORM_CTRL_ID,MMIPICVIEW_SET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPICVIEW_EFFECT_LABEL_CTRL_ID,MMIPICVIEW_EFFECT_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID,MMIPICVIEW_EFFECT_FORM_CTRL_ID),
        //time
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPICVIEW_TIME_FORM_CTRL_ID,MMIPICVIEW_SET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPICVIEW_TIME_LABEL_CTRL_ID,MMIPICVIEW_TIME_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIPICVIEW_TIME_DROPDOWN_CTRL_ID,MMIPICVIEW_TIME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//rename
WINDOW_TABLE( MMIPICVIEW_RENAME_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicRenameWinMsg), 
    WIN_ID( MMIPICVIEW_RENAME_WIN_ID ),
    WIN_TITLE(STR_GAL_TITLE_RENAME_EXT01),
    CREATE_EDIT_TEXT_CTRL(0,MMIPICVIEW_RENAME_EDITBOX_CTRL_ID),
#ifdef PIC_PREVIEW_U200_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//detail
WINDOW_TABLE( MMIPICVIEW_DETAIL_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicDetailWinMsg), 
    WIN_ID( MMIPICVIEW_DETAIL_WIN_ID ),
    //WIN_TITLE(STXT_DETAIL),  //lijinghua delete it
    CREATE_RICHTEXT_CTRL(MMIPICVIEW_DETAIL_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,TXT_COMMON_COMMON_BACK),
    END_WIN
};

#ifdef CAMERAROLL_SUPPORT	
    //SHARE LINK WAITING
WINDOW_TABLE( MMIPICVIEW_SHARELINK_WAITING_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSharelinkWaitingWinMsg), 
    WIN_ID( MMIPICVIEW_SHARELINK_WAITING_WIN_ID ),
    CREATE_SPINNER_CTRL(MMIPICVIEW_SHARELINK_WAITING_SPINNER_CTRL_ID),
    END_WIN
};

//onedrive
WINDOW_TABLE( MMIPICVIEW_ONEDRIVE_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicOneDriveWinMsg), 
	WIN_ID( MMIPICVIEW_ONEDRIVE_WIN_ID ),

// 	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID),
//     CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E,MMIPICVIEW_ONEDRIVE_LIST_SYNC_CTRL_ID,MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID),
// 	CHILD_LIST_CTRL(TRUE,GUILIST_RADIOLIST_E,MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID,MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID),

	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID),

	WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_CHANGE,TXT_COMMON_COMMON_BACK),
	END_WIN
};

WINDOW_TABLE( MMIPICVIEW_ENTRY_ONEDRIVE_TIP_WIN_TAB ) = 
{
	WIN_FUNC((uint32)HandleEntryOneDriveTipWinMsg),    
	WIN_ID(PICVIEW_ENTRY_ONEDRIVE_TIP_WIN_ID),

    WIN_SOFTKEY(TXT_INFO, STXT_SOFTKEY_OK_MK, STXT_SOFTKEY_SKIP_RSK),
	CREATE_RICHTEXT_CTRL(PICVIEW_ENTRY_ONEDRIVE_TIP_CTRL_ID),
	END_WIN
};
WINDOW_TABLE( MMIPICVIEW_ENTRY_ONEDRIVE_TIP_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleEntryOneDriveTipInfoWinMsg),    
    WIN_ID(PICVIEW_ENTRY_ONEDRIVE_TIP_INFO_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_RICHTEXT_CTRL(PICVIEW_ENTRY_ONEDRIVE_TIP_INFO_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMIPICVIEW_ONEDRIVE_UPLOAD_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicOneDriveUploadOptWinMsg), 
    WIN_ID( MMIPICVIEW_ONEDRIVE_UPLOAD_OPT_WIN_ID ),
		
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( MMIPICVIEW_CHECK_UPDATE_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicOneDriveCheckUpdateWinMsg), 
		WIN_ID( MMIPICVIEW_ONEDRIVE_CHECK_UPDATE_WIN_ID ),
//		WIN_SOFTKEY(TXT_NULL,TXT_COMMON_OK,TXT_COMMON_COMMON_BACK),
		END_WIN
};
#endif
//PDA style
#ifdef PIC_PREVIEW_U200_SUPPORT

WINDOW_TABLE( MMIPICVIEW_LIST_LONGOK_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePiclistLongokOptWinMsg),
    WIN_ID(MMIPICVIEW_LIST_LONGOK_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_LIST_LONGOK_OPT,MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID),
    CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL2_ID, MMIPICVIEW_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
// PDA style éè??′°?ú
WINDOW_TABLE( MMIPICVIEW_PDA_SETTING_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_FUNC((uint32)HandlePicPdaSettingWinMsg),
    WIN_ID(MMIPICVIEW_PDA_SETTING_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
LOCAL WINDOW_TABLE( MMIVP_SETTING_VIEW_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_PICVIEW_VIEW_SETTING ),
    WIN_FUNC( (uint32)HandlePicViewSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_VIEW_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_VIEW_SET_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};
#endif //PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL WINDOW_TABLE( MMIVP_SETTING_EFFECT_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_EFFECT ),
    WIN_FUNC( (uint32)HandlePicEffectSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_EFFECT_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE( MMIVP_SETTING_TIME_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_PICVIEW_SLIDE_TIME ),
    WIN_FUNC( (uint32)HandlePicTimeSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_TIME_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_TIME_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
WINDOW_TABLE(MMIPICVIEW_TASK_WAIT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleTaskWaitSpinnerWinMsg),
    WIN_ID(MMIPICVIEW_TASK_WAITING_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_SPINNER_CTRL(MMIPICVIEW_WAITING_SPINNER_CTRL_ID),
    END_WIN
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_MSA_Get_Token_Func( int result, char*  buf, uint16 buflen)
{
#ifdef MSA_SUPPORT 
	if(MSA_OK == result)
	{
	        SCI_TRACE_LOW("[MMIPIC]MMIPICVIEW_MSA_Get_Token_Func: MSA_OK");
		//MMIPIC_CreateOneDriveConnectTextDialogWin();
		MMIPICVIEW_EntryOneDriveTip(FALSE);
                SCI_TRACE_LOW("[MMIPIC]MMIPICVIEW_MSA_Get_Token_Func: MSA_OK 1 ");
		MMIAPIMSA_Exit();	
	}
	else if(MSA_CANCEL == result)
	{
                MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE); 
		MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);  
	}
#endif
}

PUBLIC void MMIPICVIEW_Destroy()
{
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
        MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
    }

    if(MMK_IsOpenWin(MMIPICVIEW_PICFOLDER_WIN_ID))
    {
        MMK_CloseWin(MMIPICVIEW_PICFOLDER_WIN_ID);
    }
}
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicViewerWin(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    BOOLEAN b_isFirst_enter = FALSE;
#ifdef CAMERAROLL_SUPPORT
	MMISRV_CAMERAROLL_Enter_Gallery();
#endif	
    MMINV_READ(MMINV_PICVIEW_IS_FIRST_ENTER, &b_isFirst_enter, return_value);
#ifdef MMI_SHOW_MSA
	if(MN_RETURN_SUCCESS != return_value)
	{
		MMINV_WRITE(MMINV_PICVIEW_IS_FIRST_ENTER, &b_isFirst_enter);
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER ) && (_MSC_VER >= 1400)))
    if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)){
        MMIPICVIEW_EntryOneDriveTip(FALSE);
    }else{
        MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL ,  MMIPICVIEW_MSA_Get_Token_Func);
    }
#else
    //128X160无pic_folder窗口，直接打开MMIPICVIEW_LIST_WIN_TAB
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);  
    MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE); 

#endif
		return;
	}
#else
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_IS_FIRST_ENTER, &b_isFirst_enter);
    }
#endif

#ifdef MMI_SHOW_MSA
    if(!b_isFirst_enter)
    {
        MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
        if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))//for bug Bug 494985
        {
            MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
        }else{
            MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
        }
        
    }
    else
    {
        MMINV_WRITE(MMINV_PICVIEW_IS_FIRST_ENTER, FALSE);
        MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL ,  MMIPICVIEW_MSA_Get_Token_Func);
    }

#else

    MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))//for bug Bug 494985
    {
        MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
    }else{
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }

#endif

    return ; 
}
//lijinghua add
PUBLIC MMI_RESULT_E MMIPICVIEW_GetMultiChoiceDelete()
{
      return s_mmipicview_multi_choice_delete;
}

PUBLIC MMI_RESULT_E MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_E param)
{
      s_mmipicview_multi_choice_delete = param;
      return s_mmipicview_multi_choice_delete;
}

PUBLIC MMI_RESULT_E MMIPICVIEW_GetMultiChoiceShare()
{
      return s_mmipicview_multi_choice_share;
}

PUBLIC MMI_RESULT_E MMIPICVIEW_SetMultiChoiceShare(MMI_RESULT_E param)
{
      s_mmipicview_multi_choice_share = param;
      return s_mmipicview_multi_choice_share;
}

//lijinghua end

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void)
{
   MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PHOTO);
}

/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PATH_TYPE_E type)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {       
        s_mmipicview_path_type = type;
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_MYDOC, MMIPICVIEW_OpenPicViewerWin);
    }
}

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {
        MMIAPIPICVIEW_EnterMyPhotos();     
    }
}

/*****************************************************************************/
//  Description : get path type
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_PATH_TYPE_E MMIPICVIEW_GetPathType(void)
{
    return s_mmipicview_path_type;
}

#else
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {//bug117686
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_MAX, MMIPICVIEW_OpenPicViewerWin);//MMISET_PROTECT_MYDOC,
    }
}

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {
        MMIAPIPICVIEW_EnterMyPhotos();
    }
}
#endif
/*****************************************************************************/
//  Description : close pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ClosePicViewer(void)
{
    MMK_CloseMiddleWin( MMIPICVIEW_WIN_ID_START, MMIPICVIEW_MAX_WIN_ID );
}

/*****************************************************************************/
//  Description : open set default path win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathWin(void)
{
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMIFMM_FILTER_T             filter       = {0};
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //set list enable mark
    MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));
    
    if (MMIAPIDC_IsOpened())
    {
        //set path
        path_ptr = g_mmipicview_dc_path.path;
        path_len = g_mmipicview_dc_path.path_len;
    }
    else
    {
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);

        //set path
        path_ptr = default_path.path;
        path_len = default_path.path_len;
    }

    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(path_ptr,
                                            path_len,
                                            &filter,
                                            FUNC_FIND_FILE_AND_FOLDER,
                                            FALSE,
                                            PNULL,
                                            ret_array,
                                            OpenSetDefaultPathCallBack,
                                            TRUE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : handle set default path alert win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E HandleSetDefaultPathAlertWinMsg(
//                                                    MMI_WIN_ID_T      win_id, 
//                                                    MMI_MESSAGE_ID_E  msg_id, 
//                                                    DPARAM            param
//                                                    )
// {
//     MMI_RESULT_E result = MMI_RESULT_TRUE;
// 
//     switch (msg_id)
//     {
//     case MSG_CLOSE_WINDOW:
//         //post update list msg
// #ifdef PIC_PREVIEW_U200_SUPPORT
//         MMK_PostMsg( MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 ); 
// #else
//         MMK_PostMsg( MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 );
// #endif
// 
//         result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//         break;
//         
//     default:
//         result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//         break;
//     }
// 
//     return (result);
// }

/*****************************************************************************/
//  Description : open set default path callback
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    FILEARRAY_DATA_T          file_array_data = {0};
    MMIPICVIEW_DEFAULT_PATH_T default_path = {0};

    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array) && MMIAPIFILEARRAY_Read(file_array,0,&file_array_data ))
        {
            //get path
            MMIAPICOM_Wstrncpy(default_path.path, file_array_data.filename,file_array_data.name_len );

            default_path.path_len = file_array_data.name_len;

            //set path
            if (MMIAPIDC_IsOpened())
            {
                //copy path
                g_mmipicview_dc_path.path_len = default_path.path_len;
                MMI_MEMCPY(g_mmipicview_dc_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    default_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    sizeof(wchar)*default_path.path_len);
            }
            else
            {
                MMIAPIPICVIEW_SetDefaultPath(&default_path);
            }

		//xin.li_clean_popwin
            //MMIPUB_OpenAlertWinByTextId( PNULL, TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, HandleSetDefaultPathAlertWinMsg ); 
// #ifdef PIC_PREVIEW_U200_SUPPORT
		MMK_PostMsg( MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 ); 
// #else
// 		MMK_PostMsg( MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 );
// #endif
	
	}
        else
        {
            //SCI_TRACE_LOW:"OpenSetDefaultPathCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1073_112_2_18_2_46_43_74,(uint8*)"");
        }
    }
    else
    {
         //SCI_TRACE_LOW:"OpenSetDefaultPathCallBack failed"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1078_112_2_18_2_46_43_75,(uint8*)"");
    }

    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}
//zhuyongjing add begin  for gallary ui 20150707

PUBLIC void MMIAPIPICVIEW_SetShowFolderPath(
    PIC_SOURCE_TYPE  type
)
{
    s_mmipicview_show_folder = type;
}
PUBLIC void MMIAPIPICVIEW_SetPreviousFolderIsSubfolder( BOOLEAN  val)
{
    s_previous_folder_is_subfolder = val;
}
PUBLIC BOOLEAN MMIAPIPICVIEW_GetPreviousFolderIsSubfolder(void)
{
    return s_previous_folder_is_subfolder;
}
PUBLIC PIC_SOURCE_TYPE MMIAPIPICVIEW_GetShowFolderPath(void)
{
    return s_mmipicview_show_folder;
}
LOCAL BOOLEAN HaveLocalItemsForDelOrShare(void )
{
    BOOLEAN res = TRUE;
    uint32 allnum = MMIPICVIEW_GetAllPicNum();
    uint32 thumbnailnum = MMIPICVIEW_GetAllThumbnailsNum();
    if(allnum <= thumbnailnum){
       // res = FALSE;
    }
    return res;
}

LOCAL BOOLEAN HaveItemsForUpload(void )
{
    BOOLEAN res = TRUE;
    /*FILEARRAY s_onedrive_upload_success_file_array = MMIAPIFILEARRAY_Create();
    FILEARRAY s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();
    wchar *s_onedrive_upload_success_file_path = L"D:\\onedrive_upload_success.txt";
    wchar *s_onedrive_manual_upload_list_file_path = L"D:\\onedrive_manual_upload_list.txt";
    FILEARRAY temparray = MMIAPIFILEARRAY_Create();
    uint32 size = 0;
    
    MMIPICVIEW_GetFileArrayFromOther(temparray,MMIAPIPICVIEW_GetAllPicArray());

    //delete uploaded files
    if(MMIAPIFMM_IsFileExist(s_onedrive_upload_success_file_path , MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path)))
    {
        MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));
    }
    MMIAPIPICVIEW_DeleteArray(temparray,s_onedrive_upload_success_file_array, FALSE,FALSE);
    MMIAPIFILEARRAY_Destroy(&s_onedrive_upload_success_file_array);
    //delete manu uploading files
    if(MMIAPIFMM_IsFileExist(s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path)))
    {
    	MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_manual_upload_file_array , s_onedrive_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
    }
    MMIAPIPICVIEW_DeleteArray(temparray,s_onedrive_manual_upload_file_array, FALSE,FALSE);
    MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);

    //delete thumbnails
    MMIAPIPICVIEW_DeleteArray(temparray,MMIAPIPICVIEW_GetThumbnailArray(),FALSE,FALSE);

    //delete videos
    FilterFileArrayByAttri(temparray,FALSE,FALSE);
    size = MMIAPIFILEARRAY_GetArraySize(temparray);
    if(size == 0){
        res = FALSE;
    }
    MMIAPIFILEARRAY_Destroy(&temparray);*/
    return res;
}
LOCAL void InsertMenuNodeByLabel(
                                    MMI_CTRL_ID_T      ctrl_id,        //控件id
                                    MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                                    MMI_IMAGE_ID_T     img_id,
                                    uint16          	node_id,        //节点编号
                                    uint16          	parent_node_id, //父节点编号
                                    uint16          	index           //位置
                                 )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
LOCAL void CreateListOptMenuCtrl(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUIMENU_DYNA_ITEM_T node_item = {0};
    uint16 index = 0;
    uint16 i = 0;
    uint16 sub_index = 0;
    uint16 item_num = 0;
    uint32 sub_node_id = 0;
    uint16      mark_num = 0;
    MMI_STRING_T nstring = {0};
    GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK);
    
    if(((MMIPICVIEW_GetMultiChoiceDelete() ==MMI_RESULT_TRUE)||(MMIPICVIEW_GetMultiChoiceShare()==MMI_RESULT_TRUE)
	||MMIPICVIEW_GetMultiChoiseSaveToOneDrive())&&MMIPICVIEW_GetListMarkable(list_ctrl_id)){
         mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if(GUIICONLIST_GetTotalIcon(list_ctrl_id) > mark_num){
            InsertMenuNodeByLabel(ctrl_id, STR_MARK_ALL, 0,PICVIEW_OPT_MARK_ALL, 0, index++);
         }
         if (0 < mark_num)
         {
	      InsertMenuNodeByLabel(ctrl_id, STR_UNMARK_ALL, 0,PICVIEW_OPT_UNMARK_ALL, 0, index++);
         }
    }else if(MMK_IsActiveCtrl(MMIPICVIEW_LIST_EMPTY_CTRL_ID)){
#ifdef MMI_SHOW_MSA
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_SHOW_EXT01, 0,PICVIEW_OPT_SHOW, 0, index++);
        InsertMenuNodeByLabel(ctrl_id, STR_ONEDRIVE_LIST_ONEDRIVE, 0,PICVIEW_OPT_ONEDRIVE, 0, index++);
#endif

        item_num = sizeof(menu_picview_show)/sizeof(*menu_picview_show);
        for(i = 0; i < item_num; i++)
    {
           sub_node_id = menu_picview_show[i][0];
           SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
           MMI_GetLabelTextByLang(menu_picview_show[i][1], &nstring);
           node_item.item_text_ptr = &nstring;
           node_item.select_icon_id = 0;
           GUIMENU_InsertNode(sub_index, sub_node_id, PICVIEW_OPT_SHOW, &node_item, ctrl_id);
           sub_index ++;
        }
#ifdef MMI_SHOW_MSA
#if !defined(WIN32)
        if(MMISRV_CAMERAROLL_Get_Connect_State() && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)){
            GUIMENU_SetNodeVisible(ctrl_id, 4, PICVIEW_OPT_SHOW, TRUE);
        }else{
            GUIMENU_SetNodeVisible(ctrl_id, 4, PICVIEW_OPT_SHOW, FALSE);
        }
#endif
#endif
    }else{
#ifdef MMI_SHOW_MSA
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE)
        {
         InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_SHOW_EXT01, 0,PICVIEW_OPT_SHOW, 0, index++);
        }
#endif
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE)
        {
#if defined(CAMERAROLL_SUPPORT)        
            if(
		  MMISRV_CAMERAROLL_Get_Connect_State()                
                && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL) &&
                 !HaveLocalItemsForDelOrShare())
            {
                
            }
            else
#endif							
            {
                InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_DELETE_ITEMS_EXT01, 0,PICVIEW_OPT_DELETE_ITEMS, 0, index++);
                if(FALSE == MMIAPISET_IsDisableShareViaOption())
                {
		#ifdef BLUETOOTH_SUPPORT
                    InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_SHARE_ITEMS_EXT01, 0,PICVIEW_OPT_SHARE_ITEMS, 0, index++);
		#endif
                }
            }
        }
        else if(MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY)
        {
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_DELETE_ITEMS_EXT01, 0,PICVIEW_OPT_DELETE_ITEMS, 0, index++);
            if(FALSE == MMIAPISET_IsDisableShareViaOption())
            {
		#ifdef BLUETOOTH_SUPPORT
                InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_SHARE_ITEMS_EXT01, 0,PICVIEW_OPT_SHARE_ITEMS, 0, index++);
		#endif
            }
        }

#ifdef MMI_SHOW_MSA
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE 
            &&CAMERA_ROLL_MANUAL_SYNC == MMISRV_CAMERAROLL_Get_Sync_State()
            #if !defined(WIN32)
            && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            #endif
            && MMISRV_CAMERAROLL_Get_Connect_State()
            && HaveItemsForUpload()){
            InsertMenuNodeByLabel(ctrl_id, STR_ONEDRIVE_LIST_SAVE_ITEMS_EXT01, 0,PICVIEW_OPT_SAVE_ITEMS_TO_ONEDRIVE, 0, index++);
        }
#endif

      /*  if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
            ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ALL_LOCAL
            ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_DONWLOADED
            ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_CAPTURED){
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_SLIDE_PLAY_EXT01, 0,PICVIEW_OPT_SLIDE_PLAY, 0, index++);
    }*/
#ifdef MMI_SHOW_MSA
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
            InsertMenuNodeByLabel(ctrl_id, STR_ONEDRIVE_LIST_ONEDRIVE, 0,PICVIEW_OPT_ONEDRIVE, 0, index++);
        }
#endif
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
         item_num = sizeof(menu_picview_show)/sizeof(*menu_picview_show);

         for(i = 0; i < item_num; i++)
         {
	           sub_node_id = menu_picview_show[i][0];
	           SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
	           MMI_GetLabelTextByLang(menu_picview_show[i][1], &nstring);
	           node_item.item_text_ptr = &nstring;
	           node_item.select_icon_id = 0;
	           GUIMENU_InsertNode(sub_index, sub_node_id, PICVIEW_OPT_SHOW, &node_item, ctrl_id);
	           sub_index ++;
         }
#ifdef MMI_SHOW_MSA
#if !defined(WIN32)
        if(MMISRV_CAMERAROLL_Get_Connect_State() && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)){
            GUIMENU_SetNodeVisible(ctrl_id, 4, PICVIEW_OPT_SHOW, TRUE);
        }else{
            GUIMENU_SetNodeVisible(ctrl_id, 4, PICVIEW_OPT_SHOW, FALSE);
        }
#endif
#endif
    }
#ifdef MMI_SHOW_MSA
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_DOWNLOAD_EXT01, 0,PICVIEW_OPT_DOWNLOAD, 0, index++);
        }
#endif
    
    }
}

LOCAL void CreatePreviewOptMenuCtrl(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUIMENU_DYNA_ITEM_T node_item = {0};
    uint16 cur_index = 0;
    uint16 i = 0;
    uint16 item_num = 0;
    FILEARRAY_DATA_T        file_info = {0};
    uint16 index = 0;
    MMI_STRING_T nstring = {0};
    GUIANIM_TYPE_E          img_type = GUIANIM_TYPE_NONE;
    uint8 pcode[12] = {0};
    GUIMENU_CreatDynamic(&both_rect, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK);
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    if(MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
    {
        //video file
        if(MMIPICVIEW_IsVideoSuffix(&file_info) == TRUE)
        {
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_SHARE_EXT02, 0,PICVIEW_PICVIEW_OPT_SEND, 0, index++);       
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_DELETE_EXT02, 0,PICVIEW_PICVIEW_OPT_DELETE, 0, index++);
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_DETAILS_EXT01, 0,PICVIEW_PICVIEW_OPT_DETAIL, 0, index++);
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_TITLE_RENAME_EXT01, 0,PICVIEW_PICVIEW_OPT_RENAME, 0, index++);

            if(FALSE == MMIAPISET_IsDisableShareViaOption())
            {
                
#if 0
#ifdef MMS_SUPPORT
                SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
                MMI_GetLabelTextByLang(STR_SEND_VIA_MMS, &nstring);
                node_item.item_text_ptr = &nstring;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(0, ID_PICVIEW_MMS, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
#endif
#endif
#ifdef BLUETOOTH_SUPPORT
                SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
                MMI_GetLabelTextByLang(STR_SHARE_LIST_SLAM_EXT02, &nstring);
                node_item.item_text_ptr = &nstring;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(1, ID_PICVIEW_BT, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
#endif
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef WIN32
                LP_CODE_Read(pcode);
#endif
                if(SCI_STRLEN(pcode)>0 && MMIAPICOM_StrCmp(pcode,SCI_STRLEN(pcode),"QA",SCI_STRLEN("QA"))==0)
                {
                }
                else
                {
                    SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
                    MMI_GetLabelTextByLang(STR_SEND_VIA_MAIL, &nstring);
                    node_item.item_text_ptr = &nstring;
                    node_item.select_icon_id = 0;
                    GUIMENU_InsertNode(2, ID_PICVIEW_EMAIL, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
                }
#endif
            }
            return;
        }
        //thumbnail file
#if defined(WIN32)
        if(ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len))
#else
        if(ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)
	#if defined(CAMERAROLL_SUPPORT)		
            && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            &&MMISRV_CAMERAROLL_Get_Connect_State()
	#endif
	)
#endif
        {
            InsertMenuNodeByLabel(ctrl_id, STR_ONEDRIVE_LIST_SHARE_EXT01, 0,PICVIEW_PICVIEW_OPT_ONEDRIVE_SHARE_LINK, 0, index++);
            
            SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(STR_OPTIONS_SEND_VIA_SMS, &nstring);
            node_item.item_text_ptr = &nstring;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(0, ID_PICVIEW_SHARELINK_BY_SMS, PICVIEW_PICVIEW_OPT_ONEDRIVE_SHARE_LINK, &node_item, ctrl_id);
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef WIN32
            LP_CODE_Read(pcode);
#endif
            if(SCI_STRLEN(pcode)>0 && MMIAPICOM_StrCmp(pcode,SCI_STRLEN(pcode),"QA",SCI_STRLEN("QA"))==0)
            {
            }
            else
            {
                SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
                MMI_GetLabelTextByLang(STR_SEND_VIA_MAIL, &nstring);
                node_item.item_text_ptr = &nstring;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(0, ID_PICVIEW_SHARELINK_BY_EMAIL, PICVIEW_PICVIEW_OPT_ONEDRIVE_SHARE_LINK, &node_item, ctrl_id);
            }
#endif
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_DOWNLOAD_EXT01, 0,PICVIEW_PICVIEW_OPT_ONEDRIVE_DOWNLOAD, 0, index++);
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_DETAILS_EXT01, 0,PICVIEW_PICVIEW_OPT_DETAIL, 0, index++);
            return;
        }
        // original file
#if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_SHARE_EXT02, 0,PICVIEW_PICVIEW_OPT_SEND, 0, index++);       
#endif

        img_type = MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len);
#ifdef PIC_EDITOR_SUPPORT		
        if (GUIANIM_TYPE_IMAGE_JPG == img_type)
        {
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_EDIT_EXT01, 0,PICVIEW_PICVIEW_OPT_EDIT, 0, index++);
        }
#endif		
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_DELETE_EXT02, 0,PICVIEW_PICVIEW_OPT_DELETE, 0, index++);
#ifndef MMISET_GALLERY_WALLPAPER_NO_SUPPORT
        if (GUIANIM_TYPE_IMAGE_GIF != img_type
            && GUIANIM_TYPE_UNKNOWN != img_type
            && GUIANIM_TYPE_NONE != img_type)
        {
            InsertMenuNodeByLabel(ctrl_id, STR_GAL_LIST_LOCK_SCREEN_EXT01, 0,PICVIEW_PICVIEW_OPT_SET_TO_LOCKSCREEN, 0, index++);
        }
#endif        
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_HDR_DETAILS_EXT01, 0,PICVIEW_PICVIEW_OPT_DETAIL, 0, index++);
        InsertMenuNodeByLabel(ctrl_id, STR_GAL_TITLE_RENAME_EXT01, 0,PICVIEW_PICVIEW_OPT_RENAME, 0, index++);
 #if defined(CAMERAROLL_SUPPORT)
        if(CAMERA_ROLL_MANUAL_SYNC == MMISRV_CAMERAROLL_Get_Sync_State()
			&& ONEDRIVE_THUMBNAIL != GetFileSource(file_info.filename,file_info.name_len)		
			&& MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
			&& (MMISRV_CAMERAROLL_Get_Connect_State()==TRUE)
			&&!MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(file_info.filename,file_info.name_len)
			&&!MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len))
        {
            InsertMenuNodeByLabel(ctrl_id, STR_ONEDRIVE_LIST_SAVE, 0,PICVIEW_PICVIEW_OPT_SAVE_TO_ONEDRIVE, 0, index++);
        }
#endif
        if(FALSE == MMIAPISET_IsDisableShareViaOption())
        {
#if defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)
            SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(STR_SEND_VIA_MMS, &nstring);
            node_item.item_text_ptr = &nstring;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(0, ID_PICVIEW_MMS, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
#endif
#ifdef BLUETOOTH_SUPPORT
            SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(STR_SHARE_LIST_SLAM_EXT02, &nstring);
            node_item.item_text_ptr = &nstring;
            node_item.select_icon_id = 0;
            GUIMENU_InsertNode(1, ID_PICVIEW_BT, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
#endif
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef WIN32
            LP_CODE_Read(pcode);
#endif
            if(SCI_STRLEN(pcode)>0 && MMIAPICOM_StrCmp(pcode,SCI_STRLEN(pcode),"QA",SCI_STRLEN("QA"))==0)
            {
            }
            else
            {
                SCI_MEMSET(&nstring, 0x00, sizeof(MMI_STRING_T));
                MMI_GetLabelTextByLang(STR_SEND_VIA_MAIL, &nstring);
                node_item.item_text_ptr = &nstring;
                node_item.select_icon_id = 0;
                GUIMENU_InsertNode(2, ID_PICVIEW_EMAIL, PICVIEW_PICVIEW_OPT_SEND, &node_item, ctrl_id);
            }
#endif
        }
    }
}
/*****************************************************************************/
//  Description : open different folder pic & videos with special path 
//  Global resource dependence : none
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL void OpenSpecialFolderSetDefaultPath(
                                        MMI_WIN_ID_T        win_id,
                                      PIC_SOURCE_TYPE  type )
{
    MMIPICVIEW_DEFAULT_PATH_T default_path = {0};
    SCI_TRACE_LOW("[mmipicview_wintab]OpenSpecialFolderSetDefaultPath: type=%d",type);
        switch (type)
        {
            case CAMERA_ROLL_SOURCE_ALL_LOCAL:
                //default_path.path = {};
            break;
            case CAMERA_ROLL_SOURCE_CAPTURED:

            break;
            case CAMERA_ROLL_SOURCE_DONWLOADED:

            break;
            case CAMERA_ROLL_SOURCE_VIDEO:

            break;

            case CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY:

            break;
            default:
            break;
        }

        if (MMIAPIDC_IsOpened())
            {
                //copy path
                g_mmipicview_dc_path.path_len = default_path.path_len;
                MMI_MEMCPY(g_mmipicview_dc_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    default_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    sizeof(wchar)*default_path.path_len);
            }
            else
            {

                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
                    MMIAPIPICVIEW_SetPreviousFolderIsSubfolder(FALSE);
                }else{
                    MMIAPIPICVIEW_SetPreviousFolderIsSubfolder(TRUE);
                }
                MMIAPIPICVIEW_SetShowFolderPath(type);
            }
        MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, &s_mmipicview_show_folder);
       //MMK_PostMsg( MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, &type, sizeof(type) ); //post will get focus
}
PUBLIC BOOLEAN IsAccountCIDExist(void)
{
#ifdef MSA_SUPPORT
    MMIAPIMSA_USER_INFO_T * user_info =MMIAPIMSA_get_user_info();
    BOOLEAN result = FALSE;

    if(0 != strlen(user_info->email))
    {
            result = TRUE;
    }
    return result;

#else 
	return FALSE;
#endif
}

#ifdef CAMERAROLL_SUPPORT
LOCAL MMI_RESULT_E HandleUploadOnePicture(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param){
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        preview_win_id = MMIPICVIEW_PREVIEW_WIN_ID;
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    FILEARRAY_DATA_T        file_info = {0};
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, FALSE);
        break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
            if(sim_ok_num == 0){
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
                break;
            }
            s_mmipicview_current_upload_pic_idx = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_MANUAL_UPLOAD_SINGLE_PHOTO);
#if defined(WIN32)
            MMK_SendMsg(win_id, MSG_CAMERA_ROLL_INIT_CNF, PNULL);
#endif
        }  
        break;
	case MSG_CAMERA_ROLL_INIT_CNF:		
		{
		SCI_TRACE_LOW("[MMIPIC]:HandleUploadOnePicture receive MSG_CAMERA_ROLL_INIT_CNF !!!! s_mmipicview_current_upload_pic_idx=%d",s_mmipicview_current_upload_pic_idx);
		//get picture info,only jpg support picture editor
		if (MMIPICVIEW_GetPicFileInfo(s_mmipicview_current_upload_pic_idx,&file_info))
		{
			MMISRV_CAMERAROLL_Upload_single_photo(file_info.filename,file_info.name_len);			
			//refresh cloud icon
                       SCI_TRACE_LOW("[MMIPIC]:HandleUploadOnePicture receive MSG_CAMERA_ROLL_INIT_CNF 0");
			if(PNULL == s_uploading_file_array)
			{
				s_uploading_file_array = MMIAPIFILEARRAY_Create();
			}
                       SCI_TRACE_LOW("[MMIPIC]:HandleUploadOnePicture receive MSG_CAMERA_ROLL_INIT_CNF 1");

			MMIPICVIEW_RefreshAnimDisplay(s_mmipicview_current_upload_pic_idx);
                        if(MMIPICVIEW_GetMultiUploadingDone()){
                            MMIPICVIEW_AddOneMultiUploadingNum();
		        }
                        SCI_TRACE_LOW("[MMIPIC]:HandleUploadOnePicture receive MSG_CAMERA_ROLL_INIT_CNF 2");

                        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                            MMK_PostMsg(MMIPICVIEW_PREVIEW_WIN_ID, MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY, &s_mmipicview_current_upload_pic_idx,sizeof(s_mmipicview_current_upload_pic_idx));
                        }
                        if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                            GUIICONLIST_SetIndicateIcon(list_ctrl_id , s_mmipicview_current_upload_pic_idx , IMAGE_ONEDRIVE_UPLOADING_ICON);
                        }

		}
		MMK_CloseWin(win_id);
		}
	break;
    case MSG_CAMERA_ROLL_ERROR_NOTIFY:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
        ONE_DRIVE_ERROR_E err_trype = data->camera_roll_msg_data.error_type;
        SCI_TRACE_LOW("[MMIPIC]:HandleUploadOnePicture receive MSG_CAMERA_ROLL_ERROR_NOTIFY !!!!");

        if(MMISRV_CAMERAROLL_Get_Auto_upload_status()==ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK
            ||err_trype == ONE_DRIVE_ERROR_NO_NETWORK){
            DisplaybannerInfo(win_id,STR_ONEDRIVE_UPLOAD_FAILED_EXT01);
            MMK_CloseWin(win_id);
        }else if(err_trype == ONE_DRIVE_ERROR_HTTP_ERROR){
            DisplaybannerInfo(win_id,STR_NO_DATA_CONNECTION);
            MMK_CloseWin(win_id);
        }
        }
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
//transport
//refresh cloud icon
}
#endif

PUBLIC PIC_SRC_TYPE GetFileSource(wchar* filename,uint16 name_len){
    wchar             path_thumbnail_text[] = L"Photos\\OneDrive\\Thumbnails";
    wchar             path_download_text[] = L"Photos\\OneDrive"; 
    PIC_SRC_TYPE result = PIC_MAX_SRC;
    
    if(MMIAPICOM_Wstrnstr(filename,name_len,path_thumbnail_text,MMIAPICOM_Wstrlen(path_thumbnail_text))){
        result = ONEDRIVE_THUMBNAIL;
    }else if(MMIAPICOM_Wstrnstr(filename,name_len,path_download_text,MMIAPICOM_Wstrlen(path_download_text))){
        result = ONEDRIVE_DOWNLOAD;
    }
    return result;
}
PUBLIC void MMIPICVIEW_SetMultiChoiseSaveToOneDrive(BOOLEAN value){
    s_mmipicview_save_items_onedrive = value;
}
PUBLIC BOOLEAN MMIPICVIEW_GetMultiChoiseSaveToOneDrive(void){
    return s_mmipicview_save_items_onedrive;
}

/*****************************************************************************/
//  Description : delete filearray2 from filearray 1
//                [file_array_1] - [file_array_2] -> [file_array_1]
//  Global resource dependence : 
//  Author:zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_DeleteArray(   //返回True成功或False失败
                            FILEARRAY   file_array_1,       //in/out, filearray 1
                            FILEARRAY   file_array_2,        //in, filearray 2
                            BOOLEAN isforUI,
                            BOOLEAN isDeleteFile)
{
    BOOLEAN     result = TRUE;
    uint16      file_2_total = 0;
    uint16      file_1_total = 0;
    int16       index2 = 0;
    int16       index1= 0;
    FILEARRAY_DATA_T    file_del_data = {0};
    FILEARRAY_DATA_T    file_source_data = {0};

    if(NULL == file_array_1 || NULL == file_array_2)
    {
        return FALSE;
    }

    file_2_total = MMIAPIFILEARRAY_GetArraySize(file_array_2);
    file_1_total = MMIAPIFILEARRAY_GetArraySize(file_array_1);
    
    for (index2=0; index2<file_2_total;index2++)
    {
        if(MMIPICVIEW_IsClose()||MMIAPIPICVIEW_GetTaskCancelFlag()){
            result = FALSE;
            break;
        }
        if (!MMIAPIFILEARRAY_Read(file_array_2, index2, &file_del_data))
        {
            result = FALSE;
            break;
        }
        for (index1=0; index1<file_1_total; index1++)
        {
            if(MMIPICVIEW_IsClose()||MMIAPIPICVIEW_GetTaskCancelFlag()){
                result = FALSE;
                break;
            }
            if (!MMIAPIFILEARRAY_Read(file_array_1, index1, &file_source_data))
            {
                result = FALSE;
                break;
            }
            if(file_source_data.name_len == file_del_data.name_len){
                if(0 == MMIAPICOM_Wstrcmp(file_source_data.filename, file_del_data.filename))
                {
                    MMIVIRTUALARRAY_Delete(file_array_1, index1);
                    if(isforUI){
                        MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,index1);
                    }
                    file_1_total--;
                    result = TRUE;
                    break;
                }
            }
        }
        if(isDeleteFile){
            MMIAPIFMM_DeleteFileSyn(file_del_data.filename, file_del_data.name_len);
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : delete filearray2 with subname from filearray 1
//                [file_array_1] - [file_array_2] -> [file_array_1]
//  Global resource dependence : 
//  Author:zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_DeleteArrayWithSubName(   //返回True成功或False失败
                            FILEARRAY   file_array_1,       //in/out, filearray 1
                            FILEARRAY   file_array_2,        //in, filearray 2
                            BOOLEAN isforUI,
                            BOOLEAN isDeleteFile)
{
    BOOLEAN     result = TRUE;
    uint16      file_2_total = 0;
    uint16      file_1_total = 0;
    int16       index2 = 0;
    int16       index1= 0;
    FILEARRAY_DATA_T    file_del_data = {0};
    FILEARRAY_DATA_T    file_source_data = {0};
    wchar file_del_fullname[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_del_fullname_len = 0;
    
    if(NULL == file_array_1 || NULL == file_array_2)
    {
        return FALSE;
    }

    file_2_total = MMIAPIFILEARRAY_GetArraySize(file_array_2);
    file_1_total = MMIAPIFILEARRAY_GetArraySize(file_array_1);
    
    for (index2=0; index2<file_2_total;index2++)
    {
        if(MMIPICVIEW_IsClose()||MMIAPIPICVIEW_GetTaskCancelFlag()){
            result = FALSE;
            break;
        }
        if (!MMIAPIFILEARRAY_Read(file_array_2, index2, &file_del_data))
        {
            result = FALSE;
            break;
        }
        //file_del_data.filename =xxx.jpg  without /xx/xx/

        MMIAPICOM_Wstrcpy(file_del_fullname,L":\\Photos\\OneDrive\\Thumbnails\\");
        MMIAPICOM_Wstrcat(file_del_fullname,file_del_data.filename);
        file_del_fullname_len = MMIAPICOM_Wstrlen(file_del_fullname)+1;
        for (index1=0; index1<file_1_total; index1++)
        {
            if(MMIPICVIEW_IsClose()||MMIAPIPICVIEW_GetTaskCancelFlag()){
                result = FALSE;
                break;
            }
            if (!MMIAPIFILEARRAY_Read(file_array_1, index1, &file_source_data))
            {
                result = FALSE;
                break;
            }
            if(file_source_data.name_len == file_del_fullname_len){
                if(PNULL != MMIAPICOM_Wstrstr(file_source_data.filename, file_del_fullname))
                {
                    MMIVIRTUALARRAY_Delete(file_array_1, index1);
                    if(isforUI){
                        MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,index1);
                    }
                    if(isDeleteFile){
                        MMIAPIFMM_DeleteFileSyn(file_source_data.filename, file_source_data.name_len);
                    }
                    file_1_total--;
                    result = TRUE;
                    if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD)){
                        break;
                    }
                }
            }

        }
    }

    return result;
}

PUBLIC void MMIPICVIEW_SetMarkable(MMI_CTRL_ID_T  list_ctrl_id,BOOLEAN value){
    uint16 cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    if(value){
#ifdef CAMERAROLL_SUPPORT		
        if(MMIPICVIEW_GetMultiUploadingDone()
            &&(MMIPICVIEW_GetMultiChoiceDelete() ||MMIPICVIEW_GetMultiChoiceShare())
            && (MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE)){         
            ModifyPicListViewForUploadItems(MMIPICVIEW_LIST_WIN_ID,list_ctrl_id,FALSE);
        }
        if((MMIPICVIEW_GetMultiChoiceDelete() 
            ||MMIPICVIEW_GetMultiChoiceShare())
            && (MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE)
            &&MMISRV_CAMERAROLL_Get_Connect_State()){
            
            //MMIAPIPICVIEW_DestroyBackupAllPicArray();
            MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_BACKUP_FILEARRAY,MMIAPIPICVIEW_GetAllPicArray());
            if(MMIPICVIEW_GetAllPicNum()>VIRTUALARRAY_BUFFER_SIZE){
                GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_MARKABLE_DELETE_FILEARRAY;
            Markable_Delete_Filearray_signal();
                MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);
            }else{
                MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetAllPicArray(),TRUE,FALSE);
                if(MMIPICVIEW_GetAllPicNum() >0){
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STXT_SOFTKEY_MARK_MK, MIDDLE_BUTTON, TRUE);
                }else{
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_NULL, LEFT_BUTTON, TRUE);
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_NULL, MIDDLE_BUTTON, TRUE);
                }
            }
           // MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
#endif				
        if(!MMK_IsOpenWin(MMIPICVIEW_TASK_WAITING_WIN_ID)){
            HandlePicListMarked(list_ctrl_id);
        }
    }else{
        MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
     //   MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, &s_mmipicview_show_folder);
    }              
}

//zhuyongjing add end

#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle list folder option win msg
//  Global resource dependence : 
//  Author: kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_FOLDER_LIST_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayListOptMenu(list_ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
/*        
        case ID_PICVIEW_CHANGE_FOLDER:
            OpenSetDefaultPathWin();

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            break;

        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;

        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
*/            
        case ID_PICVIEW_SETTING:
            //open setting win
            MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
            break;
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicFolderlistOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1138_112_2_18_2_46_43_76,(uint8*)"d", menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveStartTextDialogWin(void)
{
	MMI_WIN_ID_T     win_id = MMIPICVIEW_ONEDRIVE_START_WIN_ID;
	GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
	GUI_RECT_T       win_rect = {0};
	uint16           logic_width = 0;
	uint16           logic_height = 0;
	
    MMIPUB_OpenTextDialogByTextId( PNULL,
		STR_ONEDRIVE_ENABLE_ACCOUNT_EXT01,
		TXT_NULL,
		PNULL,
		&win_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK,
		HandlePicOneDriveStartWinMsg);
}
#endif

/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_LIST_OPT_MENU_CTRL_ID;
  //  MMI_CTRL_ID_T       sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_STRING_T      	title_str = {0};
    uint32              cur_index = 0;
    uint16              pic_num =0;
    uint16              mark_num = 0;
    wchar              mark_wstr[30]={0};
    char              mark_num_str[30] ={0};
    wchar              mark_num_wstr[30]={0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
//zhuyongjing change  for gallary ui 20150707
        CreateListOptMenuCtrl(win_id,ctrl_id);
        //original :SetGrayListOptMenu(list_ctrl_id);
 //add end
        MMK_SetAtvCtrl(win_id,ctrl_id);
     //   CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
//zhuyongjing change for gallary ui 20150707
menu_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (menu_id)
        {
            case PICVIEW_OPT_SHOW:

            break;

            //lijinghua add
            case PICVIEW_OPT_DELETE_ITEMS:
/*#if !defined(WIN32) 
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                && MMISRV_CAMERAROLL_Get_Connect_State() 
                && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
                && MMIPICVIEW_GetAllThumbnailsNum()>0)
#else
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                && MMISRV_CAMERAROLL_Get_Connect_State() 
                &&MMIPICVIEW_GetAllThumbnailsNum()>0)
#endif
            {
                MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray(), TRUE,FALSE);    
            }*/
            SCI_TRACE_LOW("HandlePiclistOptWinMsg PICVIEW_OPT_DELETE_ITEMS ");

                s_mmipicview_multi_choice_delete = MMI_RESULT_TRUE;
                MMIPICVIEW_SetMarkable(MMIPICVIEW_LIST_CTRL_ID,TRUE);
	        MMI_GetLabelTextByLang(STR_GAL_HDR_DELETE_EXT02, &title_str );
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
            {
                  GUIWIN_UpdateStb();
            }
            break;

            case PICVIEW_OPT_SHARE_ITEMS:
            s_mmipicview_multi_choice_share = MMI_RESULT_TRUE;
/*#if !defined(WIN32) 
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                && MMISRV_CAMERAROLL_Get_Connect_State() 
                && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
                && MMIPICVIEW_GetAllThumbnailsNum()>0)
#else
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                &&MMIPICVIEW_GetAllThumbnailsNum()>0)
#endif
            {
                MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray(), TRUE,FALSE);    
            }*/

            MMIPICVIEW_SetMarkable(MMIPICVIEW_LIST_CTRL_ID,TRUE);
            MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02, &title_str );
            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
            {
                  GUIWIN_UpdateStb();
            }
             break;
            //lijinghua end
			//zhuyongjing add for save items to onedrive 20150722
            case PICVIEW_OPT_SAVE_ITEMS_TO_ONEDRIVE:
		#ifdef MSA_SUPPORT
                if(IsAccountCIDExist()){               
                    GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_UPLOAD_FILTER_FILEARRAY;
                    if(MMIPICVIEW_GetMultiUploadingDone()){         
                        ModifyPicListViewForUploadItems(MMIPICVIEW_LIST_WIN_ID,MMIPICVIEW_LIST_CTRL_ID,FALSE);
                    }
                    MMIPICVIEW_DestroyAllUploadingPicArray();
                    MMIPICVIEW_CreateAllUploadingPicArray();
                    MMIAPIPICVIEW_DestroyBackupAllPicArray();
                    Multi_Upload_Pics_Signal();
                    MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);

                 /*   FILEARRAY s_onedrive_upload_success_file_array = MMIAPIFILEARRAY_Create();
                    FILEARRAY s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();
                    wchar *s_onedrive_upload_success_file_path = L"D:\\onedrive_upload_success.txt";
                    wchar *s_onedrive_manual_upload_list_file_path = L"D:\\onedrive_manual_upload_list.txt";
                    
                    MMIPICVIEW_SetMultiChoiseSaveToOneDrive(TRUE);
                    MMIPICVIEW_DestroyAllUploadingPicArray();
                    MMIPICVIEW_CreateAllUploadingPicArray();
                    //backup files
                    MMIAPIPICVIEW_DestroyBackupAllPicArray();
                    //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIPICVIEW_GetAllPicArray());
                    MMIPICVIEW_GetFileArrayFromOther(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIPICVIEW_GetAllPicArray());

                    //delete uploaded files
                    if(MMIAPIFMM_IsFileExist(s_onedrive_upload_success_file_path , MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path)))
                    {
			MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));
                    }
                    MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),s_onedrive_upload_success_file_array, TRUE,FALSE);
                    MMIAPIFILEARRAY_Destroy(&s_onedrive_upload_success_file_array);
                    //delete manu uploading files
                    if(MMIAPIFMM_IsFileExist(s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path)))
                    {
                    	MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_manual_upload_file_array , s_onedrive_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
                    }
                    MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),s_onedrive_manual_upload_file_array, TRUE,FALSE);
                    MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);

                    //delete thumbnails
                    MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray(), TRUE,FALSE);
                    //delete videos
                    FilterFileArrayByAttri(MMIAPIPICVIEW_GetAllPicArray(),FALSE,TRUE);
                    MMIPICVIEW_SetMarkable(MMIPICVIEW_LIST_CTRL_ID,TRUE);*/
                }else{
                    DisplaybannerInfo(win_id,STR_MSA_SIGNIN_UNSUCCESS);
                }
		#endif
			break;
			//add end
            case PICVIEW_OPT_SLIDE_PLAY:
                MMIPICVIEW_SliePlay(list_ctrl_id);
            break;
#ifdef CAMERAROLL_SUPPORT	
            case PICVIEW_OPT_ONEDRIVE:
                MMIAPIPICVIEW_OpenOnedriveSettingsWin();
            break;
            case PICVIEW_OPT_DOWNLOAD:
            {
                MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
                MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_ONEDRIVE_NOTE_DOWNLOAD_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleDownloadWin);
            }
            break;
#endif
	   //lijinghua add
            case PICVIEW_OPT_MARK_ALL:
            {
                MMI_STRING_T                title_src1_str = {0};
                MMI_STRING_T                title_src2_str = {0};
                HandlePicListMarkedAll(list_ctrl_id);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                mark_num = MMIPICVIEW_GetAllPicNum();
                if(MMIPICVIEW_GetMultiChoiceDelete())
                {
                    MMI_GetLabelTextByLang(STR_GAL_HDR_DELETE_EXT02, &title_src1_str );

                    if(mark_num>0)
                    {
                     GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_LIST_DELETE, RIGHT_BUTTON, FALSE);
                    }

                   
                }else if(MMIPICVIEW_GetMultiChoiceShare()){
                    MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02, &title_src1_str );
                    if(mark_num>0)
                {
                       GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_LIST_SHARE, RIGHT_BUTTON, FALSE);
                    }
                }else if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                    MMI_GetLabelTextByLang(STR_ONEDRIVE_HDR_SAVE_EXT01, &title_src1_str );
                    if(mark_num>0)
                    {
                       GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_ONEDRIVE_HDR_UPLOAD_EXT02, RIGHT_BUTTON, FALSE);
                    }
                  //  MMIPICVIEW_DestroyAllUploadingPicArray();
                   // MMIPICVIEW_CreateAllUploadingPicArray();
                   // MMIAPIFILEARRAY_Combine(MMIPICVIEW_GetAllUploadingPicArray(),MMIAPIPICVIEW_GetAllPicArray());               
                    MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_UPLOADING_FILEARRAY,MMIAPIPICVIEW_GetAllPicArray());

                }
                sprintf(mark_num_str,"%s%d%s", " (",mark_num, ")");
                MMIAPICOM_StrToWstr((const uint8 *)mark_num_str, mark_num_wstr);
                MMIAPICOM_Wstrcat(mark_wstr,mark_num_wstr);
                title_src2_str.wstr_ptr = mark_wstr;
                title_src2_str.wstr_len = MMIAPICOM_Wstrlen(title_src2_str.wstr_ptr);
                MMIAPICOM_CatTwoString(&title_str, &title_src1_str, &title_src2_str);
                if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
                {
                   GUIWIN_UpdateStb();
                }
                MMIAPICOM_DestroyString(title_str);
                }
            break;

            case PICVIEW_OPT_UNMARK_ALL:
                HandlePicListUnmarkedAll(list_ctrl_id);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                if(MMIPICVIEW_GetMultiChoiceDelete())
                {
                   MMI_GetLabelTextByLang(STR_GAL_HDR_DELETE_EXT02, &title_str );
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                }else if(MMIPICVIEW_GetMultiChoiceShare()){
                   MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02, &title_str );
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                }else if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                    MMI_GetLabelTextByLang(STR_ONEDRIVE_HDR_SAVE_EXT01, &title_str );
                    GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                    MMIPICVIEW_DestroyAllUploadingPicArray();
                    MMIPICVIEW_CreateAllUploadingPicArray();
                }
                if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
                {
                   GUIWIN_UpdateStb();
                }
             break;
              //lijinghua end

            case ID_PICVIEW_SHOW_ALLLOCAL:
           {
            MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

            MMIAPIPICVIEW_GetDefaultPath(&default_path);
            SCI_TRACE_LOW("MMIPICVIEW ID_PICVIEW_SHOW zyj ID_PICVIEW_SHOW_ALLLOCAL ");

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            OpenSpecialFolderSetDefaultPath(win_id,CAMERA_ROLL_SOURCE_ALL_LOCAL);
            }
            break;

        case ID_PICVIEW_SHOW_CAPTURED:
          {
            MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
            MMIAPIPICVIEW_GetDefaultPath(&default_path);
            SCI_TRACE_LOW("MMIPICVIEW ID_PICVIEW_SHOW zyj path = ID_PICVIEW_SHOW_CAPTURED");

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            OpenSpecialFolderSetDefaultPath(win_id,CAMERA_ROLL_SOURCE_CAPTURED);
            }
            break;

        case ID_PICVIEW_SHOW_DOWNLOAD:
            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            OpenSpecialFolderSetDefaultPath(win_id,CAMERA_ROLL_SOURCE_DONWLOADED);
            break;

        case ID_PICVIEW_SHOW_VIDEO:
            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            OpenSpecialFolderSetDefaultPath(win_id,CAMERA_ROLL_SOURCE_VIDEO);
            break;

        case ID_PICVIEW_SHOW_ONEDRIVE:
            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            OpenSpecialFolderSetDefaultPath(win_id,CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY); 
            break;
         default:
            break;
        }
     /*   GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_CHANGE_FOLDER:
            OpenSetDefaultPathWin();

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            break;
#endif
        case ID_PICVIEW_MMS:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByBt(cur_index);
            break;
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
                
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif            
        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
            
        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif

#ifdef PIC_EDITOR_SUPPORT
        case ID_PICVIEW_EDIT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;
#endif
#endif
        case ID_PICVIEW_MARKED:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            HandlePicListMarked(list_ctrl_id);
            MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
            break;

        case ID_PICVIEW_UNMARKED:
            HandlePicListUnmarked(list_ctrl_id);
            break;

        case ID_PICVIEW_MARKED_ALL:
            HandlePicListMarkedAll(list_ctrl_id);
            break;

        case ID_PICVIEW_UNMARKED_ALL:
            HandlePicListUnmarkedAll(list_ctrl_id);
            break;

        case ID_PICVIEW_DELETE:
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
                if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
#endif
			{
				//prompt query window
				MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
			}
            break;

        case ID_PICVIEW_RENAME:
            MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_ONEDRIVE:
			// if account not exist
			MMIPIC_CreateOneDriveStartTextDialogWin();

			// if account exist
//             MMK_CreateWin((uint32 *)MMIPICVIEW_ONEDRIVE_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_VIEW_LIST:
            if (MMIPICVIEW_VIEW_LIST != MMIPICVIEW_GetViewType())
            {
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
            }
            break;

        case ID_PICVIEW_VIEW_ICONLIST:
            if (MMIPICVIEW_VIEW_ICONLIST != MMIPICVIEW_GetViewType())
            {
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
            }
            break;
         
//         case ID_PICVIEW_SETTING:
//             //open setting win
//             MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
//             break;
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePiclistOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1335_112_2_18_2_46_43_77,(uint8*)"d", menu_id);
            break;
        }*/

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

LOCAL MMI_RESULT_E HandleShareItemsPiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32              cur_index = 0;
    MMI_STRING_T      	         title_str = {0};
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_SHARE_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    #if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
    MMI_MENU_GROUP_ID_T group_id = MENU_PICVIEW_SEND_ITEMS;
    #endif
    MMI_STRING_T  kstring = {0};
    uint16 mark_num = 0;
    uint16 select_index = 0;
    uint8 pcode[12] = {0};
    FILEARRAY_DATA_T        file_info = {0};
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
         //zhuyongjing changed
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);

        if(mark_num > 1)
        {
#if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
            CTRLMENU_SetItemVisible(ctrl_id,group_id,ID_PICVIEW_SEND_ITEMS_BY_MMS,FALSE);
#endif
        }
        else
        {
		    select_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            if (MMIPICVIEW_GetPicFileInfo(select_index, &file_info))
            {
                if(MMIPICVIEW_IsVideoSuffix(&file_info) == TRUE)
                {
#if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
                    CTRLMENU_SetItemVisible(ctrl_id,group_id,ID_PICVIEW_SEND_ITEMS_BY_MMS,FALSE);		
#endif
                }
            }
        }
#ifndef WIN32
        LP_CODE_Read(pcode);
#endif
#ifdef MMI_SHOW_MSA
        if(SCI_STRLEN(pcode)>0 &&MMIAPICOM_StrCmp(pcode,SCI_STRLEN(pcode),"QA",SCI_STRLEN("QA"))==0){
            CTRLMENU_SetItemVisible(ctrl_id,group_id,ID_PICVIEW_SEND_ITEMS_BY_EMAIL,FALSE);
        }
#endif
        //change end 
        GUIMENU_SetPopMenuCentered(ctrl_id, TRUE);
        MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02,&kstring); //lijinghua change it
        GUIMENU_SetMenuTitle(&kstring,ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
#if (defined(MMS_SUPPORT) && !defined(MMI_GALLERY_SHARE_VIA_MULTIMEDIA_DISABLE)) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)
    GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
#endif
    switch (menu_id)
    {
     case ID_PICVIEW_SEND_ITEMS_BY_BT:
#ifdef FM_SUPPORT
    if(MMIAPIFM_IsFmActive()) 
    {
        SCI_TRACE_LOW("HandleShareItemsPiclistOptWinMsg FM is avtiving!!");
        MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
    }
    else
    {
       SendMultiPicByBt();
    }
#else
       SendMultiPicByBt();
#endif
       break;
    case ID_PICVIEW_SEND_ITEMS_BY_MMS:
            //get item index
        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
        SendPicByMms(cur_index);
        break;
    case ID_PICVIEW_SEND_ITEMS_BY_EMAIL:
            SendMultiItemByEmail();
        break;
    default:
        break;
    }
	case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
    break;
    case MSG_CLOSE_WINDOW:
        //zhuyongjing changed 
        MMIPICVIEW_SetMultiChoiceShare(MMI_RESULT_FALSE);
        MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);

#ifdef CAMERAROLL_SUPPORT	
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE && MMISRV_CAMERAROLL_Get_Connect_State()){
            //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray()); 
            MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
        }
#endif

        //change end
      #ifdef CAMERAROLL_SUPPORT  
        if(MMIPICVIEW_GetMultiUploadingDone()&&MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
            MMIPICVIEW_PostUpdateMsg(0);
        }else{
            MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(list_ctrl_id));
        }
	#else
		MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(list_ctrl_id));
	#endif
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
   }
    return (result);
}
//lijinghua end

/*****************************************************************************/
//  Description : set list opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              )
{
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_LIST_OPT;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};    
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    if (0 < MMIPICVIEW_GetAllPicNum())
    {
        uint16                  cur_index = 0;
        uint16                  mark_num = 0;
        uint32                  pic_num = MMIPICVIEW_GetAllPicNum();
        FILEARRAY_DATA_T        file_info = {0};
        MMI_MENU_GROUP_ID_T     mark_group_id = MENU_PICVIEW_MARK;
        
        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
        
        //current item is mark
        if (MMIPICVIEW_IsSelectItem(list_ctrl_id,cur_index))
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_MARKED,TRUE);
        }
        else//NEWMS00172875
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED,TRUE);
        }
        
        //get mark num
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if (0 < mark_num)
        {            
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SEND,TRUE);
#endif
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
            if(mark_num == pic_num)//NEWMS00172875
            {
                GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_MARKED_ALL,TRUE);
            }
        }
        else
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED_ALL,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED,TRUE);
            //get picture info,only jpg support picture editor
            if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) && 
                (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len)))
            {
                //do nothing
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            }
        }
        #ifdef CMCC_UI_STYLE
        if(MMIPICVIEW_DOWNLOAD_PIC == s_mmipicview_path_type)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_CHANGE_FOLDER,TRUE);
        }
        #endif
    }
    else
    {
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SEND,TRUE);
#endif		
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SLIDE_PREVIEW,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MARKED,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DELETE,TRUE); 
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE); 

        #ifdef CMCC_UI_STYLE  //xuefang.jiang add 20110226 to gray change folder function.
        if(MMIPICVIEW_DOWNLOAD_PIC == s_mmipicview_path_type)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_CHANGE_FOLDER,TRUE);
        }
        #endif
    }

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW SetGrayListOptMenu view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1449_112_2_18_2_46_44_78,(uint8*)"d", view_type);
    
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        pop_item_info.menu_id = ID_PICVIEW_VIEW_LIST;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        pop_item_info.menu_id = ID_PICVIEW_VIEW_ICONLIST;
        break;
    }

    //set pop-up menu status
    pop_item_info.is_static = TRUE;
    pop_item_info.ctrl_id   = ctrl_id;
    pop_item_info.group_id  = MENU_PICVIEW_VIEW;
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);  
}
#endif

/*****************************************************************************/
// 	Description : picture slide play
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SliePlay(
								MMI_CTRL_ID_T	ctrl_id
								)
{
#ifdef DRM_SUPPORT
    uint32              item_index = MMIPICVIEW_GetCurIndex(ctrl_id);
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();

    //if mark,display the first mark picture
    if (GetFirstMarkIndex(&item_index,pic_num,ctrl_id))
    {
        //set list index
        MMIPICVIEW_SetCurIndex(ctrl_id,(uint16)item_index);
    }

//    if ((MMIPICVIEW_IsPreview(FALSE,item_index)))
    {
        MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_PLAY_WIN_TAB,PNULL);
    }
#else
    MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_PLAY_WIN_TAB,PNULL);
#endif
}

/*****************************************************************************/
//  Description : handle picture slide play win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlidePlayWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    LOCAL BOOLEAN       s_image_is_display = FALSE;
    uint16              mark_num = 0;
    uint32              item_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_SLIDE_PLAY_ANIM_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetFullScreenBothRect();
    MMI_CTRL_ID_T       setlist_ctrl_id = MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID;
	GUI_BG_T	common_bg = {0};
    FILEARRAY_DATA_T        file_info = {0};
	uint32				current_index = item_index;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        GUIAPICTRL_SetBothRect(ctrl_id,&both_rect);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);

        //if mark,display the first mark picture
        if (GetFirstMarkIndex(&item_index,pic_num,list_ctrl_id))
        {
            //set list index
            MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)item_index);
        }

        //set anim param
        s_image_is_display = FALSE;

		//Modified by lijinghua
        current_index = MMIAPIPICVIEW_GetRecentPicFileInfo(item_index, pic_num);
        MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)current_index);
        if(current_index != pic_num){
	        SetSlidePlayAnim(current_index,ctrl_id);
	        MMK_SetAtvCtrl(win_id,ctrl_id);
			s_refresh_preview_anim = TRUE;
        }
		//Modified by lijinghua end
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:
#ifdef DRM_SUPPORT
		{
			FILEARRAY_DATA_T		file_info = {0};
			GUIANIM_DISPLAY_IND_T	*display_ind_ptr = (GUIANIM_DISPLAY_IND_T *)param;

			if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
			{
				if ((PNULL != display_ind_ptr) && 
					(GUIANIM_RESULT_SUCC == display_ind_ptr->result))
				{
					MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE,file_info.filename);
				}
			}
			else
			{
                SCI_TRACE_LOW("MMIPICVIEWER HandlePicSlidePlayWinMsg MMIPICVIEW_GetPicFileInfo failed!");
			}
		}
#endif
        s_image_is_display = TRUE;
        if (MMK_IsFocusWin(win_id))
        {
            //play slide play timer
            StartSlidePlayTimer(win_id);
        }
        break;

    case MSG_TIMER:
        if (s_mmipicview_slide_play_timer_id == *(uint8*)param)
        {
            //stop slide play timer
            StopSlidePlayTimer();

            //set special effect
            MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_PIC_SLIDE);

            //next mark picture
            if (NextMarkedPic(&mark_num,item_index,pic_num,ctrl_id,list_ctrl_id))
            {
                s_image_is_display = FALSE;
            }
            else
            {
                if ((0 == mark_num) && 
                    (NextSlidePic(item_index,pic_num,ctrl_id,list_ctrl_id)))
                {
                    s_image_is_display = FALSE;
                }
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_GET_FOCUS:
        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        if (s_image_is_display)
        {
            //start timer
            StartSlidePlayTimer(win_id);
        }
        break;

    case MSG_LOSE_FOCUS:
        //stop slide play timer
        StopSlidePlayTimer();

        //reset special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_NONE);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifdef EFFECT_RIPPLE_SUPPORT         
        MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
#endif        
        break;

    //case MSG_APP_OK:
    //case MSG_APP_WEB:
    case MSG_APP_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;
    case MSG_APP_WEB:
          MMK_CloseWin(win_id);
          if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
              MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
          }
          MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
	  break;		
/*
    case MSG_APP_RED:
        s_refresh_preview_anim = FALSE;
        break;
*/        
   case MSG_CLOSE_WINDOW:
        //stop slide play timer
        StopSlidePlayTimer();
        
        //reset special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_NONE);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);

#ifdef EFFECT_RIPPLE_SUPPORT  
        MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
#endif 
	    if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)  && s_refresh_preview_anim)
		{
	        MMIPICVIEW_RefreshAnimDisplay(item_index);
		}
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlidePlayAnim(
                               uint32           item_index, //from 0
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
//         if (!MMITHEME_IsSupportGsensor())
//         {
//             //set anim auto adapt display
//             GUIANIM_SetAutoAdaptDisplay(TRUE,ctrl_id);
//         }
        
        //set anim min period
        GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD,ctrl_id);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.bg.bg_type  = GUI_BG_COLOR;
        display_info.bg.color    = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;

#ifdef DRM_SUPPORT
		if(!MMIPICVIEW_IsPreview(FALSE, item_index))
		{
			GUIANIM_DATA_INFO_T data_info={0};
			data_info.img_id  = IMAGE_DRM_PIC_LOCK_ICON;
			display_info.is_update = TRUE;
			anim_result = GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
		}
		else
#endif
		{
			anim_info.full_path_wstr_ptr = file_info.filename;
			anim_info.full_path_wstr_len = file_info.name_len;
			anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
		}

        if ((GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : start slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartSlidePlayTimer(
                               MMI_WIN_ID_T   win_id
                               )
{
    uint32  time_out = 0;

    if (0 == s_mmipicview_slide_play_timer_id)
    {
        //get slide time
        time_out = GetSlideTime();
        s_mmipicview_slide_play_timer_id = MMK_CreateWinTimer(win_id,time_out,FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"StartSlidePlayTimer:don't creat slide play timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1719_112_2_18_2_46_44_79,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopSlidePlayTimer(void)
{
    if (0 < s_mmipicview_slide_play_timer_id)
    {
        MMK_StopTimer(s_mmipicview_slide_play_timer_id);
        s_mmipicview_slide_play_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : get the first mark picture index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetFirstMarkIndex(//is mark
                                uint32          *index_ptr,     //in[current index]/out[the first mark index]
                                uint32          toatal_num,     //in
                                MMI_CTRL_ID_T   list_ctrl_id    //in
                                )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_find = FALSE;
    uint16      i = 0;
    uint16      mark_num = 0;
    uint16      *mark_index_ptr = PNULL;

    if(PNULL == index_ptr)
    {
        return FALSE;
    }

    if (1 < toatal_num)
    {
        //alloc
        mark_index_ptr = SCI_ALLOC(toatal_num*sizeof(uint16));
        SCI_MEMSET(mark_index_ptr,0,(toatal_num*sizeof(uint16)));

        //get mark picture
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)toatal_num);
        if (0 < mark_num)
        {
            for (i=0; i<mark_num; i++)
            {
                //find the mark after current item
                if (mark_index_ptr[i] >= *index_ptr)
                {
                    *index_ptr = mark_index_ptr[i];
                    is_find = TRUE;
                    break;
                }
            }

            if (!is_find)
            {
                //no than from the first mark
                *index_ptr = mark_index_ptr[0];
            }

            result = TRUE;
        }

        //free mark index
        SCI_FREE(mark_index_ptr);
        mark_index_ptr = PNULL;
    }

    return (result);
}

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextSlidePic(//is change
                           uint32            cur_index,          //in:from
                           uint32            total_num,          //in
                           MMI_CTRL_ID_T     ctrl_id,            //in
                           MMI_CTRL_ID_T     list_ctrl_id        //in
                           )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     set_result = FALSE;
	uint32      start_index = 0;

    if (0 == total_num)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW NextSlidePic total_num is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1812_112_2_18_2_46_44_80,(uint8*)"");
        return result;
    }

    if (1 < total_num)
    {
		//set start index
        start_index = cur_index;
		
        //set current index
        if (cur_index < (total_num - 1))
        {
            cur_index = cur_index + 1;
        }
        else
        {
            cur_index = 0;
        }
		
//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE,cur_index))
//        {
//            //set current index
//            if (cur_index < (total_num - 1))
//            {
//                cur_index = cur_index + 1;
//            }
//            else
//            {
//                cur_index = 0;
//            }
//            
//            if (start_index == cur_index)
//            {
//                return (result);
//            }
//        }
//#endif

        //set list index
        //MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
#ifdef EFFECT_RIPPLE_SUPPORT  
        if (MMIPICVIEW_EFFECT_RIPPLE == MMIAPIPICVIEW_GetSlideEffect())
        {
            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
        }

        if (MMIPICVIEW_EFFECT_ORDER == MMIAPIPICVIEW_GetSlideEffect())
        {
            MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
        }
#endif        
        //add pic
        //Modified by lijinghua
		cur_index = MMIAPIPICVIEW_GetRecentPicFileInfo(cur_index, total_num);
		if(cur_index != total_num){
                MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
	        set_result = SetSlidePlayAnim(cur_index,ctrl_id);
	        if (!set_result)
	        {
	            GUIANIM_UpdateDisplay(ctrl_id);
	        }
        
			result = TRUE;
		}
		//Modified by lijinghua end
    }


    return (result);
}
//zhuyongjing add FOR upload items
/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_MSA_Get_Token_For_Settings( int result, char*  buf, uint16 buflen)
{
#ifdef MSA_SUPPORT 
    SCI_TRACE_LOW("[MMIPIC]MMIPICVIEW_MSA_Get_Token_For_Settings: result=%d",result);
    if(result == MSA_OK){
        MMIAPIMSA_Exit();
        MMK_CreateWin((uint32 *)MMIPICVIEW_ONEDRIVE_WIN_TAB,PNULL);
    }
#endif
}

#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : Create one drive settings window
//  Global resource dependence : 
//  Author: zhuyongjing
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenOnedriveSettingsWin(void)
{

#if defined(CAMERAROLL_SUPPORT)
	if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
	{	//MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL, MMIPICVIEW_MSA_Get_Token_For_Settings);            
		 MMK_CreateWin((uint32 *)MMIPICVIEW_ONEDRIVE_WIN_TAB,PNULL);
	}else
#endif
	{
		MMIPIC_CreateOneDriveStartTextDialogWin();
	}
    
}
#endif

/*****************************************************************************/
//  Description : Create one drive settings window
//  Global resource dependence : 
//  Author: zhuyongjing
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPreviewWin(void)
{
    if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
        MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
    }
    MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
}
PUBLIC BOOLEAN NextMarkedPicEx(//is change
                            uint16          *mark_num_ptr,      //in/out:may PNULL
                            uint32          cur_index,          //in:from 0
                            uint32          toatal_num,         //in
                            MMI_CTRL_ID_T   list_ctrl_id        //in
                            )
{
    BOOLEAN     result = FALSE;
    uint16      j = 0;
    uint16      mark_num = 0;
    uint16      *mark_index_ptr = PNULL;
	uint32      start_index = 0;

    if (1 < toatal_num)
    {
        //alloc
        mark_index_ptr = SCI_ALLOC(toatal_num*sizeof(uint16));
        SCI_MEMSET(mark_index_ptr,0,(toatal_num*sizeof(uint16)));

        //set start index
        start_index = cur_index;
		
        //get mark picture
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)toatal_num);
        if (0 < mark_num)
        {
        for(j=0;j<toatal_num;j++){
            uint16      i = 0;
            for (i=0; i<mark_num; i++)
            {
                if (cur_index == mark_index_ptr[i])
                {
                    //set the next pic index
                     cur_index = mark_index_ptr[i];  
			//set list index
                    MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
                    if (PNULL != mark_num_ptr)
                    {
                    *mark_num_ptr = mark_num;
                    }
                    SCI_FREE(mark_index_ptr);
                    mark_index_ptr = PNULL;
                    result = TRUE;
                    return(result);

                }
              }
            cur_index++;
            }

            result = TRUE;
        }

        //free mark index
        SCI_FREE(mark_index_ptr);
        mark_index_ptr = PNULL;
    }

    if (PNULL != mark_num_ptr)
    {
        *mark_num_ptr = mark_num;
    }

    return (result);
}
#ifdef CAMERAROLL_SUPPORT
LOCAL MMI_RESULT_E HandleDownloadProcessWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
    FILEARRAY_DATA_T    file_info = {0};
    MMI_STRING_T                title_str = {0};
    uint16                             cur_index = 0;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    
    SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg msg_id=%x",msg_id);
     
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, FALSE);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_DOWNLOAD_PHOTO);
        break;

    case MSG_CAMERA_ROLL_INIT_CNF:
        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_CAMERA_ROLL_INIT_CNF s_mmipicview_current_download_pic_idx=%d",s_mmipicview_current_download_pic_idx);
	if (MMIPICVIEW_GetPicFileInfo(s_mmipicview_current_download_pic_idx,&file_info))
	{
            uint16       w_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};
            uint16      w_file_name_len=0;
			
            MMIAPIFMM_SplitLastName(file_info.filename, &file_info.name_len,w_file_name, &w_file_name_len);
			
            MMIPICVIEW_GetFixedFolderPhotoPath(
                    MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk()),
                    MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk()),
                    CAMERA_ROLL_SOURCE_DONWLOADED,
                    &download_path);
			
            MMIAPICOM_Wstrcat(download_path.path, L"\\");
            MMIAPICOM_Wstrcat(download_path.path, w_file_name);
            download_path.path_len = MMIAPICOM_Wstrlen(download_path.path);
            if(!MMISRV_CAMERAROLL_Download_Original_photo(download_path.path,download_path.path_len)){
                if(MMK_IsOpenWin(wait_win_id)){
                    MMIPUB_CloseProgressWin(&wait_win_id);
                }
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID)){
                    MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                }
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                    DisplaybannerInfo(MMIPICVIEW_PREVIEW_WIN_ID,STR_MSA_SYS_BUSY);
                }else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                    DisplaybannerInfo(MMIPICVIEW_LIST_WIN_ID,STR_MSA_SYS_BUSY);
                }    
            }
	}

        break;
    case MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_NOTIFY :
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
        uint8             progress_text[20] = {0};
        wchar           progress_text_wstr[40] = {0};
        MMI_STRING_T process_content_str = {0};
        
        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_NOTIFY download size=%d,total=%d",
            data->camera_roll_msg_data.downloaded_file_size,data->camera_roll_msg_data.total_file_size);
        if(MMK_IsOpenWin(wait_win_id)){
            MMIPUB_SetProgressTotalSize(&wait_win_id , data->camera_roll_msg_data.total_file_size);

            if(data->camera_roll_msg_data.downloaded_file_size<=data->camera_roll_msg_data.total_file_size)
            {
                MMIPUB_UpdateProgressBar(&wait_win_id, data->camera_roll_msg_data.downloaded_file_size);
            }
            SCI_MEMSET(progress_text,0,sizeof(progress_text));
            if( data->camera_roll_msg_data.total_file_size< 1024){
                sprintf((char*)progress_text, "%ldB", data->camera_roll_msg_data.total_file_size);
            }else if(( data->camera_roll_msg_data.total_file_size < 1024*1024)&& ( data->camera_roll_msg_data.total_file_size >= 1024)){
                sprintf((char*)progress_text, "%ld.%01ldk",(data->camera_roll_msg_data.total_file_size/1024),(( data->camera_roll_msg_data.total_file_size%1024)*1000/1024));
            }else{
                sprintf((char*)progress_text, "%ld.%01ldM",( data->camera_roll_msg_data.total_file_size/(1024*1024)),(( data->camera_roll_msg_data.total_file_size%(1024*1024))*1000/1024));
            }
            MMIAPICOM_StrToWstr((const char *)progress_text, progress_text_wstr);
            process_content_str.wstr_ptr = progress_text_wstr;
            process_content_str.wstr_len = MMIAPICOM_Wstrlen(progress_text_wstr);
            MMIPUB_SetProgressContentByTextPtr(&wait_win_id,&process_content_str,TRUE);
        }
    }
    break;
    case MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;		


        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF,done=%d",
            data->camera_roll_msg_data.bDone);

        if(MMK_IsOpenWin(wait_win_id)){
            if(data->camera_roll_msg_data.downloaded_file_size == data->camera_roll_msg_data.total_file_size){
                MMIPUB_CloseProgressWin(&wait_win_id);
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
                    GUIANIM_FILE_INFO_T     icon_file = {0};
                    icon_file.full_path_wstr_ptr = download_path.path;
                    icon_file.full_path_wstr_len = download_path.path_len;
                    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                    GUIICONLIST_SetTotalIcon(MMIPICVIEW_LIST_CTRL_ID, MMIPICVIEW_GetAllPicNum()+1);
                    GUIICONLIST_AppendIcon(MMIPICVIEW_GetAllPicNum(), MMIPICVIEW_LIST_CTRL_ID, PNULL, &icon_file);
                    GUIICONLIST_SetCurIconIndex(cur_index,  list_ctrl_id);
                    MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),download_path.path);
                }
                    //zhuyongjing add for search quickly
                    if(MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE){
                        MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetBackupAllPicArray(), download_path.path);
                    }
                    //add end
                MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                    MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF,  &data);
                }else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                    DisplaybannerInfo(MMIPICVIEW_LIST_WIN_ID,STR_GAL_NOTE_DOWN_SUC_EXT02);
                }
                SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg cnf finish cur_index=%d,total=%d",cur_index,MMIPICVIEW_GetAllPicNum());
            }
        }
    }
    break;
    case MSG_CAMERA_ROLL_ERROR_NOTIFY:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;		
        ONE_DRIVE_ERROR_E err_trype = data->camera_roll_msg_data.error_type;
        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_CAMERA_ROLL_ERROR_NOTIFY,done=%d,err_trype=%d",data->camera_roll_msg_data.bDone,err_trype);
        if(err_trype==ONE_DRIVE_ERROR_NO_NETWORK ||err_trype==ONE_DRIVE_ERROR_HTTP_ERROR){
            if(MMK_IsOpenWin(wait_win_id)){
                MMIPUB_CloseProgressWin(&wait_win_id);
            }
            MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
            if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                DisplaybannerInfo(MMIPICVIEW_PREVIEW_WIN_ID,STR_ONEDRIVE_DOWNLOAD_FAILED_EXT01);
            }else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                DisplaybannerInfo(MMIPICVIEW_LIST_WIN_ID,STR_ONEDRIVE_DOWNLOAD_FAILED_EXT01);
            }
        }
    }
    break;
    case MSG_APP_CANCEL:
        MMISRV_CAMERAROLL_Cancel_Download();
        MMIPUB_CloseProgressWin(&wait_win_id);
        DisplaybannerInfo(MMIPICVIEW_PREVIEW_OPT_WIN_ID,STR_DOWNLOAD_CANCELLED_EXT01);
        break;
    case MSG_APP_WEB:
    case MSG_APP_OK:
        result = MMI_RESULT_FALSE;
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

LOCAL MMI_RESULT_E HandleDownloadWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    uint32              time_out = 0;
    uint32              deleted_num = 0;
    uint32              select_num = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint32                             pic_num = 0;
    uint16                             mark_num = 0;
    wchar           progress_text_wstr[] = L" ";
    MMI_STRING_T process_content_str = {0};
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    s_mmipicview_current_download_pic_idx = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, FALSE);
        break;
        case MSG_APP_WEB:
            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
            if(sim_ok_num == 0){
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
                break;
            }
            //open wait
            MMIPUB_OpenProgressWinByTextId(STR_ID_UMMS_DOWNLOAD_IN_PROGRESS, &wait_win_id, PNULL, MMIPUB_SOFTKEY_DIALOG_OK, HandleDownloadProcessWinMsg);
            process_content_str.wstr_ptr = progress_text_wstr;
            process_content_str.wstr_len = MMIAPICOM_Wstrlen(progress_text_wstr);
            MMIPUB_SetProgressContentByTextPtr(&wait_win_id,&process_content_str,TRUE);
            MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
       MMK_CloseWin(win_id);
        break;
    case MSG_APP_OK:
        result = MMI_RESULT_FALSE;
    break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : load sharelink waiting by spinner win
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSharelinkWaitingWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    		result = MMI_RESULT_TRUE;
    MMI_HANDLE_T spinner_ctr_id = NULL;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};
    FILEARRAY_DATA_T    file_info = {0};
    MMIPICVIEW_SHARELINK_TYPE_E       by_app = (MMIPICVIEW_SHARELINK_TYPE_E)MMK_GetWinAddDataPtr(win_id);
          
    SCI_TRACE_LOW("[MMIPIC_WINTAB]  HandleSharelinkWaitingWinMsg msg_id=%x",msg_id);
    spinner_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPICVIEW_SHARELINK_WAITING_SPINNER_CTRL_ID);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (s_wait_ready_timer_id != 0)
        {
            MMK_StopTimer(s_wait_ready_timer_id);
            s_wait_ready_timer_id = 0;
        }
        s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);

        break;
    case MSG_GET_FOCUS:
        if (s_wait_ready_timer_id != 0)
        {
            MMK_StopTimer(s_wait_ready_timer_id);
            s_wait_ready_timer_id = 0;
        }
        s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);      
        break;
    case MSG_CAMERA_ROLL_INIT_CNF:
        if(MMIPICVIEW_GetPicFileInfo(MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID),&file_info)){
            MMISRV_CAMERAROLL_Get_Share_Link(file_info.filename, file_info.name_len);
        }
        break;
    case MSG_CAMERA_ROLL_SHARE_LINK_CNF:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
        MMI_STRING_T  sms_init_text = {0};

        SCI_TRACE_LOW("[MMIPIC_WINTAB]:MMIPICVIEW HandleSharelinkWaitingWinMsg MSG_CAMERA_ROLL_SHARE_LINK_CNF " );
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        MMK_CloseWin(win_id);
        if(data->camera_roll_msg_data.bDone){
            if(by_app == MMIPICVIEW_SHARELINK_BY_MMS){              
                wchar tmp_str[CAMERA_ROLL_MAX_STRING_LEN] = {0};
                MMIAPICOM_StrToWstr(data->camera_roll_msg_data.url , tmp_str);
                sms_init_text.wstr_ptr =  tmp_str;
                sms_init_text.wstr_len = MMIAPICOM_Wstrlen(sms_init_text.wstr_ptr);
                MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &sms_init_text, FALSE,PNULL, 0);
                
            }else if(by_app == MMIPICVIEW_SHARELINK_BY_EMAIL){
                wchar*  outlook_file_name = L"outlookgalleryshare.txt";
                wchar*  outlook_file_full_path = PNULL;
                MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
                wchar  *file_dev_ptr = PNULL;
                uint16 file_dev_len = 0;
                uint16 outlook_file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
                MMIFILE_HANDLE  des_file_handle = 0;
                MMIFILE_ERROR_E des_file_ret = 0;
                wchar    full_file_name [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
                wchar slash[2] = {0x005c, 0};
                wchar new_split[2] = L"/";
                char     temp_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
                char     prop_file_path [MMIFILE_FULL_PATH_MAX_LEN+1 + 10] = {0};
                MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
                if (MMIAPIUDISK_UdiskIsRun()) //Udisk is using
                {
                    MMIPUB_OpenAlertWarningWin (TXT_COMMON_UDISK_USING);
                    break;
                }
                outlook_file_full_path = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
                if(outlook_file_full_path == PNULL)
                {
                    break;
                }
                SCI_MEMSET(outlook_file_full_path, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
                file_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
                file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
                file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
                MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                    PNULL, 0,
                    outlook_file_name,
                    MMIAPICOM_Wstrlen(outlook_file_name),
                    (wchar *)outlook_file_full_path,
                    &outlook_file_full_path_len);
                des_file_handle = MMIAPIFMM_CreateFile(outlook_file_full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                if(0 != des_file_handle)
                {
                    uint32 rw_size  = 0;
                    wchar* content = PNULL;
                    uint32 utf8_content_len = 0;
                    uint8* utf8_content=PNULL;
                    wchar wurl[CAMERA_ROLL_MAX_STRING_LEN+1] = {0};
                    content = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
                    utf8_content = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);
                    if(content == PNULL)
                    {
                        if(PNULL != outlook_file_full_path)
                        {
                            SCI_FREE(outlook_file_full_path);
                            outlook_file_full_path = PNULL;
                        }
                        MMIAPIFMM_CloseFile(des_file_handle);
                        break;
                    }
                    if(utf8_content == PNULL)
                    {
                        if(PNULL != outlook_file_full_path)
                        {
                            SCI_FREE(outlook_file_full_path);
                            outlook_file_full_path = PNULL;
                        }
                        MMIAPIFMM_CloseFile(des_file_handle);
                        break;
                    }
                    if(data->camera_roll_msg_data.url_len == 0){
                        if(PNULL != outlook_file_full_path)
                        {
                            SCI_FREE(outlook_file_full_path);
                            outlook_file_full_path = PNULL;
                        }
                        MMIAPIFMM_CloseFile(des_file_handle);
                        SCI_FREE(content);
                        content = PNULL;
                        SCI_FREE(utf8_content);
                        utf8_content = PNULL;              
                        break;
                    }
                    SCI_MEMSET(content, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
                    SCI_MEMSET(utf8_content, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);
                    MMIAPICOM_Wstrcat(content, L"Gallery\n");
                    MMIAPICOM_Wstrcat(content,L"url://");
                    MMIAPICOM_StrToWstr(data->camera_roll_msg_data.url,wurl);
                    MMIAPICOM_Wstrcat(content,wurl);
                    utf8_content_len = GUI_WstrToUTF8(utf8_content, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2, content, MMIAPICOM_Wstrlen(content));
                    des_file_ret = MMIAPIFMM_WriteFile(des_file_handle, utf8_content, utf8_content_len, &rw_size, NULL);
                    if(PNULL != content)
                    {
                        SCI_FREE(content);
                        content = PNULL;
                    }
                    if(PNULL != utf8_content)
                    {
                        SCI_FREE(utf8_content);
                        utf8_content = PNULL;
                    }
                    if(SFS_ERROR_NONE != des_file_ret)
                    {
                        if(PNULL != outlook_file_full_path)
                        {
                            SCI_FREE(outlook_file_full_path);
                            outlook_file_full_path = PNULL;
                        }
                        MMIAPIFMM_CloseFile(des_file_handle);
                        break;
                    }
                    else if(rw_size != utf8_content_len)
                    {
                        if(PNULL != outlook_file_full_path)
                        {
                            SCI_FREE(outlook_file_full_path);
                            outlook_file_full_path = PNULL;
                        }
                        MMIAPIFMM_CloseFile(des_file_handle);
                        break;
                    }
                    else
                    {
#ifdef JAVA_SUPPORT_MYRIAD
                        strcat(prop_file_path, "file://");
                        MMIAPICOM_WstrReplace(outlook_file_full_path,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1), slash, new_split);
                        MMIAPICOM_WstrToStr(outlook_file_full_path,temp_path);
                        strcat(prop_file_path,temp_path);

                        MMIJAVA_SetAppProperty("com.microsoft.outlook.share",prop_file_path);
                        MMIJAVA_EnvironmentInit();
                        MMIJAVA_LaunchMIDletSuiteByWnsPushMessage("MicrosoftFeaturePhoneOutlook781994");
#endif
                    }
                    MMIAPIFMM_CloseFile(des_file_handle);
                }
                if(PNULL != outlook_file_full_path)
                {
                    SCI_FREE(outlook_file_full_path);
                    outlook_file_full_path = PNULL;
                }
            }
        }
    }
    break;
    case MSG_CAMERA_ROLL_ERROR_NOTIFY:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
        ONE_DRIVE_ERROR_E err_trype = data->camera_roll_msg_data.error_type;
        SCI_TRACE_LOW("[MMIPIC_WINTAB]  HandleSharelinkWaitingWinMsg err_trype=%d",err_trype);

        if((err_trype == ONE_DRIVE_ERROR_NO_NETWORK
            ||err_trype == ONE_DRIVE_ERROR_HTTP_ERROR)
            &&0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
            MMK_CloseWin(win_id);
            DisplaybannerInfo(win_id,STR_NO_DATA_CONNECTION);
        }
        }
        break;

    case MSG_TIMER:  
        if(MMK_IsFocusWin(win_id))
        {
            SCI_TRACE_LOW("[MMIPIC_WINTAB]  HandleSharelinkWaitingWinMsg s_wait_ready_timer_id=%d,para=%d",s_wait_ready_timer_id,*((uint8 *) param));
        }
        break;
    case MSG_APP_RED:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
            MMK_CloseWin(win_id);
        }
        result = MMI_RESULT_FALSE;
        break;
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        break;
    default:
        break;
    }
    return (result);
}

LOCAL void MMIPICVIEW_Sharelink(uint32  item_index,MMIPICVIEW_SHARELINK_TYPE_E by_app){
    FILEARRAY_DATA_T    file_info = {0};
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;
    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if(sim_ok_num == 0){
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info)){
        MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_GET_SHARELINK);
        MMK_CreateWin((uint32 *)MMIPICVIEW_SHARELINK_WAITING_WIN_TAB,(ADD_DATA)by_app);
    }
}
#endif
//add end
/*****************************************************************************/
//  Description : switch the next marked picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextMarkedPic(//is change
                            uint16          *mark_num_ptr,      //in/out:may PNULL
                            uint32          cur_index,          //in:from 0
                            uint32          toatal_num,         //in
                            MMI_CTRL_ID_T   ctrl_id,            //in
                            MMI_CTRL_ID_T   list_ctrl_id        //in
                            )
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;
    uint16      mark_num = 0;
    uint16      *mark_index_ptr = PNULL;
	uint32      start_index = 0;

    if (1 < toatal_num)
    {
        //alloc
        mark_index_ptr = SCI_ALLOC(toatal_num*sizeof(uint16));
        SCI_MEMSET(mark_index_ptr,0,(toatal_num*sizeof(uint16)));

        //set start index
        start_index = cur_index;
		
        //get mark picture
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)toatal_num);
        if (1 < mark_num)
        {
            for (i=0; i<mark_num; i++)
            {
                if (cur_index == mark_index_ptr[i])
                {
                    //set the next pic index
                    if (i < (mark_num-1))
                    {
                        cur_index = mark_index_ptr[i+1];
                    }
                    else
                    {
                        cur_index = mark_index_ptr[0];
                    }

//#ifdef DRM_SUPPORT
//                    if ((MMIPICVIEW_IsPreview(FALSE,cur_index)) &&
//                        (cur_index != start_index))
//#endif
					{
						//set list index
						MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
#ifdef EFFECT_RIPPLE_SUPPORT  
                        if (MMIPICVIEW_EFFECT_RIPPLE == MMIAPIPICVIEW_GetSlideEffect())
                        {
                            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                        }

                        if (MMIPICVIEW_EFFECT_ORDER == MMIAPIPICVIEW_GetSlideEffect())
                        {
                            MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                        }
#endif        

						//set the next pic
						if (!SetSlidePlayAnim(cur_index,ctrl_id))
						{
							GUIANIM_UpdateDisplay(ctrl_id);
						}
						break;
					}
                }
            }

            result = TRUE;
        }
        
        //free mark index
        SCI_FREE(mark_index_ptr);
        mark_index_ptr = PNULL;
    }

    if (PNULL != mark_num_ptr)
    {
        *mark_num_ptr = mark_num;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32              cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL1_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = MENU_PICVIEW_PREVIEW_OPT;
    FILEARRAY_DATA_T        file_info = {0};//zhuyongjing add 20150722
    BOOLEAN close_win = TRUE;
	//lijinghua add for pic rename
    MMI_STRING_T dialog_title_str = {0};
    MMI_WIN_ID_T rename_dialog_id = MMIPICVIEW_RENAME_WIN_ID;
    //lijinghua end
    //SCI_TRACE_LOW:"MMIPICVIEW HandlePreviewOptWinMsg msg_id = 0x%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1987_112_2_18_2_46_45_81,(uint8*)"d", msg_id);
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
      //  SetGrayPreviewOptMenu(list_ctrl_id);
        //zhuyongjing add for gallary option menu 20150713
        CreatePreviewOptMenuCtrl(win_id,ctrl_id);
        //add end
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIMENU_SetState(ctrl_id,GUIMENU_STATE_CUSTOMER_TITLE,TRUE);
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;

    //case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        menu_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (menu_id)
        {
        case ID_PICVIEW_MMS:
            //close preview win
            //MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);//delete by zhuyongjing

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

#ifdef FM_SUPPORT
            if(MMIAPIFM_IsFmActive()) 
            {
                SCI_TRACE_LOW("HandlePreviewOptWinMsg FM is avtiving!!");
                MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
            }
            else
            {
                SendPicByBt(cur_index);
            }
#else
            SendPicByBt(cur_index);
#endif

            break;
 #ifdef CAMERAROLL_SUPPORT		
        case ID_PICVIEW_EMAIL:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByEmail(cur_index);
            break;
         case ID_PICVIEW_SHARELINK_BY_SMS:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            MMIPICVIEW_Sharelink(cur_index,MMIPICVIEW_SHARELINK_BY_MMS);
            break;
         case ID_PICVIEW_SHARELINK_BY_EMAIL:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            MMIPICVIEW_Sharelink(cur_index,MMIPICVIEW_SHARELINK_BY_EMAIL);
            break;
#endif
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
				if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
				{
		             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
				}
				
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif    

        case ID_PICVIEW_SLIDE_PREVIEW:
            //close preview win
//             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);

            MMIPICVIEW_SliePlay(list_ctrl_id);
            break;
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
#endif
        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            //MMK_CloseWin(win_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif
#ifdef PICVIEW_VIEW_ZOOM

        case ID_PICVIEW_ZOOM:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
	        ZoomPic(cur_index);
            break;
#endif
#ifdef PIC_EDITOR_SUPPORT
        //case ID_PICVIEW_EDIT:
        case PICVIEW_PICVIEW_OPT_EDIT:
            //close preview win
            //MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);//zhuyongjing delete

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;
#endif
       // case ID_PICVIEW_DELETE:
        case PICVIEW_PICVIEW_OPT_DELETE:
		//zhuyongjing changed for delete 20150722
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            if(MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)){
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
#if defined(CAMERAROLL_SUPPORT)	 
                if(TRUE == MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(file_info.filename,file_info.name_len)){
                    MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandlePicPreviewDelQueryWin);
                }else
#endif
                {
            	//prompt query window
            	    //MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
            	    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_ONEDRIVE_NOTE_SAVING_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandlePicPreviewDelQueryWin);
                }
               /* if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
                }*/
            }
            else
#endif
            {
#if defined(CAMERAROLL_SUPPORT)	 
                if(TRUE == MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(file_info.filename,file_info.name_len)){
                    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_GAL_NOTE_DELETE_PHOTO_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandlePicPreviewDelQueryWin);
                }else
#endif
			{
            	    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_DELETE_SELECTED_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandlePicPreviewDelQueryWin);
                }
            }
            close_win = FALSE;
	}		//change end
            break;

        //case ID_PICVIEW_RENAME:
         case PICVIEW_PICVIEW_OPT_RENAME:
		    //lijinghua modify for pic rename
 #ifdef ZDT_PRODUCT_Z685S
		MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
	#else
            MMI_GetLabelTextByLang(STR_GAL_TITLE_RENAME_EXT01, &dialog_title_str);
            MMIPUB_OpenDialogInputFieldWinByTextPtr(
            &dialog_title_str,
            &rename_dialog_id,
            PNULL,
            MMIPUB_SOFTKEY_CUSTOMER,
            HandleRenameDialogWin
            );
	#endif
            close_win = FALSE;
            //MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
			//lijinghua modify end
            break;

        //case ID_PICVIEW_DETAIL:
        case PICVIEW_PICVIEW_OPT_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            close_win = FALSE;
            break;
            
// #ifdef PIC_PREVIEW_U200_SUPPORT
//         case ID_PICVIEW_SETTING:
//             //open setting win
//             MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
//             break;
// #endif
 //zhuyongjing add for gallary ui 20150713
        case PICVIEW_PICVIEW_OPT_SET_TO_LOCKSCREEN:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
        //case  ID_PICVIEW_SAVE_TO_ONEDRIVE:
      #ifdef CAMERAROLL_SUPPORT	   
        case PICVIEW_PICVIEW_OPT_SAVE_TO_ONEDRIVE:
        {
            MMI_WIN_ID_T confirm_id = MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID;
            if(IsAccountCIDExist()){
                MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_ONEDRIVE_NOTE_SAVING_EXT01,PNULL,PNULL,&confirm_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleUploadOnePicture);
            }else{
                DisplaybannerInfo(win_id,STR_MSA_SIGNIN_UNSUCCESS);
            }
        }
            break;
        case  PICVIEW_PICVIEW_OPT_ONEDRIVE_SHARE_LINK:
        {

        }
        break;
        case  PICVIEW_PICVIEW_OPT_ONEDRIVE_DOWNLOAD:
        {
            MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
            MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_ONEDRIVE_NOTE_DOWNLOAD_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleDownloadWin);
            //MMIPUB_OpenQueryWinByTextId(STR_ONEDRIVE_NOTE_DOWNLOAD_EXT01,IMAGE_PUBWIN_QUERY,&query_win_id,HandleDownloadWin);      
            close_win = FALSE;
        }
            break;
	#endif
//add end
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePreviewOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2125_112_2_18_2_46_45_82,(uint8*)"d",menu_id);
            break;
        }
        if(close_win){
        MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef SNS_SUPPORT
    case MSG_SNS_OPERA_RESULT:
        {
            if(pMe)
            {
                IMMISNS_Release(pMe);
                pMe = PNULL;
            }
        }
        break;
#endif     
    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set preview opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayPreviewOptMenu(
                                 MMI_CTRL_ID_T      list_ctrl_id
                                 )
{
    uint16                  cur_index = 0;
    GUIANIM_TYPE_E          img_type = GUIANIM_TYPE_NONE;
    FILEARRAY_DATA_T        file_info = {0};
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_PREVIEW_OPT;
    
    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    
    //get picture info,only jpg support picture editor
    if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
    {
        //get image type
        img_type = MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len);

#ifdef PICVIEW_VIEW_ZOOM        
        if (GUIANIM_TYPE_IMAGE_GIF == img_type)
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,TRUE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,FALSE);
        }
#endif
        if (GUIANIM_TYPE_IMAGE_JPG == img_type)
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,TRUE);
        }
    }
    else
    {
#ifdef PICVIEW_VIEW_ZOOM    
        GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,TRUE);
#endif        
        GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,TRUE);
    }
}

/*****************************************************************************/
//  Description : send picture by mms
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByMms(
                        uint32  item_index  //from 0
                        )
{
#ifdef MMS_SUPPORT
    FILEARRAY_DATA_T    pic_file_info = {0};
    MMIFILE_FILE_INFO_T file_info = {0};

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        //get picture info
        if( !MMIPICVIEW_GetPicFileInfo(item_index,&pic_file_info) )
        {
            return;
        }
#ifdef DRM_SUPPORT
        //is preview
        if (!MMIPICVIEW_IsSend(TRUE,&pic_file_info))
        {
            return;
        }
#endif

        MMIMMS_SetIsSharePicFlag(TRUE);
        MMIAPIFMM_GetFileInfoFormFullPath(pic_file_info.filename,pic_file_info.name_len,&file_info);
        if(MMIPICVIEW_IsVideoSuffix(&pic_file_info)){
            MMIAPIMMS_EditMMSFromFile(&file_info,MMIFMM_FILE_TYPE_MOVIE);
        }else{
            MMIAPIMMS_EditMMSFromFile(&file_info,MMIFMM_FILE_TYPE_PICTURE);
        }
        
    }
#endif
}

/*****************************************************************************/
//  Description : send picture by blue tooth
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByBt(
                       uint32  item_index  //from 0
                       )
{
#ifdef BLUETOOTH_SUPPORT    
    FILEARRAY_DATA_T    pic_info = {0};
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
	
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        //get picture info
       if (MMIPICVIEW_GetPicFileInfo(item_index,&pic_info))
       {
#ifdef DRM_SUPPORT
            //is preview
            if (!MMIPICVIEW_IsSend(FALSE,&pic_info))
            {
                return;
            }
#endif
            MMIAPICOM_Wstrncpy(send_file_info.filepath_name, pic_info.filename,pic_info.name_len);
            send_file_info.filepath_len = pic_info.name_len;
            send_file_info.file_size = pic_info.size;
            send_file_info.is_temp_file = FALSE;
            //send by bt
            MMIAPIBT_SendMultiFile(&send_file_info,1);	     		
        }
    }
#endif    
}

LOCAL void SendMultiItemByEmail(void)
{
    wchar*  outlook_file_name = L"outlookgalleryshare.txt";
    wchar*  outlook_file_full_path = PNULL;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
    wchar  *file_dev_ptr = PNULL;
    uint16 file_dev_len = 0;
    uint16 outlook_file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE  des_file_handle = 0;
    MMIFILE_ERROR_E des_file_ret = 0;
    wchar    full_file_name [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar    slash[2] = {0x005c, 0};
    wchar    new_split[2] = L"/";
    char     temp_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    char     path [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    char     prop_file_path [MMIFILE_FULL_PATH_MAX_LEN+1 + 10] = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    FILEARRAY_DATA_T    pic_info = {0};
    uint16      *mark_index_ptr = PNULL;
    uint32              total_num = MMIPICVIEW_GetAllPicNum();
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint16              i = 0;
    uint16              mark_num = 0;
    uint16 send_file_index=0;
        
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    mark_index_ptr = SCI_ALLOC(total_num*sizeof(uint16));
    if(mark_index_ptr == PNULL) 
        return;
    SCI_MEMSET(mark_index_ptr,0,(total_num*sizeof(uint16)));
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)total_num);
    outlook_file_full_path = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    if(outlook_file_full_path == PNULL)
    {
        SCI_FREE (mark_index_ptr);
        mark_index_ptr = PNULL;
            return;
    }
    SCI_MEMSET(outlook_file_full_path, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    file_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
        PNULL, 0,
        outlook_file_name,
        MMIAPICOM_Wstrlen(outlook_file_name),
        (wchar *)outlook_file_full_path,
        &outlook_file_full_path_len);
    des_file_handle = MMIAPIFMM_CreateFile(outlook_file_full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    if(0 != des_file_handle)
    {
        uint32 rw_size  = 0;
        wchar* content = PNULL;
        uint8* utf8_content=PNULL;
        uint32 utf8_content_len = 0;
        content = SCI_ALLOCA(total_num*sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
        utf8_content = SCI_ALLOCA(total_num*sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);
        
        if(content == PNULL)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            SCI_FREE (mark_index_ptr);
            mark_index_ptr = PNULL;
            return;
        }
        if(utf8_content == PNULL)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            SCI_FREE (mark_index_ptr);
            mark_index_ptr = PNULL;
            return;
        }
        SCI_MEMSET(content, 0x00, total_num*sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
        SCI_MEMSET(utf8_content, 0x00, total_num*sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);

        MMIAPICOM_Wstrcat(content, L"Gallery\n");
        for (i=0; i<mark_num; i++)
        {
            send_file_index = mark_index_ptr[i];
            if (MMIPICVIEW_GetPicFileInfo(send_file_index,&pic_info)){
#ifdef DRM_SUPPORT
                if (MMIAPIFMM_IsDrmFileBySuffix(pic_info.filename, pic_info.name_len))
                {	
                    if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, pic_info.filename))
                    {
                        MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                        SCI_FREE (mark_index_ptr);
                        mark_index_ptr = PNULL;
                        SCI_FREE(content);
                        content = PNULL;
                        SCI_FREE(utf8_content);
                        utf8_content = PNULL;
                        SCI_FREE(outlook_file_full_path);
                        outlook_file_full_path = PNULL;
                        return;
                    }
                }  
#endif      
                MMIAPICOM_Wstrcat(content, L"file://");
                MMIAPICOM_WstrReplace(pic_info.filename,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1), slash, new_split);
                MMIAPICOM_Wstrcat(content,pic_info.filename);
                if(i<mark_num-1){
                    MMIAPICOM_Wstrcat(content, L"\n");
                }
            }
        }
	
        utf8_content_len = GUI_WstrToUTF8(utf8_content, mark_num*sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2, content, MMIAPICOM_Wstrlen(content));
        des_file_ret = MMIAPIFMM_WriteFile(des_file_handle, utf8_content, utf8_content_len, &rw_size, NULL);
        if(PNULL != content)
        {
            SCI_FREE(content);
            content = PNULL;
        }
        if(PNULL != utf8_content)
        {
            SCI_FREE(utf8_content);
            utf8_content = PNULL;
        }
        if(SFS_ERROR_NONE != des_file_ret)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            SCI_FREE(mark_index_ptr);
            mark_index_ptr = PNULL;
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        else if(rw_size != utf8_content_len)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            SCI_FREE (mark_index_ptr);
            mark_index_ptr = PNULL;
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        else
        {
 #ifdef JAVA_SUPPORT_MYRIAD
            strcat(prop_file_path, "file://");
            MMIAPICOM_WstrReplace(outlook_file_full_path,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1), slash, new_split);
            MMIAPICOM_WstrToStr(outlook_file_full_path,temp_path);
            strcat(prop_file_path,temp_path);
            MMIJAVA_SetAppProperty("com.microsoft.outlook.share",prop_file_path);
            MMIJAVA_EnvironmentInit();
            MMIJAVA_LaunchMIDletSuiteByWnsPushMessage("MicrosoftFeaturePhoneOutlook781994");
#endif
        }
        MMIAPIFMM_CloseFile(des_file_handle);
    }
    if(PNULL != outlook_file_full_path)
    {
        SCI_FREE(outlook_file_full_path);
        outlook_file_full_path = PNULL;
    }
    SCI_FREE (mark_index_ptr);
    mark_index_ptr = PNULL;
}
LOCAL void SendMultiPicByBt()
{
#ifdef BLUETOOTH_SUPPORT    
    FILEARRAY_DATA_T    pic_info = {0};
    MMIFMM_BT_SENDFILE_INFO_T   *send_file_info = PNULL;
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint16                      send_file_num = 0;
	uint16              i = 0;
    uint16                      send_file_index = 0;
    uint16              mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    uint32              total_num = MMIPICVIEW_GetAllPicNum();
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        //get picture info
		send_file_num = mark_num;

		if(send_file_num > MMIBT_SENDFILE_MAX_NUM)//CR178620
		{
			MMIPUB_OpenAlertWarningWin(TXT_BT_SENDFILE_MAX_NUMBER);
			return;
		}

		send_file_info = (MMIFMM_BT_SENDFILE_INFO_T *) SCI_ALLOCA (send_file_num * sizeof (MMIFMM_BT_SENDFILE_INFO_T));
		
		if (PNULL == send_file_info)
		{
			return;
		}
		
		SCI_MEMSET (send_file_info, 0, send_file_num * sizeof (MMIFMM_BT_SENDFILE_INFO_T));

        //get picture info
        for(i = 0; i< total_num; i ++)
        {
           if(MMIPICVIEW_IsSelectItem(list_ctrl_id,i)){
                if (MMIPICVIEW_GetPicFileInfo(i,&pic_info)){
			                  //check if contails any empty file
                if (0 ==pic_info.size)
                {
                    MMIPUB_OpenAlertWarningWin(STR_FILE_EMPTY);
                    SCI_FREE (send_file_info);
                    send_file_info = PNULL;
                    return;
                }
                //检查发送的文件中是否含有DRM未授权文件
#ifdef DRM_SUPPORT
                if (MMIAPIFMM_IsDrmFileBySuffix(pic_info.filename, pic_info.name_len))
                {	
                    //只要不能发送，则立马提示返回
                    if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, pic_info.filename))
                    {
                    	MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                        SCI_FREE (send_file_info);
                        send_file_info = PNULL;
                    	return;
                    }
                }  
#endif
#ifdef DRM_SUPPORT
				//is preview
				  if (!MMIPICVIEW_IsSend(FALSE,&pic_info))
				  {
                        SCI_FREE (send_file_info);
                        send_file_info = PNULL;
                        return;
		  }
#endif        
		  MMIAPICOM_Wstrncpy(send_file_info[send_file_index].filepath_name, pic_info.filename,pic_info.name_len);

		  send_file_info[send_file_index].filepath_len = pic_info.name_len;
		  send_file_info[send_file_index].file_size = pic_info.size;
		  send_file_info[send_file_index].is_temp_file = FALSE;
                    send_file_index++;

			  }
		  }
		}
		

            SCI_TRACE_LOW("[mmipic_view]SendMultiPicByBt send_file_index =%d, send_file_num = %d,total=%d",send_file_index, send_file_num,total_num);
            MMIAPIBT_SendMultiFile (send_file_info, send_file_num);
            SCI_TRACE_LOW("[mmipic_view]SendMultiPicByBt send_file_num end");

            SCI_FREE (send_file_info);
            send_file_info = PNULL;

    }
#endif
}
LOCAL void SendPicByEmail(
                       uint32  item_index  //from 0
                       )
{
    wchar*  outlook_file_name = L"outlookgalleryshare.txt";
    wchar*  outlook_file_full_path = PNULL;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
    wchar  *file_dev_ptr = PNULL;
    uint16 file_dev_len = 0;
    uint16 outlook_file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE  des_file_handle = 0;
    MMIFILE_ERROR_E des_file_ret = 0;
    wchar    full_file_name [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar    slash[2] = {0x005c, 0};
    wchar    new_split[2] = L"/";
    char     temp_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    char     path [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    char     prop_file_path [MMIFILE_FULL_PATH_MAX_LEN+1 + 10] = {0};
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    FILEARRAY_DATA_T    pic_info = {0};

    if (MMIAPIUDISK_UdiskIsRun()) //U?¨?¨o1¨???D
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
//get picture info
    outlook_file_full_path = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    if(outlook_file_full_path == PNULL)
    {
        return;
    }
    SCI_MEMSET(outlook_file_full_path, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
    file_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
        PNULL, 0,
        outlook_file_name,
        MMIAPICOM_Wstrlen(outlook_file_name),
        (wchar *)outlook_file_full_path,
        &outlook_file_full_path_len);
    des_file_handle = MMIAPIFMM_CreateFile(outlook_file_full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    if(0 != des_file_handle)
    {
        uint32 rw_size  = 0;
        wchar* content = PNULL;
        uint8* utf8_content=PNULL;
        uint32 utf8_content_len = 0;
        content = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
        utf8_content = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);
        if(content == PNULL)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        if(utf8_content == PNULL)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        if(!MMIPICVIEW_GetPicFileInfo(item_index,&pic_info)){
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            SCI_FREE(content);
            content = PNULL;
            SCI_FREE(utf8_content);
            utf8_content = PNULL;
            return;
        }
#ifdef DRM_SUPPORT
        if (MMIAPIFMM_IsDrmFileBySuffix(pic_info.filename, pic_info.name_len))
        {
            if (!MMIAPIDRM_IsCanForward (PNULL, SFS_INVALID_HANDLE, pic_info.filename))
				{
                MMIPUB_OpenAlertWarningWin (TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                if(PNULL != outlook_file_full_path)
                {
                    SCI_FREE(outlook_file_full_path);
                    outlook_file_full_path = PNULL;
                }
                SCI_FREE(content);
                content = PNULL;
                SCI_FREE(utf8_content);
                utf8_content = PNULL;
                MMIAPIFMM_CloseFile(des_file_handle);
                return;
            }
        }
#endif
        SCI_MEMSET(content, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20));
        SCI_MEMSET(utf8_content, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2);

        MMIAPICOM_WstrReplace(pic_info.filename,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1), slash, new_split);
        MMIAPICOM_Wstrcat(content, L"Gallery\n");
        MMIAPICOM_Wstrcat(content,L"file://");
        MMIAPICOM_Wstrcat(content,pic_info.filename);
        utf8_content_len = GUI_WstrToUTF8(utf8_content, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1 + 20)*2, content, MMIAPICOM_Wstrlen(content));

        des_file_ret = MMIAPIFMM_WriteFile(des_file_handle, utf8_content, utf8_content_len, &rw_size, NULL);
        if(PNULL != content)
        {
            SCI_FREE(content);
            content = PNULL;
        }
        if(PNULL != utf8_content)
        {
            SCI_FREE(utf8_content);
            utf8_content = PNULL;
        }
        if(SFS_ERROR_NONE != des_file_ret)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        else if(rw_size != utf8_content_len)
        {
            if(PNULL != outlook_file_full_path)
            {
                SCI_FREE(outlook_file_full_path);
                outlook_file_full_path = PNULL;
            }
            MMIAPIFMM_CloseFile(des_file_handle);
            return;
        }
        else
        {
#ifdef JAVA_SUPPORT_MYRIAD
            strcat(prop_file_path, "file://");
            MMIAPICOM_WstrReplace(outlook_file_full_path,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1), slash, new_split);
            MMIAPICOM_WstrToStr(outlook_file_full_path,temp_path);
            strcat(prop_file_path,temp_path);
            MMIJAVA_SetAppProperty("com.microsoft.outlook.share",prop_file_path);
            MMIJAVA_EnvironmentInit();
            MMIJAVA_LaunchMIDletSuiteByWnsPushMessage("MicrosoftFeaturePhoneOutlook781994");
#endif
        }
        MMIAPIFMM_CloseFile(des_file_handle);
    }
    if(PNULL != outlook_file_full_path)
    {
        SCI_FREE(outlook_file_full_path);
        outlook_file_full_path = PNULL;
    }

}

#ifdef SNS_SUPPORT

/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandlePicViewWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    int            err             = 0;
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    widget_id = pubwin_info_ptr->user_data;
    
    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                IMMISNS_AppInstall(pMe, widget_id);
                
                IMMISNS_Release(pMe);
                pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : send pic to sns
//  Global resource dependence : 
//  Author: weiren
//  Note: 
/*****************************************************************************/
LOCAL void SendPicToSNS(
                       uint32  item_index ,
                       uint32  sns_index  //from 0
                       )
{
    FILEARRAY_DATA_T  pic_info = {0};
    MMI_STRING_T     pic_file_info = {0};
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        //get picture info
        if (MMIPICVIEW_GetPicFileInfo(item_index,&pic_info))
        {
#ifdef DRM_SUPPORT
            //is preview
            if (!MMIPICVIEW_IsSend(FALSE,&pic_info))
            {
                return;
            }
#endif
            //send to sns

            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                uint32        time_num    = 500;
                
                pic_file_info.wstr_ptr = (wchar*)SCI_ALLOC_APP(sizeof(wchar) *(pic_info.name_len +1));

                if(PNULL == pic_file_info.wstr_ptr)
                {
                    return ;
                }

                SCI_MEMSET(pic_file_info.wstr_ptr, 0, sizeof(wchar) *(pic_info.name_len+1));

                MMIAPICOM_Wstrncpy((wchar*)pic_file_info.wstr_ptr, pic_info.filename, pic_info.name_len); 	
                pic_file_info.wstr_len = pic_info.name_len;

                IMMISNS_ShareToSns(pMe, sns_index, SNS_MODULE_IMAGE_BROWSER, (uint32)&pic_file_info);

                SCI_FREE(pic_file_info.wstr_ptr);
                pic_file_info.wstr_ptr = PNULL;
                MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

            }
        }
    }
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle slide set win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlideSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set slide set
        SetSlideSettings();
        MMK_SetAtvCtrl(win_id,MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID);
        break;

    case MSG_CTL_OK:
        SaveSlideSet();
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set slide set param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideSettings(void)
{
    uint16                  cur_index = 0;
    uint32                  i = 0;
    MMI_TEXT_ID_T           effect_text[MMIPICVIEW_EFFECT_TYPE_MAX] = {TXT_PICVIEW_EFFECT_ORDER,
                                             TXT_PICVIEW_EFFECT_ENLARGE,
                                             TXT_PICVIEW_EFFECT_GRID,
                                             TXT_PICVIEW_EFFECT_LEFTWARD,
                                             TXT_PICVIEW_EFFECT_RIGHTWARD,
                                             TXT_PICVIEW_EFFECT_UPWARD,
                                             TXT_PICVIEW_EFFECT_DOWNWARD,
#ifdef EFFECT_RIPPLE_SUPPORT                                               
                                             TXT_PICVIEW_EFFECT_RIPPLE,                                             
#endif                                             
                                             TXT_NONE};
    MMI_TEXT_ID_T           time_text[MMIPICVIEW_SLIDE_TIME_NUM] = {TXT_COMM_2_SECONDS,
                                           TXT_COMM_3_SECONDS,
                                           TXT_COMM_6_SECONDS,
                                           TXT_COMM_10_SECONDS,
                                           TXT_PICVIEW_SLIDE_TIME_16S,
                                           TXT_COMM_20_SECONDS};
    MMI_CTRL_ID_T           effect_form_id = MMIPICVIEW_EFFECT_FORM_CTRL_ID;
    MMI_CTRL_ID_T           effect_ctrl_id = MMIPICVIEW_EFFECT_LABEL_CTRL_ID;
    MMI_CTRL_ID_T           effect_dropdown_id = MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID;
    MMI_CTRL_ID_T           time_form_id = MMIPICVIEW_TIME_FORM_CTRL_ID;
    MMI_CTRL_ID_T           time_ctrl_id = MMIPICVIEW_TIME_LABEL_CTRL_ID;
    MMI_CTRL_ID_T           time_dropdown_id = MMIPICVIEW_TIME_DROPDOWN_CTRL_ID;
    MMI_STRING_T            effect_item[MMIPICVIEW_EFFECT_TYPE_MAX] ={0};
    MMI_STRING_T            time_item[MMIPICVIEW_SLIDE_TIME_NUM] ={0};
    
#ifdef PIC_PREVIEW_U200_SUPPORT
    GUIFORM_SetType(MMIPICVIEW_SET_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif

    //set effect and time form style
    GUIFORM_SetStyle(effect_form_id,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(time_form_id,GUIFORM_STYLE_UNIT);

    //set effect name
    GUILABEL_SetTextById(effect_ctrl_id,TXT_EFFECT,FALSE);

    //set time name
    GUILABEL_SetTextById(time_ctrl_id,TXT_PICVIEW_SLIDE_TIME,FALSE);

    //get effect item
    for (i=0; i<MMIPICVIEW_EFFECT_TYPE_MAX; i++)
    {
        MMI_GetLabelTextByLang(effect_text[i],&effect_item[i]);
    }

    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(effect_dropdown_id,effect_item,MMIPICVIEW_EFFECT_TYPE_MAX);
    
    //set display ellipsis
    GUIDROPDOWNLIST_SetEllipsis(effect_dropdown_id,TRUE);
    
    //get current slide effect index
    cur_index = GetSlideEffectIndex();

    //set select effect item
    GUIDROPDOWNLIST_SetCurItemIndex(effect_dropdown_id,cur_index);

    //get time item
    for (i=0; i<MMIPICVIEW_SLIDE_TIME_NUM; i++)
    {
        MMI_GetLabelTextByLang(time_text[i],&time_item[i]);
    }

    //set time drop down list
    GUIDROPDOWNLIST_AppendItemArray(time_dropdown_id,time_item,MMIPICVIEW_SLIDE_TIME_NUM);

    //get current slide effect index
    cur_index = GetSlideTimeIndex();

    //set select time item
    GUIDROPDOWNLIST_SetCurItemIndex(time_dropdown_id,cur_index);

    //set display ellipsis
    GUIDROPDOWNLIST_SetEllipsis(time_dropdown_id,TRUE);
}

/*****************************************************************************/
//  Description : get slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideEffectIndex(void)
{
    uint16                      cur_index = 0;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_NONE;

    //get slide effect
    slide_effect = MMIAPIPICVIEW_GetSlideEffect();

    cur_index = (uint16)slide_effect;

    return (cur_index);
}

/*****************************************************************************/
//  Description : set slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideEffectIndex(
                               uint16   cur_index
                               )
{
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_NONE;

    if (MMIPICVIEW_EFFECT_TYPE_MAX <= cur_index)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW SetSlideEffectIndex cur_index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2536_112_2_18_2_46_46_83,(uint8*)"");
        return ;
    }

    slide_effect = (MMIPICVIEW_EFFECT_TYPE_E)cur_index;

    //set slide effect
    MMIPICVIEW_SetSlideEffect(slide_effect);
}
#endif
/*****************************************************************************/
//  Description : get slide time
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideTime(void) //return ms
{
    uint32                  time_out = 0;
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_GetSlideTime();

    time_out = s_mmipicview_slide_time[slide_time];

    return (time_out);
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void)
{
    uint16                  cur_index = 0;
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_TIME_2S;

    //get slide time
    slide_time = MMIPICVIEW_GetSlideTime();

    cur_index = (uint16)slide_time;

    return (cur_index);
}

/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(
                             uint16   cur_index
                             )
{
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_TIME_2S;

    if (MMIPICVIEW_TIME_TYPE_MAX <= cur_index)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW SetSlideTimeIndex cur_index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2589_112_2_18_2_46_46_84,(uint8*)"");
        return ;
    }

    slide_time = (MMIPICVIEW_TIME_TYPE_E)cur_index;

    //write slide time
    MMIPICVIEW_SetSlideTime(slide_time);
}

/*****************************************************************************/
//  Description : save slide set param
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SaveSlideSet(void)
{
    uint16          cur_index = 0;
    MMI_CTRL_ID_T   effect_dropdown_id = MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID;
    MMI_CTRL_ID_T   time_dropdown_id = MMIPICVIEW_TIME_DROPDOWN_CTRL_ID;

    //save slide effect
    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(effect_dropdown_id);
    SetSlideEffectIndex(cur_index);

    //save slide time
    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(time_dropdown_id);
    SetSlideTimeIndex(cur_index);
}
#endif
/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaper(
                        uint32  item_index, //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {       
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(STR_FILE_EMPTY);
            return;
        }
#ifdef DRM_SUPPORT
        //is set wallpaper
        if (!MMIPICVIEW_IsSetWallpaper(item_index))
        {
			MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_WALLPAPER);
            return;
        }
#endif
        //if (!MMIAPIFMM_SetPictureToWallpaper(FALSE,file_info.filename,file_info.name_len))
        if (!MMIAPIFMM_SetPictureToWallpaperEx(FALSE,file_info.filename,file_info.name_len))
        {
            //prompt fail
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
//#ifdef MMI_PICVIEW_MINI_SUPPORT
        /* add by langhua.yang for NEWMS00205097*/
        else
        {
        //zhuyongjing delete for keep banner from settings
		//lijinghua add it for pic set lock screen
          /*  MMICOM_BANNER_INFO_T banner_info = {0};
            banner_info.banner_type = MMI_BANNER_TEXT;
            banner_info.start_point.x = 0;
            banner_info.start_point.y = 0;
            banner_info.text1_id = STR_GAL_SET_SCREEN_DONE_EXT01;
            MMIAPICOM_OpenBannerWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID, banner_info);
            if (GUIANIM_TYPE_IMAGE_JPG != MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len))
            {
                s_mmipicview_is_set_wallpaper_complete=FALSE;  //lijinghua add
            }*/
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		//add end
        }
//#endif
    }
}

/*****************************************************************************/
//  Description : set power on/off anim
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetPowerAnim(
                        uint32  item_index,  //from 0
                        BOOLEAN is_power_on,
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {   
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(STR_FILE_EMPTY);
            return;
        }
        MMIAPIFMM_SetAsPowerOnOffAnim (is_power_on, file_info.filename, file_info.name_len);
    }
}
/*****************************************************************************/
//  Description : set picture anim control param
//  Global resource dependence :
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
BOOLEAN MMIPICVIEW_SetSelPicParam (
    const wchar          *full_path_wstr_ptr,//in
    uint16         full_path_wstr_len, //in:字节数
    MMI_CTRL_ID_T  ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    if (PNULL == full_path_wstr_ptr)
    {
        return result;
    }

    if (0 == full_path_wstr_len)
    {
        return result;
    }
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    control_info.ctrl_ptr = PNULL;

    file_info.full_path_wstr_ptr = full_path_wstr_ptr;
    file_info.full_path_wstr_len = full_path_wstr_len;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam (&control_info, PNULL, &file_info, &display_info);

    if ( (GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    return (result);
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set PB contact icon
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPhontbook(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T        file_info = {0};
    MMIPB_CUSTOM_PHOTO_T    data_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {   
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(STR_FILE_EMPTY);
            return;
        }
#ifdef DRM_SUPPORT
        //is set PB,限制相同与Wallpaper
        if (!MMIPICVIEW_IsSetWallpaper(item_index))
        {
			MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_PB);
            return;
        }
#endif
//#ifdef MMI_PIC_CROP_SUPPORT
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
		{
             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
		}
//#endif
        SCI_MEMSET(&data_info, 0, sizeof(MMIPB_CUSTOM_PHOTO_T));
        MMI_WSTRNCPY(data_info.file_name, MMIFILE_FULL_PATH_MAX_LEN, file_info.filename, MMIFILE_FULL_PATH_MAX_LEN, file_info.name_len);
        data_info.file_name_len = file_info.name_len;
        data_info.file_size     = file_info.size;
        data_info.is_valid      = TRUE;
        data_info.photo_type    = file_info.type;

        MMIAPIPB_AddToExistContact(&data_info,  MMIPB_ADD_TYPE_PIC, PNULL);
    }
}
#endif
#ifdef PICVIEW_VIEW_ZOOM
/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void ZoomPic(
                   uint32   item_index  //from 0
                   )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
	//lijinghua change it for pic zoom
    //#ifndef MMI_PICVIEW_MINI_SUPPORT 
        MMIPICVIEW_Zoom(file_info.filename,file_info.name_len);
    //#endif
    }
}
#endif

#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : edit picture
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void EditPic(
                   uint32  item_index  //from 0
                   )
{
    FILEARRAY_DATA_T    file_info = {0};
    s_editpic_index = item_index ;//add for NEWMS00108303
    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(STR_FILE_EMPTY);
            return;
        }
        if (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len))
        {
            MMIAPIPIC_OpenPicEditorMainWin(file_info.filename,file_info.name_len);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(STR_FILE_NOT_SUPPORT);
        }
    }
}
#endif
/*****************************************************************************/
//  Description : handle picture list mark option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint16      mark_num = 0;
    //uint16      cur_index = 0;
    uint32      pic_num = MMIPICVIEW_GetAllPicNum();

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        MMIPICVIEW_SetMaxSelected(list_ctrl_id,(uint16)pic_num);
    }

    //get item index
    //cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //mark item
    //MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
}

/*****************************************************************************/
//  Description : handle picture list mark cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 )
{
    uint16      cur_index = 0;
    uint16      mark_num = 0;

    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //cancel mark item
    MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,FALSE);

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
    }
}

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  )
{
    uint16      mark_num = 0;
    uint32      pic_num = MMIPICVIEW_GetAllPicNum();

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        MMIPICVIEW_SetMaxSelected(list_ctrl_id,(uint16)pic_num);
    }

    //mark all item
    MMIPICVIEW_SetAllSelected(list_ctrl_id,TRUE);
}

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    MMIPICVIEW_SetAllSelected(list_ctrl_id,FALSE);

    //set list enable mark
//    MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
}
#ifdef PIC_EDITOR_SUPPORT    //norflashcut_pic     
//add for cr NEWMS00108303
/*****************************************************************************/
//  Description : get editpic index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPICVIEW_GetAndResetEditPicIndex(void)
{
    uint16 cur_index = s_editpic_index;
    s_editpic_index = 0;
    return cur_index;
}
//add for cr NEWMS00108303
#endif

/*****************************************************************************/
//  Description : handle picture list delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPICVIEW_HandleDelQueryWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    uint32              time_out = 0;
    uint16          cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    switch (msg_id)
    {
    case MSG_PROMPTWIN_OK:
        if(MMK_IsOpenWin(query_win_id)){//Bug 494472 - [B2_0211]Popup deleted when share morethan 10 pictures via Bluetooth
            MMIPUB_CloseQuerytWin(&query_win_id);
            s_mmipicview_del_file_is_cancelled = FALSE;
             s_mmipicview_is_del_err = FALSE;
            SCI_TRACE_LOW("[pic_view]MMIAPIPICVIEW_HandleDelQueryWin start_time = %d", SCI_GetTickCount());

    /*        if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
            {
                MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
            }*/

            //open wait
            s_total_mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
            if (1 < s_total_mark_num)
            {
                MMIPUB_OpenProgressWinByTextId(STR_GAL_WAIT_REMOVING_FOLDER, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
                MMIPUB_SetProgressTotalSize(&wait_win_id, s_total_mark_num);
                MMIPUB_SetProgressTimePeriod(&wait_win_id, 1000);
            }
            else
            {
    //            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }

            //delete pictures
            s_mmipicview_is_preview_del = FALSE;
            DeleteSelectedPic(list_ctrl_id,wait_win_id,win_id);
        }
        else if(s_mmipicview_is_del_err){
            s_mmipicview_is_del_err = FALSE;
            if(!s_mmipicview_is_preview_del){
                cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
                MMIPICVIEW_PostUpdateMsg(cur_index);
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        if(MMK_IsOpenWin(query_win_id)){
            MMIPUB_CloseQuerytWin(&query_win_id);
            //zhuyongjing changed
            MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
            MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
            //change end
#if defined(CAMERAROLL_SUPPORT)
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
            &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            &&MMISRV_CAMERAROLL_Get_Connect_State()
		){
              //  MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray()); 
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            }
#endif
						
	#ifdef CAMERAROLL_SUPPORT  
            if(MMIPICVIEW_GetMultiUploadingDone()&&MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
                MMIPICVIEW_PostUpdateMsg(0);
            }else{
                MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(list_ctrl_id));
            }
	#endif
        }
        break;
        
    case MSG_PICVIEWER_DELETE_CNF:
        HandleDeletePicCnf(*((BOOLEAN*)param),list_ctrl_id,wait_win_id,win_id);



        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle picture preview delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    uint32              time_out = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_WIN_ID_T        preview_win_id = MMIPICVIEW_PREVIEW_WIN_ID;
    MMI_WIN_ID_T        list_win_id = MMIPICVIEW_LIST_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch (msg_id)
    {
    //zhuyongjing add
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
    break;
    //add end
    case MSG_APP_WEB:
	case MSG_APP_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        s_mmipicview_del_file_is_cancelled = FALSE;
        s_mmipicview_is_del_err = FALSE;
 /*       if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        } */
      
        //open wait
//        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);

        //close preview win
        //MMK_CloseWin(preview_win_id);

        //delete pictures
        s_mmipicview_is_preview_del = TRUE;
        //DeleteSelectedPic(list_ctrl_id,wait_win_id,list_win_id);
        DeleteSelectedPic(list_ctrl_id,wait_win_id,preview_win_id);
        MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_SOFTKEY_CANCEL_MK, FALSE); 
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_CANCEL:
        StopDelete();
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
//lijinghua add for pic rename
LOCAL MMI_RESULT_E HandleRenameDialogWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_CTRL_ID_T		input_field_ctrl_id = MMIPUB_INPUTFIELD_CTRL_ID;//MMIPUB_GetInputFieldDialogCtrlId(win_id); 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetDialogBgLayerStyle(win_id,DIALOG_MASK_BG_SCREEN);
        SetInitName(input_field_ctrl_id,list_ctrl_id);
      // MMK_SetAtvCtrl(win_id,input_field_ctrl_id);
    	break;
	case MSG_CTL_MIDSK:
	    if (RenamePic(input_field_ctrl_id,list_ctrl_id))
        {
        //zhuyongjing delete for Bug 490948
		//    if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
	       // {
	       //     MMIPICVIEW_RefreshAnimDisplay(MMIPICVIEW_GetCurIndex(list_ctrl_id));
	       // }
        	MMK_CloseWin(win_id);
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID)){
                    MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                }
        }
        break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
        {
            MMI_STRING_T        name_str = {0};
            GUIEDIT_GetString(input_field_ctrl_id,&name_str);
            if (name_str.wstr_len > 0){
                MMK_CloseWin(win_id);
            }
        }
        break;
      	case MSG_APP_OK:
	case MSG_CTL_OK:  
	case MSG_CLOSE_WINDOW:
        MMK_CloseWin(win_id);
		break;
    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

//lijinghua end
/*****************************************************************************/
//  Description : cancel delete    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopDelete(void)
{
    //SCI_TRACE_LOW:"StopDelete"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2993_112_2_18_2_46_47_85,(uint8*)"");
    s_mmipicview_del_file_is_cancelled = TRUE;
}

/*****************************************************************************/
//  Description : delete selected pictures
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectedPic(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in
                             )
{
    uint16              mark_num = 0;
    uint16              item_index = 0;
    FILEARRAY_DATA_T    file_info = {0};
    BOOLEAN             result    = FALSE;


    if (s_mmipicview_is_fat_deleteing)
    {
        
        SCI_TRACE_LOW("[pic_view is deleting , please waiting] %s        %d\n",__FUNCTION__,__LINE__);
        return;
    }
    SCI_TRACE_LOW("[pic_view]DeleteSelectedPic start_time = %d", SCI_GetTickCount());

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //ger current index
        item_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    }
    else
    {
        //delete all mark items
        MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,&item_index,1);
    }

    s_mmipicview_del_file_index = item_index;

    //get file info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
        if( MMIAPIFMM_DeleteFileAsyn(file_info.filename,file_info.name_len,win_id,MSG_PICVIEWER_DELETE_CNF) )
        {
            SCI_TRACE_LOW("[pic_view is deleting start] %s        %d\n",__FUNCTION__,__LINE__);
            MMIAPIPICVIEW_SetFatDelIng(TRUE);
            result = TRUE;
           /* if(!s_mmipicview_is_preview_del
                && MMIPICVIEW_GetMultiUploadingDone()
                && TRUE == MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len)){
                //multi delete
                MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(file_info.filename,file_info.name_len);
                MMIPICVIEW_DelOneMultiUploadingNum();
            }else if(MMIPICVIEW_GetMultiUploadingDone()&&MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len)){
                //single delete
                MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(file_info.filename,file_info.name_len);

                MMIPICVIEW_DelOneMultiUploadingNum();
            }*/
        }
    }
SCI_TRACE_LOW("[pic_view]DeleteSelectedPic end_time = %d", SCI_GetTickCount());
    //delete the current item
    if ( !result )
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        
        //close wait win
        MMK_CloseWin(wait_win_id);

        MMIAPIPICVIEW_SetFatDelIng(FALSE);
    }
}

/*****************************************************************************/
//  Description : handle delete selected pictures confirm message
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePicCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:
                              )
{
    uint16          pic_index = s_mmipicview_del_file_index;
    uint16          mark_num = 0;
    uint16          cur_index = 0;
#ifndef MMI_PICVIEW_MINI_SUPPORT
    uint16  		pre_folder_num  = 0;
#endif
    FILEARRAY_DATA_T    file_info = {0};

	SCI_TRACE_LOW("[pic_view FAT Delete ok] %s        %d\n",__FUNCTION__,__LINE__);
    MMIAPIPICVIEW_SetFatDelIng(FALSE);

    if (is_success)
    {
        //update list
        MMIPICVIEW_RemoveItem(list_ctrl_id,pic_index);
#ifndef MMI_PICVIEW_MINI_SUPPORT
        pre_folder_num = MMIAPIFILEARRAY_GetTotalFolderCount(MMIPICVIEW_GetPicLibArray());
#endif
        //update picture array
SCI_TRACE_LOW("[pic_view]MMIPICVIEW_DeletePicFileInfo start_time = %d", SCI_GetTickCount());
    if(MMIPICVIEW_GetPicFileInfo(pic_index,&file_info)){
#ifdef  VIDEOTHUMBNAIL_SUPPORT
	if(MMIPICVIEW_IsVideoSuffix(&file_info))
	{
            MMIFMM_VideoThumbnailDeleteRelatedFile(file_info.filename);
	}
#endif
    //zhuyongjing add for search quickly
    #if defined(CAMERAROLL_SUPPORT)
        if(MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE
            ||(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE    
            &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)    
            )){
                MMIAPIPICVIEW_DelPicArrayItem(MMIAPIPICVIEW_GetBackupAllPicArray(),file_info.filename,file_info.name_len);
        }
	#endif
        //add end
        MMIPICVIEW_DeletePicFileInfo(pic_index);
        #if defined(CAMERAROLL_SUPPORT)
        //delete camera roll filearray
        if(MMIPICVIEW_GetMultiUploadingDone()){
            if(MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len)){
                MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(file_info.filename,file_info.name_len);
                MMIPICVIEW_DelOneMultiUploadingNum();
            }
        }else if(MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
            if(MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len)){
                MMISRV_CAMERAROLL_Delete_Manual_Upload_File_array(file_info.filename,file_info.name_len);
            }
        }
	#endif
    }
SCI_TRACE_LOW("[pic_view]MMIPICVIEW_DeletePicFileInfo end_time = %d", SCI_GetTickCount());
        if(s_mmipicview_del_file_is_cancelled)
        {
            //stop delete
           // HandlePicListUnmarkedAll(list_ctrl_id);

            cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
           // MMIPICVIEW_PostUpdateMsg(cur_index);
#ifdef PIC_PREVIEW_U200_SUPPORT
            MMIPICVIEW_DestroyMarkMenuButtons(win_id);
            MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
            MMK_CloseWin(wait_win_id);
            //zhuyongjing changed
            MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
            MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
            //change end
#if defined(CAMERAROLL_SUPPORT)            
            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
            &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            &&MMISRV_CAMERAROLL_Get_Connect_State()
		){
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            }
#endif
						
	#ifdef CAMERAROLL_SUPPORT	
            if(MMIPICVIEW_GetMultiUploadingDone()&&MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
                if(!MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
                    ModifyPicListViewForUploadItems(win_id,list_ctrl_id,FALSE);
                    MMIPICVIEW_SetMultiUploadingDone(FALSE);
                    MMISRV_CAMERAROLL_Cancel_Upload();
                }
                MMIPICVIEW_PostUpdateMsg(cur_index);
            }else
	#endif
		{
                MMIPICVIEW_PostUpdateMsg(cur_index);
            }
        }
        else
        {
            //get mark num
            mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
            if (0 == mark_num)
            {
                SCI_TRACE_LOW("[pic_view]MMIAPIPICVIEW_HandleDelQueryWin end_time = %d", SCI_GetTickCount());
                //set list not enable mark
                MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
#if defined(CAMERAROLL_SUPPORT)								
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
                    &&MMISRV_CAMERAROLL_Get_Connect_State()
			){
                    MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
                }
#endif
                
                if(0 == MMIPICVIEW_GetAllPicNum())
                {

                    MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
                    //zhuyongjing add for upload items
                    MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID);
                    MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
                    //add end
#ifdef PIC_PREVIEW_U200_SUPPORT
                    MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                    MMIPICVIEW_SetButtonsVisible(TRUE);
                    GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_SLIDE_PREVIEW, TRUE, PNULL, TRUE);
                    GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_MARKED, TRUE, PNULL, TRUE);

#endif
                    MMIPICVIEW_SetListEmpty(TRUE);
                    PicviewListAllEmptyShow(MMIPICVIEW_LIST_WIN_ID);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
                    MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
                    MMK_CloseWin(wait_win_id);
                }
                else
                {
                    //prompt success
        		  //xin.li_clean_popwin	
        		  //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DELETED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleDeleteSuccAlertWin);
                    MMK_CloseWin(wait_win_id);
			#ifdef CAMERAROLL_SUPPORT			
                    if(MMIPICVIEW_GetMultiUploadingDone()
                        &&MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                        &&!MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
                        ModifyPicListViewForUploadItems(win_id,list_ctrl_id,FALSE);
                        MMIPICVIEW_SetMultiUploadingDone(FALSE);
                        MMISRV_CAMERAROLL_Cancel_Upload();
                    }
			#endif		
                    //zhuyongjing chage delete quickly
                    //get item index
                    cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
                    MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
                    MMIPICVIEW_PostUpdateMsg(cur_index);
                    //MMIAPIPICVIEW_ReloadFile();

                    //change end
                    //close wait win
    
#ifdef PIC_PREVIEW_U200_SUPPORT
                    MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                    MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
                }
                
            }
            else
            {
                //delete the next picture
                DeleteSelectedPic(list_ctrl_id,wait_win_id,win_id);
            }
        }
        if (!s_mmipicview_is_preview_del){
            uint32              select_num = 0;
            uint32              deleted_num = 0;
            select_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
            deleted_num = ((s_total_mark_num > select_num) ? (s_total_mark_num - select_num) : 0);
            MMIPUB_UpdateProgressBar(&wait_win_id, deleted_num);
            if(deleted_num == s_total_mark_num){
                if(MMIPICVIEW_GetMultiChoiceDelete()){
                    MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
                }
                DisplaybannerInfo(win_id,STR_GAL_CONF_ITEM_REMOVED);
            }
        }
    }
    else
    {
        SCI_TRACE_LOW("[mmipic_view]HandleDeletePicCnf  delete error");
        //prompt fail
        s_mmipicview_is_del_err = TRUE;
        MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
        if(!s_mmipicview_is_preview_del){
            MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
#ifdef CAMERAROLL_SUPPORT			
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                &&MMISRV_CAMERAROLL_Get_Connect_State()
                ){
                //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray()); 
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            }
#endif								
        }
        if(MMIPICVIEW_GetMultiChoiceDelete()){
            MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
        }
        MMK_CloseWin(wait_win_id);
#ifdef PIC_PREVIEW_U200_SUPPORT
        MMIPICVIEW_DestroyMarkMenuButtons(win_id);
        MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
    }
}
#if 0
/*****************************************************************************/
//  Description : handle delete picture success alert window
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSuccAlertWin(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    uint16          cur_index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);

        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        
        MMIPICVIEW_PostUpdateMsg(cur_index);
        break;
    
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : handle delete complete display
//  Global resource dependence : 
//  Author: lijinghua
//  Note: 
/*****************************************************************************/
PUBLIC void DisplaybannerInfo(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id)
{
    MMICOM_BANNER_INFO_T banner_info = {0};
    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point.x = 0;
    banner_info.start_point.y = 0;
    banner_info.text1_id = text_id;
    MMIAPICOM_OpenBannerWin(win_id, banner_info);
}

/*****************************************************************************/
//  Description : handle rename win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicRenameWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_RENAME_EDITBOX_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //init name
        SetInitName(ctrl_id,list_ctrl_id);
        
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,STXT_SOFTKEY_OK_MK,PNULL);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef PIC_PREVIEW_U200_SUPPORT
        if(PNULL != param)
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
        }
#endif //PIC_PREVIEW_U200_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (RenamePic(ctrl_id,list_ctrl_id))
        {
        //zhuyongjing delete for Bug 490948
    	   // if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
           // {
           //    MMIPICVIEW_RefreshAnimDisplay(MMIPICVIEW_GetCurIndex(list_ctrl_id));
          //  }

            MMK_CloseWin(win_id);
		if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID)){
                    MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                }
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set initial name
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetInitName(
                       MMI_CTRL_ID_T    ctrl_id,    //in
                       MMI_CTRL_ID_T    list_ctrl_id//in
                       )
{
    uint16              cur_index = 0;
    uint16              path_len = 0;
    uint16              name_len = 0;
    uint16              char_max_num = 0;
    FILEARRAY_DATA_T    file_info = {0};

    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
    {
        //split file name
        MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
            PNULL,&path_len,
            PNULL,&name_len,
            PNULL,PNULL);

        char_max_num = (uint16)(MMIFILE_FILE_NAME_MAX_LEN - file_info.name_len + name_len);

        //set edit box max len
        GUIEDIT_SetTextMaxLen(ctrl_id,char_max_num,char_max_num);
#ifdef ZDT_PRODUCT_Z685S
	GUIEDIT_SetFontColor(ctrl_id, 0x0040); 
#endif

        //set text string
        GUIEDIT_SetString(ctrl_id,file_info.filename + path_len,name_len);
    }
}

/*****************************************************************************/
//  Description : rename picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenamePic(
                        MMI_CTRL_ID_T  ctrl_id,    //in
                        MMI_CTRL_ID_T  list_ctrl_id//in
                        )
{
    BOOLEAN             result = FALSE;
    uint16              cur_index = 0;
    uint16              path_len = MMIFILE_FILE_NAME_MAX_LEN;
    uint16              suffix_len = MMIPICVIEW_SUFFIX_MAX_LEN;
    uint16              new_name_len = 0;
    wchar               *new_name_ptr = PNULL;
    wchar               suffix[MMIPICVIEW_SUFFIX_MAX_LEN+1] = {0};
    SFS_ERROR_E         rename_result = SFS_ERROR_NONE;
    MMI_STRING_T        name_str = {0};
    FILEARRAY_DATA_T    file_info = {0};

    //get edit box text info
    GUIEDIT_GetString(ctrl_id,&name_str);
    if (0 < name_str.wstr_len)
    {
        //judge file name is valid
        if(!MMIAPICOM_IsIncludeInvalidChar(name_str.wstr_ptr,name_str.wstr_len))
        {
            //new name alloc memory
            new_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));
            if (PNULL == new_name_ptr)
            {
                return FALSE;
            } 

            SCI_MEMSET(new_name_ptr,0, ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar)));

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            //get old full name
            if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
            {
                //get new full name, split old full name,get the path and suffix
                MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
                    new_name_ptr,&path_len,
                    PNULL,PNULL,
                    suffix,&suffix_len);
                new_name_len = path_len;

                if (!MMIAPICOM_GetValidFileName(FALSE,
                    name_str.wstr_ptr,name_str.wstr_len,
                    (new_name_ptr+new_name_len),(MMIFILE_FILE_NAME_MAX_LEN+1-new_name_len))){
			//prompt valid name
			MMIPUB_OpenAlertWarningWin(STR_FILES_INVALID_CHARACTER_EXT01);
		}else{
                    //get new name string len
                    new_name_len = (uint16)MMIAPICOM_Wstrlen(new_name_ptr);

                    //copy suffix
                    MMI_WSTRNCPY(new_name_ptr+new_name_len,(MMIFILE_FILE_NAME_MAX_LEN-new_name_len),suffix,suffix_len,suffix_len);
                    new_name_len = (uint16)(new_name_len + suffix_len);

                    //new name and old name is same
                    if (0 == MMIAPICOM_CompareTwoWstr(new_name_ptr,new_name_len,file_info.filename,file_info.name_len)){
                    	//prompt complete
                        //DisplaybannerInfo(PNULL,STR_GAL_CONF_RENAMED_EXT01);
                    	//MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);    //lijinghua delete it
                    	result = TRUE;
                    }else{
                        //rename file
                        rename_result = SFS_RenameFile(file_info.filename,new_name_ptr,PNULL);
                        switch (rename_result)
                        {
                        case SFS_ERROR_NONE:
							//is wallpaper
                            if(MMIAPIFMM_IsWallPaperFile(file_info.filename,file_info.name_len))
                            {
                                MMIAPIFMM_ChangeWallPaperFileName(new_name_ptr,new_name_len);

                            //update wallpaper
                                MMIAPIIDLE_SetWallpaper(TRUE);
                            }

                            //delete picture miniature
                            GUIANIM_DeleteMiniatureFile(file_info.filename,file_info.name_len);

                            //update pic array
                            UpdateListArray(cur_index,new_name_ptr,new_name_len);

                            //modify list item
                            MMIPICVIEW_ModifyItemText(list_ctrl_id,cur_index);

                                //prompt complete
                                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);   //lijinghua delete it
                                result = TRUE;
                            break;

                        case SFS_ERROR_HAS_EXIST:
                            //prompt name repeat
                            MMIPUB_OpenAlertWarningWin(STR_FILES_NAME_IN_USE_EXT01);
                            break;

                        default:
							//prompt error
                            MMIPUB_OpenAlertFailWin(TXT_ERROR);
                            break;
                        }
                    }

                    //modify name
                    if(result){
                        MMIPICVIEW_ModifyName(cur_index);
                    }
                }
            }

            //free new name
            SCI_FREE(new_name_ptr);
            new_name_ptr = PNULL;
        }
        else
        {
            //prompt valid name
            MMIPUB_OpenAlertWarningWin(STR_FILES_INVALID_CHARACTER_EXT01);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : update picture list array
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void UpdateListArray(
                           uint16   index,          //in
                           wchar    *new_name_ptr,  //in:full name
                           uint16   new_name_len    //in:full name
                           )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get old file info
    if (MMIPICVIEW_GetPicFileInfo(index,&file_info))
    {
        //set new name
        SCI_MEMSET(file_info.filename,0,sizeof(file_info.filename));
        file_info.name_len = new_name_len;
        MMI_WSTRNCPY(file_info.filename,FILEARRAY_FILENAME_MAX_LEN,
            new_name_ptr,new_name_len,
            new_name_len);

        MMIPICVIEW_ModifyPicFileInfo(index,&file_info);
    }
}

/*****************************************************************************/
//  Description : handle detail win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicDetailWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_DETAIL_RICHTEXT_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_STRING_T      	title_str = {0};
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
	MMI_GetLabelTextByLang(STR_GAL_HDR_DETAILS_EXT01, &title_str );
	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
	{
			  GUIWIN_UpdateStb();
	}
        //init detail
        GUIRICHTEXT_SetLineSpace(ctrl_id,2);
        GUIRICHTEXT_SetItemSpace(ctrl_id,2);
        SetDetail(win_id,ctrl_id,list_ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
	MMI_GetLabelTextByLang(STR_GAL_HDR_GALLERY_EXT01, &title_str );
	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
	{
	     GUIWIN_UpdateStb();
	}
        break;

#ifdef DRM_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    //case MSG_CTL_OK:
    //case MSG_CTL_MIDSK:
		/*{
			uint16				cur_index = 0;
			FILEARRAY_DATA_T    file_info = {0};
			
           cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

			//cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

			//get file info
			if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) &&
				(MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)) &&
				(DRM_LEVEL_SD == MMIAPIDRM_GetDRMFileType(SFS_INVALID_HANDLE,file_info.filename)))
			{
				MMIAPIFMM_DrmOpenDownloadMenuWin(file_info.filename);
			}
		}*/
        //break;
#ifdef PIC_PREVIEW_U200_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
    {
        uint16				cur_index = 0;
        FILEARRAY_DATA_T    file_info = {0};

        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
       // cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

        //get file info
        if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) &&
        	(MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)) &&
        	(DRM_LEVEL_SD == MMIAPIDRM_GetDRMFileType(SFS_INVALID_HANDLE,file_info.filename)))
        {
                MMIAPIDRM_DownloadRightsFile(SFS_INVALID_HANDLE, file_info.filename);
                MMK_CloseWin(win_id);
        }        
    }
    break;
#endif
#endif

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
/*****************************************************************************/
// 	Description: handle email entry tip window message
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryOneDriveTipWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	MMI_CTRL_ID_T ctrl_id = PICVIEW_ENTRY_ONEDRIVE_TIP_CTRL_ID;
	BOOLEAN       isforsetting = (BOOLEAN)MMK_GetWinAddDataPtr(win_id);
    
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
	 SCI_TRACE_LOW("[MMIPIC]HandleEntryOneDriveTipWinMsg: IN isforsetting=%d",isforsetting);
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
			uint16    index= 0;
                        
                        GUIRICHTEXT_SetBgColor(ctrl_id,MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_NEG_3));
                        item_data.img_type = GUIRICHTEXT_IMAGE_RES;
                        item_data.img_data.res.id = IMAGE_GALLERY_PROMO_PHOTOS;
                        item_data.text_type = GUIRICHTEXT_TEXT_NONE;
                        item_data.img_set_align = TRUE;
                        item_data.img_setting_align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;
                        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
                        index++;
                        SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                        item_data.text_type = GUIRICHTEXT_TEXT_RES;
                        item_data.text_data.res.id = STR_TITLE_PHOTOS_SAFE;
                        item_data.text_set_font = TRUE;
                        item_data.setting_font =MMI_DEFAULT_BIG_FONT;                  
                        item_data.text_set_align = TRUE;
                        item_data.setting_align = ALIGN_HVMIDDLE;
                        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
                        index++;

                        SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                        item_data.text_type = GUIRICHTEXT_TEXT_RES;
                        item_data.text_data.res.id = STR_QUERY_PHOTOS_SAFE;
                        item_data.text_set_font = TRUE;
                        item_data.setting_font =MMI_DEFAULT_NORMAL_FONT;
                        item_data.text_set_align = TRUE;
                        item_data.setting_align = ALIGN_HVMIDDLE;
                        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
                        index++;
                        MMK_SetAtvCtrl(win_id, ctrl_id);
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SKIP_RSK, RIGHT_BUTTON, TRUE);
		}
		
		break;
		
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
                if(!MMK_IsOpenWin(MMIPICVIEW_ONEDRIVE_WIN_ID)){
                    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
                    MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
                }
		break;
		
	case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
	case MSG_APP_OK:
                MMK_CloseWin(win_id);
                if(isforsetting){
		#if defined(CAMERAROLL_SUPPORT)			
                    MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL, MMIPICVIEW_MSA_Get_Token_For_Settings);
		#endif
                }else{
                    MMIPIC_CreateOneDriveConnectTextDialogWin();
                }
		break;
        case MSG_CTL_OK:
        case MSG_APP_MENU:
            MMK_CreateWin((uint32*)MMIPICVIEW_ENTRY_ONEDRIVE_TIP_INFO_WIN_TAB, PNULL);
            break;
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}
/*****************************************************************************/
// 	Description: handle email entry tip window message
//	Global resource dependence:
//  Author: zhuyongjing
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEntryOneDriveTipInfoWinMsg(
											  MMI_WIN_ID_T win_id, 
											  MMI_MESSAGE_ID_E msg_id, 
											  DPARAM param
											  )
{
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
	MMI_CTRL_ID_T ctrl_id = PICVIEW_ENTRY_ONEDRIVE_TIP_INFO_CTRL_ID;
	BOOLEAN       isforsetting = (BOOLEAN)MMK_GetWinAddDataPtr(win_id);
    
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
	 SCI_TRACE_LOW("[MMIPIC]HandleEntryOneDriveTipInfoWinMsg: IN isforsetting=%d",isforsetting);
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
			uint16    index= 0;
                       
                        SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
                        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                        item_data.text_type = GUIRICHTEXT_TEXT_RES;
                        item_data.text_data.res.id = STR_ONEDRIVE_INFORMATION_EXT01;
                        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
                        MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		
		break;
		
	case MSG_FULL_PAINT:
		//LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}


/*****************************************************************************/
//  Description : Start rect progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartOneDriveSyncProgressTimer(void)
{
    s_progress_timer = MMK_CreateTimerCallback(TIMER_DELAY, ProgressTimerCallBack, NULL, FALSE);
}

/*****************************************************************************/
//  Description : Stop rect progressbar timer 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopOneDriveSyncProgressTimer(void)
{
    if(MMI_NONE_TIMER_ID < s_progress_timer) 
    {
        MMK_StopTimer(s_progress_timer);
        s_progress_timer = 0;
    }
}

/*****************************************************************************/
//  Description : Callback function for progressbar timer 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void ProgressTimerCallBack(void)
{   
	MMIPUB_CloseProgressWin(PNULL);
	
}

/*****************************************************************************/
//  Description : Handle Pic One Drive Sync DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveSyncCompleteWinMsg(
												MMI_WIN_ID_T     win_id, 
												MMI_MESSAGE_ID_E msg_id, 
												DPARAM           param 
												)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	MMI_STRING_T        title_str = {0};
	wchar               title_text[] = L"Up to date";   
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		{                       
			title_str.wstr_ptr = title_text;
			title_str.wstr_len = MMIAPICOM_Wstrlen(title_text);
		    MMIPUB_SetWinTitleText(win_id, &title_str, TRUE);
		}
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
		break;
		
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}

/*****************************************************************************/
//  Description : Handle PROGRESSBAR DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOneDriveConnectProgressbarDialogWindow(
												  MMI_WIN_ID_T     win_id, 
												  MMI_MESSAGE_ID_E msg_id, 
												  DPARAM           param 
												  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;    
//     MMI_STRING_T        title_str = {0};
//     wchar               title_text[] = L"Dialog Title";   
    
//     title_str.wstr_ptr = title_text;
//     title_str.wstr_len = MMIAPICOM_Wstrlen(title_text);
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            StartOneDriveSyncProgressTimer();
        }
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
		
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : Creating dialog with progressbar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveConnectSyncProgressDialogWin(void)
{
    MMIPUB_OpenProgressWinByTextId(STR_ONEDRIVE_WAIT_SYNC_EXT01, PNULL, PNULL, PNULL, HandleOneDriveConnectProgressbarDialogWindow);
}

/*****************************************************************************/
//  Description : Creating dialog with progressbar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveSyncProgressDialogWin(void)
{
#if !defined(WIN32)
    MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_DOWNLOAD_THUMBNAIL);
#endif
	//zhuyongjing changed
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
        MMIPICVIEW_SetIfDownloadThumbnail(TRUE);
    }else{
        MMIPICVIEW_SetIfDownloadThumbnail(TRUE);
        MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE); 
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
    //change end
}

/*****************************************************************************/
//  Description : Handle Pic One Drive Sync DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveConnectDialogWinMsg(
															MMI_WIN_ID_T     win_id, 
															MMI_MESSAGE_ID_E msg_id, 
															DPARAM           param 
															)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CloseWin(win_id);
		MMIPIC_CreateOneDriveConnectSyncProgressDialogWin();
		break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}



/*****************************************************************************/
// 	Description: entry email tip window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_EntryOneDriveTip(BOOLEAN isforSetting)
{
    MMK_CreateWin((uint32*)MMIPICVIEW_ENTRY_ONEDRIVE_TIP_WIN_TAB, (ADD_DATA)isforSetting);
}

/*****************************************************************************/
//  Description : Handle Pic One Drive Sync DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveStartWinMsg(
													   MMI_WIN_ID_T     win_id, 
													   MMI_MESSAGE_ID_E msg_id, 
													   DPARAM           param 
													   )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	//     MMI_STRING_T        title_str = {0};
	//     wchar               title_text[] = L"Auto upload";   
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //zhuyongjing add softkey
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
        //add end
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
 		
		MMK_CloseWin(win_id);
		MMIPICVIEW_EntryOneDriveTip(TRUE);
		//MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CAMERA_ROLL, MMIPICVIEW_MSA_Get_Token_For_Settings);
		break;

	case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
		break;
		
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}




/*****************************************************************************/
//  Description : Handle Pic One Drive Sync DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveCheckUpdateWinMsg(
													   MMI_WIN_ID_T     win_id, 
													   MMI_MESSAGE_ID_E msg_id, 
													   DPARAM           param 
													   )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //zhuyongjing add softkey
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
        //add end
        break;
		
  //  case MSG_CTL_MIDSK:
    case MSG_APP_WEB:

        MMK_CloseWin(win_id);
        if(MMK_IsOpenWin(MMIPICVIEW_ONEDRIVE_WIN_ID)){
            MMIPICVIEW_SetOneDriveItemGray(FALSE);
            MMISRV_CAMERAROLL_Set_Connect_State(TRUE);
            GUILIST_SetToggleState(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, ONEDRIVE_INDEX_CONNECT, TRUE);
            MMK_SendMsg(MMIPICVIEW_ONEDRIVE_WIN_ID, MSG_FULL_PAINT, PNULL);
            MMK_CloseWin(MMIPICVIEW_ONEDRIVE_WIN_ID);
        }
        MMIPIC_CreateOneDriveSyncProgressDialogWin();
        if(MMISRV_CAMERAROLL_Get_Connect_State() == FALSE){
            MMISRV_CAMERAROLL_Set_Connect_State(TRUE);
            SCI_TRACE_LOW("[MMIPIC]:HandlePicOneDriveCheckUpdateWinMsg set is_sync_on=1");
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        if(!MMK_IsOpenWin(MMIPICVIEW_ONEDRIVE_WIN_ID)){
            MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
            MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
        }else{
            MMIPICVIEW_SetOneDriveItemGray(TRUE);
            MMISRV_CAMERAROLL_Set_Connect_State(FALSE);
            GUILIST_SetToggleState(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, ONEDRIVE_INDEX_CONNECT, FALSE);
        }
	break;
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}


/*****************************************************************************/
//  Description : Handle Pic One Drive Auto Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveAutoUploadDialogWinMsg(
															  MMI_WIN_ID_T     win_id, 
															  MMI_MESSAGE_ID_E msg_id, 
															  DPARAM           param 
															  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {                       
            MMIPUB_SetWinTitleTextId(win_id, STR_ONEDRIVE_TITLE_AUTO_EXT01, TRUE);
            //zhuyongjing add softkey
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, RIGHT_BUTTON, TRUE);
        //add end
        }
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
	#ifdef CAMERAROLL_SUPPORT	
	MMISRV_CAMERAROLL_Set_Sync_State(CAMERA_ROLL_AUTO_SYNC);
	MMK_CloseWin(win_id);
        if(MMIPICVIEW_GetMultiUploadingDone()){
            MMISRV_CAMERAROLL_Clean_manual_upload_list();
            MMISRV_CAMERAROLL_Cancel_Upload();
            MMIAPIPICVIEW_ReloadFile();
        }
	#endif
        break;		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
		
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}

/*****************************************************************************/
//  Description : Handle Pic One Drive Manual Upload DIALOG window 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicOneDriveManualUploadDialogWinMsg(
															MMI_WIN_ID_T     win_id, 
															MMI_MESSAGE_ID_E msg_id, 
															DPARAM           param 
															)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {                       
            MMIPUB_SetWinTitleTextId(win_id, STR_ONEDRIVE_TITLE_MANUAL_EXT01, TRUE);
        //zhuyongjing add softkey
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, RIGHT_BUTTON, TRUE);
        //add end
        }
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
	MMISRV_CAMERAROLL_Set_Sync_State(CAMERA_ROLL_MANUAL_SYNC);
	MMK_CloseWin(win_id);
	break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYDOWN_OK:
        break;
		
    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
	
    return (recode);
}

/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveConnectTextDialogWin(void)
{
	MMI_WIN_ID_T     win_id = MMIPICVIEW_ONEDRIVE_CONNECT_DIALOG_WIN_ID;
	GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
	GUI_RECT_T       win_rect = {0};
	uint16           logic_width = 0;
	uint16           logic_height = 0;
	
	// for sync option
    MMIPUB_OpenConfirmationDialogByTextId ( PNULL,
											STR_ONEDRIVE_CONNECT_INTERNET_EXT01,
											TXT_NULL,
											PNULL,
											&win_id,
											PNULL,
											MMIPUB_SOFTKEY_NONE/*MMIPUB_SOFTKEY_DIALOG_OK*/,
											HandlePicOneDriveCheckUpdateWinMsg);
}

/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveAutoUploadTextDialogWin(void)
{
	MMI_WIN_ID_T     win_id = MMIPICVIEW_ONEDRIVE_AUTO_UPLOAD_DIALOG_WIN_ID;
	GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
	GUI_RECT_T       win_rect = {0};
	uint16           logic_width = 0;
	uint16           logic_height = 0;
	
	GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr.lcd_id, &logic_width, &logic_height);        
	win_rect.bottom = win_rect.top + logic_height;
	win_rect.right = win_rect.left + logic_width;
	GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info_ptr,win_rect, 0x0);
	
	//Open text dialog by text id 
//zhuyongjing change to MMIPUB_OpenConfirmationDialogByTextId
	MMIPUB_OpenTextDialogByTextId ( PNULL,
	//MMIPUB_OpenConfirmationDialogByTextId(PNULL,
                PNULL,
		STR_ONEDRIVE_NOTE_AUTO_EXT01,
		IMAGE_NULL,
		&win_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_OK,
		HandlePicOneDriveAutoUploadDialogWinMsg
		);

}

/*****************************************************************************/
//  Description : Create one drive manual upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveManualUploadTextDialogWin(void)
{
	MMI_WIN_ID_T     win_id = MMIPICVIEW_ONEDRIVE_MANUAL_UPLOAD_DIALOG_WIN_ID;
	GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
	GUI_RECT_T       win_rect = {0};
	uint16           logic_width = 0;
	uint16           logic_height = 0;
	
	GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr.lcd_id, &logic_width, &logic_height);        
	win_rect.bottom = win_rect.top + logic_height;
	win_rect.right = win_rect.left + logic_width;
	GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info_ptr,win_rect, 0x0);
	
	//Open text dialog by text id 
	//zhuyongjing change to MMIPUB_OpenConfirmationDialogByTextId
	MMIPUB_OpenTextDialogByTextId ( PNULL,
	//MMIPUB_OpenConfirmationDialogByTextId(PNULL,
                                                                        PNULL,
									STR_ONEDRIVE_NOTE_MANUAL_EXT01,
									IMAGE_NULL,
									&win_id,
									PNULL,
									MMIPUB_SOFTKEY_DIALOG_OK,
									HandlePicOneDriveManualUploadDialogWinMsg
								   );        
}

/*****************************************************************************/
//  Description : handle onedrive win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicOneDriveUploadOptWinMsg(
										   MMI_WIN_ID_T        win_id,
										   MMI_MESSAGE_ID_E    msg_id,
										   DPARAM              param
										   )
{
    MMI_RESULT_E	result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	ctrl_id = MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID;
    uint16	cur_index = 0;

    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
		MMIPICVIEW_AppendOneDriveUploadOptItem();
		MMK_SetAtvCtrl(win_id,ctrl_id);
        //zhuyongjing add for default select item
                if(CAMERA_ROLL_AUTO_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()){
                    GUILIST_SetSelectedItem(ctrl_id, ONEDRIVE_INDEX_AUTO_UPLOAD, TRUE);
                }else{
                    GUILIST_SetSelectedItem(ctrl_id, ONEDRIVE_INDEX_MANUAL_UPLOAD, TRUE);
                }
        //change end
		break;
		
	case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_WEB:	
		cur_index =  GUILIST_GetCurItemIndex(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID);
		switch(cur_index)
		{
		
		case ONEDRIVE_INDEX_AUTO_UPLOAD:
			MMIPIC_CreateOneDriveAutoUploadTextDialogWin();
			break;
			
		case ONEDRIVE_INDEX_MANUAL_UPLOAD:
			MMIPIC_CreateOneDriveManualUploadTextDialogWin();
			break;
			
		default:
			break;
		}
		
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
    return (result);
}
/*****************************************************************************/
//  Description : handle onedrive win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateMSAOneDrivePhotoWin(void)
{
	MMK_CreateWin((uint32 *)MMIPICVIEW_ONEDRIVE_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : handle onedrive win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicOneDriveWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E	result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T	ctrl_id = MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID;
    uint16	cur_index = 0;
    BOOLEAN	is_sync_on = FALSE;
    BOOLEAN	is_sync_show = FALSE;
    MMIPICVIEW_FLASH_LIST_T flash_list_status = MMIPICVIEW_FLASH_LIST_NONE;
        
    is_sync_on = MMISRV_CAMERAROLL_Get_Connect_State();
    SCI_TRACE_LOW("[MMIPIC]:HandlePicOneDriveWinMsg is_sync_on=%d",is_sync_on);
    switch(msg_id)
    {
		case MSG_OPEN_WINDOW:
                    MMIPICVIEW_AppendOneDriveMenuItem(is_sync_on);
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                    GUILIST_GetToggleState(ctrl_id,ONEDRIVE_INDEX_CONNECT,&is_sync_show);
                    SCI_TRACE_LOW("[MMIPIC]:HandlePicOneDriveWinMsg is_sync_show=%d",is_sync_show);

                    if(is_sync_show != is_sync_on){
                        GUILIST_SetToggleState(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, ONEDRIVE_INDEX_CONNECT, is_sync_on);
                    }
                    s_origial_connect_status = MMISRV_CAMERAROLL_Get_Connect_State();
			break;
			
		case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		//case MSG_CTL_OK:
		case MSG_APP_WEB:
		//case MSG_APP_OK:
		
		if (is_sync_on == TRUE)
		{
			cur_index =  GUILIST_GetCurItemIndex(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID);
			switch(cur_index)
			{
			case ONEDRIVE_INDEX_UPLOAD:
				MMK_CreateWin((uint32 *)MMIPICVIEW_ONEDRIVE_UPLOAD_LIST_WIN_TAB,PNULL);
				break;
				
			case ONEDRIVE_INDEX_SYNC:
// 				MMK_CreateWin((uint32*)MMIPICVIEW_CHECK_UPDATE_WIN_TAB, PNULL);		
				MMIPIC_CreateOneDriveConnectTextDialogWin();
				break;
				
			default:
				break;
			}
		}

		break;
		
		case MSG_CTRL_TOGGLE_CHANGED:
		{
			is_sync_on = !is_sync_on;


                        SCI_TRACE_LOW("[MMIPIC]:HandlePicOneDriveWinMsg is_sync_on=%d",is_sync_on);
			
			if (is_sync_on == TRUE)
			{
				MMIPIC_CreateOneDriveConnectTextDialogWin();
			}
			else
			{
			        MMISRV_CAMERAROLL_Set_Connect_State(is_sync_on);
                                GUILIST_SetToggleState(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, ONEDRIVE_INDEX_CONNECT, is_sync_on);
				MMIPICVIEW_SetOneDriveItemGray(TRUE);
			}

			{
				MMK_SendMsg(MMIPICVIEW_ONEDRIVE_WIN_ID, MSG_FULL_PAINT, PNULL);
			}
		}
			break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;
		case MSG_CLOSE_WINDOW:
                {
                        uint32 total_num = MMIPICVIEW_GetAllPicNum();
                       // uint32 thumbnail_num = MMIPICVIEW_GetAllThumbnailsNum();
                        SCI_TRACE_LOW("[MMIPIC]:HandlePicOneDriveWinMsg total_num=%d",total_num);

                        if(s_origial_connect_status != MMISRV_CAMERAROLL_Get_Connect_State()){
                            
                            if(!MMISRV_CAMERAROLL_Get_Connect_State()){
                                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                                    ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                                    if(total_num > VIRTUALARRAY_BUFFER_SIZE){
                                        GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_DISCONNECT_DELETE_FILEARRAY;
                                        Onedrive_Disconnect_Delete_Filearray_Signal();
                                        MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);
                                    }else{
                                        MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetAllPicArray(),TRUE,TRUE);
                                        if(MMIPICVIEW_GetAllPicNum() > 0){
                                            flash_list_status = MMIPICVIEW_FLASH_LIST_ONCE;
                                        }else{
                                            flash_list_status = MMIPICVIEW_FLASH_LIST_EMPTY;
                                        }
                                    }
                                }else{
                                    if(total_num > VIRTUALARRAY_BUFFER_SIZE){
                                        GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_DISCONNECT_DELETE_FILEARRAY;
                                        Onedrive_Disconnect_Delete_Filearray_Signal();
                                        MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);
                                    }else{
                                        MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetBackupAllPicArray(),FALSE,TRUE);
                                    }
                                }
                                if(total_num <= VIRTUALARRAY_BUFFER_SIZE){
                                    MMIPICVIEW_DestroyThumbnailsArray();
                                }
                            }else if(MMISRV_CAMERAROLL_Get_Connect_State() ){

    		        }
                        else{
                                flash_list_status = MMIPICVIEW_FLASH_LIST_NONE;
    		        }
                            if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                                    ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){

                                if(flash_list_status == MMIPICVIEW_FLASH_LIST_EMPTY&&!MMK_IsOpenWin(MMIPICVIEW_TASK_WAITING_WIN_ID)){
                                    MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
                                    MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID);
                                    MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
                                    MMIPICVIEW_SetListEmpty(TRUE);
                                    PicviewListAllEmptyShow(MMIPICVIEW_LIST_WIN_ID);
                                }else if(flash_list_status == MMIPICVIEW_FLASH_LIST_ONCE
                                    &&!MMK_IsOpenWin(MMIPICVIEW_TASK_WAITING_WIN_ID)){
                                    if(MMIPICVIEW_GetMultiUploadingDone()){
                                        MMISRV_CAMERAROLL_Clean_manual_upload_list();
                                        MMISRV_CAMERAROLL_Cancel_Upload();
                                    }
                                    MMIPICVIEW_PostUpdateMsg(0);
                                }
                            }
                        }
		    }
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
    return (result);
}
#endif
/*****************************************************************************/
//  Description : set detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDetail(
					 MMI_WIN_ID_T	win_id,
                     MMI_CTRL_ID_T  ctrl_id,
                     MMI_CTRL_ID_T  list_ctrl_id
                     )
{
    uint8                detail_str[MMIPICVIEW_DETAIL_STR_MAX_LEN] = {0};
    uint16              cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    uint16              richtext_index = 0;
    uint16              path_len = 0;
    uint16              pic_width = 0;
    uint16              pic_height = 0;
    FILEARRAY_DATA_T    file_info = {0};
    GUIRICHTEXT_ITEM_T  richtext_item = {0};
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    uint16              name_len = 0;
    uint16              suffix_len = 0;
    wchar	file_info_wstr[MMIFMM_FULL_FILENAME_LEN] = {0};
    char file_info_num_str[MMIFMM_FULL_FILENAME_LEN] ={0}; 
    char file_info_num_new_str[MMIFMM_FULL_FILENAME_LEN] ={0}; 
    wchar  file_info_num_wstr[MMIFMM_FULL_FILENAME_LEN]={0};
    uint16 			 i = 0;
    uint8                       detail_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    wchar                       wchar_detail_arr[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    MMI_STRING_T                title_str = {0};
    MMI_STRING_T				title_src1_str = {0};
    MMI_STRING_T				title_src2_str = {0};
    DPLAYER_MEDIA_INFO_T media_info = {0};
    wchar               temp_wstr[MMIPICVIEW_DETAIL_STR_MAX_LEN + 1] = {0};

    //get file info
    if( !MMIPICVIEW_GetPicFileInfo(cur_index,&file_info) )
    {
        return;
    }

    //add file name label
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
    AddFixStrItem(&richtext_index,STR_MCD_DTAIL_COD_NAME,&richtext_item,ctrl_id);

  //lijinghua modify it
    //split file name
   MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
        PNULL,&path_len,
        PNULL,&name_len,
        PNULL,&suffix_len);

	//add file name
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item.text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN, name_len);
    richtext_item.text_data.buf.str_ptr = (file_info.filename + path_len);
    GUIRICHTEXT_AddItem(ctrl_id,&richtext_item,&richtext_index);
//lijinghua end
    //add date label
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
    AddFixStrItem(&richtext_index,STR_GAL_LAST_MODIFIED_EXT01,&richtext_item,ctrl_id);

    //get date
    //lijinghua add
    //MMIAPISET_SetDateDisplayType(MMISET_DATE_MDY);
    //lijinghua end
    SCI_MEMSET(detail_str,0,sizeof(detail_str));
    MMIAPISET_FormatDateStrByDateStyle(file_info.modify_date.year,
        file_info.modify_date.mon,file_info.modify_date.mday,
        '-', detail_str,MMIPICVIEW_DETAIL_STR_MAX_LEN);

    //add date
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id); /*lint !e64*/

  //lijinghua add extension label
  SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
   AddFixStrItem(&richtext_index,STR_GAL_EXTENSION_EXT01,&richtext_item,ctrl_id);
  
  //lijinghua add extension
  SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
  richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
  richtext_item.text_data.buf.str_ptr = (file_info.filename + path_len + name_len + 1);
  richtext_item.text_data.buf.len = suffix_len -1;
  GUIRICHTEXT_AddItem(ctrl_id,&richtext_item,&richtext_index);

    //lijinghua delete it
    //add time label
    /*AddFixStrItem(&richtext_index,TXT_COMMON_TIME,&richtext_item,ctrl_id);

    //get time
    SCI_MEMSET(detail_str,0,sizeof(detail_str));
    MMIAPISET_FormatTimeStrByTime(file_info.modify_time.hour,file_info.modify_time.min,detail_str,MMIPICVIEW_DETAIL_STR_MAX_LEN);

    //add time
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);*//*lint !e64*/

    //add size label
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
    AddFixStrItem(&richtext_index,STR_MCD_DTAIL_COD_SIZE,&richtext_item,ctrl_id);

    //get size
    SCI_MEMSET(detail_str,0,sizeof(detail_str));

    MMIAPIFMM_GetFileSizeString(file_info.size, temp_wstr, MMIPICVIEW_DETAIL_STR_MAX_LEN, FALSE);
    MMIAPICOM_WstrToStr(temp_wstr, detail_str);

    //add size
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/

   // lijinghua add file type label
   SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
   AddFixStrItem(&richtext_index,STR_GAL_FILE_TYPE_EXT01,&richtext_item,ctrl_id);
   
   //lijinghua add file type
   SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
   if(MMIPICVIEW_IsVideoSuffix(&file_info))
   {
        AddFixStrItem(&richtext_index,STR_GAL_LIST_VIDEO_ALBUM,&richtext_item,ctrl_id);
   }else
   {
        AddFixStrItem(&richtext_index,STR_LIST_IMAGE_EXT01,&richtext_item,ctrl_id);
   }
#ifdef VIDEOTHUMBNAIL_SUPPORT
   if(MMIPICVIEW_IsVideoSuffix(&file_info))
   {
        wchar  totaltime_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
        uint16  total_name_len = 0;
        uint32 media_length = 0;
        SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        richtext_item.text_set_font_color =  TRUE;
        richtext_item.setting_font_color = MMI_DARK_WHITE_COLOR;
        richtext_item.text_set_font = TRUE;
        richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
        AddFixStrItem(&richtext_index,STR_GAL_LIST_DURATION,&richtext_item,ctrl_id);
        MMIFMM_FmmVideoTotaltimeGetPathname(totaltime_name,&total_name_len,file_info.filename);
        if(!MMIAPIFMM_IsFileExist (totaltime_name, total_name_len)){
            if(!MMIAPIVP_IsThumbnailOpen()){
                MMIAPIVP_GetInfoFromFullPathName (file_info.filename, file_info.name_len, &media_info);
                media_length = media_info.media_length;
            }else
            {
                media_length = 0;
            }
        }else{
            MMIFMM_FmmGetVideoTotaltimeByPathname(&media_length ,totaltime_name);
        }
        SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
        SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
        SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        sprintf ( (char*) detail_arr, "%.2ld:%.2ld:%.2ld", ( (media_length) / (1000*3600)), ( ( (media_length) / (1000*60)) % 60), ( ( (media_length) / 1000) % 60));
        MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
        richtext_item.text_data.buf.str_ptr = wchar_detail_arr;
        AddStrItem(detail_arr,(uint16)strlen((char *)detail_arr),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/
   }else
#endif
   {
	   //add resolution label
        SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        richtext_item.text_set_font_color =  TRUE;
        richtext_item.setting_font_color = MMI_BLACK_COLOR;
        richtext_item.text_set_font = TRUE;
        richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
        AddFixStrItem(&richtext_index,STR_GAL_RESOLUTION_EXT01,&richtext_item,ctrl_id);

        //get picture width and height
        if (0 < file_info.size)
        {
           if (MMIAPIFMM_GetPictureWidthHeight(file_info.filename,file_info.name_len,file_info.size,&pic_width,&pic_height))
           {
        	   SCI_MEMSET(detail_str,0,sizeof(detail_str));
        	   sprintf((char*)detail_str, "%dX%d",pic_width,pic_height);

        	   //add resolution
                    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        	   AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/
           }
           else
           {
                    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
        	   AddFixStrItem(&richtext_index,STR_FILE_NOT_SUPPORT,&richtext_item,ctrl_id);
           }
        }
        else
        {
           SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
           AddFixStrItem(&richtext_index,STR_FILE_EMPTY,&richtext_item,ctrl_id);
        }
   }
    //add location label
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_set_font_color =  TRUE;
    richtext_item.setting_font_color = MMI_BLACK_COLOR;
    richtext_item.text_set_font = TRUE;
    richtext_item.setting_font = MMI_DEFAULT_BIG_FONT;
    AddFixStrItem(&richtext_index,STR_GAL_PATH_EXT01 ,&richtext_item,ctrl_id);
	//lijinghua add for set pic detail
    dev = MMIAPIFMM_GetDeviceTypeByPath(file_info.filename, MMIFILE_DEVICE_LEN);
    if(dev >= MMI_DEVICE_NUM)
    {
        //SCI_TRACE_LOW:"[MMIPIC]:SetDetail dev=%d,ERROR!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3585_112_2_18_2_46_48_86,(uint8*)"d",dev);
        dev = MMI_DEVICE_UDISK;
    }
    MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceMemStatusText(dev),&title_src1_str);
    MMIAPICOM_WstrToStr(file_info.filename,(const uint8 *)file_info_num_str);
    for(i=0;i<path_len-2;i++)
    {
        file_info_num_str[i]=file_info_num_str[i+2];
        if(MMIFILE_SLASH==file_info_num_str[i])
        {
        	file_info_num_str[i]='/';
        }
    }
    MMIAPICOM_StrToWstr((const uint8 *)file_info_num_str, file_info_num_wstr);
    MMIAPICOM_Wstrcat(file_info_wstr, file_info_num_wstr);
    title_src2_str.wstr_ptr = file_info_wstr;
    title_src2_str.wstr_len = MMIAPICOM_Wstrlen(title_src2_str.wstr_ptr);
    MMIAPICOM_CatTwoString(&title_str, &title_src1_str, &title_src2_str);
    
    SCI_MEMSET(&richtext_item, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item.text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,title_src1_str.wstr_len + path_len-2);
    richtext_item.text_data.buf.str_ptr = title_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id,&richtext_item,&richtext_index);
    MMIAPICOM_DestroyString(title_str);
    //AddFixStrItem(&richtext_index, MMIAPIFMM_GetDeviceMemStatusText(dev),&richtext_item,ctrl_id);
    //AddFixStrItem(&richtext_index, MMIAPIFMM_GetDeviceName(dev),&richtext_item,ctrl_id);
   //lijinghua end 
#ifdef DRM_SUPPORT
    //add drm file detail
    MMIPICVIEW_AddDrmDetail(file_info.filename,file_info.name_len,win_id,ctrl_id);
#endif
}

/*****************************************************************************/
//  Description : add fix string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddFixStrItem(
                         uint16             *index_ptr, //in/out
                         MMI_TEXT_ID_T      text_id,
                         GUIRICHTEXT_ITEM_T *richtext_item_ptr,
                         MMI_CTRL_ID_T      ctrl_id
                         )
{
    MMI_STRING_T    item_str = {0};
    
    if (PNULL == index_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddFixStrItem index_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3635_112_2_18_2_46_48_87,(uint8*)"");
        return ;
    }
    
    if (PNULL == richtext_item_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddFixStrItem richtext_item_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3641_112_2_18_2_46_48_88,(uint8*)"");
        return ;
    }

    //get resolution label
    MMI_GetLabelTextByLang(text_id,&item_str);

    //add resolution label
    richtext_item_ptr->text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item_ptr->text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,item_str.wstr_len);
    richtext_item_ptr->text_data.buf.str_ptr = item_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id,richtext_item_ptr,index_ptr);
}

/*****************************************************************************/
//  Description : add string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddStrItem(
                      const uint8           *str_ptr,           //in
                      uint16                str_len,            //in
                      uint16                *index_ptr,         //in/out
                      GUIRICHTEXT_ITEM_T    *richtext_item_ptr, //in
                      MMI_CTRL_ID_T         ctrl_id
                      )
{
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};

    if (PNULL == str_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem str_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3671_112_2_18_2_46_48_89,(uint8*)"");
        return ;
    }
    
    if (PNULL == index_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem index_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3677_112_2_18_2_46_48_90,(uint8*)"");
        return ;
    }
    
    if (PNULL == richtext_item_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem richtext_item_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3683_112_2_18_2_46_48_91,(uint8*)"");
        return ;
    }

    richtext_item_ptr->text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item_ptr->text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,str_len);
    MMI_STRNTOWSTR(item_text, GUIRICHTEXT_TEXT_MAX_LEN, str_ptr, str_len, richtext_item_ptr->text_data.buf.len); /*lint !e64*/
    richtext_item_ptr->text_data.buf.str_ptr = item_text;
    GUIRICHTEXT_AddItem(ctrl_id,richtext_item_ptr,index_ptr);
}

/*****************************************************************************/
//  Description : picture viewer is delete file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDeleteFiles(void)
{
    BOOLEAN         result = FALSE;
    MMI_WIN_ID_T    wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;

    if(MMK_IsOpenWin(wait_win_id))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop delete file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_StopDeleteFiles(void)
{
    StopDelete();
    MMK_CloseWin(MMIPICVIEW_DEL_WAIT_WIN_ID);
}


#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle list preview longok menu win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistLongokOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32                  cur_index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T            query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T           sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL2_ID;
    MMI_CTRL_ID_T           list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T           menu_id = 0;
    MMI_MENU_GROUP_ID_T     group_id = 0;
    GUIMENU_TITLE_INFO_T    title_info = {0};
    GUIMENU_BUTTON_INFO_T   button_info = {0};
    MMI_STRING_T            main_title = {0};
    FILEARRAY_DATA_T		file_info = {0};
    wchar                   file_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint16                  file_name_len = 0;
    uint32                  list_item_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayListLongokOptMenu(list_ctrl_id);
        
        //set title
        title_info.is_static    = TRUE;
        title_info.group_id     = MENU_PICVIEW_LIST_LONGOK_OPT;
        // get file name
        if(MMIPICVIEW_GetPicFileInfo(list_item_index, &file_info))
        {
	        MMIAPIFMM_SplitFullPath(file_info.filename,
	            file_info.name_len,
	            PNULL,
	            PNULL,
	            PNULL,
	            PNULL,
	            file_name,
	            &file_name_len);
        }
        
        main_title.wstr_ptr = file_name;
        main_title.wstr_len = file_name_len;
        title_info.title_ptr = &main_title;
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = MENU_PICVIEW_LIST_LONGOK_OPT;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_PICVIEW_MMS:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

#ifdef FM_SUPPORT
            if(MMIAPIFM_IsFmActive()) 
            {
                SCI_TRACE_LOW("HandlePreviewOptWinMsg FM is avtiving!!");
                MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
            }
            else
            {
                SendPicByBt(cur_index);
            }
#else
            SendPicByBt(cur_index);
#endif
            break;
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
                
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif            
        case ID_PICVIEW_SHARE_ITEMS:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SHARE_ITEMS_LIST_OPT_WIN_TAB, PNULL);
	    break;

        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;

        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif            
#ifdef PIC_EDITOR_SUPPORT
        case ID_PICVIEW_EDIT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;

#endif

        case ID_PICVIEW_DELETE:
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
                if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
#endif
			{
				//prompt query window
				MMIPUB_OpenQueryWinByTextId(STR_GAL_HDR_DELETE_EXT02,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
			}
            break;

        case ID_PICVIEW_RENAME:
            MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePiclistLongokOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3882_112_2_18_2_46_49_92,(uint8*)"d", menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
	MMI_GetLabelTextByLang(STR_GAL_HDR_GALLERY_EXT01, &title_str );
	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
	{
	     GUIWIN_UpdateStb();
	}
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set list longok opt menu gray
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListLongokOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              )
{
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_LIST_LONGOK_OPT;
    
    if(0 < MMIPICVIEW_GetAllPicNum())
    {
        uint16                  cur_index = 0;
        uint16                  mark_num = 0;
        FILEARRAY_DATA_T        file_info = {0};
        
        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
        
        
        //get mark num
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if (0 < mark_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MMS,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_BT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
        }
        else
        {
            //get picture info,only jpg support picture editor
            if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) && 
                (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len)))
            {
                //do nothing
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            }
        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MMS,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_BT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SLIDE_PREVIEW,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DELETE,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
    }
}

/*****************************************************************************/
//  Description : open pic list longok menu win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicListLongokOptWin(void)
{
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_LONGOK_OPT_WIN_TAB,PNULL);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle pda style setting win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPdaSettingWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID;
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    uint16              cur_item = 0;
    uint32              user_data = 0;
    static int32        list_offset_y = 0;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        list_offset_y = 0;
        InitPicPdaSettingsListCtrl();
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_LOSE_FOCUS:
        GUILIST_GetTopItemOffset(ctrl_id, &list_offset_y);
        break;
        
    case MSG_GET_FOCUS:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitPicPdaSettingsListCtrl();
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        //set list offset
        GUILIST_SetTopItemOffset(ctrl_id, list_offset_y);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, cur_item, &user_data);
        
        switch((MMIPICVIEW_PDA_SETTING_TYPE_E)user_data)
        {
         case MMIPICVIEW_PDA_SETTING_CHANGE_FOLDER:
            {
                OpenSetDefaultPathWin(); 
            }
            MMK_CloseWin(win_id);
            break;            
            
        case MMIPICVIEW_PDA_SETTING_EFFECT:
            MMK_CreatePubListWin((uint32 *)MMIVP_SETTING_EFFECT_WIN_TAB, PNULL);
            break;

        case MMIPICVIEW_PDA_SETTING_TIME:
            MMK_CreatePubListWin((uint32 *)MMIVP_SETTING_TIME_WIN_TAB, PNULL);
            break;       
        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
    break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitPicPdaSettingsListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id            = MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID;
    
    MMIPICVIEW_DEFAULT_PATH_T    default_path = {0};
    GUILIST_ITEM_T                          item_t = {0};
    GUILIST_ITEM_DATA_T                item_data = {0};


    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_PDA_SETTING_MAX, FALSE);
       
    AppendSplitListItem(ctrl_id, TXT_PICVIEW_SLIDE_SET);
    //Effect
    AppendPicPdaSettingsItem(
        ctrl_id,
        TXT_EFFECT,
        GetTextIdByEffectType(MMIAPIPICVIEW_GetSlideEffect()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIPICVIEW_PDA_SETTING_EFFECT);
    
    //Time
    AppendPicPdaSettingsItem(
        ctrl_id,
        TXT_PICVIEW_SLIDE_TIME,
        GetTextIdByTimeType(MMIPICVIEW_GetSlideTime()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIPICVIEW_PDA_SETTING_TIME);

#if defined(CMCC_UI_STYLE)
    if (MMIPICVIEW_DOWNLOAD_PIC != MMIPICVIEW_GetPathType())
    {
#endif
        AppendSplitListItem(ctrl_id, TXT_PICVIEW_CHANGE_FOLDER);
        
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);
        
        //chang folder
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
        item_t.item_data_ptr = &item_data;
        item_t.user_data     = (uint32)MMIPICVIEW_PDA_SETTING_CHANGE_FOLDER;
        
        //text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_PICVIEW_CHANGE_FOLDER;
        
        //anim
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = IMAGE_PICVIEW_CHANGE_FOLDER;
        
        //text 2
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = default_path.path;   
        item_data.item_content[2].item_data.text_buffer.wstr_len = default_path.path_len;
        
        GUILIST_AppendItem(ctrl_id, &item_t);
#if defined(CMCC_UI_STYLE)
    }
#endif

}
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendPicPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_TEXT_ID_T  text_2,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;
    //text 2
    if(TXT_NULL != text_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_2;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : get text_id by view type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByViewType(MMIPICVIEW_VIEW_TYPE_E view_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        text_id = TXT_COMMON_LIST;
        break;
      
    case MMIPICVIEW_VIEW_ICONLIST:
        text_id = TXT_PICVIEW_VIEW_ICONLIST;
        break;
        
    default:
        break;
    }
    
    return text_id;
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get text_id by effect type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEffectType(MMIPICVIEW_EFFECT_TYPE_E effect_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(effect_type)
    {
    case MMIPICVIEW_EFFECT_ORDER:
        text_id = TXT_PICVIEW_EFFECT_ORDER;
        break;
        
    case MMIPICVIEW_EFFECT_ENLARGE:
        text_id = TXT_PICVIEW_EFFECT_ENLARGE;
        break;
        
    case MMIPICVIEW_EFFECT_GRID:
        text_id = TXT_PICVIEW_EFFECT_GRID;
        break;
        
    case MMIPICVIEW_EFFECT_LEFTWARD:
        text_id = TXT_PICVIEW_EFFECT_LEFTWARD;
        break;
        
    case MMIPICVIEW_EFFECT_RIGHTWARD:
        text_id = TXT_PICVIEW_EFFECT_RIGHTWARD;
        break;
        
    case MMIPICVIEW_EFFECT_UPWARD:
        text_id = TXT_PICVIEW_EFFECT_UPWARD;
        break;
        
    case MMIPICVIEW_EFFECT_DOWNWARD:
        text_id = TXT_PICVIEW_EFFECT_DOWNWARD;
        break;
        
#ifdef EFFECT_RIPPLE_SUPPORT
    case MMIPICVIEW_EFFECT_RIPPLE:
        text_id = TXT_PICVIEW_EFFECT_RIPPLE;
        break;
#endif
        
    case MMIPICVIEW_EFFECT_NONE:
        text_id = TXT_NONE;
        break;
        
    default:
        break;
    }
    
    return text_id;
}

/*****************************************************************************/
//  Description : get text_id by slide time type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByTimeType(MMIPICVIEW_TIME_TYPE_E time_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(time_type)
    {
    case MMIPICVIEW_TIME_2S:
        text_id = TXT_COMM_2_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_3S:
        text_id = TXT_COMM_3_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_6S:
        text_id = TXT_COMM_6_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_10S:
        text_id = TXT_COMM_10_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_16S:
        text_id = TXT_PICVIEW_SLIDE_TIME_16S;
        break;
        
    case MMIPICVIEW_TIME_20S:
        text_id = TXT_COMM_20_SECONDS;
        break;
        
    default:
        break;
    }
    
    return text_id;
}
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle view set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicViewSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_VIEW_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendViewListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        if((MMIPICVIEW_VIEW_TYPE_E)index != MMIPICVIEW_GetViewType())
        {
            MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendViewListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_VIEW_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_VIEW_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_LIST,     STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_VIEW_ICONLIST, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    switch(MMIPICVIEW_GetViewType())
    {
    case MMIPICVIEW_VIEW_LIST:
        cur_selection = MMIPICVIEW_VIEW_LIST;
        break;
       
    case MMIPICVIEW_VIEW_ICONLIST:
        cur_selection = MMIPICVIEW_VIEW_ICONLIST;
        break;
        
    default:
        break;
    }
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicEffectSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendEffectListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        SetSlideEffectIndex(index);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendEffectListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_EFFECT_TYPE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_ORDER,      TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_ENLARGE,    TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_GRID,       TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_LEFTWARD,   TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_RIGHTWARD,  TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_UPWARD,     TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_DOWNWARD,   TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#ifdef EFFECT_RIPPLE_SUPPORT    
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_RIPPLE,     TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
    MMIAPISET_AppendListItemByTextId(TXT_NONE,       TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    cur_selection = GetSlideEffectIndex();
    
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}



/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicTimeSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_TIME_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendTimeListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        SetSlideTimeIndex(index);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendTimeListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_TIME_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_TIME_TYPE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_COMM_2_SECONDS,      STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_3_SECONDS,      STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_6_SECONDS,      STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_10_SECONDS,     STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_SLIDE_TIME_16S,     STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_20_SECONDS,     STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    cur_selection = GetSlideTimeIndex();
    
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#endif

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
#ifdef CAMERAROLL_SUPPORT	
PUBLIC MMI_RESULT_E MMIAPIPICVIEW_HandleCameraRollSig(MMI_MESSAGE_ID_E  msg_id,
                                 DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
        //if initialize finish-->MMIPICVIEW_LIST_WIN_TAB
	SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig receive %x" , msg_id);

	switch (msg_id)
	{
	case MSG_CAMERA_ROLL_DELETE_PHOTO_NOTIFY:
            if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
            }
        break;
	case MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY :
	case MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF:
            if (MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
            {
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_PREVIEW_WIN_ID");
                MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, msg_id, param);
            }
            if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
            }
        break;
	case MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY  :
	case MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF:
            if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
            }
        break;
	case MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_NOTIFY :
        case MSG_CAMERA_ROLL_DOWNLOAD_PHOTO_CNF:
            if(MMK_IsOpenWin(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID))
	    {
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID, msg_id, param);
	    }
        break;
       case MSG_CAMERA_ROLL_INIT_CNF:
    {
        CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
        CAMERA_ROLL_TYPE_E fromtype = CAMERA_ROLL_TYPE_NONE;
        if(param!=PNULL){
            fromtype = data->camera_roll_msg_data.init_type; 
        }
        if(MMK_IsOpenWin(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID)&& fromtype==CAMERA_ROLL_TYPE_DOWNLOAD_PHOTO)
        {
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID, msg_id, param);
        }
        else if(MMK_IsOpenWin(MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID)&& fromtype==CAMERA_ROLL_TYPE_MANUAL_UPLOAD_SINGLE_PHOTO)
	{
		SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID");
		HandleUploadOnePicture(MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID, msg_id, param);
	}
        else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)&& fromtype==CAMERA_ROLL_TYPE_MANUAL_UPLOAD_MULTI_PHOTO)
        {
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_LIST_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
        }
        else if(MMK_IsOpenWin(MMIPICVIEW_SHARELINK_WAITING_WIN_ID)&& fromtype==CAMERA_ROLL_TYPE_GET_SHARELINK)
        {
            SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_SHARELINK_WAITING_WIN_ID");
            MMK_SendMsg(MMIPICVIEW_SHARELINK_WAITING_WIN_ID, msg_id, param);
        }
	else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
	{
		MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
		SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_LIST_WIN_ID");
	}
        else{
		SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig error ");
        }
	}
        break;
        case MSG_CAMERA_ROLL_ERROR_NOTIFY:
        if(MMK_IsOpenWin(MMIPICVIEW_SHARELINK_WAITING_WIN_ID)){
        	SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_SHARELINK_WAITING_WIN_ID");
        	MMK_SendMsg(MMIPICVIEW_SHARELINK_WAITING_WIN_ID, msg_id, param);
        }
	else if(MMK_IsOpenWin(MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID))
	{
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID");
		HandleUploadOnePicture(MMIPICVIEW_UPLOAD_TO_ONEDRIVE_WIN_ID, msg_id, param);
	}
	else if(MMK_IsOpenWin(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID))
	{
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID, msg_id, param);
	}
        else if(MMK_IsOpenWin(MMIPICVIEW_UPLOAD_ITEMS_QUERY_WIN_ID))
        {
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_UPLOAD_ITEMS_QUERY_WIN_ID");
		MMK_SendMsg(MMIPICVIEW_UPLOAD_ITEMS_QUERY_WIN_ID, msg_id, param);
        }
        else if (MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
        {
        	SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_PREVIEW_WIN_ID");
        	MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, msg_id, param);
        }
        else if (MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
	{
		MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, msg_id, param);
		SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_LIST_WIN_ID");
        }
	else
	{
            SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to NONE!!! ");
	}

            break;
        case MSG_CAMERA_ROLL_SHARE_LINK_CNF:
        if(MMK_IsOpenWin(MMIPICVIEW_SHARELINK_WAITING_WIN_ID)){
            SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_SHARELINK_WAITING_WIN_ID");
            MMK_SendMsg(MMIPICVIEW_SHARELINK_WAITING_WIN_ID, msg_id, param);
        }else if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send msg to MMIPICVIEW_PREVIEW_WIN_ID");
            MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, msg_id, param);
        }
        break;
        case MSG_CAMERA_ROLL_SIGNOFF_CNF:
            SCI_TRACE_LOW("MMIAPIPICVIEW_HandleCameraRollSig send MSG_CAMERA_ROLL_SIGNOFF_CNF");
            MMIAPIPICVIEW_DeleteAfterSignOff();
            break;
	default:
		break;
	}
    return result;
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Discription: append split item
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_TEXT_ID_T    text_id
                               )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
    
    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SPLIT;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );
}

#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Discription: delete cur preview file
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeletePreviewFile(MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_WIN_ID_T        query_win_id    = MMIPICVIEW_QUERY_WIN_ID;

#ifdef DRM_SUPPORT
    if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
    {
        //prompt query window
        MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
    }
    else
#endif
    {
        //prompt query window
        MMIPUB_OpenQueryWinByTextId(STR_GAL_HDR_DELETE_EXT02,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
    }
}

/*****************************************************************************/
//  Discription: open set win
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenSetWin(void)
{
    MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
}
#endif


/*****************************************************************************/
//  Description : get pic or video info
//  Global resource dependence : 
//  Author: lijinghua
//  Note: 
/*****************************************************************************/

LOCAL uint32 MMIAPIPICVIEW_GetRecentPicFileInfo(uint32 index, uint32 pic_num){
	uint32 item_index = index;
	uint32 current_total = 0;
	FILEARRAY_DATA_T file_info = {0};

	//From index to pic_num -1
	while(item_index < pic_num){
		if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info)){
   	     BOOLEAN ret = MMIPICVIEW_IsVideoSuffix(&file_info);
			if(FALSE == ret){
				break;
			}
		}
		item_index ++;
		current_total ++;
	}
	
	//From 0 to pic_num - current_total.
	if(item_index >= pic_num
		&& current_total < pic_num){
		item_index = 0;
		while(item_index < pic_num - current_total){
			if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info)){
				BOOLEAN ret = MMIPICVIEW_IsVideoSuffix(&file_info);
				if(FALSE == ret){
					break;
				}
			}
			item_index ++;

		}
		//If has not valid pic file, return the pic_num taht is not a valid index.
		if(item_index >= pic_num - current_total){
			item_index = pic_num;
		}
	}
	return item_index;
}

/*Edit by script, ignore 1 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527
/*****************************************************************************/
//  Discription: set delete icon index when delete in other apps such as vp
//  Global resource dependence: none 
//  Author: zhuyongjing
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_SetDeleteIndex(void)
{
    s_mmipicview_is_preview_del = TRUE;
    s_mmipicview_del_file_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
}
PUBLIC void MMIAPIPICVIEW_RenameFromVP(wchar *new_name_ptr, uint16 new_name_len)
{
    MMI_CTRL_ID_T   list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint16 cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    UpdateListArray(cur_index,new_name_ptr,new_name_len);
   MMIPICVIEW_ModifyItemText(list_ctrl_id,cur_index);
}
PUBLIC uint16 MMIAPIPICVIEW_GetDeleteIndex(void)
{
    return s_mmipicview_del_file_index;
}

PUBLIC void MMIAPIPICVIEW_SetFatDelIng(BOOLEAN flag)
{
    s_mmipicview_is_fat_deleteing = flag;
}
LOCAL void MMIAPIPICVIEW_DeleteAfterSignOff(void)
{
#ifdef MMI_SHOW_MSA
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;   

    if(MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD))
    {
        if (MMIAPIFMM_CombineFullPath (MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                                       MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS, MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS_LEN,
                                       NULL, 0,
                                       full_path_name, &full_path_len))
        {
            if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
            {
                MMIAPIFMM_CreateDir (full_path_name, full_path_len);
            }
            MMIAPIPICVIEW_SetAttr(full_path_name, full_path_len, FALSE, FALSE, FALSE, FALSE);
            MMIAPIFMM_DeleteTreeEx(L"E:",MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS,FALSE);

            if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len))
            {
                MMIAPIFMM_CreateDir (full_path_name, full_path_len);
            }
            MMIAPIPICVIEW_SetAttr(full_path_name, full_path_len, TRUE, FALSE, FALSE, FALSE);

        }
    }
    MMIPICVIEW_DestroyThumbnailsArray();
#if defined(MSA_SUPPORT)	
    MMIAPIMSA_Signoff_AP_Clean_user_data_Done(MSA_APP_TYPE_CAMERA_ROLL);
#endif
#endif
}
LOCAL BOOLEAN Multi_Upload_Pics_Signal(void)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T*sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        GALLERY_FILEARRAY_UPLOAD_FILTER_FILEARRAY,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        );
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
        return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
        return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}
LOCAL BOOLEAN Markable_Delete_Filearray_signal(void)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T*sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        GALLERY_FILEARRAY_MARKABLE_DELETE_FILEARRAY,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        );
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
        return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
        return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}
LOCAL BOOLEAN Onedrive_Disconnect_Delete_Filearray_Signal(void)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T*sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        GALLERY_FILEARRAY_DISCONNECT_DELETE_FILEARRAY,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        );
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
        return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
        return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}
PUBLIC BOOLEAN Goto_Subfolder_Filter_Signal(void)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T*sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        GALLERY_FILEARRAY_SUBFOLDER_FILTER_FILEARRAY,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        ); 
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
        return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
        return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}

PUBLIC BOOLEAN Server_del_thumbnails_Signal(void)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T*sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        ); 
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
        return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
        return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}
PUBLIC MMI_RESULT_E MMIAPIPICVIEW_HandleGalleryTaskSig(MMI_MESSAGE_ID_E  msg_id,
                                 DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    SCI_TRACE_LOW("MMIAPIPICVIEW_HandleGalleryTaskSig receive %x" , msg_id);
    switch (msg_id)
    {
        case MSG_PICVIEWER_MUTIL_UPLOAD_FINISH:
        case MSG_PICVIEWER_DISCONNECT_DELETE_FINISH:
        case MSG_PICVIEWER_MARKABLE_DELETE_FINISH:
        case MSG_PICVIEWER_GOTO_SUBFOLDER_FINISH:
        case MSG_PICVIEWER_SERVER_DELETE_FINISH:
            if(MMK_IsOpenWin(MMIPICVIEW_TASK_WAITING_WIN_ID)){
                MMK_SendMsg(MMIPICVIEW_TASK_WAITING_WIN_ID, msg_id, param);
            }else{
                SCI_TRACE_LOW("MMIAPIPICVIEW_HandleGalleryTaskSig MMIPICVIEW_TASK_WAITING_WIN_ID close");
            }
        break;
        case MSG_PICVIEWER_GET_THUMBNAILS_FINISH:
            break;
      default:
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description :  task waiting by spinner win
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTaskWaitSpinnerWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    		result = MMI_RESULT_TRUE;
    MMI_HANDLE_T spinner_ctr_id = NULL;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};
    static uint task_wait_timer_id = 0;
    static BOOLEAN need_send_cancel = FALSE;
    GALLERY_FILEARRAY_SIG_TYPE_E       task_type = (GALLERY_FILEARRAY_SIG_TYPE_E)MMK_GetWinAddDataPtr(win_id);

    SCI_TRACE_LOW("[MMIPIC]  HandleTaskWaitSpinnerWinMsg ,msg_id=%x",msg_id);
    spinner_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPICVIEW_WAITING_SPINNER_CTRL_ID);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_task_cancel = GALLERY_TASK_IDLE;
        need_send_cancel = FALSE;
        break;
    case MSG_GET_FOCUS:
        if (task_wait_timer_id != 0)
        {
            MMK_StopTimer(task_wait_timer_id);
            task_wait_timer_id = 0;
        }
        task_wait_timer_id = MMK_CreateWinTimer(win_id, 200,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);
        break;
#ifdef CAMERAROLL_SUPPORT			
    case MSG_PICVIEWER_MUTIL_UPLOAD_FINISH:
   { 
        GALLERY_FILEARRAY_SIG_T * data =(GALLERY_FILEARRAY_SIG_T *)param;
        uint16 index = 0;
        if(task_type != GALLERY_FILEARRAY_UPLOAD_FILTER_FILEARRAY){
            break;
        }

        if(data != PNULL)
        {
            if(data->res > 0){
                index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
            }
            MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, &index);
        }
        MMK_CloseWin(win_id);
    }
        break;

    case MSG_PICVIEWER_DISCONNECT_DELETE_FINISH:
   { 
        GALLERY_DISCONNECT_TYPE_E disconnect_type = GALLERY_DISCONNECT_TYPE_MAX;
        if(task_type != GALLERY_FILEARRAY_DISCONNECT_DELETE_FILEARRAY){
            break;
        }
        if(MMIPICVIEW_GetAllPicNum() > 0){
            disconnect_type = GALLERY_DISCONNECT_MAIN_MORETHAN_THUMBNAIL;
        }else{
            disconnect_type = GALLERY_DISCONNECT_MAIN_EQUAL_THUMBNAIL;
        }
	#ifdef CAMERAROLL_SUPPORT	
        if(MMIPICVIEW_GetMultiUploadingDone()){
            MMISRV_CAMERAROLL_Clean_manual_upload_list();
            MMISRV_CAMERAROLL_Cancel_Upload();
        }
	#endif
        FinishLoadPic(MMIPICVIEW_LIST_WIN_ID,FALSE);
        MMK_CloseWin(win_id);
    }
        break;
#endif		
    case MSG_PICVIEWER_MARKABLE_DELETE_FINISH:
    {
        uint16 index = 0;
        if(task_type != GALLERY_FILEARRAY_MARKABLE_DELETE_FILEARRAY){
            break;
        }
        if(MMIPICVIEW_GetAllPicNum() > 0){
            index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        }
        MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, &index);
        MMK_CloseWin(win_id);
    }
        break;
    case MSG_PICVIEWER_GOTO_SUBFOLDER_FINISH:
        if(task_type != GALLERY_FILEARRAY_SUBFOLDER_FILTER_FILEARRAY){
            break;
        }
        FinishLoadPic(MMIPICVIEW_LIST_WIN_ID,FALSE);
	#ifdef CAMERAROLL_SUPPORT	
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
            MMIPIC_CreateOneDriveSyncProgressDialogWin();
        }
	#endif
        MMK_CloseWin(win_id);
        break;
#ifdef CAMERAROLL_SUPPORT			
    case MSG_PICVIEWER_SERVER_DELETE_FINISH:
    {
        uint16 cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        if(task_type != GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY){
            break;
        }
        if(MMIPICVIEW_GetAllPicNum() > 0){
            if(cur_index>= MMIPICVIEW_GetAllPicNum()){
                cur_index = MMIPICVIEW_GetAllPicNum()-1;
            }
            MMIPICVIEW_PostUpdateMsg(cur_index);
        }else{
            FinishLoadPic(MMIPICVIEW_LIST_WIN_ID,FALSE);
        }
        MMK_CloseWin(win_id);
    }
    break;
#endif
    case MSG_TIMER:  
        if(task_wait_timer_id == *((uint8 *) param)&& s_task_cancel == GALLERY_TASK_STOPPED){
            if((MMIPICVIEW_GetCurrentOwnTaskId()==GALLERY_FILEARRAY_UPLOAD_FILTER_FILEARRAY
                ||MMIPICVIEW_GetCurrentOwnTaskId()==GALLERY_FILEARRAY_MARKABLE_DELETE_FILEARRAY
                ||MMIPICVIEW_GetCurrentOwnTaskId()==GALLERY_FILEARRAY_SUBFOLDER_FILTER_FILEARRAY)
                && need_send_cancel){
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_CTL_CANCEL, PNULL);
            }
            MMK_CloseWin(win_id);
            if(!need_send_cancel){
                MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
            }
        }
        break;
    //case MSG_APP_FLIP:
    case MSG_APP_RED:
        if(s_task_cancel == GALLERY_TASK_IDLE){
            s_task_cancel = GALLERY_TASK_STOPPING;
        }
        need_send_cancel = FALSE;
        break;
    case MSG_APP_CANCEL:
        if(s_task_cancel == GALLERY_TASK_IDLE){
            s_task_cancel = GALLERY_TASK_STOPPING;
            need_send_cancel = TRUE;
        }
        break;
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        if(0 != task_wait_timer_id){
            MMK_StopTimer(task_wait_timer_id); 
            task_wait_timer_id = 0;
        }
        break;
    default:
        break;
    }
    return (result);
}
PUBLIC BOOLEAN Gallery_SendSignalToFilearrayTask(uint16 sig_id)
{ 
    GALLERY_FILEARRAY_HANDLE_SIG_T *sig_ptr = PNULL;
    if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
        return SCI_ERROR; 
    }
    MMI_CREATE_SIGNAL(sig_ptr,
        sig_id,
        sizeof(GALLERY_FILEARRAY_HANDLE_SIG_T),
        SCI_IdentifyThread()
        );
        
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_filearray_task_id))
    {
            return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
            return SCI_ERROR;                                                                                                                                
    }    
}

PUBLIC  void Gallery_Task_SendSignalToMMI(uint32 sig_id,uint8 res)
{
	GALLERY_FILEARRAY_SIG_T *signal = PNULL;
        if(s_task_cancel == GALLERY_TASK_STOPPING){
            return;
        }
        if(g_filearray_task_id == SCI_INVALID_BLOCK_ID){
            return;
        }
	MmiCreateSignal(sig_id, sizeof(GALLERY_FILEARRAY_SIG_T), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->res  = res;
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
}
/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL void FilearrayTask_Entry (
    uint32 argc,
    void * argv
)
{
    GALLERY_FILEARRAY_HANDLE_SIG_T    *gallery_sig = NULL;
    xSignalHeaderRec    *psig = NULL;
    
    while (1) /*lint !e716*/
    {
        MMI_RECEIVE_SIGNAL ( (xSignalHeader*) psig, g_filearray_task_id);/*lint !e63*/
        SCI_ASSERT(PNULL != psig);/*assert verified*/
        SCI_TRACE_LOW ("[mmipic]FilearrayTask_Entry psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/
	gallery_sig = (GALLERY_FILEARRAY_HANDLE_SIG_T *)psig;	
        MMIPICVIEW_SetCurrentOwnTaskId(psig->SignalCode);
        switch (psig->SignalCode) /*lint !e613*/
        {
        #ifdef CAMERAROLL_SUPPORT	
            case GALLERY_FILEARRAY_UPLOAD_FILTER_FILEARRAY:
            {
                FILEARRAY s_onedrive_upload_success_file_array = MMIAPIFILEARRAY_Create();
                FILEARRAY s_onedrive_manual_upload_file_array = MMIAPIFILEARRAY_Create();
                wchar *s_onedrive_upload_success_file_path = L"D:\\onedrive_upload_success.txt";
                wchar *s_onedrive_manual_upload_list_file_path = L"D:\\onedrive_manual_upload_list.txt";

                MMIPICVIEW_SetMultiChoiseSaveToOneDrive(TRUE);
                MMIPICVIEW_GetFileArrayFromOtherForArray(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIPICVIEW_GetAllPicArray());

                //delete uploaded files
                if(MMIAPIFMM_IsFileExist(s_onedrive_upload_success_file_path , MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path)))
                {
                    MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_upload_success_file_array , s_onedrive_upload_success_file_path ,MMIAPICOM_Wstrlen(s_onedrive_upload_success_file_path));
                }
                MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),s_onedrive_upload_success_file_array, FALSE,FALSE);
                MMIAPIFILEARRAY_Destroy(&s_onedrive_upload_success_file_array);
                //delete manu uploading files
                if(MMIAPIFMM_IsFileExist(s_onedrive_manual_upload_list_file_path , MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path)))
                {
                    MMIAPIFILEARRAY_LoadArrayFromFile(s_onedrive_manual_upload_file_array , s_onedrive_manual_upload_list_file_path ,MMIAPICOM_Wstrlen(s_onedrive_manual_upload_list_file_path));
                }
                MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),s_onedrive_manual_upload_file_array, FALSE,FALSE);
                MMIAPIFILEARRAY_Destroy(&s_onedrive_manual_upload_file_array);
                //MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray(), TRUE,FALSE);
                //FilterFileArrayByAttri(MMIAPIPICVIEW_GetAllPicArray(),FALSE,TRUE);
                MMIAPIPICVIEW_AllLocalPicFileArray(MMIAPIPICVIEW_GetAllPicArray());
                if(MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetAllPicArray()) > 0){
                    Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_MUTIL_UPLOAD_FINISH,1);
                }else{
                    Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_MUTIL_UPLOAD_FINISH,0);
                }
            }
            break;
            case GALLERY_FILEARRAY_DISCONNECT_DELETE_FILEARRAY:
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                    ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                    MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetAllPicArray(),FALSE,TRUE);
                    if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                        MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetBackupAllPicArray(),FALSE,TRUE);
                    }
                  //  MMIPICVIEW_DestroyThumbnailsArray();
                }else{
                    MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetBackupAllPicArray(),FALSE,TRUE);
                 //   MMIPICVIEW_DestroyThumbnailsArray();
                }
                    Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_DISCONNECT_DELETE_FINISH,0);
            break;
	#endif
            case GALLERY_FILEARRAY_MARKABLE_DELETE_FILEARRAY:
                MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetAllPicArray(),FALSE,FALSE);
                Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_MARKABLE_DELETE_FINISH,0);
            break;
            case GALLERY_FILEARRAY_SUBFOLDER_FILTER_FILEARRAY:
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                    FILEARRAY temparray = MMIAPIFILEARRAY_Create();
                    MMIPICVIEW_GetFileArrayFromOtherForArray(temparray,MMIAPIPICVIEW_GetAllPicArray());
                    if(!MMIAPIPICVIEW_GetPreviousFolderIsSubfolder()){
                        MMIPICVIEW_GetFileArrayFromOtherForArray(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIPICVIEW_GetAllPicArray());
                        MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetShowFolderPath());
                    }else{
                        MMIPICVIEW_GetFileArrayFromOtherForArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetBackupAllPicArray());
                        MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetShowFolderPath());
                    }
                    MMIAPIFILEARRAY_Destroy(&temparray);
                    Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_GOTO_SUBFOLDER_FINISH,0);
                }
                else{
                    if(!MMIAPIPICVIEW_GetPreviousFolderIsSubfolder()){//from main view to subview
                        MMIPICVIEW_GetFileArrayFromOtherForArray(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIPICVIEW_GetAllPicArray());
                    }else{//from subview1 to subview2 when subview1 is empty
                        MMIPICVIEW_GetFileArrayFromOtherForArray(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetBackupAllPicArray());
                    }
                    MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetShowFolderPath());
                    Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_GOTO_SUBFOLDER_FINISH,0);
                }

            break;
#ifdef CAMERAROLL_SUPPORT	
            case GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY:
            {
                FILEARRAY delete_array = MMISRV_CAMERAROLL_Get_Delete_photo_FileAry();
                //MMIAPIPICVIEW_DeleteArray(MMIAPIPICVIEW_GetCurPicArray(),delete_array, FALSE,TRUE);  
                if(MMIAPIPICVIEW_DeleteArrayWithSubName(MMIAPIPICVIEW_GetCurPicArray(),delete_array, FALSE,TRUE)){    
                    SCI_TRACE_LOW("[mmipic] GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY MMIAPIPICVIEW_DeleteArray ok!");
                }else{
                    SCI_TRACE_LOW("[mmipic] GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY MMIAPIPICVIEW_DeleteArray fail!");
                }
                MMIAPIFILEARRAY_Destroy(&delete_array);
                Gallery_Task_SendSignalToMMI(MSG_PICVIEWER_SERVER_DELETE_FINISH,0);
            }
                break;
#endif
            default:
            break;
        }
        SCI_TRACE_LOW ("[mmipic] gallery_Task: handle signal done , free Memory!");
        if(s_task_cancel == GALLERY_TASK_STOPPING){
            s_task_cancel = GALLERY_TASK_STOPPED;
        }
        MMI_FREE_SIGNAL (psig);
    }
}

/*****************************************************************************/
//  Description : create gallery filearray hanle task
//  Global resource dependence :
//  Author: zhu.yongjing
//  Note:
/*****************************************************************************/
PUBLIC void GalleryFileArray_CreateTask (void)
{
	
    if (SCI_INVALID_BLOCK_ID != g_filearray_task_id)
    {
        //之前创建的task还没有关闭!
        SCI_TRACE_LOW("[MMIPIC]GalleryFileArray_CreateTask hasn't been closed!");
        return;
    }
    g_filearray_task_id = SCI_CreateAppThread (
#ifdef WIN32
                             "T_P_APP_FILE_TASK",
                             "Q_P_APP_FILE_TASK",
#else
                             "T_P_APP_GALLERY_TASK",
                             "Q_P_APP_GALLERY_TASK",
#endif
                             FilearrayTask_Entry,
                             0,
                             0,
                             1024*20,
                             20,
#ifdef WIN32
                            PRI_APP_FILE_TASK,
#else
                             PRI_GALLERY_FILEARRAY_TASK,
#endif
                             SCI_PREEMPT,
                             SCI_AUTO_START);

    MMIPICVIEW_SetCurrentOwnTaskId(GALLERY_FILEARRAY_SIG_NONE);
    SCI_TRACE_LOW("GalleryFileArray_CreateTask, g_filearray_task_id = %d" , g_filearray_task_id);

    //MMISRV_CAMERAROLL_Download_Thumbnail();	
}
PUBLIC BOOLEAN MMIAPIPICVIEW_GetTaskCancelFlag(void)
{
    BOOLEAN result = FALSE;
    if(s_task_cancel != GALLERY_TASK_IDLE){
        result = TRUE;
   }
   return result;
}
    

PUBLIC void MMIAPIPICVIEW_ReplaceTextIdByTwoNum(MMI_TEXT_ID_T text_id,uint16 num1,uint16 num2,MMI_STRING_T * dest_string)
{

    wchar   temp_text[128] = {0};
    char num_str[10+1] = {0};
    wchar num_wstr[10+1] = {0};
    wchar                   text[128] = {0};
    MMI_STRING_T          temp_str = {0};
    
    temp_str.wstr_ptr =text;
    MMIAPICOM_CustomGenerateNumReplaceStrByTextId(text_id, L"%0N",num1,64,&temp_str);

    sprintf((char *)num_str,"%d",num2);
    MMIAPICOM_StrToWstr(num_str, num_wstr);
    MMIAPICOM_CustomGenerateReplaceStr(&temp_str, L"%1N", num_wstr, 128,dest_string);
}

#endif //#ifdef PIC_VIEWER_SUPPORT
/*Edit by script, ignore 10 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
