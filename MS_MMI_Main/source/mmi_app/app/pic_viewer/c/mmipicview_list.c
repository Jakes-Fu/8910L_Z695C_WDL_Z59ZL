/*****************************************************************************
** File Name:      mmipicview_list.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_LIST_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "window_parse.h"
#include "guilistbox.h"
#include "guiiconlist.h"
#include "guiiconfolder.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmipicview_export.h"
#include "mmipicview_internal.h"
#include "mmipicview_id.h"
#include "mmipicview_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmidc_setting.h"
#include "mmidc_save.h"
#include "mmipicview_image.h"
#include "mmipicview_position.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_image.h"
#endif
#include "mmimms_id.h"
#include "mmibt_id.h"
//#ifdef PIC_PREVIEW_U200_SUPPORT
#include "guires.h"
#include "mmidisplay_data.h"
#include "guiiconlist.h"
#include "guistatusbar.h"
#include "guirichtext.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#else
#include "mmi_appmsg.h"
#endif
#include "ctrltext_export.h"
#include "guitext.h"
#include "mmisrv_cameraroll_export.h"
#ifdef MMI_PIC_CROP_SUPPORT
#include "mmipic_crop_export.h"
#include "mmipb_export.h"
#endif
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
#include "mmi_theme.h"
//#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_TITLE_COLOR      0xFEA3
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX 12      
#define MMI_PICVIEW_MINI_SUPPORT TRUE
LOCAL BOOLEAN                   s_is_reload_file = FALSE;   //是否重新加载文件
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL BOOLEAN               s_is_need_to_reinit_picarray_from_piclib   = FALSE;  //是否重新初始化pic array
#else
LOCAL BOOLEAN               s_is_need_to_reinit_picarray_from_piclib   = TRUE;  //128X160无pic_folder窗口，需要重新初始化pic array
#endif
LOCAL uint32                s_cur_folder_index_for_reinit_picarray = 0;      //当s_is_need_to_reinit_picarray_from_piclib == TRUE时有效

BOOLEAN               s_is_pic_edit_complete  = FALSE;//lijinghua add for pic edit complete
LOCAL MMIPICVIEW_FROM_TYPE_E s_enter_from_type = MMIPICVIEW_FROM_NONE;


#ifdef PIC_PREVIEW_U200_SUPPORT
LOCAL  IMG_RES_SRC_T        res_src     = {0};
LOCAL  IMG_RES_SRC_T        targ_src    = {0};
LOCAL  IMGREF_DATA_FORMAT_E target_format   = IMGREF_FORMAT_RGB565;
#endif

LOCAL BOOLEAN  s_is_clear_pic_folder_List  = FALSE;  //是否已经释放anim_ctrl_add
LOCAL BOOLEAN  s_is_go_to_camera  = FALSE;  //是否已经释放anim_ctrl_add
MMI_WIN_ID_T g_picview_from_win = 0;

LOCAL BOOLEAN                   s_is_list_empty = FALSE;   //list界面是否为空,yaye.jiang
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
LOCAL const uint8   s_mmipicview_all_suffix[MMIPICVIEW_PIC_TYPE_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
#ifdef BMP_DEC_SUPPORT
    "*.bmp ",
    "*.wbmp ",
#endif
#ifdef GIF_DEC_SUPPORT
    "*.gif ",
#endif
#ifdef PNG_DEC_SUPPORT
    "*.png ",
#endif
#ifdef DRM_SUPPORT
    "*.dm ",
    "*.dcf ",
#endif
    "*.jpg ",
    "*.jpeg"
};
LOCAL const uint8   s_mmipic_except_gif_suffix[MMIPICVIEW_PIC_WITHOUT_GIF_TYPE_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
#ifdef BMP_DEC_SUPPORT
    "*.bmp ",
    "*.wbmp ",
#endif
#ifdef PNG_DEC_SUPPORT
    "*.png ",
#endif
#ifdef DRM_SUPPORT
    "*.dm ",
    "*.dcf ",
#endif
    "*.jpg ",
    "*.jpeg"
};
//zhuyongjing add for gallary ui 20150707
LOCAL uint32            all_pic_video_num              = 0;
//LOCAL uint16 s_thumbnail_index = 0;
LOCAL const uint8   s_mmivideo_all_suffix[MMIPICVIEW_VIDEO_TYPE_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
    "*.3gp ",
    "*.mp4 ",
    "*.flv ",
    "*.rmvb ",
    "*.avi"
};
LOCAL const uint8   s_mmipic_video_all_suffix[MMIPICVIEW_PIC_VIDEO_TYPE_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
#ifdef BMP_DEC_SUPPORT
    "*.bmp ",
    "*.wbmp ",
#endif
#ifdef GIF_DEC_SUPPORT
    "*.gif ",
#endif
#ifdef PNG_DEC_SUPPORT
    "*.png ",
#endif
#ifdef DRM_SUPPORT
    "*.dm ",
    "*.dcf ",
#endif
    "*.jpg ",
    "*.jpeg ",
#ifndef MMI_GALLERY_WITHOUT_VIDEO    
    "*.3gp ",
    "*.mp4 ",
    "*.flv ",
    "*.rmvb ",
    "*.avi"
#endif    
};
LOCAL const uint8   s_mmipic_edit_pic_all_suffix[MMIPICVIEW_PIC_EDIT_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
    "*.jpg ",
    "*.jpeg "
};

LOCAL uint16 s_mmipicview_last_total = 0;
#ifdef CAMERAROLL_SUPPORT
LOCAL uint16 s_mmipicview_save_items_marked_num = 0;
LOCAL BOOLEAN   s_mmipicview_multi_uploading_done = FALSE; //during muti uploading
LOCAL BOOLEAN   s_mmipicview_multi_uploading_highlight = FALSE; //during muti uploading
LOCAL BOOLEAN is_need_download_thumbnails = FALSE;
#endif
static uint8        s_wait_ready_timer_id = 0;
LOCAL uint8 s_thumbnail_down_total_count=0;
LOCAL BOOLEAN s_is_reload_file_aftersearch_getfous = FALSE; 
static MMIFILE_DEVICE_E s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;
static PIC_SOURCE_TYPE s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
static uint16 index = 0;//this for download thumbnails count
static  BOOLEAN             is_marked_all = FALSE;
LOCAL BOOLEAN   s_mmipicview_is_searching = FALSE; 
LOCAL BOOLEAN   s_is_shutdown = FALSE; 
LOCAL BOOLEAN s_is_close = FALSE;//for async task msg arrived after receive MSG_CLOSE_WINDOW
//add end
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
//extern MMIPICVIEW_DEFAULT_PATH_T g_mmipicview_dc_path;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void EntryPicviewListClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);//NEWMS00145771

/*****************************************************************************/
//  Description : handle pictures privacy password
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PICVIEW_ValidatePrivacyPicList(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : handle picture list win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle picture list open win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenPicListWin(
    MMI_WIN_ID_T      win_id, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
);

/*****************************************************************************/
//  Description : create pictures list control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
/*LOCAL void CreatePicListCtrl(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
);*/

/*****************************************************************************/
//  Description : load all picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*****************************************************************************/
//  Description : start load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPic(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : get all picture files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/

//zhuyongjing add for gallary ui 20150707

LOCAL MMI_RESULT_E HandlePicEmptylWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );
LOCAL MMI_RESULT_E HandlePicSaveItemsToOnedriveQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );
LOCAL void CreatUploadingLabelCtrl( MMI_WIN_ID_T   win_id);
LOCAL void CreatUploadingPrgboxCtrl( MMI_WIN_ID_T   win_id);
LOCAL void UpdateUploadingLabelCtrl( MMI_WIN_ID_T   win_id);
LOCAL void UpdateUploadingPrgboxCtrl( MMI_WIN_ID_T   win_id);
LOCAL void HandleUploadSelectPics(MMI_WIN_ID_T   win_id,MMI_CTRL_ID_T  list_ctrl_id,BOOLEAN succeed);
LOCAL BOOLEAN UploadSelectPic(MMI_WIN_ID_T   win_id,MMI_CTRL_ID_T  list_ctrl_id);
PUBLIC void ModifyPicListViewForUploadItems(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id, //in:
    BOOLEAN value
);
LOCAL void HightlightUploadPrgShow(MMI_WIN_ID_T   win_id, BOOLEAN ishighlight);
LOCAL BOOLEAN AppendOnlyPicIconListIcon(uint16          index,MMI_CTRL_ID_T   ctrl_id,BOOLEAN needgif);
LOCAL BOOLEAN AppendPicIconListIconForPB(uint16 index,MMI_CTRL_ID_T ctrl_id);
LOCAL BOOLEAN AppendPicIconListIconForMMS(uint16 index,MMI_CTRL_ID_T ctrl_id);
LOCAL void GetAllJpgPicSuffix(MMIFMM_FILTER_T  *filter_ptr);
PUBLIC uint32 MMIPICVIEW_GetPhotoIndexAccordingName(const wchar *fullpathname);


LOCAL MMI_RESULT_E HandleLoadPicSpinnerWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
PUBLIC BOOLEAN MMIPICVIEW_GetListEmpty(void);
LOCAL MMI_RESULT_E StartLoadPicForSpinner(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
LOCAL MMI_RESULT_E HandleMultiPicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

LOCAL void FreshSomeItemsForLoadPic(
    MMI_WIN_ID_T   win_id,
    MMI_CTRL_ID_T ctrl_id
);

//add end
/*****************************************************************************/
//  Description : finish load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void FinishLoadPic(
    MMI_WIN_ID_T   win_id,
    BOOLEAN flash_sync
);

/*****************************************************************************/
//  Description : clear pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearPicList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);

/*****************************************************************************/
//  Description : update pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
);

/*****************************************************************************/
//  Description : append picture list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicListItem(
    BOOLEAN         is_text,        //in:是否添加text data
    uint16          item_index,     //in:item索引
    uint16          content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T   ctrl_id         //in:control id
);

/*****************************************************************************/
//  Description : modify pictures list view
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPicListView(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
);

/*****************************************************************************/
//  Description : modify list item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ModifyListItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
);
/*****************************************************************************/
//  Description : append picture icon list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL BOOLEAN AppendPicIconListItem(
//     uint16          index,
//     MMI_CTRL_ID_T   ctrl_id
// );

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
//  Description : handle pictures list web key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListWeb(
    MMI_CTRL_ID_T   ctrl_id
);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
// 	Description : handle pic folder win msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
#endif
/*****************************************************************************/
// 	Description : handle pic folder win open msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenPicFolderWin(MMI_WIN_ID_T        win_id);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : camera button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CamerButtonCallBack(void);
#endif
/*****************************************************************************/
//  Description : clear pictures folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);
/*****************************************************************************/
//  Description : load all picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicFolderWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
/*****************************************************************************/
//  Description : start load picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPicWithFolders(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : finish load picture Folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPicWithFolders(
    MMI_WIN_ID_T   win_id
);
/*****************************************************************************/
//  Description : update picture folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicFolderList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
);

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIPICVIEW_GetClientWinRect(MMI_WIN_ID_T  win_id);
#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//  Description : set title for pic list 
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPicListTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T return_id);
#endif
/*****************************************************************************/
// 	Description : set iconlist param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_folder_list);

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set spftkey param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetButtonFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_IMAGE_ID_T* img_id, uint16 button_num);

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void InitMarkMenuButtons(MMI_WIN_ID_T win_id);

#endif//PIC_PREVIEW_U200_SUPPORT

#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// 	Description : set the iconlist groups orded in time
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetlistOrderInTime(MMI_CTRL_ID_T ctrl_id);
#endif
/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Discription: display when list win is empty
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
LOCAL void PicviewListEmptyShow(void);
#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
// 	Description : display the total pic number
//	Global resource dependence :
//  Author: langhua.yang
//	Note: just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DispTotalPicNum(void);
#endif

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicListWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void LoadPicList(MMI_CTRL_ID_T ctrl_id);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Discription: get default picture path
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void GetDefaultPicPath(MMIPICVIEW_DEFAULT_PATH_T* path_ptr);
#endif

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifdef  PIC_PREVIEW_U200_SUPPORT
//picture viewer list
WINDOW_TABLE(MMIPICVIEW_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicListWinMsg),
    WIN_ID(MMIPICVIEW_LIST_WIN_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_LIST_TITLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_RETURN, MMIPICVIEW_LIST_RETURN_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_LIST_SLIDE_PREVIEW),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_LIST_MARKED),
    END_WIN
};
// the window for  pic folder list window
WINDOW_TABLE(MMIPREVIEW_PICFOLDER_WIN_TAB) =
{
    WIN_ID(MMIPICVIEW_PICFOLDER_WIN_ID),
    WIN_FUNC((uint32)HandlePicFolderWinMsg),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_FLODER_TITLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_RETURN, MMIPICVIEW_FLODER_RETURN_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_CAMERA_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_SET_BUTTON_CTRL_ID),
    CREATE_ICONLIST_CTRL(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID),
    END_WIN
};
#else
//picture viewer list
WINDOW_TABLE(MMIPICVIEW_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicListWinMsg),
    WIN_ID(MMIPICVIEW_LIST_WIN_ID),
#ifndef MMI_GUI_STYLE_MINISCREEN
//zhuyongjing change for gallary ui 20150707 
   WIN_STATUSBAR,
    WIN_TITLE(TXT_COMMON_PIC),
//change end
#else
    WIN_TITLE(TXT_PICVIEW_TITLE),
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
//    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_LIST_TITLE_CTRL_ID),
#endif    
#ifdef MMI_GUI_STYLE_MINISCREEN
#if !defined (MAINLCD_SIZE_128X160)
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID),
#endif
#endif

    WIN_SOFTKEY(STXT_OPTION/*STXT_OPTION*/, STXT_SOFTKEY_OPEN_MK, TXT_COMMON_COMMON_BACK/*STXT_RETURN*/),//zhuyongjing change
    END_WIN
};
WINDOW_TABLE(MMIPICVIEW_WAIT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleLoadPicSpinnerWinMsg),
    WIN_ID(MMIPICVIEW_WAIT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_SPINNER_CTRL(MMIPICVIEW_WAITING_SPINNER_CTRL_ID),
    END_WIN
};
//add end

#ifndef MMI_PICVIEW_MINI_SUPPORT
WINDOW_TABLE(MMIPREVIEW_PICFOLDER_WIN_TAB) =
{
    WIN_ID(MMIPICVIEW_PICFOLDER_WIN_ID),
    WIN_FUNC((uint32)HandlePicFolderWinMsg),
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_COMMON_OK, TXT_COMMON_OK),
#else
    WIN_TITLE(TXT_PICVIEW_TITLE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_FLODER_TITLE_CTRL_ID),
#endif
    CREATE_ICONLIST_CTRL(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID),
    END_WIN
};
#endif
#endif //PIC_PREVIEW_U200_SUPPORT


LOCAL void EntryPicviewListClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)//NEWMS00145771
{
	MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;
    //get view type
	view_type = MMIPICVIEW_GetViewType();
	s_is_clear_pic_folder_List=TRUE;
	
    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_428_112_2_18_2_46_21_7,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        //GUILIST_SetCurItemIndex(ctrl_id, cur_folder_index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
       // GUIICONLIST_SetCurIconIndex(cur_folder_index,  ctrl_id);
        break;
    }

}

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
// 	Description : handle pic folder win msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E        result      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     = MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID;
    GUIANIM_DATA_INFO_T data_info   = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        OpenPicFolderWin(win_id);
        break;
        
    case MSG_FULL_PAINT:
        DisplayBackgroudImg(win_id);
        break;

    case MSG_PICVIEWER_RELOAD_FILE:
        {
            uint32                      time_out = 0;
            MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;

            MMIPICVIEW_DestroyPicLibArray();
            MMIPICVIEW_CreatePicLibArray();

            //clear pic list
            ClearPicFolderList(win_id, ctrl_id);

            MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleLoadPicFolderWinMsg);
        }
        break;
    case MSG_GET_FOCUS:
        //reload file
// #ifndef MMI_PICREVIEW_LIST_LOW_MEMORY////NEWMS00145771
//         if (s_is_reload_file)
//         {
//             MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
//             s_is_reload_file = FALSE;
//         }
// #else
        if (s_is_reload_file)
        {
            MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
            s_is_reload_file = FALSE;
        }
        else
        {
            if(s_is_clear_pic_folder_List)
            {
                uint16 pic_num = MMIPICVIEW_GetAllPicNum();
                MMK_SendMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &pic_num);
            }
        }
// #endif
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        break;
        
    case MSG_PICVIEWER_UPDATE_LIST:
        UpdatePicFolderList(win_id, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, *(uint16*)param);
        break;
    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            uint16 icon_num                     = 0;
            FILEARRAY   picfilelib_array        = MMIPICVIEW_GetPicLibArray();
            GUIICONLIST_DISP_T display_ptr      = {0};
            FILEARRAY_DATA_T   file_info        = {0};

            wchar  file_name[MMIFMM_FILENAME_LEN + 1]   = {0};
            uint16 file_name_len = 0;

            char      str_file_count[30 + 1]            = {0};
            uint16    str_file_count_len                = 0;

            //append icon item text
            uint16 folder_index = *((uint16 *)param);

#ifdef FONT_TYPE_SUPPORT_NONE   //bug117110
			GUI_FONT_ALL_T text_info = {0};
			text_info.font      = MMI_DEFAULT_TEXT_FONT/*SONG_FONT_20*/;
			text_info.color     = MMI_DEFAULT_TEXT_COLOR;
#endif
            display_ptr.softkey_id[0] = STXT_OPTION;
            display_ptr.softkey_id[2] = STXT_RETURN;
#ifndef PIC_PREVIEW_U200_SUPPORT
            if (folder_index == 0)
            {
                //MMI_GetLabelTextByLang(TXT_COMMON_CAPTURE, &display_ptr.name_str);
                wchar str_tmp[] = L"Camera";
                display_ptr.name_str.wstr_ptr = str_tmp;
                display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);
                MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
                GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
				GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif
            }
            else
            {
                if(folder_index == 1)
                {
#if defined(CMCC_UI_STYLE)
					if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
					{
						MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index - 1, &file_info);
						MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
					}
					else
#endif
					{
 //                   MMI_GetLabelTextByLang(TXT_DC_MY_PICTURE, &display_ptr.name_str);
                    wchar str_tmp[] = L"Camera";
                    display_ptr.name_str.wstr_ptr = str_tmp;
                    display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);

                    MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                    file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
					}
                }
                else
                {
                    MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index - 1, &file_info);
                    MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
                }

                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index - 1);
                }
#else
            if (folder_index == 0)
            {
//                MMI_GetLabelTextByLang(TXT_DC_MY_PICTURE, &display_ptr.name_str);
                wchar str_tmp[] = L"Camera";
                display_ptr.name_str.wstr_ptr = str_tmp;
                display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);
                
                MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
            }
            else
            {
                MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index, &file_info);
                MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
            }
            
            if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index))
            {
                icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index);
            }
#endif
//             sprintf(str_file_count, " (%d)", icon_num);
//             str_file_count_len = strlen(str_file_count);
//             if ((file_name_len + str_file_count_len) <= MMIFMM_FILENAME_LEN)
//             {
//                 MMI_STRNTOWSTR(file_name + file_name_len, MMIFMM_FILENAME_LEN - file_name_len, (const uint8*)str_file_count, str_file_count_len, str_file_count_len);
//                 file_name_len += str_file_count_len;
//             }
// 
//             display_ptr.name_str.wstr_ptr = file_name;
//             display_ptr.name_str.wstr_len = file_name_len;
//             GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
            if((0 == file_name_len) && (0 != file_info.name_len))
            {
               // file_name_len = file_info.name_len;
                file_name_len = MIN(file_info.name_len,MMIFMM_FILENAME_LEN);
                SCI_MEMCPY(file_name, file_info.filename, file_name_len * sizeof(wchar));
            }

            sprintf(str_file_count, " (%d)", icon_num);
            str_file_count_len = strlen(str_file_count);
            if (file_name_len + str_file_count_len > MMIFMM_FILENAME_LEN)
            {
				file_name_len = MMIFMM_FILENAME_LEN - str_file_count_len;
				SCI_MEMSET(file_name + file_name_len , 0, (MMIFMM_FILENAME_LEN - file_name_len)*sizeof(wchar));
            }
            display_ptr.name_str.wstr_ptr = file_name;
            display_ptr.name_str.wstr_len = file_name_len;
            GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
			GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif
			
			SCI_MEMSET(file_name, 0, MMIFMM_FILENAME_LEN * sizeof(wchar));
            MMI_STRNTOWSTR(file_name, MMIFMM_FILENAME_LEN, (const uint8*)str_file_count, str_file_count_len, str_file_count_len);
            display_ptr.name_str.wstr_ptr = file_name;
            display_ptr.name_str.wstr_len = str_file_count_len;
            GUIICONLIST_AppendTextAddtion(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
			GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif

#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }
        break;

    case MSG_CTL_ICONLIST_ICON_NUM:
        {
            uint16 icon_num = 0;
            FILEARRAY   picfilelib_array    = MMIPICVIEW_GetPicLibArray();

            uint16 folder_index = *((uint16 *)param);
#ifndef PIC_PREVIEW_U200_SUPPORT
            if (0 == folder_index)
            {
                GUIICONLIST_SetIconNum(folder_index, 1, ctrl_id);
            }
            else
            {
                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index - 1);
                }

#else
                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index);
                }
#endif
                else
                {
                    //if (0 == folder_index)
                    {
                        icon_num = 1;
                    }
                }
                GUIICONLIST_SetIconNum(folder_index, icon_num, ctrl_id);
#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*)param)->src_id)
                {
#ifdef PIC_PREVIEW_U200_SUPPORT
                case MMIPICVIEW_FLODER_RETURN_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                    break;

                case MMIPICVIEW_SET_BUTTON_CTRL_ID:
                    MMIAPIPICVIEW_OpenSetWin();
                    break;
#endif
                case MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                    break;
                    
                default:                
                    result = MMI_RESULT_FALSE;
                    break;
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_ICONFOLDER_APPEND_ICON:
        {
            //append icon
            FILEARRAY_DATA_T            file_info = {0};
            GUIANIM_FILE_INFO_T         icon_file = {0};
            GUIICONFOLDER_NOTIFY_T  *notify_ptr = (GUIICONFOLDER_NOTIFY_T *)param;

            FILEARRAY   picfilelib_array        = MMIPICVIEW_GetPicLibArray();
            
#ifndef PIC_PREVIEW_U200_SUPPORT
            if(0 == notify_ptr->folder_index)
            {
                data_info.img_id = IMAGE_PICVIEW_CAMERA_FOLDER;
                GUIICONFOLDER_AppendIcon(0, notify_ptr->notify.src_handle, &data_info, PNULL);
            }
            else
            {
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index - 1))

#else
            //if ((0 == notify_ptr->folder_index)&&(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index)))
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index))
#endif
            {
                //除了第一个文件夹，其它所有文件夹都不应该出现空文件夹的情况
                data_info.img_id = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
                GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle,&data_info, PNULL);
            }
            else
            {
#ifndef PIC_PREVIEW_U200_SUPPORT
                MMIAPIFILEARRAY_Read(picfilelib_array, notify_ptr->icon_index + MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, notify_ptr->folder_index - 1), &file_info);
#else
                MMIAPIFILEARRAY_Read(picfilelib_array, notify_ptr->icon_index + MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, notify_ptr->folder_index), &file_info);
#endif
#ifdef DRM_SUPPORT  
                if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
                {
                    //is it has right
                    if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
                    {
                        data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
                    }
                    else
                    {
                        data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
                    }
                    //append icon
                    GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle,&data_info, PNULL);
                }
                else
#endif
                {
                    //set file name
                    icon_file.full_path_wstr_ptr = file_info.filename;
                    icon_file.full_path_wstr_len = file_info.name_len;
                    //append icon
                    GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle, PNULL, &icon_file);
                }
            }
#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }

        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            uint32 folder_index = 0;
            folder_index = GUIICONLIST_GetCurIconIndex(ctrl_id);

#ifndef PIC_PREVIEW_U200_SUPPORT
            if(0 == folder_index)
            {
                CamerButtonCallBack();
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index - 1);
            }
            else if(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index - 1))
#else
            //if ((0 == folder_index)&&(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index)))
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index))
#endif
            {
                result = MMI_RESULT_FALSE;
            }
            else
            {
                EntryPicviewListClearPicFolderList(win_id,ctrl_id);
#ifndef PIC_PREVIEW_U200_SUPPORT
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index - 1);
#else
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index);
#endif
                MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB, PNULL);
            }
        }
        break;
#ifndef PIC_PREVIEW_U200_SUPPORT
    case MSG_APP_MENU:
    case MSG_CTL_OK:
        MMK_CreateWin((uint32 *)MMIPICVIEW_FOLDER_LIST_OPT_WIN_TAB, PNULL);
        break;
#endif

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
    //destroy lib array
    MMIPICVIEW_DestroyPicLibArray();
    break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : handle pic folder win open msg
//	Global resource dependence :
//   Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenPicFolderWin(MMI_WIN_ID_T        win_id)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     = MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID;
#ifdef PIC_PREVIEW_U200_SUPPORT
    MMI_CTRL_ID_T       button_id[] ={MMIPICVIEW_CAMERA_BUTTON_CTRL_ID, MMIPICVIEW_SET_BUTTON_CTRL_ID};
    MMI_IMAGE_ID_T      img_id[]    = {IMAGE_PICVIEW_CAMERA, IMAGE_PICVIEW_SET};
#endif
//    GUI_BG_T            bg_info     = {0};
    GUI_BORDER_T        boder_type  = {0};

    DisplayBackgroudImg(win_id);

//     bg_info.bg_type = GUI_BG_IMG;
// 	bg_info.img_id	= PICVIEW_DEFAULT_BG_IMG;
//     GUISTATUSBAR_SetBg(win_id, bg_info);
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#endif
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
	GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
    SetPicListTitle(win_id, MMIPICVIEW_FLODER_TITLE_CTRL_ID, MMIPICVIEW_FLODER_RETURN_CTRL_ID);
    SetButtonFormParam(win_id, button_id, img_id, 2);
    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_CAMERA_BUTTON_CTRL_ID, CamerButtonCallBack);
    boder_type.type     = GUI_BORDER_NONE;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
    SetPicListTitle(win_id, MMIPICVIEW_FLODER_TITLE_CTRL_ID, PNULL);
#else
    /*MINI UI 需求:不显示list_title*/
    CTRLICONLIST_SetIsDispTitle(ctrl_id, FALSE);
#endif
    //GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);

//    GUIWIN_SetSoftkeyCommonBg(win_id, &bg_info);
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
    boder_type.type     = GUI_BORDER_SOLID;
    boder_type.width    = 0;
    boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);    
    boder_type.width    = 2;
    boder_type.color    = MMIPICVIEW_TITLE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#endif

    SetIconListParam(win_id, ctrl_id, TRUE);
    GUIICONLIST_SetDefaultIconByPtr(ctrl_id, IMAGE_PICVIEW_FOLDER_DEFAULT_ICON, IMAGE_PICVIEW_PIC_ERROR_ICON);
    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_FOLDER_UIDT);
    
    //同步打开等待窗口, 避免闪烁
    MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
    return result;
}

/*****************************************************************************/
//  Description : camera button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CamerButtonCallBack(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
	MMIVIRTUALARRAY_SetBufferSize(MMIPICVIEW_GetPicLibArray(), 2);
#ifndef MMI_GUI_STYLE_MINISCREEN
	EntryPicviewListClearPicFolderList(MMIPICVIEW_PICFOLDER_WIN_ID, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID);
#endif
#if 0//def CAMERA_SUPPORT//delete by James li
    MMIAPIDC_OpenPhotoWin();
#endif
    return result;
}

/*****************************************************************************/
//  Description : clear pictures folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)
{
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicFolderList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_853_112_2_18_2_46_22_8,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
        GUIICONLIST_SetCurIconIndex(0,  ctrl_id);
        break;
    }

    //set title
    //GUIWIN_SetTitleTextId(win_id,TXT_PICVIEW,FALSE);

    //set softkey
    //GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_NULL,STXT_RETURN,FALSE);
}
/*****************************************************************************/
//  Description : load all picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicFolderWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = StartLoadPicWithFolders(win_id, msg_id, param);
        break;

    case MSG_PICVIEWER_LOAD_FINISH:
        FinishLoadPicWithFolders(win_id);
        break;

    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        MMK_PostMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, (DPARAM)PNULL, (uint32)0);
        //MMK_PostMsg(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, MSG_CTL_CANCEL, (DPARAM)PNULL, (uint32)0);
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : start load picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPicWithFolders(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    BOOLEAN                     is_recurse = FALSE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter = {0};
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    MMIPICVIEW_DEFAULT_PATH_T   photos_path = {0};


    //get all picture file suffix
    GetAllPicSuffix(&file_filter);
    MMIPICVIEW_SetViewType(MMIPICVIEW_VIEW_ICONLIST);

#if defined(CMCC_UI_STYLE)
    if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
    {
        default_path.path_len = MMIFMM_PATHNAME_LEN;
        
#if defined(DL_SUPPORT)
        MMIAPIDL_GetDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, default_path.path, &default_path.path_len);
#else
        GetDefaultPicPath(&default_path);
#endif
    }
    else
