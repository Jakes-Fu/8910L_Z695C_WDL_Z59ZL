/*****************************************************************************
** File Name:      mmipicview_menutable.h                                    *
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
#ifndef _MMIPICVIEW_MENUTABLE_H_
#define _MMIPICVIEW_MENUTABLE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_image.h"

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
//menu id
typedef enum
{
    ID_PICVIEW_MENU_START = (MMI_MODULE_PIC_VIEWER << 16),

    ID_PICVIEW_CHANGE_FOLDER,
    ID_PICVIEW_SEND,
    ID_PICVIEW_SLIDE_PREVIEW,
    ID_PICVIEW_SLIDE_SET,
    ID_PICVIEW_VIEW,
    ID_PICVIEW_VIEW_LIST,
    ID_PICVIEW_VIEW_ICONLIST,
    ID_PICVIEW_WALLPAPER,
    ID_PICVIEW_POWER_ON_SET,
    ID_PICVIEW_POWER_OFF_SET,
    ID_PICVIEW_PB,
    ID_PICVIEW_ZOOM,
    ID_PICVIEW_EDIT,
    ID_PICVIEW_MARKED,
    ID_PICVIEW_DELETE,
    ID_PICVIEW_RENAME,
    ID_PICVIEW_DETAIL,

    ID_PICVIEW_MMS,
    ID_PICVIEW_BT,

    ID_PICVIEW_UNMARKED,
    ID_PICVIEW_MARKED_ALL,
    ID_PICVIEW_UNMARKED_ALL,

#ifndef PIC_PREVIEW_U200_SUPPORT
    ID_PICVIEW_SETTING,
#endif
#ifdef SNS_SUPPORT
    ID_PICVIEW_SHARE_TO_SINA,
    ID_PICVIEW_SHARE_TO_FACEBOOK,
    ID_PICVIEW_SHARE_TO_TIWTTER,
#endif
//zhuyongjing add  for gallary ui 20150707
    ID_PICVIEW_SHOW_ALLLOCAL,
    ID_PICVIEW_SHOW_CAPTURED,
    ID_PICVIEW_SHOW_DOWNLOAD,
    ID_PICVIEW_SHOW_VIDEO,
    ID_PICVIEW_SHOW_ONEDRIVE,
    ID_PICVIEW_SET_TO_LOCKSCREEN,
#ifdef MMI_PIC_ONE_DRIVE_SUPPORT    
    ID_PICVIEW_SAVE_TO_ONEDRIVE,
    ID_PICVIEW_ONEDRIVE_SHARE_LINK,
    ID_PICVIEW_ONEDRIVE_DOWNLOAD,
#endif
    ID_PICVIEW_SHARE_ITEMS,
    ID_PICVIEW_SEND_ITEMS_BY_MMS,
    ID_PICVIEW_SEND_ITEMS_BY_BT,
    ID_PICVIEW_SEND_ITEMS_BY_EMAIL,
    ID_PICVIEW_EMAIL,
    ID_PICVIEW_SHARELINK_BY_SMS,
    ID_PICVIEW_SHARELINK_BY_EMAIL,
 //add end
	ID_PICVIEW_MENU_MAX
} MMIPICVIEW_MENU_ID_E;

//zhuyongjing add for gallary ui 20150707
typedef enum
{
    PICVIEW_OPT_ROOT,
    PICVIEW_OPT_SHOW,
    PICVIEW_OPT_DELETE_ITEMS,
    PICVIEW_OPT_SHARE_ITEMS,
    PICVIEW_OPT_SAVE_ITEMS_TO_ONEDRIVE,
    PICVIEW_OPT_SLIDE_PLAY,
    PICVIEW_OPT_ONEDRIVE,
    PICVIEW_OPT_DOWNLOAD,
    PICVIEW_OPT_MARK_ALL,
    PICVIEW_OPT_UNMARK_ALL,
}PICVIEW_OPT_MENU_NODE_E;/*lint !e751*/
//add end

typedef enum
{
    PICVIEW_PICVIEW_OPT_ROOT,
    PICVIEW_PICVIEW_OPT_SEND,
    PICVIEW_PICVIEW_OPT_EDIT,
    PICVIEW_PICVIEW_OPT_DELETE,
    PICVIEW_PICVIEW_OPT_SET_TO_LOCKSCREEN,
    PICVIEW_PICVIEW_OPT_ONEDRIVE_SHARE_LINK,
    PICVIEW_PICVIEW_OPT_ONEDRIVE_DOWNLOAD,
    PICVIEW_PICVIEW_OPT_DETAIL,
    PICVIEW_PICVIEW_OPT_RENAME,
    PICVIEW_PICVIEW_OPT_SAVE_TO_ONEDRIVE,
}PICVIEW_PICVIEW_OPT_MENU_NODE_E;/*lint !e751*/
typedef enum
{
  ONEDRIVE_INDEX_CONNECT = 0,
  ONEDRIVE_INDEX_UPLOAD,
  ONEDRIVE_INDEX_SYNC,
  ONEDRIVE_INDEX_MAX
}MMIPICVIEW_ONEDRIVE_SYNC_INDEX_E;

typedef enum
{
  ONEDRIVE_INDEX_AUTO_UPLOAD = 0,
  ONEDRIVE_INDEX_MANUAL_UPLOAD,
  ONEDRIVE_UPLOAD_INDEX_MAX
}MMIPICVIEW_ONEDRIVE_UPLOAD_INDEX_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,
typedef enum
{
    MMIPICVIEW_MENU_START = (MMI_MODULE_PIC_VIEWER << 16),
    #include "mmipicview_menutable.def"
    MMIPICVIEW_MENU_MAX
} MMIPICVIEW_MENU_LABEL_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register picture viewer menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif //#ifdef PIC_VIEWER_SUPPORT
