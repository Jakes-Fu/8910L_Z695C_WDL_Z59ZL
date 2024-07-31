/****************************************************************************
** File Name:      mmipb_menu.c                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file defines all option menus, management and operation of these menus *
**                 定义了电话本的所有的选项菜单，这些菜单的管理和操作
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/

#define _MMIPB_MENU_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmk_type.h"
#include "mmipb_menu.h"
#include "mmimms_export.h"
#include "mmicc_export.h"
#include "window_parse.h"
#include "mmipb_id.h"
#include "mmipb_image.h"
#include "mmipb_text.h"
#include "mmicc_text.h"
#include "mmisms_text.h"
#include "guiform.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"
#include "mmisms_id.h"
#include "mmivcard_export.h"
#include "mmipb_app.h"
#include "mmipb_view.h"
#include "mmibt_text.h"
#include "mmitheme_pos.h"
#include "mmicom_banner.h"
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#if defined(DRM_SUPPORT)
#include "mmidrm_export.h"
#endif
#include "mmimms_export.h"
#include "mmipim_text.h"
#include "guibutton.h"
#include "mmiudisk_export.h"
#include "mmiset_text.h"
#include "mmieng_text.h"
#include "mmiidle_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiocr_export.h"
#include "guires.h"
#include "mmivcard_export.h"
#include "mmipb_set.h"
#include "mmifdn_export.h"
#ifdef MMIDC_F_WORK_MODE
#include "mmidc_export.h"
#endif
#ifdef MMI_PIC_CROP_SUPPORT
#include "mmipic_crop_export.h"
#endif
#include "mmipb_task.h"
#include "mmi_resource.h"
#include "mmipb_interface.h"
#include "mmipb_search.h"
#include "mmipb_storage.h"
#include "mmipb_set.h"
#include "mmisms_text.h"
#include "guiurl.h"
//#include "guianim_internal.h"
#ifdef TOUCH_PANEL_SUPPORT 
#include "guictrl_api.h"
#endif
#ifndef WIN32
#include "device_info.h"
#endif
#ifdef MMI_BT_PBAP_SUPPORT
#include "mmibt_export.h"
#include "mmibt_id.h"
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT
static MMI_WIN_ID_T s_hero_pb_addcontact_win_id = 0;
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


#define IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET    (GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC |  GUIIM_TYPE_DIGITAL)

#define  MMIPB_POP_MENU_NODE_ID 0
#define  MMIPB_POP_MENU_TEXT_ID 1
#define  MMIPB_POP_MENU_ENABLE_FUN 2
#define  MMIPB_POP_MENU_ITEM_DIMEN 3 

#define MMIPB_DEFAULT_PHOTO_WIDTH  48
#define MMIPB_DEFAULT_PHOTO_HEIGHT 48

//#define MMIPB_PHOTO_ANIM_OFFSET 10

#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_320X480)
#define MMIPB_EDIT_ITEM_WIDTH 91
#define MMIPB_EDIT_LABEL_WIDTH 30
#else
#define MMIPB_EDIT_ITEM_WIDTH 100
#define MMIPB_EDIT_LABEL_WIDTH 100
#define MMIPB_NUM_DEL_LABEL_WIDTH 85
#endif
#define MMIPB_LABEL_MARGIN    8

#if (defined MAINLCD_SIZE_320X480)
#define MMIPB_SMALL_FONT (MMI_DEFAULT_NORMAL_FONT-2)
#elif (defined MAINLCD_SIZE_240X400)
#define MMIPB_SMALL_FONT MMI_DEFAULT_SMALL_FONT
#else
#define MMIPB_SMALL_FONT (MMI_DEFAULT_SMALL_FONT-2)
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL VARIABLES DECLARE                           */
/*---------------------------------------------------------------------------*/
GUI_RECT_T s_pb_photo_anim_rect = {0}; 
//wchar* s_default_birthdate=L"20110101";

LOCAL const uint32 s_pb_storage_text[MMIPB_STORAGE_MAX]= 
{
    TXT_NULL,
    TXT_PHONE,
    TXT_PB_SET_SNS_ADD,
    TXT_SIM_SEL_SIM1,
    TXT_SIM_SEL_SIM2,
   TXT_SIM_SEL_SIM3,
    TXT_SIM_SEL_SIM4,   

};
  
#ifndef MMI_PDA_SUPPORT    
LOCAL const MMI_TEXT_ID_T s_listbox_title_default_str[MMIPB_LISTBOX_INDEX_MAX] =
{
    TXT_NAME,
//num        
    TXT_PB_ITEM_DETAIL_MOBILE_PHONE,
    TXT_PB_ITEM_DETAIL_HOME_PHONE,
    TXT_PB_ITEM_DETAIL_OFFICE_PHONE,
    TXT_PB_FAX_NUMBER,
    TXT_PB_PHONE_NUMBER,
#ifdef MMIPB_MAIL_SUPPORT
    TXT_PB_ITEM_DETAIL_EMAIL,  //mail
#endif
#ifdef SNS_SUPPORT
    TXT_COMMON_SIGNATURE,     //SIGN
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    TXT_COMM_QQ,                    //im
#endif
#ifdef CMCC_VCARD_SUPPORT 
	TXT_PB_ITEM_DETAIL_ALIAS,//alias
#endif
#ifdef  ASP_SUPPORT
    TXT_COMM_ANSWER,
#endif
    TXT_PB_MODIFY_PHOTO,        //photo
    TXT_PB_RING_DEFAULT,         //ring
#ifndef PB_SUPPORT_LOW_MEMORY
    TXT_PB_URL,                         //url
    TXT_PB_BIRTHDAY,                //birth
#endif
    TXT_PB_ITEM_DETAIL_CUSTOM_SELECT_GROUP, //group
#ifdef CMCC_UI_STYLE
    TXT_PB_ITEM_ADRRESS,
#else
#ifndef PB_SUPPORT_LOW_MEMORY
    TXT_MEMO,
#endif
#endif
    TXT_PB_ITEM_DETAIL_ORG,
    TXT_PB_ITEM_DETAIL_COMPANY,
    TXT_PB_ITEM_ADRRESS
};
#endif 

LOCAL const MMI_CTRL_ID_T  s_num_form_id[] = {	
        MMIPB_FORM_NUM1_ID,
        MMIPB_FORM_NUM2_ID
    #ifndef PB_SUPPORT_LOW_MEMORY													
        ,
        MMIPB_FORM_NUM3_ID,
        MMIPB_FORM_NUM4_ID,
        MMIPB_FORM_NUM5_ID
    #endif													
	};
LOCAL const MMI_CTRL_ID_T s_num_sub_form_id[] = {	
        MMIPB_FORM_NUM1_SUB_ID,
        MMIPB_FORM_NUM2_SUB_ID
    #ifndef PB_SUPPORT_LOW_MEMORY													
        ,
        MMIPB_FORM_NUM3_SUB_ID,
        MMIPB_FORM_NUM4_SUB_ID,
        MMIPB_FORM_NUM5_SUB_ID
        #endif
        };
 LOCAL const MMI_CTRL_ID_T s_num_label_form_id[] = {	
        MMIPB_NUM1_LABEL_FORM_ID,
        MMIPB_NUM2_LABEL_FORM_ID
    #ifndef PB_SUPPORT_LOW_MEMORY													
        ,
        MMIPB_NUM3_LABEL_FORM_ID,
        MMIPB_NUM4_LABEL_FORM_ID,
        MMIPB_NUM5_LABEL_FORM_ID
        #endif
        };
 
    LOCAL const MMI_CTRL_ID_T  s_num_edit_id[] = {
        MMIPB_EDITBOX_NUM1_ID,
        MMIPB_EDITBOX_NUM2_ID
    #ifndef PB_SUPPORT_LOW_MEMORY																										
        ,
        MMIPB_EDITBOX_NUM3_ID,
        MMIPB_EDITBOX_NUM4_ID,
        MMIPB_EDITBOX_NUM5_ID
    #endif													
        };
LOCAL MMI_CTRL_ID_T  s_num_delete_id[] = {
        MMIPB_NUM1_DEL_BUTTON_ID,
        MMIPB_NUM2_DEL_BUTTON_ID
    #ifndef PB_SUPPORT_LOW_MEMORY													
        ,
        MMIPB_NUM3_DEL_BUTTON_ID,
        MMIPB_NUM4_DEL_BUTTON_ID,
        MMIPB_NUM5_DEL_BUTTON_ID
    #endif													
        };

    LOCAL const MMI_CTRL_ID_T   s_num_type_label_id[] = {	
        MMIPB_NUM1_LABEL_ID,
        MMIPB_NUM2_LABEL_ID
        #ifndef PB_SUPPORT_LOW_MEMORY													
        ,
        MMIPB_NUM3_LABEL_ID,
        MMIPB_NUM4_LABEL_ID,
        MMIPB_NUM5_LABEL_ID
        #endif													
        };
   
LOCAL const MMI_CTRL_ID_T  s_contact_eidt_form_id[] = {	
        MMIPB_EDITBOX_NAME_ID,
        MMIPB_EDITBOX_NUM1_ID,
        MMIPB_EDITBOX_NUM2_ID,
#ifndef PB_SUPPORT_LOW_MEMORY			
        MMIPB_EDITBOX_NUM3_ID,
        MMIPB_EDITBOX_NUM4_ID,
        MMIPB_EDITBOX_NUM5_ID,
#endif
#ifdef MMIPB_MAIL_SUPPORT
        MMIPB_EDITBOX_EMAIL_ID,
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
        MMIPB_IM_EDIT_ID,
#endif
#ifndef PB_SUPPORT_LOW_MEMORY  
        MMIPB_URL_EDITBOX_ID,
        MMIPB_EDITBOX_MEMO_ID	
#endif        
};

LOCAL const MMI_TEXT_ID_T s_sim_status_string[SIM_STATUS_MAX + 1] = 
{
    TXT_NULL,                       //SIM_STATUS_OK,
    TXT_SIM_REJECTED,               //SIM_STATUS_REJECTED,
#ifndef MMI_IDLE_MINI_SUPPORT
    TXT_SIM_REGISTRATION_FAILED,//SIM_STATUS_REGISTRATION_FAILED,
#else
    TXT_SIM_REGISTRATION_FAILED_MINI,//SIM_STATUS_REGISTRATION_FAILED,
#endif

    STR_PIN_BLOCKED_EXT01,                //SIM_STATUS_PIN_BLOCKED
    TXT_NOT_SUPPORT_SIM,            //SIM_STATUS_NOT_SUPPORT_SIM,                                                            
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK_BLOCKED,
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK2_BLOCKED,
    STR_SIM_NOT_SIM_EXT01,                     //SIM_STATUS_NO_SIM,
    STR_SIM_SEARCHING_NETWORK,  //SIM_NOT_INITED
#if defined(MMI_SIM_LOCK_TYPE2_SUPPORT) && !defined(MMI_SIM_LOCK_SUPPORT)
	TXT_SIM_LOCKED_INVALID, //SIM_STATUS_LOCKED_INVALID
#endif
    TXT_NULL                        //SIM_STATUS_MAX
};

extern uint8 g_ringtone_max;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :get menu style menu type
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL GUIMENU_STYLE_E GetMenuStyle(MMIPB_MENU_TYP_E menu_type);

/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStorageSelectWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifdef MMIPB_SIMCONTACT_SUPPORT
LOCAL MMI_RESULT_E HandleStorageSelectWinMsgList(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddSubWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactAddEditWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : 新增联系人窗口消息处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactAddWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : free menu infor contact,photo,ring and asp memory
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/                                              
LOCAL void FreeContactPhotoRingInfor(MMIPB_MENU_DATA_INFOR_T * add_data_ptr);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : save user input infor
//  Global resource dependence :
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void GetUserEditContact(MMI_HANDLE_T win_handle, //IN
						   MMIPB_CONTACT_T *contact_ptr//OUT
						   );
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set photo anim ctrl height
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetPhotoFormHeight(
    MMI_HANDLE_T win_id,
    MMI_HANDLE_T form_handle,
    MMI_HANDLE_T child_handle,
    MMI_IMAGE_ID_T img_id
    );
#endif
/*****************************************************************************/
//  Description : compound all the element into a contact item and add to nv or sim
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AddContact(
 MMIPB_MENU_DATA_INFOR_T *data_ptr
    );

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : send vcard by sms or bt and so pn
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void SendVcard(MMIPB_HANDLE_T applet_handle, 
    MMIPB_SEND_TYPE_E send_type,
    MMIPB_CONTACT_T *contact_ptr
    );
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 初始化新增或者编辑界面的信息
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void InitContactEditItems(MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T        ctrl_id,    //IN:
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr,  //IN:
    MMI_TEXT_ID_T           left_softkey_id    
);
/*****************************************************************************/
//  Description : 将电话簿记录中的号码设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetContactAddOrEditItem(MMI_CTRL_ID_T ctrl_id,
            MMIPB_MENU_DATA_INFOR_T * add_data_ptr,               
            uint32                     item_id,
            uint32                     item_index,
            MMI_TEXT_ID_T       left_softkey_id,
            MMI_TEXT_ID_T       middle_softkey_id,
            MMI_TEXT_ID_T       right_softkey_id,
            BOOLEAN is_update);
/*****************************************************************************/
//  Description : 更新新增或者编辑界面的信息
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateContactAddOrEditItems(MMI_CTRL_ID_T ctrl_id,
            MMIPB_MENU_DATA_INFOR_T * add_data_ptr,
            void*param_ptr);
            
/*****************************************************************************/
//  Description : 在手机中添加时打开相应的输入窗口
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBox(MMI_CTRL_ID_T ctrl_id,MMIPB_MENU_DATA_INFOR_T *add_data_ptr);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set sub form border
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SetSubFormBorder(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 设置添加到手机时的LISTBOX各控件的值
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void InitContactEditorCtrl(MMI_WIN_ID_T win_id,MMIPB_MENU_DATA_INFOR_T *data_ptr);
/*****************************************************************************/
//  Description : add or delete editbox by delete button
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateContactEditNumber(
    MMI_HANDLE_T win_id,	
    uint16 delete_index,
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
    );
#ifndef PB_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : handle Set birthday pub win msg
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBirthdayWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : add new editbox of contact number auto
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateNumberEditCtrl(MMI_WIN_ID_T win_id,MMIPB_CONTACT_T *contact_ptr);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create one number item(include button and editbox)
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateContactNumberItem(MMI_WIN_ID_T win_id,MMIPB_CONTACT_T *contact_ptr,
    uint16 index);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append label
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormLabel(MMI_HANDLE_T win_id,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T label_id,
						   uint16   percent,
						   MMI_STRING_T *text_ptr,
						   BOOLEAN       is_get_active
						 );
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendFormChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id
                           );
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得contact number count
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetContactNumberNum(MMIPB_CONTACT_T *contact_ptr,uint16 *index_array);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得需新增的editbox对应的号码index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetNextAddNumberIndex(uint16 index, MMIPB_CONTACT_T *contact_ptr);
#endif
/*****************************************************************************/
//  Description : add image id for contact
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SelectPhotoFromFile(MMIPB_MENU_DATA_INFOR_T*  data_ptr);

#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : add asp id
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddASPID(
    MMI_HANDLE_T add_win_handle, //IN:
    MMIPB_CONTACT_T *contact_ptr,  //IN:
    MMIPB_CUSTOM_RING_T*  asp_ptr//IN
    );
/*****************************************************************************/
//  Description : select asp id call back
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPSelectCallback(
    BOOLEAN is_fixed_id,
    uint32  fixed_id,
    wchar   *asp_file_name
    );
#endif

#if 0
/*****************************************************************************/
//  Description :get phone type and number string
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPhoneNumberString(uint16 num_index,
    wchar *wstr_ptr,
    MMIPB_CONTACT_T *contact_ptr);
#endif

/*****************************************************************************/
//  Description : deal member win option menu
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void HandleOptMenuAction(MMI_WIN_ID_T win_id,
    PB_OPTMENU_NODE_E node_id,
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
);
/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePbOptMenuWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendDesignateTypeFormChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id,
                           GUIFORM_LAYOUT_E layout_type
                           );

/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendFormSubChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id,
                           uint16 percent
                           );
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteGroupEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupNameEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupRingEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GroupsMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :add one contact menu whether enable
//  Global resource dependence : none
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :delete contact menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteMultiMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShareMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : mark all menu whether should disable
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MarkAllMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description : unmark all menu whether should disable
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CancelAllMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description :delete contact menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : Clear MostUsedContact
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ClearMostUsedMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
/*****************************************************************************/
//  Description :import from sim1 menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyFromSimEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :modify one contact menu whether enable
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ModifyMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : exist sim
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SimMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Sim1MenuEnable
(
 MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
 PB_OPTMENU_NODE_E menu_node
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Sim2MenuEnable
(
 MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
 PB_OPTMENU_NODE_E menu_node
 );

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : select sim or phone enable
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelStorageMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
/*****************************************************************************/
//  Description : open add win table
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenAddWin(MMIPB_MENU_DATA_INFOR_T *add_data_ptr, uint16 storage_id);
/*****************************************************************************/
//  Description : ContactAdd, Init Add Data
//  Global resource dependence : none
//  Author: Starmer.Sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContactAddInitAddData(MMIPB_MENU_DATA_INFOR_T *add_data_ptr);

/*****************************************************************************/
//  Description :通过存储id计算StorageId
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
//LOCAL uint16 CalStorageIdByGroupId(uint8 group_id);
/*****************************************************************************/
//  Description :get index from s_pb_menu by node id
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetArrayIndexByNodeId(uint16 node_id);
/*****************************************************************************/
//  Description :创建win table的选项菜单
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePbOptMenu(MMI_WIN_ID_T win_id,
    MMI_HANDLE_T ctrl_id,
    MMIPB_MENU_DATA_INFOR_T *judge_infor_ptr,
    uint16 *menu_num_ptr
);

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : vcard parent menu whether enable
//  Global resource dependence : none
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImportExportMenuEnable(MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
									 PB_OPTMENU_NODE_E menu_node);
/*****************************************************************************/
//  Description : vcard menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcardMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description :vcard send by bt menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :vcard send by bt menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendMultiVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :send sms menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendGroupSMSMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
/*****************************************************************************/
//  Description :dial (parent) menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : contact view menu whether enable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContactViewMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : add to contact menu whether enable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToContactMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

LOCAL BOOLEAN SendBtVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description :audio dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByAudioMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description :video dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByVideoMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
/*****************************************************************************/
//  Description :ip dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByIpMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description : copy (parent) menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ClearDateEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
	);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendMessageMenuEnable(
                                    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
                                    PB_OPTMENU_NODE_E menu_node
                                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendEmailMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DebugEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DebugSimManagerEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : copy to nv menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyOneToNVEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description : copy to sim1 menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyToSIMEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description :dial parent menu whether enable
//  Global resource dependence : none
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WriteMessageMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description : copy one to sim menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyOneToSIMEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );


/*****************************************************************************/
//  Description :add to group menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToGroupMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : contact add to favorite menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToFavoriteEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );

/*****************************************************************************/
//  Description :remove favorite menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveFavoriteEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif

#ifdef MMIPB_GROUP_MANAGE_SUPPORT

/*****************************************************************************/
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddGroupMemberEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
/*****************************************************************************/
//  Description :remove group member menu whether enable
//  Global resource dependence : none
//  Author: starmer
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveGroupMemberEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    );
#endif
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : get send menu type by node id
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_SEND_TYPE_E GetSendTypeByNodeId(uint16 node_id);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :create EDITBOX
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormEditor(
                        MMI_STRING_T    *text_ptr,
                        uint16          max_ucs2_len,
                        uint16          max_alpha_len,
                        uint8           allow_inputmethod,
                        BOOLEAN         is_compressed,
                        MMI_CTRL_ID_T   edit_id,
                        MMI_HANDLE_T    form_handle,
                        MMIPB_ITEM_TYPE_E item_type,
                        MMI_WIN_ID_T    win_id
                        );
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : delete icon of listbox
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormButton(MMI_HANDLE_T win_handle,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T button_id,
						   BOOLEAN       is_get_active
						 );
#endif
#ifdef MMI_PIC_CROP_SUPPORT
/*****************************************************************************/
//  Description : crop picture file
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CropPhotoFile(wchar *full_path_ptr,uint16 full_path_len);
#endif

#ifdef MMI_VCARD_SUPPORT  
/*****************************************************************************/
//  Description : 选择导出的目录
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN SelectExportFolder(MMI_HANDLE_T applet_handle,
wchar *path_name,uint16 path_len);

/*****************************************************************************/
//  Description : 选择导出的目录 callback
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL void FolderSelectionCallBack(BOOLEAN is_success, FILEARRAY file_array);
#endif
/*****************************************************************************/
//  Description : enter select number win
//  Global resource dependence : 
//  Author:xiaohua.liu 
//  Note: 
/*****************************************************************************/
LOCAL void EnterSelectNumberWin(MMIPB_MENU_DATA_INFOR_T *data_ptr);
/*****************************************************************************/
//  Description : Set ring from FMM file
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void SetFileRing(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Discription: 提示不同的提示信息，让用户选择是否要进行该操作
//  Global resource dependence: None
//  Author: xiaohua.liu
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteQueryWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);                
/*****************************************************************************/
//  Description : get menu array by menu type
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL MMIPB_MENU_FAMILY_INFO_T* GetMenuArrAddress(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr);

#if 0
/*****************************************************************************/
//  Description : 删除号码询问窗口
//  Global resource dependence : g_mmipb_operate_type
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void OpenPbQueryWin(MMIPB_MENU_DATA_INFOR_T *data_ptr);
#endif

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : to handle favorite query window    
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRemoveFavoriteQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

/*****************************************************************************/
//  Description : to handle Mostused query window    
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClearMostUsedQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : to handle favorite query window    
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRemoveFromGroupQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
#endif
/*****************************************************************************/
//  Description : 加入群组窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToGroupWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : edit phone number type label 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNumberTypeWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
/*****************************************************************************/
//  Description : edit instace msg type label 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectImTypeWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
/*****************************************************************************/
//  Description : select number win
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNumberWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

/*****************************************************************************/
//  Description : set pb default Photo Image
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SetPbDefaultPhotoImage(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr);


/*****************************************************************************/
//  Description : set pb default ring
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetPbDefaultRing(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetPbNosoundRing(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr);

#ifdef MMIDC_F_WORK_MODE
/*****************************************************************************/
//  Description : set pb photo file by DC
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL int32 PhotoShotCallback( MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MSISDN and only one sim number should not delete
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsNeedNumberDelButton(uint16 storage_id);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get edit win item default txt label id
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T MMIPB_GetItemDefaultTxtID(MMIPB_ITEM_TYPE_E item_type);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : hide adrress,company,birth,url and so on item
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SwitchMoreItemState(MMI_HANDLE_T ctrl_handle,
    MMI_IMAGE_ID_T image_id,
    uint32 user_data);
#endif
/*****************************************************************************/
//  Description : set photo for pb contact
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SendPhotoSetMsgForPb(wchar *full_path_ptr);

/*****************************************************************************/
//  Description : set pb contact photo infor
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/                                              
LOCAL void SetPhotoAnimParam(MMI_HANDLE_T win_id,
    GUIANIM_DATA_INFO_T *data_infor_ptr,
    GUIANIM_FILE_INFO_T *file_infor_ptr);


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : hide adrress,company,birth,url and so on item
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void HideAddressBirthUrlItem(MMI_HANDLE_T win_id);
/*****************************************************************************/
//  Description : hide adrress,company,birth,url and so on item
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAddressBirthUrlItem(MMI_HANDLE_T win_id,MMIPB_CONTACT_T* contact_ptr);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create edit email item 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateEmailEditItem(MMI_HANDLE_T win_id,
    MMIPB_CONTACT_T *contact_ptr
    );
/*****************************************************************************/
//  Description : create edit ring item 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateRingEditItem(MMI_HANDLE_T win_id,
    MMIPB_CONTACT_T *contact_ptr,
    MMIPB_CUSTOM_RING_T *ring_ptr
);
#ifndef PB_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : create the pb edit win more swtich item
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePBEditMoreSwitchItem(MMI_HANDLE_T win_id,
    MMI_HANDLE_T form_handle,
    MMI_CTRL_ID_T child_form_id,
    MMI_CTRL_ID_T ctrl_id
);
/*****************************************************************************/
//  Description : create birthday item editbox
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateBirthdayItem(MMI_HANDLE_T win_id,MMIPB_CONTACT_T *contact_ptr);
#endif
/*****************************************************************************/
//  Description : update edit win
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdatePhotoRingLabelEdit(
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
						   );
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePbSelectionMenuWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

#ifndef PB_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : get birthday date from ctrl and set it
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBirthDate
(
  MMI_HANDLE_T win_id,
  MMI_CTRL_ID_T     ctrl_id,
  MMIPB_CONTACT_T *contact_ptr
);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectRingWinMsg
(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

WINDOW_TABLE( MMIPB_ADD_SUB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleAddSubWinMsg),    
    WIN_ID( MMIPB_ADD_SUB_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    
    END_WIN
};

WINDOW_TABLE( MMIPB_STORAGE_SELECT_WIN_TAB) = 
{
#ifndef MMIPB_SIMCONTACT_SUPPORT
    WIN_FUNC( (uint32)HandleStorageSelectWinMsg),    
    WIN_ID( MMIPB_STORAGE_SELECT_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_TITLE(TXT_NEW),
#ifndef MMI_PDA_SUPPORT
//#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_NULL),
//#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#endif
#else //MMIPB_SIMCONTACT_SUPPORT
    WIN_FUNC( (uint32)HandleStorageSelectWinMsgList),
    WIN_ID( MMIPB_STORAGE_SELECT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_STORAGE_SELECT_OPT_CTRL_ID),

#endif //MMIPB_SIMCONTACT_SUPPORT
    
    END_WIN
};

#if 0
// 添加的菜单窗口
WINDOW_TABLE( MMIPB_CONTACT_ADD_OR_EDIT_WIN_TAB ) = 
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleContactAddWinMsg ),
    WIN_ID( MMIPB_ADD_IN_NV_WIN_ID),
    WIN_TITLE(TXT_NULL),

     #ifdef MMI_PDA_SUPPORT
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_EDITBOX_FORM_ID),
        CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_SAVE_MK, TXT_PB_NEXT_STEP, STXT_CANCEL,MMIPB_ADD_ENTRY_SOFTKEY_CTRL_ID,MMIPB_EDITBOX_FORM_ID),
    #else
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPB_EDITBOX_FORM_ID),
        WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_RETURN), 
     #endif
    END_WIN
};
#else
#define MMIPB_PHONE_NUM_MAX_LEN 20 //MMISET_PHONE_NUM_MAX_LEN
// 添加的菜单窗口
WINDOW_TABLE( MMIPB_CONTACT_ADD_OR_EDIT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleContactAddEditWinMsg),
    WIN_ID( MMIPB_ADD_IN_NV_WIN_ID),
    WIN_TITLE( TXT_NULL ),    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#ifndef MMIPB_RMVITEM_SUPPORT
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_AVATAR_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_CONTACT_ADD_FIRST_NAME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_CONTACT_ADD_LAST_NAME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_MOBILE_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_HOME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#ifndef PB_SUPPORT_LOW_MEMORY    
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_BUSINESS_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif
#ifdef MMIPB_MAIL_SUPPORT     
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_MAX_MAIL_LEN,MMIPB_CONTACT_ADD_EMAIL_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif    
#ifndef PB_SUPPORT_LOW_MEMORY    
    CHILD_EDIT_DATE_CTRL(TRUE,MMIPB_CONTACT_ADD_BIRTHDAY_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif
#ifndef MMIPB_RMVITEM_SUPPORT
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_RING_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_GROUP_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_PREFERRED_SIM_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
#endif

    WIN_SOFTKEY(STXT_OPTION	, TXT_NULL, TXT_COMMON_COMMON_BACK), 
    END_WIN
};
WINDOW_TABLE( MMIPB_CONTACT_ADD_OR_EDIT_WIN_TAB2 ) = 
{
    WIN_FUNC( (uint32)HandleContactAddEditWinMsg),
    WIN_ID( MMIPB_ADD_IN_NV_WIN_ID),
    WIN_TITLE( TXT_NULL ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#ifndef MMIPB_RMVITEM_SUPPORT
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_AVATAR_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_CONTACT_ADD_LAST_NAME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_CONTACT_ADD_FIRST_NAME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_MOBILE_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_HOME_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#ifndef PB_SUPPORT_LOW_MEMORY    
    CHILD_EDIT_PHONENUM_CTRL(TRUE,MMIPB_PHONE_NUM_MAX_LEN,MMIPB_CONTACT_ADD_BUSINESS_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif
#ifdef MMIPB_MAIL_SUPPORT    
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_MAX_MAIL_LEN,MMIPB_CONTACT_ADD_EMAIL_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif    
#ifndef PB_SUPPORT_LOW_MEMORY    
    CHILD_EDIT_DATE_CTRL(TRUE,MMIPB_CONTACT_ADD_BIRTHDAY_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID),
#endif
#ifndef MMIPB_RMVITEM_SUPPORT
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_RING_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_GROUP_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_CONTACT_ADD_PREFERRED_SIM_ID,MMIPB_CONTACT_ADD_WIN_FORM_ID), 
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, TXT_COMMON_COMMON_BACK), 
    END_WIN
};
#endif

//pb member option menu
WINDOW_TABLE(MMIPB_OPT_MENU_WIN_TAB) = 
{
#ifdef MAINLCD_SIZE_128X160
    WIN_TITLE( TXT_NULL),
#endif
    WIN_FUNC( (uint32)HandlePbOptMenuWinMsg),
    WIN_ID(MMIPB_MEMBER_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


WINDOW_TABLE(MMIPB_OPT_SELECTION_MENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePbSelectionMenuWinMsg),
    WIN_ID(MMIPB_OPT_SELECTION_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


#if 0
WINDOW_TABLE( MMIPB_ADD_TO_GROUP_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_PB_ADD_TO_GROUP),
    WIN_FUNC((uint32)HandleAddToGroupWinMsg), 
    WIN_ID( MMIPB_ADD_TO_GROUP_WIN_ID),
    //CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIPB_ADD_TO_GROUP_LISTBOX_CTRL_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_ADD_TO_GROUP_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    END_WIN
};
#else
WINDOW_TABLE( MMIPB_ADD_TO_GROUP_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleAddToGroupWinMsg), 
    WIN_ID( MMIPB_ADD_TO_GROUP_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_ADD_TO_GROUP_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};
#endif

WINDOW_TABLE( MMIPB_SELECT_NUMBER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_NULL),
    WIN_FUNC((uint32)HandleSelectNumberWinMsg), 
    WIN_ID( MMIPB_SET_DEFAULT_NUMBER_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SELECT_NUMBER_LISTBOX_CTRL_ID ),
    #ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #endif
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMIPB_SELECT_NUMBER_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_PB_SELECT_NUMBER_TYPE_LABEL),
    WIN_FUNC((uint32)HandleSelectNumberTypeWinMsg), 
    WIN_ID( MMIPB_SELECT_NUMBER_TYPE_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_EDIT_NUMBER_TYPE_LISTBOX_CTRL_ID),
   
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),

    END_WIN
};
#endif
#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
WINDOW_TABLE( MMIPB_SELECT_IM_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_PB_SELECT_STYPE_LABEL),
    WIN_FUNC((uint32)HandleSelectImTypeWinMsg), 
    WIN_ID( MMIPB_SELECT_IM_TYPE_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_EDIT_IM_TYPE_LISTBOX_CTRL_ID),
   
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),

    END_WIN
};
#endif

#ifdef MMI_PDA_SUPPORT
#ifndef PB_SUPPORT_LOW_MEMORY
// the window for set birthday
WINDOW_TABLE( MMIPB_SET_BIRTHDAY_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetBirthdayWinMsg),    
    WIN_ID( MMIPB_SET_BIRTHDAY_WIN_ID ),
    WIN_TITLE( TXT_SET_DATE ),

        //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_SET_BIRTHDAY_FORM_ID),
    
    CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMIPB_SET_BIRTHDAY_DATEEDITBOX_CTRL_ID,MMIPB_SET_BIRTHDAY_FORM_ID),        

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#else
WINDOW_TABLE(MMIPB_NUMBER_EDITBOX_WIN_TAB) = 
{
    WIN_ID( MMIPB_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_NULL),    
    WIN_FUNC((uint32)HandleEditBoxWinMsg),   
    CREATE_EDIT_PHONENUM_CTRL(MMIPB_NAME_MAX_LEN+1,MMIPB_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIPB_NAME_EDITBOX_WIN_TAB) = 
{
    WIN_ID( MMIPB_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_NULL),    
    WIN_FUNC((uint32)HandleEditBoxWinMsg),   
    CREATE_EDIT_TEXT_CTRL(MMIPB_NAME_MAX_LEN + 1,MMIPB_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

WINDOW_TABLE(MMIPB_SELECT_RING_WIN_TAB) = 
{
   // WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSelectRingWinMsg),    
    WIN_ID( MMIPB_SELECT_RING_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMIPB_SELECT_RING_LIST_CTRL_ID),
    END_WIN
};


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
#if 0
LOCAL const MMIPB_MENU_ITEM_INFO_T s_pb_menu[]=
{
    //call
    {PB_OPTMENU_NODE_DIAL, TXT_CALL,DialMenuEnable},        
    {PB_OPTMENU_NODE_DIAL_AUDIO,TXT_CALL_AUDIO,DialByAudioMenuEnable},
    {PB_OPTMENU_NODE_DIAL_AUDIO_DETAIL,TXT_CALL_AUDIO,DialByAudioMenuEnable},
    #ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL_VIDEO, TXT_CALL_VIDEO,DialByVideoMenuEnable},
    {PB_OPTMENU_NODE_DIAL_VIDEO_DETAIL, TXT_CALL_VIDEO,DialByVideoMenuEnable},
    #endif
#ifdef MMI_IP_CALL_SUPPORT
    {PB_OPTMENU_NODE_DIAL_IP,TXT_CALL_IPCALL,DialByIpMenuEnable},
    {PB_OPTMENU_NODE_DIAL_IP_DETAIL,TXT_CALL_IPCALL,DialByIpMenuEnable},    
#endif
    {PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL,TXT_EDIT_NUMBER_BEFORE_DIAL,DialMenuEnable},
    {PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL_DETAIL,TXT_EDIT_NUMBER_BEFORE_DIAL,DialMenuEnable},

    //sms
    {PB_OPTMENU_NODE_WRITE_MESSAGE,TXT_COMMON_WRITE_MESSAGE,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,TXT_COMMON_WRITE_MESSAGE,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_SMS, TXT_SMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_SMS_DETAIL, TXT_SMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_MMS,TXT_MMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_MMS_DETAIL,TXT_MMS,WriteMessageMenuEnable},

    //
    {PB_OPTMENU_NODE_ADDNEW,TXT_NEW, AddMenuEnable},

    {PB_OPTMENU_NODE_VCARD_BT_EXPORT_CONTACT, TXT_PB_SEND_VCARD_BY_BT,SendBtVcardEnable},
#ifdef MMIPB_SYNC_WITH_O365
    {PB_OPTMENU_NODE_CONTACTSYNC,TXT_PB_CONTACTSYNC, AddMenuEnable},
#endif

    {PB_OPTMENU_NODE_MODIFY, TXT_EDIT,ModifyMenuEnable},
    {PB_OPTMENU_NODE_DELETE_ONE,TXT_DELETE,DeleteOneMenuEnable},
    {PB_OPTMENU_NODE_DELETE_MULTI,TXT_PB_ERASE_MULTI,DeleteMultiMenuEnable},
#ifdef MMIPB_MOST_USED_SUPPORT 
    {PB_OPTMENU_NODE_CLEAR_MOSTUSED,TXT_DELALL,ClearMostUsedMenuEnable},
#endif    
    //copy on to phone & sim
    {PB_OPTMENU_NODE_COPY,TXT_COMMON_COPY,CopyMenuEnable},
    {PB_OPTMENU_NODE_ONE_COPYTOPHONE,TXT_PDA_PB_COPY_FROM_NV_TO_PHONE,CopyOneToNVEnable},
    {PB_OPTMENU_NODE_ONE_COPYTOSIM1,TXT_PDA_PB_COPY_FROM_NV_TO_SIM1,CopyOneToSIMEnable},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_ONE_COPYTOSIM2,TXT_PDA_PB_COPY_FROM_NV_TO_SIM2,CopyOneToSIMEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
    {PB_OPTMENU_NODE_ONE_COPYTOSIM3, TXT_PDA_PB_COPY_FROM_NV_TO_SIM3,CopyOneToSIMEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_ONE_COPYTOSIM4,TXT_PDA_PB_COPY_FROM_NV_TO_SIM4,CopyOneToSIMEnable},
    #endif   
    
    //import & export
    {PB_OPTMENU_NODE_IMPORT_EXPORT,TXT_PB_IMPORT_EXPORT,ImportExportMenuEnable},    
    
    //copy from sim
    {PB_OPTMENU_NODE_COPY_FROM_SIM1,TXT_PB_IMPORT_SIM1,CopyFromSimEnable},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_COPY_FROM_SIM2,TXT_PB_IMPORT_SIM2,CopyFromSimEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_COPY_FROM_SIM3,TXT_PB_IMPORT_SIM3,CopyFromSimEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_COPY_FROM_SIM4,TXT_PB_IMPORT_SIM4,CopyFromSimEnable},
    #endif

    //copy to phone & sim
    {PB_OPTMENU_NODE_COPYTOPHONE,TXT_PDA_PB_COPY_FROM_NV_TO_PHONE, PNULL},
    {PB_OPTMENU_NODE_COPYTOSIM1,TXT_PDA_PB_COPY_FROM_NV_TO_SIM1,CopyToSIMEnable},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_COPYTOSIM2,TXT_PDA_PB_COPY_FROM_NV_TO_SIM2,CopyToSIMEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
    {PB_OPTMENU_NODE_COPYTOSIM3, TXT_PDA_PB_COPY_FROM_NV_TO_SIM3,CopyToSIMEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_COPYTOSIM4,TXT_PDA_PB_COPY_FROM_NV_TO_SIM4,CopyToSIMEnable},
    #endif   
    
    // vcard import & export
    #ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT,TXT_PB_IMPORT_CONTACT,VcardMenuEnable},
    {PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT,TXT_PB_EXPORT_CONTACT,VcardMenuEnable},

    // send vcard by SMS\MMS\Email\BT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, TXT_PB_SEND_VCARD,SendVcardEnable},    
    {PB_OPTMENU_NODE_SEND_BY_SMS_MENU, TXT_SMS,SendVcardEnable},
    {PB_OPTMENU_NODE_SEND_BY_MMS_MENU, TXT_MMS,SendVcardEnable},
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU, TXT_SEND_BY_EMAIL,SendVcardEnable},   
#endif       
    {PB_OPTMENU_NODE_SEND_BY_BT_MENU,  TXT_BLUETOOTH,SendVcardEnable},    
    #endif

    //group 
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    {PB_OPTMENU_NODE_GROUP_NEW,TXT_PB_NEW_GROUP,PNULL},
    {PB_OPTMENU_NODE_GROUP_EDIT,TXT_PB_SETTING_GROUP_MODIFY,EditGroupEnable},
    {PB_OPTMENU_NODE_GROUP_NAME_EDIT,TXT_PB_SETTING_GROUP_NAME_EDIT,EditGroupNameEnable},
    {PB_OPTMENU_NODE_GROUP_MEMBER_MANAGE,TXT_PB_MANAGE_GROUP_MEMBER,AddGroupMemberEnable},
    {PB_OPTMENU_NODE_GROUP_RING,TXT_PB_SETTING_GROUP_RING,EditGroupRingEnable},
    {PB_OPTMENU_NODE_GROUP_DELETE, TXT_PB_DELETE_GROUP,DeleteGroupEnable},
    {PB_OPTMENU_NODE_SEND_GROUP_SMS, TXT_PB_WRITE_GROUP_MESSAGE,SendGroupSMSMenuEnable},      
    {PB_OPTMENU_NODE_REMOVE_FROM_GROUP,TXT_PB_REMOVE_FROM_MEMBER,RemoveGroupMemberEnable},  
#endif
    //other
    {PB_OPTMENU_NODE_OTHERS,TXT_TOOLS_OTHERS,PNULL},
    
    {PB_OPTMENU_NODE_USED_SPACE, TXT_COMMON_DETAIL_MEMORY,PNULL},
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    {PB_OPTMENU_NODE_DISPLAY_LOCATION, TXT_DISPLAY_OPTION,PNULL},
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT)
    {PB_OPTMENU_NODE_SPEED_DIAL, TXT_COMMON_SPEEDDIAL,PNULL},
#endif  
#ifdef MMIPB_SIMDN_SUPPORT
    {PB_OPTMENU_NODE_MSISDN,TXT_PB_MSISDN, SimMenuEnable},
    {PB_OPTMENU_NODE_SDN,TXT_PB_SDN, SimMenuEnable},
#endif

    {PB_OPTMENU_NODE_CONTACT_VIEW,TXT_PB_VIEW,ContactViewMenuEnable},
    {PB_OPTMENU_NODE_ADD_TO_CONTACT,TXT_ADD_TO_CONTACT,AddToContactMenuEnable},
    {PB_OPTMENU_NODE_ADD_TO_NEW_CONTACT,TXT_ADD_TO_NEW_CONTACT,PNULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {PB_OPTMENU_NODE_ADD_TO_EXIST_CONTACT,TXT_ADD_TO_EXIST_CONTACT,PNULL},
#endif

 #ifdef PIM_SUPPORT
    {PB_OPTMENU_UPATE_PB,TXT_PIM_SYNC_START,PNULL},
    {PB_OPTMENU_NET_REQ,TXT_PIM_SYNC_NET_QUERY,PNULL},
 #endif

#ifdef MMIWIDGET_SUPPORT
   { PB_OPTMENU_QUICK_CONTACT,TXT_NULL,PNULL},
#endif

#ifdef MMI_PDA_SUPPORT
    {PB_OPTMENU_NODE_GROUP,TXT_PB_GROUP,PNULL},
    {PB_OPTMENU_NODE_ACCOUNT,TXT_ACCOUNT,PNULL},
#endif

    {PB_OPTMENU_ADD_TO_GROUP,TXT_PB_ADD_TO_GROUP,AddToGroupMenuEnable},
#ifdef MMIPB_MOST_USED_SUPPORT
    {PB_OPTMENU_ADD_TO_FAVORITE,TXT_PB_ADD_TO_FAVORITE,AddToFavoriteEnable},
    {PB_OPTMENU_REMOVE_FAVORITE,TXT_PB_REMOVE_FAVORITE,RemoveFavoriteEnable},
#endif
    {PB_OPTMENU_SETTING,STXT_MAIN_SETTING,PNULL},
    {PB_OPTMENU_PHOTO_SELECT,TXT_PB_PHOTO_SELECT,PNULL},

#ifdef CAMERA_SUPPORT
    {PB_OPTMENU_PHOTO_SHOT,TXT_PB_PHOTO_SHOT,PNULL},
#endif

    {PB_OPTMENU_PHOTO_DEFAULT,TXT_PB_PHOTO_DEFAULT,PNULL},
    {PB_OPTMENU_RING_SELECT,TXT_PB_RING_SELECT,PNULL},
    {PB_OPTMENU_RING_PHONE,TXT_PB_RING_PHONE,PNULL},
    {PB_OPTMENU_RING_DEFAULT,TXT_PB_RING_DEFAULT,PNULL},

#ifdef SNS_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SNS,TXT_PB_SNS_UPDATE,PNULL},  
#ifdef WRE_WEIBO_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SINA,TXT_MAINMENU_ICON_SINA,PNULL},
#endif
#ifdef WRE_TWITTER_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_TWITTER,TXT_COMMON_TIWTTER,PNULL},
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_FACEBOOK,TXT_COMMON_FACEBOOK,PNULL},
#endif
#endif  
    {PB_OPTMENU_OPERRATE_SEL_OK,TXT_COMMON_OK,PNULL},
    {PB_OPTMENU_OPERRATE_SELECT_ALL,TXT_MARK_ALL,MarkAllMenuEnable},
    {PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL,TXT_CANCEL_MARK_ALL,CancelAllMenuEnable},
#if 0
    {PB_OPTMENU_OPERRATE_SELECT_SIM, TXT_PB_SET_SIM, SelStorageMenuEnable},
    {PB_OPTMENU_OPERRATE_SELECT_PHONE, TXT_PB_SET_PHONE, SelStorageMenuEnable},
#endif
};
#endif

LOCAL const MMIPB_MENU_ITEM_INFO_T s_pb_menu[]=
{
    //call
    {PB_OPTMENU_NODE_DIAL, TXT_CALL,DialMenuEnable},        
    {PB_OPTMENU_NODE_DIAL_AUDIO,TXT_CALL_AUDIO,DialByAudioMenuEnable},
    {PB_OPTMENU_NODE_DIAL_AUDIO_DETAIL,TXT_CALL_AUDIO,DialByAudioMenuEnable},
    #ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL_VIDEO, TXT_CALL_VIDEO,DialByVideoMenuEnable},
    {PB_OPTMENU_NODE_DIAL_VIDEO_DETAIL, TXT_CALL_VIDEO,DialByVideoMenuEnable},
    #endif
#ifdef MMI_IP_CALL_SUPPORT
    {PB_OPTMENU_NODE_DIAL_IP,TXT_CALL_IPCALL,DialByIpMenuEnable},
    {PB_OPTMENU_NODE_DIAL_IP_DETAIL,TXT_CALL_IPCALL,DialByIpMenuEnable},    
#endif
    {PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL,TXT_EDIT_NUMBER_BEFORE_DIAL,DialMenuEnable},
    {PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL_DETAIL,TXT_EDIT_NUMBER_BEFORE_DIAL,DialMenuEnable},

    //sms
    {PB_OPTMENU_NODE_WRITE_MESSAGE,TXT_COMMON_WRITE_MESSAGE,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,TXT_COMMON_WRITE_MESSAGE,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_SMS, TXT_SMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_SMS_DETAIL, TXT_SMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_MMS,TXT_MMS,WriteMessageMenuEnable},
    {PB_OPTMENU_NODE_SEND_MMS_DETAIL,TXT_MMS,WriteMessageMenuEnable},

    //
    {PB_OPTMENU_NODE_GROUPS,STXT_SMS_DISTRIBUTION_LISTS, GroupsMenuEnable},
    {PB_OPTMENU_NODE_ADDNEW,STR_LIST_ADD_NEW_CONTACT, AddMenuEnable},
    {PB_OPTMENU_NODE_VCARD_BT_EXPORT_CONTACT, TXT_PB_SEND_VCARD_BY_BT,SendBtVcardEnable},
    {PB_OPTMENU_NODE_MODIFY, STR_LIST_EDIT_CONTACT_EXT01,ModifyMenuEnable},
    {PB_OPTMENU_NODE_DELETE_ONE,STR_LIST_DELETE_CONTACT,DeleteOneMenuEnable},
    {PB_OPTMENU_NODE_DELETE_MULTI,STR_LIST_DELETE_SEVERAL,DeleteMultiMenuEnable},
    {PB_OPTMENU_NODE_SHARE,STR_CONTA_LIST_SHARE_MUL_EXT02,ShareMenuEnable},
#ifdef MMIPB_MOST_USED_SUPPORT 
    {PB_OPTMENU_NODE_CLEAR_MOSTUSED,TXT_DELALL,ClearMostUsedMenuEnable},
#endif
#ifdef MMI_BT_PBAP_SUPPORT
    {PB_OPTMENU_NODE_ANDROIDSYNC,STR_CONTACT_SYNC_VIA_BT, AddMenuEnable},
#endif
    //debug
    {PB_OPTMENU_NODE_DEBUG_MENU,STR_CONTA_DEBUG_EX01,DebugEnable},
    {PB_OPTMENU_NODE_DEBUG_ADD_NEW,STR_LIST_ADD_NEW_CONTACT,PNULL},
    {PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER,STR_CONTA_DEBUG_SIM1_MANAGER_EX01,DebugSimManagerEnable},
    {PB_OPTMENU_NODE_DEBUG_SIM2_MANAGER,STR_CONTA_DEBUG_SIM2_MANAGER_EX01,DebugSimManagerEnable},
    //addedit
    {PB_OPTMENU_ADDEDIT_SAVE_MENU, STR_LIST_SAVE_CONTACT_EXT01,PNULL},
    {PB_OPTMENU_ADDEDIT_CLEAR_DATE, STR_LIST_CLEAR_DATE_EXT01,ClearDateEnable},
    {PB_OPTMENU_ADDEDIT_PREFERRED_NONE, STR_GROUP_LIST_NONE_EXT01,PNULL},
    {PB_OPTMENU_ADDEDIT_PREFERRED_SIM1, STR_ILIST_SIM_ONE,PNULL},
    {PB_OPTMENU_ADDEDIT_PREFERRED_SIM2, STR_ILIST_SIM_TWO,PNULL},
    //copy on to phone & sim
    {PB_OPTMENU_NODE_COPY,STR_ILIST_COPY_CONTACTS,SimMenuEnable},
    {PB_OPTMENU_COPY_FROM_PHONE,STXT_PHONE,PNULL},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_COPY_FROM_SIM1,STR_ILIST_SIM_ONE,Sim1MenuEnable},
    #else
    {PB_OPTMENU_COPY_FROM_SIM1,STR_ILIST_SIM_EXT01,Sim1MenuEnable},
    #endif
    {PB_OPTMENU_COPY_FROM_SIM2,STR_ILIST_SIM_TWO,Sim2MenuEnable},
    {PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE,STR_CONTA_LIST_ANO_DEV_EX02,PNULL},
    {PB_OPTMENU_COPY_SIM1_TO_PHONE,STXT_PHONE,PNULL},
    {PB_OPTMENU_COPY_SIM1_TO_SIM2,STR_ILIST_SIM_TWO,Sim2MenuEnable},
    {PB_OPTMENU_COPY_SIM2_TO_PHONE,STXT_PHONE,PNULL},
    {PB_OPTMENU_COPY_SIM2_TO_SIM1,STR_ILIST_SIM_ONE,Sim1MenuEnable},
    {PB_OPTMENU_COPY_ANOTHER_DEVICE_TO_PHONE,STXT_PHONE,PNULL},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_COPY_PHONE_TO_SIM1,STR_ILIST_SIM_ONE,Sim1MenuEnable},
    #else
    {PB_OPTMENU_COPY_PHONE_TO_SIM1,STR_ILIST_SIM_EXT01,Sim1MenuEnable},
    #endif
    {PB_OPTMENU_COPY_PHONE_TO_SIM2,STR_ILIST_SIM_TWO,Sim2MenuEnable},
    {PB_OPTMENU_COPY_PHONE_TO_ANOTHER_DEVICE,STR_CONTA_LIST_ANO_DEV_EX02,PNULL},
    {PB_OPTMENU_NODE_SETTINGS,STXT_HEADER_PHONEBOOK_OPTIONS,CopyMenuEnable},
    {PB_OPTMENU_NODE_ONE_COPYTOPHONE,TXT_PDA_PB_COPY_FROM_NV_TO_PHONE,CopyOneToNVEnable},
    {PB_OPTMENU_NODE_ONE_COPYTOSIM1,TXT_PDA_PB_COPY_FROM_NV_TO_SIM1,CopyOneToSIMEnable},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_ONE_COPYTOSIM2,TXT_PDA_PB_COPY_FROM_NV_TO_SIM2,CopyOneToSIMEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
    {PB_OPTMENU_NODE_ONE_COPYTOSIM3, TXT_PDA_PB_COPY_FROM_NV_TO_SIM3,CopyOneToSIMEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_ONE_COPYTOSIM4,TXT_PDA_PB_COPY_FROM_NV_TO_SIM4,CopyOneToSIMEnable},
    #endif   
    
    //import & export
    {PB_OPTMENU_NODE_IMPORT_EXPORT,TXT_PB_IMPORT_EXPORT,ImportExportMenuEnable},    
    
    //copy from sim
    {PB_OPTMENU_NODE_COPY_FROM_SIM1,TXT_PB_IMPORT_SIM1,CopyFromSimEnable},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_COPY_FROM_SIM2,TXT_PB_IMPORT_SIM2,CopyFromSimEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_COPY_FROM_SIM3,TXT_PB_IMPORT_SIM3,CopyFromSimEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_COPY_FROM_SIM4,TXT_PB_IMPORT_SIM4,CopyFromSimEnable},
    #endif

    //copy to phone & sim
    {PB_OPTMENU_NODE_COPYTOPHONE,TXT_PDA_PB_COPY_FROM_NV_TO_PHONE, PNULL},
    {PB_OPTMENU_NODE_COPYTOSIM1,TXT_PDA_PB_COPY_FROM_NV_TO_SIM1,CopyToSIMEnable},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_COPYTOSIM2,TXT_PDA_PB_COPY_FROM_NV_TO_SIM2,CopyToSIMEnable},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
    {PB_OPTMENU_NODE_COPYTOSIM3, TXT_PDA_PB_COPY_FROM_NV_TO_SIM3,CopyToSIMEnable},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_COPYTOSIM4,TXT_PDA_PB_COPY_FROM_NV_TO_SIM4,CopyToSIMEnable},
    #endif   
    
    // vcard import & export
    #ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT,TXT_PB_IMPORT_CONTACT,VcardMenuEnable},
    {PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT,TXT_PB_EXPORT_CONTACT,VcardMenuEnable},

    // send vcard by SMS\MMS\Email\BT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, TXT_PB_SEND_VCARD,SendVcardEnable},    
    {PB_OPTMENU_NODE_SEND_BY_SMS_MENU, TXT_SMS,SendVcardEnable},
#ifndef MMIPB_RMVITEM_SUPPORT
    {PB_OPTMENU_NODE_SEND_BY_MMS_MENU, STR_SEND_VIA_MMS,SendVcardEnable},
#endif
//#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU, STR_SEND_VIA_MAIL,SendEmailMenuEnable},   
//#endif       
    {PB_OPTMENU_NODE_SEND_BY_BT_MENU,  STR_SHARE_LIST_SLAM_EXT02,SendVcardEnable}, 
    {PB_OPTMENU_MULTI_SHARE_BY_MMS_MENU, STR_SEND_VIA_MMS,SendMultiVcardEnable},
    {PB_OPTMENU_MULTI_SHARE_BY_EMAIL_MENU, STR_SEND_VIA_MAIL,SendEmailMenuEnable},   
    {PB_OPTMENU_MULTI_SHARE_BY_BT_MENU,  STR_SHARE_LIST_SLAM_EXT02,SendMultiVcardEnable},    
    #endif

    //group 
    {PB_OPTMENU_NODE_CONTACTS,STR_LIST_MENU_CONTACTS,PNULL},
    {PB_OPTMENU_NODE_GROUP_ADD_MEMBERS,STR_GROUP_LIST_ADD_MEMBER_EXT01,PNULL},
    {PB_OPTMENU_NODE_GROUP_SEND_MESSAGE,STR_LIST_SEND_GROUP_MSG_EXT01,SendMessageMenuEnable},
    {PB_OPTMENU_NODE_GROUP_SEND_EMAIL,STR_LIST_SEND_GROUP_MAIL_EXT01,SendEmailMenuEnable},
    {PB_OPTMENU_NODE_GROUP_DEL_MEMBERS,STR_GROUP_LIST_REMOVE_MEMBER_EXT01,PNULL},
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    {PB_OPTMENU_NODE_GROUP_NEW,STR_GROUP_LIST_ADD_NEW_EXT01,PNULL},
    {PB_OPTMENU_NODE_GROUP_EDIT,TXT_PB_SETTING_GROUP_MODIFY,EditGroupEnable},
    {PB_OPTMENU_NODE_GROUP_NAME_EDIT,STR_GROUP_LIST_EDIT_NAME_EXT01,EditGroupNameEnable},
    {PB_OPTMENU_NODE_GROUP_MEMBER_MANAGE,TXT_PB_MANAGE_GROUP_MEMBER,AddGroupMemberEnable},
    {PB_OPTMENU_NODE_GROUP_RING,STR_GROUP_LIST_TONE_EXT01,EditGroupRingEnable},
    {PB_OPTMENU_NODE_GROUP_DELETE, STR_GROUP_LIST_DELETE_EXT01,DeleteGroupEnable},
    {PB_OPTMENU_NODE_SEND_GROUP_SMS, TXT_PB_WRITE_GROUP_MESSAGE,SendGroupSMSMenuEnable},      
    {PB_OPTMENU_NODE_REMOVE_FROM_GROUP,TXT_PB_REMOVE_FROM_MEMBER,RemoveGroupMemberEnable},  
#endif
    //other
    {PB_OPTMENU_NODE_OTHERS,TXT_TOOLS_OTHERS,PNULL},
    
    {PB_OPTMENU_NODE_USED_SPACE, TXT_COMMON_DETAIL_MEMORY,PNULL},
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    {PB_OPTMENU_NODE_DISPLAY_LOCATION, TXT_DISPLAY_OPTION,PNULL},
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT)
    {PB_OPTMENU_NODE_SPEED_DIAL, STXT_MEMORY_OPTIONS_SPEED_DIAL,PNULL},
#endif  
#ifdef MMIPB_SIMDN_SUPPORT
    {PB_OPTMENU_NODE_MSISDN,TXT_PB_MSISDN, SimMenuEnable},
    {PB_OPTMENU_NODE_SDN,TXT_PB_SDN, SimMenuEnable},
#endif

    {PB_OPTMENU_NODE_CONTACT_VIEW,TXT_PB_VIEW,ContactViewMenuEnable},
    {PB_OPTMENU_NODE_ADD_TO_CONTACT,TXT_ADD_TO_CONTACT,AddToContactMenuEnable},
    {PB_OPTMENU_NODE_ADD_TO_NEW_CONTACT,TXT_ADD_TO_NEW_CONTACT,PNULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {PB_OPTMENU_NODE_ADD_TO_EXIST_CONTACT,TXT_ADD_TO_EXIST_CONTACT,PNULL},
#endif

 #ifdef PIM_SUPPORT
    {PB_OPTMENU_UPATE_PB,TXT_PIM_SYNC_START,PNULL},
    {PB_OPTMENU_NET_REQ,TXT_PIM_SYNC_NET_QUERY,PNULL},
 #endif

#ifdef MMIWIDGET_SUPPORT
   { PB_OPTMENU_QUICK_CONTACT,TXT_NULL,PNULL},
#endif

#ifdef MMI_PDA_SUPPORT
    {PB_OPTMENU_NODE_GROUP,TXT_PB_GROUP,PNULL},
    {PB_OPTMENU_NODE_ACCOUNT,TXT_ACCOUNT,PNULL},
#endif

    {PB_OPTMENU_ADD_TO_GROUP,TXT_PB_ADD_TO_GROUP,AddToGroupMenuEnable},
#ifdef MMIPB_MOST_USED_SUPPORT
    {PB_OPTMENU_ADD_TO_FAVORITE,TXT_PB_ADD_TO_FAVORITE,AddToFavoriteEnable},
    {PB_OPTMENU_REMOVE_FAVORITE,TXT_PB_REMOVE_FAVORITE,RemoveFavoriteEnable},
#endif
    {PB_OPTMENU_SETTING,STXT_MAIN_SETTING,PNULL},
    {PB_OPTMENU_PHOTO_SELECT,TXT_PB_PHOTO_SELECT,PNULL},
    {PB_OPTMENU_PHOTO_SELECT_FROM_GALLERY,STR_CONTA_AVATAR_SELECT_FROM_GALLERY,PNULL},

#ifdef CAMERA_SUPPORT
    {PB_OPTMENU_PHOTO_SHOT,TXT_PB_PHOTO_SHOT,PNULL},
#endif

    {PB_OPTMENU_PHOTO_DEFAULT,TXT_PB_PHOTO_DEFAULT,PNULL},
    {PB_OPTMENU_RING_SELECT,TXT_PB_RING_SELECT,PNULL},
    {PB_OPTMENU_RING_PHONE,STR_CHOOSE_FROM_RINGTONES_EXT01,PNULL},
    {PB_OPTMENU_RING_DEFAULT,TXT_PB_RING_DEFAULT,PNULL},
    {PB_OPTMENU_RING_NO_SOUND,STR_NO_SOUND_EXT01,PNULL},

#ifdef SNS_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SNS,TXT_PB_SNS_UPDATE,PNULL},  
#ifdef WRE_WEIBO_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SINA,TXT_MAINMENU_ICON_SINA,PNULL},
#endif
#ifdef WRE_TWITTER_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_TWITTER,TXT_COMMON_TIWTTER,PNULL},
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_FACEBOOK,TXT_COMMON_FACEBOOK,PNULL},
#endif
#endif  
    {PB_OPTMENU_OPERRATE_SEL_CANCEL,STXT_SOFTKEY_CANCEL_MK,PNULL},
    {PB_OPTMENU_OPERRATE_SEL_OK,TXT_COMMON_OK,PNULL},
    {PB_OPTMENU_OPERRATE_SELECT_ALL,STR_MARK_ALL,MarkAllMenuEnable},
    {PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL,STR_UNMARK_ALL,CancelAllMenuEnable},
#if 0
    {PB_OPTMENU_OPERRATE_SELECT_SIM, TXT_PB_SET_SIM, SelStorageMenuEnable},
    {PB_OPTMENU_OPERRATE_SELECT_PHONE, TXT_PB_SET_PHONE, SelStorageMenuEnable},
#endif
     //detail menu
     {PB_OPTMENU_DETAIL_NODE_COPY,STR_ILIST_COPY_CONTACTS,CopyMenuEnable},
     {PB_OPTMENU_DETAIL_NODE_SHARE,STR_CONTA_LIST_SHARE_ONE_EXT02,ShareMenuEnable},
     {PB_OPTMENU_DETAIL_SAVE_VCARD,STR_LIST_SAVE_CONTACT_EXT01,PNULL},
};



#if 0
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_group_entry_opt_item[] = 
{
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    {0,PB_OPTMENU_NODE_GROUP_MEMBER_MANAGE}, 
    {0,PB_OPTMENU_NODE_REMOVE_FROM_GROUP}, 
#endif
    {0,PB_OPTMENU_NODE_DIAL},                               //呼叫
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE},   //写信息
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU}, 
#endif
    //{0,PB_OPTMENU_NODE_CONTACT_VIEW},
    //dial
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO},
#ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO},
#endif
#ifdef MMI_IP_CALL_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_IP},
#endif
    //send sms,mms
#ifndef MMIMMS_SMS_IN_1_SUPPORT     
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_SMS},
#ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_MMS},
#endif
#endif  

    //send vcard
#ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_SMS_MENU},    //发送经短信
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_MMS_MENU},   //发送经彩信
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},   //发送经邮件
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_BT_MENU},      //发送经蓝牙
#endif

    {0xFFFF,0xFFFF},    
};
#else
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_group_entry_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_GROUP_ADD_MEMBERS},
    {0,PB_OPTMENU_NODE_GROUP_SEND_MESSAGE},
    #ifdef MMI_UNISOC_EMAIL_SUPPORT
    {0,PB_OPTMENU_NODE_GROUP_SEND_EMAIL},
    #endif
    {0,PB_OPTMENU_NODE_GROUP_DEL_MEMBERS},
    {0xFFFF,0xFFFF},    
};
#endif

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_add_sub_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_ADD_TO_NEW_CONTACT},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {0,PB_OPTMENU_NODE_ADD_TO_EXIST_CONTACT},
#endif
    {0xFFFF,0xFFFF},
};

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
#if 0
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_fdn_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_ADDNEW},
#ifndef MMI_PDA_SUPPORT
#ifdef VT_SUPPORT
    {0,PB_OPTMENU_NODE_DIAL},
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO},
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO},
#else
    {0,PB_OPTMENU_NODE_DIAL_AUDIO},
#endif
    {0, PB_OPTMENU_NODE_DELETE_ONE},    
#endif
    {0xFFFF,0xFFFF},
};
#else
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_fdn_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_ADDNEW},
    {0, PB_OPTMENU_NODE_DELETE_ONE},    
    {0xFFFF,0xFFFF},
};
#endif
#endif

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_fdn_longtp_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_MODIFY},
#ifdef VT_SUPPORT
    {0,PB_OPTMENU_NODE_DIAL},
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO},
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO},
#else
    {0,PB_OPTMENU_NODE_DIAL_AUDIO},
#endif
    {0,PB_OPTMENU_NODE_DELETE_ONE},
    {0xFFFF,0xFFFF},
};
#endif

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_ring_opt_item[] = 
{
    {0, PB_OPTMENU_RING_DEFAULT},
    {0, PB_OPTMENU_RING_NO_SOUND},
    {0, PB_OPTMENU_RING_PHONE},
    #ifdef PB_UI_UNISOC_DEBUG
    {0, PB_OPTMENU_RING_SELECT},
    #endif
    {0xFFFF,0xFFFF},
    
};


LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_photo_opt_item[] = 
{
    {0,PB_OPTMENU_PHOTO_SELECT},
    {0,PB_OPTMENU_PHOTO_SELECT_FROM_GALLERY},
#ifdef CAMERA_SUPPORT
    {0,PB_OPTMENU_PHOTO_SHOT},
#endif

    {0,PB_OPTMENU_PHOTO_DEFAULT},
    {0xFFFF,0xFFFF},
    
};

#ifdef MMI_PDA_SUPPORT

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_main_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_ADDNEW},
    {0, PB_OPTMENU_NODE_DELETE_MULTI},
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_IMPORT_EXPORT},
#else
    {0,PB_OPTMENU_NODE_COPY},
#endif
  //  {0,PB_OPTMENU_NODE_ACCOUNT},
#ifdef SNS_SUPPORT
    {0,PB_OPTMENU_NODE_IMPORT_FROM_SNS},
#endif
#ifndef MMIPB_RMVITEM_SUPPORT
    {0,PB_OPTMENU_NODE_GROUP}, //群组
#endif        
    {0, PB_OPTMENU_SETTING},
#ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY},
#endif
   /* {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM1},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM2},
    #endif
    #if (defined MULTI_SIM_SYS_QUAD||defined MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM3},
    #endif
    #ifdef MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM4},
    #endif
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM1},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM2},
    #endif
    #if (defined MULTI_SIM_SYS_QUAD||defined MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM3},
    #endif
    #ifdef MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM4},
    #endif*/
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SNS,PB_OPTMENU_NODE_IMPORT_FROM_SINA},
#endif
#ifdef WRE_TWITTER_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SNS,PB_OPTMENU_NODE_IMPORT_FROM_TWITTER},
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_FROM_SNS,PB_OPTMENU_NODE_IMPORT_FROM_FACEBOOK},
#endif
#endif
    
#ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT},
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT},
    //{PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_SEND_VCARD_MENU},    
#endif //MMI_VCARD_SUPPORT

    {0xFFFF,0xFFFF},    
};
#ifdef MMI_PDA_SUPPORT
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_main_longtp_opt_item[] =
{
    {0,PB_OPTMENU_NODE_MODIFY},
    {0,PB_OPTMENU_NODE_DELETE_ONE},
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE},
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU},
#endif 
#ifdef MMIPB_MOST_USED_SUPPORT
    {0,PB_OPTMENU_ADD_TO_FAVORITE},
    {0,PB_OPTMENU_REMOVE_FAVORITE},
#endif
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL},
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    {0,PB_OPTMENU_ADD_TO_GROUP},
#endif
    
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    //send sms,mms
   {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_SMS},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_MMS},
    #endif
#endif  

    //send vcard
#ifdef MMI_VCARD_SUPPORT  
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_SMS_MENU},
#ifndef MMIPB_RMVITEM_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_MMS_MENU},
#endif
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_BT_MENU},   
#endif
    {0xFFFF,0xFFFF},
    
};
#endif


#ifdef MMIPB_GROUP_MANAGE_SUPPORT
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_grouplist_longtp_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_GROUP_DELETE},
    {0,PB_OPTMENU_NODE_SEND_GROUP_SMS},
    {0, PB_OPTMENU_NODE_GROUP_EDIT},
    {0xFFFF,0xFFFF},
    
};

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_group_entry_longtp_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_REMOVE_FROM_GROUP},
#ifdef MMIPB_MOST_USED_SUPPORT
    {0,PB_OPTMENU_ADD_TO_FAVORITE},
    {0, PB_OPTMENU_REMOVE_FAVORITE},
#endif
    {0xFFFF,0xFFFF},
    
};

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_grouplist_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_GROUP_NEW},
    {0xFFFF,0xFFFF},
    
};
#endif



#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_favorite_opt_item[] = 
{
    {0,PB_OPTMENU_ADD_TO_FAVORITE},
    {0,PB_OPTMENU_REMOVE_FAVORITE},
    {0xFFFF,0xFFFF},
    
};

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_mostused_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_CLEAR_MOSTUSED},
    
    {0xFFFF,0xFFFF},
    
};

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_mostused_longtp_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE},
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL},
    {0,PB_OPTMENU_NODE_CONTACT_VIEW},
    {0,PB_OPTMENU_NODE_ADD_TO_CONTACT},
    {0,PB_OPTMENU_NODE_DELETE_ONE},
    
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    //send sms,mms
   {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_SMS},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_MMS},
    #endif
#endif  
    {0xFFFF,0xFFFF},
    
};
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_favorite_longtp_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE},
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU},
#endif
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL},    
    {0,PB_OPTMENU_NODE_CONTACT_VIEW},
    {0,PB_OPTMENU_REMOVE_FAVORITE},
    
    //send sms,mms
#ifndef MMIMMS_SMS_IN_1_SUPPORT
   {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_SMS},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_MMS},
    #endif
#endif  

    //send vcard
#ifdef MMI_VCARD_SUPPORT  
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_SMS_MENU},
#ifndef MMIPB_RMVITEM_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_MMS_MENU},
#endif
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_BT_MENU},
#endif    
    {0xFFFF,0xFFFF},
    
};
#endif

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_basic_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_MODIFY},
    {0,PB_OPTMENU_NODE_DELETE_ONE}, //删除
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU},//拨号
#endif
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL},
#ifdef MMIPB_MOST_USED_SUPPORT
    {0,PB_OPTMENU_ADD_TO_FAVORITE},
    {0,PB_OPTMENU_REMOVE_FAVORITE},
#endif
    //send vcard
#ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_SMS_MENU},
#ifndef MMIPB_RMVITEM_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_MMS_MENU},
#endif
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,PB_OPTMENU_NODE_SEND_BY_BT_MENU},
#endif

    {0xFFFF,0xFFFF},
};

#else

#if 0
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_main_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_ADDNEW},//新增
#ifdef MMIPB_SYNC_WITH_O365
    {0, PB_OPTMENU_NODE_CONTACTSYNC},
#endif
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE}, //写信息
    {0,PB_OPTMENU_NODE_DIAL},//呼叫
    {0,PB_OPTMENU_NODE_VCARD_BT_EXPORT_CONTACT},
    {0,PB_OPTMENU_NODE_DELETE_ONE},//删除单个
    {0,PB_OPTMENU_NODE_DELETE_MULTI},//删除多个
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_IMPORT_EXPORT},//导入导出
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU}, //发送名片
#else
    {0,PB_OPTMENU_NODE_COPY},//导入导出
#endif    
    {0,PB_OPTMENU_NODE_OTHERS}, //其他
    
#ifndef MMIMMS_SMS_IN_1_SUPPORT //信息的二级菜单
    //send sms,mms
   {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_SMS},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE,PB_OPTMENU_NODE_SEND_MMS},
    #endif
#endif    

    //dial
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO},
    #ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO},
    #endif
#ifdef MMI_IP_CALL_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_IP},
#endif
#ifdef MMI_VCARD_SUPPORT
     {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY},
#endif
    //导入导出
   /* {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM1},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM2},
    #endif
    #if (defined MULTI_SIM_SYS_QUAD||defined MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM3},
    #endif
    #ifdef MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPY_FROM_SIM4},
    #endif
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM1},
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM2},
    #endif
    #if (defined MULTI_SIM_SYS_QUAD||defined MULTI_SIM_SYS_TRI)
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM3},
    #endif
    #ifdef MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_COPYTOSIM4},
    #endif*/
    
#ifdef MMI_VCARD_SUPPORT
    //import & export
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT},
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT},

    //send vcard
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_SMS_MENU}, //发送经短信
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_MMS_MENU},    //发送经彩信	
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},	//发送经邮件
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_BT_MENU},  //发送经蓝牙
#endif
    
    //other
#if defined(MMI_SPEEDDIAL_SUPPORT) 
    {PB_OPTMENU_NODE_OTHERS,PB_OPTMENU_NODE_SPEED_DIAL},
#endif
   
#ifdef MMIPB_SIMDN_SUPPORT
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_MSISDN},
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_SDN},
#endif
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_DISPLAY_LOCATION},
#endif
    {PB_OPTMENU_NODE_OTHERS,PB_OPTMENU_NODE_USED_SPACE},
    
    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_main_empty_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_ADDNEW},//新增
#ifdef MMI_VCARD_SUPPORT    
    {0,PB_OPTMENU_NODE_IMPORT_EXPORT},//导入导出    
#endif    
    {0,PB_OPTMENU_NODE_OTHERS}, //其他

#ifdef MMI_VCARD_SUPPORT
    {PB_OPTMENU_NODE_IMPORT_EXPORT, PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT},
    {PB_OPTMENU_NODE_IMPORT_EXPORT,PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT},
#endif    
    
#if defined(MMI_SPEEDDIAL_SUPPORT) 
    {PB_OPTMENU_NODE_OTHERS,PB_OPTMENU_NODE_SPEED_DIAL},
#endif
#ifdef MMIPB_SIMDN_SUPPORT
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_MSISDN},
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_SDN},
#endif
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    {PB_OPTMENU_NODE_OTHERS, PB_OPTMENU_NODE_DISPLAY_LOCATION},
#endif
    {PB_OPTMENU_NODE_OTHERS,PB_OPTMENU_NODE_USED_SPACE},
    
    {0xFFFF,0xFFFF},
    
};
#endif

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_main_opt_item[] = 
{
#ifndef MMIPB_RMVITEM_SUPPORT
    {0,PB_OPTMENU_NODE_GROUPS},//groups
#endif
    {0,PB_OPTMENU_NODE_ADDNEW},//新增
    {0,PB_OPTMENU_NODE_DELETE_MULTI},//删除多个
#ifdef BLUETOOTH_SUPPORT
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SHARE},//share
#endif
#endif
    {0,PB_OPTMENU_NODE_COPY},
    //{0,PB_OPTMENU_NODE_SPEED_DIAL},
    {0,PB_OPTMENU_NODE_SETTINGS},
     {0,PB_OPTMENU_NODE_OTHERS}, //其他
#ifdef MMI_BT_PBAP_SUPPORT
//    {0,PB_OPTMENU_NODE_ANDROIDSYNC},
#endif
    {0,PB_OPTMENU_NODE_DEBUG_MENU},
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_PHONE},
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_SIM1},
#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_SIM2},
 #endif
    //{PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_ADD_NEW},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_SIM2_MANAGER},
     {PB_OPTMENU_NODE_OTHERS,PB_OPTMENU_NODE_USED_SPACE},
    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_main_empty_opt_item[] = 
{
#ifndef MMIPB_RMVITEM_SUPPORT
    {0,PB_OPTMENU_NODE_GROUPS},//groups
#endif
    {0,PB_OPTMENU_NODE_ADDNEW},//新增
    //{0,PB_OPTMENU_NODE_COPY},
    //{0,PB_OPTMENU_NODE_SPEED_DIAL},
    {0,PB_OPTMENU_NODE_SETTINGS},
#ifdef MMI_BT_PBAP_SUPPORT
//    {0,PB_OPTMENU_NODE_ANDROIDSYNC},
#endif
    {0,PB_OPTMENU_NODE_DEBUG_MENU},
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_PHONE},
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_SIM1},
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_SIM2},
#endif
    //{PB_OPTMENU_NODE_COPY,PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_ADD_NEW},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER},
    {PB_OPTMENU_NODE_DEBUG_MENU,PB_OPTMENU_NODE_DEBUG_SIM2_MANAGER},
    
    {0xFFFF,0xFFFF},
    
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_addedit_opt_menu_item[] = 
{
    {0,PB_OPTMENU_ADDEDIT_SAVE_MENU},
    {0,PB_OPTMENU_ADDEDIT_CLEAR_DATE},
    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_addedit_preferred_sim_menu_item[] = 
{
    {0,PB_OPTMENU_ADDEDIT_PREFERRED_NONE},
    {0,PB_OPTMENU_ADDEDIT_PREFERRED_SIM1},
    {0,PB_OPTMENU_ADDEDIT_PREFERRED_SIM2},
    
    {0xFFFF,0xFFFF},
};


LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_multi_share_item[] = 
{
#ifndef MMIPB_RMVITEM_SUPPORT
    {0,PB_OPTMENU_MULTI_SHARE_BY_MMS_MENU},
#endif
    {0,PB_OPTMENU_MULTI_SHARE_BY_BT_MENU},
#ifdef MMI_ORIG_UNISOC_SUPPORT
    #ifdef MMI_UNISOC_EMAIL_SUPPORT
    {0,PB_OPTMENU_MULTI_SHARE_BY_EMAIL_MENU},
    #endif/*MMI_UNISOC_EMAIL_SUPPORT*/
#endif
    {0xFFFF,0xFFFF},
};

#if 0
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_basic_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_MODIFY},
    {0,PB_OPTMENU_NODE_DIAL},//拨号    
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL},//写信息
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL_DETAIL},
    {0,PB_OPTMENU_NODE_DELETE_ONE}, //删除
    {0,PB_OPTMENU_NODE_COPY}, //复制
#ifdef MMI_VCARD_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_VCARD_MENU}, //名片
 #endif 

    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO_DETAIL},
    #ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO_DETAIL},
    #endif
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_IP_DETAIL},
    
#ifndef MMIMMS_SMS_IN_1_SUPPORT    
    //send sms,mms
   {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,PB_OPTMENU_NODE_SEND_SMS_DETAIL},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,PB_OPTMENU_NODE_SEND_MMS_DETAIL},
    #endif
#endif    
    
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM1},
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM2},
    #endif
    #if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM3},
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM4},  
    #endif
    {PB_OPTMENU_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOPHONE},
    
#ifdef MMI_VCARD_SUPPORT    
    //send vcard
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_SMS_MENU}, //发送经短信
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_MMS_MENU},    //发送经彩信	
#ifdef MMIEMAIL_SUPPORT
    {PB_OPTMENU_NODE_SEND_VCARD_MENU, PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},	//发送经邮件
#endif
    {PB_OPTMENU_NODE_SEND_VCARD_MENU,  PB_OPTMENU_NODE_SEND_BY_BT_MENU},  //发送经蓝牙
#endif

    {0xFFFF,0xFFFF},
};
#else
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_basic_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_MODIFY},
    {0,PB_OPTMENU_NODE_DELETE_ONE},
#ifdef PB_UI_UNISOC_DEBUG
    {0,PB_OPTMENU_DETAIL_NODE_COPY},
#endif
#ifdef BLUETOOTH_SUPPORT
    {0,PB_OPTMENU_DETAIL_NODE_SHARE},
#endif
//#if defined(MMI_SPEEDDIAL_SUPPORT) 
//    {0,PB_OPTMENU_NODE_SPEED_DIAL},
//#endif
#ifdef PB_UI_UNISOC_DEBUG
    {PB_OPTMENU_DETAIL_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM1},
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    {PB_OPTMENU_DETAIL_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOSIM2},
#endif
    {PB_OPTMENU_DETAIL_NODE_COPY,PB_OPTMENU_NODE_ONE_COPYTOPHONE},
#endif
    //send vcard
    //{PB_OPTMENU_DETAIL_NODE_SHARE, PB_OPTMENU_NODE_SEND_BY_SMS_MENU}, //发送经短信
#ifndef MMIPB_RMVITEM_SUPPORT
    {PB_OPTMENU_DETAIL_NODE_SHARE,  PB_OPTMENU_NODE_SEND_BY_MMS_MENU},    //发送经彩信	
#endif
    {PB_OPTMENU_DETAIL_NODE_SHARE,  PB_OPTMENU_NODE_SEND_BY_BT_MENU},  //发送经蓝牙
#ifdef MMI_UNISOC_EMAIL_SUPPORT
    {PB_OPTMENU_DETAIL_NODE_SHARE, PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},	//发送经邮件
#endif /*MMI_UNISOC_EMAIL_SUPPORT*/

    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_vcard_opt_item[] = 
{
    {0,PB_OPTMENU_DETAIL_SAVE_VCARD},

    {0xFFFF,0xFFFF},
};

#endif

#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_mostused_opt_item[] = 
{
   // {0,  PB_OPTMENU_NODE_DIAL},//拨号
   // {0,  PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL},//写信息
    //{0,PB_OPTMENU_NODE_DELETE_ONE}, //删除
    {0,PB_OPTMENU_NODE_CLEAR_MOSTUSED}, //清空
    
  /*  {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_AUDIO},
    #ifdef VT_SUPPORT
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_VIDEO},
    #endif
    {PB_OPTMENU_NODE_DIAL,PB_OPTMENU_NODE_DIAL_IP},

#ifndef MMIMMS_SMS_IN_1_SUPPORT  
    //send sms,mms
   {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,PB_OPTMENU_NODE_SEND_SMS_DETAIL},
    #ifdef MMS_SUPPORT
    {PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,PB_OPTMENU_NODE_SEND_MMS_DETAIL},
    #endif
#endif  */  
        {0xFFFF,0xFFFF},
};
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_grouplist_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_CONTACTS},
    {0, PB_OPTMENU_NODE_GROUP_NEW},
    //{0,PB_OPTMENU_NODE_SEND_GROUP_SMS},
    {0, PB_OPTMENU_NODE_GROUP_NAME_EDIT},
    {0, PB_OPTMENU_NODE_GROUP_RING},
    {0, PB_OPTMENU_NODE_GROUP_DELETE},
    //{PB_OPTMENU_NODE_GROUP_RING, PB_OPTMENU_RING_DEFAULT},
    //{PB_OPTMENU_NODE_GROUP_RING, PB_OPTMENU_RING_NO_SOUND},
    //{PB_OPTMENU_NODE_GROUP_RING, PB_OPTMENU_RING_PHONE},
    //{PB_OPTMENU_NODE_GROUP_RING, PB_OPTMENU_RING_SELECT},
    {0xFFFF,0xFFFF},
    
};

LOCAL const  MMIPB_MENU_FAMILY_INFO_T s_pb_grouplist_empty_opt_item[] = 
{
    {0, PB_OPTMENU_NODE_CONTACTS},
    {0, PB_OPTMENU_NODE_GROUP_NEW},
    {0xFFFF,0xFFFF},
    
};

#endif



//check list menu
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_operate_sel_opt_item[] =
{
    {0, PB_OPTMENU_OPERRATE_SEL_OK},                //确定
#if 0
    {0, PB_OPTMENU_OPERRATE_SELECT_SIM},        //选择卡
    {0, PB_OPTMENU_OPERRATE_SELECT_PHONE},        //选择手机
#endif
    {0, PB_OPTMENU_OPERRATE_SELECT_ALL},        //选择全部
    {0, PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL},	//取消全部
    //{0, PB_OPTMENU_OPERRATE_SEL_CANCEL},	//取消
    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_operate_sel_all_opt_item[] =
{
    {0, PB_OPTMENU_OPERRATE_SELECT_ALL},        //选择全部
    {0, PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL},	//取消全部
    {0xFFFF,0xFFFF},
};
#endif

#ifdef MMI_PDA_SUPPORT
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_number_item_opt_item[] =
{
//send sms,mms
#ifdef MMIMMS_SMS_IN_1_SUPPORT 
    {0,PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL},//写信息    
#else    
    {0,PB_OPTMENU_NODE_SEND_SMS_DETAIL},
#ifdef MMS_SUPPORT
    {0,PB_OPTMENU_NODE_SEND_MMS_DETAIL},
#endif
#endif 
    {0,PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL},    
    {0xFFFF,0xFFFF},
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_detail_msglog_opt_item[] = 
{
    {0,PB_OPTMENU_NODE_DELETE_ONE},
    {0xFFFF,0xFFFF},
};
#endif

//write messge sub men
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_writemsg_sub_item[] =
{
    {0, PB_OPTMENU_NODE_SEND_SMS},  //短信
#ifdef MMS_SUPPORT
    {0, PB_OPTMENU_NODE_SEND_MMS},  //彩信
#endif
{0xFFFF,0xFFFF},
};

//dial sub menu
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_dial_sub_item[] =
{
  {0,  PB_OPTMENU_NODE_DIAL_AUDIO},//语音
#ifdef VT_SUPPORT
  {0,  PB_OPTMENU_NODE_DIAL_VIDEO},//视频
#endif        
#ifdef MMI_IP_CALL_SUPPORT
  {0,  PB_OPTMENU_NODE_DIAL_IP},//IP拨号
#endif
  {0xFFFF,0xFFFF},

};
#ifdef MMI_VCARD_SUPPORT
//send vcard sub menu
LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_vcardsend_sub_item[] =
{
    {0, PB_OPTMENU_NODE_SEND_BY_SMS_MENU},        //发送经短信
#ifndef MMIPB_RMVITEM_SUPPORT
    {0, PB_OPTMENU_NODE_SEND_BY_MMS_MENU},        //发送经彩信
#endif
#ifdef MMIEMAIL_SUPPORT
    {0, PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU},	//发送经邮件
#endif
    {0, PB_OPTMENU_NODE_SEND_BY_BT_MENU},           //发送经蓝牙
    {0xFFFF,0xFFFF},
};
#endif

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_copy_from_item[] = 
{
    {0,PB_OPTMENU_COPY_FROM_PHONE},
    {0,PB_OPTMENU_COPY_FROM_SIM1},
    {0,PB_OPTMENU_COPY_FROM_SIM2},
    //{0,PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE},

    {0xFFFF,0xFFFF},
    
};


LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_copy_from_phone_item[] = 
{
    {0,PB_OPTMENU_COPY_PHONE_TO_SIM1},
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    {0,PB_OPTMENU_COPY_PHONE_TO_SIM2},
#endif
    //{0,PB_OPTMENU_COPY_PHONE_TO_ANOTHER_DEVICE},
    
    {0xFFFF,0xFFFF},
    
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_copy_from_sim1_item[] = 
{
    {0,PB_OPTMENU_COPY_SIM1_TO_PHONE},
    {0,PB_OPTMENU_COPY_SIM1_TO_SIM2},
    {0xFFFF,0xFFFF},
    
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_copy_from_sim2_item[] = 
{
    {0,PB_OPTMENU_COPY_SIM2_TO_PHONE},
    {0,PB_OPTMENU_COPY_SIM2_TO_SIM1},
    {0xFFFF,0xFFFF},
    
};

LOCAL const MMIPB_MENU_FAMILY_INFO_T s_pb_copy_from_other_device_item[] = 
{
    {0,PB_OPTMENU_COPY_ANOTHER_DEVICE_TO_PHONE},
    
    {0xFFFF,0xFFFF},
    
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_EnterStorageSelectWin(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
)
{
   MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
   BOOLEAN result = FALSE;

   if(PNULL == data_ptr)
    {
        return FALSE;
    }

    if(MMIAPIPB_IsPbReady())
    {    
        menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
        if(PNULL == menu_infor_ptr)
        {
            return FALSE;
        }
        SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));
        
        menu_infor_ptr->contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        if(PNULL == menu_infor_ptr->contact_ptr)
        {
            SCI_FREE(menu_infor_ptr);
            return FALSE;
        }
        if(PNULL != data_ptr->contact_ptr)
        {
            SCI_MEMCPY(menu_infor_ptr->contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
        }
        else
        {
            SCI_MEMSET(menu_infor_ptr->contact_ptr,0x00,sizeof(MMIPB_CONTACT_T));
        }
        MMK_CreateWinByApplet(data_ptr->applet_handle,
                (uint32 *)MMIPB_STORAGE_SELECT_WIN_TAB, 
                (MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr
                );
        result = TRUE;
    }
    else
    {
        //pb not ready
        if (!MMK_IsOpenWin(MMIPB_WAITING_ALERT_WIN_ID))
        {
            MMIPUB_OpenAlertWinByTextIdEx(data_ptr->applet_handle,
                PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,
                MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
        }
    }
    return result;
}
/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddSubWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result        = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id       = MMK_GetCtrlHandleByWin(win_id,MMIPB_ADD_OPT_CTRL_ID);
    PB_OPTMENU_NODE_E              node_id       = 0;
    GUIMENU_STYLE_E menu_style = GUIMENU_STYLE_POPUP_AUTO;
    
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    uint16 menu_num = 0;
    
    menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == menu_infor_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    } 
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   // 打开窗口:

        menu_style = GetMenuStyle(menu_infor_ptr->menu_type);
               			//creat dynamic menu

        GUIMENU_CreatDynamic(PNULL,win_id, MMIPB_ADD_OPT_CTRL_ID,menu_style);
        
        CreatePbOptMenu(win_id,
            MMK_GetCtrlHandleByWin(win_id,MMIPB_ADD_OPT_CTRL_ID),
            menu_infor_ptr,
            &menu_num);
        
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIPB_ADD_OPT_CTRL_ID));
       
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        node_id = (PB_OPTMENU_NODE_E)GUIMENU_GetCurNodeId(ctrl_id);
        HandleOptMenuAction(win_id,node_id,menu_infor_ptr);
    
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

   case MSG_CLOSE_WINDOW:
        if(PNULL != menu_infor_ptr->contact_ptr)
        {
            SCI_FREE(menu_infor_ptr->contact_ptr);
        }
        MMK_FreeWinAddData(win_id);
        
        MMK_DestroyDynaCtrl(ctrl_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStorageSelectWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_STORAGE_SELECT_OPT_CTRL_ID);
    MMIPB_NAME_T        name_t = {0};
    uint16               i = 0;
    uint16               node_id = 0;
    uint16              index = 0;
    //uint16              arr_num = 0;
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    //GUI_BOTH_RECT_T     both_rect = {0};
    MMI_STRING_T        str_t = {0};

    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;

    add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    switch (msg_id)
    {
      case MSG_OPEN_WINDOW:
        //list avaliabel groups
          {
              GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);     
              GUIMENU_CreatDynamic(&both_rect,win_id,MMIPB_STORAGE_SELECT_OPT_CTRL_ID, GUIMENU_STYLE_THIRD);
              //
          }
        ctrl_id=MMK_GetCtrlHandleByWin(win_id,MMIPB_STORAGE_SELECT_OPT_CTRL_ID);	    	
        for (i = 0; i < MMIPB_STORAGE_MAX; i++)
        {
            if(!MMIPB_IsFixedStorage(i))
            {
                if(i < MMIPB_STORAGE_SIM1 + MMI_DUAL_SYS_MAX)
                {
                    if(i >= MMIPB_STORAGE_SIM1 && i < MMIPB_STORAGE_SIM1 + MMI_DUAL_SYS_MAX)
                    {
                        if((FALSE == MMIAPIPHONE_GetSimExistedStatus(i -MMIPB_STORAGE_SIM1)) || ((SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(i -MMIPB_STORAGE_SIM1)) || (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(i -MMIPB_STORAGE_SIM1))))
                        {
                            //sim卡不存在，不列出
                             continue;
                        }
                    }
                    MMI_GetLabelTextByLang(s_pb_storage_text[i], &str_t);/*lint !e661*/
                    if(PNULL != str_t.wstr_ptr)
                    {
                        name_t.wstr_len = MIN(str_t.wstr_len, MMIPB_NAME_MAX_LEN);
                        MMIAPICOM_Wstrncpy(name_t.wstr, str_t.wstr_ptr, name_t.wstr_len);
                    }
                    
                    str_t.wstr_len = name_t.wstr_len;
                    str_t.wstr_ptr = name_t.wstr;                    
                    node_item.item_text_ptr = &str_t;
                    GUIMENU_InsertNode(index, i, 0, &node_item, ctrl_id);                    
                    index++;
                }                                
            }
        }


//#ifndef MMI_PDA_SUPPORT
  //      GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, TXT_NULL, STXT_RETURN);
   //     #endif
        {
            GUIMENU_BUTTON_INFO_T button_infor = {0};
        button_infor.is_static = FALSE;
        
        button_infor.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_infor);
        }

        MMI_GetLabelTextByLang(TXT_NEW, &str_t);
        GUIMENU_SetMenuTitle(&str_t,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);

        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        //SCI_TRACE_LOW:"HandleStorageSelectWinMsg node_id is %d"
       // SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_2153_112_2_18_2_43_39_236,(uint8*)"d",node_id);
        //HandleStorageSelectAction(add_data_ptr,node_id,TRUE); 
        //add_data_ptr->contact_ptr->storage_id = node_id;
        OpenAddWin(add_data_ptr, node_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    
        MMK_CloseWin(win_id);
    
        break;
    case MSG_CLOSE_WINDOW:
        if(PNULL!=add_data_ptr->contact_ptr)
        {
            SCI_FREE(add_data_ptr->contact_ptr);
        }
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#ifdef MMIPB_SIMCONTACT_SUPPORT
/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_CTRL_ID_T ctrl_id,
//  Output                              : none
//  Return                              : none
//  Caution                             : none
/*****************************************************************************/
LOCAL void mmipb_SetSelectWinList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{     
	uint32 item_index = 0;
	
	GUILIST_SetMaxItem(
		ctrl_id, 
		MMIPB_SETTINGS_MEMORY_INDEX_MAX,
		FALSE
		);
    AppendListItemForSettings(
        ctrl_id,
        MMIPB_SETTINGS_MEMORY_INDEX_PHONE,
        &item_index,
        GUIITEM_STYLE_ONE_LINE_TEXT_MS,
        STXT_PHONE,
        TXT_NULL,
        FALSE
        );
    
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {        
        AppendListItemForSettings(
            ctrl_id,
            MMIPB_SETTINGS_MEMORY_INDEX_SIM1,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            #ifdef MMI_MULTI_SIM_SYS_SINGLE
            STR_ILIST_SIM_EXT01,	
            #else
            STR_CONTACT_LIST_MEMORY_S1,
            #endif
            TXT_NULL,
            FALSE
            );
    }
    
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {        
        AppendListItemForSettings(
            ctrl_id,
            MMIPB_SETTINGS_MEMORY_INDEX_SIM2,
            &item_index,
            GUIITEM_STYLE_ONE_LINE_TEXT_MS,
            STR_CONTACT_LIST_MEMORY_S2,
            TXT_NULL,
            FALSE
            );
    }
	GUILIST_SetBgColor(ctrl_id, MMI_WHITE_COLOR);
	return;
}

/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_CTRL_ID_T ctrl_id,
//  Output                              : none
//  Return                              : none
//  Caution                             : none
/*****************************************************************************/
LOCAL uint16 GUILIST_GetCurItemNodeID(uint16 uiIndex)
{
    uint16 storage_id = 0;
    
    if (0 == uiIndex)
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    }
    else if (1 == uiIndex)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, 0);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM2, 0);
        }
        else
        {
            SCI_TRACE_LOW("[MMIPB-memory] Error display index: %d", uiIndex);
        }
    }
    else if (2 == uiIndex)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM2, 0);
        }
        else
        {
            SCI_TRACE_LOW("[MMIPB-memory] Error display index: %d", uiIndex);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB-memory] Error display index: %d", uiIndex);
    }

    return storage_id;
}

/*****************************************************************************/
//  Description                         : Get contact memory status
//  Global resource dependence          : none 
//  Author                              : zhongping.shi
//  Input                               : MMI_WIN_ID_T win_id,
//                                        MMI_CTRL_ID_T ctrl_id,
//  Output                              : none
//  Return                              : none
//  Caution                             : none
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStorageSelectWinMsgList(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E                   result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                 ctrl_id = PNULL;
    MMIPB_NAME_T                   name_t = {0};
    uint16                              i = 0;
    uint16                        node_id = 0;
    uint16                          index = 0;
    GUIMENU_DYNA_ITEM_T         node_item = {0};
    MMI_STRING_T                    str_t = {0};
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;

    add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_STORAGE_SELECT_OPT_CTRL_ID);
        
    switch (msg_id)
    {
      case MSG_OPEN_WINDOW:
        mmipb_SetSelectWinList(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
		MMK_SetActiveCtrl(ctrl_id, FALSE);
                GUIWIN_SetTitleTextId(win_id, STR_CONTA_HDR_MEMORY_EXT01, FALSE);
		if (MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CONTA_HDR_MEMORY_EXT01))
		{
			GUIWIN_UpdateStb();
		}
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    //disable MSG_CTL_OK for bug 1064757 ,because no left key icon is displayed
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
		node_id = GUILIST_GetCurItemNodeID(GUILIST_GetCurItemIndex(ctrl_id));
        OpenAddWin(add_data_ptr, node_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    
        MMK_CloseWin(win_id);
    
        break;
    case MSG_CLOSE_WINDOW:
        if(PNULL!=add_data_ptr->contact_ptr)
        {
            SCI_FREE(add_data_ptr->contact_ptr);
        }
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#endif    
/*****************************************************************************/
//  Description : open add win
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenAddWin(MMIPB_MENU_DATA_INFOR_T *add_data_ptr, uint16 storage_id)
{
    //MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    uint32 time_period = MMI_2SECONDS;
    BOOLEAN result = FALSE;
    MMIPB_ERROR_E space_status = MMIPB_ERROR_SUCCESS;
    MMI_TEXT_ID_T alert_text = TXT_NULL;
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID; 
    
    //SCI_TRACE_LOW:"[MMIPB]OpenAddWin"
   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_2196_112_2_18_2_43_39_237,(uint8*)"");
    
    if(PNULL == add_data_ptr)
    {
        return result;
    }
    
/***bug 99494***/
#ifdef FDN_SUPPORT
    if(!MMIPB_IsFdnContact(storage_id) &&
		MMIAPIFDN_Is2GFdnEnable(MMIPB_GetDualSysByStorageId(storage_id)))
    {
        //失败窗口
        alert_text = TXT_FDN_ACTIVATED;

        #if 0
        MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle, &time_period,
                                        alert_text,PNULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,
                                        MMIPUB_SOFTKEY_ONE,PNULL,PNULL);  

        MMIPUB_SetWinTitleTextId(MMK_GetWinHandle(add_data_ptr->applet_handle,alert_id), 
                                 TXT_PB_NEW_CONTACT, TRUE);
        #endif
        MMIPB_OpenCommonDialogWithWinId(add_data_ptr->applet_handle, alert_id, alert_text);
        return TRUE;
    }
#endif
    
    if (PNULL == add_data_ptr->contact_ptr||0 == add_data_ptr->contact_ptr->contact_id)
    {
        //新增联系人需要判断存储器空间
        space_status = MMIPB_GetStorageFreeSpace(storage_id, PNULL);       			
    }
    SCI_TRACE_LOW("[mmipb] OpenAddWin space_status %d", space_status);
    
    if(MMIPB_ERROR_SUCCESS == space_status)
    {
        BOOLEAN         is_sim_ok  = TRUE;        
        SIM_STATUS_E    sim_status = MMIAPIPHONE_GetSimStatus(MMIPB_GetDualSysByStorageId(storage_id));
        SCI_TRACE_LOW("[mmipb] OpenAddWin sim_status %d", sim_status);  
        
		// 根据storage id来判断该SIM 卡是否可用:sim卡是否存在且处于待机设置,飞行模式
        if(MMIPB_IsSimRecord(storage_id) && !MMIAPIPHONE_GetSimExistedStatus(MMIPB_GetDualSysByStorageId(storage_id)))
		{		    
            SCI_TRACE_LOW("[mmipb] OpenAddWin sim is not available");
            is_sim_ok = FALSE;
            alert_text = TXT_SIM_REJECTED;
            
        }
        
        //判断该sim卡status,如puk是否被锁....
        if(MMIPB_IsSimRecord(storage_id) && (SIM_STATUS_OK != sim_status) && (SIM_STATUS_REGISTRATION_FAILED != sim_status))
        {
            SCI_TRACE_LOW("[mmipb] OpenAddWin sim is not ok");
            is_sim_ok = FALSE;
            alert_text = s_sim_status_string[sim_status];
            
        }

        if(!is_sim_ok)
        {
            #if 0
            MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle, &time_period,
				alert_text,PNULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,
				MMIPUB_SOFTKEY_ONE,PNULL,PNULL);  
            MMIPUB_SetWinTitleTextId(MMK_GetWinHandle(add_data_ptr->applet_handle,alert_id), 
				TXT_PB_NEW_CONTACT, TRUE);
            #endif
            MMIPB_OpenCommonDialogWithWinId(add_data_ptr->applet_handle, alert_id, alert_text);
            return FALSE;
        }
        //nv有空间 或者 sim卡状态ok 
        {
            menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
            if(PNULL == menu_infor_ptr)
            {
                return FALSE;
            }
            SCI_MEMCPY(menu_infor_ptr, add_data_ptr, sizeof(MMIPB_MENU_DATA_INFOR_T));
            
            menu_infor_ptr->contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == menu_infor_ptr->contact_ptr)
            {
                SCI_FREE(menu_infor_ptr);
                return FALSE;
            }
            SCI_MEMSET(menu_infor_ptr->contact_ptr,0x00,sizeof(MMIPB_CONTACT_T));
            if(PNULL!=add_data_ptr->contact_ptr)
            {
                SCI_MEMCPY(menu_infor_ptr->contact_ptr,add_data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
            }
            
            menu_infor_ptr->contact_ptr->storage_id = storage_id;
            if(!MMIAPIPB_IsSortBySurname())
            {
                MMK_CreateWinByApplet(add_data_ptr->applet_handle, 
					(uint32 *)MMIPB_CONTACT_ADD_OR_EDIT_WIN_TAB,
					(MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);
            }
            else
            {
                MMK_CreateWinByApplet(add_data_ptr->applet_handle, 
					(uint32 *)MMIPB_CONTACT_ADD_OR_EDIT_WIN_TAB2,
					(MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);
				
            }
            result = TRUE;
        }
    }
    else
    {
        //提示错误，退出新增
        if(MMIPB_ERROR_NO_SPACE == space_status)
        {            
            if(MMIPB_IsPhoneContact(storage_id))
            {
                alert_text = STR_PHONE_MEMORY_FULL_EXT01;    
            }
            else  if(MMIPB_IsSimRecord(storage_id))
            {
                alert_text = STR_SIM_FULL_EXT01;    
            }
            else
            {
                alert_text = TXT_PB_CURRENT_STORAGE_FULL_SELECT_ANOTHER;
            }
        }
        else if(MMIPB_ERROR_SIM_ERROR == space_status)
        {
            alert_text = STR_SIM_NOT_SIM_EXT01;
        }
        else
        {
            alert_text = TXT_ERROR;
        }

         #if 0
         MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle, &time_period,
                alert_text,PNULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,
                MMIPUB_SOFTKEY_ONE,PNULL,PNULL);  
         
         MMIPUB_SetWinTitleTextId(MMK_GetWinHandle(add_data_ptr->applet_handle,alert_id), 
            TXT_PB_NEW_CONTACT, TRUE);
         #else
         MMIPB_OpenCommonDialogWithWinId(add_data_ptr->applet_handle, alert_id, alert_text);
         #endif
    }
    return result;      
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : save user input infor
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetUserEditNumber(MMI_HANDLE_T win_handle, //IN
						   MMIPB_CONTACT_T *contact_ptr//OUT
						   )
{
    MMI_STRING_T edit_str = {0};
    //MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    uint8 loop_count = 0;
    uint16       i = 0;
    uint16       index = 0;
	
    MMI_PARTY_NUMBER_T  party_num = {0};
    uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN + 1] = {0};
    uint16 num_len = 0;
	
    if(PNULL != contact_ptr)
    {
		 //get number 
         MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&loop_count);
    
        for(i = 0; i < loop_count; i++)
        {
            if(PNULL != MMK_GetCtrlPtrByWin(win_handle, s_num_edit_id[i]))
            {
                SCI_MEMSET(&party_num,0,sizeof(MMI_PARTY_NUMBER_T));
                GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,s_num_edit_id[i]), &edit_str);
                num_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, edit_str.wstr_len);
                SCI_MEMSET(num_buf, 0x00, sizeof(num_buf));
                MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, edit_str.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN, num_len);
                SCI_MEMSET(&(contact_ptr->number[i]),0,sizeof(MMIPB_BCD_NUMBER_T));
                if (num_len > 0  && MMIAPICOM_GenPartyNumber((uint8 *)(num_buf), num_len, &party_num))
                {
                    
                    contact_ptr->number[i].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
                    contact_ptr->number[i].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                    SCI_MEMCPY(contact_ptr->number[i].number, party_num.bcd_num, sizeof(party_num.bcd_num));
                }
                index = i;
                		 
            }
        }
	 }
    return index;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : save user input infor
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void GetUserEditContact(MMI_HANDLE_T win_handle, //IN
						   MMIPB_CONTACT_T *contact_ptr//OUT
						   )
{
    MMI_STRING_T edit_str = {0};
    //MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    //uint8 loop_count = 0;
    //uint16       i = 0;

    //MMI_PARTY_NUMBER_T  party_num = {0};
    //uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN + 1] = {0};
    //uint16 num_len = 0;

    //MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    //uint8 loop_count = 0;
    //uint16       i = 0;
	
    //MMI_PARTY_NUMBER_T  party_num = {0};
    //uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN + 1] = {0};
    //uint16 num_len = 0;
	
    if(PNULL != contact_ptr)
    {
        
		 //get entry name
        SCI_MEMSET(&contact_ptr->name,0,sizeof(MMIPB_NAME_T));
        SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_EDITBOX_NAME_ID), &edit_str);
        contact_ptr->name.wstr_len = MIN(edit_str.wstr_len, MMIPB_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(contact_ptr->name.wstr, edit_str.wstr_ptr, contact_ptr->name.wstr_len);

 //alias
    #ifdef CMCC_VCARD_SUPPORT
         if(MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_SNE))
         {
             SCI_MEMSET(&contact_ptr->alias,0,sizeof(MMIPB_NAME_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
             GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_EDITBOX_ALIAS_ID), &edit_str);
             contact_ptr->alias.wstr_len = MIN(edit_str.wstr_len, MMIPB_NAME_MAX_LEN);
             
             MMIAPICOM_Wstrncpy(contact_ptr->alias.wstr, edit_str.wstr_ptr, contact_ptr->alias.wstr_len);	 
         }
         else
         {
             contact_ptr->alias.wstr_len = 0;
         }
    #endif	
		 //get number 
         GetUserEditNumber(win_handle, contact_ptr);
#ifdef MMIPB_MAIL_SUPPORT
		 //get email
        if(PNULL != MMK_GetCtrlPtrByWin(win_handle, MMIPB_EDITBOX_EMAIL_ID))
        {
            SCI_MEMSET(&contact_ptr->mail,0,sizeof(MMIPB_MAIL_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_EDITBOX_EMAIL_ID), &edit_str);
                contact_ptr->mail.wstr_len = MIN(edit_str.wstr_len, MMIPB_MAX_MAIL_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->mail.wstr, edit_str.wstr_ptr, contact_ptr->mail.wstr_len);
        }
        else
        {
            contact_ptr->mail.wstr_len = 0;
        }
#endif
		//sign
        //目前还没有实现，待完善！！
#ifdef MMIPB_IM_MESSAGE_SUPPORT
        //imessager
        if(PNULL != MMK_GetCtrlPtrByWin(win_handle, MMIPB_IM_EDIT_ID))
        {
            SCI_MEMSET(&contact_ptr->imessager,0,sizeof(MMIPB_IM_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_IM_EDIT_ID), &edit_str);
                contact_ptr->imessager.wstr_len = MIN(edit_str.wstr_len, MMIPB_IM_MAX_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->imessager.wstr, edit_str.wstr_ptr, contact_ptr->imessager.wstr_len);
        }
        else
        {
            contact_ptr->imessager.wstr_len = 0;
        }
#endif
        //birth
  #ifndef PB_SUPPORT_LOW_MEMORY
  
        if(PNULL != MMK_GetCtrlPtrByWin(win_handle, MMIPB_BIRTHDAY_EDIT_ID))
        {

            SCI_MEMSET(&contact_ptr->birth,0,sizeof(MMIPB_BIRTH_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
            GUILABEL_GetText(MMK_GetCtrlHandleByWin(win_handle,MMIPB_BIRTHDAY_EDIT_ID), &edit_str);
                contact_ptr->birth.wstr_len = MIN(edit_str.wstr_len, MMIPB_SNS_BIRTH_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->birth.wstr, edit_str.wstr_ptr, contact_ptr->birth.wstr_len);
        
        }
        else
        {
            SCI_MEMSET(&contact_ptr->birth,0,sizeof(MMIPB_BIRTH_T));
        }
        //url
        if(PNULL != MMK_GetCtrlPtrByWin(win_handle, MMIPB_URL_EDITBOX_ID))
        {
            SCI_MEMSET(&contact_ptr->url,0,sizeof(MMIPB_URL_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_URL_EDITBOX_ID), &edit_str);
                contact_ptr->url.wstr_len = MIN(edit_str.wstr_len, MMIPB_URL_MAX_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->url.wstr, edit_str.wstr_ptr, contact_ptr->url.wstr_len);
        }
        else
        {
            SCI_MEMSET(&contact_ptr->url,0,sizeof(MMIPB_URL_T));
        }
	  //get memo
        if(PNULL != MMK_GetCtrlHandleByWin(win_handle,MMIPB_EDITBOX_MEMO_ID))
        {
            SCI_MEMSET(&contact_ptr->memo,0,sizeof(MMIPB_CUSTOM_TEXT_T));
            SCI_MEMSET(&edit_str,0x00,sizeof(MMI_STRING_T));
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_handle,MMIPB_EDITBOX_MEMO_ID), &edit_str);
            contact_ptr->memo.wstr_len = MIN(edit_str.wstr_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->memo.wstr, edit_str.wstr_ptr, contact_ptr->memo.wstr_len);
        }
        else
        {
            SCI_MEMSET(&contact_ptr->memo,0,sizeof(MMIPB_CUSTOM_TEXT_T));
        }
#endif

	 }
}
#endif

#ifdef MMI_PDA_SUPPORT
#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_320X480)
/*****************************************************************************/
//  Description : add new editbox of contact number auto
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateNumberEditCtrl(MMI_WIN_ID_T win_id,MMIPB_CONTACT_T *contact_ptr)
{
    uint16 index = 0;
    uint16 cur_index = 0;
    uint16 i = 0;
    uint8 loop_count = 0;
    
    //MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
      
    MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&loop_count);
    
    cur_index = GetUserEditNumber(win_id,contact_ptr);
    
    index = GetNextAddNumberIndex(cur_index, contact_ptr);

    if(0 == MMK_GetCtrlHandleByWin(win_id, s_num_edit_id[index]))
    {
        CreateContactNumberItem(win_id,contact_ptr, index);       
    }

    for(i = 0;i<loop_count;i++)
    {
        
        if(
            MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton,
            contact_ptr->number[i].number_len)&&
            (loop_count > 1)
            )
        {
            GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i]),
                    MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[i]),
                    GUIFORM_CHILD_DISP_NORMAL);
           
        }
        else
        {
            GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i]),
                    MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[i]),
                    GUIFORM_CHILD_DISP_HIDE);
        }
    }
  /*  if(contact_ptr->birth.wstr_len>0)
    {
        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_NORMAL);
    }
    else
    {
        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_HIDE);
    }
    */
    MMK_UpdateScreen();
}
#else
/*****************************************************************************/
//  Description : add new editbox of contact number auto
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateNumberEditCtrl(MMI_WIN_ID_T win_id,MMIPB_CONTACT_T *contact_ptr)
{
    uint16 index = 0;
    uint16 cur_index = 0;
    uint16 i = 0;
    uint8 loop_count = 0;
    GUI_BG_T bg_infor = {0};
    
    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
      
    MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&loop_count);
    
    cur_index = GetUserEditNumber(win_id,contact_ptr);
    
    index = GetNextAddNumberIndex(cur_index, contact_ptr);

    if(0 == MMK_GetCtrlHandleByWin(win_id, s_num_edit_id[index]))
    {
        CreateContactNumberItem(win_id,contact_ptr, index);
       
    }

    for(i = 0;i<loop_count;i++)
    {
        SCI_MEMSET(&bg_infor,0,sizeof(GUI_BG_T));
        if(
            (loop_count > 1)&&MMIPB_IsValidPhoneNumber(
                contact_ptr->number[i].npi_ton,
                contact_ptr->number[i].number_len)
            
            )
        {
            bg_infor.bg_type = GUI_BG_IMG;
            bg_infor.img_id = IMAGE_PB_DELETE;
        }
        else
        {
            bg_infor.bg_type = GUI_BG_COLOR;
            bg_infor.color = MMI_DARK_GRAY_COLOR;
        }
        GUIBUTTON_SetBg(
                    MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[i]),
                    &bg_infor);
    }
 /*   if(contact_ptr->birth.wstr_len>0)
    {
        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_NORMAL);
    }
    else
    {
        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_HIDE);
    }
    */
    MMK_UpdateScreen();
}
#endif
#endif
/*****************************************************************************/
//  Description : ContactAdd, Init Add Data
//  Global resource dependence : none
//  Author: Starmer.Sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContactAddInitAddData(MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{      
    if(PNULL == add_data_ptr)
    {
        return FALSE;
    }

    if (PNULL==add_data_ptr->ring_ptr)
    {
        add_data_ptr->ring_ptr = (MMIPB_CUSTOM_RING_T *)SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
    }

   if (PNULL == add_data_ptr->photo_ptr)
    {
        add_data_ptr->photo_ptr = (MMIPB_CUSTOM_PHOTO_T *)SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_PHOTO_T));
    }

#ifdef ASP_SUPPORT
    if(PNULL == add_data_ptr->asp_ptr)
    {
        add_data_ptr->asp_ptr = (MMIPB_CUSTOM_RING_T *)SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
    }
#endif
    if(
   (PNULL == add_data_ptr->photo_ptr)||(PNULL == add_data_ptr->ring_ptr)
#ifdef ASP_SUPPORT
    || (PNULL == add_data_ptr->asp_ptr)
        #endif
    )
    {

        if(PNULL != add_data_ptr->ring_ptr)
        {
            SCI_FREE(add_data_ptr->ring_ptr)
        }
        if(PNULL != add_data_ptr->photo_ptr)
        {
            SCI_FREE(add_data_ptr->photo_ptr)
        }
        #ifdef ASP_SUPPORT
        if(PNULL != add_data_ptr->asp_ptr)
        {
            SCI_FREE(add_data_ptr->asp_ptr)
        }
        #endif
        return FALSE;
    }

    SCI_MEMSET(add_data_ptr->ring_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));

    SCI_MEMSET(add_data_ptr->photo_ptr, 0x00, sizeof(MMIPB_CUSTOM_PHOTO_T));

#ifdef ASP_SUPPORT
    SCI_MEMSET(add_data_ptr->asp_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));
#endif

    //get photo file path and ring file path
    if(add_data_ptr->contact_ptr->ring_id_flag)
    {
        MMIPB_ReadRingFile(add_data_ptr->contact_ptr->contact_id,add_data_ptr->contact_ptr->storage_id,
             add_data_ptr->ring_ptr);

        if(add_data_ptr->ring_ptr->is_valid &&(!MMIFILE_IsFileExist((const wchar *)add_data_ptr->ring_ptr->file_name,
                (uint16)add_data_ptr->ring_ptr->file_name_len)))
        {
            //ring file not exist
            add_data_ptr->ring_ptr->is_valid = FALSE;
        }
    }

    if(add_data_ptr->contact_ptr->photo_id_flag)
    {
        if(!MMIPB_ReadPhotoFile(add_data_ptr->contact_ptr->contact_id,
            add_data_ptr->contact_ptr->storage_id, 
             add_data_ptr->photo_ptr))
        {
            add_data_ptr->photo_ptr->is_valid = FALSE;
        }

        if(add_data_ptr->photo_ptr->is_valid &&(!MMIFILE_IsFileExist((const wchar *)add_data_ptr->photo_ptr->file_name,
            (uint16)add_data_ptr->photo_ptr->file_name_len)))
        {
            //photo file not exist
            add_data_ptr->photo_ptr->is_valid = FALSE;
        }
    }

#ifdef ASP_SUPPORT
    if(add_data_ptr->contact_ptr->asp_id_flag )
    {
        MMIPB_ReadASPFile(add_data_ptr->contact_ptr->contact_id,
            add_data_ptr->contact_ptr->storage_id,
            add_data_ptr->asp_ptr);
        if(add_data_ptr->asp_ptr->is_valid &&(!MMIFILE_IsFileExist((const wchar *)add_data_ptr->asp_ptr->file_name,
        	(uint16)add_data_ptr->asp_ptr->file_name_len)))
        {
            //photo file not exist
            add_data_ptr->asp_ptr->is_valid = FALSE;
        }
    }
#endif    
    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CreateEditGroupWin(MMI_WIN_ID_T  win_id, MMIPB_MENU_DATA_INFOR_T* add_data_ptr)
{
    add_data_ptr->parent_win = win_id;
    MMK_CreateWin((uint32*)MMIPB_ADD_TO_GROUP_WIN_TAB, add_data_ptr);
    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E SaveEditContact(MMI_HANDLE_T win_id,MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_SUCCESS;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    int32 confirm_len_ptr = 0;

    if(PNULL == add_data_ptr||PNULL == add_data_ptr->contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    applet_ptr =(MMIPB_APPLET_T *)MMK_GetAppletPtr(add_data_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
#if defined (MMIPB_SIMDN_SUPPORT)     
    if(        
       (!MMIPB_IsContactValid(add_data_ptr->contact_ptr))
       &&MMIPB_IsMsisdnContact(add_data_ptr->contact_ptr->storage_id)
       
     )
    {// if is msisdn contact have no name and number ,then delete is
        
        if(0==add_data_ptr->contact_ptr->contact_id)
        {
            MMK_CloseWin(win_id);
            ret = MMIPB_ERROR_BREAK;
        }
        else
        {
            MMIPB_CONTACT_T* contact_info =PNULL;
            contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
            SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
            if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(add_data_ptr->contact_ptr->contact_id,add_data_ptr->contact_ptr->storage_id,contact_info))
            {
                if(MMIPB_IsContactValid(&contact_info))
                {
                    ret = MMIPB_DeleteUnListContact(applet_ptr->instance.handle,
                        add_data_ptr->contact_ptr->contact_id,
                        add_data_ptr->contact_ptr->storage_id
                        );
                          
                    if(ret == MMIPB_ERROR_PENDING||MMIPB_ERROR_SUCCESS== ret)
                    {                
                        MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_ENTRY_DETAILS_WIN_ID));
                        MMK_CloseWin(win_id);
                    }
                }
                else
                {
                    MMK_CloseWin(win_id);
                    ret = MMIPB_ERROR_BREAK;
                }
            }
            else
            {
                MMK_CloseWin(win_id);
                ret = MMIPB_ERROR_BREAK;
            }
            
            SCI_FREE(contact_info);
        }
    }
    else 
#endif
    {
#ifdef MMIPB_MAIL_SUPPORT    
        if(add_data_ptr->contact_ptr->mail.wstr_len > 0) 
        {
            if(!(URL_IsMailtoDefault((URL_CHAR_T const *)add_data_ptr->contact_ptr->mail.wstr, &confirm_len_ptr)
				&& confirm_len_ptr == add_data_ptr->contact_ptr->mail.wstr_len))
            	{
				
            	    //MMIPB_DisplaybannerInfo(0, STR_INVALID_EMAIL_ADDRESS_EXT01, PNULL);
            	    MMIPB_OpenCommonDialogWin(applet_ptr->caf_applet.app_handle, STR_INVALID_EMAIL_ADDRESS_EXT01);
            	    ret = MMIPB_ERROR_BREAK;
            	    return ret;
            	}
        }
#endif				
        if(!MMIPB_IsContactValid(add_data_ptr->contact_ptr))
        {
			if (MMIPB_IsSimRecord(add_data_ptr->contact_ptr->storage_id))
			{
				//MMIPB_DisplaybannerInfo(0, TXT_PB_SIM_CONTACT_EMPTY, PNULL);
				MMIPB_OpenCommonDialogWin(applet_ptr->caf_applet.app_handle, TXT_PB_SIM_CONTACT_EMPTY);
				//MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
					//TXT_PB_SIM_CONTACT_EMPTY,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL); 
			}
			else
			{
				//MMIPB_DisplaybannerInfo(0, STR_CONTA_INFO_EMPTY_EXT01, PNULL);
				MMIPB_OpenCommonDialogWin(applet_ptr->caf_applet.app_handle, STR_CONTA_INFO_EMPTY_EXT01);
				//MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
				//TXT_PB_CONTACT_EMPTY,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
			}	
            ret = MMIPB_ERROR_BREAK;
        }
        else
        {
            ret = AddContact(add_data_ptr);
        }
    }
    return ret;
}
#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
/*****************************************************************************/
//  Description : create instant messanger editbox
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateImEditBox(MMI_HANDLE_T win_id,MMIPB_CONTACT_T *contact_ptr)
{
    uint16 hor_space = 0;
    uint16 max_ucs2_len = MMIPB_IM_MAX_LEN>>1;
    uint16 max_alpha_len = MMIPB_IM_MAX_LEN;
    uint8 input_type = GUIIM_TYPE_DIGITAL;
    MMI_STRING_T edit_str_t = {0};
    wchar forbid_char_value[]= {'*', '#', 'P', 'W'};//禁止换行回车的输入
        
    if(PNULL == contact_ptr)
    {
        return;
    }
    edit_str_t.wstr_len = contact_ptr->imessager.wstr_len;
    edit_str_t.wstr_ptr = contact_ptr->imessager.wstr;

    input_type = GUIIM_TYPE_SET_ALL;

    AppendFormEditor(&edit_str_t, 
                max_ucs2_len,
                max_alpha_len, 
                input_type,
                FALSE,
                MMIPB_IM_EDIT_ID,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_SUB_FORM_ID),
                MMIPB_ITEM_INSTANTMSG,
                win_id);
    
    GUIFORM_SetSpace(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_FORM_ID),
        &hor_space,PNULL);
    GUIEDIT_ConfigDisableChar(MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_EDIT_ID), forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
}
#endif

/*****************************************************************************/
//  Description : set pb contact photo
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetContactFilePhoto(MMI_HANDLE_T win_id,
    MMIPB_CONTACT_T *contact_ptr,
    wchar *file_path_ptr,
    uint16 file_path_len,
    MMIPB_CUSTOM_PHOTO_T *photo_ptr
    )
{
    GUIANIM_FILE_INFO_T file_info = {0};
    uint16 path_len = 0;
    MMI_HANDLE_T        applet_handle = 0;
    
    if(PNULL == contact_ptr||PNULL==file_path_ptr||PNULL == photo_ptr||0==file_path_len)
    {
        return;
    }
    
    path_len = MIN(file_path_len, MMIFILE_FULL_PATH_MAX_LEN);
    
    //contact_ptr->photo_id_flag = (contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_CLEAN)+
    //                    MMIPB_PHOTO_ID_EXIST;
    contact_ptr->photo_id_flag = contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_CLEAN;
                    
    SCI_MEMSET(photo_ptr->file_name,0,sizeof(photo_ptr->file_name));

    photo_ptr->is_valid = TRUE;
    photo_ptr->file_name_len = path_len;
    MMIAPICOM_Wstrncpy(photo_ptr->file_name,file_path_ptr,path_len);
 
   // MMIPB_AddContact_PhotoInfo(data_ptr->contact_ptr,data_ptr->photo_ptr);
        file_info.full_path_wstr_len = path_len;
        file_info.full_path_wstr_ptr = file_path_ptr;
#ifdef MMI_PDA_SUPPORT
        
    SetPhotoAnimParam(win_id,PNULL,&file_info);
#else
	// + Bug 125726 
	//after selecting photo file from album, 判断是否空间充足
    if(MMIPB_ERROR_NO_SPACE == MMIPB_AddContact_PhotoInfo(contact_ptr, photo_ptr))
    {
        
        if(MMIPB_GetCurAppletHandle(&applet_handle))
        {
            //MMIPB_DisplaybannerInfo(0, STR_NO_SPACE_EXT01, PNULL);
            MMIPB_OpenCommonDialogWin(applet_handle, STR_NO_SPACE_EXT01);
            //MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,
                //TXT_NO_SPACE,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
        }
    }
	//- Bug 125726 
    MMK_SendMsg(win_id,MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG,PNULL);
#endif                
}

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
LOCAL void ActiveNextEditCtrl(MMI_WIN_ID_T  win_id,
        uint16 storage_id)
{
     uint8 i = 0;
    uint8 active_form_index = 0;
    uint8 next_form_index = 0;
    uint8 next_next_form_index = 0;
    uint8 total_count = sizeof(s_contact_eidt_form_id)/sizeof(s_contact_eidt_form_id[0]);
    MMI_CTRL_ID_T active_ctrl_id = 0;

    active_ctrl_id = MMK_GetActiveCtrlId(win_id);

    for(i = 0; i < total_count; i++)
    {
        if(active_ctrl_id == s_contact_eidt_form_id[i])
        {
            active_form_index = i;
            break;
        }
    }
    
    if(i== total_count)
    {
        GUIWIN_SetSoftkeyBtnGray(win_id,1,FALSE); 
        MMK_SetAtvCtrlEx(win_id,MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[0]),TRUE); 
        return;
    } 
    else if(active_form_index == total_count - 1 )
    {
        next_form_index = 0;
    }

    else
    {        
        for(i = active_form_index+1; i <= total_count; i++)
        {
            if(0 != MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[i]))
            {
                next_form_index = i;
                break;
            }
        }
        for(i = next_form_index+1; i <= total_count; i++)
        {
            if(0 != MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[i]))
            {
                next_next_form_index = i;
                break;
            }
        }
    }

    if((next_form_index == 0) || (next_next_form_index == 0) || (0 == MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[next_form_index])))
    {
       GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE); 
    }
#ifndef PB_SUPPORT_LOW_MEMORY 
    else if(s_contact_eidt_form_id[next_next_form_index]>=MMIPB_URL_EDITBOX_ID)  
    {
        uint32 item_data = 0;
        GUILIST_GetItemData(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_MORE_SWITCH_LIST_ID),
                            0,&item_data);
        if (0 == item_data)
        {
            GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
        }
    }
#endif
   /* else
    {
        uint32 item_data = 0;
        GUILIST_GetItemData(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_MORE_SWITCH_LIST_ID),
                            0,&item_data);
        if (0 == item_data)
        {
            GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
            return;
        }
    }*/
    if(next_form_index !=0)
    {
        MMK_SetAtvCtrlEx(win_id,MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[next_form_index]),TRUE); 
    }
   /* if(MMIPB_EDITBOX_MEMO_ID == s_contact_eidt_form_id[next_form_index]
        || active_form_index == total_count - 1)
    {
        GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
        return;
    }
    else if(MMIPB_IM_EDIT_ID == s_contact_eidt_form_id[next_form_index])
    {
            uint32 item_data = 0;
            GUILIST_GetItemData(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_MORE_SWITCH_LIST_ID),
                                0,&item_data);
            if (0 == item_data)
            {
                GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
                return;
            }
    }
    else if(MMIPB_EDITBOX_NUM1_ID == s_contact_eidt_form_id[next_form_index])
    {
            if (MMIPB_IsSIMContact(storage_id)
                #if defined (MMIPB_SIMDN_SUPPORT) 
                || MMIPB_IsMsisdnContact(storage_id)
                #endif
                #if defined (MMIPB_SIMDN_SUPPORT)
                || MMIPB_IsFdnContact(storage_id)
                #endif
                )
            {
                GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
                return;
            }
    } */
       
}
#endif
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
void UpdateSoftkeyStatus(MMI_HANDLE_T win_id)
{
    MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
    uint8 total_count = sizeof(s_contact_eidt_form_id)/sizeof(MMI_CTRL_ID_T);
    uint16 next_form_index = 0;
    uint16 active_form_index = 0;
    int i = 0;
    int j = 0;
    
    for(i = 0; i < total_count; i++)
    {
        if(active_ctrl_id == s_contact_eidt_form_id[i])
        {
            active_form_index = i;
            break;
        }
    }
    for(j = active_form_index+1; j <= total_count; j++)
    {
        if(0 != MMK_GetCtrlHandleByWin(win_id,s_contact_eidt_form_id[j]))
        {
            next_form_index = j;
            break;
        }
    }
#ifndef PB_SUPPORT_LOW_MEMORY 
    if(s_contact_eidt_form_id[next_form_index]>=MMIPB_URL_EDITBOX_ID)  
    {
        uint32 item_data = 0;
        GUILIST_GetItemData(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_MORE_SWITCH_LIST_ID),
                            0,&item_data);
        if (0 == item_data)
        {
            GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
        }
    }
    else 
#endif
    if(next_form_index != 0)
    {
       GUIWIN_SetSoftkeyBtnGray(win_id,1,FALSE);  
    }
    else
    {
        GUIWIN_SetSoftkeyBtnGray(win_id,1,TRUE);
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactAddWinHook
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{

    switch(msg_id)
    {
    case MSG_TP_PRESS_UP:
        {
            UpdateSoftkeyStatus(win_id);
        }
        break;
    default:
        break;
    }

    return MMI_RESULT_FALSE;
   
}   
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItemForAddEdit(MMI_CTRL_ID_T       ctrl_id,
	uint32              item_id,
	uint32*             item_index,      
	GUIITEM_STYLE_E     item_style,
	MMI_TEXT_ID_T       text1_id,
	MMI_TEXT_ID_T       text2_id,
	const MMI_STRING_T*  text2_ptr,
	MMI_IMAGE_ID_T      icon_id,
	GUIANIM_FILE_INFO_T* anim_path
)
{    
	GUILIST_ITEM_T      item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	uint16               content_index = 0;
	
	item_t.item_style    = item_style;
	item_t.item_data_ptr = &item_data;
	item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;
	item_data.softkey_id[MIDDLE_BUTTON] = TXT_NULL;
	item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
	item_t.user_data     = item_id;
	
	
	if(item_style == GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS)
	{
		content_index = 0;
		
		if(PNULL != anim_path && PNULL != anim_path->full_path_wstr_ptr && 0 != anim_path->full_path_wstr_len)
		{
			//need admin
			item_data.item_content[content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
			item_data.item_content[content_index].item_data.anim_path_ptr = anim_path;
			item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_EDIT_MK;
		}
		else 
		{
			item_data.item_content[content_index].item_data_type =GUIITEM_DATA_IMAGE_ID;
			item_data.item_content[content_index].item_data.image_id = icon_id;
			item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_ADD_MK;
		}
		content_index++;
		
		item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		
		GUILIST_AppendItem( ctrl_id, &item_t );
		(*item_index)++;
		
		
	}
	else if(item_style == GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS)
	{
		
		
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		content_index++;
		if(PNULL != text2_ptr && text2_ptr->wstr_len > 0)
		{
			item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
			item_data.item_content[content_index].item_data.text_buffer = *text2_ptr;
		}
		else
		{
			item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
			item_data.item_content[content_index].item_data.text_id = text2_id;
		}
		item_data.item_content[content_index].is_opacity = TRUE;
		GUILIST_SetItemContentOpacityFontColor(&item_data,content_index,255*0.6,255*0.6);
		GUILIST_AppendItem( ctrl_id, &item_t );
		(*item_index)++;
	}
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ReplaceListItemForAddEdit(MMI_CTRL_ID_T       ctrl_id,
	uint32              item_id,
	uint32             item_index,      
	GUIITEM_STYLE_E     item_style,
	MMI_TEXT_ID_T       text1_id,
	MMI_TEXT_ID_T       text2_id,
	const MMI_STRING_T*  text2_ptr,
	MMI_IMAGE_ID_T      icon_id,
	GUIANIM_FILE_INFO_T* anim_path
)
{    
	GUILIST_ITEM_T      item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	uint16               content_index = 0;
	
	item_t.item_style    = item_style;
	item_t.item_data_ptr = &item_data;
	item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;
	item_data.softkey_id[MIDDLE_BUTTON] = TXT_NULL;
	item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
	item_t.user_data     = item_id;
	
	
	if(item_style == GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS)
	{
		content_index = 0;
		
		if(PNULL != anim_path && PNULL != anim_path->full_path_wstr_ptr && 0 != anim_path->full_path_wstr_len)
		{
			//need admin
			item_data.item_content[content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
			item_data.item_content[content_index].item_data.anim_path_ptr = anim_path;
			item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_EDIT_MK;
		}
		else 
		{
			item_data.item_content[content_index].item_data_type =GUIITEM_DATA_IMAGE_ID;
			item_data.item_content[content_index].item_data.image_id = icon_id;
			item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_ADD_MK;
		}
		content_index++;
		
		item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		
		GUILIST_ReplaceItem( ctrl_id, &item_t,item_index);
	}
	else if(item_style == GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS)
	{
		
		content_index = 0;
		item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
		item_data.item_content[content_index].item_data.text_id = text1_id;
		content_index++;
		
		if(PNULL != text2_ptr && text2_ptr->wstr_len > 0)
		{
			item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
			item_data.item_content[content_index].item_data.text_buffer = *text2_ptr;
		}
		else
		{
			item_data.item_content[content_index].item_data_type =GUIITEM_DATA_TEXT_ID;
			item_data.item_content[content_index].item_data.text_id = text2_id;
		}
		item_data.item_content[content_index].is_opacity = TRUE;
		GUILIST_SetItemContentOpacityFontColor(&item_data,content_index,255*0.6,255*0.6);
		GUILIST_ReplaceItem( ctrl_id, &item_t,item_index);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinSetEditorSoftkey(MMI_HANDLE_T editor_ctr_id)
{
	GUIEDIT_SetSoftkeyEx(editor_ctr_id, 
		0, 
		0,
		STXT_OPTION,
		STXT_OPTION,
		IMAGE_NULL,
		IMAGE_NULL, 
		EDIT_BUTTON_TEXT_ID, 
		PNULL
		);
	GUIEDIT_SetSoftkeyEx(editor_ctr_id, 
		1, 
		0,
		TXT_NULL,
		TXT_NULL,
		IMAGE_NULL,
		IMAGE_NULL, 
		EDIT_BUTTON_TEXT_ID, 
		PNULL
		);
	GUIEDIT_SetSoftkeyEx(editor_ctr_id, 
		2, 
		1,
		TXT_COMMON_COMMON_BACK,
		TXT_COMMON_CLEAR,
		IMAGE_NULL,
		IMAGE_NULL, 
		EDIT_BUTTON_TEXT_ID, 
		PNULL
		);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinInitAvatarListCtrl
(
 MMI_HANDLE_T avatar_ctr_id, 
 MMIPB_MENU_DATA_INFOR_T *add_data_ptr,
 MMIPB_FIELD_USAGE_INOF_T *field_infor
 )
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
#ifdef DRM_SUPPORT
	MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
	MMIDRMFILE_HANDLE       drm_handle = 0;
	DRM_PERMISSION_MODE_E   drm_permission = DRM_PERMISSION_NONE;
	BOOLEAN                 is_valid = FALSE;
	BOOLEAN                 is_drm_permission = TRUE;
#endif
	uint32 item_index = 0;
	MMI_STRING_T text2_str = {0};
	wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
	MMI_STRING_T    str_t = {0};
	
	GUILIST_SetMaxItem(
		avatar_ctr_id, 
		1,
		FALSE
		);
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		if(field_infor->photo_free_flag) 
		{
			GUIANIM_FILE_INFO_T anim_path = {0};
			BOOLEAN     is_photo_id_exist = FALSE;
			
			if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST))
			{
				if (PNULL!=add_data_ptr->photo_ptr&&add_data_ptr->photo_ptr->file_name_len>0)
				{
					is_photo_id_exist = TRUE;
					//判断DRM文件，是否版权过期 @Bug 114162 
#ifdef DRM_SUPPORT
					file_type = MMIAPIFMM_GetFileTypeByFileName(add_data_ptr->photo_ptr->file_name, add_data_ptr->photo_ptr->file_name_len);
					if(MMIFMM_FILE_TYPE_DRM == file_type)
					{
						drm_handle = SFS_CreateFile(add_data_ptr->photo_ptr->file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
						if(SFS_INVALID_HANDLE != drm_handle)
						{
							drm_permission = MMIAPIDRM_GetFilePermission(drm_handle, PNULL);    
							is_valid = MMIAPIDRM_IsRightsValid(drm_handle, PNULL, TRUE, drm_permission);
							if (TRUE != is_valid)
							{
								is_drm_permission = FALSE;
							}
						}
						SCI_TRACE_LOW("[MMIPB] SetContactAddOrEditItem is_drm_permission = %d", is_drm_permission);
						SFS_CloseFile(drm_handle);
					}       
#endif

				}
			}
			
			if(is_photo_id_exist
#ifdef DRM_SUPPORT
				&& is_drm_permission 
#endif
				&& PNULL != add_data_ptr->photo_ptr)
			{	            
				anim_path.full_path_wstr_len = add_data_ptr->photo_ptr->file_name_len;
				anim_path.full_path_wstr_ptr = add_data_ptr->photo_ptr->file_name; 
			}			
			AppendListItemForAddEdit(avatar_ctr_id,
				MMIPB_ADD_EDIT_INDEX_AVATAR,
				&item_index,      
				GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS,
				is_photo_id_exist ? STR_LIST_EDIT_IMAGE_EXT01:STR_LIST_ADD_IMAGE,
				TXT_NULL,
				PNULL,
				IMAGE_LISTITEM_AVATAR_INITIALS,
				&anim_path
				);			
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinInitRingListCtrl
(
 MMI_HANDLE_T ring_ctr_id, 
 MMIPB_MENU_DATA_INFOR_T *add_data_ptr,
 MMIPB_FIELD_USAGE_INOF_T *field_infor
 )
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	uint32 item_index = 0;
	MMI_STRING_T text2_str = {0};
	wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
	MMI_STRING_T    str_t = {0};
	
	GUILIST_SetMaxItem(
		ring_ctr_id, 
		1,
		FALSE
		);
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		item_index = 0;
		if(field_infor->ring_free_falg)
		{
			if(contact_ptr->ring_id_flag&MMIPB_FIXED_RING_FLAG)
			{
				if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) == 0 )
				{
					AppendListItemForAddEdit(ring_ctr_id,
						MMIPB_ADD_EDIT_INDEX_RING,
						&item_index,     
						GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
						STXT_HEADER_RINGING_TYPE,
						STR_NO_SOUND_EXT01,
						PNULL,
						IMAGE_NULL,
						PNULL
						);				
				}
				else if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
				{
					str_t.wstr_ptr = wstr; 
					str_t.wstr_len = MMIAPISET_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK),str_t.wstr_ptr, MMIPB_MAX_STRING_LEN); 
					AppendListItemForAddEdit(ring_ctr_id,
						MMIPB_ADD_EDIT_INDEX_RING,
						&item_index,      
						GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
						STXT_HEADER_RINGING_TYPE,
						TXT_NULL,
						&str_t,
						IMAGE_NULL,
						PNULL
						);
					
				}
				else
				{
					AppendListItemForAddEdit(ring_ctr_id,
						MMIPB_ADD_EDIT_INDEX_RING,
						&item_index,      
						GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
						STXT_HEADER_RINGING_TYPE,
						STR_COMMON_LIST_DEFAULT,
						PNULL,
						IMAGE_NULL,
						PNULL
						);
					
				}
				
			}
			else if((PNULL != add_data_ptr->ring_ptr) && (add_data_ptr->ring_ptr->is_valid))
			{
				//text2_str.wstr_ptr = add_data_ptr->ring_ptr->file_name;
				//text2_str.wstr_len = add_data_ptr->ring_ptr->file_name_len;
				wchar file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
				uint16 file_name_len = 0;
				MMIAPIFMM_SplitFullPath(add_data_ptr->ring_ptr->file_name, add_data_ptr->ring_ptr->file_name_len,
					NULL, NULL, NULL, NULL,
					file_name, &file_name_len);
				text2_str.wstr_ptr = file_name;
				text2_str.wstr_len = file_name_len;
				AppendListItemForAddEdit(ring_ctr_id,
					MMIPB_ADD_EDIT_INDEX_RING,
					&item_index,      
					GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
					STXT_HEADER_RINGING_TYPE,
					TXT_NULL,
					&text2_str,
					IMAGE_NULL,
					PNULL
					);
				
			}
			else
			{
				AppendListItemForAddEdit(ring_ctr_id,
					MMIPB_ADD_EDIT_INDEX_RING,
					&item_index,      
					GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
					STXT_HEADER_RINGING_TYPE,
					STR_COMMON_LIST_DEFAULT,
					PNULL,
					IMAGE_NULL,
					PNULL
					);
			}
		}
		else
		{
			AppendListItemForAddEdit(ring_ctr_id,
				MMIPB_ADD_EDIT_INDEX_RING,
				&item_index,      
				GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
				STXT_HEADER_RINGING_TYPE,
				STR_COMMON_LIST_DEFAULT,
				PNULL,
				IMAGE_NULL,
				PNULL
				);
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinInitGroupListCtrl
(
 MMI_HANDLE_T group_ctr_id, 
 MMIPB_MENU_DATA_INFOR_T *add_data_ptr,
 MMIPB_FIELD_USAGE_INOF_T *field_infor
 )
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	uint32 item_index = 0;
	MMI_STRING_T text2_str = {0};
	wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
	MMI_STRING_T    str_t = {0};
	
	GUILIST_SetMaxItem(
		group_ctr_id, 
		1,
		FALSE
		);
	
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		item_index = 0;
		SCI_MEMSET(wstr, 0x00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
		str_t.wstr_len = MMIPB_GetMultiGrouName(contact_ptr->union_t.group, wstr);
		str_t.wstr_ptr = wstr;        
		AppendListItemForAddEdit(group_ctr_id,
			MMIPB_ADD_EDIT_INDEX_GROUP,
			&item_index,      
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
			STR_CONTA_GROUP_EXT01,
			TXT_NULL,
			&str_t,
			IMAGE_NULL,
			PNULL
			);
	}
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinInitPreferredSimListCtrl
(
 MMI_HANDLE_T preferred_sim_ctr_id, 
 MMIPB_MENU_DATA_INFOR_T *add_data_ptr,
 MMIPB_FIELD_USAGE_INOF_T *field_infor
 )
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	uint32 item_index = 0;
	MMI_STRING_T text2_str = {0};
	wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
	MMI_STRING_T    str_t = {0};
	GUILIST_SetMaxItem(
		preferred_sim_ctr_id, 
		1,
		FALSE
		);
	
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		item_index = 0;
		item_index++;
		if(0 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_GROUP_LIST_NONE_EXT01, &str_t);
		}
		else if(1 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE, &str_t);
		}
		else if(2 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO, &str_t);
		}
		
		AppendListItemForAddEdit(preferred_sim_ctr_id,
			MMIPB_ADD_EDIT_INDEX_PREFERRED_SIM,
			&item_index,      
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
			STR_SIM_LIST_PRE_SIM_EXT02,
			TXT_NULL,
			&str_t,
			IMAGE_NULL,
			PNULL
			);
	}
}
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinInitEditorCtrl
(
 MMI_WIN_ID_T win_id, 
 MMIPB_MENU_DATA_INFOR_T *add_data_ptr,
 MMIPB_FIELD_USAGE_INOF_T *field_infor
 )
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	GUIFORM_CHILD_HEIGHT_T birthday_ctr_height = {66,GUIFORM_CHILD_HEIGHT_FIXED};
	MMI_STRING_T title_str = {0};
	
	MMI_HANDLE_T win_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_WIN_FORM_ID);
	MMI_HANDLE_T first_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_FIRST_NAME_ID);
	MMI_HANDLE_T last_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_LAST_NAME_ID);
	MMI_HANDLE_T mobile_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_MOBILE_ID);
	MMI_HANDLE_T business_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BUSINESS_ID);
	MMI_HANDLE_T home_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_HOME_ID);
#ifdef MMIPB_MAIL_SUPPORT	
	MMI_HANDLE_T email_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_EMAIL_ID);
#endif
	MMI_HANDLE_T birthday_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BIRTHDAY_ID);
	{
		uint8          i = 0;
		GUIEDIT_SetString(first_name_ctr_id, contact_ptr->name.wstr, contact_ptr->name.wstr_len);
#ifdef MMI_PB_SURNAME_SUPPORT
		GUIEDIT_SetString(last_name_ctr_id, contact_ptr->surname.wstr, contact_ptr->surname.wstr_len);
#endif /*MMI_PB_SURNAME_SUPPORT*/		
		for(i = 0;i<MMIPB_MAX_NV_PHONE_NUM;i++)
		{
			
			if(MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton,contact_ptr->number[i].number_len))
			{
				MN_NUMBER_TYPE_E ton = 0;
				MMIPB_STRING_T  string_t = {0};
				MMI_STRING_T    com_string_t = {0};
				wchar           num_wstr[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
				
				ton = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[i].npi_ton);    
				string_t.strlen = MMIAPICOM_GenDispNumber(
					(ton == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : ton),
					contact_ptr->number[i].number_len, 
					contact_ptr->number[i].number,
					string_t.strbuf,
					MMIPB_NUMBER_MAX_STRING_LEN+2
					);
				
				com_string_t.wstr_ptr = num_wstr;
				com_string_t.wstr_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, string_t.strlen);   
				MMI_STRNTOWSTR(com_string_t.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN+2, 
					(const uint8*)string_t.strbuf, MMIPB_MAX_STRING_LEN +2, 
					com_string_t.wstr_len);	
				
				if(contact_ptr->num_type[i] == MMIPB_ADN_NUMBER)
				{
					GUIEDIT_SetString(mobile_ctr_id,com_string_t.wstr_ptr, com_string_t.wstr_len);
				}
				else if(contact_ptr->num_type[i] == MMIPB_OFFICE_NUMBER)
				{
					GUIEDIT_SetString(business_ctr_id, com_string_t.wstr_ptr, com_string_t.wstr_len);
				}
				else if(contact_ptr->num_type[i] == MMIPB_HOME_NUMBER)
				{
					GUIEDIT_SetString(home_ctr_id, com_string_t.wstr_ptr, com_string_t.wstr_len);
				}
			}
		}
	}
	//mail
#ifdef MMIPB_MAIL_SUPPORT	
	GUIEDIT_SetString(email_ctr_id, contact_ptr->mail.wstr, contact_ptr->mail.wstr_len);
#endif
	//birthday
#ifndef PB_SUPPORT_LOW_MEMORY	
	GUIEDIT_SetDateRange(birthday_ctr_id,GUIEDIT_DATE_YEAR_RANGE_QUERY);
	GUIFORM_SetChildHeight(win_form_ctr_id, birthday_ctr_id, &birthday_ctr_height);
	CTRLDATEEDIT_SetIsFullDateEditorFlag(birthday_ctr_id, TRUE);
	MMI_GetLabelTextByLang(STR_LABEL_BIRTHDAY_EXT01, &title_str);
	CTRLDATEEDIT_SetTitle(birthday_ctr_id,&title_str);
	if(contact_ptr->birth.wstr_len)
	{
		uint16 year = 0;
		uint8 month = 0;
		uint8 day = 0;
		char   date_str[MMIPB_SNS_BIRTH_LEN + 1]; 
		MMIAPICOM_WstrToStr(contact_ptr->birth.wstr, date_str);
		year = atoi(date_str);
		month = atoi(&date_str[5]);
		day = atoi(&date_str[8]);
		GUIEDIT_SetDate(birthday_ctr_id, year, month, day);
	}
	else
	{
	    GUIEDIT_SetDate(birthday_ctr_id, 1900, 01, 01);
	}
#endif	
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddEditFdnWinMidSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
	BOOLEAN  result = FALSE;
	uint32 i = 0;
	MMI_HANDLE_T applet_handle = NULL;
	MMIPB_APPLET_T*    applet_ptr = PNULL;        
	MMI_QUEUE_NODE_T * pb_queue = MMIPB_GetAppletQueue();
	
	if(pb_queue != PNULL)
	{
		for (i=0; i< pb_queue->current_count; i++)
		{
			applet_handle =  *(MMI_HANDLE_T *)MMIQUEUE_PeekQueue(pb_queue, i);
			applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);  
			if(PNULL != applet_ptr)
			{
				MMI_HANDLE_T win_id = MMK_GetWinHandle(applet_handle, MMIPB_ADD_IN_NV_WIN_ID);
				if(MMK_IsOpenWin(win_id))
				{
					MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
					MMIPB_CONTACT_T *contact_ptr = PNULL;
					
					add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
					if(PNULL != add_data_ptr && PNULL!= add_data_ptr->contact_ptr)
					{
						contact_ptr = add_data_ptr->contact_ptr;
						if(0 == contact_ptr->name.wstr_len && 0 == contact_ptr->number[0].number_len)
						{
							*text_id_ptr = STXT_SOFTKEY_PB_MK;
							result = TRUE;							
						}
						else
						{
							*text_id_ptr = STXT_SOFTKEY_SAVE_MK;
							result = TRUE;							
						}
						
					}
					break;
				}
			}
		} 
		
	}
	return (result);
} 

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAddEditWinCtrl(MMI_WIN_ID_T win_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	uint32 item_index = 0;
	uint16 new_id = 0;
	MMIPB_FIELD_USAGE_INOF_T field_infor = {0};
	MMI_STRING_T text2_str = {0};
	wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
	MMI_STRING_T    str_t = {0};
	GUI_RECT_T  client_rect = MMITHEME_GetClientRect();
	uint16 ver_space = 0;
	uint16 hor_space = 0;
	GUI_BG_T bg = {0};
	MMIPB_APPLET_T* applet_ptr =(MMIPB_APPLET_T*)MMK_GetAppletPtr(add_data_ptr->applet_handle);
	MMISET_DATE_DISPLAY_TYPE_E date_display_type = MMIAPISET_GetDateDisplayType();
	GUIEDIT_DATE_STYLE_E date_style = GUIEDIT_DATE_STYLE_NULL;
	wchar           forbid_char_value[]= {0x0d, 0x0a};
	
	MMI_HANDLE_T win_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_WIN_FORM_ID);
	MMI_HANDLE_T avatar_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_AVATAR_ID);
	MMI_HANDLE_T first_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_FIRST_NAME_ID);
	MMI_HANDLE_T last_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_LAST_NAME_ID);
	MMI_HANDLE_T mobile_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_MOBILE_ID);
	MMI_HANDLE_T business_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BUSINESS_ID);
	MMI_HANDLE_T home_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_HOME_ID);
#ifdef MMIPB_MAIL_SUPPORT	
	MMI_HANDLE_T email_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_EMAIL_ID);
#endif
	MMI_HANDLE_T birthday_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BIRTHDAY_ID);
	MMI_HANDLE_T ring_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_RING_ID);
	MMI_HANDLE_T group_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_GROUP_ID);
#ifdef MMI_ORIG_UNISOC_SUPPORT
	MMI_HANDLE_T preferred_sim_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_PREFERRED_SIM_ID);
#endif

	if(PNULL == applet_ptr)
	{
		return FALSE;
	}
    if (MMIPB_STORAGE_PHONE != MMIPB_GET_STORAGE(contact_ptr->storage_id))
    {
        GUIEDIT_SetTextMaxLen(first_name_ctr_id, MMIPB_SIM_NAME_UNICODE_MAX_LEN, MMIPB_SIM_NAME_ASCII_MAX_LEN);
    }
    else
    {
        GUIEDIT_SetTextMaxLen(first_name_ctr_id, MMIPB_NAME_MAX_LEN, MMIPB_NAME_MAX_LEN);
    }
	GUIEDIT_SetTextMaxLen(last_name_ctr_id, MMIPB_NAME_MAX_LEN, MMIPB_NAME_MAX_LEN);
	GUIEDIT_SetTextMaxLen(mobile_ctr_id, MMIPB_PHONE_NUM_MAX_LEN, MMISET_PHONE_NUM_MAX_LEN);
	GUIEDIT_SetTextMaxLen(business_ctr_id, MMIPB_PHONE_NUM_MAX_LEN, MMISET_PHONE_NUM_MAX_LEN);
	GUIEDIT_SetTextMaxLen(home_ctr_id, MMIPB_PHONE_NUM_MAX_LEN, MMISET_PHONE_NUM_MAX_LEN);
#ifdef MMIPB_MAIL_SUPPORT	
	GUIEDIT_SetTextMaxLen(email_ctr_id, MMIPB_MAX_MAIL_LEN, MMIPB_MAX_MAIL_LEN);
#endif

	AddEditWinSetEditorSoftkey(first_name_ctr_id);
	AddEditWinSetEditorSoftkey(last_name_ctr_id);
	AddEditWinSetEditorSoftkey(mobile_ctr_id);
	AddEditWinSetEditorSoftkey(business_ctr_id);
	AddEditWinSetEditorSoftkey(home_ctr_id);
#ifdef MMIPB_MAIL_SUPPORT	
	AddEditWinSetEditorSoftkey(email_ctr_id);
#endif
	if(add_data_ptr->is_need_pin_verify)
	{
		GUIEDIT_SetSoftkey(first_name_ctr_id,1,0,TXT_NULL,TXT_NULL,AddEditFdnWinMidSoftkey);
		GUIEDIT_SetSoftkey(mobile_ctr_id,1,0,TXT_NULL,TXT_NULL,AddEditFdnWinMidSoftkey);
	}

	GUIEDIT_SetDefaultStringById(first_name_ctr_id, STR_CONTA_ADD_NAME_EXT01);
	GUIEDIT_SetDefaultStringById(last_name_ctr_id, STR_CONTA_ADD_NAME_EXT01);
	GUIEDIT_SetDefaultStringById(mobile_ctr_id, STR_CONTA_ADD_NUMBER_EXT01);
	GUIEDIT_SetDefaultStringById(business_ctr_id, STR_CONTA_ADD_NUMBER_EXT01);
	GUIEDIT_SetDefaultStringById(home_ctr_id, STR_CONTA_ADD_NUMBER_EXT01);
#ifdef MMIPB_MAIL_SUPPORT	
	GUIEDIT_SetDefaultStringById(email_ctr_id, STR_LIST_ADD_MAIL_ADDRESS_EXT01);
#endif

	client_rect.bottom = client_rect.bottom -6;
	GUIFORM_SetRect(win_form_ctr_id, &client_rect);
	bg.bg_type = GUI_BG_COLOR;
	bg.color = MMITHEME_GetTransparenceColor();
	GUIFORM_SetBg(win_form_ctr_id,&bg);
	//GUICTRL_SetProgress(MMK_GetCtrlPtr(win_form_ctr_id),FALSE);
	GUIFORM_SetCircularHandleUpDown(win_form_ctr_id,TRUE);
	GUIFORM_SetMargin(win_form_ctr_id,0);
	GUIFORM_PermitChildBorder(win_form_ctr_id,FALSE);
	//GUIFORM_PermitChildBg(win_form_ctr_id,TRUE);
	
	GUIFORM_SetChildSpace(win_form_ctr_id, first_name_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, last_name_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, mobile_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, business_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, home_ctr_id,&hor_space, &ver_space);
#ifdef MMIPB_MAIL_SUPPORT	
	GUIFORM_SetChildSpace(win_form_ctr_id, email_ctr_id,&hor_space, &ver_space);
#endif
	GUIFORM_SetChildSpace(win_form_ctr_id, birthday_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, avatar_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, ring_ctr_id,&hor_space, &ver_space);
	GUIFORM_SetChildSpace(win_form_ctr_id, group_ctr_id,&hor_space, &ver_space);
#ifdef MMI_ORIG_UNISOC_SUPPORT
	GUIFORM_SetChildSpace(win_form_ctr_id, preferred_sim_ctr_id,&hor_space, &ver_space);
#endif
	if(!MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		if(!MMIAPIPB_IsDisplayDebugMenu())
		{
			GUIFORM_SetChildDisplay(win_form_ctr_id, business_ctr_id, GUIFORM_CHILD_DISP_HIDE);
			GUIFORM_SetChildDisplay(win_form_ctr_id, home_ctr_id, GUIFORM_CHILD_DISP_HIDE);
#ifdef MMIPB_MAIL_SUPPORT 			
			GUIFORM_SetChildDisplay(win_form_ctr_id, email_ctr_id, GUIFORM_CHILD_DISP_HIDE);
#endif
		}
		GUIFORM_SetChildDisplay(win_form_ctr_id, avatar_ctr_id, GUIFORM_CHILD_DISP_HIDE);
		GUIFORM_SetChildDisplay(win_form_ctr_id, last_name_ctr_id, GUIFORM_CHILD_DISP_HIDE);
		GUIFORM_SetChildDisplay(win_form_ctr_id, birthday_ctr_id, GUIFORM_CHILD_DISP_HIDE);
		GUIFORM_SetChildDisplay(win_form_ctr_id, ring_ctr_id, GUIFORM_CHILD_DISP_HIDE);
		GUIFORM_SetChildDisplay(win_form_ctr_id, group_ctr_id, GUIFORM_CHILD_DISP_HIDE);
#ifdef MMI_ORIG_UNISOC_SUPPORT
		GUIFORM_SetChildDisplay(win_form_ctr_id, preferred_sim_ctr_id, GUIFORM_CHILD_DISP_HIDE);
#endif
	}

	/*GUIEDIT_PermitBorder(first_name_ctr_id, FALSE);
	GUIEDIT_PermitBorder(last_name_ctr_id, FALSE);
	GUIEDIT_PermitBorder(mobile_ctr_id, FALSE);
	GUIEDIT_PermitBorder(business_ctr_id, FALSE);
	GUIEDIT_PermitBorder(home_ctr_id, FALSE);
#ifdef MMIPB_MAIL_SUPPORT 	
	GUIEDIT_PermitBorder(email_ctr_id, FALSE);
#endif
	GUIEDIT_PermitBorder(birthday_ctr_id, FALSE);*/
	
	CTRLBASEEDIT_SetLabelId(win_id,first_name_ctr_id,STR_CONTA_FIRST_NAME_EXT01,FALSE);
	GUIEDIT_SetStyle(first_name_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
	GUIEDIT_ConfigDisableChar(first_name_ctr_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
	CTRLBASEEDIT_SetLabelId(win_id,last_name_ctr_id,STR_CONTA_LAST_NAME_EXT01,FALSE);
	GUIEDIT_SetStyle(last_name_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
	GUIEDIT_ConfigDisableChar(last_name_ctr_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
	CTRLBASEEDIT_SetLabelId(win_id,mobile_ctr_id,STXT_NUMBER_TYPE_MOBILE,FALSE);
	GUIEDIT_SetStyle(mobile_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
#ifndef MMIPB_RMVITEM_SUPPORT
	CTRLBASEEDIT_SetLabelId(win_id,business_ctr_id,STXT_NUMBER_TYPE_OFFICE,FALSE);
	GUIEDIT_SetStyle(business_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
	CTRLBASEEDIT_SetLabelId(win_id,home_ctr_id,STXT_NUMBER_TYPE_HOME,FALSE);
	GUIEDIT_SetStyle(home_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
#else
    CTRLBASEEDIT_SetLabelId(win_id,business_ctr_id,STXT_NUMBER_TYPE_HOME,FALSE);
    GUIEDIT_SetStyle(business_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
    CTRLBASEEDIT_SetLabelId(win_id,home_ctr_id,STXT_NUMBER_TYPE_OFFICE,FALSE);
    GUIEDIT_SetStyle(home_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
#endif
#ifdef MMIPB_MAIL_SUPPORT 	
	CTRLBASEEDIT_SetLabelId(win_id,email_ctr_id,STR_LABEL_EMAIL_EXT01,FALSE);
	GUIEDIT_SetStyle(email_ctr_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);
	GUIEDIT_SetIm(email_ctr_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
	GUIEDIT_ConfigDisableChar(email_ctr_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
#endif

	if(MMISET_DATE_YMD == date_display_type)
	{
		date_style = GUIEDIT_DATE_STYLE_YMD;
	}
	else if(MMISET_DATE_MDY == date_display_type)
	{
		date_style = GUIEDIT_DATE_STYLE_MDY;
	}
	else if(MMISET_DATE_DMY == date_display_type)
	{
		date_style = GUIEDIT_DATE_STYLE_DMY;
	}
	GUIEDIT_SetDateStyle(birthday_ctr_id,PNULL,&date_style,FALSE);

	new_id = MMIPB_GetFieldUageInfo(contact_ptr->storage_id,contact_ptr->contact_id,&field_infor);
	
	if(new_id>0)
	{
		contact_ptr->contact_id = new_id;
	}
	applet_ptr->instance.cur_entry_id =contact_ptr->contact_id;
	applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;

	AddEditWinInitAvatarListCtrl(avatar_ctr_id, add_data_ptr, &field_infor);
	AddEditWinInitEditorCtrl(win_id, add_data_ptr, &field_infor);
	AddEditWinInitRingListCtrl(ring_ctr_id, add_data_ptr, &field_infor);
	AddEditWinInitGroupListCtrl(group_ctr_id, add_data_ptr, &field_infor);
#ifdef MMI_ORIG_UNISOC_SUPPORT
	#ifndef MMI_MULTI_SIM_SYS_SINGLE
	AddEditWinInitPreferredSimListCtrl(preferred_sim_ctr_id, add_data_ptr, &field_infor);
	#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
#endif

	//if(MMIPB_OPERATE_TYPE_ADD_ONE == add_data_ptr->operate_type)			 	
	//{
	//}
	//else  if(MMIPB_OPERATE_TYPE_EDIT_ONE == add_data_ptr->operate_type)
	
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void UpdateAddEditWinAvatarListCtrl(MMI_WIN_ID_T win_id, MMI_HANDLE_T avatar_ctr_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	BOOLEAN is_focus = FALSE;
	if(avatar_ctr_id == MMK_GetActiveCtrl(win_id))
	{
		is_focus = TRUE;
	}
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
		MMI_STRING_T    str_t = {0};
		uint32 item_index = 0;	
		GUIANIM_FILE_INFO_T anim_path = {0};
		anim_path.full_path_wstr_len = add_data_ptr->photo_ptr->file_name_len;
		anim_path.full_path_wstr_ptr = add_data_ptr->photo_ptr->file_name; 
		ReplaceListItemForAddEdit(avatar_ctr_id,
			MMIPB_ADD_EDIT_INDEX_AVATAR,
			item_index,      
			GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS,
			anim_path.full_path_wstr_len ? STR_LIST_EDIT_IMAGE_EXT01:STR_LIST_ADD_IMAGE,
			TXT_NULL,
			PNULL,
			is_focus? IMAGE_CONTACTS_ADD_AVATAR_ICON:IMAGE_LISTITEM_AVATAR_INITIALS,
			&anim_path
			);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void UpdateAddEditWinRingListCtrl(MMI_HANDLE_T ring_ctr_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
		MMI_STRING_T    str_t = {0};
		uint32 item_index = 0;
		if(contact_ptr->ring_id_flag&MMIPB_FIXED_RING_FLAG)
		{
			if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) == 0 )
			{
				ReplaceListItemForAddEdit(ring_ctr_id,
					MMIPB_ADD_EDIT_INDEX_RING,
					item_index,      
					GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
					STXT_HEADER_RINGING_TYPE,
					STR_NO_SOUND_EXT01,
					PNULL,
					IMAGE_NULL,
					PNULL
					);				
			}
			else if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
			{
				str_t.wstr_ptr = wstr; 
				str_t.wstr_len = MMIAPISET_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK),str_t.wstr_ptr, MMIPB_MAX_STRING_LEN); 
				ReplaceListItemForAddEdit(ring_ctr_id,
					MMIPB_ADD_EDIT_INDEX_RING,
					item_index,      
					GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
					STXT_HEADER_RINGING_TYPE,
					TXT_NULL,
					&str_t,
					IMAGE_NULL,
					PNULL
					);
			}
			else
			{
				ReplaceListItemForAddEdit(ring_ctr_id,
					MMIPB_ADD_EDIT_INDEX_RING,
					item_index,      
					GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
					STXT_HEADER_RINGING_TYPE,
					STR_COMMON_LIST_DEFAULT,
					PNULL,
					IMAGE_NULL,
					PNULL
					);
			}
			
		}
		else if((PNULL != add_data_ptr->ring_ptr) && (add_data_ptr->ring_ptr->is_valid))
		{
			//str_t.wstr_ptr = add_data_ptr->ring_ptr->file_name;
			//str_t.wstr_len = add_data_ptr->ring_ptr->file_name_len;
			wchar file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
			uint16 file_name_len = 0;
			MMIAPIFMM_SplitFullPath(add_data_ptr->ring_ptr->file_name, add_data_ptr->ring_ptr->file_name_len,
				NULL, NULL, NULL, NULL,
				file_name, &file_name_len);
			str_t.wstr_ptr = file_name;
			str_t.wstr_len = file_name_len;
			ReplaceListItemForAddEdit(ring_ctr_id,
				MMIPB_ADD_EDIT_INDEX_RING,
				item_index,      
				GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
				STXT_HEADER_RINGING_TYPE,
				TXT_NULL,
				&str_t,
				IMAGE_NULL,
				PNULL
				);			
		}
		else
		{
			ReplaceListItemForAddEdit(ring_ctr_id,
				MMIPB_ADD_EDIT_INDEX_RING,
				item_index,      
				GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
				STXT_HEADER_RINGING_TYPE,
				STR_COMMON_LIST_DEFAULT,
				PNULL,
				IMAGE_NULL,
				PNULL
				);
		}		
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void UpdateAddEditWinGroupListCtrl(MMI_HANDLE_T group_ctr_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
		MMI_STRING_T    str_t = {0};
		uint32 item_index = 0;
		SCI_MEMSET(wstr, 0x00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
		str_t.wstr_len = MMIPB_GetMultiGrouName(contact_ptr->union_t.group, wstr);
		str_t.wstr_ptr = wstr;        
		ReplaceListItemForAddEdit(group_ctr_id,
			MMIPB_ADD_EDIT_INDEX_GROUP,
			item_index,      
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
			STR_CONTA_GROUP_EXT01,
			TXT_NULL,
			&str_t,
			IMAGE_NULL,
			PNULL
			);
	}
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void UpdateAddEditWinPreferredSimListCtrl(MMI_HANDLE_T preferred_sim_ctr_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
		wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
		MMI_STRING_T    str_t = {0};
		uint32 item_index = 0;
		if(0 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_GROUP_LIST_NONE_EXT01, &str_t);
		}
		else if(1 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE, &str_t);
		}
		else if(2 == contact_ptr->preferred_sim)
		{
			MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO, &str_t);
		}
		
		ReplaceListItemForAddEdit(preferred_sim_ctr_id,
			MMIPB_ADD_EDIT_INDEX_PREFERRED_SIM,
			item_index,      
			GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS,
			STR_SIM_LIST_PRE_SIM_EXT02,
			TXT_NULL,
			&str_t,
			IMAGE_NULL,
			PNULL
			);
	}
}
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateAddEditWinListCtrl(MMI_WIN_ID_T win_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMI_HANDLE_T avatar_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_AVATAR_ID);
	MMI_HANDLE_T ring_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_RING_ID);
	MMI_HANDLE_T group_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_GROUP_ID);
#ifdef MMI_ORIG_UNISOC_SUPPORT
	MMI_HANDLE_T preferred_sim_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_PREFERRED_SIM_ID);
#endif
	UpdateAddEditWinAvatarListCtrl(win_id,avatar_ctr_id, add_data_ptr);
	UpdateAddEditWinRingListCtrl(ring_ctr_id, add_data_ptr);
	UpdateAddEditWinGroupListCtrl(group_ctr_id, add_data_ptr);
#ifdef MMI_ORIG_UNISOC_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	UpdateAddEditWinPreferredSimListCtrl(preferred_sim_ctr_id, add_data_ptr);
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
#endif
	return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddEditWinSave(MMI_WIN_ID_T win_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
	MMI_STRING_T        str_info = {0};
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	MMISET_DATE_T     date = {0};
	
	
	MMI_HANDLE_T birthday_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BIRTHDAY_ID);

		
	#ifndef PB_SUPPORT_LOW_MEMORY
	SetBirthDate(win_id, birthday_ctr_id, contact_ptr);
	#endif
	
	return SaveEditContact(win_id, add_data_ptr);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddEditWinEditboxUpdate(MMI_WIN_ID_T win_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr, MMI_HANDLE_T src_handle)
{
	MMI_STRING_T        str_info = {0};
	MMIPB_CONTACT_T *contact_ptr = add_data_ptr->contact_ptr;
	MMISET_DATE_T     date = {0};
	uint8 number_index = MMIPB_MAX_NV_PHONE_NUM;
	
	MMI_HANDLE_T first_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_FIRST_NAME_ID);
	MMI_HANDLE_T last_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_LAST_NAME_ID);
	MMI_HANDLE_T mobile_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_MOBILE_ID);
	MMI_HANDLE_T business_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BUSINESS_ID);
	MMI_HANDLE_T home_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_HOME_ID);
#ifdef MMIPB_MAIL_SUPPORT	
	MMI_HANDLE_T email_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_EMAIL_ID);
#endif

	if(  src_handle == first_name_ctr_id)
	{
		GUIEDIT_GetString(first_name_ctr_id, &str_info);
		if(str_info.wstr_len != 0)
		{
			contact_ptr->name.wstr_len = MIN(str_info.wstr_len, MMIPB_NAME_MAX_LEN);
			SCI_MEMSET(contact_ptr->name.wstr, 0x00,sizeof(contact_ptr->name.wstr));
			MMIAPICOM_Wstrncpy(contact_ptr->name.wstr, str_info.wstr_ptr, contact_ptr->name.wstr_len);
		}
		else
		{
			contact_ptr->name.wstr_len = 0;
			SCI_MEMSET(contact_ptr->name.wstr, 0x00,sizeof(contact_ptr->name.wstr));
		}
		
	}
	else if(src_handle  == last_name_ctr_id)
	{
		
#ifdef MMI_PB_SURNAME_SUPPORT	
		GUIEDIT_GetString(last_name_ctr_id, &str_info);
		if(str_info.wstr_len != 0)
		{
			contact_ptr->surname.wstr_len = MIN(str_info.wstr_len, MMIPB_NAME_MAX_LEN);
			SCI_MEMSET(contact_ptr->surname.wstr, 0x00,sizeof(contact_ptr->surname.wstr));
			MMIAPICOM_Wstrncpy(contact_ptr->surname.wstr, str_info.wstr_ptr, contact_ptr->surname.wstr_len);
		}
		else
		{
			contact_ptr->surname.wstr_len = 0;
			SCI_MEMSET(contact_ptr->surname.wstr, 0x00,sizeof(contact_ptr->surname.wstr));
		}
#endif /*MMI_PB_SURNAME_SUPPORT*/
	}
	else if(src_handle  == mobile_ctr_id
		|| src_handle  == business_ctr_id
		|| src_handle  == home_ctr_id)
	{
		uint8 num_type =  MMIPB_INVLID_NUMBER;
		if(src_handle  == mobile_ctr_id)
		{
			num_type = MMIPB_ADN_NUMBER;
		}
		else if(src_handle  == home_ctr_id)
		{
			num_type = MMIPB_HOME_NUMBER;
		}
		else if(src_handle  == business_ctr_id)
		{
			num_type = MMIPB_OFFICE_NUMBER;
		}
		if(!GetExistNumTypeIndex(contact_ptr, num_type, &number_index))
		{
			GetNoExistNumTypeIndex(contact_ptr, num_type, &number_index);
		}
		
		if(MMIPB_MAX_NV_PHONE_NUM != number_index)
		{
			MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
			uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
			uint16 num_len = 0;
			
			GUIEDIT_GetString(src_handle, &str_info);
			num_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, str_info.wstr_len);
			MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, str_info.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN, num_len);
			
			if (str_info.wstr_len > 0 && number_index < MMIPB_MAX_NV_PHONE_NUM && MMIAPICOM_GenPartyNumber((uint8 *)(num_buf), num_len, &party_num))
			{
				contact_ptr->num_type[number_index] = num_type;
				contact_ptr->number[number_index].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
				contact_ptr->number[number_index].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
				SCI_MEMCPY(contact_ptr->number[number_index].number, party_num.bcd_num, contact_ptr->number[number_index].number_len);
			}
			else
			{
				if (MMIPB_MAX_NV_PHONE_NUM > number_index)
				{
					contact_ptr->number[number_index].number_len = 0;
					contact_ptr->number[number_index].npi_ton = 0;
					contact_ptr->num_type[number_index] = MMIPB_INVLID_NUMBER;
					SCI_MEMSET(contact_ptr->number[number_index].number, 0x00,sizeof(contact_ptr->number[number_index].number));
				}
			}
		}
	}
#ifdef MMIPB_MAIL_SUPPORT 	
	else if(src_handle  == email_ctr_id)
	{
		GUIEDIT_GetString(email_ctr_id, &str_info);
		SCI_MEMSET(&(contact_ptr->mail),0x00,sizeof(MMIPB_MAIL_T));
		contact_ptr->mail.wstr_len = MIN(str_info.wstr_len, MMIPB_MAX_MAIL_LEN);
		MMIAPICOM_Wstrncpy(contact_ptr->mail.wstr, str_info.wstr_ptr, contact_ptr->mail.wstr_len);            
	}
#endif		
}

/*****************************************************************************/
//	Description : 
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void FdnSelectContactCallback(MMIPB_HANDLE_T handle)
{
	uint32 i = 0;
	MMI_HANDLE_T applet_handle = NULL;
	MMIPB_APPLET_T*    applet_ptr = PNULL;        
	MMI_QUEUE_NODE_T * pb_queue = MMIPB_GetAppletQueue();
	
	if(pb_queue != PNULL)
	{
		for (i=0; i< pb_queue->current_count; i++)
		{
			applet_handle =  *(MMI_HANDLE_T *)MMIQUEUE_PeekQueue(pb_queue, i);
			applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);  
			if(PNULL != applet_ptr)
			{
				MMI_HANDLE_T win_id = MMK_GetWinHandle(applet_handle, MMIPB_ADD_IN_NV_WIN_ID);
				if(MMK_IsOpenWin(win_id))
				{
					MMK_SendMsg(win_id, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
					break;
				}
			}
		} 
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContactIsSame(MMIPB_CONTACT_T* one, MMIPB_CONTACT_T* two)
{
	if(PNULL == one || PNULL == two)
	{
		return FALSE;
	}
	else if((one->contact_id != two->contact_id) || (one->storage_id != two->storage_id))
	{
		return FALSE;
	}
	else
	{
		if(
			one->union_t.group == two->union_t.group
			&& one->ring_id_flag == two->ring_id_flag
			&& one->photo_id_flag == two->photo_id_flag
#ifndef MMI_MULTI_SIM_SYS_SINGLE
			&& one->preferred_sim == two->preferred_sim
#endif
			&& 0 == memcmp(one->num_type, two->num_type, sizeof(uint8)*MMIPB_MAX_NV_PHONE_NUM)
			//&& 0 == memcmp(one->number, two->number, sizeof(MMIPB_BCD_NUMBER_T)*MMIPB_MAX_NV_PHONE_NUM)
			&& 0 == memcmp(&(one->name), &(two->name), sizeof(MMIPB_NAME_T))
#ifdef MMI_PB_SURNAME_SUPPORT
			&& 0 == memcmp(&(one->surname), &(two->surname), sizeof(MMIPB_NAME_T))
#endif /*MMI_PB_SURNAME_SUPPORT*/
#ifdef MMIPB_MAIL_SUPPORT
			&& 0 == memcmp(&(one->mail), &(two->mail), sizeof(MMIPB_MAIL_T))
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
			&& 0 == memcmp(&(one->birth), &(two->birth), sizeof(MMIPB_BIRTH_T))
#endif
			)
		{
			uint8 i = 0;
			for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
			{
				if(one->number[i].number_len != two->number[i].number_len
				    ||one->number[i].npi_ton != two->number[i].npi_ton
				    ||0 != memcmp(&(one->number[i].number),&(two->number[i].number),one->number[i].number_len)
				    )
				{
					return FALSE;
				}
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDiscardDialogWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
	MMI_RESULT_E        result = MMI_RESULT_TRUE;
	MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
	
	if (PNULL == win_info_ptr)
	{
		return MMI_RESULT_FALSE;
	}
	
	switch(msg_id)
	{
	case MSG_APP_RED:
	case MSG_APP_MENU:
	case MSG_CTL_OK:
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
    case MSG_APP_OK:
		MMK_SendMsg(MMK_GetPrevWinId(win_info_ptr->win_handle),MSG_PROMPTWIN_OK,(DPARAM)&(win_info_ptr->win_handle));
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_SendMsg(MMK_GetPrevWinId(win_info_ptr->win_handle),MSG_PROMPTWIN_CANCEL,(DPARAM)&(win_info_ptr->win_handle));
		break;
	default:
		result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
	}
	
	return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactAddEditWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
	MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
	MMI_RESULT_E  result  = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_EDITBOX_FORM_ID);
	MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
	MMIPB_CONTACT_T *contact_ptr = PNULL;
	uint16 *ring_id_ptr = PNULL;
	MMI_HANDLE_T    applet_handle = {0};
	MMIPB_APPLET_T* applet_ptr = PNULL;
	MMI_WIN_ID_T query_id = MMIPUB_QUERY_WIN_ID; 
	
	MULTIM_SELECT_RETURN_T*     multim_select_ptr = PNULL;
	uint32                      name_len = 0;   
	
	LOCAL wchar *s_old_photo_file_name_ptr = PNULL; //缓存联系人旧头像文件名
	LOCAL wchar *s_old_ring_file_name_ptr = PNULL; //缓存联系人旧铃声文件名
	LOCAL MMIPB_CONTACT_T* s_old_contact_info =PNULL;
	
	MMI_HANDLE_T win_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_WIN_FORM_ID);
	MMI_HANDLE_T avatar_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_AVATAR_ID);
	MMI_HANDLE_T first_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_FIRST_NAME_ID);
	MMI_HANDLE_T last_name_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_LAST_NAME_ID);
	MMI_HANDLE_T mobile_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_MOBILE_ID);
	MMI_HANDLE_T business_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BUSINESS_ID);
	MMI_HANDLE_T home_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_HOME_ID);
#ifdef MMIPB_MAIL_SUPPORT 	
	MMI_HANDLE_T email_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_EMAIL_ID);
#endif
	MMI_HANDLE_T birthday_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_BIRTHDAY_ID);
	MMI_HANDLE_T ring_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_RING_ID);
	MMI_HANDLE_T group_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_GROUP_ID);
#ifdef MMI_ORIG_UNISOC_SUPPORT
	MMI_HANDLE_T preferred_sim_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_ADD_PREFERRED_SIM_ID);
#endif

	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return result;
	}
	applet_handle = applet_ptr->caf_applet.app_handle;

	
	add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
	if(PNULL == add_data_ptr||PNULL == add_data_ptr->contact_ptr)
	{
		result  = MMI_RESULT_FALSE;
		return result;
	}
	contact_ptr = add_data_ptr->contact_ptr;
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
		{
			GUIWIN_SetTitleTextId(win_id, STR_LIST_EDIT_CONTACT_EXT01, FALSE);
			if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_LIST_EDIT_CONTACT_EXT01))
			{
				GUIWIN_UpdateStb();
			}
		}
		else
		{
			GUIWIN_SetTitleTextId(win_id, TXT_ADDTOPB_NEW, FALSE);
			if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_PBSAV_HDR_NEW_NAME))
			{
				GUIWIN_UpdateStb();
			}
		}
		if(!(ContactAddInitAddData(add_data_ptr)))
		{
			MMK_CloseWin(win_id);
		}
		if(!(InitAddEditWinCtrl(win_id,add_data_ptr)))
		{
			MMK_CloseWin(win_id);
		}
		//InitContactEditItems(win_id,ctrl_id, add_data_ptr, STXT_SOFTKEY_SAVE_MK);
		if(!MMIAPIPB_IsSortBySurname() || !MMIPB_IsPhoneContact(contact_ptr->storage_id))
		{
		    MMK_SetAtvCtrl(win_id, first_name_ctr_id);
		}
		else
		{
		    MMK_SetAtvCtrl(win_id, last_name_ctr_id);
		}
		if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
		{
			s_old_contact_info = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
			if(PNULL != s_old_contact_info)
			{
				SCI_MEMSET(s_old_contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
				SCI_MEMCPY(s_old_contact_info, contact_ptr, sizeof(MMIPB_CONTACT_T))
			}
		}
		break;
	case MSG_FULL_PAINT:
		{
			GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
			GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
			LCD_FillRect(&lcd_dev_info, full_rect,MMI_WINDOW_BACKGROUND_COLOR);
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
		{
			GUIWIN_SetTitleTextId(win_id, STR_LIST_EDIT_CONTACT_EXT01, FALSE);
			if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_LIST_EDIT_CONTACT_EXT01))
			{
				GUIWIN_UpdateStb();
			}
		}
		else
		{
			GUIWIN_SetTitleTextId(win_id, TXT_ADDTOPB_NEW, FALSE);
			if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_PBSAV_HDR_NEW_NAME))
			{
				GUIWIN_UpdateStb();
			}
		}
		break;
	case MSG_PRE_LCD_SWITCH:
		{
			GUI_RECT_T  form_rect = MMITHEME_GetClientRect();
			form_rect.bottom = form_rect.bottom -6;
			GUIFORM_SetRect(win_form_ctr_id, &form_rect);
		}
		break;
	case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
		{
			MMI_HANDLE_T src_handle = ((MMI_NOTIFY_T*)param)->src_handle;
			AddEditWinEditboxUpdate(win_id, add_data_ptr, src_handle);
			if(add_data_ptr->is_need_pin_verify)
			{
				if(0 == contact_ptr->name.wstr_len && 0 == contact_ptr->number[0].number_len)
				{
					GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_PB_MK, 1, TRUE);
				}
				else
				{
					GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SAVE_MK, 1, TRUE);
				}
			}
		}
		break;
	case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
		if(PNULL != add_data_ptr->photo_ptr && !add_data_ptr->photo_ptr->is_valid)
		{
		    UpdateAddEditWinAvatarListCtrl(win_id, avatar_ctr_id, add_data_ptr);
		    MMK_SendMsg(avatar_ctr_id, MSG_CTL_PAINT, 0);
		}
		break;
	case MSG_APP_DOWN:
	case MSG_KEYREPEAT_DOWN:
		result = MMI_RESULT_FALSE;
		break;
		
	case MSG_APP_UP:
	case MSG_KEYREPEAT_UP:
		result = MMI_RESULT_FALSE;
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		//add_data_ptr->parent_win = win_id;
		//CreateEditBox(ctrl_id,add_data_ptr);
		//AddEditWinSave(win_id,add_data_ptr);
		if(avatar_ctr_id == MMK_GetActiveCtrl(win_id))
		{
			#ifdef PB_UI_UNISOC_DEBUG
			add_data_ptr->parent_win= win_id;      
			add_data_ptr->menu_type = MMIPB_PHOTO_OPT_MENU;            
			//  add_data_ptr->operate_type = MMIPB_OPERATE_TYPE_MAX;
			add_data_ptr->need_reset_storage = FALSE;
			MMIPB_EnterPbTabOptMenu(add_data_ptr);
			#else
#ifdef PIC_VIEWER_SUPPORT
			MMIAPIPICVIEW_EnterFromPB();
#endif
			#endif
		}
		else if(ring_ctr_id == MMK_GetActiveCtrl(win_id))
		{
			add_data_ptr->parent_win= win_id;      
			add_data_ptr->menu_type = MMIPB_RING_OPT_MENU;            
			//  add_data_ptr->operate_type = MMIPB_OPERATE_TYPE_MAX;
			add_data_ptr->need_reset_storage = FALSE;
			//MMIPB_EnterPbTabOptMenu(add_data_ptr);
			//MMIPB_EnterPbSelectionMenu(add_data_ptr);
			MMIPB_EnterPbSelectionRingWin(add_data_ptr);
		}
		else if(group_ctr_id == MMK_GetActiveCtrl(win_id))
		{
			MMIPB_CreateEditGroupWin(win_id, add_data_ptr);
		}
#ifdef MMI_ORIG_UNISOC_SUPPORT
		else if(preferred_sim_ctr_id == MMK_GetActiveCtrl(win_id))
		{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
			add_data_ptr->parent_win= win_id;      
			add_data_ptr->menu_type = MMIPB_ADDEDIT_PREFERRED_SIM_MENU;            
			add_data_ptr->need_reset_storage = FALSE;
			MMIPB_EnterPbSelectionMenu(add_data_ptr);
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
		}
#endif
		else if(add_data_ptr->is_need_pin_verify)
		{
			if(0 == contact_ptr->name.wstr_len && 0 == contact_ptr->number[0].number_len)
			{
				MMIAPIPB_OpenMainWin(
					MMIPB_ALL_CONTACT,
					MMIPB_LIST_RADIO, 
					MMIPB_LIST_FIELD_NUMBER,
					0,
					PNULL,
					FdnSelectContactCallback
					);
			}
			else
			{
				MMK_SendMsg(win_id, MMI_PB_OPERATE_OK, PNULL);
			}
		}
		break;
	case MMI_SELECT_CONTACT_CNF:
		if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
		{
			MMIPB_BCD_NUMBER_T	bcd_number = {0};
			uint8				temp_num[CC_MAX_TELE_NUM_LEN + 1]= {0};
			wchar 				tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};             
			uint16                      number_len = 0;
			MMI_STRING_T         name_str = {0};
			MMI_STRING_T         num_str = {0};
			wchar  name_wchar[MMIPB_NAME_MAX_LEN + 1] = {0};             
			
			name_str.wstr_ptr = name_wchar;
			if(MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)param, &name_str,MMIPB_NAME_MAX_LEN,0))
			{
				
				if(0 == name_str.wstr_len)
				{
					GUIEDIT_ClearAllStr(first_name_ctr_id);
				}
				else
				{
					GUIEDIT_SetString(first_name_ctr_id, name_str.wstr_ptr, name_str.wstr_len);
				}			
			}
			if (MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, 0))
			{
				number_len = MMIAPIPB_BCDNumberToString(&bcd_number, 
					(CC_MAX_TELE_NUM_LEN + 2), 
					temp_num);
				
				MMI_STRNTOWSTR(tele_num, CC_MAX_TELE_NUM_LEN + 1, 
					(uint8 *)temp_num, CC_MAX_TELE_NUM_LEN + 1, 
					CC_MAX_TELE_NUM_LEN + 1);   
				num_str.wstr_len = number_len;
				num_str.wstr_ptr = tele_num;
				if(0 == num_str.wstr_len)
				{
					GUIEDIT_ClearAllStr(mobile_ctr_id);
				}
				else
				{
					GUIEDIT_SetString(mobile_ctr_id,num_str.wstr_ptr, num_str.wstr_len);
				}			
			}
		}		
		else
		{
		    MMIPB_DisplaybannerInfo(win_id, STR_CONTA_NO_NUMBER_EXT01, PNULL);
		}
		break;
	case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        MMIPB_UpdateListContentItemForContact(
			add_data_ptr->photo_ptr,
			avatar_ctr_id,
			contact_ptr,
			param);
		break;
	case MMI_PB_ADDEDIT_CLEAR_DATE:
		GUIEDIT_SetDate(birthday_ctr_id, 1900, 01, 01);
		break;
	case MMI_PB_OPERATE_OK:
		if(add_data_ptr->is_need_pin_verify && 0 == contact_ptr->number[0].number_len)
		{
		    MMIPB_OpenCommonDialogWin(applet_ptr->caf_applet.app_handle, STR_FDN_NO_NUMBER_EXT01);
		    break;
		}
		//ret = SaveEditContact(win_id,add_data_ptr);
		ret = AddEditWinSave(win_id,add_data_ptr);
		SCI_TRACE_LOW("[MMIPB] SaveEditContact ret %d", ret);
		if(MMIPB_ERROR_BREAK==ret)
		{
			break;
		}
		else if(MMIPB_ERROR_PENDING == ret || MMIPB_ERROR_SUCCESS == ret)
		{
			break;
		}
		else
		{
			MMIPB_OPC_E opc_type = MMIPB_OPC_ADD;
			if(MMIPB_OPERATE_TYPE_EDIT_ONE == add_data_ptr->operate_type)
			{
				opc_type = MMIPB_OPC_EDIT;
			}
			else
			{
				opc_type = MMIPB_OPC_ADD;
			}
			MMIPB_OpenOperateResultDialogWin(add_data_ptr->applet_handle, ret, opc_type);
		}
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_MENU:
		{     
			MMIPB_MENU_DATA_INFOR_T menu_info = {0};
			menu_info.parent_win = win_id;
			menu_info.menu_type = MMIPB_ADDEDIT_OPT_MENU;
			menu_info.applet_handle = applet_handle;
			MMIPB_EnterPbTabOptMenu(&menu_info); 
			break;
		}
		break;
		
	case MMI_PB_PHOTO_SET_DEFAULT:
		{
			uint8 photo_flag = *(uint8 *)param;
			contact_ptr->photo_id_flag = photo_flag;
			MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL); 
		}
		break;
	case MMI_PB_SET_PHOTO_FILE:
		{
			wchar *file_name_ptr = (wchar *)param;
			uint16 file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);
			//SCI_TRACE_LOW:"MMI_PB_SET_PHOTO_FILE msg file_name_ptr is %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_2982_112_2_18_2_43_40_238,(uint8*)"d",file_name_ptr);
			//内存分配、旧头像保存
			if(PNULL == s_old_photo_file_name_ptr)
			{
				s_old_photo_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar));
				
				if(PNULL == s_old_photo_file_name_ptr)
				{
					break;
				}
				SCI_MEMSET(s_old_photo_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar)));
				MMIAPICOM_Wstrncpy(
					s_old_photo_file_name_ptr,
					add_data_ptr->photo_ptr->file_name,
					MIN(MMIAPICOM_Wstrlen(add_data_ptr->photo_ptr->file_name), MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
					);//保存旧头像路径
			}
			
			if(PNULL != file_name_ptr && 0 != file_name_len)
			{
				SetContactFilePhoto(win_id,contact_ptr,file_name_ptr,file_name_len,
					add_data_ptr->photo_ptr);
			}
			else
			{
				contact_ptr->photo_id_flag = contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_CLEAN;				
				if(add_data_ptr->photo_ptr)
				{
					SCI_MEMSET(add_data_ptr->photo_ptr,0x00,sizeof(MMIPB_CUSTOM_PHOTO_T));
					MMIPB_AddContact_PhotoInfo(contact_ptr, add_data_ptr->photo_ptr);
				}
				MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL); 
			}
		}
		break;
		
	case MMI_PB_SET_DEFAULT_RING:
		contact_ptr->ring_id_flag = *(uint8*)param;                 
		MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);             
		break;
		
	case MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG:
		//add_data_ptr->parent_win = win_id;
		//UpdateContactAddOrEditItems(ctrl_id,add_data_ptr,param);
#ifdef MMI_ORIG_UNISOC_SUPPORT
		if(preferred_sim_ctr_id == MMK_GetActiveCtrl(win_id))
		{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
		    contact_ptr->preferred_sim = *(uint8*)param;
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
		}	
#endif
		UpdateAddEditWinListCtrl(win_id,add_data_ptr);
		break;
		
		
	case MSG_MULTIM_SELECTED_RETURN:        
		if(PNULL == param)
		{
			//SCI_TRACE_LOW:"[MMIPB] MSG_MULTIM_SELECTED_RETURN param == 0 !"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3287_112_2_18_2_43_41_240,(uint8*)"");
			break;
		}
		multim_select_ptr = (MULTIM_SELECT_RETURN_T *)param;
		switch (multim_select_ptr->file_type)
		{
		case MMIFMM_FILE_TYPE_PICTURE:
			if (multim_select_ptr->file_name_len > MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
			{
				return MMI_RESULT_FALSE;
			}
			else
			{
#ifdef MMI_PIC_CROP_SUPPORT
				CropPhotoFile(multim_select_ptr->file_name_ptr,multim_select_ptr->file_name_len);
#else
				//内存分配、旧头像保存
				if(PNULL == s_old_photo_file_name_ptr)
				{
					s_old_photo_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar));
					
					if(PNULL == s_old_photo_file_name_ptr)
					{
						return MMI_RESULT_FALSE;
					}
					SCI_MEMSET(s_old_photo_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar)));
					MMIAPICOM_Wstrncpy(
						s_old_photo_file_name_ptr,
						add_data_ptr->photo_ptr->file_name,
						MIN(MMIAPICOM_Wstrlen(add_data_ptr->photo_ptr->file_name), MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
						);//保存旧头像路径
				}
				SetContactFilePhoto(win_id,contact_ptr,multim_select_ptr->file_name_ptr,
					multim_select_ptr->file_name_len,add_data_ptr->photo_ptr);                   
#endif
			}
			break;
			
		case MMIFMM_FILE_TYPE_MUSIC:
			if (multim_select_ptr->file_name_len > MMIPB_MAX_CUSTOM_RING_PATH_LEN)
			{
				return MMI_RESULT_FALSE;
			}
			else
			{
				//内存分配、旧铃声保存
				if(PNULL == s_old_ring_file_name_ptr)
				{
					s_old_ring_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_RING_PATH_LEN +1) * sizeof(wchar));
					
					if(PNULL == s_old_ring_file_name_ptr)
					{
						return MMI_RESULT_FALSE;
					}
					SCI_MEMSET(s_old_ring_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_RING_PATH_LEN +1) * sizeof(wchar)));
					MMIAPICOM_Wstrncpy(
						s_old_ring_file_name_ptr,
						add_data_ptr->ring_ptr->file_name,
						MIN(MMIAPICOM_Wstrlen(add_data_ptr->ring_ptr->file_name), MMIPB_MAX_CUSTOM_RING_PATH_LEN)
						);//保存旧铃声路径
				}
				add_data_ptr->ring_ptr->is_valid = TRUE;
				contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN);
				contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN);
				name_len = MIN((multim_select_ptr->file_name_len), MMIPB_MAX_CUSTOM_RING_PATH_LEN);
				add_data_ptr->ring_ptr->file_name_len = name_len;
				SCI_MEMSET(add_data_ptr->ring_ptr->file_name,0,sizeof(add_data_ptr->ring_ptr->file_name));
				
				MMIAPICOM_Wstrncpy(add_data_ptr->ring_ptr->file_name, multim_select_ptr->file_name_ptr, name_len);
				
				//contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag&MMIPB_RING_ID_CLEAN)+MMIPB_RING_ID_EXIST; 
				contact_ptr->ring_id_flag = contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN; 
				add_data_ptr->ring_ptr->ring_type = multim_select_ptr->music_type;
				add_data_ptr->ring_ptr->file_size = multim_select_ptr->file_size;
				
				//+ Bug 125726 
				//after selecting ring file from sound files, 判断是否空间充足
				if(MMIPB_ERROR_NO_SPACE == MMIPB_AddContact_RingInfo(contact_ptr, add_data_ptr->ring_ptr))
				{
					MMI_HANDLE_T    applet_handle = 0;
					
					if(MMIPB_GetCurAppletHandle(&applet_handle))
					{
						//MMIPB_DisplaybannerInfo(0, STR_NO_SPACE_EXT01, PNULL);
						MMIPB_OpenCommonDialogWin(applet_handle, STR_NO_SPACE_EXT01);
						//MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,
							//TXT_NO_SPACE,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
					}
				}
				//- Bug 125726 
				MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);      
			}
			break;
		default:
			break;
		}
		
		break;
		
		case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
			if(PNULL == param)
			{
				//SCI_TRACE_LOW:"[MMIPB] MSG_SET_SELECT_FIX_CALL_RING_RETURN param == 0 !"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3352_112_2_18_2_43_41_241,(uint8*)"");
				break;
			}
			
			ring_id_ptr = (uint16 *)param;
			//SCI_TRACE_LOW:"[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3357_112_2_18_2_43_41_242,(uint8*)"d", *ring_id_ptr);
			add_data_ptr->ring_ptr->is_valid = FALSE; //TRUE;  //CR223191
			contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST;
			contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG;//exist fixed id
			contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr;//set fixed id
			
			//update list display
			MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
			
			break;
			
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			{
				MMI_TEXT_ID_T prompt_text_id = TXT_NULL;
				BOOLEAN has_change = TRUE;
				MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle,PNULL);
				if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
				{
					prompt_text_id = STR_SAVE_CHANGES_EXT01;
					if(applet_ptr->instance.function_type == MMIPB_ADD_TO_EXIST_ENTRY)
					{
						has_change = TRUE;
					}
					else if(PNULL != s_old_photo_file_name_ptr || PNULL != s_old_ring_file_name_ptr)
					{
						has_change = TRUE;
					}
					else if(PNULL != s_old_contact_info && ContactIsSame(s_old_contact_info,contact_ptr))
					{
						has_change = FALSE;
					}
				}
				else
				{
					prompt_text_id = STR_QUERY_SAVE_CONTACT_EXT01;
				}
				
				if(MMIPB_IsContactValid(contact_ptr) && has_change)
				{
					MMI_STRING_T prompt_text = {0};
					MMI_GetLabelTextByLang(prompt_text_id, &prompt_text);
					MMIPUB_OpenConfirmationDialogWinByTextPtrEx(
						applet_handle,
						PNULL,
						&prompt_text,
						PNULL,
						IMAGE_NULL,
						&query_id,
						PNULL,
						MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
						HandleDiscardDialogWinMsg,
						PNULL
						);				
					//MMIPUB_OpenQueryWinByTextIdEx(add_data_ptr->applet_handle, TXT_PB_IS_SAVE_CONTACT, IMAGE_PUBWIN_QUERY, PNULL, PNULL, PNULL);
				}
				else
				{
					MMK_CloseWin(win_id);
				}
			}
			break;
			
		case MSG_APP_RED:
			{
				MMI_TEXT_ID_T prompt_text_id = TXT_NULL;
				BOOLEAN has_change = TRUE;
				MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle,PNULL);
				if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
				{
					prompt_text_id = STR_SAVE_CHANGES_EXT01;
					if(applet_ptr->instance.function_type == MMIPB_ADD_TO_EXIST_ENTRY)
					{
						has_change = TRUE;
					}
					else if(PNULL != s_old_photo_file_name_ptr || PNULL != s_old_ring_file_name_ptr)
					{
						has_change = TRUE;
					}
					else if(PNULL != s_old_contact_info && ContactIsSame(s_old_contact_info,contact_ptr))
					{
						has_change = FALSE;
					}
				}
				else
				{
					prompt_text_id = STR_QUERY_SAVE_CONTACT_EXT01;
				}
				
				if(MMIPB_IsContactValid(contact_ptr) && has_change)
				{
					MMI_STRING_T prompt_text = {0};
					MMI_GetLabelTextByLang(prompt_text_id, &prompt_text);
					MMIPUB_OpenConfirmationDialogWinByTextPtrEx(
						applet_handle,
						PNULL,
						&prompt_text,
						PNULL,
						IMAGE_NULL,
						&query_id,
						PNULL,
						MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
						HandleDiscardDialogWinMsg,
						PNULL
						);				
					//MMIPUB_OpenQueryWinByTextIdEx(add_data_ptr->applet_handle, TXT_PB_IS_SAVE_CONTACT, IMAGE_PUBWIN_QUERY, PNULL, PNULL, PNULL);
				}
				else
				{
					//MMK_ReturnIdleWin();
					MMK_ReturnMainmenuWin();
					//MMK_CloseWin(win_id);
				}
			}
			break;
			
		case MSG_PROMPTWIN_CANCEL: 
			if(*((MMI_HANDLE_T*)param) != MMK_GetWinHandle(applet_handle, query_id))
			{
			    break;
			}
			if(PNULL != s_old_photo_file_name_ptr)
			{
				//获取旧头像显示
				add_data_ptr->photo_ptr->file_name_len = MMIAPICOM_Wstrlen(s_old_photo_file_name_ptr);
				if(0 != add_data_ptr->photo_ptr->file_name_len)
				{
				    add_data_ptr->photo_ptr->is_valid = TRUE;
				}
				SCI_MEMSET(add_data_ptr->photo_ptr->file_name, 0, MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 1);
				MMIAPICOM_Wstrncpy(
					add_data_ptr->photo_ptr->file_name,
					s_old_photo_file_name_ptr,
					MIN(add_data_ptr->photo_ptr->file_name_len, MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
					);//保存旧头像路径
				MMIPB_AddContact_PhotoInfo(add_data_ptr->contact_ptr, add_data_ptr->photo_ptr);
			}
			if(PNULL != s_old_ring_file_name_ptr)
			{
				//获取旧铃声显示
				add_data_ptr->ring_ptr->file_name_len = MMIAPICOM_Wstrlen(s_old_ring_file_name_ptr);
				SCI_MEMSET(add_data_ptr->ring_ptr->file_name, 0, MMIPB_MAX_CUSTOM_RING_PATH_LEN + 1);
				MMIAPICOM_Wstrncpy(
					add_data_ptr->ring_ptr->file_name,
					s_old_ring_file_name_ptr,
					MIN(add_data_ptr->ring_ptr->file_name_len, MMIPB_MAX_CUSTOM_RING_PATH_LEN)
					);//保存旧铃声路径
				MMIPB_AddContact_RingInfo(add_data_ptr->contact_ptr, add_data_ptr->ring_ptr);
			}
			MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle, PNULL);
			MMK_CloseWin(win_id);
			break;
		case MSG_PROMPTWIN_OK:      //确定退出
			if(*((MMI_HANDLE_T*)param) != MMK_GetWinHandle(applet_handle, query_id))
			{
			    break;
			}
			MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle, PNULL);
			MMK_SendMsg(win_id, MMI_PB_OPERATE_OK, PNULL);            
			break;
			
		case MSG_CLOSE_WINDOW:            
			MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_SELECT_IM_TYPE_WIN_ID)); 
			MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_SELECT_NUMBER_TYPE_WIN_ID)); 
			MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_ADD_TO_GROUP_WIN_ID)); 
			FreeContactPhotoRingInfor(add_data_ptr);
			MMK_FreeWinAddData(win_id);		
			if(PNULL != s_old_photo_file_name_ptr)
			{
				SCI_FREE(s_old_photo_file_name_ptr);
			}
			if(PNULL != s_old_ring_file_name_ptr)
			{
				SCI_FREE(s_old_ring_file_name_ptr);
			}
			if(PNULL != s_old_contact_info)
			{
				SCI_FREE(s_old_contact_info);
			}
			CTRLDATEEDIT_FreeTitle(birthday_ctr_id, PNULL);
			break;
			
		default:
			result  = MMI_RESULT_FALSE;
			break;
	}
	return result;
}

/*****************************************************************************/
//  Description : 新增联系人窗口消息处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleContactAddWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT 
	//void *anim_ctrl_ptr = PNULL;
    GUI_POINT_T tp_point = {0};
#endif
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_EDITBOX_FORM_ID);
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    uint16 *ring_id_ptr = PNULL;

    MULTIM_SELECT_RETURN_T*     multim_select_ptr = PNULL;
    uint32                      name_len = 0;   

    LOCAL wchar *s_old_photo_file_name_ptr = PNULL; //缓存联系人旧头像文件名
    LOCAL wchar *s_old_ring_file_name_ptr = PNULL; //缓存联系人旧铃声文件名

    add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr||PNULL == add_data_ptr->contact_ptr)
    {
        result  = MMI_RESULT_FALSE;
        return result;
    }
    contact_ptr = add_data_ptr->contact_ptr;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
		#ifdef HERO_ENGINE_TTS_SUPPORT
		s_hero_pb_addcontact_win_id = win_id;
        #endif		
#if defined (MMIPB_SIMDN_SUPPORT)             
        if(MMIPB_IsMsisdnContact(contact_ptr->storage_id))
        {
            GUIWIN_SetTitleTextId(win_id,TXT_PB_MSISDN,TRUE);
        }
        else
#endif           
        if(MMIPB_OPERATE_TYPE_EDIT_ONE==add_data_ptr->operate_type)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_PB_EDIT_CONTACT,TRUE);
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id,TXT_PB_NEW_CONTACT,TRUE);
        }
        if(!(ContactAddInitAddData(add_data_ptr)))
        {
            MMK_CloseWin(win_id);
        }
        #ifdef MMI_PDA_SUPPORT
        InitContactEditorCtrl(win_id,add_data_ptr);
        MMK_SetWinHookFunc( win_id, HandleContactAddWinHook);
        #else
        InitContactEditItems(win_id,ctrl_id, add_data_ptr, STXT_SOFTKEY_SAVE_MK);
        #endif      
        MMK_SetAtvCtrl(win_id, ctrl_id);  
        break;

        case MSG_APP_OK:
        case MSG_CTL_OK:    // 完成:
        case MSG_APP_MENU:
            #ifdef MMI_PDA_SUPPORT
            GetUserEditContact(win_id,contact_ptr);
            #endif

            ret = SaveEditContact(win_id,add_data_ptr);
            SCI_TRACE_LOW("[MMIPB] SaveEditContact ret %d", ret);
            if(MMIPB_ERROR_BREAK==ret)
            {
                break;
            }
            else if(MMIPB_ERROR_SUCCESS== ret
                || MMIPB_ERROR_PENDING== ret)
            {
                MMK_CloseWin(win_id);
            }
            else if(MMIPB_ERROR_INVALID_NUMBER == ret)
            {
                MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
                    TXT_CC_INVALID_NUM,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
            }
            else if(ret == MMIPB_ERROR_NO_SPACE)
            {
                MMIPB_OPC_E opc_type = MMIPB_OPC_ADD;
                if(MMIPB_OPERATE_TYPE_EDIT_ONE == add_data_ptr->operate_type)
                {
                    opc_type = MMIPB_OPC_EDIT;
                }
                else
                {
                    opc_type = MMIPB_OPC_ADD;
                }
                MMIPB_OpenAlertTipWin(add_data_ptr->applet_handle, ret, opc_type);
            }           
            else
            {
                MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
                    TXT_ERROR,TXT_NULL, IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
            }

        break;

        case MMI_PB_PHOTO_SET_DEFAULT:
            {
                uint8 photo_flag = *(uint8 *)param;
                contact_ptr->photo_id_flag = photo_flag;
                 #ifndef MMI_PDA_SUPPORT
                MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL); 
                #endif
            }
        break;


#ifdef MMI_PDA_SUPPORT
        case MMI_PB_CHANGE_BIRTHDAY:
        {
            MMI_STRING_T str_t = {0};
            str_t.wstr_ptr = (wchar *)param;
            str_t.wstr_len = MMIAPICOM_Wstrlen((wchar *)param);
            GUILABEL_SetText(
            MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_EDIT_ID),
            &str_t, TRUE);
            if(str_t.wstr_len>0)
            {
                GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_NORMAL);
            }
        }
        break;
#endif


        case MMI_PB_SET_PHOTO_FILE:
        {
            wchar *file_name_ptr = (wchar *)param;
            uint16 file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);
            //SCI_TRACE_LOW:"MMI_PB_SET_PHOTO_FILE msg file_name_ptr is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_2982_112_2_18_2_43_40_238,(uint8*)"d",file_name_ptr);
            //内存分配、旧头像保存
            if(PNULL == s_old_photo_file_name_ptr)
            {
                s_old_photo_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar));
                
                if(PNULL == s_old_photo_file_name_ptr)
                {
                    //return;
                    //added by Tong Yongli. According to Coverity CID 20106
                    result  = MMI_RESULT_FALSE;
                    break;
                }
                SCI_MEMSET(s_old_photo_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar)));
                MMIAPICOM_Wstrncpy(
                        s_old_photo_file_name_ptr,
                        add_data_ptr->photo_ptr->file_name,
                        MIN(MMIAPICOM_Wstrlen(add_data_ptr->photo_ptr->file_name), MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
                        );//保存旧头像路径
            }
            
            if(PNULL != file_name_ptr)
            {
                SetContactFilePhoto(win_id,contact_ptr,file_name_ptr,file_name_len,
                        add_data_ptr->photo_ptr);
            }
        }
        break;

     case MMI_PB_SET_DEFAULT_RING:
            contact_ptr->ring_id_flag = *(uint8*)param;                 
            MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);             
            break;

#if defined(MMI_PDA_SUPPORT)
        case MMI_PB_CHANGE_NUMBER_TYPE:
            {
                MMI_STRING_T label_str = {0};
                
                MMIPB_GetNumberTypeString(contact_ptr->storage_id,
                    contact_ptr->num_type[add_data_ptr->number_type_edit_index],
                    &label_str);
                GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id,s_num_type_label_id[add_data_ptr->number_type_edit_index]), 
                    &label_str, TRUE);
            }
            break;
#if defined(MMIPB_IM_MESSAGE_SUPPORT)
        case MMI_PB_CHANGE_IM_TYPE:
            {
                uint8 messager_type = *(uint8 *)param;
                
                MMI_STRING_T label_str = {0};
                
                contact_ptr->im_type = messager_type;
                MMIPB_GetImTypeString(messager_type,&label_str);
                GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_LABEL_ID), 
                    &label_str, TRUE);
                
                GUIFORM_DestroyDynaChildCtrl(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_SUB_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_EDIT_ID));
                
                CreateImEditBox(win_id,contact_ptr);
            }
            break;
#endif
#endif
#ifdef TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
        GUIAPICTRL_GetRect(MMK_GetCtrlHandleByWin(win_id,MMIPB_ANIM_PHOTO_ID), &s_pb_photo_anim_rect);
		//anim_ctrl_ptr = GUIANIM_GetPtr(MMK_GetCtrlHandleByWin(win_id,MMIPB_ANIM_PHOTO_ID));/*lint !e64 !e718 !e746 !e628*/
       // if(anim_ctrl_ptr != PNULL)
        {
           // GUIANIM_GetAnimRect(anim_ctrl_ptr, );/*lint !e718 !e746 !e628*/
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            if(tp_point.x>s_pb_photo_anim_rect.left&&tp_point.x<s_pb_photo_anim_rect.right&&
                tp_point.y>s_pb_photo_anim_rect.top&&tp_point.y<s_pb_photo_anim_rect.bottom)
            {
                MMIPB_FIELD_USAGE_INOF_T free_info = {0};
                  MMIPB_GetFieldUageInfo(
                                 contact_ptr->storage_id,//                                   
                                 contact_ptr->contact_id,//
                                 &free_info
                                 );
                if(!free_info.photo_free_flag)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,
                        PNULL,TXT_PB_SIM_NO_PHOTO,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,
                        MMIPUB_SOFTKEY_ONE,PNULL, PNULL); 
                    break;
                }
                add_data_ptr->parent_win= win_id;
                add_data_ptr->menu_type = MMIPB_PHOTO_OPT_MENU;
                
                add_data_ptr->need_reset_storage = FALSE;
                MMIPB_EnterPbTabOptMenu(add_data_ptr);
            }
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	#ifdef MMI_PDA_SUPPORT
			
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        //add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
        //SCI_TRACE_LOW:"[baokun] add MSG_CTL_PENOK param %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3062_112_2_18_2_43_40_239,(uint8*)"d", param);
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:                 
			    //保存
                GetUserEditContact(win_id,contact_ptr);                
                ret = SaveEditContact(win_id,add_data_ptr);
                if(MMIPB_ERROR_BREAK==ret)
                {
                    break;
                }
                else if(MMIPB_ERROR_SUCCESS== ret
                || MMIPB_ERROR_PENDING== ret)
                {
                    MMK_CloseWin(win_id);
                }
                else if(MMIPB_ERROR_INVALID_NUMBER == ret)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
                        TXT_CC_INVALID_NUM,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
                }
                else if((MMIPB_ERROR_PENDING != ret) && (ret != MMIPB_ERROR_NO_SPACE))
                {
                    MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
                        TXT_ERROR,TXT_NULL, IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
                }	
                else
                {
                    MMIPB_OPC_E opc_type = MMIPB_OPC_ADD;
                    if(MMIPB_OPERATE_TYPE_EDIT_ONE == add_data_ptr->operate_type)
                    {
                        opc_type = MMIPB_OPC_EDIT;
                    }
                    else
                    {
                        opc_type = MMIPB_OPC_ADD;
                    }
                    MMIPB_OpenAlertTipWin(add_data_ptr->applet_handle, ret, opc_type);
                }
                break;
                
            
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);                
                break;    

            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                //MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);  
                ActiveNextEditCtrl(win_id,contact_ptr->storage_id);
                break;         
                
            default:
            {
                ctrl_id = MMK_GetActiveCtrlId(win_id);
                if(MMIPB_LABEL_GROUP_EDIT_ID == ctrl_id)
                {
                     //change group id
                     MMIPB_CreateEditGroupWin(win_id, add_data_ptr);                   
                }
                else if(MMIPB_LABEL_RING_EDIT_ID == ctrl_id)
                {
			        //add ring id
                    add_data_ptr->parent_win= win_id;
                    add_data_ptr->menu_type = MMIPB_RING_OPT_MENU;
                    
                    add_data_ptr->need_reset_storage = FALSE;
                    MMIPB_EnterPbTabOptMenu(add_data_ptr);
                }
#ifdef MMIPB_IM_MESSAGE_SUPPORT
                else if(MMIPB_IM_LABEL_ID == ctrl_id)
                {
			        add_data_ptr->parent_win = win_id;                        
                    #ifdef MMIPUBWIN_PDASTYLE_SUPPORT                     
                        MMK_CreatePubwinTabEx(add_data_ptr->applet_handle, SPRD_GUI_LIST_ID,
                            (uint32 *)MMIPB_SELECT_IM_TYPE_WIN_TAB, add_data_ptr);
                    #else
                        MMK_CreateWin((uint32*)MMIPB_SELECT_IM_TYPE_WIN_TAB, add_data_ptr);
                    #endif
                }
#endif
#ifdef ASP_SUPPORT
                else if(MMIPB_LABEL_ASP_EDIT_ID == ctrl_id)
                {
			             //add ring id
                    AddASPID(win_id,contact_ptr, add_data_ptr->asp_ptr);
                }
#endif		 

                else if( ctrl_id>= MMIPB_NUM1_LABEL_ID &&ctrl_id <=MMIPB_NUM5_LABEL_ID)
                {
                    uint8 valid_count = 0;
                    
#ifdef CMCC_VCARD_SUPPORT
                    //不可以编辑usim联系人第1个ADN号码的类型字符串
                    if(MMIPB_IsUsim(contact_ptr->storage_id) && (MMIPB_NUM1_LABEL_ID == ctrl_id))
                    {
                        break;
                    }
#endif
                    MMIPB_GetMaxNumbernum(contact_ptr->storage_id, &valid_count);
                    if(valid_count > 1)
                    {
                        add_data_ptr->parent_win = win_id;
                        add_data_ptr->number_type_edit_index = ctrl_id-MMIPB_NUM1_LABEL_ID;
                    #ifdef MMIPUBWIN_PDASTYLE_SUPPORT                   
                        MMK_CreatePubwinTabEx(add_data_ptr->applet_handle, SPRD_GUI_LIST_ID,
                            (uint32 *)MMIPB_SELECT_NUMBER_TYPE_WIN_TAB, add_data_ptr);
                    #else
                        MMK_CreateWin((uint32*)MMIPB_SELECT_NUMBER_TYPE_WIN_TAB, add_data_ptr);
                    #endif
                    }                    
                }
                else if(ctrl_id>=MMIPB_NUM1_DEL_BUTTON_ID&&ctrl_id <= MMIPB_NUM5_DEL_BUTTON_ID)
                {
                    uint16 index = ctrl_id -MMIPB_NUM1_DEL_BUTTON_ID;
                    UpdateContactEditNumber(win_id,index,add_data_ptr);                   
                }
                #ifndef PB_SUPPORT_LOW_MEMORY
                else if(MMIPB_BIRTHDAY_EDIT_ID==ctrl_id)
                {
                    add_data_ptr->parent_win = win_id;
                    MMK_CreatePubFormWin(MMIPB_SET_BIRTHDAY_WIN_TAB,add_data_ptr);/*lint !e605*/
                }
                else if(MMIPB_BUTTON_BIRTHDAY_DELETE_ID==ctrl_id)
                {
                      MMI_STRING_T str_t = {0};
                      SCI_MEMSET(&contact_ptr->birth,0,sizeof(MMIPB_BIRTH_T));
                        GUILABEL_SetText(
                            MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_EDIT_ID),
                            &str_t,TRUE); 
                        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_HIDE);
                }
                #endif
                else if(MMIPB_EDITBOX_MORE_SWITCH_LIST_ID==ctrl_id)
                {
                    GUILIST_ITEM_T *item_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(
                        MMK_GetCtrlHandleByWin(win_id,ctrl_id),0); 
                    if(PNULL!=item_ptr)
                    {
                        if(0==item_ptr->user_data)
                        {
                            SwitchMoreItemState(
                                MMK_GetCtrlHandleByWin(win_id,ctrl_id),
                                IMAGE_PB_MORE_OPEN,
                                1);
                            DisplayAddressBirthUrlItem(win_id,contact_ptr);
                            
                        }
                        else
                        {
                            SwitchMoreItemState(
                                MMK_GetCtrlHandleByWin(win_id,ctrl_id),
                                IMAGE_PB_MORE_CLOSE,
                                0);
                            HideAddressBirthUrlItem(win_id);
                        }
                         GUIWIN_SetSoftkeyBtnGray(win_id,1,FALSE);
                    }
                }
            }
        } 
        break;
    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        UpdateNumberEditCtrl(win_id,contact_ptr);
        UpdateSoftkeyStatus(win_id);
        break;
        
#else  //MMI_PDA_SUPPORT

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        add_data_ptr->parent_win = win_id;
        CreateEditBox(ctrl_id,add_data_ptr);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        MMIPB_UpdateListContentItemForContact(
                add_data_ptr->photo_ptr,
                ctrl_id,
                contact_ptr,
                param);

        break;

#endif //MMI_PDA_SUPPORT
        
     case MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG:
        add_data_ptr->parent_win = win_id;
    #ifdef MMI_PDA_SUPPORT
        UpdatePhotoRingLabelEdit(add_data_ptr);
    #else
        UpdateContactAddOrEditItems(ctrl_id,add_data_ptr,param);
     
    #endif
        break;
        
    #ifdef ASP_SUPPORT
         case MMI_PB_ASP_FIXID_SELECTED:
             {
                 uint32 asp_id = (uint32)param;
                 add_data_ptr->asp_ptr->is_valid = FALSE;
                 contact_ptr->asp_id_flag = MMIPB_ASP_FIX_ID_EXIST + (uint8)asp_id;                 
                 MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
             }
             break;
         case MMI_PB_ASP_USERDEF_SELECTED:
             {
                wchar *file_name_ptr = (wchar *)param;

                if(file_name_ptr != PNULL)
                {
                    add_data_ptr->asp_ptr->is_valid = TRUE;
                    contact_ptr->asp_id_flag = 0;
                    add_data_ptr->asp_ptr->file_name_len = MIN(MMIAPICOM_Wstrlen(file_name_ptr), MMIFILE_FULL_PATH_MAX_LEN);/*lint !e666*/
                    SCI_MEMCPY(add_data_ptr->asp_ptr->file_name, file_name_ptr , add_data_ptr->asp_ptr->file_name_len*sizeof(wchar));
                    MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
                }                 
             }
             break;
         case MMI_PB_DELETE_ASP_ID:
             //delete asp id
             add_data_ptr->asp_ptr->is_valid = FALSE;
             add_data_ptr->asp_ptr->file_name_len = 0;    
             contact_ptr->asp_id_flag = 0;
             SCI_MEMSET(add_data_ptr->asp_ptr->file_name, 0 , sizeof(add_data_ptr->asp_ptr->file_name));
             MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
             break;
    #endif
         
        case MSG_MULTIM_SELECTED_RETURN:        
            if(PNULL == param)
            {
                //SCI_TRACE_LOW:"[MMIPB] MSG_MULTIM_SELECTED_RETURN param == 0 !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3287_112_2_18_2_43_41_240,(uint8*)"");
                break;
            }
            multim_select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            switch (multim_select_ptr->file_type)
            {
                case MMIFMM_FILE_TYPE_PICTURE:
                if (multim_select_ptr->file_name_len > MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
                {
                    return MMI_RESULT_FALSE;
                }
                else
                {
                    #ifdef MMI_PIC_CROP_SUPPORT
                    CropPhotoFile(multim_select_ptr->file_name_ptr,multim_select_ptr->file_name_len);
                    #else
					//内存分配、旧头像保存
					if(PNULL == s_old_photo_file_name_ptr)
					{
                        s_old_photo_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar));
                        
                        if(PNULL == s_old_photo_file_name_ptr)
                        {
                            return MMI_RESULT_FALSE;
                        }
                        SCI_MEMSET(s_old_photo_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN +1) * sizeof(wchar)));
                        MMIAPICOM_Wstrncpy(
                            s_old_photo_file_name_ptr,
                            add_data_ptr->photo_ptr->file_name,
                            MIN(MMIAPICOM_Wstrlen(add_data_ptr->photo_ptr->file_name), MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
                            );//保存旧头像路径
					}
                    SetContactFilePhoto(win_id,contact_ptr,multim_select_ptr->file_name_ptr,
                    multim_select_ptr->file_name_len,add_data_ptr->photo_ptr);                   
                    #endif
                }
               break;

            case MMIFMM_FILE_TYPE_MUSIC:
                if (multim_select_ptr->file_name_len > MMIPB_MAX_CUSTOM_RING_PATH_LEN)
                {
                    return MMI_RESULT_FALSE;
                }
                else
                {
					//内存分配、旧铃声保存
					if(PNULL == s_old_ring_file_name_ptr)
					{
                        s_old_ring_file_name_ptr = SCI_ALLOC_APP((MMIPB_MAX_CUSTOM_RING_PATH_LEN +1) * sizeof(wchar));
                        
                        if(PNULL == s_old_ring_file_name_ptr)
                        {
                            return MMI_RESULT_FALSE;
                        }
                        SCI_MEMSET(s_old_ring_file_name_ptr, 0, ((MMIPB_MAX_CUSTOM_RING_PATH_LEN +1) * sizeof(wchar)));
                        MMIAPICOM_Wstrncpy(
                            s_old_ring_file_name_ptr,
                            add_data_ptr->ring_ptr->file_name,
                            MIN(MMIAPICOM_Wstrlen(add_data_ptr->ring_ptr->file_name), MMIPB_MAX_CUSTOM_RING_PATH_LEN)
                            );//保存旧铃声路径
					}
                    add_data_ptr->ring_ptr->is_valid = TRUE;
                    contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN);
                    contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN);
                    name_len = MIN((multim_select_ptr->file_name_len), MMIPB_MAX_CUSTOM_RING_PATH_LEN);
                    add_data_ptr->ring_ptr->file_name_len = name_len;
                    SCI_MEMSET(add_data_ptr->ring_ptr->file_name,0,sizeof(add_data_ptr->ring_ptr->file_name));

                    MMIAPICOM_Wstrncpy(add_data_ptr->ring_ptr->file_name, multim_select_ptr->file_name_ptr, name_len);

                    //contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag&MMIPB_RING_ID_CLEAN)+MMIPB_RING_ID_EXIST; 
                    contact_ptr->ring_id_flag = contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN; 
                    add_data_ptr->ring_ptr->ring_type = multim_select_ptr->music_type;
                    add_data_ptr->ring_ptr->file_size = multim_select_ptr->file_size;
                    
					//+ Bug 125726 
                    //after selecting ring file from sound files, 判断是否空间充足
                    if(MMIPB_ERROR_NO_SPACE == MMIPB_AddContact_RingInfo(contact_ptr, add_data_ptr->ring_ptr))
                    {
                        MMI_HANDLE_T    applet_handle = 0;
                        
                        if(MMIPB_GetCurAppletHandle(&applet_handle))
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,
                                TXT_NO_SPACE,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
                        }
                    }
					//- Bug 125726 
                    MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);      
                }
                break;
            default:
                break;
        }
        
        break;
        
        case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
        if(PNULL == param)
        {
            //SCI_TRACE_LOW:"[MMIPB] MSG_SET_SELECT_FIX_CALL_RING_RETURN param == 0 !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3352_112_2_18_2_43_41_241,(uint8*)"");
            break;
        }

        ring_id_ptr = (uint16 *)param;
        //SCI_TRACE_LOW:"[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3357_112_2_18_2_43_41_242,(uint8*)"d", *ring_id_ptr);
        add_data_ptr->ring_ptr->is_valid = FALSE; //TRUE;  //CR223191
        contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST;
        contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG;//exist fixed id
        contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr;//set fixed id

        //update list display
        MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
        
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        #ifdef MMI_PDA_SUPPORT
        GetUserEditContact(win_id,contact_ptr);
        #endif

//        MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle,PNULL);
		MMIPUB_CloseAlertWinEx(add_data_ptr->applet_handle);
        if(MMIPB_IsContactValid(contact_ptr))
        {
//            MMIPUB_OpenQueryWinByTextIdEx(add_data_ptr->applet_handle, TXT_PB_IS_SAVE_CONTACT, IMAGE_PUBWIN_QUERY, PNULL, PNULL, PNULL);
			MMIPUB_OpenAlertWinByTextIdEx(add_data_ptr->applet_handle,PNULL,
                        TXT_PB_IS_SAVE_CONTACT,TXT_NULL, IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL,PNULL);
        }
        else
        {
	        MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_RED:
        /*对于featurephone，编辑联系人&短信等情况下，在按power键退出编辑时，需要提示是否保存，若要回到idle界面下，需要按两次power键*/
#ifndef MMI_PDA_SUPPORT
        MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle,PNULL);
        if(MMIPB_IsContactValid(contact_ptr))
        {
            MMIPUB_OpenQueryWinByTextIdEx(add_data_ptr->applet_handle, TXT_PB_IS_SAVE_CONTACT, IMAGE_PUBWIN_QUERY, PNULL, PNULL, PNULL);
        }
        else
        {
            MMK_ReturnIdleWin();
        }
#endif      
		break;
        
    case MSG_PROMPTWIN_CANCEL: 
		if(PNULL != s_old_photo_file_name_ptr)
		{
			//获取旧头像显示
			add_data_ptr->photo_ptr->file_name_len = MMIAPICOM_Wstrlen(s_old_photo_file_name_ptr);
			SCI_MEMSET(add_data_ptr->photo_ptr->file_name, 0, MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN + 1);
			MMIAPICOM_Wstrncpy(
				add_data_ptr->photo_ptr->file_name,
				s_old_photo_file_name_ptr,
				MIN(add_data_ptr->photo_ptr->file_name_len, MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN)
                    );//保存旧头像路径
			MMIPB_AddContact_PhotoInfo(add_data_ptr->contact_ptr, add_data_ptr->photo_ptr);
		}
        if(PNULL != s_old_ring_file_name_ptr)
		{
			//获取旧铃声显示
			add_data_ptr->ring_ptr->file_name_len = MMIAPICOM_Wstrlen(s_old_ring_file_name_ptr);
			SCI_MEMSET(add_data_ptr->ring_ptr->file_name, 0, MMIPB_MAX_CUSTOM_RING_PATH_LEN + 1);
			MMIAPICOM_Wstrncpy(
				add_data_ptr->ring_ptr->file_name,
				s_old_ring_file_name_ptr,
				MIN(add_data_ptr->ring_ptr->file_name_len, MMIPB_MAX_CUSTOM_RING_PATH_LEN)
                    );//保存旧铃声路径
			MMIPB_AddContact_RingInfo(add_data_ptr->contact_ptr, add_data_ptr->ring_ptr);
		}
 //       MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle, PNULL);
	    MMIPUB_CloseAlertWinEx(add_data_ptr->applet_handle);
        MMK_CloseWin(win_id);
		break;
    case MSG_PROMPTWIN_OK:      //确定退出
 //       MMIPUB_CloseQuerytWinEx(add_data_ptr->applet_handle, PNULL);
	    MMIPUB_CloseAlertWinEx(add_data_ptr->applet_handle);
//        MMK_SendMsg(win_id, MSG_APP_OK, PNULL);            
		break;
        
    case MSG_CLOSE_WINDOW:            
		#ifdef HERO_ENGINE_TTS_SUPPORT
		s_hero_pb_addcontact_win_id = 0;
        #endif			
        MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_SELECT_IM_TYPE_WIN_ID)); 
        MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_SELECT_NUMBER_TYPE_WIN_ID)); 
        MMK_CloseWin(MMK_GetWinHandle(add_data_ptr->applet_handle,MMIPB_ADD_TO_GROUP_WIN_ID)); 
        FreeContactPhotoRingInfor(add_data_ptr);
        MMK_FreeWinAddData(win_id);		
        if(PNULL != s_old_photo_file_name_ptr)
        {
            SCI_FREE(s_old_photo_file_name_ptr);
        }
        if(PNULL != s_old_ring_file_name_ptr)
        {
            SCI_FREE(s_old_ring_file_name_ptr);
        }
        break;
#ifdef HERO_ENGINE_TTS_SUPPORT
	 case MSG_LOSE_FOCUS:
		s_hero_pb_addcontact_win_id = 0;
		break;
	 case MSG_GET_FOCUS:
		s_hero_pb_addcontact_win_id = win_id;
		break;
#endif
        
    default:
        result  = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description : set pb contact photo infor
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/                                              
LOCAL void SetPhotoAnimParam(MMI_HANDLE_T win_id,
    GUIANIM_DATA_INFO_T *data_infor_ptr,
    GUIANIM_FILE_INFO_T *file_infor_ptr)
{
    GUIANIM_CTRL_INFO_T ctrl_infor = {0};
    GUIANIM_DISPLAY_INFO_T display_info={0};

    ctrl_infor.is_wallpaper_ctrl = FALSE;
    ctrl_infor.is_ctrl_id = TRUE;
    ctrl_infor.ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_ANIM_PHOTO_ID);

    if(PNULL!=file_infor_ptr)
    {
        display_info.is_update = FALSE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_crop_rect = TRUE;
            display_info.is_zoom = TRUE;
            display_info.is_syn_decode = TRUE;
    }
    else
    {
        display_info.is_update   = FALSE;
            display_info.is_disp_one_frame = TRUE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    }
    GUIANIM_SetParam(&ctrl_infor,data_infor_ptr,file_infor_ptr, 
        &display_info);
}


/*****************************************************************************/
//  Description : free menu infor contact,photo,ring and asp memory
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/                                              
LOCAL void FreeContactPhotoRingInfor(MMIPB_MENU_DATA_INFOR_T * add_data_ptr)
{
    if(PNULL == add_data_ptr)
    {
        return;
    }

    if(PNULL != add_data_ptr->contact_ptr)
    {
        SCI_FREE(add_data_ptr->contact_ptr);
    }

    if (PNULL != add_data_ptr->ring_ptr)
    {
        SCI_FREE(add_data_ptr->ring_ptr);
        add_data_ptr->ring_ptr = PNULL;
    }

    if (PNULL != add_data_ptr->photo_ptr)
    {
        SCI_FREE(add_data_ptr->photo_ptr);
        add_data_ptr->photo_ptr = PNULL;
    }

#ifdef ASP_SUPPORT
    if(PNULL != add_data_ptr->asp_ptr)
    {
        SCI_FREE(add_data_ptr->asp_ptr);
        add_data_ptr->asp_ptr = PNULL;
    }  
#endif
}

/*****************************************************************************/
//  Description : add image id
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SelectPhotoFromFile(MMIPB_MENU_DATA_INFOR_T*  data_ptr)
{
    BOOLEAN result = FALSE;
    MMIFMM_SELECT_INFO_T    select_info = {0};
    if(PNULL == data_ptr||PNULL == data_ptr->photo_ptr)
    {
        return FALSE;
    }
#ifdef DRM_SUPPORT   
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL | MMIFMM_DRM_ALL;
#else
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
#endif
    select_info.win_id = data_ptr->parent_win;
    if(MMIAPIFMM_OpenSelectPictureWin(&select_info))
    {
#ifdef DRM_SUPPORT 
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
        limit_value.is_rights_valid = 1;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        result = TRUE;
    }
    
    return result;
}


#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : select asp id call back
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPSelectCallback(
								BOOLEAN is_fixed_id,
								uint32  fixed_id,
								wchar   *asp_file_name
						        )
{
        BOOLEAN result = FALSE;
        MMI_HANDLE_T handle = 0;
//    
        MMIPB_GetCurAppletHandle(&handle);
        if(is_fixed_id)
        {
		//fixed asp id
            
		MMK_SendMsg(MMK_GetWinHandle(handle,MMIPB_ADD_IN_NV_WIN_ID), MMI_PB_ASP_FIXID_SELECTED, (ADD_DATA)fixed_id);
		result = TRUE;
        
        }
        else
        {
		//user defined asp file
		MMK_SendMsg(MMK_GetWinHandle(handle,MMIPB_ADD_IN_NV_WIN_ID), MMI_PB_ASP_USERDEF_SELECTED, (ADD_DATA)asp_file_name);
		result = TRUE;
        }
        return result;
}


/*****************************************************************************/
//  Description : add asp id
//  Global resource dependence : 
//  Author:xiaohua.liu 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddASPID(MMI_HANDLE_T add_win_handle, //IN:
						MMIPB_CONTACT_T *contact_ptr,  //IN:
						MMIPB_CUSTOM_RING_T*  asp_ptr//IN
						)
{
    MMIAPIASP_SelectAnswerRing(ASPSelectCallback);
  
    return TRUE;
}
#endif


/*****************************************************************************/
//  Description :append  icon and text to list item
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(MMI_CTRL_ID_T       ctrl_id,
    uint32              item_state,
    uint32              item_id,
    uint32              item_index,      
    GUIITEM_STYLE_E     item_style,
    MMI_TEXT_ID_T       left_softkey_id,
    MMI_TEXT_ID_T       middle_softkey_id,
    MMI_TEXT_ID_T       right_softkey_id,
    GUIITEM_DATA_TYPE_E icon_type,
    MMI_IMAGE_ID_T      icon_id,
    GUIANIM_FILE_INFO_T* anim_path,
    const MMI_STRING_T*  str_ptr,
    MMI_TEXT_ID_T       default_text_id,
    BOOLEAN is_update
    )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16               content_index = 0;
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    item_t.item_state    = item_state;
    item_t.user_data     = item_id;
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style || GUIITEM_STYLE_TWO_LINE_ANIM_TEXT== item_style)
    {
       // item_data.item_content[content_index].item_data_type = icon_type;
        
        if(GUIITEM_DATA_ANIM_PATH == icon_type && PNULL != anim_path
            && PNULL != anim_path->full_path_wstr_ptr && 0 != anim_path->full_path_wstr_len)
        {
            //need admin
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[content_index].item_data.anim_path_ptr = anim_path;
        }
        else if(GUIITEM_DATA_IMAGE_ID == icon_type)
        {
            //need icon
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[content_index].item_data.image_id = icon_id;
        }
        content_index++;
    }
    
    if ( PNULL != str_ptr && 0 < str_ptr->wstr_len && PNULL != str_ptr->wstr_ptr )
    {
        item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer = *str_ptr;
    }
    else
    {
        item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[content_index].item_data.text_id = default_text_id;
    }
    if(TRUE == is_update)
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t ,item_index);
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 将电话簿记录中的号码设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void AppendListNumberItem(MMI_CTRL_ID_T       ctrl_id,
        uint32              item_id,
        uint16              item_index,
        uint16              storage_id,
        uint8               num_type,
        MMIPB_BCD_NUMBER_T  *number_ptr,
        MMI_TEXT_ID_T       left_softkey_id,
        MMI_TEXT_ID_T       middle_softkey_id,
        MMI_TEXT_ID_T       right_softkey_id,
        BOOLEAN             is_update
)
{
    MN_NUMBER_TYPE_E ton = 0;
    MMIPB_STRING_T  string_t = {0};
    MMI_STRING_T    com_string_t = {0};
    wchar           wstr[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};

    ton = MMIPB_GetNumberTypeFromUint8(number_ptr->npi_ton);    
    string_t.strlen = MMIAPICOM_GenDispNumber(
                                        (ton == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : ton),
                                        number_ptr->number_len, 
                                        number_ptr->number,
                                        string_t.strbuf,
                                        MMIPB_NUMBER_MAX_STRING_LEN+2
                                        );
    
    com_string_t.wstr_ptr = wstr;
    com_string_t.wstr_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, string_t.strlen);   
    MMI_STRNTOWSTR(com_string_t.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN+2, 
                                (const uint8*)string_t.strbuf, MMIPB_MAX_STRING_LEN +2, 
                                com_string_t.wstr_len);	
    
    AppendListItem(ctrl_id, 0, 
            item_id, 
            item_index,
            GUIITEM_STYLE_ONE_LINE_ICON_TEXT, 
            left_softkey_id,middle_softkey_id,right_softkey_id, 
            GUIITEM_DATA_IMAGE_ID,
             MMIPB_GetIconIdOfPhone( num_type ), 
            PNULL,
            &com_string_t,
            s_listbox_title_default_str[item_id],
            is_update);
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 将电话簿记录中的号码设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetContactAddOrEditItem(MMI_CTRL_ID_T ctrl_id,
            MMIPB_MENU_DATA_INFOR_T * add_data_ptr,               
            uint32                     item_id,
            uint32                     item_index,
            MMI_TEXT_ID_T       left_softkey_id,
            MMI_TEXT_ID_T       middle_softkey_id,
            MMI_TEXT_ID_T       right_softkey_id,
            BOOLEAN is_update)
{  
    wchar wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    BOOLEAN is_append_item = FALSE;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    MMI_STRING_T str_t = {0};
    MMI_IMAGE_ID_T default_img_id = 0;
#ifdef DRM_SUPPORT
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMIDRMFILE_HANDLE       drm_handle = 0;
    DRM_PERMISSION_MODE_E   drm_permission = DRM_PERMISSION_NONE;
    BOOLEAN                 is_valid = FALSE;
    BOOLEAN                 is_drm_permission = TRUE;
#endif

    if(PNULL == add_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]SetContactAddOrEditItem paramer illegal !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3762_112_2_18_2_43_42_243,(uint8*)"");
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]SetContactAddOrEditItem item_id = %d,item_index = %d, is_update =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3768_112_2_18_2_43_42_244,(uint8*)"ddd",item_id,item_index,is_update);        
        contact_ptr = add_data_ptr->contact_ptr;
    }
    //added by Tong Yongli. According to Coverity CID 18427
    if(PNULL == add_data_ptr->photo_ptr)
    {
        return FALSE;
    }
       
    switch (item_id)
    {
    case MMIPB_LISTBOX_INDEX_PHOTO:
        {
            GUIITEM_DATA_TYPE_E icon_type = GUIITEM_DATA_IMAGE_ID;
            GUIANIM_FILE_INFO_T anim_path = {0};
            BOOLEAN     is_photo_id_exist = FALSE;
            if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST))
            {
                if (PNULL!=add_data_ptr->photo_ptr&&add_data_ptr->photo_ptr->file_name_len>0)
                {
                    is_photo_id_exist = TRUE;
                }
            }
            //判断DRM文件，是否版权过期 @Bug 114162 
#ifdef DRM_SUPPORT
            if(PNULL != add_data_ptr->photo_ptr)
            {
                file_type = MMIAPIFMM_GetFileTypeByFileName(add_data_ptr->photo_ptr->file_name, add_data_ptr->photo_ptr->file_name_len);
                SCI_TRACE_LOW("[MMIPB] SetContactAddOrEditItem file_type =%d", file_type);
                if(MMIFMM_FILE_TYPE_DRM == file_type)
                {
					drm_handle = SFS_CreateFile(add_data_ptr->photo_ptr->file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
					if(SFS_INVALID_HANDLE != drm_handle)
					{
						drm_permission = MMIAPIDRM_GetFilePermission(drm_handle, PNULL);    
						is_valid = MMIAPIDRM_IsRightsValid(drm_handle, PNULL, TRUE, drm_permission);
						if (TRUE != is_valid)
						{
							icon_type = GUIITEM_DATA_IMAGE_ID;
							is_drm_permission = FALSE;
						}
					}
					SCI_TRACE_LOW("[MMIPB] SetContactAddOrEditItem is_drm_permission = %d", is_drm_permission);
					SFS_CloseFile(drm_handle);
                }
            }
			
#endif
			
            if(is_photo_id_exist && PNULL != add_data_ptr->photo_ptr)
            {	            
                icon_type = GUIITEM_DATA_ANIM_PATH;            	    
                anim_path.full_path_wstr_len = add_data_ptr->photo_ptr->file_name_len;
                anim_path.full_path_wstr_ptr = add_data_ptr->photo_ptr->file_name; 
            }
            else
            {                
                icon_type = GUIITEM_DATA_IMAGE_ID;
            }

#ifdef DRM_SUPPORT
            if (FALSE == is_drm_permission)
            {
                icon_type = GUIITEM_DATA_IMAGE_ID;
            }
#endif
            AppendListItem(ctrl_id, 0, 
                    item_id, 
                    item_index,
                    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT,
                    left_softkey_id,middle_softkey_id,right_softkey_id, 
                    icon_type,
                    IAMGE_PB_DEFAULT_PHOTO, 
                    &anim_path,
                    PNULL,
                    TXT_PB_MODIFY_PHOTO,
                    is_update);   
        }
        break;
    case MMIPB_LISTBOX_INDEX_NAME:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_NAME;20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = contact_ptr->name.wstr_len;
        str_t.wstr_ptr = contact_ptr->name.wstr;
        is_append_item = TRUE;         
        break;   
        
    case MMIPB_LISTBOX_INDEX_NUMBER_MOBILE:
    case MMIPB_LISTBOX_INDEX_NUMBER_HOME:
#ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_NUMBER_OFFICE:
    //case MMIPB_LISTBOX_INDEX_NUMBER_FAX:
    //case MMIPB_LISTBOX_INDEX_NUMBER_OTHER:
#endif
        {
            uint8 number_index = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
            AppendListNumberItem(ctrl_id,
                                item_id,
                                item_index,
                                contact_ptr->storage_id,
                                contact_ptr->num_type[number_index],
                                &contact_ptr->number[number_index],
                                left_softkey_id,middle_softkey_id,right_softkey_id,
                                is_update);
        }
        break;
#ifdef MMIPB_MAIL_SUPPORT
    case MMIPB_LISTBOX_INDEX_MAIL:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_MAIL_ADDRESS; 20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = contact_ptr->mail.wstr_len;
        str_t.wstr_ptr = contact_ptr->mail.wstr;        
        is_append_item = TRUE;            
        break;
#endif

    case MMIPB_LISTBOX_INDEX_GROUP:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_SELECT_GROUP; 20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = MMIPB_GetMultiGrouName(contact_ptr->union_t.group, wstr);
        str_t.wstr_ptr = wstr;        
        is_append_item = TRUE; 
        break;

    case MMIPB_LISTBOX_INDEX_RING:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_MIDI; 20151126 wang.qinggang pb_image_clean
        if(contact_ptr->ring_id_flag&MMIPB_FIXED_RING_FLAG)
        {
            if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
            {
                str_t.wstr_ptr = wstr; 
                str_t.wstr_len = MMIAPISET_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
                                                        str_t.wstr_ptr, MMIPB_MAX_STRING_LEN);
            }
        }
        else if((PNULL != add_data_ptr->ring_ptr) && (add_data_ptr->ring_ptr->is_valid))
        {
            str_t.wstr_len = add_data_ptr->ring_ptr->file_name_len;
            str_t.wstr_ptr = add_data_ptr->ring_ptr->file_name;
        }
        else
        {
            str_t.wstr_ptr = wstr;
            str_t.wstr_len = MMIPB_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
            str_t.wstr_ptr, MMIPB_MAX_STRING_LEN);            
        }
        is_append_item = TRUE;  
        break;


#ifdef MMIPB_IM_MESSAGE_SUPPORT
    case MMIPB_LISTBOX_INDEX_QQ:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_SNE; 20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = contact_ptr->imessager.wstr_len;
        str_t.wstr_ptr = contact_ptr->imessager.wstr; 
        is_append_item = TRUE; 
        break;
#endif

 #ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_URL:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_SNE; 20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = contact_ptr->url.wstr_len;
        str_t.wstr_ptr = contact_ptr->url.wstr;
        is_append_item = TRUE;
        break;       
    case MMIPB_LISTBOX_INDEX_BIRTH:
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_DUTY; 20151126 wang.qinggang pb_image_clean
        str_t.wstr_len = contact_ptr->birth.wstr_len;
        str_t.wstr_ptr = contact_ptr->birth.wstr;
        is_append_item = TRUE;
        break;
    case MMIPB_LISTBOX_INDEX_MEMO:
#ifdef CMCC_UI_STYLE
        default_img_id = IMAGE_PHONEBOOK_ADDRESS;
#else
        default_img_id = IMAGE_NULL;//IMAGE_PHONEBOOK_MEMO;20151126 wang.qinggang pb_image_clean
#endif
        str_t.wstr_len = contact_ptr->memo.wstr_len;
        str_t.wstr_ptr = contact_ptr->memo.wstr;
        is_append_item = TRUE;
        break;   
#endif
    default:
        //SCI_TRACE_LOW:"[MMIPB]SetContactAddOrEditItem index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3919_112_2_18_2_43_42_245,(uint8*)"d",item_id);
        break;
    }

    if(TRUE == is_append_item )
    {    
        AppendListItem(ctrl_id, 0, 
                item_id, 
                item_index,
                GUIITEM_STYLE_ONE_LINE_ICON_TEXT, 
                left_softkey_id,middle_softkey_id,right_softkey_id, 
                GUIITEM_DATA_IMAGE_ID,
                default_img_id, 
                PNULL,
                &str_t,
                s_listbox_title_default_str[item_id],
                is_update); 
            
    }
	return FALSE;
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 更新新增或者编辑界面的信息
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateContactAddOrEditItems(MMI_CTRL_ID_T ctrl_id,
            MMIPB_MENU_DATA_INFOR_T * add_data_ptr,
            void*param_ptr)
{
    MMI_TEXT_ID_T       left_softkey_id= STXT_SOFTKEY_SAVE_MK;
#ifdef GUI_MID_SOFTKEY_USE_IMG
    MMI_IMAGE_ID_T      middle_softkey_id =  IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
    MMI_TEXT_ID_T       middle_softkey_id= TXT_EDIT;
#endif
    MMI_TEXT_ID_T       right_softkey_id= STXT_RETURN;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    MMI_STRING_T*str_ptr = PNULL;
    uint32 item_index = 0;
    uint32 item_id = 0;  

    if(PNULL == add_data_ptr || PNULL == add_data_ptr->contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]UpdateContactAddEditInfo paramer illegal !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3959_112_2_18_2_43_42_246,(uint8*)"");
        return FALSE;
    }
    else
    {
        contact_ptr = add_data_ptr->contact_ptr;
    }
    
    item_index = GUILIST_GetCurItemIndex(ctrl_id);    
    GUILIST_GetItemData(ctrl_id, item_index, &item_id);
   
    //SCI_TRACE_LOW:"[MMIPB]UpdateContactAddEditInfo cur item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_3970_112_2_18_2_43_42_247,(uint8*)"d",item_id);
    switch (item_id)
    {    
    case MMIPB_LISTBOX_INDEX_PHOTO: 
	 /*   SetContactAddOrEditItem(ctrl_id, 
                                    add_data_ptr,
                                    MMIPB_LISTBOX_INDEX_PHOTO, 
                                    item_index,
                                    left_softkey_id, middle_softkey_id,right_softkey_id,
                                    TRUE); */
        break;

    case MMIPB_LISTBOX_INDEX_RING:
	   /* SetContactAddOrEditItem(ctrl_id, 
                                    add_data_ptr,
                                    MMIPB_LISTBOX_INDEX_RING, 
                                    item_index,
                                    left_softkey_id, middle_softkey_id,right_softkey_id,
                                    TRUE); */
        break;
 
    case MMIPB_LISTBOX_INDEX_NAME:
        str_ptr = (MMI_STRING_T*)param_ptr;
        if(str_ptr->wstr_len != 0)
        {
            contact_ptr->name.wstr_len = MIN(str_ptr->wstr_len, MMIPB_NAME_MAX_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->name.wstr, str_ptr->wstr_ptr, contact_ptr->name.wstr_len);
        }
        else
        {
            contact_ptr->name.wstr_len = 0;
            SCI_MEMSET(contact_ptr->name.wstr, 0x00,sizeof(contact_ptr->name.wstr));
        }                 
        break;   
    case MMIPB_LISTBOX_INDEX_NUMBER_MOBILE:
    case MMIPB_LISTBOX_INDEX_NUMBER_HOME:
#ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_NUMBER_OFFICE:
    //case MMIPB_LISTBOX_INDEX_NUMBER_FAX:
    //case MMIPB_LISTBOX_INDEX_NUMBER_OTHER:
#endif    
     {
            MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
            uint8  num_buf[MMIPB_NUMBER_MAX_STRING_LEN] = {0};
            uint8  number_index = 0;
            uint16 num_len = 0;
            
            str_ptr = (MMI_STRING_T*)param_ptr;
            number_index = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE; 
            num_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, str_ptr->wstr_len);
            MMI_WSTRNTOSTR(num_buf, MMIPB_NUMBER_MAX_STRING_LEN, str_ptr->wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN, num_len);

            if (str_ptr->wstr_len > 0 && number_index < MMIPB_MAX_NV_PHONE_NUM && MMIAPICOM_GenPartyNumber((uint8 *)(num_buf), num_len, &party_num))
            {
                contact_ptr->num_type[number_index] = MMIPB_ADN_NUMBER+number_index;
                contact_ptr->number[number_index].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
                contact_ptr->number[number_index].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                SCI_MEMCPY(contact_ptr->number[number_index].number, party_num.bcd_num, contact_ptr->number[number_index].number_len);
            }
            else
            {
                if (MMIPB_MAX_NV_PHONE_NUM > number_index)
                {
                    contact_ptr->number[number_index].number_len = 0;
                    contact_ptr->number[number_index].npi_ton = 0;
                    contact_ptr->num_type[number_index] = MMIPB_INVLID_NUMBER;
                    
                }
            }
        }
        break;
#ifdef MMIPB_MAIL_SUPPORT
    case MMIPB_LISTBOX_INDEX_MAIL:
        str_ptr = (MMI_STRING_T*)param_ptr;
        contact_ptr->mail.wstr_len = MIN(str_ptr->wstr_len, MMIPB_MAX_MAIL_LEN);
        MMIAPICOM_Wstrncpy(contact_ptr->mail.wstr, str_ptr->wstr_ptr, contact_ptr->mail.wstr_len);            
        break;
#endif

#ifdef MMIPB_IM_MESSAGE_SUPPORT
    case MMIPB_LISTBOX_INDEX_QQ:
        str_ptr = (MMI_STRING_T*)param_ptr;
        contact_ptr->imessager.wstr_len = MIN(str_ptr->wstr_len, MMIPB_IM_MAX_LEN);
        MMIAPICOM_Wstrncpy(contact_ptr->imessager.wstr, str_ptr->wstr_ptr, contact_ptr->imessager.wstr_len);            
        break;
#endif

#ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_URL:
        str_ptr = (MMI_STRING_T*)param_ptr;
        contact_ptr->url.wstr_len = MIN(str_ptr->wstr_len, MMIPB_URL_MAX_LEN);
        MMIAPICOM_Wstrncpy(contact_ptr->url.wstr, str_ptr->wstr_ptr, contact_ptr->url.wstr_len);            
        break;
        
    //case MMIPB_LISTBOX_INDEX_BIRTH:
        //str_ptr = (MMI_STRING_T*)param_ptr;
        //contact_ptr->mail.wstr_len = MIN(str_ptr->wstr_len, MMIPB_URL_MAX_LEN);
        //MMIAPICOM_Wstrncpy(contact_ptr->mail.wstr, str_ptr->wstr_ptr, contact_ptr->mail.wstr_len);            
        //break;
    case MMIPB_LISTBOX_INDEX_MEMO:
        str_ptr = (MMI_STRING_T*)param_ptr;
        if (str_ptr->wstr_len > 0)
        {
            contact_ptr->memo.wstr_len = MIN(str_ptr->wstr_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
            MMIAPICOM_Wstrncpy(contact_ptr->memo.wstr, str_ptr->wstr_ptr, contact_ptr->memo.wstr_len);
        }
        else
        {
            contact_ptr->memo.wstr_len = 0;
            SCI_MEMSET(contact_ptr->memo.wstr, 0x00, sizeof(contact_ptr->memo.wstr));
        }
        break;      
#endif
    default:
        //SCI_TRACE_LOW:"[MMIPB]UpdateContactAddEditInfo default error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_4122_112_2_18_2_43_43_248,(uint8*)"");
        break;
    }

    SetContactAddOrEditItem(ctrl_id, 
                            add_data_ptr,
                            item_id, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            TRUE); 
	return TRUE;
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 设置添加到手机时的LISTBOX各控件的值
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void InitContactEditItems(MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T        ctrl_id,    //IN:
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr,  //IN:
    MMI_TEXT_ID_T           left_softkey_id    
)
{
    uint8          i = 0;
    //uint8          loop_count = 0;
    uint16         item_index = 0;
    uint16 new_id = 0;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
#ifdef GUI_MID_SOFTKEY_USE_IMG  
    MMI_IMAGE_ID_T      middle_softkey_id= IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else    
    MMI_TEXT_ID_T       middle_softkey_id= TXT_EDIT;
#endif
    MMI_TEXT_ID_T       right_softkey_id= STXT_RETURN;
    MMIPB_FIELD_USAGE_INOF_T field_infor = {0};
    
    if(PNULL == add_data_ptr || PNULL == add_data_ptr->contact_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_4157_112_2_18_2_43_43_249,(uint8*)"");
        return;
    }
    else
    {
        contact_ptr = add_data_ptr->contact_ptr;
    }
    new_id = MMIPB_GetFieldUageInfo(contact_ptr->storage_id,contact_ptr->contact_id,
        &field_infor);
//Bug203018: update联系人字段时，各个字段的空间使用情况不需要再从底层接口判断
#ifdef CMCC_VCARD_SUPPORT
    if(MMIPB_OPERATE_TYPE_EDIT_ONE == add_data_ptr->operate_type)
    {
        PHONEBOOK_VCARD_STATUS_T usim_storage_info = {0};
        SCI_TRACE_LOW("yanyan InitContactEditItems enter cmcc_vcard_support");       
        PHONEBOOK_Get3GPhonebookStatusEx(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id), &usim_storage_info);
        if(PHONEBOOK_SIM_STORAGE == MMIPB_GET_CONTACT_TYPE(contact_ptr->storage_id) && MMIPB_IsUsim(contact_ptr->storage_id))
        {
            //set number flag
            field_infor.number_free_flag[0] = TRUE; 
            for(i = 0; i < MIN(MMIPB_GetUSIMFreeANRCount(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id)), (MMIPB_MAX_NV_PHONE_NUM -1)); i++)
            {
                if(0 < add_data_ptr->contact_ptr->number[i+1].number_len)
                {
                    field_infor.number_free_flag[i+1] = TRUE;
                }
                else
                {
                    if((0 < usim_storage_info.storage_usage[0].anr_usage[i].free_space_num)
                        &&(0 < usim_storage_info.storage_usage[0].anr_usage[i].rec_len))
                    {
                        field_infor.number_free_flag[i+1] = TRUE;
                    }
                }
            }           
#ifdef MMIPB_MAIL_SUPPORT
            //set mail flag
            if(0 < add_data_ptr->contact_ptr->mail.wstr_len)
            {
                field_infor.mail_free_flag = TRUE;
            }
            else
            {
                if((0 < usim_storage_info.storage_usage[0].email_usage.free_space_num)
                            &&(0 < usim_storage_info.storage_usage[0].email_usage.rec_len))
                {
                    field_infor.mail_free_flag = TRUE;
                }
            }
#endif

            //set sne flag
            if(0 < add_data_ptr->contact_ptr->alias.wstr_len)
            {
                field_infor.sne_free_flag = TRUE;
            }
            else
            {
                if((0 < usim_storage_info.storage_usage[0].sne_usage.free_space_num)
                        &&(0 < usim_storage_info.storage_usage[0].sne_usage.rec_len))
                {
                    field_infor.sne_free_flag = TRUE;
                }
            }
#ifdef MMIPB_MAIL_SUPPORT						
            SCI_TRACE_LOW("yanyan InitContactEditItems nf[1]:%d, ef:%d,sf:%d",
                field_infor.number_free_flag[1],
                field_infor.mail_free_flag,
                field_infor.sne_free_flag
                );
#endif
        }
        
    }
#endif
    if(new_id>0)
    {
        contact_ptr->contact_id = new_id;
    }
    // 设置item的最大数目
    GUILIST_SetMaxItem(
        ctrl_id, 
        MMIPB_LISTBOX_INDEX_MAX,
        FALSE
        );
     // photo
	if(field_infor.photo_free_flag)        
	{     
	    SetContactAddOrEditItem(ctrl_id, 
                                    add_data_ptr,
                                    MMIPB_LISTBOX_INDEX_PHOTO, 
                                    item_index,
                                    left_softkey_id, middle_softkey_id,right_softkey_id,
                                    FALSE); 
                    item_index++;
	}

    // name
    SetContactAddOrEditItem(ctrl_id, 
                            add_data_ptr,
                            MMIPB_LISTBOX_INDEX_NAME, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE); 
    item_index++;
     
    //number 
    //MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&loop_count);   
    for ( i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++ )
    { 
        if(field_infor.number_free_flag[i])
        {
            SetContactAddOrEditItem(ctrl_id, 
                add_data_ptr,
                MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i,
                item_index,
                left_softkey_id, middle_softkey_id,right_softkey_id,
                FALSE); 
            item_index++;
        }
    }
#ifdef MMIPB_MAIL_SUPPORT
     // mail
    if(field_infor.mail_free_flag)
    {
        if(!MMIPB_IsInSIMGroupByStorage(contact_ptr->storage_id) 
            || ((MMI_SIM_TYPE_USIM == MMIPB_GetSimTypeByStorageId(contact_ptr->storage_id)) && 
            (PHONEBOOK_SIM_STORAGE ==  (contact_ptr->storage_id>>8)))
            )
        {
            //nv or usim 
            SetContactAddOrEditItem(ctrl_id, /*lint !e64*/
                add_data_ptr,
                MMIPB_LISTBOX_INDEX_MAIL,
                item_index,
                left_softkey_id, middle_softkey_id,right_softkey_id,
                FALSE); 
            item_index++;
            
        }
    }
#endif

        if(field_infor.group_free_falg)
        {
        //group
            SetContactAddOrEditItem(ctrl_id, 
                                add_data_ptr,
                                MMIPB_LISTBOX_INDEX_GROUP,
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 	
            item_index++;	
        }
        // ring
        if(field_infor.ring_free_falg)
        {
            SetContactAddOrEditItem(ctrl_id, 
                                add_data_ptr,
                                MMIPB_LISTBOX_INDEX_RING,
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE);
            item_index++;
        }

#ifdef MMIPB_IM_MESSAGE_SUPPORT
        // qq
        if(field_infor.im_free_flag)
        {
            SetContactAddOrEditItem(ctrl_id, 
                                add_data_ptr,
                                MMIPB_LISTBOX_INDEX_QQ,
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 	
            item_index++;	
        }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY  	
        //url
        if(field_infor.url_free_flag)
        {
            SetContactAddOrEditItem(ctrl_id, 
                                add_data_ptr,
                               MMIPB_LISTBOX_INDEX_URL,
                               item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 	
             item_index++;	
        }
        //生日
//            SetContactAddOrEditItem(ctrl_id, 
//                                add_data_ptr,
//                                MMIPB_LISTBOX_INDEX_BIRTH,
//                                item_index,
//                                left_softkey_id, middle_softkey_id,right_softkey_id,
//                                FALSE); 	    
//            item_index++;        

        //备忘录
        if(field_infor.memo_free_flag)
        {
            SetContactAddOrEditItem(ctrl_id, 
                                add_data_ptr,
                                MMIPB_LISTBOX_INDEX_MEMO,
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 	    
            item_index++;	 
        }
#endif

    //SCI_TRACE_LOW:"[MMIPB]InitContactEditItems total item = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_4295_112_2_18_2_43_43_250,(uint8*)"d",item_index);
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 打开输入窗口
//  Global resource dependence : none
//  Author:  starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetInputBox(MMI_STRING_T *text_ptr,
                         uint16       max_ucs2_len,
                         uint16       max_alpha_len,
                         uint8        allow_inputmethod)
{
    MMISET_LANGUAGE_TYPE_E    lang_type = MMISET_MAX_LANGUAGE;
    GUIIM_TYPE_T                      input_mode = GUIIM_TYPE_SMART;
    GUI_FONT_T                        font_type = MMI_DEFAULT_TEXT_FONT;
    MMI_CTRL_ID_T                   ctrl_id = MMIPB_EDITBOX_CTRL_ID;    

    if(GUIIM_TYPE_DIGITAL == allow_inputmethod)
    {
        //only digital mode 
        input_mode = GUIIM_TYPE_DIGITAL;
    }
    else 
    {
        if((IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET)  == allow_inputmethod)/*lint !e655*/
        {
            //only character mode 
            input_mode = GUIIM_TYPE_ABC;
        }
        else
        {
            MMIAPISET_GetLanguageType(&lang_type);
            if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type)
            {
                input_mode = GUIIM_TYPE_SMART;
            }
            else
            {
                input_mode = GUIIM_TYPE_ABC;
            }
        }
    }

    GUIEDIT_SetIm(ctrl_id,allow_inputmethod,input_mode);
    if(GUIIM_TYPE_DIGITAL == allow_inputmethod)
    {
        GUIEDIT_SetMaxLen(ctrl_id, max_alpha_len);
    }
    else
    {
        GUIEDIT_SetTextMaxLen(ctrl_id,max_ucs2_len,max_alpha_len);
        
    }
    if(PNULL != text_ptr)
    {
        GUIEDIT_SetString(ctrl_id, text_ptr->wstr_ptr, text_ptr->wstr_len);
    }
    GUIEDIT_SetFont(ctrl_id,font_type);    
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 在手机中添加时打开相应的编辑窗口
//  Global resource dependence : none
//  Author: Starmer.Sun
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBox(MMI_CTRL_ID_T ctrl_id, MMIPB_MENU_DATA_INFOR_T *add_data_ptr)
{
    uint16                              index = 0;
    uint16                              max_alpha_len = 0;
    uint16                              max_ucs2_len = 0;
    BOOLEAN                             is_editbox = FALSE;
    uint32                              item_id = 0;
    MMI_STRING_T                        input_str = {0};
    uint8                               *str_ptr = PNULL;
    wchar                               *wstr_ptr = PNULL;    
    MN_NUMBER_TYPE_E                    number_type = 0;
    GUIIM_TYPE_T                               allow_input_mode = GUIIM_TYPE_SET_ALL;

    if(PNULL == add_data_ptr)
    {
        return ;
    }
    index = GUILIST_GetCurItemIndex(ctrl_id);   
    GUILIST_GetItemData(ctrl_id, index, &item_id);

    

    switch (item_id)
    {
    case MMIPB_LISTBOX_INDEX_PHOTO:
        add_data_ptr->menu_type = MMIPB_PHOTO_OPT_MENU;            
      //  add_data_ptr->operate_type = MMIPB_OPERATE_TYPE_MAX;
        add_data_ptr->need_reset_storage = FALSE;
        MMIPB_EnterPbTabOptMenu(add_data_ptr);            
        break;

    case MMIPB_LISTBOX_INDEX_NAME:
        // 打开编辑姓名的窗口
        input_str.wstr_ptr = add_data_ptr->contact_ptr->name.wstr;
        input_str.wstr_len =  add_data_ptr->contact_ptr->name.wstr_len;
        max_ucs2_len = MMIPB_GetMaxWstrLen(add_data_ptr->contact_ptr->storage_id, MMIPB_CONTACT_NAME);
        max_alpha_len = MMIPB_GetMaxAlphaLen(add_data_ptr->contact_ptr->storage_id, MMIPB_CONTACT_NAME);
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_NAME);
        is_editbox = TRUE;
        if(MMIPB_IsInSIMGroupByStorage(add_data_ptr->contact_ptr->storage_id))
	    {
            GUIEDIT_SetPbEncode(TRUE, MMIPB_EDITBOX_CTRL_ID);
	    }
        break;
          
    case MMIPB_LISTBOX_INDEX_NUMBER_MOBILE:
    case MMIPB_LISTBOX_INDEX_NUMBER_HOME:
#ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_NUMBER_OFFICE:
    //case MMIPB_LISTBOX_INDEX_NUMBER_FAX:
    //case MMIPB_LISTBOX_INDEX_NUMBER_OTHER:
#endif
        {
            //MMIPB_STORAGE_INFO_T pb_info = {0};
            MMIPB_BCD_NUMBER_T        number = {0};
            //PHONEBOOK_STORAGE_E storage = PHONEBOOK_SIM_STORAGE;
            uint32 index_of_phone = 0;
            number_type = 0;

            // 打开编辑号码的窗口
            index_of_phone = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
            str_ptr = (uint8 *)SCI_ALLOC_APP(MMIPB_NUMBER_MAX_STRING_LEN +2);
            wstr_ptr = (wchar *)SCI_ALLOC_APP((MMIPB_NUMBER_MAX_STRING_LEN +2)*sizeof(wchar));
            if(PNULL == str_ptr || PNULL == wstr_ptr)
            {
                //SCI_TRACE_LOW:"[MMIPB]OpenOperateWinInNvAdd PNULL == str_ptr !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_4442_112_2_18_2_43_43_252,(uint8*)"");
                break;
            }
            input_str.wstr_ptr = wstr_ptr;

            SCI_MEMCPY(&number, &add_data_ptr->contact_ptr->number[index_of_phone],sizeof(MMIPB_BCD_NUMBER_T));

            number_type = MMIPB_GetNumberTypeFromUint8(number.npi_ton);  
            input_str.wstr_len = MMIAPICOM_GenDispNumber(
                                            (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),
                                            number.number_len,
                                            number.number,
                                            str_ptr,
                                            MMIPB_MAX_STRING_LEN+2
                                            );

            input_str.wstr_len = MIN(input_str.wstr_len, MMIPB_NUMBER_MAX_STRING_LEN+2);
            MMI_STRNTOWSTR(input_str.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN+2, (const uint8*)str_ptr, MMIPB_NUMBER_MAX_STRING_LEN+2, input_str.wstr_len);

            //pb_info = MMIPB_GetContactStorageInfo(add_data_ptr->contact_ptr->storage_id);
            max_alpha_len = MMIPB_GetNumberMaxLen(add_data_ptr->contact_ptr->storage_id, 
                            item_id,
                            number.number_len
                            );
            max_ucs2_len = max_alpha_len ; 
            allow_input_mode = GUIIM_TYPE_DIGITAL;
            MMK_CreateWin((uint32*)MMIPB_NUMBER_EDITBOX_WIN_TAB, (ADD_DATA)s_listbox_title_default_str[item_id]);  
            is_editbox = TRUE;
        }
        break;
#ifdef MMIPB_MAIL_SUPPORT
    case MMIPB_LISTBOX_INDEX_MAIL:
        input_str.wstr_len = add_data_ptr->contact_ptr->mail.wstr_len;
        input_str.wstr_ptr = add_data_ptr->contact_ptr->mail.wstr;
        max_ucs2_len = max_alpha_len = MMIPB_MAX_MAIL_LEN;
        allow_input_mode = IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET;/*lint !e655*/
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_PB_ITEM_DETAIL_EMAIL);
        is_editbox = TRUE;
        break;
#endif

    case MMIPB_LISTBOX_INDEX_GROUP:
        MMIPB_CreateEditGroupWin(add_data_ptr->parent_win, add_data_ptr);
        break;
    case MMIPB_LISTBOX_INDEX_RING:
        add_data_ptr->menu_type = MMIPB_RING_OPT_MENU;            
       // add_data_ptr->operate_type = MMIPB_OPERATE_TYPE_MAX;
        add_data_ptr->need_reset_storage = FALSE;
        MMIPB_EnterPbTabOptMenu(add_data_ptr);
        break;

#ifdef MMIPB_IM_MESSAGE_SUPPORT
    case MMIPB_LISTBOX_INDEX_QQ:
        input_str.wstr_len = add_data_ptr->contact_ptr->imessager.wstr_len;
        input_str.wstr_ptr = add_data_ptr->contact_ptr->imessager.wstr;
        max_ucs2_len = max_alpha_len = MMIPB_IM_MAX_LEN;
        allow_input_mode = GUIIM_TYPE_DIGITAL;
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_COMM_QQ);
        is_editbox = TRUE;
        break;
#endif
 #ifndef PB_SUPPORT_LOW_MEMORY
    case MMIPB_LISTBOX_INDEX_URL:
        input_str.wstr_len = add_data_ptr->contact_ptr->url.wstr_len;
        input_str.wstr_ptr = add_data_ptr->contact_ptr->url.wstr;
        max_ucs2_len = max_alpha_len = MMIPB_URL_MAX_LEN;
        allow_input_mode = IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET;/*lint !e655*/
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_PB_URL);
        is_editbox = TRUE;
        break;
                
    case MMIPB_LISTBOX_INDEX_BIRTH:
        //add_data_ptr->menu_type = MMIPB_PHOTO_OPT_MENU;            
        //add_data_ptr->operate_type = MMIPB_OPERATE_TYPE_MAX;
        //add_data_ptr->need_reset_storage = FALSE;
        //MMIPB_EnterPbTabOptMenu(add_data_ptr);            
        break;
        
    case MMIPB_LISTBOX_INDEX_MEMO:
        input_str.wstr_ptr = add_data_ptr->contact_ptr->memo.wstr;
        input_str.wstr_len =  add_data_ptr->contact_ptr->memo.wstr_len;                   
        max_alpha_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
        max_ucs2_len = max_alpha_len/sizeof(wchar);
        is_editbox = TRUE;
#ifdef CMCC_UI_STYLE
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_PB_ITEM_ADRRESS);
#else
        MMK_CreateWin((uint32*)MMIPB_NAME_EDITBOX_WIN_TAB, (ADD_DATA)TXT_MEMO);
#endif
        break;
#endif
    default:
        //SCI_TRACE_LOW:"[MMIPB]OpenOperateWinInNvAdd ERROR index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_4548_112_2_18_2_43_43_253,(uint8*)"d",index);
        break;
    }
    
    if(is_editbox)
    {
        if(max_ucs2_len > 0)
        {
            SetInputBox(&input_str, max_ucs2_len, max_alpha_len, allow_input_mode);
        }
    }

    if(PNULL != str_ptr)
    {
        SCI_Free(str_ptr);
        str_ptr = PNULL;
    }
    if(PNULL!=wstr_ptr)
    {
        SCI_FREE(wstr_ptr);
        wstr_ptr=PNULL;
     }

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get edit win item default txt label id
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T MMIPB_GetItemDefaultTxtID(MMIPB_ITEM_TYPE_E item_type)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    switch(item_type)
    {
        case MMIPB_ITEM_NAME:
            text_id = TXT_NAME;
            break;
        case MMIPB_ITEM_NUMBER:
            text_id = TXT_PB_PHONE;
            break;
#ifdef MMIPB_MAIL_SUPPORT
        case MMIPB_ITEM_EMAIL:
            text_id = TXT_PB_ITEM_DETAIL_EMAIL;
            break;
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
        case MMIPB_ITEM_INSTANTMSG:
            text_id = TXT_PB_INSTANTMSG;
            break;
#endif
        case MMIPB_ITEM_ORG:
            text_id = TXT_PB_ORG;
            break;
        case MMIPB_ITEM_TITLE:
            text_id = TXT_PB_ITEM_DETAIL_ORG;
            break;
#ifndef PB_SUPPORT_LOW_MEMORY
        case MMIPB_ITEM_URL:
            text_id = TXT_PB_URL;
            break;
        case MMIPB_ITEM_BIRTHDAY:
            text_id = TXT_DATE;
            break;
#endif
#ifdef CMCC_VCARD_SUPPORT
        case MMIPB_ITEM_ALIAS:
            text_id = TXT_PB_ITEM_DETAIL_ALIAS;
            break;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
        case MMIPB_ITEM_MEMO:
            text_id = TXT_MEMO;
#endif
            break;
	default:
		break;
    }
    return text_id;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :create EDITBOX
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormEditor(
                        MMI_STRING_T    *text_ptr,
                        uint16          max_ucs2_len,
                        uint16          max_alpha_len,
                        uint8           allow_inputmethod,
                        BOOLEAN         is_compressed,
                        MMI_CTRL_ID_T   edit_id,
                        MMI_HANDLE_T    form_handle,
                        MMIPB_ITEM_TYPE_E item_type,
                        MMI_WIN_ID_T    win_id
                        )
{
    BOOLEAN					is_special_edit = FALSE;
    GUIIM_TYPE_T         input_mode = GUIIM_TYPE_ABC;
    GUI_FONT_T              font_type = MMIPB_SMALL_FONT;
    GUIEDIT_INIT_DATA_T		init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    wchar                               forbid_char_value[]= {0x0d, 0x0a};//禁止换行回车的输入    
    if(GUIIM_TYPE_DIGITAL == allow_inputmethod)
    {
        init_data.type = GUIEDIT_TYPE_PHONENUM;
        init_data.str_max_len = max_alpha_len;
    }
    else 
    {
        if(IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET  == allow_inputmethod)/*lint !e655*/
        {
            //special editor, this only allow abc, number 
            input_mode = GUIIM_TYPE_ABC;
            is_special_edit = TRUE;
        }
        init_data.type = GUIEDIT_TYPE_TEXT;
    }
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    if(MMIPB_ITEM_INSTANTMSG == item_type)
    {
        input_mode = GUIIM_TYPE_ABC;
    }
#endif
    init_data.is_rect_adaptive = TRUE;
	
    child_data.is_bg = FALSE;
    child_data.is_get_active = TRUE;
    child_data.guid = SPRD_GUI_EDITBOX_ID;
    child_data.init_data_ptr = &init_data;
    child_data.child_handle = edit_id;

    GUIFORM_CreatDynaChildCtrl(win_id, form_handle, &child_data);
    if(is_special_edit)
    {
        //edit box need reset its input mode:such as email, url 
        GUIEDIT_SetIm(MMK_GetCtrlHandleByWin(win_id,child_data.child_handle),
                allow_inputmethod,input_mode);
        GUIEDIT_SetImTag(MMK_GetCtrlHandleByWin(win_id,child_data.child_handle),
                GUIIM_TAG_EMAIL);
    }
    if(GUIEDIT_TYPE_TEXT == init_data.type)
    {
        GUIEDIT_SetTextMaxLen(MMK_GetCtrlHandleByWin(win_id,child_data.child_handle),
                max_ucs2_len,max_alpha_len);
    }
    if(is_compressed)
    {
        GUIEDIT_SetPbEncode(TRUE, MMK_GetCtrlHandleByWin(win_id,edit_id));
    }
    
    if(PNULL != text_ptr&&(text_ptr->wstr_len>0))
    {
        GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_id,edit_id), 
            text_ptr->wstr_ptr, text_ptr->wstr_len);
    }
    else if(MMIPB_ITEM_MAX!=item_type)
    {
        MMI_TEXT_ID_T default_string_id = MMIPB_GetItemDefaultTxtID(item_type);
            
        GUIEDIT_SetDefaultStringById(
            MMK_GetCtrlHandleByWin(win_id,edit_id),
            default_string_id);
    }

    GUIEDIT_SetFont(MMK_GetCtrlHandleByWin(win_id,edit_id),font_type);
    GUIEDIT_SetStyle(MMK_GetCtrlHandleByWin(win_id,edit_id), GUIEDIT_STYLE_SINGLE);
    GUIEDIT_ConfigDisableChar(MMK_GetCtrlHandleByWin(win_id,edit_id), forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
    #ifdef MMI_PDA_SUPPORT
       GUIEDIT_CreateImEditActive(MMK_GetCtrlHandleByWin(win_id,child_data.child_handle),TRUE);
    #endif
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : delete icon of listbox
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormButton(MMI_HANDLE_T win_handle,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T button_id,
						   BOOLEAN       is_get_active
						 )
{
    GUIBUTTON_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};
  
    {
         child_data.is_bg = FALSE;
        
        child_data.is_get_active = is_get_active;
        child_data.guid = SPRD_GUI_BUTTON_ID;
        child_data.init_data_ptr = &init_data;
        child_data.child_handle = button_id;
    //edit_ptr = MMK_CreateControl( &create );
        GUIFORM_CreatDynaChildCtrl(win_handle, form_handle, &child_data);
    }
    
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append animal
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormAnim(MMI_HANDLE_T win_handle,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T anim_id,
						   BOOLEAN       is_get_active
						 )
{
    GUIANIM_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};

    init_data.is_form_bg = FALSE;
    child_data.is_bg = FALSE;
	child_data.is_get_active = is_get_active;
	child_data.guid = SPRD_GUI_ANIM_ID;
	child_data.init_data_ptr = &init_data;
	child_data.child_handle = anim_id;
    //edit_ptr = MMK_CreateControl( &create );
    GUIFORM_CreatDynaChildCtrl(win_handle, form_handle, &child_data);
 //   GUIANIM_SetBgImg(MMI_HANDLE_T ctrl_handle, MMI_IMAGE_ID_T bg_img_id);

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append label
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormListBox(MMI_HANDLE_T win_id,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T ctrl_id,
						   BOOLEAN       is_get_active
						 )
{
    GUILIST_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    //GUIFORM_CHILD_WIDTH_T   width_infor = {0};

    init_data.type = GUILIST_TEXTLIST_E;
    init_data.max_item_num = 1;
    
	child_data.is_bg = FALSE;
	child_data.is_get_active = is_get_active;
	child_data.guid = SPRD_GUI_LIST_ID;
	child_data.init_data_ptr = &init_data;
	child_data.child_handle = ctrl_id;
    
    GUIFORM_CreatDynaChildCtrl(win_id, form_handle, &child_data);
    
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append label
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendFormLabel(MMI_HANDLE_T win_id,
                           MMI_HANDLE_T form_handle,
						   MMI_CTRL_ID_T label_id,
						   uint16   percent,
						   MMI_STRING_T *text_ptr,
						   BOOLEAN       is_get_active
						 )
{
    GUILABEL_INIT_DATA_T    init_data = {0};
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    GUIFORM_CHILD_WIDTH_T   width_infor = {0};
	GUI_BG_T bg_infor = {0};
    
	child_data.is_bg = FALSE;
	child_data.is_get_active = is_get_active;
	child_data.guid = SPRD_GUI_LABEL_ID;
	child_data.init_data_ptr = &init_data;
	child_data.child_handle = label_id;
    
    GUIFORM_CreatDynaChildCtrl(win_id, form_handle, &child_data);
    GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id,label_id), 
            text_ptr, FALSE);
    if((MMIPB_LABEL_RING_EDIT_ID != label_id)&&(MMIPB_LABEL_GROUP_EDIT_ID!= label_id)
        &&(MMIPB_BIRTHDAY_EDIT_ID!= label_id))
    {
        width_infor.type = GUIFORM_CHILD_WIDTH_PERCENT;
        width_infor.add_data = percent;
        
        GUIFORM_SetChildWidth(form_handle, MMK_GetCtrlHandleByWin(win_id,label_id), 
            &width_infor);
        
        bg_infor.bg_type = GUI_BG_COLOR;
    bg_infor.color =GUI_RGB2RGB565(178, 178, 178);// MMI_DARK_GRAY_COLOR;
    GUILABEL_SetBg(
        MMK_GetCtrlHandleByWin(win_id,label_id), 
        &bg_infor);
    }
    GUILABEL_SetFont(MMK_GetCtrlHandleByWin(win_id,label_id), 
            (MMIPB_SMALL_FONT), MMI_BLACK_COLOR);
    GUILABEL_SetMargin(MMK_GetCtrlHandleByWin(win_id,label_id),MMIPB_LABEL_MARGIN);
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendDesignateTypeFormChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id,
                           GUIFORM_LAYOUT_E layout_type
                           )
{
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};

    //init data
    
    init_data.layout_type = layout_type;

    child_data.is_get_active = TRUE;
    child_data.guid          = SPRD_GUI_FORM_ID;
    child_data.child_handle  = ctrl_id;
    child_data.init_data_ptr = &init_data;

    //create
    GUIFORM_CreatDynaChildCtrl(win_handle,
        form_handle,
        &child_data);

    GUIFORM_SetStyle(MMK_GetCtrlHandleByWin(win_handle, ctrl_id),GUIFORM_STYLE_NORMAL);
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendFormChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id)
{
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};

    //init data
    #if (defined MAINLCD_SIZE_320X480||defined MAINLCD_SIZE_240X400)
        init_data.layout_type = GUIFORM_LAYOUT_SBS;
    #else
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;
    #endif
    child_data.is_get_active = TRUE;
    child_data.guid          = SPRD_GUI_FORM_ID;
    child_data.child_handle  = ctrl_id;
    child_data.init_data_ptr = &init_data;

    //create
    GUIFORM_CreatDynaChildCtrl(win_handle,
        form_handle,
        &child_data);

    GUIFORM_SetStyle(MMK_GetCtrlHandleByWin(win_handle, ctrl_id),GUIFORM_STYLE_NORMAL);
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append form
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendFormSubChild(
                           MMI_HANDLE_T     win_handle,
                           MMI_HANDLE_T     form_handle,
                           MMI_CTRL_ID_T    ctrl_id,
                           uint16 percent)
{
    GUIFORM_DYNA_CHILD_T    child_data = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};
    GUIFORM_CHILD_WIDTH_T width_infor= {0};
    //init data
    #if (defined MAINLCD_SIZE_320X480||defined MAINLCD_SIZE_240X400)
        init_data.layout_type = GUIFORM_LAYOUT_SBS;
    #else
        init_data.layout_type = GUIFORM_LAYOUT_ORDER;
    #endif
    child_data.is_get_active = TRUE;
    child_data.guid          = SPRD_GUI_FORM_ID;
    child_data.child_handle  = ctrl_id;
    child_data.init_data_ptr = &init_data;

    //create
    GUIFORM_CreatDynaChildCtrl(win_handle,
        form_handle,
        &child_data);

    GUIFORM_SetStyle(MMK_GetCtrlHandleByWin(win_handle, ctrl_id),GUIFORM_STYLE_NORMAL);

    width_infor.type = GUIFORM_CHILD_WIDTH_PERCENT;
    width_infor.add_data = percent;
    GUIFORM_SetChildWidth(form_handle, MMK_GetCtrlHandleByWin(win_handle, ctrl_id),
        &width_infor);
    
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得contact number count
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetContactNumberNum(MMIPB_CONTACT_T *contact_ptr,uint16 *index_array)
{
    uint16 number_num = 0;
    uint16 i = 0;
    if(PNULL == contact_ptr)
    {
        return number_num;
    }
    
    for(i = 0;i<MMIPB_MAX_NV_PHONE_NUM;i++)
    {
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton,
            contact_ptr->number[i].number_len))//contact_ptr->number[i].number_len>0)
        {
            index_array[number_num] = i;
            number_num++;
        }
    }
    return number_num;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得需新增的editbox对应的号码index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetNextAddNumberIndex(uint16 index,MMIPB_CONTACT_T *contact_ptr)
{
    uint8 number_index = 0;
    uint8 i = 0;
    uint8 max_number = 0;
    
    if(PNULL == contact_ptr)
    {
        return number_index;
    }
    

    MMIPB_GetMaxNumbernum(contact_ptr->storage_id, &max_number);
    if((max_number - 1) == index) 
    {
        index = 0;
    }
    for(i = index;i< max_number;i++)
    {
        if(!MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton, contact_ptr->number[i].number_len))
        {
            number_index = i;
            break;
        }
    }
    return number_index;
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MSISDN and only one sim number should not delete
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsNeedNumberDelButton(uint16 storage_id)
{
    BOOLEAN result = TRUE;

    #ifdef PB_SUPPORT_LOW_MEMORY
    result = FALSE;
    #else
    #if defined (MMIPB_SIMDN_SUPPORT) 
    if(MMIPB_IsMsisdnContact(storage_id))
    {
        result = FALSE;
    }
    else 
   #endif
    if(MMIPB_IsSIMContact(storage_id))
    {
        result = FALSE;
    }
    #endif
    
    return result;
}
#endif
#ifdef MMI_PDA_SUPPORT
#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_320X480)
/*****************************************************************************/
//  Description : create one number item(include button and editbox)
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateContactNumberItem(MMI_WIN_ID_T win_id,
    MMIPB_CONTACT_T *contact_ptr,
    uint16 index)
{
    MMI_STRING_T   label_str_t = {0};
    MMI_STRING_T   edit_str_t = {0};	
    MMIPB_STRING_T  string_t = {0};
    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
    
    wchar wstr[MMIPB_MAX_STRING_LEN+2] = {0};
    
    //GUIFORM_CHILD_WIDTH_T  width_infor = {0};
    GUI_BG_T bg_infor = {0};
    //GUI_BORDER_T border_infor = {0};
    //IGUICTRL_T  *form_ptr = PNULL;
    
    //uint16 max_ucs2_len = 0;
    //uint16 max_alpha_len = 0;
    uint16 max_num_len = 0;
    uint16 hor_space = 0; 
        
    if(index > (MMIPB_MAX_NV_PHONE_NUM-1) || PNULL == contact_ptr)
    {
        return;
    }

    //max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_NUMBER);
    //max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_NUMBER);
    
    AppendDesignateTypeFormChild(win_id, 
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_NUMBER_FORM_ID),
                        s_num_form_id[index], 
                        GUIFORM_LAYOUT_SBS);    
    
    AppendFormSubChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,s_num_form_id[index]),
                        s_num_sub_form_id[index],
                        MMIPB_EDIT_ITEM_WIDTH);    
    SetSubFormBorder(win_id, s_num_sub_form_id[index]);
    
    if(0 == contact_ptr->num_type[index] )
    {
        contact_ptr->num_type[index] =index + 1;
    }
    MMIPB_GetNumberTypeString(contact_ptr->storage_id, 
                                    		contact_ptr->num_type[index],
                                    		&label_str_t);
    
    AppendFormLabel(win_id, 
                MMK_GetCtrlHandleByWin(win_id,s_num_sub_form_id[index]),
                s_num_type_label_id[index], 
                MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);    
 
   //append number editor
    number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[index].npi_ton);
    string_t.strlen = MMIAPICOM_GenDispNumber( 
			(number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),
			contact_ptr->number[index].number_len, 
			contact_ptr->number[index].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN+2
			);
    
    edit_str_t.wstr_ptr = wstr;
    edit_str_t.wstr_len = MIN(MMIPB_MAX_STRING_LEN+2, string_t.strlen);		
    MMI_STRNTOWSTR(wstr, MMIPB_MAX_STRING_LEN+2, string_t.strbuf, MMIPB_MAX_STRING_LEN+2, string_t.strlen);
    max_num_len = MMIPB_GetNumberMaxLen(contact_ptr->storage_id, contact_ptr->num_type[index], contact_ptr->number[index].number_len);
    AppendFormEditor(&edit_str_t, 
                        max_num_len, 
                        max_num_len, 
                        GUIIM_TYPE_DIGITAL,
                        MMIPB_IsSIMContact(contact_ptr->storage_id),
                        s_num_edit_id[index],
                        MMK_GetCtrlHandleByWin(win_id,s_num_sub_form_id[index]),
                        MMIPB_ITEM_NUMBER,
                        win_id);
        
    if(IsNeedNumberDelButton(contact_ptr->storage_id))         
    {
        AppendFormButton(win_id, 
                            MMK_GetCtrlHandleByWin(win_id,s_num_form_id[index]), 
                            s_num_delete_id[index],
                            TRUE);        
        
        GUIBUTTON_PermitBorder(MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[index]),
                            FALSE);    
    
        SCI_MEMSET(&bg_infor,0,sizeof(GUI_BG_T));
        bg_infor.bg_type = GUI_BG_IMG;
    //#ifdef GUIF_LIST_PDASTYLE
        bg_infor.img_id = IMAGE_PB_DELETE;
    //#endif
        GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[index]), &bg_infor);
        if(0 == edit_str_t.wstr_len)
        {
            GUIFORM_SetChildDisplay(
                                MMK_GetCtrlHandleByWin(win_id,s_num_form_id[index]),
                                MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[index]),
                                GUIFORM_CHILD_DISP_HIDE);
        }
    }

    GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,s_num_form_id[index]),&hor_space,PNULL);
 #ifdef TOUCH_PANEL_SUPPORT
    GUIFORM_ForceLayout(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID));
#endif
 }
#else
/*****************************************************************************/
//  Description : create one number item(include button and editbox)
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateContactNumberItem(MMI_WIN_ID_T win_id,
    MMIPB_CONTACT_T *contact_ptr,
    uint16 index)
{
    MMI_STRING_T   label_str_t = {0};
    MMI_STRING_T   edit_str_t = {0};	
    MMIPB_STRING_T  string_t = {0};
    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
    
    wchar wstr[MMIPB_MAX_STRING_LEN+1] = {0};
    
    GUIFORM_CHILD_WIDTH_T  width_infor = {0};
    GUI_BG_T bg_infor = {0};
    GUI_BORDER_T border_infor = {0};
    IGUICTRL_T  *form_ptr = PNULL;

    uint16 hor_space = 0;
    //uint16 max_ucs2_len = 0;
    //uint16 max_alpha_len = 0;
    uint16 max_num_len = 0;
    
    if(index>(MMIPB_MAX_NV_PHONE_NUM-1)||PNULL==contact_ptr)
    {
        return;
    }
    //max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_NUMBER);
    //max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_NUMBER);
   
    AppendDesignateTypeFormChild(win_id, 
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_NUMBER_FORM_ID),
                s_num_form_id[index], 
                GUIFORM_LAYOUT_SBS);    
    AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,s_num_form_id[index]),
                s_num_sub_form_id[index],
                MMIPB_EDIT_ITEM_WIDTH);    
    SetSubFormBorder(win_id, s_num_sub_form_id[index]);

    AppendDesignateTypeFormChild(win_id, 
                            MMK_GetCtrlHandleByWin(win_id,
                            s_num_sub_form_id[index]),
                            s_num_label_form_id[index], 
                            GUIFORM_LAYOUT_SBS);

    GUIFORM_PermitChildBorder(MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
                            FALSE);
    GUIFORM_PermitChildBg( MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
                            FALSE);
    
     //append number type 
    if(0 == contact_ptr->num_type[index] )
    {
        contact_ptr->num_type[index] =index + 1;
    }
    MMIPB_GetNumberTypeString(contact_ptr->storage_id, contact_ptr->num_type[index],&label_str_t);
   if(IsNeedNumberDelButton(contact_ptr->storage_id)) 
   { 
        AppendFormLabel(win_id, 
            MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
            s_num_type_label_id[index],MMIPB_NUM_DEL_LABEL_WIDTH, 
            &label_str_t,
            TRUE);        
        AppendFormButton(win_id, 
            MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
            s_num_delete_id[index],
            TRUE);    
        GUIFORM_SetSpace(
            MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
            &hor_space,PNULL);
    
        if(MMIPB_IsValidPhoneNumber(
            contact_ptr->number[index].npi_ton,
            contact_ptr->number[index].number_len))
        {
            bg_infor.bg_type = GUI_BG_IMG;
            bg_infor.img_id = IMAGE_PB_DELETE;
        }
        else
        {
            bg_infor.bg_type = GUI_BG_COLOR;
            bg_infor.color = MMI_DARK_GRAY_COLOR;
        }
        GUIBUTTON_SetBg(
            MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[index]),
            &bg_infor);
        
    }
    else
    {  
        AppendFormLabel(win_id, 
            MMK_GetCtrlHandleByWin(win_id,s_num_label_form_id[index]),
            s_num_type_label_id[index],MMIPB_EDIT_LABEL_WIDTH, 
            &label_str_t,
            TRUE);
    }
   //append number editor
    number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[index].npi_ton);
    string_t.strlen = MMIAPICOM_GenDispNumber(
			(number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),
			contact_ptr->number[index].number_len, 
			contact_ptr->number[index].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN
			);
    edit_str_t.wstr_ptr = wstr;
    edit_str_t.wstr_len = MIN(MMIPB_MAX_STRING_LEN, string_t.strlen);		
    MMI_STRNTOWSTR(wstr, MMIPB_MAX_STRING_LEN, string_t.strbuf, MMIPB_MAX_STRING_LEN, string_t.strlen);
    max_num_len = MMIPB_GetNumberMaxLen(contact_ptr->storage_id, contact_ptr->num_type[index], contact_ptr->number[index].number_len);
    AppendFormEditor(&edit_str_t, 
        max_num_len, 
        max_num_len, 
        GUIIM_TYPE_DIGITAL,
        MMIPB_IsSIMContact(contact_ptr->storage_id),
        s_num_edit_id[index],
        MMK_GetCtrlHandleByWin(win_id,s_num_sub_form_id[index]),
        MMIPB_ITEM_NUMBER,
        win_id);
#ifdef TOUCH_PANEL_SUPPORT    
    GUIFORM_ForceLayout(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID));
#endif
 }
#endif
#endif//pda support

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set sub form border
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSubFormBorder(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    IGUICTRL_T   *form_ptr = PNULL;
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id,ctrl_id);
    GUI_BORDER_T border_infor = {0};
    
    GUIFORM_PermitChildBorder(MMK_GetParentCtrlHandle(form_handle), FALSE);
    GUIFORM_PermitChildBorder(form_handle, FALSE);
    GUIFORM_PermitChildBg(form_handle,FALSE);

    form_ptr = MMK_GetCtrlPtr(form_handle);
    if(PNULL!=form_ptr)
    {
        border_infor.type = GUI_BORDER_SOLID;
        border_infor.color = MMI_DARK_GRAY_COLOR;
        border_infor.width = 2;
        IGUICTRL_SetBorder(form_ptr, &border_infor);
    }
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set photo anim ctrl height
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetPhotoFormHeight(
    MMI_HANDLE_T win_id,
    MMI_HANDLE_T form_handle,
    MMI_HANDLE_T child_handle,
    MMI_IMAGE_ID_T img_id
    )
{
    GUIFORM_CHILD_HEIGHT_T height_infor = {0};
    GUIFORM_CHILD_WIDTH_T width_infor = {0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 hor_space = 0;
    uint16 ver_space = 0;
    
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height,img_id,win_id);
    if(0==img_width||0==img_height)
    {
        img_width = MMIPB_DEFAULT_PHOTO_WIDTH;
        img_height = MMIPB_DEFAULT_PHOTO_HEIGHT;
    }

    width_infor.type = GUIFORM_CHILD_WIDTH_FIXED;
    width_infor.add_data = img_width;//+MMIPB_PHOTO_ANIM_OFFSET;

    GUIFORM_SetChildWidth(form_handle,child_handle,&width_infor);

     height_infor.type = GUIFORM_CHILD_HEIGHT_FIXED;
    height_infor.add_data = img_height;//+MMIPB_PHOTO_ANIM_OFFSET;

    GUIFORM_SetChildHeight(form_handle, child_handle, &height_infor);

    GUIFORM_SetSpace(
        form_handle,
        &hor_space,&ver_space);

}
#endif

#if defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : create photo anim 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePhotoAnim(MMI_HANDLE_T win_id,
    MMI_HANDLE_T form_handle,
    MMIPB_CONTACT_T *contact_ptr,
    MMIPB_CUSTOM_PHOTO_T *photo_ptr)
{
    GUIANIM_DATA_INFO_T data_infor={0};
    GUIANIM_FILE_INFO_T file_infor={0};

    if(PNULL == contact_ptr||PNULL == photo_ptr)
    {
            return;
    }
    
    AppendFormAnim(win_id,
            form_handle,
            MMIPB_ANIM_PHOTO_ID,
            TRUE);

    if((contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_EXIST))
    {
        if (photo_ptr->file_name_len>0 && MMIFILE_IsFileExist(photo_ptr->file_name, photo_ptr->file_name_len))
        {
            file_infor.full_path_wstr_ptr = photo_ptr->file_name;
            file_infor.full_path_wstr_len = photo_ptr->file_name_len;
            SetPhotoAnimParam(win_id, PNULL,&file_infor);
        }
        else
        {
            data_infor.img_id = IAMGE_PB_DEFAULT_PHOTO;
            SetPhotoAnimParam(win_id,&data_infor,PNULL);
        }
    }
    else 
    {
        data_infor.img_id = IAMGE_PB_DEFAULT_PHOTO;
        SetPhotoAnimParam(win_id,&data_infor,PNULL);
    }

    SetPhotoFormHeight(win_id,
                    form_handle,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_ANIM_PHOTO_ID), 
                    IAMGE_PB_DEFAULT_PHOTO);
}
#endif

#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_MAIL_SUPPORT)
/*****************************************************************************/
//  Description : create edit email item 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateEmailEditItem(MMI_HANDLE_T win_id,
    MMIPB_CONTACT_T *contact_ptr
    )
{
    MMI_STRING_T label_str_t = {0};
    MMI_STRING_T edit_str_t = {0};
    uint16 hor_space = 0;
    uint8 max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_EMAIL); 
    uint8 max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_EMAIL);

    AppendFormChild(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                    MMIPB_EMAIL_FORM_ID);
    AppendFormSubChild(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EMAIL_FORM_ID),
                    MMIPB_EMAIL_SUB_FORM_ID,
                    MMIPB_EDIT_ITEM_WIDTH);
    SetSubFormBorder(win_id,MMIPB_EMAIL_SUB_FORM_ID);

    //append email label text
    MMI_GetLabelTextByLang(TXT_EMAIL_BOX, &label_str_t);
    AppendFormLabel(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EMAIL_SUB_FORM_ID),
                    MMIPB_LABEL_EMAIL_ID,
                    MMIPB_EDIT_LABEL_WIDTH,
                    &label_str_t,
                    TRUE);	
    
    //append email editor
    edit_str_t.wstr_len = contact_ptr->mail.wstr_len;
    edit_str_t.wstr_ptr = contact_ptr->mail.wstr;
    AppendFormEditor(&edit_str_t, 
                    max_ucs2_len, 
                    max_alpha_len, 
                    IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET,/*lint !e655*/
                    FALSE,
                    MMIPB_EDITBOX_EMAIL_ID,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EMAIL_SUB_FORM_ID),
                    MMIPB_ITEM_EMAIL,
                    win_id);
    GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_EMAIL_FORM_ID),
                    &hor_space,PNULL);
    GUIEDIT_SetImHw( MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_EMAIL_ID),FALSE);

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create edit ring item 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateRingEditItem(MMI_HANDLE_T win_id,
    MMIPB_CONTACT_T *contact_ptr,
    MMIPB_CUSTOM_RING_T *ring_ptr
)
{
    MMI_STRING_T label_str_t = {0};
    MMI_STRING_T edit_str_t = {0};
    wchar wstr[MMIPB_MAX_STRING_LEN + 1] = {0};
    uint16 hor_space = 0;

    if(PNULL==contact_ptr||PNULL==ring_ptr)
    {
        return;
    }
    
    AppendFormChild(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                    MMIPB_RING_FORM_ID);
    AppendFormSubChild(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_RING_FORM_ID), 
                    MMIPB_RING_SUB_FORM_ID, 
                    MMIPB_EDIT_ITEM_WIDTH);
    SetSubFormBorder(win_id,MMIPB_RING_SUB_FORM_ID);

    //append ring id label text
    MMI_GetLabelTextByLang(TXT_RING, &label_str_t);
    AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_RING_SUB_FORM_ID),
                MMIPB_LABEL_RING_ID,MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);

    //append ring id edit label
    if(contact_ptr->ring_id_flag&MMIPB_FIXED_RING_FLAG)
    {
        //Fixed ring id exist
        if((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
        {
            edit_str_t.wstr_ptr = wstr; 
            edit_str_t.wstr_len = MMIPB_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
            edit_str_t.wstr_ptr, MMIPB_MAX_STRING_LEN);
        }
    }
    else if(ring_ptr->is_valid)
    {
        MMIPB_ReadRingFile(contact_ptr->contact_id,contact_ptr->storage_id,ring_ptr);
        edit_str_t.wstr_len = ring_ptr->file_name_len;
        edit_str_t.wstr_ptr = ring_ptr->file_name;
    }
    else
    {
        edit_str_t.wstr_ptr = wstr;
        edit_str_t.wstr_len = MMIPB_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
        edit_str_t.wstr_ptr, MMIPB_MAX_STRING_LEN);
    }
    
    AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_RING_SUB_FORM_ID),
                MMIPB_LABEL_RING_EDIT_ID,
                100-MMIPB_EDIT_LABEL_WIDTH,
                &edit_str_t,
                TRUE);

    GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_RING_FORM_ID),
                &hor_space,PNULL);
#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_320X480)    
    GUILABEL_SetMarginEx(MMK_GetCtrlHandleByWin(win_id,MMIPB_LABEL_RING_EDIT_ID),
                    12,0);
#endif
}
#endif

#if defined(MMI_PDA_SUPPORT) && (!defined(PB_SUPPORT_LOW_MEMORY))
/*****************************************************************************/
//  Description : create birthday item editbox
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreateBirthdayItem(MMI_HANDLE_T win_id,MMIPB_CONTACT_T *contact_ptr)
{
    //GUIEDIT_SEPARATOR_E  date_sep = GUIEDIT_SEPARATOR_SOLIDUS;
    //GUIEDIT_DATE_STYLE_E date_style = GUIEDIT_DATE_STYLE_YMD;            
    GUI_BG_T bg_infor = {0};
    MMI_STRING_T label_str_t = {0};
    MMI_STRING_T edit_str_t = {0};
    uint16 hor_space = 0;

    AppendDesignateTypeFormChild(win_id,
            MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
            MMIPB_BIRTHDAY_FORM_ID,
            GUIFORM_LAYOUT_SBS);
    AppendFormSubChild(win_id,
            MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID), 
            MMIPB_BIRTHDAY_SUB_FORM_ID, 
            MMIPB_EDIT_ITEM_WIDTH);
    SetSubFormBorder(win_id,MMIPB_BIRTHDAY_SUB_FORM_ID);
    
    //append birth label 
    MMI_GetLabelTextByLang(TXT_PB_BIRTHDAY, &label_str_t);
    AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_SUB_FORM_ID),
                MMIPB_BIRTHDAY_LABEL_ID,
                MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);
        
    //append birth editor
    if(contact_ptr->birth.wstr_len>0)
    {
        edit_str_t.wstr_len = contact_ptr->birth.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->birth.wstr;
    }

    AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_SUB_FORM_ID),
                MMIPB_BIRTHDAY_EDIT_ID,
                MMIPB_EDIT_LABEL_WIDTH,
                &edit_str_t,
                TRUE);

    AppendFormButton(win_id, 
                MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID), 
                MMIPB_BUTTON_BIRTHDAY_DELETE_ID,
                TRUE);

    GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                &hor_space,PNULL);

    GUIBUTTON_PermitBorder(MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                FALSE);  

    SCI_MEMSET(&bg_infor,0,sizeof(GUI_BG_T));
    bg_infor.bg_type = GUI_BG_IMG;
    bg_infor.img_id = IMAGE_PB_DELETE;
    
    GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                &bg_infor);

    if(0==edit_str_t.wstr_len)
    {
        GUIFORM_SetChildDisplay(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID),
                    GUIFORM_CHILD_DISP_HIDE);
    }
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create the pb edit win more swtich item
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePBEditMoreSwitchItem(MMI_HANDLE_T win_id,
    MMI_HANDLE_T form_handle,
    MMI_CTRL_ID_T child_form_id,
    MMI_CTRL_ID_T ctrl_id
)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //uint16               content_index = 0;
    uint16 hor_space = 0;
    uint16 ver_space = 0;
    
    item_t.item_style = GUIITEM_STYLE_HALF_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;  
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_COMMON_MORE;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_PB_MORE_CLOSE;
    item_t.user_data = 0;
   
    AppendFormSubChild(win_id,form_handle,child_form_id,MMIPB_EDIT_ITEM_WIDTH);
    SetSubFormBorder(win_id,child_form_id);   
    
    
    AppendFormListBox(win_id, 
                MMK_GetCtrlHandleByWin(win_id,child_form_id), 
                ctrl_id,
                TRUE);
    GUILIST_SetMaxItem(MMK_GetCtrlHandleByWin(win_id,ctrl_id), 1, FALSE);    
    GUILIST_AppendItem(MMK_GetCtrlHandleByWin(win_id,ctrl_id), &item_t);
    GUIFORM_SetSpace(form_handle,hor_space, ver_space);/*lint !e64*/
} 
#endif
#ifdef MMI_PDA_SUPPORT
#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_320X480)
/*****************************************************************************/
//  Description : 设置添加到手机时的LISTBOX各控件的值
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void InitContactEditorCtrl(MMI_WIN_ID_T win_id,
    MMIPB_MENU_DATA_INFOR_T *data_ptr
)
{
    uint8          i = 0;
    uint8   next_add_index = 0;
    uint8          free_count = 0;
    uint8          number_count = 0;
    uint8 create_count = 0;
    uint8 len = 0;
    wchar multi_group_name[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    
    uint16 num_index[MMIPB_MAX_NV_PHONE_NUM] = {0};
    MMI_STRING_T   label_str_t = {0};
    MMI_STRING_T   edit_str_t = {0};	
    //MMIPB_NAME_T   name_t = {0};
    //MMIPB_STRING_T  string_t = {0};
    //wchar wstr[MMIPB_MAX_STRING_LEN + 1] = {0};

    uint16 max_ucs2_len = 0;
    uint16 max_alpha_len = 0;
    MMIPB_CUSTOM_PHOTO_T *photo_ptr = PNULL;
    MMIPB_CUSTOM_RING_T *ring_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    GUIFORM_CHILD_WIDTH_T  width_infor = {0};
    //GUIFORM_CHILD_HEIGHT_T height_infor = {0};
    //GUI_BG_T bg_infor = {0};
    //GUI_FONT_ALL_T font_infor = {0};
    
    //GUIANIM_FILE_INFO_T file_infor = {0};
   // GUIANIM_DATA_INFO_T data_infor = {0};
    
    //MN_NUMBER_TYPE_E number_type = 0;
    uint16 new_id = 0;
    MMIPB_FIELD_USAGE_INOF_T field_infor = {0};
    uint16 hor_space = 0;        
    BOOLEAN  expand_flag = FALSE;
         
    if( PNULL == data_ptr->contact_ptr )
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_5672_112_2_18_2_43_46_254,(uint8*)"");
        return;
    }
    contact_ptr = data_ptr->contact_ptr;
    photo_ptr = data_ptr->photo_ptr;
    ring_ptr = data_ptr->ring_ptr;

    //get new contact id
    new_id = MMIPB_GetFieldUageInfo(contact_ptr->storage_id,contact_ptr->contact_id,
        &field_infor);
    if(new_id>0)
    {
        contact_ptr->contact_id = new_id;
    }
    
    //---Photo && Name--------
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_5690_112_2_18_2_43_46_255,(uint8*)"d",contact_ptr->storage_id);        
    max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_NAME);
    max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_NAME);    
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_5698_112_2_18_2_43_46_256,(uint8*)"d",max_ucs2_len);

    if(max_ucs2_len > 0)
    {
        AppendDesignateTypeFormChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                        MMIPB_NAME_FORM_ID,
                        GUIFORM_LAYOUT_SBS);

        AppendDesignateTypeFormChild(
                        win_id, MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_FORM_ID), 
                        MMIPB_NAME_SUB_FORM_ID,
                        GUIFORM_LAYOUT_SBS);

        width_infor.type = GUIFORM_CHILD_WIDTH_PERCENT;
        width_infor.add_data = MMIPB_EDIT_ITEM_WIDTH;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_SUB_FORM_ID), &width_infor);

        //photo edit
        if(field_infor.photo_free_flag)
        {
            CreatePhotoAnim(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_SUB_FORM_ID),
                    contact_ptr,photo_ptr);
        }

        AppendDesignateTypeFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_SUB_FORM_ID),
                MMIPB_NAME_EDIT_SUB_FORM_ID,
                GUIFORM_LAYOUT_ORDER);

        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_EDIT_SUB_FORM_ID), 
                MMIPB_NAME_EDIT_SUB_UP_FORM_ID);
        AppendFormChild(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_EDIT_SUB_FORM_ID), 
                    MMIPB_NAME_EDIT_SUB_DOWN_FORM_ID);
        SetSubFormBorder(win_id, MMIPB_NAME_EDIT_SUB_UP_FORM_ID);

        if(!MMIPB_IsPhoneContact(contact_ptr->storage_id))
        {
            //sim contact should add label
            MMI_GetLabelTextByLang(TXT_NAME, &label_str_t);
            AppendFormLabel(win_id, 
                                MMK_GetCtrlHandleByWin(win_id,
                                MMIPB_NAME_EDIT_SUB_UP_FORM_ID), 
                                MMIPB_LABEL_NAME_ID, 
                                MMIPB_EDIT_LABEL_WIDTH,
                                &label_str_t,
                                TRUE);
        }

        //name edit
        edit_str_t.wstr_len = contact_ptr->name.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->name.wstr;
        AppendFormEditor(&edit_str_t,
                                max_ucs2_len,
                                max_alpha_len, 
                                GUIIM_TYPE_SET_ALL, 
                                MMIPB_IsSIMContact(contact_ptr->storage_id),
                                MMIPB_EDITBOX_NAME_ID,
                                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_EDIT_SUB_UP_FORM_ID),
                                MMIPB_ITEM_NAME,
                                win_id);
    }
    
     //init number edit ctrl
    AppendDesignateTypeFormChild(win_id, 
                                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                                    MMIPB_EDITBOX_NUMBER_FORM_ID,
                                    GUIFORM_LAYOUT_ORDER);

    MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&free_count);  
    number_count = GetContactNumberNum(contact_ptr,num_index);
    create_count = MIN(number_count,free_count);

    for(i = 0;i< create_count;i++)
    {
        CreateContactNumberItem(win_id,contact_ptr,num_index[i]);
    }
    if(0 == number_count || free_count > create_count)
    {
        next_add_index = GetNextAddNumberIndex(0,contact_ptr);
        CreateContactNumberItem(win_id,contact_ptr,next_add_index);
    }
#ifdef MMIPB_MAIL_SUPPORT
     // Email
    if(field_infor.mail_free_flag)
    {
        CreateEmailEditItem(win_id, contact_ptr);
    }
#endif

#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
    //imessager
    if(field_infor.im_free_flag)        
    {
        AppendFormChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                        MMIPB_IM_FORM_ID);
        AppendFormSubChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_FORM_ID),
                        MMIPB_IM_SUB_FORM_ID,MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_IM_SUB_FORM_ID);

        MMIPB_GetImTypeString(contact_ptr->im_type,&edit_str_t);
        AppendFormLabel(win_id, 
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_SUB_FORM_ID), 
                        MMIPB_IM_LABEL_ID,
                        MMIPB_EDIT_LABEL_WIDTH,
                        &edit_str_t,
                        TRUE);

        CreateImEditBox(win_id,contact_ptr);
    }
#endif

    //Group
    if(field_infor.group_free_falg)
    {
        AppendFormChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                        MMIPB_GROUP_FORM_ID);

        AppendFormSubChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_FORM_ID), 
                        MMIPB_GROUP_SUB_FORM_ID, 
                        MMIPB_EDIT_ITEM_WIDTH);

        SetSubFormBorder(win_id,MMIPB_GROUP_SUB_FORM_ID);
        MMI_GetLabelTextByLang(TXT_PB_ITEM_DETAIL_CUSTOM_SELECT_GROUP, &label_str_t);
        AppendFormLabel(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_SUB_FORM_ID),
                        MMIPB_LABEL_GROUP_ID,
                        MMIPB_EDIT_LABEL_WIDTH,
                        &label_str_t,
                        TRUE);
		
        len = MMIPB_GetMultiGrouName(contact_ptr->union_t.group, multi_group_name);
        edit_str_t.wstr_len = len;
        edit_str_t.wstr_ptr = multi_group_name;

        AppendFormLabel(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_SUB_FORM_ID),
                        MMIPB_LABEL_GROUP_EDIT_ID,
                        MMIPB_EDIT_LABEL_WIDTH,
                        &edit_str_t,
                        TRUE);    
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_FORM_ID),
                        &hor_space,PNULL);   
        GUILABEL_SetMarginEx(MMK_GetCtrlHandleByWin(win_id,MMIPB_LABEL_GROUP_EDIT_ID),
                        12,0);
    }

    //ring
    if(field_infor.ring_free_falg)
    {
        CreateRingEditItem(win_id,contact_ptr,ring_ptr);
    }


#ifndef PB_SUPPORT_LOW_MEMORY  
    // more swtich
    if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
    {
        CreatePBEditMoreSwitchItem(win_id, 
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                    MMIPB_EDITBOX_MORE_SWITCH_FORM_ID, 
                    MMIPB_EDITBOX_MORE_SWITCH_LIST_ID);
    }

    //url
    if(field_infor.url_free_flag)
    {
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_URL_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID), 
                MMIPB_URL_SUB_FORM_ID, 
                MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_URL_SUB_FORM_ID);
        
        //append URL label 
        MMI_GetLabelTextByLang(TXT_PB_URL, &label_str_t);
        AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_SUB_FORM_ID),
                MMIPB_URL_LABEL_ID,
                MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);
        
        //append URL editor
        edit_str_t.wstr_len = contact_ptr->url.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->url.wstr;
        AppendFormEditor(&edit_str_t, 
                (MMIPB_URL_MAX_LEN>>1), 
                MMIPB_URL_MAX_LEN, 
                IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET,/*lint !e655*/
                FALSE,
                MMIPB_URL_EDITBOX_ID,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_SUB_FORM_ID),
                MMIPB_ITEM_URL,
                win_id);
        
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID),
                &hor_space,PNULL);
        
        if(contact_ptr->url.wstr_len > 0)
        {
            expand_flag = TRUE;
        }

    }   
        //生日
    if(field_infor.birth_free_flag)
    {
        CreateBirthdayItem(win_id,contact_ptr);
    }      

    //备忘录
    if(field_infor.memo_free_flag)
    {    
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_MEMO_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID), 
                MMIPB_MEMO_SUB_FORM_ID, 
                MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_MEMO_SUB_FORM_ID);

        //append memo label 
        MMI_GetLabelTextByLang(TXT_MEMO, &label_str_t);
        AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_SUB_FORM_ID),
                MMIPB_LABEL_MEMO_ID,MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);
        
        //append memo editor
        edit_str_t.wstr_len = contact_ptr->memo.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->memo.wstr;
        AppendFormEditor(&edit_str_t, 
                (MMIPB_MAX_CUSTOM_TEXT_LEN >> 1), 
                MMIPB_MAX_CUSTOM_TEXT_LEN, 
                GUIIM_TYPE_SET_ALL,
                FALSE,
                MMIPB_EDITBOX_MEMO_ID,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_SUB_FORM_ID),
                MMIPB_ITEM_MEMO,
                win_id);

        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID),
                &hor_space,PNULL);
    }    
    #endif

    #ifdef CMCC_VCARD_SUPPORT	
    if(field_infor.sne_free_flag)
    {
        max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_SNE);
        max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_SNE);
        if(max_alpha_len > 0|| contact_ptr->alias.wstr_len > 0)
        {
            AppendFormChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                        MMIPB_ALIAS_FORM_ID);
            AppendFormSubChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_FORM_ID), 
                        MMIPB_ALIAS_SUB_FORM_ID, 
                        MMIPB_EDIT_ITEM_WIDTH);
            SetSubFormBorder(win_id,MMIPB_ALIAS_SUB_FORM_ID);

            MMI_GetLabelTextByLang(TXT_PB_ITEM_DETAIL_ALIAS, &label_str_t);
            AppendFormLabel(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_SUB_FORM_ID),
                        MMIPB_LABEL_ALIAS_ID,
                        MMIPB_EDIT_LABEL_WIDTH,
                        &label_str_t,
                        TRUE);
            
            //append name edit
            edit_str_t.wstr_len = contact_ptr->alias.wstr_len;
            edit_str_t.wstr_ptr = contact_ptr->alias.wstr;

            AppendFormEditor(&edit_str_t, 
                        max_ucs2_len, 
                        max_alpha_len, 
                        GUIIM_TYPE_SET_ALL,
                        0,
                        MMIPB_EDITBOX_ALIAS_ID,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_SUB_FORM_ID),
                        MMIPB_ITEM_ALIAS,
                        win_id);

            GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_FORM_ID),
                        &hor_space,PNULL);
         }
    }
    #endif
    
    if(expand_flag)
    {
        SwitchMoreItemState(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_MORE_SWITCH_LIST_ID),IMAGE_PB_MORE_OPEN,1);
    }
    else
    {
        HideAddressBirthUrlItem(win_id);
    }
}
#else
/*****************************************************************************/
//  Description : 设置添加到手机时的LISTBOX各控件的值
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void InitContactEditorCtrl(MMI_WIN_ID_T win_id,
    MMIPB_MENU_DATA_INFOR_T *data_ptr
)
{
    uint8          i = 0;
    uint8          next_add_index = 0;
    uint8          free_count = 0;
    uint8          number_count = 0;
    uint8 create_count = 0;
    uint8 len = 0;
    wchar multi_group_name[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    
    uint16 num_index[MMIPB_MAX_NV_PHONE_NUM] = {0};
    MMI_STRING_T   label_str_t = {0};
    MMI_STRING_T   edit_str_t = {0};	
    MMIPB_NAME_T   name_t = {0};
    MMIPB_STRING_T  string_t = {0};
    

    uint16 max_ucs2_len = 0;
    uint16 max_alpha_len = 0;
    MMIPB_CUSTOM_PHOTO_T *photo_ptr = PNULL;

    MMIPB_CUSTOM_RING_T *ring_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    GUIFORM_CHILD_WIDTH_T  width_infor = {0};
    GUIFORM_CHILD_HEIGHT_T height_infor = {0};
    GUI_BG_T bg_infor = {0};
    GUI_FONT_ALL_T font_infor = {0};
    
    GUIANIM_FILE_INFO_T file_infor = {0};
    GUIANIM_DATA_INFO_T data_infor = {0};
    
    MN_NUMBER_TYPE_E number_type = 0;
    uint16 new_id = 0;
    MMIPB_FIELD_USAGE_INOF_T field_infor = {0};
    uint16 hor_space = 0;
        
           
    if( PNULL == data_ptr->contact_ptr )
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_6215_112_2_18_2_43_47_257,(uint8*)"");
        return;
    }
    contact_ptr = data_ptr->contact_ptr;
    photo_ptr = data_ptr->photo_ptr;
    ring_ptr = data_ptr->ring_ptr;
    new_id = MMIPB_GetFieldUageInfo(contact_ptr->storage_id,contact_ptr->contact_id,&field_infor);
    if(new_id>0)
    {
        contact_ptr->contact_id = new_id;
    }
    //Name
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_6233_112_2_18_2_43_47_258,(uint8*)"d",contact_ptr->storage_id);
    MMIPB_GetMaxNumbernum(contact_ptr->storage_id,&free_count);   
    max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_NAME);
    max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_NAME);
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_6241_112_2_18_2_43_47_259,(uint8*)"d",max_ucs2_len);

    if(max_ucs2_len > 0)
    {
        AppendDesignateTypeFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_NAME_FORM_ID,
                GUIFORM_LAYOUT_SBS);

    if(field_infor.photo_free_flag)
    {
        CreatePhotoAnim(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_FORM_ID),
                contact_ptr,
                photo_ptr);
    }


    AppendDesignateTypeFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_FORM_ID),
                MMIPB_NAME_EDIT_SUB_FORM_ID,
                GUIFORM_LAYOUT_ORDER);
    SetSubFormBorder(win_id, MMIPB_NAME_EDIT_SUB_FORM_ID);

    MMI_GetLabelTextByLang(TXT_NAME, &label_str_t);
    AppendFormLabel(win_id, 
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_EDIT_SUB_FORM_ID), 
                MMIPB_LABEL_NAME_ID, 
                MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);

    edit_str_t.wstr_len = contact_ptr->name.wstr_len;
    edit_str_t.wstr_ptr = contact_ptr->name.wstr;
    AppendFormEditor(&edit_str_t,
                max_ucs2_len,
                max_alpha_len, 
                GUIIM_TYPE_SET_ALL, 
                MMIPB_IsSIMContact(contact_ptr->storage_id),
                MMIPB_EDITBOX_NAME_ID,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_NAME_EDIT_SUB_FORM_ID),
                MMIPB_ITEM_NAME,
                win_id);
    }
    
    //NV number entry
    AppendDesignateTypeFormChild(win_id, 
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_EDITBOX_NUMBER_FORM_ID,
                GUIFORM_LAYOUT_ORDER);

    number_count = GetContactNumberNum(contact_ptr,num_index);
    create_count = MIN(number_count,free_count);

    for(i = 0;i< create_count;i++)
    {
        CreateContactNumberItem(win_id,contact_ptr,num_index[i]);
    }
    if(0==number_count || free_count > create_count)
    {
        next_add_index = GetNextAddNumberIndex(0,contact_ptr);
        CreateContactNumberItem(win_id,contact_ptr,next_add_index);
    }

#ifdef MMIPB_MAIL_SUPPORT
     //mail
    if(field_infor.mail_free_flag)
    {
        CreateEmailEditItem(win_id, contact_ptr);
    }
#endif

#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
    //imessager
    if(field_infor.im_free_flag)        
	{
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_IM_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_FORM_ID),
                MMIPB_IM_SUB_FORM_ID,
                MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_IM_SUB_FORM_ID);
        
        MMIPB_GetImTypeString(contact_ptr->im_type,&edit_str_t);
        AppendFormLabel(win_id, 
                MMK_GetCtrlHandleByWin(win_id,MMIPB_IM_SUB_FORM_ID), 
                MMIPB_IM_LABEL_ID,
                MMIPB_EDIT_LABEL_WIDTH,
                &edit_str_t,
                TRUE);
        CreateImEditBox(win_id, contact_ptr);
    }
#endif

    //group
    if(field_infor.group_free_falg)
    {
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_GROUP_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_FORM_ID), 
                MMIPB_GROUP_SUB_FORM_ID, 
                MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_GROUP_SUB_FORM_ID);
        
        //append Group label text
        MMI_GetLabelTextByLang(TXT_PB_ITEM_DETAIL_CUSTOM_SELECT_GROUP, &label_str_t);
        AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_SUB_FORM_ID),
                MMIPB_LABEL_GROUP_ID,MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);
        len = MMIPB_GetMultiGrouName(contact_ptr->union_t.group, multi_group_name);
        edit_str_t.wstr_len = len;
        edit_str_t.wstr_ptr = multi_group_name;
        AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_SUB_FORM_ID),
                MMIPB_LABEL_GROUP_EDIT_ID,MMIPB_EDIT_LABEL_WIDTH,
                &edit_str_t,
                TRUE);    
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_FORM_ID),
                &hor_space,PNULL);   
    }

    //ring
    if(field_infor.ring_free_falg)
    {
        CreateRingEditItem(win_id,contact_ptr,ring_ptr);
    }

    #ifndef PB_SUPPORT_LOW_MEMORY
    //more switch
    if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
    {
        CreatePBEditMoreSwitchItem(win_id, 
            MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
            MMIPB_EDITBOX_MORE_SWITCH_FORM_ID, 
            MMIPB_EDITBOX_MORE_SWITCH_LIST_ID);
    }

    //url        
    if(field_infor.url_free_flag)
    {
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_URL_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID), 
                MMIPB_URL_SUB_FORM_ID, 
                MMIPB_EDIT_ITEM_WIDTH);
        SetSubFormBorder(win_id,MMIPB_URL_SUB_FORM_ID);
        
        //append URL label 
        MMI_GetLabelTextByLang(TXT_PB_URL, &label_str_t);
        AppendFormLabel(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_SUB_FORM_ID),
                MMIPB_URL_LABEL_ID,
                MMIPB_EDIT_LABEL_WIDTH,
                &label_str_t,
                TRUE);
        
        //append URL editor
        edit_str_t.wstr_len = contact_ptr->url.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->url.wstr;
        AppendFormEditor(&edit_str_t, 
                (MMIPB_URL_MAX_LEN>> 1), 
                MMIPB_URL_MAX_LEN, 
                IM_DEFAULT_PB_EMAIL_INPUT_MODE_SET,
                FALSE,
                MMIPB_URL_EDITBOX_ID,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_SUB_FORM_ID),
                MMIPB_ITEM_URL,
                win_id);
        
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID),
                &hor_space,PNULL);
    }   
    
    //生日
    if(field_infor.birth_free_flag)
    {
        CreateBirthdayItem(win_id, contact_ptr);
    }      
    #endif
        
   #ifndef PB_SUPPORT_LOW_MEMORY
    //备忘录
    if(field_infor.memo_free_flag)
    {    
        AppendFormChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                MMIPB_MEMO_FORM_ID);
        AppendFormSubChild(win_id,
                MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID), 
                MMIPB_MEMO_SUB_FORM_ID, 
                MMIPB_EDIT_ITEM_WIDTH);        
        SetSubFormBorder(win_id,MMIPB_MEMO_SUB_FORM_ID);
        
        //append memo label 
        MMI_GetLabelTextByLang(TXT_MEMO, &label_str_t);
        AppendFormLabel(win_id,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_SUB_FORM_ID),
                    MMIPB_LABEL_MEMO_ID,
                    MMIPB_EDIT_LABEL_WIDTH,
                    &label_str_t,
                    TRUE);
        
        //append memo editor
        edit_str_t.wstr_len = contact_ptr->memo.wstr_len;
        edit_str_t.wstr_ptr = contact_ptr->memo.wstr;
        AppendFormEditor(&edit_str_t, 
                    (MMIPB_MAX_CUSTOM_TEXT_LEN >> 1), 
                    MMIPB_MAX_CUSTOM_TEXT_LEN, 
                    GUIIM_TYPE_SET_ALL,
                    FALSE,
                    MMIPB_EDITBOX_MEMO_ID,
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_SUB_FORM_ID),
                    MMIPB_ITEM_MEMO,
                    win_id);
        GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID),
                    &hor_space,PNULL);
      }
    
    #endif

    #ifdef CMCC_VCARD_SUPPORT	
    if(field_infor.sne_free_flag)
    {
        max_ucs2_len = MMIPB_GetMaxWstrLen(contact_ptr->storage_id, MMIPB_CONTACT_SNE);
        max_alpha_len = MMIPB_GetMaxAlphaLen(contact_ptr->storage_id, MMIPB_CONTACT_SNE);
        if(max_alpha_len > 0|| contact_ptr->alias.wstr_len > 0)
        {
            AppendFormChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID),
                        MMIPB_ALIAS_FORM_ID);
            AppendFormSubChild(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_FORM_ID), 
                        MMIPB_ALIAS_SUB_FORM_ID, 
                        MMIPB_EDIT_ITEM_WIDTH);
            SetSubFormBorder(win_id, MMIPB_ALIAS_SUB_FORM_ID);

            MMI_GetLabelTextByLang(TXT_PB_ITEM_DETAIL_ALIAS, &label_str_t);
            AppendFormLabel(win_id,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_SUB_FORM_ID),
                        MMIPB_LABEL_ALIAS_ID,
                        MMIPB_EDIT_LABEL_WIDTH,
                        &label_str_t,
                        TRUE);

            //append name edit
            edit_str_t.wstr_len = contact_ptr->alias.wstr_len;
            edit_str_t.wstr_ptr = contact_ptr->alias.wstr;
            AppendFormEditor(&edit_str_t, 
                        max_ucs2_len, 
                        max_alpha_len, 
                        GUIIM_TYPE_SET_ALL,
                        0,
                        MMIPB_EDITBOX_ALIAS_ID,
                        MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_SUB_FORM_ID),
                        MMIPB_ITEM_ALIAS,
                        win_id);
            
            GUIFORM_SetSpace(MMK_GetCtrlHandleByWin(win_id,MMIPB_ALIAS_FORM_ID),
            &hor_space,PNULL);
        }
    }
    #endif

  HideAddressBirthUrlItem(win_id);
}
#endif
#endif//pad support
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : add or delete editbox by delete button
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateContactEditNumber(
    MMI_HANDLE_T win_id,
    uint16 delete_index,
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
        )
{

    uint8 i = 0;
    uint8 loop_count = 0;
    uint8 next_add_index = 0;
    uint16 cur_index = 0;
    //MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    //BOOLEAN is_in_sim = FALSE;
    
    MMI_STRING_T edit_str_t = {0};
    MMIPB_STRING_T string_t = {0};
    MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
    wchar wstr[MMIPB_MAX_STRING_LEN + 2] = {0};
    MMIPB_FIELD_USAGE_INOF_T usage_flag = {0};
    MMIPB_CONTACT_T *contact_ptr = PNULL;
  
    if(PNULL == menu_infor_ptr||PNULL == menu_infor_ptr->contact_ptr)
    {
        return;
    }

    contact_ptr= menu_infor_ptr->contact_ptr;
    // Number num
    MMIPB_GetMaxNumbernum(contact_ptr->storage_id, &loop_count);
    if(delete_index>=loop_count)
    {
        return;
    }
    	//update name
    edit_str_t.wstr_len = contact_ptr->name.wstr_len;
    edit_str_t.wstr_ptr = contact_ptr->name.wstr;

    GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_NAME_ID),
        edit_str_t.wstr_ptr,edit_str_t.wstr_len);
    
    MMIPB_GetFieldUageInfo(contact_ptr->storage_id, contact_ptr->contact_id, &usage_flag);
    cur_index = GetUserEditNumber(win_id,contact_ptr);
    //NV entry
    next_add_index = GetNextAddNumberIndex(cur_index,contact_ptr);

	if(delete_index >= MMIPB_MAX_NV_PHONE_NUM)
	{
		//SCI_TRACE_LOW:"[MMIPB] UpdateContactEditNumber delete_index >= MMIPB_MAX_NV_PHONE_NUM"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_6737_112_2_18_2_43_48_260,(uint8*)"");
		return;
	}
    SCI_MEMSET(&contact_ptr->number[delete_index],0,sizeof(MMIPB_BCD_NUMBER_T));
                    
    for ( i = 0; i < loop_count; i++ )
    {
        if(contact_ptr->number[i].number_len>0) 
        {
            number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[i].npi_ton);
        string_t.strlen = MMIAPICOM_GenDispNumber(
			(number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),
			contact_ptr->number[i].number_len, 
			contact_ptr->number[i].number,
			string_t.strbuf,
			MMIPB_MAX_STRING_LEN+2
			);
        
        edit_str_t.wstr_ptr = wstr;
        edit_str_t.wstr_len = MIN(MMIPB_MAX_STRING_LEN+2, string_t.strlen);
		
        MMI_STRNTOWSTR(wstr, MMIPB_MAX_STRING_LEN+2, string_t.strbuf, MMIPB_MAX_STRING_LEN+2, string_t.strlen);
        
        GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_id,s_num_edit_id[i]),
            edit_str_t.wstr_ptr,edit_str_t.wstr_len);
        }
        else
        {
          
            
            if(i!= next_add_index&&MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i])>0)
            {
                GUIFORM_DestroyDynaChildCtrl(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_NUMBER_FORM_ID),
                    MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i]));
                GUIFORM_ForceLayout(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID));

				MMK_UpdateScreen();
            }
            else if(MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i])>0)
            {
              GUIEDIT_ClearAllStr(MMK_GetCtrlHandleByWin(win_id,s_num_edit_id[i]));
            GUIFORM_SetChildDisplay(
                MMK_GetCtrlHandleByWin(win_id,s_num_form_id[i]),
                MMK_GetCtrlHandleByWin(win_id,s_num_delete_id[i]), 
               GUIFORM_CHILD_DISP_HIDE);
            }
        }
    }
 /*   if(usage_flag.mail_free_flag)
    {
        
        if(contact_ptr->mail.wstr_len>0)
        {
            edit_str_t.wstr_len = contact_ptr->mail.wstr_len;
            edit_str_t.wstr_ptr = contact_ptr->mail.wstr;
            GUIEDIT_SetString(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_EMAIL_ID),
                edit_str_t.wstr_ptr,edit_str_t.wstr_len);
            }
            else
            {
            GUIEDIT_ClearAllStr(MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_EMAIL_ID));
            
        }
    }
    */
}
/*****************************************************************************/
//  Description : update edit win
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdatePhotoRingLabelEdit(						
						  MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
						   )
{

    MMI_STRING_T text_str = {0};
    
    #ifdef ASP_SUPPORT
    MMI_TEXT_ID_T name_id = TXT_NULL;
    #endif
    MMI_HANDLE_T win_id  =0;

    wchar multi_group_name[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};
    
    uint16          file_name[MMIPB_MAX_CUSTOM_RING_PATH_LEN+1] = {0};
    uint8           len = 0;

    if(PNULL == menu_infor_ptr)
    {
        return;
    }
    win_id = menu_infor_ptr->parent_win;

	//update ring id
    if(PNULL != menu_infor_ptr->ring_ptr)
    {
        if (menu_infor_ptr->ring_ptr->is_valid || 
            (menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG))
        {
            if(menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG) 
            {
                text_str.wstr_len = MMIPB_GetRingName((menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
					file_name, MMIPB_MAX_CUSTOM_RING_PATH_LEN);
                text_str.wstr_ptr = file_name;
            }
            else
            {
                text_str.wstr_len = menu_infor_ptr->ring_ptr->file_name_len;
                text_str.wstr_ptr = menu_infor_ptr->ring_ptr->file_name;
            }
        }
        else
        {
            text_str.wstr_len = MMIPB_GetRingName((menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
					file_name, MMIPB_MAX_CUSTOM_RING_PATH_LEN);
            text_str.wstr_ptr = file_name;
        }

        GUILABEL_SetText(
            MMK_GetCtrlHandleByWin(win_id,MMIPB_LABEL_RING_EDIT_ID),
            &text_str, TRUE);
	}

#ifdef ASP_SUPPORT
	if(menu_infor_ptr->asp_ptr != PNULL)
	{
		if(menu_infor_ptr->contact_ptr->asp_id_flag & MMIPB_ASP_FIX_ID_EXIST) 
		{
			//fixed asp id
            name_id = MMIAPIASP_GetFixedRingNameByID(menu_infor_ptr->contact_ptr->asp_id_flag & MMIPB_ASP_FIX_ID_MASK);

			MMI_GetLabelTextByLang(name_id, &text_str);//need function from asp
		}
		else
		{
			text_str.wstr_len = menu_infor_ptr->asp_ptr->file_name_len;
			text_str.wstr_ptr = menu_infor_ptr->asp_ptr->file_name;
		}
		GUILABEL_SetText(
                    MMK_GetCtrlHandleByWin(win_id,MMIPB_LABEL_ASP_EDIT_ID),
                    &text_str,TRUE);
	}
#endif

	//update group
    len = MMIPB_GetMultiGrouName(menu_infor_ptr->contact_ptr->union_t.group, multi_group_name);
    
	text_str.wstr_len = len;
	text_str.wstr_ptr = multi_group_name;
	GUILABEL_SetText(
           MMK_GetCtrlHandleByWin(win_id,MMIPB_LABEL_GROUP_EDIT_ID),
            &text_str, TRUE);

}
#endif

#ifndef PB_SUPPORT_LOW_MEMORY
#if 0
/*****************************************************************************/
//  Description : get birthday date from ctrl and set it
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBirthDate(
    MMI_HANDLE_T win_id,
    MMI_CTRL_ID_T     ctrl_id,
    MMIPB_CONTACT_T *contact_ptr
                      )
{
    uint8           month       =   0;
    uint8           day         =   0;
    uint16          year        =   0;
    //BOOLEAN         result      = TRUE;
    MMISET_DATE_T   date        =   {0};
    //SCI_DATE_T      sys_date    =   {0};
    char   str_buf[8] = {0};
    char    date_str[20]={0};

  //  MMI_STRING_T str_t = {0};
    //get date
    GUIEDIT_GetDate(ctrl_id,&year,&month,&day);
    date.date_day = day;
    date.date_month = month;
    date.date_year = year;
    
    //check date if is valid
//    if (MMIAPICOM_CheckDateIsValid(date))
    {
        extern int8 *itoa( int32 v, int8* str, uint32 r); 
        SCI_MEMSET(&contact_ptr->birth,0,sizeof(MMIPB_BIRTH_T));
        itoa(year,str_buf,10);/*lint !e64*/
            strcat(date_str,str_buf);
            SCI_MEMSET(str_buf,0,sizeof(str_buf));
            itoa(month,str_buf,10);/*lint !e64*/
			if(month<10)
			{
					strcat(date_str,"0");
			}
            strcat(date_str,str_buf);
            SCI_MEMSET(str_buf,0,sizeof(str_buf));
            itoa(day,str_buf,10);/*lint !e64*/
			if(day<10)
			{
					strcat(date_str,"0");
			}
            strcat(date_str,str_buf);
            
            MMIAPICOM_StrToWstr(date_str,contact_ptr->birth.wstr);/*lint !e64*/
            
            contact_ptr->birth.wstr_len = MIN(strlen(date_str), MMIPB_SNS_BIRTH_LEN);/*lint !e666*/
         //   str_t.wstr_ptr = contact_ptr->birth.wstr;
        //    str_t.wstr_len = contact_ptr->birth.wstr_len;
       //     GUILABEL_SetText(
       //         MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_EDIT_ID),
        //        &str_t, TRUE);
            MMK_SendMsg(win_id,MMI_PB_CHANGE_BIRTHDAY,contact_ptr->birth.wstr);
            return TRUE;
    }
//    else
//    {
//        return FALSE;
//     }
}
#else
extern int8 *itoa( int32 v, int8* str, uint32 r); 
/*****************************************************************************/
//  Description : get birthday date from ctrl and set it
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetBirthDate
(
  MMI_HANDLE_T win_id,
  MMI_CTRL_ID_T     ctrl_id,
  MMIPB_CONTACT_T *contact_ptr
)
{
	uint8           month       =   0;
	uint8           day         =   0;
	uint16          year        =   0;
	MMISET_DATE_T   date        =   {0};
	char   str_buf[8] = {0};
	char    date_str[20]={0};
    MMISET_DATE_DISPLAY_TYPE_E   date_type = MMISET_DATE_YMD;
	
	//get date
	GUIEDIT_GetDate(ctrl_id,&year,&month,&day);
	date.date_day = day;
	date.date_month = month;
	date.date_year = year;
	
	SCI_MEMSET(&contact_ptr->birth,0,sizeof(MMIPB_BIRTH_T));
	if( 1900 == date.date_year //MMITHEME_EDI_DATE_YEAR_QUERY_MIN
	    && 01 == date.date_month
	    && 01 == date.date_day
	)
	{
	    return FALSE;
	}
    date_type = MMIAPISET_GetDateDisplayType();
    switch (date_type)
    {
    case MMISET_DATE_YMD:
        sprintf(date_str, "%04d-%02d-%02d",year, month, day);
        break;
        
    case MMISET_DATE_MDY:
        sprintf(date_str, "%02d-%02d-%04d",month, day, year);
        break;
        
    case MMISET_DATE_DMY:
        sprintf(date_str, "%02d-%02d-%04d",day, month, year);
        break;
        
    default:
        sprintf(date_str, "%04d-%02d-%02d",year, month, day);
        break;
    }
#if 0
	itoa(year,str_buf,10);
	strcat(date_str,str_buf);
	strcat(date_str,"-");
	SCI_MEMSET(str_buf,0,sizeof(str_buf));
	itoa(month,str_buf,10);
	if(month<10)
	{
		strcat(date_str,"0");
	}
	strcat(date_str,str_buf);
	strcat(date_str,"-");
	SCI_MEMSET(str_buf,0,sizeof(str_buf));
	itoa(day,str_buf,10);
	if(day<10)
	{
		strcat(date_str,"0");
	}
	strcat(date_str,str_buf);
#endif
	MMIAPICOM_StrToWstr(date_str,contact_ptr->birth.wstr);
	contact_ptr->birth.wstr_len = MIN(strlen(date_str), MMIPB_SNS_BIRTH_LEN);
	return TRUE;
}
#endif

/*****************************************************************************/
//  Description : handle Set birthday pub win msg
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBirthdayWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_BIRTHDAY_DATEEDITBOX_CTRL_ID);
    MMIPB_MENU_DATA_INFOR_T *data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == data_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIEDIT_SetDateRange(MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_BIRTHDAY_DATEEDITBOX_CTRL_ID),GUIEDIT_DATE_YEAR_RANGE_QUERY);
            GUIFORM_IsSlide(MMIPB_SET_BIRTHDAY_FORM_ID,FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if(SetBirthDate(data_ptr->parent_win,ctrl_id,data_ptr->contact_ptr))
                {
                    MMK_CloseWin(win_id);      
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextIdEx(data_ptr->applet_handle,PNULL,
                        TXT_INVALID_DATE,TXT_NULL,
                        IMAGE_PUBWIN_WARNING,
                        PNULL,
                        PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    PNULL,
                    PNULL);
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);      
                break; 
            default:
                break;
            }
        }
        break; 
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        if(SetBirthDate(data_ptr->parent_win,ctrl_id,data_ptr->contact_ptr))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextIdEx(data_ptr->applet_handle,PNULL,
                TXT_INVALID_DATE,TXT_NULL,
                IMAGE_PUBWIN_WARNING,
                PNULL,
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL,
                PNULL);
        }
        break;

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
#endif
/*****************************************************************************/
//  Description : compound all the element into a contact item and add to nv or sim
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AddContact(
   MMIPB_MENU_DATA_INFOR_T *data_ptr
    )
{
    uint8 loop_count = {0};
    uint16 i = 0;
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    uint8 num_buf[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
    //SCI_TRACE_LOW("[baokun] AddContact enter id 0x%x", data_ptr->contact_ptr->storage_id);
    if(PNULL == data_ptr||PNULL == data_ptr->contact_ptr)
    {
        return ret;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(data_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return ret;
    }
    
    MMIPB_GetMaxNumbernum(
        data_ptr->contact_ptr->storage_id, 
        &loop_count);

    for(i=0;i<loop_count;i++)
    {      
        SCI_MEMSET(num_buf,0,sizeof(num_buf));
        
        if(!MMIPB_IsValidPhoneNumber(
            data_ptr->contact_ptr->number[i].npi_ton,
            data_ptr->contact_ptr->number[i].number_len))
            {
                data_ptr->contact_ptr->num_type[i] = 0;
            }
        else
        {
            MMIPB_BCDNumberToUTF8(&data_ptr->contact_ptr->number[i], MMIPB_NUMBER_MAX_STRING_LEN+2, num_buf);
            if(!MMIAPICOM_IsValidNumberString((char*)num_buf, strlen(num_buf)))/*lint !e64*/
            {
                return MMIPB_ERROR_INVALID_NUMBER;
            }
        }
    }
    if(PNULL!=data_ptr->photo_ptr&&!(data_ptr->photo_ptr->is_valid))
    {
        MMIPB_DeleteContactInfoFile(data_ptr->contact_ptr->contact_id,
            data_ptr->contact_ptr->storage_id,
        MMIPB_PHOTO_FILE);
        SCI_MEMSET(data_ptr->photo_ptr,0,sizeof(MMIPB_CUSTOM_PHOTO_T));
    }
    if(PNULL!=data_ptr->ring_ptr&&!(data_ptr->ring_ptr->is_valid))
    {
        MMIPB_DeleteContactInfoFile(data_ptr->contact_ptr->contact_id,
            data_ptr->contact_ptr->storage_id,
        MMIPB_RING_FILE);
        SCI_MEMSET(data_ptr->ring_ptr,0,sizeof(MMIPB_CUSTOM_RING_T));
    }
    #ifdef ASP_SUPPORT
    if(PNULL!=data_ptr->asp_ptr&&!(data_ptr->asp_ptr->is_valid))
    {
        MMIPB_DeleteContactInfoFile(data_ptr->contact_ptr->contact_id,
            data_ptr->contact_ptr->storage_id,
        MMIPB_ASP_FILE);
        SCI_MEMSET(data_ptr->asp_ptr,0,sizeof(MMIPB_CUSTOM_RING_T));
    }
    #endif

    
    if(PNULL !=data_ptr->ring_ptr && data_ptr->ring_ptr->is_valid
        ||PNULL != data_ptr->photo_ptr && data_ptr->photo_ptr->is_valid
#ifdef ASP_SUPPORT
        ||PNULL != data_ptr->asp_ptr&&data_ptr->asp_ptr->is_valid
#endif
        )
    {
       MMIPB_AddContact_Fileinfo(data_ptr->contact_ptr, 
            data_ptr->photo_ptr, 
            data_ptr->ring_ptr
            #ifdef ASP_SUPPORT
            ,data_ptr->asp_ptr
            #else
            ,PNULL
            #endif
            );
    }

    applet_ptr->instance.operate_type =  data_ptr->operate_type;
    //SCI_TRACE_LOW("[mmipb] AddContact %x %x %x",data_ptr->contact_ptr->name.wstr[0],data_ptr->contact_ptr->name.wstr[1],data_ptr->contact_ptr->name.wstr[2]);
    if(MMIPB_OPERATE_TYPE_EDIT_ONE == data_ptr->operate_type)
    {//modify
        ret = MMIPB_ModifyContactAsynForPB(applet_ptr->instance.handle,
            data_ptr->contact_ptr);
    }
    else    
    {
        ret = MMIPB_AddContactAsynForPB(applet_ptr->instance.handle,
        data_ptr->contact_ptr);
    }
    //SCI_TRACE_LOW:"[baokun] AddContact ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_7145_112_2_18_2_43_49_261,(uint8*)"d", ret);

    if(ret == MMIPB_ERROR_PENDING)
    {
        MMIPB_OpenWaitWin(applet_ptr->caf_applet.app_handle, TXT_COMM_SAVE_CONTACT);
    } 
    return ret;
}


/*****************************************************************************/
//  Description : delete one or more contact 
//  Global resource dependence : none
//  Parameter: 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactWin(MMI_HANDLE_T applet_handle,
                                           MMIPB_HANDLE_T cmd_handle 
                                           )
{
    MMIPB_APPLET_T *applet_ptr =PNULL;
    
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    
    if(MMIPB_IsMark(applet_handle))
    {
        MMIPB_AbortPB(FALSE);
        ret = MMIPB_DeleteContactAsynForPB(cmd_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"DeleteContact applet handle is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_7177_112_2_18_2_43_49_262,(uint8*)"d",applet_handle);
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(applet_handle);
        //SCI_TRACE_LOW:"DeleteContact applet_ptr is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_7179_112_2_18_2_43_49_263,(uint8*)"d",applet_ptr);
        if(PNULL!= applet_ptr)
        {
            MMIPB_AbortPB(FALSE);
            ret = MMIPB_DeleteUnListContact(cmd_handle,
                applet_ptr->instance.cur_entry_id,
                applet_ptr->instance.cur_storage_id
                );
        }
    }
    if(ret == MMIPB_ERROR_PENDING)
    {
    		#ifdef HERO_ENGINE_TTS_SUPPORT 
    		{
    			extern uint8 s_hero_no_tts;
			s_hero_no_tts = 1;
    		}
		#endif
        MMIPB_OpenProcessWin(applet_handle,STR_DELETING_CONTACTS, TXT_PB_DELETE_CONTACT);
    }    
    
    return ret;
}
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteGroupEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    uint32 group_id = 0;
    
    if(PNULL == judge_info_ptr /*|| PNULL == judge_info_ptr->contact_ptr*/)
    {
        result = FALSE;
    }
    else
    {
#ifndef MMIPB_SYSTEM_GROUP_DELETE_SUPPORT
        MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);/*lint !e64 !e613*/
        if(MMIPB_IsUserGroup(group_id))
        {
            result = TRUE;
        }
#else /*MMIPB_SYSTEM_GROUP_DELETE_SUPPORT*/
        result = TRUE;
#endif /*MMIPB_SYSTEM_GROUP_DELETE_SUPPORT*/
    }
    return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : edit group menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    uint32 group_id = 0;
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else
    {
        MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);/*lint !e64 !e613*/
        if(!MMIPB_IsInNvGroup(group_id))
        {
            result = FALSE;
        }
    }
    return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : group rename menu whether should disable
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupNameEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    uint32 group_id = 0;
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else
    {
        MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);/*lint !e64 !e613*/
#ifndef MMIPB_SYSTEM_GROUP_DELETE_SUPPORT
        if(!MMIPB_IsUserGroup(group_id))
        {
            result = FALSE;
        }
#else /*MMIPB_SYSTEM_GROUP_DELETE_SUPPORT*/
        result = TRUE;
#endif /*MMIPB_SYSTEM_GROUP_DELETE_SUPPORT*/
    }
    return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : delete group menu whether should disable
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditGroupRingEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    uint32 group_id = 0;
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else
    {
        MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);/*lint !e64 !e613*/
        if(!MMIPB_IsInNvGroup(group_id))
        {
            result = FALSE;
        }
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description :modify one contact menu whether enable
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ModifyMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{

    if(PNULL == (MMIPB_MENU_DATA_INFOR_T *)judge_info_ptr)
    {
        return FALSE;
    }    
    if(PNULL != (MMIPB_CONTACT_T *)(judge_info_ptr->contact_ptr))
    {
#ifdef MMIPB_MOST_USED_SUPPORT
        if(MMIPB_IsMostUsedContact(judge_info_ptr->contact_ptr->storage_id))
        {
            return FALSE;
        }
#endif
        if(MMIPB_IsSDNContact(judge_info_ptr->contact_ptr->storage_id))
        {
            return FALSE;
        }        
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GroupsMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description :add one contact menu whether enable
//  Global resource dependence : none
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;

    if(PNULL == (MMIPB_MENU_DATA_INFOR_T *)judge_info_ptr)
    {
        result = FALSE;
		return result;
    }

    if(PNULL != (MMIPB_CONTACT_T *)(judge_info_ptr->contact_ptr))
    {
#ifdef MMIPB_SIMDN_SUPPORT        
        if(
            MMIPB_IsMsisdnContact(judge_info_ptr->contact_ptr->storage_id) 
            || MMIPB_IsSDNContact(judge_info_ptr->contact_ptr->storage_id) 
               
            )
        {
            return FALSE;
        }
#endif        
#ifdef SNS_SUPPORT 
       if(MMIPB_IsSNSContact(judge_info_ptr->contact_ptr->storage_id))
       {
           return FALSE;
       }
#endif
#ifdef FDN_SUPPORT        
       if(MMIPB_IsFdnContact(judge_info_ptr->contact_ptr->storage_id))
        {
            return TRUE;
        }
#endif        
    }
#ifdef MMI_PDA_SUPPORT    
    if((1 <<PB_GROUP_ALL) == judge_info_ptr->group)/*lint !e613*/
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#else
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_MOSTUSED_OPT_MENU == judge_info_ptr->menu_type)
    {
        result  = FALSE;
    }
#endif
#endif
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShareMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;

    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : delete menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteMultiMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(applet_ptr == PNULL)
    {
        return FALSE;
    }
    
    if(judge_info_ptr->valid_contact_count == 0)
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : delete menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendBtVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
 /*   BOOLEAN result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(applet_ptr == PNULL)
    {
        return FALSE;
    }
    
    if(judge_info_ptr->valid_contact_count == 0)
    {
        result = FALSE;
    }

    return result;*/
	return TRUE;
}
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : select sim or phone enable
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelStorageMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }

    if(MMIPB_OPERATE_TYPE_ERASE_ALL == judge_info_ptr->operate_type
      || MMIPB_OPERATE_TYPE_ERASE_ONE == judge_info_ptr->operate_type
      ||MMIPB_OPERATE_TYPE_ERASE_MULTI == judge_info_ptr->operate_type        
        )
    {
        result = TRUE;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : mark all menu whether should disable
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MarkAllMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MMI_HANDLE_T win_handle = 0;
    MMI_HANDLE_T ctr_handle = 0;
    MMI_WIN_ID_T win_id = 0;
    uint16       Tot_Num = 0, Sel_Num = 0, Split_Num = 0;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
//	    win_handle = MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, MMIPB_OPERATE_SEL_LIST_WIN_ID);
//	    ctr_handle = MMK_GetCtrlHandleByWin(win_handle, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);
    win_id = MMK_GetWinId(judge_info_ptr->parent_win);
    win_handle = MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, win_id);
    ctr_handle = MMK_GetCtrlHandleByWin(win_handle, judge_info_ptr->ctl_id);

    Tot_Num = GUILIST_GetTotalItemNum(ctr_handle);
    Sel_Num = GUILIST_GetSelectedItemNum(ctr_handle);
    Split_Num = applet_ptr->instance.split_num;

    if (0 == Tot_Num || Tot_Num == Sel_Num 
        || applet_ptr->instance.is_all_select
        || (Tot_Num-Split_Num) == Sel_Num)
    {
        result = FALSE;
    }
    return result;
    
}
/*****************************************************************************/
//  Description : unmark all menu whether should disable
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CancelAllMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    MMIPB_APPLET_T  *applet_ptr = PNULL;
    MMI_HANDLE_T win_handle = 0;
    MMI_HANDLE_T ctr_handle = 0;
    MMI_WIN_ID_T win_id = 0;
    uint16 item_num = 0;
    
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
//	    win_handle = MMK_GetWinHandle(applet_ptr->caf_applet.app_handle,MMIPB_OPERATE_SEL_LIST_WIN_ID);
//	    ctr_handle = MMK_GetCtrlHandleByWin(win_handle, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);
    win_id = MMK_GetWinId(judge_info_ptr->parent_win);
    win_handle = MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, win_id);
    ctr_handle = MMK_GetCtrlHandleByWin(win_handle, judge_info_ptr->ctl_id);
    item_num = GUILIST_GetSelectedItemNum(ctr_handle);
    if(0 == item_num)
    {
        result = FALSE;
    }
    return result;  
}

/*****************************************************************************/
//  Description :delete contact menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
#ifndef MMI_PDA_SUPPORT 
    MMIPB_APPLET_T *applet_ptr = PNULL;
#endif
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
#ifndef MMI_PDA_SUPPORT    
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(applet_ptr == PNULL)
    {
        return FALSE;
    }

    if(judge_info_ptr->valid_contact_count == 0)
    {
        result = FALSE;
    }
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif

    return result;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : Clear MostUsedContact
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ClearMostUsedMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
    
    if(MMIPB_MOSTUSED_OPT_MENU == judge_info_ptr->menu_type)
    {
        if(0==MMIPB_GetGroupContactCount(PB_GROUP_MOSTUSED))
        {
            result = FALSE;
        }
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : copy contact from sim1 menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyFromSimEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    if(menu_node >= PB_OPTMENU_NODE_COPY_FROM_SIM1 &&  menu_node < PB_OPTMENU_NODE_COPY_FROM_SIM1 + MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1 + menu_node - PB_OPTMENU_NODE_COPY_FROM_SIM1;
    }
    if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        result =  TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : copy contact from sim1 menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SimMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;


    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            result = TRUE;
            break;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description :添加到群组菜单是否enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToGroupMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN  result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
    
    if(!MMIPB_IsPhoneContact(applet_ptr->instance.cur_storage_id))
    {
        result = FALSE;
    }
    
    return result;
}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description :add to favorite menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToFavoriteEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN  result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;

    if(PNULL == judge_info_ptr||PNULL == judge_info_ptr->contact_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
    if(MMIPB_FAVORITE_OPT_MENU != judge_info_ptr->menu_type)
    {
        if(!MMIPB_IsPhoneContact(applet_ptr->instance.cur_storage_id))
        {
            result = FALSE;
        }
        else if(MMIPB_IsFavouriteContact(judge_info_ptr->contact_ptr->union_t.group))
        {
            result = FALSE;
        }
    }
    return result;
}
#endif

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description :remove favorite menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveFavoriteEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN  result = TRUE;
    MMIPB_APPLET_T *applet_ptr = PNULL;

    if(PNULL == judge_info_ptr||PNULL == judge_info_ptr->contact_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }     
    if(MMIPB_FAVORITE_OPT_MENU == judge_info_ptr->menu_type)
    {
        if(0==MMIPB_GetGroupContactCount(PB_GROUP_FAVROUTE))
        {
            result = FALSE;
        }
    }
    else if(!MMIPB_IsPhoneContact(applet_ptr->instance.cur_storage_id))
    {
        result = FALSE;
    }
    else if(!MMIPB_IsFavouriteContact(judge_info_ptr->contact_ptr->union_t.group))
    {
        result = FALSE;
    }
    return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :add group member menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddGroupMemberEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
#ifndef MMI_PDA_SUPPORT
    uint8   group_id = 0;
#endif
    if(PNULL == judge_info_ptr||PNULL == judge_info_ptr->contact_ptr)
    {
        return FALSE;
    }
    
    #ifndef MMI_PDA_SUPPORT
    MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);
    if(!MMIPB_IsInNvGroup(group_id))
    {
        result = FALSE;
    }
    #endif        
   return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :remove group member menu whether enable
//  Global resource dependence : none
//  Author: starmer
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveGroupMemberEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
    uint8   group_id = 0;
    if(PNULL == judge_info_ptr||PNULL == judge_info_ptr->contact_ptr)
    {
        return FALSE;
    }
    
    MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);
    if(!MMIPB_IsInNvGroup(group_id))
    {
        result = FALSE;
    }
    else if(0==MMIPB_GetGroupContactCount(group_id))
    {
        result = FALSE;
    }
    else if(0==  judge_info_ptr->valid_contact_count)
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif
          
   return result;
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :send sms menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendGroupSMSMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )

{
    BOOLEAN result = TRUE;
    uint8   group_id = 0;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
    MMIPB_GetAllGroupIDs(judge_info_ptr->group, &group_id, 1);/*lint !e613*/
    
#ifdef SNS_SUPPORT
    if(MMIPB_IsSNSGroup(group_id))
    {
        result = FALSE;
    }
#endif

    if(0==MMIPB_GetGroupContactCount(group_id))
    {
        result = FALSE;
    }

    return result;
}
#endif
/*****************************************************************************/
//  Description :dial parent menu whether enable
//  Global resource dependence : none
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WriteMessageMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
     
    #ifndef MMI_PDA_SUPPORT
    if(PNULL == judge_info_ptr)
    {
       return FALSE;
    }
    else if((judge_info_ptr->number_index<0)
        ||(judge_info_ptr->number_index > (MMIPB_LISTBOX_INDEX_NUMBER_OTHER-MMIPB_LISTBOX_INDEX_NUMBER_MOBILE)) /*lint !e656*/
        || (0 == judge_info_ptr->valid_contact_count))
    {
        result = FALSE;
    }
    else if((PNULL != judge_info_ptr->contact_ptr) 
        &&  (MMIPB_GetFirstValidNumberIndex(judge_info_ptr->contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM) >= MMIPB_MAX_NV_PHONE_NUM))
    {
        result = FALSE;
    }
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
    #endif    
    
   return result;    
}
/*****************************************************************************/
//  Description :dial parent menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN DialMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
    
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    else if((judge_info_ptr->number_index < 0)
        ||(judge_info_ptr->number_index > (uint16)(MMIPB_LISTBOX_INDEX_NUMBER_OTHER - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE))||/*lint !e64 !e656 !e72*/
        (0 == judge_info_ptr->valid_contact_count))
    {
        result = FALSE;
    }
    else if((PNULL !=judge_info_ptr->contact_ptr) 
        &&  (MMIPB_GetFirstValidNumberIndex(judge_info_ptr->contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM)>=MMIPB_MAX_NV_PHONE_NUM))
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif

   return result;
}
/*****************************************************************************/
//  Description :audio dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByAudioMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
    
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else if(0==  judge_info_ptr->valid_contact_count)
    {
        result = FALSE;
    }   
    else if((PNULL !=judge_info_ptr->contact_ptr) 
        &&  (MMIPB_GetFirstValidNumberIndex(judge_info_ptr->contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM)>=MMIPB_MAX_NV_PHONE_NUM))
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif
          
   return result;
}
#ifdef VT_SUPPORT
/*****************************************************************************/
//  Description :video dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByVideoMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
    
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else if(0== judge_info_ptr->valid_contact_count)
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif
           
   return result;
}
#endif
/*****************************************************************************/
//  Description :ip dial menu whether enable
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DialByIpMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
     BOOLEAN result = TRUE;
    
    if(PNULL == judge_info_ptr)
    {
        result = FALSE;
    }
    else if(0== judge_info_ptr->valid_contact_count)
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif

   return result;
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : vcard parent menu whether enable
//  Global resource dependence : none
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ImportExportMenuEnable(MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
									 PB_OPTMENU_NODE_E menu_node)
{
	BOOLEAN result = TRUE;

	if(PNULL == judge_info_ptr)
	{
		return FALSE;
	}

	if ((0 == judge_info_ptr->valid_contact_count) && (MMIAPIUDISK_UdiskIsRun() || (MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidDevice())))
	{
		result = FALSE;	
	}
	return result;
}
/*****************************************************************************/
//  Description : vcard menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcardMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    
    if((PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT == menu_node)
       &&(0 == judge_info_ptr->valid_contact_count)
     )
    {
        result = FALSE;
    }

    if(MMIAPIUDISK_UdiskIsRun()|| MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidDevice())
    {
        result = FALSE;
    }
    
    return result;
}   
#endif

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description :vcard send by bt menu whether enable
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    if(PNULL == judge_info_ptr)
    {
        SCI_TRACE_LOW("[MMIPB] SendVcardEnable PNULL == judge_info_ptr");
            return FALSE;
    }

    if(TRUE == MMIAPISET_IsDisableShareViaOption())
    {
        result = FALSE;
    }

	//bug 103822: FDN没有send vcard功能，只有电话本中联系人有send vcard功能
#ifdef FDN_SUPPORT
    if(MMIPB_IsFdnContact(judge_info_ptr->contact_ptr->storage_id))
    {
        //return FALSE;
    }
#endif
    
    if(PB_OPTMENU_NODE_SEND_BY_MMS_MENU == menu_node)
    {
        #if !defined(MMIMMS_SMS_IN_1_SUPPORT) && !defined(MMS_SUPPORT)
        return FALSE;
        #endif
    }
//#ifdef MMIEMAIL_SUPPORT
    else if(PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU == menu_node)
    {   
        return  TRUE;
    }
//#endif
    else if(PB_OPTMENU_NODE_SEND_BY_BT_MENU == menu_node)
    {
        #ifndef BLUETOOTH_SUPPORT    
        return  FALSE;
        #endif
    }

    if(0== judge_info_ptr->valid_contact_count)
    {
        result = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(TRUE == judge_info_ptr->not_support_single_operate)
    {
        result = FALSE;
    }
#endif
#ifdef MMIPB_MOST_USED_SUPPORT    
    else if(MMIPB_IsMostUsedContact(judge_info_ptr->contact_ptr->storage_id))
    {
       return FALSE;
    }
#endif     

    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendMultiVcardEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    return result;
}
#endif


/*****************************************************************************/
//  Description :copy one contact or more than one contact
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E CopyContact(
    uint16 to_storage_id,
    MMIPB_MENU_DATA_INFOR_T *data_ptr
    )
{
    MMIPB_ERROR_E res = MMIPB_ERROR_ERROR;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    
    if(PNULL == data_ptr||PNULL == data_ptr->contact_ptr)
    {
        return res;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(data_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return res;
    }
    applet_ptr->instance.operate_type = data_ptr->operate_type;
    if(MMIPB_IsMark(data_ptr->applet_handle))
    {
        MMIPB_AbortPB(FALSE);
        res = MMIPB_CopyMultiContactAsyn(applet_ptr->instance.handle, to_storage_id);
    }
    else
    {
        MMIPB_AbortPB(FALSE);
        res = MMIPB_CopyOneContactAsyn(applet_ptr->instance.handle, 
            to_storage_id,
            data_ptr->contact_ptr->contact_id,
            data_ptr->contact_ptr->storage_id,
            MMIPB_FEILD_FLAG_MASK_ALL
            );
    }
    if(res == MMIPB_ERROR_PENDING)
    {
        MMIPB_OpenProcessWin(data_ptr->applet_handle, TXT_PB_COPY_OR_NOT, TXT_PB_COPY_CONTACT);
    }
    else 
    {
        MMIPB_OpenAlertTipWin(data_ptr->applet_handle, res, MMIPB_OPC_COPY);
        MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
    }
    
    return res;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ClearDateEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
	)
{
	if(MMIPB_CONTACT_ADD_BIRTHDAY_ID == MMK_GetActiveCtrlId(judge_info_ptr->parent_win))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendMessageMenuEnable(
                                  MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
                                  PB_OPTMENU_NODE_E menu_node
                                  )
{
    if(MMIAPISET_IsOrthodoxVariant())
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SendEmailMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
#ifdef MMI_ORIG_UNISOC_SUPPORT
	if(MMIPB_IsChinaVersion())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DebugEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = MMIAPIPB_IsDisplayDebugMenu();
    #ifdef WIN32
    result = TRUE;
    #endif
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DebugSimManagerEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }

    if(menu_node >= PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER && menu_node < PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER + MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1 + menu_node- PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER;
        if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            result =  TRUE;
        }
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Sim1MenuEnable
(
 MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
 PB_OPTMENU_NODE_E menu_node
 )
{
	BOOLEAN result = FALSE;
	if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
	{
		result =  TRUE;
	}
	else
	{
		result = FALSE;
	}
	return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Sim2MenuEnable
(
 MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
 PB_OPTMENU_NODE_E menu_node
 )
{
	BOOLEAN result = FALSE;
	
	if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
	{
		result =  TRUE;
	}
	else
	{
		result = FALSE;
	}
	return result;
}

/*****************************************************************************/
//  Description : copy (parent) menu whether should disable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if(PNULL ==judge_info_ptr)
    {
        return FALSE;
    }  
    if(judge_info_ptr->contact_ptr != PNULL)
    {
#ifdef MMIPB_MOST_USED_SUPPORT    
        if(MMIPB_IsMostUsedContact(judge_info_ptr->contact_ptr->storage_id))
        {
           return FALSE;
        }
#endif
#ifdef MMIPB_SIMDN_SUPPORT 
        if(MMIPB_IsMsisdnContact(judge_info_ptr->contact_ptr->storage_id))
        {
            return FALSE;
        }
        if(MMIPB_IsSDNContact(judge_info_ptr->contact_ptr->storage_id))
        {
             return FALSE;
        }
#endif        
#ifdef FDN_SUPPORT        
        if(MMIPB_IsFdnContact(judge_info_ptr->contact_ptr->storage_id))
        {
            return FALSE;
        }
#endif        
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_MOSTUSED_OPT_MENU != judge_info_ptr->menu_type)
    {
         result = TRUE;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description : copy to nv menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyOneToNVEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    
    MMIPB_APPLET_T *applet_ptr = PNULL;
    
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }

    if(MMIPB_IsPhoneContact(applet_ptr->instance.cur_storage_id))
    {
        result = FALSE;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : copy one to sim menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyOneToSIMEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }

    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
    if(menu_node >= PB_OPTMENU_NODE_ONE_COPYTOSIM1 && menu_node < PB_OPTMENU_NODE_ONE_COPYTOSIM1 + MMI_DUAL_SYS_MAX)
    {
        if ((PB_OPTMENU_NODE_ONE_COPYTOSIM1 == menu_node) && (MMIPB_STORAGE_SIM1 == MMIPB_GET_STORAGE(judge_info_ptr->contact_ptr->storage_id)))
        {
            result = FALSE;
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else if ((PB_OPTMENU_NODE_ONE_COPYTOSIM2 == menu_node) && (MMIPB_STORAGE_SIM2 == MMIPB_GET_STORAGE(judge_info_ptr->contact_ptr->storage_id)))
        {
            result = FALSE;
        }
#if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
        else if ((PB_OPTMENU_NODE_ONE_COPYTOSIM3 == menu_node) && (MMIPB_STORAGE_SIM3 == MMIPB_GET_STORAGE(judge_info_ptr->contact_ptr->storage_id)))
        {
            result = FALSE;
        }
#ifdef MMI_MULTI_SIM_SYS_QUAD
        else if ((PB_OPTMENU_NODE_ONE_COPYTOSIM4 == menu_node) && (MMIPB_STORAGE_SIM4 == MMIPB_GET_STORAGE(judge_info_ptr->contact_ptr->storage_id)))
        {
            result = FALSE;
        }
#endif
#endif
#endif
        else
        {
            dual_sys = MN_DUAL_SYS_1 + menu_node- PB_OPTMENU_NODE_ONE_COPYTOSIM1;
            if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
            {
                result =  TRUE;
            }
        }

    }
    else
    {
        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : copy to sim menu whether should disable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CopyToSIMEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = FALSE;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if(PNULL == judge_info_ptr)
    {
        return FALSE;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(judge_info_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return FALSE;
    }

    if(menu_node >= PB_OPTMENU_NODE_COPYTOSIM1 && menu_node < PB_OPTMENU_NODE_COPYTOSIM1 + MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1 + menu_node- PB_OPTMENU_NODE_COPYTOSIM1;
        if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            result =  TRUE;
        }
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : contact view menu whether enable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContactViewMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
    uint16 entry_id=0;
    uint16 storage_id=0;  
    uint16 number_num = 0;
    uint16 number_index[MMIPB_MAX_NV_PHONE_NUM] = {0};
    MMIPB_APPLET_T* applet_ptr = MMK_GetAppletPtr(judge_info_ptr->applet_handle);/*lint !e64*/
    if(PNULL != applet_ptr )
    {
        
        if(MMIPB_MOSTUSED_CONTACTS == applet_ptr->instance.contacts_type)
        {            
            number_num = MMIPB_GetContactNumberNum(
                judge_info_ptr->contact_ptr, number_index);
            
            if(!MMIPB_SearchPhoneNumber(
                &judge_info_ptr->contact_ptr->number[number_index[0]], 
                &entry_id,&storage_id,PNULL))
            {
                result = FALSE;
            }
        }
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
//  Description : add to contact menu whether enable
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddToContactMenuEnable(
    MMIPB_MENU_DATA_INFOR_T *judge_info_ptr,
    PB_OPTMENU_NODE_E menu_node
    )
{
    BOOLEAN result = TRUE;
	uint16 entry_id=0;
    uint16 storage_id=0;  
    uint16 number_num = 0;
    uint16 number_index[MMIPB_MAX_NV_PHONE_NUM] = {0};

    if(PNULL == judge_info_ptr||PNULL == judge_info_ptr->contact_ptr)
    {
        return FALSE;
    }

    number_num = MMIPB_GetContactNumberNum(
        judge_info_ptr->contact_ptr, number_index);
   
    if(MMIPB_SearchPhoneNumber(
        &judge_info_ptr->contact_ptr->number[number_index[0]], 
        &entry_id,&storage_id,PNULL))
    {
        result = FALSE;
    }
    return result;
}
/*****************************************************************************/
//  Description :get menu group
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_MENU_FAMILY_INFO_T* GetMenuArrAddress(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr)
{
    MMIPB_MENU_FAMILY_INFO_T *address = PNULL;

    if(PNULL == menu_infor_ptr)
    {
        return (MMIPB_MENU_FAMILY_INFO_T *)s_pb_main_opt_item;
    }
    switch(menu_infor_ptr->menu_type)
    {
        case MMIPB_MAIN_OPT_MENU:  
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_main_opt_item;
  #ifndef MMI_PDA_SUPPORT
            if(0 == menu_infor_ptr->valid_contact_count)
            {
                address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_main_empty_opt_item;
            }
  #endif
            break;
#ifdef MMIPB_MOST_USED_SUPPORT
             
        case MMIPB_MOSTUSED_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_mostused_opt_item;
            break; 
#endif            
                
  #ifdef MMI_PDA_SUPPORT
        case MMIPB_MAIN_LONGTP_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_main_longtp_opt_item;
            break;

        case MMIPB_DETAIL_NUMBER_ITEM_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_detail_number_item_opt_item;
                break;
        case MMIPB_DETAIL_MSGLOG_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_detail_msglog_opt_item;
                break;  
#ifdef MMIPB_GROUP_MANAGE_SUPPORT            
          case MMIPB_GROUPLIST_LONGTP_MENU:
             address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_grouplist_longtp_opt_item;
            break;

        case MMIPB_GROUP_ENTRY_LONGTP_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_group_entry_longtp_opt_item;
            break;
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
       case MMIPB_FAVORITE_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_favorite_opt_item;
            break;
        case MMIPB_FAVORITE_LONGTP_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_favorite_longtp_opt_item;
            break; 
            
        case MMIPB_MOSTUSED_LONGTP_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_mostused_longtp_opt_item;
            break; 
#endif
#else
    case MMIPB_OPERATE_SEL_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_operate_sel_opt_item;
            break;
	case MMIPB_OPERATE_SEL_ALL_OPT_MENU:
		address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_operate_sel_all_opt_item;
	      	break;
#endif   
#if defined(MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
        case MMIPB_SIM_DN_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_fdn_opt_item;
            break;
        case MMIPB_SIM_DN_LONGTP_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_fdn_longtp_opt_item;
            break;
#endif
        case MMIPB_DETAIL_BASIC_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_detail_basic_opt_item;
            break;
        case MMIPB_DETAIL_VCARD_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_detail_vcard_opt_item;
            break;
         case MMIPB_DIAL_SUB_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_dial_sub_item;
            break;   
        case MMIPB_MSG_SUB_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_writemsg_sub_item;
            break;
#ifdef MMI_VCARD_SUPPORT
        case MMIPB_SENDVCARD_SUB_MENU:
           address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_vcardsend_sub_item;
            break;
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        case MMIPB_GROUPLIST_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_grouplist_opt_item;
            break;
        case MMIPB_GROUPLIST_EMPTY_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_grouplist_empty_opt_item;
            break;
#endif
    case MMIPB_GROUP_ENTRY_OPT_MENU:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_group_entry_opt_item;
            break;

    case MMIPB_ADD_SUB_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_add_sub_opt_item;
            break;
     case MMIPB_RING_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_ring_opt_item;
            break; 
    case MMIPB_PHOTO_OPT_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_photo_opt_item;
            break;
    case MMIPB_ADDEDIT_OPT_MENU: 
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_addedit_opt_menu_item;
            break;
    case MMIPB_ADDEDIT_PREFERRED_SIM_MENU: 
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_addedit_preferred_sim_menu_item;
            break;
    case MMIPB_MULTI_SHARE_BY_MENU: 
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_multi_share_item;
            break;  
    case MMIPB_COPY_FROM_MENU: 
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_copy_from_item;
            break;  
    case MMIPB_COPY_FROM_PHONE_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_copy_from_phone_item;
            break;  
    case MMIPB_COPY_FROM_SIM1_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_copy_from_sim1_item;
            break;  
    case MMIPB_COPY_FROM_SIM2_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_copy_from_sim2_item;
            break;  
    case MMIPB_COPY_FROM_ANOTHER_DEVICE_MENU:
        address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_copy_from_other_device_item;
            break;  

       default:
            address = (MMIPB_MENU_FAMILY_INFO_T *)s_pb_main_opt_item;
            break;
    }
   return address;
}

#if 0
/*****************************************************************************/
//  Description :get phone type and number string
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPhoneNumberString(uint16 num_index,
    wchar *wstr_ptr,
    MMIPB_CONTACT_T *contact_ptr)
{
    uint16 len = 0;
    if(PNULL == contact_ptr||PNULL == wstr_ptr)
    {
        return 0;
    }
    if(num_index>=MMIPB_MAX_NV_PHONE_NUM)
    {
        len = contact_ptr->mail.wstr_len;
        MMIAPICOM_Wstrncpy(wstr_ptr,contact_ptr->mail.wstr,len);
    }
    else
    {
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[num_index].npi_ton, 
            contact_ptr->number[num_index].number_len)
            )
        {
            uint16 tele_len = 0;
            MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
            uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            wchar tele_wstr[MMIPB_NUMBER_MAX_STRING_LEN+2];
            
            SCI_MEMSET(tele_num,0,sizeof(tele_num));
            SCI_MEMSET(tele_wstr,0,sizeof(tele_wstr));
               
            number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[num_index].npi_ton);
    
    // 将号码转换为字符串
            tele_len = MMIAPICOM_GenDispNumber(
                                        (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton),
                                        contact_ptr->number[num_index].number_len,
                                        contact_ptr->number[num_index].number,
                                        tele_num,
                                        MMIPB_NUMBER_MAX_STRING_LEN+2
                                        );
            if(tele_len>0)
            {
                MMIAPICOM_StrToWstr(tele_num,wstr_ptr);

                len =tele_len;
            }     
        }
    }
    return len;
}
#endif

/*****************************************************************************/
//  Description :get index from s_pb_menu by node id
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetArrayIndexByNodeId(uint16 node_id)
{
    uint16 i = 0;
    uint16 arr_num = sizeof(s_pb_menu)/sizeof(s_pb_menu[0]);

    for(i = 0;i<arr_num;i++)
    {
        if(node_id == s_pb_menu[i].node_id)
        {
            break;
        }
    }
    if(i>=arr_num)
    {
        i = 0;
    }
    return i;
}

/*****************************************************************************/
//  Description :创建win table的选项菜单
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePbOptMenu(MMI_WIN_ID_T win_id,
    MMI_HANDLE_T ctrl_id,
    MMIPB_MENU_DATA_INFOR_T *judge_infor_ptr,
    uint16 *menu_num_ptr
)
{
    GUIMENU_STYLE_E menu_style = GUIMENU_STYLE_POPUP_AUTO;
    
   MMIPB_MENU_FAMILY_INFO_T *arrAddress = PNULL;
   MMI_STRING_T  kstring = {0};
   GUIMENU_DYNA_ITEM_T node_item = {0};
   uint16 index = 0;
   uint16        node_id = 0;
   uint16        parent_node = 0;
   uint16        node_index = 0;
   
  // wchar     number_str[MMIPB_NUMBER_MAX_STRING_LEN] = {0};

    if(PNULL == judge_infor_ptr)
    {
        return;
    }
   menu_style = GetMenuStyle(judge_infor_ptr->menu_type);
   arrAddress = GetMenuArrAddress(judge_infor_ptr);

    while(PNULL !=arrAddress&&(0xFFFF != arrAddress->node_id))
    {
        node_id = arrAddress->node_id;
        index = GetArrayIndexByNodeId(node_id);
        if(PNULL == s_pb_menu[index].enable_fun||
            s_pb_menu[index].enable_fun(judge_infor_ptr, (PB_OPTMENU_NODE_E)node_id))
        {
            if(parent_node!= arrAddress->parent_node)
            {
                node_index= 0;
            }
            parent_node = arrAddress->parent_node;
            MMI_GetLabelTextByLang(s_pb_menu[index].text_id, &kstring);
          
            node_item.item_text_ptr = &kstring;
            
            GUIMENU_InsertNode(node_index, s_pb_menu[index].node_id, 
                        parent_node, &node_item, ctrl_id);
            //  GUIMENU_SetNodeGrayed((!is_enable), nodex_index, 0,  ctrl_id);
            
            node_index ++;

            (*menu_num_ptr)++;
        }
        arrAddress++;
    }
    if(MMIPB_GROUPLIST_LONGTP_MENU == judge_infor_ptr->menu_type)
    {
        uint8 group_id = 0;
        MMIPB_NAME_T group_name = {0};
        MMIPB_GetAllGroupIDs(judge_infor_ptr->group, &group_id, 1);
        MMIPB_ReadGroupName(&group_name, group_id);

        kstring.wstr_len = group_name.wstr_len;/*lint !e733*/
        kstring.wstr_ptr = group_name.wstr;/*lint !e733*/
        GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    }
    else if(MMIPB_MSG_SUB_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_WRITE_MESSAGE,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }

#ifdef MMI_VCARD_SUPPORT
    else if(MMIPB_SENDVCARD_SUB_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_PB_SEND_VCARD,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
#endif
    else if(MMIPB_DIAL_SUB_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_CALL,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
    else if(MMIPB_PHOTO_OPT_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_PB_CONTACT_PHOTO,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
    else if(MMIPB_RING_OPT_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_RING,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
    else if(MMIPB_ADD_SUB_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_ADD_TO_CONTACT,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
    else if(MMIPB_MAIN_LONGTP_MENU == judge_infor_ptr->menu_type)
    {
        if(0 != judge_infor_ptr->contact_ptr->name.wstr_len)
        {
            kstring.wstr_ptr = judge_infor_ptr->contact_ptr->name.wstr;
            kstring.wstr_len = judge_infor_ptr->contact_ptr->name.wstr_len;
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NO_NAME,&kstring);
        }
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(MMIPB_MOSTUSED_LONGTP_OPT_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(TXT_PB_MOSTUSED_GROUP,&kstring);
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(MMIPB_FAVORITE_LONGTP_OPT_MENU == judge_infor_ptr->menu_type)
    {
        if(0 != judge_infor_ptr->contact_ptr->name.wstr_len)
        {
            kstring.wstr_ptr = judge_infor_ptr->contact_ptr->name.wstr;
            kstring.wstr_len = judge_infor_ptr->contact_ptr->name.wstr_len;
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NO_NAME,&kstring);
        }
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
#endif
    else if(MMIPB_GROUP_ENTRY_LONGTP_MENU == judge_infor_ptr->menu_type)
    {        
        if(0 != judge_infor_ptr->contact_ptr->name.wstr_len)
        {
            kstring.wstr_ptr = judge_infor_ptr->contact_ptr->name.wstr;
            kstring.wstr_len = judge_infor_ptr->contact_ptr->name.wstr_len;
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NO_NAME,&kstring);
        }      
        GUIMENU_SetMenuTitle(&kstring, ctrl_id);
    }
#if 0
    else if(GUIMENU_STYLE_POPUP_AUTO ==menu_style&&(PNULL!=judge_infor_ptr->contact_ptr))
    {
        if(judge_infor_ptr->contact_ptr->name.wstr_len>0)
        {
            kstring.wstr_len = judge_infor_ptr->contact_ptr->name.wstr_len;
            kstring.wstr_ptr = judge_infor_ptr->contact_ptr->name.wstr;
            GUIMENU_SetMenuTitle(&kstring,ctrl_id);
        }
        else
        {
            uint16 number_num = 0;
            uint16 number_index[MMIPB_MAX_NV_PHONE_NUM]={0};
            uint16 num_len = 0;
            uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            wchar wstr_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            
            number_num = MMIPB_GetContactNumberNum(judge_infor_ptr->contact_ptr,
                number_index);
            if(number_num>0)
            {
                num_len = MMIPB_GetPhoneNumberByBcdNumber(
                    judge_infor_ptr->contact_ptr->number[number_index[0]],
                    tele_num,
                    MMIPB_NUMBER_MAX_STRING_LEN+2
                    );
                MMIAPICOM_StrToWstr(tele_num,wstr_num);
                kstring.wstr_len = num_len;/*lint !e733*/
                kstring.wstr_ptr = wstr_num;/*lint !e733*/
                GUIMENU_SetMenuTitle(&kstring,ctrl_id);
            }
        }
    }
#endif
#if defined (MAINLCD_SIZE_128X160)
    else if(MMIPB_MAIN_OPT_MENU == judge_infor_ptr->menu_type)
    {
        MMI_GetLabelTextByLang(STXT_SOFTKEY_MENU_MK, &kstring);
        GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    }
#else
    else if(MMIPB_MAIN_OPT_MENU == judge_infor_ptr->menu_type)
    {
		GUIMENU_TITLE_INFO_T      title_info={0};
		MMI_GetLabelTextByLang(STR_PRMPT_COPY_FROM,&kstring);
		title_info.is_static=FALSE;;
		title_info.node_id=PB_OPTMENU_NODE_COPY;;
		title_info.title_ptr=&kstring;
		GUIMENU_SetMenuTitleEx(ctrl_id,&title_info);
    }
#endif

    
 //   if(GUIMENU_STYLE_POPUP_AUTO ==menu_style)
    {
        GUIMENU_BUTTON_INFO_T button_infor = {0};
        button_infor.is_static = FALSE;
		button_infor.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_infor);
    #ifdef MMI_VCARD_SUPPORT		
        button_infor.node_id = PB_OPTMENU_NODE_SEND_VCARD_MENU;
		button_infor.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_infor);
    #endif
        button_infor.node_id = PB_OPTMENU_NODE_IMPORT_EXPORT;
        button_infor.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_infor);
    }
    
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : Call back function for adding file to play list.
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void LoadVcardCallBack(
                    BOOLEAN     is_success,
                    FILEARRAY   file_array
                    )
{

    MMI_HANDLE_T applet_handle = 0;
    MMIPB_APPLET_T * applet_ptr = PNULL;
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
  
    MMIPB_GetCurAppletHandle(&applet_handle);
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(applet_handle);
    if(PNULL!=applet_ptr)
    {        
        if(is_success)
        {
            uint32 file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
            if(file_num>0)
            {
                if(MMIPB_SaveSelectFileInfo(applet_ptr->instance.handle, file_array))
                {          
                    MMIPB_AbortPB(FALSE);
                    ret = MMIPB_ImportContactAsynForPB(applet_ptr->instance.handle);
                    if(ret == MMIPB_ERROR_PENDING)
                    {
                        MMIPB_OpenProcessWin(applet_handle,TXT_PB_IS_IMPORTING, TXT_PB_IMPORT_CONTACT);
                    } 
                }         
            }
            else
            {
                MMIPB_DisplaybannerInfo(0, TXT_PB_FOLDER_EMPTY, PNULL);
                //MMIPB_OpenCommonDialogWin(applet_handle, TXT_PB_FOLDER_EMPTY);
                //MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,TXT_PB_FOLDER_EMPTY,
                    //TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0);
            }            
        }
    }
    if(ret != MMIPB_ERROR_PENDING)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
    }
}
#endif

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 进入文件系统，导入VCard到电话本
//  Global resource dependence : none
//  Author: jesse.yu
//  Note: 
/***************************************************************************/
LOCAL void MMIPB_LoadVCardFromDir(wchar* path_name,uint16 path_len)
{
	FILEARRAY  vcard_array = PNULL;
	MMIFMM_FILTER_T filter = {{"*.vcf"}};
	
	vcard_array = MMIAPIFILEARRAY_Create();
    
	if(PNULL == vcard_array)
	{
	    return;
	}
	if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(
		path_name,
		path_len,
		&filter,
		FUNC_FIND_FILE_AND_ALLFOLER,
		TRUE,
		PNULL,
		vcard_array,
		LoadVcardCallBack,
		FALSE
		))
    {
        MMIAPIFILEARRAY_Destroy(&vcard_array);
        vcard_array = PNULL;
    }	  
}
#endif

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 导入指定文件的VCard到电话本
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
PUBLIC void MMIPB_LoadVCardFile(wchar *full_name,uint16 name_len)
{
    FILEARRAY_DATA_T*data_ptr=PNULL;
    FILEARRAY  vcard_array = PNULL;
    
    vcard_array = MMIAPIFILEARRAY_Create();
    if(PNULL == vcard_array)
    {
        return;
    }
    
    data_ptr=(FILEARRAY_DATA_T*)SCI_ALLOC_APP(sizeof(FILEARRAY_DATA_T));
    if(PNULL==data_ptr )
    {
        MMIAPIFILEARRAY_Destroy(&vcard_array);
        return;
    }
    SCI_MEMSET(data_ptr,0,sizeof(FILEARRAY_DATA_T));
	data_ptr->name_len=name_len;
    MMIAPICOM_Wstrncpy(data_ptr->filename,full_name,data_ptr->name_len);
    data_ptr->type=FILEARRAY_TYPE_FILE;
    MMIAPIFILEARRAY_Add(vcard_array,data_ptr);
    SCI_FREE(data_ptr);
    LoadVcardCallBack(TRUE,vcard_array);
}
#endif
/*****************************************************************************/
//  Discription:after query,execute copy
//  Global resource dependence: None
//  Author: xiaohua.liu
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCopyToSimWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)                
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = PNULL;

     add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
     if(PNULL == add_data_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    menu_infor_ptr= (MMIPB_MENU_DATA_INFOR_T *)add_data_ptr->user_data;
    if(PNULL==menu_infor_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
            
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinTitleTextId(win_id,TXT_PB_COPY_CONTACT,TRUE);
        break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        menu_infor_ptr->group = (1<<PB_GROUP_ALL);
        menu_infor_ptr->mask_group |= MMIPB_GetAllSIMGroup();
        MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        if(PNULL != applet_ptr)
        {
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        } 
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(add_data_ptr->user_data != PNULL)
        {
            SCI_FREE(add_data_ptr->user_data);/*lint !e64*/
        }
        MMK_FreeWinAddData(win_id);
        break;
           default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
	return result;
}


/*****************************************************************************/
//  Discription: 提示询问信息，让用户选择是否要进行删除操作
//  Global resource dependence: None
//  Author: xiaohua.liu
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteQueryWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)                
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    uint8 group_id = 0;
#endif
     add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
     if(PNULL==add_data_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    menu_infor_ptr= (MMIPB_MENU_DATA_INFOR_T *)add_data_ptr->user_data;
    if(PNULL==menu_infor_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
            
    switch(msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        if(MMIPB_OPERATE_TYPE_DELETE_GROUP== menu_infor_ptr->operate_type)
        {
            applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
            if(PNULL == applet_ptr)
            {
                result = MMI_RESULT_FALSE;
                break;
            }
            applet_ptr->instance.operate_type = menu_infor_ptr->operate_type;
            MMIPB_GetAllGroupIDs(menu_infor_ptr->group, &group_id, 1);
            MMIPB_DeleteUserGroup(menu_infor_ptr->applet_handle, group_id);
            MMIPB_UpdateCurrentList(menu_infor_ptr->applet_handle, MMIPB_OPC_MANAGE_GROUP);
        }
        else
#endif
        {
            applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
            if(PNULL == applet_ptr)
            {
                result = MMI_RESULT_FALSE;
                break;
            }
            if(menu_infor_ptr->is_need_pin_verify)
            {               
#if defined(FDN_SUPPORT)                   
                //PIN2 verify                
                MMIAPIPHONE_SetCurHandlingPinSimID(menu_infor_ptr->dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMIPB_GetDailNumberListWinTab(),
                    TRUE); 
                applet_ptr->instance.cmd = MMIPB_OPC_DELETE;
#endif                 
            }
            else
            {
                applet_ptr->instance.operate_type = menu_infor_ptr->operate_type;
                MMIPB_DeleteContactWin(menu_infor_ptr->applet_handle,applet_ptr->instance.handle);
            }
            if(MMIPB_DETAIL_BASIC_OPT_MENU== menu_infor_ptr->menu_type)
            {
             MMK_CloseWin(menu_infor_ptr->parent_win);
            }
        }        
        MMK_CloseWin(win_id);
         break;
            
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        if(PNULL != applet_ptr)
        {
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            if(MMIPB_OPERATE_TYPE_DELETE_SIM1_CONTACTS==menu_infor_ptr->operate_type
                ||MMIPB_OPERATE_TYPE_DELETE_SIM2_CONTACTS==menu_infor_ptr->operate_type
                )
            {
                MMK_CloseWin(MMK_GetWinHandle(applet_ptr->instance.handle, MMIPB_OPERATE_SEL_LIST_WIN_ID));
            }
        }        
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(PNULL != add_data_ptr->user_data)
        {
            SCI_FREE(add_data_ptr->user_data);/*lint !e64*/
        }
        MMK_FreeWinAddData(win_id);
        break;
           default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
	return result;
}

/*****************************************************************************/
//  Description : 删除号码等弹出询问窗口
//  Global resource dependence : g_mmipb_operate_type
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void OpenPbQueryWin(MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
    //MMI_TEXT_ID_T title_id = 0;
    MMI_STRING_T   text_str = {0};
    MMI_STRING_T   text_ptr_1 = {0};
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPUB_HANDLE_FUNC pub_handle_fun = PNULL;
    MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID; 
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
    MMIPB_NAME_T group_name = {0};
#endif
    wchar result_wchar[PB_UI_GENERATE_STR_MAX_LEN] = {0};
	
    if(PNULL == data_ptr)
    {
        return;
    }
    menu_infor_ptr = SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_infor_ptr)
    {
        return;
    }
    SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));        

#ifdef HERO_ENGINE_TTS_SUPPORT
{
	extern uint8 s_hero_no_tts;
	s_hero_no_tts = 1;
}
#endif
    // 确定电话簿的操作类型    
    if(MMIPB_OPERATE_TYPE_ERASE_ONE == data_ptr->operate_type
        ||MMIPB_OPERATE_TYPE_ERASE_MULTI==data_ptr->operate_type
        ||MMIPB_OPERATE_TYPE_DELETE_SIM1_CONTACTS==data_ptr->operate_type
        ||MMIPB_OPERATE_TYPE_DELETE_SIM2_CONTACTS==data_ptr->operate_type
        )
    {
		if(MMIPB_OPERATE_TYPE_ERASE_ONE == data_ptr->operate_type)
		{
			MMIRES_GetText(STR_DELETE_CONTACT_EXT01,0,&text_str);
		}
		else if(MMIPB_OPERATE_TYPE_ERASE_MULTI == data_ptr->operate_type)
		{
			MMIPB_APPLET_T *applet_ptr = PNULL;
			applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);
			if(PNULL != applet_ptr && PNULL != applet_ptr->instance.select_list)
			{
				if(1 == applet_ptr->instance.select_list->sel_count)
				{
					MMIRES_GetText(STR_DELETE_SEVERAL_CONTACT_EXT01,0,&text_str);
				}
				else
				{
					text_str.wstr_ptr = result_wchar;
					MMIAPICOM_CustomGenerateNumReplaceStrByTextId(STR_DELETE_SEVERAL_CONTACTS_EXT01, 
						L"%N", 
						applet_ptr->instance.select_list->sel_count,
						PB_UI_GENERATE_STR_MAX_LEN, 
						&text_str);
				}
			}
			else
			{
				MMIRES_GetText(TXT_DELETE_QUERY,0,&text_str);
			}
			
		}
		else
		{
			MMIRES_GetText(TXT_DELETE_QUERY,0,&text_str);
		}
        pub_handle_fun = HandleDeleteQueryWinMsg;
    }
    else if(MMIPB_OPERATE_TYPE_COPY_TO_SIM1 == data_ptr->operate_type
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        ||MMIPB_OPERATE_TYPE_COPY_TO_SIM2 == data_ptr->operate_type
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        ||MMIPB_OPERATE_TYPE_COPY_TO_SIM3 == data_ptr->operate_type
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ||MMIPB_OPERATE_TYPE_COPY_TO_SIM4 == data_ptr->operate_type
#endif
#endif
#endif
	)
    {
        MMIRES_GetText(TXT_PB_INFORMATION_LOST,0,&text_str);
        pub_handle_fun = HandleCopyToSimWinMsg;
    }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT    
    else if(MMIPB_OPERATE_TYPE_DELETE_GROUP== data_ptr->operate_type)
    {
        uint8 group_id = 0;
        MMIRES_GetText(STR_CONF_GROUP_DELETE_EXT01,0,&text_str);
        pub_handle_fun = HandleDeleteQueryWinMsg;
        
        //MMIPB_GetAllGroupIDs(menu_infor_ptr->group, &group_id, 1);              
        //MMIPB_ReadGroupName(&group_name,group_id);        
        //text_ptr_1.wstr_ptr = group_name.wstr;/*lint !e733*/
        //text_ptr_1.wstr_len = group_name.wstr_len;
    }
    else if(MMIPB_OPERATE_TYPE_REMOVE_FROM_GROUP == data_ptr->operate_type)
    {
        MMIRES_GetText(TXT_PB_REMOVE_FROM_MEMBER,0,&text_str);
        pub_handle_fun = HandleRemoveFromGroupQueryWinMsg;
    } 
#endif
#ifdef MMIPB_MOST_USED_SUPPORT    
    else if(MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE == data_ptr->operate_type)
    {
        MMIRES_GetText(TXT_PB_FAVOURITE_CONTACT_REMOVED,0,&text_str);
        pub_handle_fun = HandleRemoveFavoriteQueryWinMsg;
    }   
    else if(MMIPB_OPERATE_TYPE_CLEAR_MOSTUSED == data_ptr->operate_type)
    {
        MMIRES_GetText(TXT_PB_DELETE_ALL_OR_NOT,0,&text_str);
        pub_handle_fun = HandleClearMostUsedQueryWinMsg;
    }   
#endif

#if 0
    MMIPUB_OpenQueryWinByTextPtrEx(data_ptr->applet_handle,
        &text_str,&text_ptr_1,IMAGE_PUBWIN_QUERY,&alert_id,pub_handle_fun,
        (uint32)menu_infor_ptr);
#else
    MMIPUB_OpenConfirmationDialogWinByTextPtrEx(
                                           data_ptr->applet_handle,
                                           PNULL,   //time period,default is 3s
                                           &text_str,         //text 1
                                           &text_ptr_1,         //text 2
                                           IMAGE_PUBWIN_QUERY,           //alert image id
                                           &alert_id,        //win id,default is MMIPUB_ALERT_WIN_ID
                                           PNULL,  //win priority,default is WIN_ONE_LEVEL
                                           MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,      //softkey style
                                           pub_handle_fun,        //handle function
                                           (uint32)menu_infor_ptr           //user_data
                                           );
#endif

    if(pub_handle_fun == PNULL)
    {
        SCI_FREE(menu_infor_ptr);
    }
}

/*****************************************************************************/
//  Description : enter second level menu
//  Global resource dependence : g_mmipb_operate_type
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_EnterSecondMenu(PB_OPTMENU_NODE_E node_id,
    MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
   
   MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
   MMIPB_CONTACT_T *contact_ptr = PNULL;
   if(PNULL == data_ptr)
    {
        return FALSE;
    }
    menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_infor_ptr)
    {
        return FALSE;
    }
    SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        SCI_FREE(menu_infor_ptr);
        return FALSE;
    }

    if(PNULL!=data_ptr->contact_ptr)
    {
        SCI_MEMCPY(contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
    }
    else
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    }
    menu_infor_ptr->contact_ptr = contact_ptr;
    
    switch(node_id)
    {
      
        case PB_OPTMENU_NODE_ADD_TO_CONTACT: 
            menu_infor_ptr->menu_type = MMIPB_ADD_SUB_MENU;
        #ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            MMK_CreateWinByApplet(data_ptr->applet_handle,
            (uint32 *)MMIPB_ADD_SUB_WIN_TAB,
            (MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);
        #else
            MMK_CreateWin(
            (uint32 *)MMIPB_ADD_SUB_WIN_TAB,
            (MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);
        #endif
            break;
         
        case PB_OPTMENU_ADD_TO_GROUP:
          menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_ADD_TO_GROUP;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT //ping.gao modify for non-pda compile error

          MMK_CreatePubwinTabEx(menu_infor_ptr->applet_handle,SPRD_GUI_LIST_ID,
            (uint32*)MMIPB_ADD_TO_GROUP_WIN_TAB, menu_infor_ptr);
#else
           MMK_CreateWin((uint32*)MMIPB_ADD_TO_GROUP_WIN_TAB, menu_infor_ptr);
#endif
            break;
		default:
			break;
    }
    
    return TRUE;
}

const wchar TEMP_EXPORT_PATH[] = { 'T', 'E', 'M','P','V','C','A','R','D','.','v','c','f', 0 };
/*****************************************************************************/
//  Description : deal member win option menu
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_HandleOperateSelCallBack(MMIPB_MENU_DATA_INFOR_T* menu_infor_ptr)//!!!!!!!!!menu_ptr进入函数之后直接使用没有copy
{
    MMIPB_APPLET_T *applet_ptr = PNULL;
    BOOLEAN         ret = FALSE;
    
    if(PNULL == menu_infor_ptr)
    {
        return ret;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);
    if(PNULL == applet_ptr)
    {
        return ret;
    }
    switch(menu_infor_ptr->operate_type)
    {
    case MMIPB_OPERATE_TYPE_ERASE_MULTI:
    case MMIPB_OPERATE_TYPE_DELETE_SIM1_CONTACTS:
    case MMIPB_OPERATE_TYPE_DELETE_SIM2_CONTACTS:
        OpenPbQueryWin(menu_infor_ptr);
        break;
    case MMIPB_OPERATE_TYPE_MULTI_SHARE:
            menu_infor_ptr->menu_type = MMIPB_MULTI_SHARE_BY_MENU;
            menu_infor_ptr->need_reset_storage = FALSE;
            MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
            return FALSE;
        break;
#ifdef MMI_VCARD_SUPPORT
    case MMIPB_OPERATE_TYPE_SEND_VCARD:
            menu_infor_ptr->menu_type = MMIPB_SENDVCARD_SUB_MENU;
            menu_infor_ptr->need_reset_storage = FALSE;
            MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
        break;
#endif
    case MMIPB_OPERATE_TYPE_COPY_TO_PHONE:
        CopyContact(
            MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT),
            menu_infor_ptr);
        break;
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM1:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM2:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM3:
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM4:
#endif
#endif
#endif
        CopyContact(
            MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + (menu_infor_ptr->operate_type -MMIPB_OPERATE_TYPE_COPY_TO_SIM1),/*lint !e656*/
            PHONEBOOK_SIM_STORAGE),
            menu_infor_ptr);

        break;
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM1:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM2:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM3:
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM4:
#endif
#endif
#endif
        CopyContact(
            MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT),
            menu_infor_ptr);

        break;
#ifdef MMIPB_MOST_USED_SUPPORT
    case MMIPB_OPERATE_TYPE_FAVOURITE_ADD:
        {
            //MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID; 
            uint16 favourite_count = MMIPB_GetGroupContactCount(PB_GROUP_FAVROUTE);
            if(MMIPB_GetSelectCount(applet_ptr->instance.handle) + favourite_count> MMIPB_MAX_FAVOURITE_NUM)
            {
                 MMIPB_OpenAlertTipWin(applet_ptr->caf_applet.app_handle, MMIPB_ERROR_MAX_LIMIT, MMIPB_OPC_ADD_FAVOURITE);
                 MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
				 return ret;
            }
            else
            {
            
                MMIPB_ERROR_E error = MMIPB_AddContactToFavouriteAsynForPB(applet_ptr->instance.handle);
                if((error == MMIPB_ERROR_SUCCESS) || (error == MMIPB_ERROR_PENDING))
                {
                    MMIPB_OpenProcessWin(applet_ptr->caf_applet.app_handle,TXT_COMMON_WAITING, TXT_PB_ADD_FAVOURITE_CONTACT); 
                }
            }
        }
        break;

    case MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE:
        OpenPbQueryWin(menu_infor_ptr);
        //MMIPB_RemoveContactFromFavouriteAsynForPB(menu_ptr->app_handle);
        break;
#endif
#ifdef MMI_VCARD_SUPPORT  
    case MMIPB_OPERATE_TYPE_EXPORT_CONTACT:  
        {           
            SelectExportFolder(menu_infor_ptr->applet_handle,PNULL,0);
        }
        break;
    case MMIPB_OPERATE_TYPE_BT_EXPORT_CONTACT:
	{
	     MMI_HANDLE_T cur_applet_handle = 0;
	     MMIPB_APPLET_T *applet_ptr = PNULL;

            MMIPB_GetCurAppletHandle(&cur_applet_handle);
            applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(cur_applet_handle);
            if(applet_ptr != PNULL)
            {
                if(MMIPB_IsMark(cur_applet_handle))      
                {
                    MMIPB_AbortPB(FALSE);
                   
                    ret = MMIPB_ExportContactAsynForPB(applet_ptr->instance.handle , "temp");

                }
                if(ret == MMIPB_ERROR_PENDING)
                {
                    MMIPB_OpenProcessWin(cur_applet_handle,TXT_COMMON_WAITING, TXT_COMMON_WAITING);
                
                } 				
     }		
    	}
	break;
#endif        
        case MMIPB_OPERATE_TYPE_WRITE_MESSAGE:
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        case MMIPB_OPERATE_TYPE_SEND_GROUP_MSG:
            MMIPB_SendGroupMsg(applet_ptr->instance.handle);
            break;
#endif
    default:
        break;
        
    }
	ret = TRUE;
	return ret;

}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : get send menu type by node id
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_SEND_TYPE_E GetSendTypeByNodeId(uint16 node_id)
{
    MMIPB_SEND_TYPE_E send_type = MMIPB_SEND_BY_SMS;
    switch(node_id)
    {
        case PB_OPTMENU_NODE_SEND_BY_MMS_MENU:
            send_type = MMIPB_SEND_BY_MMS;
            break;
//#ifdef MMIEMAIL_SUPPORT
        case PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU:
            send_type = MMIPB_SEND_BY_EMAIL;
            break;
//#endif
        case PB_OPTMENU_NODE_SEND_BY_BT_MENU:
            send_type = MMIPB_SEND_BY_BT;
            break;    
        default:
            break;
    }
    return send_type;
}
#endif

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : handle contact with with pbap option menu action
//  Global resource dependence : none
//  Author: cheney.wu
//  Note: 
/*****************************************************************************/
PUBLIC void HandleContactSyncPBAPAction(MMI_WIN_ID_T win_id)
{
    uint32       dev_service = 0;
    BT_ADDRESS   bt_addr = {0};
    MMI_WIN_ID_T pre_win_id;
    MMI_STRING_T wait_text = {0};

    BT_DEVICE_INFO bt_dev_info = {0};
    MMI_STRING_T   name_str = {0};
    uint8          paired_device_index[MMIBT_MAX_DEV_ITEMS]= {0};

    SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction START");

    pre_win_id = MMK_GetPrevWinId(win_id);
    MMIAPIPB_SetActivateBTWinId(pre_win_id);

    MMIBT_SetPbapRequestType(BT_PBAP_TYPE_PB);


    if (FALSE == BT_GetState())
    {
        SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction BT OFF");

        //power on BT first
        MMIBT_SetOpertor(MMIBT_OPC_CONNECT_PBAP, MMIBT_OPC_CONNECT_PBAP);	
        
        MMI_GetLabelTextByLang(STR_BT_TURN_ON, &wait_text);
        MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_POWER_ONOFF);
        return;
    }

    /* Check Paired PBAP device list */
    if(MMIBT_GetPairedPBAPDevList(paired_device_index))
    {
        SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction BT ON and paired deviced");
        if(0 == paired_device_index[0])
        {
            SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction BT ON and paired deviced no PBAP");
            //power on BT first
            MMIBT_SetOpertor(MMIBT_OPC_CONNECT_PBAP, MMIBT_OPC_CONNECT_PBAP);	
            
            MMI_GetLabelTextByLang(STR_CONTACT_SYNC_NOT_SUPPORT, &wait_text);
            MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_SEARCH);
            return;
        }
        else
        {
            BT_GetPairedDeviceInfo(BT_SERVICE_PBAP_SERVER, paired_device_index[0], &bt_dev_info);
            name_str.wstr_ptr = bt_dev_info.name;
            name_str.wstr_len = MMIAPICOM_Wstrlen(bt_dev_info.name);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction BT ON and paired deviced NO PBAP");
        //power on BT first
        MMIBT_SetOpertor(MMIBT_OPC_CONNECT_PBAP, MMIBT_OPC_CONNECT_PBAP);	
        
        MMI_GetLabelTextByLang(STR_BT_URN_NO_DEV_FOUND, &wait_text);
        MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_SEARCH);
        return;
    }

    SCI_TRACE_LOW("[MMIBT] HandleContactSyncPBAPAction: paired address: %02X-%02X-%02X-%02X-%02X-%02X", bt_dev_info.addr.addr[5], bt_dev_info.addr.addr[4], bt_dev_info.addr.addr[3], bt_dev_info.addr.addr[2], bt_dev_info.addr.addr[1], bt_dev_info.addr.addr[0]);
    
    if(!MMIBT_IsConnectDevice(&bt_dev_info.addr))
    {
        MMIBT_ConnectHandsetGatewayService(0);
//         SCI_TRACE_LOW("HandleContactSyncPBAPAction device not connect");
//         //power on BT first
//         MMIBT_SetOpertor(MMIBT_OPC_CONNECT_PBAP, MMIBT_OPC_CONNECT_PBAP);	
//         
//         MMI_GetLabelTextByLang(STR_CONTACT_SYNC_NOT_SUPPORT, &wait_text);
//         MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_SEARCH);
//         return;
    }
    
    if(MMIAPIBT_PbapIsConnected())
    {
        SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction send MSG_BT_PBAP_TP_CONNECT_SUCCESS");
        /************************************************************************/
        /* whether Static Data List is empty or not                             */
        /************************************************************************/
        MMIAPIPB_InitPBAPDataList();
        MMK_SendMsg(pre_win_id, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
    }
    else
    {
        if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
        {
            SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction not in MMIBT_PBAP_CONNECT_WAIT_WIN_ID");
            if(MMK_IsOpenWin(pre_win_id))
            {
                SCI_TRACE_LOW("[MMIPB] HandleContactSyncPBAPAction send MSG_BT_CONTACT_SYNC_CNF");
                MMK_SendMsg(pre_win_id, MSG_BT_CONTACT_SYNC_CNF, PNULL);
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : deal with win option menu action
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/

LOCAL void HandleOptMenuAction(
    MMI_WIN_ID_T win_id,
    PB_OPTMENU_NODE_E node_id,
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr
)
{
    MMIPB_APPLET_T *applet_ptr = PNULL;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    uint8          group_id = 0;
#endif
    if(PNULL == menu_infor_ptr)
    {
         return;
    }
    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        return;
    }
    
    switch(node_id)
    {
        case PB_OPTMENU_NODE_GROUPS:
            MMIPB_OpenGroupsListWin(menu_infor_ptr->applet_handle);
            //MMK_CloseWin(menu_infor_ptr->parent_win);
        break;
		
        case PB_OPTMENU_NODE_CONTACTS:
            //MMIPB_OpenContactsWin(menu_infor_ptr->applet_handle);
            MMK_CloseWin(menu_infor_ptr->parent_win);
        break;

        case PB_OPTMENU_NODE_SETTINGS:
            MMIPB_OpenContactsSettingsWin(menu_infor_ptr->applet_handle);
        break;
		
#ifdef MMI_VCARD_SUPPORT
        case PB_OPTMENU_NODE_SHARE:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_MULTI_SHARE;                
            MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);
        break;

        case PB_OPTMENU_MULTI_SHARE_BY_MMS_MENU:
            //SendMultiVcard(menu_infor_ptr->applet_handle, MMIPB_SEND_BY_MMS);
            //MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, &node_id);
        break;

        case PB_OPTMENU_MULTI_SHARE_BY_EMAIL_MENU:
            //SendMultiVcard(menu_infor_ptr->applet_handle, MMIPB_SEND_BY_EMAIL);
            //MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, &node_id);
        break;

        case PB_OPTMENU_MULTI_SHARE_BY_BT_MENU:
            //SendMultiVcard(menu_infor_ptr->applet_handle, MMIPB_SEND_BY_BT);
            //MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            #ifdef FM_SUPPORT
                 if(MMIAPIFM_IsFmActive()) 
                 {
                     SCI_TRACE_LOW("HandlePlayerOptPopMenuOK FM is avtiving!!");
                     MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
                 }
                 else
                 {
                      MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, &node_id);
                 }
            #else
                      MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, &node_id);
            #endif
        break;
#endif

        case PB_OPTMENU_COPY_FROM_PHONE:
#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
                    break;
                }
#endif
                menu_infor_ptr->parent_win= win_id;      
                menu_infor_ptr->menu_type = MMIPB_COPY_FROM_PHONE_MENU;            
                menu_infor_ptr->need_reset_storage = FALSE;
                MMIPB_EnterPbSelectionMenu(menu_infor_ptr);
        break;

        case PB_OPTMENU_COPY_FROM_SIM1:
#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
                    break;
                }
#endif
                menu_infor_ptr->parent_win= win_id;      
                menu_infor_ptr->menu_type = MMIPB_COPY_FROM_SIM1_MENU;            
                menu_infor_ptr->need_reset_storage = FALSE;
                MMIPB_EnterPbSelectionMenu(menu_infor_ptr);
        break;

        case PB_OPTMENU_COPY_FROM_SIM2:
#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
                    break;
                }
#endif
                menu_infor_ptr->parent_win= win_id;      
                menu_infor_ptr->menu_type = MMIPB_COPY_FROM_SIM2_MENU;            
                menu_infor_ptr->need_reset_storage = FALSE;
                MMIPB_EnterPbSelectionMenu(menu_infor_ptr);
        break;

        case PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE:
			
        break;
        case PB_OPTMENU_COPY_PHONE_TO_SIM1:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_PHONE_TO_SIM1;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;
        case PB_OPTMENU_COPY_PHONE_TO_SIM2:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_PHONE_TO_SIM2;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;
        case PB_OPTMENU_COPY_PHONE_TO_ANOTHER_DEVICE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_PHONE_TO_ANOTHER_DEVICE;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;
        case PB_OPTMENU_COPY_SIM1_TO_PHONE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_SIM1_TO_PHONE;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#else /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
        break;
        case PB_OPTMENU_COPY_SIM1_TO_SIM2:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_SIM1_TO_SIM2;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#else /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
        break;
        case PB_OPTMENU_COPY_SIM2_TO_PHONE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_SIM2_TO_PHONE;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#else /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
        break;
        case PB_OPTMENU_COPY_SIM2_TO_SIM1:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_SIM2_TO_SIM1;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#else /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
        break;
        case PB_OPTMENU_COPY_ANOTHER_DEVICE_TO_PHONE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_ANOTHER_DEVICE_TO_PHONE;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;
		
        case PB_OPTMENU_NODE_GROUP_ADD_MEMBERS:
#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
                    break;
                }
#endif                
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;

        case PB_OPTMENU_NODE_GROUP_SEND_MESSAGE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_SEND_GROUP_MSG;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);
        break;

        case PB_OPTMENU_NODE_GROUP_SEND_EMAIL:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_SEND_GROUP_EMAIL;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);
        break;

        case PB_OPTMENU_NODE_GROUP_DEL_MEMBERS:
#ifdef MMIPB_SYNC_WITH_O365        
                if (MMIPB_SYNC_IsSyncing())
                {
                    MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
                    break;
                }
#endif                
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_DEL_MEMBER;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;

        case PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_DELETE_SIM1_CONTACTS;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
        break;

        case PB_OPTMENU_NODE_DEBUG_SIM2_MANAGER:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_DELETE_SIM2_CONTACTS;
#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
                MMIPB_OpenWaitLoadSimReadyWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/
#endif
        break;

        case PB_OPTMENU_ADDEDIT_SAVE_MENU:
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, PNULL);
        break;

        case PB_OPTMENU_ADDEDIT_CLEAR_DATE:
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_ADDEDIT_CLEAR_DATE, PNULL);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case PB_OPTMENU_ADDEDIT_PREFERRED_NONE:
            menu_infor_ptr->contact_ptr->preferred_sim = 0;
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, (ADD_DATA)&menu_infor_ptr->contact_ptr->preferred_sim);
        break;

        case PB_OPTMENU_ADDEDIT_PREFERRED_SIM1:
            menu_infor_ptr->contact_ptr->preferred_sim = 1;
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, (ADD_DATA)&menu_infor_ptr->contact_ptr->preferred_sim);
        break;

        case PB_OPTMENU_ADDEDIT_PREFERRED_SIM2:
            menu_infor_ptr->contact_ptr->preferred_sim = 2;
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, (ADD_DATA)&menu_infor_ptr->contact_ptr->preferred_sim);
        break;
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/

        case PB_OPTMENU_DETAIL_SAVE_VCARD:
            MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, PNULL);
        break;

        case PB_OPTMENU_NODE_DEBUG_ADD_NEW:
        case PB_OPTMENU_NODE_ADDNEW://新增
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif
        if(!MMIPB_IsPbReady())
        {
            MMIPB_DisplaybannerInfo(0, STR_CONTACTS_NOT_READY_EXT01, PNULL);
            //MMIPB_OpenCommonDialogWin(menu_infor_ptr->applet_handle, STR_CONTACTS_NOT_READY_EXT01);
            //MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, 
                //PNULL,TXT_PB_NOT_READY,TXT_NULL,IMAGE_PUBWIN_WARNING,
                //PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
        }
        else
        {
#if defined(FDN_SUPPORT)
            if(menu_infor_ptr->is_need_pin_verify)
            {

                MMIPB_ERROR_E space_status = MMIPB_GetStorageFreeSpace(menu_infor_ptr->contact_ptr->storage_id, PNULL);

                if (MMIPB_ERROR_NO_SPACE == space_status)
                {
                    MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;
                    MMIPB_OpenCommonDialogWithWinId(menu_infor_ptr->applet_handle, alert_id, STR_FDN_LIST_FULL);
                    #if 0
                     MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, PNULL,
                            TXT_FDN_FULL, PNULL, IMAGE_PUBWIN_WARNING,&alert_id, PNULL,
                            MMIPUB_SOFTKEY_ONE, PNULL, PNULL);  
                    #endif
                }
                else
                {

                    //PIN2 verify                
                    MMIAPIPHONE_SetCurHandlingPinSimID(menu_infor_ptr->dual_sys);
                    MMIAPIPHONE_HandlePinAndPukFlow(
                        MNSIM_PIN_VERIFY,
                        MNSIM_PIN2,
                        (uint32)MMIPB_GetDailNumberListWinTab(),
                        TRUE); 
                    applet_ptr->instance.cmd = MMIPB_OPC_ADD;
                }

            }
            else
#endif                
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
                if(PB_OPTMENU_NODE_DEBUG_ADD_NEW == node_id)
                {
                    menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_DEBUG_ADD_ONE;
                }
                if(PNULL != menu_infor_ptr->contact_ptr)
                {//for keypad(not pda) phone, should memset contact_ptr,but for fdn and adn,should
                //not reset storageid
                    //uint16 storage_id = menu_infor_ptr->contact_ptr->storage_id;
                    SCI_MEMSET(menu_infor_ptr->contact_ptr,0x00,sizeof(MMIPB_CONTACT_T));
                   // menu_infor_ptr->contact_ptr->storage_id = storage_id;
                }
                MMIPB_AddOrEditContactOfPb(menu_infor_ptr);
                
            }
            
        }
        break;
        case PB_OPTMENU_NODE_MODIFY://修改
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif
#if defined(FDN_SUPPORT)
            if(menu_infor_ptr->is_need_pin_verify)
            {                
                //PIN2 verify                
                MMIAPIPHONE_SetCurHandlingPinSimID(menu_infor_ptr->dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMIPB_GetDailNumberListWinTab(),
                    TRUE); 
                applet_ptr->instance.cmd = MMIPB_OPC_EDIT;
            }
            else
#endif
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
                menu_infor_ptr->need_reset_storage = FALSE;
                MMIPB_AddOrEditContactOfPb(menu_infor_ptr);
            }
        break;

        case PB_OPTMENU_NODE_DELETE_ONE://删除
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif	
#ifdef MMI_PDA_SUPPORT                
            if (MMIPB_DETAIL_MSGLOG_OPT_MENU == menu_infor_ptr->menu_type)
            {
                MMIPB_EnterDetailMsgListSelectWin(menu_infor_ptr->applet_handle);
            }
                        
            else
#endif
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_ERASE_ONE;
                #if defined(FDN_SUPPORT)                   
                if(menu_infor_ptr->is_need_pin_verify)
                {               
                    //PIN2 verify                
                    MMIAPIPHONE_SetCurHandlingPinSimID(menu_infor_ptr->dual_sys);
                    MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMIPB_GetDailNumberListWinTab(),
                    TRUE); 
                    applet_ptr->instance.cmd = MMIPB_OPC_DELETE;
                    if(MMIPB_DETAIL_BASIC_OPT_MENU== menu_infor_ptr->menu_type)
                    {
                        MMK_CloseWin(menu_infor_ptr->parent_win);
                    }
                }
                else
                #endif                 
                {
                    OpenPbQueryWin(menu_infor_ptr);
                }
            }
        break;
        
        case PB_OPTMENU_NODE_DELETE_MULTI://删除
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif
        /*     if(menu_infor_ptr->is_need_pin_verify)
            {     
#ifdef FDN_SUPPORT                 
                //PIN2 verify                
                MMIAPIPHONE_SetCurHandlingPinSimID(menu_infor_ptr->dual_sys);
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMIPB_GetDailNumberListWinTab(),
                    TRUE); 
                applet_ptr->instance.cmd = MMIPB_OPC_DELETE;
#endif
            }
            else*/
            {
                //MMIPB_ERROR_E error = MMIPB_ERROR_ERROR;
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_ERASE_MULTI;                
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);
            }
        break;
#ifdef MMIPB_MOST_USED_SUPPORT            
        case PB_OPTMENU_NODE_CLEAR_MOSTUSED:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_CLEAR_MOSTUSED;
            OpenPbQueryWin(menu_infor_ptr);
            
            break;
#endif
        case PB_OPTMENU_NODE_WRITE_MESSAGE:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
        break;
        
        case PB_OPTMENU_NODE_SEND_SMS:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
        break;
        
        case PB_OPTMENU_NODE_SEND_MMS:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MMS;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
        break;
        
        case PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);          
        break;   
        
        case PB_OPTMENU_NODE_SEND_SMS_DETAIL:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);           
        break;
        
        case PB_OPTMENU_NODE_SEND_MMS_DETAIL:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MMS;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);           
            break;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT            
       case PB_OPTMENU_NODE_SEND_GROUP_SMS:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_SEND_GROUP_MSG;//MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);
        break;
#endif
        
 #ifdef MMI_PDA_SUPPORT           
        case PB_OPTMENU_NODE_DIAL_AUDIO://语音呼出
    #ifdef VT_SUPPORT
        case PB_OPTMENU_NODE_DIAL_VIDEO://可视呼出
    #endif
#ifdef MMI_IP_CALL_SUPPORT
        case PB_OPTMENU_NODE_DIAL_IP:        //IP拨号
#endif
            MMIPB_CallBCDNumber(MMI_DUAL_SYS_MAX,
            (MMIPB_CALL_TYPE_E)(node_id- PB_OPTMENU_NODE_DIAL_AUDIO+MMIPB_CALL_TYPE_MAKECALL_AUDIO), /*lint !e656*/
            menu_infor_ptr->contact_ptr,
            menu_infor_ptr->number_index);
        
        break;
#else
        case PB_OPTMENU_NODE_DIAL_AUDIO:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_AUDIO_DIAL;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
            break;
#ifdef VT_SUPPORT
        case PB_OPTMENU_NODE_DIAL_VIDEO:
            menu_infor_ptr->operate_type =MMIPB_OPERATE_TYPE_VIDEO_DIAL;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case PB_OPTMENU_NODE_DIAL_IP:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_IP_DIAL;
            MMIPB_HandleNumberSelect(menu_infor_ptr);
            break;
#endif
#endif
        
        case PB_OPTMENU_NODE_DIAL_AUDIO_DETAIL:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_AUDIO_DIAL;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);
            break;
#ifdef VT_SUPPORT
        case PB_OPTMENU_NODE_DIAL_VIDEO_DETAIL:
            menu_infor_ptr->operate_type =MMIPB_OPERATE_TYPE_VIDEO_DIAL;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case PB_OPTMENU_NODE_DIAL_IP_DETAIL:
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_IP_DIAL;
            MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);
            break;
#endif

        case PB_OPTMENU_NODE_ONE_COPYTOPHONE://复制到手机组
              menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_TO_PHONE;   
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif
              CopyContact(
                    MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT),
                    menu_infor_ptr
                    );
            break;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT      
        case PB_OPTMENU_NODE_GROUP_NEW:  
            MMIPB_AddUserGroup(menu_infor_ptr->applet_handle);            
            break;  
            
        case PB_OPTMENU_NODE_GROUP_EDIT:  //edit group
              #ifdef MMI_PDA_SUPPORT
              MMIPB_GetAllGroupIDs(menu_infor_ptr->group, &group_id, 1);
              MMIPB_EditUserGroup(menu_infor_ptr->applet_handle, 
                                  group_id);
              #endif
            break;
            
        case PB_OPTMENU_NODE_GROUP_NAME_EDIT:     
              MMIPB_GetAllGroupIDs(menu_infor_ptr->group, &group_id, 1);
              MMIPB_EditUserGroupName(menu_infor_ptr->applet_handle, 
                                  group_id);
            break;
        case PB_OPTMENU_NODE_GROUP_RING:  //edit group ring
            {    
                menu_infor_ptr->menu_type = MMIPB_RING_OPT_MENU;                    
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_RINGTONE;
                menu_infor_ptr->need_reset_storage = FALSE;
                //MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
                //MMIPB_EnterPbSelectionMenu(menu_infor_ptr);
                MMIPB_EnterPbSelectionRingWin(menu_infor_ptr);
            }
            break;
        case PB_OPTMENU_NODE_GROUP_DELETE:
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_DELETE_GROUP;
               OpenPbQueryWin(menu_infor_ptr);
            break;
#endif            
   #ifdef MMI_PDA_SUPPORT         
        case PB_OPTMENU_NODE_GROUP:
            MMIPB_OpenGroupWin(menu_infor_ptr->applet_handle);
            break;
        case PB_OPTMENU_NODE_ACCOUNT:
            break;
    #endif
#ifdef MMIPB_SIMDN_SUPPORT
        case PB_OPTMENU_NODE_MSISDN://本机号码
           MMIPB_OpenMSISDNWin(menu_infor_ptr->applet_handle, FALSE);
            break;
        case PB_OPTMENU_NODE_SDN:
            MMIPB_OpenSDNWin(menu_infor_ptr->applet_handle);
            break;
#endif            
#ifndef MMI_PDA_SUPPORT
    #if defined(MMI_SPEEDDIAL_SUPPORT) 
        case PB_OPTMENU_NODE_SPEED_DIAL:    //快捷拨号
            //MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0);
            MMIAPICC_OpenSpeedDialSettingsWin();
            break;
    #endif

        case PB_OPTMENU_NODE_USED_SPACE:
            MMIPB_OpenMemDetail(menu_infor_ptr->applet_handle);
            break;

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
        case PB_OPTMENU_NODE_DISPLAY_LOCATION:
            MMIPB_OpenSetDisplayOptWin(menu_infor_ptr->applet_handle);
            break;
#endif
#endif      
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        case PB_OPTMENU_NODE_GROUP_MEMBER_MANAGE:
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
            }
            break;

        case PB_OPTMENU_NODE_REMOVE_FROM_GROUP:
            if(PNULL == menu_infor_ptr->contact_ptr)
            {
                break;
            }                
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_REMOVE_FROM_GROUP;                
            OpenPbQueryWin(menu_infor_ptr);
            break;
#endif
   
    case PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL:
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL;        
        MMIPB_HandleNumberSelect(menu_infor_ptr);
        break;
        
    case PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL_DETAIL:
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL;        
        MMIPB_HandleOneNumberAction(menu_infor_ptr,menu_infor_ptr->number_index);
        break;
        
#ifdef MMIPB_GROUP_MANAGE_SUPPORT        
    case PB_OPTMENU_ADD_TO_GROUP:
        MMIPB_EnterSecondMenu(node_id,menu_infor_ptr);
            break;
#endif
#ifdef MMIPB_MOST_USED_SUPPORT            
    case PB_OPTMENU_ADD_TO_FAVORITE:
        if(MMIPB_FAVORITE_OPT_MENU == menu_infor_ptr->menu_type)
        {            
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_FAVOURITE_ADD;
            applet_ptr->instance.cmd = MMIPB_OPC_ADD_FAVOURITE;
            MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,
                menu_infor_ptr);
        }
        else
        {
            MMIPB_ERROR_E ret =MMIPB_ERROR_ERROR;

           // MMIPB_AddContactToFavourite(applet_ptr->instance.handle,menu_infor_ptr->contact_ptr);
            ret = MMIPB_AddUnlistContactFromFavourite(applet_ptr->instance.handle, menu_infor_ptr->contact_ptr->contact_id, menu_infor_ptr->contact_ptr->storage_id);
            if((MMIPB_ERROR_PENDING == ret) ||(MMIPB_ERROR_SUCCESS == ret) )
            {
                MMIPB_OpenProcessWin(menu_infor_ptr->applet_handle, TXT_COMMON_WAITING, TXT_PB_ADD_FAVOURITE_CONTACT);
            }
		}
        break;

     case PB_OPTMENU_REMOVE_FAVORITE:
        if(MMIPB_FAVORITE_OPT_MENU == menu_infor_ptr->menu_type)
        {
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE;    
            applet_ptr->instance.cmd = MMIPB_OPC_REMOVE_FAVOURITE;
            MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        }
        else
        {
            MMIPB_ERROR_E ret =MMIPB_ERROR_ERROR;
            
             ret = MMIPB_RemoveUnlistContactFromFavourite(applet_ptr->instance.handle, menu_infor_ptr->contact_ptr->contact_id, menu_infor_ptr->contact_ptr->storage_id);
            if((MMIPB_ERROR_PENDING == ret) ||(MMIPB_ERROR_SUCCESS == ret) )
            {
                MMIPB_OpenProcessWin(menu_infor_ptr->applet_handle, TXT_COMMON_WAITING, TXT_PB_REMOVE_FAVOURITE_CONTACT);
            }
          }
        break;
#endif        
        case PB_OPTMENU_NODE_COPY_FROM_SIM1://copy from
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif		
            if(0==MMIPB_GetContactCount(MMIPB_STORAGE_SIM1+node_id-PB_OPTMENU_NODE_COPY_FROM_SIM1))
            {
                MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, 
                    PNULL,TXT_SIM_NO_CONTACT_CAN_NOT_COPY,TXT_NULL,
                    IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,
                    PNULL, PNULL
                    );
            }
            else
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_FROM_SIM1;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
            } 
            break;

    #ifndef MMI_MULTI_SIM_SYS_SINGLE
        case PB_OPTMENU_NODE_COPY_FROM_SIM2:
            if(0==MMIPB_GetContactCount(MMIPB_STORAGE_SIM1+node_id-PB_OPTMENU_NODE_COPY_FROM_SIM1))
            {
                MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, 
                    PNULL,TXT_SIM_NO_CONTACT_CAN_NOT_COPY,TXT_NULL,
                    IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,
                    PNULL, PNULL
                    );
            }
            else
            {
                menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_FROM_SIM2;
                MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
            } 
  
            break;
    #endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
    case PB_OPTMENU_NODE_COPY_FROM_SIM3:
        if(0==MMIPB_GetContactCount(MMIPB_STORAGE_SIM1+node_id-PB_OPTMENU_NODE_COPY_FROM_SIM1))
        {
            MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, 
                PNULL,TXT_SIM_NO_CONTACT_CAN_NOT_COPY,TXT_NULL,
                IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,
                PNULL, PNULL
                );
        }
        else
        {
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_FROM_SIM3;
            MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        } 
        break;
#endif

#ifdef MMI_MULTI_SIM_SYS_QUAD
    case PB_OPTMENU_NODE_COPY_FROM_SIM4:
        if(0==MMIPB_GetContactCount(MMIPB_STORAGE_SIM1+node_id-PB_OPTMENU_NODE_COPY_FROM_SIM1))
        {
            MMIPUB_OpenAlertWinByTextIdEx(menu_infor_ptr->applet_handle, 
                PNULL,TXT_SIM_NO_CONTACT_CAN_NOT_COPY,TXT_NULL,
                IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,
                PNULL, PNULL
                );
        }
        else
        {
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_FROM_SIM4;
            MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        }
        break;
#endif    

    case PB_OPTMENU_NODE_COPY:
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif		
        MMIPB_OpenCopySelectWin(menu_infor_ptr->applet_handle);
        break;
     
    case PB_OPTMENU_NODE_COPYTOPHONE://复制到手机组
#ifdef MMIPB_SYNC_WITH_O365        
#if 0
		if (MMIPB_SYNC_IsSyncing())
		{
		  break;
		}
#endif
#endif		
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_TO_PHONE;   
        MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);        
        break;
        
    case PB_OPTMENU_NODE_ONE_COPYTOSIM1://copy to 
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    case PB_OPTMENU_NODE_ONE_COPYTOSIM2:
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    case PB_OPTMENU_NODE_ONE_COPYTOSIM3:
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
   case PB_OPTMENU_NODE_ONE_COPYTOSIM4:
    #endif       
       if(PB_OPTMENU_NODE_ONE_COPYTOSIM1 == node_id){
           menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_TO_SIM1;
       }
       #ifndef MMI_MULTI_SIM_SYS_SINGLE
       else if(PB_OPTMENU_NODE_ONE_COPYTOSIM2 == node_id)
       {
           menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_TO_SIM2;
       }
       #endif
       CopyContact(
           MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + node_id - PB_OPTMENU_NODE_ONE_COPYTOSIM1, PHONEBOOK_SIM_STORAGE),
           menu_infor_ptr
           );
    break;

    case PB_OPTMENU_NODE_COPYTOSIM1://copy to 
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    case PB_OPTMENU_NODE_COPYTOSIM2:
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    case PB_OPTMENU_NODE_COPYTOSIM3:
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
   case PB_OPTMENU_NODE_COPYTOSIM4:
    #endif
    if(0==MMIPB_GetContactCount(MMIPB_STORAGE_PHONE))
    {
        MMIPUB_OpenAlertWinByTextIdEx(
            menu_infor_ptr->applet_handle, 
                PNULL,TXT_NO_PHONE_CONTACT_CAN_NOT_COPY,TXT_NULL,
                IMAGE_PUBWIN_WARNING,
            PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
    }
    else if(!MMIAPIPHONE_GetSimExistedStatus(node_id -PB_OPTMENU_NODE_COPYTOSIM1))/*lint !e64 !e656*/
    {
        MMIPUB_OpenAlertWinByTextIdEx(
            menu_infor_ptr->applet_handle, 
                PNULL,TXT_PB_NO_SIM,TXT_NULL,
                IMAGE_PUBWIN_WARNING,
            PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
    }
    else
    {
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY_TO_SIM1+/*lint !e656*/
           (node_id-PB_OPTMENU_NODE_COPYTOSIM1);/*lint !e656*/
       OpenPbQueryWin(menu_infor_ptr);
    }
    break;
    
#ifdef MMI_VCARD_SUPPORT     
    case PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT:      //导入
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_IMPORT_VCARD;
        MMIPB_LoadVCardFromDir(PNULL,0);
        break; 

    case PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT://导出
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_EXPORT_CONTACT;
        MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);       
        break;

    case PB_OPTMENU_NODE_VCARD_BT_EXPORT_CONTACT://导?
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_BT_EXPORT_CONTACT;
        MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle, menu_infor_ptr);    
        break;
        
    case PB_OPTMENU_NODE_SEND_VCARD_MENU:
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_SEND_VCARD;
        MMIPB_OpenOperationSelectWin(menu_infor_ptr->applet_handle,menu_infor_ptr);
        break;    
        
    case PB_OPTMENU_NODE_SEND_BY_SMS_MENU://经短信
    case PB_OPTMENU_NODE_SEND_BY_MMS_MENU://经彩信
//#ifdef MMIEMAIL_SUPPORT
    case PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU:
//#endif
    case PB_OPTMENU_NODE_SEND_BY_BT_MENU: //经蓝牙
        #ifdef FM_SUPPORT
            if(MMIAPIFM_IsFmActive()) 
            {
                SCI_TRACE_LOW("HandlePlayerOptPopMenuOK FM is avtiving!!");
                MMIPUB_OpenAlertWarningWin(TXT_EXIT_BACKGROUND_FM);
            }
            else
            {
                 SendVcard(menu_infor_ptr->applet_handle,
                 GetSendTypeByNodeId(node_id),menu_infor_ptr->contact_ptr);
                 MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            }
        #else
                 SendVcard(menu_infor_ptr->applet_handle,
                 GetSendTypeByNodeId(node_id),menu_infor_ptr->contact_ptr);
                 MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        #endif
        break; 
#endif  
    
#ifdef MMI_PDA_SUPPORT    
    case PB_OPTMENU_SETTING:
        MMIPB_OpenSetWin(menu_infor_ptr->applet_handle);        
        break;
#endif

    
#ifdef SNS_SUPPORT
  /*  case PB_OPTMENU_NODE_IMPORT_FROM_SNS:
        if(MMIPB_IsSnsAccountLogIn())
        {    
            MMIPB_OpenSnsUpdateWaitWin(menu_infor_ptr->applet_handle);  
            MMIPB_StartUpdateSnsContact(menu_infor_ptr->applet_handle);   
        }
        else
        {
            MMIPB_SnsNotLogin(menu_infor_ptr->applet_handle); 
        }
        break;*/
#ifdef WRE_WEIBO_SUPPORT
    case PB_OPTMENU_NODE_IMPORT_FROM_SINA:
       // MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
        MMIPB_UpdateSnsContactByGroupId(MMIPB_GROUP_SNS_SINA);
        break;
#endif
#ifdef WRE_TWITTER_SUPPORT
    case PB_OPTMENU_NODE_IMPORT_FROM_TWITTER:
        // MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
        MMIPB_UpdateSnsContactByGroupId(MMIPB_GROUP_SNS_TIWTTER);
        break;
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    case PB_OPTMENU_NODE_IMPORT_FROM_FACEBOOK:
        // MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
        MMIPB_UpdateSnsContactByGroupId(MMIPB_GROUP_SNS_FACEBOOK);
        break;
#endif
#endif
    case PB_OPTMENU_PHOTO_SELECT_FROM_GALLERY:
#ifdef PIC_VIEWER_SUPPORT
        MMIAPIPICVIEW_EnterFromPB();
#endif
        break;
    case PB_OPTMENU_PHOTO_SELECT:
        SelectPhotoFromFile(menu_infor_ptr);
        break;

#ifdef CAMERA_SUPPORT
    case PB_OPTMENU_PHOTO_SHOT:
        #if (defined(CAMERA_SUPPORT) && defined(MMIDC_F_WORK_MODE))
        {
            MMIDC_WORK_PARAM_T work_param = {0};
            MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 
            
            work_param.work_mode = MMIDC_WORK_DC;
            work_param.work_callback = PhotoShotCallback;
            error_ret = MMIAPIDC_StartWork(&work_param);
            SCI_TRACE_LOW("[MMIPB] MMIAPIDC_StartWork error_ret:%d", error_ret);
        }
        #endif
        break;
#endif

    case PB_OPTMENU_PHOTO_DEFAULT:
        SetPbDefaultPhotoImage(menu_infor_ptr);
        break;
    case PB_OPTMENU_RING_SELECT:
        SetFileRing(menu_infor_ptr->parent_win);
      
        break;       
    case PB_OPTMENU_RING_PHONE:
        if(MMIPB_OPERATE_TYPE_GROUP_MANAGE == menu_infor_ptr->operate_type)
        {
            MMIPB_GROUP_T   group_info = {0};
            MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
            MMIAPISET_OpenFixCallRingWin(MMI_DUAL_SYS_MAX,menu_infor_ptr->parent_win,
                group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK
                );
        }
        else
        {
            MMIAPISET_OpenFixCallRingWin(MMI_DUAL_SYS_MAX,menu_infor_ptr->parent_win,
                menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK
                );
        }
        break;
    case PB_OPTMENU_RING_DEFAULT:
        SetPbDefaultRing(menu_infor_ptr);
        break;
    case PB_OPTMENU_RING_NO_SOUND:
        SetPbNosoundRing(menu_infor_ptr);
        break;

    case PB_OPTMENU_NODE_CONTACT_VIEW:
        if(MMIPB_MOSTUSED_CONTACTS == applet_ptr->instance.contacts_type)
        {
            uint16 entry_id = 0,storage_id = 0;
            //modify for coverity
            if(MMIPB_SearchPhoneNumber(&menu_infor_ptr->contact_ptr->number[0], 
                                        &entry_id,&storage_id,PNULL))
            {
                MMIPB_OpenContactWin(menu_infor_ptr->applet_handle,entry_id, storage_id);
            }
        }
        else
        {
            MMIPB_OpenContactWin(menu_infor_ptr->applet_handle,
                applet_ptr->instance.cur_entry_id, applet_ptr->instance.cur_storage_id);
        }
        break;
        
    case PB_OPTMENU_NODE_ADD_TO_CONTACT:
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif		
        MMIPB_EnterSecondMenu(node_id, menu_infor_ptr);
        break;
        
    case PB_OPTMENU_NODE_ADD_TO_NEW_CONTACT:
#ifdef MMIPB_SYNC_WITH_O365        
		if (MMIPB_SYNC_IsSyncing())
		{
			MMIPB_OpenSyncConflictPromptWin(menu_infor_ptr->applet_handle);
			break;
		}
#endif
        menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
        if(PNULL!=menu_infor_ptr->contact_ptr)
        {
            menu_infor_ptr->contact_ptr->storage_id = 0;
            menu_infor_ptr->contact_ptr->contact_id = 0;
        }        
        MMIPB_AddOrEditContactOfPb(menu_infor_ptr);
        break;

    case PB_OPTMENU_OPERRATE_SEL_CANCEL:
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, PNULL);
        break;
    case PB_OPTMENU_OPERRATE_SEL_OK:
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_OK, PNULL);
        break;
    case PB_OPTMENU_OPERRATE_SELECT_ALL: 
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_SELECT_ALL, PNULL);
        break;
    case PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL:
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_UNSELECT_ALL, PNULL);
        break;
#ifdef CMCC_UI_STYLE
    case PB_OPTMENU_OPERRATE_SELECT_SIM: 
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_SELECT_SIM, PNULL);
        break;

    case PB_OPTMENU_OPERRATE_SELECT_PHONE:
        MMK_SendMsg(menu_infor_ptr->parent_win, MMI_PB_OPERATE_SELECT_PHONE, PNULL);
        break;
#endif
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT        
    case PB_OPTMENU_NODE_ADD_TO_EXIST_CONTACT:
        {
            uint16 number_num = 0;
            uint16 number_index[MMIPB_MAX_NV_PHONE_NUM] = {0};
            
            if(PNULL == menu_infor_ptr->contact_ptr)
            {
                break;
            }
            number_num = MMIPB_GetContactNumberNum(
               menu_infor_ptr->contact_ptr, number_index);
            
            menu_infor_ptr->operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
            MMIPB_AddToExistContact(
                &menu_infor_ptr->contact_ptr->number[number_index[0]],
                MMIPB_ADD_TYPE_NUMBER,PNULL);
        }
    break;
#endif
#ifdef MMI_BT_PBAP_SUPPORT
    case PB_OPTMENU_NODE_ANDROIDSYNC:
        {
            HandleContactSyncPBAPAction(win_id);
        }
        break;
#endif

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Set ring from FMM file
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void SetFileRing(MMI_HANDLE_T win_id)
{
    MMIFMM_SELECT_INFO_T select_info = {0};
#ifdef DRM_SUPPORT 
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_MUSIC_ALL | MMIFMM_DRM_ALL;
#else
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_MUSIC_ALL;
#endif
    select_info.win_id = win_id;
            
    select_info.ring_vol = -1;
    if(MMIAPIFMM_OpenSelectMusicWin(&select_info))
    {
#ifdef DRM_SUPPORT
        {
            MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
            limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
            limit_value.is_rights_valid = 1;
            MMIAPIFMM_SetSelDrmLimitValue(limit_value);
        }
#endif        
    }
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 选择导出的目录
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN SelectExportFolder(MMI_HANDLE_T applet_handle,
    wchar *path_name,uint16 path_len)
{
    FILEARRAY ret_array = {0};
    MMIFMM_FILTER_T filter = {0};
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWinByTextIdEx(applet_handle, PNULL,
            TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,
            PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
        return FALSE;
    }
    SCI_MEMCPY(filter.filter_str, "*", strlen("*")); /*lint !e605 !e666*/
    //get directory from FMM
    ret_array = MMIAPIFILEARRAY_Create();          
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(path_name, 
        path_len, &filter, FUNC_FIND_FOLDER, FALSE, NULL, 
        ret_array, FolderSelectionCallBack,FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 选择导出的目录 callback
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void FolderSelectionCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    uint32  file_num = 0;
    BOOLEAN is_saved = FALSE;
    FILEARRAY_DATA_T *file_array_data_ptr = PNULL;
    wchar vcard_export_path[MMIFMM_PATHNAME_LEN] = {0};
    MMI_HANDLE_T cur_applet_handle = 0;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MMIPB_ERROR_E  ret = MMIPB_ERROR_ERROR;
    
    if(is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"MMIBT_OpenFileCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_9818_112_2_18_2_43_55_264,(uint8*)"d", file_num);
        if(file_num > 0)
        {
            //read user selected dir
			file_array_data_ptr = (FILEARRAY_DATA_T *)SCI_ALLOC_APP(sizeof(FILEARRAY_DATA_T));
            if(file_array_data_ptr != PNULL)
            {
                SCI_MEMSET(file_array_data_ptr, 0x00, sizeof(FILEARRAY_DATA_T));
                if(MMIAPIFILEARRAY_Read(file_array, 0, file_array_data_ptr))
                {
                    SCI_MEMSET(vcard_export_path, 0x00, MMIFMM_PATHNAME_LEN*sizeof(wchar));
                    MMIAPICOM_Wstrncpy(vcard_export_path, file_array_data_ptr->filename, file_array_data_ptr->name_len);
                    //call mary xiao api
                    MMIPB_GetCurAppletHandle(&cur_applet_handle);
                    applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(cur_applet_handle);
                    if(applet_ptr != PNULL)
                    {
                        if(MMIPB_IsMark(cur_applet_handle))      
                        {
                            MMIPB_AbortPB(FALSE);
                           
                            ret = MMIPB_ExportContactAsynForPB(applet_ptr->instance.handle , vcard_export_path);

                        }
                        else
                        {
                            if(0!=applet_ptr->instance.cur_entry_id)
                            {
                                MMIPB_AbortPB(FALSE);
                                MMIPB_ExportUnListContact(applet_ptr->instance.handle,applet_ptr->instance.cur_entry_id, 
                                    applet_ptr->instance.cur_storage_id, vcard_export_path);
                            }
                        }
                        if(ret == MMIPB_ERROR_PENDING)
                        {
                            MMIPB_OpenProcessWin(cur_applet_handle,TXT_PB_IS_EXPORTING, TXT_PB_EXPORT_CONTACT);
                        
                        } 
                        
                        is_saved= TRUE;
                    }
                }
                SCI_FREE(file_array_data_ptr);
            }			
		}
	}
    else
    {
        MMIPB_GetCurAppletHandle(&cur_applet_handle);
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(cur_applet_handle);
        if(applet_ptr != PNULL)
        {
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        }
    }
	if(!is_saved)
	{
		 //call mary xiao api
	}
	MMIAPIFILEARRAY_Destroy(&file_array);
}


/*****************************************************************************/
//  Description :临时 存放蓝牙要发送的多个联系人
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void ExportContactsToTempPath(wchar* path)
{
	MMI_HANDLE_T cur_applet_handle = 0;
	MMIPB_APPLET_T *applet_ptr = PNULL;
	MMIPB_ERROR_E ret = MMIPB_ERROR_SUCCESS;

	MMIPB_GetCurAppletHandle(&cur_applet_handle);
	applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(cur_applet_handle);
	if(applet_ptr != PNULL)
	{
	    if(MMIPB_IsMark(cur_applet_handle))      
	    {
	        MMIPB_AbortPB(FALSE);
	       
	        ret = MMIPB_ExportContactAsynForPB(applet_ptr->instance.handle , path);

	    }

	    if(ret == MMIPB_ERROR_PENDING)
	    {
	        MMIPB_OpenProcessWin(cur_applet_handle,TXT_PB_IS_EXPORTING, TXT_PB_EXPORT_CONTACT);
	    
	    } 
	    
	}
}

#endif
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : to handle favorite query window    
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRemoveFavoriteQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T * add_data_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    MMIPB_ERROR_E   error = MMIPB_ERROR_ERROR;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinTitleTextId(win_id,TXT_PB_REMOVE_FAVOURITE_CONTACT,TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if(win_info_ptr != PNULL)
        {
            add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)win_info_ptr->user_data;
            if(PNULL != add_data_ptr) //+ for coverity
            {
                applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(add_data_ptr->applet_handle);
                if(PNULL!=applet_ptr)
                {
                   MMIPB_AbortPB(FALSE);
                   error =  MMIPB_RemoveContactFromFavouriteAsynForPB(applet_ptr->instance.handle);
                   if((error == MMIPB_ERROR_SUCCESS) || (error ==MMIPB_ERROR_PENDING))
                   {
                       //MMIPB_OpenWaitWin(applet_ptr->caf_applet.app_handle,TXT_COMMON_WAITING);
    				   MMIPB_OpenProcessWin(applet_ptr->caf_applet.app_handle, TXT_COMMON_WAITING, TXT_PB_REMOVE_FAVOURITE_CONTACT);  //Bug 117694 
                   }

                }
            }
        }  
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL: 
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        if(PNULL != applet_ptr)
        {
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        }        
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if(win_info_ptr != PNULL)
        {
            add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)win_info_ptr->user_data;
            if(PNULL != add_data_ptr)
            {
                SCI_FREE(add_data_ptr);            
            }
        }        
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : to handle Mostused query window    
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleClearMostUsedQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T * add_data_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    //MMIPB_ERROR_E   error = MMIPB_ERROR_ERROR;
    
    
    win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != win_info_ptr)
    {
        add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)win_info_ptr->user_data;
    } 

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        MMIPUB_SetWinTitleTextId(win_id,TXT_PB_RECENT_USED,TRUE);
#else
        MMIPUB_SetWinTitleTextId(win_id,TXT_PB_MOSTUSED_GROUP,TRUE);
#endif
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:              
        if(MMIPB_ERROR_SUCCESS ==  MMIPB_DeleteAllMostUsedContact())
        {
            if(PNULL != add_data_ptr)
            {                
                applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(add_data_ptr->applet_handle);
                if(PNULL != applet_ptr)  //+ for coverity
                {
                    MMIPB_UpdateCurrentList(applet_ptr->caf_applet.app_handle, MMIPB_OPC_DELETE);  
                }
            }
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:       
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(PNULL != add_data_ptr)
        {
            SCI_FREE(add_data_ptr);            
        }       
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}
#endif

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : to handle favorite query window    
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRemoveFromGroupQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:                
        applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
        //+for coverity
        if(PNULL != applet_ptr)  
        {
            MMIPB_ManageUnlistGroupContact(applet_ptr->instance.handle,
                                applet_ptr->instance.cur_group_id, 
                                applet_ptr->instance.cur_entry_id, 
                                applet_ptr->instance.cur_storage_id, 
                                FALSE);
        }   
        MMK_CloseWin(win_id);  
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if(PNULL != add_data_ptr)
        {
            if(PNULL != add_data_ptr->user_data)
            {
                SCI_FREE(add_data_ptr->user_data);/*lint !e64*/
            }
        }
        MMK_FreeWinAddData(win_id);
        break;   
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}
#endif
/*****************************************************************************/
//  Description :get menu style menu type
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL GUIMENU_STYLE_E GetMenuStyle(MMIPB_MENU_TYP_E menu_type)
{
    GUIMENU_STYLE_E menu_style = GUIMENU_STYLE_POPUP_AUTO;
    switch(menu_type)
    {
        case MMIPB_MAIN_OPT_MENU:
#ifdef MMIPB_MOST_USED_SUPPORT
        case MMIPB_MOSTUSED_OPT_MENU:
        case MMIPB_FAVORITE_OPT_MENU:
#endif
        case MMIPB_GROUPLIST_OPT_MENU:
        case MMIPB_GROUPLIST_EMPTY_OPT_MENU:
        case MMIPB_GROUP_ENTRY_OPT_MENU:
        case MMIPB_DETAIL_BASIC_OPT_MENU:
        case MMIPB_DETAIL_MSGLOG_OPT_MENU:
        case MMIPB_SIM_DN_OPT_MENU:  
        case MMIPB_COPY_FROM_MENU:
        case MMIPB_ADDEDIT_OPT_MENU:
        case MMIPB_ADDEDIT_PREFERRED_SIM_MENU:
        case MMIPB_MULTI_SHARE_BY_MENU:
        case MMIPB_DETAIL_VCARD_OPT_MENU:
            menu_style = GUIMENU_STYLE_POPUP;
        break;
            
#ifndef MMI_PDA_SUPPORT
        case MMIPB_PHOTO_OPT_MENU:
        case MMIPB_RING_OPT_MENU:
        case MMIPB_OPERATE_SEL_OPT_MENU:
	 	case MMIPB_OPERATE_SEL_ALL_OPT_MENU:
            menu_style = GUIMENU_STYLE_POPUP;
        break;
#endif
        default:
            break;
    }
    return menu_style;
}
/*****************************************************************************/
//  Description : 选择添加存储介质窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePbOptMenuWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result        = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id       = MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMBER_OPT_CTRL_ID);
    PB_OPTMENU_NODE_E              node_id       = 0;
    GUIMENU_STYLE_E menu_style = GUIMENU_STYLE_POPUP_AUTO;
    uint16 menu_num = 0;
    
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPB_APPLET_T*          applet_ptr = PNULL; 
    menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == menu_infor_ptr )
    {
        result = MMI_RESULT_FALSE;
        return result;
    } 
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);

    if(PNULL == applet_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   // 打开窗口:

        menu_style = GetMenuStyle(menu_infor_ptr->menu_type);
               			//creat dynamic menu
#ifndef MMI_PDA_SUPPORT
        if(GUIMENU_STYLE_POPUP_RECT == menu_style)
        {
            GUI_BOTH_RECT_T     both_rect = {0};
            both_rect = MMITHEME_GetWinClientBothRect(win_id);
            both_rect.v_rect.top+=MMI_STATUSBAR_HEIGHT+MMITHEME_GetWinTitleHeight();
            both_rect.h_rect.top+=MMI_STATUSBAR_HEIGHT+MMITHEME_GetWinTitleHeight();
            GUIMENU_CreatDynamic(&both_rect,win_id,MMIPB_MEMBER_OPT_CTRL_ID, menu_style);
        }
        else
#endif
        GUIMENU_CreatDynamic(PNULL,win_id, MMIPB_MEMBER_OPT_CTRL_ID,menu_style);
        ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMBER_OPT_CTRL_ID);
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
        CreatePbOptMenu(win_id,
            ctrl_id,
            menu_infor_ptr,
            &menu_num);
        if(0==menu_num)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMBER_OPT_CTRL_ID));
        }
        if(MMIPB_GROUP_ENTRY_OPT_MENU == menu_infor_ptr->menu_type)
        {
            uint8 group_id = 0;
            MMIPB_GetAllGroupIDs(menu_infor_ptr->group, &group_id, 1);
            if(MMIPB_GetContactNumOfGroup(group_id))
            {
                MMIPB_CONTACT_LIST_INFO_T contact_list_info = {0};
                MMIPB_ReadContactList(&contact_list_info);
                if(contact_list_info.group_number_count[group_id] 					
                || contact_list_info.group_mail_count[group_id])
                {
                    GUIMENU_SetNodeGrayedByNodeId(FALSE, PB_OPTMENU_NODE_GROUP_SEND_MESSAGE, 0, ctrl_id);
                }
                else
                {
                    GUIMENU_SetNodeGrayedByNodeId(TRUE, PB_OPTMENU_NODE_GROUP_SEND_MESSAGE, 0, ctrl_id);
                }
                if(contact_list_info.group_mail_count[group_id])
                {
                    GUIMENU_SetNodeGrayedByNodeId(FALSE, PB_OPTMENU_NODE_GROUP_SEND_EMAIL, 0, ctrl_id);
                }
                else
                {
                    GUIMENU_SetNodeGrayedByNodeId(TRUE, PB_OPTMENU_NODE_GROUP_SEND_EMAIL, 0, ctrl_id);
                }
                GUIMENU_SetNodeGrayedByNodeId(FALSE, PB_OPTMENU_NODE_GROUP_DEL_MEMBERS, 0, ctrl_id);
            }
            else
            {
                GUIMENU_SetNodeGrayedByNodeId(TRUE, PB_OPTMENU_NODE_GROUP_SEND_MESSAGE, 0, ctrl_id);
                GUIMENU_SetNodeGrayedByNodeId(TRUE, PB_OPTMENU_NODE_GROUP_SEND_EMAIL, 0, ctrl_id);
                GUIMENU_SetNodeGrayedByNodeId(TRUE, PB_OPTMENU_NODE_GROUP_DEL_MEMBERS, 0, ctrl_id);
            }
        }
        break;

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    case MMI_PB_SET_DEFAULT_RING:
        {
            MMIPB_GROUP_T      group_info = {0};
            group_info.ring_id_flag = *(uint8*)param; 
            MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
        }
        break;
    case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
        {
            MMIPB_GROUP_T      group_info = {0};
            uint16 *ring_id_ptr = PNULL;


            MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
            ring_id_ptr = (uint16 *)param;
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr);
            MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);

        }
        
        break;

    case MSG_MULTIM_SELECTED_RETURN: 
        {
            MMIPB_GROUP_T      group_info = {0};
            MULTIM_SELECT_RETURN_T* select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            MMIPB_CUSTOM_RING_T* ring_ptr =SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
            MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
                                
            if(PNULL != ring_ptr)
            {
                SCI_MEMSET(ring_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));
                ring_ptr->is_valid =TRUE;			
                group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN;
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN;   
                ring_ptr->file_name_len = select_ptr->file_name_len;
                MMIAPICOM_Wstrncpy(ring_ptr->file_name, select_ptr->file_name_ptr, select_ptr->file_name_len);
                ring_ptr->file_size = select_ptr->file_size;
                ring_ptr->ring_type = select_ptr->music_type;

                MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
                MMIPB_SaveGroupRingInfo(applet_ptr->instance.cur_group_id, ring_ptr);
                SCI_FREE(ring_ptr);
            }
        }
        break;
#endif

    //case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    //case MSG_APP_OK:
    case MSG_APP_WEB:
        node_id = (PB_OPTMENU_NODE_E)GUIMENU_GetCurNodeId(ctrl_id);
        HandleOptMenuAction(win_id,node_id,menu_infor_ptr);
    
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
//    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

   case MSG_CLOSE_WINDOW:
        if(PNULL != menu_infor_ptr->contact_ptr)
        {
            SCI_FREE(menu_infor_ptr->contact_ptr);
        }
        MMK_FreeWinAddData(win_id);
        
        MMK_DestroyDynaCtrl(ctrl_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : create pb option menu
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbTabOptMenu(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
)
{
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
#ifdef HERO_ENGINE_TTS_SUPPORT
	{
		extern void hero_PB_ResetWinId(void);
		hero_PB_ResetWinId();
	}
#endif
    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_10196_112_2_18_2_43_56_265,(uint8*)"");
        return;
    }
    menu_infor_ptr = SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_infor_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == menu_infor_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_10202_112_2_18_2_43_56_266,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIPB_EnterPbTabOptMenu menu_infor_ptr is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_10205_112_2_18_2_43_56_267,(uint8*)"d",menu_infor_ptr);
    
    SCI_MEMSET(menu_infor_ptr, 0x00, sizeof(MMIPB_MENU_DATA_INFOR_T));
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_EnterPbTabOptMenu PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_10211_112_2_18_2_43_56_268,(uint8*)"");
        SCI_FREE(menu_infor_ptr);
        return;
    }
    SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
    
    SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));

    if(PNULL != data_ptr->contact_ptr)
    {
        SCI_MEMCPY(contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
    }

    menu_infor_ptr->contact_ptr = contact_ptr;
    
   //SCI_TRACE_LOW:"MMIPB_EnterPbTabOptMenu the applet handle is %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_10226_112_2_18_2_43_56_269,(uint8*)"d",data_ptr->applet_handle);
    
    MMK_CreateWinByApplet(data_ptr->applet_handle,
            (uint32 *)MMIPB_OPT_MENU_WIN_TAB,
            (MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);

}

/*****************************************************************************/
//  Description : 隐私检查验证后，打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: yanyan.an
//  Note: 只用于隐私检查后的回调处理函数
/*****************************************************************************/
PUBLIC void MMIPB_AddOrEditContactOfPbExt(BOOLEAN is_permitted, DPARAM param_ptr)
{
	MMIPB_MENU_DATA_INFOR_T *add_data = (MMIPB_MENU_DATA_INFOR_T*)param_ptr;

	if (is_permitted)
	{
		MMIPB_AddOrEditContactOfPb(add_data);	
	}	
	SCI_FREE(add_data->contact_ptr);
}

/*****************************************************************************/
//  Description : 打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AddOrEditContactOfPb(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
)
{
    uint16 storage_id = 0;
    //uint8  group_id = 0;
#ifdef MMI_PDA_SUPPORT
    PB_SAVE_POSITION_TYPE_E save_pos = PB_SAVE_POSITION_MAX;
#endif
    MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID;
    uint8 i = 0;
    BOOLEAN sim_existed_num = FALSE;
#ifdef MMIPB_SIMCONTACT_SUPPORT
    MMIPB_MEMORY_E enStatus = MMIPB_GetContactSaveStatus();
#endif // MMIPB_SIMCONTACT_SUPPORT
    if(PNULL == data_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("data_ptr->operate_type %d, data_ptr->need_reset_storage %d", data_ptr->operate_type,data_ptr->need_reset_storage);
    
    if(data_ptr->contact_ptr== PNULL
        ||(MMIPB_OPERATE_TYPE_ADD_ONE == data_ptr->operate_type&& data_ptr->need_reset_storage)
        ||(MMIPB_OPERATE_TYPE_DEBUG_ADD_ONE == data_ptr->operate_type&& data_ptr->need_reset_storage)
     )
    {
        if(MMIPB_PbIsFull())/*lint !e746*/
        {
            #if 0
            uint32 time_period = 2000;
            MMIPUB_OpenAlertWinByTextIdEx(data_ptr->applet_handle,&time_period,
                TXT_PB_FULL_CAN_NOT_ADD_CONTACT,TXT_NULL,IMAGE_PUBWIN_WARNING,
                &alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
            MMIPUB_SetWinTitleTextId(MMK_GetWinHandle(data_ptr->applet_handle,alert_id),TXT_PB_NEW_CONTACT,TRUE);
            #else
            MMIPB_OpenCommonDialogWithWinId(data_ptr->applet_handle, alert_id, STR_PHONE_MEMORY_FULL_EXT01);
            #endif
            return;
        }
        
        //新增联系人，没有设置storage id,需要选择或者从默设置中读取storage id
#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error        
       save_pos = MMIPB_ReadSavePos();
        if(PB_SAVE_POSITION_ALWAYS_QUERY ==save_pos)
        {
            //需要先选择存储位置
            MMIPB_EnterStorageSelectWin(data_ptr);
        }
        else
        {
            //直接打开添加窗口
             if(PB_SAVE_POSITION_PHONE ==save_pos)
             {
                 //保存在手机存储器
                 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);

             }
             else if(save_pos >= PB_SAVE_POSITION_SIM_BEGAIN && save_pos <= PB_SAVE_POSITION_SIM_END)
             {
                 //保存在sim卡存储器
                 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 +save_pos -PB_SAVE_POSITION_SIM_BEGAIN, 0);
             }
             OpenAddWin(data_ptr, storage_id);
        }
#else
        /*Bug 74606 start*/
        {
#ifdef MMIPB_SIMCONTACT_SUPPORT
            if (MMIPB_MEMORY_PHONEANDSIM == enStatus)
            {
                MMIPB_EnterStorageSelectWin(data_ptr);
            }
            else if (MMIPB_MEMORY_SIM1 == enStatus)
            {
                OpenAddWin(data_ptr, MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, 0));
            }
            else if (MMIPB_MEMORY_SIM2 == enStatus)
            {
                OpenAddWin(data_ptr, MMIPB_GetStorageID(MMIPB_STORAGE_SIM2, 0));
            }
            else if (MMIPB_MEMORY_PHONE == enStatus)
            {
                OpenAddWin(data_ptr, MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
            }
            else
            {
                SCI_TRACE_LOW("[MMIPB-memory] Error memory status: %d", enStatus);
            }
#else
        	for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
        	{
        		if((MMIAPIPHONE_GetSimExistedStatus(i)) && ((SIM_STATUS_PUK_BLOCKED != MMIAPIPHONE_GetSimStatus(i)) && (SIM_STATUS_PIN_BLOCKED != MMIAPIPHONE_GetSimStatus(i))))
        		{
                        sim_existed_num = TRUE;
                        break;
        		}
        	}

        	if (sim_existed_num && MMIPB_OPERATE_TYPE_DEBUG_ADD_ONE == data_ptr->operate_type)
        	{
        		//有sim卡存在，需要先选择存储位置
        		MMIPB_EnterStorageSelectWin(data_ptr);
        	}
        	else
        	{
        		//没有sim卡存在，直接打开添加窗口保存在手机存储器
        		storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        		OpenAddWin(data_ptr, storage_id);
        	}
#endif //MMIPB_SIMCONTACT_SUPPORT
        }
        /*Bug 74606 end*/
#endif
    }
    else
    {
        OpenAddWin(data_ptr, data_ptr->contact_ptr->storage_id);
    }
}

#if 0
/*****************************************************************************/
//  Description : 加入群组窗口处理函数
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToGroupWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_HANDLE_T        ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_ADD_TO_GROUP_LISTBOX_CTRL_ID);
    MMIPB_NAME_T        name_t = {0};
    
    uint16               i = 0;
    uint16               item_index = 0;
    uint16               select_num = 0;
    uint16              select_index[MMIPB_MAX_GROUP_NUM] = {0};
    GUILIST_ITEM_T * item_ptr = PNULL;
    uint8            start_group_id = PB_GROUP_ALL;
    
    
    MMI_STRING_T        str_t = {0};
    
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
    
    add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr||PNULL ==  add_data_ptr->contact_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //list avaliabel groups
        GUILIST_SetMaxItem(ctrl_id, MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM, FALSE );
        GUILIST_SetMaxSelectedItem(ctrl_id,MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM);
        GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, MIDDLE_BUTTON);
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
        
        for (i = start_group_id; i < MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM; i++)
        {
            if(MMIPB_IsInNvGroup(i) && (MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i)))
            {                    
                str_t.wstr_len = name_t.wstr_len;
                str_t.wstr_ptr = name_t.wstr;
                
                MMIAPISET_AppendListItemByTextBuffer(str_t.wstr_ptr,str_t.wstr_len,i,
                    TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
               
                if( add_data_ptr->contact_ptr->union_t.group  & (1<<i) )
                {
                    //set it selected
                    GUILIST_SetSelectedItem(ctrl_id, item_index, TRUE);
                } 
                item_index++;
            }
            
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);

            GUILIST_SetSelectedItem(ctrl_id, cur_index, 
			                        (!GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, cur_index), GUIITEM_STATE_SELECTED))/*lint !e730*/
			                        );/*lint !e730*/
            MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, 0);
        }
         
        break;
    case MSG_APP_OK:    
    case MSG_CTL_OK:
#ifdef MMIPB_MOST_USED_SUPPORT 
        //selected this group
        if((1<<PB_GROUP_FAVROUTE) &add_data_ptr->contact_ptr->union_t.group)
        {
            add_data_ptr->contact_ptr->union_t.group = 0;
            add_data_ptr->contact_ptr->union_t.group = (1<<PB_GROUP_FAVROUTE);
        }
        else
#endif
        {
            add_data_ptr->contact_ptr->union_t.group = 0;
        }
        
        select_num = GUILIST_GetSelectedItemIndex(ctrl_id, select_index, MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM);
        for(i = 0;i<select_num;i++)
        {
            item_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, select_index[i]);
            if(PNULL!=item_ptr)
            {
                add_data_ptr->contact_ptr->union_t.group |= (1<<item_ptr->user_data);
            
            }
            
        }
        if(MMIPB_OPERATE_TYPE_ADD_TO_GROUP == add_data_ptr->operate_type)
        {
            MMIPB_ModifyContact(add_data_ptr->contact_ptr);
            MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_RELOAD_SEARCH_LIST_MSG,PNULL);
        }
        else if(MMIPB_OPERATE_TYPE_DETAIL_GROUP == add_data_ptr->operate_type)
        {
            MMIPB_ModifyContact(add_data_ptr->contact_ptr);
            MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG,PNULL);
        }
        else
        {
           // MMIPB_ModifyContact(add_data_ptr->contact_ptr );
            MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG,PNULL);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_CLOSE_WINDOW:
        if((MMIPB_OPERATE_TYPE_ADD_TO_GROUP == add_data_ptr->operate_type) || (MMIPB_OPERATE_TYPE_DETAIL_GROUP == add_data_ptr->operate_type))
        {
            if(PNULL != add_data_ptr->contact_ptr)
            {
                SCI_FREE(add_data_ptr->contact_ptr);
            }
            MMK_FreeWinAddData(win_id);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#else
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void AppendListItemForAddToGroupWin(                                    
                                                const wchar *        text_ptr,
                                                uint16               text_len,
                                                uint32               user_data,
                                                MMI_CTRL_ID_T        ctrl_id
                                                )
{
	GUILIST_ITEM_T      item_t      =   {0};
	GUILIST_ITEM_DATA_T item_data   =   {0};
	
	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
	item_t.item_data_ptr = &item_data;
	
	item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_ptr;
	item_data.item_content[0].item_data.text_buffer.wstr_len = text_len;
	
	item_data.softkey_id[0] = TXT_NULL;
	item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
	item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
	
	item_t.user_data = user_data;
	
	GUILIST_AppendItem( ctrl_id, &item_t );      
}
/*****************************************************************************/
//  Description : 加入群组窗口处理函数
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddToGroupWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
	MMI_RESULT_E        result = MMI_RESULT_TRUE;
	MMI_HANDLE_T        ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_ADD_TO_GROUP_LISTBOX_CTRL_ID);
	MMIPB_NAME_T        name_t = {0};
	
	uint16               i = 0;
	uint16               item_index = 0;
	GUILIST_ITEM_T * item_ptr = PNULL;
	uint8            start_group_id = PB_GROUP_HOME;
	
	
	MMI_STRING_T        str_t = {0};	
	MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;	
	add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
	if(PNULL == add_data_ptr||PNULL ==  add_data_ptr->contact_ptr)
	{
		result = MMI_RESULT_FALSE;
		return result;
	}
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:	
		GUILIST_SetMaxItem(ctrl_id, MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM + 1, FALSE );
		MMI_GetLabelTextByLang(STR_GROUP_LIST_NONE_EXT01, &str_t); 
		AppendListItemForAddToGroupWin(str_t.wstr_ptr,str_t.wstr_len,0,ctrl_id); //none
		if( add_data_ptr->contact_ptr->union_t.group  & (1<<i) )
		{
		    GUILIST_SetCurItemIndex(ctrl_id,item_index);
		} 
		item_index++;
		for (i = start_group_id; i < MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM; i++)
		{
			if(MMIPB_IsInNvGroup(i) && (MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i)))
			{                    
				str_t.wstr_len = name_t.wstr_len;
				str_t.wstr_ptr = name_t.wstr;
				AppendListItemForAddToGroupWin(str_t.wstr_ptr,str_t.wstr_len,i,ctrl_id);
				if( add_data_ptr->contact_ptr->union_t.group  & (1<<i) )
				{
					GUILIST_SetCurItemIndex(ctrl_id,item_index);
				} 
				item_index++;
			}
			
		}
		MMK_SetAtvCtrl(win_id, ctrl_id);
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_SMS_DISTRIBUTION_LISTS))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_SMS_DISTRIBUTION_LISTS))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		{
			uint16 cur_index = GUILIST_GetCurItemIndex(ctrl_id);
			add_data_ptr->contact_ptr->union_t.group = 0;
			item_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
			if(PNULL!=item_ptr)
			{
				add_data_ptr->contact_ptr->union_t.group |= (1<<item_ptr->user_data);
				
			}
			if(MMIPB_OPERATE_TYPE_ADD_TO_GROUP == add_data_ptr->operate_type)
			{
				MMIPB_ModifyContact(add_data_ptr->contact_ptr);
				MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_RELOAD_SEARCH_LIST_MSG,PNULL);
			}
			else if(MMIPB_OPERATE_TYPE_DETAIL_GROUP == add_data_ptr->operate_type)
			{
				MMIPB_ModifyContact(add_data_ptr->contact_ptr);
				MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG,PNULL);
			}
			else
			{
				MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG,PNULL);
			}
			MMK_CloseWin(win_id);
		}		
		break;		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:		
		MMK_CloseWin(win_id);		
		break;
		
	case MSG_CLOSE_WINDOW:
		if((MMIPB_OPERATE_TYPE_ADD_TO_GROUP == add_data_ptr->operate_type) || (MMIPB_OPERATE_TYPE_DETAIL_GROUP == add_data_ptr->operate_type))
		{
			if(PNULL != add_data_ptr->contact_ptr)
			{
				SCI_FREE(add_data_ptr->contact_ptr);
			}
			MMK_FreeWinAddData(win_id);
		}
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);    
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : edit phone number type label 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNumberTypeWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_EDIT_NUMBER_TYPE_LISTBOX_CTRL_ID);

    uint16  edit_index = 0;
    uint16               i = 0;
    uint16    index = 0;
    uint16 number_type = 0;

    MMI_STRING_T        str_t = {0};
    GUILIST_ITEM_T *list_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
#ifdef CMCC_VCARD_SUPPORT
    uint8       usim_num_count = 0;
#endif

   add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr||PNULL ==  add_data_ptr->contact_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
        
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        //list avaliabel groups
        GUILIST_SetMaxItem(ctrl_id, MMIPB_FIXED_NUMBER, FALSE );
        GUILIST_SetMaxSelectedItem(ctrl_id,1);
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(add_data_ptr->contact_ptr->storage_id))
        {
            usim_num_count = MMIPB_GetUSIMAASCount(MMIPB_GetDualSysByStorageId(add_data_ptr->contact_ptr->storage_id));
            SCI_TRACE_LOW("yanyan.an num_count:%d", usim_num_count);
            for(i = MMIPB_ADN_NUMBER; i <= MIN((usim_num_count + 1), MMIPB_FIXED_NUMBER); i++)
            {
                MMIPB_GetNumberTypeString(add_data_ptr->contact_ptr->storage_id,
                    i, &str_t);

                MMIAPISET_AppendListItemByTextBuffer(str_t.wstr_ptr,str_t.wstr_len,i,
                        TXT_NULL,TXT_NULL,STXT_CANCEL,ctrl_id,GUIITEM_STYLE_ONE_LINE_TEXT);
                
            }    
        }
        else
#endif
        {
            for (i = MMIPB_ADN_NUMBER; i < (MMIPB_FIXED_NUMBER+1); i++)
            {
                MMIPB_GetNumberTypeString(add_data_ptr->contact_ptr->storage_id,
                    i, &str_t);

                MMIAPISET_AppendListItemByTextBuffer(str_t.wstr_ptr,str_t.wstr_len,i,
                        TXT_NULL,TXT_NULL,STXT_CANCEL,ctrl_id,GUIITEM_STYLE_ONE_LINE_TEXT);
                
            }
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);

        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        list_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, index);
        if(PNULL!=list_ptr)
        {
            number_type = list_ptr->user_data;
            edit_index = add_data_ptr->number_type_edit_index;
            add_data_ptr->contact_ptr->num_type[edit_index] = number_type;
            MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_CHANGE_NUMBER_TYPE,PNULL);
        }
       MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
    
        break;

    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#endif
#if defined(MMI_PDA_SUPPORT)&&defined(MMIPB_IM_MESSAGE_SUPPORT)
/*****************************************************************************/
//  Description : edit instance msg type label 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectImTypeWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_EDIT_IM_TYPE_LISTBOX_CTRL_ID);

    //uint16  edit_index = 0;
    uint16               i = 0;
    uint16    index = 0;
    uint16 im_type = 0;

    MMI_STRING_T        str_t = {0};
    GUILIST_ITEM_T *list_ptr = PNULL;
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;

   add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr||PNULL ==  add_data_ptr->contact_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
        
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        //list avaliabel groups
        GUILIST_SetMaxItem(ctrl_id, MMIPB_IM_AIM+1, FALSE );
        GUILIST_SetMaxSelectedItem(ctrl_id,1);
        
        for (i = MMIPB_IM_QQ; i < (MMIPB_IM_AIM+1); i++)
        {
            MMIPB_GetImTypeString(i, &str_t);

            MMIAPISET_AppendListItemByTextBuffer(str_t.wstr_ptr,str_t.wstr_len,i,
                    TXT_NULL,TXT_NULL,STXT_CANCEL,ctrl_id,GUIITEM_STYLE_ONE_LINE_TEXT);
            
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);

        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        list_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, index);
        if(PNULL!=list_ptr)
        {
            im_type = list_ptr->user_data;
            
            MMK_SendMsg(add_data_ptr->parent_win,MMI_PB_CHANGE_IM_TYPE,&im_type);
        }
       MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
    
        break;

    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理全选按钮操作
//  Global resource dependence :
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL   void HandleSelectAllFunc(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMI_HANDLE_T select_all_button_handle)
{
    GUI_BG_T  button_bg = {0};
    MMIPB_APPLET_T *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return;
    }
    //   MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
    applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
    GUILIST_SetAllSelected(list_handle, applet_ptr->instance.is_all_select);
    if(applet_ptr->instance.is_all_select)
    {
        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = IMAGE_CHECK_SELECTED_ICON;
        GUIBUTTON_SetBg(select_all_button_handle, &button_bg);
        GUIBUTTON_SetPressedBg(select_all_button_handle, &button_bg);
    }
    else
    {
        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = IMAGE_CHECK_UNSELECTED_ICON;
        GUIBUTTON_SetBg(select_all_button_handle, &button_bg);
    }
    MMK_UpdateScreen();
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_HANDLE_T       ctrl_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_CTRL_ID);
    MMI_CTRL_ID_T      ctrl_id = MMIPB_EDITBOX_CTRL_ID;
    MMI_TEXT_ID_T       title_id = 0; 
    MMI_RESULT_E        result   = MMI_RESULT_TRUE;
    MMI_STRING_T        str_info = {0};
    MMI_STRING_T        str_t    = {0};
    BOOLEAN             str_is_valid = TRUE;  
    BOOLEAN             is_disp_im_icon = TRUE;
    BOOLEAN             is_disp_num_info = FALSE;
	BOOLEAN				is_sim_contact   = FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		title_id = (MMI_TEXT_ID_T)MMK_GetWinAddDataPtr(win_id);
		GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
		//no left number display for sim contact name editor  
		is_sim_contact = GUIEDIT_GetPbEncode(ctrl_id);
		if(GUIEDIT_TYPE_TEXT == GUIEDIT_GetType(ctrl_id) && is_sim_contact)
		{
			GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
		}
        MMK_SetAtvCtrl(win_id, ctrl_handle); 
        break;        

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        // 从EDITBOX中获得数据
        GUIEDIT_GetString(ctrl_id, &str_info);
        if(GUIEDIT_TYPE_PHONENUM == GUIEDIT_GetType(ctrl_id))
        {
            if(0 != str_info.wstr_len && !MMIAPICOM_IsValidNumberStringEx(&str_info))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                str_is_valid = FALSE;                   
            }
        }        
        if(str_is_valid)
        {
            if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
            {
                str_t.wstr_len = str_info.wstr_len; 
                str_t.wstr_ptr = str_info.wstr_ptr;
            }            
            if (MMK_IsOpenWin(MMK_GetPrevWinId(win_id)))
            {
                MMK_SendMsg(MMK_GetPrevWinId(win_id), MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, (ADD_DATA)&str_t);
            }
            MMK_CloseWin(win_id);  
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}
#endif

extern PUBLIC void MMIAPICL_OpenEditBeforeDialWin(uint8   tel_len,
                                        uint8*  tel_num_ptr
                                        );
/*****************************************************************************/
//  Description : Handle tool bar action of pb view 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_HandleOneNumberAction(
    MMIPB_MENU_DATA_INFOR_T *data_ptr,
    uint16 number_index
    )
{
    if(PNULL == data_ptr||PNULL == data_ptr->contact_ptr)
    {
        return;
    }
    
 
    switch(data_ptr->operate_type)
    {
        case MMIPB_OPERATE_TYPE_AUDIO_DIAL:
        #ifdef VT_SUPPORT
        case MMIPB_OPERATE_TYPE_VIDEO_DIAL:
        #endif
#ifdef MMI_IP_CALL_SUPPORT
        case MMIPB_OPERATE_TYPE_IP_DIAL:
#endif
        {
            MMIPB_CallBCDNumber(MMI_DUAL_SYS_MAX,
                (MMIPB_CALL_TYPE_E)(data_ptr->operate_type-MMIPB_OPERATE_TYPE_AUDIO_DIAL),/*lint !e656*/
                data_ptr->contact_ptr,
				number_index
				);
        }
        break;
        
        case MMIPB_OPERATE_TYPE_WRITE_MESSAGE:
        {
            MMIPB_SendSMSByNumber(data_ptr->contact_ptr->number[number_index]);
        }
        break;

        case MMIPB_OPERATE_TYPE_WRITE_MMS:
          {
            MMIPB_SendMMSByBcdNumber(
                data_ptr->contact_ptr->number[number_index]
                );
          }
        break;
#ifdef MMIPB_MAIL_SUPPORT        
        case MMIPB_OPERATE_TYPE_WRITE_MAIL:
        {
                MMI_STRING_T        str_t = {0};            
                str_t.wstr_len = data_ptr->contact_ptr->mail.wstr_len;
                str_t.wstr_ptr = data_ptr->contact_ptr->mail.wstr;
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                MMIAPIMMS_AnswerMMS(MMI_DUAL_SYS_MAX,PNULL,&str_t);   
        }
        break;
 #endif       
        case MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL:
        {
            uint16 num_len = 0;
            uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            
            num_len= MMIPB_GetPhoneNumberByBcdNumber(
                data_ptr->contact_ptr->number[number_index],
                tele_num,
                MMIPB_NUMBER_MAX_STRING_LEN+2
                );
            if(num_len>0)
            {
                MMIAPICL_OpenEditBeforeDialWin(num_len,tele_num);
            }
        }
        break;
        
        default:
            break;
    }
}
/*****************************************************************************/
//  Description : Handle tool bar action of pb view 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_HandleNumberSelect(MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
    uint16 number_num = 0;
    uint16 number_index[MMIPB_MAX_NV_PHONE_NUM] = {0};

    if(PNULL == data_ptr||PNULL == data_ptr->contact_ptr)
    {
        return;
    }

    number_num = MMIPB_GetContactNumberNum(data_ptr->contact_ptr,number_index);
#ifdef MMIPB_MAIL_SUPPORT
    if(MMIPB_OPERATE_TYPE_WRITE_MESSAGE==data_ptr->operate_type||
        MMIPB_OPERATE_TYPE_WRITE_MMS==data_ptr->operate_type)
    {
        if(data_ptr->contact_ptr->mail.wstr_len>0)
        {
            if(0 == number_num)
            {
                data_ptr->operate_type = MMIPB_OPERATE_TYPE_WRITE_MAIL;
            }
            number_num++;
        }
    }
#endif
    if(MMIPB_DETAIL_NUMBER_ITEM_OPT_MENU ==data_ptr->menu_type)
    {
        MMIPB_HandleOneNumberAction(data_ptr,data_ptr->number_index);
    }
    else if(number_num>1)
    {
        EnterSelectNumberWin(data_ptr);
    }
    else if(1==number_num)
    {
        MMIPB_HandleOneNumberAction(data_ptr,number_index[0]);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextIdEx(data_ptr->applet_handle, 
            PNULL,
            TXT_CL_EMPTY_NUM,
            TXT_NULL,
            IMAGE_PUBWIN_WARNING,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL,
            PNULL);
    }
    
}

/*****************************************************************************/
//  Description : enter select number win
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void EnterSelectNumberWin(MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    if(PNULL == data_ptr)
    {
        return;
    }
    menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_infor_ptr)
    {
        return;
    }
    SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        SCI_FREE(menu_infor_ptr);
        return;
    }

    if(PNULL!=data_ptr->contact_ptr)
    {
        SCI_MEMCPY(contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
    }
    else
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    }
    menu_infor_ptr->contact_ptr = contact_ptr;
    
    
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        MMK_CreatePubwinTabEx(menu_infor_ptr->applet_handle,
        SPRD_GUI_LIST_ID,(uint32*)MMIPB_SELECT_NUMBER_WIN_TAB, menu_infor_ptr);
#else
        MMK_CreateWin((uint32*)MMIPB_SELECT_NUMBER_WIN_TAB, menu_infor_ptr);
#endif
}
/*****************************************************************************/
//  Description : 将电话簿记录中的号码设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void InitSelectNumberItem(MMI_CTRL_ID_T       ctrl_id,
        MMIPB_CONTACT_T *contact_ptr, 
        BOOLEAN is_include_email,
        MMI_TEXT_ID_T       left_softkey_id
)
{ 
    uint16  i = 0;
    MN_NUMBER_TYPE_E ton = 0;
    MMIPB_STRING_T  pb_str_t = {0};
    MMI_STRING_T    str_t = {0};
    wchar           wstr[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
#ifdef MMI_GUI_STYLE_MINISCREEN
	MMI_IMAGE_ID_T mid_softkey_id = TXT_NULL;
#else
	MMI_TEXT_ID_T  mid_softkey_id = TXT_NULL;
#endif
    GUILIST_SetMaxItem(ctrl_id,MMIPB_MAX_NV_PHONE_NUM + 1, FALSE );

    if(PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("InitSelectNumberItem PNULL == contact_ptr");
        return ;
    }
    
    for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)    
    {           
        ton = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[i].npi_ton);    
        pb_str_t.strlen = MMIAPICOM_GenDispNumber(
                                            (ton == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : ton),
                                            contact_ptr->number[i].number_len, 
                                            contact_ptr->number[i].number,
                                            pb_str_t.strbuf,
                                            MMIPB_NUMBER_MAX_STRING_LEN+2
                                            );
        if(pb_str_t.strlen > 0)
        {
            SCI_MEMSET(wstr,0x00,sizeof(wstr));
            str_t.wstr_ptr = wstr;
            str_t.wstr_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, pb_str_t.strlen);   
            MMI_STRNTOWSTR(str_t.wstr_ptr, MMIPB_NUMBER_MAX_STRING_LEN+2, 
                                        (const uint8*)pb_str_t.strbuf, MMIPB_MAX_STRING_LEN +2, 
                                        str_t.wstr_len);	
            AppendListItem(ctrl_id, 0, 
                    MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, i,/*lint !e64*/
                    GUIITEM_STYLE_ONE_LINE_ICON_TEXT, 
                    left_softkey_id,mid_softkey_id,STXT_CANCEL, 
                    GUIITEM_DATA_IMAGE_ID,
                     MMIPB_GetIconIdOfPhone( contact_ptr->num_type[i]), 
                    PNULL,
                    &str_t,
                    TXT_NULL,
                    FALSE);
        }
    }
#ifdef MMIPB_MAIL_SUPPORT
    if(TRUE == is_include_email && 0 != contact_ptr->mail.wstr_len)
    {       
        str_t.wstr_len = contact_ptr->mail.wstr_len;
        str_t.wstr_ptr = contact_ptr->mail.wstr; 
        AppendListItem(ctrl_id, 0, 
                MMIPB_LISTBOX_INDEX_MAIL, i,
                GUIITEM_STYLE_ONE_LINE_ICON_TEXT, 
                left_softkey_id,mid_softkey_id,STXT_CANCEL, 
                GUIITEM_DATA_IMAGE_ID,
                 IMAGE_NULL,//IMAGE_PHONEBOOK_MAIL_ADDRESS, 20151126 wang.qinggang pb_image_clean
                PNULL,
                &str_t,
                TXT_NULL,
                FALSE);
    }
#endif
}

/*****************************************************************************/
//  Description : 设置默认号码窗口处理函数
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectNumberWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_SELECT_NUMBER_LISTBOX_CTRL_ID);
   
    MMIPB_MENU_DATA_INFOR_T *add_data_ptr = PNULL;
    MMI_TEXT_ID_T       left_softkey_id = TXT_NULL;
#ifdef MMIPB_MAIL_SUPPORT
    MMI_STRING_T    str_t = {0};
#endif
    BOOLEAN is_include_email = FALSE;      
    uint16 num_index = 0;
    uint16 item_id = 0; 
    
    add_data_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == add_data_ptr||PNULL ==  add_data_ptr->contact_ptr)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        if(MMIPB_OPERATE_TYPE_WRITE_MESSAGE == add_data_ptr->operate_type)
        {
#ifndef MMIPB_MAIL_SUPPORT
            is_include_email = TRUE;
#else
            is_include_email = FALSE;
#endif
        }
        #ifndef MMI_PDA_SUPPORT
        left_softkey_id = STXT_SOFTKEY_OK_MK;
        #endif
        
        InitSelectNumberItem(ctrl_id,add_data_ptr->contact_ptr,is_include_email,left_softkey_id);    
        MMK_SetAtvCtrl(win_id, ctrl_id);

        if(MMIPB_OPERATE_TYPE_WRITE_MESSAGE == add_data_ptr->operate_type)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_COMMON_WRITE_MESSAGE, FALSE);            
        }
        else if(MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL== add_data_ptr->operate_type)
        {
            GUIWIN_SetTitleTextId(win_id,TXT_EDIT_NUMBER_BEFORE_DIAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id,TXT_PB_SELECT_NUMBER, FALSE);
        }
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:  
        GUILIST_GetItemData(ctrl_id,GUILIST_GetCurItemIndex(ctrl_id),&item_id);/*lint !e64*/
        num_index = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
        if(num_index < MMIPB_MAX_NV_PHONE_NUM) //num
        {
            add_data_ptr->number_index = num_index;        
            if(MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL== add_data_ptr->operate_type)
            {
                uint16  num_len = 0;
                uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
                          
                num_len = MMIPB_GetPhoneNumberByBcdNumber(
                                                            add_data_ptr->contact_ptr->number[num_index], 
                                                            tele_num, (MMIPB_NUMBER_MAX_STRING_LEN+2));             
                if(num_len>0)
                {
                    MMIAPICL_OpenEditBeforeDialWin(num_len,tele_num);
                }           
            }
            else if(MMIPB_OPERATE_TYPE_AUDIO_DIAL == add_data_ptr->operate_type)
            {
                MMIPB_CallBCDNumber(MMI_DUAL_SYS_MAX,
                    add_data_ptr->operate_type-MMIPB_OPERATE_TYPE_AUDIO_DIAL,/*lint !e64 !e656*/
                    add_data_ptr->contact_ptr,
				    num_index
				    );
            }
#ifdef VT_SUPPORT
            else if(MMIPB_OPERATE_TYPE_VIDEO_DIAL == add_data_ptr->operate_type)
            {
                MMIPB_CallBCDNumber(MMI_DUAL_SYS_MAX,
                    add_data_ptr->operate_type-MMIPB_OPERATE_TYPE_AUDIO_DIAL,
                    add_data_ptr->contact_ptr,
                    num_index
                    );
            }
#endif
#ifdef MMI_IP_CALL_SUPPORT
            else if(MMIPB_OPERATE_TYPE_IP_DIAL == add_data_ptr->operate_type)
            {
                MMIPB_CallBCDNumber(MMI_DUAL_SYS_MAX,
                    add_data_ptr->operate_type-MMIPB_OPERATE_TYPE_AUDIO_DIAL,/*lint !e64 !e656*/
                    add_data_ptr->contact_ptr,
                    num_index
                    );
            }
#endif
            else if(MMIPB_OPERATE_TYPE_WRITE_MESSAGE == add_data_ptr->operate_type)
            {
                MMIPB_SendSMSByNumber(add_data_ptr->contact_ptr->number[num_index]);
            }
            else if(MMIPB_OPERATE_TYPE_WRITE_MMS == add_data_ptr->operate_type)
            {
                MMIPB_SendMMSByBcdNumber(add_data_ptr->contact_ptr->number[num_index]);
            }
        }
#ifdef MMIPB_MAIL_SUPPORT
        else // mail
        {
            str_t.wstr_len = add_data_ptr->contact_ptr->mail.wstr_len;
            str_t.wstr_ptr = add_data_ptr->contact_ptr->mail.wstr;
            MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
            MMIAPIMMS_AnswerMMS(MMI_DUAL_SYS_MAX,PNULL,&str_t);    
        }
#endif
       MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);    
        break;

    case MSG_CLOSE_WINDOW:
        if(PNULL != add_data_ptr->contact_ptr)
        {
            SCI_FREE(add_data_ptr->contact_ptr);
        }
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;    
}

/*****************************************************************************/
//  Description : set pb default Photo Image
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetPbDefaultPhotoImage(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr)
{
    GUIANIM_DATA_INFO_T    data_info={0};     //in:may PNULL
    
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    if(PNULL == menu_infor_ptr||PNULL == menu_infor_ptr->contact_ptr)
    {
        return;
    }
    contact_ptr = menu_infor_ptr->contact_ptr;
    
    
#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error    
    data_info.img_id = IAMGE_PB_DEFAULT_PHOTO;
#endif
            
    SetPhotoAnimParam(
        MMK_GetWinHandle(menu_infor_ptr->applet_handle,MMIPB_ADD_IN_NV_WIN_ID),
        &data_info,PNULL);

    contact_ptr->photo_id_flag = contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_CLEAN;

    if(PNULL!=menu_infor_ptr->photo_ptr)
    {
        menu_infor_ptr->photo_ptr->is_valid = FALSE;
    }
    MMK_SendMsg(menu_infor_ptr->parent_win,MMI_PB_PHOTO_SET_DEFAULT,&contact_ptr->photo_id_flag );
    
    
}
/*****************************************************************************/
//  Description : set pb default ring
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetPbDefaultRing(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr)
{
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    if(PNULL == menu_infor_ptr||PNULL == menu_infor_ptr->contact_ptr)
    {
        return;
    }
    contact_ptr = menu_infor_ptr->contact_ptr;
    

    contact_ptr->ring_id_flag = 0;
    if(PNULL!=menu_infor_ptr->ring_ptr)
    {
        menu_infor_ptr->ring_ptr->is_valid = FALSE;
    }

    MMK_SendMsg(menu_infor_ptr->parent_win,MMI_PB_SET_DEFAULT_RING,&contact_ptr->ring_id_flag);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetPbNosoundRing(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr)
{
	MMIPB_CONTACT_T *contact_ptr = PNULL;
	
	if(PNULL == menu_infor_ptr||PNULL == menu_infor_ptr->contact_ptr)
	{
		return;
	}
	contact_ptr = menu_infor_ptr->contact_ptr;
	if(PNULL!=menu_infor_ptr->ring_ptr)
	{
		menu_infor_ptr->ring_ptr->is_valid = FALSE;
	}
	contact_ptr->ring_id_flag = 0;
	contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST;
	contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG;
	contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN) + 0;//no sound
	MMK_SendMsg(menu_infor_ptr->parent_win,MMI_PB_SET_DEFAULT_RING,&contact_ptr->ring_id_flag);
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : send vcard by sms or bt and so pn
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
LOCAL void SendVcard(MMIPB_HANDLE_T applet_handle, 
    MMIPB_SEND_TYPE_E send_type,
    MMIPB_CONTACT_T *contact_ptr
    )
{
    MMIPB_ERROR_E send_error = MMIPB_ERROR_SUCCESS;
    
    MMI_TEXT_ID_T text_id = STR_OPERATION_FAIL_EXT01;
    MMI_WIN_ID_T  alert_id = MMIPB_ALERT_INFO_WIN_ID;

    
    send_error = MMIPB_SendVcard(send_type,contact_ptr);
    SCI_TRACE_LOW("[MMIPB] SendVcard send_error %d",send_error);

    if(MMIPB_ERROR_UDISK_ISRUN == send_error)
    {
            //udis is run
        text_id = STR_SHARE_FAILED_EXT01;
    }
    else if(MMIPB_ERROR_NO_DEV == send_error)
    {
           //device selected doesn't exist!
        text_id = STR_NO_SPACE_EXT01;//TXT_NO_UDISK_OR_SD;
    }
    else if(MMIPB_ERROR_NO_SPACE == send_error)
    {
        text_id = STR_NO_SPACE_EXT01;
    }
    else if(MMIPB_ERROR_SMS_TOO_LARGE == send_error)
    {
        text_id = TXT_COMMON_BEYOND_LEN;
    }
    else if(MMIPB_ERROR_SFS_ERROR == send_error)
    {
        text_id = TXT_MMI_SAVEFAILED;
    }
    
    if(MMIPB_ERROR_SUCCESS!=send_error)
    {
        //MMIPB_OpenCommonDialogWithWinId(applet_handle, alert_id, text_id);
        MMIPB_DisplaybannerInfo(0, text_id, PNULL);
        #if 0
        MMIPUB_OpenAlertWinByTextIdEx(applet_handle, 
                                            PNULL,
                                            text_id,
                                            TXT_NULL,
                                            IMAGE_PUBWIN_WARNING,
											&alert_id,
                                            PNULL,
                                            MMIPUB_SOFTKEY_ONE,
                                            PNULL,
                                            PNULL);
        #endif
    }      
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/***************************************************************************/
PUBLIC void SendMultiVcard(MMIPB_HANDLE_T applet_handle, MMIPB_SEND_TYPE_E send_type)
{
    MMIPB_ERROR_E send_error = MMIPB_ERROR_SUCCESS;
    
    MMI_TEXT_ID_T text_id = STR_OPERATION_FAIL_EXT01;
    MMI_WIN_ID_T  alert_id = MMIPB_ALERT_INFO_WIN_ID;
    //Bug 697450 因通过MMS发送多个联系人时，会生成多个vcf文件，而生成多个vcf文件时会循环进行创建文件、写入文件、关闭文件的动作，很耗时,
    //修改为和bt操作一样，将多个联系人信息写到一个vcf文件中。
    send_error = MMIPB_SendMultiVcard(applet_handle, send_type);
    SCI_TRACE_LOW("[MMIPB] SendMultiVcard send_error %d",send_error);

    if(MMIPB_ERROR_UDISK_ISRUN == send_error)
    {
            //udis is run
        text_id = STR_SHARE_FAILED_EXT01;
    }
    else if(MMIPB_ERROR_NO_DEV == send_error)
    {
           //device selected doesn't exist!
        text_id = STR_NO_SPACE_EXT01;//TXT_NO_UDISK_OR_SD;
    }
    else if(MMIPB_ERROR_NO_SPACE == send_error)
    {
        text_id = STR_NO_SPACE_EXT01;
    }
    else if(MMIPB_ERROR_SMS_TOO_LARGE == send_error)
    {
        text_id = TXT_COMMON_BEYOND_LEN;
    }
    else if(MMIPB_ERROR_SFS_ERROR == send_error)
    {
        text_id = TXT_MMI_SAVEFAILED;
    }
    
    if(MMIPB_ERROR_SUCCESS!=send_error)
    {
        //MMIPB_OpenCommonDialogWithWinId(applet_handle, alert_id, text_id);
        MMIPB_DisplaybannerInfo(0, text_id, PNULL);
        #if 0
        MMIPUB_OpenAlertWinByTextIdEx(applet_handle, 
                                            PNULL,
                                            text_id,
                                            TXT_NULL,
                                            IMAGE_PUBWIN_WARNING,
											&alert_id,
                                            PNULL,
                                            MMIPUB_SOFTKEY_ONE,
                                            PNULL,
                                            PNULL);
        #endif
    }      
}
#endif

/*****************************************************************************/
//  Description : set photo for pb contact
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SendPhotoSetMsgForPb(wchar *full_path_ptr)
{
    MMI_HANDLE_T applet_handle = 0;
    //if(PNULL == full_path_ptr)
    //{
        //return;
    //}
    if(MMIPB_GetCurAppletHandle(&applet_handle))
    {
        MMK_SendMsg(MMK_GetWinHandle(applet_handle,MMIPB_ADD_IN_NV_WIN_ID),
            MMI_PB_SET_PHOTO_FILE,(wchar *)full_path_ptr);   
    }
}

#ifdef MMI_PIC_CROP_SUPPORT
/*****************************************************************************/
//  Description : picture crop call back
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void PicCropCallBack(MMIPICCROP_CB_T *crop_cb)
{
    if(PNULL != crop_cb)
    {
        switch(crop_cb->err_type)
        {
            case MMI_PICCROP_SUCCESS:
            case MMI_PICCROP_SUCCESS_NO_CROP: 
            case MMI_PICCROP_ERR_NOT_SUPPORT:    
                SendPhotoSetMsgForPb(crop_cb->full_path_ptr); 
            break;
            
            default:
                break;
        }
    }
}
/*****************************************************************************/
//  Description : crop picture file
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void CropPhotoFile(wchar *full_path_ptr,uint16 full_path_len)
{
    MMIPICCROP_SRC_T crop_src = {0};

    if(PNULL == full_path_ptr||0==full_path_len)
    {
        return;
    }
    
    crop_src.crop_type = MMIPIC_CROP_DEFAULT_SAVE;
    crop_src.callback = PicCropCallBack;
    crop_src.full_path_ptr = (wchar*)full_path_ptr;
    crop_src.full_path_len = full_path_len;

    crop_src.min_width = (s_pb_photo_anim_rect.right-s_pb_photo_anim_rect.left);
    crop_src.min_height = (s_pb_photo_anim_rect.bottom-s_pb_photo_anim_rect.top);
    MMIAPIPICCROP_OpenPicCrop(&crop_src);    
}
#endif

#ifdef MMIDC_F_WORK_MODE

/*****************************************************************************/
//  Description : DC shot callback
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL int32 PhotoShotCallback( MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr) 
{
    int32 ret = 0; 
    
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 

    //step 1. check return value
    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 
        //step 2. check file name len 
        if((data_info_ptr->file_name_len >0)&&(PNULL!=data_info_ptr->file_name))
        {
            ret =1;
            
    #ifdef MMI_PIC_CROP_SUPPORT
            CropPhotoFile(data_info_ptr->file_name,data_info_ptr->file_name_len);
    #else
        SendPhotoSetMsgForPb(data_info_ptr->file_name);
    #endif
        }
    }
    return ret; 
}

#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_UpdateListContentItemForContact(MMIPB_CUSTOM_PHOTO_T *photo_ptr, 
                            MMI_CTRL_ID_T ctrl_id, 
                            MMIPB_CONTACT_T * contact_ptr, 
                            void *item_ptr)
{
    GUIANIM_FILE_INFO_T anim_path = {0};

    uint32 item_index  = 0;
    uint16 item_content_index = 0;

	BOOLEAN is_photo_id_exist = FALSE;

	GUILIST_ITEM_DATA_T item_data = {0};

	if(PNULL ==photo_ptr||PNULL == contact_ptr || PNULL == item_ptr)
	{
		//SCI_TRACE_LOW:"[mmipb] MMIPB_UpdateListContentItemForContact param null"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_11365_112_2_18_2_43_59_270,(uint8*)"");
		return FALSE;
	}
	
    item_index =((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_index;    


    item_content_index = ((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_content_index;


 	if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST))
    {
        item_data.item_content[item_content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
        anim_path.full_path_wstr_ptr = photo_ptr->file_name;
        anim_path.full_path_wstr_len = photo_ptr->file_name_len;            
        item_data.item_content[item_content_index].item_data.anim_path_ptr =&anim_path;

    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_UpdateListContentItemForContact is_photo_id_exist %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_MENU_11385_112_2_18_2_43_59_271,(uint8*)"d", is_photo_id_exist);
        return FALSE; 

    }
     GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index );
	 return TRUE;

   
}
/*****************************************************************************/
//  Description : 将电话簿记录中的号码设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetContacItem(MMI_CTRL_ID_T ctrl_id,
            MMIPB_MENU_DATA_INFOR_T * add_data_ptr,               
            uint32                     item_id,
            uint32                     item_index,
            MMI_TEXT_ID_T       left_softkey_id,
            MMI_TEXT_ID_T       middle_softkey_id,
            MMI_TEXT_ID_T       right_softkey_id,
            BOOLEAN is_update)
{
    SetContactAddOrEditItem(ctrl_id, 
                            add_data_ptr,
                            item_id, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            is_update); 
}
#endif

/*****************************************************************************/
//  Description : 指定的记录类型是否支持菜单项
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsContactHasMenu(uint16 storage_id)

{
    BOOLEAN result = TRUE;
#if defined (MMIPB_SIMDN_SUPPORT)    
    if(MMIPB_IsSDNContact(storage_id)       
        ||MMIPB_IsMsisdnContact(storage_id)
      )
    {
        result = FALSE;
    }
    //else
#endif
#ifdef SNS_SUPPORT  
    if(MMIPB_IsSNSContact(storage_id))
    {
        result = FALSE;
    }
 #endif        
    return result;
}/*lint !e533*/


/*****************************************************************************/
//  Description : 指定的记录类型是否可编辑
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsFixedContact(uint16 storage_id)

{
    BOOLEAN result = FALSE;
    if(MMIPB_IsSDNContact(storage_id)
#ifdef SNS_SUPPORT        
        ||MMIPB_IsSNSContact(storage_id)
#endif        
      )
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : 获取铃声的名字
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetRingName(uint16 idx, wchar *ring_name, uint32 len)
{
    uint16 id = idx;
    MMI_STRING_T        string = {0};
    wchar wbuff[30 + 4]={0};
    char buff[5 + 1]={0};   

    if(PNULL == ring_name)
        return 0;
    
    if(len <= 5)
        return 0;
    
    if(id > /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max || 0 == id )
    {
        id = 0;
    }
    if(0 == id)
    {
        MMIRES_GetText(TXT_PB_RING_DEFAULT,0,&string);
        MMIAPICOM_Wstrncpy(ring_name, string.wstr_ptr, string.wstr_len);
        return string.wstr_len;
    }
    MMIRES_GetText( TXT_RING, 0, &string );

    if(string.wstr_len > len - 5)
    {
        string.wstr_len = len - 5;
    }
    MMIAPICOM_Wstrncpy(wbuff, string.wstr_ptr, string.wstr_len);
    
    sprintf(buff, "%d", id);
    MMIAPICOM_StrToWstr((uint8 *)buff, wbuff + string.wstr_len);
    MMIAPICOM_Wstrncpy(ring_name, wbuff, len);

    return string.wstr_len + strlen(buff);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : hide adrress,company,birth,url and so on item
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SwitchMoreItemState(MMI_HANDLE_T ctrl_handle,
    MMI_IMAGE_ID_T image_id,
    uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //uint16               content_index = 0;

    GUILIST_RemoveAllItems(ctrl_handle);
    
    item_t.item_style = GUIITEM_STYLE_HALF_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
  
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_COMMON_MORE;

    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = image_id;

    item_t.user_data = user_data;
   
    GUILIST_AppendItem(ctrl_handle,&item_t);
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : hide adrress,company,birth,url and so on item
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void HideAddressBirthUrlItem(MMI_HANDLE_T win_id)
{
    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID), 
        GUIFORM_CHILD_DISP_HIDE);

    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID), 
        GUIFORM_CHILD_DISP_HIDE);   
    
    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID), 
        GUIFORM_CHILD_DISP_HIDE);
}
/*****************************************************************************/
//  Description : set adrress,company,birth,url and so on item visible
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAddressBirthUrlItem(MMI_HANDLE_T win_id,MMIPB_CONTACT_T* contact_ptr)
{   
    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_URL_FORM_ID), 
        GUIFORM_CHILD_DISP_NORMAL);

    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID), 
        GUIFORM_CHILD_DISP_NORMAL);
    
#ifndef PB_SUPPORT_LOW_MEMORY
    if(PNULL!=contact_ptr&&0==contact_ptr->birth.wstr_len)
    {
        GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_BIRTHDAY_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_BUTTON_BIRTHDAY_DELETE_ID), 
        GUIFORM_CHILD_DISP_HIDE);
    }
   
    GUIFORM_SetChildDisplay(
        MMK_GetCtrlHandleByWin(win_id,MMIPB_EDITBOX_FORM_ID), 
        MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMO_FORM_ID), 
        GUIFORM_CHILD_DISP_NORMAL);
#endif
}
#endif
/*****************************************************************************/
//  Description : set multi group name
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMultiGrouName(uint32 group,wchar* name_ptr)
{
    uint8 i=0;
    uint8 count = 0;
    uint16 len = 0;
    uint8          group_id_arr[MMIPB_MAX_GROUP_NUM] = {0};
   // wchar item_text_ptr[GUIRICHTEXT_TEXT_MAX_LEN] ={0};

    MMIPB_NAME_T name_t = {0};
    MMI_STRING_T str_t = {0};

    if(PNULL == name_ptr)
    {
        return 0;
    }
    
    count = MMIPB_GetAllGroupIDs(group, group_id_arr, MMIPB_MAX_GROUP_NUM);
    for(i = 0;i<count;i++)
    {
        MMIAPIPB_ReadGroupName(&name_t, group_id_arr[i]);
        //显示分组项时，超过最大长度后，后面的分组项不予显示，否则会出现内存越界现象 for Bug 205651
		if((len + name_t.wstr_len + 2) > GUIRICHTEXT_TEXT_MAX_LEN)
		{
			break;
		}

        if(name_t.wstr_len>0)
        {
           /* MMIAPICOM_Wstrcat(name_ptr,name_t.wstr); 
            MMIAPICOM_Wstrcat(name_ptr,L" ");
            len+=(name_t.wstr_len+1);*/
           if(MMIAPICOM_Wstrlen(name_ptr) >0)
           {
                MMIAPICOM_Wstrcat(name_ptr,L",");
                MMIAPICOM_Wstrcat(name_ptr,L" ");
                len+=2;
           }
           MMIAPICOM_Wstrcat(name_ptr,name_t.wstr); 
           len+=(name_t.wstr_len+1);
                    
        }
    }
        
    if(0==len)
	{
        int min_len = 0;
        MMI_GetLabelTextByLang(STR_GROUP_LIST_NONE_EXT01, &str_t); 
        min_len = MIN(str_t.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        // added by Tong Yongli. According to Coverity CID 10094
        if(PNULL == str_t.wstr_ptr)
        {
            return 0;
        }
        SCI_MEMCPY(name_ptr, str_t.wstr_ptr, sizeof(wchar)*min_len)/*lint !e661 !e737*/
       // MMIAPICOM_Wstrcat(name_ptr,);
        len = min_len;
    }
    return len;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePbSelectionMenuWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
	MMI_RESULT_E        result        = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T       ctrl_id       = MMK_GetCtrlHandleByWin(win_id,MMIPB_SELECTION_MENU_CTRL_ID);
	PB_OPTMENU_NODE_E              node_id       = 0;
	GUIMENU_STYLE_E menu_style = GUIMENU_STYLE_POPUP_AUTO;
	uint16 menu_num = 0;
	MMI_STRING_T  kstring = {0};
	
	MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
	MMIPB_CONTACT_T *contact_ptr = PNULL;
	MMIPB_APPLET_T*          applet_ptr = PNULL; 
	menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
	if(PNULL == menu_infor_ptr )
	{
		result = MMI_RESULT_FALSE;
		return result;
	} 
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);
	contact_ptr = menu_infor_ptr->contact_ptr;
	
	if(PNULL == applet_ptr)
	{
		result = MMI_RESULT_FALSE;
		return result;
	}
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		GUIMENU_CreatDynamic(PNULL,win_id, MMIPB_SELECTION_MENU_CTRL_ID,GUIMENU_STYLE_POPUP);
		ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_SELECTION_MENU_CTRL_ID);
		GUIMENU_SetPopMenuCentered(ctrl_id, TRUE);
		GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
		if( MMIPB_COPY_FROM_PHONE_MENU <= menu_infor_ptr->menu_type
			&& MMIPB_COPY_FROM_SIM2_MENU >= menu_infor_ptr->menu_type)
		{
			MMI_GetLabelTextByLang(STR_PRMPT_COPY_TO,&kstring);
		}
		else if(MMIPB_ADDEDIT_PREFERRED_SIM_MENU == menu_infor_ptr->menu_type)
		{
			MMI_GetLabelTextByLang(STR_SIM_LIST_PRE_SIM_EXT02,&kstring);
		}
		else if(MMIPB_RING_OPT_MENU == menu_infor_ptr->menu_type)
		{
			MMI_GetLabelTextByLang(STXT_RINGING_TYPE,&kstring);
		}
		GUIMENU_SetMenuTitle(&kstring,ctrl_id);
		CreatePbOptMenu(win_id,
			ctrl_id,
			menu_infor_ptr,
			&menu_num);
		if(0==menu_num)
		{
			MMK_CloseWin(win_id);
		}
		else
		{
			MMK_SetAtvCtrl(win_id, ctrl_id);
			if(MMIPB_ADDEDIT_PREFERRED_SIM_MENU == menu_infor_ptr->menu_type)
			{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
				GUIMENU_SetSelectedItem(ctrl_id, menu_infor_ptr->contact_ptr->preferred_sim);
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
			}
			else if(MMIPB_RING_OPT_MENU == menu_infor_ptr->menu_type && PNULL != contact_ptr)
			{
				uint8 ring_id_flag = 0;
				if(MMIPB_OPERATE_TYPE_GROUP_RINGTONE == menu_infor_ptr->operate_type)
				{
					MMIPB_GROUP_T      group_info = {0};
					MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
					ring_id_flag = group_info.ring_id_flag;
				}
				else
				{
					ring_id_flag = contact_ptr->ring_id_flag;
				}
				if(ring_id_flag&MMIPB_FIXED_RING_FLAG)
				{
					if((ring_id_flag & MMIPB_FIXED_RING_ID_MASK) == 0 )
					{
						GUIMENU_SetSelectedItem(ctrl_id, 1);
					}
					else if((ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
					{
						GUIMENU_SetSelectedItem(ctrl_id, 2);
					}
				}
				else
				{
					GUIMENU_SetSelectedItem(ctrl_id, 0);
				}
			}
		}
		break;
		
		
	//case MSG_CTL_OK:
	//case MSG_APP_OK:
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		node_id = (PB_OPTMENU_NODE_E)GUIMENU_GetCurNodeId(ctrl_id);
		HandleOptMenuAction(win_id,node_id,menu_infor_ptr);
		
		MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_LOSE_FOCUS:
        break;


   case MSG_CLOSE_WINDOW:
        if(PNULL != menu_infor_ptr->contact_ptr)
        {
		    SCI_FREE(menu_infor_ptr->contact_ptr);
		}
		MMK_FreeWinAddData(win_id);
		MMK_DestroyDynaCtrl(ctrl_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	
	return (result);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbSelectionMenu(MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
    MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    if(PNULL == data_ptr)
    {
        return;
    }
    menu_infor_ptr = SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_infor_ptr)
    {
        return;
    }
    SCI_MEMSET(menu_infor_ptr, 0x00, sizeof(MMIPB_MENU_DATA_INFOR_T));
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        SCI_FREE(menu_infor_ptr);
        return;
    }
    SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
    SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL != data_ptr->contact_ptr)
    {
        SCI_MEMCPY(contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
    }
    menu_infor_ptr->contact_ptr = contact_ptr;
    MMK_CreateWinByApplet(data_ptr->applet_handle,
            (uint32 *)MMIPB_OPT_SELECTION_MENU_WIN_TAB,
            (MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);

}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendRingListItem
(
 MMI_WIN_ID_T    win_id,
 MMI_CTRL_ID_T       ctrl_id,
 uint32              item_id,
 MMI_TEXT_ID_T text_id
 )
{
	GUILIST_ITEM_T      item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	uint16               content_index = 0;
	MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
	if(PNULL == applet_ptr)
	{
		return;
	}
	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
	item_t.item_data_ptr = &item_data;
	item_t.user_data     = item_id;
	item_data.softkey_id[LEFT_BUTTON] = TXT_NULL;  
	item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_SELECT_MK;
	item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
	
	item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[content_index].item_data.text_id = text_id;
	GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectRingWinMsg
(
 MMI_WIN_ID_T        win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
	MMI_RESULT_E        result = MMI_RESULT_TRUE;
	uint16 list_index = 0;
	MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_SELECT_RING_LIST_CTRL_ID); 
	MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
	MMIPB_CONTACT_T *contact_ptr = PNULL;
	MMIPB_APPLET_T*          applet_ptr = PNULL; 
	menu_infor_ptr = (MMIPB_MENU_DATA_INFOR_T *)MMK_GetWinAddDataPtr(win_id);
	if(PNULL == menu_infor_ptr )
	{
		result = MMI_RESULT_FALSE;
		return result;
	} 
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(menu_infor_ptr->applet_handle);
	contact_ptr = menu_infor_ptr->contact_ptr;
	if(PNULL == applet_ptr)
	{
		result = MMI_RESULT_FALSE;
		return result;
	}
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		GUILIST_SetMaxItem(ctrl_id, MMIPB_MAX_GROUP_NUM, FALSE);
		GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
		AppendRingListItem(win_id, ctrl_id, 0, STR_COMMON_LIST_DEFAULT);
		AppendRingListItem(win_id, ctrl_id, 1, STR_NO_SOUND_EXT01);
		AppendRingListItem(win_id, ctrl_id, 2, STR_CHOOSE_FROM_RINGTONES_EXT01);
		MMK_SetAtvCtrl(win_id, ctrl_id);
		if(PNULL != contact_ptr)
		{
			uint8 ring_id_flag = 0;
			if(MMIPB_OPERATE_TYPE_GROUP_RINGTONE == menu_infor_ptr->operate_type)
			{
				MMIPB_GROUP_T      group_info = {0};
				MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
				ring_id_flag = group_info.ring_id_flag;
			}
			else
			{
				ring_id_flag = contact_ptr->ring_id_flag;
			}
			if(ring_id_flag&MMIPB_FIXED_RING_FLAG)
			{
				if((ring_id_flag & MMIPB_FIXED_RING_ID_MASK) == 0 )
				{
					GUILIST_SetCurItemIndex(ctrl_id, 1);
				}
				else if((ring_id_flag & MMIPB_FIXED_RING_ID_MASK) <= /*MMISET_RING_FIXED_DEFAUT_NUM*/g_ringtone_max )
				{
					GUILIST_SetCurItemIndex(ctrl_id, 2);
				}
			}
			else
			{
				MMIPB_CUSTOM_RING_T custom_ring = {0};
				if(MMIPB_OPERATE_TYPE_GROUP_RINGTONE == menu_infor_ptr->operate_type)
				{
					//MMIPB_ReadGroupRingFile(applet_ptr->instance.cur_group_id, &custom_ring);
					if(ring_id_flag&MMIPB_RING_ID_EXIST)
					{
						GUILIST_SetCurItemIndex(ctrl_id, 2);
					}
					else
					{
						GUILIST_SetCurItemIndex(ctrl_id, 0);
					}
				}
				else
				{
					if(PNULL != menu_infor_ptr->ring_ptr && menu_infor_ptr->ring_ptr->is_valid)
					{
						GUILIST_SetCurItemIndex(ctrl_id, 2);
					}
					else
					{
						GUILIST_SetCurItemIndex(ctrl_id, 0);
					}
				}
			}
		}
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_RINGING_TYPE))
		{
			GUIWIN_UpdateStb();
		}
		break;
	case MSG_GET_FOCUS:
		if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_RINGING_TYPE))
		{
			GUIWIN_UpdateStb();
		}
		break;
		
	case MSG_APP_OK:    
	case MSG_CTL_OK: 
		
		break;
		
	case MSG_CTL_MIDSK:
	case MSG_APP_WEB:
		
		list_index = GUILIST_GetCurItemIndex(ctrl_id);
		if(0 == list_index)
		{
			SetPbDefaultRing(menu_infor_ptr);
		}
		else if(1 == list_index)
		{
			SetPbNosoundRing(menu_infor_ptr);
		}
		else if(2 == list_index)
		{
			if(MMIPB_OPERATE_TYPE_GROUP_RINGTONE == menu_infor_ptr->operate_type)
			{
				MMIPB_GROUP_T   group_info = {0};
				MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
				MMIAPISET_OpenFixCallRingWin(MMI_DUAL_SYS_MAX,menu_infor_ptr->parent_win,
					group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK);
			}
			else
			{
				MMIAPISET_OpenFixCallRingWin(MMI_DUAL_SYS_MAX,menu_infor_ptr->parent_win,
					menu_infor_ptr->contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK);
			}
		}
		MMK_CloseWin( win_id );
		break;
		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin( win_id );
		break;
	case MSG_CLOSE_WINDOW:
		if(PNULL != menu_infor_ptr->contact_ptr)
		{
			SCI_FREE(menu_infor_ptr->contact_ptr);
		}
		MMK_FreeWinAddData(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	
	return (result);    
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbSelectionRingWin(MMIPB_MENU_DATA_INFOR_T *data_ptr)
{
	MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr = PNULL;
	MMIPB_CONTACT_T *contact_ptr = PNULL;
	
	if(PNULL == data_ptr)
	{
		return;
	}
	menu_infor_ptr = SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
	if(PNULL == menu_infor_ptr)
	{
		return;
	}
	SCI_MEMSET(menu_infor_ptr, 0x00, sizeof(MMIPB_MENU_DATA_INFOR_T));
	contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
	if(PNULL == contact_ptr)
	{
		SCI_FREE(menu_infor_ptr);
		return;
	}
	SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
	SCI_MEMCPY(menu_infor_ptr,data_ptr,sizeof(MMIPB_MENU_DATA_INFOR_T));
	if(PNULL != data_ptr->contact_ptr)
	{
		SCI_MEMCPY(contact_ptr,data_ptr->contact_ptr,sizeof(MMIPB_CONTACT_T));
	}
	menu_infor_ptr->contact_ptr = contact_ptr;
	MMK_CreateWinByApplet(data_ptr->applet_handle,
		(uint32 *)MMIPB_SELECT_RING_WIN_TAB,
		(MMIPB_MENU_DATA_INFOR_T *)menu_infor_ptr);
	
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_WriteDisplayDebugMenu(BOOLEAN value)
{
	BOOLEAN ret = FALSE;
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;   
	nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_DISPLAY_DEBUG_MENU), sizeof(BOOLEAN), (void *)(&value));  
	ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
	return ret;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_IsDisplayDebugMenu()
{
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	BOOLEAN value = FALSE;
	
	nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_DISPLAY_DEBUG_MENU), sizeof(BOOLEAN), (void *)(&value));  
	
	if(ERR_MNDATAMAG_NO_ERR != nv_result)
	{
		value = FALSE;
		MMIAPIPB_WriteDisplayDebugMenu(value);
	}
	return value;
}
#ifdef HERO_ENGINE_TTS_SUPPORT
PUBLIC unsigned int hero_PB_GetAddContactWinId(void)
{
	return s_hero_pb_addcontact_win_id;
}
#endif