#endif
    {
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);
        MMIAPIPICVIEW_GetMyPhotosPath(&photos_path);
		if(!MMIAPICOM_Wstrncmp(photos_path.path, default_path.path, photos_path.path_len))
		{
			default_path = photos_path;//如果default_path是photos_path的子目录，则只搜一次
		}
    }

    is_recurse = TRUE;

#if 1
    MMIAPIFMM_SearchFileInPathEx(default_path.path, default_path.path_len,
                              (const wchar *)photos_path.path,
                              &file_filter,
                              is_recurse,
                              FUNC_FIND_FOLDERS_WITH_SUBFILES,
                              FILEARRAY_SORT_TIME_CREATE_DESCEND,
                              MMIPICVIEW_GetPicLibArray(),
                              win_id,
                              MSG_PICVIEWER_LOAD_FINISH
                             );
#else
    //search all the picture file aysn
    MMIAPIFMM_SearchFileInPath(path_ptr,
                            path_len,
                            &file_filter,
                            is_recurse,
                            FUNC_FIND_FOLDERS_WITH_SUBFILES,
                            MMIPICVIEW_GetPicLibArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH);
#endif

    //set softkey
    //MMIPUB_SetWinSoftkey(win_id,TXT_NULL,STXT_CANCEL,FALSE);

    result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

    return (result);
}
/*****************************************************************************/
//  Description : finish load picture Folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPicWithFolders(
    MMI_WIN_ID_T   win_id
)
{
    uint16      cur_index = 0;
    uint32      pic_num = 0;

    pic_num = MMIPICVIEW_GetAllPicNum();
    if (0 < pic_num)
    {
        if (MMIAPIDC_IsOpened())
        {
            cur_index = (uint16)MMIAPIDC_GetSavePhotoIndex(pic_num, MMIPICVIEW_GetPicLibArray());
        }
    }

    //need update list

    MMK_PostMsg(MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, &cur_index, sizeof(uint16));

    MMK_CloseWin(win_id);
}
/*****************************************************************************/
//  Description : update picture folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicFolderList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
)
{
    uint16                  i = 0;
    GUILIST_ITEM_T          item = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;
    FILEARRAY   			picfilelib_array    = MMIPICVIEW_GetPicLibArray();
    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicFolderList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1034_112_2_18_2_46_23_9,(uint8*)"d", view_type);

	if(s_is_clear_pic_folder_List)
	{
		MMIVIRTUALARRAY_SetBufferSize(picfilelib_array, 50);
	}

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        {
            uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNumInLib();
            //set max item
            GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);

            //st title
            GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);

            if (0 < total_num)
            {
                //set title index
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

                //set item
                item.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
                for (i = 0; i < total_num; i++)
                {
                    GUILIST_AppendItem(ctrl_id, &item);
                }

                //set current index
                GUILIST_SetCurItemIndex(ctrl_id, cur_index);
            }
        }
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        {
            uint32          total_folder_num  = MMIAPIFILEARRAY_GetTotalFolderCount(picfilelib_array);
#if 0
            //set title
            if (0 == total_folder_num)
            {
                GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);
            }
            else
#endif                
            {
#ifdef PIC_PREVIEW_U200_SUPPORT
                GUIICONLIST_SetTotalIcon(ctrl_id, total_folder_num);
#else
                GUIICONLIST_SetTotalIcon(ctrl_id, total_folder_num + 1);
#endif

        		if(s_is_clear_pic_folder_List)
        		{
        	        s_is_clear_pic_folder_List=FALSE;
//#ifdef PIC_PREVIEW_U200_SUPPORT
                    GUIICONLIST_SetCurIconIndex(MMIPICVIEW_GetCurFolderIndex(), ctrl_id);
//#else
//                    GUIICONLIST_SetCurIconIndex(MMIPICVIEW_GetCurFolderIndex() + 1, ctrl_id);
//#endif
        		}
        		else
        		{
                    GUIICONLIST_SetCurIconIndex(0, ctrl_id);
                }

            }
        }

        break;
    }

}
#endif
/*****************************************************************************/
// 	Description : show pic list in my photos path and video
//	Global resource dependence : none
//    Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterMyPhotos(void)
{
    s_is_need_to_reinit_picarray_from_piclib = TRUE;
    MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);

    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
    }
    else
    {
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB, PNULL);
    }

}
/*****************************************************************************/
// 	Description : show pic preview for camera --captured
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_PreviewCapturedItem(void)
{
    MMIPICVIEW_SetEnterType(MMIPICVIEW_PHOTOS_FROM_CAMERA);
    MMIAPIDC_Exit();
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);     
    }else{
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
}
#ifdef MMI_PIC_CROP_SUPPORT
/*****************************************************************************/
// 	Description : show pic preview for PB--PB->gallery-->camera-->gallery crop
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_PreviewCropAfterCapture(wchar* filename)
{
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        MMIPICCROP_SRC_T crop_src = {0};
        crop_src.crop_type = MMIPIC_CROP_PB;
        crop_src.callback = PicCropCallBack;
        crop_src.full_path_ptr = filename;
        crop_src.full_path_len =  MMIAPICOM_Wstrlen(filename);
        crop_src.min_width = 30;//(s_pb_photo_anim_rect.right-s_pb_photo_anim_rect.left);
        crop_src.min_height = 30;//(s_pb_photo_anim_rect.bottom-s_pb_photo_anim_rect.top);
        MMIAPIPICCROP_OpenPicCrop(&crop_src); 
    }else{
        MMIPICVIEW_SetEnterType(MMIPICVIEW_PHOTOS_FROM_PB);
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
}
#endif
// 	Description : show pic list in my photos path
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterOnlyPhotos(void)
{
    MMIPICVIEW_SetEnterType(MMIPICVIEW_PHOTOS_FROM_WALLPAPER);

    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
       // MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
       MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
       MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
    else
    {
        SCI_TRACE_LOW("[MMIPIC]:MMIAPIPICVIEW_EnterOnlyPhotos");
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB, PNULL);
    }
}
/*****************************************************************************/
// 	Description : show pic list in my photos path for outlook and sms
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterFromPB(void)
{
   MMIPICVIEW_SetEnterType(MMIPICVIEW_PHOTOS_FROM_PB); 

    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
       // MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
       MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
       MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
}
/*****************************************************************************/
// 	Description : show pic list in my photos path for outlook and sms
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterOnlyPhotosForMMS(MMI_WIN_ID_T prev_win)
{
    MMIPICVIEW_SetEnterType(MMIPICVIEW_PHOTOS_FROM_MMS); 
    g_picview_from_win = prev_win;
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
       // MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
        MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
        g_picview_from_win = prev_win;
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
}
/*****************************************************************************/
// 	Description : show pic list from pb //zhuyongjing add s_is_need_to_edit_from_pb for pb 20150826
//	Global resource dependence : none
//    Author: zhuyongjing
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterOnlyVideosForMMS(MMI_WIN_ID_T prev_win )
{
    MMIPICVIEW_SetEnterType(MMIPICVIEW_VIDEOS_FROM_MMS); 
    g_picview_from_win = prev_win;
    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        //MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
        MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
        g_picview_from_win = prev_win;
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);
    }

}
PUBLIC void MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_TYPE_E type)
{
    s_enter_from_type = type;
}

PUBLIC MMIPICVIEW_FROM_TYPE_E MMIPICVIEW_GetEnterType(void)
{
    return s_enter_from_type;
}
#ifdef CAMERAROLL_SUPPORT
PUBLIC void MMIPICVIEW_SetMultiUploadingDone(BOOLEAN val)
{
    s_mmipicview_multi_uploading_done = val;
}
PUBLIC BOOLEAN MMIPICVIEW_GetMultiUploadingDone(void)
{
    return s_mmipicview_multi_uploading_done;
}
PUBLIC void MMIPICVIEW_AddOneMultiUploadingNum(void){
    s_mmipicview_save_items_marked_num+=1;
}
PUBLIC void MMIPICVIEW_DelOneMultiUploadingNum(void){
    if(s_mmipicview_save_items_marked_num >1){
        s_mmipicview_save_items_marked_num-=1;
    }
}
#endif
PUBLIC BOOLEAN MMIPICVIEW_IsClose(void){
    return s_is_close;
}
LOCAL void MMIPICVIEW_SetIconMaskThemeImg(MMI_CTRL_ID_T ctrl_id){
    MMI_IMAGE_ID_T image_id = IMAGE_AVATAR_CHECKBOX_UNSELECTED_ICON;
    GUIICONLIST_SetIconMaskThemeImg(ctrl_id,image_id);
}

LOCAL void MMIPICVIEW_SetMarkThemeImg(MMI_CTRL_ID_T ctrl_id){
    MMI_IMAGE_ID_T image_id = IMAGE_AVATAR_CHECKBOX_SELECTED_ICON;
    GUIICONLIST_SetMarkThemeImg(ctrl_id,image_id);
}

    /*****************************************************************************/
//  Description : create directory with RO or HIDDEN attr
//  Global resource dependence :
//  Author:zhuyongjing
//  Note:
    /*****************************************************************************/
    PUBLIC BOOLEAN MMIAPIPICVIEW_SetAttr (
        const wchar       *full_path_ptr,     //in
        uint16            full_path_len,       //in, 双字节为单位
        BOOLEAN           is_hidden,         //in, 是否为隐藏属性
        BOOLEAN           is_ro,             //in, 是否为只读属性
        BOOLEAN           is_system,         //in, 是否为系统属性
        BOOLEAN           is_arch           //in, 是否为存档属性
    )
    {
        wchar           *file_name_ptr = PNULL;
        wchar           *path_name_ptr = PNULL;
        uint16          file_name_len = full_path_len;
        wchar           attr[MMIFILE_FILE_ATTR_LEN] = {0};

        if (NULL == full_path_ptr || 0 == full_path_len)
        {
            return FALSE;
        }


        file_name_ptr = (wchar *) SCI_ALLOCA ( (file_name_len + 1) * sizeof (wchar));
        path_name_ptr = (wchar *) SCI_ALLOCA ( (full_path_len + 1) * sizeof (wchar));

        if (file_name_ptr == PNULL || path_name_ptr == PNULL)
        {
            //SCI_TRACE_LOW:"[MMIFILE] SetAttr NO memory !"
            SCI_FREE (file_name_ptr);
            SCI_FREE (path_name_ptr);
            file_name_ptr = PNULL;
            path_name_ptr = PNULL;
            return FALSE;
        }

        SCI_MEMSET (file_name_ptr, 0x00, ( (file_name_len + 1) *sizeof (wchar)));
        SCI_MEMSET (path_name_ptr, 0x00, ( (full_path_len + 1) *sizeof (wchar)));
        MMIAPICOM_Wstrncpy (path_name_ptr, full_path_ptr, full_path_len);
        MMIAPIFMM_SplitLastName (path_name_ptr, &full_path_len, file_name_ptr, &file_name_len);
        MMIAPICOM_Wstrcpy (attr, L"/SA:");

        if (is_hidden)
        {
            MMIAPICOM_Wstrcat (attr, L"+H");
        }
        else
        {
            MMIAPICOM_Wstrcat (attr, L"-H");
        }

        if (is_ro)
        {
            MMIAPICOM_Wstrcat (attr, L"+R");
        }

        if (is_system)
        {
            MMIAPICOM_Wstrcat (attr, L"+S");
        }

        if (is_arch)
        {
            MMIAPICOM_Wstrcat (attr, L"+A");
        }

        SFS_SetAttrAsyn (path_name_ptr, file_name_ptr, attr, 0);
        SCI_FREE (file_name_ptr);
        SCI_FREE (path_name_ptr);
        file_name_ptr = PNULL;
        path_name_ptr = PNULL;

        return TRUE;
    }
PUBLIC void MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(BOOLEAN val)
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
            MMIAPIPICVIEW_SetAttr(full_path_name, full_path_len, val, FALSE, FALSE, FALSE);
        }
    }

   if(MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_UDISK))
   {
        SCI_MEMSET(full_path_name, 0, sizeof(uint16));
        if (MMIAPIFMM_CombineFullPath (MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK),
                                       MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS, MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS_LEN,
                                       NULL, 0,
                                       full_path_name, &full_path_len))
        {
            if (!MMIAPIFMM_IsFolderExist (full_path_name, full_path_len)){
                MMIAPIFMM_CreateDir (full_path_name, full_path_len);
            }
            MMIAPIPICVIEW_SetAttr(full_path_name, full_path_len, val, FALSE, FALSE, FALSE);
        }
    } 
#endif
}

#ifdef CAMERAROLL_SUPPORT
LOCAL void DeleteLocalThumbnailsByServer(MMI_CTRL_ID_T  ctrl_id){
    uint32              photo_index = 0;
    uint32              i = 0;
    uint32              pic_num =MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetCurPicArray());
    uint32              pic_del_num =MMIAPIFILEARRAY_GetArraySize(MMISRV_CAMERAROLL_Get_Delete_photo_FileAry());
    FILEARRAY_DATA_T    file_info = {0};
    FILEARRAY_DATA_T    file_del_info = {0};
    uint16 cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
        
    if(MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetCurPicArray())>VIRTUALARRAY_BUFFER_SIZE){
        GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_SERVER_DELETE_FILEARRAY;
        Server_del_thumbnails_Signal();
        MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);
    }else{
        FILEARRAY delete_array = MMISRV_CAMERAROLL_Get_Delete_photo_FileAry();
        if(MMIAPIPICVIEW_DeleteArrayWithSubName(MMIAPIPICVIEW_GetCurPicArray(),delete_array, TRUE,TRUE)){    
            SCI_TRACE_LOW("[mmipic] DeleteLocalThumbnailsByServer MMIAPIPICVIEW_DeleteArray ok!");
        }else{
            SCI_TRACE_LOW("[mmipic] DeleteLocalThumbnailsByServer MMIAPIPICVIEW_DeleteArray fail!");
        }
        MMIAPIFILEARRAY_Destroy(&delete_array);

        if(MMIPICVIEW_GetAllPicNum() > 0){
            if(cur_index>= MMIPICVIEW_GetAllPicNum()){
                cur_index =  MMIPICVIEW_GetAllPicNum()-1;
            }
            MMIPICVIEW_PostUpdateMsg(cur_index);
        }else{
            FinishLoadPic(MMIPICVIEW_LIST_WIN_ID,FALSE);
        }
    }
    return;
}
#endif
PUBLIC void PICVIEW_ReloadAfterSearch(void){
    PIC_SOURCE_TYPE path = MMIAPIPICVIEW_GetShowFolderPath();
    uint16 size = MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetSearchAllPicArray());
    SCI_TRACE_LOW("[mmipic] PICVIEW_ReloadAfterSearch path=%d,endtime=%d,size=%d",path,SCI_GetTickCount(),size);
    MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_PREVIOUS_FILEARRAY,MMIAPIPICVIEW_GetAllPicArray());      

    if(path == CAMERA_ROLL_SOURCE_NONE){            
        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetSearchAllPicArray());      

        if(MMIPICVIEW_GetListEmpty() 
            && MMIPICVIEW_GetAllPicNum()>0
            &&!MMK_IsActiveCtrl(MMIPICVIEW_LIST_CTRL_ID)){
            CreatePicListCtrl(MMIPICVIEW_LIST_WIN_ID, MMIPICVIEW_LIST_CTRL_ID);
        }

    }else{
        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_BACKUP_FILEARRAY,MMIAPIPICVIEW_GetSearchAllPicArray());
        MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetSearchAllPicArray(),path);
        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetSearchAllPicArray());
    }
     //   MMIAPIPICVIEW_DestroySearchAllPicArray();
}
/*****************************************************************************/
//  Description : handle pictures privacy password
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PICVIEW_ValidatePrivacyPicList(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_WIN_ID_T win_id = 0;

    if (PNULL != param_ptr)
    {
        win_id = *(MMI_WIN_ID_T*)param_ptr;
    }
    
    if ( is_permitted )
    {
        //同步打开等待窗口, 避免闪烁
        MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
    }
    else
    {
        MMK_CloseWin(win_id);
    }
}

/*****************************************************************************/
//  Description : handle picture list win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicListWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;
    GUILIST_NEED_ITEM_DATA_T    *item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    MMI_STRING_T                title_str = {0};
    MMI_WIN_ID_T                      query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    uint16                      pic_num =0;
    uint32                      cur_index = 0;
    uint16                      mark_num = 0;
    wchar                       mark_wstr[60] ={0};
    char                        mark_num_str[30] ={0}; 
    wchar                       mark_num_wstr[30]={0};
    //zhuyongjing add
    PIC_SOURCE_TYPE path = CAMERA_ROLL_SOURCE_NONE;
    // static  BOOLEAN             is_marked_all = FALSE;
    MMIPICVIEW_FROM_TYPE_E      enter_form = MMIPICVIEW_FROM_NONE;
    ONE_DRIVE_AUTO_UPLOAD_STATE_E auto_status = ONE_DRIVE_AUTO_UPLOAD_NONE;
    static BOOLEAN is_rec_keydown = FALSE;//just for idle shortcut
    
    //add end
#ifdef  PIC_PREVIEW_U200_SUPPORT
    // static  BOOLEAN             is_marked_all = FALSE;
    MMI_WIN_ID_T                query_win_id = MMIPICVIEW_QUERY_WIN_ID;//add by zhuyongjing for empty 20150722
#endif
    SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg msg_id =%x", msg_id);
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        index = 0;
        is_marked_all = FALSE;
        s_is_close = FALSE;
        s_is_shutdown = FALSE;
        MMIPICVIEW_SetListEmpty(FALSE);
        enter_form = MMIPICVIEW_GetEnterType();
#if defined(CAMERAROLL_SUPPORT)	
        auto_status = MMISRV_CAMERAROLL_Get_Auto_upload_status();
        SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_OPEN_WINDOW enter_form=%d,s_mmipicview_multi_uploading_done=%d,auto_status=%d",
            enter_form,s_mmipicview_multi_uploading_done,auto_status);
        
        
        if(s_mmipicview_multi_uploading_done){
            if(ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == MMISRV_CAMERAROLL_Get_Auto_upload_status()
                && MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){

            }else if(ONE_DRIVE_AUTO_UPLOAD_IDLE == MMISRV_CAMERAROLL_Get_Auto_upload_status()
                &&!MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
                s_mmipicview_multi_uploading_done = FALSE;
            }else if(ONE_DRIVE_AUTO_UPLOAD_SNAPSHOT_UPLOADING == MMISRV_CAMERAROLL_Get_Auto_upload_status()
                &&!MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
                s_mmipicview_multi_uploading_done = FALSE;
            }else if( MMIPICVIEW_GetEnterType() != MMIPICVIEW_FROM_NONE){
                s_mmipicview_multi_uploading_done = FALSE;
            }
        }
        
        if(ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK == MMISRV_CAMERAROLL_Get_Auto_upload_status()
            &&CAMERA_ROLL_MANUAL_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
            &&s_mmipicview_multi_uploading_done
            &&MMISRV_CAMERAROLL_Get_Connect_State()
            &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            &&MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()
            &&MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){
            s_mmipicview_multi_uploading_done = FALSE;
            s_mmipicview_multi_uploading_highlight = FALSE;
            DisplaybannerInfo(win_id,STR_ONEDRIVE_UPLOAD_FAILED_EXT01);
            MMISRV_CAMERAROLL_Clean_manual_upload_list();
            if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                ModifyPicListViewForUploadItems(win_id,ctrl_id,FALSE);
            }
        }else if(ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK == MMISRV_CAMERAROLL_Get_Auto_upload_status()
            &&CAMERA_ROLL_AUTO_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
            &&MMISRV_CAMERAROLL_Get_Connect_State()
            &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
            &&MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()
            &&MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){
            DisplaybannerInfo(win_id,STR_ONEDRIVE_ERROR_SYNC_EXT01);
        }
        if(s_mmipicview_multi_uploading_done
            &&!MMIPICVIEW_GetListEmpty()
            &&MMISRV_CAMERAROLL_Get_Connect_State()
            &&CAMERA_ROLL_MANUAL_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()){
            if(!MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                ModifyPicListViewForUploadItems(win_id,ctrl_id,TRUE);
            }
        }
#endif
        OpenPicListWin(win_id, ctrl_id);
        GalleryFileArray_CreateTask();
        SCI_TRACE_LOW("[pic_view init Fat Del flag...]%s            %d\n",__FUNCTION__,__LINE__);        
        MMIAPIPICVIEW_SetFatDelIng(FALSE);
        s_is_go_to_camera = FALSE;
 
        break;
        
    case MSG_FULL_PAINT:
        {
            //zhuyongjing add for status area header show
            MMI_TEXT_ID_T text_id = STR_GAL_HDR_GALLERY_EXT01;
            uint16 total_num = MMIPICVIEW_GetAllPicNum();
          //  SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_FULL_PAINT s_mmipicview_multi_uploading_done=%d",s_mmipicview_multi_uploading_done);
            DisplayBackgroudImg(win_id);
        if(FALSE == MMIPICVIEW_GetListMarkable(ctrl_id)){
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS){
                    text_id = STR_UC_ADD_PICTURE_ID;
            }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                    text_id = STR_MSG_LIST_ADD_VIDEO_EXT01;
            }else if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ALL_LOCAL){
                    text_id = STR_GAL_LIST_ALL_EXT01;
            }else if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_CAPTURED){
                    text_id = STR_GALLERY_CAPTURED_PHOTOS_MK;
            }else if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_DONWLOADED){
                    text_id = STR_GALLERY_DOWNLOADED_EXT01;
            }else if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_VIDEO){
                    text_id = STR_GAL_LIST_VIDEO_ALBUM;
            }else if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                    text_id = STR_GAL_LIST_ONEDRIVE_EXT01;
                }
                
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(text_id)){
                    GUIWIN_UpdateStb();
                }
            }
            
            
        if(!s_is_list_empty && MMK_IsActiveCtrl(MMIPICVIEW_LIST_EMPTY_CTRL_ID) ) {
                MMK_DestroyControl(MMIPICVIEW_LIST_EMPTY_CTRL_ID);
            }
            //zhuyongjing add for set total >0, but all dont exist. then all removed
            if(s_is_list_empty && MMK_IsActiveCtrl(MMIPICVIEW_LIST_CTRL_ID)
            &&!MMK_IsActiveCtrl(MMIPICVIEW_LIST_EMPTY_CTRL_ID)) {
                MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
                MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID);
                MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
                PicviewListAllEmptyShow(MMIPICVIEW_LIST_WIN_ID); 
                SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_FULL_PAINT--- PicviewListAllEmptyShow");
                break;
            }
            //add end
            if(s_is_list_empty)
            {
                //zhuyongjing change for gallary UI 20150707
                //original:PicviewListEmptyShow();
            if(all_pic_video_num == 0){
                    //PicviewListAllEmptyShow(win_id);
            }else{
                    // PicviewListEmptyShow();
                    // PicviewListAllEmptyShow(win_id);
                }
                // MMK_CreateWin((uint32 *)MMIPICVIEW_EMPTY_WIN_TAB, PNULL);
                //add end
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
                // GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
            }
            if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_CAMERA)
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
            }
#ifdef CAMERAROLL_SUPPORT
        if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){
            if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    CTRLPROGRESSBAR_Update(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
                    UpdateUploadingPrgboxCtrl(win_id);
                    UpdateUploadingLabelCtrl(win_id);
            }else if(s_mmipicview_multi_uploading_done
                    && MMIPICVIEW_GetCurIndex(ctrl_id) == 0
                    &&!MMIPICVIEW_GetListMarkable(ctrl_id)
                &&!MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
                    CTRLPROGRESSBAR_Update(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
            }else if(s_mmipicview_multi_uploading_done &&!MMIPICVIEW_GetListMarkable(ctrl_id)
            &&!MMK_IsOpenWin(MMIPICVIEW_LIST_OPT_WIN_ID)){
                    UpdateUploadingPrgboxCtrl(win_id);
                    UpdateUploadingLabelCtrl(win_id);
                }
            }
#endif
            /* add by langhua yang to display the total pic number*/
#ifdef MMI_GUI_STYLE_MINISCREEN
            DispTotalPicNum();
#endif
        }
        break;

    case MSG_PICVIEWER_RELOAD_FILE:
        //load file
#ifdef GUIF_ICONLIST_DELIMITER
        if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        }
#endif
        
        SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_PICVIEWER_RELOAD_FILE param=0x%x", param);
        //clear pic list
        // ClearPicList(win_id, ctrl_id);//delete by zhuyongjing for reload quickly
        //zhuyongjing change
        //original:MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleLoadPicWinMsg);
        if(param != PNULL){
            PIC_SOURCE_TYPE  foldertype = *(PIC_SOURCE_TYPE*)param;
            SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_PICVIEWER_RELOAD_FILE foldertype=%d", foldertype);
            if((!MMIPICVIEW_IfGetFileArrayFromListFile(*(PIC_SOURCE_TYPE*)param)) /*|| 
                (MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS) ||
                (MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS)*/){
                    //MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleLoadPicWinMsg,foldertype);
                    MMK_CreateWin((uint32 *)MMIPICVIEW_WAIT_WIN_TAB, (ADD_DATA)foldertype);
            }else{
                if(foldertype == CAMERA_ROLL_SOURCE_NONE){
                    MMIPICVIEW_SearchMainViewFile(win_id);
                    FinishLoadPic(win_id,TRUE);
                }else{
                   if(MMIPICVIEW_GetAllPicNum() > VIRTUALARRAY_BUFFER_SIZE){
                        GALLERY_FILEARRAY_SIG_TYPE_E task_type = GALLERY_FILEARRAY_SUBFOLDER_FILTER_FILEARRAY;
                        if(!MMIAPIPICVIEW_GetPreviousFolderIsSubfolder()){
                            MMIAPIPICVIEW_DestroyBackupAllPicArray();
                        }else{
                            MMIPICVIEW_DestroyAllPicArray();
                            MMIPICVIEW_CreateAllPicArray();
                        }
                        Goto_Subfolder_Filter_Signal();
                        MMK_CreateWin((uint32 *)MMIPICVIEW_TASK_WAIT_WIN_TAB, (ADD_DATA)task_type);
                    }else{
                        MMK_CreateWin((uint32 *)MMIPICVIEW_WAIT_WIN_TAB, (ADD_DATA)foldertype);
                    }
                }
            }
        }else{
            PIC_SOURCE_TYPE type = CAMERA_ROLL_SOURCE_NONE;
#ifdef MMI_SHOW_MSA
            if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_CAMERA){
                type = CAMERA_ROLL_SOURCE_CAPTURED;
            }
#endif
            if((!MMIPICVIEW_IfGetFileArrayFromListFile(type)) /*|| 
                (MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS) ||
                (MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS)*/){
                    MMK_CreateWin((uint32 *)MMIPICVIEW_WAIT_WIN_TAB, PNULL);
            }else{
                if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
                    MMIPICVIEW_SearchMainViewPicWithoutGifFile(win_id);
                    FinishLoadPic(win_id,TRUE);
                }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB
                    ||MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS){
                    MMIPICVIEW_SearchMainViewPicFile(win_id);
                    FinishLoadPic(win_id,TRUE);
                }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS){
                    MMIPICVIEW_SearchMainViewVideoFile(win_id);
                    FinishLoadPic(win_id,TRUE);
                }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_CAMERA){
#ifdef MMI_SHOW_MSA
                    MMK_CreateWin((uint32 *)MMIPICVIEW_WAIT_WIN_TAB, PNULL);
#else
                    MMIPICVIEW_SearchMainViewFile(win_id);
                    FinishLoadPic(win_id,TRUE);
#endif
                }else{
                    MMIPICVIEW_SearchMainViewFile(win_id);
                    FinishLoadPic(win_id,TRUE);
                }
            }
            //change end
        }     
        break;
    case MSG_PICVIEWER_LOAD_FINISH: //zhuyongjing add for backgroud search all file then flash
        {SCI_TRACE_LOW(" end search files, SearchFileTime: curr time: %d\n",__FUNCTION__,__LINE__, SCI_GetTickCount());
         if(s_is_close){
                SCI_TRACE_LOW("[mmipic] MSG_PICVIEWER_LOAD_FINISH s_is_close break");
                break;
            }
            s_is_reload_file_aftersearch_getfous = FALSE;//if lasttime goto gallery =true, error
            if(MMK_IsFocusWin(win_id)&&!MMIPICVIEW_GetListMarkable(ctrl_id)
                &&!MMK_IsOpenWin(MMIPICVIEW_LIST_OPT_WIN_ID)){
                //PICVIEW_ReloadAfterSearch();
                MMIPICVIEW_ReloadAfterSearchEx();
                if(MMIPICVIEW_GetListEmpty()){
                    FinishLoadPic(win_id,TRUE);
                }else{
                    //FinishLoadPic(win_id,FALSE);
                    FreshSomeItemsForLoadPic(win_id,ctrl_id);
                }
            }else{
                s_is_reload_file_aftersearch_getfous = TRUE;
            }
        }
        break;
    case MSG_PICVIEWER_UPDATE_LIST:
        //update file
        UpdatePicList(win_id, ctrl_id, *(uint16*)param);
#if defined(CAMERAROLL_SUPPORT)
        if(s_mmipicview_multi_uploading_done){
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE
                &&!MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
      
                s_mmipicview_multi_uploading_done = FALSE;
                s_mmipicview_multi_uploading_highlight = FALSE;
                ModifyPicListViewForUploadItems(win_id,ctrl_id,FALSE);
            }
        }
