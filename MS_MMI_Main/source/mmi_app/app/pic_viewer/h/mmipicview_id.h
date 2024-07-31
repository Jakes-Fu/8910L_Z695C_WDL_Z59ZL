/*****************************************************************************
** File Name:      mmipicview_id.h                                           *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _MMIPICVIEW_ID_H_
#define _MMIPICVIEW_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
//window id
typedef enum
{
    MMIPICVIEW_WIN_ID_START = (MMI_MODULE_PIC_VIEWER << 16),

#include "mmipicview_id.def"
#ifdef PIC_QUICKVIEW_SUPPORT
    WIN_ID_DEF(MMIPICVIEW_QUICKVIEW_WIN_ID)    //quick view win
#endif

    MMIPICVIEW_MAX_WIN_ID
} MMIPICVIEW_WINDOW_ID_E;
#undef WIN_ID_DEF

//control id
typedef enum
{
    MMIPICVIEW_LIST_CTRL_ID = MMIPICVIEW_MAX_WIN_ID,//list ctrl
    MMIPICVIEW_LIST_OPT_MENU_CTRL_ID,               //list popmenu ctrl
    MMIPICVIEW_SHARE_OPT_MENU_CTRL_ID,       //share popmenu ctrl
    MMIPICVIEW_SEL_PICTURE_CTRL_ID,                 //用于设置开关机动画 
    MMIPICVIEW_PREVIEW_ANIM_CTRL_ID,                //preview anim ctrl
    MMIPICVIEW_PREVIEW_SETLIST_CTRL_ID,             //title setting list ctrl
    MMIPICVIEW_ZOOM_ANIM_CTRL_ID,                   //zoom anim ctrl
    MMIPICVIEW_ZOOM_OSD_ANIM_CTRL_ID,               //zoom osd anim ctrl
    MMIPICVIEW_GLIDE1_ANIM_CTRL_ID,                 //preview anim ctrl
    MMIPICVIEW_GLIDE2_ANIM_CTRL_ID,                 //preview anim ctrl
    MMIPICVIEW_GLIDE3_ANIM_CTRL_ID,                 //preview anim ctrl
    MMIPICVIEW_GLIDE4_ANIM_CTRL_ID,                 //preview anim crrl              
    MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,            //preview popmenu ctrl
    MMIPICVIEW_SEL_PICTURE_CTRL1_ID,                 //用于设置开关机动画 
    MMIPICVIEW_SLIDE_PLAY_ANIM_CTRL_ID,             //slide play anim ctrl
    MMIPICVIEW_SET_FORM_CTRL_ID,                    //set form ctrl
    MMIPICVIEW_EFFECT_FORM_CTRL_ID,                 //effect form ctrl
    MMIPICVIEW_EFFECT_LABEL_CTRL_ID,                //effect label ctrl
    MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID,             //effect drop down list ctrl
    MMIPICVIEW_TIME_FORM_CTRL_ID,                   //time form ctrl
    MMIPICVIEW_TIME_LABEL_CTRL_ID,                  //time label ctrl
    MMIPICVIEW_TIME_DROPDOWN_CTRL_ID,               //time drop down list ctrl
    MMIPICVIEW_RENAME_EDITBOX_CTRL_ID,              //rename editbox ctrl
    MMIPICVIEW_DETAIL_RICHTEXT_CTRL_ID,             //detail richtext ctrl  
    
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID,              //total number label ctrl  
#endif

#ifndef PIC_PREVIEW_U200_SUPPORT
    MMIPICVIEW_LIST_MARK_MENU_CTRL_ID,
#else
    MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID,
    MMIPICVIEW_SEL_PICTURE_CTRL2_ID,                 //用于设置开关机动画 
    //MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID,
    MMIPICVIEW_VIEW_SET_LIST_CTRL_ID,
//    MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID,
//    MMIPICVIEW_TIME_SET_LIST_CTRL_ID,
    MMIPICVIEW_OPTION_BUTTON_CTRL_ID,                //option button ctrl
    MMIPICVIEW_RETURN_BUTTON_CTRL_ID,                //returnbutton ctrl

        
//    MMIPICVIEW_PROGRESS_ANIM_CTRL_ID,               //progress gif anim
    MMIPICVIEW_MAINSET_FORM_CTRL_ID,
    
    MMIPICVIEW_FLODER_RETURN_CTRL_ID,
    MMIPICVIEW_LIST_RETURN_CTRL_ID,
    
//    MMIPICVIEW_FLODER_TITLE_CTRL_ID,
//    MMIPICVIEW_LIST_TITLE_CTRL_ID,

    MMIPICVIEW_FLODER_FORM_CTRL_ID,
    MMIPICVIEW_SET_BUTTON_CTRL_ID,

    MMIPICVIEW_LIST_FORM_CTRL_ID,
    MMIPICVIEW_LIST_SLIDE_PREVIEW,
    MMIPICVIEW_LIST_MARKED,

    MMIPICVIEW_MARK_ALLMARK_CTRL_ID,
    MMIPICVIEW_MARK_DELETE_CTRL_ID,
    MMIPICVIEW_MARK_CANCEL_CTRL_ID,

    MMIPICVIEW_PREVIEW_FORM_CTRL_ID,
    MMIPICVIEW_PREVIEW_DELETE_BUTTON_CTRL_ID,
#endif
    MMIPICVIEW_FOLDER_LIST_OPT_MENU_CTRL_ID,
    MMIPICVIEW_CAMERA_BUTTON_CTRL_ID,
    
    MMIPICVIEW_FLODER_TITLE_CTRL_ID,
    MMIPICVIEW_LIST_TITLE_CTRL_ID,
    MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID,
    MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID,
    MMIPICVIEW_TIME_SET_LIST_CTRL_ID,
    //MMIPICVIEW_FOLDER_TITLE_LABEL_CTRL_ID,
    MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID,
    //MMIPICVIEW_PREVIEW_ZOOMIN_BUTTON_CTRL_ID,
   // MMIPICVIEW_PREVIEW_ZOOMOUT_BUTTON_CTRL_ID,
    MMIPICVIEW_PREVIEW_SLIDEPLAY_BUTTON_CTRL_ID,

    MMIPICVIEW_PREVIEW_LABEL_CTRL_ID,
    MMIPICVIEW_PROGRESS_ANIM_CTRL_ID,               //progress gif anim
    
	//MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID,
	PICVIEW_ENTRY_ONEDRIVE_TIP_CTRL_ID,
	PICVIEW_ENTRY_ONEDRIVE_TIP_INFO_CTRL_ID,//zhuyongjing add for tip lsk info
	MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID,
	MMIPICVIEW_ONEDRIVE_LIST_CONNECT_CTRL_ID,
	MMIPICVIEW_ONEDRIVE_LIST_SYNC_CTRL_ID,
	MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID,
    MMIPICVIEW_LIST_EMPTY_CTRL_ID,//zhuyongjing add for empty win 20150722
    MMIPICVIEW_NETWORK_PROGRESS_LABEL_CTRL_ID,
    MMIPICVIEW_NETWORK_PROGRESS_CTRL_ID,
	MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_LABEL_CTRL_ID,//zhuyongjing add for progress bar for uploading
	MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_PRGBOX_CTRL_ID,//zhuyongjing add for progress bar for uploading
    MMIPICVIEW_WAITING_SPINNER_CTRL_ID,//zhuyongjing add for waiting ctrl
    MMIPICVIEW_SHARELINK_WAITING_SPINNER_CTRL_ID,//zhuyongjing add for SHARE LINK waiting ctrl
    MMIPICVIEW_MAX_CTRL_ID
} MMIPICVIEW_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Register picture viewer menu group
//  Global resource dependence : none
//  Author: jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif//#ifdef PIC_VIEWER_SUPPORT