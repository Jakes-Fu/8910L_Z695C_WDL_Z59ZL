/*****************************************************************************
** File Name:      mmipb_menu.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file defines all option menus, management and operation of these menus *
**                 定义了电话本的所有的选项菜单，这些菜单的管理和操作
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_MENU_H_
#define _MMIPB_MENU_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

#include "mmipb_common.h"
#include "mmipb_adapt.h"
#include "mmipb_app.h"
//#include "mmipb_mnapi.h"
//#include "mmipb_export.h"
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

//pb option menu id
typedef enum
{
    PB_PB_OPTMENU_NODE_ROOT,

    //dial
    PB_OPTMENU_NODE_DIAL,            
    PB_OPTMENU_NODE_DIAL_AUDIO,        //audio       
   #ifdef VT_SUPPORT
   PB_OPTMENU_NODE_DIAL_VIDEO,         //video call 
   #endif
   PB_OPTMENU_NODE_DIAL_IP,         //IP
    PB_OPTMENU_NODE_DIAL_AUDIO_DETAIL,  
   #ifdef VT_SUPPORT
   PB_OPTMENU_NODE_DIAL_VIDEO_DETAIL,       
   #endif 
   PB_OPTMENU_NODE_DIAL_IP_DETAIL,      
   
    PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL,   
    PB_OPTMENU_EDIT_NUMBER_BEFORE_DIAL_DETAIL,   

    //send message
    PB_OPTMENU_NODE_WRITE_MESSAGE,  
    PB_OPTMENU_NODE_WRITE_MESSAGE_DETAIL,  
    PB_OPTMENU_NODE_SEND_SMS,       //sms
    PB_OPTMENU_NODE_SEND_SMS_DETAIL, 
    PB_OPTMENU_NODE_SEND_MMS,       //mms  
    PB_OPTMENU_NODE_SEND_MMS_DETAIL, 
    PB_OPTMENU_NODE_SEND_EMAIL,     //email      

    PB_OPTMENU_NODE_GROUPS,
    PB_OPTMENU_NODE_ADDNEW,         //new
    PB_OPTMENU_NODE_MODIFY,         //modify
    PB_OPTMENU_NODE_DELETE_ONE,   //delete
    PB_OPTMENU_NODE_DELETE_MULTI,  
    PB_OPTMENU_NODE_SHARE,  
#ifdef MMIPB_MOST_USED_SUPPORT     
    PB_OPTMENU_NODE_CLEAR_MOSTUSED, //clear mostused
#endif
#ifdef MMI_BT_PBAP_SUPPORT
    PB_OPTMENU_NODE_ANDROIDSYNC,
#endif
   //copy
    PB_OPTMENU_NODE_COPY,   
    PB_OPTMENU_NODE_SETTINGS,
    //copy one to phone & sim
    PB_OPTMENU_NODE_ONE_COPYTOPHONE,    //复制到手机组   
    PB_OPTMENU_NODE_ONE_COPYTOSIM1,
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    PB_OPTMENU_NODE_ONE_COPYTOSIM2,
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    PB_OPTMENU_NODE_ONE_COPYTOSIM3,
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    PB_OPTMENU_NODE_ONE_COPYTOSIM4,
    #endif  
    
    //import & export
    PB_OPTMENU_NODE_IMPORT_EXPORT,

    //copy form sim
    PB_OPTMENU_NODE_COPY_FROM_SIM1,
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    PB_OPTMENU_NODE_COPY_FROM_SIM2,
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    PB_OPTMENU_NODE_COPY_FROM_SIM3,
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    PB_OPTMENU_NODE_COPY_FROM_SIM4,
    #endif

    //copy to phone & sim
    PB_OPTMENU_NODE_COPYTOPHONE,    //复制到手机组
    PB_OPTMENU_NODE_COPYTOSIM1,
     #ifndef MMI_MULTI_SIM_SYS_SINGLE
    PB_OPTMENU_NODE_COPYTOSIM2,
    #endif
    #if (defined MMI_MULTI_SIM_SYS_QUAD||defined MMI_MULTI_SIM_SYS_TRI)
    PB_OPTMENU_NODE_COPYTOSIM3,
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    PB_OPTMENU_NODE_COPYTOSIM4,
    #endif
    PB_OPTMENU_NODE_CONTACTS, 
    PB_OPTMENU_NODE_GROUP_ADD_MEMBERS,  
    PB_OPTMENU_NODE_GROUP_SEND_MESSAGE,  
    PB_OPTMENU_NODE_GROUP_SEND_EMAIL,  
    PB_OPTMENU_NODE_GROUP_DEL_MEMBERS,  
#ifdef MMIPB_GROUP_MANAGE_SUPPORT     
    PB_OPTMENU_NODE_GROUP_NEW,  
    PB_OPTMENU_NODE_GROUP_EDIT,   //编辑分组
    PB_OPTMENU_NODE_GROUP_NAME_EDIT,//更改组名
    PB_OPTMENU_NODE_GROUP_MEMBER_MANAGE,
    PB_OPTMENU_NODE_GROUP_DELETE,   //解散分组
    PB_OPTMENU_NODE_GROUP_RING,//分组铃声    
    PB_OPTMENU_NODE_REMOVE_FROM_GROUP,    
    PB_OPTMENU_NODE_SEND_GROUP_SMS, //
#endif    
    PB_OPTMENU_NODE_OTHERS,         //其他
    PB_OPTMENU_NODE_USED_SPACE,     //已用空间
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    PB_OPTMENU_NODE_DISPLAY_LOCATION,   //显示选项
#endif
    #if defined(MMI_SPEEDDIAL_SUPPORT) 
    PB_OPTMENU_NODE_SPEED_DIAL,     //快捷拨号
    #endif
    
#ifdef MMI_VCARD_SUPPORT
    PB_OPTMENU_NODE_VCARD_IMPORT_CONTACT,
    PB_OPTMENU_NODE_VCARD_EXPORT_CONTACT,
    PB_OPTMENU_NODE_VCARD_BT_EXPORT_CONTACT,
    MMIPB_OPERATE_TYPE_BT_EXPORT_CONTACT,
    PB_OPTMENU_NODE_SEND_VCARD_MENU,     //发送
    PB_OPTMENU_NODE_SEND_BY_SMS_MENU,  //发送经短信
    PB_OPTMENU_NODE_SEND_BY_MMS_MENU,  //发送经彩信
//#ifdef MMIEMAIL_SUPPORT
    PB_OPTMENU_NODE_SEND_BY_EMAIL_MENU,
//#endif
    PB_OPTMENU_NODE_SEND_BY_BT_MENU,   //发送经蓝牙
#endif

#ifdef MMIPB_SIMDN_SUPPORT
    PB_OPTMENU_NODE_MSISDN,//本机号码，
    PB_OPTMENU_NODE_SDN,
#endif
    PB_OPTMENU_NODE_CONTACT_VIEW,
    PB_OPTMENU_NODE_ADD_TO_CONTACT,
    PB_OPTMENU_NODE_ADD_TO_NEW_CONTACT,
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    PB_OPTMENU_NODE_ADD_TO_EXIST_CONTACT,
#endif    
    #ifdef PIM_SUPPORT
    PB_OPTMENU_UPATE_PB,
    PB_OPTMENU_NET_REQ,
    #endif

    #ifdef MMIWIDGET_SUPPORT
    PB_OPTMENU_QUICK_CONTACT,
    #endif
    
    #ifdef MMI_PDA_SUPPORT
    PB_OPTMENU_NODE_GROUP,
    PB_OPTMENU_NODE_ACCOUNT,
    #endif

    PB_OPTMENU_ADD_TO_GROUP,
    PB_OPTMENU_ADD_TO_FAVORITE,
    PB_OPTMENU_REMOVE_FAVORITE,
    PB_OPTMENU_SETTING,
    PB_OPTMENU_PHOTO_SELECT,
    PB_OPTMENU_PHOTO_SELECT_FROM_GALLERY,

#ifdef CAMERA_SUPPORT
    PB_OPTMENU_PHOTO_SHOT,
#endif

    PB_OPTMENU_PHOTO_DEFAULT,
    PB_OPTMENU_RING_SELECT,
    PB_OPTMENU_RING_PHONE,
    PB_OPTMENU_RING_DEFAULT,
    PB_OPTMENU_RING_NO_SOUND,
    PB_OPTMENU_GROUP_VCARD,
    PB_OPTMENU_GROUP_PHONE,
    PB_OPTMENU_GROUP_SIM1,
    PB_OPTMENU_GROUP_SIM2,
    PB_OPTMENU_GROUP_SIM3,
    PB_OPTMENU_GROUP_SIM4,   

    PB_OPTMENU_OPERRATE_SEL_CANCEL,
    PB_OPTMENU_OPERRATE_SEL_OK,
    PB_OPTMENU_OPERRATE_SELECT_ALL,
    PB_OPTMENU_OPERRATE_SELECT_ALL_CANCEL,
    
#ifdef CMCC_UI_STYLE
    PB_OPTMENU_OPERRATE_SELECT_SIM,
    PB_OPTMENU_OPERRATE_SELECT_PHONE,
#endif

    #ifdef SNS_SUPPORT
    PB_OPTMENU_NODE_IMPORT_FROM_SNS,
    PB_OPTMENU_NODE_IMPORT_FROM_SINA,
    PB_OPTMENU_NODE_IMPORT_FROM_TWITTER,
    PB_OPTMENU_NODE_IMPORT_FROM_FACEBOOK,
    #endif

    //debug
    PB_OPTMENU_NODE_DEBUG_MENU,
    PB_OPTMENU_NODE_DEBUG_ADD_NEW,
    PB_OPTMENU_NODE_DEBUG_SIM1_MANAGER,
    PB_OPTMENU_NODE_DEBUG_SIM2_MANAGER,
    //detail menu
    PB_OPTMENU_DETAIL_NODE_COPY,
    PB_OPTMENU_DETAIL_NODE_SHARE,
    PB_OPTMENU_DETAIL_SAVE_VCARD,
    //addedit menu
    PB_OPTMENU_ADDEDIT_SAVE_MENU,
    PB_OPTMENU_ADDEDIT_CLEAR_DATE,
    PB_OPTMENU_ADDEDIT_PREFERRED_NONE,
    PB_OPTMENU_ADDEDIT_PREFERRED_SIM1,
    PB_OPTMENU_ADDEDIT_PREFERRED_SIM2,
    //share
    PB_OPTMENU_MULTI_SHARE_BY_MMS_MENU,
    PB_OPTMENU_MULTI_SHARE_BY_EMAIL_MENU,
    PB_OPTMENU_MULTI_SHARE_BY_BT_MENU,

    //copy
    PB_OPTMENU_COPY_FROM_PHONE,
    PB_OPTMENU_COPY_FROM_SIM1,
    PB_OPTMENU_COPY_FROM_SIM2,
    PB_OPTMENU_COPY_FROM_ANOTHER_DEVICE,
    PB_OPTMENU_COPY_SIM1_TO_PHONE,
    PB_OPTMENU_COPY_SIM1_TO_SIM2,
    PB_OPTMENU_COPY_SIM2_TO_PHONE,
    PB_OPTMENU_COPY_SIM2_TO_SIM1,
    PB_OPTMENU_COPY_ANOTHER_DEVICE_TO_PHONE,
    PB_OPTMENU_COPY_PHONE_TO_SIM1,
    PB_OPTMENU_COPY_PHONE_TO_SIM2,
    PB_OPTMENU_COPY_PHONE_TO_ANOTHER_DEVICE,
    
    PB_OPTMENU_NODE_MAX
}PB_OPTMENU_NODE_E;


// 操作电话簿的类型
typedef enum _MMIPB_OPERATE_TYPE_E
{
#ifdef MMI_VCARD_SUPPORT
    MMIPB_OPERATE_TYPE_SEND_VCARD,     // 打开一条记录
#endif
	MMIPB_OPERATE_TYPE_ADD_ONE,     // 只增加一条记录
   
	MMIPB_OPERATE_TYPE_EDIT_ONE,    // 只编辑一条记录
	MMIPB_OPERATE_TYPE_AUDIO_DIAL, 
	MMIPB_OPERATE_TYPE_IP_DIAL,
	#ifdef VT_SUPPORT
    MMIPB_OPERATE_TYPE_VIDEO_DIAL,
    #endif
	MMIPB_OPERATE_TYPE_ERASE_ONE,   // 删除一条记录
	MMIPB_OPERATE_TYPE_ERASE_MULTI,
	MMIPB_OPERATE_TYPE_ERASE_ALL,   // 删除全部记录
    MMIPB_OPERATE_TYPE_COPY,
    MMIPB_OPERATE_TYPE_VCARD_BT,
    MMIPB_OPERATE_TYPE_COPY_FROM_SIM1,
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIPB_OPERATE_TYPE_COPY_FROM_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIPB_OPERATE_TYPE_COPY_FROM_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIPB_OPERATE_TYPE_COPY_FROM_SIM4,
#endif
#endif
#endif
    MMIPB_OPERATE_TYPE_COPY_TO_PHONE,
    MMIPB_OPERATE_TYPE_COPY_TO_SIM1,
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIPB_OPERATE_TYPE_COPY_TO_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIPB_OPERATE_TYPE_COPY_TO_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIPB_OPERATE_TYPE_COPY_TO_SIM4,
#endif
#endif
#endif
#ifdef MMI_VCARD_SUPPORT
    MMIPB_OPERATE_TYPE_EXPORT_CONTACT,
    MMIPB_OPERATE_TYPE_IMPORT_VCARD,
#endif
    MMIPB_OPERATE_TYPE_GROUP_MANAGE,//群组管理add
    MMIPB_OPERATE_TYPE_GROUP_DEL_MEMBER,//群组管理del
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIPB_OPERATE_TYPE_FAVOURITE_ADD,//FAVOURITE MANAGEMENT
    MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE,
    MMIPB_OPERATE_TYPE_CLEAR_MOSTUSED,    
#endif
    MMIPB_OPERATE_TYPE_EDIT_BEFORE_DIAL,
    MMIPB_OPERATE_TYPE_WRITE_MESSAGE,
    MMIPB_OPERATE_TYPE_WRITE_MMS,
    MMIPB_OPERATE_TYPE_WRITE_MAIL,
    MMIPB_OPERATE_TYPE_ADD_TO_GROUP,

    MMIPB_OPERATE_TYPE_GROUP_RINGTONE,
    MMIPB_OPERATE_TYPE_DELETE_GROUP,
    MMIPB_OPERATE_TYPE_DETAIL_GROUP,

    MMIPB_OPERATE_TYPE_REMOVE_FROM_GROUP,
    MMIPB_OPERATE_TYPE_SEND_GROUP_MSG,
    MMIPB_OPERATE_TYPE_MULTI_SHARE,
    MMIPB_OPERATE_TYPE_ADD_GROUP_RECIPIENTS,
    MMIPB_OPERATE_TYPE_SEND_GROUP_EMAIL,
    MMIPB_OPERATE_TYPE_ADD_GROUP_EMAIL,
    MMIPB_OPERATE_TYPE_COPY_PHONE_TO_SIM1,
    MMIPB_OPERATE_TYPE_COPY_PHONE_TO_SIM2,
    MMIPB_OPERATE_TYPE_COPY_PHONE_TO_ANOTHER_DEVICE,
    MMIPB_OPERATE_TYPE_COPY_SIM1_TO_PHONE,
    MMIPB_OPERATE_TYPE_COPY_SIM1_TO_SIM2,
    MMIPB_OPERATE_TYPE_COPY_SIM2_TO_PHONE,
    MMIPB_OPERATE_TYPE_COPY_SIM2_TO_SIM1,
    MMIPB_OPERATE_TYPE_COPY_ANOTHER_DEVICE_TO_PHONE,
    //debug
    MMIPB_OPERATE_TYPE_DEBUG_ADD_ONE,
    MMIPB_OPERATE_TYPE_DELETE_SIM1_CONTACTS,
    MMIPB_OPERATE_TYPE_DELETE_SIM2_CONTACTS,

    MMIPB_OPERATE_TYPE_PROMPT_IMPORT_SIM_CONTACT,

	MMIPB_OPERATE_TYPE_MAX
} MMIPB_OPERATE_TYPE_E;


typedef enum
{
    MMIPB_ITEM_NAME,
    MMIPB_ITEM_NUMBER,
    MMIPB_ITEM_EMAIL,
    MMIPB_ITEM_INSTANTMSG,
    MMIPB_ITEM_ORG,
    MMIPB_ITEM_TITLE,
    MMIPB_ITEM_URL,
    MMIPB_ITEM_BIRTHDAY,
    MMIPB_ITEM_ALIAS,
    MMIPB_ITEM_MEMO,
    MMIPB_ITEM_MAX
}MMIPB_ITEM_TYPE_E;

typedef enum
{
    MMIPB_LISTBOX_INDEX_NAME = 0,
    MMIPB_LISTBOX_INDEX_NUMBER_MOBILE,
    MMIPB_LISTBOX_INDEX_NUMBER_HOME,  
    MMIPB_LISTBOX_INDEX_NUMBER_OFFICE,
    MMIPB_LISTBOX_INDEX_NUMBER_FAX,
    MMIPB_LISTBOX_INDEX_NUMBER_OTHER,
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_LISTBOX_INDEX_MAIL,
#endif
#ifdef SNS_SUPPORT
    MMIPB_LISTBOX_INDEX_SIGN,
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    MMIPB_LISTBOX_INDEX_QQ,
#endif
#ifdef CMCC_VCARD_SUPPORT
	MMIPB_LISTBOX_INDEX_ALIAS,
#endif
#ifdef ASP_SUPPORT
    MMIPB_LISTBOX_INDEX_ASP,
#endif
    MMIPB_LISTBOX_INDEX_PHOTO,
    MMIPB_LISTBOX_INDEX_RING,
    
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_LISTBOX_INDEX_URL,
    MMIPB_LISTBOX_INDEX_BIRTH,
#endif
    
    MMIPB_LISTBOX_INDEX_GROUP,//@maggie add for 6800A PB
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_LISTBOX_INDEX_MEMO,
#endif
    MMIPB_LISTBOX_INDEX_TITLE,
    MMIPB_LISTBOX_INDEX_COMPANY,
    MMIPB_LISTBOX_INDEX_WORKADR,
    MMIPB_LISTBOX_INDEX_HOME_CALL,
    MMIPB_LISTBOX_INDEX_HOME_SMS,
    MMIPB_LISTBOX_INDEX_MOBILE_CALL,
    MMIPB_LISTBOX_INDEX_MOBILE_SMS,
    MMIPB_LISTBOX_INDEX_BUSINESS_CALL,
    MMIPB_LISTBOX_INDEX_BUSINESS_SMS,
    MMIPB_LISTBOX_INDEX_PREFERRED_SIM,
    //MMIPB_LISTBOX_INDEX_MAIL,
    //MMIPB_LISTBOX_INDEX_BIRTH,
    //MMIPB_LISTBOX_INDEX_RING,
    //MMIPB_LISTBOX_INDEX_GROUP,


    MMIPB_LISTBOX_INDEX_MAX 
}MMIPB_LISTBOX_INDEX_E;

typedef enum
{
    MMIPB_ADD_EDIT_INDEX_AVATAR = 0,
    MMIPB_ADD_EDIT_LIST1_INDEX_MAX 
}MMIPB_ADD_EDIT_LIST1_INDEX_E;

typedef enum
{
    MMIPB_ADD_EDIT_INDEX_RING = 0,
    MMIPB_ADD_EDIT_INDEX_GROUP,
    MMIPB_ADD_EDIT_INDEX_PREFERRED_SIM,
    MMIPB_ADD_EDIT_LIST2_INDEX_MAX 
}MMIPB_ADD_EDIT_LIST2_INDEX_E;

typedef enum _MMIPB_MENU_TYPE_E
{
//all contact
    MMIPB_MAIN_EMPTY_OPT_MENU,//所有联系人为空时选项菜单
    MMIPB_MAIN_OPT_MENU,//所有联系人
    MMIPB_MAIN_LONGTP_MENU,
    MMIPB_DETAIL_BASIC_OPT_MENU,
    MMIPB_DETAIL_NUMBER_ITEM_OPT_MENU,
    MMIPB_DETAIL_MSGLOG_OPT_MENU,

#ifdef MMIPB_MOST_USED_SUPPORT
//mostused
    MMIPB_MOSTUSED_OPT_MENU,
    MMIPB_MOSTUSED_LONGTP_OPT_MENU,
//favorite    
    MMIPB_FAVORITE_OPT_MENU,
    MMIPB_FAVORITE_LONGTP_OPT_MENU,
#endif
//group   
    MMIPB_GROUPLIST_OPT_MENU,
    MMIPB_GROUPLIST_EMPTY_OPT_MENU,
    MMIPB_GROUPLIST_LONGTP_MENU,
    MMIPB_GROUP_ENTRY_LONGTP_MENU,
    MMIPB_GROUP_ENTRY_OPT_MENU,   
    
    MMIPB_DIAL_SUB_MENU,//常用联系人
    MMIPB_MSG_SUB_MENU,//本机号码
#ifdef MMI_VCARD_SUPPORT
    MMIPB_SENDVCARD_SUB_MENU,
#endif
  
    MMIPB_ADD_SUB_MENU,
    
    MMIPB_OPERATE_SEL_OPT_MENU,
    MMIPB_OPERATE_SEL_ALL_OPT_MENU,
    MMIPB_PHOTO_OPT_MENU,
    MMIPB_RING_OPT_MENU,
    MMIPB_SIM_DN_OPT_MENU,
    MMIPB_SIM_DN_LONGTP_MENU,
    MMIPB_ADDEDIT_OPT_MENU,
    MMIPB_ADDEDIT_PREFERRED_SIM_MENU,
    MMIPB_MULTI_SHARE_BY_MENU,
    MMIPB_COPY_FROM_MENU,
    MMIPB_COPY_FROM_PHONE_MENU,
    MMIPB_COPY_FROM_SIM1_MENU,
    MMIPB_COPY_FROM_SIM2_MENU,
    MMIPB_COPY_FROM_ANOTHER_DEVICE_MENU,
    MMIPB_DETAIL_VCARD_OPT_MENU,
    MMIPB_MENU_TYPE_MAX
}MMIPB_MENU_TYP_E;


typedef struct _MMIPB_MENU_DATA_INFOR_T_
{
    MMI_HANDLE_T applet_handle;
  
    MMI_HANDLE_T parent_win;
    MMI_CTRL_ID_T ctl_id;    //新增属性，用于不同窗口下的不同控件对相同窗口进行操作
    BOOLEAN need_reset_storage;
    int16 number_index;
    int16 number_type_edit_index;
    uint16 valid_contact_count; //有效的联系人个数
    BOOLEAN is_need_pin_verify;//是否需要PIN2码，主要用在fdn操作，其他情况都为false
    BOOLEAN not_support_single_operate; //焦点在搜索框的时候为TRUE，不可以对单个联系人操作
    MMIPB_MENU_TYP_E menu_type;
    
    MMIPB_OPERATE_TYPE_E operate_type;
    MN_DUAL_SYS_E dual_sys;
    uint32 group;
    uint32 mask_group;
    MMIPB_CUSTOM_PHOTO_T *photo_ptr;
    MMIPB_CUSTOM_RING_T *ring_ptr;
#ifdef ASP_SUPPORT
    MMIPB_CUSTOM_RING_T *asp_ptr; 
#endif

    MMIPB_CONTACT_T *contact_ptr;

}MMIPB_MENU_DATA_INFOR_T;


typedef struct _MMIPB_MENU_ITEM_INFO_T_
{
    uint32 node_id;
    MMI_TEXT_ID_T text_id;

    BOOLEAN (*enable_fun)(MMIPB_MENU_DATA_INFOR_T *judge_info_ptr, PB_OPTMENU_NODE_E menu_node);
    
}MMIPB_MENU_ITEM_INFO_T;

typedef struct _MMIPB_MENU_FAMILY_INFO_T_
{
    uint16 parent_node;
    uint16 node_id;
    
}MMIPB_MENU_FAMILY_INFO_T;

typedef  void(*MMIPB_OPERATE_SEL_CALLBACK_FUNC)(MMIPB_MENU_DATA_INFOR_T* menu_ptr, PB_OPTMENU_NODE_E menu_node);

/*****************************************************************************/
//  Description : 打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_EnterStorageSelectWin(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
);