#endif				
        //zhuyongjing add for save items to onedrive 20150722
        if((MMIPICVIEW_GetMultiChoiseSaveToOneDrive() ||MMIPICVIEW_GetMultiChoiceDelete()||MMIPICVIEW_GetMultiChoiceShare())
            &&!MMIPICVIEW_GetListMarkable(ctrl_id)){
            HandlePicListMarked(ctrl_id);
        }
        //add end  
#if defined(CAMERAROLL_SUPPORT)        
        if(s_mmipicview_multi_uploading_done && MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
            ModifyPicListViewForUploadItems(win_id,ctrl_id,FALSE);
        }else if(s_mmipicview_multi_uploading_done
            && !MMIPICVIEW_GetListEmpty()
            && MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE){
            ModifyPicListViewForUploadItems(win_id,ctrl_id,FALSE);
        }else if(s_mmipicview_multi_uploading_done
            &&!MMIPICVIEW_GetListEmpty()
            &&!MMIPICVIEW_GetListMarkable(ctrl_id)
            &&MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()
            &&MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
            ModifyPicListViewForUploadItems(win_id,ctrl_id,TRUE);
        }
#endif
        //zhuyongjing add for softkey 20150815
        mark_num = MMIPICVIEW_GetSelectItemIndex(MMIPICVIEW_LIST_CTRL_ID, PNULL, 0);
        if(MMIPICVIEW_GetListMarkable(MMIPICVIEW_LIST_CTRL_ID)){
            if(GUIICONLIST_GetTotalIcon(ctrl_id)>0){
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MARK_MK, MIDDLE_BUTTON, TRUE);
            }else{
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
            }
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
        }else if(MMK_IsActiveCtrl(ctrl_id)){
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
            //zhuyongjing add for pb 20150826
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB
                ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS
                ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                /* GUIWIN_SetSoftkeyBgAlpha(win_id,100-35);
                GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);
                GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);*/
                GUIWIN_UpdateSoftkey(win_id);
            }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_WALLPAPER
                &&!MMIPICVIEW_GetListEmpty()){
                //dong.chunguang add for wallpaper
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);
            }else{
                //add end
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
            }
        }
        if(
#ifdef CAMERAROLL_SUPPORT
	     s_mmipicview_multi_uploading_done &&
#endif
            MMIPICVIEW_GetCurIndex(ctrl_id) != 0
            &&!MMIPICVIEW_GetListMarkable(ctrl_id)){
            MMIPICVIEW_SetCurIndex(ctrl_id,0);
        }
        //add end
        
        if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID) || MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID) || MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID) \
            || MMK_IsOpenWin(MMIPUB_ALERT_WIN_ID) || MMK_IsOpenWin(MMICOMMON_BANNER_WIN_ID ))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //add item text etc.
        item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
        if (PNULL == item_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_data_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1209_112_2_18_2_46_23_10,(uint8*)"");
            break;
        }
        
        AppendPicListItem(TRUE, item_data_ptr->item_index, 0, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //add item anim
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        if (PNULL == item_content_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_content_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1221_112_2_18_2_46_23_11,(uint8*)"");
            break;
        }
        
        AppendPicListItem(FALSE, item_content_ptr->item_index, item_content_ptr->item_content_index, ctrl_id);
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
        }
        break;
        
    case MSG_PICVIEWER_MODIFY_VIEW:
        ModifyPicListView(win_id, ctrl_id);
        break;
        
    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            /* GUIICONLIST_DISP_T display_ptr      = {0};
            display_ptr.softkey_id[0] = IMAGE_COMMON_MENU;
            display_ptr.softkey_id[1] = TXT_COMM_OPEN;//zhuyongjing add
            display_ptr.softkey_id[2] = IMAGE_COMMON_BACK;
            SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_CTL_ICONLIST_APPEND_TEXT");
            GUIICONLIST_AppendText(*(uint16 *)param, ctrl_id, &display_ptr);*/
            //zhuyongjing add
            cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
            if(MMIPICVIEW_GetListMarkable(ctrl_id)
#ifdef CAMERAROLL_SUPPORT
		&&!s_mmipicview_multi_uploading_done
#endif
		){
                if(MMIPICVIEW_IsSelectItem(ctrl_id,cur_index)){
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, TRUE);//STXT_SOFTKEY_UNMARK_MK
                }else{
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MARK_MK, MIDDLE_BUTTON, TRUE);
                }
            }
            //add end   
        }
        break;
        
    case MSG_CTL_ICONLIST_APPEND_ICON:
        {
            BOOLEAN res = FALSE;
            ////zhuyongjing add s_is_need_to_edit_from_pb for pb 20150826
        if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
                res = AppendPicIconListIconForPB(*(uint16 *)param, ctrl_id);
        }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS
        ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                res = AppendPicIconListIconForMMS(*(uint16 *)param, ctrl_id);
        }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
                BOOLEAN need_gif = FALSE;
                res = AppendOnlyPicIconListIcon(*(uint16 *)param, ctrl_id,need_gif);
        }else{
                //add end
                res = AppendPicIconListIcon(*(uint16 *)param, ctrl_id);
            }
        if(!res){
            if(GUIICONLIST_GetTotalIcon(ctrl_id)>0){
                    MMIPICVIEW_SetListEmpty(FALSE);
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }else{
                    MMIPICVIEW_SetListEmpty(TRUE);
                }
        }else{
                MMIPICVIEW_SetListEmpty(FALSE);
            }
        }
        break;
        
#ifdef  PIC_PREVIEW_U200_SUPPORT
    case MSG_CTL_ICONLIST_MARK_ITEM:
        {
            uint16 mark_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
            
            if((mark_num != GUIICONLIST_GetTotalIcon(ctrl_id)) && is_marked_all)
            {
                is_marked_all = FALSE;
                GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else if ((mark_num == GUIICONLIST_GetTotalIcon(ctrl_id)) && !is_marked_all)
            {
                is_marked_all = TRUE;
                GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_CANCEL_MARK);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#endif
        
    case MSG_GET_FOCUS:
        SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg MSG_GET_FOCUS ,s_is_reload_file_aftersearch_getfous=%d,s_is_reload_file=%d",s_is_reload_file_aftersearch_getfous,s_is_reload_file);
        // SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1250_112_2_18_2_46_23_12,(uint8*)"");
#ifdef DRM_SUPPORT
        if(MMIPICVIEW_GetIsDownloadRightsFile())
        {
            MMIPICVIEW_ModifyItemIcon(ctrl_id, MMIPICVIEW_GetCurIndex(ctrl_id));
            MMIPICVIEW_SetIsDownloadRightsFile(FALSE);
        }
#endif
        // GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        //	GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        // GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);        
        //zhuyongjing add for capture/record from camera when no item show 20150713        
        if(s_is_reload_file_aftersearch_getfous
            &&!MMK_IsOpenWin(MMIPICVIEW_LIST_OPT_WIN_ID)
            &&!MMIPICVIEW_GetListMarkable(ctrl_id)){
            BOOLEAN is_sync = FALSE;
                if(s_is_reload_file){
                    if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
                    MMIPICVIEW_SearchMainViewPicWithoutGifFile(win_id);
                    }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB
                        ||MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS){
                    MMIPICVIEW_SearchMainViewPicFile(win_id);
                    }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS){
                    MMIPICVIEW_SearchMainViewVideoFile(win_id);
                    }else{
                    MMIPICVIEW_SearchMainViewFile(win_id);
                }
                }else{
                //PICVIEW_ReloadAfterSearch();
                MMIPICVIEW_ReloadAfterSearchEx();
            }
            //if(GUIICONLIST_GetTotalIcon(ctrl_id) == 0 && MMIPICVIEW_GetAllPicNum() > 0){
            //    is_sync = TRUE;
            //}
            //FinishLoadPic(win_id,is_sync);
            FreshSomeItemsForLoadPic(win_id,ctrl_id);
            s_is_reload_file_aftersearch_getfous = FALSE;
        }
        //zhuyongjing add for capture/record from camera when no item show 20150713        
#if 0
    else if (s_is_need_to_reinit_picarray_from_piclib)
    {
        MMIPICVIEW_DestroyAllPicArray();
        MMIPICVIEW_CreateAllPicArray();
        //MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_CONTINUE, PNULL);
        MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_VIDEO_CONTINUE, PNULL);//zhuyongjing change for show video and photos
		s_is_need_to_reinit_picarray_from_piclib = FALSE;   
    }
#endif
        else if (s_is_reload_file&&!s_is_reload_file_aftersearch_getfous)
        {
            BOOLEAN is_sync = FALSE;
            
            //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            s_is_reload_file = FALSE;
            if(GUIICONLIST_GetTotalIcon(ctrl_id) == 0 && MMIPICVIEW_GetAllPicNum() > 0){
                is_sync = TRUE;
            }
            FinishLoadPic(win_id,is_sync);
        }
        //add end
        break;
    case MSG_KEYDOWN_OK:
        result = MMI_RESULT_FALSE;
        is_rec_keydown = TRUE;
        break;
#ifndef PIC_PREVIEW_U200_SUPPORT
    case MSG_KEYUP_OK:
        if(!is_rec_keydown){
            break;
        }else{
            is_rec_keydown = FALSE;
        }
    case MSG_APP_MENU:
    case MSG_CTL_OK:
        //dong.chunguang add for wallpaper
        if((MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_WALLPAPER)||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB)||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS)||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS)||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_CAMERA))
            break;
        //end
        if(MMIPICVIEW_GetAllPicNum() ==0
            &&GUIICONLIST_GetTotalIcon(ctrl_id) == 0
#ifdef MMI_SHOW_MSA
            &&(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()
            ||MMIPICVIEW_GetMultiChoiceDelete()==TRUE
            ||MMIPICVIEW_GetMultiChoiceShare() == TRUE)
#endif
            )
        {
            break;
        }
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_OPT_WIN_TAB, PNULL);
        break;
        
    case MSG_CTL_ICONLIST_MARK_ITEM:
        {
            FILEARRAY_DATA_T            file_info = {0};
            uint32              upload_index = 0;
            uint32              select_upload_num = 0;
            
            BOOLEAN is_exist = FALSE;
            MMI_STRING_T                title_src1_str = {0};
            MMI_STRING_T                title_src2_str = {0};
            mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0);
            
             if(MMIPICVIEW_GetMultiChoiceDelete()){
                if(mark_num == 0){
                    MMI_GetLabelTextByLang(STR_GAL_HDR_DELETE_EXT02, &title_str );
                }else{
                    MMI_GetLabelTextByLang(STR_GAL_HDR_DELETE_EXT02, &title_src1_str );
                    sprintf(mark_num_str,"%s%d%s", "(",mark_num, ")");
                    MMIAPICOM_StrToWstr((const uint8 *)mark_num_str, mark_num_wstr);
                    MMIAPICOM_Wstrcat(mark_wstr, mark_num_wstr);
                    title_src2_str.wstr_ptr = mark_wstr;
                    title_src2_str.wstr_len = MMIAPICOM_Wstrlen(title_src2_str.wstr_ptr);
                    MMIAPICOM_CatTwoString(&title_str, &title_src1_str, &title_src2_str);
                }
             }else if(MMIPICVIEW_GetMultiChoiceShare()){
                if(mark_num == 0){
                    MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02, &title_str );
                }else{
                    MMI_GetLabelTextByLang(STR_GAL_HDR_SHARE_EXT02, &title_src1_str );
                    sprintf(mark_num_str,"%s%d%s", "(",mark_num, ")");
                    MMIAPICOM_StrToWstr((const uint8 *)mark_num_str, mark_num_wstr);
                    MMIAPICOM_Wstrcat(mark_wstr, mark_num_wstr);
                    title_src2_str.wstr_ptr = mark_wstr;
                    title_src2_str.wstr_len = MMIAPICOM_Wstrlen(title_src2_str.wstr_ptr);
                    MMIAPICOM_CatTwoString(&title_str, &title_src1_str, &title_src2_str);
                }
             }else if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                if(mark_num == 0){
                    MMI_GetLabelTextByLang(STR_ONEDRIVE_HDR_SAVE_EXT01, &title_str );
                }else{
                    MMI_GetLabelTextByLang(STR_ONEDRIVE_HDR_SAVE_EXT01, &title_src1_str );
                    sprintf(mark_num_str,"%s%d%s", " (",mark_num, ")");
                    MMIAPICOM_StrToWstr((const uint8 *)mark_num_str, mark_num_wstr);
                    MMIAPICOM_Wstrcat(mark_wstr, mark_num_wstr);
                    title_src2_str.wstr_ptr = mark_wstr;
                    title_src2_str.wstr_len = MMIAPICOM_Wstrlen(title_src2_str.wstr_ptr);
                    MMIAPICOM_CatTwoString(&title_str, &title_src1_str, &title_src2_str);
                }
            }
            
               if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len)){
                GUIWIN_UpdateStb();
            }
                if(mark_num != 0){
                MMIAPICOM_DestroyString(title_str);
            }
            //zhuyongjing add
            cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
            if(MMIPICVIEW_GetListMarkable(ctrl_id)){
                if(MMIPICVIEW_IsSelectItem(ctrl_id,cur_index)){
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, FALSE);//STXT_SOFTKEY_UNMARK_MK
                }else{
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MARK_MK, MIDDLE_BUTTON, FALSE);
                }
            }
            
            if((mark_num != GUIICONLIST_GetTotalIcon(ctrl_id)) && is_marked_all)
            {
                is_marked_all = FALSE;
            }
            else if ((mark_num == GUIICONLIST_GetTotalIcon(ctrl_id)) && !is_marked_all)
            {
                is_marked_all = TRUE;
            }
            //for mutil items upload
            select_upload_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
            if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                if(MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)){
                    is_exist = MMIPICVIEW_IsFileExistUploadingPicArray(file_info.filename,file_info.name_len,&upload_index);
                    
                    if(MMIPICVIEW_IsSelectItem(ctrl_id,cur_index) && !is_exist){
                        MMIPICVIEW_AddFileToUploadingPicArray(MMIPICVIEW_GetAllUploadingPicArray(),file_info.filename);
                    }else if(!MMIPICVIEW_IsSelectItem(ctrl_id,cur_index) && is_exist){
                        MMIPICVIEW_DeleteUploadingPicFileInfoByIndex(upload_index);
                    }
                }
            }
            if(mark_num > 0 && MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_ONEDRIVE_HDR_UPLOAD_EXT02, RIGHT_BUTTON, FALSE);
            }else if(mark_num > 0 && (MMI_RESULT_TRUE ==MMIPICVIEW_GetMultiChoiceShare())){
                GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_LIST_SHARE, RIGHT_BUTTON, FALSE);
            }else if(mark_num > 0 && (MMI_RESULT_TRUE ==MMIPICVIEW_GetMultiChoiceDelete())){
                GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STR_LIST_DELETE, RIGHT_BUTTON, FALSE);
            }else{
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            //add end
        }
        break;
#else
    case MSG_CTL_LIST_LONGOK:
    case MSG_NOTIFY_ICONLIST_TPLONG:
        //长按List弹出context menu菜单
        if(!MMIPICVIEW_GetListMarkable(ctrl_id))
        {
            MMIPICVIEW_OpenPicListLongokOptWin();
        }
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(PNULL != param)
        {   
            uint16  marked_num = 0;
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#ifdef PIC_PREVIEW_U200_SUPPORT
            case MMIPICVIEW_LIST_RETURN_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
                
            case MMIPICVIEW_LIST_SLIDE_PREVIEW:
                MMIPICVIEW_SliePlay(ctrl_id);
                break;
                
            case MMIPICVIEW_LIST_MARKED:
                MMIPICVIEW_SetButtonsVisible(FALSE);
                InitMarkMenuButtons(win_id);
                HandlePicListMarked(ctrl_id);
                is_marked_all = FALSE;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case MMIPICVIEW_MARK_ALLMARK_CTRL_ID:
                //全部标记的状态下
                if(is_marked_all)
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                    HandlePicListUnmarkedAll(ctrl_id);
                    is_marked_all   = FALSE;
                }
                else
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_CANCEL_MARK);
                    HandlePicListMarkedAll(ctrl_id);
                    is_marked_all   = TRUE;
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case MMIPICVIEW_MARK_DELETE_CTRL_ID:
                marked_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0);
#ifdef DRM_SUPPORT
                if (MMIPICVIEW_IsDeleteDrm(ctrl_id))
                {
                    if (1 < marked_num)
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
                    if(1 <= marked_num)
                    {
                        //prompt query window
                        MMIPUB_OpenQueryWinByTextId(STR_GAL_HDR_DELETE_EXT02,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                    }
                }
                if(0 == MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0))
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case MMIPICVIEW_MARK_CANCEL_CTRL_ID:
                MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                MMIPICVIEW_SetButtonsVisible(TRUE);
                MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
#endif
            case MMIPICVIEW_LIST_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                break;
                
            default:              
                result = MMI_RESULT_FALSE;
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER &&!MMIPICVIEW_GetListEmpty())
        {
#ifndef MMI_SETTING_WALLPAER_WITH_PREVIEW
            MMIFMM_SELECT_RETURN_T          selected_info = {0};
            FILEARRAY_DATA_T 				file_info_ptr={0};
            wchar   						abs_preview_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
            uint16   						abs_preview_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
            uint16                          pic_item_start_index = 0;
            
            cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
            if(TRUE == MMIAPISET_IsDisableShareViaOption())
            {
                pic_item_start_index = 1;
                if(cur_index == 0)
                {
                    MMISET_OpenPreviewWallpaperWin((uint32)(3));
                    break;
                }
            }
            if(MMIPICVIEW_GetPicFileInfo(cur_index-pic_item_start_index,&file_info_ptr)){
                MMIAPIFMM_SplitFullPathExt(file_info_ptr.filename,MMIAPICOM_Wstrlen(file_info_ptr.filename),abs_preview_path, &abs_preview_path_length, PNULL, PNULL, PNULL, PNULL);
                
                selected_info.file_name_len = (uint16) MMIAPICOM_Wstrlen (file_info_ptr.filename);
                MMIAPICOM_Wstrncpy (selected_info.file_name_ptr, file_info_ptr.filename, selected_info.file_name_len);
            }
            selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
            
            MMK_SendMsg(MMISet_GetBackgroundWin(), MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
            MMK_CloseWin(win_id);
            break;
#endif
        }
        else if((MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS
		||(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS)) &&!MMIPICVIEW_GetListEmpty()){
		
            FILEARRAY_DATA_T file_info={0};
            MMIFMM_SELECT_RETURN_T          selected_info = {0};
            cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
#if 0
            if(cur_index == 0){
                if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS){
                    MMIAPIDC_OpenPhotoWin();
                }else{
                    MMIAPIDC_OpenVideoWin();
                }
            }else
#endif
            {
                if(MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)){
                    selected_info.file_name_len = (uint16) MMIAPICOM_Wstrlen (file_info.filename);
                    MMIAPICOM_Wstrncpy (selected_info.file_name_ptr, file_info.filename, selected_info.file_name_len);
                }
                if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS)
                {
                    selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
                }
                else
                {
                    selected_info.file_type = MMIFMM_FILE_TYPE_MOVIE;
                }
                
                selected_info.file_size = file_info.size;

                //for bug 1048522 , use post msg instead of send msg,free pic resource when return to sms
                MMK_SendMsg(g_picview_from_win, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);//bug1061760
                //MMK_PostMsg(g_picview_from_win, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info,sizeof(selected_info));
                MMK_CloseWin(win_id);
            }
            break;
        }
        
        if((0 < MMIPICVIEW_GetAllPicNum()) || 
            ((MMIPICVIEW_GetEnterType() != MMIPICVIEW_PHOTOS_FROM_MMS) &&
            (MMIPICVIEW_GetEnterType() != MMIPICVIEW_VIDEOS_FROM_MMS)) && 
            (MMIPICVIEW_GetEnterType() != MMIPICVIEW_PHOTOS_FROM_CAMERA)){
                HandlePicListWeb(ctrl_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        // {
        //     GUIICONLIST_PrepareSpecial(ctrl_id, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID);
        // }
        path = MMIAPIPICVIEW_GetShowFolderPath();//zhuyongjing add for diff path
        //lijinghua add
        if(MMIPICVIEW_GetMultiChoiceDelete())
        {
            uint16 mark_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
            if(0 == mark_num)
            {
                MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
#ifdef CAMERAROLL_SUPPORT								
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                    &&MMISRV_CAMERAROLL_Get_Connect_State()){
                    if(!s_is_reload_file_aftersearch_getfous){
                        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
                    }
                }
#endif								
                MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
                MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
                //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }else
            {
#ifdef DRM_SUPPORT
                if (MMIPICVIEW_IsDeleteDrm(ctrl_id))
                {
                    //prompt query window
                    MMIPUB_OpenConfirmationDialogByTextId(PNULL,TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL,PNULL);
                }
                else
#endif
                {
                    //prompt query window
                    MMI_STRING_T	result_str = {0};
                    wchar result_wchar[200] = {0};
                    mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0);
                    //sprintf(mark_num_str,"%s%d%s", "Delete ", mark_num ," selected items?");
                    //MMIAPICOM_StrToWstr((const uint8 *)mark_num_str, mark_num_wstr);
                    //MMIAPICOM_Wstrcat(mark_wstr, mark_num_wstr);
                    //title_str.wstr_ptr = mark_wstr;
                    //title_str.wstr_len = MMIAPICOM_Wstrlen(mark_wstr);
                    if(mark_num == 1){
                        MMI_GetLabelTextByLang(STR_QUEST_DELETE_ITEM, &result_str );    
                    }else{
                        result_str.wstr_ptr = result_wchar;
                        MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_QUEST_DELETE_ITEMS, 
                            L"%N", 
                            mark_num,
                            200, 
                            &result_str);
                    }
                    MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&result_str,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleMultiPicPreviewDelQueryWin);
                }
            }
        }
        else if(MMIPICVIEW_GetMultiChoiceShare())
        {
            uint16 mark_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
            if(0 == mark_num)
            {
                MMIPICVIEW_SetMultiChoiceShare(MMI_RESULT_FALSE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
#ifdef CAMERAROLL_SUPPORT																
                if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
                    &&MMISRV_CAMERAROLL_Get_Connect_State()){
                    if(!s_is_reload_file_aftersearch_getfous){
                        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
                    }
                }
#endif								
                MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
                //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
            }else{
                MMK_CreateWin((uint32 *)MMIPICVIEW_SHARE_ITEMS_LIST_OPT_WIN_TAB, PNULL);
            }//lijinghua end
            //zhuyongjing add 20150722
        }else if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
#ifdef CAMERAROLL_SUPPORT        
            MMI_WIN_ID_T  query_win_id = MMIPICVIEW_UPLOAD_ITEMS_QUERY_WIN_ID;
            uint16 mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id, PNULL, 0);
            uint16 index = MMIPICVIEW_GetCurIndex(ctrl_id);
            if(0 == mark_num)
            {
                MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
                MMIPICVIEW_DestroyAllUploadingPicArray();
                
                //MMIPICVIEW_DestroyAllPicArray();
                //  MMIPICVIEW_CreateAllPicArray();
                
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
                GUIICONLIST_SetTotalIcon(ctrl_id,MMIPICVIEW_GetAllPicNum());
                MMIPICVIEW_SetMarkable(ctrl_id,FALSE);
                MMIPICVIEW_PostUpdateMsg(index);
                }else{
                MMIPUB_OpenTextDialogByTextId(PNULL,STR_ONEDRIVE_NOTE_SAVING_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandlePicSaveItemsToOnedriveQueryWin);
            }
#endif								
	}else if(!MMIPICVIEW_GetListMarkable(ctrl_id)
            &&(path == CAMERA_ROLL_SOURCE_ALL_LOCAL
            || path == CAMERA_ROLL_SOURCE_CAPTURED
            || path == CAMERA_ROLL_SOURCE_DONWLOADED
            || path == CAMERA_ROLL_SOURCE_VIDEO
            ||path == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY)){
            
            //zhuyongjing change for search quickly
            BOOLEAN is_sync =  FALSE;
            // MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);	
            // MMIPICVIEW_DestroyAllPicArray();
            // MMIPICVIEW_CreateAllPicArray();
            MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            if(path == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                // MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetThumbnailArray());
            }
            MMIAPIPICVIEW_DestroyBackupAllPicArray();
            
            if(GUIICONLIST_GetTotalIcon(ctrl_id) == 0 && MMIPICVIEW_GetAllPicNum() > 0){
                is_sync = TRUE;
            }
            MMIAPIPICVIEW_SetShowFolderPath(CAMERA_ROLL_SOURCE_NONE);
            FinishLoadPic(win_id,is_sync);
            
            //end add
        }
#ifdef CAMERAROLL_SUPPORT
		else if(s_mmipicview_multi_uploading_done){
            s_mmipicview_multi_uploading_highlight = FALSE;
            MMK_CloseWin(win_id);
        }
#endif		
        else{
            MMK_CloseWin(win_id);
#ifdef  VIDEOTHUMBNAIL_SUPPORT
            //  MMIFMM_VideoThumbnailClose();
#endif
        }
        break;
        
    case MSG_APP_UP:
#ifdef CAMERAROLL_SUPPORT			
        if(s_mmipicview_multi_uploading_done){
            uint16 index =MMIPICVIEW_GetCurIndex(ctrl_id);
            if((index == 0 ||index == 1 ||index ==2) && GUIICONLIST_GetTotalIcon(ctrl_id) > 0){
                HightlightUploadPrgShow(win_id,TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, TRUE);
            }else{
                if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                    MMK_PostMsg(ctrl_id, MSG_APP_UP,  PNULL, PNULL);
                }
            }
        }else{
            // result = MMI_RESULT_FALSE;
        }
#endif				
        break;
    case MSG_APP_DOWN:
#ifdef CAMERAROLL_SUPPORT			
        if(s_mmipicview_multi_uploading_done){
            //must first update prg(active prg),then active iconlist
            if(s_mmipicview_multi_uploading_highlight){
                HightlightUploadPrgShow(win_id,FALSE);
                if(!MMK_IsActiveCtrl(ctrl_id)
                    &&MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                }
            }else{
                if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                    MMK_PostMsg(ctrl_id, MSG_APP_DOWN,  PNULL, PNULL);
                }
            }
            if(GUIICONLIST_GetTotalIcon(ctrl_id) > 0){
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
            }
        }
#endif				
        break;
    case MSG_APP_LEFT:
#ifdef CAMERAROLL_SUPPORT			
        if(s_mmipicview_multi_uploading_done){
            //must first update prg(active prg),then active iconlist
            if(s_mmipicview_multi_uploading_highlight){
                
            }else{
                if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                    MMK_PostMsg(ctrl_id, MSG_APP_LEFT,  PNULL, PNULL);
                }
            }
        }
#endif				
        break;
    case MSG_APP_RIGHT:
#ifdef CAMERAROLL_SUPPORT			
        if(s_mmipicview_multi_uploading_done){
            //must first update prg(active prg),then active iconlist
            if(s_mmipicview_multi_uploading_highlight){
                
            }else{
                if(MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                    MMK_SetAtvCtrl(win_id,ctrl_id);
                    MMK_PostMsg(ctrl_id, MSG_APP_RIGHT,  PNULL, PNULL);
                }
            }
        }
#endif				
        break;
    case MSG_KEYUP_DOWN:
        cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
	#ifdef CAMERAROLL_SUPPORT			
        if(MMISRV_CAMERAROLL_Get_Connect_State()==TRUE
            && !MMIPICVIEW_GetListMarkable(ctrl_id)
            && !MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()
            && (MMIAPIPICVIEW_GetShowFolderPath()== CAMERA_ROLL_SOURCE_NONE
            ||MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY)
            &&MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE){
            
            uint32 total =MMIPICVIEW_GetAllPicNum();
            SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_KEYUP_DOWN is_need_download_thumbnails=%d",is_need_download_thumbnails);
            #ifdef MMI_PIC_ONE_DRIVE_SUPPORT
            if(total >3){
                uint32 remainder = total % 3;
                if(remainder ==0 && (cur_index<total)&&(cur_index >= total-3)){
                    MMIPIC_CreateOneDriveSyncProgressDialogWin();
                    is_need_download_thumbnails =  TRUE;
                }else if((remainder ==1) && (cur_index==total-1)){
                    MMIPIC_CreateOneDriveSyncProgressDialogWin();
                    is_need_download_thumbnails =  TRUE;
                }else if((remainder ==2)&& (cur_index<total)&&(cur_index >= total-2)){
                    MMIPIC_CreateOneDriveSyncProgressDialogWin();
                    is_need_download_thumbnails =  TRUE;
                }  
            }else if(total <=3){                
                MMIPIC_CreateOneDriveSyncProgressDialogWin();
                is_need_download_thumbnails =  TRUE;
            }
            #endif	
        }
	#endif
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_UP:
    case MSG_KEYUP_LEFT:
