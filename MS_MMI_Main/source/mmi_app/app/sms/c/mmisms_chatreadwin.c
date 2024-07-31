/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef MMI_SMS_CHAT_SUPPORT

#include "mmisms_chatreadwin.h"
#include "mmisms_read.h"
#if defined(MMI_VCARD_SUPPORT)
#include "mmivcard_export.h"
#endif

#if 0
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
//#include "mmiset.h"
#include "mmiset_export.h"
#include "wdp_update_wintab.h"
#endif
#endif
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"

#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmimms_file.h"
#include "mmimms_edit.h"
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
#include "guibutton.h"
#endif
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#include "mmisrvaud_api.h"
#endif
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmicc_export.h"
#include "tb_dal.h"
#include "guilabel.h"
#include "mmipb_export.h"
#include "guimsgbox.h"
#include "mmimms_export.h"
#include "mmimms_id.h"
#include "mmimms_text.h"
#include "mmi_appmsg.h"
#include "mmisms_menutable.h"
#include "mmipub.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmisms_commonui.h"
#include "guiurl.h"
#include "mmi_pubmenuwin.h"
#include "guiform.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmienvset_export.h"
#include "mmidc_export.h"
#include "mmicl_export.h"
#include "mmikl_export.h"
#include "mmiudisk_export.h"
#include "mmiota_id.h"
#include "guirichtext.h"

#include "mmisms_set.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#include "mmisms_appcontrol.h"
#include "mmisms_delete.h"
#include "mmialarm_export.h"
#include "mmisms_receive.h"

//#include "mmifmm.h"
#include "mmifmm_export.h"
#include "mmismsapp_interface.h"
#include "mmicom_banner.h"

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmimms_internal.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmiset_func.h"
#include "mmipicview_text.h"
#ifdef UNISOC_VERSION
#include "mmipb_export.h"
#include "mmiset_func.h"
#ifdef MMI_SUPPORT_CONTROLPANEL
#include "mmicp_export.h"
#endif
#endif

#ifdef HERO_ENGINE_TTS_SUPPORT   
#include "mmitts_export.h"
//MMI_STRING_T	g_hero_msg_content = {0};
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//BEGIN for delete all msg 11/02/18

#define    VCF_CONTENT_TYPE_NAME "text/x-vCard"//should be same as mmimms_edit.c 
#define VCF_CONTENT_TYPE_LEN 12
#define ATTACHMENT_CONTENT_TYPE_DEFAULT_NAME "application/x-octet-stream"//should be same as mmimms_edit.c 
#define ATTACHMENT_CONTENT_TYPE_DEFAULT_LEN 26

#define	VCS_CONTENT_TYPE_NAME "text/x-vCalendar"//should be same as mmimms_edit.c 
#define VCS_CONTENT_TYPE_LEN 16



#define MMISMS_SMS_DATE_WIDTH   100
#define MMISMS_SMS_MIN_TEXT_BG_WIDTH   40

#define MMISMS_PROMPT_STR_LEN           52

#define MMISMS_FILE_SIZE_DISP_LEN 20    
#define MMISMS_MAX_MMS_READED 100

#define MMISMS_CHAT_FOCUS_COLOR         0xFC60
//END
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MMI_WIN_ID_T            win_id;
    uint32                  menu_id;
    MMISMS_CHAT_ITEM_T      chat_item[MMISMS_CHAT_MAX_ITEM];
    //MMI_QUEUEHANDLE_T       operation_queue;
    MMISMS_BCD_NUMBER_T     chat_number;
    uint8                   sender[MMISMS_NUMBER_MAX_LEN+1];//added for mms...
    wchar               title_name[MMISMS_PBNAME_MAX_LEN + 1];
 
    MMISMS_SMSEDIT_CONTENT_T edit_content;
    wchar         edit_string[MMISMS_MAX_MESSAGE_LEN + 2];    //edit_string
    uint32        edit_strlen;
    MN_DUAL_SYS_E   dual_sys;
    MMISMS_READ_MSG_T   read_result;
    MMISMS_ORDER_ID_T       chat_del_order_id;
    MMISMS_ORDER_ID_T       chat_draft_order_id;

    uint16                  del_index;
    BOOLEAN         is_resent_cmd;
    BOOLEAN         is_email_number;
    BOOLEAN         is_closed_by_editMMS;
#ifdef MMIMMS_SMS_IN_1_SUPPORT    
    MMISMS_CHAT_INSERT_GLOBLE_T sms_chat_insert_info;
#endif
    
    BOOLEAN is_del_all;   
    BOOLEAN is_add_contact;
    BOOLEAN is_have_item;
#ifdef SNS_SUPPORT
    IMMISns *pMe;
#endif
    uint16 mms_count;
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr[MMISMS_MAX_MMS_READED];  // for edit doc free issue
}MMISMS_CHATREAD_WINDATA_T;


typedef struct 
{
    uint8 file_type;
    uint8 file_name[MMIMMS_FILE_NAME_LEN + 1];
}MMISMS_VIDEO_PREVIEW_T;


wchar file_save_path[MMIMMS_FILE_NAME_LEN + 1] = {0};
MMIMMS_EDIT_DOCUMENT_T* attach_editdoc_ptr = PNULL;

BOOLEAN is_enter_music_preview = FALSE;
BOOLEAN is_draft_existed = FALSE;
LOCAL BOOLEAN s_is_need_update_list = FALSE;
LOCAL MMI_HANDLE_T g_mms_active_handle = 0;
LOCAL MMI_HANDLE_T g_sms_active_handle = 0;
LOCAL MMI_HANDLE_T g_last_active_handle = 0;
extern MMISMS_GLOBAL_T g_mmisms_global;
extern BOOLEAN is_exit_from_save_draft;

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//MMISMS_DELMSG_GLOBLE_T g_sms_delmsg_all_info = {0};


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL uint16 s_all_del_msg_num = 0;
//LOCAL BOOLEAN s_mms_is_sendrecv = FALSE;
//LOCAL BOOLEAN s_is_mms_del = FALSE;
/**---------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

#ifdef MMI_SMS_MOVE_SUPPORT

/*****************************************************************************/
//  Description : to handle the message of move query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenMoveQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveFinishCallback(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr);

#endif


#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinMoveSecurity(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinResendItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetChatItemDateWidth(void);

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_INDEX_E GetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 *index_ptr,
    MMISMS_ORDER_ID_T order_id
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptOK(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : truncate first line of msg content
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TruncateMsgContent(MMI_STRING_T *msg_content_ptr );

#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif
#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : open the SMS copy query window
//  Global resource dependence :
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenCopyQuery(void);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyFinishCallback(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//  Description : to operate the copy
//  Global resource dependence :
//  Author:feingming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOperateCopy(void);

/*****************************************************************************/
//  Description : to handle the message of copy waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopySimToMe(void);

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopyMeToSim(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleChatWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinShareSNS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinReadTTS(void);
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinInsertOptOK(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : speed dial edit select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EditSelectVcardCallback(MMIPB_HANDLE_T handle);

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description :mms edit insert video
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertVideo(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert vcard
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertVcard(void);

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertMusic(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertPic(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertNormalAttachment(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :聊天处理现拍,现摄,现录流程
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSHandleSnapshot(MMIFILE_FILE_INFO_T *file_info, MMIFMM_FILE_TYPE_E   file_type);

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenRecord(void);

/*****************************************************************************/
//  Description :录音 后 的 处 理
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void SMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr);

#endif

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDC(void);

/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDV(void);

/*****************************************************************************/
//  Description :拍 照 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr);

/*****************************************************************************/
//  Description :录 像 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr);
#endif

/*****************************************************************************/
//  Description : 处理阅读界面来一条相同号码的情况
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ChatWinSetLongSmsItemContent(
                                               MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               );

/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void ChatWinCalcChatItemSplitLenth(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,uint32  index);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptInit(MMIPUB_MENU_NOTIFY_T *param );
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlGotoBrowser(GUITEXT_TAG_INFO_T tag_info);

/*****************************************************************************/
//     Description : handle extract number save contact
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberSaveContact(GUITEXT_TAG_INFO_T tag_info);

/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTagUpdateSoftkey(MMI_CTRL_ID_T ctrl_id);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTag(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id);
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
);
#endif

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param );
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenInsertOpt(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void ChatWinCloseCtrlFunc (MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : opertate after del single chat item
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void UpdateChatItemDisplayAfterDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);


#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description :update readed mms icon 
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_UpdateReadedMMSIcon(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description :create dyna child form for mms
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void SetMMSChatItemParam(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);

/*****************************************************************************/

//  Description :mms attachment list opt win handle fun
//  Global resource dependence : 
//  Author:arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageAttachmentListOptWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              );

/*****************************************************************************/
//  Description : 获取彩信的相关信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_INFO_T *ReadMMSContent(int32 record_id);

/*****************************************************************************/
//  Description : 获取彩信相关部分信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void GetMMSTimeSujectSize(MMI_STRING_T *string_ptr, MMI_STRING_T  *time_str, MMISMS_ORDER_INFO_T *mms_display_info);
#endif

#if 0
/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);
#endif

/*****************************************************************************/
//  Description :set chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 index,
    BOOLEAN is_active,
    MMISMS_ORDER_ID_T order_id
);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelboxAll(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCallItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBNew(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBExist(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBBlackList(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

/*****************************************************************************/
//     Description : to delete all mared message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinDelAllMarkedMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinForwardMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinLockOrUnLockMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//  Description :init all MMS attachments into the given list 
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void AppendAttaListItem(MMI_CTRL_ID_T list_ctrl_id, MMIFMM_FILE_TYPE_E file_type, uint32 atta_index_code, uint32 specific_file_type,
                                                                      uint8 *file_name_ptr, wchar* file_size, wchar* file_suffix,uint8 *multim_buf_ptr, uint32 multim_buf_len, MMI_IMAGE_ID_T img_id);

/*****************************************************************************/
//  Description :init all MMS attachments into the given list 
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void InitMMSAttaViewList(MMI_CTRL_ID_T list_ctrl_id, uint32 record_id,MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description : 号码添加到联系人后需要更新显示
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatReadWinUpdateDispNum(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : 获取聊天显示窗口中某条信息的order_id
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetChatWinItemId(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message enable send
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsSendEnable(BOOLEAN is_update);

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendSms(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Init all controls layout on SMS Create New
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitEditingAreaControls(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display sim card icon
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
LOCAL void SimIconOwnDrawFun(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : display the shortcut indicator for editor
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
//LOCAL void EditShortcutIndicatorOwnDrawFun(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinOpenDelteQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDelQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerSms(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerMMS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptOK(MMIPUB_MENU_NOTIFY_T *param );

/******************************************************************************
//  Description : Update Windata,and save it to global variables.
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMI_HANDLE_T win_id,BOOLEAN is_resend);

/*****************************************************************************/
//     Description : display the owndraw icons on conversation view(i.e. send
//                        fail and mms attachment icon)
//    Global resource dependence :
//    Author: arvin.zhang
//    Note:
/*****************************************************************************/
LOCAL void DisplayMessageViewOwnDrawIcons(GUIOWNDRAW_INFO_T    *draw_ptr);

/*****************************************************************************/
//     Description : display the owndraw icons on conversation view(i.e. send
//                        fail and mms attachment icon)
//    Global resource dependence :
//    Author: arvin.zhang
//    Note:
/*****************************************************************************/
LOCAL void DisplayMMSAttachementIcon(GUIOWNDRAW_INFO_T    *draw_ptr);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN ChatWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data);

/*****************************************************************************/
//     Description : get chat win w number,such as :10086, email@163.com
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetChatWinWNumber(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,wchar *wnumber);

/*****************************************************************************/
//  Description :GetFileSizeDispString
//  Global resource dependence : 
//  Author:rong.gu
//  Note: [in] fils_size   [out]disp_str
/*****************************************************************************/
LOCAL void GetFileSizeDispString(uint32 file_size, wchar *disp_str);
/*****************************************************************************/
//  Description :preview pic
//  Global resource dependence : 
//  Author:arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicturePreviewWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DrawAttachmentListItemWithPicture(
                                        uint16           item_index,     //in:item索引
                                        uint16           content_index,  //in:item内容索引,text or anim etc.
                                        MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                        uint32 record_id 
                                      );

/*****************************************************************************/
// Description : show preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterPreviewPicture (GUIANIM_DATA_INFO_T    *anim_data_ptr);

/*****************************************************************************/
// Description : show preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CleanTempFile(void);

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : MMISMS_VideoPlayCallBack
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_VideoPlayCallBack(VP_CALLBACK_T *msg_ptr);
#endif

#if defined (MMS_SUPPORT) && defined (VIDEO_PLAYER_SUPPORT)
/*****************************************************************************/
//  Description :mms preview richtext play 3gp item 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_PreviewPlayVideoItem(MMISMS_VIDEO_PREVIEW_T*         video_preview_ptr);

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPreviewWin (
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param);

/*****************************************************************************/
//  Description :mms MMISMS_EnterVideoPreviewWin
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_EnterVideoPreviewWin(MMISMS_VIDEO_PREVIEW_T *video_preview);
#endif

/*****************************************************************************/
//     Description : handle sms chat edit opt windows msg
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSMSChatEditMenuOptWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : Create Edit OptionMeny Dynamic menu
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void OpenSMSChatEditOptPopMenu(void);

/*****************************************************************************/
//     Description : set AttachmentList  edit_doc_ptr
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void SetAttachmentListEditDoc(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr );

/*****************************************************************************/
//     Description : get attach list edit doc
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL MMIMMS_EDIT_DOCUMENT_T* GetAttachmentListEditDoc(void);

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:此接口不包括tag的softkey显示
/*****************************************************************************/
LOCAL void  MMISMS_UpdateChatWinSoftkey(MMI_WIN_ID_T        win_id);

/*****************************************************************************/
//     Description : handle mms edit send MMS
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendMMS(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : for chat win email send MMS
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ChatWinSendMMS(uint8 *sender,MMI_STRING_T *edit_string_ptr, MN_DUAL_SYS_E dual_sys);

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 
**---------------------------------------------------------------------------*/
/*lint -save -e785 */
//各语言顺序必须与MMISET_LANGUAGE_TYPE_E的定义顺序相同
extern MMISET_LANG_SET_T s_lang_set[MMISET_MAX_LANGUAGE + 1];
extern BOOLEAN s_lang_T9_status_info[MMISET_MAX_LANGUAGE+1][2];

/*lint -save -e785 */
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
#ifdef MMIMMS_SMS_IN_1_SUPPORT
WINDOW_TABLE( MMISMS_SHOW_CHAT_SMS_WIN_TAB ) =
{
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif

    WIN_FUNC((uint32)HandleShowChatSmsWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_WIN_ID),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    WIN_TITLE(TXT_NULL),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),     
    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    //CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID),
    //CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    //form1-2(container of editor and sim-icon)
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
        //form1-2-1(container of text editor)
         CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISMS_SMS_EDIT_CONTAINER_FORM_ID, MMISMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),
             //form1-2-1-1(editor_main_body(text_form + shortcut_form))
             CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISMS_SMS_EDIT_BODY_FORM_ID, MMISMS_SMS_EDIT_CONTAINER_FORM_ID),
                 CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMISMS_SMS_EDIT_TEXT_FORM_ID, MMISMS_SMS_EDIT_BODY_FORM_ID),
                     CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_DEF_BYTE_LEN,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SMS_EDIT_TEXT_FORM_ID),
#if 0                    
               CHILD_OWNDRAW_CTRL(FALSE, MMISMS_SMS_EDIT_SHORTCUT_INDICATOR_CTL_ID, MMISMS_SMS_EDIT_BODY_FORM_ID, (uint32)EditShortcutIndicatorOwnDrawFun),
             CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMISMS_SMS_EDIT_UNDER_BACKGROUND_LABEL_CTL_ID, MMISMS_SMS_EDIT_CONTAINER_FORM_ID),
#endif
    //form1-2-2(container of sim-icon)
        //CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMISMS_SMS_EDIT_SIM_AREA_FORM_ID, MMISMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),
            //CHILD_OWNDRAW_CTRL(FALSE, MMISMS_SMS_EDIT_SIM_OWNDRAW_CTL_ID, MMISMS_SMS_EDIT_SIM_AREA_FORM_ID,  (uint32)SimIconOwnDrawFun),


#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMM_INSERT, TXT_NULL, TXT_SEND, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#endif

    END_WIN
};
#else
WINDOW_TABLE( MMISMS_SHOW_CHAT_SMS_WIN_TAB ) =
{
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif

    WIN_FUNC((uint32)HandleShowChatSmsWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID),
    END_WIN
};
#endif
#ifdef MMS_SUPPORT
WINDOW_TABLE( MMISMS_ATTA_OPT_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMessageAttachmentListOptWinMsg),
        WIN_ID(MMISMS_ATTA_OPT_LIST_WIN_ID),
        WIN_STATUSBAR,
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_MMS_ATTACHMENT_LIST_CTRL_ID),
        WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, STXT_SOFTKEY_OPEN_MK, TXT_COMMON_COMMON_BACK),
        END_WIN
};
#endif
//the pictures preview window
WINDOW_TABLE (MMISMS_PIC_PREVIEW_WIN_TAB) =
{
    //WIN_TITLE (TXT_NULL), //BUG 492829
    WIN_FUNC ((uint32)HandlePicturePreviewWin),
    WIN_ID (MMISMS_PICTURE_PREVIEW_WIN_ID),
    WIN_STYLE (WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL (MMISMS_PICTURE_PREVIEW_CTRL_ID, MMISMS_PICTURE_PREVIEW_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

#if defined (MMS_SUPPORT) && defined (VIDEO_PLAYER_SUPPORT)
//the video preview window
WINDOW_TABLE (MMISMS_VIDEO_PREVIEW_WIN_TAB) =
{
    WIN_FUNC ((uint32)HandleVideoPreviewWin),
    WIN_ID (MMISMS_VIDEO_PREVIEW_WIN_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMISMS_EDIT_MAINMENU_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleSMSChatEditMenuOptWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN ChatWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    MMISMS_READ_MSG_T *read_msg = PNULL;

    if(PNULL == callback_data)
    {
       return TRUE;
    }
    
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }

    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))  //此窗口如已关闭，则win_data_ptr已被释放
    {
        read_msg = MMISMS_GetReadResult();
        memcpy(&win_data_ptr->read_result, read_msg, sizeof(MMISMS_READ_MSG_T));
        if(callback_data->result)
        {
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            order_id = MMISMS_GetChatOperOrderID();
            MMK_SendMsg(
                        (MMI_HANDLE_T)win_data_ptr->win_id,
                        MSG_SMS_FINISH_READ,
                        (ADD_DATA)order_id               
                       ); 
        }
        else
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
            MMISMS_ClearOperInfo();
        }  
    }
      
    return TRUE;
}


/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SAVE(MN_DUAL_SYS_E dual_sys, MMISMS_CHATREAD_WINDATA_T * windata_p)
{
    SMS_SAVE_OPERATE_DATA_T op_data={0};
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    MMISMS_ORDER_ID_T draft_order_id = PNULL;

    op_data.dual_sys = dual_sys;
    op_data.is_needLoadSaved = FALSE;
    op_data.sms_addr = &windata_p->edit_content.dest_info;
    op_data.save_strlen = windata_p->edit_strlen;
    op_data.save_string_ptr = windata_p->edit_string;

    current_order_id = windata_p->chat_draft_order_id;

    if((current_order_id != PNULL) && (MMISMS_MO_DRAFT == current_order_id->flag.mo_mt_type))
    {
        MMISMS_SetSMSUpdateInfo(current_order_id);
        MMISMSAPP_ReplaceSms(op_data,PNULL,PNULL,PNULL);
    }
    else
    {
		/*
    	   if(is_draft_existed)
    	   {
    	   	//delete the existed draft
    	   	uint16 index = 0; 
		
    	   	MMISMS_CHAT_ITEM_T chat_item = windata_p->chat_item[0];
		while((PNULL != chat_item.cur_order_id) && (index < MMISMS_CHAT_MAX_ITEM))
		{
			if(MMISMS_MO_DRAFT == chat_item.cur_order_id->flag.mo_mt_type)
			{
				draft_order_id = chat_item.cur_order_id;
				break;
			}
		}
    
		if(MMISMS_TYPE_SMS == draft_order_id->flag.msg_type)
		{
			MMISMSAPP_DeleteSms(draft_order_id,PNULL,PNULL,PNULL);
		}
	 	else if(MMISMS_TYPE_MMS == draft_order_id->flag.msg_type)
	 	{
			if (!MMIAPIUDISK_UdiskIsRun())
			{                     
			 MMIAPIMMS_DeleteMMS(draft_order_id->record_id,PNULL);
			}	 	
	 	}
		//MMISMS_MarkAndRemoveDraftByOrderId(draft_order_id->record_id);
    	   }
		   */
        MMISMSAPP_SaveSms(op_data,PNULL,PNULL,PNULL);
    }

}


/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/\
LOCAL void SMS_SEND(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_handle);

    if (win_data_ptr == PNULL)
    {
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif

    MMISMSAPP_SendSmsWithData(dual_sys,
            win_data_ptr->is_resent_cmd,
            &win_data_ptr->edit_content.dest_info,
            win_data_ptr->edit_string,
            win_data_ptr->edit_strlen,
            PNULL
            );
}

/*****************************************************************************/
//  Description : for chat win email send MMS
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_ChatWinSendMMS(uint8 *sender,MMI_STRING_T *edit_string_ptr, MN_DUAL_SYS_E dual_sys)
{
#if defined (MMS_SUPPORT)
    MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
    MMIMMS_EDIT_DOCUMENT_T *   editdoc_ptr = PNULL;    
    MMI_STRING_T  string_to = {0}; 
    wchar  address_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    string_to.wstr_ptr = address_wstr;
    
    MMIMMS_InitData(); 
    editdoc_ptr = MMIMMS_CreateEditDocument();
    if(PNULL == editdoc_ptr)
    {
        return;
    }
    mms_op_result = MMIMMS_AddSlide(editdoc_ptr,1);
    if (MMIMMS_NO_ERROR != mms_op_result)
    {
        MMIMMS_EditMMSAlertWin(mms_op_result);
        return;
    }              
    MMIAPICOM_StrToWstr(sender, string_to.wstr_ptr);
    string_to.wstr_len = MMIAPICOM_Wstrlen(string_to.wstr_ptr);
    MMIMMS_SetMMSTo(&string_to,editdoc_ptr);
    SCI_MEMSET( editdoc_ptr->wstr_ptr, 0,  sizeof( editdoc_ptr->wstr_ptr ));
    
    if(edit_string_ptr->wstr_len > 0)
    {        
        MMI_WSTRNCPY(editdoc_ptr->wstr_ptr, MMIMESSAGE_DIAPLAY_LEN, edit_string_ptr->wstr_ptr, MMIMESSAGE_DIAPLAY_LEN, MMIMESSAGE_DIAPLAY_LEN);
    }
    MMIMMS_AddText(editdoc_ptr, edit_string_ptr);
  
    MMIMMS_SendNewMMS(dual_sys, editdoc_ptr);
    MMSMMI_SetSendNewMMSId(editdoc_ptr->record_id);
        
    MMIMMS_MMSEditExit();
#endif
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T win_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        SMS_SEND(dual_sys,win_handle);
        //Don't need close chat read win
    }
    return recode;
}

/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendMMSSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T edit_string = {0};
    MMI_CTRL_ID_T    ctrl_id = MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID;

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if (PNULL == win_data_ptr)
    {
        return recode;
    }
    
    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString(ctrl_id, &edit_string);
        MMISMS_ChatWinSendMMS(win_data_ptr->sender,&edit_string,dual_sys);
        GUIEDIT_ClearAllStr(ctrl_id);
    }
    return recode;
}
#endif

/******************************************************************************
//  Description : Update Windata,and save it to global variables.
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMI_HANDLE_T win_id,BOOLEAN is_resend_msg)
{
    MMI_STRING_T edit_string = {0};
    MMI_CTRL_ID_T ctrl_text_id = MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID;

    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
#ifdef UNISOC_VERSION
	MMIPB_PREFERRED_SIM_TYPE_T prefer_sim = MMIPB_PREFERRED_SIM_NONE;
	MMIPB_BCD_NUMBER_T bcd_number = {0};
	MMI_STRING_T   name_str		 = {0};
    wchar  str_buf[CC_MAX_PBNAME_LEN + 1] = {0};
	name_str.wstr_ptr = str_buf;
#endif	
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);

    win_data_ptr->is_resent_cmd = is_resend_msg;
    SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
    GUIEDIT_GetString(ctrl_text_id, &edit_string);
    // to save the information
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if (MMISMS_GetAutoSignatureFlag() && !MMIAPIMMS_GetIsMMSMode() &&!win_data_ptr->is_resent_cmd)
    {
        MMI_STRING_T sig_string = {0};
        MMI_STRING_T display_string = {0};
        wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
        uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        signature_wstr[0] = 0x0a00;
#else
        signature_wstr[0] = 0x000a;
#endif
        signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;

        sig_string.wstr_ptr = signature_wstr;
        sig_string.wstr_len = signature_str_len;

        display_string.wstr_ptr = (wchar *)SCI_ALLOCAZ( ( MMISMS_MAX_MESSAGE_LEN + 1 ) * sizeof(wchar) );

        if (PNULL == display_string.wstr_ptr)
        {
            return;
        }

        MMIAPICOM_MergeTwoString(
            &edit_string,
            &sig_string,
            &display_string,
            MMISMS_MAX_MESSAGE_LEN
        );

        win_data_ptr->edit_strlen = display_string.wstr_len;
        SCI_MEMCPY(win_data_ptr->edit_string,display_string.wstr_ptr,sizeof(wchar)*win_data_ptr->edit_strlen);

        SCI_FREE(display_string.wstr_ptr);
    }
    else
#endif
#endif
    {
        SCI_MEMCPY(win_data_ptr->edit_string,edit_string.wstr_ptr,sizeof(wchar)*edit_string.wstr_len);
        win_data_ptr->edit_strlen = edit_string.wstr_len;

    }
    num_len = MMIAPICOM_GenDispNumber(
                  MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                  MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                  win_data_ptr->chat_number.number,
                  number,
                  MMISMS_PBNUM_MAX_LEN + 2
              );
    MMISMS_SetAddressToMessage((uint8 *)number, (uint8)strlen((char *)number), &win_data_ptr->edit_content.dest_info.dest_list);    
    MMISMS_SetMessageContent(win_data_ptr->edit_strlen, win_data_ptr->edit_string, TRUE, &win_data_ptr->edit_content);
#ifdef UNISOC_VERSION
	MMIAPIPB_StringToBCDNumber(number, &bcd_number);
	MMIAPIPB_GetNamePreferredSimByNumber(&bcd_number, &name_str, &prefer_sim, 10, TRUE);
	MMIAPISET_SetPreferSimFromPB(prefer_sim);
#endif
              
}

/*****************************************************************************/
//     Description : display the owndraw icons on conversation view(i.e. send
//						fail and mms attachment icon)
//    Global resource dependence :
//    Author: arvin.zhang
//    Note:
/*****************************************************************************/
LOCAL void DisplayMessageViewOwnDrawIcons(GUIOWNDRAW_INFO_T    *draw_ptr)
{
	uint16 icon_width = 0;
	uint16 icon_height = 0;

	MMI_WIN_ID_T    win_id=0;
	GUI_RECT_T	form_display_rect = {0};
	GUI_RECT_T	icon_display_rect = {0};
	GUI_POINT_T	icon_display_point = {0};

	//we should solve the problem of distinguish the two scenario:
	// send fail case(SF)
	// mms attachment case(MA)
	// now I adopt the ugly temp solution: display rect
	// this is because SF is only existing on the type MO and MA is only existing on the type MT
	// Oh Oh Oh, A bright idea occurs...
	// I think we'd better use the button/label ctrl rather than owndraw so that we can reduce the effort greatly.[!!!! Perfect]
	
	
	MMI_IMAGE_ID_T	fail_icon = IMAGE_MESSAGING_ERROR_BLUE_MINUS_2;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	//should add icon selection code here based on current system theme
	// fail_icon = balabala...
	win_id = MMK_GetWinId(draw_ptr->win_handle);
	GUIRES_GetImgWidthHeight(&icon_width, &icon_height, fail_icon, win_id);
	GUIOWNDRAW_GetDisplayRect(draw_ptr->ctrl_handle, &form_display_rect);

	icon_display_point.x = form_display_rect.left + 6;
	icon_display_point.y = form_display_rect.bottom - 12 - icon_height;
    GUIRES_DisplayImg(
            &icon_display_point,
            PNULL,
            PNULL,
            win_id,
            fail_icon,
            (const GUI_LCD_DEV_INFO*)&lcd_dev_info);
}

/*****************************************************************************/
//     Description : display the owndraw icons on conversation view(i.e. send
//						fail and mms attachment icon)
//    Global resource dependence :
//    Author: arvin.zhang
//    Note:
/*****************************************************************************/
LOCAL void DisplayMMSAttachementIcon(GUIOWNDRAW_INFO_T    *draw_ptr)
{
	uint16 icon_width = 0;
	uint16 icon_height = 0;

	MMI_WIN_ID_T    win_id=0;
	GUI_RECT_T	form_display_rect = {0};
	GUI_RECT_T	icon_display_rect = {0};
	GUI_POINT_T	icon_display_point = {0};	
	
	MMI_IMAGE_ID_T	attachment_icon = IMAGE_MESSAGING_ATTACHMENT_ICON;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	win_id = MMK_GetWinId(draw_ptr->win_handle);
	GUIRES_GetImgWidthHeight(&icon_width, &icon_height, attachment_icon, win_id);
	GUIOWNDRAW_GetDisplayRect(draw_ptr->ctrl_handle, &form_display_rect);

	icon_display_point.x = form_display_rect.left + 6;
	icon_display_point.y = form_display_rect.bottom - 12 - icon_height;
    GUIRES_DisplayImg(
            &icon_display_point,
            PNULL,
            PNULL,
            win_id,
            attachment_icon,
            (const GUI_LCD_DEV_INFO*)&lcd_dev_info);	
}

/*****************************************************************************/
//     Description : handle mms edit send MMS
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendMMS(MMI_WIN_ID_T win_id)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_STRING_T edit_string = {0};
    MMI_CTRL_ID_T    ctrl_id = MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID;

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);
    SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
    GUIEDIT_GetString(ctrl_id, &edit_string);
   // open the input number window
    if (0 < win_data_ptr->chat_number.number_len)
    {
        // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        uint16 sim_num = 0;
        uint16 sim_ok = 0;

        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            win_data_ptr->dual_sys = (MN_DUAL_SYS_E)sim_ok;            
            MMISMS_ChatWinSendMMS(win_data_ptr->sender, &edit_string, win_data_ptr->dual_sys);
            GUIEDIT_ClearAllStr(ctrl_id);            
        }
        else
        {
            if (win_data_ptr->dual_sys == MN_DUAL_SYS_MAX)
            {
#if defined UNISOC_VERSION && defined MMI_SUPPORT_CONTROLPANEL
                PREFERRED_SIM_E prefer_sim = MMIAPICP_GetPreferredSimState();

                SCI_TRACE_LOW("mmisms: prefer_sim = %d", prefer_sim);

                if (ALWAYS_ASK == prefer_sim)
                {
                    MMIPB_BCD_NUMBER_T pb_num = {0};
                    MN_DUAL_SYS_E cur_chat_sim_id = MMI_DUAL_SYS_MAX;
                    MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

                    pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                    cur_chat_sim_id = MMISMS_GetCurChatSIMId(pb_num);

                    major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

                    if (cur_chat_sim_id < MMI_DUAL_SYS_MAX
                        && MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS))
                    {
                        win_data_ptr->dual_sys = cur_chat_sim_id;
                    }
                    else if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                    {
                        win_data_ptr->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                    }
                    else
                    {
                MMISMS_SelectSIMFunc(SendMMSSelectSimCallback,(ADD_DATA)win_id,TRUE);
                return;
                }
                }
                else
                {
                SCI_TRACE_LOW("mmisms: get prefer sim from pb = %d", MMIAPISET_GetPreferSimFromPB());

                switch(MMIAPISET_GetPreferSimFromPB())
                {
                case MMIPB_PREFERRED_SIM_NONE:
                    win_data_ptr->dual_sys = prefer_sim; 
                    break;

                case MMIPB_PREFERRED_SIM1:
                    if (prefer_sim == (MMIPB_PREFERRED_SIM1 - 1))
                    {
                        win_data_ptr->dual_sys = prefer_sim;
                    }
                    else
                    {
                        MMISMS_SelectSIMFunc(SendMMSSelectSimCallback,(ADD_DATA)win_id,TRUE);
                        return;
                    }
                    break;

                case MMIPB_PREFERRED_SIM2:
                  if (prefer_sim == (MMIPB_PREFERRED_SIM2 - 1))
                  {
                    win_data_ptr->dual_sys = prefer_sim;
                  } 
                  else
                  {
                    MMISMS_SelectSIMFunc(SendMMSSelectSimCallback,(ADD_DATA)win_id,TRUE);
                    return;
                  }
                          break;

                      default:

                          break;
                     }
                }
            }
#else
            MMIPB_BCD_NUMBER_T pb_num = {0};
            MN_DUAL_SYS_E cur_chat_sim_id = MMI_DUAL_SYS_MAX;
            MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            cur_chat_sim_id = MMISMS_GetCurChatSIMId(pb_num);

            major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

            if (cur_chat_sim_id < MMI_DUAL_SYS_MAX
                && MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS))
            {
                win_data_ptr->dual_sys = cur_chat_sim_id;
            }
            else if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            {
                win_data_ptr->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
            }
            else
            {
#ifdef MMI_SMS_FIX_SEND_SIM
                if (MMISMS_GetFixSIMSendingOnOFF())
                {
                    win_data_ptr->dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
                }
                else
#endif
                {                            
                    MMISMS_SelectSIMFunc(SendMMSSelectSimCallback,(ADD_DATA)win_id,TRUE);
                    return;
                }
            }
        }
        else
        {
            win_data_ptr->dual_sys = MMISMS_GetCurSendDualSys();
        }
#endif

            MMISMS_ChatWinSendMMS(win_data_ptr->sender, &edit_string, win_data_ptr->dual_sys);
            GUIEDIT_ClearAllStr(ctrl_id);
        }
#else
        if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else
        {                    
            MMISMS_ChatWinSendMMS(win_data_ptr->sender, &edit_string, win_data_ptr->dual_sys);
            GUIEDIT_ClearAllStr(ctrl_id);
        }
#endif
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}


/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendSms(MMI_WIN_ID_T win_id)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(win_data_ptr->is_email_number)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return;
    }
   // open the input number window
    if (0 < win_data_ptr->chat_number.number_len)
    {
        // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        uint16 sim_num = 0;
        uint16 sim_ok = 0;

        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            win_data_ptr->dual_sys = (MN_DUAL_SYS_E)sim_ok;

            SMS_SEND(win_data_ptr->dual_sys,win_id);
        }
        else
        {
            if (win_data_ptr->dual_sys == MN_DUAL_SYS_MAX)
            {
#if defined UNISOC_VERSION && defined MMI_SUPPORT_CONTROLPANEL
                PREFERRED_SIM_E prefer_sim = MMIAPICP_GetPreferredSimState();

                SCI_TRACE_LOW("mmisms: prefer_sim = %d", prefer_sim);

                if (ALWAYS_ASK == prefer_sim)
                {
                        MMIPB_BCD_NUMBER_T pb_num = {0};
                        MN_DUAL_SYS_E cur_chat_sim_id = MMI_DUAL_SYS_MAX;
                        MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

                        pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                        cur_chat_sim_id = MMISMS_GetCurChatSIMId(pb_num);

                        major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

                        if (cur_chat_sim_id < MMI_DUAL_SYS_MAX
                            && MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS))
                        {
                            win_data_ptr->dual_sys = cur_chat_sim_id;
                        }
                        else if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                        {
                            win_data_ptr->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                        }
                        else
                        {
                             MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_id,TRUE);
                             return;
                        }
                }
                else
                {
                     SCI_TRACE_LOW("mmisms: get prefer sim from pb = %d", MMIAPISET_GetPreferSimFromPB());

                     switch(MMIAPISET_GetPreferSimFromPB())
                    {
                        case MMIPB_PREFERRED_SIM_NONE:
                            win_data_ptr->dual_sys = prefer_sim; 
                            break;

                        case MMIPB_PREFERRED_SIM1:
                            if (prefer_sim == (MMIPB_PREFERRED_SIM1 - 1))
                            {
                                win_data_ptr->dual_sys = prefer_sim;
                            }
                            else
                            {
                                MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_id,TRUE);
                                return;
                            }
                            break;

                        case MMIPB_PREFERRED_SIM2:
                            if (prefer_sim == (MMIPB_PREFERRED_SIM2 - 1))
                            {
                                win_data_ptr->dual_sys = prefer_sim;
                            } 
                            else
                            {
                                MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_id,TRUE);
                                return;
                            }
                          break;

                      default:

                          break;
                     }
                }
            }
#else
                MMIPB_BCD_NUMBER_T pb_num = {0};
                MN_DUAL_SYS_E cur_chat_sim_id = MMI_DUAL_SYS_MAX;
                MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

                pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                cur_chat_sim_id = MMISMS_GetCurChatSIMId(pb_num);

                major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

                if (cur_chat_sim_id < MMI_DUAL_SYS_MAX
                    && MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS))
                {
                    win_data_ptr->dual_sys = cur_chat_sim_id;
                }
                else if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    win_data_ptr->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                }
                else
                {
#ifdef MMI_SMS_FIX_SEND_SIM
                    if (MMISMS_GetFixSIMSendingOnOFF())
                    {
                        win_data_ptr->dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
                    }
                    else
#endif
                    {                            
                        MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_id,TRUE);
                        return;
                    }
                }
            }
            else
            {
                win_data_ptr->dual_sys = MMISMS_GetCurSendDualSys();
            }
#endif

            SMS_SEND(win_data_ptr->dual_sys,win_id);
        }
#else
        if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else
        {
            SMS_SEND(MN_DUAL_SYS_1,win_id);
        }
#endif
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ChatReadWinNewEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    MMIPB_BCD_NUMBER_T mt_num = {0};
    MN_SMS_MT_SMS_T *mt_sms_ptr = PNULL;
    BOOLEAN is_num_compare = FALSE;
    MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_MAX;    
    MN_SMS_STORAGE_E        storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        record_id = 0;
    MMI_WIN_ID_T win_id = MMISMS_SHOW_CHAT_SMS_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
            
    if(PNULL == event_info_ptr || PNULL == win_data_ptr)
    {
        return;
    }
    new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;
    if(PNULL == new_sms_info_ptr)
    {
        SCI_TRACE_LOW("mmisms new_sms_info_ptr PNULL");
        return;
    }
    dual_sys = new_sms_info_ptr->dual_sys;
    storage = new_sms_info_ptr->storage;
    record_id = new_sms_info_ptr->record_id;
    SCI_TRACE_LOW("mmisms ChatReadWinNewEventCallback: dual_sys=%d, storage=%d, record_id=%d", dual_sys, storage, record_id);
    
    mt_sms_ptr = &new_sms_info_ptr->mt_sms;
    mt_num.number_len = mt_sms_ptr->origin_addr_t.num_len;
    mt_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(mt_sms_ptr->origin_addr_t.number_type,MN_NUM_PLAN_UNKNOW);
    SCI_MEMCPY(
               mt_num.number,
               mt_sms_ptr->origin_addr_t.party_num,
               mt_num.number_len
               ); 

    if(0 == MMIAPIPB_BcdSmartCmp(&mt_num,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
    {
        is_num_compare = TRUE;
    }

    if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && is_num_compare
      )
    {               
        // to modify the status from to_be_read to READ
        MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);

        //MNSMS_ReadSmsEx(dual_sys, storage, record_id);
        MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_MT_READED);
        MMISMS_SetNeedUpdateInfo();
        if(new_sms_info_ptr->cur_chat_order_id)
        {
            new_sms_info_ptr->cur_chat_order_id->flag.is_need_update = TRUE;
        }
        
        if (MMIAPIKL_IsPhoneLocked() && MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
        {
            MMISMS_SetIsKeyLockNeedCountSms(TRUE);
        }
        else
        {
            MMISMS_SetIsKeyLockNeedCountSms(FALSE);
        }
        
    }
    else
    {
        MMISMS_SetIsKeyLockNeedCountSms(FALSE);
    }   
    if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && is_num_compare
      )
    {
        MMISMS_READ_MSG_T *read_msg = PNULL;

        MMISMS_ChatSetMTGlobalReadMsg(
            dual_sys,
            &new_sms_info_ptr->user_data,    //IN:
            storage,        //IN:
            record_id,        //IN:
            mt_sms_ptr->dcs.alphabet_type,
            &mt_sms_ptr->time_stamp_t,
            &mt_sms_ptr->origin_addr_t,
            MMISMS_MT_HAVE_READ
            ); 

        read_msg = MMISMS_GetReadResult();
        memcpy(&win_data_ptr->read_result, read_msg, sizeof(MMISMS_READ_MSG_T));
        if((new_sms_info_ptr->cur_chat_order_id != PNULL) && ((new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms && new_sms_info_ptr->cur_chat_order_id->flag.is_receive_all)
            || (!new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms))
        )
        {
            MMIDEFAULT_TurnOnBackLight();
            // save the flag
            MMISMS_SetNewSMSRingFlag(TRUE);
            //MMISMS_SetSimIdOfNewMsg(dual_sys);
            MMISMS_PlayNewSMSRing(new_sms_info_ptr->dual_sys);
        }
            
        if (MMIAPIKL_IsPhoneLocked() && MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
        {
            MMIAPIKL_SetNewSMSStatus();
            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNREAD_MESSAGE,PNULL,0);
        }
       
        MMK_SendMsg(
                    MMISMS_SHOW_CHAT_SMS_WIN_ID,
                    MSG_SMS_NEW_SMS,
                    (ADD_DATA)new_sms_info_ptr->cur_chat_order_id
                    ); 
    }
      
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ChatReadWinDataChangeCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_SHOW_CHAT_SMS_WIN_ID;
    MMISMS_ORDER_ID_T chat_add_order_id = PNULL;
    MMISMS_DATACHANGE_TYPE_E *data_change_ptr = PNULL;    
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
    BOOLEAN is_num_compare = FALSE;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
       data_change_ptr = (MMISMS_DATACHANGE_TYPE_E *)event_info_ptr->param_ptr;
       if(MMISMS_DATACHANGE_TYPE_ADD == *data_change_ptr) 
       {       
           chat_add_order_id = MMISMS_GetChatAddOrderID();
           if(chat_add_order_id != PNULL)
           {
               if(0 == MMIAPIPB_BcdSmartCmp((MMIPB_BCD_NUMBER_T *)&chat_add_order_id->number.bcd_number,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
               {
                   MMISMS_READ_MSG_T *read_msg = PNULL;

                   is_num_compare = TRUE;
                   read_msg = MMISMS_GetReadResult();
                   memcpy(&win_data_ptr->read_result, read_msg, sizeof(MMISMS_READ_MSG_T));
               }
                              
               if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                   && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
                   && is_num_compare
                   && (chat_add_order_id->flag.folder_type != MMISMS_FOLDER_SECURITY)
                 )
               {    
                   MMK_SendMsg(
                        win_id,
                        MSG_SMS_NEW_SMS,
                        (ADD_DATA)chat_add_order_id
                        );        
               }
               MMISMS_SetChatAddOrderID(PNULL);
           }
       }
       else if(MMISMS_DATACHANGE_TYPE_DELETE == *data_change_ptr)
       {
           if(!MMK_IsOpenWin(MMISMS_DELETE_ANIM_WIN_ID))
           {
               MMK_SendMsg(win_id, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容 
           }
           else
           {
               s_is_need_update_list = TRUE;
           }
       }
    }      
}
LOCAL void DestroyMMSEditDoc(MMISMS_CHATREAD_WINDATA_T *win_data_ptr) //must destory edit docment
{
#if defined (MMS_SUPPORT)
    uint16 i = 0;
    for(i = 0; i < win_data_ptr->mms_count; i++)
    {
         MMIMMS_DestroyEditDocument(win_data_ptr->edit_doc_ptr[i]);
    }
#endif
}
/*****************************************************************************/
//	Description : show file too large banner
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void ShowFileLargeBanner(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T result_str = {0};
    wchar result_wchar[200] = {0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    GUI_POINT_T start_point = {0, 0};
    banner_info.banner_type = MMI_BANNER_STR_TEXT;
    banner_info.start_point = start_point;    
    result_str.wstr_ptr = result_wchar;
    MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_MSG_ATTACHMENT_BIG_EXT01, 
    L"%U", 
    MMIAPIMMS_GetMMSLimitSize()/1024,
    200, 
    &result_str);
    banner_info.text1_str = result_str;
    MMIAPICOM_OpenBannerWin(win_id, banner_info);
}

PUBLIC void MMISMS_UpdateChatItemOrder(MMISMS_ORDER_ID_T old_order_id,MMISMS_ORDER_ID_T new_order_id)
{
        MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        uint16 i = 0;
        if(PNULL == win_data_ptr)
        {
            return;
        }
        for(i = 0;i<MMISMS_CHAT_MAX_ITEM;i++)
        {
            if(win_data_ptr->chat_item[i].is_valid && win_data_ptr->chat_item[i].cur_order_id == old_order_id)
            {               
                win_data_ptr->chat_item[i].cur_order_id = new_order_id;
                break;
            }
        }                    
}
/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:此接口不包括tag的softkey显示
/*****************************************************************************/
LOCAL void  MMISMS_UpdateChatWinSoftkey(MMI_WIN_ID_T        win_id)
{    
    MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
    MMISMS_ORDER_ID_T oper_order_id = PNULL;

    if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID == active_ctrl_id)
    {
        MMI_STRING_T input_str_ptr = {0};
        GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &input_str_ptr);
        if(input_str_ptr.wstr_len > 0)
        {
            //GUIEDIT_SetSoftkeyEx(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 2, 0, TXT_NULL, TXT_NULL, IMAGE_COMMON_BACKSPACE, IMAGE_NULL, EDIT_BUTTON_IMAGE_ID,  PNULL);                                    
            //GUIWIN_SeSoftkeytButtonIconId(win_id,(MMI_IMAGE_ID_T)IMAGE_COMMON_BACKSPACE, RIGHT_BUTTON,FALSE);
            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
        }
        else
        {
            //GUIEDIT_SetSoftkeyEx(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 2, 0, TXT_NULL, TXT_NULL, IMAGE_COMMON_BACK, IMAGE_NULL, EDIT_BUTTON_IMAGE_ID,  PNULL);
            //GUIWIN_SeSoftkeytButtonIconId(win_id,(MMI_IMAGE_ID_T)IMAGE_COMMON_BACK, RIGHT_BUTTON,FALSE);
            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  TRUE);
        }                
    }
    else if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
             && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        oper_order_id = GetChatWinItemId(win_data_ptr);
        if (oper_order_id == PNULL)
        {
            return;
        }
        switch (oper_order_id->flag.msg_type)
        {
            case MMISMS_TYPE_SMS:
                 if(MMISMS_MO_SEND_FAIL == oper_order_id->flag.mo_mt_type)
                 {
                     if( !MMISMS_IsSmsBusy(oper_order_id))
                     {
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_RESEND_MK, MIDDLE_BUTTON, TRUE);
                     }
                     else
                     {
                         GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
                     }
                     GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
                 }
                 else
                 {
                     GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL,TXT_NULL , TRUE);
                 }                       
                break;

            case MMISMS_TYPE_MMS:
//we should judge the status and update the downloading process info if current MMS is on downloading status
                if (MMISMS_MT_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type || MMISMS_MT_NOT_NOTIFY == oper_order_id->flag.mo_mt_type || MMISMS_MT_NEED_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type )
                {
                        if (MMIAPIMMS_IsProcessMMSById(oper_order_id->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(oper_order_id->record_id))//如果是正在下载
                        {
                            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, FALSE);
                        }
                        else
                        {               
                            GUIWIN_SeSoftkeytButtonTextId(win_id, STR_SOFTKEY_DOWNLOAD_EXT02, MIDDLE_BUTTON, FALSE);
                        }
                }
                else if (MMISMS_MO_SEND_FAIL == oper_order_id->flag.mo_mt_type)
                {
                        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_RESEND_MK, MIDDLE_BUTTON, TRUE);
                        if(MMIAPIMMS_IsProcessMMSById(oper_order_id->record_id))
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  TRUE);     
                        }  
                        else
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);     
                        }
                }
                else
                {
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
                }
            
                GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);

                break;

            default:
                break;
        }
        GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK, LEFT_BUTTON,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK, RIGHT_BUTTON,TRUE);
    }
    GUIWIN_UpdateSoftkey(win_id);
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
#ifdef HERO_ENGINE_TTS_SUPPORT 
static MMI_CTRL_ID_T gHeroTtsSmsListCtrlId = 0xFFFFFFFF;
#endif
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
	GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T name = {0};
    GUISTBDATA_TEXT_T stb_data_text = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint32 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_ORDER_ID_T oper_order_id = PNULL;
	MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID;
    MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
    uint32 real_file_size                          = 0;
    uint32 insert_total_size                       = 0;
    uint16 ver_space_ptr						= 6;
    GUIANIM_TYPE_E    pic_type                     = GUIANIM_TYPE_NONE;
    MMISMS_CHAT_INSERT_TYPE edit_type = MMISMS_INSERT_MUSIC;
    MMI_WIN_ID_T    quer_win_id = MMISMS_INSERT_PIC_QUERY_WIN_ID;
    MMI_STRING_T mms_content_str = {0};
    MMI_STRING_T transferring_str = {0};
	static s_need_delay_active = FALSE;
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    GUIEDIT_SetFont(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, MMISMS_GetWinFont());
#endif
#endif
    name.wstr_ptr = temp_arr;
    
    SCI_TRACE_LOW("MMISMS: HandleShowChatSmsWinMsg msg_id = 0x%x",msg_id);

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr pnull");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
			//LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
			s_need_delay_active = FALSE;
            g_sms_active_handle = 0;
            g_mms_active_handle = 0;
            g_last_active_handle = 0;
            SCI_MEMSET(&g_mmisms_global.update_chat_disp, 0 ,sizeof(MMISMS_SEND_UPDATE_CHAT_DISP_T));

		#ifdef HERO_ENGINE_TTS_SUPPORT 
			gHeroTtsSmsListCtrlId = 0xFFFFFFFF;
		#endif
			cur_order_id = MMISMS_GetCurOperationOrderId();
            win_data_ptr->win_id = win_id;
            win_data_ptr->is_have_item = TRUE;
            win_data_ptr->is_closed_by_editMMS = FALSE;
            if(cur_order_id != PNULL)   //fix code coverity CID12302
            {
                MMISMS_GetNameInfo((MMISMS_STATE_T *)cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);
            }
            GUIWIN_SetTitleText(win_id, name.wstr_ptr, name.wstr_len, FALSE);
			/*
            GUISTBDATA_GetItemText(0,&stb_data_text);
            stb_data_text.wstr_len = name.wstr_len;
            SCI_MEMCPY(win_data_ptr->title_name, name.wstr_ptr,
                sizeof(wchar)*MIN(stb_data_text.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN)); 
            
            SCI_MEMCPY(stb_data_text.wstr_ptr, name.wstr_ptr,
                sizeof(wchar)*MIN(stb_data_text.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN)); 
            GUISTBDATA_SetItemText(0,&stb_data_text);
			*/
            SCI_MEMSET(&win_data_ptr->chat_item, 0x0, sizeof(MMISMS_CHAT_ITEM_T) * MMISMS_CHAT_MAX_ITEM);

            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }


                                
            //GUIFORM_PermitChildBg(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, FALSE);
            InitEditingAreaControls(win_id);
            
#ifdef MMIMMS_SMS_IN_1_SUPPORT
		 	GUIFORM_SetSpace(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, PNULL, &ver_space_ptr);
            GUIFORM_SetChildDock(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,GUIFORM_CHILD_DOCK_BOTTOM);
            GUIFORM_SetSizeRange(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,20, 100);
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_REPLY, STXT_RETURN, FALSE);
#else
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            GUIEDIT_SetSoftkey(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 1, 0, STXT_SOFTKEY_SEND_MK,TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 0, 0, STXT_SOFTKEY_MENU_MK,TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL);
#endif
            GUIEDIT_SetFontColor(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, MMI_BLACK_COLOR);
#endif
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            if(cur_order_id != PNULL)
            {
                SCI_TRACE_LOW("MMISMS: HandleShowChatSmsWinMsg: msg_type=%d,storage=%d,mo_mt_type=%d,is_locked=%d,is_marked=%d", 
                    cur_order_id->flag.msg_type, cur_order_id->flag.storage, cur_order_id->flag.mo_mt_type, cur_order_id->flag.is_locked, cur_order_id->flag.is_marked);
            }
            if (cur_order_id != PNULL && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
            {
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};                                        
                MMIAPICOM_StrToWstr(cur_order_id->number.sender, wnumber);
                num_len = MMIAPICOM_Wstrlen(wnumber);
                if (URL_IsMailtoDefault((URL_CHAR_T const *)wnumber, (int32 *)&num_len))
                {
                    win_data_ptr->is_email_number = TRUE;
                }
   
                MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);
                MMISMS_SetChatOperOrderID(cur_order_id);
                MMK_PostMsg(win_id,  MSG_MMS_FINISH_READ, PNULL, 0);
            }
            else if(cur_order_id != PNULL && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T)); 
                if (!MMISMSAPP_ReadChatSms(cur_order_id,
                        ChatWinReadSmsCallback,
                        PNULL,
                        PNULL))
                {
                    MMK_CloseWin(win_id);
                    break;
                }        
            }
            
            
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            GUIEDIT_SetDefaultStringById(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, TXT_MMS_TOUCH_TO_INPUT);
#endif
            MMISMS_SetEditTextProperty(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
#endif
//////////////////////////////////////////////////////////////////////////
// fixed by feng.xiao for NEWMS00182269
// -add: compile swith for non-PDA ver
#ifdef MMI_PDA_SUPPORT
            HandleSmsSendEnable(FALSE);
#endif
            //MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, TRUE); 
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, TRUE); 
            break;

            case MSG_GET_FOCUS:
			if(s_need_delay_active)
			{
                if(g_last_active_handle != 0)
                {
                    MMITHEME_SetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT, MMI_WHITE_COLOR);
                    MMK_SetAtvCtrl(win_id, g_last_active_handle);
                }                     
				if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id)) 
				{
                    CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
                    MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
					CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
				} 
                HandleChatSmsTagUpdateSoftkey(MMK_GetActiveCtrlId(win_id));  
			}
			s_need_delay_active = FALSE;
			/*
            GUISTBDATA_GetItemText(0,&stb_data_text);
            stb_data_text.wstr_len = MMIAPICOM_Wstrlen(win_data_ptr->title_name);
            SCI_MEMCPY(stb_data_text.wstr_ptr, win_data_ptr->title_name,
            sizeof(wchar)*MIN(stb_data_text.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN)); 
            GUISTBDATA_SetItemText(0,&stb_data_text);
			*/
            break;
        case MSG_LOSE_FOCUS:
            MMITHEME_SetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT, MMISMS_CHAT_FOCUS_COLOR);
            break;
#if 1
		case MSG_FULL_PAINT:
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            MMITHEME_SetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT, MMISMS_CHAT_FOCUS_COLOR);
			LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
            CTRLFORM_CheckAllChildInDisplayRect(MMK_GetCtrlPtr(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID));
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
#if 0
			GUISTBDATA_GetItemText(0,&stb_data_text);
			stb_data_text.wstr_len = name.wstr_len;
			SCI_MEMCPY(stb_data_text.wstr_ptr, name.wstr_ptr,
				sizeof(wchar)*MIN(stb_data_text.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN)); 
			GUISTBDATA_SetItemText(0,&stb_data_text);
#endif//0
			break;
#endif
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
//////////////////////////////////////////////////////////////////////////
// fixed by feng.xiao for NEWMS00182269
// -add: compile swith for non-PDA ver
#ifdef MMI_PDA_SUPPORT
            HandleSmsSendEnable(TRUE);
#endif
            {
                MMI_STRING_T input_str_ptr = {0};

                SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                MMI_CheckAllocatedMemInfo();
                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &input_str_ptr);
                GUIEDIT_SetSoftkey(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 1, 0, STXT_SOFTKEY_SEND_MK, TXT_NULL, PNULL);
                
                if(input_str_ptr.wstr_len > 0)
                {
                    //GUIWIN_SeSoftkeytButtonIconId(win_id,(MMI_IMAGE_ID_T)IMAGE_COMMON_BACKSPACE, RIGHT_BUTTON,FALSE);//GUIEDIT_SetSoftkeyEx can not refresh auto
                    GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
                }
                else
                {					
                    //GUIWIN_SeSoftkeytButtonIconId(win_id,(MMI_IMAGE_ID_T)IMAGE_COMMON_BACK, RIGHT_BUTTON,FALSE);
                    GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  TRUE);
                }
                GUIWIN_UpdateSoftkey(win_id);
                SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                MMI_CheckAllocatedMemInfo();
            }
            break;

#ifdef MMS_SUPPORT
        case MSG_MMS_OPEN_ATTA_LIST_VIEW:
            oper_order_id = GetChatWinItemId(win_data_ptr);
            if(PNULL != oper_order_id)
            {                
                MMK_CreateWin((uint32*)MMISMS_ATTA_OPT_LIST_WIN_TAB, (ADD_DATA)(oper_order_id->record_id));
            }
            break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            //case MSG_TP_PRESS_SHORT:
            // save the current msg_id
            //not slide
            if (!MMK_GET_TP_SLIDE(param))
            {
                GUI_RECT_T  form_rect = {0};
                MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
                MMI_CTRL_ID_T active_ctrl_id = 0;
                active_ctrl_id = MMK_GetActiveCtrlId(win_id);

                SCI_MEMCPY(&para, (MMI_MULTI_KEY_TP_MSG_PARAM_T*)param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
                GUICTRL_GetRect( MMK_GetCtrlPtr(active_ctrl_id), &form_rect);
                if (GUI_PointIsInRect(para.cur_tp_point, form_rect))
                {
           //         SetHandleOkMenuMsg(MSG_TP_PRESS_UP);
                    oper_order_id = GetChatWinItemId(win_data_ptr);
                    if (oper_order_id == PNULL)
                    {
                        break;
                    }
                    switch (oper_order_id->flag.msg_type)
                    {
                        case MMISMS_TYPE_SMS:

                            break;

                        case MMISMS_TYPE_MMS:
                            //解决cr MS00150274,U盘使用过程中不能看MMS
                            if (MMIAPIUDISK_UdiskIsRun())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                                break;
                            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                            if (MMIAPIBROWSER_IsRunning())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                                break;
                            }
#endif
#endif
                            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                            {
                                win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                                MMISMS_SetDelIndex(win_data_ptr->del_index);
                            }
                            if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                            {
#ifdef MMS_SUPPORT
                                MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                            }                        
                            MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);
                        
                            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                            {
                                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMS_SUPPORT
        case MSG_MMS_FINISH_READ:
            oper_order_id = MMISMS_GetChatOperOrderID();
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);

            SetMMSChatItemParam(win_data_ptr,win_id, both_rect, oper_order_id);
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);

            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }

            if (PNULL != oper_order_id->next_chat_ptr)
            {
                if (MMISMS_TYPE_SMS == oper_order_id->next_chat_ptr->flag.msg_type)
                {
                    SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                    MMISMSAPP_ReadChatSms(oper_order_id->next_chat_ptr,
                            ChatWinReadSmsCallback,
                            PNULL,
                            PNULL
                        );
                }
                else
                {
                    MMISMS_SetGlobalOperationOrderId( oper_order_id->next_chat_ptr );
                    MMISMS_SetChatOperOrderID(oper_order_id->next_chat_ptr);
                    MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_MMS_FINISH_READ, PNULL, 0);
                }
            }
            else
            {
                MMISMS_UpdateUnreadOrder();
                if (MMK_IsFocusWin(win_id))
                {
                    s_need_delay_active = FALSE;
                    if(g_last_active_handle != 0)
                    {
                        //baokun test
                        MMK_SetAtvCtrl(win_id, g_last_active_handle);                                        
                    }                     
				    if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id)) 
				    {
                        CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
                        MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
						CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
				    }  
                    HandleChatSmsTagUpdateSoftkey(MMK_GetActiveCtrlId(win_id));           
                }
                else
                {
                    s_need_delay_active = TRUE;
                }
                       
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }

            break;

        case MSG_MMS_UPDATE_CHAT:
            {
                MMI_CTRL_ID_T  replace_child_ctrl = 0;
                uint16 i = 0;
                
                oper_order_id = MMISMS_GetChatOperOrderID();
                
                if (PNULL == oper_order_id)
                {
                    break;
                }
    
                for(i = 0;i<MMISMS_CHAT_MAX_ITEM;i++)
                {
                    if(win_data_ptr->chat_item[i].is_valid && win_data_ptr->chat_item[i].cur_order_id != PNULL && win_data_ptr->chat_item[i].cur_order_id->record_id == oper_order_id->record_id)
                    {
                        replace_child_ctrl = win_data_ptr->chat_item[i].parent_ctrl_id;
                        win_data_ptr->chat_item[i].is_valid = FALSE;
                        break;
                    }
                }
                
                if(i != MMISMS_CHAT_MAX_ITEM)
                {
                    GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
                    SCI_MEMSET(&win_data_ptr->chat_item[i],0x0,sizeof(MMISMS_CHAT_ITEM_T));
                }
                    
                MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
    
                SetMMSChatItemParam(win_data_ptr,win_id, both_rect, oper_order_id);
                MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
                if (MMK_IsFocusWin(win_id))
                {
					if(g_last_active_handle != 0)
					{
                        MMITHEME_SetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT, MMI_WHITE_COLOR);
                        MMK_SetAtvCtrl(win_id, g_last_active_handle);
					}                     
					if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id)) 
					{
						CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
						MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
					} 
					MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                }
				else
				{
					s_need_delay_active = TRUE;
				}
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }
            break;
#endif

        case MSG_SMS_FINISH_READ:
            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            SetChatItemParam(win_id, both_rect, oper_order_id);
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);

            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
            
            if (PNULL != oper_order_id->next_chat_ptr)
            {
                if (MMISMS_TYPE_SMS == oper_order_id->next_chat_ptr->flag.msg_type)
                {
                    SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                    MMISMSAPP_ReadChatSms(oper_order_id->next_chat_ptr,
                            ChatWinReadSmsCallback,
                            PNULL,
                            PNULL
                        );
                    {//temp debug	
                    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                    MMI_CheckAllocatedMemInfo();
                    }
                }
                else
                {
                    MMISMS_SetGlobalOperationOrderId( oper_order_id->next_chat_ptr );
                    MMISMS_SetChatOperOrderID(oper_order_id->next_chat_ptr);
                    MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_MMS_FINISH_READ, PNULL, 0);
                }
            }
            else
            {                                                  
                    MMISMS_UpdateUnreadOrder();
                    MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                    MMISMS_ClearOperInfo();                    
                    if (MMK_IsFocusWin(win_id))
                    {
                        s_need_delay_active = FALSE;
				        if(g_last_active_handle != 0 )
                        {
                            MMK_SetAtvCtrl(win_id, g_last_active_handle);                                            
                        }
				        if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id)) 
				        {                            
							CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
                            MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
							CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
				        }
                        HandleChatSmsTagUpdateSoftkey(MMK_GetActiveCtrlId(win_id));
                    }
                    else
                    {
                        s_need_delay_active = TRUE;
                    }                    
            }

            break;

        case MSG_SMS_NEW_SMS:

            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();

            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
            MMK_CloseWin(MMISMS_EXTRACT_NUMBER_POP_WIN_ID);
            MMK_CloseWin(MENU_SMS_EXTRACT_URL_POP_MENU);
            if( g_mmisms_global.update_chat_disp.is_update)
            {
                if( (oper_order_id->flag.is_concatenate_sms && 1 == oper_order_id->longsms_seq_num) || (!oper_order_id->flag.is_concatenate_sms && g_mmisms_global.update_chat_disp.is_send_finish))
                {
                    uint16 i = 0;
                    for(i = 0; i  < MMISMS_CHAT_MAX_ITEM;i++)
                    {
                        if(
                        win_data_ptr->chat_item[i].is_valid 
						//&&win_data_ptr->chat_item[i].cur_order_id->record_id != PNULL 
                        &&win_data_ptr->chat_item[i].cur_order_id->record_id == g_mmisms_global.update_chat_disp.update_record_id
                        &&win_data_ptr->chat_item[i].cur_order_id->flag.storage == g_mmisms_global.update_chat_disp.update_storage
                        &&win_data_ptr->chat_item[i].cur_order_id->flag.dual_sys == g_mmisms_global.update_chat_disp.update_dual_sys)
                        {
                            win_data_ptr->chat_item[i].cur_order_id = oper_order_id;    //reset this to send success order
                            g_mmisms_global.update_chat_disp.send_finish_order_id = oper_order_id;
                            // for bug 1090575
                            MMISMS_UpdateChatWinSoftkey(win_id);
                            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);                              
                            break;
                        }
                    }
                }
                g_mmisms_global.update_chat_disp.is_update = FALSE;
            }
            else
            {  
                SetChatItemParam(win_id, both_rect, oper_order_id);      
                MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);  
                if (MMK_IsFocusWin(win_id))
                {
                    s_need_delay_active = FALSE;
                    if(g_last_active_handle != 0)
                    {
                        MMK_SetAtvCtrl(win_id, g_last_active_handle);                                                
                    } 				
			        if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id)) 
			        {
                        CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
                        MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
						CTRLFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
			        }   
                    HandleChatSmsTagUpdateSoftkey(MMK_GetActiveCtrlId(win_id));  
                }
                else
                {
                    s_need_delay_active = TRUE;
                }

            }
                                 
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();

            break;
            
         
        case MMISMS_MSG_UPDATELIST:
            {        
                if(!win_data_ptr->is_del_all) //全部删除，直接关闭当前窗口，避免多次刷新
                {
                    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                    MMI_CheckAllocatedMemInfo();
#ifndef MMI_GUI_STYLE_TYPICAL
                    MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
                    if(g_mmisms_global.update_chat_disp.is_update && g_mmisms_global.update_chat_disp.is_send_finish)
                    {
                         uint16 del_index = 0;
                         del_index = MMISMS_GetDelIndex();                    
                         if(!g_mmisms_global.update_chat_disp.is_success)
                         {
                             GUILABEL_SetVisible(MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_CTRL_ID + del_index,TRUE,FALSE);
                             GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_RESEND_MK,TXT_NULL , FALSE);
                             GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK, LEFT_BUTTON,TRUE);
                             GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK, RIGHT_BUTTON,TRUE);                             
                             GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);                                
                             
                         }
                         else
                         {
                              GUILABEL_SetVisible(MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_CTRL_ID + del_index,FALSE,FALSE);
                              //GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL,TXT_NULL , TRUE);   
                              GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK, LEFT_BUTTON,TRUE);
                              GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK, RIGHT_BUTTON,TRUE);                              
                         }
                                                 
                         SCI_MEMSET(&g_mmisms_global.update_chat_disp, 0 ,sizeof(MMISMS_SEND_UPDATE_CHAT_DISP_T));
                    }
                    else
                    {
                        UpdateChatItemDisplayAfterDelSingle(win_data_ptr);
                    }
#endif 
                    if (MMK_IsFocusWin(win_id))
                    {
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    }
                }
                SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                MMI_CheckAllocatedMemInfo();
            }

            break;
            
        case MMISMS_MSG_UPDATE_NUMBER:
            HandleChatReadWinUpdateDispNum(win_data_ptr);
            break;  
            
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
            //check is no log or not
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                MMI_CTRL_ID_T active_ctrl_id = 0;
                MMISMS_ORDER_ID_T order_id = PNULL;
                active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                {
                    GUITEXT_TAG_INFO_T tag_info = {0};
                    MMIPUB_MENU_PARAM_T menuparam = {0};
                    uint16 min = 0;
                    uint16 max = 0;
                    if(TRUE == GUITEXT_GetHighlightInfo(active_ctrl_id,&min,&max))
                    {
						GUITEXT_GetCurTag(active_ctrl_id , &tag_info);           
                    }
                    if(tag_info.type == GUITEXT_TAG_PHONENUM)
                    {            
                        menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
                        menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
                        menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
                        menuparam.dst_handle = win_data_ptr->win_id;
                        menuparam.func.OnMenuOk = HandleChatWinExtractNumberOptOk;    
                        MMIPUB_OpenPubMenuWin( &menuparam );   
                    }                        
                    else if(tag_info.type == GUITEXT_TAG_URL)
                    {
                        //do nothing
                    }
                    else
                    {
                        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
                        if (PNULL != order_id)
                        {
                            MMISMS_SetGlobalOperationOrderId(order_id);
                        }
                        menuparam.group_id = MENU_SMS_CHAT_OPT;
                        menuparam.win_id =   MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID;
                        menuparam.ctrl_id =  MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID;

                        menuparam.dst_handle = win_id;
                        menuparam.func.OnMenuOk = HandleSmsChatMenuOptOK;
                        menuparam.func.OnMenuInit = HandleSmsChatMenuOptInit;
                        SCI_TRACE_LOW("[%s:%d] current order index: %d\n",__FUNCTION__,__LINE__,active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID);
                        MMIPUB_OpenPubMenuWin( &menuparam ); 
                    }
                }
            else//text editor is activitied now
            {
                OpenSMSChatEditOptPopMenu();
            }	


            }
            break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_LONG:
        {
            MMISMS_ORDER_ID_T order_id = PNULL;
            MMI_CTRL_ID_T active_ctrl_id = 0;
            MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
            GUI_RECT_T  form_rect = {0};
            MMIPUB_MENU_PARAM_T menuparam = {0};
            active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);

            SCI_MEMCPY(&para, (MMI_MULTI_KEY_TP_MSG_PARAM_T*)param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            GUICTRL_GetRect( MMK_GetCtrlPtr(active_ctrl_id), &form_rect);
            if (GUI_PointIsInRect(para.cur_tp_point, form_rect))
            {
                if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                {
                    order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
                }
                if (PNULL != order_id)
                {
                    MMISMS_SetGlobalOperationOrderId(order_id);
                        
                    menuparam.group_id = MENU_SMS_CHAT_ITEM_OPT;
                    menuparam.win_id =   MMISMS_SHOW_CHAT_ITEM_OPT_WIN_ID;
                    menuparam.ctrl_id =  MMISMS_SHOW_CHAT_ITEM_OPT_CTRL_ID;

                    menuparam.dst_handle = win_id;
                    menuparam.func.OnMenuInit = HandleSmsChatItemOptInit;
                    menuparam.func.OnMenuOk = HandleSmsChatItemOptOK;                    
                    MMIPUB_OpenPubMenuWin( &menuparam ); 
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
        case MMI_SELECT_CONTACT_CNF:    //added by taiping.lai 2011/01/18 添加联系人信息处理
            if (win_data_ptr->is_add_contact)
            {
                AddContactInfo2Editbox((MMIPB_HANDLE_T)param);
                win_data_ptr->is_add_contact = FALSE;
            }
            break;

        case MSG_PROMPTWIN_OK:            
            {
                //需要判断是否加帧显示
                if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == win_data_ptr->sms_chat_insert_info.file_type)
                {
                    // allocate memory
                    MMI_STRING_T      str_ptr = {0};
                    MMI_STRING_T      str_to = {0};
    
                    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                    MMK_CloseWin(quer_win_id);
                    mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                    if (mms_content_str.wstr_ptr == PNULL)
                    {
                        //SCI_TRACE_LOW:"mmisms alloc failed"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28546_112_2_18_2_55_19_850, (uint8*)"");
                        if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                        {
                            SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                            win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                        }
                        break;
                    }
                    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                    str_to.wstr_ptr = wnumber;
                    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif 
                    if (MMIAPIMMS_EditAddMMSAttach(win_data_ptr->sms_chat_insert_info.file_info_ptr, win_data_ptr->sms_chat_insert_info.file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE))
                    {
                        MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(STR_MSG_ATTACHMENT_BIG_EXT01);
                    }
    
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    if (PNULL != mms_content_str.wstr_ptr)
                    {
                        SCI_FREE(mms_content_str.wstr_ptr);
                        mms_content_str.wstr_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
            }
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(&quer_win_id);
            break;
#if defined (MMS_SUPPORT) && defined (MMIMMS_SMS_IN_1_SUPPORT)
        case MSG_MULTIM_SELECTED_RETURN:
        {
            MMIFILE_FILE_INFO_T  *file_info = PNULL;

            file_info = (MMIFILE_FILE_INFO_T*)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
            if (PNULL == file_info)
            {
                //SCI_TRACE_LOW:"sms file info alloc error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28604_112_2_18_2_55_19_851, (uint8*)"");
                break;
            }

            multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;
            //选中多媒体文件返回

            MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr, multim_return_info_ptr->file_name_len, file_info);
            real_file_size = file_info->file_size;
            edit_type = MMISMS_GetChatEditInsertOpType();
            insert_total_size = real_file_size ;
            if(MMIMMS_IsMaxNum())
            {
               SCI_TRACE_LOW("[MMIMMS] HandleShowChatSmsWinMsg: reach max mms number");
               MMIPUB_OpenAlertWarningWin(TXT_MMS_FULL_NUMBER);
            }
            else if(MMIMMS_IsCurrentChatTotalSizeOverrun(insert_total_size))
            {
                SCI_TRACE_LOW("[MMIMMS] HandleShowChatSmsWinMsg: reach max mms size in current chat");
                MMIPUB_OpenAlertWarningWin(TXT_MMS_FULLSPACE);
            }
            else if (0 == real_file_size)
            {
                MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }          
            else if (insert_total_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {

                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;

                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);

                reduce_pic_size = reduce_width * reduce_height * 2;
                SCI_TRACE_LOW("HandleShowChatSmsWinMsg: real_file_size=%d byte, reduce_pic_size=%d byte, MAX_CONTENT_SIZE=%d byte", __LINE__, __FILE__, __FUNCTION__, real_file_size, reduce_pic_size, MMIMMS_MAX_CONTENT_SIZE);

                pic_type = MMIAPICOM_GetImgaeType(file_info->file_name, file_info->file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type
                        && GUIANIM_TYPE_IMAGE_JPG == pic_type
                        && real_file_size > reduce_pic_size
                        //&& reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE //bug973439
                        && real_file_size <= MMIMMS_MAX_CONTENT_SIZE
                        && MMISMS_INSERT_ADD_ATTACH != edit_type
                   )
                {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL == mms_content_str.wstr_ptr)
                {
                    //SCI_TRACE_LOW:"mmisms alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852, (uint8*)"");

                    SCI_FREE(file_info);

                    break;
                }
                //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                str_to.wstr_ptr = wnumber;
                str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif 
                win_data_ptr->is_closed_by_editMMS = TRUE;
                MMK_CloseWin(win_id);//从聊天界面无法通过添加vcard进入编辑彩信，需要增加此修改

                MMIAPIMMS_EditAddMMSAttach(file_info, multim_return_info_ptr->file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE);
//useless code ,because in if MMISMS_INSERT_ADD_ATTACH != edit_type, just use the above line code
#if 0   
                if (MMISMS_INSERT_ADD_ATTACH != edit_type)
                {
                    MMIAPIMMS_EditAddMMSAttach(file_info, multim_return_info_ptr->file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE);
 
                }             
                else
                {
                    MMIAPIMMS_EditAddMMSAttach(file_info, MMIFMM_FILE_TYPE_NORMAL, PNULL, 0, PNULL, &mms_content_str, &str_to, FALSE);
                }
#endif
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    SCI_FREE(mms_content_str.wstr_ptr);
                    mms_content_str.wstr_ptr = PNULL;
                }

#if 0              
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;

                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);

                reduce_pic_size = reduce_width * reduce_height * 2;

                pic_type = MMIAPICOM_GetImgaeType(file_info->file_name, file_info->file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type
                        && GUIANIM_TYPE_IMAGE_JPG == pic_type
                        && real_file_size > reduce_pic_size
                        && reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE
                        && MMISMS_INSERT_ADD_ATTACH != edit_type
                   )
                {
               
                    win_data_ptr->sms_chat_insert_info.file_type = multim_return_info_ptr->file_type;
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        SCI_FREE(file_info);
                        break;
                    }
                    //SCI_MEMSET((char *)g_sms_chat_insert_info.file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
                    SCI_MEMCPY((char *)win_data_ptr->sms_chat_insert_info.file_info_ptr, (char *)file_info, sizeof(MMIFILE_FILE_INFO_T));
                   
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC, IMAGE_PUBWIN_QUERY, &quer_win_id, PNULL);
                }
                  
                else        
#endif                    
                }
                else
                    {
                        ShowFileLargeBanner(win_id);
                    }                   
            }
            else
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL == mms_content_str.wstr_ptr)
                {
                    //SCI_TRACE_LOW:"mmisms alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852, (uint8*)"");

                    SCI_FREE(file_info);

                    break;
                }
                //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                str_to.wstr_ptr = wnumber;
                str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif 
                win_data_ptr->is_closed_by_editMMS = TRUE;
                MMK_CloseWin(win_id);//从聊天界面无法通过添加vcard进入编辑彩信，需要增加此修改

                if (MMISMS_INSERT_ADD_ATTACH != edit_type)
                {
                    MMIAPIMMS_EditAddMMSAttach(file_info, multim_return_info_ptr->file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE);
 
                }
                else
                {
                    MMIAPIMMS_EditAddMMSAttach(file_info, MMIFMM_FILE_TYPE_NORMAL, PNULL, 0, PNULL, &mms_content_str, &str_to, FALSE);
                }

                if (PNULL != mms_content_str.wstr_ptr)
                {
                    SCI_FREE(mms_content_str.wstr_ptr);
                    mms_content_str.wstr_ptr = PNULL;
                }
            }
            SCI_FREE(file_info);
        }

        break;
#endif

#if defined(MMI_VCARD_SUPPORT)
        case MMI_PB_VCARD_SELECTED:
        {
            uint32 vcard_data_len = 0;
            uint8 *vcard_buf_ptr  = 0;
            MMI_STRING_T vacard_name_str = {0};
            wchar vacard_suf[] = {'.', 'v', 'c', 'f', 0};

            vcard_buf_ptr = (uint8 *)SCI_ALLOCA(VCARD_MAX_FILE_LEN * sizeof(uint8));
            if (vcard_buf_ptr == PNULL)
            {
                break;
            }
            SCI_MEMSET(vcard_buf_ptr, 0x00, VCARD_MAX_FILE_LEN * sizeof(uint8));
            MMIAPIPB_GetSelectVcard((MMIPB_HANDLE_T)param, vcard_buf_ptr, VCARD_MAX_FILE_LEN, 0);
            vcard_data_len = strlen((char *)vcard_buf_ptr);
            vacard_name_str.wstr_ptr = SCI_ALLOCAZ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
            if (PNULL == vacard_name_str.wstr_ptr)
            {
                SCI_FREE(vcard_buf_ptr);
                break;
            }
            //SCI_MEMSET((char *)vacard_name_str.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar)));
            MMIAPIPB_GetSelectName((uint32)param, &vacard_name_str, MMIMMS_FILE_NAME_LEN - MMIAPICOM_Wstrlen(vacard_suf), 0);
            MMIAPICOM_Wstrncpy(vacard_name_str.wstr_ptr + vacard_name_str.wstr_len, vacard_suf, MMIAPICOM_Wstrlen(vacard_suf));
            vacard_name_str.wstr_len = vacard_name_str.wstr_len + MMIAPICOM_Wstrlen(vacard_suf);
            insert_total_size = vcard_data_len;
            if (0 == vcard_data_len)
            {
                MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID,TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
            else if (insert_total_size > MMIMMS_MMS_LIMIT_SIZE)
            {
                MMIPUB_OpenAlertWarningWin(STR_MSG_ATTACHMENT_BIG_EXT01);
            }
            else
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                MMIPB_BCD_NUMBER_T pb_bcd = {0};
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                    str_to.wstr_ptr = wnumber;
                    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif                     
                    win_data_ptr->is_closed_by_editMMS = TRUE;
                    MMK_CloseWin(win_id);  // close win will DestroyMMSEditDoc(win_data_ptr); fix 476884 issue

                    if (MMIAPIMMS_EditAddMMSAttach(PNULL, MMIFMM_FILE_TYPE_VCARD, vcard_buf_ptr, vcard_data_len, &vacard_name_str, &mms_content_str, &str_to, FALSE))
                    {
                        //MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
                    }
                    SCI_FREE(vcard_buf_ptr);
                    SCI_FREE(vacard_name_str.wstr_ptr);
                    if (PNULL != mms_content_str.wstr_ptr)
                    {
                        SCI_FREE(mms_content_str.wstr_ptr);
                        mms_content_str.wstr_ptr = PNULL;
                    }
                }
            }
        }
        break;
#endif
     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (ctrl_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
#ifdef MMIMMS_SMS_IN_1_SUPPORT                    
                    HandleChatWinOpenInsertOpt(win_data_ptr); 
#endif
                    break;

                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    BuildSendData(win_id,FALSE);
                    HandleChatWinSendSms(win_id);
                    break;

                default:
                    if ( (ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) && (ctrl_id < MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM)) //信息内容提取处理
                    {
                        HandleChatSmsTag(win_data_ptr,ctrl_id);
                    }
                    break;

            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
            GUITEXT_TAG_INFO_T tag_info = {0};//Tong Yongli

            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID == active_ctrl_id)
            {
                MMI_STRING_T str = {0};
                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str);
                if (0 != str.wstr_len)
                {
                    if(!win_data_ptr->is_email_number)
                    {
                        BuildSendData(win_id,FALSE);
                        HandleChatWinSendSms(win_id);
                    }
                    else
                    {
                        HandleChatWinSendMMS(win_id);
                    }                    
                }
            }
            else if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                     && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        if(MMISMS_MO_SEND_FAIL == oper_order_id->flag.mo_mt_type && !MMISMS_IsSmsBusy(oper_order_id))
                        {
                            MMISMS_SetGlobalOperationOrderId(oper_order_id);
                            HandleChatWinResendItem(win_data_ptr);
                        }
                        else
                        {                            
                            uint16 min = 0;
                            uint16 max = 0;
                            if(TRUE == GUITEXT_GetHighlightInfo(active_ctrl_id,&min,&max))
							{
                                if(MMIAPIIDLE_IsPhoneLocked())
                                {
                                    break;
                                }
								GUITEXT_GetCurTag(active_ctrl_id , &tag_info);           
								if(tag_info.type == GUITEXT_TAG_PHONENUM)
								{
									//HandleChatSmsTag(win_data_ptr,active_ctrl_id); //中键保存
									HandleChatWinExtractNumberSaveContact(tag_info);                                
								}
								else if(tag_info.type == GUITEXT_TAG_URL)
								{
									//中键 上 网 
									HandleChatWinExtractUrlGotoBrowser(tag_info);
								}
							}
                        }
                        break;

#ifdef MMS_SUPPORT
                    case MMISMS_TYPE_MMS:
                        SCI_TRACE_LOW("MMIMMS: HandleShowChatSmsWinMsg WEB-MMS: mo_mt_type=%d", oper_order_id->flag.mo_mt_type);
                        if (MMISMS_MT_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type || MMISMS_MT_NOT_NOTIFY == oper_order_id->flag.mo_mt_type ||MMISMS_MT_NEED_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type )
                        {
                            MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;

                            if (MMIAPIMMS_IsSendingMMS())
                            {
                                MMSMMI_OpenMMSAlertWin(STR_MSA_SYS_BUSY, TXT_NULL, IMAGE_PUBWIN_WARNING);

                                break;
                            }

                            mms_list_info_ptr = MMIMMS_GetListItemByID(oper_order_id->record_id);
                            if (PNULL == mms_list_info_ptr)
                            {
                                break;
                            }
                                
                            if(! MMIAPIMMS_IsProcessMMSById(oper_order_id->record_id) )
                            {
                                MMIMMS_PUSH_FILE_T *push_info_ptr      = PNULL;        
                                int32 record_id = 0;
                                record_id = oper_order_id->record_id;
 
                                SCI_TRACE_LOW("MMIMMS: HandleShowChatSmsWinMsg WEB-MMS: is not process mms by id");
                                push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
                                if (PNULL == push_info_ptr)
                                {
                                    break;
                                }
                                //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
                                MMIMMS_ReadPush(push_info_ptr, record_id);
                                if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面，加入下载链表
                                {
                                    if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                                    {
                                        MMISMS_ORDER_INFO_T *mms_display_info = PNULL;
                                        mms_display_info = ReadMMSContent(record_id);
                                        if (MMIMMS_IsExpire(mms_list_info_ptr->time,(uint32)push_info_ptr->expiry_time))
                                        {
                                            MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                                        }
                                        else if (!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
                                        {
                                            SCI_TRACE_LOW("[MMIMMS]  func [%s] line [%d] ",__FUNCTION__,__LINE__);
                                            if(MMIAPIFMM_GetFirstValidSD()>= MMI_DEVICE_NUM )
                                            {
                                                MMSMMI_OpenMMSAlertWin(STR_NOT_ENOUGH_MEMORY_JT, TXT_NULL, IMAGE_PUBWIN_WARNING);
                                            }
                                            else 
                                            {
                                                MMSMMI_OpenMMSAlertWin(STXT_MESSAGE_MEMORY_FULL, TXT_NULL, IMAGE_PUBWIN_WARNING);
                                            }
                                        }
                                        else if(MMIMMS_IsCurrentChatTotalSizeOverrun(mms_display_info->file_size))
                                        {
                                            MMIPUB_OpenAlertWarningWin(TXT_MMS_FULLSPACE);
                                        }
                                        else if(MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, record_id))
                                        {
                                            //更新短消息的状态图标                
                                            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, TRUE);
                                            MMIAPISMS_UpdateMsgStatus(); 
                                            if (MMIMMS_IsAutolistWithItem())
                                            {
                                                MMSMMI_OpenMMSAlertWin(TXT_MMS_STXT_PREPARE_DOWNLOAD, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                                            }                        
                                        }
                                        else
                                        {
                                            SCI_TRACE_LOW("MMIMMS: HandleInboxMMSNotiWinMsg active mms error!");
                                        }
                                    }
                                    else
                                    {
                                        MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                                    }            
                                } 
                                else if (MMIMMS_IsNeedDelete(record_id))//record_id已经在下载链表里面，从下载列表里面删除
                                {
                                    MMIMMS_CancelSendRecvById(record_id);
                                    GUIWIN_SeSoftkeytButtonTextId(win_id, STR_SOFTKEY_DOWNLOAD_EXT02, MIDDLE_BUTTON, TRUE);
                                    MMIAPISMS_UpdateMsgStatus();    
                                }
                                SCI_FREE(push_info_ptr);
                                push_info_ptr = PNULL;
                            }
                            else if (MMSMMI_MMSCheckDataReceiveCompleted(oper_order_id->record_id) == FALSE)
                            {
                                MMI_CTRL_ID_T   title_ctrl_id = 0;
                                MMI_CTRL_ID_T   size_ctrl_id = 0;
                                MMI_STRING_T content_str = {0};
                                MMI_STRING_T time_str = {0};
                                MMISMS_ORDER_INFO_T *mms_display_info = PNULL;
                                mms_display_info = ReadMMSContent(oper_order_id->record_id);
                                GetMMSTimeSujectSize(&content_str, &time_str, mms_display_info);

                                SCI_TRACE_LOW("MMIMMS: HandleShowChatSmsWinMsg WEB-MMS: is process mms by id");
                                //[start]changed by minghu cr166756
                                if (mms_list_info_ptr->mo_mt_type == MMISMS_MT_NOT_NOTIFY)
                                {
                                    mms_list_info_ptr->mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                                    MMIMMS_SaveMMSListFile();
                                }                                
                                MMSMMI_OnMMSDataReceived(oper_order_id->record_id, &transferring_str);                  
                                if (transferring_str.wstr_ptr) 
                                {
                                    SCI_FREE(transferring_str.wstr_ptr);
                                    transferring_str.wstr_ptr = PNULL;
                                }
                                //Media message
                                //Size:85KB
                                title_ctrl_id = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID);    
                                GUILABEL_SetTextById(title_ctrl_id, STR_DTAIL_TYPE_MMS, TRUE);
                                
                                size_ctrl_id = MMISMS_SHOW_CHAT_SMS_SIZE_ITEM_FORM_CTRL_ID + (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID);    

                                GUILABEL_SetText(size_ctrl_id, &content_str, TRUE);
                    
                                //[end]
                                MMIMMS_CancelSendRecvById(oper_order_id->record_id);
                                GUIWIN_SeSoftkeytButtonTextId(win_id, STR_SOFTKEY_DOWNLOAD_EXT02, MIDDLE_BUTTON, TRUE);
                                MMIAPISMS_UpdateMsgStatus();                                                                                
                            }                            
                        }
                        else if(MMISMS_MO_SEND_FAIL == oper_order_id->flag.mo_mt_type)
                        {
                            if(!MMIAPIMMS_IsProcessMMSById(oper_order_id->record_id))
                            {
                                MMISMS_SetGlobalOperationOrderId(oper_order_id);                        
                                HandleChatWinResendItem(win_data_ptr);
                            }
                        }
                        else
                        {                
                            uint16 min = 0;
                            uint16 max = 0;                        
                            //解决cr MS00150274,U盘使用过程中不能看MMS
                            if (MMIAPIUDISK_UdiskIsRun())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                                break;
                            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                            if (MMIAPIBROWSER_IsRunning())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                                break;
                            }
#endif
#endif
                            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                            {
                                win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                                SCI_TRACE_LOW("[%s:%d]del_index: %d\n",__FUNCTION__,__LINE__,win_data_ptr->del_index );
                                MMISMS_SetDelIndex(win_data_ptr->del_index);
                            }                        
                            if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                            {
#ifdef MMS_SUPPORT
                                MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                            }

                            if(TRUE == GUITEXT_GetHighlightInfo(active_ctrl_id,&min,&max))
							{
								GUITEXT_GetCurTag(active_ctrl_id , &tag_info);           
							}
							else
							{
								tag_info.type = GUITEXT_TAG_NONE;
							}
                            SCI_TRACE_LOW("mmisms tag_info.type %d",tag_info.type);
                            if(tag_info.type == GUITEXT_TAG_PHONENUM)
                            {
                                //HandleChatSmsTag(win_data_ptr,active_ctrl_id); //中键保存
                                HandleChatWinExtractNumberSaveContact(tag_info);                                
                            }
                            else if(tag_info.type == GUITEXT_TAG_URL)
                            {
                                //中键 上 网 
                                HandleChatWinExtractUrlGotoBrowser(tag_info);
                            }
                            else
                            {
                                MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);
                            }
                                                    
                            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                            {
                                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                            }
                            break;
                        }
#endif
                    default:
                        break;
                }
            }
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
        }
        break;

#ifdef MMS_SUPPORT
	  //handler of mms recv msg moved from waitingwin--start
        case  MSG_MMS_RECV_RETRYING:
        {
            oper_order_id = GetChatWinItemId(win_data_ptr);//coverity cid 26124,need check more
            if (oper_order_id == PNULL)
            {
                break;
            }               
            MMSMMI_UpdateAndGetProgressInfo(oper_order_id->record_id, &transferring_str); 
            if (transferring_str.wstr_ptr) 
            {
                SCI_FREE(transferring_str.wstr_ptr);
                transferring_str.wstr_ptr = PNULL;
            }
        }
        break;
        case MSG_MMS_DATA_RECEIVED:
        {
            //收到彩信数据，进行解码及对sms的各项进行保存
			uint16 i = 0;     
            MMI_CTRL_ID_T   title_ctrl_id = 0;
            MMI_CTRL_ID_T   size_ctrl_id = 0;
            oper_order_id = GetChatWinItemId(win_data_ptr);//coverity cid 26124
            if (oper_order_id == PNULL)
            {
                break;
            }
            SCI_TRACE_LOW("mmisms enter data received ");
            for(i = 0; i < MMISMS_CHAT_MAX_ITEM; i++)  //可能有多条 彩信，需要 对正在下载的进度进行实时更新
            {
                if(win_data_ptr->chat_item[i].is_valid 
					&& PNULL != win_data_ptr->chat_item[i].cur_order_id
                    && win_data_ptr->chat_item[i].cur_order_id->flag.msg_type == MMISMS_TYPE_MMS
                    && (MMISMS_MT_NOT_DOWNLOAD == win_data_ptr->chat_item[i].cur_order_id->flag.mo_mt_type || MMISMS_MT_NOT_NOTIFY == win_data_ptr->chat_item[i].cur_order_id->flag.mo_mt_type)
                    && MMIAPIMMS_IsProcessMMSById(win_data_ptr->chat_item[i].cur_order_id->record_id) )
                {
                    SCI_TRACE_LOW("mmisms find  record_id = %d rong gu",win_data_ptr->chat_item[i].cur_order_id->record_id);
                    title_ctrl_id = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + i;    
                   
#ifdef PIC_VIEWER_SUPPORT
                    GUILABEL_SetTextById(title_ctrl_id, STR_ID_UMMS_DOWNLOAD_IN_PROGRESS, TRUE);//need modify to 'Downloading...'
#else
                    GUILABEL_SetTextById(title_ctrl_id, TXT_COMMON_DOWNLOADING_WAIT, TRUE);//need modify to 'Downloading...'
#endif
                    MMSMMI_OnMMSDataReceived(oper_order_id->record_id, &transferring_str);

                    size_ctrl_id = MMISMS_SHOW_CHAT_SMS_SIZE_ITEM_FORM_CTRL_ID + i;    
                    GUILABEL_SetText(size_ctrl_id, &transferring_str, TRUE);
                    if (transferring_str.wstr_ptr)
                    {
                        SCI_FREE(transferring_str.wstr_ptr);
                        transferring_str.wstr_ptr = PNULL;
                    }
                    break;
                }
            }
    
            //maybe we should update the label of processing message form later...
        }
        break;
#endif
       case MSG_KEYUP_UP:
       case MSG_KEYUP_DOWN:
	#ifdef HERO_ENGINE_TTS_SUPPORT 
	{
		MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
		if(gHeroTtsSmsListCtrlId != active_ctrl_id)
		{
			MMI_STRING_T	msg_content = {0};

			gHeroTtsSmsListCtrlId = active_ctrl_id;
			MMISMS_ChatWinGetItemContent(&msg_content);

			//SCI_MEMCPY(g_hero_msg_content.wstr_ptr,msg_content.wstr_ptr,msg_content.wstr_len);
			//g_hero_msg_content.wstr_len = msg_content.wstr_len;

			if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
			{        
				MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
			}

			if(!MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
			{
				BOOLEAN ret = FALSE;

				MMITTS_PLAY_PARAM_T play_param = {0};

				//if (!is_tts_played)
				{
					if(PNULL != msg_content.wstr_ptr)
					{
						play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
						play_param.txt_len_in_byte = msg_content.wstr_len * 2;
						play_param.is_ucs2 = TRUE;
						play_param.play_times = 1;
						play_param.tts_vol = MMIAPISET_GetMultimVolume();
						play_param.ring_type = TTS_RING_TYPE_SMS;
						play_param.win_handle = MMISMS_SHOW_CHAT_SMS_WIN_ID;//MMISMS_CHATBOX_CHILD_WIN_ID;
						play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
						play_param.digit_mode = TTS_DIGIT_AUTO;
						play_param.punc_mode = TTS_PUNC_OFF;
						play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED; 				   
						ret = MMIAPITTS_PlayText(&play_param);
						SCI_FREE(msg_content.wstr_ptr);
						msg_content.wstr_ptr = PNULL;
					}					
				}
			}
		}
	}
	#endif
            HandleChatSmsTagUpdateSoftkey(MMK_GetActiveCtrlId(win_id));                       
            break;

        //handler of mms recv msg moved from waitingwin--start
       case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
       {
           SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
           MMI_CheckAllocatedMemInfo();
           MMISMS_UpdateChatWinSoftkey(win_id);
           SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
           MMI_CheckAllocatedMemInfo();
       }
       break;
#endif
#else           
        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                    && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
                        break;

                    case MMISMS_TYPE_MMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_READ, MIDDLE_BUTTON, TRUE);
                        break;

                    default:
                        break;
                }

            }
        }
        break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);

            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                    && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        HandleChatSmsTag(win_data_ptr,active_ctrl_id);
                        break;
#ifdef MMS_SUPPORT
                    case MMISMS_TYPE_MMS:
                        //解决cr MS00150274,U盘使用过程中不能看MMS
                        if (MMIAPIUDISK_UdiskIsRun())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                            break;
                        }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                        if (MMIAPIBROWSER_IsRunning())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                            break;
                        }
#endif
#endif                        
                        if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                        {
                            win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                            SCI_TRACE_LOW("[%s:%d]del_index: %d\n",__FUNCTION__,__LINE__,win_data_ptr->del_index );
                            MMISMS_SetDelIndex(win_data_ptr->del_index);
                        }
                        if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                        {
#ifdef MMS_SUPPORT
                            MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                        }
                        MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);

                        //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                        if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                        }
                        break;
#endif
                    default:
                        break;
                }
            }
        }
        break;
#endif

        case MSG_CTL_NOT_IN_DISPLAYREC:
        case MSG_CTL_IN_DISPLAYREC:
            {
                MMI_HANDLE_T handle = *((MMI_HANDLE_T *)param);
                MMI_CTRL_ID_T id = MMK_GetCtrlId(handle);
                uint16 index = 0;
                
                SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                MMI_CheckAllocatedMemInfo();
                if (id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID && id < MMISMS_MAX_SHOW_CHAT_SMS_ITEM_FATHER_FORM_CTRL_ID)
                {
                    index = id-MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
                    handle = MMK_ConvertIdToHandle(MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID+index);
                    MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID+index,msg_id,NULL);
                }
                SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                MMI_CheckAllocatedMemInfo();
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
#if defined(HERO_ENGINE_TTS_SUPPORT)
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
		gHeroTtsSmsListCtrlId = 0xFFFFFFFF;
#endif	
            MMK_CloseWin( win_id );
        }
        break;

        case MSG_CLOSE_WINDOW:
        {
            // to get the content which the user input
            MMICOM_BANNER_INFO_T save_banner = {0};            
            MMI_STRING_T edit_string = {0};
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &edit_string);
			
#if defined(HERO_ENGINE_TTS_SUPPORT)
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
		gHeroTtsSmsListCtrlId = 0xFFFFFFFF;
#endif	

            if (edit_string.wstr_len > 0)
            {
                if(!win_data_ptr->is_closed_by_editMMS)
                {
                    if(!win_data_ptr->is_email_number)
                    {
                        num_len = MMIAPICOM_GenDispNumber(
                                      MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                                      MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                                      win_data_ptr->chat_number.number,
                                      number,
                                      MMISMS_PBNUM_MAX_LEN + 2
                                  );

                        // to save the information
                        win_data_ptr->edit_strlen = edit_string.wstr_len;
                        SCI_MEMCPY(win_data_ptr->edit_string,edit_string.wstr_ptr,sizeof(wchar)*win_data_ptr->edit_strlen);
                        MMISMS_SetAddressToMessage((uint8 *)number, (uint8)strlen((char *)number), &win_data_ptr->edit_content.dest_info.dest_list);    
                        MMISMS_SetIsSaveDraft(TRUE);            
                        SMS_SAVE(MN_DUAL_SYS_1,win_data_ptr);
                        save_banner.banner_type = MMI_BANNER_TEXT;                        
                        save_banner.text1_id =  STXT_STORED;
                        is_exit_from_save_draft = TRUE;

                        MMIAPICOM_OpenBannerWin(MMK_GetFocusWinId(),save_banner); 
                      }
#if defined (MMS_SUPPORT)
                      else
                      {
                          MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
                          MMIMMS_EDIT_DOCUMENT_T *   editdoc_ptr = PNULL;    
                          MMI_STRING_T  string_to = {0}; 
                          wchar  address_wstr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                          string_to.wstr_ptr = address_wstr;
                          
                          MMIMMS_InitData(); 
                          editdoc_ptr = MMIMMS_CreateEditDocument();
                          if(PNULL == editdoc_ptr)
                          {
                              break;
                          }
                          mms_op_result = MMIMMS_AddSlide(editdoc_ptr,1);
                          if (MMIMMS_NO_ERROR != mms_op_result)
                          {
                              MMIMMS_EditMMSAlertWin(mms_op_result);
                              break;
                          }              
                          MMIAPICOM_StrToWstr(win_data_ptr->sender, string_to.wstr_ptr);
                          string_to.wstr_len = MMIAPICOM_Wstrlen(string_to.wstr_ptr);
                          MMIMMS_SetMMSTo(&string_to,editdoc_ptr);
                          SCI_MEMSET( editdoc_ptr->wstr_ptr, 0,  sizeof( editdoc_ptr->wstr_ptr ));
                          
                          if(edit_string.wstr_len > 0)
                          {        
                              MMI_WSTRNCPY(editdoc_ptr->wstr_ptr, MMIMESSAGE_DIAPLAY_LEN, edit_string.wstr_ptr, MMIMESSAGE_DIAPLAY_LEN, MMIMESSAGE_DIAPLAY_LEN);
                          }
                          MMIMMS_AddText(editdoc_ptr, &edit_string);
  
                          mms_op_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, editdoc_ptr);
                          if (MMIMMS_NO_ERROR != mms_op_result)
                          {
                              MMIMMS_EditMMSAlertWin(mms_op_result);
                          }
                          else
                          {
                              MMICOM_BANNER_INFO_T save_banner = {0};
                              save_banner.banner_type = MMI_BANNER_TEXT;                        
                              save_banner.text1_id =  STXT_STORED;
                              MMIAPICOM_OpenBannerWin(MMK_GetFocusWinId(),save_banner);
                          }
                          MMIMMS_MMSEditExit();
                      }        
#endif
                }
                else
                {
                    if(PNULL != win_data_ptr->chat_draft_order_id)
                    {
                        MMISMSAPP_DeleteSms(win_data_ptr->chat_draft_order_id,MMISMS_DeleteOneDraftSmscallback,PNULL,PNULL);
                        MMISMS_SetIsDraftDeleting(TRUE);
                    }
                }
            }
            else
            {
                if(PNULL != win_data_ptr->chat_draft_order_id)
                {
                    MMISMSAPP_DeleteSms(win_data_ptr->chat_draft_order_id,MMISMS_DeleteOneDraftSmscallback,PNULL,PNULL);
                    MMISMS_SetIsDraftDeleting(TRUE);
                }
            }            
            is_draft_existed = FALSE;
            MMISMS_SetDraftOperationOrderId(PNULL);
            
#ifdef TTS_SUPPORT
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
#endif
            MMITHEME_SetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT, MMISMS_CHAT_FOCUS_COLOR);
            MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
            MMISMS_SetExceedUnreadNeedUpdate();
            MMISMS_UpdateUnreadOrder();            
            ChatWinCloseCtrlFunc(win_data_ptr);            
            DestroyMMSEditDoc(win_data_ptr);
            s_need_delay_active = FALSE;
        }
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :init all editing MMS attachments into the given list 
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void AppendAttaListItem(MMI_CTRL_ID_T list_ctrl_id, MMIFMM_FILE_TYPE_E file_type, uint32 atta_index_code, uint32 specific_file_type,
                                                            uint8 *file_name_ptr,wchar* file_size, wchar* file_suffix,uint8 *multim_buf_ptr, uint32 multim_buf_len, MMI_IMAGE_ID_T img_id)
{
    uint16 cur_item_index = 0;
    uint16 file_name_len = 0;
    GUILIST_ITEM_T item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0}; /*lint !e64*/
    wchar file_name_wstr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    GUIITEM_STYLE_E item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS;
    //MMIFMM_FILE_TYPE_E file_type = (MMIFMM_FILE_TYPE_E)(atta_index_code & 0xFF);

    file_name_len = (uint8)strlen(file_name_ptr);
    GUI_UTF8ToWstr(file_name_wstr, file_name_len, file_name_ptr, file_name_len);

    switch(file_type)
    {
        case MMIFMM_FILE_TYPE_PICTURE:
        case MMIFMM_FILE_TYPE_MOVIE:            
        {
            item_style = GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_2TEXT_MS;//GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_2TEXT_MS;
            if(multim_buf_ptr)
            {

                item_data.item_content[0].item_data_type     = GUIITEM_DATA_ANIM_DATA; 

            }
            else
            {
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = img_id;
            }

            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name_wstr;
            item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;

            item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = file_size;
            item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(file_size);
            
            //item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
            //item_data.item_content[1].item_data.image_id = imgae_id;
        }
        break;
        
        case MMIFMM_FILE_TYPE_MUSIC:
            {
                item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS;
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = file_name_wstr;
                item_data.item_content[0].item_data.text_buffer.wstr_len = file_name_len;
                
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_suffix;
                item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(file_suffix);

                item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[2].item_data.text_buffer.wstr_ptr = file_size;
                item_data.item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(file_size);   
            }            
            break;
            
        case MMIFMM_FILE_TYPE_VCARD:
#ifdef MMI_VCALENDAR_SUPPORT            
        case MMIFMM_FILE_TYPE_VCALENDAR:
#endif            
        {
            item_style = GUIITEM_STYLE_ONE_LINE_TEXT_MS;//temp
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = file_name_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = file_name_len;
        }
        break;

        //case MMIFMM_FILE_TYPE_MUSIC:
        //break;

        default:
            break;
    }

    item_t.item_style = item_style;
    item_t.user_data = atta_index_code;
    item_t.item_data_ptr = &item_data;

    GUILIST_AppendItem(list_ctrl_id, &item_t);
    //cur_item_index = GUILIST_GetTotalItemNum(list_ctrl_id);
    //GUILIST_SetItemData(list_ctrl_id, &item_data, (cur_item_index - 1));
}
PUBLIC MMIFMM_FILE_TYPE_E MMISMS_GetAccessoryFileType(uint32 accessory_type)
{
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    switch(accessory_type)
    {
	case MMS_TYPE_MIDI:
	case MMS_TYPE_WAV:
	case MMS_TYPE_AMR:
	case MMS_TYPE_MP3:
	case MMS_TYPE_MMF:
	case MMS_TYPE_IMY:
	case MMS_TYPE_WMA:
	case MMS_TYPE_AAC:
	case MMS_TYPE_M4A:
           file_type = MMIFMM_FILE_TYPE_MUSIC;            
           break;
           
	case MMS_TYPE_WBMP:
	case MMS_TYPE_GIF:
	case MMS_TYPE_PNG:
	case MMS_TYPE_JPEG:
	case MMS_TYPE_BMP1:   /* support bmp only for parse a received mms */
           file_type = MMIFMM_FILE_TYPE_PICTURE;
           break;
           
	case MMS_TYPE_MP4:
       case MMS_TYPE_3GP:
       case MMS_TYPE_AVI:
           file_type = MMIFMM_FILE_TYPE_MOVIE;
           break;
           
        default:
           break;           
     }
     return file_type; 
}

/*****************************************************************************/
//  Description :init all MMS attachments into the given list 
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void InitMMSAttaViewList(MMI_CTRL_ID_T list_ctrl_id, uint32 record_id,MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
#if defined (MMS_SUPPORT)
    //the lowest 8bit : type
    //the sub-lowest 8bit:slide_number(current max length is 20)
    //the sub-highest 8bit:accessory index(max number is not fixed but i think 128 is enough,only for vcf file)
    //the highest 8bit: reserved
    uint32 file_size = 0;
    uint32 atta_index_code = 0;
    uint32 specific_file_type = 0;//!!!we need type string to show, later we must modify to support to get the name by specific_file_type!!!
    uint32 slide_number = 0;
    uint32 accessory_index = 0;
    uint16 atta_file_num = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    uint8 *multim_buf_ptr                  = PNULL;
    uint32 multim_buf_len                 = 0;
    uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};
    wchar file_name_wstr[MMIMMS_FILE_NAME_LEN + 1] = {0};
    wchar file_size_wstring[MMISMS_FILE_SIZE_DISP_LEN + 1] = {0};
    wchar suffix_wstr[MMIMMS_FILE_NAME_LEN + 1] = {0};
    uint16 suffix_len = 0;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    P_MMS_SLIDE_T slide_ptr = PNULL;
    P_MMS_EDIT_BODY_T     editbody_ptr = PNULL;
    MMS_ACCESSORY_PARAM_T*	accessory_ptr = PNULL;	

    if(PNULL == edit_doc_ptr ||PNULL == edit_doc_ptr->editbody_ptr)
    {
        return;
    }

    atta_file_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
    GUILIST_SetMaxItem(list_ctrl_id, atta_file_num, FALSE);

    editbody_ptr = edit_doc_ptr->editbody_ptr;
    slide_ptr = editbody_ptr->slide_root;

    //Traverse all multimedia type files
    for(; slide_ptr; slide_ptr = slide_ptr->next, ++slide_number)
    {
        SCI_MEMSET(file_size_wstring, 0, sizeof(file_size_wstring));
        SCI_MEMSET(file_name, 0, sizeof(file_name));
        multim_buf_ptr = PNULL;
        if(slide_ptr->image_flag)
        {
            atta_index_code = 0;//fix bug487638
    //extract image file info and append it into list
            file_type = MMIFMM_FILE_TYPE_PICTURE;
            file_size = MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);
            GetFileSizeDispString(file_size,file_size_wstring);
#ifdef DRM_SUPPORT
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
            {
                img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }
            else
#endif
            {
                MMIMMS_GetMultimediaBufferInSlide(file_type, &multim_buf_len, &multim_buf_ptr, slide_ptr);
                if (PNULL  == multim_buf_ptr)
                {
                    img_id = IMAGE_COMMON_PIC_UDISK_ICON;//what should we display if get img data buffer failed?
                }
            }
            atta_index_code|=(uint32)file_type;
            atta_index_code|=(slide_number<<8);
            atta_index_code|=(accessory_index<<16);
         
            //AppendAttaListItem(list_ctrl_id, atta_index_code, specific_file_type, file_name, multim_buf_ptr, multim_buf_len, img_id);
            AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, specific_file_type, file_name, file_size_wstring,PNULL,multim_buf_ptr, multim_buf_len, img_id);
        }

        if(slide_ptr->video_flag)
        {
            atta_index_code = 0;//fix bug487638
            SCI_MEMSET(file_size_wstring, 0, sizeof(file_size_wstring));
            SCI_MEMSET(file_name, 0, sizeof(file_name));
            //extract video file info and append it into list
            file_type = MMIFMM_FILE_TYPE_MOVIE;
            file_size = MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);
            GetFileSizeDispString(file_size,file_size_wstring);
            
#ifdef DRM_SUPPORT
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
            {
                img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }
            else
#endif
            {
                img_id = IMAGE_MESSAGING_ATTACHMENT_VIDEO;
            }

            atta_index_code|=(uint32)file_type;
            atta_index_code|=(slide_number<<8);
            atta_index_code|=(accessory_index<<16);
            AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, specific_file_type, file_name, file_size_wstring,PNULL,PNULL, 0, img_id);
        }

        if(slide_ptr->audio_type)
        {    
            atta_index_code = 0;//fix bug487638
            SCI_MEMSET(file_size_wstring, 0, sizeof(file_size_wstring));
            SCI_MEMSET(file_name, 0, sizeof(file_name));
            file_type = MMIFMM_FILE_TYPE_MUSIC;
            file_size = MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);
            GetFileSizeDispString(file_size,file_size_wstring);
            MMIAPICOM_StrToWstr(file_name, file_name_wstr);
            suffix_len = MMIMMS_FILE_NAME_LEN; 
            MMIAPIFMM_SplitFileName(file_name_wstr, MMIAPICOM_Wstrlen(file_name_wstr), PNULL, PNULL, suffix_wstr, &suffix_len);
            MMIAPICOM_Wstrupper(suffix_wstr);  
            atta_index_code|=(uint32)file_type;
            atta_index_code|=(slide_number<<8);
            atta_index_code|=(accessory_index<<16);                    
            AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, specific_file_type, file_name, file_size_wstring,suffix_wstr,PNULL, 0, img_id);
        }        
    }

    //Traverse all contact card files
    multim_buf_ptr = PNULL;
    multim_buf_len = 0;
    accessory_ptr = editbody_ptr->accessory_ptr;
    for(; accessory_ptr; accessory_ptr = accessory_ptr->next, accessory_index++)
    {		
        SCI_MEMSET(file_size_wstring, 0, sizeof(file_size_wstring));
        SCI_MEMSET(file_name, 0, sizeof(file_name));
        atta_index_code = 0;
        multim_buf_ptr = PNULL;
        if((strlen(accessory_ptr->content_type_string) != 0) &&       //coverity 26078
        (  (0 == GUI_StringCompare(accessory_ptr->content_type_string, VCF_CONTENT_TYPE_LEN, VCF_CONTENT_TYPE_NAME, VCF_CONTENT_TYPE_LEN))
	    || (0 == GUI_StringCompare(accessory_ptr->content_type_string, ATTACHMENT_CONTENT_TYPE_DEFAULT_LEN, ATTACHMENT_CONTENT_TYPE_DEFAULT_NAME, ATTACHMENT_CONTENT_TYPE_DEFAULT_LEN))
		))//equal
        {
            file_type = MMIFMM_FILE_TYPE_VCARD;
            atta_index_code|=(uint32)file_type;
            atta_index_code|=(slide_number<<8);
            atta_index_code|=(accessory_index<<16);
            atta_index_code|=(1<<24);  // this flag is used to figure out accessory
            
            AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, specific_file_type, (uint8*)accessory_ptr->filename,PNULL,PNULL, multim_buf_ptr, multim_buf_len, img_id);
        }
 #ifdef MMI_VCALENDAR_SUPPORT       
        else if((strlen(accessory_ptr->content_type_string) != 0) &&       //coverity 26078
          (0 == GUI_StringCompare(accessory_ptr->content_type_string, VCS_CONTENT_TYPE_LEN, VCS_CONTENT_TYPE_NAME, VCS_CONTENT_TYPE_LEN)))//equal
        {
            file_type = MMIFMM_FILE_TYPE_VCALENDAR;
            atta_index_code|=(uint32)file_type;
            atta_index_code|=(slide_number<<8);
            atta_index_code|=(accessory_index<<16);
            atta_index_code|=(1<<24);  // this flag is used to figure out accessory
            
            AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, specific_file_type, (uint8*)accessory_ptr->filename,PNULL,PNULL, multim_buf_ptr, multim_buf_len, img_id);
        }
#endif        
        else
        {
            if((strlen(accessory_ptr->content_type_string) != 0) )
            {
                file_type = MMISMS_GetAccessoryFileType(accessory_ptr->type);

                if( MMIFMM_FILE_TYPE_PICTURE == file_type)
                {
            //extract image file info and append it into list
                    file_type = MMIFMM_FILE_TYPE_PICTURE;
                    //file_size = MMIMMS_GetMultimediaFileInfoInAccessory(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, accessory_ptr);
                    GetFileSizeDispString(accessory_ptr->contentsize,file_size_wstring);
#ifdef DRM_SUPPORT
                    if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
                    {
                        img_id = IMAGE_DRM_PIC_LOCK_ICON;
                    }
                    else
#endif
                    {
                        //MMIMMS_GetMultimediaBufferInSlide(file_type, &multim_buf_len, &multim_buf_ptr, slide_ptr);
                        sysVFGetConstFileData(accessory_ptr->filename,&multim_buf_ptr,(int32 *)&multim_buf_len);

                        if (PNULL  == multim_buf_ptr)
                        {
                            img_id = IMAGE_COMMON_PIC_UDISK_ICON;//what should we display if get img data buffer failed?
                        }
                    }
                    atta_index_code|=(uint32)file_type;
                    atta_index_code|=(slide_number<<8);
                    atta_index_code|=(accessory_index<<16);
                    atta_index_code|=(1<<24);  // this flag is used to figure out accessory
                    
                    //AppendAttaListItem(list_ctrl_id, atta_index_code, specific_file_type, file_name, multim_buf_ptr, multim_buf_len, img_id);
                    AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, accessory_ptr->type, accessory_ptr->filename, file_size_wstring,PNULL,multim_buf_ptr, multim_buf_len, img_id);
                }

                if( MMIFMM_FILE_TYPE_MOVIE == file_type)
                {
                    //extract video file info and append it into list
                    file_type = MMIFMM_FILE_TYPE_MOVIE;
                    //file_size = MMIMMS_GetMultimediaFileInfoInAccessory(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, accessory_ptr);
                    GetFileSizeDispString(accessory_ptr->contentsize,file_size_wstring);
                    
#ifdef DRM_SUPPORT
                    if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(accessory_ptr->filename) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(accessory_ptr->filename))
                    {
                        img_id = IMAGE_DRM_PIC_LOCK_ICON;
                    }
                    else
#endif
                    {
                        img_id = IMAGE_MESSAGING_ATTACHMENT_VIDEO;
                    }

                    atta_index_code|=(uint32)file_type;
                    atta_index_code|=(slide_number<<8);
                    atta_index_code|=(accessory_index<<16);
                    atta_index_code|=(1<<24);  // this flag is used to figure out accessory
                    
                    AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, accessory_ptr->type, accessory_ptr->filename, file_size_wstring,PNULL,PNULL, 0, img_id);
                }

                if(MMIFMM_FILE_TYPE_MUSIC == file_type)
                {    
                    file_type = MMIFMM_FILE_TYPE_MUSIC;
                    //file_size = MMIMMS_GetMultimediaFileInfoInAccessory(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, accessory_ptr);
                    GetFileSizeDispString(accessory_ptr->contentsize,file_size_wstring);
                    MMIAPICOM_StrToWstr(accessory_ptr->filename, file_name_wstr);
                    suffix_len = MMIMMS_FILE_NAME_LEN; 
                    MMIAPIFMM_SplitFileName(file_name_wstr, MMIAPICOM_Wstrlen(file_name_wstr), PNULL, PNULL, suffix_wstr, &suffix_len);
                    MMIAPICOM_Wstrupper(suffix_wstr);  
                    atta_index_code|=(uint32)file_type;
                    atta_index_code|=(slide_number<<8);
                    atta_index_code|=(accessory_index<<16);
                    atta_index_code|=(1<<24);  // this flag is used to figure out accessory
                                       
                    AppendAttaListItem(list_ctrl_id, file_type, atta_index_code, accessory_ptr->type, accessory_ptr->filename, file_size_wstring,suffix_wstr,PNULL, 0, img_id);
                }                     
            }
        }               
        
    }
#endif
}

/*****************************************************************************/
//  Description :GetFileSizeDispString
//  Global resource dependence : 
//  Author:rong.gu
//  Note: [in] fils_size   [out]disp_str
/*****************************************************************************/
LOCAL void GetFileSizeDispString(uint32 file_size, wchar *disp_str)
{
    uint8 file_size_str[MMISMS_FILE_SIZE_DISP_LEN] = {0};       
    uint32 file_kb_size = 0;
    if(PNULL == disp_str)
    {
        return;
    }
    file_kb_size = file_size / ONE_K_LEN;
    if (0 == file_kb_size)
    {          
        _snprintf((void*)file_size_str, MMISMS_FILE_SIZE_DISP_LEN,"%dKB",1);//at least 1KB
    }
    else
    {
        _snprintf((void*)file_size_str, MMISMS_FILE_SIZE_DISP_LEN,"%dKB",file_kb_size);
    }
    MMI_STRNTOWSTR(disp_str, MMISMS_FILE_SIZE_DISP_LEN, (uint8 *)file_size_str, MMISMS_FILE_SIZE_DISP_LEN, sizeof(file_size_str));
}
LOCAL void SetAttachmentListEditDoc(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr )
{
    attach_editdoc_ptr = edit_doc_ptr;
}

LOCAL MMIMMS_EDIT_DOCUMENT_T* GetAttachmentListEditDoc(void)
{
    return attach_editdoc_ptr;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DrawAttachmentListItemWithPicture(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                      uint32 record_id 
                                      )
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    GUIANIM_DATA_INFO_T     anim_data    = {0};      /*lint !e64*/
    uint32 atta_index_code = 0;
    uint32 slide_number = 0;
    uint32 accessory_index = 0;
    
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    P_MMS_SLIDE_T slide_ptr = PNULL;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    uint32 specific_file_type = 0;
    uint8 *multim_buf_ptr                  = PNULL;
    uint32 multim_buf_len                 = 0;
    uint8 is_accessory = 0;
    
    MMS_ACCESSORY_PARAM_T*	accessory_ptr = PNULL;	
    GUILIST_GetItemData(ctrl_id,item_index, &atta_index_code);
    slide_number = (atta_index_code>>8) & 0xff;
    accessory_index = (atta_index_code>>16) & 0xff;
    file_type = (atta_index_code & 0xf);
    is_accessory = (atta_index_code >> 24 ) & 0xf;    
    edit_doc_ptr = GetAttachmentListEditDoc();    
    
#if defined (MMS_SUPPORT)
    if(MMIFMM_FILE_TYPE_PICTURE == file_type)    
    {  
        if(is_accessory)
        {
            accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
            sysVFGetConstFileData(accessory_ptr->filename,&multim_buf_ptr,(int32 *)&multim_buf_len);
        }
        else
        {
            slide_ptr = MMIMMS_GetSlidePtrByIndex(edit_doc_ptr,slide_number);
            MMIMMS_GetMultimediaBufferInSlide(MMIFMM_FILE_TYPE_PICTURE, &multim_buf_len, &multim_buf_ptr, slide_ptr);        
        }
        if (PNULL  == multim_buf_ptr)
        {
            return;
        }
    }    
    else
#endif
    {
        return;
    }
    item_content.item_data_type = GUIITEM_DATA_ANIM_DATA;
    anim_data.data_ptr = multim_buf_ptr;
    anim_data.data_size = multim_buf_len;
    item_content.item_data.anim_data_ptr = &anim_data;
    GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);           
}


PUBLIC MMIMMS_ERROR_E MMISMS_SavePreviewFile(uint8 *fname,
                                                                               MMIFMM_FILE_TYPE_E file_type
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
);

PUBLIC MMIMMS_ERROR_E MMISMS_GenTempPreviewFile(uint8 *fname,
                                                                               MMIFMM_FILE_TYPE_E file_type
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
);
#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description :mms attachment list opt win handle fun
//  Global resource dependence : 
//  Author:arvin.zhang
//  Note: 
/*****************************************************************************/
 LOCAL MMI_RESULT_E HandleMessageAttachmentListOptWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              )
{
    uint32 record_id  = 0;
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id     = MMISMS_MMS_ATTACHMENT_LIST_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;

    MMI_STRING_T str_msg = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
	wchar temp_arr1[MMISMS_PBNAME_MAX_LEN+1] = {0};

    record_id = (uint32)MMK_GetWinAddDataPtr(win_id);

    SCI_TRACE_LOW("HandleMessageAttachmentListOptWinMsg msg_id = 0x%x", msg_id);
    SCI_TRACE_LOW("HandleMessageAttachmentListOptWinMsg is_enter_music_preview = %d", is_enter_music_preview);

    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_ATTALISTOPTWIN_100_112_2_18_2_39_3_437,(uint8*)"d", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            edit_doc_ptr = MMIMMS_ReadMMS(record_id);
            SetAttachmentListEditDoc(edit_doc_ptr);
            InitMMSAttaViewList(list_ctrl_id, record_id,edit_doc_ptr);
            GUILIST_SetOwnSofterKey(list_ctrl_id, TRUE);
            GUILIST_SetCurItemIndex(list_ctrl_id, 0);
            CleanTempFile();
            is_enter_music_preview = FALSE;
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;

        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {            
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            uint16                      item_index;
            uint16                      item_content_index;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                item_content_index = need_item_content_ptr->item_content_index;
                DrawAttachmentListItemWithPicture(item_index,item_content_index,list_ctrl_id,record_id);
            }
        }
        break;           

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            GUIANIM_DATA_INFO_T anim_data = {0};   
            uint32 atta_index_code = 0;
            MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
            uint16 item_index  = 0;
            uint32 slide_number = 0;
            uint32 accessory_index = 0;

            P_MMS_SLIDE_T slide_ptr = PNULL;
            uint8 *multim_buf_ptr                  = PNULL;
            uint32 multim_buf_len                 = 0;
            uint32 specific_file_type = 0;
            uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};
            uint8 is_accessory = 0;
            MMS_ACCESSORY_PARAM_T*            accessory_ptr = PNULL;
            item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            GUILIST_GetItemData(list_ctrl_id,item_index, &atta_index_code);
            slide_number = (atta_index_code>>8) & 0xff;
            accessory_index = (atta_index_code>>16) & 0xff;
            is_accessory = (atta_index_code>>24) & 0xff;
            file_type = (atta_index_code & 0xf);

            edit_doc_ptr = GetAttachmentListEditDoc();
            if(is_accessory)
            {
                accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
                SCI_MEMCPY(file_name,accessory_ptr->filename,MMS_FILE_NAME_LEN);
                specific_file_type = accessory_ptr->type;
            }
            else
            {
                slide_ptr =  MMIMMS_GetSlidePtrByIndex(edit_doc_ptr,slide_number);
                MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);            
            }

#ifdef DRM_SUPPORT
            if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);    
            }
            else
#endif
            {
               if(MMIFMM_FILE_TYPE_PICTURE == file_type)    
               {
                   if(is_accessory)
                   {      
	                 sysVFGetConstFileData(file_name,&multim_buf_ptr,(int32 *)&multim_buf_len);                        
                   }
                   else
                   {
                        MMIMMS_GetMultimediaBufferInSlide(MMIFMM_FILE_TYPE_PICTURE, &multim_buf_len, &multim_buf_ptr, slide_ptr);                                           
                   }
                   if (PNULL  == multim_buf_ptr)
                   {
                       SCI_TRACE_LOW("mmisms no slide pic found");
                       return err_code;
                   }
                   anim_data.data_ptr = multim_buf_ptr;
                   anim_data.data_size = multim_buf_len;
                   MMISMS_EnterPreviewPicture(&anim_data);
               }
               else if(MMIFMM_FILE_TYPE_MUSIC == file_type)
               {                   
                   MMIMMS_ERROR_E mms_op_res = MMIMMS_NO_ERROR;
                   MMIFILE_ERROR_E file_error = SFS_NO_ERROR;
                   file_error = CleanTempFile();
                   if(file_error != SFS_NO_ERROR)
                   {
                       return err_code;
                   }
                   if(MMIFMM_FILE_TYPE_MUSIC != MMISMS_GetAccessoryFileType(specific_file_type)) // for gcf test , audio file is text file
                   {
                       return err_code;
                   }
                   mms_op_res = MMISMS_GenTempPreviewFile(file_name,
                                                                     MMIFMM_FILE_TYPE_MUSIC
                                                                     #ifdef DRM_SUPPORT
                                                                     ,item_index
                                                                     #endif                                                                     
                                                                     );
                   if(MMIMMS_NO_ERROR == mms_op_res)      
                   {                                          
                        MMIAPIMP3_PlayFile (file_save_path, (const uint32) MMIAPICOM_Wstrlen(file_save_path));         
                        is_enter_music_preview = TRUE;
                   }
               } 
#ifdef VIDEO_PLAYER_SUPPORT
               else if(MMIFMM_FILE_TYPE_MOVIE == file_type)
               {
                   MMISMS_VIDEO_PREVIEW_T video_preview = {0};
                   MMIAPIVP_ExitPlay();
                   SCI_MEMCPY(video_preview.file_name,file_name,MMIMMS_FILE_NAME_LEN);
                   video_preview.file_type = specific_file_type;
                   MMISMS_EnterVideoPreviewWin(&video_preview);
               }
#endif
#ifdef  MMI_VCARD_SUPPORT               
               else if(MMIFMM_FILE_TYPE_VCARD == file_type)
               {
                   MMIMMS_ERROR_E mms_op_res = MMIMMS_NO_ERROR;
                   MMS_ACCESSORY_PARAM_T* accessory_ptr = PNULL;
                   MMIFILE_ERROR_E file_error = SFS_NO_ERROR;

                   file_error = CleanTempFile();
                   if(file_error != SFS_NO_ERROR)
                   {
                       return err_code;
                   }
                   
                   accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
                   if(accessory_ptr != PNULL)
                   {
                       mms_op_res = MMIMMS_SaveMMSContent(accessory_ptr->filename, FALSE, file_save_path
#ifdef DRM_SUPPORT
                        ,item_index
#endif
                       ); 
                       MMIAPIPB_OpenVcardFileContactWin(file_save_path);
                   }
               }
#endif         
#ifdef  MMI_VCALENDAR_SUPPORT              
               else if(MMIFMM_FILE_TYPE_VCALENDAR == file_type)
               {
                   MMIMMS_ERROR_E mms_op_res = MMIMMS_NO_ERROR;
                   MMS_ACCESSORY_PARAM_T* accessory_ptr = PNULL;
                   MMIFILE_ERROR_E file_error = SFS_NO_ERROR;

                   file_error = CleanTempFile();
                   if(file_error != SFS_NO_ERROR)
                   {
                       return err_code;
                   }
                   
                   accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
                   if(accessory_ptr != PNULL)
                   {
                       mms_op_res = MMIMMS_SaveMMSContent(accessory_ptr->filename, FALSE, file_save_path
#ifdef DRM_SUPPORT
                        ,item_index
#endif
                       );
					   MMIAPISCH_ReadVcalendarFile (file_save_path);
                   }
               }
#endif    
           }
        }
        break;

        case MSG_APP_OK:
        case MSG_CTL_OK:
           //save to SD card    
           {
                    GUIANIM_DATA_INFO_T anim_data = {0};   
                    uint32 atta_index_code = 0;
                    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
                    uint16 item_index  = 0;
                    uint32 slide_number = 0;
                    uint32 accessory_index = 0;

                    P_MMS_SLIDE_T slide_ptr = PNULL;
                    uint8 *multim_buf_ptr                  = PNULL;
                    uint32 multim_buf_len                 = 0;
                    uint32 specific_file_type = 0;
                    uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};
                    uint8 is_accessory = 0;
                    MMS_ACCESSORY_PARAM_T*            accessory_ptr = PNULL;
                   
                   item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
                   GUILIST_GetItemData(list_ctrl_id,item_index, &atta_index_code);
                   slide_number = (atta_index_code>>8) & 0xff;
                   accessory_index = (atta_index_code>>16) & 0xff;
                   is_accessory = (atta_index_code>>24) & 0xff;
                   file_type = (atta_index_code & 0xf);

                   /*
                   // delete this code for BUG526391. The place where can save the picture from MMS is not only sd card but phone.
                   // And the system should save to wherever that according to the user settings which is the sd card or phone.
                   // so this code should not finish at the beginning if device does not insert sd card.
                   if(!MMIFILE_GetDeviceSDStatus())
                   {
                      MMIPUB_OpenAlertSuccessWin(STR_NOTE_NO_CARD);

                      return err_code;
                   }*/

                   edit_doc_ptr = GetAttachmentListEditDoc();
                   if(is_accessory)
                   {
                       accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
                       SCI_MEMCPY(file_name,accessory_ptr->filename,MMS_FILE_NAME_LEN);
                       specific_file_type = accessory_ptr->type;
                   }
                   else
                   {
                       slide_ptr =  MMIMMS_GetSlidePtrByIndex(edit_doc_ptr,slide_number);
                       MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);            
                   }
    #ifdef DRM_SUPPORT
                if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);    
                }
                else
    #endif     
                {
                       if(MMIMMS_NO_ERROR == MMISMS_SavePreviewFile(file_name,
                                                 file_type
                                                 #ifdef DRM_SUPPORT
                                                  ,item_index
                                                   #endif                                                                     
                                                  ))
                       {
                           MMIPUB_OpenAlertSuccessWin(STR_ATTACHMENT_SAVED_EXT01);
                       }
                }
           }
        break;

        case MSG_FULL_PAINT:
        break;
        
        case MSG_GET_FOCUS:
            // added by Tong Yongli
            cur_order_id = MMISMS_GetCurOperationOrderId();
			str_msg.wstr_ptr = temp_arr1;
            if(cur_order_id != PNULL)
            {
                MMISMS_GetNameInfo((MMISMS_STATE_T *)cur_order_id, &str_msg, MMISMS_PBNAME_MAX_LEN);
            }

            if(is_enter_music_preview)
            {
                MMIAPIMP3_StopAudioPlayer();
                CleanTempFile();
                is_enter_music_preview = FALSE;
            }  
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            if(is_enter_music_preview)
            {
                MMIAPIMP3_StopAudioPlayer();
                CleanTempFile();
                is_enter_music_preview = FALSE;
            }
            MMK_CloseWin(win_id);
        break;

        case MSG_CLOSE_WINDOW:
#if defined (MMS_SUPPORT)
             edit_doc_ptr = GetAttachmentListEditDoc();
             MMIMMS_DestroyEditDocument(edit_doc_ptr);
#endif
             CleanTempFile();             
        break;

        default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}
#endif
#if defined (MMS_SUPPORT) && defined (VIDEO_PLAYER_SUPPORT)    
/*****************************************************************************/
//  Description :mms preview richtext play 3gp item 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMISMS_EnterVideoPreviewWin(MMISMS_VIDEO_PREVIEW_T *video_preview)
{
    MMISMS_VIDEO_PREVIEW_T  *data_ptr = PNULL;

    data_ptr = (MMISMS_VIDEO_PREVIEW_T*) SCI_ALLOCA (sizeof(MMISMS_VIDEO_PREVIEW_T));

    if (video_preview != PNULL && data_ptr != PNULL)
    {
        SCI_MEMSET(data_ptr,0, sizeof(MMISMS_VIDEO_PREVIEW_T));
        SCI_MEMCPY(data_ptr, video_preview, sizeof(MMISMS_VIDEO_PREVIEW_T));
        MMK_CreateWin ( (uint32*) MMISMS_VIDEO_PREVIEW_WIN_TAB, (ADD_DATA) data_ptr);
    }
}
/*****************************************************************************/
//  Description :mms preview richtext play 3gp item 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_PreviewPlayVideoItem(MMISMS_VIDEO_PREVIEW_T*         video_preview_ptr)
{    
    uint32 video_type     = MMS_TYPE_MAX;   
    DPLAYER_MEDIA_INFO_T media_info = {0};
    uint8 voice_volume    = MMIAPISET_GetCallRingVolume();//cr191444 MutMultimVolume can not be set and user set in idle or setting menu to make also VP's;
    VP_PLAY_PARAM_T play_param  = {0};
    GUI_RECT_T full_rect = {0};
    GUICTRL_STATE_T dispay_state     = 0;
    MMI_HANDLE_T play_ctrl_handle    = NULL;
    uint32 specific_file_type = 0;
    if(PNULL == video_preview_ptr)
    {
        return FALSE;
    }
    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {//forcr247444
        return FALSE;
    }
#ifdef DRM_SUPPORT
    {
        //uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};
        //get file name
        //MMIMMS_GetMultimediaFileInfoInSlide(MMIFMM_FILE_TYPE_MOVIE, &specific_file_type,file_name, MMIMMS_FILE_NAME_LEN, slide_ptr);
  
        if (MMIFMM_DCF_FILE ==  MMIMMS_GetDrmFileTypeFromName(video_preview_ptr->file_name))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);                   
            return FALSE;
        }        
    }
#endif
    SCI_MEMSET(&play_param,0,sizeof(VP_PLAY_PARAM_T));
    if (sysVFGetConstFileData(video_preview_ptr->file_name,(int8 **)&(play_param.video_buf_info.video_buf_ptr),(int32 *)&(play_param.video_buf_info.video_buf_len)))
    {
        video_type = video_preview_ptr->file_type;
        
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
        if(play_param.video_buf_info.video_buf_len > MMIMMS_LIMIT_VEDIO_SIZE)
        {
            return FALSE;
        }
#endif
        switch(video_type) 
        {
        case MMS_TYPE_3GP:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
            break;
#ifdef AVI_DEC_SUPPORT
        case MMS_TYPE_AVI:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_AVI;
            break;
#endif
        case MMS_TYPE_MP4:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_MP4;
            break;
        default:
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }           
  
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_IN_CALL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
        else if (MMIAPIMTV_IsMTVRunning())
        {
            MMSMMI_OpenMMSAlertWin(TXT_EXIT_MOBILETV, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#ifdef BROWSER_SUPPORT
        else if (MMIAPIBROWSER_IsRunning())
        {
            MMSMMI_OpenMMSAlertWin(TXT_BROWSER_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#endif
#ifdef VIDEO_PLAYER_SUPPORT 
        else if (MMIAPIVP_IsOpenPlayerWin())
        {
            MMSMMI_OpenMMSAlertWin(TXT_VP_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#endif
        else
        {
            MMIAPIVP_GetMediaInfoFromBuf(play_param.video_buf_info.video_buf_ptr,play_param.video_buf_info.video_buf_len,play_param.video_buf_info.type,&media_info);
            //不显示title             
            MMIMMS_SetWinStatusBarVisible(MMISMS_VIDEO_PREVIEW_WIN_ID,FALSE);  
                
            //关闭3d效果
            //MMI_Enable3DMMI(FALSE);  
#ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_MMS_PREVIEW);
#endif            
            MMIAPISET_StopAllRing(FALSE);
            if (!MMIMMS_Setting_GetPlaysound())
            {
                voice_volume = MMISET_VOL_ZERO;
            }
            play_param.play_type = VP_PLAY_FROM_BUFFER;   
            
            play_param.display_param.audioSupport = 1;
            play_param.display_param.videoSupport = 1;
            play_param.display_param.RotateAngle = 1;          
            play_param.is_scal_up = TRUE;
            play_param.display_param.disp_rect.x = 0;
            play_param.display_param.disp_rect.y = 0;

            play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
            full_rect = MMITHEME_GetFullScreenRect();   
            if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_90;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_270;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else         
            {
                play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
            }

            play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
            play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
            play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
            play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
            play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
            play_param.vol = voice_volume;
            
            if (MMIAPIVP_Play(&play_param,MMISMS_VideoPlayCallBack,0))
            {              
                MMK_SetWinSupportAngle(MMISMS_VIDEO_PREVIEW_WIN_ID, WIN_SUPPORT_ANGLE_CUR);
            }
            return TRUE;
        }                
               
    }
    else 
    {
        SCI_TRACE_LOW("MmsPreviewPlayVideoItem data error");       
        return FALSE;
    }
}
#endif

/*****************************************************************************/
//  Description :video play callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
#ifdef VIDEO_PLAYER_SUPPORT
LOCAL void MMISMS_VideoPlayCallBack(VP_CALLBACK_T *msg_ptr)
{
    GUICTRL_STATE_T dispay_state     = 0;
    MMI_HANDLE_T view_ctrl_handle    = NULL;
    MMI_HANDLE_T play_ctrl_handle    = NULL;
        
    if (PNULL == msg_ptr)
    {
        return;
    }    
    
    SCI_TRACE_LOW("[VP] MmsVideoPlayCallBack msg_ptr->msg_id = %x",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {       
    case VP_STOP_IND: 
        {
            MMK_SetWinSupportAngle(MMISMS_VIDEO_PREVIEW_WIN_ID, WIN_SUPPORT_ANGLE_ALL);            
            MMIMMS_SetWinStatusBarVisible(MMISMS_VIDEO_PREVIEW_WIN_ID,TRUE);              
        }
        
        //关闭3d效果
        //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_MMS_PREVIEW);
#endif        
        MMIAPISET_StopAllRing(TRUE);
        MMK_CloseWin(MMISMS_VIDEO_PREVIEW_WIN_ID);
        break;


    default:
        break;
    }
}
#endif

/*****************************************************************************/
// Description : save preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMISMS_SavePreviewFile(uint8 *fname,
                                                                               MMIFMM_FILE_TYPE_E file_type
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
)
{
    MMIMMS_ERROR_E mms_op_res                        = MMIMMS_NO_ERROR;
    //uint8 multi_temp[MMS_FILE_NAME_LEN*3 + 1]        = {0};
    
#if defined (MMS_SUPPORT)
    uint32 specific_file_type = 0;
    uint32 file_size = 0;
    wchar save_path[MMIMMS_FILE_NAME_LEN + 1] = {0};

    //file_size = MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,multi_temp, MMIMMS_FILE_NAME_LEN, slide_ptr);

    mms_op_res = MMIMMS_SaveMMSContent(fname, FALSE, save_path
#ifdef DRM_SUPPORT
                ,selectIndex
#endif
                );   

    if (MMIMMS_NO_ERROR != mms_op_res)
    {
        //没有保存成功需要删除文件
        MMIAPIFMM_DeleteFile(save_path, PNULL);
        if (MMIMMS_ERROR_NO_SPACE == mms_op_res)
        {
            MMSMMI_OpenMMSAlertWin(STR_NO_SPACE_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
        }
        else
        {
            MMIMMS_EditMMSAlertWin(mms_op_res);
        }                
    }
#endif
    return mms_op_res;
    
}

/*****************************************************************************/
// Description : show preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMISMS_GenTempPreviewFile(uint8 *fname,
                                                                               MMIFMM_FILE_TYPE_E file_type
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
)
{
    MMIMMS_ERROR_E mms_op_res                        = MMIMMS_NO_ERROR;
    //uint8 multi_temp[MMS_FILE_NAME_LEN*3 + 1]        = {0};
    
#if defined (MMS_SUPPORT)
    uint32 specific_file_type = 0;
    uint32 file_size = 0;

    //file_size = MMIMMS_GetMultimediaFileInfoInSlide(file_type, &specific_file_type,multi_temp, MMIMMS_FILE_NAME_LEN, slide_ptr);

    mms_op_res = MMIMMS_SaveMMSContent(fname, FALSE, file_save_path
#ifdef DRM_SUPPORT
                ,selectIndex
#endif
                );   

    if (MMIMMS_NO_ERROR != mms_op_res)
    {
        //没有保存成功需要删除文件
        MMIAPIFMM_DeleteFile(file_save_path, PNULL);
        if (MMIMMS_ERROR_NO_SPACE == mms_op_res)
        {
            MMSMMI_OpenMMSAlertWin(STR_NO_SPACE_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
        }
        else
        {
            MMIMMS_EditMMSAlertWin(mms_op_res);
        }                
    }
#endif
    return mms_op_res;
    
}

/*****************************************************************************/
// Description : show preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CleanTempFile(void)
{
    MMIFILE_ERROR_E file_error = SFS_NO_ERROR;

    SCI_TRACE_LOW("[MMISMS] CleanTempFile");
    if(MMIAPICOM_Wstrlen(file_save_path))
    {
        file_error = MMIAPIFMM_DeleteFile(file_save_path, PNULL);
        SCI_MEMSET(file_save_path,0,sizeof(file_save_path));
    }
	return file_error;
}
/*****************************************************************************/
// Description : show preview picture
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterPreviewPicture (GUIANIM_DATA_INFO_T    *anim_data_ptr)
{
    GUIANIM_DATA_INFO_T  *data_ptr = PNULL;

    data_ptr = (GUIANIM_DATA_INFO_T*) SCI_ALLOCA (sizeof(GUIANIM_DATA_INFO_T));

    if (data_ptr != PNULL)
    {
        SCI_MEMSET(data_ptr,0, sizeof(GUIANIM_DATA_INFO_T));
        SCI_MEMCPY(data_ptr, anim_data_ptr, sizeof(GUIANIM_DATA_INFO_T));
        MMK_CreateWin ( (uint32*) MMISMS_PIC_PREVIEW_WIN_TAB, (ADD_DATA) data_ptr);
    }
}

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicturePreviewWin (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    GUIANIM_DATA_INFO_T         *anim_data_ptr = (GUIANIM_DATA_INFO_T *) MMK_GetWinAddDataPtr (win_id);
    uint16        full_path_len = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set anim param
        {
            GUIANIM_CTRL_INFO_T     ctrl_info = {0};
            GUIANIM_DISPLAY_INFO_T disp_info = {0};
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMISMS_PICTURE_PREVIEW_CTRL_ID;
            disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            disp_info.is_play_once = FALSE;
            disp_info.bg.bg_type = GUI_BG_COLOR;
            disp_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

            GUIANIM_SetParam(&ctrl_info,anim_data_ptr,PNULL,&disp_info);
            MMK_SetAtvCtrl (win_id, MMISMS_PICTURE_PREVIEW_CTRL_ID);
        }
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#if defined (MMS_SUPPORT) && defined (VIDEO_PLAYER_SUPPORT)
/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPreviewWin (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMISMS_VIDEO_PREVIEW_T* video_preview_ptr = (MMISMS_VIDEO_PREVIEW_T*) MMK_GetWinAddDataPtr (win_id);
    static  VP_HANDLE s_vp_handle = PNULL; 
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            BOOLEAN ret = TRUE;
            ret = MMISMS_PreviewPlayVideoItem(video_preview_ptr);
            if(!ret)
            {
                MMK_CloseWin (win_id);        
            }   
			else
			{
				s_vp_handle = VP_GetCurrentFSMHandle();
			}
        }
        break;
    case MSG_GET_FOCUS:
		//MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
		MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,VP_Setting_GetBrightness()); 
		GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
		MMIAPIVP_FSMCompulsiveResume(s_vp_handle);       
		break;
		 
	case MSG_LOSE_FOCUS:
		MMIAPIVP_FSMCompulsivePause(s_vp_handle);
		break;
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);        
        break;

    case MSG_CLOSE_WINDOW:
#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
        MMK_FreeWinAddData (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif



/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void ChatWinCloseCtrlFunc (MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    if (win_data_ptr != PNULL)
    {
        SCI_FREE(win_data_ptr);
    }
    MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, FALSE); 
    MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, FALSE); 
}

/*****************************************************************************/
//  Description : 号码添加到联系人后需要更新显示
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatReadWinUpdateDispNum(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{        
    MMI_STRING_T name = {0};
    GUISTBDATA_TEXT_T stb_data_text = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};

    name.wstr_ptr = temp_arr;
    pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
    MMIAPIPB_GetNameByNumber(&pb_num,&name,MMISMS_PBNAME_MAX_LEN,FALSE);

    GUISTBDATA_GetItemText(0,&stb_data_text);
    stb_data_text.wstr_len = name.wstr_len;
    SCI_MEMCPY(stb_data_text.wstr_ptr, name.wstr_ptr,
		sizeof(wchar)*MIN(stb_data_text.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN)); 
    GUISTBDATA_SetItemText(0,&stb_data_text);
}

/*****************************************************************************/
//  Description : 获取聊天显示窗口中某条信息的order_id
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetChatWinItemId(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return PNULL;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    return order_id;
}

/*****************************************************************************/
//  Description : to handle the message enable send
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsSendEnable(BOOLEAN is_update)
{
    MMI_STRING_T str = {0};
    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str);
    if (0 == str.wstr_len)
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, 2, TRUE);
#else
        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_OPTION, TXT_NULL, STXT_RETURN, is_update);
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, 2, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_OPTION, TXT_SEND, TXT_DELETE, is_update);
#endif
    }
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinInsertOptOK(MMIPUB_MENU_NOTIFY_T *param )
{       
    uint32 group_id, menu_id;
   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleChatWinInsertOptOK param pnull");
        return;
    }
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    if (menu_id != MMISMS_MENU_INSERT_TXT)
    {
    #ifdef MMS_SUPPORT
        MMIMMS_ERROR_E edit_err = MMIMMS_NO_ERROR;
        edit_err = MMIAPIMMS_EnterEditCheck();
        if (MMIMMS_NO_ERROR != edit_err)
        {
            //提示彩信初始化失败
            MMK_CloseWin(param->dst_handle);
            MMIAPIMMS_EditMMSAlertWin(edit_err);
            return;
        }
    #endif
    }
    
    switch (menu_id)
    {
        case MMISMS_MENU_INSERT_SUBJECT:
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                MMI_STRING_T mms_content_str = {0};
                uint8 num_len = 0;
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL == mms_content_str.wstr_ptr)
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869, (uint8*)"");
                    break;
                }
                MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                str_to.wstr_ptr = wnumber;
                str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif                 
                if(MMIAPIMMS_EditAddMMSSubject(&mms_content_str,&str_to))
                {
                    MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
                }
                
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    SCI_FREE(mms_content_str.wstr_ptr);
                    mms_content_str.wstr_ptr = PNULL;
                }
            }
	    break;
         
        case MMISMS_MENU_INSERT_PIC:
        {
            SmsChatEditInsertPic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }    
        break;
    
        case MMISMS_MENU_INSERT_PHIZ:
            MMISMS_OpenPhizWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case MMISMS_MENU_INSERT_CONTACT_INFO:
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_NUMBER,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                EditSmsSelectContactCallback
            );
            //SetIsAddContact(TRUE);
            win_data_ptr->is_add_contact = TRUE;
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case     MMISMS_MENU_INSERT_TXT:  //插入范本
            MMISMS_CreateSelectWordWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case     MMISMS_MENU_INSERT_AUDIO:
        {
            SMSEditInsertMusic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        break;
        case MMISMS_MENU_INSERT_TAKE_PIC:
        {
            SMSOpenDC();
        }
        break;

#ifdef MMI_RECORD_SUPPORT
        case MMISMS_MENU_INSERT_TAKE_AUDIO:
        {
            SMSOpenRecord();
        }
        break;
#endif
    
        case MMISMS_MENU_INSERT_TAKE_VIDEO:
        {
            SMSOpenDV();
        }
        break;
    
        case     MMISMS_MENU_INSERT_VIDEO:
        {
            SMSEditInsertVideo(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        break;
    
        case     MMISMS_MENU_INSERT_NEXTPAGE:
        case     MMISMS_MENU_INSERT_PREPAGE:
        {
            MMI_STRING_T      str_ptr = {0};
            MMI_STRING_T      str_to = {0};
            MMI_STRING_T mms_content_str = {0};
            uint8 num_len = 0;
            uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            uint32 slide_direct = 0;
            GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
            mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
            if (PNULL == mms_content_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"mmisms alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869, (uint8*)"");
                break;
            }
            //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
            mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);
            
            num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
            str_to.wstr_ptr = wnumber;
            str_to.wstr_len = num_len;
        
            if (menu_id == MMISMS_MENU_INSERT_NEXTPAGE)
            {
                slide_direct = 1;
            }
            else
            {
                slide_direct = 0;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif   
            if (MMIAPIMMS_EditAddMMSSlide(slide_direct, &mms_content_str, &str_to))
            {
                MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
            }
            SCI_FREE(mms_content_str.wstr_ptr);
        }
        break;
    
        case MMISMS_MENU_INSERT_ATTACH_VCARD:
            //MMIAPIPB_OpenNumberList(MMIPB_LIST_VCARD,MMIPB_LIST_TYPE_SINGLE,1,PNULL);
    
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_WHOLE,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                EditSelectVcardCallback
            );
    
            MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
            break;
    
        case MMISMS_MENU_INSERT_ATTACH_OTHER:
            MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
            SMSEditInsertNormalAttachment(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            break;
    
        default:
            break;
    }
}
#endif


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : truncate first line of msg content
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TruncateMsgContent(MMI_STRING_T *msg_content_ptr )
{
    int16 i;
    if (msg_content_ptr == PNULL)
    {
        return FALSE;
    }
    for (i = 0; i < msg_content_ptr->wstr_len; i++)
    {
        if (msg_content_ptr->wstr_ptr[i] == 0x000d)
        {
            msg_content_ptr->wstr_len = i;
            return TRUE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptInit(MMIPUB_MENU_NOTIFY_T *param )
{       
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T msg_content = {0};
#ifdef TTS_SUPPORT
    MMI_STRING_T    tts_read_menu = {0};
#endif
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_CHAT_ITEM_OPT_CTRL_ID;
    MMI_STRING_T    lock_menu = {0};
        
    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    MMISMS_ChatWinGetItemContent(&msg_content);
    TruncateMsgContent(&msg_content);
    //Truncate this content,only use first line
    GUIMENU_SetMenuTitle(&msg_content, ctrl_id);
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if (PNULL != cur_order_id && (MMISMS_MO_SEND_FAIL != cur_order_id->flag.mo_mt_type || 
                MMISMS_IsSmsBusy(cur_order_id) || 
                (MMISMS_TYPE_MMS == cur_order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(cur_order_id->record_id)
                ||MMIAPIUDISK_UdiskIsRun()
                ))))
    {
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_RESEND_ITEM_ID, FALSE);
    }
    if (PNULL != cur_order_id && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
    {
#ifdef MMI_SMS_COPY_SUPPORT
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_COPY_ITEM_ID,TRUE);
#endif
#ifdef SNS_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_SINA_ID, FALSE);
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_FACEBOOK_ID, FALSE);
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_TIWTTER_ID, FALSE);
#endif
#ifdef TTS_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
#endif
    }
    if (PNULL != cur_order_id && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type && MMIAPIUDISK_UdiskIsRun())
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_FORWARD_ITEM_ID,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_DEL_ITEM_ID,TRUE);
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_BOX_SECURITY_ITEM_ID,TRUE);
#endif
    }
#ifdef MMI_SMS_COPY_SUPPORT
    if (PNULL != cur_order_id && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type && MN_SMS_STORAGE_SIM == cur_order_id->flag.storage)
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_COPY_OPT,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);
    }

    if (PNULL != cur_order_id && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type && cur_order_id ->flag.storage == MN_SMS_STORAGE_ME)
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_COPY_OPT,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE);
    }
#endif
#ifdef TTS_SUPPORT
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
    } 
    GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
    
    if (MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
    {
        MMI_GetLabelTextByLang(STXT_STOP, &tts_read_menu);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SET_TTS_PLAY, &tts_read_menu);
    }
    GUIMENU_SetStaticItem(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, &tts_read_menu, 0);
#endif
    if(PNULL != cur_order_id)
    {
        if (cur_order_id->flag.is_locked)
        {
            MMI_GetLabelTextByLang(TXT_SMS_UNLOCK, &lock_menu);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_DEL_ITEM_ID,TRUE);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_LOCK, &lock_menu);
        }
        GUIMENU_SetStaticItem(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_LOCK_ITEM_ID, &lock_menu, 0);
    }

}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptOK(MMIPUB_MENU_NOTIFY_T *param )
{       
    uint32 group_id, menu_id;
   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    switch (menu_id)
    {
        case MMISMS_MENU_FORWARD_ITEM_ID:          
            HandleChatWinForwardMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleChatWinLockOrUnLockMsg(win_data_ptr);
            break;
                        
        case MMISMS_MENU_DEL_ITEM_ID:
            HandleChatWinDelSingle(win_data_ptr);
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:            
            HandleChatWinMoveSecurity(win_data_ptr);
            break;
#endif
#ifdef MMI_SMS_COPY_SUPPORT
        case MMISMS_MENU_BOX_COPYTOME_ITEM_ID:
            HandleChatWinCopySimToMe();
            break;

        case MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID:
            HandleChatWinCopyMeToSim(win_data_ptr);
            break;
#endif
#ifdef SNS_SUPPORT
        case MMISMS_MENU_SHARE_TO_SINA_ID:
        case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
        case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
            HandleChatWinShareSNS(win_data_ptr);
            break;        
#endif
        
#ifdef TTS_SUPPORT
        case MMISMS_MENU_TTS_READ_ID:
            HandleChatWinReadTTS();
            break;            
#endif       
        case MMISMS_MENU_RESEND_ITEM_ID:
            HandleChatWinResendItem(win_data_ptr);
            break;
        default:
            break;
    }
}
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinResendItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
//    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
//    MMI_WIN_ID_T win_id = win_data_ptr->win_id;
//    uint16 num_len = 0;
    cur_order_id = MMISMS_GetCurOperationOrderId();
    if(PNULL == cur_order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        //cur_order_id->backsend_status = MMISMS_SENDSTATUS_SENDING; //set this order is busy
        BuildSendData(win_data_ptr->win_id,TRUE);
        HandleChatWinSendSms(win_data_ptr->win_id);

    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    else if(MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
    {
        MMIAPIMAIL_SendMailById((uint32)mail_getMsgIdByRecordId(cur_order_id->record_id));
    }
#endif
    else //is mms
    {
        MMIAPIMMS_OutboxSendMMS((uint32)cur_order_id->record_id);
    }
}


#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinReadTTS(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        MMI_STRING_T    msg_content = {0};
        if (MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
        }
        else
        {
            MMISMS_ChatWinGetItemContent(&msg_content);
            if (PNULL != msg_content.wstr_ptr)
            {
                MMITTS_PLAY_PARAM_T play_param = {0};
    
                play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                play_param.is_ucs2 = TRUE;
                play_param.play_times = 1;
                play_param.tts_vol = MMIAPISET_GetMultimVolume();
                play_param.ring_type = TTS_RING_TYPE_SMS;
                play_param.win_handle = MMISMS_SHOW_CHAT_SMS_WIN_ID;
                play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
                play_param.digit_mode = TTS_DIGIT_AUTO;
                play_param.punc_mode = TTS_PUNC_OFF;
                play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
                
                MMIAPITTS_PlayText(&play_param);
    
                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
    }        
}
#endif  

/*****************************************************************************/
//     Description : open sms chat edit menu ( dyanmic menu)
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void  OpenSMSChatEditOptPopMenu(void)
{

        GUIMENU_POP_SEL_INFO_T     pop_info     = {0};
        GUIMENU_DYNA_ITEM_T     node_item = {0};
        MMI_STRING_T            dict_string = {0};
        GUIMENU_TITLE_INFO_T        title_info = {0};
        MMI_STRING_T                main_title = {0};
        MMI_STRING_T                sub_title = {0};
        uint8 lang_name_len = 0;
        uint8 i=0;
        uint8 menu_index =0;
        MMI_STRING_T string = {0};
        GUIIM_LANGUAGE_T sel_im_index  =  0;
        MMISET_LANGUAGE_TYPE_E  cur_im_language = MMISET_LANGUAGE_ENGLISH;
        MMI_STRING_T edit_string = {0};
        uint16 sms_num = 0;
        sel_im_index = MMIAPIIM_GetLanguage();
        cur_im_language = (MMISET_LANGUAGE_TYPE_E)MMIAPIIM_GetSetLangByGUILangIdx(sel_im_index);
       
        MMK_CreateWin((uint32 *)MMISMS_EDIT_MAINMENU_OPT_WIN_TAB,PNULL);   
        GUIMENU_CreatDynamic(   PNULL,      MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID,
                                  MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID,     GUIMENU_STYLE_POPUP  );

        MMI_GetLabelTextByLang(STR_SMS_HDR_INSERT_OPTIONS, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(menu_index++, MMISMS_MENU_INSERT_OPTIONS, 0, &node_item, MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
        GUIMENU_SetSubMenuStyle(MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID,MMISMS_MENU_INSERT_OPTIONS,GUIMENU_STYLE_POPUP);
        i = 0;
        MMI_GetLabelTextByLang(STR_SMILEYS_LIST_INSERT, &string);
        node_item.item_text_ptr = &string;
        if((0 == GUIEDIT_GetLeftNumInTextEditBox(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,&sms_num)) && sms_num == MMISMS_SPLIT_MAX_NUM)
        {
            node_item.is_grayed = TRUE;
        } 
        else
        {
            node_item.is_grayed = FALSE;
        }
        GUIMENU_InsertNode(i++,MMISMS_MENU_INSERT_SMILEY,MMISMS_MENU_INSERT_OPTIONS,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

        MMI_GetLabelTextByLang(STR_LIST_DQE_INS_SYMBOL, &string);
        node_item.item_text_ptr = &string;
        GUIMENU_InsertNode(i++,MMISMS_MENU_INSERT_SYMBOL,MMISMS_MENU_INSERT_OPTIONS,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

        MMI_GetLabelTextByLang(STR_SMS_INSERT_TEMPLATE, &string);
        node_item.item_text_ptr = &string;
        GUIMENU_InsertNode(i++,MMISMS_MENU_INSERT_TEMPLATE,MMISMS_MENU_INSERT_OPTIONS,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

#ifdef MMS_SUPPORT
        MMI_GetLabelTextByLang(STR_MSG_LIST_ADD_ATTACHMENTS_EXT01, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(menu_index++, MMISMS_MENU_INSERT_ELEMENT, 0, &node_item, MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
        GUIMENU_SetSubMenuStyle(MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID,MMISMS_MENU_INSERT_ELEMENT,GUIMENU_STYLE_POPUP);
        i = 0;
        MMI_GetLabelTextByLang(STR_UC_ADD_PICTURE_ID, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(i++,MMISMS_MENU_ADD_IMAGE,MMISMS_MENU_INSERT_ELEMENT,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
        MMI_GetLabelTextByLang(STR_MSG_LIST_ADD_SOUND_EXT01, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(i++,MMISMS_MENU_ADD_SOUND,MMISMS_MENU_INSERT_ELEMENT,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
        //MMI_GetLabelTextByLang(STR_MSG_LIST_ADD_VIDEO_EXT01, &string);
        //node_item.item_text_ptr = &string;
        //node_item.is_grayed = FALSE;
        //GUIMENU_InsertNode(i++,MMISMS_MENU_ADD_VIDEO,MMISMS_MENU_INSERT_ELEMENT,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
#endif
#if 0
	MMI_GetLabelTextByLang(STR_MSG_LIST_ADD_CONTACT_CARD_EXT01, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(i++,MMISMS_MENU_ADD_CONTACT_CARD,MMISMS_MENU_INSERT_ELEMENT,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
#endif
        MMI_GetLabelTextByLang(STXT_SMS_OPTION_CLEAR_ALL, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(menu_index++, MMISMS_MENU_CLEAR_TEXT_ITEM_ID, 0, &node_item, MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

        GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &edit_string);
        MMI_GetLabelTextByLang(STXT_SMS_OPTION_SAVE, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = (edit_string.wstr_len == 0);
        GUIMENU_InsertNode(menu_index++, MMISMS_MENU_SAVE_AS_DRAFT_ITEM_ID, 0, &node_item, MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

//BUG 492001 Tong Yongli
#if 0
        MMI_GetLabelTextByLang(STXT_T9_SETTINGS, &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(menu_index++, MMISMS_MENU_DICTIONARY_ITEM_ID, 0, &node_item, MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
        GUIMENU_SetSubMenuStyle(MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID,MMISMS_MENU_DICTIONARY_ITEM_ID,GUIMENU_STYLE_POPUP_RADIO);
        {

            MMI_GetLabelTextByLang(STXT_T9_SETTINGS_OFF, &string);
            node_item.item_text_ptr = &string; 
            node_item.is_grayed = FALSE;
            GUIMENU_InsertNode(0,MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF,MMISMS_MENU_DICTIONARY_ITEM_ID,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
            pop_info.is_static = FALSE;
            pop_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID;
            pop_info.node_id =MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF ;
            GUIMENU_SetPopItemStatus(FALSE,&pop_info);

        			
#ifdef  MMI_SIM_LANGUAGE_SUPPORT			
	for(i=1; i < MMISET_MAX_LANGUAGE; i++)
#else
	for(i=0; i < MMISET_MAX_LANGUAGE; i++)
#endif
	{    
		if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i)&&s_lang_T9_status_info[i][1])
		{
			lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name); 
			dict_string.wstr_ptr =s_lang_set[i].lang_name ;
			dict_string.wstr_len= lang_name_len;
			node_item.item_text_ptr= &dict_string;
			//node_item.item_text_ptr->wstr_len = lang_name_len;	
			node_item.is_grayed = FALSE;
#ifdef  MMI_SIM_LANGUAGE_SUPPORT			
                GUIMENU_InsertNode(i,i+MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF,MMISMS_MENU_DICTIONARY_ITEM_ID,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);
#else
                GUIMENU_InsertNode(i+1,i+1+MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF,MMISMS_MENU_DICTIONARY_ITEM_ID,&node_item,MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID);

#endif
                pop_info.is_static = FALSE;
                pop_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID;
#ifdef  MMI_SIM_LANGUAGE_SUPPORT			
                pop_info.node_id =i+MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF ;
#else
                pop_info.node_id =i+MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF+1 ;

#endif
                if(cur_im_language==i) 
                {
                    GUIMENU_SetPopItemStatus(TRUE,&pop_info);
                }
                else
                {
                    GUIMENU_SetPopItemStatus(FALSE,&pop_info);
                }
                }
        	}
        }
#endif
}

/*****************************************************************************/
//     Description : handle sms chat edit opt windows msg
//    Global resource dependence :
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSMSChatEditMenuOptWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID;
    uint32 node_id                        = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_HANDLE_T im_handle = 0;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_METHOD_T im_method = GUIIM_M_ENGLISH_ABC;
    
     win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入选项               
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
            {
                case MMISMS_MENU_INSERT_SMILEY:
                    MMISMS_OpenPhizWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    MMK_CloseWin(win_id);
                    break;
                 case MMISMS_MENU_INSERT_SYMBOL:                    
                    MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    GUIEDIT_GetImSymbol(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    MMK_CloseWin(win_id);
                    break;
                 case MMISMS_MENU_INSERT_TEMPLATE:
                    MMISMS_CreateSelectWordWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                    MMK_CloseWin(win_id);
                    break;

                 case MMISMS_MENU_ADD_IMAGE:
                    SmsChatEditInsertPic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;

                 case MMISMS_MENU_ADD_SOUND:
                    SMSEditInsertMusic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;

                 case MMISMS_MENU_ADD_VIDEO:
                    SMSEditInsertVideo(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                    MMK_CloseWin(win_id);
                    break;

                 case MMISMS_MENU_ADD_CONTACT_CARD:
                    SmsChatEditInsertVcard();
                    MMK_CloseWin(win_id);                    
                    break;

#ifdef  MMIMMS_SMS_IN_1_SUPPORT    
                case MMISMS_MENU_INSERT_ELEMENT:
				#ifdef HERO_ENGINE_TTS_SUPPORT 
				if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
				{        
					MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
				}
				#endif 	
                    HandleChatWinOpenInsertOpt(win_data_ptr); 

                    break;
#endif
                case MMISMS_MENU_FORWARD_ITEM_ID:          
                    HandleChatWinForwardMsg(win_data_ptr);
                    break;
                    
                case MMISMS_MENU_LOCK_ITEM_ID:
                    HandleChatWinLockOrUnLockMsg(win_data_ptr);
                    break;
                    
                case MMISMS_MENU_DEL_ITEM_ID:   
                    HandleChatWinDelSingle(win_data_ptr);
                    break;
#if 0 
                case MMISMS_MENU_SAVE_TO_ONENOTE_ID:
        		//There's no OneNote API available now.
                    break;
#endif
                    
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                case MMISMS_MENU_BOX_SECURITY_ITEM_ID:            
                    HandleChatWinMoveSecurity(win_data_ptr);
                    break;
#endif

#ifdef SNS_SUPPORT
                case MMISMS_MENU_SHARE_TO_SINA_ID:
                case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
                case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
                    HandleChatWinShareSNS(win_data_ptr);
                    break;        
#endif

#ifdef TTS_SUPPORT
                case MMISMS_MENU_TTS_READ_ID:
                    HandleChatWinReadTTS();
                    break;            
#endif                   
                case MMISMS_MENU_ANSWER_SMS_ITEM_ID:          
                    HandleChatWinAnswerSms(win_data_ptr);
                    break;
                    
                case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
                    HandleChatWinAnswerMMS(win_data_ptr);
                    break;
                    
                case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
                    HandleChatWinDelboxAll(win_data_ptr);           
                    break;
                    
                case MMISMS_MENU_DIAL_ITEM_ID:
                case MMISMS_MENU_IPDIAL_ITEM_ID:
                case MMISMS_MENU_VIDEOCALL_ITEM_ID:
                    HandleChatWinCallItem(win_data_ptr);              
                    break;
                    
                case  MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:                
                    HandleChatWinAddPBNew(win_data_ptr); 
                    break;
                    
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
                case  MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:    
                    HandleChatWinAddPBExist(win_data_ptr); 
                    break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
                case  MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:    
                    HandleChatWinAddPBBlackList(win_data_ptr); 
                    break;
#endif            
                case MMISMS_MENU_CLEAR_TEXT_ITEM_ID:
        	       GUIEDIT_ClearAllStr(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                     MMK_CloseWin(win_id);
        		 break;
                case MMISMS_MENU_SAVE_AS_DRAFT_ITEM_ID:
					MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_CTL_CANCEL, PNULL, 0);
                     MMK_CloseWin(win_id);			
        		 break;
                case MMISMS_MENU_DICTIONARY_ITEM_ID:
                case  MMISMS_DICTIONARY_LANGUAGE_ENGLISH:                  
                case  MMISMS_DICTIONARY_LANGUAGE_SIMP_CHINESE:             
                case  MMISMS_DICTIONARY_LANGUAGE_TRAD_CHINESE:             
                case  MMISMS_DICTIONARY_LANGUAGE_ARABIC:                   
                case  MMISMS_DICTIONARY_LANGUAGE_FRENCH:                   
                case  MMISMS_DICTIONARY_LANGUAGE_HINDI:                    
                case  MMISMS_DICTIONARY_LANGUAGE_HUNGARIAN:                
                case  MMISMS_DICTIONARY_LANGUAGE_INDONESIAN:               
                case  MMISMS_DICTIONARY_LANGUAGE_MALAY:                    
                case  MMISMS_DICTIONARY_LANGUAGE_PORTUGUESE:               
                case  MMISMS_DICTIONARY_LANGUAGE_RUSSIAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_SPANISH:                  
                case  MMISMS_DICTIONARY_LANGUAGE_TAGALOG:                  
                case  MMISMS_DICTIONARY_LANGUAGE_THAI:                     
                case  MMISMS_DICTIONARY_LANGUAGE_VIETNAMESE:               
                case  MMISMS_DICTIONARY_LANGUAGE_URDU:                     
                case  MMISMS_DICTIONARY_LANGUAGE_ITALIAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_PERSIAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_TURKISH:                  
                case  MMISMS_DICTIONARY_LANGUAGE_GERMAN:                   
                case  MMISMS_DICTIONARY_LANGUAGE_GREEK:                    
                case  MMISMS_DICTIONARY_LANGUAGE_HEBREW:                   
                case  MMISMS_DICTIONARY_LANGUAGE_BENGALI:                  
                case  MMISMS_DICTIONARY_LANGUAGE_CZECH:                    
                case  MMISMS_DICTIONARY_LANGUAGE_SLOVENIAN:                
                case  MMISMS_DICTIONARY_LANGUAGE_ROMANIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_TELUGU:                   
                case  MMISMS_DICTIONARY_LANGUAGE_MARATHI:                  
                case  MMISMS_DICTIONARY_LANGUAGE_TAMIL:                    
                case  MMISMS_DICTIONARY_LANGUAGE_GUJARATI:                 
                case  MMISMS_DICTIONARY_LANGUAGE_KANNADA:                  
                case  MMISMS_DICTIONARY_LANGUAGE_MALAYALAM:                
                case  MMISMS_DICTIONARY_LANGUAGE_ORIYA:                    
                case  MMISMS_DICTIONARY_LANGUAGE_PUNJABI:                  
                case  MMISMS_DICTIONARY_LANGUAGE_AFRIKAANS:                
                case  MMISMS_DICTIONARY_LANGUAGE_ALBANIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_ARMENIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_AZERBAIJANI:              
                case  MMISMS_DICTIONARY_LANGUAGE_BASQUE:                   
                case  MMISMS_DICTIONARY_LANGUAGE_BULGARIAN:                
                case  MMISMS_DICTIONARY_LANGUAGE_CATALAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_CROATIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_DANISH:                   
                case  MMISMS_DICTIONARY_LANGUAGE_DUTCH:                    
                case  MMISMS_DICTIONARY_LANGUAGE_ESTONIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_FILIPINO:                 
                case  MMISMS_DICTIONARY_LANGUAGE_FINNISH:                  
                case  MMISMS_DICTIONARY_LANGUAGE_GALICIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_GEORGIAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_HAUSA:                    
                case  MMISMS_DICTIONARY_LANGUAGE_ICELANDIC:                
                case  MMISMS_DICTIONARY_LANGUAGE_IGBO:                     
                case  MMISMS_DICTIONARY_LANGUAGE_IRISH:                    
                case  MMISMS_DICTIONARY_LANGUAGE_KAZAKH:                   
                case  MMISMS_DICTIONARY_LANGUAGE_LATVIAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_LITHUANIAN:               
                case  MMISMS_DICTIONARY_LANGUAGE_MACEDONIAN:               
                case  MMISMS_DICTIONARY_LANGUAGE_MOLDOVAN:                 
                case  MMISMS_DICTIONARY_LANGUAGE_NORWEGIAN:                
                case  MMISMS_DICTIONARY_LANGUAGE_POLISH:                   
                case  MMISMS_DICTIONARY_LANGUAGE_SERBIAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_SESOTHO:                  
                case  MMISMS_DICTIONARY_LANGUAGE_SLOVAK:                   
                case  MMISMS_DICTIONARY_LANGUAGE_SWEDISH:                  
                case  MMISMS_DICTIONARY_LANGUAGE_UKRAINIAN:                
                case  MMISMS_DICTIONARY_LANGUAGE_YORUBA:                 
                case  MMISMS_DICTIONARY_LANGUAGE_XHOSA:		                 
                case  MMISMS_DICTIONARY_LANGUAGE_ZULU:		                 
                case  MMISMS_DICTIONARY_LANGUAGE_ASSAMESE:		             
                case  MMISMS_DICTIONARY_LANGUAGE_SWAHILI:	                 
                case  MMISMS_DICTIONARY_LANGUAGE_MYANMAR:                  
                case  MMISMS_DICTIONARY_LANGUAGE_AMHARIC:                  
                case  MMISMS_DICTIONARY_LANGUAGE_KHMER:                    
                case  MMISMS_DICTIONARY_LANGUAGE_LAO:                      
                case  MMISMS_DICTIONARY_LANGUAGE_UYGHUR:                   
                case  MMISMS_DICTIONARY_LANGUAGE_TIBETAN:                  
                case  MMISMS_DICTIONARY_LANGUAGE_SINHALESE:	               
                case MMISMS_DICTIONARY_LANGUAGE_BOSNIAN:
                case MMISMS_DICTIONARY_LANGUAGE_TRAD_CHINESE_HK:
                case MMISMS_DICTIONARY_LANGUAGE_AMERICAN:	
                case MMISMS_DICTIONARY_LANGUAGE_PORTUGUESE_BR:
                case MMISMS_DICTIONARY_LANGUAGE_TURKMEN_LATIN:
                case MMISMS_DICTIONARY_LANGUAGE_SPANISH_AM: 
                case  MMISMS_DICTIONARY_MAX_LANGUAGE:                       
                {
                	GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
                	uint32 cur_dict_index =node_id-MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF;
                	GUI_POINT_T start_point = {0, 0};
                	MMICOM_BANNER_INFO_T banner_info = {0};
                    if(node_id == MMISMS_DICTIONARY_LANGUAGE_TRAD_CHINESE_HK)
                        cur_dict_index =  MMISMS_DICTIONARY_LANGUAGE_TRAD_CHINESE-MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF;
                    if(node_id == MMISMS_DICTIONARY_LANGUAGE_AMERICAN)
                        cur_dict_index =  MMISMS_DICTIONARY_LANGUAGE_ENGLISH-MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF;
                	lang = MMIAPIIM_GetGUILangBySetLangIdx((MMISET_LANGUAGE_TYPE_E)cur_dict_index);
                	MMIAPIIM_SetLanguage(lang);
                	//GUIEDIT_SetIm(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,GUIIM_TYPE_SET_ALL, GUIIM_TYPE_SMART);  // predictive on
                  //  im_method =MMITHEME_IMGetDefaultMethod();
                    notify_pack.data.set_lang = lang;
                    notify_pack.notify = GUIIM_NOTIFY_SET_LANG;
                    im_handle = GUIEDIT_GetIMHandle(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);                            
                    GUIIM_CommitData(im_handle, &notify_pack);                            
               	MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                	banner_info.banner_type = MMI_BANNER_TEXT;
                	banner_info.text1_id = STXT_T9_ENABLED_CONF;
                	banner_info.start_point = start_point;
                	MMIAPICOM_OpenBannerWin(MMISMS_SHOW_CHAT_SMS_WIN_ID, banner_info);
                	MMK_CloseWin(win_id);
                }
                break;    
                case MMISMS_DICTIONARY_LANGUAGE_DICTIONARY_OFF:
                {
                	GUI_POINT_T start_point = {0, 0};
                	MMICOM_BANNER_INFO_T banner_info = {0};
 //               	GUIEDIT_SetIm(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,GUIIM_TYPE_SET_ALL, GUIIM_TYPE_MULTITAP); // predictive off
                    notify_pack.data.set_type= GUIIM_TYPE_ABC;
                    notify_pack.notify = GUIIM_NOTIFY_SET_TYPE;
                    im_handle = GUIEDIT_GetIMHandle(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);   
                    GUIIM_CommitData(im_handle, &notify_pack);                            

                	MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                	banner_info.banner_type = MMI_BANNER_TEXT;
                	banner_info.text1_id = STXT_T9_DISABLED_CONF;
                	banner_info.start_point = start_point;
                	MMIAPICOM_OpenBannerWin(MMISMS_SHOW_CHAT_SMS_WIN_ID, banner_info);
                	MMK_CloseWin(win_id);
                }	
                	break;
                default:
                    break;
            }
                
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    
    return err_code;
}

/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    MMISMS_ORDER_ID_T oper_order_id = PNULL;
#ifndef MMI_PDA_SUPPORT        
    MMI_STRING_T    lock_menu = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#endif        

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);

    oper_order_id = GetChatWinItemId(win_data_ptr);
	if( NULL == oper_order_id ){
		SCI_TRACE_LOW("mmisms oper_order_id param PNULL");
		return;
	}

    if( (MMISMS_TYPE_MMS == oper_order_id->flag.msg_type ) && (MMISMS_MT_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type || MMISMS_MT_NOT_NOTIFY == oper_order_id->flag.mo_mt_type  || MMISMS_MT_NEED_NOT_DOWNLOAD == oper_order_id->flag.mo_mt_type ))
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_FORWARD_ITEM_ID, TRUE);
    }
    else
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_FORWARD_ITEM_ID, FALSE);
    }

	if (MMISMS_SENDSTATUS_SAVING == oper_order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == oper_order_id->backsend_status || MMISMS_SENDSTATUS_SENDING == oper_order_id->backsend_status)
	{
		MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_DEL_ITEM_ID, TRUE);
	}
    
    if(0 == MMISMS_GetChatAvailDeleteNum(pb_num))
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_DELBOX_ALL_ITEM_ID, TRUE);        
    }
    if(win_data_ptr->is_email_number)
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_CALL_ITEM_ID, TRUE);                
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_ADDNUMTOPB_ITEM_ID, TRUE);                
#if defined(MMI_BLACKLIST_SUPPORT)        
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID, TRUE);                
#endif
    }
    	
#ifndef MMI_PDA_SUPPORT    
    cur_order_id = MMISMS_GetCurOperationOrderId();    
    if (PNULL == cur_order_id)
    {
        return;
    }
    if (cur_order_id->flag.is_locked)
    {
        MMI_GetLabelTextByLang(TXT_SMS_UNLOCK, &lock_menu);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_LOCK, &lock_menu);
    }
    GUIMENU_SetStaticItem(MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_LOCK_ITEM_ID, &lock_menu, 0);
#endif    

    
}
/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptOK(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;
    
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    switch (menu_id)
    {
        case MMISMS_MENU_INSERT_SMILEY:
            MMISMS_OpenPhizWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
         case MMISMS_MENU_INSERT_SYMBOL:
            
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            GUIEDIT_GetImSymbol(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
         case MMISMS_MENU_INSERT_TEMPLATE:
            MMISMS_CreateSelectWordWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;

         case MMISMS_MENU_ADD_IMAGE:
		SmsChatEditInsertPic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            break;

         case MMISMS_MENU_ADD_SOUND:
		SMSEditInsertMusic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            break;

         case MMISMS_MENU_ADD_VIDEO:
		SMSEditInsertVideo(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            break;

         case MMISMS_MENU_ADD_CONTACT_CARD:
            SmsChatEditInsertVcard();            
            break;

#ifdef  MMIMMS_SMS_IN_1_SUPPORT    
        case MMISMS_MENU_INSERT_ELEMENT:
			#ifdef HERO_ENGINE_TTS_SUPPORT 
			if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
			{        
				MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
			}
			#endif 
            HandleChatWinOpenInsertOpt(win_data_ptr); 

            break;
#endif
        case MMISMS_MENU_FORWARD_ITEM_ID:          
            HandleChatWinForwardMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleChatWinLockOrUnLockMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_DEL_ITEM_ID:   
            HandleChatWinDelSingle(win_data_ptr);
            break;
#if 0
        case MMISMS_MENU_SAVE_TO_ONENOTE_ID:
		//There's no OneNote API available now.
            break;
#endif
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:            
            HandleChatWinMoveSecurity(win_data_ptr);
            break;
#endif

#ifdef SNS_SUPPORT
        case MMISMS_MENU_SHARE_TO_SINA_ID:
        case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
        case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
            HandleChatWinShareSNS(win_data_ptr);
            break;        
#endif

#ifdef TTS_SUPPORT
        case MMISMS_MENU_TTS_READ_ID:
            HandleChatWinReadTTS();
            break;            
#endif                   
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:          
            HandleChatWinAnswerSms(win_data_ptr);
            break;
            
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
            HandleChatWinAnswerMMS(win_data_ptr);
            break;
            
        case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
            HandleChatWinDelboxAll(win_data_ptr);           
            break;
            
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_IPDIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
            HandleChatWinCallItem(win_data_ptr);              
            break;
            
        case  MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:                
            HandleChatWinAddPBNew(win_data_ptr); 
            break;
            
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case  MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:    
            HandleChatWinAddPBExist(win_data_ptr); 
            break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case  MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:    
            HandleChatWinAddPBBlackList(win_data_ptr); 
            break;
#endif            
        case MMISMS_MENU_CLEAR_TEXT_ITEM_ID:
	       GUIEDIT_ClearAllStr(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
		 break;
	case MMISMS_MENU_DICTIONARY_ITEM_ID:
		//MMISMSEdit_OpenDictionaryPopMenu();	
		break;
        default:
            break;
    }

}

/*****************************************************************************/
//     Description : to handle the message of waiting window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReadWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_APP_RED:
            break;

        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            break;

        case MSG_CLOSE_WINDOW:
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }
    return (recode);
}

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerSms(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint16 num_len = 0;
    MMI_STRING_T string_to = {0};
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MN_DUAL_SYS_E     dual_sys = MN_DUAL_SYS_MAX;
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    cur_order_id = MMISMS_GetChatOperOrderID();
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    if (MMISMS_GetReplyPathFlag())
    {        
        if (cur_order_id != PNULL && cur_order_id->flag.is_concatenate_sms)
        {
            MMI_STRING_T prompt_str = {0};

            MMI_GetLabelTextByLang(STR_NOTE_WAITING, &prompt_str);
            MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMISMS_READSMS_WAIT_WIN_ID, IMAGE_NULL,
                               ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleReadWaitWinMsg);
        }
        // to read the pointed SMS
        //Just read the message,and the read cnf will updated the message center number
        if(cur_order_id != PNULL)  //fix coverity cid 18357
        {
            MMISMSAPP_ReadSmsCenter(cur_order_id,PNULL,PNULL,PNULL);
        }
    }
    else
#endif
    {
        if ( cur_order_id != PNULL)
        {
            dual_sys = cur_order_id->flag.dual_sys;
        }
        if (!win_data_ptr->is_email_number)
        {
            num_len = MMIAPICOM_GenDispNumber(
                MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                win_data_ptr->chat_number.number,
                number,
                MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_WriteNewMessage(
                dual_sys,
                PNULL,
                FALSE,
                number,
                num_len
            );
        }
        else
        {
            num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
            string_to.wstr_ptr = wnumber;
            string_to.wstr_len = num_len;
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIAPIMMS_SetIsEmailAddrExist(TRUE);
    #endif
            MMIAPIMMS_AnswerMMS(dual_sys, PNULL, &string_to);
        }

    }
}

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerMMS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint16 num_len = 0;
    MMI_STRING_T string_to = {0};
    wchar wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    else
    {
        return;
    }


    MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER);
    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);    
    string_to.wstr_ptr = wnumber;
    string_to.wstr_len = num_len;
    
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif          
    if(order_id != PNULL)
    {
        MMIAPIMMS_AnswerMMS(order_id->flag.dual_sys, PNULL, &string_to); //释放subject空间
    }


}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelboxAll(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

//	uint16 cur_index = 0;    
    MMIPB_BCD_NUMBER_T pb_num = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    if (MMISMS_IsUpdateOk())
    {

        pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
        MMISMS_SetLastChatMsgMarked(pb_num, TRUE);
    }
    win_data_ptr->is_del_all = TRUE;
    ChatWinOpenDelteQueryWin(win_data_ptr);
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }   
    if (MMISMS_IsUpdateOk())
    {
        MMI_CTRL_ID_T active_ctrl_id = 0;
        MMISMS_ORDER_ID_T order_id = PNULL;
    
        active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
        if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
        {
            order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
        }
        else
        {
            return;
        }
#ifdef MMS_SUPPORT
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
        {
            //解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                return;
            }
            if(MMIAPIMMS_IsProcessMMSById(order_id->record_id))
            {
                MMIMMS_SENDRECV_TYPE_E send_recv_type = MMIMMS_GetSendRecvType();
                MMICOM_BANNER_INFO_T delete_banner = {0};

                delete_banner.banner_type = MMI_BANNER_TEXT;
  
                if(MMIMMS_SENDRECV_SENDING == send_recv_type)
                {
                    delete_banner.text1_id = STR_MSG_BEING_SENT;                
                    MMIAPICOM_OpenBannerWin(MMK_GetFocusWinId(),delete_banner);
                    return;
                }
                else if(MMIMMS_SENDRECV_RECVING == send_recv_type)
                {
                    delete_banner.text1_id = STR_MSG_BEING_RECEIVED;                
                    MMIAPICOM_OpenBannerWin(MMK_GetFocusWinId(),delete_banner);
                    return;
                }
            }
        }
#endif
        win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;     
        SCI_TRACE_LOW("[%s:%d]del_index: %d\n",__FUNCTION__,__LINE__,win_data_ptr->del_index );
        MMISMS_SetDelIndex(win_data_ptr->del_index);
        MMISMS_SetCurOrderMarked(order_id,TRUE);
        win_data_ptr->is_del_all = FALSE;
        //win_data_ptr->chat_del_order_id = order_id;
        ChatWinOpenDelteQueryWin(win_data_ptr);
    }

}


/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBNew(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);    
    MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);  
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBExist(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);    
    MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
}
#endif

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinMoveSecurity(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }   
    if (MMISMS_IsUpdateOk())
    {
#ifdef MMI_SMS_MOVE_SUPPORT    
        HandleChatWinOpenMoveQueryWin(win_data_ptr);
#endif
    }

}
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBBlackList(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);  
    
    MMISMS_AddSenderToBlackList(pb_bcd);
}
#endif

/*****************************************************************************/
//     Description : handle chat win call item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCallItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    uint16 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};    
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

	if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    else
    {
        return;
    }
    if(order_id == PNULL)
    {
        return;            
    }
    
    num_len = MMIAPICOM_GenDispNumber(
        MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
        MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
        win_data_ptr->chat_number.number,
        number,
        MMISMS_PBNUM_MAX_LEN + 2
        );

    if (num_len > 0)
    {
        CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

        if (MMISMS_MENU_DIAL_ITEM_ID == win_data_ptr->menu_id)
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                        );
        }
        else if (MMISMS_MENU_IPDIAL_ITEM_ID == win_data_ptr->menu_id)
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_IP_CALL,
                            PNULL
                        );
        }
        else
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_VIDEO_CALL,
                            PNULL
                        );
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }

}

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleChatWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    //int            err             = 0;
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    widget_id = pubwin_info_ptr->user_data;

    switch (msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&win_data_ptr->pMe);

            if (win_data_ptr->pMe)
            {
                IMMISNS_AppInstall(win_data_ptr->pMe, widget_id);

                IMMISNS_Release(win_data_ptr->pMe);
                win_data_ptr->pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinShareSNS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    int share_id = 0;

    if (MMISMS_MENU_SHARE_TO_SINA_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_SINA;
    }
    else if (MMISMS_MENU_SHARE_TO_FACEBOOK_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_FACEBOOK;
    }
    else
    {
        share_id = MMI_SNS_TIWTTER;
    }

    IMMISNS_CreateObject(&win_data_ptr->pMe);

    if (win_data_ptr->pMe)
    {
        if (IMMISNS_IsFixAppInstalled(win_data_ptr->pMe, share_id) < 0)
        {
            IMMISNS_Release(win_data_ptr->pMe);
            win_data_ptr->pMe = PNULL;

            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(), PNULL, TXT_WHETHER_INSTALL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, HandleChatWinSnsInstallWarningWinMsg, share_id);
        }
        else
        {
            MMI_STRING_T msg_content = {0};

            MMISMS_ChatWinGetItemContent(&msg_content);

            if (PNULL != msg_content.wstr_ptr)
            {
                uint32        time_num    = 500;

                IMMISNS_ShareToSns(win_data_ptr->pMe, share_id, SNS_MODULE_SMS , (uint32)&msg_content);
                MMIPUB_OpenAlertWinByTextId(&time_num, STR_NOTE_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
    }   
}
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinForwardMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{         

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    if (PNULL == cur_order_id)
    {
        return;
    }

    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {   
        MMI_STRING_T msg_content = {0};    
        MMISMS_ChatWinGetItemContent(&msg_content);
        // open edit window
        MMISMS_SetIsForwardSms(TRUE);
        MMISMS_ClearDestNum();
        MMISMS_WriteNewMessage(
            MN_DUAL_SYS_MAX,
            &msg_content,
            FALSE,
            PNULL,
            0
        );
    
        if (PNULL != msg_content.wstr_ptr)
        {
            SCI_FREE(msg_content.wstr_ptr);
            msg_content.wstr_ptr = PNULL;
        }        
    }
    else //IS MMS, not OTA and PUSH
    {
		MMISMS_SetIsForwardSms(TRUE);
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD);
        MMIAPIMMS_FarwardMMS((uint32)cur_order_id->record_id);
    }
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinLockOrUnLockMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{         
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint16 index = 0;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    if (PNULL == cur_order_id)
    {
        return;
    }
    
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    cur_order_id->flag.is_locked = !cur_order_id->flag.is_locked;    
    MMISMS_SetCurMsgLocked(cur_order_id->flag.is_locked);
    index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
    
    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        if (MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type) 
        {
            if(cur_order_id->flag.is_locked)
            {          
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_SMS_CHAT_LOCK_ICON);
            }      
            else
            {
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_NULL);
            }
        }
    }
    else
    {
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
    
        ctrl_info.is_ctrl_id = TRUE;
        ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;
        if (MMISMS_MT_NOT_DOWNLOAD == cur_order_id->flag.mo_mt_type  ||
                MMISMS_MT_NOT_NOTIFY == cur_order_id->flag.mo_mt_type ||
                MMISMS_MT_NEED_NOT_DOWNLOAD == cur_order_id->flag.mo_mt_type
           )
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_MMS_NOTIFY_NEW_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
            }
        }
        else if (MMISMS_MT_HAVE_READ == cur_order_id->flag.mo_mt_type || MMISMS_MO_SEND_SUCC == cur_order_id->flag.mo_mt_type || MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type)
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_READ_MMS_ICON;        
            }
            else
            {
                data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
            }
        }
        else
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_NOT_READ_MMS_ICON;
            }
            else
            { 
                data_info.img_id = IMAGE_SMS_NOT_READ_MMS_ICON;
            }
        }
        
        display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);        
    }
}

/*****************************************************************************/
//     Description : to delete all mared message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinDelAllMarkedMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    //避免反复写彩信索引文件
    LOCAL BOOLEAN is_mms_exist = FALSE;
    MMISMS_OPER_ERR_E del_result = MMISMS_NO_ERR;
    uint16 msg_type = MMISMS_TYPE_MAX;

    del_result = MMISMS_AppDeleteMsgBox(MMISMS_BOX_CHAT, &msg_type, MMISMS_FOLDER_NORMAL);

    if (MMISMS_FINISH_OPER == del_result) //begin deleting sms
    {
        //finish the del
        if (is_mms_exist)
        {
            MMIAPIMMS_SaveListInfo();
            is_mms_exist = FALSE;
        }

        UpdateChatItemDisplayAfterDelSingle(win_data_ptr);

#ifndef MMI_PDA_SUPPORT
        MMISMS_OpenMsgBox(TXT_DELETED, IMAGE_PUBWIN_SUCCESS);
#endif
        MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
        MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
        if (win_data_ptr->is_del_all)
        {
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        win_data_ptr->is_del_all = FALSE;
    }
    else if (msg_type == MMISMS_TYPE_MMS)
    {
        is_mms_exist = TRUE;
    }
    else if (MMISMS_TYPE_SMS == msg_type && MMISMS_DEL_FAIL == del_result)
    {
        MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL, 0);
    }
}


/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)user_date;
    MMIPB_BCD_NUMBER_T cmp_bcd = {0}; 
    BOOLEAN is_blocklist = FALSE;

    if(PNULL == win_data_ptr || PNULL == data_info_ptr)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("mmisms chatwin del call back");
    MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);

    MMISMS_SetDelAllFlag(FALSE);  
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        //finish the del
        if(data_info_ptr->is_mms_del)
        {
            MMIAPIMMS_SaveListInfo();
        }

        cmp_bcd = MMISMS_SmsBCDToPbBCD(g_mmisms_global.chat_number);
        is_blocklist = MMIAPISET_IsSMSBlacklist(&cmp_bcd);
        SCI_TRACE_LOW("[%s:%d]msg_num === %d\n",__FUNCTION__,__LINE__,MMISMS_GetCurChatMsgNumEx());
        if(win_data_ptr->is_del_all ||!win_data_ptr->is_have_item 
            || (MMISMS_GetCurChatMsgNumEx() == 0 && !is_blocklist))
        {
            //UpdateChatItemDisplayAfterDelSingle(win_data_ptr);
            MMK_CloseWin(win_data_ptr->win_id);
        }
       
    }
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMISMS_ClearOperInfo();       
    }
    //for 126318
    MMK_CloseWin(MMISMS_QUERY_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
    if(MMISMS_GetIsNeedUpdateList())
    {
        MMISMS_SetIsNeedUpdateList(FALSE);
        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, 
            MMISMS_MSG_UPDATELIST, 
            PNULL,
            PNULL);    //通知相应的窗口，更新list内容    
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDelQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
	MMICOM_BANNER_INFO_T delete_banner = {0};
	uint16 msg_num = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_FALSE;
    } 
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
			delete_banner.banner_type = MMI_BANNER_TEXT;
			delete_banner.text1_id = STXT_SMS_ERASED;
            //HandleChatOperateDelorDelAll(win_data_ptr);		
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif
        
        MMISMS_SetDelAllFlag(TRUE);
        data_info.box_type = MMISMS_BOX_CHAT; 
        MMISMS_EnterDelWaitWin(&data_info,HandleChatWinDeleteCallBack,(void*)win_data_ptr);
		msg_num = MMISMS_GetCurChatMsgNum();
		if(msg_num > 1)
		{
			MMIAPICOM_OpenBannerWin(win_id,delete_banner);
		}
 		MMK_CloseWin(win_id);
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:

            
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            MMISMS_SetCurChatMsgMarked(pb_num, FALSE);
            
            win_data_ptr->is_del_all = FALSE;

            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of move query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_MOVE_DATA_INFO_T data_info = {0};
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_FALSE;
    }
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:         
            MMISMS_SetMoveAllFlag(TRUE);
            MMISMS_SetCurMsgMarked(TRUE);    
            data_info.oper_status = MMISMS_MOVE_SMS_TO_SECURITY;
            data_info.box_type = MMISMS_BOX_CHAT;
            data_info.pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            //data_info.chat_index = GUILIST_GetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
            MMISMS_EnterMoveWaitWin(&data_info,HandleChatWinMoveFinishCallback,win_data_ptr);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetCurMsgMarked(FALSE);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenMoveQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
            
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
        win_data_ptr->is_del_all = FALSE;
        MMISMS_SetDelIndex(win_data_ptr->del_index);
        order_id = win_data_ptr->chat_item[win_data_ptr->del_index].cur_order_id;
    } 
    if(PNULL == order_id)
    {        
        return;
    }
    MMI_GetLabelTextByLang(TXT_MOVE, &prompt_str1);
    
    if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
    {
        prompt_str2.wstr_len = order_id->display_content.content_len;
        prompt_str2.wstr_ptr = order_id->display_content.content;
    }
    else
    {
        MMI_WSTRNCPY(str,
                     MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                     order_id->display_content.content,
                     MMISMS_QUERY_STRING_MAX_NUM,
                     MMISMS_QUERY_STRING_MAX_NUM);

        MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                       MMISMS_ELLIPSE_STR_LEN,
                       (const uint8 *)ellipse_str,
                       MMISMS_ELLIPSE_STR_LEN,
                       MMISMS_ELLIPSE_STR_LEN);
        prompt_str2.wstr_ptr = str;
        prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
    }
    MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinMoveQueryWinMsg );

}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveFinishCallback(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)user_date;
    if(PNULL == data_info_ptr || PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms chat win move error");
       return ret;
    }
    win_data_ptr->is_del_all = FALSE;

    if (!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
   
    MMISMS_SetMoveAllFlag(FALSE);
   
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }


        MMISMS_WriteFolderType();
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        if(!win_data_ptr->is_have_item)
        {          
            MMK_CloseWin(win_data_ptr->win_id);
        }                
        ret = TRUE;
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }

        MMISMS_OpenMsgBox(STR_NO_SPACE_EXT01, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);        
        MMISMS_ClearOperInfo();   
        ret = FALSE;
    }     
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);     
        MMISMS_ClearOperInfo();     
        ret = FALSE;
    }

    return ret;
}


/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
	win_data_ptr  = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;
    }
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->is_del_all = FALSE;

            if (!MMISMS_GetMoveSingleFlag())
            {
                MMISMS_ClearOperInfo();
            }
           
            MMISMS_SetMoveAllFlag(FALSE);
           
            //全部删除完毕，将未处理链表中的sms插入到相应的链表中
            MMISMS_InsertUnhandledSmsToOrder();
            MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            SCI_TRACE_LOW("mmisms move cancel");            
            MMISMS_SetMoveAllFlag(FALSE);
            MMISMS_WriteFolderType();
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}
#endif

/*****************************************************************************/
//  Description : Init all controls layout on SMS Create New
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitEditingAreaControls(MMI_WIN_ID_T win_id
								)
{
    uint16  										    h_space = 0;
    uint16  										    v_space = MMISMS_SMS_EDIT_VER_MARGIN;
    uint16										    image_w = 0;
    uint16										    image_h = 0;		
    GUI_BG_T 									    bg_info = {0};
    GUI_BG_T                                                    button_bg = {0};
//    GUI_FONT_T                                              edit_font = MMI_DEFAULT_NORMAL_FONT;
//    GUI_FONT_T                                              edit_font = MMI_DEFAULT_BIG_FONT_SIZE;
    GUIFORM_CHILD_WIDTH_T               editor_width = {0}; 
    GUIFORM_CHILD_HEIGHT_T              height_info = {0};
    GUIFORM_CHILD_WIDTH_T               width_info = {0};
	GUI_BG_T chat_bg = {0};

    GUI_BG_T chat_focus_bg = {0};
    GUI_BG_T chat_underline_bg = {0};
	GUI_BORDER_T border = {0};
	GUI_FONT_ALL_T	font_type = {0};
    GUI_PADDING_T							    edit_padding = {0};
    GUI_RECT_T								    bg_display_rect = {0};
    GUI_RECT_T      							    form_rect   = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO    					    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    MMI_HANDLE_T                        	         form_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_area_form_ctr_id = NULL;
    MMI_HANDLE_T                                     edittext_form_ctr_id = NULL;
    MMI_HANDLE_T                                     edittext_ctr_id = NULL;
    //MMI_HANDLE_T                                     sim_area_form_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_container_form_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_body_form_ctr_id = NULL;
    //MMI_HANDLE_T                                     sim_owndraw_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_shortcut_indicator_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_under_background_label_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_input_length_owndraw_ctr_id = NULL;
    MMI_HANDLE_T                                     editing_ime_owndraw_ctr_id = NULL;
    MMITHEME_EDIT_T edit_theme = {0};

    MMITHEME_GetEditTheme(&edit_theme);	

    form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
    editing_area_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_ELEMENT_AREA_FORM_ID);
    editing_container_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_CONTAINER_FORM_ID);
    editing_body_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_BODY_FORM_ID);
    edittext_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_TEXT_FORM_ID);	
    edittext_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);

    editing_shortcut_indicator_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_SHORTCUT_INDICATOR_CTL_ID);

    editing_under_background_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_UNDER_BACKGROUND_LABEL_CTL_ID);
    //editing_input_length_owndraw_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_INPUT_LENGTH_OWNDRAW_CTL_ID);
    //editing_ime_owndraw_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_IME_OWNDRAW_CTL_ID);

    //sim_area_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_SIM_AREA_FORM_ID);
    //sim_owndraw_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMISMS_SMS_EDIT_SIM_OWNDRAW_CTL_ID);

    //editing area docked to the bottom
    GUIFORM_SetChildDock(form_ctr_id, editing_area_form_ctr_id, GUIFORM_CHILD_DOCK_BOTTOM);
   
    GUIEDIT_PermitBorder(edittext_ctr_id,FALSE);
    GUIEDIT_SetStyle(edittext_ctr_id,GUIEDIT_STYLE_MULTI_DYN_UP);
    if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
    {
        CTRLBASEFLEX_SetLine(edittext_ctr_id,GUIEDIT_MMS_SEPERATE_LINE, IMAGE_MESSAGING_CONVERSATION_RECIPIENT);
        GUIFORM_SetAlign(editing_area_form_ctr_id, GUIFORM_CHILD_ALIGN_RIGHT);
        GUIFORM_SetAlign(edittext_form_ctr_id, GUIFORM_CHILD_ALIGN_RIGHT);
    }
    else
    {
        CTRLBASEFLEX_SetLine(edittext_ctr_id,GUIEDIT_MMS_SEPERATE_LINE, IMAGE_MESSAGING_CONVERSATION_USER);
    }

    GUIFORM_SetMargin(form_ctr_id, 0);
    //GUIFORM_SetMargin(sim_area_form_ctr_id, 0);
    GUIFORM_SetSpace(editing_body_form_ctr_id, &h_space, PNULL);
    //GUIFORM_SetSpace(editing_container_form_ctr_id, PNULL, &v_space);

    bg_info.bg_type = GUI_BG_COLOR;  
    bg_info.color = MMI_WHITE_COLOR; 
    GUIFORM_SetBg(form_ctr_id, &bg_info);

#ifndef ARVIN_DEBUG_MESSAGING_UNISOC
    bg_info.color = MMI_YELLOW_COLOR;
#endif
    //GUIFORM_SetBg(sim_area_form_ctr_id, &bg_info);
	
    //GUIFORM_PermitChildBg(editing_body_form_ctr_id, FALSE);//default is TRUE
    //GUIFORM_SetBg(edittext_form_ctr_id, &bg_info);//we set the parent form bg as theme color
    //GUIFORM_SetBg(editing_body_form_ctr_id, &bg_info);
	//GUIOWNDRAW_SetBg(editing_shortcut_indicator_ctr_id, &chat_bg);

#ifndef ARVIN_DEBUG_MESSAGING_UNISOC
    bg_info.color = MMI_RED_COLOR;
#endif
    //GUIEDIT_SetBg(edittext_ctr_id, &bg_info);
#ifndef ARVIN_DEBUG_MESSAGING_UNISOC
    bg_info.color = MMI_GREEN_COLOR;
#endif

    edit_padding.left = MMISMS_SMS_EDIT_VER_MARGIN;
    edit_padding.top = MMISMS_SMS_EDIT_VER_MARGIN;//3
    edit_padding.bottom = MMISMS_SMS_EDIT_VER_MARGIN / 2;//why only (edit_padding.bottom-3) can be shown???
    //GUIFORM_SetPadding(edittext_form_ctr_id, &edit_padding);
    GUIFORM_SetSpace(editing_body_form_ctr_id, PNULL, &v_space);
    v_space += 1; 
    GUIFORM_SetSpace(editing_container_form_ctr_id, PNULL, &v_space);

    GUIEDIT_SetMargin(edittext_ctr_id, 1, 0);//!!!left an issue:editor don't support set the different value to top margin and bottom margin!!! temp solution---form padding + 3
	//GUIFORM_SetStyle(editing_container_form_ctr_id, GUIFORM_STYLE_UNIT); // must not set GUIFORM_STYLE_UNIT hero

    GUIFORM_PermitChildBg(editing_container_form_ctr_id,TRUE);
//GUIEDIT_SetBg(edittext_ctr_id)
	GUIFORM_PermitChildFont(editing_container_form_ctr_id,FALSE);
	GUIFORM_SetChildAlign(editing_container_form_ctr_id, editing_body_form_ctr_id, GUIFORM_CHILD_ALIGN_HMIDDLE);

    chat_bg.bg_type = GUI_BG_COLOR;
    chat_focus_bg.bg_type = GUI_BG_COLOR;
    chat_underline_bg.bg_type = GUI_BG_IMG;
 
	chat_bg.color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(0x30, 0x30, 0x30);
	chat_focus_bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); ;
    if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
    {
        chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_RECIPIENT;
    }
    else
    {
        chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_USER;
    }

	//GUIEDIT_SetBg(edittext_ctr_id,&chat_focus_bg); // do not set GUIFORM_STYLE_UNIT,must set focus bg
	border.type = GUI_BORDER_NONE;
	font_type.color = MMI_BLACK_COLOR;
	font_type.font = edit_theme.font.font;

	GUIFORM_SetChildTheme(editing_container_form_ctr_id, &chat_bg, &border, &font_type);//font should be set as SPEC later...
	GUIFORM_SetActiveChildTheme(editing_container_form_ctr_id, &chat_focus_bg, &border, &font_type);
	GUIFORM_SetFormChildTheme(editing_container_form_ctr_id, &chat_bg, &border);
	GUIFORM_SetFormActiveChildTheme(editing_container_form_ctr_id, &chat_focus_bg, &border);

	//GUIFORM_SetStyle(edittext_form_ctr_id, GUIFORM_STYLE_UNIT);
	GUIFORM_PermitChildBg(editing_body_form_ctr_id, FALSE);//default is TRUE
	GUIFORM_PermitChildBg(editing_container_form_ctr_id,FALSE);

    GUIFORM_SetUseFormBg(editing_container_form_ctr_id, TRUE);
	GUIFORM_SetUseFormBg(editing_body_form_ctr_id, TRUE);
	GUIFORM_SetUseFormBg(edittext_form_ctr_id, TRUE);

	GUIFORM_PermitChildFont(editing_container_form_ctr_id,FALSE);
	//GUIFORM_PermitChildBg(edittext_form_ctr_id, FALSE);
	GUILABEL_SetBg(editing_under_background_label_ctr_id, &chat_underline_bg);//underline_bg_img
	//GUIFORM_SetBg(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, &chat_bg);//full background of client space
	//GUIFORM_SetBg(editing_area_form_ctr_id, &chat_bg);//used to set the initial bg of form --- unfocused

	width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
	width_info.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	GUIFORM_SetChildWidth(editing_area_form_ctr_id, editing_container_form_ctr_id, &width_info);
	//width_info.add_data = MMISMS_SMS_FIXED_SIM_RECT_WIDTH;
	//GUIFORM_SetChildWidth(editing_area_form_ctr_id, sim_area_form_ctr_id, &width_info);

	//under_bg
	width_info.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	GUIFORM_SetChildWidth(editing_container_form_ctr_id, editing_under_background_label_ctr_id, &width_info);
	height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
	height_info.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
     GUIFORM_SetChildHeight(editing_container_form_ctr_id, editing_under_background_label_ctr_id, &height_info);
	GUIFORM_SetChildAlign(editing_container_form_ctr_id, editing_under_background_label_ctr_id, GUIFORM_CHILD_ALIGN_HMIDDLE);

	//EDIT_SHORTCUT_INDICATOR
//	GUIRES_GetImgWidthHeight(&image_w, &image_h, IMAGE_MESSAGING_SHORTCUT_INDICATOR, win_id);
//	width_info.add_data = image_w;
//     GUIFORM_SetChildWidth(editing_body_form_ctr_id, editing_shortcut_indicator_ctr_id, &width_info);


     //text editor
     width_info.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH ;
     GUIFORM_SetChildWidth(editing_body_form_ctr_id, edittext_form_ctr_id, &width_info);
     GUIFORM_IsAutoDir(edittext_form_ctr_id, TRUE);
     GUIEDIT_SetFont(edittext_ctr_id, edit_theme.font.font);
     GUIEDIT_SetDefaultStringById(edittext_ctr_id, STR_MSG_LIST_HINT_INPUT_EXT02);
     bg_info.color = MMI_WHITE_COLOR;
     GUIEDIT_SetBg(edittext_ctr_id, &bg_info);
     CTRLBASEEDIT_PermitBorder(edittext_ctr_id, TRUE);

 	GUIFORM_IsSlide(MMIMMS_SMS_EDIT_FORM_ID, FALSE);//no scrollbar???
 	//CTRLFORM_SetCircularHandleUpDown(MMIMMS_SMS_EDIT_FORM_ID, TRUE);
}


/*****************************************************************************/
//  Description : display sim card icon
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
LOCAL void SimIconOwnDrawFun(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    uint32 ctrl_id=0;
    GUI_RECT_T rect={0};
    GUI_RECT_T sim_rect={0};
    GUI_RECT_T icon_rect={0};
    uint16 image_w = 0;
    uint16 image_h = 0;
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    GUI_COLOR_T       bg_color = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if(PNULL == owner_draw_ptr)   
    {
        return;
    }

    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);
    rect   = MMITHEME_GetClientRectEx(win_id);

    //According to MS Spec, we should display a fixed area for sim icon in the fixed rect.
    sim_rect.left = rect.right - 34;
    sim_rect.top = rect.bottom - 60;
    sim_rect.right = rect.right;
    sim_rect.bottom = rect.bottom - 24 - 1;
#if 0
    if (s_test_count % 2 != 0) {
         bg_color = MMI_BLACK_COLOR; 
    }
	else
#endif//0
	{
		bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); 
    }

    GUI_FillRect(&lcd_dev_info, sim_rect, bg_color);

    GUIRES_GetImgWidthHeight(&image_w, &image_h, IMAGE_CUSTOM_SIM1, win_id);
    icon_rect.left = (sim_rect.left + sim_rect.right -image_w) /2 + 1;
    icon_rect.top = (sim_rect.top + sim_rect.bottom -image_h) /2;
    icon_rect.right = (sim_rect.left + sim_rect.right + image_w) /2 + 1;
    icon_rect.bottom = (sim_rect.top + sim_rect.bottom + image_h) /2;
    GUIRES_DisplayImg(
            PNULL,
            &icon_rect,
            PNULL,
            owner_draw_ptr->win_handle,
            IMAGE_CUSTOM_SIM1,
            (const GUI_LCD_DEV_INFO*)&lcd_dev_info);
}
#if 0
/*****************************************************************************/
//  Description : display the shortcut indicator for editor
//  Global resource dependence : none
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
LOCAL void EditShortcutIndicatorOwnDrawFun(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    MMI_HANDLE_T		win_handle = 0;
    MMI_WIN_ID_T		win_id = 0;
    GUI_RECT_T                          client_rect = {0};
    GUI_RECT_T                          icon_rect = {0};
    GUI_RECT_T                          bg_display_rect = {0};
    uint16                                     image_h = 0;
    uint16                                     image_w = 0;
    GUI_LCD_DEV_INFO           lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if (PNULL == owner_draw_ptr)
    {
        return;
    }

    win_handle = MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id = MMK_GetWinId(win_handle);
    client_rect = MMITHEME_GetClientRectEx(win_id);
    GUIRES_GetImgWidthHeight(&image_w, &image_h, IMAGE_MESSAGING_SHORTCUT_INDICATOR, win_id);

    //According to MS Spec, we should display a shortcut indicatior icon in the fixed rect.
    icon_rect.left = owner_draw_ptr->display_rect.right - image_w;
    icon_rect.top = owner_draw_ptr->display_rect.bottom - image_h;
    icon_rect.right = owner_draw_ptr->display_rect.right;
    //icon_rect.bottom = client_rect.bottom - 28 -7;
    icon_rect.bottom = owner_draw_ptr->display_rect.bottom;

    GUIRES_DisplayImg(
		PNULL,
		&icon_rect,
		PNULL,
		owner_draw_ptr->win_handle,
		IMAGE_MESSAGING_SHORTCUT_INDICATOR,
		(const GUI_LCD_DEV_INFO*)&lcd_dev_info);

}
#endif
/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinOpenDelteQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};

    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2 + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint32 delete_sms_num = 0;

    if (MMISMS_MENU_DELBOX_ALL_ITEM_ID == win_data_ptr->menu_id)//删除全部
    {        
        delete_sms_num = MMISMS_GetSmsBoxDeleteAvailNum(MMISMS_GetCurBoxType());

        MMI_GetLabelTextByLang(TXT_DELETE, &prompt_str1);

        disp_num_len = sprintf((char *)delete_msg_num, "%ld ", delete_sms_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (delete_sms_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEMS, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEM, &tmp_prompt);
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;

        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOC_APP((disp_str_len + 1) * sizeof(wchar));
        SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len + 1)*sizeof(wchar)));

        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;

        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr,
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
        );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        if (0 < delete_sms_num)
        {
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinDelQueryWinMsg );
        }
        else
        {
            MMIPUB_OpenAlertWinByTextPtr(PNULL, PNULL, &prompt_str2, IMAGE_PUBWIN_WARNING, &query_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(STR_QUEST_DELETE_MARKED, &prompt_str1);
        //order_id = win_data_ptr->chat_del_order_id;
        order_id = MMISMS_GetCurOperationOrderId();
        if (PNULL == order_id)
        {
            SCI_TRACE_LOW("mmisms order_id == PNULL when delete");
            return;
        }
        if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
        {
            prompt_str2.wstr_len = order_id->display_content.content_len;
            prompt_str2.wstr_ptr = order_id->display_content.content;
        }
        else
        {
            MMI_WSTRNCPY(str,
                         MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                         order_id->display_content.content,
                         MMISMS_QUERY_STRING_MAX_NUM,
                         MMISMS_QUERY_STRING_MAX_NUM);

            MMI_STRNTOWSTR( str + MMISMS_QUERY_STRING_MAX_NUM,
                            MMISMS_ELLIPSE_STR_LEN,
                            (const uint8 *)ellipse_str,
                            MMISMS_ELLIPSE_STR_LEN,
                            MMISMS_ELLIPSE_STR_LEN);
            prompt_str2.wstr_ptr = str;
            prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
        }

      //  MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinDelQueryWinMsg );
		MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&prompt_str1,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleChatWinDelQueryWinMsg);
    }
}

/*****************************************************************************/
//  Description :set chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 index,
    BOOLEAN is_active,
    MMISMS_ORDER_ID_T order_id
)
{
    if(PNULL == win_data_ptr)
    {
        //SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
        return;
    }
    win_data_ptr->chat_item[index].is_valid = is_active;
    win_data_ptr->chat_item[index].cur_order_id = order_id;
    win_data_ptr->chat_item[index].parent_ctrl_id = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
    //SCI_TRACE_LOW:"SetChatWinIndex: index is %d,is_active is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30054_112_2_18_2_55_22_861, (uint8*)"dd", index, is_active);
}

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_INDEX_E GetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 *index_ptr,
    MMISMS_ORDER_ID_T order_id
)
{
    uint32 max = MMISMS_CHAT_MAX_ITEM;
    uint32 i = 0;
    MMISMS_CHAT_INDEX_E result = MMISMS_CHAT_INDEX_MAX;
    MMISMS_ORDER_ID_T cur_longsms_order_id = PNULL;

    //long sms
    if (order_id->flag.is_concatenate_sms)
    {
        for (i = 0; i < max; i++)
        {
            if ((win_data_ptr->chat_item[i].is_valid)
                    && (PNULL != win_data_ptr->chat_item[i].cur_order_id)
               )
            {
                cur_longsms_order_id = win_data_ptr->chat_item[i].cur_order_id;
                {
                    if (MMISMS_HaveSamePart(cur_longsms_order_id, order_id))
                    {
                        *index_ptr = i;
                        return MMISMS_CHAT_INDEX_SAME_LONG_SMS;
                    }
                }
            }
        }
    }

    for (i = 0; i < max; i++)
    {
        if (!win_data_ptr->chat_item[i].is_valid)
        {
            *index_ptr = i;
            result = MMISMS_CHAT_INDEX_NEW;
            break;
        }
    }
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    //SCI_TRACE_LOW:"GetChatWinIndex: index is %d,result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30107_112_2_18_2_55_22_862, (uint8*)"dd", i, result);
    return result;
}

/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTagUpdateSoftkey(MMI_CTRL_ID_T ctrl_id)
{
    GUITEXT_TAG_INFO_T tag_info = {0};
    MMIPUB_MENU_PARAM_T menuparam = {0}; 
    uint16 min = 0;
    uint16 max = 0;      
    static BOOLEAN is_need_update_softkey = FALSE;
    if(TRUE == GUITEXT_GetHighlightInfo(ctrl_id,&min,&max))
    {
        if(MMIAPIIDLE_IsPhoneLocked())
        {
            return;
        }
        GUITEXT_GetCurTag(ctrl_id, &tag_info);               
        switch (tag_info.type)
        {
            case GUITEXT_TAG_PHONENUM:
                {          
                    GUIWIN_SeSoftkeytButtonTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON,FALSE);                        
                    GUIWIN_SeSoftkeytButtonTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_SOFTKEY_SAVE_MK, MIDDLE_BUTTON, FALSE);        
                    GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, MIDDLE_BUTTON,  FALSE);
                    GUIWIN_UpdateSoftkey(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                }
                break;

            case GUITEXT_TAG_URL:
                {
                    GUIWIN_SeSoftkeytButtonTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, TXT_NULL, LEFT_BUTTON,FALSE);                      
                    GUIWIN_SeSoftkeytButtonTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_SOFTKEY_GO_TO_MK, MIDDLE_BUTTON, FALSE);       
                    GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, MIDDLE_BUTTON,  FALSE);
                    GUIWIN_UpdateSoftkey(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                }
                break;

            default:
                break;
        }
        is_need_update_softkey = TRUE;
    }  
    else
    {
        if(is_need_update_softkey)
        {
            MMISMS_UpdateChatWinSoftkey(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            is_need_update_softkey = FALSE;
        }        
    }
}
#if 0
/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTag(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id)
{
    GUITEXT_TAG_INFO_T tag_info = {0};
	MMIPUB_MENU_PARAM_T menuparam = {0};
    GUITEXT_GetCurTag(ctrl_id, &tag_info);                
    switch (tag_info.type)
    {
        case GUITEXT_TAG_PHONENUM:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractNumberOptOk;    
                //menuparam.func.OnMenuInit = HandleChatWinExtractNumberOptInit;     
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;

        case GUITEXT_TAG_URL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_URL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_URL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_URL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractUrlOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#ifdef MMIEMAIL_SUPPORT
        case GUITEXT_TAG_EMAIL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_EMAIL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_EMAIL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_EMAIL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractEmailOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#endif
        default:
            break;
    }
}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTag(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id)
{
    GUITEXT_TAG_INFO_T tag_info = {0};
	MMIPUB_MENU_PARAM_T menuparam = {0};
    GUITEXT_GetCurTag(ctrl_id, &tag_info);                
    switch (tag_info.type)
    {
        case GUITEXT_TAG_PHONENUM:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractNumberOptOk;    
                //menuparam.func.OnMenuInit = HandleChatWinExtractNumberOptInit;     
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;

        case GUITEXT_TAG_URL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_URL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_URL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_URL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractUrlOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#ifdef MMIEMAIL_SUPPORT
        case GUITEXT_TAG_EMAIL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_EMAIL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_EMAIL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_EMAIL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractEmailOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#endif
        default:
            break;
    }
}
#endif
/*****************************************************************************/
//     Description : open the SMS read win by number
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSChatReadWinByNumber(MMIPB_BCD_NUMBER_T *number_ptr)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T prompt_str = {0};

    if(number_ptr != PNULL)
    {
        cur_order_id = MMISMS_GetChatOrderByNumber(*number_ptr);
    }

    if(PNULL == cur_order_id)
    {
        MMIPUB_OpenConfirmationDialogByTextId(PNULL,
                                              STR_NOTIF_OPEN_FAIL,
                                              TXT_NULL,
                                              IMAGE_PUBWIN_SUCCESS,
                                              PNULL,
                                              PNULL,
                                              MMIPUB_SOFTKEY_DIALOG_CLOSE,
                                              PNULL);
        return;
    }
	if( SCI_FALSE == MMIAPISMS_OpenConfirmation((void *)number_ptr,MMISMS_ENTER_BY_IDLE))
	{
		return ;
	}
	
    SCI_TRACE_LOW("mmisms MMISMS_EnterSMSChatReadWinByNumber: open chat read win cur_order_id = %d",cur_order_id);
    //save chat number
    MMISMS_SetSMSUpdateInfo(cur_order_id);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);    
    MMISMS_EnterSMSChatReadWin(cur_order_id);
    MMI_GetLabelTextByLang( STR_NOTE_WAITING, &prompt_str );
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_READSMS_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);             
    
}

/*****************************************************************************/
//     Description : open the SMS read win
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSChatReadWin(MMISMS_ORDER_ID_T cur_order_id)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_BCD_NUMBER_T mms_bcd_num = {0};

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_CHATREAD_WINDATA_T));

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    
    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        SCI_MEMSET(&win_data_ptr->chat_number, 0x0, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&win_data_ptr->chat_number, &cur_order_id->number.bcd_number, sizeof(MMISMS_BCD_NUMBER_T));
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,
           win_data_ptr->chat_number.number,
           win_data_ptr->chat_number.number_len << 1,
           (char*)win_data_ptr->sender);
    }
    else if(MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
    {
        MMISMS_MMSNumberToBcd((char*)cur_order_id->number.sender, mms_bcd_num.number, &mms_bcd_num.number_len);
        SCI_MEMSET(&win_data_ptr->chat_number, 0x0, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&win_data_ptr->chat_number, &mms_bcd_num, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMSET(win_data_ptr->sender, 0x0, MMISMS_NUMBER_MAX_LEN + 1);
        SCI_MEMCPY(win_data_ptr->sender, cur_order_id->number.sender, (sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));
    }
    SCI_TRACE_LOW("mmisms MMISMS_EnterSMSChatReadWin: msg_type=%d", cur_order_id->flag.msg_type);
    win_data_ptr->dual_sys  =MN_DUAL_SYS_MAX;
    MMISMS_SetGlobalChatNumber(win_data_ptr->chat_number,win_data_ptr->sender);
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
    {
        MMK_CloseWin( MMISMS_SHOW_CHAT_SMS_WIN_ID );
    }
    win_handle  = MMK_CreateWin((uint32*)MMISMS_SHOW_CHAT_SMS_WIN_TAB, PNULL);
	//MMISMS_SetCurOperationOrderId(MMISMS_BOX_CHAT,cur_order_id);
	MMISMS_SetGlobalOperationOrderId(cur_order_id);
	
    if(win_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);

}


/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle)
{
    MMI_STRING_T contact_str = {0};
    BOOLEAN is_right = TRUE;

    contact_str.wstr_ptr = SCI_ALLOC_APP((MMISMS_MAX_MESSAGE_LEN + 1) * sizeof(wchar));
    SCI_MEMSET(contact_str.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN + 1)*sizeof(wchar)));

    // get the detail information from the phone_book
    MMISMS_GetContactInfo((uint32)handle,
                          &contact_str,
                          MMISMS_MAX_MESSAGE_LEN
                         );

    is_right = GUIEDIT_InsertString(
                   MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,
                   contact_str.wstr_ptr,
                   contact_str.wstr_len
               );
    if (!is_right)
    {
        // prompt the error information
        MMIPUB_OpenAlertWarningWin(STR_MSG_MAX_LENGTH);
    }

    if (PNULL != contact_str.wstr_ptr)
    {
        SCI_FREE(contact_str.wstr_ptr);
        contact_str.wstr_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : opertate after del single chat item
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void UpdateChatItemDisplayAfterDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMI_CTRL_ID_T del_child_ctrl = 0;
    int32 i = 0;

    MMI_CTRL_ID_T ctrl_id = 0;

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return ;
    }
    win_data_ptr->del_index = MMISMS_GetDelIndex();
    win_data_ptr->is_have_item = FALSE;
    if (win_data_ptr->del_index < MMISMS_CHAT_MAX_ITEM)
    {
        del_child_ctrl = win_data_ptr->chat_item[win_data_ptr->del_index].parent_ctrl_id;
    }
    else
    {
		win_data_ptr->is_have_item = TRUE; // make delete long sms not exist chat win
        return;
    }
    GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, del_child_ctrl);
    SCI_MEMSET(&win_data_ptr->chat_item[win_data_ptr->del_index], 0x0, sizeof(MMISMS_CHAT_ITEM_T));

    for (i = 0; i < MMISMS_CHAT_MAX_ITEM; i++)
    {
        ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, (i + 1));
        if (0 == ctrl_id || (ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID || ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM))
        {
            ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, i);
            if (ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID && ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM)
            {
                win_data_ptr->is_have_item = TRUE;
                break;
            }

        }
    }

      
    if (win_data_ptr->is_have_item)    
    {
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, ctrl_id);
        }
#ifdef TOUCH_PANEL_SUPPORT         
        if (!MMK_IsFocusWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {       
            GUIFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
        }
#endif        
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30331_112_2_18_2_55_23_865, (uint8*)"d", win_data_ptr->del_index);
    win_data_ptr->del_index = MMISMS_CHAT_MAX_ITEM;
    MMISMS_SetDelIndex(win_data_ptr->del_index);
    MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
}

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetChatItemDateWidth(void)   // 12小时制加大长度
{

     MMISET_TIME_DISPLAY_TYPE_E time_disp_type = MMISET_TIME_24HOURS;
     time_disp_type = MMIAPISET_GetTimeDisplayType();
     if(MMISET_TIME_24HOURS == time_disp_type)
     {
         return MMISMS_CHATFORM_SIM_DATE_WIDTH;
     }
     else
     {
         return MMISMS_CHATFORM_SIM_DATE_WIDTH + 4;
     }
}

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
)
{
    GUIFORM_DYNA_CHILD_T parent_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T chat_bg_form_child_ctrl = {0};
    GUIFONT_REGION_STYLE_T  text_region = {0};
    GUI_BG_T chat_underline_bg = {0};
    GUI_PADDING_T	form_padding = {0};
    GUI_FONT_ALL_T	font_type = {0};
    MMI_STRING_T content_str = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
    GUI_FONT_T font_size = MMI_DEFAULT_BIG_FONT_SIZE;
    GUI_COLOR_T font_color = MMI_BLACK_COLOR;
    uint32 index = 0;
    GUI_BG_T chat_bg = {0};
    GUI_BG_T chat_focus_bg = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUILABEL_INIT_DATA_T    label_init = {0};
    GUITEXT_INIT_DATA_T date_text_init = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T replace_child_ctrl = 0;
    MMI_STRING_T text_content = {0};
    BOOLEAN is_need_replace = FALSE;
    MMI_STRING_T long_content_str = {0};
    int32 i = 0;
    uint16 long_content_alloc_len = 0;
    GUI_BORDER_T border = {0};

    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_DYNA_CHILD_T date_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T date_text_form_child_ctrl = {0};
    uint16 ver_space_ptr = 0;
    uint16 hor_space_ptr = 0;
    wchar sms_time[20] = {0};
    GUIFORM_DYNA_CHILD_T owndraw_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T fail_label_child_ctrl = {0};

    MMISMS_CHAT_INDEX_E chat_index = MMISMS_CHAT_INDEX_NEW;

    MMI_STRING_T sms_time_str = {0};
    uint8 time_len = 0;
    uint16 disp_width = 0;
    uint16 child_form_width = 0;
    uint16 width = 0;
    BOOLEAN is_send_msg = FALSE;

    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        //SCI_PASSERT(0,("mmisms win_data_ptr error"));  
        return;
    }
    chat_index = GetChatWinIndex(win_data_ptr,&index, order_id);
    //index is full
    if (MMISMS_CHAT_INDEX_MAX == chat_index)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        //SCI_TRACE_LOW:"SetChatItemParam:not find index!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29530_112_2_18_2_55_21_860, (uint8*)"");
#ifdef MMIMMS_SMS_IN_1_SUPPORT   //短彩合一第0 个控件为MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 1);
#else
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 0);
#endif
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
        if(replace_child_ctrl == 0)
        {
        	SCI_TraceLow("Error:  SetChatItemParam replace_child_ctrl should not be 0,set it to  MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID");
		replace_child_ctrl = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
        }
        index = replace_child_ctrl - MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
    }
    else if (MMISMS_CHAT_INDEX_SAME_LONG_SMS == chat_index)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        GUITEXT_GetString(MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index, &text_content);

        long_content_alloc_len = (content_str.wstr_len + text_content.wstr_len);
        long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
        long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
        long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));
        if (PNULL == long_content_str.wstr_ptr)
        {
            return;
        }
        SCI_MEMCPY(long_content_str.wstr_ptr, text_content.wstr_ptr, text_content.wstr_len * sizeof(wchar));

        ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index);
        for (i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
        {
            if (win_data_ptr->chat_item[index].split_sms_enable[i])
            {
                break;
            }
        }
        if (
            ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM != 0) && (order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM - 1) > i)
            || ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM == 0) && ((MMISMS_SPLIT_MAX_NUM - 1) > i)))
        {
            order_id = win_data_ptr->chat_item[index].cur_order_id;
        }

        is_need_replace = TRUE;
    }
    else
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        //first reached of a long sms
        if (order_id->flag.is_concatenate_sms)
        {
            if ( !order_id->flag.is_receive_all)
            {
                if (!MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)) //聊天界面来短信，而非读短信
                {
                    long_content_alloc_len = content_str.wstr_len;
                    long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
                    long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
                    long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));

                    if (PNULL == long_content_str.wstr_ptr)
                    {
                        SCI_FREE( content_str.wstr_ptr );
                        return;
                    }

                    ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
                    is_need_replace = TRUE;
                }
                else
                {
                    order_id = MMISMS_GetOperationOrderId(0);
                    ChatWinCalcChatItemSplitLenth(win_data_ptr,index);
                }
            }
            else
            {
                order_id = MMISMS_GetOperationOrderId(0);
            }
        }
    }

    //Extract draft message and fulfill the content into the text editor then return
    if (MMISMS_MO_DRAFT == win_data_ptr->read_result.mo_mt_type)
    {         
        if(MMISMS_GetIsDraftDeleting())
        {    
            MMISMS_ORDER_ID_T delete_order_id = PNULL;
            delete_order_id = MMISMS_GetCurOperationOrderId();
            if(delete_order_id && (MMISMS_CompareSmsState(delete_order_id,MMISMS_MO_TYPE,order_id) || (delete_order_id == order_id)))
            {
                SCI_TRACE_LOW("mmisms draft in chat is deleting, do not disp");
            }
        }
        else
        {
            is_draft_existed = TRUE;
            if(content_str.wstr_ptr)
            {
                GUIEDIT_SetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, content_str.wstr_ptr, content_str.wstr_len);
                if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id))      
                {
                    MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
                }            
            }
            win_data_ptr->chat_draft_order_id = order_id;
            MMISMS_SetDraftOperationOrderId(order_id);
        }
    }
    else
    {
    SetChatWinIndex(win_data_ptr,index, TRUE, order_id);   // do not handle draft, make index is correct

    //create parent form
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type)
    {
        form_init.layout_type = GUIFORM_LAYOUT_SBS;
    }
    else
    {
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    }
    //form_init.both_rect = both_rect;

    parent_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    parent_form_child_ctrl.is_bg = FALSE;
    parent_form_child_ctrl.is_get_active = TRUE;
    parent_form_child_ctrl.init_data_ptr = &form_init;
    parent_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;

    GUIFORM_CreatDynaChildCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,  &parent_form_child_ctrl);
    GUIFORM_PermitChildBg(parent_form_child_ctrl.child_handle,FALSE);//any problem?
	GUIFORM_SetChildDisplay(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_DISP_FROM_HEAD);

    //init bg customization and label.init.data
    chat_bg.bg_type = GUI_BG_COLOR;
    chat_focus_bg.bg_type = GUI_BG_COLOR;
    chat_underline_bg.bg_type = GUI_BG_IMG;
    if ((MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type) ||
			(MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type) ||
			(MMISMS_MO_DRAFT == win_data_ptr->read_result.mo_mt_type))
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
        	chat_bg.color = MMI_WHITE_COLOR;
            chat_focus_bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); ;
            if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
            {
                chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_RECIPIENT;
                label_init.align = GUILABEL_ALIGN_FORCE_LEFT;
            }
            else
            {
                chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_USER;
                label_init.align = GUILABEL_ALIGN_RIGHT;
            }
        }
#if 0
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#endif//0
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
        	chat_bg.color = MMI_WHITE_COLOR;
            chat_focus_bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); ;
            if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
            {
                chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_USER;
                label_init.align = GUILABEL_ALIGN_RIGHT;
            }
            else
            {
                chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_RECIPIENT;
                label_init.align = GUILABEL_ALIGN_FORCE_LEFT;
            }
        }
#if 0
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#endif//0
    }

    date_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    date_form_child_ctrl.is_bg = FALSE;
    date_form_child_ctrl.is_get_active = FALSE;
    date_form_child_ctrl.init_data_ptr = &label_init;
    date_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index;

    //set text param
    text_form_child_ctrl.guid = SPRD_GUI_TEXTBOX_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = TRUE;
    text_form_child_ctrl.init_data_ptr = &date_text_init;
    text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;

    chat_bg_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    chat_bg_form_child_ctrl.is_bg = FALSE;
    chat_bg_form_child_ctrl.is_get_active = FALSE;
    chat_bg_form_child_ctrl.init_data_ptr = &label_init;
    chat_bg_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_UNDERBG_ITEM_FORM_CTRL_ID + index;

    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = 20;
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT
    font_size = MMISMS_GetWinFont();
#endif
#endif
    if (MMITHEME_IsMainScreenLandscape())
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, PNULL, &disp_width);
    }
    else
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &disp_width, PNULL);
    }
    if (!is_need_replace)
    {
        width = GUI_GetStringWidth(font_size, content_str.wstr_ptr, content_str.wstr_len); //没有char space的情况
        width += MMITHEME_TEXT_CHAR_SPACE * content_str.wstr_len;
    }
    else
    {
        width = GUI_GetStringWidth(font_size, long_content_str.wstr_ptr, long_content_str.wstr_len);
        width += MMITHEME_TEXT_CHAR_SPACE * long_content_str.wstr_len;
    }
    
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
	//set anim param
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type) //发送失败才显示图标
    {
#ifdef MMI_TIMERMSG_SUPPORT
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
#endif
        GUIFORM_INIT_DATA_T owndraw_init_data = {0};
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIFORM_INIT_DATA_T date_text_form_init = {0};
        GUILABEL_INIT_DATA_T fail_label_init_data = {0};
        GUI_BG_T owndraw_bg = {0};

        uint16 width = 0;
        uint16 height = 0;
            
        owndraw_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        owndraw_form_child_ctrl.is_bg = FALSE;
        owndraw_form_child_ctrl.is_get_active = FALSE;
        owndraw_form_child_ctrl.init_data_ptr = &owndraw_init_data;
        owndraw_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;

        fail_label_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        fail_label_child_ctrl.is_bg = FALSE;
        fail_label_child_ctrl.is_get_active = FALSE;
        fail_label_child_ctrl.init_data_ptr = &fail_label_init_data;
        fail_label_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_CTRL_ID + index;

        
#if 0//use the owndraw func of anim ctrl to get and display the right icon
#ifdef MMI_TIMERMSG_SUPPORT
        event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);
        if (event_fast_info_ptr != PNULL)
        {
            data_info.img_id = IMAGE_SMS_CHATBOX_MO_TIMERMSG_ICON;
        }
        else
#endif
        {
            if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                    && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
            {
                is_send_msg = TRUE;
            }
            if (is_send_msg)
            {
                data_info.img_id = IMAGE_SMS_CHATBOX_MO_SENDING_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_SMS_CHATBOX_MO_FAIL_ICON;
            }
        }
#endif
        date_text_form_init.layout_type = GUIFORM_LAYOUT_ORDER;
        date_text_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_text_form_child_ctrl.is_bg = FALSE;
        date_text_form_child_ctrl.is_get_active = TRUE;
        date_text_form_child_ctrl.init_data_ptr = &date_text_form_init;
        date_text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID + index;


        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &owndraw_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, owndraw_form_child_ctrl.child_handle,  &fail_label_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
		GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &chat_bg_form_child_ctrl);
		GUIFORM_SetStyle(parent_form_child_ctrl.child_handle, GUIFORM_STYLE_UNIT);
        GUIFORM_PermitChildBg(date_text_form_child_ctrl.child_handle,FALSE);
		GUIFORM_SetChildAlign(date_text_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
		//GUIOWNDRAW_SetBg(owndraw_form_child_ctrl.child_handle, &chat_bg);

        GUIFORM_SetChildDock(owndraw_form_child_ctrl.child_handle, fail_label_child_ctrl.child_handle, GUIFORM_CHILD_DOCK_BOTTOM);
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MESSAGING_ERROR_BLUE_MINUS_2, win_id);
        child_height.add_data = height;
        GUIFORM_SetChildHeight(owndraw_form_child_ctrl.child_handle, fail_label_child_ctrl.child_handle, &child_height);
        GUILABEL_SetIcon(fail_label_child_ctrl.child_handle,IMAGE_MESSAGING_ERROR_BLUE_MINUS_2);


        owndraw_bg.color = MMI_WHITE_COLOR;
        owndraw_bg.bg_type = GUI_BG_COLOR;
        GUIFORM_SetBg(owndraw_form_child_ctrl.child_handle,&owndraw_bg);

        {
            child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            child_form_width = MAX(width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH, MMISMS_SENDFAIL_DATE_TEXT_WIDTH);

            child_width.add_data = MMISMS_CHATFORM_ANIM_WIDTH;
            GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, owndraw_form_child_ctrl.child_handle, &child_width);
			child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
            GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, date_text_form_child_ctrl.child_handle, &child_width);
            child_form_width = width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH;

            GUIFORM_SetChildWidth(date_text_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);
            GUIFORM_SetChildWidth(date_text_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_width);
        }

        GUIFORM_PermitChildFont(date_text_form_child_ctrl.child_handle, FALSE);
#if defined (MAINLCD_SIZE_128X160)
		child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;// + 2;
#else
		child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
#endif
        GUIFORM_SetChildHeight(date_text_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_height);
    }
    else
    {
#if 0
        sim_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        sim_form_child_ctrl.is_bg = FALSE;
        sim_form_child_ctrl.is_get_active = FALSE;
        sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID + index;
        sim_form_child_ctrl.init_data_ptr = &label_init;

        form_init.layout_type = GUIFORM_LAYOUT_SBS;
        date_sim_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_sim_form_child_ctrl.is_bg = FALSE;
        date_sim_form_child_ctrl.is_get_active = FALSE;
        date_sim_form_child_ctrl.init_data_ptr = &form_init;
        date_sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID + index;
#endif
		GUIFORM_SetStyle(parent_form_child_ctrl.child_handle,GUIFORM_STYLE_UNIT);
		GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &text_form_child_ctrl);
		GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &chat_bg_form_child_ctrl);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_form_width = width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH ;
        if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
        {
            child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
        }
        child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_width);

	    child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
        GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_height);

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMISMS_CHATFORM_SIM_LABEL_WIDTH;
        
        if (MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type
            || MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type
            || MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type
            )
        {
            child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            child_width.add_data = GetChatItemDateWidth();
        }

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_form_width = MAX(width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH, MMISMS_SENDFAIL_DATE_TEXT_WIDTH);

        if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
        {
            child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
        }

        child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;

        GUIFORM_SetChildWidth(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, &child_width);
	    GUIFORM_SetPadding(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, &form_padding);
	    GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_PermitChildFont(parent_form_child_ctrl.child_handle, FALSE);
        if ((MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type))
        {
            if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
            {
                GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
            }
            else
            {
                GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
            }
        }
		else
		{
            if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
            {
                GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
            }
            else
            {
                GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
                GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
            }
		}
    }

    time_len = MMISMS_GetDateTimeStringByOrder(order_id,TRUE,FALSE,TRUE,sms_time);
    
    sms_time_str.wstr_len = (uint16)time_len ;
    sms_time_str.wstr_ptr = sms_time ;

//     GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
//     GUILABEL_SetFont(chat_bg_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type)
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        //MMI_STRING_T fail_str = {0};
#if 0//There's no need display tip string for sent fail message
#ifdef MMI_TIMERMSG_SUPPORT
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
        event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);
        if (PNULL == event_fast_info_ptr)
#endif
        {
            if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                    && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
            {
                MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
            }
        }

        wstr.wstr_len = sms_time_str.wstr_len + fail_str.wstr_len + 1;

        str = SCI_ALLOCAZ((wstr.wstr_len + 1) * sizeof(wchar));

        if (PNULL == str)
        {
            SCI_FREE( content_str.wstr_ptr );
            SCI_FREE( long_content_str.wstr_ptr);
            return;
        }

        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, wstr.wstr_len , sms_time_str.wstr_ptr, sms_time_str.wstr_len, sms_time_str.wstr_len);
#ifdef MMI_TIMERMSG_SUPPORT
        if (PNULL == event_fast_info_ptr)
#endif
        {
            MMIAPICOM_Wstrcat(str, L" ");
            MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        }

        wstr.wstr_len = MMIAPICOM_Wstrlen(str);
#endif 
        //GUILABEL_SetText(date_form_child_ctrl.child_handle, &wstr, FALSE);
        GUILABEL_SetText(chat_bg_form_child_ctrl.child_handle, &sms_time_str, FALSE);
        SCI_FREE(str);
#if 0
        if(order_id->flag.is_locked)
        {          
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_SMS_CHAT_LOCK_ICON);
        } 
        else
        {
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_NULL);
        }
#endif//0  
        {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
        }            
    }
    else
    {
        if(order_id->flag.is_locked)
        {         
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_SMS_CHAT_LOCK_ICON);
        }
        else
        {
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_NULL);
        }

        GUILABEL_SetText(date_form_child_ctrl.child_handle, &sms_time_str, FALSE);
        GUILABEL_SetText(chat_bg_form_child_ctrl.child_handle, &sms_time_str, FALSE);
#if 0
		text_region.length = sms_time_str.wstr_len;
		text_region.start = 0;
		text_region.state = GUISTR_STATE_UNDERLINE;
		GUILABEL_SetTextRegion(chat_bg_form_child_ctrl.child_handle, &text_region, 1);
#endif
        {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
        }
    }

    //set margin
    GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                        MMISMS_CHAT_TEXT_MARGIN_LEFT,
                        MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                        MMISMS_CHAT_TEXT_MARGIN_TOP,
                        MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
	GUITEXT_SetLineSpace(text_form_child_ctrl.child_handle, MMISMS_CHAT_TEXT_LINE_SPACE);
    
    //no border
    border.type = GUI_BORDER_NONE;
	//set focused and unfocused form different bg color
	GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, PNULL, &ver_space_ptr);
	GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, &hor_space_ptr, PNULL);
	font_type.color = MMI_BLACK_COLOR;
	font_type.font = font_size;

	//common
    {

		GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border, &font_type);//font should be set as SPEC later...
		GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border, &font_type);
		GUIFORM_SetFormChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border);
		GUIFORM_SetFormActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border);
		//
    }

    GUILABEL_SetBg(chat_bg_form_child_ctrl.child_handle, &chat_underline_bg);//underline_bg_img
    GUIFORM_SetBg(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, &chat_bg);//full background of client space
    GUIFORM_SetBg(parent_form_child_ctrl.child_handle, &chat_bg);//used to set the initial bg of form --- unfocused
    ver_space_ptr = MMISMS_CHAT_FORM_VER_SPACE;//SPEC
    GUIFORM_SetSpace(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, PNULL, &ver_space_ptr);


    GUITEXT_SetBorder(&border, text_form_child_ctrl.child_handle);
    GUILABEL_SetFont(chat_bg_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    GUITEXT_SetTagType(text_form_child_ctrl.child_handle, GUITEXT_TAG_PHONENUM|GUITEXT_TAG_URL, PNULL);

    //set font
#ifdef MMI_PDA_SUPPORT
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
#ifdef MMI_PDA_SUPPORT
        if (!MMITHEME_IsIstyle())
#endif
        {
            font_color = MMI_BLACK_COLOR;
        }
    }
#endif
	GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
    //set string
    if (!is_need_replace)
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            content_str.wstr_ptr,
            content_str.wstr_len,
            FALSE
        );
    }
    else
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            long_content_str.wstr_ptr,
            long_content_str.wstr_len,
            FALSE
        );
    }

#ifdef MMI_PDA_SUPPORT
    GUITEXT_SetHandleTpMsg(
        FALSE,  //in:
        text_form_child_ctrl.child_handle    //in:
    );
#endif

    if(MMISMS_IsSmsBusy(order_id))
    {
        GUILABEL_SetVisible(fail_label_child_ctrl.child_handle,FALSE,FALSE);   //发送时不显示失败图片,失败后显示
    }

    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    //if ( MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id))
#endif        
    {
        g_sms_active_handle = text_form_child_ctrl.child_handle;
        g_last_active_handle = g_sms_active_handle;
        if (MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                || MMK_IsFocusWin(win_id)
                || MMK_IsFocusWin(MMISMS_SEND_ANIM_WIN_ID))

        {             
            //MMK_SetAtvCtrl(win_id, text_form_child_ctrl.child_handle);
#ifndef  MMI_PDA_SUPPORT
            //GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
                    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type)
                    {
                        if( MMISMS_IsSmsBusy(order_id))
                        {
                            //GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL,TXT_NULL , TRUE);    
                            
                        }
                        else
                        {
                            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, STXT_SOFTKEY_RESEND_MK,TXT_NULL , TRUE);
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
                        }
                    }
                    else
                    {
                        //GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL,TXT_NULL , TRUE);
                    }
            
			
			GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK,0,TRUE);
			GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
#endif
        }
    }

}//end if draft type

    if (PNULL != content_str.wstr_ptr)
    {
        SCI_FREE( content_str.wstr_ptr );
        content_str.wstr_ptr = PNULL;
    }
    if (PNULL != long_content_str.wstr_ptr)
    {
        SCI_FREE( long_content_str.wstr_ptr );
        long_content_str.wstr_ptr = PNULL;
    }
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
}

#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description :update readed mms icon 
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_UpdateReadedMMSIcon(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    GUIANIM_CTRL_INFO_T ctrl_info = {0};
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + win_data_ptr->del_index;
    data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
    display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;
    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
}

/*****************************************************************************/
//  Description :create dyna child form for mms
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void SetMMSChatItemParam(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
	MMISMS_ORDER_ID_T order_id
)
{
    GUIFORM_DYNA_CHILD_T parent_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T body_background_container_form_child_ctrl = {0};//used to contain body and background only for the MO_SEND_FAIL type
    GUIFORM_DYNA_CHILD_T body_container_form_child_ctrl = {0};//the body container
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl = {0};//text container title label and size info(downloading size progress info) label
    GUI_BG_T chat_underline_bg = {0};
    GUI_FONT_ALL_T font_type = {0};
    GUI_PADDING_T	form_padding = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    GUITEXT_INIT_DATA_T date_text_init = {0};
    GUIFONT_REGION_STYLE_T  text_region = {0};
    GUIFORM_INIT_DATA_T owndraw_init_data = {0};
    GUIRICHTEXT_INIT_DATA_T richtext_init_data = {0};
#if 1
    GUIFORM_DYNA_CHILD_T title_form_child_ctrl = {0};//used to display 'Media message' string
    GUIFORM_DYNA_CHILD_T size_form_child_ctrl = {0};//size info of the state NOT_DOWNLOAD(DOWNLOADING...)
#else
    GUIFORM_DYNA_CHILD_T richtext_form_child_ctrl = {0};//used to display title & size info
#endif //0
    GUIFORM_DYNA_CHILD_T image_form_child_ctrl = {0};//maybe we can use it to show video_thumbnail as well for video file.
    GUIFORM_DYNA_CHILD_T nonvision_form_child_ctrl = {0};//music,calendar,cvf...
    GUIFORM_DYNA_CHILD_T more_attachments_tip_child_ctrl = {0};//used to show "1/2/... more attachments"
    GUIFORM_DYNA_CHILD_T chat_bg_form_child_ctrl = {0};//used to display the fixed chat background icon
    GUIFORM_DYNA_CHILD_T owndraw_form_child_ctrl = {0};//attachment icon for MT || fail icon for MO_failed
    MMI_HANDLE_T parent_form_handle = 0;//use to iditify the main body container form handle
    MMI_STRING_T content_str = {0};
    MMI_STRING_T    mms_text_string = {0};
    MMI_STRING_T downloading_str = {0};
    MMI_STRING_T time_str = {0};
    MMI_STRING_T filename_str = {0};  
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_INIT_DATA_T date_sim_form_init = {0};
    GUI_FONT_T font_size = MMI_DEFAULT_NORMAL_FONT;
    GUI_COLOR_T font_color = MMI_BLACK_COLOR;
    uint32 index = 0;
    GUI_BG_T chat_bg = {0};
    GUI_BG_T chat_focus_bg = {0};
    GUIANIM_INIT_DATA_T anim_init = {0};
    GUIANIM_CTRL_INFO_T anim_ctrl_info = {0};
    GUIANIM_DATA_INFO_T anim_data_info = {0};
    GUIANIM_DISPLAY_INFO_T anim_disp_info = {0};
    GUITEXT_INIT_DATA_T text_init = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T replace_child_ctrl = 0;
    GUI_BORDER_T border = {0};
    MMISMS_ORDER_INFO_T *mms_display_info = PNULL;
    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_DYNA_CHILD_T date_form_child_ctrl = {0};
    GUILABEL_INIT_DATA_T    bg_label_init = {0};
    GUILABEL_INIT_DATA_T    text_label_init = {0};

    GUIFORM_INIT_DATA_T body_container_form_init = {0};
    GUIFORM_INIT_DATA_T	text_container_form_init = {0};
    uint16 ver_space_ptr = 0;
    uint16 hor_space_ptr = 0;
    uint16 chat_item_index = 0;
    uint16 i_richtext_index = 0;

	//get mms content
    uint16 slide_num = 0;
	uint16 content_num = 0;
	BOOLEAN is_file_exist = FALSE;
	BOOLEAN is_text_exist = FALSE;
	uint32 file_type = 0;
	uint32 multim_buf_len = 0;
    uint8 *image_buf_ptr = PNULL;
    uint8 file_name_temp[MMS_FILE_NAME_LEN + 1]  = {0};
    wchar file_name_wstr[MMS_FILE_NAME_LEN + 1] = {0};
	MMI_IMAGE_ID_T file_tip_icon_id = IMAGE_NULL;
    MMIMMS_EDIT_DOCUMENT_T *    editdoc_ptr = PNULL;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    GUIFORM_DYNA_CHILD_T sim_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T date_sim_form_child_ctrl = {0};
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    GUI_COLOR_T sim_color = MMI_BLACK_COLOR;
#endif
    MMI_STRING_T sim_str = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif

    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam:PNULL == order_id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29154_112_2_18_2_55_20_856, (uint8*)"");
        return;
    }

    chat_item_index = GetChatWinIndex(win_data_ptr,&index, order_id);  //fix coverity CID12305
    
    if (MMISMS_MO_DRAFT == order_id->flag.mo_mt_type)
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam:MMISMS_MO_DRAFT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29159_112_2_18_2_55_20_857, (uint8*)"");
        return;
    }

    //index is full
    if (MMISMS_CHAT_INDEX_MAX == chat_item_index)
    {
        mms_display_info = ReadMMSContent(order_id->record_id);
        GetMMSTimeSujectSize(&content_str, &time_str, mms_display_info);
        //SCI_TRACE_LOW:"SetMMSChatItemParam:not find index!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29167_112_2_18_2_55_20_858, (uint8*)"");
#ifdef MMIMMS_SMS_IN_1_SUPPORT   //短彩合一第0 个控件为MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 1);
#else
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 0);
#endif
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
        if(replace_child_ctrl == 0)
        {
        	SCI_TraceLow("Error:  SetMMSChatItemParam replace_child_ctrl should not be 0,set it to  MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID");
		replace_child_ctrl = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
        }
        index = replace_child_ctrl - MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
    }
    else if (MMISMS_CHAT_INDEX_NEW == chat_item_index)
    {
        mms_display_info = ReadMMSContent(order_id->record_id);
        GetMMSTimeSujectSize(&content_str, &time_str, mms_display_info);
    }
    else
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam: else"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29183_112_2_18_2_55_20_859, (uint8*)"");
        return;
    }

    if (PNULL == mms_display_info)
    {
        return;
    }
    SetChatWinIndex(win_data_ptr,index, TRUE, order_id);

    //create parent form
    if (MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
    	//MMISMS_MO_SEND_FAIL:
        form_init.layout_type = GUIFORM_LAYOUT_SBS;
    }
    else
    {
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    }

    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
    {
        bg_label_init.align = GUILABEL_ALIGN_FORCE_LEFT;
    }
	else
	{
        bg_label_init.align = GUILABEL_ALIGN_RIGHT;
	}

    if (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type || MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
    {
    	  //MMS title and size info should be aligned to the right
	  text_label_init.align = GUILABEL_ALIGN_RIGHT;
    }
    else
    {
	  text_label_init.align = GUILABEL_ALIGN_LEFT;
    }

    form_init.both_rect = both_rect;
    parent_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    parent_form_child_ctrl.is_bg = FALSE;
    parent_form_child_ctrl.is_get_active = TRUE;
    parent_form_child_ctrl.init_data_ptr = &form_init;
    parent_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;
    GUIFORM_CreatDynaChildCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,  &parent_form_child_ctrl);
    GUIFORM_SetChildDisplay(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_DISP_FROM_HEAD);
    //Allow all child form or ctrl set their own background by themselvels
    GUIFORM_SetStyle(parent_form_child_ctrl.child_handle, GUIFORM_STYLE_UNIT);
    GUIFORM_PermitChildBg(parent_form_child_ctrl.child_handle,FALSE);
    GUIFORM_SetPadding(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, &form_padding);

    chat_bg_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    chat_bg_form_child_ctrl.is_bg = FALSE;
    chat_bg_form_child_ctrl.is_get_active = FALSE;
    chat_bg_form_child_ctrl.init_data_ptr = &bg_label_init;
    chat_bg_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_UNDERBG_ITEM_FORM_CTRL_ID + index;

#if 0 //arvin
    date_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    date_form_child_ctrl.is_bg = FALSE;
    date_form_child_ctrl.is_get_active = FALSE;
    date_form_child_ctrl.init_data_ptr = &label_init;
    date_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index;
#endif//0
   /**
   **
   ** LAYOUT SPEC DESCRIPTION OF DIFFERENT MMS MESSAGE
   **
   ** MMISMS_MT_TO_BE_READ
   ** MMISMS_MT_HAVE_READ
   ** MMISMS_MO_SEND_SUCC
   **
   ***************************
     *							        *
     *		     IMAGE                *
	     *							        *
     *************************
     * NON-VISION-ATTA   *
     * mutually-exclusive    *
     * only show 1st one       *
     *************************
     *more attach... tip(opt) *
     *************************
	     *						             *
     *	            TEXT			  *
     *						             *
     *************************
	     *	  BACKGROUND        *
   ***************************
   *NOTE:
   * 1) IMAGE is the 1st priority
   * 2) If there's no image(or video maybe) attachment, we should show the NON-VISION-ATTA if needed and only the 1st attachment should be
   *       drawn here(priority sequence:music>calendar>vcf...temp solution)
   * 3) Display the "more attachments tip" if there's more than one attachment existes;
**
   **
   ** MMISMS_MO_SEND_FAIL
   **
   ******************************
     *							        *     *
     *		     IMAGE                *     *
	     *							        *     *
     ************************      *
     * NON-VISION-ATTA   *     *
     * mutually-exclusive    *     *
     * only show 1st one       *     *
     ************************     *
     *more attach... tip(opt) *     *
     ************************     *
	     *						             *     *
     *	            TEXT			  *     *
     *						             *     *
     ************************  F *
	     *	  BACKGROUND        *     *
   ******************************
   *NOTE: 'F' stands for the send-fail-icon (anim_form_child_ctrl)
   *
   ** MMISMS_MT_NOT_DOWNLOAD
   **
   ************************** 	
     *	    *	              TITLE   *     
     *   A  *          		      SIZE   *     
     *	    *					       *     
     ************************
	*	  BACKGROUND        * 
   **************************
   *NOTE: we should check the state of DOWNLOADING and display the dynamic downloading progress
   *
 **/
    if (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type || MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
    {
	   body_container_form_init.layout_type = GUIFORM_LAYOUT_SBS;
	   text_container_form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    }
    else
    {
	   body_container_form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    }

    body_container_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    body_container_form_child_ctrl.is_bg = FALSE;
    body_container_form_child_ctrl.is_get_active = TRUE;
    body_container_form_child_ctrl.init_data_ptr = &body_container_form_init;
    body_container_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID + index;

#if 0
	richtext_init_data.both_rect = both_rect;
	richtext_form_child_ctrl.guid =SPRD_GUI_RICHTEXT_ID;
     richtext_form_child_ctrl.is_bg = FALSE;
     richtext_form_child_ctrl.is_get_active = TRUE;
     richtext_form_child_ctrl.init_data_ptr = &richtext_init_data;
     richtext_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;
#endif//0		
    chat_underline_bg.bg_type = GUI_BG_IMG;
    chat_bg.bg_type = GUI_BG_COLOR;
    chat_bg.color = MMI_WHITE_COLOR;
    chat_focus_bg.bg_type = GUI_BG_COLOR;
    chat_focus_bg.color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT); ;
    if ((MMISMS_MO_SEND_SUCC == mms_display_info->mo_mt_type) ||
			(MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type) ||
			(MMISMS_MO_DRAFT == mms_display_info->mo_mt_type))
    {
        if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
        {
            chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_RECIPIENT;
        }
        else
        {
            chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_USER;
        }
    }
    else
    {
        if(TRUE == MMISET_IsCurrentLanguageTextAlignRTLDirection())
        {
            chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_USER;
        }
        else
        {
            chat_underline_bg.img_id = IMAGE_MESSAGING_CONVERSATION_RECIPIENT;
        }
    }

    //no border
    border.type = GUI_BORDER_NONE;
    font_type.color = MMI_BLACK_COLOR;
    font_type.font = font_size;
#if 0
    if (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type  ||
            MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type ||
            MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type
       )
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_MMS_NOTIFY_NEW_ICON;
        }
        else
        {
            data_info.img_id = IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
        }
    }
    else if (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type || MMISMS_MO_SEND_SUCC == mms_display_info->mo_mt_type || MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_READ_MMS_ICON;        
        }
        else
        {
            data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
        }
    }
    else
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_NOT_READ_MMS_ICON;
        }
        else
        { 
            data_info.img_id = IMAGE_SMS_NOT_READ_MMS_ICON;
        }
    }
#endif//0
    //set anim align
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;

    //used to display 'Media message' string
    title_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    title_form_child_ctrl.is_bg = FALSE;
    title_form_child_ctrl.is_get_active = TRUE;
    title_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;
    title_form_child_ctrl.init_data_ptr = &text_label_init;

    //size info of the state NOT_DOWNLOAD(DOWNLOADING...)
    size_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    size_form_child_ctrl.is_bg = FALSE;
    size_form_child_ctrl.is_get_active = FALSE;
    size_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SIZE_ITEM_FORM_CTRL_ID + index;
    size_form_child_ctrl.init_data_ptr = &text_label_init;

#if 0
    //create parent child ctrl
    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
#endif //0
    //we should differ the three types:
    // 1) MMISMS_MT_NOT_DOWNLOAD
    // 2) MMISMS_MO_SEND_FAIL
    // 3) other types
    if (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type || MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
    {

        GUILABEL_INIT_DATA_T attach_label_init_data = {0};
        GUIFORM_DYNA_CHILD_T attach_label_child_ctrl = {0};
        GUI_BG_T owndraw_bg = {0};
        uint16 width = 0;
        uint16 height = 0;
            
       //Owndraw ctrl
       //owndraw_init_data.both_rect = both_rect;
       //owndraw_init_data.OwnDrawFunc = DisplayMMSAttachementIcon;//DisplayMessageViewOwnDrawIcons;//we should expand to support two icon draw(fail icon & attachment icon)

       owndraw_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
       owndraw_form_child_ctrl.is_bg = FALSE;
       owndraw_form_child_ctrl.is_get_active = FALSE;
       owndraw_form_child_ctrl.init_data_ptr = &owndraw_init_data;
       owndraw_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_OWNDRAW_ITEM_FORM_CTRL_ID + index;
       
        attach_label_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        attach_label_child_ctrl.is_bg = FALSE;
        attach_label_child_ctrl.is_get_active = FALSE;
        attach_label_child_ctrl.init_data_ptr = &attach_label_init_data;
        attach_label_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_OWNDRAW_ITEM_LABEL_CTRL_ID + index;

       //set text container param(only for NOT_DOWNLOAD_TYPE)
       text_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
       text_form_child_ctrl.is_bg = FALSE;
       text_form_child_ctrl.is_get_active = TRUE;
       text_form_child_ctrl.init_data_ptr = &text_container_form_init;
       text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_TEXT_CONTAINER_ITEM_FORM_CTRL_ID + index;

       //Framework
       GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &body_container_form_child_ctrl);
       GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &chat_bg_form_child_ctrl);



	   //Main body
       GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &owndraw_form_child_ctrl);
       GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &text_form_child_ctrl);


       GUIFORM_CreatDynaChildCtrl(win_id, owndraw_form_child_ctrl.child_handle,  &attach_label_child_ctrl);
        //GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MESSAGING_ATTACHMENT_ICON, win_id);
        //child_height.add_data = height;
        //GUIFORM_SetChildHeight(owndraw_form_child_ctrl.child_handle, attach_label_child_ctrl.child_handle, &child_height);
        GUILABEL_SetIcon(attach_label_child_ctrl.child_handle,IMAGE_MESSAGING_ATTACHMENT_ICON);
        GUIFORM_SetChildDock(owndraw_form_child_ctrl.child_handle, attach_label_child_ctrl.child_handle, GUIFORM_CHILD_DOCK_BOTTOM);

		
	   //Title & Size
       GUIFORM_CreatDynaChildCtrl(win_id, text_form_child_ctrl.child_handle,  &title_form_child_ctrl);
       GUIFORM_CreatDynaChildCtrl(win_id, text_form_child_ctrl.child_handle,  &size_form_child_ctrl);
	   GUIFORM_SetStyle(parent_form_child_ctrl.child_handle, GUIFORM_STYLE_UNIT);
	   //Customize the Layout
	   child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
#if defined (MAINLCD_SIZE_128X160)
       child_height.add_data = 26;//40;
#else
       child_height.add_data = 40;
#endif
       GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
	   GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);

	   GUIFORM_SetChildWidth(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, &child_width);
	   GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, body_container_form_child_ctrl.child_handle, &child_height);
#if defined (MAINLCD_SIZE_128X160)
	   child_width.add_data = 17;//6 + 26;
#else
	   child_width.add_data = 6 + 26;
#endif
	   GUIFORM_SetChildWidth(body_container_form_child_ctrl.child_handle, owndraw_form_child_ctrl.child_handle, &child_width);
#if defined (MAINLCD_SIZE_128X160)
        child_height.add_data = 21;//6+ 26;
#else
        child_height.add_data = 6+ 26;
#endif
        GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, owndraw_form_child_ctrl.child_handle, &child_height);
        //GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, richtext_form_child_ctrl.child_handle, &child_height);
#if defined (MAINLCD_SIZE_128X160)
       child_height.add_data = MMISMS_TEXT_LAYOUT_MARGIN_WIDTH - 2;//4 ;
#else
       child_height.add_data = MMISMS_TEXT_LAYOUT_MARGIN_WIDTH - 4 ;
#endif
	   GUIFORM_SetChildHeight(text_form_child_ctrl.child_handle, title_form_child_ctrl.child_handle, &child_height);
#if defined (MAINLCD_SIZE_128X160)
       child_height.add_data = MMISMS_TEXT_LAYOUT_MARGIN_WIDTH - 2;//4 ;//here we use form margin to achieve the two label acting as one text ctrl effect...
#else
       child_height.add_data = MMISMS_TEXT_LAYOUT_MARGIN_WIDTH - 4 ;//here we use form margin to achieve the two label acting as one text ctrl effect...
#endif
	   GUIFORM_SetChildHeight(text_form_child_ctrl.child_handle, size_form_child_ctrl.child_handle, &child_height);

       child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
       GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_width);
       child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT ;
       GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_height);


	   //no border
	  border.type = GUI_BORDER_NONE;
      font_type.color = MMI_BLACK_COLOR;
	  font_type.font = font_size;
	  GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border, &font_type);//font should be set as SPEC later...
	  GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border, &font_type);
	  GUIFORM_SetFormChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border);
	  GUIFORM_SetFormActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border);

	  GUIFORM_PermitChildBg(body_container_form_child_ctrl.child_handle, FALSE);
	  GUIFORM_PermitChildFont(body_container_form_child_ctrl.child_handle, FALSE);

	  GUIFORM_SetSpace(text_form_child_ctrl.child_handle, &hor_space_ptr, &ver_space_ptr);
	  //GUIFORM_PermitChildBg(text_form_child_ctrl.child_handle, FALSE);
	  //GUIFORM_PermitChildFont(text_form_child_ctrl.child_handle, FALSE);
#if 0//no richtext
	  //append title and size info into the richtext child ctrl
      item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
      item_data.text_type = GUIRICHTEXT_TEXT_RES;
	  item_data.text_set_align = TRUE;
	  item_data.setting_align = ALIGN_RIGHT;
	  item_data.text_set_font = TRUE;
	  item_data.setting_font = font_size;
	  item_data.text_data.res.id = TXT_SMS_NEW_MMS;//temp string id , correct string is "Media message"
      GUIRICHTEXT_AddItem(richtext_form_child_ctrl.child_handle, &item_data, &i_richtext_index);
      item_data.text_type = GUIRICHTEXT_TEXT_BUF;
	  item_data.text_data.buf.str_ptr = content_str.wstr_ptr;
	  item_data.text_data.buf.len = content_str.wstr_len;
      GUIRICHTEXT_AddItem(richtext_form_child_ctrl.child_handle, &item_data, &i_richtext_index);
	  GUIRICHTEXT_SetTxtMargin(richtext_form_child_ctrl.child_handle, 0);
	  GUIRICHTEXT_SetItemSpace(richtext_form_child_ctrl.child_handle, 0);	
	  GUIRICHTEXT_SetItemBorderSpace(richtext_form_child_ctrl.child_handle, 0);
#endif//0

	  //Display MMS Title and Size Info(maybe we should get the downloading status and show the downloading progress here later)
	  GUILABEL_SetFont(title_form_child_ctrl.child_handle, font_size, font_color);
	  GUILABEL_SetFont(size_form_child_ctrl.child_handle, font_size, font_color);

         SCI_TRACE_LOW("SetMMSChatItemParam: mms_display_info->mo_mt_type=%d", mms_display_info->mo_mt_type);
	  //Foucsed: we should add font opacity of the SIZE label font!!! 
	  if (MMIAPIMMS_IsProcessMMSById(order_id->record_id))
	  {
		//Downloading...
		//20/85KB
#ifdef PIC_VIEWER_SUPPORT
		  GUILABEL_SetTextById(title_form_child_ctrl.child_handle, STR_ID_UMMS_DOWNLOAD_IN_PROGRESS, FALSE);//need modify to 'Downloading...'
#else
		  GUILABEL_SetTextById(title_form_child_ctrl.child_handle, TXT_COMMON_DOWNLOADING_WAIT, FALSE);//need modify to 'Downloading...'
#endif
		  MMSMMI_UpdateAndGetProgressInfo(order_id->record_id, &downloading_str);
		  GUILABEL_SetText(size_form_child_ctrl.child_handle, &downloading_str, FALSE);
		  if (downloading_str.wstr_ptr)
		  {
			  SCI_FREE(downloading_str.wstr_ptr);
			  downloading_str.wstr_ptr = PNULL;
		  }
		  SCI_TRACE_LOW("SetMMSChatItemParam: Downloading...");
	  }
	  else
	  {
		//Media message
		//Size:85KB
	
		GUILABEL_SetTextById(title_form_child_ctrl.child_handle, STR_DTAIL_TYPE_MMS, FALSE);
		GUILABEL_SetText(size_form_child_ctrl.child_handle, &content_str, FALSE);
              SCI_TRACE_LOW("SetMMSChatItemParam: details");
	  }
    }
    else
    {
      //let's handle the MO_SEND_SUCC and MT_RECV_SUCC first and then put the fail icon on its right ....K.O.
	  if (MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
	  {
            GUILABEL_INIT_DATA_T fail_label_init_data = {0};
            GUIFORM_DYNA_CHILD_T fail_label_child_ctrl = {0};
            GUI_BG_T owndraw_bg = {0};
            uint16 width = 0;
            uint16 height = 0;
            
		body_background_container_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
	    body_background_container_form_child_ctrl.is_bg = FALSE;
	    body_background_container_form_child_ctrl.is_get_active = TRUE;
	    body_background_container_form_child_ctrl.init_data_ptr = &body_container_form_init;
	    body_background_container_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_TEXT_CONTAINER_ITEM_FORM_CTRL_ID + index;//reuse with sms under the same type MO_SEND_FAIL
	    
           //do not use owndraw ctrl now, owndraw calc wrong rect that make display error
		//Owndraw ctrl
		//owndraw_init_data.both_rect = both_rect;
		//owndraw_init_data.OwnDrawFunc = DisplayMessageViewOwnDrawIcons;//DisplayMessageViewOwnDrawIcons;//we should expand to support two icon draw(fail icon & attachment icon)

		owndraw_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
		owndraw_form_child_ctrl.is_bg = FALSE;
		owndraw_form_child_ctrl.is_get_active = FALSE;
		owndraw_form_child_ctrl.init_data_ptr = &owndraw_init_data;
		owndraw_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_OWNDRAW_ITEM_FORM_CTRL_ID + index;


        fail_label_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        fail_label_child_ctrl.is_bg = FALSE;
        fail_label_child_ctrl.is_get_active = FALSE;
        fail_label_child_ctrl.init_data_ptr = &fail_label_init_data;
        fail_label_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_OWNDRAW_ITEM_LABEL_CTRL_ID + index;

        //Framework
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &body_background_container_form_child_ctrl);//left main body
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &owndraw_form_child_ctrl);//right fail icon
        GUIFORM_CreatDynaChildCtrl(win_id, owndraw_form_child_ctrl.child_handle,  &fail_label_child_ctrl);

        GUIFORM_SetChildDock(owndraw_form_child_ctrl.child_handle, fail_label_child_ctrl.child_handle, GUIFORM_CHILD_DOCK_BOTTOM);
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MESSAGING_ERROR_BLUE_MINUS_2, win_id);
        child_height.add_data = height;
        GUIFORM_SetChildHeight(owndraw_form_child_ctrl.child_handle, fail_label_child_ctrl.child_handle, &child_height);
        GUILABEL_SetIcon(fail_label_child_ctrl.child_handle,IMAGE_MESSAGING_ERROR_BLUE_MINUS_2);

        owndraw_bg.color = MMI_WHITE_COLOR;
        owndraw_bg.bg_type = GUI_BG_COLOR;
        GUIFORM_SetBg(owndraw_form_child_ctrl.child_handle,&owndraw_bg);

        GUIFORM_CreatDynaChildCtrl(win_id, body_background_container_form_child_ctrl.child_handle,  &body_container_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, body_background_container_form_child_ctrl.child_handle,  &chat_bg_form_child_ctrl);
        GUIFORM_SetStyle(parent_form_child_ctrl.child_handle, GUIFORM_STYLE_UNIT);
			
	 	GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
		GUIFORM_SetChildAlign(body_background_container_form_child_ctrl.child_handle, body_container_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
	    GUIFORM_SetChildAlign(body_background_container_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);

        child_width.add_data = MMISMS_CHATFORM_ITEM_WIDTH;
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, body_background_container_form_child_ctrl.child_handle, &child_width);
        GUIFORM_SetSpace(body_background_container_form_child_ctrl.child_handle, &hor_space_ptr, &ver_space_ptr);
		GUIFORM_PermitChildBg(body_background_container_form_child_ctrl.child_handle, FALSE);
		GUIFORM_PermitChildFont(body_background_container_form_child_ctrl.child_handle, FALSE);
		//Owndraw layout and param setting
        child_width.add_data = MMISMS_CHATFORM_ANIM_WIDTH;
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, owndraw_form_child_ctrl.child_handle, &child_width);
        GUIOWNDRAW_SetBg(owndraw_form_child_ctrl.child_handle, &chat_bg);

	    child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	    GUIFORM_SetChildWidth(body_background_container_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_width);
	    child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
 	    GUIFORM_SetChildHeight(body_background_container_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_height);
	   }
	   else
	   {
	    //GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &body_container_form_child_ctrl);
	   	//Framework
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &body_container_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &chat_bg_form_child_ctrl);
	    child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	  	GUIFORM_SetChildWidth(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, &child_width);//common

	    child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	    GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_width);
	    child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
 	    GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, &child_height);

		if(MMISMS_MO_SEND_SUCC == mms_display_info->mo_mt_type)
		 {
	 		GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
			GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, body_container_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_LEFT);
			GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
		 }
		 else
		 {
	 		GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
			GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, body_container_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
			GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, chat_bg_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
		 }
	   }
	   //reuse the following code to satisfy the MO_FAIL and other types

       editdoc_ptr = MMIMMS_ReadMMS(order_id->record_id);
	   if (MMISMS_MT_TO_BE_READ == order_id->flag.mo_mt_type)
	   {
		   MMIMMS_ChangeRecordType(order_id->record_id, MMISMS_MT_HAVE_READ);
	   }

       if(win_data_ptr->mms_count < MMISMS_MAX_MMS_READED)
       {
            win_data_ptr->edit_doc_ptr[win_data_ptr->mms_count++] = editdoc_ptr;
       }
#if defined (MMS_SUPPORT)
       content_num = MMIMMS_GetAllContentCount(editdoc_ptr);//can get all valid attachments count
	  is_text_exist = MMIMMS_GetTextContent(editdoc_ptr, &mms_text_string);
#endif

	   if(0 == content_num)
	   {
		 //Impossible case: no text and no attachment!
		 //if there's no text content we should show a fixed tip like 'Empty MMS'(although it's an impossible case)
		 //[ARVIN_ADD_TRACE]warning info ---empty mms
	   }
	   else
	   {
	     //attchment vision construct
	     is_file_exist = MMIMMS_GetFirstImageFileIfExists(&file_type, &multim_buf_len, &image_buf_ptr, editdoc_ptr);
	     if(is_file_exist)
	     {
	        //create image file ctrl and insert the image info into it
	        anim_init.is_form_bg = FALSE;
			//anim_init.both_rect = both_rect;                      //not need malloc buff for init process
			image_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
			image_form_child_ctrl.is_bg = FALSE;
			//Text should be the only active ctrl if text exists
			image_form_child_ctrl.is_get_active = FALSE;//(!is_text_exist);//TRUE;
			image_form_child_ctrl.init_data_ptr = &anim_init;
			image_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID + index;
			GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &image_form_child_ctrl);

			GUIFORM_SetSpace(body_container_form_child_ctrl.child_handle, &hor_space_ptr, &ver_space_ptr);
#ifdef LOW_MEMORY_SUPPORT     
#if defined (MAINLCD_SIZE_128X160)
			child_width.add_data = 90;//(MMISMS_SMS_FIXED_TEXT_BG_WIDTH*3) >> 2;
			child_height.add_data = 90;//(MMISMS_SMS_FIXED_IMAGE_HEIGTH*3) >> 2;
#else
			child_width.add_data = (MMISMS_SMS_FIXED_TEXT_BG_WIDTH*3) >> 2;
			child_height.add_data = (MMISMS_SMS_FIXED_IMAGE_HEIGTH*3) >> 2;
#endif
#else
			child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
			child_height.add_data = MMISMS_SMS_FIXED_IMAGE_HEIGTH;
#endif
			GUIFORM_SetChildWidth(body_container_form_child_ctrl.child_handle, image_form_child_ctrl.child_handle, &child_width);
			GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, image_form_child_ctrl.child_handle, &child_height);

			anim_ctrl_info.is_ctrl_id = TRUE;
			anim_ctrl_info.ctrl_id = image_form_child_ctrl.child_handle;

			anim_data_info.data_ptr= image_buf_ptr;
			anim_data_info.data_size = multim_buf_len;

			anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_disp_info.is_free_buf_lose_focus = TRUE;
            anim_disp_info.is_free_buf_not_in_displayrec = TRUE;
			GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);
	     }
	     else
	     {
			is_file_exist = MMIMMS_GetFirstNonImageFileInfoIfExists(&file_type, MMS_FILE_NAME_LEN, &file_name_temp, &file_tip_icon_id, editdoc_ptr);
			if(is_file_exist)
			{
			    //create the NON-IMAGE attachment file label
			    nonvision_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
			    nonvision_form_child_ctrl.is_bg = FALSE;
				//Text should be the only active ctrl if text exists
			    nonvision_form_child_ctrl.is_get_active = FALSE;//(!is_text_exist);
			    nonvision_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID + index;//we can use this ID for there'll be only one of the two can be shown on the screen
			    nonvision_form_child_ctrl.init_data_ptr = &text_label_init;
       		    GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &nonvision_form_child_ctrl);

			    child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
		 	    child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;
		 	    GUIFORM_SetChildWidth(body_container_form_child_ctrl.child_handle, nonvision_form_child_ctrl.child_handle, &child_width);
		 	    GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, nonvision_form_child_ctrl.child_handle, &child_height);

                GUILABEL_SetFont(nonvision_form_child_ctrl.child_handle, font_size, MMI_BLACK_COLOR);
			    //GUILABEL_SetMargin(nonvision_form_child_ctrl.child_handle, 6);
			    //GUILABEL_SetBackgroundColor(nonvision_form_child_ctrl.child_handle, GUI_RGB2RGB565(0x60, 0x60, 0x60));
			    GUILABEL_SetIcon(nonvision_form_child_ctrl.child_handle, file_tip_icon_id);

	  		    multim_buf_len = MIN(MMS_FILE_NAME_LEN, SCI_STRLEN(file_name_temp));
                GUI_UTF8ToWstr(file_name_wstr, multim_buf_len, file_name_temp, multim_buf_len);

	  		    filename_str.wstr_len = multim_buf_len;
	  		    filename_str.wstr_ptr = SCI_ALLOC_APPZ((filename_str.wstr_len + 1) * sizeof(wchar));

				MMI_WSTRNCPY(
				    &(filename_str.wstr_ptr[0]),
				    filename_str.wstr_len,
				    file_name_wstr,
				    filename_str.wstr_len,
				    filename_str.wstr_len
				);

			    GUILABEL_SetText(nonvision_form_child_ctrl.child_handle, &filename_str, FALSE);
			}
	     }

		content_num = ((is_text_exist) ? (content_num -1) : content_num);
		if(1 < content_num)
		{
			//create the tip label '(content_num -1) more attachment...'
			text_label_init.align = GUILABEL_ALIGN_RIGHT;
			more_attachments_tip_child_ctrl.guid = SPRD_GUI_LABEL_ID;
			more_attachments_tip_child_ctrl.is_bg = FALSE;
			more_attachments_tip_child_ctrl.is_get_active = FALSE;
			more_attachments_tip_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_MORE_ATTA_TIP_ITEM_FORM_CTRL_ID + index;
			more_attachments_tip_child_ctrl.init_data_ptr = &text_label_init;
       		GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &more_attachments_tip_child_ctrl);
			child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
		 	child_height.add_data = MMISMS_SMS_FIXED_TEXT_BG_HEIGHT;

		 	GUIFORM_SetChildWidth(body_container_form_child_ctrl.child_handle, more_attachments_tip_child_ctrl.child_handle, &child_width);
		 	GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, more_attachments_tip_child_ctrl.child_handle, &child_height);

			//!!!should splice the specific string like '(content_num -1) more attachment...'!!!
			GUILABEL_SetFont(more_attachments_tip_child_ctrl.child_handle, font_size, MMI_BLACK_COLOR);
			GUILABEL_SetMargin(more_attachments_tip_child_ctrl.child_handle, 6);
			GUILABEL_SetTextById(more_attachments_tip_child_ctrl.child_handle, STR_MSG_MORE_ATTACHMENT_EXT01, FALSE);
		}
		//!!!if(there's text content)  Get all text context and create the text form!!!
		// SPEC UI !!!!
		//text content: get the text content and display in the new dynamic text form
		//no text content: set the 4px margin between body_container and under_bg_form and set the under_bg_label background color as BLACK!!!
		//which is used to satisfy the SPEC :under_bg_form is NOT allowed to modify its background color!!!

		//create new text form to display the text contents
	     //set text param
	    text_form_child_ctrl.guid = SPRD_GUI_TEXTBOX_ID;
	    text_form_child_ctrl.is_bg = FALSE;
	    text_form_child_ctrl.is_get_active = TRUE;
	    text_form_child_ctrl.init_data_ptr = &date_text_init;
	    text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;

        GUIFORM_CreatDynaChildCtrl(win_id, body_container_form_child_ctrl.child_handle,  &text_form_child_ctrl);
	    child_width.add_data = MMISMS_SMS_FIXED_TEXT_BG_WIDTH;
	    GUIFORM_SetChildWidth(body_container_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);
		if(is_text_exist && MMIMMS_GetAllSlideTextContent(editdoc_ptr, &mms_text_string))//text content existes
		{
            //set margin
		    GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
		                        MMISMS_CHAT_TEXT_MARGIN_LEFT,
		                        MMISMS_CHAT_TEXT_MARGIN_RIGHT,
		                        MMISMS_CHAT_TEXT_MARGIN_TOP,
		                        MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
            GUITEXT_SetBorder(&border, text_form_child_ctrl.child_handle);
            GUITEXT_SetTagType(text_form_child_ctrl.child_handle, GUITEXT_TAG_PHONENUM  | GUITEXT_TAG_URL, PNULL);
			GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
            GUITEXT_SetString(
		                     text_form_child_ctrl.child_handle,
				             mms_text_string.wstr_ptr,
				             mms_text_string.wstr_len,
				             FALSE);
			GUITEXT_SetLineSpace(text_form_child_ctrl.child_handle, MMISMS_CHAT_TEXT_LINE_SPACE);
			MMIMMS_FreeTextContentMem();
		}
		else
		{
			//no text case!!!!
#if defined (MAINLCD_SIZE_128X160)
	    	child_height.add_data = 3;//;//should be replaced by macro
#else
	    	child_height.add_data = 4;//should be replaced by macro
#endif
	    	GUIFORM_SetChildHeight(body_container_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_height);
		    GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
		                        MMISMS_CHAT_TEXT_MARGIN_LEFT,
		                        MMISMS_CHAT_TEXT_MARGIN_RIGHT,
		                        0,
		                        0);
		}
	  }

	  GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border, &font_type);//font should be set as SPEC later...
	  GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border, &font_type);
	  GUIFORM_SetFormChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, &border);
	  GUIFORM_SetFormActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, &border);

      GUIFORM_PermitChildBg(body_container_form_child_ctrl.child_handle, FALSE);
	  GUIFORM_PermitChildFont(body_container_form_child_ctrl.child_handle, FALSE);
	   //reuse end
    }

    //Define common layout params/Start
    GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, &hor_space_ptr, &ver_space_ptr);
    //chat_bg_label ---Display timestamp
    GUILABEL_SetText(chat_bg_form_child_ctrl.child_handle, &time_str, FALSE); 
    GUILABEL_SetFont(chat_bg_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    GUILABEL_SetBg(chat_bg_form_child_ctrl.child_handle, &chat_underline_bg);//underline_bg_img	   
    //Define common layout params/End

#if 0
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡
    if (MMISMS_MO_SEND_FAIL != mms_display_info->mo_mt_type)
    {
        sim_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        sim_form_child_ctrl.is_bg = FALSE;
        sim_form_child_ctrl.is_get_active = FALSE;
        sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID + index;
        sim_form_child_ctrl.init_data_ptr = &label_init;

        date_sim_form_init.layout_type = GUIFORM_LAYOUT_SBS;
        date_sim_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_sim_form_child_ctrl.is_bg = FALSE;
        date_sim_form_child_ctrl.is_get_active = FALSE;
        date_sim_form_child_ctrl.init_data_ptr = &date_sim_form_init;
        date_sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID + index;
#if 0
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &date_sim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &sim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);

        GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, date_sim_form_child_ctrl.child_handle, &child_height);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMISMS_CHATFORM_SIM_LABEL_WIDTH;
        GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, sim_form_child_ctrl.child_handle, &child_width);

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = GetChatItemDateWidth();
        GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_width);
#endif//0
    }
    else
#endif
    {
        //GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        //GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        //GUIFORM_SetChildHeight(date_text_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_height);
    }

    //set anim
    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_width.add_data = MMISMS_CHATFORM_ANIM_WIDTH;
    GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, ctrl_info.ctrl_id, &child_width);
    GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, PNULL, &ver_space_ptr);
    //set bg
    chat_bg.bg_type = GUI_BG_IMG;
    chat_focus_bg.bg_type = GUI_BG_IMG;

    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type) )
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
		GUITEXT_SetLineSpace(text_form_child_ctrl.child_handle, MMISMS_CHAT_TEXT_LINE_SPACE);
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MT_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MT_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
		GUITEXT_SetLineSpace(text_form_child_ctrl.child_handle, MMISMS_CHAT_TEXT_LINE_SPACE);
    }
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, PNULL, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, PNULL, PNULL);

    border.type = GUI_BORDER_NONE;
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);

    //GUIFORM_PermitChildFont(date_text_form_child_ctrl.child_handle, FALSE);

#ifdef MMI_PDA_SUPPORT
    if (!MMITHEME_IsIstyle())
    {
        GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    else
    {
        GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    }
#endif	
#else
    GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
#endif

    if (MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        MMI_STRING_T fail_str = {0};

        if (MMISMS_TYPE_MMS == order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(order_id->record_id)))
        {
            MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
        }
        str = SCI_ALLOC_APPZ((MMISMS_CHAT_SENDFAIL_STR_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == str)
        {
            SCI_FREE( content_str.wstr_ptr );
            return;
        }
        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN , time_str.wstr_ptr, time_str.wstr_len, MIN(time_str.wstr_len, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN));
        MMIAPICOM_Wstrcat(str, L" ");
        MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        wstr.wstr_len = MMIAPICOM_Wstrlen(str);
        GUILABEL_SetText(date_form_child_ctrl.child_handle, &wstr, FALSE);
        SCI_FREE(str);
    }
    else
    {
        GUILABEL_SetText(date_form_child_ctrl.child_handle, &time_str, FALSE);
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡    
        sim_name = MMIAPISET_GetSimName(order_id->flag.dual_sys);

        sim_str.wstr_len  = sim_name.wstr_len;
        sim_str.wstr_ptr = sim_name.wstr_arr;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
        sim_color = MMIAPISET_GetSimColor(order_id->flag.dual_sys);
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, sim_color);
#else
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
#endif
        GUILABEL_SetText(sim_form_child_ctrl.child_handle, &sim_str, FALSE);
#endif
    }
#ifdef MMI_PDA_SUPPORT
    //set font
    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
    {
        if (!MMITHEME_IsIstyle())
        {
            font_color = MMI_GRAY_WHITE_COLOR;
        }
    }
    else
    {
        if (!MMITHEME_IsIstyle())
        {
            font_color = MMI_DARK_GRAY_COLOR;
        }
    }
#else
    font_color = MMI_BLACK_COLOR;
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT
    font_size = MMISMS_GetWinFont();
#endif
#endif
    GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
    //set string
    GUITEXT_SetString(
        text_form_child_ctrl.child_handle,
        content_str.wstr_ptr,
        content_str.wstr_len,
        FALSE
    );

#ifdef MMI_PDA_SUPPORT
    GUITEXT_SetHandleTpMsg(
        FALSE,  //in:
        text_form_child_ctrl.child_handle    //in:
    );
#endif
#endif//0
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    //if ( MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id))
#endif        
    {
        if (MMK_IsFocusWin(win_id)
            || MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
            || MMK_IsFocusWin(MMISMS_SEND_ANIM_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_NEWMSG_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_SENDING_WIN_ID)//发送waiting框未关闭返回时，设置active控件
            || MMK_IsFocusWin(MMIMMS_DOWNLOADING_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_PREVIEW_WIN_ID)//预览mms界面打开时，设置active控件。防止预览界面关闭后，active控件丢失。
          )
        {
            g_mms_active_handle = parent_form_child_ctrl.child_handle;
            g_last_active_handle = g_mms_active_handle;
            //MMK_SetAtvCtrl(win_id, parent_form_child_ctrl.child_handle);
        }
        GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MENU_MK,0,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,2,TRUE);
    }

    if (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type || MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type ||MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type )
    {
            if (MMIAPIMMS_IsProcessMMSById(mms_display_info->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(mms_display_info->record_id) )//如果是正在下载
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, MIDDLE_BUTTON, FALSE);
            }
            else
            {               
                GUIWIN_SeSoftkeytButtonTextId(win_id, STR_SOFTKEY_DOWNLOAD_EXT02, MIDDLE_BUTTON, FALSE);
            }
    }
    else if(MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_RESEND_MK, MIDDLE_BUTTON, FALSE);
    }
    else
    {
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_OPEN_MK, MIDDLE_BUTTON, FALSE);
    }
    
    GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON,  FALSE);
        
    if (PNULL != content_str.wstr_ptr)
    {
        SCI_FREE( content_str.wstr_ptr );
    }

    if (PNULL != downloading_str.wstr_ptr)
    {
        SCI_FREE( downloading_str.wstr_ptr );
    }

    if (PNULL != time_str.wstr_ptr)
    {
        SCI_FREE( time_str.wstr_ptr );
    }
    if (PNULL != filename_str.wstr_ptr)
    {
        SCI_FREE( filename_str.wstr_ptr );
    }
}
#endif
#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description : 获取彩信的相关信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_INFO_T *ReadMMSContent(int32 record_id)
{
    MMISMS_ORDER_INFO_T *mms_order_info_ptr = PNULL;

    mms_order_info_ptr = MMIAPIMMS_GetListItemByID(record_id);

    return mms_order_info_ptr;
}

/*****************************************************************************/
//  Description : 获取彩信相关部分信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void GetMMSTimeSujectSize(MMI_STRING_T *string_ptr, MMI_STRING_T  *time_str, MMISMS_ORDER_INFO_T *mms_display_info)
{
    uint16 copy_offset = 0;
    uint8 time_len = 0;
    wchar time[20] = {0};
    wchar cr_ch = MMIAPICOM_GetCRCh();
    uint8 cr_len = 1;
    //MMI_STRING_T subject_temp_string = {0};
    MMI_STRING_T subject_none_string = {0};
    uint32 total_size = 0;
    MMI_STRING_T size_temp_string = {0};
    //char display_str_size[MMIMMS_SIZE_STRING_MAX_LEN+1] ={0};
    wchar display_wstr_size[MMIMMS_SIZE_STRING_MAX_LEN + 1] = {0};
    uint8 display_wstr_size_len = 0;

    if (PNULL == string_ptr || PNULL == time_str || PNULL == mms_display_info)
    {
        return;
    }
    total_size = mms_display_info->file_size;
    //time length

    time_len = MMISMS_GetDateTimeStringBySecond(mms_display_info->time,TRUE,FALSE,TRUE,time);
    
    time_str->wstr_ptr = SCI_ALLOCAZ((time_len + 1) * sizeof(wchar));
    if (PNULL == time_str->wstr_ptr)
    {
        return;
    }

    if (0 < time_len)
    {
        MMI_WSTRNCPY(
            time_str->wstr_ptr,
            time_len,
            (const wchar *)time,
            time_len,
            time_len
        );

        time_str->wstr_len = time_len;
    }

#if 0//temp delete subject code for MS SPEC---1
    //subject length
    MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT, &subject_temp_string);
    if (0 < mms_display_info->display_content.content_len)
    {
        string_ptr->wstr_len += subject_temp_string.wstr_len + mms_display_info->display_content.content_len + cr_len;
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_BLANK, &subject_none_string);
        string_ptr->wstr_len += subject_temp_string.wstr_len + subject_none_string.wstr_len + cr_len;
    }
#endif//0
    //size length
    MMI_GetLabelTextByLang(TXT_DETAIL_SIZE , &size_temp_string);
    display_wstr_size_len = MMIAPIMMS_GetMMSFileSizeStr(total_size, display_wstr_size);
    string_ptr->wstr_len += size_temp_string.wstr_len + display_wstr_size_len;

    // allocate memory
    string_ptr->wstr_ptr = SCI_ALLOC_APPZ((string_ptr->wstr_len + 1) * sizeof(wchar));
    if (PNULL == string_ptr->wstr_ptr )
    {
        //not free time
        return;
    }
#if 0//temp delete subject code for MS SPEC---2
    //subject:
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        subject_temp_string.wstr_len,
        subject_temp_string.wstr_ptr,
        subject_temp_string.wstr_len,
        subject_temp_string.wstr_len
    );
    copy_offset += subject_temp_string.wstr_len;
    //subject content
    if (0 < mms_display_info->display_content.content_len)
    {
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            mms_display_info->display_content.content_len,
            (const wchar*)mms_display_info->display_content.content,
            mms_display_info->display_content.content_len,
            mms_display_info->display_content.content_len
        );
        copy_offset += mms_display_info->display_content.content_len;
    }
    else
    {
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            subject_none_string.wstr_len,
            subject_none_string.wstr_ptr,
            subject_none_string.wstr_len,
            subject_none_string.wstr_len
        );
        copy_offset += subject_none_string.wstr_len;
    }
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        &cr_ch,
        cr_len,
        cr_len
    );
    copy_offset += cr_len;
#endif//0
    //size:
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        size_temp_string.wstr_len,
        size_temp_string.wstr_ptr,
        size_temp_string.wstr_len,
        size_temp_string.wstr_len
    );
    copy_offset += size_temp_string.wstr_len;
    //size value
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        display_wstr_size_len,
        display_wstr_size,
        display_wstr_size_len,
        display_wstr_size_len
    );
}
#endif
/*****************************************************************************/
//  Description : get chat content , delete time
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ChatWinGetItemContent(MMI_STRING_T *content)
{
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMI_STRING_T text_string = {0};

    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id < MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID || active_ctrl_id > MMISMS_MAX_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID)
    {
        //SCI_TRACE_LOW:"mmisms chat not focus in chat item"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30347_112_2_18_2_55_23_866, (uint8*)"");
        return FALSE;
    }

    GUITEXT_GetString(active_ctrl_id, &text_string);

    if (text_string.wstr_ptr == PNULL)
    {
        return FALSE;
    }

    content->wstr_len = text_string.wstr_len;

    content->wstr_ptr = SCI_ALLOCA(content->wstr_len * sizeof(wchar) + 1);
    if (PNULL == content->wstr_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(content->wstr_ptr, 0x00, (content->wstr_len * sizeof(wchar) + 1));

    SCI_MEMCPY(content->wstr_ptr, text_string.wstr_ptr, (content->wstr_len * sizeof(wchar) + 1));

    return TRUE;
}

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopySimToMe(void)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
        HandleChatWinOpenCopyQuery();
    }
}

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopyMeToSim(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E copy_dual_sys = MN_DUAL_SYS_1;
        uint32 copy_sim_num = 0;
        uint32 copy_mark_num = 0;
        MMIPB_BCD_NUMBER_T pb_num = {0};
        //uint16 cur_index = 0;
		//MMI_CTRL_ID_T cur_ctrl_id = 0;
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        MMISMS_SetCurMsgMarked(TRUE);

        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);

        if (0 == sim_num)
        {
            MMISMS_SetCurMsgMarked(FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
        }
        else if (1 == sim_num)
        {
            copy_dual_sys = (MN_DUAL_SYS_E)sim_ok;

            //cur_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
            //cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            copy_sim_num = MMISMS_GetCurChatMarkedMsgNumByDualsys(pb_num, MN_SMS_STORAGE_SIM, copy_dual_sys);
            copy_mark_num = MMISMS_GetCurChatMarkedMsgNum(pb_num, MN_SMS_STORAGE_ALL);

            if (copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurMsgMarked(FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(copy_dual_sys);
                //MMISMS_OperateCopy();
                HandleChatWinOpenCopyQuery();
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(HandleChatWinCopySelectSimCallback, PNULL, FALSE);
        }
#endif
    }
}


/*****************************************************************************/
//  Description : open the SMS copy query window
//  Global resource dependence :
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenCopyQuery(void)
{
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;

    MMISMS_SetCurMsgMarked(TRUE);

    MMI_GetLabelTextByLang(TXT_COMMON_COPY, &prompt_str1);
    order_id = MMISMS_GetCurOperationOrderId();
    if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
    {
        prompt_str2.wstr_len = order_id->display_content.content_len;
        prompt_str2.wstr_ptr = order_id->display_content.content;
    }
    else
    {
        MMI_WSTRNCPY(str,
                     MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                     order_id->display_content.content,
                     MMISMS_QUERY_STRING_MAX_NUM,
                     MMISMS_QUERY_STRING_MAX_NUM);

        MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                       MMISMS_ELLIPSE_STR_LEN,
                       (const uint8 *)ellipse_str,
                       MMISMS_ELLIPSE_STR_LEN,
                       MMISMS_ELLIPSE_STR_LEN);
        prompt_str2.wstr_ptr = str;
        prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
    }

    MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinCopyQueryWinMsg );

}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyFinishCallback(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    if(data_info_ptr == PNULL)
    {
       return ret;
    }
    MMISMS_SetMoveAllFlag(FALSE);

    if (!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }

    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        MMISMS_WriteFolderType();
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = TRUE;
    }
    else if (MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(STR_NO_SPACE_EXT01, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        //MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = FALSE;
    }
    else if (MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = FALSE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;    
    MMISMS_COPY_DATA_INFO_T data_info = {0};
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr PNULL");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            //HandleChatWinOperateCopy();
          
            #ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
        
                return recode;
            }
            #endif
            MMISMS_SetMoveAllFlag(TRUE);
            MMISMS_SetCurMsgMarked(TRUE);  
            data_info.box_type = MMISMS_BOX_CHAT;
            data_info.oper_status = MMISMS_GetCurrentOperStatus();
            data_info.pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                //GUILIST_GetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
            MMISMS_EnterCopyWaitWin(&data_info,HandleChatWinCopyFinishCallback,PNULL);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetCurMsgMarked(FALSE);
            //MMK_CloseWin(win_id);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to operate the copy
//  Global resource dependence :
//  Author:feingming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOperateCopy(void)
{
    MMI_STRING_T prompt_str = {0};

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif

    MMISMS_SetMoveAllFlag(TRUE);
    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &prompt_str);

    MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMISMS_COPY_ANIM_WIN_ID, IMAGE_NULL,
                       ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_CUSTOMER, HandleChatWinCopyWaitWinMsg);

    MMISMS_SetCurMsgMarked(TRUE);

    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_COPY_NEXT);
}

/*****************************************************************************/
//  Description : to handle the message of copy waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
            break;

        case MSG_CLOSE_WINDOW:
            MMISMS_SetMoveAllFlag(FALSE);

            if (!MMISMS_GetMoveSingleFlag())
            {
                MMISMS_ClearOperInfo();
            }

            //全部删除完毕，将未处理链表中的sms插入到相应的链表中
            MMISMS_InsertUnhandledSmsToOrder();
            MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MMIAPICOM_StatusAreaSetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetMoveAllFlag(FALSE);
            MMISMS_WriteFolderType();
            if (!MMISMS_GetMoveSingleFlag())
            {
                MMK_CloseWin(win_id);
            }
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 copy_sim_num = 0;
    uint32 copy_mark_num = 0;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    //MMI_CTRL_ID_T cur_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
    //uint16 cur_index = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
    if (isReturnOK)
    {
        if (!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            MMISMS_SetCurMsgMarked(FALSE);
        }
        else
        {
            //cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            copy_sim_num = MMISMS_GetCurChatMarkedMsgNumByDualsys(pb_num, MN_SMS_STORAGE_SIM, (MN_DUAL_SYS_E)dual_sys);
            copy_mark_num = MMISMS_GetCurChatMarkedMsgNum(pb_num, MN_SMS_STORAGE_ALL);

            if (copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurMsgMarked(FALSE);

            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                HandleChatWinOpenCopyQuery();
            }
        }
    }

    return recode;
}
#endif
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description : speed dial edit select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EditSelectVcardCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMI_PB_VCARD_SELECTED, (DPARAM)handle);
}

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
}

/*****************************************************************************/
//  Description :mms edit insert video
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertVideo(MMI_WIN_ID_T pre_win_id)
{
#ifndef UNISOC_VERSION
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MOVIE_ALL, 0, 0, 0, PNULL, 0, -1};

    select_info.max_size = MMIMMS_MMS_LIMIT_SIZE;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectMovieWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_VIDEO);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID,TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
#else   
#ifdef PIC_VIEWER_SUPPORT
    MMIAPIPICVIEW_EnterOnlyVideosForMMS(pre_win_id);
#endif
    MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_VIDEO);
#endif
}

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertMusic(MMI_WIN_ID_T pre_win_id)
{
    int32 music_max_size     = 0;
    MMIFMM_SELECT_INFO_T        select_info = {MMIFMM_MUSIC_ALL, 0, 0, 0, PNULL, 0, -1};
    music_max_size = MMIMMS_MMS_LIMIT_SIZE;

    select_info.max_size = music_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectMusicWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_MUSIC);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}
/*****************************************************************************/
//  Description :mms edit insert vcard
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertVcard(void)
{
    MMIAPIPB_OpenMainWin(
        MMIPB_ALL_CONTACT,//
        MMIPB_LIST_RADIO, //IN: 号码列表的类型
        MMIPB_LIST_FIELD_WHOLE,
        1,//如果是多选时，此项限制了多选的最大个数
        PNULL,
        EditSelectVcardCallback
    );
    
    MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
}

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertPic(MMI_WIN_ID_T pre_win_id)
{
#ifndef UNISOC_VERSION
    MMIFMM_SELECT_INFO_T    select_info =
    {
        MMIFMM_PIC_ALL, 0,
        0, 0, PNULL, 0, -1
    };

    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
    select_info.max_size = 0;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectPictureWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_PIC);
    }
    else
    {
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
#else
#ifdef PIC_VIEWER_SUPPORT
    MMIAPIPICVIEW_EnterOnlyPhotosForMMS(pre_win_id);
#endif
    MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_PIC);
#endif	
}

/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertNormalAttachment(MMI_WIN_ID_T pre_win_id)
{
    int32 file_max_size               = 0;
    MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL, 0, 0, 0, PNULL, 0, -1};

    file_max_size = MMIMMS_MMS_LIMIT_SIZE;

    select_info.max_size = file_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
    if (MMIAPIFMM_OpenSelectAllFileWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}

/*****************************************************************************/
//  Description :聊天处理现拍,现摄,现录流程
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSHandleSnapshot(MMIFILE_FILE_INFO_T *file_info, MMIFMM_FILE_TYPE_E   file_type)
{
    MMI_STRING_T      str_ptr = {0};
    MMI_STRING_T      str_to = {0};
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T mms_content_str = {0};
    uint16  num_len = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
    mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
    if (PNULL == mms_content_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"mmisms alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852, (uint8*)"");
        return;
    }
    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
    str_to.wstr_ptr = wnumber;
    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif       
    if (MMIAPIMMS_EditAddMMSAttach(file_info, file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
    }

    if (PNULL != mms_content_str.wstr_ptr)
    {
        SCI_FREE(mms_content_str.wstr_ptr);
        mms_content_str.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDC(void)
{
    MMIDC_WORK_PARAM_T work_param = {0};
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR;

    work_param.work_mode = MMIDC_WORK_DC;
    //work_param.param.dc.max_photo_size = WORK_PHOTO_SIZE_640_480; //use default value
    work_param.work_callback = SMSDCWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param);
    if (MMIDC_WORK_SUCCESS != error_ret)
    {
        SCI_TRACE_LOW("sms dc error %d", error_ret);
    }

}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenRecord(void)
{
    MMIRECORD_WORK_PARAM_T parameter = {0};

    parameter.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT;
    parameter.max_file_time = NULL;
    parameter.record_support_type = MMIRECORD_WORK_FORMAT_WAV;
    parameter.work_callback = SMSRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) == MMIRECORD_WORK_SUCCESS)
    {
        SCI_TRACE_LOW("[MMIMMS]MMSOpenRecord mms record error!");
    }
}

/*****************************************************************************/
//  Description :录音 后 的 处 理
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void SMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    MMIFILE_FILE_INFO_T file_info = {0};

    if (PNULL == data_info_ptr)
    {
        return;
    }

    if (MMIRECORD_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, STR_MSG_ATTACHMENT_BIG_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_MUSIC);
            }
        }
    }

    return ;
}
#endif

/*****************************************************************************/
//  Description :拍 照 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (PNULL == data_info_ptr)
    {
        return ret;
    }
    
    if (MMIDC_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
#if 0			
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;
                GUIANIM_TYPE_E pic_type = GUIANIM_TYPE_NONE;
                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);

                reduce_pic_size = reduce_width * reduce_height * 2;

                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (GUIANIM_TYPE_IMAGE_JPG == pic_type)
                {
                    win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        return ret;
                    }

                    SCI_MEMCPY((char *)win_data_ptr->sms_chat_insert_info.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC, IMAGE_PUBWIN_QUERY, &quer_win_id, PNULL);
                }
                else
#endif				
                {
                    MMIPUB_OpenAlertWarningWin(STR_MSG_ATTACHMENT_BIG_EXT01);
                }
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_PICTURE);
            }
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDV(void)
{
    MMIDC_WORK_PARAM_T work_param = {0};
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR;

    work_param.work_mode = MMIDC_WORK_DV;
    work_param.param.dv.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT;
    work_param.param.dv.max_video_size = WORK_VIDEO_SIZE_160_128;
    work_param.work_callback = SMSDVWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param);
    if (MMIDC_WORK_SUCCESS != error_ret)
    {
        SCI_TRACE_LOW("sms dv error %d", error_ret);
    }

}

/*****************************************************************************/
//  Description :录 像 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0;
    MMIFILE_FILE_INFO_T file_info = {0};

    if (PNULL == data_info_ptr)
    {
        return ret;
    }

    if (MMIDC_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(STR_UC_UNSUPPORT_CONTENT_BY_SERVER_ID, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, STR_MSG_ATTACHMENT_BIG_EXT01, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_MOVIE);
            }
        }
    }
    return ret;
}

#endif

/*****************************************************************************/
//  Description : 处理阅读界面来一条相同号码的情况
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ChatWinSetLongSmsItemContent(
                                               MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               )
{
    int32 i = 0;
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    uint16 pos = 0;
    uint16 prompt_len = 0;

    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;

    uint16 split_pos_before_len = 0;
    uint16 split_pos_after_len = 0;
    uint16 split_pos_after_pos = 0;
    
    if (PNULL == order_id || PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms param error");
        return FALSE ;
    }
     
    chat_item = &win_data_ptr->chat_item[index];

    //SCI_TRACE_LOW:"MMISMS_SetChatLongSmsItemContet:seq_len is %d,ref_num is %d,max is %d,seq is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_5245_112_2_18_2_53_0_177,(uint8*)"dddd",chat_item->split_sms_length[order_id->longsms_seq_num],order_id->longsms_max_num,order_id->longsms_seq_num);
    max_num = order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    seq_num = order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
    seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num; 
            
    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    SCI_TRACE_LOW("ChatWinSetLongSmsItemContent:orig_content.wstr_len is %d",orig_content.wstr_len);
    SCI_TRACE_LOW("ChatWinSetLongSmsItemContent:split[0] is %d,split[1] is %d,split[2] is %d,split[3] is %d",chat_item->split_sms_length[0],chat_item->split_sms_length[1],chat_item->split_sms_length[2],chat_item->split_sms_length[3]);
    if(orig_content.wstr_len == 0)  //当前长短信第一个到达的信息
    {                    
        for(i = 0; i < max_num; i++)
        {
            if(i + 1 == seq_num)
            {               
                MMIAPICOM_Wstrncpy(
                &string_ptr->wstr_ptr[pos],
                new_insert_content.wstr_ptr,
                new_insert_content.wstr_len
                );
                pos += new_insert_content.wstr_len;                         
                chat_item->split_sms_length[i] = new_insert_content.wstr_len;
                chat_item->split_sms_enable[i] = TRUE;
            }
            else
            {
                if (MN_SMS_UCS2_ALPHABET != win_data_ptr->read_result.read_content.alphabet
                && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
                {
                    uint8 temp_prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0};
                    uint16 default_len = 0;
                    
                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);
                    
                    MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_len);
                    
                    prompt_len = sms_lost_promt.wstr_len;
                    
                    default_len = MMIAPICOM_Default2Wchar(temp_prompt_str, &string_ptr->wstr_ptr[pos], sms_lost_promt.wstr_len);
                }
                else
                {
                    MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_ptr,
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_len);                
                }
                pos += sms_lost_promt.wstr_len;  
                chat_item->split_sms_length[i] = sms_lost_promt.wstr_len;
                chat_item->split_sms_enable[i] = FALSE;
            }
        } 
    }
    else
    {
        for(i = 0; i < seq_num - 1; i++)
        {
            split_pos_before_len += chat_item->split_sms_length[i];  
        }  
    
        split_pos_after_pos = split_pos_before_len + sms_lost_promt.wstr_len;            
        
        for(i = seq_num; i < max_num; i++)
        {
            split_pos_after_len += chat_item->split_sms_length[i];  
        } 
        
        if(split_pos_before_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            split_pos_before_len,
            orig_content.wstr_ptr,
            split_pos_before_len,
            split_pos_before_len);
            pos += split_pos_before_len;
        }
        
        MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
        new_insert_content.wstr_len,
        new_insert_content.wstr_ptr,
        new_insert_content.wstr_len,
        new_insert_content.wstr_len);
        pos += new_insert_content.wstr_len;
        chat_item->split_sms_length[seq_num - 1] = new_insert_content.wstr_len;
        
        if(split_pos_after_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            orig_content.wstr_len,
            orig_content.wstr_ptr + split_pos_after_pos,
            split_pos_after_len,
            split_pos_after_len);
            pos += split_pos_after_len;        
        }
    }
    
    string_ptr->wstr_len = pos;
    return TRUE;
}


/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void ChatWinCalcChatItemSplitLenth(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,uint32  index)
{
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;
    uint16 i = 0;
    MMISMS_ORDER_ID_T         cur_order_id = PNULL;
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return ;
    }

    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    chat_item = &win_data_ptr->chat_item[index];
    cur_order_id = MMISMS_GetOperationOrderId(0);
    if(cur_order_id != PNULL)
	{
        max_num = cur_order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
	}
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    
    while( i < MMISMS_SPLIT_MAX_NUM )
    {
        cur_order_id = MMISMS_GetOperationOrderId(i);
        if(cur_order_id != PNULL)
        {
            seq_num = cur_order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
            seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num;         
            index = seq_num - 1;
            chat_item->split_sms_length[index] = win_data_ptr->read_result.read_content.split_sms_length[index];
            chat_item->split_sms_enable[i] = TRUE;             
        }
        i++;  
    }
    for(i = 0; i < max_num; i++)
    {   
        if(0 == chat_item->split_sms_length[i])
        {
            chat_item->split_sms_length[i] = sms_lost_promt.wstr_len; 
            chat_item->split_sms_enable[i] = FALSE;
        }        
    }    
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    CC_RESULT_E cc_result = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if(PNULL == cur_order_id)
    {
        return;
    }
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    number_ptr = SCI_ALLOCAZ(tag_info.str_len + 1);
    if (PNULL == number_ptr)
    {
        return;
    }
    MMISMS_GetPhoneNumber(number_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len); //对-连接的号码处理
  
    switch (menu_id)
    {
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
        case MMISMS_MENU_IPDIAL_ITEM_ID:
            num_len = SCI_STRLEN((char *)number_ptr);
            if (MMISMS_MENU_DIAL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_NORMAL_CALL,
                                PNULL
                            );
            }
            else if (MMISMS_MENU_VIDEOCALL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_VIDEO_CALL,
                                PNULL
                            );
            }
            else
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_IP_CALL,
                                PNULL
                            );
            }
            break;
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMIAPISMS_AnswerMessage((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, PNULL, number_ptr,
                                    (uint8)(strlen((char *)number_ptr) > MMIMMS_MAX_ADDRESS_LEN ? MMIMMS_MAX_ADDRESS_LEN : strlen((char *)number_ptr)));
            break;
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
        {
            num_len = strlen((char *)number_ptr);
            //answer mms
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)number_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;

        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        }
        break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            //  MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
        }
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:
            HandleChatAddSenderToBlackList((uint8 *)number_ptr);
            break;
#endif

        default :
            break;
    }    
    SCI_FREE(number_ptr);
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
)
{
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};

    MMIAPICOM_GenPartyNumber(num_array_ptr, (uint16)SCI_STRLEN((char *)num_array_ptr), &party_num);
    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY(
        pb_bcd.number,
        party_num.bcd_num,
        pb_bcd.number_len
    );
    MMISMS_AddSenderToBlackList(pb_bcd);
}
#endif


/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    
}

/*****************************************************************************/
//     Description : handle extract number save contact
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberSaveContact(GUITEXT_TAG_INFO_T tag_info)
{
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;

    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    number_ptr = SCI_ALLOCAZ(tag_info.str_len + 1);
    if (PNULL == number_ptr)
    {
        return;
    }
    MMISMS_GetPhoneNumber(number_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len); //对-连接的号码处理
    // save to phonebook
    MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY(
        pb_bcd.number,
        party_num.bcd_num,
        pb_bcd.number_len
    );
    MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);  
	SCI_FREE(number_ptr);
	number_ptr = PNULL;
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlGotoBrowser(GUITEXT_TAG_INFO_T tag_info)
{
    wchar  *url_winparam_ptr              = PNULL;
    uint8 *url_ptr = PNULL;
    uint16 str_len = 0;
    
    url_winparam_ptr = SCI_ALLOCAZ((tag_info.str_len + 1) * sizeof(wchar));
    if( PNULL == url_winparam_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     
    MMIAPICOM_Wstrncpy(url_winparam_ptr, tag_info.str_ptr, tag_info.str_len);  
	str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));
    SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));

#ifdef BROWSER_SUPPORT
    {
        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

        entry_param.type = MMIBROWSER_ACCESS_URL;
        entry_param.dual_sys = MN_DUAL_SYS_MAX;
        entry_param.url_ptr = (char *)url_ptr;
        entry_param.user_agent_ptr = PNULL;
#ifdef  CMCC_UI_STYLE
        entry_param.is_alert = TRUE;
#endif                
        MMIAPIBROWSER_Entry(&entry_param);
    }
#endif

    SCI_FREE(url_ptr);
    url_ptr = PNULL;          
}


/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    wchar  *url_winparam_ptr              = PNULL;
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
#endif
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if(PNULL == cur_order_id)
    {
        return;
    }
        
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    url_winparam_ptr = SCI_ALLOCAZ((tag_info.str_len + 1) * sizeof(wchar));
    if( PNULL == url_winparam_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     
    MMIAPICOM_Wstrncpy(url_winparam_ptr, tag_info.str_ptr, tag_info.str_len);

    switch (menu_id)
    {
        case MMISMS_MENU_EXACT_URL_BROWSE_ITEM_ID:
        {
          
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));
    
            SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    
    #ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
#ifdef  CMCC_UI_STYLE
                entry_param.is_alert = TRUE;
#endif                
                MMIAPIBROWSER_Entry(&entry_param);
            }
    #endif
    
            SCI_FREE(url_ptr);
            url_ptr = PNULL;          
        }
        break;
        case MMISMS_MENU_EXTRACT_URL_ADDTOBOOKBARK_ITEM_ID:
        {
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
            if ( PNULL == url_ptr)
            {
                break;
            }
    
            //SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    #ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);
            }
    #endif
            SCI_FREE(url_ptr);
            url_ptr = PNULL;
        }
        break;
    
    #ifndef PB_SUPPORT_LOW_MEMORY
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                MMIAPIPB_AddContactWin(&pb_url, PB_GROUP_ALL, MMIPB_ADD_TYPE_URL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_url,MMIPB_ADD_TYPE_URL);
                MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_URL, PNULL);
            }
        }
        break;
    #endif
    #endif
    
        default :
            break;
    }
    
    SCI_FREE(url_winparam_ptr);     
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    uint8 *email_ptr = PNULL;
    MMIPB_MAIL_T pb_email = {0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    if (PNULL == cur_order_id )
    {
        return;
    }
    //进入文件管理器界面选择文件

    email_ptr = SCI_ALLOCAZ((tag_info.str_len + 1) * sizeof(wchar));
    if( PNULL == email_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     

    MMI_WSTRNTOSTR((uint8 *)email_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len, tag_info.str_len);
    
    switch (menu_id)
    {
        case MMISMS_MENU_EXTRACT_SEND_MMS_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;
    
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                MMIAPIPB_AddContactWin(&pb_email, PB_GROUP_ALL, MMIPB_ADD_TYPE_EMAIL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_email,MMIPB_ADD_TYPE_EMAIL);
                MMIAPIPB_AddToExistContact(&pb_email, MMIPB_ADD_TYPE_EMAIL, PNULL);
            }
        }
        break;
    #endif
        default :
            break;
    }
    
    SCI_FREE(email_ptr);    
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenInsertOpt(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    menuparam.group_id = MENU_SMS_CHATEDIT_INSERT_SELECT;
    menuparam.win_id =   MMISMS_SHOW_CHAT_INSERT_OPT_WIN_ID;
    menuparam.ctrl_id =  MMISMS_CHAT_INSERT_OPT_CTRL_ID;    
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleChatWinInsertOptOK;                    
    MMIPUB_OpenPubMenuWin( &menuparam ); 
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_CHAT_INSERT_TYPE MMISMS_GetChatEditInsertOpType(void)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return MMISMS_INSERT_INIT;
    }
    else
    {
        return win_data_ptr->sms_chat_insert_info.insert_type;
    }    
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatEditInsertOpType(MMISMS_CHAT_INSERT_TYPE insert_type)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return;
    }
    win_data_ptr->sms_chat_insert_info.insert_type = insert_type;
}
#endif

/*****************************************************************************/
//     Description : get chat win wstring number,such as :10086, email@163.com
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetChatWinWNumber(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,wchar *wnumber)
{
    uint16 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    if(PNULL == win_data_ptr || PNULL == wnumber)
    {
        return 0;
    }
    if(!win_data_ptr->is_email_number)
    {
        MMIAPICOM_GenDispNumber(
                      MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                      MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                      win_data_ptr->chat_number.number,
                      number,
                      MMISMS_PBNUM_MAX_LEN + 2
                  );
        MMIAPICOM_StrToWstr(number, wnumber);
    }
    else
    {
        MMIAPICOM_StrToWstr(win_data_ptr->sender, wnumber);    
    }
    num_len = (uint16)MMIAPICOM_Wstrlen(wnumber);
    return num_len;                    
} 

/*****************************************************************************/
//     Description : delete draft sms call back
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteOneDraftSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
	if (MMK_IsOpenWin(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID))
	{
		MMK_SendMsg(MMISMS_CHATBOX_BLOCK_CHILD_WIN_ID, MSG_GET_FOCUS, PNULL);
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : MMISMS_GetIsNeedUpdateList
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetIsNeedUpdateList(void)
{
    return s_is_need_update_list;
}

/*****************************************************************************/
//  Description : MMISMS_SetIsNeedUpdateList
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetIsNeedUpdateList(BOOLEAN is_need)
{
    s_is_need_update_list = is_need;
}
#endif