/*****************************************************************************/
//  Description : delete one or more contact 
//  Global resource dependence : none
//  Parameter: 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactWin(MMI_HANDLE_T applet_handle,
                                           MMIPB_HANDLE_T cmd_handle 
                                           );

/*****************************************************************************/
//  Description :创建main win table 分组成员选项菜单进行准备工作
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbTabOptMenu(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbSelectionMenu(MMIPB_MENU_DATA_INFOR_T *data_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EnterPbSelectionRingWin(MMIPB_MENU_DATA_INFOR_T *data_ptr);

/*****************************************************************************/
//  Description : 导入指定文件的VCard到电话本
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
PUBLIC void MMIPB_LoadVCardFile(wchar *full_name,uint16 name_len);

/*****************************************************************************/
//  Description : 打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AddOrEditContactOfPb(
    MMIPB_MENU_DATA_INFOR_T *data_ptr
);


/*****************************************************************************/
//  Description : enter second level
//  Global resource dependence : g_mmipb_operate_type
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_EnterSecondMenu(PB_OPTMENU_NODE_E node_id,
    MMIPB_MENU_DATA_INFOR_T *data_ptr);

/*****************************************************************************/
//  Description : deal member win option menu
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_HandleOperateSelCallBack(MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr);

PUBLIC void MMIPB_HandleOneNumberAction(
    MMIPB_MENU_DATA_INFOR_T *data_ptr,
    uint16 number_index
    );
/*****************************************************************************/
//  Description : Handle tool bar action of pb view 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_HandleNumberSelect(MMIPB_MENU_DATA_INFOR_T *data_ptr);

/*****************************************************************************/
//  Description : 指定的记录类型是否支持菜单项
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsContactHasMenu(uint16 storage_id);

/*****************************************************************************/
//  Description : 指定的记录类型是否可编辑
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsFixedContact(uint16 storage_id);

/*****************************************************************************/
//  Description : 获取铃声的名字
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetRingName(uint16 idx, wchar *ring_name, uint32 len);
/*****************************************************************************/
//  Description : set multi group name
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMultiGrouName(uint32 group,wchar* name_ptr);
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CreateEditGroupWin(MMI_WIN_ID_T  win_id, MMIPB_MENU_DATA_INFOR_T* add_data_ptr);

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : handle contact with with pbap option menu action
//  Global resource dependence : none
//  Author: cheney.wu
//  Note: 
/*****************************************************************************/
PUBLIC void HandleContactSyncPBAPAction(MMI_WIN_ID_T win_id);
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
                            void *item_ptr);
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
            BOOLEAN is_update);
#endif

/*****************************************************************************/
//  Description : 隐私检查验证后，打开新增电话本记录的群组选择窗口
//  Global resource dependence : none
//  Author: yanyan.an
//  Note: 只用于隐私检查后的回调处理函数
/*****************************************************************************/
PUBLIC void MMIPB_AddOrEditContactOfPbExt(BOOLEAN is_permitted, DPARAM param_ptr);

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


#ifdef   __cplusplus
    }
#endif

#endif