#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
        if(((cur_index==0) ||(cur_index==1)||(cur_index==2))
            && s_mmipicview_multi_uploading_done
            &&!s_mmipicview_multi_uploading_highlight
            &&GUIICONLIST_GetTotalIcon(ctrl_id) > 0
            &&!MMIPICVIEW_GetListMarkable(ctrl_id)
            &&!MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)
            &&MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE){
            SCI_TRACE_LOW("[key up]:active MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID ");
            MMK_SetAtvCtrl(win_id,MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
        }
#endif
        break;
        //add end
    case MSG_APP_RED:
        if(APPFILE_IsFindingFile()){
            s_mmipicview_is_searching = TRUE;
            MMIAPIFMM_SearchFileStop();
        }else{
            s_mmipicview_is_searching = FALSE;
        }
        result = MMI_RESULT_FALSE;
        break;
    case MSG_KEYLONG_RED:
        s_is_shutdown = TRUE;
        result = MMI_RESULT_FALSE;
        break;
   #ifdef CAMERAROLL_SUPPORT      
        //zhuyongjing add for onedrive upload and download notify
    case MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY :      
        SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY ");
        break;
    case MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF:
        {
            CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
            uint32 original_index = 0;
            pic_num = MMIPICVIEW_GetAllPicNum();
            
            original_index = MMIPICVIEW_GetPhotoIndexAccordingName(data->camera_roll_msg_data.filename);
            SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF zyj done=%d, index=%d" , data->camera_roll_msg_data.bDone,original_index);
            
        if(CAMERA_ROLL_MANUAL_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()){  
            if(data->camera_roll_msg_data.bDone){
                    //set succeed cloud icon in iconlist
                    GUIICONLIST_SetIndicateIcon(ctrl_id , original_index, IMAGE_GALLERY_ONEDRIVE_UPLOADED_ICON);

            }else{
                    
                    //set failed cloud icon in iconlist
                }
        }else if(CAMERA_ROLL_AUTO_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()){
                
            if(data->camera_roll_msg_data.bDone){
                    //notify remove error cloud in iconlist
                    GUIICONLIST_SetIndicateIcon(ctrl_id , original_index, NULL);
            }else{
                    GUIICONLIST_SetIndicateIcon(ctrl_id , original_index, IMAGE_GALLERY_ONEDRIVE_FAILED_UPLOAD_ICON);
                }
            }
            SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_CAMERA_ROLL_UPLOAD_PHOTO_CNF s_mmipicview_multi_uploading_done=%d" , s_mmipicview_multi_uploading_done);
            
            //d. set next item upload if multi items upload
        if(s_mmipicview_multi_uploading_done){            
                HandleUploadSelectPics(win_id,ctrl_id,data->camera_roll_msg_data.bDone);    
        }else if(MMK_IsFocusWin(win_id)&&!MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            
        }
        break;
        
    case MSG_CAMERA_ROLL_INIT_CNF:
        {
            CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
            CAMERA_ROLL_TYPE_E fromtype = CAMERA_ROLL_TYPE_NONE;
        if(param!=PNULL){
                fromtype = data->camera_roll_msg_data.init_type; 
            }
            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_CAMERA_ROLL_INIT_CNF is_need_download_thumbnails=%d,s_mmipicview_multi_uploading_done=%d,fromtype=%d",is_need_download_thumbnails,s_mmipicview_multi_uploading_done,fromtype);
        if(s_mmipicview_multi_uploading_done){
                MMISRV_CAMERAROLL_Upload_multiple_photos();
        }else if(is_need_download_thumbnails){
                //  s_thumbnail_down_total_count = 0;
                MMISRV_CAMERAROLL_Download_Thumbnail();
            }
        }
        break;
    case MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY  :
        {
            CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
            GUIANIM_DATA_INFO_T data_info   = {0};
            uint8 i = 0;
            if(    MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE
                    && MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                break;
            }               
            cur_index=MMIPICVIEW_GetCurIndex(ctrl_id);
            
            data_info.img_id = IMAGE_ONEDRIVE_DEFAULT_PIC;
            //total_count = data->camera_roll_msg_data.total_num ; 
            s_thumbnail_down_total_count = data->camera_roll_msg_data.curr_download_num;
            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_NOTIFY total_count=%d",s_thumbnail_down_total_count);
            if( s_thumbnail_down_total_count == 0
                    ||MMIPICVIEW_GetListMarkable(ctrl_id)){
                break;
            }
                if(s_thumbnail_down_total_count <= MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX){
                    if(MMIPICVIEW_GetAllPicNum() == 0){
                    MMK_DestroyControl(MMIPICVIEW_LIST_EMPTY_CTRL_ID);
                    MMIPICVIEW_SetListEmpty(FALSE);
                    CreatePicListCtrl(win_id, ctrl_id);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
                    GUIICONLIST_SetTotalIcon(ctrl_id, s_thumbnail_down_total_count);
                        for(i=0;i<s_thumbnail_down_total_count;i++){
                        GUIICONLIST_AppendIcon(i, ctrl_id, &data_info, PNULL);
                    }
                    }else{
                    GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_GetAllPicNum()+s_thumbnail_down_total_count);
                    MMIPICVIEW_SetCurIndex(ctrl_id,cur_index);
                    for(i=0;i<s_thumbnail_down_total_count;i++){
                        GUIICONLIST_AppendIcon(MMIPICVIEW_GetAllPicNum()+i, ctrl_id, &data_info, PNULL);
                    }
                }
                }else{//more than 12 pics
                    if(MMIPICVIEW_GetAllPicNum() == 0){
                    GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX);
                        for(i=0;i<MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX;i++){
                        GUIICONLIST_AppendIcon(i, ctrl_id, &data_info, PNULL);
                    }
                    }else{
                    GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_GetAllPicNum()+MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX);
                    MMIPICVIEW_SetCurIndex(ctrl_id,cur_index);
                        for(i=0;i<MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX;i++){
                        GUIICONLIST_AppendIcon(MMIPICVIEW_GetAllPicNum()+i, ctrl_id, &data_info, PNULL);
                    }
                }
            }
            index = 0;
            if(MMK_IsFocusWin(win_id)){
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;	
    case MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF:
        {
            CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
            GUIANIM_FILE_INFO_T     icon_file = {0};
            uint8 count = 0;
            uint8 i = 0;
            uint16 pic_array_num = MMIPICVIEW_GetAllPicNum();
            char    temp_code[100]={0};
            GUIANIM_DATA_INFO_T data_info   = {0};
            
            count = data->camera_roll_msg_data.success_num; 
            MMIAPICOM_WstrToStr(data->camera_roll_msg_data.filename, (uint8 *)temp_code);
            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF count %d,total=%d,filename=%s,done=%d" , count,
                data->camera_roll_msg_data.total_num,temp_code,data->camera_roll_msg_data.bDone);
                if(!data->camera_roll_msg_data.bDone){
                    if(s_thumbnail_down_total_count > 0){
                    s_thumbnail_down_total_count--;
                }
                    if(pic_array_num < GUIICONLIST_GetTotalIcon(ctrl_id)){
                    MMIPICVIEW_RemoveItem(ctrl_id,GUIICONLIST_GetTotalIcon(ctrl_id)-1);
                }
                    if(MMK_IsFocusWin(win_id)){
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
                break;
            }
            icon_file.full_path_wstr_ptr = data->camera_roll_msg_data.filename;
            icon_file.full_path_wstr_len = MMIAPICOM_Wstrlen(data->camera_roll_msg_data.filename);
            if(    MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE
                && MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY
                    ||MMIPICVIEW_GetListMarkable(ctrl_id)){
                // MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetThumbnailArray(),data->camera_roll_msg_data.filename);
                MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetBackupAllPicArray(),data->camera_roll_msg_data.filename);
                break;
            }
            data_info.img_id = IMAGE_ONEDRIVE_DEFAULT_PIC;
		if(pic_array_num == 0){
                    if(GUIICONLIST_GetTotalIcon(ctrl_id) == 0){
                    MMK_DestroyControl(MMIPICVIEW_LIST_EMPTY_CTRL_ID);
                    MMIPICVIEW_SetListEmpty(FALSE);
                    CreatePicListCtrl(win_id, ctrl_id);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
                    GUIICONLIST_SetTotalIcon(ctrl_id, s_thumbnail_down_total_count);
                    
                        for(i=0;i<s_thumbnail_down_total_count;i++){
                        GUIICONLIST_AppendIcon(i, ctrl_id, &data_info, PNULL);
                    }
                    
                }
                GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
                    if(MMIAPIPICVIEW_GetAllPicArray() == PNULL){
                    MMIPICVIEW_CreateAllPicArray();
                    MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),data->camera_roll_msg_data.filename);
                    }else{
                    MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),data->camera_roll_msg_data.filename);
                }
            }
                else{
                
                    if(GUIICONLIST_GetTotalIcon(ctrl_id) <= pic_array_num){
                    cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
                    GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_GetAllPicNum()+s_thumbnail_down_total_count -index);
                    MMIPICVIEW_SetCurIndex(ctrl_id,cur_index);
                    
                        for(i=0;i<s_thumbnail_down_total_count-index;i++){
                        GUIICONLIST_AppendIcon(pic_array_num +i, ctrl_id, &data_info, PNULL);
                    }
                }
                GUIICONLIST_AppendIcon(pic_array_num, ctrl_id, PNULL, &icon_file);
                MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),data->camera_roll_msg_data.filename);
            }
                if(MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE){
                MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetBackupAllPicArray(),data->camera_roll_msg_data.filename);
            }
                if(MMK_IsFocusWin(win_id)){
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            index++;
            if(s_thumbnail_down_total_count <= MMIPICVIEW_ONEDRIVE_DOWNLOAD_THUMBNAIL_MAX
                    &&index >= s_thumbnail_down_total_count){
                is_need_download_thumbnails = FALSE;
                s_thumbnail_down_total_count = 0;//finish download thumbnails
                }else{
                is_need_download_thumbnails = TRUE;
            }
            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_CAMERA_ROLL_DOWNLOAD_THUMBNAIL_CNF index %d,is_need_download_thumbnails=%d",
                index,is_need_download_thumbnails);
            
            
    }
    break;
    case MSG_CAMERA_ROLL_DELETE_PHOTO_NOTIFY:
        DeleteLocalThumbnailsByServer(ctrl_id);
        break;
    case MSG_CAMERA_ROLL_ERROR_NOTIFY:
        {
            CAMERA_ROLL_SIG_T * data =(CAMERA_ROLL_SIG_T *)param;
            ONE_DRIVE_ERROR_E err_trype = data->camera_roll_msg_data.error_type;
            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_CAMERA_ROLL_ERROR_NOTIFY err_trype=%d",err_trype);
#ifdef WIN32
            break;
#endif
        if(s_is_shutdown){
                break;
            }
            if(CAMERA_ROLL_AUTO_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
                && (err_trype == ONE_DRIVE_ERROR_NO_NETWORK||err_trype == ONE_DRIVE_ERROR_HTTP_ERROR)
                && MMISRV_CAMERAROLL_Need_snapshot_auto_upload_photo()
                && MMISRV_CAMERAROLL_Get_Connect_State()
            &&MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE){
                DisplaybannerInfo(win_id,STR_ONEDRIVE_ERROR_SYNC_EXT01);
                //need api which file is auto uploading
                //GUIICONLIST_SetIndicateIcon(ctrl_id , original_index, IMAGE_GALLERY_ONEDRIVE_FAILED_UPLOAD_ICON);
        }else if(CAMERA_ROLL_MANUAL_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
                &&(s_mmipicview_multi_uploading_done||MMISRV_CAMERAROLL_Need_manual_auto_upload_photo())){
                if(MMISRV_CAMERAROLL_Get_Auto_upload_status()==ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK
                    ||err_trype == ONE_DRIVE_ERROR_NO_NETWORK
                    ||err_trype == ONE_DRIVE_ERROR_HTTP_ERROR){
                    
                    MMISRV_CAMERAROLL_Clean_manual_upload_list();
                    MMISRV_CAMERAROLL_Cancel_Upload();
                    
                if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE){
                        if(err_trype == ONE_DRIVE_ERROR_HTTP_ERROR){
                            DisplaybannerInfo(win_id,STR_NO_DATA_CONNECTION);
                    }else{
                            DisplaybannerInfo(win_id,STR_ONEDRIVE_UPLOAD_FAILED_EXT01);
                        }
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
                        
                    if(MMK_IsFocusWin(win_id)){
                            s_mmipicview_multi_uploading_done = FALSE;
                            s_mmipicview_multi_uploading_highlight = FALSE;
                            ModifyPicListViewForUploadItems(win_id,ctrl_id,FALSE);
                            MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
                        }
                    }
                }     
            }
            
        if(MMIPICVIEW_GetEnterType()!=MMIPICVIEW_FROM_NONE){
                break;
            }
            if(MMIAPIPICVIEW_GetShowFolderPath()!=CAMERA_ROLL_SOURCE_NONE
            &&MMIAPIPICVIEW_GetShowFolderPath()!=CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
                break;
            }
            //delete default thumbnail icons when err
            if(index< s_thumbnail_down_total_count
                && (err_trype == ONE_DRIVE_ERROR_NO_NETWORK
            ||err_trype == ONE_DRIVE_ERROR_HTTP_ERROR)){
                uint16 icon_num = GUIICONLIST_GetTotalIcon(ctrl_id);
                uint16 file_num = MMIPICVIEW_GetAllPicNum();
                SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg thumbnais  icon_num=%d,file_num=%d",icon_num,file_num);
            while((GUIICONLIST_GetTotalIcon(ctrl_id)-file_num > 0) && (s_thumbnail_down_total_count -index > 0)){
                    SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg thumbnais remove index=%d,s_thumbnail_down_total_count=%d",index,s_thumbnail_down_total_count);
                    MMIPICVIEW_RemoveItem(ctrl_id,GUIICONLIST_GetTotalIcon(ctrl_id)-1);
                    s_thumbnail_down_total_count--;
                }
                //when end icon with file,but the icon middle without file,such as download thumbnail timeout, then insert others normal icon,need reload
           if((GUIICONLIST_GetTotalIcon(ctrl_id)-file_num > 0)&&(s_thumbnail_down_total_count -index <= 0) && MMIPICVIEW_GetAllPicNum() > 0){
                    MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
            }else if(MMIPICVIEW_GetAllPicNum() == 0
                &&!GUIICONLIST_GetTotalIcon(ctrl_id) ==0){
                    FinishLoadPic(win_id,FALSE);
            }else if(GUIICONLIST_GetTotalIcon(ctrl_id)==file_num){
                if(MMK_IsFocusWin(win_id)){
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                }
                index = 0;
                s_thumbnail_down_total_count = 0;
            }
            
        }
        break;
        //add end
     #endif
    case MSG_CLOSE_WINDOW:
        // #ifndef PIC_PREVIEW_U200_SUPPORT
        //         s_is_reload_file = FALSE;
        // #endif
        //destroy pic array
        s_is_close = TRUE;  
        index = 0;
        s_thumbnail_down_total_count = 0;
        MMIAPIFMM_SearchFileStop();
        //lijinghua add
        MMIPICVIEW_SetMultiChoiceDelete(MMI_RESULT_FALSE);
        MMIPICVIEW_SetMultiChoiceShare(MMI_RESULT_FALSE);
        MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
        
        g_picview_from_win = 0;
#ifdef CAMERAROLL_SUPPORT		
        MMISRV_CAMERAROLL_Leave_Gallery();
#endif        
        MMI_GetLabelTextByLang(STR_GAL_HDR_GALLERY_EXT01, &title_str );
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
        {
            GUIWIN_UpdateStb();
        }
        //lijinghua end
        if(!s_mmipicview_is_searching){
            //MMIPICVIEW_SaveFileArrayToListFile(MMIAPIPICVIEW_GetShowFolderPath());//save main filearray to file
        }else{
            s_mmipicview_is_searching = FALSE;
        }
        
        //MMIPICVIEW_DestroyAllPicArray();
        MMIVIRTUALARRAY_UpdateBufferToFile(MMIAPIPICVIEW_GetCurPicArray());
        MMIPICVIEW_DestroyThumbnailsArray();
        MMIAPIPICVIEW_DestroySearchAllPicArray();
        MMIAPIPICVIEW_DestroyBackupAllPicArray();
        MMIPICVIEW_DestroyAllUploadingPicArray();
        MMIAPIPICVIEW_DestroyPreviousPicArray();
        s_is_reload_file_aftersearch_getfous = FALSE;//if lasttime goto gallery =true, error
        
        MMIAPIPICVIEW_SetShowFolderPath(CAMERA_ROLL_SOURCE_NONE);
        if (s_is_need_to_reinit_picarray_from_piclib)
        {
#ifndef MMI_PICVIEW_MINI_SUPPORT
#ifndef PIC_PREVIEW_U200_SUPPORT
            MMIPICVIEW_InitPicArrayFromPicLib(s_cur_folder_index_for_reinit_picarray - 1);
#else
            MMIPICVIEW_InitPicArrayFromPicLib(s_cur_folder_index_for_reinit_picarray);
            
#endif
#endif
            
#ifndef MMI_PICVIEW_MINI_SUPPORT
            s_is_need_to_reinit_picarray_from_piclib = FALSE;
#endif
        }
        break;
    default:
        result = MMIAPIPICVIEW_HandleDelQueryWin(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle picture list open win msg
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void OpenPicListWin(
                          MMI_WIN_ID_T      win_id, //in:
                          MMI_CTRL_ID_T     ctrl_id //in:
                          )
{
#ifdef  PIC_PREVIEW_U200_SUPPORT
    MMI_CTRL_ID_T   ctrl_array[]= {MMIPICVIEW_LIST_SLIDE_PREVIEW, MMIPICVIEW_LIST_MARKED};
    MMI_IMAGE_ID_T  img_array[] = {IMAGE_PICVIEW_PLAY_DEFAULT, IMAGE_PICVIEW_MARK_DEFAULT};  
#endif
    //    GUI_BG_T        bg_info     = {0};
    
    DisplayBackgroudImg(win_id);
    
    // bg_info.bg_type = GUI_BG_IMG;
    // bg_info.img_id	= PICVIEW_DEFAULT_BG_IMG;
    // GUISTATUSBAR_SetBg(win_id, bg_info);
    // GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    // GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
    // GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#ifdef  PIC_PREVIEW_U200_SUPPORT
    // SetPicListTitle(win_id, MMIPICVIEW_LIST_TITLE_CTRL_ID, MMIPICVIEW_LIST_RETURN_CTRL_ID);
    CreatePicListCtrl(win_id, ctrl_id);
    SetButtonFormParam(win_id, ctrl_array, img_array, 2);
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
    //MINI UI设计中无ListTitle
    // SetPicListTitle(win_id, MMIPICVIEW_LIST_TITLE_CTRL_ID, PNULL);//delete by zhuyongjing
#else
    if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        GUITITLE_SetTextAlignType(MMIPICVIEW_LIST_WIN_ID, MMITHEME_GetTitleCtrlId(), ALIGN_HMIDDLE);
    }
#endif
    CreatePicListCtrl(win_id, ctrl_id);
    /* MINI UI 需求:add by langhua.yang display the total num */   
#ifdef MMI_GUI_STYLE_MINISCREEN
    {
        GUI_BOTH_RECT_T         both_rect={0};
        both_rect.v_rect.top    = MMIPICVIEW_TOTAL_NUM_TOP;
        both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPICVIEW_TOTAL_NUM_HEIGHT;
        both_rect.v_rect.left   = MMIPICVIEW_TOTAL_NUM_V_LEFT;
        both_rect.v_rect.right  = MMIPICVIEW_TOTAL_NUM_V_RIGHT;
        
        both_rect.h_rect.top    = MMIPICVIEW_TOTAL_NUM_TOP;
        both_rect.h_rect.bottom = both_rect.v_rect.top + MMIPICVIEW_TOTAL_NUM_HEIGHT;
        both_rect.h_rect.left   = MMIPICVIEW_TOTAL_NUM_H_LEFT;
        both_rect.h_rect.right  = MMIPICVIEW_TOTAL_NUM_H_RIGHT;
        GUIAPICTRL_SetBothRect(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID,&both_rect);
        if (MMK_GetCtrlPtr(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID))
        {
            IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        }
    }
#endif
    
    // GUIWIN_SetSoftkeyCommonBg(win_id, &bg_info);
    // GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMM_OPEN/*TXT_NULL*/, TXT_NULL, FALSE);//zhuyongjing change 20150707
    //dong.chunguang add for wallpaper
	if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);
        //end
        //zhuyongjing add for pb 20150826
	}else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB){
        GUIWIN_SetSoftkeyBgAlpha(win_id,100-35);
        GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);
        
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);
        }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS
            ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
        GUIWIN_SetSoftkeyBgAlpha(win_id,100-35);
        GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);
        
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);   
        
	}else
        //end
    {
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
    }
#endif
    if (s_is_need_to_reinit_picarray_from_piclib)
    { 
#ifndef MMI_PICVIEW_MINI_SUPPORT
        s_cur_folder_index_for_reinit_picarray = MMIPICVIEW_ReleasePicArrayFromPicLib();
#endif
        if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_MYDOC))
        {
            //validate privacy password
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_MYDOC, (MMISET_PRIVACY_CALLBACK_PFUNC)PICVIEW_ValidatePrivacyPicList, (void*)&win_id, sizeof(win_id));
        }
        else
        {

            //同步打开等待窗口, 避免闪烁
            MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
        }
    }
    else if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
    {
        GUIICONLIST_PrepareSpecial(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, ctrl_id);
        GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_GetAllPicNum());
        GUIICONLIST_SetCurIconIndex(0, ctrl_id);
        LoadPicList(ctrl_id);
        //SetlistOrderInTime(ctrl_id);
    }
}

/*****************************************************************************/
//  Description : create pictures list control
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
PUBLIC void CreatePicListCtrl(
                             MMI_WIN_ID_T   win_id, //in:
                             MMI_CTRL_ID_T  ctrl_id //in:
                             )
{
    GUILIST_INIT_DATA_T     list_init_data = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIICONLIST_INIT_DATA_T icon_init_data = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_GetViewType();
    GUI_BOTH_RECT_T         both_rect = MMIPICVIEW_GetClientWinRect(win_id);
    GUI_BORDER_T    		boder_type  = {0};
    
    //SCI_TRACE_LOW:"MMIPICVIEW CreatePicListCtrl view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1488_112_2_18_2_46_24_13,(uint8*)"d", view_type);
    
    //zhuyongjing add for uploading items
    if(MMK_IsActiveCtrl(MMIPICVIEW_LIST_EMPTY_CTRL_ID)) {
        MMK_DestroyControl(MMIPICVIEW_LIST_EMPTY_CTRL_ID);
    }
#ifdef CAMERAROLL_SUPPORT		
    CreatUploadingLabelCtrl(win_id);
    CreatUploadingPrgboxCtrl(win_id);
    //add end
#endif    
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set init data
        list_init_data.both_rect = both_rect;
        list_init_data.type      = GUILIST_TEXTLIST_E;
        
        create.guid          = SPRD_GUI_LIST_ID;
        create.init_data_ptr = &list_init_data;
        break;
        
    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set init data
        icon_init_data.both_rect = both_rect;
        
        create.guid          = SPRD_GUI_ICONLIST_ID;
        create.init_data_ptr = &icon_init_data;
        break;
    }
    
    //creat control
    create.ctrl_id           = ctrl_id;
    create.parent_win_handle = win_id;
    
    MMK_CreateControl(&create);
    
    //set active
    MMK_SetAtvCtrl(win_id, ctrl_id);
    if(view_type == MMIPICVIEW_VIEW_ICONLIST)//NEWMS00172348
    {
        GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);
        
        GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);//zhuyongjing open
        MMIPICVIEW_SetIconMaskThemeImg(ctrl_id);//zhuyongjing add for mark circle
        MMIPICVIEW_SetMarkThemeImg(ctrl_id);
        
        boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
        boder_type.width    = 0;
        boder_type.type     = GUI_BORDER_SOLID;
        GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);    
        boder_type.width    = 4;
        boder_type.color    = MMI_RED_COLOR;//MMI_WHITE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);/*MMI_DARK_GREEN_COLOR;*/zhuyongjing
        GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
        
#ifdef MMI_GUI_STYLE_MINISCREEN
        /*MINI UI 需求:不显示list_title*/
        CTRLICONLIST_SetIsDispTitle(ctrl_id, FALSE);
#endif
        
        SetIconListParam(win_id, ctrl_id, FALSE);
        GUIICONLIST_SetDefaultIconByPtr(ctrl_id, IMAGE_ONEDRIVE_DEFAULT_PIC, IMAGE_PICVIEW_PIC_ERROR_ICON);
        //change by zhuyongjing for iconlist line margion 20150707
        GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON);
        // GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);
        //change end
        GUIICONLIST_SetLoadType(ctrl_id, GUIICONLIST_LOAD_ONE_BY_ONE);
        // GUIICONLIST_SetIconCacheNum(ctrl_id,1);
    }
    else
    {
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_SOFTKEY, TRUE);
    }
}

/*****************************************************************************/
//  Description : load all picture files by spinner win
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicSpinnerWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    		result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T 		file_filter = {0};
    //static MMIFILE_DEVICE_E s_searching_device = MMI_DEVICE_UDISK;
    PIC_SOURCE_TYPE     type = CAMERA_ROLL_SOURCE_NONE;
    //static PIC_SOURCE_TYPE s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
    MMI_HANDLE_T spinner_ctr_id = NULL;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};

    type = (PIC_SOURCE_TYPE)MMK_GetWinAddDataPtr(win_id);
    SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicSpinnerWinMsg type=%d,msg_id=%x",type,msg_id);
    spinner_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPICVIEW_WAITING_SPINNER_CTRL_ID);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;

        if(type == CAMERA_ROLL_SOURCE_CAPTURED){
            s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
        }
        if (s_wait_ready_timer_id != 0)
        {
            MMK_StopTimer(s_wait_ready_timer_id);
            s_wait_ready_timer_id = 0;
        }
        s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        //data_info.anim_id = ANIM_PUBWIN_WAIT;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_BLACK_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);

	//if(type != CAMERA_ROLL_SOURCE_NONE){
	    //MMIPICVIEW_SaveFileArrayToListFile(CAMERA_ROLL_SOURCE_NONE);
	//}
        SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicSpinnerWinMsg MSG_OPEN_WINDOW  tick=%d",SCI_GetTickCount());
        result = StartLoadPicForSpinner(win_id, msg_id, &type);
        if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_CAMERA){
            GUIWIN_SetSoftkeyVisible(win_id, FALSE);
        }
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
        //data_info.anim_id = ANIM_PUBWIN_WAIT;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_BLACK_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);      
        break;
    case MSG_PICVIEWER_LOAD_FINISH:
    {
       /* FILEARRAY temparray = MMIAPIFILEARRAY_Create();
        MMIAPIFILEARRAY_Combine(temparray,MMIAPIPICVIEW_GetAllPicArray());

        if(MMISRV_CAMERAROLL_Get_Connect_State() 
        && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
        &&!MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
            MMIPICVIEW_GetThumbnailsByMainArray(temparray);
        }else{
            MMIPICVIEW_GetThumbnailsByMainArray(temparray);
            MMIAPIPICVIEW_AllLocalFileArray(MMIAPIPICVIEW_GetAllPicArray());
        }
        MMIAPIFILEARRAY_Destroy(&temparray);*/
        
        if(   type ==CAMERA_ROLL_SOURCE_NONE
            || type ==CAMERA_ROLL_SOURCE_ALL_LOCAL){
            all_pic_video_num = MMIPICVIEW_GetAllPicNum();
        }
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        FinishLoadPic(win_id,FALSE);
        SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicSpinnerWinMsg MSG_PICVIEWER_LOAD_FINISH  tick=%d",SCI_GetTickCount());
        }
        break;
    case MSG_PICVIEWER_LOAD_CONTINUE:
        GetAllPicSuffix(&file_filter);
        if(!MMIPICVIEW_SearchAllPhotos(win_id, &file_filter, &s_searching_device))
        {
        	if(MMIAPIFMM_GetAllAvarialeDiskNum() > 1)
        	{
        		MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(), FILEARRAY_SORT_TIME_CREATE_DESCEND);
        	}
                s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;
        	MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
        }
    break;
    case MSG_PICVIEWER_LOAD_VIDEO_CONTINUE:
        GetAllPicAndVideoSuffix(&file_filter);
        if(0==MMIPICVIEW_SearchAllCaptured(win_id, &file_filter, &s_path_type,&s_searching_device)){
            int32 len = MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetAllPicArray());
            s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
            s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;
            MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(), FILEARRAY_SORT_TIME_CREATE_DESCEND);
            MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
        }
	break;
    case MSG_TIMER:  
        if(MMK_IsFocusWin(win_id))
        {
            SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicSpinnerWinMsg s_wait_ready_timer_id=%d,para=%d",s_wait_ready_timer_id,*((uint8 *) param));

        }
        break;
    //case MSG_APP_FLIP:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        //add by qin.he for bugId 1068645
        if (APPFILE_IsFindingFile())
        {
            MMIAPIFMM_SearchFileStop();
        }
        else
        {
            FinishLoadPic(win_id,FALSE);
        }
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

/*****************************************************************************/
//  Description : load all picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    		result = MMI_RESULT_TRUE;
	MMIFMM_FILTER_T 		file_filter = {0};
    static MMIFILE_DEVICE_E s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;
    //zhuyongjing add for gallary ui 20150707
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    PIC_SOURCE_TYPE     type = CAMERA_ROLL_SOURCE_NONE;
    static PIC_SOURCE_TYPE s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
    //add end
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicWinMsg");
    if(PNULL == pubwin_info_ptr)
    {
        type = PNULL;
    }else{
        type = pubwin_info_ptr->user_data;
    }
    //add end
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //zhuyongjing change for gallary ui 20150707
        //origial:result = StartLoadPic(win_id, msg_id, param);
    if(type == CAMERA_ROLL_SOURCE_CAPTURED){
            s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
        }
        
    if(type == PNULL){
            result = StartLoadPic(win_id, msg_id, param);
    }else{
            result = StartLoadPic(win_id, msg_id, &type);
        }
        //add end     
        break;
        
    case MSG_PICVIEWER_LOAD_FINISH:
        //zhuyongjing add for gallary ui 20150707
        if(type ==CAMERA_ROLL_SOURCE_NONE
        || type ==CAMERA_ROLL_SOURCE_ALL_LOCAL){
            all_pic_video_num = MMIPICVIEW_GetAllPicNum();
        }
        //add end
        FinishLoadPic(win_id,FALSE);
        break;
        
    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        break;
        
    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_PICVIEWER_LOAD_CONTINUE:
        GetAllPicSuffix(&file_filter);
        if(!MMIPICVIEW_SearchAllPhotos(win_id, &file_filter, &s_searching_device))
        {
            if(MMIAPIFMM_GetAllAvarialeDiskNum() > 1)
            {
                MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(), FILEARRAY_SORT_TIME_CREATE_DESCEND);
            }
            s_searching_device = MMI_DEVICE_SDCARD;//MMI_DEVICE_UDISK;
            MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
        }
        break;
    //zhuyongjing add for ms folder 20150811
    case MSG_PICVIEWER_LOAD_VIDEO_CONTINUE:

  /*      const wchar* dev_path = PNULL;
        MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
        uint32 size = 0;
        FILEARRAY   videofile_array = MMIAPIFILEARRAY_Create();
        int dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
        dev_path = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);
        
 
       // MMIAPIFMM_SearchFileStop();
        GetAllPicAndVideoSuffix(&file_filter);
        MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, CAMERA_ROLL_SOURCE_VIDEO,&default_path);
        MMIAPIFMM_SearchFileInPathAndSort(default_path.path,
                    default_path.path_len,
                    &file_filter,
                    FALSE,
                    FUNC_FIND_FILE,
                    videofile_array,
                    win_id,
                    MSG_PICVIEWER_LOAD_FINISH,
                    FILEARRAY_SORT_TIME_CREATE_DESCEND);
        MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetAllPicArray());
        MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),videofile_array);
*/
        GetAllPicAndVideoSuffix(&file_filter);
        //if(!MMIPICVIEW_SearchAllCaptured(win_id, &file_filter, &s_path_type))
        if(0==MMIPICVIEW_SearchAllCaptured(win_id, &file_filter, &s_path_type,&s_searching_device)){
            int32 len = MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetAllPicArray());
            
            s_path_type = CAMERA_ROLL_SOURCE_CAPTURED;
            MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(), FILEARRAY_SORT_TIME_CREATE_DESCEND);
            MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
        }
        break;
        //add end
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : start load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPic(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    BOOLEAN                     is_recurse = FALSE;
#endif
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter = {0};
//    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    FILEARRAY_DATA_T file_info_ptr={0}; 
    wchar                   abs_default_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
    uint16                  abs_default_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    PIC_SOURCE_TYPE type =CAMERA_ROLL_SOURCE_NONE;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};//zhuyongjing add for gallary ui 20150707
#if defined(CMCC_UI_STYLE)
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
#endif
        
    //get all picture file suffix
  //  GetAllPicSuffix(&file_filter);//zhuyongjing delete for gallary ui 20150707

//#if defined(CMCC_UI_STYLE)
//    if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
//    {
//        default_path.path_len = MMIFMM_PATHNAME_LEN;
//
//#if defined(DL_SUPPORT)
//        MMIAPIDL_GetDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, path_ptr, &path_len);
//#else
//        GetDefaultPicPath(&default_path);
//#endif
//    }
//    else
// #endif

    MMIPICVIEW_SetViewType(MMIPICVIEW_VIEW_ICONLIST);//zhuyongjing add for empty view
    if (MMIAPIDC_IsOpened())
    {
        //MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_CONTINUE, PNULL);
        MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_VIDEO_CONTINUE, PNULL);//zhuyongjing change for show video and photos
        
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    else
    {
        MMIAPIPICVIEW_GetCurPicFileInfo(TRUE,&file_info_ptr);
        //获取当前文件夹
        MMIAPIFMM_SplitFullPathExt(file_info_ptr.filename,MMIAPICOM_Wstrlen(file_info_ptr.filename),abs_default_path, &abs_default_path_length, PNULL, PNULL, PNULL, PNULL);
        //分离出的路径最后一个\.字符  
        if(MMIFILE_SLASH==abs_default_path[abs_default_path_length-1])
        {
            abs_default_path[abs_default_path_length-1]=0;
            abs_default_path_length--;
		}
        path_ptr =abs_default_path;
        path_len =abs_default_path_length;
        
        MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
                                path_len,
                                &file_filter,
                                is_recurse,
                                FUNC_FIND_FILE,
                                MMIAPIPICVIEW_GetAllPicArray(),
                                win_id,
                                MSG_PICVIEWER_LOAD_FINISH,
                                FILEARRAY_SORT_TIME_CREATE_DESCEND);
     }
#else
//zhuyongjing add for gallary ui 20150707
else{
    if(param != PNULL){
            type = *(PIC_SOURCE_TYPE*)param;
    }else{
            type =CAMERA_ROLL_SOURCE_NONE;
        }
        SCI_TRACE_LOW("[MMIPICVIEW_list]  StartLoadPic type=%d",type);
        
    if((type == CAMERA_ROLL_SOURCE_ALL_LOCAL)||(type ==CAMERA_ROLL_SOURCE_NONE)){
            path_ptr = PNULL;
            if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive() 
                ||(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB)
            ||(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS)){
                GetAllPicSuffix(&file_filter);
        }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
                GetAllPicWithoutGifSuffix(&file_filter);
        }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                GetAllVideoSuffix(&file_filter);
        }else{
                GetAllPicAndVideoSuffix(&file_filter);
            }
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            MMIPICVIEW_DEFAULT_PATH_T   video_path = {0};
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);     
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd not exist!");
                
            }
        if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive()){
                GetAllPicSuffix(&file_filter);
                
                MMIAPIFMM_SearchFileInPathAndSort(default_path.path,
                    default_path.path_len,
                    &file_filter,
                    TRUE,
                    FUNC_FIND_FILE,
                    MMIAPIPICVIEW_GetAllPicArray(),
                    win_id,
                    MSG_PICVIEWER_LOAD_FINISH,
                    FILEARRAY_SORT_TIME_CREATE_DESCEND);
                
        }else{
                
                MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_VIDEO_CONTINUE, PNULL);
            }
            result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            return (result); 
    }else if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                path_ptr = default_path.path;
                path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd--download not exist!");
            }
            GetAllPicSuffix(&file_filter);
            //dont serch child folder
            MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
                path_len,
                &file_filter,
                FALSE,
                FUNC_FIND_FILE,
                MMIAPIPICVIEW_GetAllPicArray(),
                win_id,
                MSG_PICVIEWER_LOAD_FINISH,
                FILEARRAY_SORT_TIME_MODIFY_ASCEND);
            
            result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            return (result);  
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                // path_ptr = default_path.path;
                //  path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd -video not exist!");
            }
            path_ptr = PNULL;
            GetAllVideoSuffix(&file_filter);
    }else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                path_ptr = default_path.path;
                path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd --onedrive not exist!");
            }
            GetAllPicSuffix(&file_filter);
        }
        
        //add end
        MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
            path_len,
            &file_filter,
            TRUE,
            FUNC_FIND_FILE,
            MMIAPIPICVIEW_GetAllPicArray(),
            win_id,
            MSG_PICVIEWER_LOAD_FINISH,
            FILEARRAY_SORT_TIME_MODIFY_ASCEND);
    }
#endif
//set softkey
//    MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);

    result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

    return (result);
}
//zhuyongjing add for gallary UI 
/*****************************************************************************/
//  Description : start load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPicForSpinner(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{

    BOOLEAN                     is_recurse = TRUE;
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter = {0};
//    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    FILEARRAY_DATA_T file_info_ptr={0}; 
    wchar                   abs_default_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
    uint16                  abs_default_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    PIC_SOURCE_TYPE type =CAMERA_ROLL_SOURCE_NONE;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};//zhuyongjing add for gallary ui 20150707
#if defined(CMCC_UI_STYLE)
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
#endif

    //get all picture file suffix
  //  GetAllPicSuffix(&file_filter);//zhuyongjing delete for gallary ui 20150707

//#if defined(CMCC_UI_STYLE)
//    if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
//    {
//        default_path.path_len = MMIFMM_PATHNAME_LEN;
//
//#if defined(DL_SUPPORT)
//        MMIAPIDL_GetDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, path_ptr, &path_len);
//#else
//        GetDefaultPicPath(&default_path);
//#endif
//    }
//    else
// #endif

    MMIPICVIEW_SetViewType(MMIPICVIEW_VIEW_ICONLIST);//zhuyongjing add for empty view
    
    if (MMIAPIDC_IsOpened())
    {
        MMIPICVIEW_DestroyAllPicArray();
        MMIPICVIEW_CreateAllPicArray();
        //MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_CONTINUE, PNULL);
        MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_VIDEO_CONTINUE, PNULL);//zhuyongjing change for show video and photos
        
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    else
    {
        MMIAPIPICVIEW_GetCurPicFileInfo(TRUE,&file_info_ptr);
        //获取当前文件夹
        MMIAPIFMM_SplitFullPathExt(file_info_ptr.filename,MMIAPICOM_Wstrlen(file_info_ptr.filename),abs_default_path, &abs_default_path_length, PNULL, PNULL, PNULL, PNULL);
        //分离出的路径最后一个\.字符  
        if(MMIFILE_SLASH==abs_default_path[abs_default_path_length-1])
        {
            abs_default_path[abs_default_path_length-1]=0;
            abs_default_path_length--;
		}
        path_ptr =abs_default_path;
        path_len =abs_default_path_length;
        
        MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
                                path_len,
                                &file_filter,
                                is_recurse,
                                FUNC_FIND_FILE,
                                MMIAPIPICVIEW_GetAllPicArray(),
                                win_id,
                                MSG_PICVIEWER_LOAD_FINISH,
                                FILEARRAY_SORT_TIME_CREATE_DESCEND);
     }
#else
//zhuyongjing add for gallary ui 20150707
else{
    if(param != PNULL){
            type = *(PIC_SOURCE_TYPE*)param;
    }else{
            type =CAMERA_ROLL_SOURCE_NONE;
        }
        SCI_TRACE_LOW("[MMIPICVIEW_list]  StartLoadPicForSpinner type=%d",type);
    if((type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY||type == CAMERA_ROLL_SOURCE_NONE)){
            MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(FALSE);
        }
        
    if((type == CAMERA_ROLL_SOURCE_ALL_LOCAL)||(type ==CAMERA_ROLL_SOURCE_NONE)){
            path_ptr = PNULL;
            if(MMIPICVIEW_GetMultiChoiseSaveToOneDrive() 
                ||(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB)
            ||(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS)){
                GetAllPicSuffix(&file_filter);
        }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER){
                GetAllPicWithoutGifSuffix(&file_filter);
        }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                GetAllVideoSuffix(&file_filter);
        }else{
                GetAllPicAndVideoSuffix(&file_filter);
            }
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            MMIPICVIEW_DEFAULT_PATH_T   video_path = {0};
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);     
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd not exist!");

            }
            // MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_VIDEO_CONTINUE, PNULL);
            
            // return (result); 
    }else if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                path_ptr = default_path.path;
                path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd--download not exist!");
            }
            GetAllPicSuffix(&file_filter);
            //dont serch child folder
            is_recurse = FALSE;
            
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                // path_ptr = default_path.path;
                //  path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd -video not exist!");
            }
            path_ptr = PNULL;
            GetAllVideoSuffix(&file_filter);
    }else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
            const wchar* dev_path = PNULL;
            int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
            dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());
            
            if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
            {
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
                path_ptr = default_path.path;
                path_len = default_path.path_len;
        }else{
                SCI_TRACE_LOW("MMIPICVIEW error sd --onedrive not exist!");
            }
            GetAllPicSuffix(&file_filter);
        }
        
        //add end
        if(type == CAMERA_ROLL_SOURCE_NONE){
#ifdef CAMERAROLL_SUPPORT					
            if(MMISRV_CAMERAROLL_Get_Connect_State()
                && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
        ){
                MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(FALSE);
        }else{
                MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
            }
#else
			MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
#endif				
            MMIPICVIEW_DestroyAllPicArray();
            MMIPICVIEW_CreateAllPicArray();

#if 1//modify by qin.he for only search specially dir not all dir
         {
            MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
            wchar      **abs_path_array = NULL;
            uint8       i = 0, j = 0;
            wchar   *mmimultim_picture_dir_array[MMIMULTIM_PIC_DIR_NUM] = {MMIMULTIM_DIR_PICTURE, MMIMULTIM_DIR_REVFILE, NULL};
            uint16      path_array_len = 0;
            abs_path_array = SCI_MallocApp(4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM, _D_FILE_NAME, _D_FILE_LINE);
            if (abs_path_array == NULL)
            {
                SCI_TRACE_LOW("[qin.he %s:%d] malloc fail \n",__FUNCTION__,__LINE__);
                MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
                return FALSE;  
            }

            SCI_MEMSET(abs_path_array,0x00,4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM);
            SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p, filter_str: %s \n",__FUNCTION__,__LINE__,abs_path_array,file_filter.filter_str);

            for (device = MMI_DEVICE_UDISK, j= 0; device < MMI_DEVICE_NUM; device++,j++)
            {
                if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
                {
                    for (i=0; i< MMIMULTIM_PIC_DIR_NUM && abs_path_array != NULL; i++)
                    {
                        abs_path_array[path_array_len] = SCI_MallocApp((MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2,_D_FILE_NAME,_D_FILE_LINE);
                        if (abs_path_array[path_array_len])
                        {
                            SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p \n",__FUNCTION__,__LINE__,abs_path_array[path_array_len]);
                            SCI_MEMSET(abs_path_array[path_array_len],0x00,(MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2);
                            if (mmimultim_picture_dir_array[i])
                            {
                                abs_path_array[path_array_len][0] = TRUE;
                                abs_path_array[path_array_len][1] = MMIFMM_FILE_FILE_NAME_MAX_LEN;
                                if (MMIAPIFMM_CombineFullPath(
                                    MMIAPIFMM_GetDevicePath(device), 
                                    MMIAPIFMM_GetDevicePathLen(device),
                                    mmimultim_picture_dir_array[i], 
                                    MMIAPICOM_Wstrlen(mmimultim_picture_dir_array[i]),
                                    PNULL, 
                                    PNULL,
                                    &(abs_path_array[path_array_len][2]),
                                    &(abs_path_array[path_array_len][1])
                                    ))
                                {
                                    uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                                    MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                                    SCI_TRACE_LOW("[qin.he %s:%d]index:%d,  path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                                    path_array_len++;
                                }
                                else
                                {
                                    SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array[path_array_len]: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array[path_array_len]);
                                    SCI_FREE (abs_path_array[path_array_len]);
                                    abs_path_array[path_array_len] = NULL;
                                }
                            }
                            else
                            {
                                uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                                abs_path_array[path_array_len][0] = FALSE;
                                abs_path_array[path_array_len][1] = MMIAPIFMM_GetDevicePathLen(device);
                                MMI_WSTRNCPY (&(abs_path_array[path_array_len][2]), MMIFMM_FILE_FILE_NAME_MAX_LEN, MMIAPIFMM_GetDevicePath(device), abs_path_array[path_array_len][1], abs_path_array[path_array_len][1]);
                                abs_path_array[path_array_len][2+(abs_path_array[path_array_len][1])] = MMIFILE_COLON;
                                abs_path_array[path_array_len][1] += 1;
                                MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                                SCI_TRACE_LOW("[qin.he %s:%d]index:%d,   path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                                path_array_len++;
                            }
                        }
                    }
                } 
            }

            if (path_array_len ==  0)
            {
                SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array);
                SCI_FREE (abs_path_array);
                MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
                return FALSE;  
            }
            else
            {
                MMIAPIFMM_SearchFileInPathsAndSort(
                                    abs_path_array, 
                                    path_array_len,
                                    &file_filter,
                                    TRUE,
                                    FUNC_FIND_FILE,
                                    MMIAPIPICVIEW_GetAllPicArray(),
                                    win_id,
                                    MSG_PICVIEWER_LOAD_FINISH,
                                    FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
            }
        }
#else
        MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
            path_len,
            &file_filter,
            is_recurse,
            FUNC_FIND_FILE,
            MMIAPIPICVIEW_GetAllPicArray(),
            win_id,
            MSG_PICVIEWER_LOAD_FINISH,
            FILEARRAY_SORT_TIME_MODIFY_DESCEND);
#endif
    }else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
			  FILEARRAY temparray = MMIAPIFILEARRAY_Create();
            MMIPICVIEW_GetFileArrayFromOtherForArray(temparray,MMIAPIPICVIEW_GetAllPicArray());
            
            //when goto onedrive only  just keep all local=backup
            if(!MMIAPIPICVIEW_GetPreviousFolderIsSubfolder()){
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_BACKUP_FILEARRAY,MMIAPIPICVIEW_GetAllPicArray());
                
                MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),type);
        }else{
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
                MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),type);
            }
            
            MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
            MMIAPIFILEARRAY_Destroy(&temparray);
            MMIPIC_CreateOneDriveSyncProgressDialogWin();
#endif            
        }
    else{
            if(!MMIAPIPICVIEW_GetPreviousFolderIsSubfolder()){//from main view to subview
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_BACKUP_FILEARRAY,MMIAPIPICVIEW_GetAllPicArray());
                
        }else{//from subview1 to subview2 when subview1 is empty
                
                MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            }
            //MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(),FILEARRAY_SORT_TIME_MODIFY_DESCEND);
            MMIAPIPICVIEW_FilterPicArrayByType(MMIAPIPICVIEW_GetAllPicArray(),type);
            MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
            
        }
}
#endif

    return (result);
}
//zhuyongjing add for gallary UI 
#ifdef CAMERAROLL_SUPPORT
PUBLIC void MMIPICVIEW_SetIfDownloadThumbnail(BOOLEAN value){
    is_need_download_thumbnails = value;
}
#endif
PUBLIC uint32 MMIPICVIEW_GetAllPicVideoNum(void){
    return all_pic_video_num;
}
//zhuyongjing add 
PUBLIC void MMIPICVIEW_InsertPicArray(const wchar *filename){
    
    if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
        MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),filename);
        // MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(),FILEARRAY_SORT_TIME_MODIFY_DESCEND);
    }else{
        MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),filename);
        MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetBackupAllPicArray(),filename);
        //  MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(),FILEARRAY_SORT_TIME_MODIFY_DESCEND);
    }
}
PUBLIC void MMIPICVIEW_RemoveIastFromArray(void){
    if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetAllPicArray(),MMIPICVIEW_GetAllPicNum()-1);
    }else{
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetAllPicArray(),MMIPICVIEW_GetAllPicNum()-1);
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetBackupAllPicArray(),MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetBackupAllPicArray())-1);
    }
}

#if 0  //fix bug732092
PUBLIC void MMIPICVIEW_RemovePicFromArray(uint32 index){
    if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE){
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetAllPicArray(),index);
    }else{
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetAllPicArray(),index);
        MMIAPIFILEARRAY_Delete(MMIAPIPICVIEW_GetBackupAllPicArray(),index);
    }
}
#endif
/*****************************************************************************/
// 	Description : get current photo index accprding to filename
//	Global resource dependence : none
//  Author: zhu.yongjing
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetPhotoIndexAccordingName(const wchar *fullpathname)
{
    uint16              file_name_len = 0;
    uint32              photo_index = 0;
    uint32              pic_num =MMIAPIFILEARRAY_GetArraySize(MMIAPIPICVIEW_GetCurPicArray());
    FILEARRAY_DATA_T    file_info = {0};

    if (PNULL == fullpathname)
    {
        SCI_TRACE_LOW("MMIPICVIEW_GetPhotoIndexAccordingInfo fullpathname=null!");
        return 0;
    }
    file_name_len = MMIAPICOM_Wstrlen(fullpathname);
    for (photo_index = 0; photo_index < pic_num; photo_index ++)
    {
        if(!MMIAPIFILEARRAY_Read(MMIAPIPICVIEW_GetCurPicArray(),photo_index, &file_info))
        {
            break;
        }
        if(0 == MMIAPICOM_CompareTwoWstr(file_info.filename, file_info.name_len, fullpathname, file_name_len))
        {
            break;
        }
    }

    if(photo_index == pic_num)
    {
        photo_index = 0;
    }

    return (photo_index);
}
/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendOnlyPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN needgif
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};
    MMI_STRING_T  time_string = {0};
    uint8  temp_time_str[20] = {0};
    wchar  temp_time_wstr[20]= {0};
    ONE_DRIVE_AUTO_UPLOAD_STATE_E upload_status = ONE_DRIVE_AUTO_UPLOAD_NONE;
    GUIANIM_DATA_INFO_T datainfo   = {0};
    uint8  pic_index_start_index = 0;

    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        pic_index_start_index = 1;
        if(0 == index)
        {
            datainfo.img_id = IMAGE_WALLPAPER_THUMB_1 + 3;
            GUIICONLIST_AppendIcon(0, ctrl_id, &datainfo, PNULL);
            return TRUE;
        } 
    }

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(index - pic_index_start_index, &file_info))
    {
#ifdef DRM_SUPPORT
        GUIANIM_DATA_INFO_T data_info = {0};

        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);
        }
        else
#endif
        {
            //zhuyongjing add for search quickly, for previous filearray
            while(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index - pic_index_start_index);
                if(!MMIPICVIEW_GetPicFileInfo(index - pic_index_start_index, &file_info)){
                    return result;
                }
            }
            //add end
            //zhuyongjing add for search quickly, for delete video, or gif if need, or thumbnails
            while(MMIPICVIEW_IsVideoSuffix(&file_info)
                ||(!needgif && MMIPICVIEW_IsGifSuffix(&file_info))
                ||ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index - pic_index_start_index);
                if(!MMIPICVIEW_GetPicFileInfo(index - pic_index_start_index, &file_info)){
                    return result;
                }
            }

            //set file name
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;
            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, FALSE);

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
        }

        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIconForMMS(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};
    MMI_STRING_T  time_string = {0};
    uint8  temp_time_str[20] = {0};
    wchar  temp_time_wstr[20]= {0};
    GUIANIM_DATA_INFO_T datainfo   = {0};
#if 0
    if(index == 0){
        datainfo.img_id = IMAGE_IDLE_SCREEN_SHORTCUT_CAMERA;
        GUIICONLIST_AppendIcon(0, ctrl_id, &datainfo, PNULL);
        return TRUE;
    } 
#endif
    //get picture info
    if ((MMIPICVIEW_GetPicFileInfo(index, &file_info)) && (index < MMIPICVIEW_GetAllPicNum()))
    {
#ifdef DRM_SUPPORT
        GUIANIM_DATA_INFO_T data_info = {0};

        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);
        }
        else
#endif
        {
          //zhuyongjing add for search quickly, for previous filearray
            while(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index);
                if(!MMIPICVIEW_GetPicFileInfo(index, &file_info)){
                    return result;
                }
            }
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS){
                while(MMIPICVIEW_IsVideoSuffix(&file_info) ||ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)){
                    MMIPICVIEW_RemoveItem(ctrl_id,index);
                    MMIPICVIEW_DeletePicFileInfo(index);
                    if(!MMIPICVIEW_GetPicFileInfo(index, &file_info)){
                        return result;
                    }
                }
            }
            else{
                while(!MMIPICVIEW_IsVideoSuffix(&file_info)){
                    MMIPICVIEW_RemoveItem(ctrl_id,index);
                    MMIPICVIEW_DeletePicFileInfo(index);
                    if(!MMIPICVIEW_GetPicFileInfo(index, &file_info)){
                        return result;
                    }
                }
            }
            //add end
            //set file name
                    if(MMIPICVIEW_IsVideoSuffix(&file_info)){
#ifdef  VIDEOTHUMBNAIL_SUPPORT
            wchar  thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
            uint16  thumbnail_name_len = 0;
            wchar  totaltime_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
            uint16  total_name_len = 0;
            //get duration 
            MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_name, &thumbnail_name_len, file_info.filename);
            MMIFMM_FmmVideoTotaltimeGetPathname(totaltime_name,&total_name_len,file_info.filename);
           if (!MMIAPIFMM_IsFileExist (thumbnail_name, thumbnail_name_len)
                ||!MMIAPIFMM_IsFileExist (totaltime_name, total_name_len))
            {
                GUIANIM_DATA_INFO_T data_info   = {0};
            DPLAYER_MEDIA_INFO_T media_info = {0};
                MMIFMM_VideoThumbnailAdd (file_info.filename);                    
                data_info.img_id = 0;//IMAGE_ONEDRIVE_DEFAULT_PIC;
                GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);             
                result = TRUE;
                return (result);
            }else{
                uint32 media_length = 0;
                MMIFMM_FmmGetVideoTotaltimeByPathname(&media_length ,totaltime_name);
                SCI_TRACE_LOW("AppendPicIconListIcon   media_length=%d,index=%d",media_length,index);
                SCI_MEMSET(temp_time_str, 0, 20*sizeof(uint8));
                SCI_MEMSET(temp_time_wstr, 0, 20*sizeof(wchar));
                if((media_length) / (1000*3600) > 0){
                    sprintf ( (char*) temp_time_str, "%02d:%02d:%02d", ( (media_length) / (1000*3600)), ( ( (media_length) / (1000*60)) % 60), ( ( (media_length) / 1000) % 60));
            }else{
                    sprintf ( (char*) temp_time_str, "%02d:%02d", ( ( (media_length) / (1000*60)) % 60), ( ( (media_length) / 1000) % 60));
            }
            //set time stamp

            MMIAPICOM_StrToWstr((const char *)temp_time_str, temp_time_wstr);
            SCI_MEMSET(&time_string, 0, sizeof(MMI_STRING_T));
            time_string.wstr_ptr = temp_time_wstr;
            time_string.wstr_len = MMIAPICOM_Wstrlen(temp_time_wstr);

            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, TRUE);
            GUIICONLIST_SetTimeStr(ctrl_id, index, time_string);

            //change filename to video thumnail filename
               // s_thumbnail_index = index;
                 //set file name
                icon_file.full_path_wstr_ptr = thumbnail_name;
                icon_file.full_path_wstr_len = thumbnail_name_len;
                GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
                result = TRUE;
                return (result);
            }
#endif
        }else{
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;
            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, FALSE);
        }
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);

        }
        }

        result = TRUE;    

    return result;
}

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIconForPB(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};
    MMI_STRING_T  time_string = {0};
    uint8  temp_time_str[20] = {0};
    wchar  temp_time_wstr[20]= {0};
    GUIANIM_DATA_INFO_T datainfo   = {0};
    uint8  pic_index_start_index = 2;
    
    if(index == 0)
    {
        datainfo.img_id = IMAGE_COMMON_CANCEL;
        GUIICONLIST_AppendIcon(0, ctrl_id, &datainfo, PNULL);
        return TRUE;
    }
#if defined (MAINLCD_SIZE_240X320)
    if(FALSE == MMIAPISET_IsDisableShareViaOption())
    {
        if(1 == index)
        {
            datainfo.img_id = 0;//IMAGE_IDLE_SCREEN_SHORTCUT_CAMERA;
            GUIICONLIST_AppendIcon(1, ctrl_id, &datainfo, PNULL);
            return TRUE;
        } 
    }
    else
#endif
    {
        pic_index_start_index = 1;
    }
    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(index-pic_index_start_index, &file_info))
    {
#ifdef DRM_SUPPORT
        GUIANIM_DATA_INFO_T data_info = {0};

        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);
        }
        else
#endif
        {
            //zhuyongjing add for search quickly, for previous filearray
            while(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index-pic_index_start_index);
                if(!MMIPICVIEW_GetPicFileInfo(index-pic_index_start_index, &file_info)){
                    return result;
                }
            }
            while(MMIPICVIEW_IsVideoSuffix(&file_info)||ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index-pic_index_start_index);
                if(!MMIPICVIEW_GetPicFileInfo(index-pic_index_start_index, &file_info)){
                    return result;
                }
            }    
            //add end
            
            //set file name
            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, FALSE);
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
            
        }
    }
    
    result = TRUE;
    
    return result;
}

#ifdef CAMERAROLL_SUPPORT	
LOCAL void HandleUploadSelectPics(MMI_WIN_ID_T   win_id,MMI_CTRL_ID_T  list_ctrl_id,BOOLEAN succeed){
    uint16 upload_num = 0;
    uint32 cur_index =0;

    if(!succeed){

        MMIPICVIEW_DestroyAllUploadingPicArray();

        ModifyPicListViewForUploadItems(win_id,list_ctrl_id,FALSE);
        cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        s_mmipicview_multi_uploading_done = FALSE;
        MMIPICVIEW_PostUpdateMsg(cur_index);
        
    }else{     
        if(MMK_IsFocusWin(win_id)&&!MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)&&!MMIPICVIEW_GetListMarkable(list_ctrl_id)){    
            UpdateUploadingPrgboxCtrl(win_id);
            UpdateUploadingLabelCtrl(win_id);
        }
        upload_num = MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count();//MMIPICVIEW_GetAllUploadingPicNum();
        SCI_TRACE_LOW("[pic_view]HandleUploadSelectPics uploading_num = %d", upload_num);

        if (0 == upload_num)
        {
        //prompt success

            cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
            MMIPICVIEW_DestroyAllUploadingPicArray();
            
            if(!MMIPICVIEW_GetListMarkable(list_ctrl_id)&&MMK_IsFocusWin(win_id)&&!MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                ModifyPicListViewForUploadItems(win_id,list_ctrl_id,FALSE);
                GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
                s_mmipicview_multi_uploading_done = FALSE;
            }else if(!MMIPICVIEW_GetListMarkable(list_ctrl_id)){
                MMIAPIPICVIEW_ReloadFile();
            }else{
                s_mmipicview_multi_uploading_done = FALSE;
            }
        }
        if(MMK_IsFocusWin(win_id)&&!MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)&&!MMIPICVIEW_GetListMarkable(list_ctrl_id)){
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }

    }
}
LOCAL BOOLEAN UploadSelectPic(MMI_WIN_ID_T   win_id,MMI_CTRL_ID_T  list_ctrl_id){

    FILEARRAY_DATA_T    file_info = {0};
    BOOLEAN             result    = FALSE;
    SCI_TRACE_LOW("[pic_view]UploadSelectPic start_time = %d", SCI_GetTickCount());



    //get file info
    if (MMIPICVIEW_GetCurUploadingPicFileInfo(&file_info))
    {
        //transport single file
        if(MMISRV_CAMERAROLL_Upload_single_photo(file_info.filename,file_info.name_len))
        {
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("[pic_view]UploadSelectPic end_time = %d", SCI_GetTickCount());
    
    return result;
}
LOCAL void HightlightUploadPrgShow(MMI_WIN_ID_T   win_id, BOOLEAN ishighlight){

    GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
    GUI_RECT_T                      icon_rect   = MMITHEME_GetClientRectEx(win_id);
    uint16           logic_width = 0;
    uint16           logic_height = 0;

    GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr.lcd_id, &logic_width, &logic_height);   
    icon_rect.bottom = icon_rect.top +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN+ MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT +MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT +MMIPICPREVIEW_UPLOADING_PRG_BOTTOM_MARGIN;

    if(ishighlight){
        s_mmipicview_multi_uploading_highlight = TRUE;
    }else{
        s_mmipicview_multi_uploading_highlight = FALSE;
    }
    UpdateUploadingPrgboxCtrl(win_id);
    UpdateUploadingLabelCtrl(win_id);
}
LOCAL void UpdateUploadingLabelCtrl( MMI_WIN_ID_T   win_id){
    MMI_STRING_T        title_txt   = {0};
    GUISTR_STYLE_T      txt_style   = {0};
    uint16 uploaded_num = 0;
    uint16 total_num = s_mmipicview_save_items_marked_num +s_mmipicview_last_total;
    MMI_CTRL_ID_T  ctrl_id = MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID;
    uint32 uploading_num = 1;
    MMI_STRING_T    src_str = {0};
    char num_buf1[10] = {0};
    char num_buf2[10] = {0};
    wchar wnum_buf1[10] = {0};
    wchar wnum_buf2[10] = {0};
    wchar src_wchar[100] = {0};
    // char uploading_str[100] = {0};
    
    uploading_num = MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count();//MMIAPIFILEARRAY_GetArraySize(MMIPICVIEW_GetAllUploadingPicArray()/*wenny api:get uploading array*/);
    
    //draw uploading lable: Uploading 1/4...
    uploaded_num = ((total_num > uploading_num) ? (total_num - uploading_num) : 0);
    
    SCI_TRACE_LOW("[mmipic_view]UpdateUploadingLabelCtrl uploading_num = %d,total_num=%d", uploading_num,total_num);
    title_txt.wstr_ptr = src_wchar;
    MMIAPIPICVIEW_ReplaceTextIdByTwoNum(STR_GAL_UPLOADING_NUMBER_EXT01,uploaded_num,total_num,&title_txt);
    GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,FALSE);
    if(s_mmipicview_multi_uploading_highlight){
        GUILABEL_SetBackgroundColor(ctrl_id, MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));
    }else{
        GUILABEL_SetBackgroundColor(ctrl_id, MMI_BACKGROUND_COLOR);
    }
    // GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_INVISIBLE,FALSE);
    GUILABEL_SetText(ctrl_id, &title_txt, FALSE);
    GUILABEL_SetVisible(ctrl_id,TRUE,TRUE);

}

LOCAL void UpdateUploadingPrgboxCtrl( MMI_WIN_ID_T   win_id){
    uint16 total_num = s_mmipicview_save_items_marked_num + s_mmipicview_last_total;
    MMI_CTRL_ID_T  ctrl_id = MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID;
    uint32 uploading_num = 1;
    uint16 uploaded_num = 0;
    GUI_LCD_DEV_INFO lcd_dev_info_ptr = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   
    GUI_RECT_T                      icon_rect   = MMITHEME_GetClientRectEx(win_id);
        
    //draw uploading progress bar
    uploading_num =  MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count(); //MMIAPIFILEARRAY_GetArraySize(MMIPICVIEW_GetAllUploadingPicArray()/*wenny api:get uploading array*/);

    uploaded_num = ((total_num > uploading_num) ? (total_num - uploading_num ) : 0);
    //CTRLBASE_SetVisible(ctrl_id,TRUE);
    GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,FALSE);
    GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_INVISIBLE,FALSE);
    if(!MMK_IsActiveCtrl(ctrl_id)){
        MMK_SetAtvCtrlEx(win_id,ctrl_id,TRUE);
    }
    icon_rect.bottom = icon_rect.top +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN+ MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT +MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT +MMIPICPREVIEW_UPLOADING_PRG_BOTTOM_MARGIN;

    if(s_mmipicview_multi_uploading_highlight){
        GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info_ptr,icon_rect, MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT));
        CTRLPROGRESSBAR_SetIsRectBarInDialog(ctrl_id, TRUE);
    }else{
        GUI_FillRect((const GUI_LCD_DEV_INFO*) &lcd_dev_info_ptr,icon_rect, MMI_BACKGROUND_COLOR);
        CTRLPROGRESSBAR_SetIsRectBarInDialog(ctrl_id, FALSE);
    }
    GUIPRGBOX_SetParamByCtrl(ctrl_id, total_num, 1);
    GUIPRGBOX_SetPosByCtrl(TRUE, uploaded_num, 0, ctrl_id);
}

LOCAL void CreatUploadingLabelCtrl( MMI_WIN_ID_T   win_id){
    MMI_CONTROL_CREATE_T    create1 = {0};
    GUILABEL_INIT_DATA_T label_init_data = {0};
    MMI_STRING_T        title_txt   = {0};
    GUISTR_STYLE_T      txt_style   = {0};
    uint16 uploaded_num = 0;
    uint16 total_num = 0;
    GUI_BOTH_RECT_T         label_rect = MMIPICVIEW_GetClientWinRect(win_id);
    MMI_CTRL_ID_T  ctrl_id = MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID;
    GUI_BG_T            bg          = {0};
    wchar src_wchar[50] = {0};
    MMI_STRING_T    src_str = {0};
    char src_char[50] = {0};
    char uploading_str[50] = {0};
    
    total_num = MMIPICVIEW_GetSelectItemIndex(MMIPICVIEW_LIST_CTRL_ID, PNULL, 0);
    //draw uploading lable: Uploading 1/4...
    //sprintf(uploading_str,"%s%d%s%d%s", "Uploading ",uploaded_num, "/",total_num,"...");    
   // title_txt.wstr_ptr = src_wchar;
  // ReplaceNumByTextId(STR_GAL_UPLOADING_NUMBER_EXT01,L"%0N",L"%1N",uploaded_num,total_num,50,&title_txt);
    MMI_GetLabelTextByLang(STR_GAL_STATUS_UPLOADING_EXT01, &src_str);
    MMIAPICOM_WstrToStr(src_str.wstr_ptr,src_char);

    sprintf(uploading_str,"%s%s%d%s%d%s", src_char," ",uploaded_num, "/",total_num,"..."); 
    MMIAPICOM_StrToWstr(uploading_str,src_wchar);
    title_txt.wstr_ptr = src_wchar;
    title_txt.wstr_len = MMIAPICOM_Wstrlen(src_wchar);

    bg.bg_type  = GUI_BG_COLOR;
    bg.color = MMI_BACKGROUND_COLOR;
    label_rect.v_rect.bottom = label_rect.v_rect.top + MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN-1;
    label_rect.h_rect.bottom = label_rect.h_rect.top + MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN-1;
    label_rect.v_rect.left = label_rect.v_rect.left + MMIPICPREVIEW_UPLOADING_LABEL_HORIZION_MARGIN;
    label_rect.h_rect.left = label_rect.h_rect.left + MMIPICPREVIEW_UPLOADING_LABEL_HORIZION_MARGIN;

    create1.ctrl_id           = ctrl_id;
    create1.parent_win_handle = win_id;
    create1.guid          = SPRD_GUI_LABEL_ID;
    label_init_data.both_rect = label_rect;
    label_init_data.align = GUILABEL_ALIGN_LEFT;
    create1.init_data_ptr = &label_init_data;

    MMK_CreateControl(&create1);
    //MMK_SetAtvCtrl(win_id, MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID);
    
  //  GUILABEL_SetMarginEx(ctrl_id,MMIPICPREVIEW_UPLOADING_LABEL_HORIZION_MARGIN,MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN);
    GUILABEL_SetRect(ctrl_id, &label_rect.v_rect, FALSE);
    GUILABEL_SetBg(ctrl_id, &bg);
    GUILABEL_SetFont(ctrl_id, MMI_DEFAULT_SMALL_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetText(ctrl_id, &title_txt, FALSE);
   // GUILABEL_SetVisible(ctrl_id,FALSE,FALSE);
    GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);

}

LOCAL void CreatUploadingPrgboxCtrl( MMI_WIN_ID_T   win_id){
    GUIPRGBOX_INIT_DATA_T prg_init_data = {0};
    MMI_CONTROL_CREATE_T    create2 = {0};   
    GUI_BOTH_RECT_T         prg_rect = MMIPICVIEW_GetClientWinRect(win_id);
    uint16 total_num = 1;
    MMI_CTRL_ID_T  ctrl_id = MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID;

    //draw uploading progress bar
    
    prg_rect.v_rect.top = prg_rect.v_rect.top +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN+ MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT;
    prg_rect.v_rect.bottom = prg_rect.v_rect.top + MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT;
    prg_rect.v_rect.left = prg_rect.v_rect.left + MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HORIZION_MARGIN ;
    prg_rect.v_rect.right = prg_rect.v_rect.right - MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HORIZION_MARGIN;

    prg_rect.h_rect.top = prg_rect.h_rect.top +MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN +MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT;
    prg_rect.h_rect.bottom = prg_rect.h_rect.top + MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT ;
    prg_rect.h_rect.left = prg_rect.h_rect.left + MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HORIZION_MARGIN;
    prg_rect.h_rect.right = prg_rect.h_rect.right - MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HORIZION_MARGIN;
    
    create2.ctrl_id           = ctrl_id;
    create2.parent_win_handle = win_id;
    create2.guid          = SPRD_GUI_PRGBOX_ID;
    prg_init_data.both_rect = prg_rect;
    prg_init_data.is_forbid_paint = TRUE;
    prg_init_data.style = GUIPRGBOX_STYLE_HORIZONTAL_PROCESS;
    prg_init_data.theme_type = MMITHEME_PRGBOX_DEFAULT_TYPE;
    prg_init_data.lcd_id = MAIN_LCD_ID;
    create2.init_data_ptr = &prg_init_data;
    MMK_CreateControl(&create2);
    GUIPRGBOX_SetRect(ctrl_id,&prg_rect);
    GUIPRGBOX_SetParamByCtrl(ctrl_id, total_num, 1);
    GUIPRGBOX_SetPosByCtrl(TRUE, 0, 0, ctrl_id);
    //MMK_SetAtvCtrl(win_id, ctrl_id);

    GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
    CTRLPROGRESSBAR_SetIsInWaitingInd(ctrl_id, FALSE,FALSE);
}
PUBLIC void ModifyPicListViewForUploadItems(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id, //in:
    BOOLEAN value
)
{
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_ICONLIST;
    GUI_RECT_T                      icon_rect   = MMITHEME_GetClientRectEx(win_id);

    if(value){

        icon_rect.top += MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT +MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT +MMIPICPREVIEW_UPLOADING_PRG_BOTTOM_MARGIN*2;
        GUIICONLIST_SetRect(ctrl_id, &icon_rect);
        
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
        
        UpdateUploadingPrgboxCtrl(win_id);
        UpdateUploadingLabelCtrl(win_id);
        GUILABEL_SetBackgroundColor(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID, MMI_BACKGROUND_COLOR);
    }else{
        GUILABEL_SetVisible(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID,FALSE,TRUE);
        CTRLBASE_SetVisible(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID,FALSE);
        GUIICONLIST_SetRect(ctrl_id, &icon_rect);       
    }
}
#endif

/*****************************************************************************/
//  Description : handle picture preview delete msg
//  Global resource dependence : none
//  Author:zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMultiPicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_WIN_ID_T        list_win_id = MMIPICVIEW_LIST_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OK_MK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
    break;
    case MSG_APP_WEB: 
        MMK_SendMsg(list_win_id, MSG_PROMPTWIN_OK, PNULL);
        break;

    case MSG_APP_CANCEL:
        MMK_SendMsg(list_win_id, MSG_PROMPTWIN_CANCEL, PNULL);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
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
#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
LOCAL MMI_RESULT_E HandlePicSaveItemsToOnedriveQueryWin
(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    uint32              time_out = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        list_win_id = MMIPICVIEW_LIST_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_UPLOAD_ITEMS_QUERY_WIN_ID;
    uint32              item_index = 0;
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();
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
    case MSG_NOTIFY_MIDSK:
	#ifdef CAMERAROLL_SUPPORT
        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
        if(sim_ok_num == 0){
            MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());
            MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
            MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
            MMIPICVIEW_DestroyAllUploadingPicArray();
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIPICVIEW_PostUpdateMsg(item_index);
            MMIAPIPHONE_AlertSimNorOKStatus();
            break;
        }
//interface to set iconlist change state: marked-->uploading
        MMISRV_CAMERAROLL_CreateTask(MMI_MODULE_PIC_VIEWER , CAMERA_ROLL_TYPE_MANUAL_UPLOAD_MULTI_PHOTO);
      //  MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)item_index);
        MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
        if(s_mmipicview_multi_uploading_done 
            && ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == MMISRV_CAMERAROLL_Get_Auto_upload_status()){
            s_mmipicview_last_total = s_mmipicview_save_items_marked_num;//lastime multi uploading continue
        }else if(!s_mmipicview_multi_uploading_done
            &&ONE_DRIVE_AUTO_UPLOAD_MANUAL_UPLOADING == MMISRV_CAMERAROLL_Get_Auto_upload_status()
            && MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count()>0){
            s_mmipicview_last_total = MMISRV_CAMERAROLL_Get_Manual_Upload_Photo_Count();
        }else{
            s_mmipicview_last_total = 0;
        }



        s_mmipicview_save_items_marked_num = MMIPICVIEW_GetSelectItemIndex(MMIPICVIEW_LIST_CTRL_ID, PNULL, 0);
        SCI_TRACE_LOW("[MMIPIC]:HandlePicSaveItemsToOnedriveQueryWin  s_mmipicview_last_total=%d, this mark num=%d",s_mmipicview_last_total,s_mmipicview_save_items_marked_num);

       // MMIPICVIEW_DestroyAllPicArray();
      //  MMIPICVIEW_CreateAllPicArray();
        //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetBackupAllPicArray());
        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());

        ModifyPicListViewForUploadItems(list_win_id,list_ctrl_id,TRUE);
        MMIPICVIEW_SetListMarkable(list_ctrl_id, FALSE);
        s_mmipicview_multi_uploading_done = TRUE;
        MMISRV_CAMERAROLL_Set_Upload_multiple_photos(MMIPICVIEW_GetAllUploadingPicArray());
        MMIPUB_CloseQuerytWin(&query_win_id);
	#endif
        MMIPICVIEW_PostUpdateMsg(item_index);
        break;
    case MSG_CAMERA_ROLL_ERROR_NOTIFY:
#if defined(WIN32)
        break;
#endif
        if(MMISRV_CAMERAROLL_Get_Auto_upload_status()==ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK){

            if (s_mmipicview_multi_uploading_done)
            {
                s_mmipicview_multi_uploading_done = FALSE;
            }
           // MMIPICVIEW_DestroyAllPicArray();
           // MMIPICVIEW_CreateAllPicArray();
            //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetBackupAllPicArray());
            MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());

            MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
            MMIPICVIEW_DestroyAllUploadingPicArray();
            MMIPICVIEW_SetMarkable(list_ctrl_id,FALSE);
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMIPICVIEW_PostUpdateMsg(item_index);
        }
    break;
    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
       // MMIPICVIEW_DestroyAllPicArray();
     //   MMIPICVIEW_CreateAllPicArray();
        //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetAllPicArray(),MMIAPIPICVIEW_GetBackupAllPicArray());
        MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_CURRENT_FILEARRAY,MMIAPIPICVIEW_GetBackupAllPicArray());

        MMIPICVIEW_SetMultiChoiseSaveToOneDrive(FALSE);
        MMIPICVIEW_DestroyAllUploadingPicArray();
        MMIPICVIEW_SetMarkable(list_ctrl_id,FALSE);
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMIPICVIEW_PostUpdateMsg(item_index);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : get all picture and video files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GetAllPicAndVideoSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14,(uint8*)"");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIPICVIEW_PIC_VIDEO_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmipic_video_all_suffix[i], sizeof(s_mmipic_video_all_suffix[i]),
                   sizeof(s_mmipic_video_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_mmipic_video_all_suffix[i]); /*lint !e64*/
    }
}
/*****************************************************************************/
//  Description : get all jpg suffix for pb 20150826
//  Global resource dependence :
//  Author: zhuyongjing 
//  Note:
/*****************************************************************************/

LOCAL void GetAllJpgPicSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14,(uint8*)"");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIPICVIEW_PIC_EDIT_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmipic_edit_pic_all_suffix[i], sizeof(s_mmipic_edit_pic_all_suffix[i]),
                   sizeof(s_mmipic_edit_pic_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_mmipic_edit_pic_all_suffix[i]); /*lint !e64*/
    }
}
/*****************************************************************************/
//  Description : get all video files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GetAllVideoSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        SCI_TRACE_LOW("MMIPICVIEW GetAllVideoSuffix filter_ptr is PNULL !!!");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIPICVIEW_VIDEO_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmivideo_all_suffix[i], sizeof(s_mmivideo_all_suffix[i]),
                   sizeof(s_mmivideo_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_mmivideo_all_suffix[i]); /*lint !e64*/
    }
}
PUBLIC void GetAllPicWithoutGifSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;
    if (PNULL == filter_ptr)
    {
        SCI_TRACE_LOW("MMIPICVIEW GetAllPicWithoutGifSuffix filter_ptr is PNULL !!!");
        return ;
    }
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));
    for (i = 0; i < MMIPICVIEW_PIC_WITHOUT_GIF_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmipic_except_gif_suffix[i], sizeof(s_mmipic_except_gif_suffix[i]),
                   sizeof(s_mmipic_except_gif_suffix[i]));
        suffix_len = suffix_len + strlen((char *)s_mmipic_except_gif_suffix[i]); /*lint !e64*/
    }
}
PUBLIC void PicviewListAllEmptyShow( MMI_WIN_ID_T   win_id)
{

    MMI_CONTROL_CREATE_T    create = {0};
    //GUILABEL_INIT_DATA_T    init_data = {0};
  GUIRICHTEXT_INIT_DATA_T init_data = {0};
   //GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
  //  GUITEXT_INIT_DATA_T init_data = {0};
    GUILIST_INIT_DATA_T     list_init_data = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T string = {0}; 
    uint16    index= 0;
    MMI_CTRL_ID_T ctrl_id = MMIPICVIEW_LIST_EMPTY_CTRL_ID;
    GUI_BG_T        pic_bg = {0};
    GUI_COLOR_T     font_color = MMI_BLUE_COLOR;
    GUI_FONT_T      font=MMIPICPREVIEW_TITLE_FONT;
    GUILIST_EMPTY_INFO_T empty_info = {0};


    GUI_LCD_DEV_INFO    dev_info    = {0};
    MMI_STRING_T        str_data    = {0};
    GUISTR_STYLE_T      style_info  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_RECT_T          disp_rect   = {0};
    
    list_init_data.both_rect = MMIPICVIEW_GetClientWinRect(win_id);
    list_init_data.type      = GUILIST_TEXTLIST_E;
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_LIST_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &list_init_data;
    MMK_CreateControl(&create); 
    MMK_SetAtvCtrl(win_id, ctrl_id);
    SCI_TRACE_LOW("MMIPICVIEW  PicviewListAllEmptyShow");

    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
    if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
    }else{
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);

    }
    if(MMIAPIPICVIEW_GetShowFolderPath() != CAMERA_ROLL_SOURCE_NONE
        ||MMIAPIDC_IsOpened()){
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
    }else{
        if((MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS) ||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS) ||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_CAMERA)){
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
        }else{
        	#if 0//def CAMERA_SUPPORT//delete by James li
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CAMERA_MK, MIDDLE_BUTTON, FALSE);
			#else
				GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
			#endif
        }
    }
    empty_info.text_id = STR_PHOTO_EMPTY;//STR_PHOTO_EMPTY_HINT_MK;//change by James li
    GUILIST_SetEmptyInfo(ctrl_id, &empty_info);

}
/*****************************************************************************/
//  Description : camera button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CamerButtonCallBackEx(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    MMIVIRTUALARRAY_SetBufferSize(MMIAPIPICVIEW_GetAllPicArray(), 2);
	#if 0//def CAMERA_SUPPORT//delete by James li
    result = MMIAPIDC_OpenPhotoWin();
    if( TRUE == result )
    {
        MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
        s_is_go_to_camera = TRUE;
    }
//	s_is_need_to_reinit_picarray_from_piclib = TRUE;
#endif
    return result;
}
#ifdef  VIDEOTHUMBNAIL_SUPPORT
PUBLIC void MMIPICVIEW_VideoThumbnailOpenCallback(const wchar *filename,const wchar *video_filename,uint32 totaltime)
{
    MMI_CTRL_ID_T ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint32 index = 0;
    index = MMIPICVIEW_GetIndexInFileArray(video_filename,MMIAPIPICVIEW_GetAllPicArray());
    if(index == MMI_PICVIEW_UPLOADING_INVALID_INDEX){
        return;
    }
    if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
        index+=1;
    }
    SCI_TRACE_LOW("MMIPICVIEW_VideoThumbnailOpenCallback totaltime=%d,index=%d",totaltime,index);

    CTRLICONLIST_SetItemLoadIcon(ctrl_id,index,FALSE);
    CTRLICONLIST_DispalyItemIcon(ctrl_id,index,TRUE);
}
#endif
PUBLIC BOOLEAN MMIPICVIEW_IsVideoSuffix( FILEARRAY_DATA_T *fileinfo)
{
    uint16 suffix_len = MMIFMM_FILENAME_LEN;
    uint32 type =0;
    wchar list_suffix_name[MMIFMM_FILENAME_LEN+1] = {0};

    if (MMIAPIFMM_SplitFileName (fileinfo->filename, fileinfo->name_len, PNULL, PNULL, list_suffix_name, &suffix_len))
    {
        type = MMIAPIFMM_GetFileType (list_suffix_name, suffix_len);
        if((type!=MMIFMM_MOVIE_MP4)
            && (type!=MMIFMM_MOVIE_3GP)
            && (type!=MMIFMM_MOVIE_RMVB)
            && (type!=MMIFMM_MOVIE_AVI)
            && (type!=MMIFMM_MOVIE_FLV)){
            return FALSE;
        }
        return TRUE;

    } else{
        return FALSE;
    }
}
PUBLIC BOOLEAN MMIPICVIEW_IsGifSuffix( FILEARRAY_DATA_T *fileinfo)
{
    uint16 suffix_len = MMIFMM_FILENAME_LEN;
    uint32 type =0;
    wchar list_suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
    if (MMIAPIFMM_SplitFileName (fileinfo->filename, fileinfo->name_len, PNULL, PNULL, list_suffix_name, &suffix_len))
    {
        type = MMIAPIFMM_GetFileType (list_suffix_name, suffix_len);
        if(type!=MMIFMM_PICTURE_GIF){
            return FALSE;
        }
        return TRUE;
    } else{
        return FALSE;
    }
}
LOCAL BOOLEAN MMIPICVIEW_GetFileIsVideotoPlay(uint32 item_index){
    FILEARRAY_DATA_T    file_info = {0};
    MMI_STRING_T        note_info   = {0};

    if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info)){
        if(!MMIPICVIEW_IsVideoSuffix(&file_info)){
            return MMI_RESULT_FALSE;
        }
        if(0 == file_info.size)
        {
            MMI_GetLabelTextByLang(STR_FILE_EMPTY,&note_info); 
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&note_info,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
           // MMIAPIVP_PlayVideoFromVideoPlayer (VP_ENTRY_FROM_FILE/*VP_ENTRY_FROM_PLAYER*/, file_info.filename, file_info.name_len, PNULL);
        }
        return MMI_RESULT_TRUE;
    }else{
        return MMI_RESULT_FALSE;
    }
}

/*****************************************************************************/
//  Description : get all picture files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GetAllPicSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14,(uint8*)"");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIPICVIEW_PIC_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmipicview_all_suffix[i], sizeof(s_mmipicview_all_suffix[i]),
                   sizeof(s_mmipicview_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_mmipicview_all_suffix[i]); /*lint !e64*/
    }
}
/*****************************************************************************/
//  Description : refresh some picture files for second fresh
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
LOCAL void FreshSomeItemsForLoadPic(
                                    MMI_WIN_ID_T   win_id,
                                    MMI_CTRL_ID_T ctrl_id
                                    )
{
    uint16      cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32      pic_num = 0;
    uint16 i = 0;
    uint16 array_top_index = 0;
    uint16 array_bottom_index = 0;
    FILEARRAY_DATA_T    file_info_pre = {0};
    FILEARRAY_DATA_T    file_info_cur = {0};
    uint32      pre_pic_num = 0;
    uint16 top_index = CTRLICONLIST_TopIndex(ctrl_id);
    uint16 bottom_index = CTRLICONLIST_BottomIndex(ctrl_id);
    BOOLEAN need_redisplay = FALSE;
    uint16 pic_start_index = 2;
    
    pre_pic_num = MMIAPIFILEARRAY_GetArraySize(MMIPICVIEW_GetPreviousPicArray());
    pic_num = MMIPICVIEW_GetAllPicNum();
    if(pre_pic_num==0 ||pic_num == 0
        ||MMIPICVIEW_GetListEmpty()){/*when pic_num>pre_pic_num>0*/
        FinishLoadPic(win_id,TRUE);
        return;
    }
    
    if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            pic_start_index = 1;
        }

        if(CTRLICONLIST_TopIndex(ctrl_id) < pic_start_index){
            array_top_index = 0;
        }else{
            array_top_index = CTRLICONLIST_TopIndex(ctrl_id) - pic_start_index;
        }
        if(CTRLICONLIST_BottomIndex(ctrl_id) < pic_start_index){
            array_bottom_index = 0;
        }else{
            array_bottom_index = CTRLICONLIST_BottomIndex(ctrl_id) - pic_start_index;
        }
        CTRLICONLIST_ReallocIconMemory(ctrl_id, (pic_num + pic_start_index));
    }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS
          ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS){
        if(CTRLICONLIST_TopIndex(ctrl_id) < 1){
            array_top_index = 0;
        }else{
            array_top_index = CTRLICONLIST_TopIndex(ctrl_id) -1;
        }
        if(CTRLICONLIST_BottomIndex(ctrl_id) < 1){
            array_bottom_index = 0;
        }else{
            array_bottom_index = CTRLICONLIST_BottomIndex(ctrl_id) -1;
        }
        CTRLICONLIST_ReallocIconMemory(ctrl_id,pic_num);
    }else{
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            pic_start_index = 1;
            array_top_index = CTRLICONLIST_TopIndex(ctrl_id) - pic_start_index;
            if(CTRLICONLIST_BottomIndex(ctrl_id) < pic_start_index)
            {
                array_bottom_index = 0;
            }
            else
            {
                array_bottom_index = CTRLICONLIST_BottomIndex(ctrl_id) - pic_start_index;
            }
            CTRLICONLIST_ReallocIconMemory(ctrl_id,pic_num + pic_start_index);
        }
        else
        {
            pic_start_index = 0;
            array_top_index = CTRLICONLIST_TopIndex(ctrl_id);
            array_bottom_index = CTRLICONLIST_BottomIndex(ctrl_id);
            CTRLICONLIST_ReallocIconMemory(ctrl_id,pic_num);
        }
    }
    SCI_TRACE_LOW("[mmipic]FreshSomeItemsForLoadPic pre_pic_num=%d, pic_num=%d,top=%d,bottom=%d",pre_pic_num,pic_num,array_top_index,array_bottom_index);
    
    for (i=array_top_index; i<=array_bottom_index; i++)
    {
        if(!MMIAPIFILEARRAY_Read(MMIPICVIEW_GetPreviousPicArray(),i, &file_info_pre))
        {
            SCI_TRACE_LOW("[mmipic] FreshSomeItemsForLoadPic MMIPICVIEW_GetPreviousPicArray ERROR!");
            break;
        }
        if(!MMIAPIFILEARRAY_Read(MMIAPIPICVIEW_GetAllPicArray(),i, &file_info_cur))
        {
            SCI_TRACE_LOW("[mmipic] FreshSomeItemsForLoadPic MMIAPIPICVIEW_GetAllPicArray ERROR!");
            break;
        }
        if(0 != MMIAPICOM_Wstrcmp(file_info_cur.filename, file_info_pre.filename)){
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
                CTRLICONLIST_SetItemLoadIcon(ctrl_id,i+pic_start_index,FALSE);
            }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS
                ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS){
                CTRLICONLIST_SetItemLoadIcon(ctrl_id,i+1,FALSE);
            }else{
                CTRLICONLIST_SetItemLoadIcon(ctrl_id,i+pic_start_index,FALSE);
            }
            if(!need_redisplay){
                need_redisplay = TRUE;
            }
        }
    }
    
    if((array_bottom_index==0
        ||(pre_pic_num >= 1 && (pre_pic_num-1 ==array_bottom_index)))
        && (pre_pic_num < pic_num)){
        uint16 bottom_finish_index = 0;
        
        
        if(bottom_index-top_index >= 12){
            bottom_finish_index = MIN(bottom_index + pic_num -pre_pic_num,top_index + 15-1);
        }else{
            bottom_finish_index = MIN(bottom_index + pic_num -pre_pic_num,top_index + 12-1);
        }
        SCI_TRACE_LOW("[mmipic] FreshSomeItemsForLoadPic bottom_index =%d,bottom_finish_index=%d",bottom_index,bottom_finish_index);
        
        for (i=bottom_index+1;i<=bottom_finish_index;i++)
        {
            CTRLICONLIST_SetItemLoadIcon(ctrl_id,i,FALSE);
        }
        if(!need_redisplay && bottom_index+1<= bottom_finish_index){
            need_redisplay = TRUE;
        }
    }
    
    if(need_redisplay){
        CTRLICONLIST_DispalyItemIcon(ctrl_id,top_index,TRUE);
    }
    SCI_TRACE_LOW("[mmipic]FreshSomeItemsForLoadPic end");

}
/*****************************************************************************/
//  Description : finish load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void FinishLoadPic(
    MMI_WIN_ID_T   win_id,
    BOOLEAN flash_sync
)
{
    uint16      cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32      pic_num = 0;

    pic_num = MMIPICVIEW_GetAllPicNum();
    SCI_TRACE_LOW("[mmipic]FinishLoadPic pic_num=%d",pic_num);
    if((MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER) && (TRUE == MMIAPISET_IsDisableShareViaOption()))
    {
        pic_num +=1;
    }

    if (0 < pic_num)
    {
        if (MMIAPIDC_IsOpened())
        {
         //   cur_index = (uint16)MMIAPIDC_GetSavePhotoIndex(pic_num, MMIAPIPICVIEW_GetAllPicArray());
        }
            //zhuyongjing add for from empty folder to non-empty folder 
        if(MMIPICVIEW_GetListEmpty()){
            CreatePicListCtrl(MMIPICVIEW_LIST_WIN_ID, MMIPICVIEW_LIST_CTRL_ID);       
        }
         MMIPICVIEW_SetListEmpty(FALSE);
        //add end
	//zhuyongjing add for pb 20150826
    }else if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB
    /*||MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS
    ||MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS */){
	//add end
    }else{
#ifdef PIC_PREVIEW_U200_SUPPORT
        GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_SLIDE_PREVIEW, TRUE, PNULL, TRUE);
        GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_MARKED, TRUE, PNULL, TRUE);
#else
//zhuyongjing change for gallary ui 20150707
       // GUIWIN_SetSoftkeyBtnGray(MMIPICVIEW_LIST_WIN_ID, LEFT_BUTTON, TRUE);	
     //   GUIWIN_SetSoftkeyTextId(MMIPICVIEW_LIST_WIN_ID, TXT_NULL, STXT_SOFTKEY_OPEN_MK, TXT_NULL, FALSE);//zhuyongjing change
//change end
#endif
        MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID);
        MMIPICVIEW_SetListEmpty(TRUE);
        PicviewListAllEmptyShow(MMIPICVIEW_LIST_WIN_ID);
	}
    if(flash_sync){
        MMK_SendMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &cur_index);
    }else{
    //need update list
        MMIPICVIEW_PostUpdateMsg(cur_index);
    }
    if(win_id == MMIPICVIEW_WAIT_WIN_ID){
        MMK_CloseWin(win_id);
    }

#ifdef MMI_SHOW_MSA
    if (MMIAPIDC_IsOpened()&&MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_CAMERA
        &&!MMIPICVIEW_GetListEmpty()){
        MMIPICVIEW_SetCurIndex(MMIPICVIEW_LIST_CTRL_ID,(uint16)cur_index);
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
        }
        MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB,PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : post update list msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_PostUpdateMsg(
    uint16 index
)
{
    MMI_WIN_ID_T    win_id = MMIPICVIEW_LIST_WIN_ID;

    MMK_PostMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &index, sizeof(uint16));
}

/*****************************************************************************/
//  Description : clear pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearPicList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)
{
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1765_112_2_18_2_46_24_15,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
        break;
    }

    //set title
    //GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW, FALSE);

    //set softkey
    //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
}

/*****************************************************************************/
//  Description : update pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
)
{
    uint16                  i = 0;
    uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNum();
    GUILIST_ITEM_T          item = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();
    SCI_TRACE_LOW("MMIPICVIEW UpdatePicList    cur_index=%d",cur_index);
  //  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1809_112_2_18_2_46_24_16,(uint8*)"d", cur_index);

    //zhuyongjing add for pb 20150826
    if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB){
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            total_num += 1;//first none icon
        }
        else
        {
            total_num += 2;//first none icon,second camera icon
        }
    }
#if 0
    if(    MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_MMS
        || MMIPICVIEW_GetEnterType()==MMIPICVIEW_VIDEOS_FROM_MMS){
        total_num += 1;//first camera icon
    }
#endif
    //add end
    if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_WALLPAPER)
    {
        if(TRUE == MMIAPISET_IsDisableShareViaOption())
        {
            total_num += 1;//first none icon
        }
    }
    SCI_TRACE_LOW("MMIPICVIEW-UpdatePicList-total_num=%d", total_num);
    
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);

        //st title
        GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);

        if (0 < total_num)
        {
            //set title index
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

            //set item
            item.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
            for (i = 0; i < total_num; i++)
            {
                GUILIST_AppendItem(ctrl_id, &item);
            }

            //set current index
            GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        }
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //MMIAPIPICVIEW_DeleteOrderInTime(ctrl_id);
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, total_num);
        //set title
        if (0 == total_num)
        {
#ifndef MMI_GUI_STYLE_MINISCREEN
        //    GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);
#else
            /*MINI UI 需求*/
            GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW_TITLE, FALSE);
#endif
        }
        //set current index
        GUIICONLIST_SetCurIconIndex(cur_index, ctrl_id);
     //   LoadPicList(ctrl_id);//zhuyongjing delete 
       //SetlistOrderInTime(ctrl_id);
        break;
    }

   //set softkey
//     if (0 == total_num)
//     {
//         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
//     }
//     else
//     {
//         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_PREVIEW, STXT_RETURN, FALSE);
//     }
}

/*****************************************************************************/
//  Description : append picture list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicListItem(
    BOOLEAN         is_text,        //in:是否添加text data
    uint16          item_index,     //in:item索引
    uint16          content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T   ctrl_id         //in:control id
)
{
    BOOLEAN             result = FALSE;
    wchar               temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar               *file_name_ptr = PNULL;
    uint16              file_name_len = 0;
    FILEARRAY_DATA_T    file_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index, &file_info))
    {
        //set softkey
    //    item_data.softkey_id[0] = STXT_OPTION;
    //    item_data.softkey_id[1] = STXT_PREVIEW;
    //    item_data.softkey_id[2] = STXT_RETURN;

        //icon
#ifdef DRM_SUPPORT
        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            MMI_IMAGE_ID_T  icon_id = IMAGE_NULL;

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                icon_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                icon_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = icon_id;
        }
        else
#endif
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
            anim_path.full_path_wstr_ptr = file_info.filename;
            anim_path.full_path_wstr_len = file_info.name_len;
        }

        //alloc file name mem
        file_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == file_name_ptr)
        {
             return FALSE;
        } 

        SCI_MEMSET(file_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));

        //split file name
        MMIAPIFMM_SplitFullPath(file_info.filename,
                              file_info.name_len,
                              PNULL,
                              PNULL,
                              PNULL,
                              PNULL,
                              file_name_ptr,
                              &file_name_len);

        //text1,file name
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name_ptr;

        //text2,file size
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        MMIAPIFMM_GetFileSizeString(file_info.size, temp_wstr, MMIFMM_SIZE_STR_LEN, TRUE);
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(item_data.item_content[2].item_data.text_buffer.wstr_ptr);

        if (is_text)
        {
            //add item text etc.
            result = GUILIST_SetItemData(ctrl_id,
                                         &item_data,
                                         item_index);
        }
        else
        {
            //add item anim
            result = GUILIST_SetItemContent(ctrl_id,
                                            &item_data.item_content[content_index],
                                            item_index,
                                            content_index);
        }

        //free file name mem
        SCI_FREE(file_name_ptr);
        file_name_ptr = PNULL;
    }

    return (result);
}

/*****************************************************************************/
//  Description : modify pictures list view
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPicListView(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
)
{
    uint16                  cur_index = 0;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get current index
    cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ModifyPicListView view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1990_112_2_18_2_46_25_17,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        view_type = MMIPICVIEW_VIEW_ICONLIST;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        view_type = MMIPICVIEW_VIEW_LIST;
        break;
    }

    //modify view type
    MMIPICVIEW_SetViewType(view_type);

    //destroy control
    MMK_DestroyControl(ctrl_id);

    //creat control
    CreatePicListCtrl(win_id, ctrl_id);

    //need update list
    MMIPICVIEW_PostUpdateMsg(cur_index);
}

/*****************************************************************************/
//  Description : append picture icon list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL BOOLEAN AppendPicIconListItem(
//     uint16          index,
//     MMI_CTRL_ID_T   ctrl_id
// )
// {
//     BOOLEAN                 result = FALSE;
//     uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNum();
//     wchar                   *file_name_ptr = PNULL;
//     uint16                  file_name_len = 0;
//     FILEARRAY_DATA_T        file_info = {0};
//     GUIICONLIST_DISP_T      item = {0};
// 
//     if (0 < total_num)
//     {
//         //get picture info
//         if (MMIPICVIEW_GetPicFileInfo(index, &file_info))
//         {
//             //alloc file name mem
//             file_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof(wchar));
//             SCI_MEMSET(file_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
// 
//             //split file name
//             MMIAPIFMM_SplitFullPath(file_info.filename,
//                                   file_info.name_len,
//                                   PNULL,
//                                   PNULL,
//                                   PNULL,
//                                   PNULL,
//                                   file_name_ptr,
//                                   &file_name_len);
// 
//             //set title
//             item.name_str.wstr_ptr = file_name_ptr;
//             item.name_str.wstr_len = file_name_len;
// 
//             item.softkey_id[0] = STXT_OPTION;
//             item.softkey_id[1] = STXT_PREVIEW;
//             item.softkey_id[2] = STXT_RETURN;
// 
//             //append item
//             GUIICONLIST_AppendText(index, ctrl_id, &item);
// 
//             //free file name mem
//             SCI_FREE(file_name_ptr);
// 
//             result = TRUE;
//         }
//     }
// 
//     return (result);
// }

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};
    MMI_STRING_T  time_string = {0};
    uint8  temp_time_str[20] = {0};
    wchar  temp_time_wstr[20]= {0};
    ONE_DRIVE_AUTO_UPLOAD_STATE_E upload_status = ONE_DRIVE_AUTO_UPLOAD_NONE;

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(index, &file_info))
    {
#ifdef DRM_SUPPORT
        GUIANIM_DATA_INFO_T data_info = {0};

        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);
        }
        else
#endif
        {
            //zhuyongjing add for search quickly, for previous filearray
#if 1 //modify for bugid 1180126
            while(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index);
                if(!MMIPICVIEW_GetPicFileInfo(index, &file_info)){
                    return result;
                }
            }
#else
            if(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIPICVIEW_RemoveItem(ctrl_id,index);
                MMIPICVIEW_DeletePicFileInfo(index);
                return result;
            }
#endif
            //add end
            //set file name
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;

//zhuyongjing add for video time stamp for gallary ui 20150713
        if(MMIPICVIEW_IsVideoSuffix(&file_info)){
#ifdef  VIDEOTHUMBNAIL_SUPPORT
            wchar  thumbnail_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
            uint16  thumbnail_name_len = 0;
            wchar  totaltime_name [MMIFMM_FULL_FILENAME_LEN+1] = {0};
            uint16  total_name_len = 0;
            //get duration 
            MMIFMM_FmmVideoThumbnailGetPathname (thumbnail_name, &thumbnail_name_len, file_info.filename);
            MMIFMM_FmmVideoTotaltimeGetPathname(totaltime_name,&total_name_len,file_info.filename);
           if (!MMIAPIFMM_IsFileExist (thumbnail_name, thumbnail_name_len)
                ||!MMIAPIFMM_IsFileExist (totaltime_name, total_name_len))
            {
                GUIANIM_DATA_INFO_T data_info   = {0};
                DPLAYER_MEDIA_INFO_T media_info = {0};
                MMIFMM_VideoThumbnailAdd (file_info.filename);                    
                data_info.img_id = IMAGE_ONEDRIVE_DEFAULT_PIC;
                GUIICONLIST_SetIndicateIcon(ctrl_id , index, NULL);
                GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);             
                result = TRUE;
                SCI_TRACE_LOW("[mmipic] AppendPicIconListIcon  need callback index=%d",index);
                return (result);
            }else{
                //MMIAPIVP_GetInfoFromFullPathName (file_info.filename, file_info.name_len, &media_info);
                uint32 media_length = 0;
                MMIFMM_FmmGetVideoTotaltimeByPathname(&media_length ,totaltime_name);
                SCI_TRACE_LOW("[mmipic] AppendPicIconListIcon   media_length=%d,index=%d",media_length,index);
                SCI_MEMSET(temp_time_str, 0, 20*sizeof(uint8));
                SCI_MEMSET(temp_time_wstr, 0, 20*sizeof(wchar));
                if((media_length) / (1000*3600) > 0){
                    sprintf ( (char*) temp_time_str, "%02d:%02d:%02d", ( (media_length) / (1000*3600)), ( ( (media_length) / (1000*60)) % 60), ( ( (media_length) / 1000) % 60));
            }else{
                    sprintf ( (char*) temp_time_str, "%02d:%02d", ( ( (media_length) / (1000*60)) % 60), ( ( (media_length) / 1000) % 60));
            }
            //set time stamp

            MMIAPICOM_StrToWstr((const char *)temp_time_str, temp_time_wstr);
            SCI_MEMSET(&time_string, 0, sizeof(MMI_STRING_T));
            time_string.wstr_ptr = temp_time_wstr;
            time_string.wstr_len = MMIAPICOM_Wstrlen(temp_time_wstr);

            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, TRUE);
            GUIICONLIST_SetTimeStr(ctrl_id, index, time_string);

            //change filename to video thumnail filename
                 //set file name
                icon_file.full_path_wstr_ptr = thumbnail_name;
                icon_file.full_path_wstr_len = thumbnail_name_len;
                GUIICONLIST_SetIndicateIcon(ctrl_id , index, NULL);
                GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
                result = TRUE;
                return (result);
            }
#endif
        }else{
             //set file name
            GUIICONLIST_SetDisplayTimeStampFlag(ctrl_id, index, FALSE);
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;
        }
  //add end
       #if defined(CAMERAROLL_SUPPORT)
            upload_status = MMISRV_CAMERAROLL_Get_Auto_upload_status();
            SCI_TRACE_LOW("MMIPICVIEW AppendPicIconListIcon upload_status=%d,index=%d",upload_status,index);
            //append icon


            if(CAMERA_ROLL_MANUAL_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
                &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
                &&MMISRV_CAMERAROLL_Get_Connect_State())

            {
                if(TRUE ==MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(file_info.filename,file_info.name_len)){  
                    GUIICONLIST_SetIndicateIcon(ctrl_id , index , IMAGE_GALLERY_ONEDRIVE_UPLOADED_ICON);
                    SCI_TRACE_LOW("MMIPICVIEW AppendPicIconListIcon uploaded index=%d",index);
                }else if((TRUE == MMISRV_CAMERAROLL_Is_Pic_Uploading(file_info.filename,file_info.name_len))
                    &&MMISRV_CAMERAROLL_Need_manual_auto_upload_photo()){
                    GUIICONLIST_SetIndicateIcon(ctrl_id , index , IMAGE_ONEDRIVE_UPLOADING_ICON);
                    SCI_TRACE_LOW("MMIPICVIEW AppendPicIconListIcon uploading index=%d",index);
                }else{
                    GUIICONLIST_SetIndicateIcon(ctrl_id , index, NULL);
                }
            }else if(CAMERA_ROLL_AUTO_SYNC== MMISRV_CAMERAROLL_Get_Sync_State()
                &&MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
                &&MMISRV_CAMERAROLL_Get_Connect_State()
                && !MMISRV_CAMERAROLL_Is_Pic_On_OneDrive(file_info.filename,file_info.name_len)
                &&TRUE== MMISRV_CAMERAROLL_Is_Pic_Auto_Uploading(file_info.filename,file_info.name_len)){
                if(upload_status== ONE_DRIVE_AUTO_UPLOAD_NO_NETWORK){
                    GUIICONLIST_SetIndicateIcon(ctrl_id , index, IMAGE_GALLERY_ONEDRIVE_FAILED_UPLOAD_ICON);
                    SCI_TRACE_LOW("MMIPICVIEW AppendPicIconListIcon failed index=%d",index);
                }else{
                   // GUIICONLIST_SetIndicateIcon(ctrl_id , index , IMAGE_ONEDRIVE_UPLOADING_ICON);
                   GUIICONLIST_SetIndicateIcon(ctrl_id , index, NULL);
                }
            }else
	#endif
		{
                GUIICONLIST_SetIndicateIcon(ctrl_id , index, NULL);
            }
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
        }
        SCI_TRACE_LOW("MMIPICVIEW AppendPicIconListIcon finish");

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle pictures list web key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListWeb(
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN     is_enable_mark = FALSE;
    uint16      mark_num = 0;
//#ifdef DRM_SUPPORT
    uint16	item_index = 0;
//#endif
    FILEARRAY_DATA_T    file_info = {0};
    uint16  pic_item_start_index = 2;

    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        pic_item_start_index = 1;
    }

    //zhuyongjing add for pb 20150826
    if(MMIPICVIEW_GetEnterType()==MMIPICVIEW_PHOTOS_FROM_PB){
        item_index = MMIPICVIEW_GetCurIndex(ctrl_id);
        if(item_index == 0){
#ifdef MMI_PIC_CROP_SUPPORT					
            MMIPICCROP_CB_T crop_cb = {0};
            crop_cb.full_path_ptr = PNULL;
            PicCropCallBack(&crop_cb);
#endif						
            MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
            MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
//#ifdef CAMERA_SUPPORT
//        }else if((item_index == 1) && (FALSE == MMIAPISET_IsDisableShareViaOption())){
//            MMIAPIDC_OpenPhotoWinForGallery();
//#endif
        }else{
#ifdef MMI_PIC_CROP_SUPPORT					            
            MMIPICCROP_SRC_T crop_src = {0};
#endif
            if(MMIPICVIEW_GetPicFileInfo(item_index-pic_item_start_index,&file_info)){
#ifdef MMI_PIC_CROP_SUPPORT												
                crop_src.crop_type = MMIPIC_CROP_PB;          //lijinghua modify for pb
                crop_src.callback = PicCropCallBack;
                crop_src.full_path_ptr = file_info.filename;
                crop_src.full_path_len = file_info.name_len;
                
                crop_src.min_width = 42;//(s_pb_photo_anim_rect.right-s_pb_photo_anim_rect.left);
                crop_src.min_height = 42;//(s_pb_photo_anim_rect.bottom-s_pb_photo_anim_rect.top);
                MMIAPIPICCROP_OpenPicCrop(&crop_src); 
#endif								
            }
        }
        
        return;
    }
    //add end
    
    if (0 < MMIPICVIEW_GetAllPicNum())
    {
        
        //get mark status
        is_enable_mark = MMIPICVIEW_GetListMarkable(ctrl_id);
        if (is_enable_mark)
        {
            //get mark num
            mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id, PNULL, 0);
            //zhuyongjing add for first icon mark not select
            if(mark_num == 0){
                MMIPICVIEW_SetSelectedItem(ctrl_id,MMIPICVIEW_GetCurIndex(ctrl_id),TRUE);  
                //MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_CTL_ICONLIST_MARK_ITEM, PNULL);
                return;
            }
            //add end
        }
        
        item_index = MMIPICVIEW_GetCurIndex(ctrl_id);
        
#ifdef DRM_SUPPORT
        if (MMIPICVIEW_IsPreview(TRUE, item_index))
#endif
        {
            //win_id = MMK_GetFocusWinId();
            //zhuyongjing add for cancel multi uploading
        #ifdef CAMERAROLL_SUPPORT
            if(    s_mmipicview_multi_uploading_done 
                && s_mmipicview_multi_uploading_highlight
                && MMK_IsActiveCtrl(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID)){
                //cancel mutil upload api wenny
                MMISRV_CAMERAROLL_Cancel_Upload();
                SCI_TRACE_LOW("[HandlePicListWeb]:mmipic cancel uploading.... ");
                
                //change ui
                //delete uploading cloud icon
                s_mmipicview_multi_uploading_done = FALSE;
                s_mmipicview_multi_uploading_highlight = FALSE;
                MMIPICVIEW_DestroyAllUploadingPicArray();
                GUIWIN_SeSoftkeytButtonTextId(MMIPICVIEW_LIST_WIN_ID, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, TRUE);
                ModifyPicListViewForUploadItems(MMIPICVIEW_LIST_WIN_ID,ctrl_id,FALSE);
                MMISRV_CAMERAROLL_Clean_manual_upload_list();
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, &item_index);
                //MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL);
                return;
            }
            if(s_mmipicview_multi_uploading_done 
                && (s_mmipicview_multi_uploading_highlight ||MMIPICVIEW_GetListMarkable(ctrl_id))
                && !MMK_IsActiveCtrl(ctrl_id)){
                MMK_SetAtvCtrl(MMIPICVIEW_LIST_WIN_ID,ctrl_id);
            }
        #endif						
            //zhuyongjing add for play video 20150713
            if(MMIPICVIEW_GetFileIsVideotoPlay(item_index)){
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
                    MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
                }
                MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
                return;
            }
            //add end
            if (MMIPICVIEW_isFileError(ctrl_id, item_index, MMIPICVIEW_PICOPE_POINT))
            {
                return;
            }
            
            //GUIICONLIST_PrepareSpecial(ctrl_id, 0);
            
#ifdef PIC_GLIDE_SUPPORT
            MMK_CreateWin((uint32 *)MMIPICVIEW_GLIDE_WIN_TAB, PNULL);
#else
            if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
                MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
            }
            MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
#endif
        }
        //zhuyongjing add for all empty UI 20150713
    }    else if(MMIPICVIEW_GetAllPicNum() ==0){
        if(MMIAPIPICVIEW_GetShowFolderPath() == CAMERA_ROLL_SOURCE_NONE
            &&GUIICONLIST_GetTotalIcon(ctrl_id) == 0
            && !MMIAPIDC_IsOpened()
            &&!MMIPICVIEW_GetMultiChoiseSaveToOneDrive()
            &&MMIPICVIEW_GetMultiChoiceDelete() == FALSE
            &&MMIPICVIEW_GetMultiChoiceShare() == FALSE){
            CamerButtonCallBackEx();
    }else{
            
        }
    }
//add end
        
}
/*****************************************************************************/
//  Description : get current index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetCurIndex(
    MMI_CTRL_ID_T  ctrl_id
)
{
    uint16                      cur_index = 0;
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_GetCurIndex view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2190_112_2_18_2_46_25_18,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
        break;
    }

    return (cur_index);
}

/*****************************************************************************/
//  Description : set current index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetCurIndex(
    MMI_CTRL_ID_T    ctrl_id,
    uint16           cur_index
)
{
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetCurIndex view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2221_112_2_18_2_46_25_19,(uint8*)"d", cur_index);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        GUIICONLIST_SetCurIconIndex(cur_index, ctrl_id);
        break;
    }
}

/*****************************************************************************/
//  Description : set max selected number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetMaxSelected(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         selected_max_num
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetMaxSelected view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2251_112_2_18_2_46_25_20,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetMaxSelectedItem(ctrl_id, selected_max_num);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetMaxSelectedItem(ctrl_id, selected_max_num);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get list markable
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetListMarkable(
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2282_112_2_18_2_46_25_21,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_GetListStateById(ctrl_id, GUILIST_STATE_ENABLE_MARK);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_GetMarkable(ctrl_id);
        break;
    }
    SCI_TRACE_LOW("MMIPICVIEW MMIPICVIEW_GetListMarkable result=%d",result);

    return (result);
}

/*****************************************************************************/
//  Description : set list markable
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetListMarkable(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_markable
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetListMarkable markable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2314_112_2_18_2_46_25_22,(uint8*)"d", is_markable);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetTextListMarkable(ctrl_id, is_markable);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetMarkable(ctrl_id, is_markable);
        result = GUIICONLIST_SetIconMaskable(ctrl_id,is_markable);
        break;
    }
    if(!is_markable && s_is_reload_file_aftersearch_getfous
        && MMK_IsFocusWin(MMIPICVIEW_LIST_WIN_ID)){
        //PICVIEW_ReloadAfterSearch();
        MMIPICVIEW_ReloadAfterSearchEx();
        //FinishLoadPic(win_id);
        s_is_reload_file_aftersearch_getfous = FALSE;
     }
    return (result);
}

/*****************************************************************************/
//  Description : item is select
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSelectItem(
    MMI_CTRL_ID_T    ctrl_id,
    uint16           index
)
{
    BOOLEAN                 result = FALSE;
    const GUILIST_ITEM_T    *item_ptr = PNULL;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW IsSelectItem view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2347_112_2_18_2_46_25_23,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //get item pointer
        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);

        //current item is mark
        result = GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_IsSelectedItem(ctrl_id, index);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get selected item index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetSelectItemIndex(//selected number
    MMI_CTRL_ID_T   ctrl_id,    //in:
    uint16          *index_ptr, //in/out:may PNULL
    uint16		    array_len   //in:
)
{
    uint16                  select_num = 0;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW GetSelectItemIndex view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2384_112_2_18_2_46_25_24,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        select_num = GUILIST_GetSelectedItemIndex(ctrl_id, index_ptr, array_len);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        select_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, index_ptr, array_len);
        break;
    }

    return (select_num);
}

/*****************************************************************************/
//  Description : set selected item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetSelectedItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index,
    BOOLEAN       is_selected
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW SetSelectedItem view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2417_112_2_18_2_46_25_25,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetSelectedItem(ctrl_id, index, is_selected);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetSelectedItem(ctrl_id, index, is_selected);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetAllSelected(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN        is_selected
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetAllSelected view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2449_112_2_18_2_46_25_26,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        GUILIST_SetAllSelected(ctrl_id, is_selected);
        result = TRUE;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetAllSelected(ctrl_id, is_selected);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_RemoveItem(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    SCI_TRACE_LOW("MMIPICVIEW RemoveItem index=%d",index);
   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2482_112_2_18_2_46_25_27,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_RemoveItem(ctrl_id, index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_RemoveItem(ctrl_id, index);
        break;
    }

    return (result);
}
#if defined(DRM_SUPPORT)//norflashcut_pic
/*****************************************************************************/
//  Description : modify item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_ModifyItemIcon view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2514_112_2_18_2_46_26_28,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        ModifyListItemIcon(ctrl_id, index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
        result = AppendPicIconListIcon(index, ctrl_id);
        break;

    default:
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : modify list item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ModifyListItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
#ifdef DRM_SUPPORT
    const GUILIST_ITEM_T    *item_ptr = PNULL;
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    //get item
    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
    if (PNULL != item_ptr)
    {
        //copy item
        MMI_MEMCPY(&item_t, sizeof(GUILIST_ITEM_T),
                   item_ptr, sizeof(GUILIST_ITEM_T),
                   sizeof(GUILIST_ITEM_T));

        MMI_MEMCPY(&item_data, sizeof(GUILIST_ITEM_DATA_T),
                   item_ptr->item_data_ptr, sizeof(GUILIST_ITEM_DATA_T),
                   sizeof(GUILIST_ITEM_DATA_T));

        //set item
        item_t.item_data_ptr = &item_data;
        if (IMAGE_DRM_PIC_LOCK_ICON != item_data.item_content[0].item_data.image_id)
        {
            item_data.item_content[0].item_data.image_id = IMAGE_DRM_PIC_LOCK_ICON;

            //set item
            GUILIST_ReplaceItem(ctrl_id, &item_t, index);

            result = TRUE;
        }
    }
#endif

    return (result);
}

/*****************************************************************************/
//  Description : modify item text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemText(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_ModifyItemText view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2594_112_2_18_2_46_26_29,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //need update list
        MMIPICVIEW_PostUpdateMsg(index);
        result = TRUE;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
//         result = AppendPicIconListItem(index, ctrl_id);
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : reload picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ReloadFile(void)
{
    //reload file
    s_is_reload_file = TRUE;
}

PUBLIC BOOLEAN MMIAPIPICVIEW_GetAfterSearchFousStatus(void)
{
    return s_is_reload_file_aftersearch_getfous;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicFolderWinOpen(void)
{
//zhuyongjing change  for gallary UI picture and video 20150713
   // return MMK_IsOpenWin(MMIPICVIEW_PICFOLDER_WIN_ID);
    return MMK_IsOpenWin(MMIPICVIEW_PICFOLDER_WIN_ID) ||MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID);
//add end
}

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIPICVIEW_GetClientWinRect(MMI_WIN_ID_T  win_id)
{
    GUI_BOTH_RECT_T     client_rect = MMITHEME_GetWinClientBothRect(win_id);
    client_rect.v_rect.top     += MMIPICPREVIEW_TITLE_HEIGHT;
#ifdef  PIC_PREVIEW_U200_SUPPORT
    client_rect.v_rect.bottom  -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    client_rect.v_rect.bottom = MAX(client_rect.v_rect.bottom, client_rect.v_rect.top);
#endif    
    client_rect.h_rect.top     += MMIPICPREVIEW_TITLE_HEIGHT;
#ifdef  PIC_PREVIEW_U200_SUPPORT
    client_rect.h_rect.bottom  -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    client_rect.h_rect.bottom = MAX(client_rect.h_rect.bottom, client_rect.h_rect.top);
#endif
    return client_rect;
}
#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//  Description : set title for pic list 
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPicListTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T return_id)
{
    GUI_BOTH_RECT_T     title_rect   = MMITHEME_GetWinClientBothRect(win_id);    
    GUI_BOTH_RECT_T     button_rect  = title_rect;
    uint16              title_height = MMIPICPREVIEW_TITLE_HEIGHT;
	GUI_BG_T            bg          = {0};
    MMI_STRING_T        title_txt   = {0};
    GUISTR_STYLE_T      txt_style   = {0};
    GUISTR_STATE_T      txt_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD;
    uint16              img_width   = 0;
    uint16              img_height  = 0;
    uint16              button_wid  = 0;

#ifdef  PIC_PREVIEW_U200_SUPPORT
    //label rect,txt,font....
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICVIEW_RETURN, win_id);
#endif
//     button_wid = 2 * MMIPICPREVIEW_TITLE_MARGIN + img_width;

    title_rect.v_rect.bottom = title_rect.v_rect.top + title_height - 1;
    title_rect.h_rect.bottom = title_rect.h_rect.top + title_height - 1;
    title_rect.v_rect.left   = MMIPICPREVIEW_TITLE_MARGIN;
    title_rect.h_rect.left   = MMIPICPREVIEW_TITLE_MARGIN;

    if(PNULL != return_id)
    {
        button_wid = 2 * MMIPICPREVIEW_TITLE_MARGIN + img_width;
        //return button
        button_rect.v_rect.top      = title_rect.v_rect.top;
        button_rect.h_rect.top      = title_rect.h_rect.top;
        button_rect.v_rect.bottom   = title_rect.v_rect.bottom;
        button_rect.h_rect.bottom   = title_rect.h_rect.bottom;
        button_rect.v_rect.right    -= MMIPICPREVIEW_TITLE_MARGIN;
        button_rect.h_rect.right    -= MMIPICPREVIEW_TITLE_MARGIN;
        button_rect.v_rect.left     = button_rect.v_rect.right - button_wid + 1;
        button_rect.h_rect.left     = button_rect.h_rect.right - button_wid + 1;
        GUIAPICTRL_SetBothRect(return_id, &button_rect);
    }
    if(PNULL != ctrl_id)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_PIC, &title_txt);

        bg.bg_type  = GUI_BG_NONE;
        
        txt_style.font      = MMIPICPREVIEW_TITLE_FONT;
        txt_style.font_color= MMI_WHITE_COLOR;
        txt_style.align     = ALIGN_LEFT;//水平剧居中,垂直方向居中

		GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);

        GUILABEL_SetBg(ctrl_id, &bg);
        GUILABEL_SetFont(ctrl_id, MMIPICPREVIEW_TITLE_FONT, MMI_WHITE_COLOR);
        GUILABEL_SetText(ctrl_id, &title_txt, FALSE);

        button_wid = GUISTR_GetStringWidth(&txt_style, &title_txt, txt_state) + MMIPICPREVIEW_TITLE_MARGIN;

        title_rect.v_rect.right  = title_rect.v_rect.left + button_wid - 1;
        title_rect.h_rect.right  = title_rect.h_rect.left + button_wid - 1;
        if (PNULL != return_id)
        {
            title_rect.v_rect.right  = MIN(button_rect.v_rect.left, title_rect.v_rect.right);
            title_rect.h_rect.right  = MIN(button_rect.h_rect.left, title_rect.h_rect.right);
        }
        else
        {
            title_rect.v_rect.right  = MIN(button_rect.v_rect.right, title_rect.v_rect.right);
            title_rect.h_rect.right  = MIN(button_rect.h_rect.right, title_rect.h_rect.right);
        }
        GUIAPICTRL_SetBothRect(ctrl_id, &title_rect);
    }
}
#endif
/*****************************************************************************/
// 	Description : set iconlist param for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_folder_list)
{
    GUI_BG_T            ctrl_bg     = {0};
    GUI_BOTH_RECT_T     icon_rect   = MMIPICVIEW_GetClientWinRect(win_id);
    uint16              icon_width  = MMIPICPREVIEW_LIST_ICON_SIZE;
    GUIICONLIST_MARGINSPACE_INFO_T  icon_space = {0};

    //设置iconlist的范围
    GUIAPICTRL_SetBothRect(ctrl_id, &icon_rect);
    SCI_TRACE_LOW("MMIPICVIEW SetIconListParam ZYJ is_folder_list=%d",is_folder_list);	
    
    //设置背景
    ctrl_bg.bg_type         = GUI_BG_COLOR;//GUI_BG_IMG;//zhuyongjing change from img to color
    //ctrl_bg.is_screen_img   = TRUE;
    //ctrl_bg.img_id          = PICVIEW_DEFAULT_BG_IMG;
    ctrl_bg.color = MMI_BACKGROUND_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &ctrl_bg);

    if(!is_folder_list)
    {
        //设置每个icon的高、宽距
        icon_space.iconlist_hmargin = 0;
        icon_space.iconlist_vmargin = MMIPICPREVIEW_LIST_ICON_MARGIN;
        icon_space.icon_item_hspace = MMIPICPREVIEW_LIST_ICON_MARGIN;
        icon_space.icon_item_vspace = MMIPICPREVIEW_LIST_ICON_MARGIN;
        GUIICONLIST_SetIconItemSpace(ctrl_id, icon_space);
        GUIICONLIST_SetIconWidthHeight(ctrl_id, MMIPICPREVIEW_LIST_ICON_SIZE, MMIPICPREVIEW_LIST_ICON_SIZE);
    }
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

#ifdef  PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set spftkey param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetButtonFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_IMAGE_ID_T* img_id, uint16 button_num)
{
    BOOLEAN             result      = TRUE;
    GUI_BG_T            bg_info     = {0};
    GUI_BG_T            fg_info     = {0};
    GUI_BOTH_RECT_T     form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T     button_rect = {0};
    uint16              button_height   = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    uint16              button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16              button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
    int                 i = 0;
    
    if(button_num > 0)
    {
        form_rect.v_rect.top = form_rect.v_rect.bottom - button_height + 1;
        form_rect.h_rect.top = form_rect.h_rect.bottom - button_height + 1;
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = IMAGE_PICVIEW_BUTTON;
		
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
        button_rect       = form_rect;

        fg_info.bg_type = GUI_BG_IMG;
        for(i = 0; i < button_num; i++)
        {
            button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
            button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
            button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
            button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;
            GUIAPICTRL_SetBothRect(ctrl_id[i], &button_rect);

            fg_info.img_id = img_id[i];
            GUIBUTTON_SetBg(ctrl_id[i], &bg_info);
            GUIBUTTON_SetFg(ctrl_id[i], &fg_info);
            GUIBUTTON_PermitBorder(ctrl_id[i], FALSE);
        }
    }
    else
    {
        result = FALSE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetButtonsVisible(BOOLEAN is_visible)
{
    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_SLIDE_PREVIEW, is_visible, FALSE);
    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_MARKED, is_visible, FALSE);    
}

/*****************************************************************************/
// 	Description : handle after point the mark menu
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void InitMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create_ptr  = {0};
    GUIBUTTON_INIT_DATA_T   init_data   = {0};
    GUI_BOTH_RECT_T         form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T         button_rect = form_rect;
    uint16                  button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16                  button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
//    GUI_BG_T                button_bg      = {0};
    GUI_FONT_ALL_T          button_font_all = {0};//button的字体与颜色
    MMI_CTRL_ID_T           button_id[] = {MMIPICVIEW_MARK_ALLMARK_CTRL_ID, MMIPICVIEW_MARK_DELETE_CTRL_ID, MMIPICVIEW_MARK_CANCEL_CTRL_ID};//
    MMI_TEXT_ID_T           txt_id[]    = {TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL};// 
    uint8                   button_num  = 3;
    uint8                   i;

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;
        
        button_rect.v_rect.top = button_rect.v_rect.bottom - MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
        button_rect.h_rect.top = button_rect.h_rect.bottom - MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
                 
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
                
        init_data.bg.bg_type    = GUI_BG_IMG;
        init_data.bg.img_id     = IMAGE_PICVIEW_BUTTON;
        button_font_all.font    = MMI_DEFAULT_NORMAL_FONT;
        button_font_all.color   = MMI_WHITE_COLOR;
        for(i = 0; i < button_num; i++)
        {
            button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
            button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
            button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
            button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;

            if(i == (button_num - 1))
            {
                button_rect.v_rect.right = MAX(button_rect.v_rect.right, form_rect.v_rect.right);
                button_rect.h_rect.right = MAX(button_rect.h_rect.right, form_rect.h_rect.right);
            }
            init_data.both_rect = button_rect;
            create_ptr.ctrl_id = button_id[i];
            MMK_CreateControl(&create_ptr);
            
            GUIBUTTON_SetFont(button_id[i], &button_font_all);
            GUIBUTTON_SetTextId(button_id[i], txt_id[i]);
            GUIBUTTON_PermitBorder(button_id[i], FALSE);
        }
    }
}

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    if(PNULL != MMK_GetCtrlPtrByWin(win_id, MMIPICVIEW_MARK_ALLMARK_CTRL_ID))
    {
        MMK_DestroyControl(MMIPICVIEW_MARK_ALLMARK_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_MARK_DELETE_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_MARK_CANCEL_CTRL_ID);
    }
}
#endif
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// 	Description : set the iconlist groups orded in time
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetlistOrderInTime(MMI_CTRL_ID_T ctrl_id)
{
    MMISET_DATE_DISPLAY_TYPE_E      date_type   = MMISET_DATE_YMD;
    GUIICONLIST_DELIMITER_INFO_T*   group_info_ptr  = {0};
    FILEARRAY_DATA_T                file_info   = {0};
    uint32                          pic_num     = MMIPICVIEW_GetAllPicNum();
    SCI_DATE_T                      tmp_date    = {0};   //DT??è??ú
    char                            tmp_char[9] = {0};
    uint8                           str_len     = 0;
    GUI_BG_T                        bg_ptr      = {0};
    GUI_FONT_ALL_T                  txt_style   = {0};
    uint16                          txt_left_space = 0;
    uint32                          i,j;
    wchar 							tmp_str[9]  = {0};
	DELIMITER_BG_SPACE_T    		bg_space	= {0};
    uint32                          start_index = 0;
    FILEARRAY                       file_array  = MMIAPIPICVIEW_GetCurPicArray();

    SCI_TRACE_LOW("[mmipicview] SetlistOrderInTime all_pic_num = %d", pic_num);
    if (pic_num > 0)
    {
        date_type = MMIAPISET_GetDateDisplayType();

        MMIVIRTUALARRAY_SetBufferSize(file_array,500);
#ifndef MMI_PICVIEW_MINI_SUPPORT
        start_index = MMIAPIPICVIEW_GetFirstPos(file_array);
#endif
        SCI_TRACE_LOW("[mmipicview] SetlistOrderInTime start_index = %d", start_index);

        group_info_ptr = (GUIICONLIST_DELIMITER_INFO_T *)SCI_ALLOC_APP(sizeof(GUIICONLIST_DELIMITER_INFO_T) );
        if (PNULL == group_info_ptr)
        {
             return;
        } 
        SCI_MEMSET(group_info_ptr, 0, sizeof(GUIICONLIST_DELIMITER_INFO_T) );
        
        //get all the time info of all pics
        for(i = 0, j = 0; (i < pic_num) && (j < pic_num); i++)
        {
            if(!MMIAPIFILEARRAY_Read(file_array, (start_index + i), &file_info))
            {
                break;
            }
            if((file_info.create_date.year !=  tmp_date.year)
                || (file_info.create_date.mon != tmp_date.mon))
            {
                
                tmp_date.year = file_info.create_date.year;
                tmp_date.mon  = file_info.create_date.mon;
                
                if(MMISET_DATE_YMD == date_type)
                {
                    sprintf(tmp_char, "%04d.%02d:", tmp_date.year, tmp_date.mon);
                }
                else
                {
                    sprintf(tmp_char, "%02d.%04d:", tmp_date.mon, tmp_date.year);
                }
                str_len = strlen(tmp_char);
                MMI_STRNTOWSTR(tmp_str, 8, (uint8 *)tmp_char, 8, str_len);
                group_info_ptr->delimiter_str_info.wstr_ptr = tmp_str;
                group_info_ptr->delimiter_str_info.wstr_len = str_len;
                group_info_ptr->start_item_index = i;
                GUIICONLIST_CreateGroup(ctrl_id, group_info_ptr);
                j++;
            }
        }
        		
        txt_style.color = MMIPICVIEW_TITLE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        txt_style.font  = MMI_DEFAULT_NORMAL_FONT;
        txt_left_space  = 10;
        GUIICONLIST_SetDelimiterStrInfo(ctrl_id, &txt_style, txt_left_space);
#ifndef MMI_GUI_STYLE_MINISCREEN
        bg_ptr.bg_type  = GUI_BG_IMG;//GUI_BG_NONE;
        bg_ptr.img_id	= IMAGE_PICVIEW_OSD_BG; 
#else
        //设置背景
        bg_ptr.bg_type = GUI_BG_IMG; 
        bg_ptr.img_id  = IMAGE_PICVIEW_TIME_BG;
#endif
        GUIICONLIST_SetDelimiterBg(ctrl_id, &bg_ptr);
#ifndef MMI_GUI_STYLE_MINISCREEN
  {
    	bg_space.height = GUI_GetStringHeight(txt_style.font, tmp_str, 8)+ 5;
		bg_space.width	= GUI_GetStringWidth(txt_style.font, tmp_str, 8) + 8;
		bg_space.left	= txt_left_space;
		bg_space.top	= 5;
  }
#else
  { 
        //IGUICTRL_T  *iconlist_ctrl_ptr = MMK_GetCtrlPtr(ctrl_id);
        bg_space.top = 0;
        bg_space.height = MMIPICPREVIEW_TIME_HEIGHT;
        bg_space.left = 0;
        //if(PNULL != iconlist_ctrl_ptr)
        {  
            /*判断是否需要显示滚动条*/
			if(GUIICONLIST_GetRelativeHeight( ctrl_id) > GUIICONLIST_GetDispHeight( ctrl_id))
            {
                bg_space.width = MMITHEME_GetClientRectWidth() - MMITHEME_GetScrollBarWidth() - MMIPICVIEW_DELIMETER_MARGIN;
            }
            else
            {
                bg_space.width = MMITHEME_GetClientRectWidth();
            }
        }
         //128X160 DelimiterHeight 与背景图片同高
        GUIICONLIST_SetDelimiterHeight(ctrl_id, MMIPICPREVIEW_TIME_HEIGHT); 
        /*设置背景图片的显示区域*/
  }
#endif
        GUIICONLIST_SetDelimiterBgSpace(ctrl_id, &bg_space);
        
        SCI_FREE(group_info_ptr);
        MMIVIRTUALARRAY_SetBufferSize(file_array,50);
    }
    MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID, MSG_PICLIST_LOAD_END, (DPARAM)PNULL, (uint32)0);
}
#endif
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// 	Description : refresh the iconlist groups orded in time
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeleteOrderInTime(MMI_CTRL_ID_T ctrl_id)
{
    uint16 group_num = GUIICONLIST_GetDelimiterGroupNum(ctrl_id);
//    uint16 i = 0;
    if(group_num > 0)
    {
//         for(i = group_num; i > 0; i--)
//         {
//             GUIICONLIST_RemoveGroup(ctrl_id, i - 1);
//         }
		GUIICONLIST_RemoveAllDelimiter(ctrl_id);
    }
}
#endif
/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          disp_rect   = MMITHEME_GetFullScreenRect();
    GUI_RECT_T          client_rect   = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO    dev_info    = {0};
    
//zhuyongjing change from img to color
   // GUIRES_DisplayImg(PNULL, &disp_rect, &disp_rect, win_id, PICVIEW_DEFAULT_BG_IMG, &dev_info);
    GUI_FillRect((const GUI_LCD_DEV_INFO*) &dev_info,disp_rect, MMI_BACKGROUND_COLOR);

}

/*****************************************************************************/
//  Discription: display when list win is empty
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
LOCAL void PicviewListEmptyShow(void)
{
    MMI_WIN_ID_T        win_id      = MMIPICVIEW_LIST_WIN_ID;
    GUI_RECT_T          client_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T          disp_rect   = {0};
    MMI_IMAGE_ID_T      img_id      = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
    uint16              img_width   = 0;
    uint16              img_height  = 0;
    GUI_LCD_DEV_INFO    dev_info    = {0};
    MMI_STRING_T        str_data    = {0};
    GUISTR_STYLE_T      style_info  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);
    disp_rect.top    = client_rect.top + ((client_rect.bottom - client_rect.top + 1) / 2 - img_height / 2); 
    disp_rect.bottom = disp_rect.top + img_height - 1;
    disp_rect.left   = client_rect.left + ((client_rect.right - client_rect.left + 1) / 2 - img_width / 2);
    disp_rect.right  = disp_rect.left + img_width - 1;
    GUIRES_DisplayImg(PNULL, &disp_rect, PNULL, win_id, img_id, &dev_info);
    
    style_info.align    = ALIGN_HMIDDLE;
    style_info.font     = SONG_FONT_14;
    style_info.font_color= MMI_GRAY_WHITE_COLOR;
    disp_rect.top       = disp_rect.bottom + 8;
    disp_rect.bottom    = disp_rect.top + 18;
    disp_rect.left      = client_rect.left;
    disp_rect.right     = client_rect.right;
    MMI_GetLabelTextByLang(TXT_PICVIEW_NOPHOTO, &str_data);
   
    GUISTR_DrawTextToLCDInRect(&dev_info, &disp_rect, &disp_rect, &str_data, &style_info,  state, GUISTR_TEXT_DIR_AUTO);
}


/*****************************************************************************/
// 	Description : set if the list win is empty
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetListEmpty(BOOLEAN is_empty)
{
    s_is_list_empty = is_empty;
}

PUBLIC BOOLEAN MMIPICVIEW_GetListEmpty(void)
{
    return s_is_list_empty;
}
/*****************************************************************************/
// 	Description : get if the list win is open form DCamera
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetifListFromDc(void)
{
	return s_is_need_to_reinit_picarray_from_piclib;
}

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicListWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN s_piclist_cancel_load = FALSE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMI_CTRL_ID_T     ctrl_id=0;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    ctrl_id = pubwin_info_ptr->user_data;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_PostMsg(win_id, MSG_PICLIST_LOAD_START, PNULL, 0);
        break;
        
    case MSG_PICLIST_LOAD_START:
#ifdef GUIF_ICONLIST_DELIMITER
         if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        }
      //  SetlistOrderInTime(ctrl_id);//zhuyongjing delete for gallary ui 20150707
      //zhuyongjing add for close win
          MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID, MSG_PICLIST_LOAD_END, (DPARAM)PNULL, (uint32)0);
 #endif
        break;
        
    case MSG_PICLIST_LOAD_END:
        MMK_CloseWin(win_id);
        break;
                
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void LoadPicList(MMI_CTRL_ID_T ctrl_id)
{
    MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;
    
    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleLoadPicListWinMsg, ctrl_id);
}
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Discription: get default picture path
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void GetDefaultPicPath(MMIPICVIEW_DEFAULT_PATH_T* path_ptr)
{
    MMIFILE_DEVICE_E fmm_dev =  0;
    const wchar* dev_ptr = PNULL;
    uint16       dev_len = 0;
    
    if (PNULL == path_ptr)
    {
        SCI_TRACE_LOW("MMIPICVIEW GetDefaultPicPath path_ptr is PNULL !!!");
        return ;
    }
    
    fmm_dev = MMIAPIFMM_GetDefaultDisk();
    
    dev_ptr = MMIAPIFMM_GetDevicePath(fmm_dev);
    dev_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);
    path_ptr->path_len = MMIFMM_PATHNAME_LEN;
    
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
        MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
        PNULL, 0,
        path_ptr->path, &path_ptr->path_len);
}
#endif

// /*****************************************************************************/
// // 	Description : prepare special photo data as bg
// //	Global resource dependence :
// //  Author: yaye.jiang
// //	Note: //目前只实现用固有资源实现
// /*****************************************************************************/
// LOCAL void PrepareSpecialPhotoBgData(MMI_WIN_ID_T win_id)
// {
//     BOOLEAN             result          = FALSE;
//     GUIIMG_SRC_T        src_info        = {0};
//     GUIIMG_DISPLAY_T    img_display     = {0};
//     GUIIMG_DEC_OUT_T    img_output      = {0};
//     MMI_IMAGE_ID_T      img_id          = PICVIEW_DEFAULT_BG_IMG;
//     uint16              src_size        = 0;
//     uint16              input_width     = 0;
//     uint16              input_height    = 0;
//     uint16              target_size     = 0;
//     uint16              target_width    = 0;
//     uint16              target_height   = 0;
//     IMG_INPUT_PARAM_T   input_param     = {0};
//     GUI_LCD_DEV_INFO    lcd_info        = {0};
// 
//     GUILCD_GetLogicWidthHeight(lcd_info, &target_width, &target_height);
//     
//     target_format   = IMGREF_FORMAT_RGB565;
//     input_param.smooth_type         = IMGREF_SMOOTH_MEAN_EX;//目前只支持这一种
//     input_param.target_data_format  = target_format;
//     
//     src_size = input_height * input_width * 4;
//     res_src->imgstream_ptr = (uint8 *)SCI_ALLOC_APP(sizeof(src_size));
//     if (PNULL == res_src->imgstream_ptr)
//     {
//         SCI_TRACE_LOW("[Mmipicview_list]PrepareSpecialPhotoBgData: No Memory!");
//         return FALSE;
//     }
// 
//     SCI_MEMSET(res_src->imgstream_ptr, 0, res_src->imgstream_ptr);
//     
//     //get valid file or resource img
//     if()    //data
//     {
//         src_info.is_file        = FALSE;
//         src_info.src_buf_ptr    = (uint8 *)MMI_GetLabelImage(img_id, win_id, &(src_info.src_data_size));
//         GUIRES_GetImgWidthHeight(&input_width, &input_height, img_id, win_id);
//     }
//     else    //file
//     {
//         src_info.is_file        = TRUE;
//         src_info.full_path_ptr  = GetValidFile();
//         //如何获取file的size信息????????????????????????????????????????????????????????????
//     }
//     
//     //decode to get the input data
//     input_width     = min(input_width, target_width);
//     input_height    = min(input_height, target_height);
//     img_display.dest_width  = input_width;
//     img_display.dest_height = input_height;
//     img_output.actual_width = input_height;
//     img_output.actual_height= input_height;
//     img_output.decode_data_ptr  = res_src->imgstream_ptr;
//     img_output.decode_data_size = src_size;
//     GUIIMG_Decode(&src_info, &img_display, &img_output);
// 
//     //set the input buffer param
//     if(GUIIMG_DATE_TYPE_RGB565 == img_output.data_type)
//     {
//         input_param.src_data_format     = IMGREF_FORMAT_RGB565;
//         src_size   = input_height * input_width * 2;
//         res_src->imgstreamsize = src_size;
//     }
//     else
//     {
//         input_param.src_data_format     = IMGREF_FORMAT_ARGB888;
//         res_src->imgstreamsize = src_size;
//     }
//     input_param.src_chn.ch0.ptr     = res_src.imgstream_ptr;
//     input_param.src_size.h          = img_output.actual_height; 
//     input_param.src_size.w          = img_output.actual_width;
//     input_param.src_chn.chn0.size   = res_src.imgstreamsize;
//     
//     //set the target buffer param
//     if(IMGREF_FORMAT_ARGB888 == input_param.target_data_format )
//     {
//         target_size = target_height * target_width * 4;
//     }
//     else if(IMGREF_FORMAT_RGB565 == input_param.target_data_format)
//     {
//         target_size = target_height * target_width * 2;
//     }       
//     targ_src->imgstream_ptr = (uint8 *)SCI_ALLOC_APP(sizeof(target_size));
//     targ_src->imgstreamsize = target_size;
//     input_param.target_chn.chn0.ptr = targ_src.imgstream_ptr;
//     input_param.target_size.h       = target_height;
//     input_param.target_size.w       = target_width;
//     input_param.target_chn.chn0.size= targ_src.imgstreamsize;
//     
//     input_param.trim_rect.x = 300;
//     input_param.trim_rect.y = 300;
//     input_param.trim_rect.h = 50;
//     input_param.trim_rect.w = 100;
//     
//     ScaleAndDiffusionImage_Ex(&input_param);
// }
// 
// /*****************************************************************************/
// // 	Description : display the special photo data
// //	Global resource dependence :
// //  Author: yaye.jiang
// //	Note: 
// /*****************************************************************************/
// LOCAL BOOLEAN ShowSpecialPhotoBg(MMI_WIN_ID_T win_id)
// {
//     BOOLEAN             result          = FALSE;
//     GUIIMG_DISPLAYBMP_PARAM_T display_info  = {0};
//     GUI_LCD_DEV_INFO    lcd_info        = {0};
//     GUI_RECT_T          display_rect    = MMITHEME_GetClientRect();
//     uint16              target_width    = 0;
//     uint16              target_height   = 0;
//     
//     GUILCD_GetLogicWidthHeight(lcd_info, &target_width, &target_height);
//     
//     if(PNULL != targ_src->imgstream_ptr)
//     {
//         display_info.bitmap_ptr.bit_ptr     = targ_src->imgstream_ptr;
//         display_info.bitmap_ptr.img_height  = target_height;
//         display_info.bitmap_ptr.img_width   = target_width;
//         display_info.bitmap_ptr.date_type   = target_format;
// 
//         display_info.lcd_dev_ptr            = &lcd_info;
//         display_info.rect_ptr               = &display_rect;
//         
//         if(GUIIMG_DisplayBmpEx(display_info))
//         {
//             result = TRUE;
//         }
//     }
//     return result;
// }
// 
#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
// 	Description : display the total pic number
//	Global resource dependence :
//  Author: langhua.yang
//	Note: just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DispTotalPicNum(void)
{    
    uint8                   str[8] = {0};
    wchar                   wstr[8] = {0};
    uint16                  freq_len = 0;
    uint16                  str_len = 0;
    MMI_STRING_T            string = {0};/*lint !e64*/
    uint32                  total_num = MMIPICVIEW_GetAllPicNum();
    sprintf((char *)str, "%ld", total_num); /*lint !e64*/
    str_len = (uint16)strlen((char *)str);
    MMI_STRNTOWSTR(wstr, 8,
                str, 8,
                str_len);
    freq_len = MMIAPICOM_Wstrlen(wstr);
    if(freq_len > 0)
    {           
      string.wstr_len = freq_len;
      string.wstr_ptr = wstr;        
      GUILABEL_SetFont(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID, SONG_FONT_10, MMI_GRAY_WHITE_COLOR);
      GUILABEL_SetText(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID,&string,FALSE);
    }  
}  
#endif
#endif //#ifdef PIC_VIEWER_SUPPORT

PUBLIC BOOLEAN MMIPICVIEW_GetIsGoToCamera(void)
{
    return s_is_go_to_camera;
}

PUBLIC void MMIPICVIEW_SetIsGoToCamera(BOOLEAN is_go_to_camera)
{
    s_is_go_to_camera = is_go_to_camera;
}

/*Edit by script, ignore 2 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
